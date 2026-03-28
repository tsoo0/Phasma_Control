from .devices import HWdevices
# from ..group_channels import group_channels

from pathlib import Path

ROOT_DIR = Path(__file__).parent
name_mds = str(ROOT_DIR).split('/')

grouping = "PHASMA"
tag = "POS"
diagnostic = HWdevices[0].field_names