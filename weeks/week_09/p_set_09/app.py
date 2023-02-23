import os
import mysql.connector

from mysql.connector import errorcode
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

try:
    mysql_db = mysql.connector.connect(
        host=os.getenv("DB_HOST"),
        user=os.getenv("DB_USER"),
        password=os.getenv("DB_PASSWORD"),
        port=os.getenv("DB_PORT"),
        db=os.getenv("DB_NAME"),
    )

    db = mysql_db.cursor(dictionary=True)
except mysql.connector.Error as err:
    if err.errno == errorcode.ER_ACCESS_DENIED_ERROR:
        print("Something is wrong with your user name or password")
    elif err.errno == errorcode.ER_BAD_DB_ERROR:
        print("Database does not exist")
    else:
        print(err)

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.route("/")
@login_required
def index():
    sql = "SELECT symbol, company, CAST(SUM(shares) AS SIGNED) AS shares, SUM(price * shares) AS exposure FROM Transactions WHERE user_id = %s GROUP BY company, symbol"
    db.execute(sql, [session["user_id"]])
    stocks_owned = db.fetchall()

    sql = "SELECT cash from Users WHERE id = %s"
    db.execute(sql, [session["user_id"]])
    cash = db.fetchone()["cash"]
    portfolio_value = 0

    for stock in stocks_owned:
        quote = lookup(stock["symbol"])
        price = quote["price"]
        stock["current_price"] = price
        stock["holding_value"] = stock["shares"] * price
        stock["holding_growth"] = stock["holding_value"] - stock["exposure"]
        stock["holding_growth_percent"] = (stock["holding_growth"] * 100) / stock[
            "exposure"
        ]

        portfolio_value += stock["holding_value"]

    capital = {
        "cash": cash,
        "portfolio_value": round(portfolio_value, 2),
        "grand_total": round(portfolio_value + cash, 2),
    }

    return render_template("index.html", stocks_owned=stocks_owned, capital=capital)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    if request.method == "GET":
        return render_template("buy.html")

    if request.method == "POST":
        symbol = request.form.get("symbol")
        shares = int(request.form.get("shares"))
        quoted = lookup(symbol)

        if not symbol or not quoted:
            return apology("not valid", "symbol")

        if not shares or shares <= 0:
            return apology("not valid", "share quantity")

        total_price = quoted["price"] * shares

        sql = "SELECT cash FROM Users WHERE id = %s"
        db.execute(sql, [session["user_id"]])
        user_balance = db.fetchone()["cash"]

        if total_price > user_balance:
            return apology("balance", "insufficient")

        new_balance = user_balance - total_price
        try:
            sql = "UPDATE Users SET cash = %s WHERE id = %s"
            db.execute(sql, [new_balance, session["user_id"]])

            sql = "INSERT INTO Transactions (symbol, company, shares, user_id, price) VALUES (%s, %s, %s, %s, %s)"
            val = [
                symbol,
                quoted["company"],
                shares,
                session["user_id"],
                quoted["price"],
            ]
            db.execute(sql, val)
            mysql_db.commit()

            if not quoted["is_market_open"]:
                message = f"Stock bought for previous close price: ${quoted['price']}"
                flash("Market Closed", "info")
                flash(message, "info")

        except Exception as error:
            print(error)
            return apology("error", "transaction")

        return redirect("/")


@app.route("/history")
@login_required
def history():
    sql = "SELECT symbol, shares, price, created_at AS date FROM Transactions WHERE user_id = %s GROUP BY created_at"
    db.execute(sql, [session["user_id"]])
    history = db.fetchall()

    for transaction in history:
        transaction["price"] = float(transaction["price"])
        if transaction["shares"] > 0:
            transaction["type"] = "buy"
        else:
            transaction["type"] = "sell"
            transaction["shares"] += -transaction["shares"] * 2

    return render_template("history.html", history=history)


@app.route("/login", methods=["GET", "POST"])
def login():
    session.clear()

    if request.method == "POST":

        if not request.form.get("username"):
            return apology("must provide username", 403)

        elif not request.form.get("password"):
            return apology("must provide password", 403)

        sql = "SELECT id, hash FROM Users WHERE username = %s"
        db.execute(sql, [request.form.get("username")])
        user = db.fetchone()

        if not user or not check_password_hash(
            user["hash"], request.form.get("password")
        ):
            return apology("invalid username and/or password", 403)

        session["user_id"] = user["id"]

        return redirect("/")

    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    session.clear()

    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    if request.method == "GET":
        return render_template("quote.html")

    if request.method == "POST":
        symbol = request.form.get("symbol")
        quoted = lookup(symbol)
        if not symbol or not quoted:
            return apology("not valid", "symbol")

        return render_template("quoted.html", quoted=quoted)


@app.route("/register", methods=["GET", "POST"])
def register():
    if request.method == "GET":
        return render_template("register.html")

    if request.method == "POST":
        username = request.form.get("username")
        if not username:
            return apology("not valid", "username")

        try:
            sql = "SELECT username FROM Users WHERE username = %s"
            db.execute(sql, [username])
            db_username = db.fetchone()["username"]
            if db_username == username:
                return apology("already exists", "username")
        except TypeError:
            db_username = None

        password = request.form.get("password")
        confirmation = request.form.get("confirmation")

        if not password or not confirmation:
            return apology("not valid", "password")

        if password != confirmation:
            return apology("does not match", "password")

        password_hash = generate_password_hash(password)
        sql = "INSERT INTO Users (username, hash) VALUES (%s, %s)"
        db.execute(sql, [username, password_hash])
        mysql_db.commit()

    return redirect("/login")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    if request.method == "GET":
        sql = "SELECT symbol, company, CAST(SUM(shares) AS SIGNED) AS shares FROM Transactions WHERE user_id = %s GROUP BY company HAVING shares > 0"
        db.execute(sql, [session["user_id"]])
        stocks_owned = db.fetchall()

        return render_template("sell.html", stocks_owned=stocks_owned)

    if request.method == "POST":
        symbol = request.form.get("stock")
        shares = int(request.form.get("shares"))
        quoted = lookup(symbol)

        if not symbol or not quoted:
            return apology("not valid", "symbol")

        sql = "SELECT CAST(SUM(shares) AS SIGNED) AS shares FROM Transactions WHERE symbol = %s AND user_id = %s"
        db.execute(sql, [symbol, session["user_id"]])
        shares_owned = db.fetchone()["shares"]

        if not shares or shares <= 0 or shares > shares_owned:
            return apology("not valid", "share quantity")

        sale_value = quoted["price"] * shares
        try:
            sql = "UPDATE Users SET cash = cash + %s WHERE id = %s"
            db.execute(sql, [sale_value, session["user_id"]])

            sql = "INSERT INTO Transactions (symbol, company, shares, user_id, price) VALUES (%s, %s, %s, %s, %s)"
            val = [
                symbol,
                quoted["company"],
                -shares,
                session["user_id"],
                quoted["price"],
            ]
            db.execute(sql, val)
            mysql_db.commit()

            if not quoted["is_market_open"]:
                message = f"Stock sold for previous close price: ${quoted['price']}"
                flash("Market Closed", "info")
                flash(message, "info")
        except Exception as error:
            print(error)
            return apology("error", "transaction")

        return redirect("/")


def errorhandler(e):
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
