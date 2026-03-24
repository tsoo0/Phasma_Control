from .devices import HWdevices
from ..group_channels import group_channels

from pathlib import Path


ROOT_DIR = Path(__file__).parent
name_mds = str(ROOT_DIR).split('/')

tag = "HOUSE"

VX4244_chmap = {
     1:'IA_4',
     2:'BDOT',
     3:'IB_4',
     4: 'VB_4',
     5: 'IA_B',
     6:'ISAT',
     7:'IB_B',
    }

MSO5104_01_chmap = {
    1:"IA_HIGHRES",
    2:"IB_HIGHRES",
    3:"TS_PERP",
    4:"TS_PARA"
    }

PD1_chmap = {
    1:'PD1_HORZ1',
    2:'PD2_HORZ2',
    3:'PD3_HORZ3',
    4:'PD4_HORZ4',
    }

PD2_chmap = {
    1:'PD5_HORZ5',
    2:'PD6_HORZ6',
    3:'PD7_HORZ7',
    4:'PD8_HORZ8',
    }

PD3_chmap = {
    1:'PD9_VERT1',
    2:'PD10_VERT2',
    3:'PD11_VERT3',
    4:'PD12_VERT4',
    }

PD4_chmap = {
    1:'PD13_VERT5',
    2:'PD14_VERT6',
    3:'PD15_VERT7',
    4:'PD16_VERT8',
    }

chmaps = [MSO5104_01_chmap, VX4244_chmap, PD1_chmap, PD2_chmap, PD3_chmap, PD4_chmap]
    
diagnostic= group_channels(chmaps)