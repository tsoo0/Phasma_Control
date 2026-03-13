from .base import setup_base


class TSConfig(setup_base):
    
    def __init__(self,  name_mds, name_local, diagnostic, grouping, tag, description='Thomson'):
        super().__init__(name_mds, name_local, diagnostic, grouping, tag, description='')
    
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
    
    def write_dummy_local(self, destdir,shot=1,length=1):
        import pandas as pd
        
        import random
        
        testfid = f"{shot}_{self.name_mds}_test.txt"
        
        colnames = list(self.field_names.values())
        
        data = {}
        for field in self.field_names.values():
            data.update({field:random.randint(0,10000)})
        
        # data = ['flub' for field in colnames]
        
        # df = pd.DataFrame(data=data,columns=colnames)
        df = pd.DataFrame.from_dict(data)
        
        df.to_csv(f"{destdir}/{testfid}",sep=',',header=colnames, index= False,float_format="%.3f")
        
        # dat = [self.field_names.values for field in self.fields]
