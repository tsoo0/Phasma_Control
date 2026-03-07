from .devices import HWdevices
from ..group_channels import group_channels

tag = HWdevices[0].tag

chmaps = [devchans.channel_names for devchans in HWdevices]

diagnostic = group_channels(chmaps)