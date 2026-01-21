from ...device_classes.MSO5104 import MSO5104

MSO5104_01 = MSO5104('MSO5104_01','MSO5104_01',
                     nchannels = 5, 
                     grouping = 'housekeeping',
                     MToffset = 0,
                     channel_prefix = 'MSO5104_01_',
                     description='Gun current I(t), [kA]',
                     diagnostic = 'housekeeping'
                     )
                     
HWdevices = [MSO5104_01]

# if __name__=='__main__':
#     from pathlib import Path
#     import os
#     ROOT_DIR = Path(__file__).resolve().parent.parent