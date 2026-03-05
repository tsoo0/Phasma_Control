# -*- coding: utf-8 -*-
"""
Created on Thu Jan 29 22:10:32 2026

@author: Scime Lab
"""

# import numpy as np
import os
import pandas as pd
import mdsthin as mds

#read in a locally-stored white-space seperated table containing raw data for one shot on one digitizer
def read_format_mds(device_fid):
    
    # shotdat = pd.read_csv(device_fid,delimiter='\t',header=0)
    shotdat = pd.read_csv(device_fid,delimiter='\t',header=0)
    return shotdat

# Take raw shot data stored locally and write it to the mds server under a new 
# shot within treename
def push_all_mds(treename, exp_ip, raw_data_dir, shotnum):   

    c = mds.Connection(exp_ip)
    
    shotfids = [shotfid for shotfid in os.listdir(raw_data_dir) if str(shotnum) in shotfid]
    
    #get tree structure from model shot by parsing the return from tcl; probably an easier way to do this 
    c.openTree(treename,-1)
    c.tcl('set def \TOP:DEVICES')
    devstr = c.tcl('directory')    
    devstr=devstr.replace("\PHASMA::TOP.DEVICES","")
    devs = devstr.split('\n')
    devs = devstr.split('  ')
    devs = [dev for dev in [dev.strip() for dev in devs] if len(dev)>0 and dev.isupper()]
    
    # Get the locally stored raw shot data; scan the phasma_raw directory for fids starting 
    # with desired shot number, then look for device labels in the resulting list
    rawdata={}
    os.chdir(raw_data_dir)
    for shotfid in shotfids:
        for dev in devs:
            newprefix = str(shotnum) + '_' + dev
            
            if newprefix in shotfid:
                shotdat = read_format_mds(shotfid)
            
                rawdata[dev] = shotdat
    
    # c = mds.Connection(exp_ip)
    
    c.tcl(f"create pulse {shotnum}")
    # c.openTree(treename, shotnum)
    for dev in rawdata.keys():
        
        devdat = rawdata.get(dev)
        
        c.tcl(f"set def \TOP.DEVICES.{dev}")
        
        channels = devdat.columns
        
        for n,channel in enumerate(channels):
            
            if channel.lower() == 'time': # independent time axis should be irrelevant using signal datatype. Writing to 'Time' node fails
                continue
            
            sig = mds.Signal(devdat.iloc[:,n].values, None, devdat['Time'].values)
            print(channel)
            c.put(channel.upper(), '`SerializeIn($)', sig.serialize())
        
        # c.tcl("set def TOP")

    c.tcl('write')
    c.tcl('close')

def push_all_mds_latest(treename, exp_ip, raw_data_dir):
    c = mds.Connection(exp_ip)
    c.openTree(treename,-1)
    curshot = int(c.tcl(f'show current {treename}').split(' ')[-1])
    push_all_mds(treename, exp_ip, raw_data_dir, shotnum = curshot)

if __name__ == '__main__':

    treename = 'phasma2025'
    exp_ip = '127.0.0.1:57800'
    raw_data_dir = "D:\\PHASMA_RawData"    

    # push_all_mds(treename, exp_ip, raw_data_dir, shotnum = 1)
    push_all_mds_latest(treename, exp_ip, raw_data_dir)