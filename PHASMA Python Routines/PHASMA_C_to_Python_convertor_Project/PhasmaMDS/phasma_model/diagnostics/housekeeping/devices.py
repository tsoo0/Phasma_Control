from ...device_classes.MSO5104 import MSO5104
from ...device_classes.VX4244 import VX4244

diagname = "HOUSEKEEPING"
devtag = "HOUSE"
devgroup = "DATA"

MSO5104_01 = MSO5104(name_mds = 'HOUSEHIGHRES',
                     name_local = 'HousekeepingHighRes',
                     nchannels = 4, 
                     grouping = devgroup,
                     MToffset = 0,
                     channel_prefix = 'MSO5104_01_',
                     description='Housekeeping data, fast, bias currents for both plasma guns, MSO5104',
                     diagnostic = diagname,
                     tag = devtag
                     )

VX4244 = VX4244(name_mds = 'HOUSE',  # node name "HOUSE_VX4244" breaks the tree for some unknowable reason
                name_local='HousekeepingVXI',
                     nchannels = 7, 
                     grouping = devgroup,
                     MToffset = 0,
                     channel_prefix = 'VX4244_',
                     description='Slow Houskeeping, VX4244',
                     diagnostic = diagname,
                     tag = devtag
                     
                     )  

MSO5104_PD1 = MSO5104('MSO5104_PD1',
                      'MSO5104_PD1',
                     nchannels = 4, 
                     grouping = devgroup,
                     MToffset = 0,
                     channel_prefix = 'PD1_',
                     description='16-channel fast photodiodes, channels 1-4 (Horizontal 1-4)',
                     diagnostic = diagname,
                     tag = devtag
                     )

MSO5104_PD2 = MSO5104('MSO5104_PD2','MSO5104_PD2',
                     nchannels = 4, 
                     grouping = devgroup,
                     MToffset = 0,
                     channel_prefix = 'PD2_',
                     description='16-channel fast photodiodes, channels 5-8 (Horizontal 5-8)',
                     diagnostic = diagname,
                     tag = devtag
                     )                   

MSO5104_PD3 = MSO5104('MSO5104_PD3','MSO5104_PD3',
                     nchannels = 4, 
                     grouping = devgroup,
                     MToffset = 0,
                     channel_prefix = 'PD3_',
                     description='16-channel fast photodiodes, channels 9-12 (Vertical 1-4)',
                     diagnostic = diagname,
                     tag = devtag
                     )

MSO5104_PD4 = MSO5104('MSO5104_PD4','MSO5104_PD4',
                     nchannels = 4, 
                     grouping = devgroup,
                     MToffset = 0,
                     channel_prefix = 'PD4_',
                     description='16-channel fast photodiodes, channels 13-16 (Vertical 5-8)',
                     diagnostic = diagname,
                     tag = devtag
                     )
   
                
HWdevices = [MSO5104_01, VX4244, MSO5104_PD1, MSO5104_PD2, MSO5104_PD3, MSO5104_PD4]