from ...device_classes.MSO5104 import MSO5104

diagnostic = "DIODSE16CH"

MSO5104_PD1 = MSO5104('MSO5104_PD1','MSO5104_PD1',
                     nchannels = 4, 
                     grouping = 'Optical',
                     MToffset = 0,
                     channel_prefix = 'PD1_',
                     description='16-channel fast photodiodes, channels 1-4',
                     diagnostic = diagnostic
                     )

MSO5104_PD2 = MSO5104('MSO5104_PD2','MSO5104_PD2',
                     nchannels = 4, 
                     grouping = 'Optical',
                     MToffset = 0,
                     channel_prefix = 'PD2_',
                     description='16-channel fast photodiodes, channels 5-8',
                     diagnostic = diagnostic
                     )                   

MSO5104_PD3 = MSO5104('MSO5104_PD3','MSO5104_PD3',
                     nchannels = 4, 
                     grouping = 'Optical',
                     MToffset = 0,
                     channel_prefix = 'PD3_',
                     description='16-channel fast photodiodes, channels 9-12',
                     diagnostic = diagnostic
                     )

MSO5104_PD4 = MSO5104('MSO5104_PD4','MSO5104_PD4',
                     nchannels = 4, 
                     grouping = 'Optical',
                     MToffset = 0,
                     channel_prefix = 'PD4_',
                     description='16-channel fast photodiodes, channels 13-16',
                     diagnostic = diagnostic
                     )

HWdevices = [MSO5104_PD1, MSO5104_PD2, MSO5104_PD3, MSO5104_PD4]

# if __name__=='__main__':
#     from pathlib import Path
#     import os
#     ROOT_DIR = Path(__file__).resolve().parent.parent