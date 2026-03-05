# REPLACES EXISTING MODEL TREE! #
# Generate a new experimental model on a running mds server, !!overwriting the old one if it exists.!!
import os
print(os.getcwd())
# tree type devices,diagnostics,analysis
import itertools
import mdsthin as mds
from phasma_model.phasma_devices import devdict


# %%
treename = 'phasma2025'

c = mds.Connection('127.0.0.1:57800')

print(c.tcl(f'edit {treename}/new'))

root='DEVICES'
c.tcl("set def \TOP")
c.tcl(f"add node .{root}")
c.tcl(f"add tag .{root} ENG")
c.tcl(f"set def .{root}")

for diagname in devdict.keys():
    for dev in devdict.get(diagname).devices.HWdevices: 
        devname = dev.name_mds
        c.tcl(f"add node .{devname}")
        
        c.tcl(f"add tag .{devname} {devname}")
        
        c.tcl(f"add node .{devname}:device_desc/usage=text")
        
        c.tcl(f"put {devname}:device_desc {dev.description}")
        
        c.tcl(f"add node .{devname}:grouping/usage=text")
        c.tcl(f"put .{devname}:grouping {dev.grouping}")

        c.tcl(f"add node .{devname}:sample_rate/usage=text")
        c.tcl(f"put .{devname}:sample_rate {dev.sample_rate}")

        # for channel_name in dev.channel_names.values():
        #     c.tcl(f"add node {devname}:{channel_name}/usage=signal")
        #     c.tcl
            
        c.tcl(f"set def \TOP:{root}:{diagname}")
    c.tcl(f"set def \TOP:{root}")        

# %%
root="DIAGNOSTICS"
c.tcl("set def \TOP")   

c.tcl(f"add node .{root}")
c.tcl(f"add tag {root} diag")
c.tcl(f"set def .{root}")

for diagname in devdict.keys():
    
    c.tcl(f'add node .{diagname}')
    
    c.tcl(f"add tag .{diagname} {diagname[:3]}")
    c.tcl(f'set def .{diagname}')
    
    # diagdevs = devdict.get(diagname)
    # diagchannels = []
    # for dch in diagdevs.config.diagnostic:
    #     diagchannels.append(dch.channel_names.values())
    
    # diagchannels = list(itertools.chain.from_iterable(diagchannels))
    
    c.tcl("set default \TOP:DIAGNOSTICS.{diagname}")
    for n,chname in enumerate (devdict.get(diagname).config.diagnostic.values()):
        newname = f"{chname}"
        c.tcl(f"add node .{newname}/usage=signal")
        c.tcl(f"add tag .{newname} {diagname[0]}{n}")
    
    # c.tcl('set def \TOP:DIAGNOSTICS:{diagname}')
        
    c.tcl("set default \TOP:DIAGNOSTICS")
        

c.tcl("write")
c.tcl("close")


