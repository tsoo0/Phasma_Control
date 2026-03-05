#!/usr/bin/env python ## this should eventually point to the dedicated ACQ enviro

basedir = "C:/Users/tjroo/Desktop/Research/acq tomo/standalone"

acqdir = "/acq400_hapi-master/acq400_hapi-master"

import os
import matplotlib.pyplot as plt

os.chdir(basedir + acqdir)
import acq400_hapi

import numpy as np
import pandas as pd
import time

#pushes default configuration to one box - only needs to be run once, and not before every acquisition
def config_master(uut,shotnum=0,numsamples = 1000):
 
    uut.s0.transient = 'SOFT_TRIGGER=0'
    uut.s0.transient = 'PRE=0'
    uut.s0.transient='POST='+str(samples)

    uut.s1.clk = "0,3,1" #internal clock
    uut.s1.trg = "1,0,1"

    uut.s1.clkdiv = 1
    
    uut.s0.SIG_SRC_TRG_0 = "HDMI" # Use breakout board's external trigger
    
    enable_50Ohm_all(uut)

# Arm the trigger for a transient acquisition
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

def enable_50Ohm_all(uut):
    
    sites = uut.sites
    
    for site in sites:
        uut[site].T50R = 1;
        
"""
'mr_10dec': 'mr_10dec',
'mr_en': 'mr_en',
'mr_sel0': 'mr_sel0',
'mr_sel1': 'mr_sel1',
"""
def enable_MR_40MSPS(uut):
    
    sites = uut.sites
    
    for site in sites:
    
        uut[site].mr_en = 1
    
        uut[site].mr_sel0 = 0
        uut[site].mr_sel1 = 1    


if __name__ == "__main__":
    
    fid = "dat.txt"
    
    dev1 = 'acq2106_433'
    # dev1 = "192.168.0.177" # 433
    
    # dev2 = "acq2106_373"
    
    uut1 = acq400_hapi.factory(dev1)
    # uut2 = acq400_hapi.factory(dev2)
    # uuts = [uut1,uut2]
    uuts=[uut1]
    
    sc = acq400_hapi.shotcontrol.ShotController(uuts)
    
    samples = int(4000)
    
    shot = int(0)
    
    config_master(uut1,shotnum=0,numsamples=samples)
    
    tstart = time.time()
    
    enable_MR_40MSPS(uut1)
    
    # for n in range(0,4):
    sc.run_shot()

    acq400_hapi.cleanup.sleep(1.0) # Apparently this absolutely needs to be here
    
    rawdat = sc.read_channels()
    
    elapsed = time.time() - tstart
    print(elapsed)
    
    uut1dat = rawdat[0][0] 
    uut1dat = bits_to_volts(uut1,uut1dat)
    
    timearr = get_uut_timebase(uut1,samples)
    
    datdf = pd.DataFrame(np.rot90(uut1dat))
    datdf.insert(0,"Time",timearr)
    
    datdf.plot('Time')