import time
from serial import Serial  #pySerial

loc = 'COM4'
baud = 9600


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
    s.read()  # read sync signal
    s.write('T%s\n' % int(time.time()))


if __name__ == '__main__':
    s = Serial(loc, baud)
    send_time(s)
    while True:
        print s.readline()
        time.sleep(1)