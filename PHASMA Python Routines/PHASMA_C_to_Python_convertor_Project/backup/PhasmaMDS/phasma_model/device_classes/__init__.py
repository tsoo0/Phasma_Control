import os
import importlib

fpath = os.path.dirname(os.path.realpath(__file__))
print(f"classlevel:{fpath}")

topfids = os.listdir(fpath)
# print(topfids)

devfids = [fid for fid in topfids if fid.isalnum()]
# print(devfids)


for fid in devfids:
    importlib.import_module(fid)
    
    
    
    # from . import fid

# __all__ = devfids    
    # importlib.import_module(f".{fid}",package=fid)
    # from fid import devices