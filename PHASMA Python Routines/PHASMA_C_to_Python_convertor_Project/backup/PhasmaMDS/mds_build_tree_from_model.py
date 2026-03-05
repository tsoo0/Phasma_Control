# REPLACES EXISTING MODEL TREE! #
# Generate a new experimental model on a running mds server, !!overwriting the old one if it exists.!!

# tree type devices,diagnostics,analysis

import mdsthin as mds

# import diagnostics
import os
print(os.getcwd())
from phasma_model.diagnostics import *

from phasma_model.phasma_devices import devdict

treename = 'phasma'

c = mds.Connection('127.0.0.1:8000')

print(c.tcl(f'edit {treename}/new'))

# %%
root='DEVICES'
c.tcl(f"set def \TOP")
c.tcl(f"add node .{root}")
c.tcl(f"add tage .{root} devices")
c.tcl(f"set def .{root}")

for diagname in devdict.keys():
    for dev in devdict.get(diagname):
       
        devname = dev.name_mds
        
        c.tcl(f"add node .{devname}")
        
        c.tcl(f"add tag .{devname} {devname}")
        
        c.tcl(f"add node .{devname}:device_desc/usage=text")
        
        c.tcl(f"put {devname}:device_desc {dev.description}")
        
        # c.tcl(f"add node .{devname}:grouping/usage=text")
        # c.tcl(f"put .{devname}:grouping {dev.grouping}")
        
        # for n in range(0,dev.num_channels):
        for channel_name in dev.channel_names:
            c.tcl(f"add node {devname}:{channel_name}/usage=signal")
        
        c.tcl(f"set def \TOP:{root}:{diagname}")
    c.tcl(f"set def \TOP:{root}")        

# %%
c.tcl(f"set def \TOP")   

c.tcl("add node .DIAGNOSTICS")
c.tcl("add tag .DIAGNOSTICS diagnostics")
c.tcl("set def .DIAGNOSTICS")

for diagname in devdict.keys():
    
    c.tcl(f'add node .{diagname}')
    
    c.tcl(f"add tag .{diagname}")
    c.tcl(f'set def .{diagname}')
    
    for dev in devdict.get(diagname):
       
        devname = dev.name_mds
        
        c.tcl(f"add node .{devname}")
        
        c.tcl(f"add tag .{devname} {devname}")
        
        c.tcl(f"add node .{devname}:device_desc/usage=text")
        
        c.tcl(f"put {devname}:device_desc {dev.description}")
        
        # c.tcl(f"add node .{devname}:grouping")
        # c.tcl(f"put .{devname}:grouping {dev.grouping}")
        
        # for n in range(1,dev.num_channels):
        #     newname = str(dev.channel_names[n])
        #     c.tcl(f"add node {devname}:{n:d}/usage=signal")
        
        c.tcl('set def \TOP:DIAGNOSTICS:{diagname}')
            
    c.tcl(f"set default \TOP:DIAGNOSTICS")
        
# c.tcl(f"set def \TOP")
# c.tcl("add node .ANALYSIS")
# c.tcl("add tag .ANALYSIS ANALYSIS")
# c.tcl("set def .ANALYSIS")

# for diagname in devdict.keys():

#     c.tcl(f'add node .{diagname}')
#     c.tcl(f"add tag .{diagname}")



c.tcl(f"write")
c.tcl(f"close")


