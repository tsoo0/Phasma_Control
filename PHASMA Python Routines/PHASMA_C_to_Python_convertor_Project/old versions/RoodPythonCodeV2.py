# #!/usr/bin/env python ## this should eventually point to the dedicated ACQ enviro

basedir = "C:/Users/Scime Lab/Desktop/DTAQ integration/48_channel_tomo_drivers"

acqdir = "/acq400_hapi-master"

import os
import sys

os.chdir(basedir + acqdir)
import acq400_hapi as acq

import numpy as np

import time

V_full_scale = 4

#pushes default configuration to one box - only needs to be run once, and not before every acquisition
# dev_ip - static ip (or DHCP alias) of device to change settings on 
# num_samples - number of samples per channel per transient acquisition
# Defaults to 50 Ohm termination on all channels, trigger from front panel
def config_master(dev_ip,num_samples = 1000,termination='50R', trig_source='FP',sample_rate=20e6):

    site0 = comm_s0(dev_ip)
    #Don't currently foresee a need to change any of these fields aside from the number of samples (POST)
    site0.sr("set.site 0 transient PRE=0 POST={post:d} OSAM=1 DEMUX={demux:d} SOFT_TRIGGER={soft_tr:d}".format(
        post=num_samples,
        demux=1,
        soft_tr=0,
        ))
    
    site0.sr('set.site 1 trg 1,0,1')
    
    # site0.sr('set.site 0 transient SOFT_TRIGGER=1') # useful for debugging only
    
    if trig_source == "HDMI":# Use breakout board's external trigger (48-channel acq2106)
        
        comm_s0(dev_ip).sr("set.site 0 SIG:SRC:TRG:0 HDMI")
    
    if termination == '50R':
        enable_50Ohm_all(dev_ip)

# Arm the hard trigger of a specific acq
# ip or DNS name of acq to arm
def arm_transient(dev_ip):
    
    site0 = comm_s0(dev_ip)
    
    return site0.sr("set_arm")

def Config_Arm(dev_ip,num_samples):
    
    config_master(dev_ip,num_samples=num_samples,trig_source='HDMI')
    
    # time.sleep(1)

    arm_transient(dev_ip)    
    
# returns time array starting at 0 corresponding to last shot
def get_timebase(dev_ip, num_samples):
    site1 = comm_s1(dev_ip)

    Fs = int(site1.sr('ACQ480:OSR').split(' ')[1])
    
    timearr = np.linspace(0,num_samples/Fs, num_samples)
    
    return timearr

def int16_to_volts(rawdat,V_full_scale):
    
    if type(rawdat) is list:
        return [raw * V_full_scale / (2**15 - 1) for raw in rawdat]
    else:
        return np.multiply(rawdat, V_full_scale / (2**15 - 1))

def enable_50Ohm_all(dev_ip): # enable 50 Ohm termination across all channels
    
    sitelist = list(comm_s0(dev_ip).sr("sites"))
    sitelist = [int(k) for k in sitelist if k.isnumeric()]
    
    for site in sitelist:
        site_n = acq.Siteclient(dev_ip, acq.AcqPorts.SITE0 + site)            

        site_n.sr("T50R 1")

# Open a socket with Site 0 at a given IP or alias; returns an open client
def comm_s0(dev_ip):
    
    return acq.Siteclient(dev_ip, acq.AcqPorts.SITE0)

def comm_s1(dev_ip):
    
    return acq.Siteclient(dev_ip, acq.AcqPorts.SITE0 + acq.acq400.AcqSites.SITE1)

def read_format_raw(dev_ip, num_channels, num_samples, sample_size_bytes = int(2)):
    
    channels = list(range(1, num_channels+1))
    
    rawdata = []
    
    tstart = time.time()
    for channel in channels:
        
        nc = acq.ChannelClient(dev_ip,channel)
        
        rawdata.append(nc.read(num_samples, data_size=sample_size_bytes))
    
    print("read took " + str(time.time() - tstart) + " seconds")

    return rawdata

def post_proc(dev_ip,rawdat,num_samples,V_full_scale):
    
    rawdat = int16_to_volts(rawdat,V_full_scale)
    
    rawdat.insert(0,get_timebase(dev_ip, num_samples))
    
    return np.array(rawdat).T

def store_local(data, datafid, path):
    import pandas as pd
    
    df = pd.DataFrame(data)
    
    df.to_csv(path + "/" +  datafid)
    
    
def readout_store(dev_ip, num_channels, num_samples, shotnum, readout_dir):
    
    #readout_dir.replace("'\'", "/")
    
    # readout_dir = basedir
    
    # try:
    #     raw = read_format_raw(dev1,num_channels,num_samples)
        
    #     data = post_proc(dev1, raw, num_samples,V_full_scale)
    
    #     store_local(data, str(shotnum) + "_Tomographic_48channel" + ".txt", readout_dir)
    
    # except: 
    #     sys.stderr.write("readout error - probably reading from empty buffer")
    
    dev_ip = "192.168.1.254"
    num_channels = 48
    num_samples = 30000
    # shotnum = 12122121
    readout_dir = "D:\MDSPLUS_PHASMA\RAWDATA"

    # time.sleep(1)

    raw = read_format_raw(dev_ip,num_channels,num_samples)
    
    data = post_proc(dev_ip, raw, num_samples,V_full_scale)

    store_local(data, str(shotnum) + "_Tomographic_48channel" + ".txt", readout_dir)
    

if __name__ == "__main__":
#%%
    
    dev1 = "192.168.1.254" # 433
    # dev1 = "192.168.0.178" # 434
    
    num_samples = int(3e4) # max 4 million samples with 48 channels

    read_delay = 10
    arm_delay = 2
    
    num_channels = 48
    
    tstart = time.time_ns()
    
    # config_master(dev1,num_samples=num_samples,trig_source='HDMI') # Set the acq up for transient (one-shot) acquisition using an 
    #                             #  external trigger. This line and everything before it only needs to be run 
    #                             #  once - the subsequent lines can be put in a loop
    #                             #  for repeated measurements.
    time.sleep(arm_delay)     

    Config_Arm(dev1, num_samples)
                       
# %%
    # arm_transient(dev1)
    
    time.sleep(read_delay) # need some time between arming and acquiring
                            # if the functions are called consecutively
    #%%
    # raw = read_format_raw(dev1,num_channels,num_samples)
    
    # data = post_proc(dev1, raw, num_samples,V_full_scale)

    # store_local(data, "test.csv", basedir + '/')
    
    # readout_store(dev1, num_channels, num_samples, 90000, basedir)
    # readout_store(dev1, num_channels, num_samples, 1000000, "D:\MDSPLUS_PHASMA\RAWDATA")
    
    raw = read_format_raw(dev1,num_channels,num_samples)