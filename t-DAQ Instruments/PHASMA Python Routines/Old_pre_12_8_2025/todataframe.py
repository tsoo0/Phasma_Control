# -*- coding: utf-8 -*-
"""
Created on Wed Aug  9 22:04:44 2023

@author: tom
"""
import numpy as np
import pandas as pd
import time
# from transient2MasterDataHandling import uut1dat

start=0
sample_rate = 20e6
num_samples = len(uut1dat[0][:])

timearr = np.linspace(start,start + num_samples / sample_rate, num_samples)
datarr1 = uut1dat
dat_df = pd.DataFrame(data={'Time':timearr})
for n,arr in enumerate(datarr1):
    
    temparr = pd.Series(data=arr,name='ch'+str(n),dtype=float)
    
    dat_df = pd.concat([dat_df, temparr],axis=1)

basedir = "C:/Users/Scime Lab/Desktop/DTAQ integration"

fidstr = basedir + str(time.time())[:10] + ".csv"
dat_df.to_csv(fidstr)