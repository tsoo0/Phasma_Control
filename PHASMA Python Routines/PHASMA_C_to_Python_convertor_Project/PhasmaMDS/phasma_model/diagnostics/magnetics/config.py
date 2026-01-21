from .devices import JOERGER01, JOERGER02, JOERGER03, JOERGER04

J1_chmap = {1:'BDOT_BX1',
            2:'BDOT_BY1',
            3:'BDOT_BX2',
            4: 'BDOT_BY2',
            5: 'BDOT_BX3',
            6:'BDOT_BY3',
            7:'BDOT_BX4',
            8:'BDOT_BY4',
            }

J2_chmap = {1:'BDOT_BX1',
            2:'BDOT_BY1',
            3:'BDOT_BX2',
            4: 'BDOT_BX1',
            5: 'BDOT_BX1',
            6:'BDOT_BX1',
            7:'BDOT_BX1',
            8:'BDOT_BX1',
            }

J3_chmap = {1:'BDOT_BX1',
            2:'BDOT_BY1',
            3:'BDOT_BX2',
            4: 'BDOT_BX1',
            5: 'BDOT_BX1',
            6:'BDOT_BX1',
            7:'BDOT_BX1',
            8:'BDOT_BX1',
            }

J4_chmap = {1:'BDOT_BX1',
            2:'BDOT_BY1',
            3:'BDOT_BX2',
            4: 'BDOT_BX1',
            5: 'BDOT_BX1',
            6:'BDOT_BX1',
            7:'BDOT_BX1',
            8:'BDOT_BX1',
            }

JOERGER01.channel_names = list(J1_chmap.values())
JOERGER02.channel_names = list(J2_chmap.values())
JOERGER03.channel_names = list(J3_chmap.values())
JOERGER04.channel_names = list(J4_chmap.values())