from ...device_classes.base import setup_base


diagname = "POSITIONS" # 12-character limit, case insensitive (will be converted to uppercase); 
                          # MDS name of diagnostic system that every device in this file is a member of

class positions(setup_base):
    
    def __init__(self,  name_mds, name_local, diagnostic='', grouping="PHASMA", tag='', description=''):
        super().__init__(name_mds, name_local, diagnostic='', grouping="PHASMA", tag='', description='')
    
        fields=[
            "POS_GUN1",
            "POS_GUN2", 
            "POS_BDOT",
            "POS_2PROBE",
            "POS_3PROBE", 
            "POS_RFEA",
            "PLIF_WAVELN",

            ]
    
        chd={}
        [chd.update({i:chstr}) for i,chstr in enumerate(fields)]
        self.field_names = chd
        
        self.grouping = "PHASMA"

pos = positions(
        name_mds = 'POSITIONS',
        name_local = 'ProbeandLIF_Positions',
        description='temporary home of motor position data',
        diagnostic = "position",
        tag = "POS"
)

                
HWdevices = [pos]