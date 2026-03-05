from ...device_classes.MSO5104 import MSO5104

# MSO5104_02 = MSO5104('MSO5104_02','MSO5104_02',
#                      nchannels = 4, 
#                      grouping = 'thomson',
#                      MToffset = 0,
#                      channel_prefix = '',
#                      description='Thomson Stuff',
#                      diagnostic = 'thomson'
#                      )
                     
MSO5104_02 = MSO5104('THOMSON','THOMSON',
                     nchannels = 4, 
                     grouping = 'thomson',
                     MToffset = 0,
                     channel_prefix = '',
                     description='Thomson Stuff',
                     diagnostic = 'thomson'
                     )
                     


HWdevices = [MSO5104_02]

# if __name__=='__main__':
#     from pathlib import Path
#     import os
#     ROOT_DIR = Path(__file__).resolve().parent.parent