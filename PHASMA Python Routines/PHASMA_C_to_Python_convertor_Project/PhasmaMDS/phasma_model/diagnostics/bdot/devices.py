from ...device_classes.Joerger import Joerger

diagnostic = "BDOT"
devtag = "MAG"
devgroup = "DATA"

JOERGER01 = Joerger(name_mds='JOERGER01',
                 name_local='JOERGER01',
                 grouping = devgroup,
                 nchannels=8,
                 MToffset = 0,
                 channel_prefix = 'Joerger01_',
                 description='Bdot channels 1-8',
                 diagnostic = diagnostic,
                 tag = devtag
                 )

JOERGER02 = Joerger(name_mds='JOERGER02',
                 name_local='JOERGER02',
                 grouping = devgroup,
                 nchannels=8,
                 MToffset = 0,
                 channel_prefix = 'Joerger02_',
                 description='Bdot channels 9-16',
                 diagnostic = diagnostic,
                 tag = devtag
                 )

JOERGER03 = Joerger(name_mds='JOERGER03',
                 name_local='JOERGER03',
                 grouping = devgroup,
                 nchannels=8,
                 MToffset = 0,
                 channel_prefix = 'Joerger03_',
                 description='Bdot channels 17-24',
                 diagnostic = diagnostic,
                 tag = devtag
                 )

JOERGER04 = Joerger(name_mds='JOERGER04',
                 name_local='JOERGER04',
                 grouping = devgroup,
                 nchannels=8,
                 MToffset = 0,
                 channel_prefix = 'Joerger04_',
                 description='Bdot channels 25-32',
                 diagnostic = diagnostic,
                 tag = devtag
                 )
                 
JOERGER05 = Joerger(name_mds='JOERGER05',
                 name_local='JOERGER05',
                 grouping = devgroup,
                 nchannels=8,
                 MToffset = 0,
                 channel_prefix = 'Joerger05_',
                 description='Bdot channels 33-40',
                 diagnostic = diagnostic,
                 tag = devtag
                 )

HWdevices = [JOERGER01,JOERGER02,JOERGER03,JOERGER04,JOERGER05]
