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

excludediag = ['PulsedLIF']
excludestr = ['name_mds', 'num_channels', 'diagnostic', 'channel_prefix', 'channel_names','tag','write_dummy_local']

"""
Creates an MDS model tree with the format:

\TOP:
    DIAGNOSTICS                     # List of diagnostic systems
        HOUSEKEEPING                # Diagnostic node containing data and metadata
            DATA
                channel 1 name          # named arrays of experimental data
                channel 2 name, etc            

            
            DEVICES                 # subtree with information on all instruments that compose the diagnostic. Sample rate,    
                                      bit depth, name in local file system, an informative description, other engineering
                                      minutiae
            
            POSITION                # physical position(s) reported by motor mount, if applicable. Context dependent.
            
            SETUP                   # subtree containing diagnostic configuration data (laser timing, wavelength settings, etc)
                                      if applicable. This subtree is intended to contain information necessary for data
                                      analysis; stuff of more general scientific relevance than what is stored in DEVICES
            
            
    PHASMA                          # machine state; guide field, gun pressure, etc 
        MAGNETS
            field coil currents, B field data including B(x) reported by labwindows
        GUNS
            Pressure going to plasma guns reported by gas regulator; gas species
    
"""

import mdsthin as mds

treename = 'phasma2025'
# treename = 'phasma_testo'
# c = mds.Connection('127.0.0.1:8000')
c = mds.Connection('127.0.0.1:57800')

# %%

print(c.tcl(f'edit {treename}/new'))
        
root="DIAGNOSTICS"
c.tcl("set def \TOP")
c.tcl(f"add node .{root}")
c.tcl(f"add tag .{root} DIAG")

for diagname in devdict.keys():
    
    if diagname not in excludediag:
        newdiag = devdict.get(diagname)
        
        c.tcl(f"set default \TOP:{root}")
        
        c.tcl(f'add node .{diagname}')
        
        c.tcl(f"add tag .{diagname} {newdiag.channel_map.tag}")
        
        c.tcl(f"set default .{diagname}")
        
        # Create DATA node and and populate with diagnostic's data channels
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
            
            print(c.tcl(f"add node .{HW.name_mds}"))
            print(c.tcl(f"set def .{HW.name_mds}") )   
        
            metastrs=list(filter(lambda a: not a.startswith("__") and a not in excludestr, dir(HW))) # strip out non-relevant fields

            for metastr in metastrs:
                param = getattr(HW,metastr)
                if type(param) == str:
                
                    c.tcl(f"add node .{metastr}/usage=text")
                    
                elif type(param) == int or type(param) == float:
                    c.tcl(f"add node .{metastr}/usage=numeric")
                    
                print(c.tcl(f"put .{metastr} {param}"))   
                    
            c.tcl(f"set def \TOP:{root}.{diagname}.DEVICES")

        c.tcl("set def .-")
        
        # add SETUP node, if applicable
       
        if ("setup" in dir(newdiag.channel_map)):
            c.tcl("add node .SETUP")
            c.tcl("set default .SETUP")
    
            field_names = newdiag.channel_map.setup.values()
        
            for n,fieldname in enumerate(field_names):
            
                
                c.tcl(f"add node .{fieldname}/usage=numeric")
                c.tcl(f"add tag .{fieldname} {fieldname}")
                # except:
                #     print(f"skipping {root}.{diagname}.SETUP.{fieldname}")
                #     continue
                    
            c.tcl(f"set def \TOP:{root}.{diagname}.SETUP")
            
            
            
        if ("position" in dir(newdiag.channel_map)):   # Create position subtree, if it's in channel_map
            
            c.tcl("add node .POSITION")
            c.tcl("set def .POSITION") 
           
            metastrs=list(filter(lambda a: not a.startswith("__") and a not in excludestr, newdiag.channel_map.position.values())) # strip out non-relevant fields
        
            for n,fieldname in enumerate(newdiag.channel_map.position.values()):
                
                
                try:
                    print(c.tcl(f"add node .{fieldname}/usage=numeric"))
                    c.tcl(f"add tag .{fieldname} {fieldname}")
                except:
                    print(f"skipping {root}.{diagname}.POSITION.{fieldname}")
                    continue
            
            c.tcl("set def .-")
    
root="PHASMA"
c.tcl("set def \TOP")
c.tcl(f"add node .{root}")
c.tcl(f"add tag .{root} PHASMA")

c.tcl("set default .PHASMA")

c.tcl("add node .MAGNETS")
c.tcl("add node .PRESSURE")
  


c.tcl("write")
c.tcl("close")


