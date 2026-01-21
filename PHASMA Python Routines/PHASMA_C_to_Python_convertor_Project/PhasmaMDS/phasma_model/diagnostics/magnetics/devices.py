from ...device_classes.Joerger import Joerger

JOERGER01 = Joerger(name_mds='JOERGER01',
                 name_local='JOERGER01',
                 grouping = 'magnetics',
                 nchannels=8,
                 MToffset = 0,
                 channel_prefix = 'Joerger01_',
                 description='Bdot channels 1-8',
                 diagnostic = 'magnetics'
                 )

JOERGER02 = Joerger(name_mds='JOERGER02',
                 name_local='JOERGER02',
                 grouping = 'magnetics',
                 nchannels=8,
                 MToffset = 0,
                 channel_prefix = 'Joerger02_',
                 description='Bdot channels 9-16',
                 diagnostic = 'magnetics'
                 )

JOERGER03 = Joerger(name_mds='JOERGER03',
                 name_local='JOERGER03',
                 grouping = 'magnetics',
                 nchannels=8,
                 MToffset = 0,
                 channel_prefix = 'Joerger03_',
                 description='Bdot channels 17-24',
                 diagnostic = 'magnetics'
                 )

JOERGER04 = Joerger(name_mds='JOERGER04',
                 name_local='JOERGER04',
                 grouping = 'magnetics',
                 nchannels=8,
                 MToffset = 0,
                 channel_prefix = 'Joerger04_',
                 description='Bdot channels 25-32',
                 diagnostic = 'magnetics'
                 )
                 
HWdevices = [JOERGER01,JOERGER02,JOERGER03,JOERGER04]
