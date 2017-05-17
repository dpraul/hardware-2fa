# Hardware-Device 2FA

## Hardware requirements
 - Arduino Uno
 - DS3231 RTC module: http://a.co/eDsdnLE
 - MAX7219-driven 7-segment display matrix. Currently configured for this 8-digit matrix: http://a.co/1kaq6FC


## Software requirements
 - Arduino IDE: https://www.arduino.cc/en/Main/Software
 - Python 3


## Setup environment
1. Create a virtualenv: `virtualenv env`
    - if virtualenv isn't installed: `pip install virtualenv`
2. Activate the virtualenv: `env\Scripts\activate` (or, preface each command with `env\Scripts\python`)
3. Install dependencies: `python -m pip install -r requirements.txt`


## Setup the TOTP secret
1. Choose a random 10-character ASCII password, for example: `k30asvb6yd`
2. Using the OTP tool at https://www.dylanpraul.com/tools/otp/
    1. Put whatever you want into the account name, for example: `My App`
    2. Enter your secret key into the box
    3. Click go
    4. Replace `hmacKey[]` in `totp_interface\totp_interface.ino` with the contents of the Arduino HEX array box.
       For example, `k30asvb6yd` results in `{0x6b, 0x33, 0x30, 0x61, 0x73, 0x76, 0x62, 0x36, 0x79, 0x64}`
    5. Copy your password to `SECRET_PASSWORD` in `server\2fa.py`
    6. Optionally, use the Google Authenticator app to scan the generated QRCode. This can be used to verify
       functionality later.


## Setup the Arduino
1. Wire up the DS3231
    - GND to Arduino GND
    - VCC to Arduino 3.3V
    - SDA to Arduino A4
    - SCL to Arduino A5
2. Wire up the 7-segment displays (these can be changed in the top of `totp_interface.ino`)
    - GND to Arduino GND
    - VCC to Arduino 5V
    - DIN to Arduino D12
    - CLK to Arduino D11
    - CS/LOAD to Arduino D10
3. Connect the Arduino to a computer using a USB cable.
4. In the Arduino IDE, go to Tools--> Port--> Select the COM port that the Arduino UNO is listed in.
5. Replace `port` in `sync_time\sync_time.py` with the port (e.g. `COM4`) from the Arduino IDE
6. Also copy this port to `port` in `server\2fa.py`
7. Use the Arduino IDE to upload `sync_time\sync_time.ino` to the device.
8. Run `env\Scripts\python sync_time\sync_time.py` to set the RTC module to the current system time.
9. Use Arduino IDE to upload `totp_interface\totp_interface.ino` to the device.

The 7-segment display matrix should now be displaying the 6-digit TOTP.


## Running the test server
An example Flask 2FA server is provided that can either match TOTP keys with the device or from a saved secret.

If the Arduino is connected to the computer, change `USE_DEVICE_COMMUNICATION` in `server\2fa.py` to `True`.
Otherwise, leave it set to `False`. Enabling this feature allows the server to gather the 2FA token from the
hardware device instead of by using the Python implementation.

1. Run `cd server`
2. Run `python 2fa.py` to start the server at http://127.0.0.1:5000
3. The password is `password`. Enter the displayed TOTP token to access `secret.html`
