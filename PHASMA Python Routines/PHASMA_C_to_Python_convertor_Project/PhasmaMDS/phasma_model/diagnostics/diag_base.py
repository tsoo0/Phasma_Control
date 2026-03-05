class diag:

    def __init__(self, name_mds, chmap): 
        self.name_mds = name_mds     # mds node name
        # self.name_local = name_local  # device name in local file system
        # self.grouping = grouping        # class of measurement (magnetics, optical, etc)
        self.num_channels=len(chmap)   # number of data channels, not including time
        # self.MToffset = MToffset # time offset relative to master trigger in seconds (float)
        # self.channel_prefix = channel_prefix
        self.channel_names = chmap