import os
# import importlib

fpath = os.path.dirname(os.path.realpath(__file__))
print(fpath)


topfids = os.listdir(fpath)
print(topfids)
# os.chdir(fpath)
devfids = [fid for fid in topfids if fid.isalnum()]
print(devfids)
# for fid in devfids:
    # from . import fid

# __all__ = devfids    
    # importlib.import_module(f".{fid}",package=fid)
    # from fid import devices

from . import tomography
from . import housekeeping
from . import magnetics
from . import thomson

__all__ = [
    "devfids",
    "tomography",
    "housekeeping",
    "magnetics",
    "thomson",
    ]

# devs = 