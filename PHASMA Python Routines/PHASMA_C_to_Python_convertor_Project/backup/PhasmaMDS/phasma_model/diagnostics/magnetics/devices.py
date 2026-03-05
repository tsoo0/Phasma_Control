from ...device_classes.Joerger import Joerger
# from ...device_classes.MSO5104 import MSO5104

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
                 
JOERGER05 = Joerger(name_mds='JOERGER05',
                 name_local='JOERGER05',
                 grouping = 'magnetics',
                 nchannels=4,
                 MToffset = 0,
                 channel_prefix = 'Joerger05_',
                 description='Bdot channels 32-36',
                 diagnostic = 'magnetics'
                 )

# MSO5104_BDOT = MSO5104(name_mds='MSO5104_BDOT',
#                  name_local='MSO5104_BDOT',
#                  grouping = 'magnetics',
#                  nchannels=4,
#                  MToffset = 0,
#                  channel_prefix = 'MSO5104_BDOT_',
#                  description='Bdot channels 33-37',
#                  diagnostic = 'magnetics'
#                  )

HWdevices = [JOERGER01,JOERGER02,JOERGER03,JOERGER04,JOERGER05]
