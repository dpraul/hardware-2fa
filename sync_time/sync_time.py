import time
from datetime import datetime
from serial import Serial  # pySerial

port = 'COM7'
baud = 500000


def send_time(serial):
    """ Syncs current UTC time to Arduino
    
    To use, first upload send_time.ino to the Arduino.
    Afterward, make sure that a USB connection is made,
    setup the config at the top of the file (loc and baud),
    then reset the Arduino and run this script. 
    
    After a few moments, the current time (as UTC) should
    begin to print to the screen every second. The RTC module
    is now programmed and will store the time as long as the
    battery keeps a charge.
    """
    serial.read()  # read sync signal

    # attempt to set as close to on the second as possible
    now = time.time()
    time.sleep(now - int(now))

    serial.write(b'T%s\n' % bytes(str(int(time.time())), 'ascii'))


def main():
    s = Serial(port, baud)

    send_time(s)
    while True:
        got = datetime.strptime(s.readline().decode("utf-8"), "%Y-%m-%d %H:%M:%S\r\n")
        now = datetime.utcnow()
        print('Real: %s, Got: %s, Diff: %s' % (now, got, now - got))
        time.sleep(0.98)


if __name__ == '__main__':
    main()
