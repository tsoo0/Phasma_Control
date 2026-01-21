from globalsvars import *

def Config_Arm(num_samples,shot_number):

    samples = int(num_samples)

    shot = int(shot_number)
    
    def config_master(uut1,shot,samples):
        # role = 'master' #{fpmaster|rpmaster|master|slave|slavemb|solo}
        # clk = 
        # #[CLK|TRG:SENSE=falling|rising] [CLK|TRG:DX=d0|d1]
    
        #syncarg = "master 40M 1M TRG:DX=d0" #Maximum sample rate is 40Msps; not implemented
        # uut1.s0.sync_role = syncarg
       # uut1.s1.ACQ480_T50R = _50Ohm
        uut1.s0.transient='DEMUX=0'
        uut1.s0.sync_role = "master"
        uut1.s0.shot = shot

        uut1.s1.trg='1,0,1'
        uut1.s0.transient = 'PRE=0'
        uut1.s0.transient='POST='+str(samples)
        #uut1.s0.TRANSIENT_POST='100'

        # uut1.s1.ACQ480_GAIN_ALL=7
    
    config_master(uut1,shot,samples)
    config_master(uut2,shot,samples)
    
    #sc.run_shot() #run_shot blocks labview execution... no good
    
    # sc.prep_shot()

    # sc.arm_shot()

    for uut in uuts:
        
        uut.s0.TRANSIENT_SET_ARM = 1

    return 0
    
def Readout(readoutdir,time_offset,shot_number):  
    # acq400_hapi.cleanup.sleep(5.0) #wait before read to avoid grabbing stale data
    if __name__ != "__main__":
        sc.on_shot_complete()
       
    rawdat = sc.read_channels()

    uut1dat = rawdat[0][0]
    uut2dat = rawdat[0][1]
    
    uutdat = [uut1dat,uut2dat]

    num_samples = len(uut1dat[0][:])
    timearr = np.linspace(time_offset,time_offset + num_samples / sample_rate, num_samples)
    
    def to_csv(datarr,timearr,readoutdir,fid):
        
        dat_df = pd.DataFrame(data={'Time':timearr})
        for n,arr in enumerate(datarr):
            
            temparr = pd.Series(data=arr,name='ch'+str(n),dtype=float)
            
            dat_df = pd.concat([dat_df, temparr],axis=1)
        
        fidstr = fid + ".txt"
        dat_df.to_csv(fidstr)

    acq1readout_fid =  str(shot_number) + "_" + dev1
    acq2readout_fid =  str(shot_number) + "_" + dev2
    for k,uut in enumerate(uuts):
        for n in range(0,len(uut1dat)):
            uutdat[k][n] = uut.chan2volts(n+1,uutdat[k][n])
            # time.sleep(.1)    
    
    os.chdir(readoutdir)
    to_csv(uut1dat,timearr,readoutdir,acq1readout_fid)
    to_csv(uut2dat,timearr,readoutdir,acq2readout_fid)
    
    timearr = timearr.reshape((len(timearr),1))
    
    #Build matrices of data to send back to labview
    uut1dat_return = np.concatenate((timearr,np.asarray(uut1dat).T),axis=1)    
    uut2dat_return = np.concatenate((timearr,np.asarray(uut2dat).T),axis=1)  
    
    return uut1dat_return,uut2dat_return
    
if __name__ == "__main__":
    
    nsamples = 1e4
    Config_Arm(nsamples,0)
    # sc.on_shot_complete()
    tstart  = time.time()
    d1,d2=Readout(basedir,0,0)
    
    ttransfer = time.time() - tstart
    bitrate = 32/8 * 2 * d1.size / (1e6*ttransfer) # MB/s
    # sample_freq_actual = 1/np.mean(np.diff(d1[:,0]))
    print(ttransfer)