from ...device_classes.acq2106 import acq2106

acq2106_433 = acq2106(name_mds='ACQ2106_433',
                 name_local='ACQ2106_433',
                 grouping = 'DATA',
                 nchannels=48,
                 MToffset = 0,
                 channel_prefix = 'tomo_A_',
                 description='96-channel Optical Tomography, Diode Array A, ACQ2106_433, 192.168.1.254',
                 diagnostic = 'tomography',
                 tag = "TOMO"
                 )

acq2106_434 = acq2106(name_mds='ACQ2106_434',
                 name_local='ACQ2106_434',
                 grouping = 'DATA',
                 nchannels=48,
                 MToffset = 0,
                 channel_prefix = 'tomo_B_',
                 description='96-channel Optical Tomography, Diode Array B, ACQ2106_434, 192.168.1.226',
                 diagnostic = 'tomography',
                 tag = "TOMO"
                 )
                 
HWdevices = [acq2106_433,acq2106_434]