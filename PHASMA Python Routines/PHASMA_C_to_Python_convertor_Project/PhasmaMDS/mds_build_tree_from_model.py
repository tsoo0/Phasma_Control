# REPLACES EXISTING MODEL TREE! #
# Generate a new experimental model on a running mds server


import sys


import os
from pathlib import Path
ROOT_DIR = Path(__file__).parent
os.chdir(ROOT_DIR)

print(os.getcwd())
# tree type devices,diagnostics,analysis

sys.path.append(r'C:\PHASMA 2025 DAQ\PHASMA Python Routines\PHASMA_C_to_Python_convertor_Project\PhasmaMDS\Python\Lib\site-packages')


from phasma_model.phasma_devices import devdict

excludestr = ['name_mds', 'num_channels', 'diagnostic', 'channel_prefix', 'channel_names','tag','write_dummy_local']

"""
Builds tree of  type

\TOP:
    DIAGNOSTICS                     # List of diagnostic systems
        HOUSEKEEPING                # Diagnostic node containing data and metadata
            DATA
                channel 1 name          # named arrays of experimental data
                channel 2 name, etc            

            SETUP                   # node containing configuration data (timing, wavelength settings, etc)
            
            DEVICES
            
            POSITION                # physical position reported by motor mount
            
        MAGNETICS...    

    PHASMA                          # machine state; guide field, gun pressure, etc 
        MAGNETS
            field coil currents, B field data
        PRESSURE
            gun pressure setting, gas species
    
"""


import mdsthin as mds
# %%
# treename = 'phasma2025'
treename = 'phasma_testo'
c = mds.Connection('127.0.0.1:57800')
# %%


print(c.tcl(f'edit {treename}/new'))
        
root="DIAGNOSTICS"
c.tcl("set def \TOP")
c.tcl(f"add node .{root}")
c.tcl(f"add tag .{root} DIAG")

for diagname in devdict.keys():
    
    newdiag = devdict.get(diagname)
    
    c.tcl(f"set default \TOP:{root}")
    
    c.tcl(f'add node .{diagname}')
    
    c.tcl(f"add tag .{diagname} {newdiag.channel_map.tag}")
    
    c.tcl(f"set default .{diagname}")
    
    # Create data node and and populate with diagnostic channels
    c.tcl("add node .DATA")
    c.tcl("set default .DATA")
    
    for n,chname in enumerate (newdiag.channel_map.diagnostic.values()): 
        newname = f"{chname}"
        c.tcl(f"add node .{newname}/usage=signal")
        c.tcl(f"add tag .{newname} {newdiag.channel_map.tag}{n}")
    
    c.tcl("set def .-")
    
    # Create node with metadata for each physical device
    c.tcl("add node .DEVICES")
    c.tcl("set def .DEVICES")
    
    HWdevs = devdict[diagname].channel_map.HWdevices
    
    for HW in HWdevs:
    
        c.tcl(f"add node .{HW.name_mds}")
        c.tcl(f"set def .{HW.name_mds}")    
    
        metastrs=list(filter(lambda a: not a.startswith("__") and a not in excludestr, dir(HW))) # strip out non-relevant fields
        
        for metastr in metastrs:
            try:
                c.tcl(f"add node {metastr}/usage=text")
                param = getattr(HW,metastr)
                c.tcl(f"put {metastr} {param}")
            except:
                print(f"skipping {root}.{diagname}.DEVICES.{HW.name_mds}.{metastr}, value = {getattr(HW,metastr)}")
                continue
            
        c.tcl("set def .-")
        
    c.tcl("set def .-")
    
    
    # Position data subtree 
    
    c.tcl("add node .POSITION")
    c.tcl("set def .POSITION")  
    
    
    c.tcl("set def .-")
    
    # add SETUP node, if applicable
   
    if ("setup" in dir(newdiag.channel_map)):
        c.tcl("add node .SETUP")
        c.tcl("set default .SETUP")

        for n,fieldname in enumerate(newdiag.channel_map.setup.values()):

            try:
                c.tcl(f"add node .{fieldname}/usage=text")
                c.tcl(f"add tag .{fieldname} {fieldname}")
            except:
                print(f"skipping {root}.{diagname}.SETUP.{fieldname}")
                continue
                
        c.tcl("set def .-")
    
root="PHASMA"
c.tcl("set def \TOP")
c.tcl(f"add node .{root}")
c.tcl(f"add tag .{root} PHASMA")

c.tcl("set default .PHASMA")

c.tcl("add node .MAGNETS")
c.tcl("add node .PRESSURE")
c.tcl("add node .POSITION")
# c.tcl("add node .")    


c.tcl("write")
c.tcl("close")


