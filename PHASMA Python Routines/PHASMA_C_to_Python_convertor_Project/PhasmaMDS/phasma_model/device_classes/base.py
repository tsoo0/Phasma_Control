"""
generic base class for all time-domain DAQ devices

"""
import numpy as np

class device_base:
    
    def __init__(self, name_mds, name_local, nchannels : int, channel_prefix ='', 
                 grouping='DATA', MToffset=0, description='', diagnostic = '',sample_rate=0, tag=''): 
        self.name_mds = name_mds     # mds node name
        self.name_local = name_local  # device name in local file system
        self.grouping = 'DATA'        # Users of this class should all be experimental data, not setup information
        self.num_channels=nchannels   # number of data channels, not including time
        self.MToffset = MToffset # time offset relative to master trigger in seconds (float)
        self.channel_prefix = channel_prefix # used to build default channel names in device subtree
        ch = {0:'Time'}
        for i in range(1,nchannels+1): ch.update({i:f"{channel_prefix}{i:d}"})
        self.channel_names = ch
        self.description = description # brief description of what this class contains
        self.diagnostic = diagnostic
        self.sample_rate = sample_rate
        self.tag = tag
        
    def write_dummy_local(self, destdir,shot=1,length=1000):
        
        import pandas as pd
        import random
        dummydata = np.array([random.randrange(-1024,1024,1)/1024 for n in range(len(self.channel_names))])[:,np.newaxis]
        df = pd.DataFrame(data=np.tile(dummydata,(len(self.channel_names))))

        testfid = f"{shot}_{self.name_mds}_test.txt"
        
        colnames = list(self.channel_names.values())
        
        df.to_csv(f"{destdir}/{testfid}",sep=',',header=colnames, index= False,float_format="%.3f")
        return testfid
    
    
class setup_base:
    
    def __init__(self,  name_mds, name_local, diagnostic, grouping="SETUP", tag='', description=''):

        self.name_mds=name_mds
        self.name_local=name_local
        self.diagnostic = diagnostic # diagnostic for this setup describes
        self.grouping = "SETUP" # Users of this class should all be setup information
        self.description = description
        self.tag = tag
        
    def write_dummy_local(self, destdir,shot=1,length=2):
        import pandas as pd
        import random

        dummydata = [random.randrange(0,1024,1)/1024 for n in range(len(self.field_names))]
        colnames = list(self.field_names.values())
        data={}
        [data.update({colnames[n]:dummydata[n]}) for n in range(len(self.field_names))]
        
        df = pd.DataFrame(data=data,index = [1],columns=colnames)

        testfid = f"{shot}_{self.name_mds}_test.txt"
        
        df.to_csv(f"{destdir}/{testfid}",sep=',',index= False,float_format="%.3f")
        return testfid
    
if __name__=='__main__':
    import pandas as pd
    import os
    
    testdir = os.path.dirname(os.path.realpath(__file__))

    testo = device_base('whochares','gumby',12,'flub')
    
    testfid = testo.write_dummy_local(testdir,  shot=1)
    
    recall = pd.read_csv(testdir + '/' + testfid, delimiter = ',',header=0)
    
    