from ...device_classes.MasterTrigger import MasterTriggerConfig


times = MasterTriggerConfig(
    name_mds = "MASTER_TRIG",
    name_local = "MasterTrigger",
    diagnostic = 'MASTER_TRIG',
    description='Trigger delays and offset information for guns, lasers, etc',
    grouping = 'SETUP', 
    tag = 'TRG'
    
    
    )

HWdevices = [times]