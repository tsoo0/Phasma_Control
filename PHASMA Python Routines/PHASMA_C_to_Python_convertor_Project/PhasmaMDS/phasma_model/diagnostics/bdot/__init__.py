from . import devices
from . import channel_map

import os
fpath = os.path.dirname(os.path.realpath(__file__))
diag_name_mds = os.path.split(fpath)[1] # set the default mds identifier for the diagnostic as the current folder name

__all__ = [
    'devices',
    'channel_map',
]