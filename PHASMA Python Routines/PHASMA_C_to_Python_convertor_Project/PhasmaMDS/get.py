# import numpy as np
import os
import pandas as pd
import mdsthin as mds

from phasma_model.phasma_devices import devdict

treename = "phasma2025"

exp_ip = '127.0.0.1:8000'

raw_data_dir = r"C:\Users\tjroo\Desktop\Research\Light Data Analysis Tools\datavis\PhasmaMDS\example_data"    

shotnum=417

#read in a locally-stored white-space seperated table containing raw data for one shot on one digitizer
def read_format_mds(device_fid):
    
    # shotdat = pd.read_csv(device_fid,delimiter='\t',header=0)
    shotdat = pd.read_csv(device_fid,delimiter=',',header=0)
    
    shotdat.columns = [acol.split(':')[-1] for acol in list(shotdat.columns)]
    
    return shotdat

# Take raw shot data stored locally and write it to the mds server under a new 
# shot within treename

c = mds.Connection(exp_ip)

def trycast(intorstr):
    try:
        res = int(intorstr)
        
    except:
        return 0
    
    return res

#get tree structure from model shot by parsing the return from tcl; probably an easier way to do this 
c.openTree(treename,-1)
c.tcl('set def \TOP:DIAGNOSTICS')
# devstr = c.tcl('directory')    
# devstr=devstr.replace("\PHASMA::TOP.DEVICES","")
# devs = devstr.split('\n')
# devs = devstr.split('  ')
# devs = [dev for dev in [dev.strip() for dev in devs] if len(dev)>0 and dev.isupper()]

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
            continue
        
        datadict.update({dev.name_mds:devdat})
    
    diagdata.update({diag:datadict})


#remove diagnostics with no found data for this shot
for key in list(diagdata.keys()):
    if not diagdata[key]:
        del diagdata[key]
        
        
# compile device data according to diagnostic


diagdata_col = {}
for diag in list(diagdata.keys()):
    
    for data in list(diagdata[diag]):
        
        try:
            newdat = newdat.join(data.set_index('Time'),on='Time')
        except:
            newdat = data
            
        
   
    diagdata_col.update({diag.name_mds:newdat})



    
    
    



# c.tcl(f"create pulse {shotnum}")
# c.tcl(f"set tree {treename}/shot={shotnum}")
# for shotdiag in doof.keys():
    
#     diagdata = doof[shotdiag]
    
#     c.tcl(f"set def \TOP.DIAGNOSTICS.{shotdiag}")
    
#     channels = diagdata.columns
#     timearr = diagdata['Time'].values
#     for n,channel in enumerate(channels):
#         try:
#             channeldata = diagdata.iloc[:,n].values
#             if channel.lower() == 'time': # independent time axis should be irrelevant using signal datatype. Writing to 'Time' node fails
#                 continue
#             sig = mds.Signal(channeldata, None, timearr)
#             print(channel)
#             c.put(channel.upper(), '`SerializeIn($)', sig.serialize())
#         except:
#             continue
    
#     # c.tcl("set def TOP")
