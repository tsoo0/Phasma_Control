from ...device_classes.acq2106 import acq2106

acq2106_433 = acq2106(name_mds='acq2106_433',
                 name_local='acq2106_433',
                 grouping = 'DATA',
                 nchannels=48,
                 MToffset = 0,
                 channel_prefix = 'tomo_A_',
                 description='48channel tomo DAQ A',
                 diagnostic = 'tomography',
                 tag = "TOMO"
                 )

acq2106_434 = acq2106(name_mds='acq2106_434',
                 name_local='acq2106_434',
                 grouping = 'DATA',
                 nchannels=48,
                 MToffset = 0,
                 channel_prefix = 'tomo_B_',
                 description='48channel tomo DAQ B',
                 diagnostic = 'tomography',
                 tag = "TOMO"
                 
                 )
                 
HWdevices = [acq2106_433,acq2106_434]