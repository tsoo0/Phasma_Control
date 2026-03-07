from .base import setup_base

class pressure_regulator(setup_base):
    
    def __init__(self,  name_mds, name_local, diagnostic, grouping, tag, description):
        super().__init__(name_mds, name_local, diagnostic, grouping, tag, description)
    
        fields=[
            "P_SET",
            "P_MEAS"]
    
        chd={}
        [chd.update({i:chstr}) for i,chstr in enumerate(fields)]
        self.field_names = chd
    
    def write_dummy_local(self, destdir,shot=1,length=1):
        import pandas as pd
        
        testfid = f"{shot}_{self.name_mds}_test.txt"
        
        colnames = list(self.field_names.values())
        
        data = ['flub' for field in colnames]
        
        df = pd.DataFrame(data=data,index = [1],columns=colnames)
        
        df.to_csv(f"{destdir}/{testfid}",sep=',',header=colnames, index= False,float_format="%.3f")
        
        # dat = [self.field_names.values for field in self.fields]
        
p_reg = pressure_regulator(
    name_mds = "PRESSURE",
    name_local = "idk",
    diagnostic = "PRESSURE",
    grouping = "ENG",
    tag = "PRS"
    
    )

HWdevices = [p_reg]