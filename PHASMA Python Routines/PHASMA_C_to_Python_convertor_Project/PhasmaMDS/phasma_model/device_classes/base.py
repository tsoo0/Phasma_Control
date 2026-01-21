# -*- coding: utf-8 -*-
"""
Created on Thu Dec 18 23:50:24 2025

@author: tjroo
"""

"""
generic base class for all time-domain DAQ devices

"""
import numpy as np

class device_base:
    
    def __init__(self, name_mds, name_local, nchannels, channel_prefix ='', 
                 grouping='None', MToffset=0, description='', diagnostic = '',thingy = ''): 
        self.name_mds = name_mds     # mds node name
        self.name_local = name_local  # device name in local file system
        self.group = grouping        # class of measurement (magnetics, optical, etc)
        self.num_channels=nchannels   # number of data channels, not including time
        self.MToffset = MToffset # time offset relative to master trigger in seconds (float)
        self.channel_prefix = channel_prefix
        self.channel_names = [f"{channel_prefix}{i:d}" for i in range(1,nchannels+1)]
        self.channel_names.insert(0,'Time')
        self.description = description
        self.diagnostic = diagnostic
        
    def write_dummy_local(self, destdir,shot=1,length=1000):
        
        import pandas as pd
        dummydata = np.linspace(0,1,length)[:,np.newaxis]
        df = pd.DataFrame(data=np.tile(dummydata,(self.num_channels+1)))
        testfid = f"{shot}_{self.name_mds}_test.txt"
        df.to_csv(f"{destdir}/{testfid}",sep='\t',header=self.channel_names, index= False)
        return testfid
    
if __name__=='__main__':

    import os
    import pandas as pd
    testdir = os.path.dirname(os.path.realpath(__file__))

    testo = device_base('whochares','gumby',12,'flub')
    
    testfid = testo.write_dummy_local(testdir,  shot=1)
    
    recall = pd.read_csv(testdir + '/' + testfid, delimiter = '\t')
    
    