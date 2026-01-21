import serial
import time
import numpy as np
import re

#opening config.txt
try:
    with open("McPherson209config.txt", 'r') as file:
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

#the current location of the spectrometer in counts
Count = initialWavelength * COUNTSPERNM

#Helper Functions
def send_command(command, wait_time=0.1):
    """Send a command and read the response."""
    ser.write((f'{command}').encode('ascii'))
    #self.ser.write(("\n\r").encode('ascii'))
    time.sleep(wait_time)
    response = ser.read_all().decode('ascii').strip()
    return response

def plus(nm:float):
    """Move a certain number of nanometers up"""
    counts = int(np.floor(nm*COUNTSPERNM))
    Count += counts
    response = send_command(f'+{counts}\r')
    return response

def minus(nm:float):
    """Move a certain number of nanometers down"""
    counts = int(np.floor(nm*COUNTSPERNM))
    Count -= counts
    response = send_command(f'-{counts}\r')
    return response

def close(location = Count/COUNTSPERNM):
    """Close the serial port and writes the new wavelength to config.txt.
       If you give it a value it'll write that instead of the calculated location.
    """
    ser.close()
    with open("config.txt", 'w') as file:
        newlines = re.sub('(?<=\= )\d+(?:\.\d+)?(?=\n)', str(location), lines, count=1)
        file.write(newlines)

def open():
    """Connects and Initiates the spectrometer by calling send_command("\ r")"""
    send_command("\ r")

def vel(velocity = 60000):
    """Change velocity, in counts/sec"""
    response = send_command(f'V{velocity}\r')

    return response
#End Helper Functions


#User Functions
def move209(wavelength:float):
    """Move to a specific wavelength (does not wait for movement to finish before closing)"""
    open()
    vel()


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
       
def stop209():
    """Stop all movement."""
    open()
    vel()

    response = send_command('STOP\r')

    close()
    return response

def overwriteLocation209(location: float):
    """Overwrites the calculated location in the file"""
    close(location)
#End User Functions



# Example usage
#if __name__ == '__main__':
#
#    print(send_command(" \r")) #space to initialize
#    time.sleep(2)
#
#    print(vel(60000)) #set velocity
#    time.sleep(5)
#    print(plus(4)) #move up 4nm
#    time.sleep(5)
#    print(minus(4)) #move down 4nm
#    time.sleep(5)
#    print(move(656)) #move to 656nm
#
#
#    time.sleep(15)
#    print(stop()) #stop motion
#
#    close() #close and write to config.txt