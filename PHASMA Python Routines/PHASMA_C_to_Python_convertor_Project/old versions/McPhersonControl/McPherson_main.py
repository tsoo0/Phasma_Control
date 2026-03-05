import sys
import os

os.chdir(os.path.dirname(os.path.abspath(__file__)))

from McPherson import C207
from McPherson import C209 #The calibration is likely wrong for this one
import time

# print(C207.send_command(" \r")) 
# print(C207.vel(60000)) #set velocity
# print(C209.send_command(" \r")) 
# print(C209.vel(60000)) #set velocity


## 207 Spectrometer
def move207(wavelength: float):
   '''moves the spectrometer to a given wavelength (it will not wait for it to finish moving)'''
   print(wavelength)
   C207.move(wavelength)
   time.sleep(1)
   C207.close()

def command207(command: str):
   C207.send_command(command)
   time.sleep(1)
   C207.close()

def overwrite207(wavelength: float):
   '''manually changes the wavelength in the config file. Must be a positive float.'''
   C207.close(wavelength)

## 209 Spectrometer
def move209(wavelength: float):
   '''moves the spectrometer to a given wavelength (it will not wait for it to finish moving)'''
   C209.move(wavelength)
   time.sleep(1)
   C209.close()

def command209(command: str):
   C209.send_command(command)
   time.sleep(1)
   C209.close()

def overwrite209(wavelength: float):
   '''manually changes the wavelength in the config file. Must be a positive float.'''
   C209.close(wavelength)


if __name__ == "__main__":
    failed = 0
    command = 2;
    
    print(f"Moving to {sys.argv[1]} nm")
    
    wavelength = float(sys.argv[1])
    
    
    if len(sys.argv) > 2:
        if sys.argv[command] == 'overwrite207':
            overwrite207(wavelength)
        elif sys.argv[command] == 'overwrite209':
            overwrite209(wavelength)
        elif sys.argv[command] == 'move207':
            move207(wavelength)
        elif sys.argv[command] == 'move209':
            move209(wavelength)
            
    else:        
        if sys.argv[command] == 'command207':
            command207(wavelength)
        elif sys.argv[command] == 'command209':
            command209(wavelength)
        else:
            print(f'McPherson command not recognized: {sys.argv[command]}')
            failed = 1;

    with open ("log.txt", "a" ) as logfile:
        logfile.write(f" {wavelength} {sys.argv[2]} failflag={failed} \n")

#if __name__ == "__main__":
#    try:
#        overwrite207(200)
#    except:
#        with open('doof.txt') as f:
#            f.write(os.curdir)
#    
#    #move207(550.4)
