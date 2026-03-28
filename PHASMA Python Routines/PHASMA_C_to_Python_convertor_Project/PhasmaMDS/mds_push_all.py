# import numpy as np
import os
import pandas as pd
import mdsthin as mds

from phasma_model.phasma_devices import devdict

# from contextlib import redirect_stdout
stream_output = False
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


def streamout(arg):
    if stream_output == True:
        if type(arg) == str:
            print(arg)
            
            with open(r"C:\PHASMA 2025 DAQ\PyStdOutBuffer.txt", 'a') as f:
                f.write(arg)
                f.write('\n')
            f.close()

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
                print(f"found {dev.name_mds}")
            except:
                print(f"unable to find or read {dev.name_mds}")
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
    
    streamout(c.tcl(f"create pulse {shotnum}"))
    streamout(c.tcl(f"set tree {treename}/shot={shotnum}"))
    
    for curdiag in list(diagdata.keys()):
        curdata = diagdata.get(curdiag)
        streamout(c.tcl(f"set def \TOP.DIAGNOSTICS.{curdiag.diag_name_mds}:DATA"))
        
         #Write to DATA from each device channel
        for devname in list(curdata.keys()):
            
            devdata = curdata.get(devname)
        
            HWdevice = [a for a in curdiag.devices.HWdevices if a.name_mds == devname][0]
            
            devtype = HWdevice.grouping
        
            if devtype == "DATA":
        
                channels = devdata.columns
                
                x_axis_name = channels[0]
                
                x_axis = devdata.get(x_axis_name)
                
                for n,channel in enumerate(channels):
                    try:
                        channeldata = devdata.iloc[:,n].values
                        # if channel.lower() == x_axis_name.lower(): # independent time axis should be irrelevant using signal datatype. Writing to 'Time' node fails
                        #     continue
                        sig = mds.Signal(channeldata, None, x_axis)
                       
                        channel = channel.strip() # remove any leading or trailing spaces that may occur in the column name 
                       
                        streamout(c.put(channel, '`SerializeIn($)', sig.serialize())) # do not remove `
                    except:
                        
                        streamout(f"failed to write data: {curdiag.diag_name_mds}.DATA.{channel}")
                        
                        continue
                    
            elif devtype == "SETUP":
                c.tcl(f"set def \TOP.DIAGNOSTICS.{curdiag.diag_name_mds}:SETUP")
                channels = devdata.columns
                channels = [a.upper()[:12] for a in channels]
                devdata = devdata
                for n,channel in enumerate(channels):
                    try:
                        channeldata = devdata.iloc[0,n]
                        streamout(c.put(channel, '$', mds.Float32(channeldata)))
                        # if type(channeldata==str):
                       
                        #     print(c.put(channel.upper(), '$', channeldata))
                            
                        # elif type(channeldata==float) or type(channel_data == int):
                            
                            
                    except:
                        
                        streamout(f"failed to write data: {curdiag.diag_name_mds}.SETUP.{channel}")
                        
                        continue
    c.tcl('close')
    
    streamout('donezo')

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

    # push_all_mds(treename, exp_ip, raw_data_dir, shotnum = 1400)
    push_all_mds_latest(treename, exp_ip, raw_data_dir)