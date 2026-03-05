from .devices import HWdevices
from ..diag_base import diag


# J1_chmap = {1:'BDOT_BX1',
#             2:'BDOT_BY1',
#             3:'BDOT_BX2',
#             4: 'BDOT_BY2',
#             5: 'BDOT_BX3',
#             6:'BDOT_BY3',
#             7:'BDOT_BX4',
#             8:'BDOT_BY4',
#             }

# J2_chmap = {1:'BDOT_BX5',
#             2:'BDOT_BY5',
#             3:'BDOT_BX6',
#             4: 'BDOT_BY6',
#             5: 'BDOT_BX7',
#             6:'BDOT_BY7',
#             7:'BDOT_BX8',
#             8:'BDOT_BY8',
#             }

# J3_chmap = {1:'BDOT_BX9',
#             2:'BDOT_BY9',
#             3:'BDOT_BX10',
#             4: 'BDOT_BY10',
#             5: 'BDOT_BX11',
#             6:'BDOT_BY11',
#             7:'BDOT_BX12',
#             8:'BDOT_BY12',
#             }

# J4_chmap = {1:'BDOT_BX13',
#             2:'BDOT_BY13',
#             3:'BDOT_BX14',
#             4: 'BDOT_BY14',
#             5: 'BDOT_BX15',
#             6:'BDOT_BY15',
#             7:'BDOT_BX16',
#             8:'BDOT_BY16',
#             }

# J5_chmap= {
#     1:'BDOT_BX17',
#     2:'BDOT_BY17',
#     3:'BDOT_BX18',
#     4:'BDOT_BY18',
#     }


J1_chmap = {1:'BDOT_BX1',
            2:'BDOT_BY1',
            3:'BDOT_BX2',
            4: 'BDOT_BY2',
            5: 'BDOT_BX3',
            6:'BDOT_BY3',
            7:'BDOT_BX4',
            8:'BDOT_BY4',
            }

J2_chmap = {9:'BDOT_BX5',
            10:'BDOT_BY5',
            11:'BDOT_BX6',
            12: 'BDOT_BY6',
            13: 'BDOT_BX7',
            14:'BDOT_BY7',
            15:'BDOT_BX8',
            16:'BDOT_BY8',
            }

J3_chmap = {17:'BDOT_BX9',
            18:'BDOT_BY9',
            19:'BDOT_BX10',
            20: 'BDOT_BY10',
            21: 'BDOT_BX11',
            22:'BDOT_BY11',
            23:'BDOT_BX12',
            24:'BDOT_BY12',
            }

J4_chmap = {25:'BDOT_BX13',
            26:'BDOT_BY13',
            27:'BDOT_BX14',
            28: 'BDOT_BY14',
            29: 'BDOT_BX15',
            30:'BDOT_BY15',
            31:'BDOT_BX16',
            32:'BDOT_BY16',
            }

J5_chmap= {
    33:'BDOT_BX17',
    34:'BDOT_BY17',
    35:'BDOT_BX18',
    36:'BDOT_BY18',
    }

chmaps = [J1_chmap, J2_chmap, J3_chmap, J4_chmap, J5_chmap]




chmap = {}
for i,dev in enumerate(HWdevices):
    dev.channel_names=chmaps[i]
    chmap |= chmaps[i]

diagnostic = chmap
# diagnostic = diag('MAGNETICS', chmap)


# MSO5104_BDOT_chmap = {
#     1:'BDOT_BX17',
#     2:'BDOT_BY17',
#     3:'BDOT_BX18',
#     4:'BDOT_BY18'
#     }

# JOERGER01.channel_names = list(J1_chmap.values())
# JOERGER02.channel_names = list(J2_chmap.values())
# JOERGER03.channel_names = list(J3_chmap.values())
# JOERGER04.channel_names = list(J4_chmap
# MSO5104_BDOT.channel_names = list(MSO5104_BDOT_chmap.values())