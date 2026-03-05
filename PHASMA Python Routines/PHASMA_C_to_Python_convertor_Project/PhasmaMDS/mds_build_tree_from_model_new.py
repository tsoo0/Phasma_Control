# REPLACES EXISTING MODEL TREE! #
# Generate a new experimental model on a running mds server
import os
print(os.getcwd())
# tree type devices,diagnostics,analysis
import mdsthin as mds
from phasma_model.phasma_devices import devdict

excludestr = ['name_mds', 'name_local', 'num_channels', 'channel_prefix', 'channel_names','tag','write_dummy_local']

# %%
treename = 'phasma2025'

c = mds.Connection('127.0.0.1:8000')

print(c.tcl(f'edit {treename}/new'))

root='SETUP'
c.tcl("set def \TOP")
c.tcl(f"add node .{root}")
c.tcl(f"add tag .{root} ENG")
c.tcl(f"set def .{root}")

for diagname in devdict.keys():
    for dev in devdict.get(diagname).devices.HWdevices: 
        devname = dev.name_mds
        c.tcl(f"add node .{devname}")
        
        c.tcl(f"add tag .{devname} SETUP_{dev.tag}")
        
        metastrs=list(filter(lambda a: not a.startswith("__") and a not in excludestr, dir(dev)))
        
        for metastr in metastrs:
            try:
                c.tcl(f"add node .{devname}.{metastr}/usage=text")
                # c.tcl(f"put .{devname}:{metastr} {dev.metastr}")
            except:
                print(f"skipping {root}.{devname}.{metastr}")
                continue
        
        c.tcl(f"set def \TOP:{root}:{diagname}")
    c.tcl(f"set def \TOP:{root}")        

# %%
root="DIAGNOSTICS"
c.tcl("set def \TOP")
c.tcl(f"add node .{root}")
c.tcl(f"add tag .{root} DATA")

for diagname in devdict.keys():
    c.tcl(f"set default \TOP:{root}")
    c.tcl(f'add node .{diagname}')
    
    c.tcl(f"add tag .{diagname} {diagname[:3]}")
    
    c.tcl(f"set default .{diagname}")
    
    tag = devdict.get(diagname).config.diagnostic.
    
    for n,chname in enumerate (devdict.get(diagname).config.diagnostic.values()):
        newname = f"{chname}"
        c.tcl(f"add node .{newname}/usage=signal")
        c.tcl(f"add tag .{newname} {diagname[:3]}{n}")
    
        
    
        

c.tcl("write")
c.tcl("close")


