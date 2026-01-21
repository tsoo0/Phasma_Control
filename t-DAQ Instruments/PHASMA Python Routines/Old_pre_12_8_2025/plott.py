# -*- coding: utf-8 -*-
"""
Created on Tue Aug 15 15:11:50 2023

@author: Scime Lab
"""

import pandas as pd
import os 
basedir = "D:\MDSPLUS_PHASMA\RAWDATA"

fid = "98283_acq2106_373.txt"

#basedir = "C:/Users/Scime Lab/Desktop/DTAQ integration"
#fid = "0_acq2106_373.txt"

os.chdir(basedir)
df = pd.read_csv(fid,dtype='float')

df = df.drop(df.columns[0],axis='columns')

df.plot(0)