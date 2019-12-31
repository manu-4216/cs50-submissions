import cs50
import csv

from flask import Flask, jsonify, redirect, render_template, request

# Configure application
app = Flask(__name__)

# Reload templates when they are changed
app.config["TEMPLATES_AUTO_RELOAD"] = True


@app.after_request
def after_request(response):
    """Disable caching"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/", methods=["GET"])
def get_index():
    return redirect("/form")


@app.route("/form", methods=["GET"])
def get_form():
    return render_template("form.html")


@app.route("/form", methods=["POST"])
def post_form():
    nameField = request.form.get('name')
    genderField = request.form.get('gender')
    emailsConsentField = request.form.get('consent')

    if not nameField or not genderField:
        return render_template("error.html", message="One of the fields has not been filled")
    else:
        with open('survey.csv', 'a') as file:
            writer = csv.writer(file)
            writer.writerow((nameField, genderField, emailsConsentField))
            file.close()
        return redirect('/sheet')


@app.route("/sheet", methods=["GET"])
def get_sheet():
    with open('survey.csv', 'r') as file:
        reader = csv.reader(file)
        answers = list(reader)
    return render_template("table.html", answers=answers)
