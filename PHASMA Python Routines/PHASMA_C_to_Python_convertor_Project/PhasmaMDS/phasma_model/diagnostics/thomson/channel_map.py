from .devices import HWdevices, TS_setup

tag = "TS"


scopechmap = {
    1:"TIME_PHOTO",
    2:"TS_PHOTO1",
    3:"TS_PHOTO2",
    }

andorchmap = {
    1:"ANDOR_WAVELEN",
    2:"ANDOR_IMG1",
    3:"ANDOR_IMG2",
    }

chmaps = [scopechmap, andorchmap]

diagdict={}
n=1;
for i,chmap in enumerate(chmaps):
    for j in chmap.values():
        diagdict.update({n:j})
        n+=1
    
diagnostic=diagdict

setup = TS_setup.field_names