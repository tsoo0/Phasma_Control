from phasma_model.phasma_devices import devdict
import os

from pathlib import Path
ROOT_DIR = Path(__file__).parent
    
testdatadir = str(ROOT_DIR) + '/testdata'
# %%


for diagnostic in list(devdict.keys()):
    for device in devdict.get(diagnostic):
        device.write_dummy_local(testdatadir, shot=1)