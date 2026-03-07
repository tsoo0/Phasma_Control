# import numpy as np
import os
import pandas as pd
import mdsthin as mds

from phasma_model.phasma_devices import devdict

#read in a locally-stored white-space seperated table containing raw data for one shot on one digitizer
def read_format_mds(device_fid):
    
    # shotdat = pd.read_csv(device_fid,delimiter='\t',header=0)
    shotdat = pd.read_csv(device_fid,delimiter=',',header=0)
    
    shotdat.columns = [acol.split(':')[-1] for acol in list(shotdat.columns)]
    
    return shotdat

# Take raw shot data stored locally and write it to the mds server under a new 
# shot within treename
def push_all_mds(treename, exp_ip, raw_data_dir, shotnum):   

    c = mds.Connection(exp_ip)
    
    def trycast(intorstr):
        try:
            res = int(intorstr)
            
        except:
            return 0
        
        return res

    shotfids = [shotfid for shotfid in os.listdir(raw_data_dir) if trycast(shotfid.split('_')[0]) == shotnum]
    
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
            
            if newprefix.upper() in shotfid.upper():
                shotdat = read_format_mds(shotfid)
            
                rawdata[dev] = shotdat
    

    # Reorganize data from rawdata into clusters defined by the mapping in their config file
    
    doof = {}
    for diag in devdict:
        
        diagdevs = [HWdev.name_local.upper() for HWdev in devdict[diag].devices.HWdevices]
        for shotdev in list(rawdata.keys()):
            if shotdev in diagdevs:
               try:
                   newdat= newdat.join(rawdata[shotdev].set_index('Time'),on='Time')
                 
               except:
                   newdat = rawdata[shotdev]
        try:
            doof.update({diag:newdat})   
            del newdat        
        except:
            continue
    
    c.tcl(f"create pulse {shotnum}")
    c.tcl(f"set tree phasma2025/shot={shotnum}")
    for shotdiag in doof.keys():
        
        diagdata = doof[shotdiag]
        
        c.tcl(f"set def \TOP.DIAGNOSTICS.{shotdiag}")
        
        channels = diagdata.columns
        timearr = diagdata['Time'].values
        for n,channel in enumerate(channels):
            try:
                channeldata = diagdata.iloc[:,n].values
                if channel.lower() == 'time': # independent time axis should be irrelevant using signal datatype. Writing to 'Time' node fails
                    continue
                sig = mds.Signal(channeldata, None, timearr)
                print(channel)
                c.put(channel.upper(), '`SerializeIn($)', sig.serialize())
            except:
                continue
        
        # c.tcl("set def TOP")

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