from pathlib import Path
ROOT_DIR = Path(__file__)

old_name = 'config'
new_name = 'channel_map'

folder = Path()
try:
    folder.rename('new_dir')
except FileNotFoundError:
    print("Folder not found.")