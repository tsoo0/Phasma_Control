import mdsthin as mds

# import diagnostics
from .diagnostics import *

devs = [magnetics, housekeeping, tomography]
devs = [dev.devices.HWdevices for dev in devs]

devdict = {}
for dev in devs:
    devdict.update({dev[0].diagnostic:dev})