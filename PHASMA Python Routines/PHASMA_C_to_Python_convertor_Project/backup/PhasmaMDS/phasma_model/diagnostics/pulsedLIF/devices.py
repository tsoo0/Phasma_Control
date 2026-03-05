from ...device_classes.pulsedLIF import pulsedLIF


plif = pulsedLIF(name_mds='PulsedLIF',
                 name_local='PulsedLIF',
                 grouping='PLIF',
                 nchannels=2,
                 param1=1,
                 param2=2
                 )

HWdevices=[plif]