import acq400_hapi as acq400_hapi
import pandas as pd
import time

#Configure an acq for transient acquisiton
def config_master(uut,samples,num_channels):

    uut.configure_post(role='master',trigger=[1,0,1],post=samples)
    uut.s0.SIG_SRC_TRG_0 = "HDMI" # Use breakout board's external trigger
    
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
    
    channels = list(range(1, nchan+1))
    rawdat = uut.read_channels((channels))
    
    return rawdat

def get_uut_timebase(uut,nsamples):
    return uut.read_transient_timebase(nsamples)

def bits_to_volts(uut,dat):
    dat_volts = []
    for channel,channel_data in enumerate(dat,start=1):
        channel_volts = uut.chan2volts(channel,channel_data)
        
        dat_volts.append(channel_volts)
        
    return dat_volts