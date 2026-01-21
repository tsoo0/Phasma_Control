# -*- coding: utf-8 -*-
"""
Created on Tue Jul 16 14:24:18 2024

@author: tom
"""

import os
os.chdir("C:/Users/tom/Desktop/research/New ACQ Daq/Drivers/NRL Implementation/Send1_22_24/acq400_hapi-master")
import acq400_hapi


os.chdir("C:/Users/tom/Desktop/research/New ACQ Daq/Drivers/NRL Implementation/Send1_22_24/")

#Make sure to go into the local host file (Windows) and associate the name acq2106_392 with
# its respective static IP!
import matplotlib.pyplot as plt
from acq2106_functions import *
import os

import numpy as np
import pandas as pd
import time

sample_rate = 20e6 # currently no provision to change sample rate - the default 
                   # sample rate is 20MSPS, 14-bit

num_samples = int(1e4) # The number of samples to collect after the trigger pulse
num_channels = int(40)               # Number of channels to read out - maximum 48

basedir = os.getcwd()
readout_dir = basedir
readout_fid = "dat.txt"

devname = 'acq2106_392' #This is unique to the box - it has to be linked to static  
                        # ip in the windows host file (we agreed on 192.168.0.112)

uut = acq400_hapi.factory(devname)

sc = acq400_hapi.shotcontrol.ShotController([uut])

config_master(uut, num_samples, num_channels) # Set the acq up for transient (one-shot) acquisition using an 
                                #  external trigger. This line and everything before it only needs to be run 
                                #  once - the subsequent lines can be put in a loop
                                #  for repeated measurements.

arm_transient(uut,sc) # This will arm the acq and leave it waiting for the trigger

dat = readout(uut,sc,num_channels) # Call this after triggering to readout data

timearr = get_uut_timebase(uut,num_samples) # Time array isn't read out with the voltages - it has to be 
                                            #  grabbed seperately. 
data_volts = bits_to_volts(uut,dat)         # raw data is read out as adc counts -
                                            #  Convert each sample to volts 


#Plot the output if you're in the mood
for k in range(0,num_channels):
    plt.plot(timearr,data_volts[k])