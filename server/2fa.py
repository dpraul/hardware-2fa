import base64
from serial import Serial

from flask import Flask, render_template, request, session, redirect, url_for, flash

import onetimepass as otp

app = Flask(__name__)

app.secret_key = 'VERY_SECURE_FLASK_SECRET_KEY'
port = 'COM7'
baud = 500000

# user enters this password first
SECRET_PASSWORD = 'password'


# if True, will use Serial communication to check passcode, otherwise will use onetimepass library
USE_DEVICE_COMMUNICATION = False

if USE_DEVICE_COMMUNICATION:
    ser = Serial(port, baud)

# user enters code based on this OTP secret key second
# this code is the same code synced with the Arduino
TOTP_SECRET = b'k30asvb6yd'
TOTP_SECRET_ENCODED = base64.b32encode(TOTP_SECRET)


def validate_totp_with_arduino(token):
    ser.write(b'1')
    actual = (ser.read(6)).decode("utf-8")
    return actual == token


@app.route('/', methods=['GET', 'POST'])
def index():
    if request.method == 'GET':
        return render_template('index.html')

    if request.form.get('password', '') == SECRET_PASSWORD:
        session['pass'] = True
        return redirect(url_for('code'))

    flash('Invalid password')
    return redirect(url_for('index'))


@app.route('/code', methods=['GET', 'POST'])
def code():
    if not session.get('pass', False):
        flash('Password must be entered.')
        return redirect(url_for('index'))

    if request.method == 'GET':
        return render_template('code.html')

    token = request.form.get('code', '')
    if USE_DEVICE_COMMUNICATION:
        valid = validate_totp_with_arduino(token)
    else:
        valid = otp.valid_totp(token=token, secret=TOTP_SECRET_ENCODED)

    if valid:
        session['code'] = True
        return redirect(url_for('secret'))

    flash('Invalid code')
    return redirect(url_for('code'))


@app.route('/secret')
def secret():
    if not session.get('pass', False):
        flash('Password must be entered.')
        return redirect(url_for('index'))

    if not session.get('code', False):
        flash('Valid code must be entered.')
        return redirect(url_for('code'))

    return render_template('secret.html')


@app.route('/logout')
def logout():
    session['pass'] = False
    session['code'] = False
    return redirect(url_for('index'))


if __name__ == '__main__':
    app.run()
