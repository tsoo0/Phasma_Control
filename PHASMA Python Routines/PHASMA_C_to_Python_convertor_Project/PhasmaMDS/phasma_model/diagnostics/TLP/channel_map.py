from .devices import HWdevices
from ..group_channels import group_channels

tag = "TLP"

chmaps = [devchans.channel_names for devchans in HWdevices]

diagnostic = group_channels(chmaps)