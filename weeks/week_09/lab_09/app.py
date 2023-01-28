import os

from flask import Flask, flash, jsonify, make_response, redirect, render_template, request, session
from cs50 import SQL

app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///birthdays.db")


@app.route("/", methods=["GET"])
def index():
    people = db.execute("SELECT id, name, month, day from birthdays")
    return render_template("index.html", people=people)


@app.route("/new", methods=["POST"])
def new():
    name = request.form.get("name")
    month = int(request.form.get("month"))
    day = int(request.form.get("day"))
    if name and 1 <= month <= 12 and 1 <= day <= 31:
        db.execute(
            "INSERT INTO birthdays (name, month, day) VALUES (?, ?, ?)", name, month, day)

    return redirect("/")


@app.route("/delete", methods=["POST"])
def delete():
    id = request.form.get("id")
    if id:
        db.execute("DELETE FROM birthdays WHERE id = ?", id)
    print(id)
    return redirect("/")
