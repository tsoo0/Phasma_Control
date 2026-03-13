from ...device_classes.MSO5104 import MSO5104
from ...device_classes.ThomsonSettings import TSConfig
from ...device_classes.Andor import Andor

devtag = "TS"
         
MSO5104_02 = MSO5104('TS_PHOTO',
                     'Thomson_Photodiode',
                     nchannels = 2, 
                     grouping = 'DATA',
                     MToffset = 0,
                     channel_prefix = 'TS_',
                     description='Thomson Scattering Rigol Scope MSO5104 2',
                     diagnostic = 'Thomson',
                     tag = devtag
                     )   

Andor = Andor(
        name_mds='TS_ANDOR',
        name_local='Thomson_Andor',
        diagnostic='Thomson',
        channel_prefix = "ANDOR_",
        description = "Thomson Spectra data from Andor",
        nchannels = 3,
        grouping = "DATA",
        tag = devtag
    )

TS_setup = TSConfig(name_mds='TS_CONFIG',
                    name_local='Thomson_Settings',
                    diagnostic='Thomson',
                    grouping = "SETUP",
                    tag = "ENG_TS",
                    description = "Thomson Scattering setup information"
    )     

HWdevices = [MSO5104_02, Andor, TS_setup]