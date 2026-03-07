from ...device_classes.TimingSettings import TimeConfig


times = TimeConfig(
    name_mds = "MasterTrigger",
    name_local = "MasterTrigger",
    diagnostic = 'TIMING',
    description='Timing information for guns, lasers, etc',
    grouping = 'SETUP', 
    tag = 'TRG'
    
    
    )

HWdevices = [times]