import importlib

from .diagnostics import devfids
# diagdict = {}
devdict = {}
for fid in devfids:
    
    dev = importlib.import_module(f".{fid}",'phasma_model.diagnostics')
    devdict.update({fid:dev})
    # diagdict.update({fid:dev.config.diagnostic})
    


