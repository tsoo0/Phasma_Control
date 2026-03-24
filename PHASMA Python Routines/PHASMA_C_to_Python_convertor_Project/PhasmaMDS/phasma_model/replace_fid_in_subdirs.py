"""
 search all directories contained within a given directory and replace all instances of 'replace_file_fid' in subdirectories with the one specified by model_fid
 """
 
import os
import shutil
fpath = os.path.dirname(os.path.realpath(__file__))

replace_file_fid = "__init__.py"

model_fid = "model_init_.py"

diagroot = os.path.join(fpath,'diagnostics')

diagfids = os.listdir(diagroot)

model_path = os.path.join(fpath, model_fid)

diagfids = [fid for fid in diagfids if os.path.isdir(os.path.join(diagroot,fid))]

diagfids = [fid for fid in diagfids if "__" not in fid and fid]


for diagfid in diagfids:
    diagdir = os.path.join(diagroot, diagfid)
    diaginitpath = os.path.join(diagdir, replace_file_fid)
    
    os.chdir(diagdir)
    
    try: 
        shutil.copy2(model_path, diaginitpath)

        print(f"replaced {diaginitpath}")
    except :
        print(f"failed to repalce {diaginitpath}\n")