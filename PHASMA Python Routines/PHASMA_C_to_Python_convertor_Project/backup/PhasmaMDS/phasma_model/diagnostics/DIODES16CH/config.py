
# from .devices import HWdevices

from ..diag_base import diag

PD1_chmap = {
    1:'PD1_HORZ1',
    2:'PD2_HORZ2',
    3:'PD3_HORZ3',
    4:'PD4_HORZ4',
    }

PD2_chmap = {
    5:'PD5_HORZ5',
    6:'PD6_HORZ6',
    7:'PD7_HORZ7',
    8:'PD8_HORZ8',
    }

PD3_chmap = {
    9:'PD9_VERT1',
    10:'PD10_VERT2',
    11:'PD11_VERT3',
    12:'PD12_VERT4',
    }

PD4_chmap = {
    13:'PD13_VERT5',
    14:'PD14_VERT6',
    15:'PD15_VERT7',
    16:'PD16_VERT8',
    }

maps = [PD1_chmap,PD2_chmap,PD3_chmap,PD4_chmap]

chmap = PD1_chmap | PD2_chmap | PD3_chmap | PD4_chmap


# for i,dev in enumerate(HWdevices):
#     HWdevices[i].channel_names = maps[i]

diagnostic = chmap
# diagnostic = diag(name_mds = 'DIODES16CH',
#                   chmap = chmap
#                   )

# HWdevices.MSO5104_PD1.channel_names = PD1_chmap
# HWdevices.MSO5104_PD2.channel_names = PD2_chmap
# HWdevices.MSO5104_PD3.channel_names = PD3_chmap
# HWdevices.MSO5104_PD4.channel_names = PD4_chmap