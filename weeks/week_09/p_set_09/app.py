import os

from cs50 import SQL
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

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    stocks_owned = db.execute(
        "SELECT symbol, company, SUM(shares) AS shares, SUM(price * shares) AS exposure FROM transactions WHERE user_id = ? AND shares > 0 GROUP BY company",
        session["user_id"],
    )

    cash = db.execute("SELECT cash from users WHERE id = ?", session["user_id"])[0][
        "cash"
    ]
    portfolio_value = 0

    for stock in stocks_owned:
        quote = lookup(stock["symbol"])
        price = quote["price"] if quote["is_market_open"] else quote["previous_close"]
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
    """Buy shares of stock"""

    if request.method == "GET":
        return render_template("buy.html")

    if request.method == "POST":
        symbol = request.form.get("symbol")
        shares = int(request.form.get("shares"))
        quoted = lookup(symbol)
        if not quoted["is_market_open"]:
            return apology("closed", "market")

        if not symbol or not quoted:
            return apology("not valid", "symbol")

        if not shares or shares <= 0:
            return apology("not valid", "share quantity")

        total_price = quoted["price"] * shares

        user_balance = db.execute(
            "SELECT cash FROM users WHERE id = ?", session["user_id"]
        )[0]["cash"]
        if total_price > user_balance:
            return apology("balance", "insufficient")

        new_balance = user_balance - total_price
        try:
            db.execute(
                "UPDATE users SET cash = ? WHERE id = ?",
                new_balance,
                session["user_id"],
            )
            db.execute(
                "INSERT INTO transactions (symbol, company, shares, user_id, price) VALUES (?, ?, ?, ?, ?)",
                symbol,
                quoted["company"],
                shares,
                session["user_id"],
                quoted["price"],
            )
        except Exception:
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
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute(
            "SELECT * FROM users WHERE username = ?", request.form.get("username")
        )

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(
            rows[0]["hash"], request.form.get("password")
        ):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
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
    """Register user"""

    if request.method == "GET":
        return render_template("register.html")

    if request.method == "POST":
        username = request.form.get("username")
        if not username:
            return apology("not valid", "username")

        db_username = db.execute("SELECT * FROM users WHERE username = ?", username)
        if db_username == username:
            return apology("already exists", "username")

        password = request.form.get("password")
        confirmation = request.form.get("confirmation")

        if not password or not confirmation:
            return apology("not valid", "password")

        if password != confirmation:
            return apology("does not match", "password")

        password_hash = generate_password_hash(password)
        db.execute(
            "INSERT INTO users (username, hash) VALUES (?, ?)", username, password_hash
        )
    return redirect("/login")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    if request.method == "GET":
        stocks_owned = db.execute(
            "SELECT symbol, company, SUM(shares) AS shares FROM transactions WHERE user_id = ? GROUP BY company",
            session["user_id"],
        )
        return render_template("sell.html", stocks_owned=stocks_owned)

    if request.method == "POST":
        symbol = request.form.get("stock")
        shares = int(request.form.get("shares"))
        quoted = lookup(symbol)

        if not quoted["is_market_open"]:
            return apology("closed", "market")

        if not symbol or not quoted:
            return apology("not valid", "symbol")

        shares_owned = db.execute(
            "SELECT SUM(shares) AS shares FROM transactions WHERE symbol = ? AND user_id = ?",
            symbol,
            session["user_id"],
        )
        if not shares or shares <= 0 or shares > shares_owned:
            return apology("not valid", "share quantity")

        sale_value = quoted["price"] * shares
        try:
            db.execute(
                "UPDATE users SET cash = cash + ? WHERE id = ?",
                sale_value,
                session["user_id"],
            )
            db.execute(
                "INSERT INTO transactions (symbol, company, shares, user_id, price) VALUES (?, ?, ?, ?, ?)",
                symbol,
                quoted["company"],
                -shares,
                session["user_id"],
                quoted["price"],
            )
        except Exception:
            return apology("error", "transaction")

        return redirect("/")


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
