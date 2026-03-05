# from .devices import MSO5104_01,VX4244
# from ..diag_base import diag



# MSO5104_01_chmap = {
#     1:"IA_HIGHRES",
#     2:"IB_HIGHRES",
#     3:"TS_PERP",
#     4:"TS_PARA"
#     }

# MSO5104_01_chmap = {
#     1:"IA_HIGHRES",
#     2:"IB_HIGHRES",
#     3:"TS_PERP",
#     4:"TS_PARA"
#     }


# VX4244_chmap = {
#      1:'IA_4',
#      2:'BDOT',
#      3:'IB_4',
#      4: 'VB_4',
#      5: 'IA_B',
#      6:'ISAT',
#      7:'IB_B',
#      8:'HOUSE7',
#     }

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
     8:'HOUSE7',
    }

chmap = VX4244_chmap | MSO5104_01_chmap

diagnostic = chmap

# diagn= diag('Housekeeping',
#                     house_map
#     )

