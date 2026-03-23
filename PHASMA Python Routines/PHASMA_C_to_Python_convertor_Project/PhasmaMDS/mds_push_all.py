# import numpy as np
import os
import pandas as pd
import mdsthin as mds

from phasma_model.phasma_devices import devdict

# Read a .csv file into a dataframe; first row must contain field names and first column 
#   will be interpreted as the independent axis
def read_format_mds(device_fid):
    
    shotdat = pd.read_csv(device_fid,delimiter=',',header=0)
    
    # shotdat.columns = [acol.split(':')[-1] for acol in list(shotdat.columns)]
    
    return shotdat

def trycast(intorstr):
    try:
        res = int(intorstr)
        
    except:
        return 0
    
    return res

# Error checking tcl write; print errors from mds server they any
def tcl_write(conn, msgstr):
    
    retval = conn.tcl(msgstr)

    if retval is not None:
        print(retval)

# creates a new shot tree in the mds server and populates it with data from the local file system
def push_all_mds(treename, exp_ip, raw_data_dir, shotnum):
    
    # construct a dict of dicts with each diagnostic cotaining a dict of data for each found hardware device
    os.chdir(raw_data_dir)
    shotfids = [shotfid for shotfid in os.listdir(raw_data_dir) if trycast(shotfid.split('_')[0]) == shotnum]
    diagdata = {}
    for diag in list(devdict.values()):
        datadict = {}
        devices = diag.devices.HWdevices
    
        for dev in devices:
            
            devfid = list(filter(lambda x: dev.name_local in x, shotfids)) #shoud return one element
            
            try:
                devdat = read_format_mds(devfid[0])
            except:
                print(f"couldn't find or open data for device {dev.name_local}")
                continue
            
            datadict.update({dev.name_mds:devdat})
    
        diagdata.update({diag:datadict})
    
    #remove entries for diagnostics with no found data for this shot
    for key in list(diagdata.keys()):
        if not diagdata[key]:
            del diagdata[key]
            
    # create a new shot tree for the given shot number and populate it with any files in raw_data_dir that 
    # match the local names in phasma_model
    c = mds.Connection(exp_ip)
    
    tcl_write(c,f'set tree {treename}')
    tcl_write(c,f"create pulse {shotnum}")
    tcl_write(c,f"set tree {treename}/shot={shotnum}")
    
    for curdiag in list(diagdata.keys()):
        curdata = diagdata.get(curdiag)
        tcl_write(c,f"set def \TOP.DIAGNOSTICS.{curdiag.diag_name_mds}:DATA")
        
         #Write to DATA from each device channel
        for devname in list(curdata.keys()):
            
            devdata = curdata.get(devname)
        
            HWdevice = [a for a in curdiag.devices.HWdevices if a.name_mds == devname][0]
            
            devtype = HWdevice.grouping
        
            if devtype == "DATA":
        
                channels = devdata.columns
                channels = [a.upper()[:12] for a in channels]
                x_axis_name = channels[0]
                
                x_axis = devdata.get(x_axis_name)
                
                for n,channel in enumerate(channels):
                    try:
                        channeldata = devdata.iloc[:,n].values
                        # if channel.lower() == x_axis_name.lower(): # independent time axis should be irrelevant using
                        #                                            # signal datatype. Writing to 'Time' node fails
                                                                   
                        #     sig = mds.Signal(x_axis,None,x_axis)
                           
                        #     retval = c.put(channel.strip(), '`SerializeIn($)', sig.serialize())
                            
                        #     if retval is not None:
                        #         print(retval)                                           
                                                                   
                        #     continue
                        
                        # else:
                        sig = mds.Signal(channeldata, None, x_axis)
                       
                        retval = c.put(channel.strip(), '`SerializeIn($)', sig.serialize())
                        
                        if retval is not None:
                            print(retval)
                        
                    except:
                        
                        print(f"failed to write data: {curdiag.diag_name_mds}.DATA.{channel}")
                        
                        continue
                    
            elif devtype == "SETUP":
                tcl_write(c,f"set def \TOP.DIAGNOSTICS.{curdiag.diag_name_mds}:SETUP")
                channels = devdata.columns
                channels = [a.upper()[:12] for a in channels]
                devdata = devdata
                for n,channel in enumerate(channels):
                    try:
                        channeldata = devdata.iloc[0,n]
                        
                        retval = c.put(channel, '$', mds.Float32(channeldata))
                        
                        if retval is not None:
                            print(retval)
  
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
    # exp_ip = '127.0.0.1:8000'
    raw_data_dir = "D:\\PHASMA_RawData"   
    # raw_data_dir = r'C:\Users\tjroo\Desktop\Research\Light Data Analysis Tools\datavis\PhasmaMDS\example_data'

    # push_all_mds(treename, exp_ip, raw_data_dir, shotnum = 1005)
    push_all_mds_latest(treename, exp_ip, raw_data_dir)