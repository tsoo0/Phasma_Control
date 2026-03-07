from ...device_classes.pulsedLIF import pulsedLIF,pulsedLIF_setup


plif = pulsedLIF(name_mds='PulsedLIF',
                 name_local='PulsedLIF',
                 nchannels=2,
                 diagnostic='PLIF',
                 grouping = "DATA",
                 tag = "PLIF",
                 description = "Pulsed LIF Data"
                 )

plif_meta = pulsedLIF_setup(name_mds='PulsedLIF_setup',
                           name_local='PulsedLIF_setup',
                           diagnostic='PLIF',
                           grouping='SETUP',
                           tag = 'ENG_PLIF',
                           description = "Pulsed LIF setup"
                           )


HWdevices=[plif,plif_meta]