from ...device_classes.pulsedLIF import pulsedLIF_data,pulsedLIF_spectra


plif = pulsedLIF_data(name_mds='PLIF_DATA',
                 name_local='PulsedLIF',
                 nchannels=3,
                 diagnostic='PLIF',
                 grouping = "DATA",
                 tag = "PLIF",
                 description = "Pulsed LIF signal, photodiode time series"
                 )

plif_meta = pulsedLIF_spectra(name_mds='PLIF_SPECTRA',
                           name_local='PulsedLIF_Spectrum',
                           nchannels = 2,
                           diagnostic='PLIF',
                           grouping='DATA',
                           tag = 'PLIF_SPECTRA',
                           description = "Pulsed LIF spectrum"
                           )


HWdevices=[plif,plif_meta]