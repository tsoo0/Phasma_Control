# import numpy as np
import os
import pandas as pd
import mdsthin as mds

from phasma_model.phasma_devices import devdict


#read in a locally-stored comma seperated table containing raw data for one shot on one digitizer
def read_format_mds(device_fid):
    
    shotdat = pd.read_csv(device_fid,delimiter=',',header=0)
    
    shotdat.columns = [acol.split(':')[-1] for acol in list(shotdat.columns)]
    
    return shotdat

def trycast(intorstr):
    try:
        res = int(intorstr)
        
    except:
        return 0
    
    return res


def push_all_mds(treename, exp_ip, raw_data_dir, shotnum):
    
    # construct a dict of dicts with each diagnostic cotaining a dict of data for each found hardware device
    os.chdir(raw_data_dir)
    shotfids = [shotfid for shotfid in os.listdir(raw_data_dir) if trycast(shotfid.split('_')[0]) == shotnum]
    diagdata = {}
    for diag in list(devdict.values()):
        datadict = {}
        devices = diag.devices.HWdevices
    
    # devmap = list(map(lambda dev: {dev.name_mds:dev.name_local}, devices))
    
        for dev in devices:
            
            devfid = list(filter(lambda x: dev.name_local in x, shotfids)) #shoud return one element
            
            try:
                devdat = read_format_mds(devfid[0])
            except:
                print(f"failed initial read of {dev.name_mds}")
                continue
            
            datadict.update({dev.name_mds:devdat})
    
        diagdata.update({diag:datadict})
    
    #remove diagnostics with no found data for this shot
    for key in list(diagdata.keys()):
        if not diagdata[key]:
            del diagdata[key]
            
    c = mds.Connection(exp_ip)
    
    # create a new shot tree for the given shot number and populate with any files in raw_data_dir that match the local names in phasma_model
    c.tcl(f'set tree {treename}')
    
    print(c.tcl(f"create pulse {shotnum}"))
    print(c.tcl(f"set tree {treename}/shot={shotnum}"))
    
    for curdiag in list(diagdata.keys()):
        curdata = diagdata.get(curdiag)
        c.tcl(f"set def \TOP.DIAGNOSTICS.{curdiag.diag_name_mds}:DATA")
        
         #Write to DATA from each device channel
        for devname in list(curdata.keys()):
            
            devdata = curdata.get(devname)
        
            HWdevice = [a for a in curdiag.devices.HWdevices if a.name_mds == devname][0]
            
            devtype = HWdevice.grouping
        
            if devtype == "DATA":
        
                channels = devdata.columns
                
                x_axis_name = channels[0]
                
                x_axis = devdata.get(x_axis_name)
                
                # if 
                
                for n,channel in enumerate(channels):
                    try:
                        channeldata = devdata.iloc[:,n].values
                        # if channel.lower() == x_axis_name.lower(): # independent time axis should be irrelevant using signal datatype. Writing to 'Time' node fails
                        #     continue
                        sig = mds.Signal(channeldata, None, x_axis)
                       
                        print(c.put(channel.strip(), '`SerializeIn($)', sig.serialize()))
                    except:
                        
                        print(f"failed to write data: {curdiag.diag_name_mds}.DATA.{channel}")
                        
                        continue
                    
            elif devtype == "SETUP":
                c.tcl(f"set def \TOP.DIAGNOSTICS.{curdiag.diag_name_mds}:SETUP")
                channels = devdata.columns
                channels = [a.upper()[:12] for a in channels]
                devdata = devdata
                for n,channel in enumerate(channels):
                    try:
                        channeldata = devdata.iloc[0,n]
                        
                        chname_sanitized = channel.strip()
                        
                        print(c.put(chname_sanitized, '$', mds.Float32(channeldata)))
                        # if type(channeldata==str):
                       
                        #     print(c.put(channel.upper(), '$', channeldata))
                            
                        # elif type(channeldata==float) or type(channel_data == int):
                            
                            
                    except:
                        
                        print(f"failed to write data: {curdiag.diag_name_mds}.SETUP.{channel}")
                        
                        continue
                    
                    
    c.tcl("close")

def push_all_mds_latest(treename, exp_ip, raw_data_dir):
    c = mds.Connection(exp_ip)
    c.openTree(treename,-1)
    
    curshot = int(c.tcl(f'show current {treename}').split(' ')[-1])
    c.tcl("close")
    push_all_mds(treename, exp_ip, raw_data_dir, shotnum = curshot)

if __name__ == '__main__':

    treename = 'phasma2025'
    exp_ip = '127.0.0.1:57800'
    raw_data_dir = "D:\\PHASMA_RawData"    

    push_all_mds(treename, exp_ip, raw_data_dir, shotnum = 1288)
    # push_all_mds_latest(treename, exp_ip, raw_data_dir)