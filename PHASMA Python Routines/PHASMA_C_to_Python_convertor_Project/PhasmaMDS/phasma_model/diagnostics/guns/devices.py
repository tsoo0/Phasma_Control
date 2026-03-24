from ...device_classes.base import setup_base

class plasma_gun_settings(setup_base):
    
    def __init__(self,  name_mds, name_local, diagnostic='', grouping="PHASMA", tag='', description=''):
        super().__init__(name_mds, name_local, diagnostic='', grouping="PHASMA", tag='', description='')
    
        fields=[
            "GUN1_V_BIAS",
            "GUN2_V_BIAS",
            "GUN1_V_ARC",
            "GUN2_V_ARC",
            "GUN1_I_BIAS",
            "GUN2_I_BIAS",
            "GUN1_I_ARC",
            "GUN2_I_ARC",
            ]
    
        chd={}
        [chd.update({i:chstr}) for i,chstr in enumerate(fields)]
        self.field_names = chd
        
        self.grouping = "PHASMA"
        
plasmaguns = plasma_gun_settings(
    name_mds = "GUNS",
    name_local = "Plasma_Gun_Settings",
    description = "Settings for both plasma guns (Voltage and current applied to guns for arc and bias pulses)",
    tag = "GUN",
    ) 

        
HWdevices = [plasmaguns]

