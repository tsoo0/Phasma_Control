from .base import device_base, setup_base
        
class pulsedLIF_data(device_base):
    pass

class pulsedLIF_spectra(device_base):

    pass
        

# class pulsedLIF_spectra(setup_base):
    
#     def __init__(self,  name_mds, name_local, diagnostic, grouping, tag='', description=''):
#         super().__init__(name_mds, name_local, diagnostic, grouping, tag='',description='')
    
#         fields=[ # 75-element power, wavelength arrays 
#     	"PLIF_POWER",
#     	"PLIF_WAVELEN",
#         ]
    
#         chd={}
#         [chd.update({i:chstr}) for i,chstr in enumerate(fields)]
#         self.field_names = chd
        
        
        