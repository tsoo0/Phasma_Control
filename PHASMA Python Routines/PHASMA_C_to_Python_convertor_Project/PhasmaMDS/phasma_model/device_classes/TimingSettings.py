from .base import setup_base

class TimeConfig(setup_base):
    
    def __init__(self,  name_mds, name_local, diagnostic, grouping, tag, description=''):
        super().__init__( name_mds, name_local, diagnostic, grouping, tag, description='')
    
        fields=["Total_Cycle_Time",
                "Laser_Warmup",
                "Laser_Warmup_Pulse_Length",
                "Laser_Warmup_High",
                "Laser_Warmup_Low",
                "Gun_Charging",
                "Gun_Charging_Pulse_Length",
                "Gun_Charging_High",
                "Gun_Charging_Low",
                "Astrella_Synch",
                "Astrella_Synch_Pulse_Length",
                "Astrella_Synch_High",
                "Astrella_Synch_Low",
                "iCCD_Shutter",
                "iCCD_Shutter_Pulse_Length",
                "iCCD_Shutter_High",
                "iCCD_Shutter_Low",
                "Gas_Puff",
                "Gas_Puff_Pulse_Length",
                "Gas_Puff_High",
                "Gas_Puff_Low",
                "Arc",
                "Arc_Pulse_Length",
                "Arc_High",
                "Arc_Low",
                "Bias1",
                "Bias1_Pulse_Length",
                "Bias1_High",
                "Bias1_Low",
                "Bias2",
                "Bias2_Pulse_Length",
                "Bias2_High",
                "Bias2_Low",
                "PerpTS",
                "PerpTS_Pulse_Length",
                "PerpTS_High",
                "PerpTS_Low",
                "ParaTS",
                "ParaTS_Pulse_Length",
                "ParaTS_High",
                "ParaTS_Low",
                "PLIF",
                "PLIF_Pulse_Length",
                "PLIF_High",
                "PLIF_Low",
                "Other",
                "Other_Pulse_Length",
                "Other_High",
                "Other_Low"
                ]
    
        chd={}
        [chd.update({i:chstr}) for i,chstr in enumerate(fields)]
        self.field_names = chd
                

    