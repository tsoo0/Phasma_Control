from . import devices
from . import diag_def

import os
fpath = os.path.dirname(os.path.realpath(__file__))
diag_name_mds = os.path.split(fpath)[1] # set the default mds identifier for the diagnostic as the current folder name

grouping = "DATA" #most members are data so it's easier to change the few that aren't to SETUP manual

__all__ = [
    'devices',
    'diag_def',
]