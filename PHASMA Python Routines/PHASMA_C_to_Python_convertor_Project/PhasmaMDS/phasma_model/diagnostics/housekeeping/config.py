from .devices import HWdevices

MSO5104_01_chmap = {
    9:"IA_HIGHRES",
    10:"IB_HIGHRES",
    11:"TS_PERP",
    12:"TS_PARA"
    }

VX4244_chmap = {
     1:'IA_4',
     2:'BDOT',
     3:'IB_4',
     4: 'VB_4',
     5: 'IA_B',
     6:'ISAT',
     7:'IB_B',
    }

chmaps = [VX4244_chmap,MSO5104_01_chmap]
chmap = {}
for i,dev in enumerate(HWdevices):
    dev.channel_names=chmaps[i]
    chmap |= chmaps[i]

diagnostic = chmap