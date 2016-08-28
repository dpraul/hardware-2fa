# Hardware-Device 2FA

## Running the server
Run the following commands from inside the `server` directory to launch the Flask 2FA server.

1. Create a virtualenv: `virtualenv env`
2. Join the virtualenv: `env\Scripts\activate`
3. Install dependencies: `pip install -r requirements.txt`
4. If the Arduino is connected to the computer, change `USE_DEVICE_COMMUNICATION` in `2fa.py` to `True`.
   Otherwise, leave it set to `False`. **
5. Start server at [http://127.0.0.1:5000](http://127.0.0.1:5000): `python 2fa.py`  
   
   
**Enabling this feature allows the server to gather the 2FA token from the hardware device instead of
by using the Python implementation.