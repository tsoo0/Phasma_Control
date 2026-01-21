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
        
        uut1.s0.shot = shot
        uut1.s0.transient = 'SOFT_TRIGGER=0'
        uut1.s0.transient = 'PRE=0'
        uut1.s0.transient='POST='+str(samples)
        #uut1.s0.TRANSIENT_POST='100'
        uut1.s0.SIG_SYNC_OUT_CLK = "CLK"
        uut1.s0.SIG_SYNC_OUT_CLK_DX = "d3"
        uut1.s0.SIG_SYNC_OUT_TRG = "TRG"
        uut1.s0.SIG_SYNC_OUT_TRG_DX = 0
        uut1.s0.SIG_SYNC_OUT_SYNC = "SYNC"
        uut1.s0.SIG_SYNC_OUT_SYNC_DX = "d2"
        # uut1.s1.clk = "1,3,1"
        uut1.s1.clk = "0,3,1" #internal clock
        uut1.s1.trg = "1,0,1"
        uut1.s1.sync = "0,0,0"
        uut1.s1.clkdiv = 1
        uut1.s1.sync_trg_to_clk=1
    
    config_master(uut1,shot,samples)
    config_master(uut2,shot,samples)
    
    #sc.run_shot() #run_shot blocks labview execution... no good
    
    sc.prep_shot()

    sc.arm_shot()

    return 0
    
def Readout(readoutdir,time_offset,shot_number):  
   
    sc.on_shot_complete()
       
    acq400_hapi.cleanup.sleep(2.0) #wait before read to avoid grabbing stale data
    
    rawdat = sc.read_channels()

    uut1dat = rawdat[0][0]
    uut2dat = rawdat[0][1]
    
    uutdat = [uut1dat,uut2dat]

    def to_csv(datarr,time_offset,readoutdir,fid):
        
        start=time_offset
        num_samples = len(datarr[0][:])
        timearr = np.linspace(start,start + num_samples / sample_rate, num_samples) + float(time_offset)

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
            time.sleep(.1)    
    
    os.chdir(readoutdir)
    to_csv(uut1dat,time_offset,readoutdir,acq1readout_fid)
    to_csv(uut2dat,time_offset,readoutdir,acq2readout_fid)
    
    return np.asarray(uut1dat)
    
if __name__ == "__main__":
    Config_Arm(1000,0)
    
    Readout(basedir,0,0)
