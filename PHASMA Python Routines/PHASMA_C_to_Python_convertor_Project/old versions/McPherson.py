from McPherson import C207
from McPherson import C209 #The calibration is likely wrong for this one
import time

print(C207.send_command(" \r")) 
print(C207.vel(60000)) #set velocity
print(C209.send_command(" \r")) 
print(C209.vel(60000)) #set velocity

## 207 Spectrometer
def move207(wavelength: float):
   '''moves the spectrometer to a given wavelength (it will not wait for it to finish moving)'''
   C207.move(wavelength)
   time.sleep(5)
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
