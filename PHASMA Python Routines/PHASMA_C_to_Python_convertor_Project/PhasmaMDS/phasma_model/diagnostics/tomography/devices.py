from ...device_classes.acq2106 import acq2106

acq2106_433 = acq2106(name_mds='acq2106_433',
                 name_local='acq2106_433',
                 grouping = 'photodiodes',
                 nchannels=48,
                 MToffset = 0,
                 channel_prefix = 'tomo_A_',
                 description='48channel tomo DAQ A',
                 diagnostic = 'tomography'
                 )

acq2106_434 = acq2106(name_mds='acq2106_434',
                 name_local='acq2106_434',
                 grouping = 'photodiodes',
                 nchannels=48,
                 MToffset = 0,
                 channel_prefix = 'tomo_B_',
                 description='48channel tomo DAQ B',
                 diagnostic = 'tomography'
                 )
                 
HWdevices = [acq2106_433,acq2106_434]