from .devices import HWdevices

from ..group_channels import group_channels

tag = "PLIF"




chmap = group_channels([a.channel_names for a in HWdevices])

diagnostic = chmap

# setup = plif_setup.field_names