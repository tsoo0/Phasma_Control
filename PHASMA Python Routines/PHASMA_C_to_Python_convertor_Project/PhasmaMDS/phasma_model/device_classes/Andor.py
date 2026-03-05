from .base import device_base


class Andor(device_base):
    pass

# class Andor:
    
#     def __init__(self, name_mds, name_local, nchannels : int, channel_prefix ='', 
#                  grouping='None',description='', diagnostic = ''): 
#         self.name_mds = name_mds     # mds node name
#         self.name_local = name_local  # device name in local file system
#         self.grouping = grouping        # class of measurement (magnetics, optical, etc)
#         self.num_channels=nchannels   # number of data channels, not including time
#         self.channel_prefix = channel_prefix
#         a = {}
#         for i in range(1,nchannels+1): a.update({i:f"{channel_prefix}{i:d}"})
#         self.channel_names = a
#         self.description = description
#         self.diagnostic = diagnostic
