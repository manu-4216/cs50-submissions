import os

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session
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

    userId = session["user_id"]

    # Query database for cash
    userRow = db.execute("SELECT cash FROM users WHERE id = :id", id=userId)
    cash = userRow[0]["cash"]

    stocks = db.execute("SELECT symbol, SUM(number) FROM buy WHERE userId = :id GROUP BY symbol", id=userId)

    stocksValue = 0
    for stock in stocks:
        price = lookup(stock["symbol"])["price"]
        stock["price"] = price
        stock["value"] = price * stock["SUM(number)"]
        stocksValue += price * stock["SUM(number)"]

    total = cash + stocksValue


    return render_template("index.html", stocks=stocks, cash=usd(cash), total=usd(total), usd=usd)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        symbol = request.form.get("symbol")
        shares = request.form.get("shares")
        userId = session["user_id"]

        # Ensure symbol is valid
        if not symbol:
            return apology("must provide symbol", 403)

        result = lookup(symbol)
        if result == None:
            return apology("symbol not valid", 403)

        # Ensure shares is positive integer
        if not shares.isdigit():
            return apology("must provide shares number, positive integer", 403)

        shares = int(shares)

        # Query database for cash
        rows = db.execute("SELECT cash FROM users WHERE id = :id", id=userId)

        price = result["price"]
        cash = rows[0]["cash"]

        if price * shares > cash:
            return apology("Not enough funds", 403)

        # Update the cash
        newCash = cash - price * shares
        # Update the new cash
        rows = db.execute("UPDATE users SET cash = :newCash WHERE id = :id", newCash=newCash, id=userId)

        # Create  an entry inside buy table
        newId = db.execute("INSERT INTO buy (userId, symbol, price, number) VALUES (:userId, :symbol, :price, :shares)"
            , userId=userId, symbol=symbol, price=price, shares=shares)

        # Create  an entry inside portfolio of the user
        # newPortfolioEntry = db.execute("INSERT INTO portfolio (userId, symbol, number) VALUES (:userId, :symbol, :shares)"
        #    , userId=userId, symbol=symbol, shares=shares)


        return render_template("bought.html", name=result["name"], price=usd(price), funds=usd(newCash))
    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("buy.html")


@app.route("/check", methods=["GET"])
def check():
    """Return true if username available, else false, in JSON format"""
    return jsonify("TODO")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""

    userId = session["user_id"]
    buyRows = db.execute("SELECT symbol, price, number, date FROM buy WHERE userId = :userId" , userId=userId)
    sellRows = db.execute("SELECT symbol, price, number, date FROM sell WHERE userId = :userId" , userId=userId)

    for buy in buyRows:
        buy["type"] = "BUY"

    for sell in sellRows:
        sell["type"] = "SELL"

    return render_template("history.html", transactions=(buyRows + sellRows))


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
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
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

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        symbol = request.form.get("symbol")
        result = lookup(symbol)

        print(result)

        if result is None:
            return apology("invalid symbol", 400)

        return render_template("quoted.html", name=result["name"], price=usd(result["price"]))
    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

     # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        username = request.form.get("username")
        password = request.form.get("password")
        confirmation = request.form.get("confirmation")

        # Ensure username was submitted
        if not username:
            return apology("must provide username", 400)

        # Ensure password was submitted
        elif not password:
            return apology("must provide password", 400)

        # Ensure passwords match
        elif password != confirmation:
            return apology("Passwords don't match", 400)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=username)

        # Ensure username doesn't exist
        if len(rows) > 0:
            return apology("Username already exists", 400)

        hash = generate_password_hash(password)
        newId = db.execute("INSERT INTO users (username, hash) VALUES (:username, :hash)",
                          username=username, hash=hash)


        # Remember which user has logged in
        session["user_id"] = newId

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        symbol = request.form.get("symbol")
        shares = request.form.get("shares")
        userId = session["user_id"]

        # Ensure symbol is valid
        if not symbol:
            return apology("must provide symbol", 403)

        result = lookup(symbol)
        if result == None:
            return apology("symbol not valid", 403)
        price = result["price"]

        # Ensure shares is positive integer
        if not shares.isdigit():
            return apology("must provide shares number, positive integer", 403)

        shares = int(shares)

        # Query database for cash
        rows = db.execute("SELECT cash FROM users WHERE id = :id", id=userId)

        price = result["price"]
        cash = rows[0]["cash"]

        # Query buy for shares
        rows = db.execute("SELECT SUM(number) FROM buy WHERE (userId = :userId AND symbol = :symbol) GROUP BY number",
            userId=userId, symbol=symbol)

        if len(rows) == 0:
            return apology("No such share in your portfolio", 403)

        currentSharesOfSymbol = rows[0]["SUM(number)"]

        if currentSharesOfSymbol < shares:
            return apology("Not enough shares in your portfolio", 403)

        # Update the cash
        newCash = cash + price * shares
        # Update the new cash
        rows = db.execute("UPDATE users SET cash = :newCash WHERE id = :id", newCash=newCash, id=userId)

        # Create  an entry inside sell table
        newId = db.execute("INSERT INTO sell (userId, symbol, price, number) VALUES (:userId, :symbol, :price, :shares)"
            , userId=userId, symbol=symbol, price=price, shares=shares)

        # Create  an entry inside portfolio of the user
        # newPortfolioEntry = db.execute("INSERT INTO portfolio (userId, symbol, number) VALUES (:userId, :symbol, :shares)"
        #    , userId=userId, symbol=symbol, shares=shares)


        return render_template("sold.html", name=result["name"], price=usd(price), funds=usd(newCash))
    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("sell.html")


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
