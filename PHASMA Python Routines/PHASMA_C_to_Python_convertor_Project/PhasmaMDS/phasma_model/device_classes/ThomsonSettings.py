from .base import setup_base

class TSConfig(setup_base):
    
    def __init__(self,  name_mds, name_local, diagnostic, grouping, tag, description):
        super().__init__(name_mds, name_local, diagnostic, grouping, tag, description)
    
        fields=[
    	"Laser_Orientation",
    	"Type_of_Collection_Optics",
    	"Q1500_Frequency",
    	"Q1500_InstrinsicDelay",
    	"Q1500_Q_delay",
    	"Q1500_Circuit_Delay",
    	"Q1500_QSW_Source",
    	"Q1500_DDG_Delay",
    	"Q1500_Firing_Time",
    	"Q850_Frequency",
    	"Q850_InstrinsicDelay",
    	"Q850_Q_delay",
    	"Q850_Circuit_Delay",
    	"Q850_QSW_Source",
    	"Q850_DDG_Delay",
    	"Q850_Firing_Time",
    	"Andor_Gain",
    	"Andor_Pre_Amp",
    	"Andor_Exp_Time",
    	"Andor_DDGDelay",
    	"Andor_GateWidth",
    	"Andor_H_bin",
    	"Andor_V_bin",
    	"McP_Current_Wlength",
    	"McP_GrooveDen",
    	"McP_AngleDiff",
    	"McP_FocalLength"]
    
        chd={}
        [chd.update({i:chstr}) for i,chstr in enumerate(fields)]
        self.field_names = chd
                

    