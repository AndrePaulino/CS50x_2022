import os
import requests
import urllib.parse

from flask import redirect, render_template, request, session
from functools import wraps


def apology(message, code=400):
    """Render message as an apology to user."""

    def escape(s):
        """
        Escape special characters.

        https://github.com/jacebrowning/memegen#special-characters
        """
        for old, new in [
            ("-", "--"),
            (" ", "-"),
            ("_", "__"),
            ("?", "~q"),
            ("%", "~p"),
            ("#", "~h"),
            ("/", "~s"),
            ('"', "''"),
        ]:
            s = s.replace(old, new)
        return s

    return render_template("apology.html", top=code, bottom=escape(message)), code


def login_required(f):
    """
    Decorate routes to require login.

    https://flask.palletsprojects.com/en/1.1.x/patterns/viewdecorators/
    """

    @wraps(f)
    def decorated_function(*args, **kwargs):
        if session.get("user_id") is None:
            return redirect("/login")
        return f(*args, **kwargs)

    return decorated_function


def lookup(symbol):
    """Look up quote for symbol."""

    # Contact API
    try:
        api_key = os.getenv("API_KEY")
        url_quote = "https://api.twelvedata.com/quote"
        url_price = "https://api.twelvedata.com/price"

        query_string_price = {
            "apikey": {api_key},
            "symbol": {symbol},
            "dp": 2,
        }

        query_string_quote = {
            "apikey": {api_key},
            "symbol": {symbol},
            "interval": "1min",
            "dp": 2,
        }

        response_quote = requests.request("GET", url_quote, params=query_string_quote)
        response_price = requests.request("GET", url_price, params=query_string_price)

        response_quote.raise_for_status()
        response_price.raise_for_status()
    except requests.RequestException:
        return None

    try:
        quote = response_quote.json()
        price = response_price.json()
        final_price = (
            float(price["price"])
            if quote["is_market_open"]
            else float(quote["previous_close"])
        )
        quoted = {
            "company": quote["name"],
            "previous_close": float(quote["previous_close"]),
            "symbol": quote["symbol"],
            "currency": quote["currency"],
            "exchange": quote["exchange"],
            "price": final_price,
            "is_market_open": quote["is_market_open"],
        }
        return quoted
    except (KeyError, TypeError, ValueError):
        return None


def usd(value):
    """Format value as USD."""
    return f"${value:,.2f}"
