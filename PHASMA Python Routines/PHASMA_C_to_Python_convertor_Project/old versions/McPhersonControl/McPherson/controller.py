import serial
import time
import numpy as np
import re

# Example usage
#if __name__ == '__main__':
#   # c = McPherson789Controller() #create the spectrometer object
#
#
#    print(c.send_command(" \r")) #space to initialize
#    time.sleep(2)
#
#    print(c.vel(60000)) #set velocity
#    time.sleep(2)
#    #print(c.plus(4)) #move up 4nm
#    #time.sleep(5)
#    #print(c.minus(4)) #move down 4nm
#    #time.sleep(5)
#    print(c.move(600)) #move to 656nm
#
#    #706.9
#    time.sleep(15)
#    print(c.stop()) #stop motion
#
#    c.close() #close and write to config.txt




#controller class
class McPherson789Controller:

    def __init__(self, baudrate=9600, timeout=1, config="Config207.txt"):
        #opening config.txt
        try:
            with open(config, 'r') as file:
                self.lines = file.read()
                results = re.findall('(?<=\= )\d+(?:\.\d+)?(?=\n)', self.lines)
                print(results)
                self.initialWavelength = float(results[0])
                self.COUNTSPERNM = float(results[1])
                self.port = "COM"+str(results[2])
                self.config = config

        except:
            raise Exception("config.txt or config settings not found!")

        self.ser = serial.Serial(
            port=self.port,
            baudrate=baudrate,
            bytesize=serial.EIGHTBITS,
            parity=serial.PARITY_NONE,
            stopbits=serial.STOPBITS_ONE,
            timeout=timeout
        )
        self.Count = self.initialWavelength * self.COUNTSPERNM
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
        counts = int(np.floor(nm*self.COUNTSPERNM))
        self.Count += counts
        return self.send_command(f'+{counts}\r')
    
    def minus(self, nm:float):
        """Move a certain number of nanometers down"""
        counts = int(np.floor(nm*self.COUNTSPERNM))
        self.Count -= counts
        return self.send_command(f'-{counts}\r')
    
    def move(self, wavelength:float):
        """Move to a specific wavelength"""
        enteredCount = wavelength * self.COUNTSPERNM
        moveCount = self.Count-enteredCount

        if (np.floor(moveCount) == 0):
            return "Already at that wavelength!!"
        elif (np.floor(moveCount) < 0 ):
            return self.plus(np.abs(moveCount/self.COUNTSPERNM))
        else:
            return self.minus(np.abs(moveCount/self.COUNTSPERNM))
    
    def vel(self, velocity: int):
        """Change velocity, in counts/sec"""
        return self.send_command(f'V{velocity}\r')


    def stop(self):
        """Stop all movement."""
        return self.send_command('STOP\r')

    def close(self, location=-1.0):
        """Close the serial port and writes the new wavelength to config.txt."""
        self.ser.close()
        if (location==-1.0): #allows for overwriting the position
            location = self.Count/self.COUNTSPERNM
        with open(self.config, 'w') as file:
            newlines = re.sub('(?<=\= )\d+(?:\.\d+)?(?=\n)', str(location), self.lines, count=1)
            file.write(newlines)
           

