import sys
sys.path.append(r"C:\PHASMA 2025 DAQ\D_TACQ Instruments")
sys.path.append(r"C:\PHASMA 2025 DAQ\D_TACQ Instruments\acq400_hapi_master")

print(sys.path)
import numpy as np
import acq400_hapi as acq
import time

V_full_scale = 4

#pushes default configuration to one box - only needs to be run once, and not before every acquisition
# dev_ip - static ip (or DHCP alias) of device to change settings on 
# num_samples - number of samples per channel per transient acquisition
# Defaults to 50 Ohm termination on all channels, trigger from front panel
def config_master(dev_ip,num_samples = 1000,termination='50R', trig_source='FP',sample_rate=20e6):

    site0 = comm_s0(dev_ip)
    #Don't currently foresee a need to change any of these fields aside from the number of samples (POST)
    site0.sr("set.site 0 transient PRE=0 POST={post:d} OSAM=1 DEMUX={demux:d} SOFT_TRIGGER={soft_tr:d}".format(
        post=num_samples,
        demux=1,
        soft_tr=0,
        ))
    
    termination = '50R' # hard code this because LW can only do 1M
    
    
    # site1 = comm_s1(devip);
    site0.sr('set.site 1 trg 1,0,1')
    
    # site0.sr('set.site 0 transient SOFT_TRIGGER=1') # useful for debugging only

    if trig_source == "HDMI":# Use breakout board's external trigger (48-channel acq2106)
        # site0.sr(f"set.site 0 sync_role master {sample_rate} TRG:DX=d0")
        comm_s0(dev_ip).sr("set.site 0 SIG:SRC:TRG:0 HDMI")
    if termination == '50R':
        enable_50Ohm_all(dev_ip)
    else:
        enable_1MEG_all(dev_ip)
    

# Arm the hard trigger of a specific acq
# ip or DNS name of acq to arm
def arm_transient(dev_ip):
    
    site0 = comm_s0(dev_ip)
    
    return site0.sr("set_arm")

def Config_Arm(dev_ip,num_samples):
    
    config_master(dev_ip,num_samples=num_samples,trig_source='HDMI')
    
    # time.sleep(1)

    arm_transient(dev_ip)    
    
# returns time array starting at 0 corresponding to last shot
def get_timebase(dev_ip, num_samples):
    site1 = comm_s1(dev_ip)

    Fs = int(site1.sr('ACQ480:OSR').split(' ')[1])
    
    timearr = np.linspace(0,num_samples/Fs, num_samples)
    
    return timearr

def int16_to_volts(rawdat,V_full_scale):
    
    if type(rawdat) is list:
        return [raw * V_full_scale / (2**15 - 1) for raw in rawdat]
    else:
        return np.multiply(rawdat, V_full_scale / (2**15 - 1))

def enable_50Ohm_all(dev_ip): # enable 50 Ohm termination across all channels
    
    sitelist = list(comm_s0(dev_ip).sr("sites"))
    sitelist = [int(k) for k in sitelist if k.isnumeric()]
    
    for site in sitelist:
        site_n = acq.Siteclient(dev_ip, acq.AcqPorts.SITE0 + site)            

        site_n.sr("T50R 1")

def enable_1MEG_all(dev_ip): # enable 50 Ohm termination across all channels
    
    sitelist = list(comm_s0(dev_ip).sr("sites"))
    sitelist = [int(k) for k in sitelist if k.isnumeric()]
    
    for site in sitelist:
        site_n = acq.Siteclient(dev_ip, acq.AcqPorts.SITE0 + site)            

        site_n.sr("T50R 0")

# Open a socket with Site 0 at a given IP or alias; returns an open client
def comm_s0(dev_ip):
    
    return acq.Siteclient(dev_ip, acq.AcqPorts.SITE0)

def comm_s1(dev_ip):
    
    return acq.Siteclient(dev_ip, acq.AcqPorts.SITE0 + acq.acq400.AcqSites.SITE1)

def read_format_raw(dev_ip, num_channels, num_samples, sample_size_bytes = int(2)):
    
    channels = list(range(1, num_channels+1))
    
    rawdata = []
    
    tstart = time.time()
    for channel in channels:
        
        nc = acq.ChannelClient(dev_ip,channel)
        
        rawdata.append(nc.read(num_samples, data_size=sample_size_bytes))
    
    print("read took " + str(time.time() - tstart) + " seconds")

    return rawdata

def post_proc(dev_ip,rawdat,num_samples,V_full_scale):
    
    rawdat = int16_to_volts(rawdat,V_full_scale)
    
    rawdat.insert(0,get_timebase(dev_ip, num_samples))
    
    return np.array(rawdat).T

def store_local(data, shotnum, path,device,diagnostic):
    import pandas as pd
    
    sys.path.append(r"C:\PHASMA 2025 DAQ\PHASMA Python Routines\PHASMA_C_to_Python_convertor_Project\PhasmaMDS")
    
    from phasma_model.phasma_devices import devdict
    
    diagnames = devdict.keys()
    
    diagname = [diagname for diagname in diagnames if diagnostic.lower() in diagname.lower()][0] # need to make dict access insensitive to case if key string
    print(device)
    print(diagname)
    dev = [a for a in devdict[diagname].devices.HWdevices if a.name_mds.upper() == device.upper()]
    
    chnames = dev[0].channel_names
    # print(chnames)
    df = pd.DataFrame(data)
    
    df = df.rename(columns=chnames)
    
    datafid = f"{shotnum}_{device}.txt"
    
    df.to_csv(path + "/" +  datafid)
    
def readout_store(dev_ip, num_channels, num_samples, shotnum, readout_dir,moniker,diagnostic):

    raw = read_format_raw(dev_ip,num_channels,num_samples)
    
    data = post_proc(dev_ip, raw, num_samples,V_full_scale)

    store_local(data, shotnum, readout_dir, moniker, diagnostic)
    
if __name__ == "__main__":
    # dev1= "192.168.1.22" # acq2106_372
    
    dev1 = "192.168.1.226" # 433
    # dev1 = "192.168.0.178" # 434
    # dev1 = "192.169.0.112"
    
    num_samples = int(1e3) # max 4 million samples with 48 channels

    read_delay = 5
    arm_delay = 10
    
    num_channels = 48
    
    tstart = time.time_ns()
    
    config_master(dev1,num_samples=num_samples,trig_source='HDMI') # Set the acq up for transient (one-shot) acquisition using an 

    
    # %%


    arm_transient(dev1)

    time.sleep(arm_delay)     

    # Config_Arm(dev1, num_samples)
                       

    #%%
    raw = read_format_raw(dev1,num_channels,num_samples)
    
    data = post_proc(dev1, raw, num_samples,V_full_scale)
    
    # plt.plot(data)
    basedir = r"C:\Users\Scime Lab\Desktop\DTAQ integration"
    # store_local(data, "test.csv", basedir + '/', 'TLP', 'ACQ2106_372')
    store_local(data, "test.csv", basedir + '/', 'TOMOGRAPHY', 'ACQ2106_434')
    # readout_store(dev1, num_channels, num_samples, 90000, basedir)
    # readout_store(dev1, num_channels, num_samples, 10000, "D:\MDSPLUS_PHASMA\RAWDATA")