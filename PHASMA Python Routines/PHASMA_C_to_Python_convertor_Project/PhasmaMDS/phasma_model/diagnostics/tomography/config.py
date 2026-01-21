import numpy as np

from .devices import acq2106_434,acq2106_433

# import devices

# 96-channel Tomographic array channel mapping
# acqbiasToff = 895e-6
acqbiasToff=860e-6

#channel mapping (3-angle config)
horzinds_a = np.arange(1,9)
horzinds_b = np.arange(9,17) # ch 9 to 16
horzinds_c = np.arange(17,25)
horzinds_d = np.arange(25,33) # ch 25 to 32
horzinds_e = np.arange(33,41)
horzinds_f = np.arange(41,49)# ch 41 to 48  

horzinds = [horzinds_a, horzinds_b, horzinds_c,horzinds_d]

vertinds_a = np.arange(1,9)
vertinds_b = np.arange(9,17) # ch 9 to 16
vertinds_c = np.arange(17,25)
vertinds_d = np.arange(25,33) # ch 25 to 32
vertinds_e = np.arange(33,41)
vertinds_f = np.arange(41,49)# ch 41 to 48  

vertinds = [vertinds_a, vertinds_b, vertinds_c, vertinds_d]

diaginds_a = np.arange(33,41)
diaginds_b = np.arange(41,49)
diaginds_c = np.arange(33,41)
diaginds_d = np.arange(41,49)

diaginds = [diaginds_a, diaginds_b, diaginds_c, diaginds_d]

diaginds = np.hstack((diaginds_a,
                      diaginds_b,
                      diaginds_c,
                      diaginds_d,
                      ))

channel_map = {
    acq2106_433:{'vertical':vertinds[:2], "horzinds":horzinds[:2], "diaginds":diaginds[:2]},
    acq2106_434:{'vertical':vertinds[2:], "horzinds":horzinds[2:], "diaginds":diaginds[2:]}
    }
    

class tomography: 
    def __init__(self, name, devices, channel_map):
        self.name = name
        self.devices = devices
        self.channel_map = channel_map
        
tomo = tomography('tomo96', [acq2106_434, acq2106_433], channel_map)
    
    
    
    
    