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

phasmadiags = [d for d in list(devdict.keys()) if devdict.get(d).grouping == "PHASMA" ]

excludediag = ['PulsedLIF','new_device_template']
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
from functions import tcl_write_print
# %%


treename = 'phasma2025'
# treename = 'phasma_testo'
# c = mds.Connection('127.0.0.1:8000')
c = mds.Connection('127.0.0.1:57800')

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
        
        c.tcl(f"add tag .{diagname} {newdiag.diag_def.tag}")
        
        c.tcl(f"set default .{diagname}")
        
        # Create DATA node and and populate with diagnostic's data channels
        c.tcl("add node .DATA")
        c.tcl("set default .DATA")
        
        for n,chname in enumerate (newdiag.diag_def.diagnostic.values()): 
            newname = f"{chname}"
            c.tcl(f"add node .{newname}/usage=signal")
            c.tcl(f"add tag .{newname} {newdiag.diag_def.tag}{n}")
        
        c.tcl("set def .-")
    
        # Create node with metadata for each physical device
        c.tcl("add node .DEVICES")
        c.tcl("set def .DEVICES")
        
        HWdevs = devdict[diagname].diag_def.HWdevices
        
        for HW in HWdevs:
            
            tcl_write_print(c,f"add node .{HW.name_mds}")
            tcl_write_print(c,f"set def .{HW.name_mds}")  
        
            metastrs=list(filter(lambda a: not a.startswith("__") and a not in excludestr, dir(HW))) # strip out non-relevant fields

            for metastr in metastrs:
                param = getattr(HW,metastr)
                if type(param) == str:
                    print(param)
                    c.tcl(f"add node .{metastr}/usage=text")
                    
                    # if ' ' in param:

                    arg = f"put {metastr} \"'{param}'\"" # strings containing spaces need to be wrapped with
                                                              # "' to be as interpreted as literals
                        
                    # else:
                    #     arg = f"put .{metastr} {param}"
                    tcl_write_print(c,arg)

                
                elif type(param) == int or type(param) == float:
                    c.tcl(f"add node .{metastr}/usage=numeric")
                    tcl_write_print(c,f"put .{metastr} {param}")
                 
                    
            c.tcl(f"set def \TOP:{root}.{diagname}.DEVICES")

        c.tcl("set def .-")
        
        # add SETUP node, if applicable
       
        if ("setup" in dir(newdiag.diag_def)):
            c.tcl("add node .SETUP")
            c.tcl("set default .SETUP")
    
            field_names = newdiag.diag_def.setup.values()
        
            for n,fieldname in enumerate(field_names):
            
                
                c.tcl(f"add node .{fieldname}/usage=numeric")
                c.tcl(f"add tag .{fieldname} {fieldname}")
                # except:
                #     print(f"skipping {root}.{diagname}.SETUP.{fieldname}")
                #     continue
                    
            c.tcl(f"set def \TOP:{root}.{diagname}.SETUP")
            
            
            
        if ("position" in dir(newdiag.diag_def)):   # Create position subtree, if it's in diag_def
            
            c.tcl("add node .POSITION")
            c.tcl("set def .POSITION") 
           
            metastrs=list(filter(lambda a: not a.startswith("__") and a not in excludestr, newdiag.diag_def.position.values())) # strip out non-relevant fields
        
            for n,fieldname in enumerate(newdiag.diag_def.position.values()):
                
                
                try:
                   
                    tcl_write_print(c,f"add node .{fieldname}/usage=numeric")
                    c.tcl(f"add tag .{fieldname} {fieldname}")
                except:
                    print(f"skipping {root}.{diagname}.POSITION.{fieldname}")
                    continue
            
            c.tcl("set def .-")
    
root="PHASMA"
c.tcl("set def \TOP")
c.tcl(f"add node .{root}")
c.tcl(f"add tag .{root} PHASMA")


for diagname in phasmadiags:
    
    newdiag = devdict.get(diagname)
    
    c.tcl(f"set default \TOP:{root}")
    
    c.tcl(f'add node .{diagname}')
    
    c.tcl(f"add tag .{diagname} {newdiag.diag_def.tag}")
    
    c.tcl(f"set default .{diagname}")
    

      
    field_names = list(devdict.get(diagname).diag_def.diagnostic.values())

    for n,fieldname in enumerate(field_names):
    
        try:
            
            c.tcl(f"add node .{fieldname}/usage=numeric")
            c.tcl(f"add tag .{fieldname} {fieldname}")
        except:
            print(f"skipping {root}.{diagname}.SETUP.{fieldname}")
            continue
            

              


c.tcl("write")
c.tcl("close/confirm")


