from .devices import HWdevices, TS_setup

tag = "TS"


scopechmap = {
    1:"PD1",
    2:"PD2",
    }

andorchmap = {
    1:"TSWAVELEN",
    2:"HORZ",
    3:"VERT",
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