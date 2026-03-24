from ...device_classes.base import setup_base

class magnetic_field(setup_base):
    
    def __init__(self,  name_mds, name_local, diagnostic='', grouping="SETUP", tag='', description=''):
        super().__init__(name_mds, name_local, diagnostic='', grouping="SETUP", tag='', description='')
    
        fields=[
            "I_coil",
            "otherstuff"]
    
        chd={}
        [chd.update({i:chstr}) for i,chstr in enumerate(fields)]
        self.field_names = chd
        
        self.grouping = "PHASMA"
        
    
    def write_dummy_local(self, destdir,shot=1,length=1):
        import pandas as pd
        
        testfid = f"{shot}_{self.name_mds}_test.txt"
        
        colnames = list(self.field_names.values())
        
        data = ['flub' for field in colnames]
        
        df = pd.DataFrame(data=data,index = [1],columns=colnames)
        
        df.to_csv(f"{destdir}/{testfid}",sep=',',header=colnames, index= False,float_format="%.3f")
        
     
mag_field = magnetic_field(
    
    name_mds = "MAGNETS",
    name_local = "Bfield",
    
    )


HWdevices = [mag_field]
