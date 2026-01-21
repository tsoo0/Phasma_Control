# Configures and acq2106 for transient (one-shot) acquisition
# written by Tom Rood, WVU Center for Kinetic Plasma Physics
import os
os.chdir("C:/Users/tom/Desktop/research/New ACQ Daq/Drivers/NRL Implementation/Send1_22_24/acq400_hapi-master")


#Make sure to go into the local host file (Windows) and associate the name acq2106_392 with
# its respective static IP!
import matplotlib.pyplot as plt
# from acq2106_functions import *
import acq400_hapi as acq400_hapi


import numpy as np
import pandas as pd
import time


def config_master(uut,samples,num_channels):

    uut.configure_post(role='master',trigger=[1,1,1],post=samples)
    # uut.s0.SIG_SRC_TRG_0 = "HDMI" # Use breakout board's external trigger

    # uut.s0.TRANSIENT_DEMUX = 0 # DEMUX=0 to stream data from 53000 instead of hopping between each port-channel; results in a minor speed increase
    # uut.s0.transient = 'SOFT_TRIGGER=0'
    # # uut.s0.transient = 'PRE=0'
    # uut.s0.transient='POST='+str(samples)
    # uut.s1.clk = "0,3,1" # use internal clock
    # uut.s1.trg = "1,0,1"
    # uut.s1.clkdiv = 1
    # uut.s1.sync_trg_to_clk=1
    
# Arm an acq that has been configured for transient acquisition
def arm_transient(uut, sc):

    sc.run_shot() # arm and wait for trigger

# Read out the results of the last acquisiton and store it as csv to a specified directory
def readout(uut,sc,nchan):

    acq400_hapi.cleanup.sleep(1.0) # Possible unnecessary: Waiting a second or two after the trigger solves the problem of 
                                      #reading out stale data from the acq's memory
    starttime = time.time_ns()
    channels = list(range(1, nchan+1))
    rawdat = uut.read_channels((channels))
    print("read done in" + str((time.time_ns()- starttime)*1e-9))
    return rawdat

def get_uut_timebase(uut,nsamples):
    return uut.read_transient_timebase(nsamples)

def bits_to_volts(uut,dat):
    dat_volts = []
    starttime = time.time_ns()
    for channel,channel_data in enumerate(dat,start=1):
        channel_volts = uut.chan2volts(channel,channel_data)
        
        dat_volts.append(channel_volts)
    print("dat2volt done in" + str((time.time_ns()- starttime)*1e-9))
    return dat_volts

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

# dat = readout(uut,sc,num_channels) # Call this after triggering to readout data

rawdat = sc.read_channels() #readout all channels to local variable rawdat

timearr = get_uut_timebase(uut,num_samples) # Time array isn't read out with the voltages - it has to be 
                                            #  grabbed seperately. 
# data_volts = bits_to_volts(uut,dat)         # raw data is read out as adc counts -
                                            #  Convert each sample to volts 


# #Plot the output if you're in the mood
# for k in range(0,num_channels):
#     plt.plot(timearr,data_volts[k])