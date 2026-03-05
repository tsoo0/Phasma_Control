import serial
import time
import numpy as np
import re

#opening config.txt
try:
    with open("config.txt", 'r') as file:
        lines = file.read()
        results = re.findall('(?<=\= )\d+(?:\.\d+)?(?=\n)', lines)
        print(results)
        initialWavelength = float(results[0])
        COUNTSPERNM = float(results[1])
        port = "COM"+str(results[2])

except:
    raise Exception("config.txt or config settings not found!")

#controller class
class McPherson789Controller:
    def __init__(self, baudrate=9600, timeout=1):
        self.ser = serial.Serial(
            port=port,
            baudrate=baudrate,
            bytesize=serial.EIGHTBITS,
            parity=serial.PARITY_NONE,
            stopbits=serial.STOPBITS_ONE,
            timeout=timeout
        )
        self.Count = initialWavelength * COUNTSPERNM
        time.sleep(2)  # Give device time to initialize

    def send_command(self, command, wait_time=0.1):
        """Send a command and read the response."""
        self.ser.write((f'{command}').encode('ascii'))
        #self.ser.write(("\n\r").encode('ascii'))
        time.sleep(wait_time)
        response = self.ser.read_all().decode('ascii').strip()
        return response
    
    def plus(self, nm:float):
        """Move a certain number of nanometers up"""
        counts = int(np.floor(nm*COUNTSPERNM))
        self.Count += counts
        return self.send_command(f'+{counts}\r')
    
    def minus(self, nm:float):
        """Move a certain number of nanometers down"""
        counts = int(np.floor(nm*COUNTSPERNM))
        self.Count -= counts
        return self.send_command(f'-{counts}\r')
    
    def move(self, wavelength:float):
        """Move to a specific wavelength"""
        enteredCount = wavelength * COUNTSPERNM
        moveCount = self.Count-enteredCount

        if (np.floor(moveCount) == 0):
            return "Already at that wavelength!!"
        elif (np.floor(moveCount) < 0 ):
            return self.plus(np.abs(moveCount/COUNTSPERNM))
        else:
            return self.minus(np.abs(moveCount/COUNTSPERNM))
    
    def vel(self, velocity: int):
        """Change velocity, in counts/sec"""
        return self.send_command(f'V{velocity}\r')


    def stop(self):
        """Stop all movement."""
        return self.send_command('STOP\r')

    def close(self):
        """Close the serial port and writes the new wavelength to config.txt."""
        self.ser.close()
        with open("config.txt", 'w') as file:
            newlines = re.sub('(?<=\= )\d+(?:\.\d+)?(?=\n)', str(self.Count/COUNTSPERNM), lines, count=1)
            file.write(newlines)
           

# Example usage
#if __name__ == '__main__':
#    c = McPherson789Controller() #create the spectrometer object
#
#
#    print(c.send_command(" \r")) #space to initialize
#    time.sleep(2)
#
#    print(c.vel(60000)) #set velocity
#    time.sleep(5)
#    print(c.plus(4)) #move up 4nm
#    time.sleep(5)
#    print(c.minus(4)) #move down 4nm
#    time.sleep(5)
#    print(c.move(656)) #move to 656nm
#
#
#    time.sleep(15)
#    print(c.stop()) #stop motion
#
#    c.close() #close and write to config.txt