# -*- coding: utf-8 -*-
"""
Created on Tue Aug 15 11:41:39 2023

@author: Scime Lab
"""

import acq400_hapi as acq400_hapi
import os
#os.chdir("C:/Users/Scime Lab/Desktop/DTAQ integration/acq400_hapi-master/acq400_hapi-master")
# curdir = os.pwd()
# libdir = curdir + "/acq400_hapi-master"

import numpy as np
import pandas as pd
import time

sample_rate_ = "20M"; #not used
sample_rate = 20e6 # currently no provision to change sample rate

_50Ohm = 1 #not implemented

basedir = "C:/Users/Scime Lab/Desktop/DTAQ integration"
fid = "dat.txt"

dev1 = 'acq2106_372'
dev2 = "acq2106_373"

uut1 = acq400_hapi.factory(dev1)
uut2 = acq400_hapi.factory(dev2)
uuts = [uut1,uut2]

sc = acq400_hapi.shotcontrol.ShotController(uuts)