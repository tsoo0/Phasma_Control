import importlib

from .diagnostics import devfids

devdict = {}
for fid in devfids:
    
    dev = importlib.import_module(f".{fid}",'phasma_model.diagnostics')
    devdict.update({fid:dev})
    



