from .base import device_base


        

class pulsedLIF(device_base):
    def __init__(self, name_mds, name_local, nchannels,param1:float,param2:float, **kwargs):
        self.param1=param1
        self.param2=param2
        super().__init__(name_mds, name_local, nchannels, **kwargs)
        
