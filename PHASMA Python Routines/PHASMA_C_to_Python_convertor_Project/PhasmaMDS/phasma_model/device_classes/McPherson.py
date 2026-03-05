from .base import device_base

class McPherson(device_base):
    
    def __init__(self,name_mds, name_local, diagnostic):
        self.name_mds = name_mds
        self.name_local = name_local
        self.diagnostic = diagnostic
        self.channel_names = ['Wavlength','AndorV', 'AndorH']
        
