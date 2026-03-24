from ...device_classes.MasterTrigger import MasterTriggerConfig


times = MasterTriggerConfig(
    name_mds = "MasterTrig",
    name_local = "MasterTrigger",
    diagnostic = 'MasterTrig',
    description='Trigger delays and offset information for guns, lasers, etc',
    grouping = 'PHASMA', 
    tag = 'TRG'
    
    
    )

HWdevices = [times]