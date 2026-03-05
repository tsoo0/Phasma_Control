import serial
import time
import numpy as np
import re

#opening config.txt
try:
    with open("McPherson207config.txt", 'r') as file:
        lines = file.read()
        results = re.findall('(?<=\= )\d+(?:\.\d+)?(?=\n)', lines)
        print(results)
        initialWavelength = float(results[0])
        COUNTSPERNM = float(results[1])
        port = "COM"+str(results[2])

except:
    raise Exception("config.txt or config settings not found!")

#assigning serial port properties
baudrate = 9600
timeout = 1
bytesize = serial.EIGHTBITS
parity=serial.PARITY_NONE
stopbits=serial.STOPBITS_ONE

#creating the serial client
ser = serial.Serial(
    port=port,
    baudrate=baudrate,
    bytesize=serial.EIGHTBITS,
    parity=serial.PARITY_NONE,
    stopbits=serial.STOPBITS_ONE,
    timeout=timeout
)
time.sleep(2)

#the current location of the spectrometer in counts
Count = initialWavelength * COUNTSPERNM

#Helper Functions
def send_command(command, wait_time=1):
    """Send a command and read the response."""
    global ser
    ser.write((f'{command}').encode('ascii'))
    #self.ser.write(("\n\r").encode('ascii'))
    time.sleep(wait_time)
    response = ser.read_all().decode('ascii').strip()
    return response

def plus(nm:float):
    global ser
    """Move a certain number of nanometers up"""
    counts = int(np.floor(nm*COUNTSPERNM))
    global Count 
    Count += counts
    response = send_command(f'+{counts}\r')
    return response

def minus(nm:float):
    global ser
    """Move a certain number of nanometers down"""
    counts = int(np.floor(nm*COUNTSPERNM))
    global Count 
    Count -= counts
    response = send_command(f'-{counts}\r')
    return response

def close(location = Count/COUNTSPERNM):
    global ser
    """Close the serial port and writes the new wavelength to config.txt.
       If you give it a value it'll write that instead of the calculated location.
    """
    ser.close()
    with open("McPherson207config.txt", 'w') as file: 
        newlines = re.sub('(?<=\= )\d+(?:\.\d+)?(?=\n)', str(location), lines, count=1)
        file.write(newlines)

def openSer():
    global ser
    """Connects and Initiates the spectrometer by calling send_command(" \r")"""
    send_command(" \r")

def vel(velocity = 60000):
    global ser
    """Change velocity, in counts/sec"""
    response = send_command(f'V{velocity}\r')

    return response
#End Helper Functions


#User Functions
def move(wavelength:float):
    global ser
    """Move to a specific wavelength (does not wait for movement to finish before closing)"""
    openSer()
    time.sleep(2)
    vel()
    time.sleep(2)

    global Count 
    enteredCount = wavelength * COUNTSPERNM
    moveCount = Count-enteredCount

    if (np.floor(moveCount) == 0):
        response = "Already at that wavelength!!"
    elif (np.floor(moveCount) < 0 ):
        response = plus(np.abs(moveCount/COUNTSPERNM))
    else:
        response = minus(np.abs(moveCount/COUNTSPERNM))

    close()
    return response
       
def stop():
    global ser
    """Stop all movement."""
    

    response = send_command('STOP\r')

    close()
    return response

def overwriteLocation(location: float):
    global ser
    """Overwrites the calculated location in the file"""
    close(location)
#End User Functions


print(move(600)) #move to 656nm