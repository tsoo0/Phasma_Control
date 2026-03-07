from ...device_classes.acq2106 import acq2106

devtag = "TLP"
diagnostic = "TLP"

acq2106_372 = acq2106(name_mds='ACQ2106_372',
                 name_local='ACQ2106_372',
                 nchannels=16,
                 MToffset = 0,
                 channel_prefix = 'TLP1_',
                 description='Triple Langmuir Probe, 16-channel digitizer, acq2106 carrier with 2x acq482ELF-16-4V-H acquisition cards',
                 diagnostic = diagnostic,
                 tag = devtag,
                 grouping = "DATA"
    )

acq2106_373 = acq2106(name_mds='ACQ2106_373',
                 name_local='ACQ2106_373',
                 nchannels=16,
                 MToffset = 0,
                 channel_prefix = 'TLP2_',
                 description='Triple Langmuir Probe, 16-channel digitizer, acq2106 carrier with 2x acq482ELF-16-4V-H acquisition cards',
                 diagnostic = diagnostic,
                 tag = devtag,
                 grouping  = "DATA"
                 )

HWdevices = [acq2106_372, acq2106_373]