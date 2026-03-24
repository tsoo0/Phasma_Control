from pathlib import Path
import os
ROOT_DIR = Path(__file__).parent
os.chdir(ROOT_DIR)

old_name = 'diag_setup.py'
new_name = 'diag_def.py'

fids = [fid for fid in os.listdir(ROOT_DIR) if Path(fid).is_dir() and "__pycache__" not in fid]
# %%

for fid in fids:
    
    try:
        os.chdir(ROOT_DIR.joinpath(fid))
        print(f"{os.curdir}")
        os.rename(old_name,new_name)
        print(f"renamed {fid}")
    except:
        print(f"failed to rename in {ROOT_DIR.joinpath(fid)} ")
        continue

