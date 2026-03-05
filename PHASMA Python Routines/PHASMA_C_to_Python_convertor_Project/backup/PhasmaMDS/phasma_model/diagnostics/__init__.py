import os

fpath = os.path.dirname(os.path.realpath(__file__))

topfids = os.listdir(fpath)

devfids = [fid for fid in topfids if fid.isalnum()]

