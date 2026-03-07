from ...device_classes.TimingSettings import TimeConfig


times = TimeConfig(
    name_mds = "MSTR_TRIG",
    name_local = "MasterTrigger",
    diagnostic = 'TIMING',
    description='Timing information for guns, lasers, etc',
    grouping = 'SETUP', 
    tag = 'TRG'
    
    
    )

HWdevices = [times]