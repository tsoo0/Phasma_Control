from ...device_classes.MSO5104 import MSO5104
from ...device_classes.VX4244 import VX4244

MSO5104_01 = MSO5104(name_mds = 'HousekeepingHighRes',
                     name_local = 'housekeepingHighRes',
                     nchannels = 4, 
                     grouping = 'housekeeping',
                     MToffset = 0,
                     channel_prefix = 'MSO5104_01_',
                     description='Housekeeping data, bias currents for both plasma guns, [kA]',
                     diagnostic = 'housekeeping'
                     )

VX4244 = VX4244(name_mds = 'Housekeeping',
                name_local='Housekeeping',
                     nchannels = 8, 
                     grouping = 'housekeeping',
                     MToffset = 0,
                     channel_prefix = 'VX4244_',
                     description='Slow Houskeeping, VX4244',
                     diagnostic = 'housekeeping'
                     )     
                
HWdevices = [MSO5104_01, VX4244]

# if __name__=='__main__':
#     from pathlib import Path
#     import os
#     ROOT_DIR = Path(__file__).resolve().parent.parent