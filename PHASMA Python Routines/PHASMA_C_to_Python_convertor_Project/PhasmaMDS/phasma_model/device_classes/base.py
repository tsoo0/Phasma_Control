"""
generic base classes for all time-domain DAQ devices

"""

class device_base:
    
    def __init__(self, name_mds, name_local, nchannels : int, channel_prefix ='', 
                 grouping='DATA', MToffset=0, description='', diagnostic = '',sample_rate=0, tag=''): 
        self.name_mds = name_mds     # mds node name
        self.name_local = name_local  # device name in local file system
        self.grouping = 'DATA'        # Users of this class should all describe experimental data, with metadata stored in seperate SETUP classses
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
        
    
class setup_base:
    
    def __init__(self,  name_mds, name_local, diagnostic, grouping="SETUP", tag='', description=''):

        self.name_mds=name_mds
        self.name_local=name_local
        self.diagnostic = diagnostic # diagnostic for this setup describes
        self.grouping = grouping #  SETUP class is meant to describe diagnostic metadata, eg position for motorized probes
        self.description = description
        self.tag = tag
        
