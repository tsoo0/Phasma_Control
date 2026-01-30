// 4780 Filter Setup Function
//
// Modified on 3/9/99 by E. Scime
// 	New version includes soft front panel that allows control of settings of 
//	each group of channels. All channels set to DC coupled and only gains of
//	1, 10, and 100 directly converted to "named" variables.

#include "Tkvx4780.h"
#include <ansi_c.h>
#include <vpptype.h>


void setup_4780(int sig_cond_hndl,ViInt16 group1_status, ViInt32 group1_attn,
				ViInt32 group1_gain,ViInt32 group1_cutoff,ViInt16 group2_status,
				ViInt32 group2_attn,ViInt32 group2_gain,ViInt32 group2_cutoff,
				ViInt16 group3_status, ViInt32 group3_attn,ViInt32 group3_gain,
				ViInt32 group3_cutoff, ViInt16 group4_status, ViInt32 group4_attn,
				ViInt32 group4_gain,ViInt32 group4_cutoff)
{
    static	ViStatus   	result;                 		  /* function return status */
	void 	displayErr(ViSession InstrHndl,ViStatus err); /* For Displaying Error Messages */ 

//	Group 1 Settings
	if (group1_status==1) {
	
		if (group1_attn==1) group1_attn=tkvx4780_BYPASS_ATTEN;
 		if (group1_attn==10) group1_attn=tkvx4780_MIN_ATTEN;
 		if (group1_attn==100) group1_attn=tkvx4780_MAX_ATTEN;
 		
		if (group1_gain==1) group1_gain=tkvx4780_MIN_GAIN;
 		if (group1_gain==10) group1_gain=tkvx4780_GAIN_10;
 		if (group1_gain==100) group1_gain=tkvx4780_MAX_GAIN;
 		
  		if (result = tkvx4780_setInputState_grp1 (sig_cond_hndl,
											  tkvx4780_SELECT_CHANNEL,
											  tkvx4780_SELECT_CHANNEL,
											  tkvx4780_SELECT_CHANNEL,
											  tkvx4780_SELECT_CHANNEL,
											  tkvx4780_SELECT_ALL_CHANNELS,
											  tkvx4780_INPUT_STATE_ON)){
        	displayErr(sig_cond_hndl,result);
    	}
 		if (result = tkvx4780_setOutputState_grp1 (sig_cond_hndl,
											   tkvx4780_SELECT_CHANNEL,
											   tkvx4780_SELECT_CHANNEL,
											   tkvx4780_SELECT_CHANNEL,
											   tkvx4780_SELECT_CHANNEL,
											   tkvx4780_SELECT_ALL_CHANNELS,
											   tkvx4780_OUTPUT_STATE_ON)){
        	displayErr(sig_cond_hndl,result);
    	}
 		if (result = tkvx4780_setCoupling_grp1 (sig_cond_hndl,
											tkvx4780_SELECT_CHANNEL,
											tkvx4780_SELECT_CHANNEL,
											tkvx4780_SELECT_CHANNEL,
											tkvx4780_SELECT_CHANNEL,
											tkvx4780_SELECT_ALL_CHANNELS,
											tkvx4780_DC_COUPLE_STR)){
        	displayErr(sig_cond_hndl,result);
    	}
 		if (result = tkvx4780_setAttenuation_grp1 (sig_cond_hndl,
											   tkvx4780_SELECT_CHANNEL,
											   tkvx4780_SELECT_CHANNEL,
											   tkvx4780_SELECT_CHANNEL,
											   tkvx4780_SELECT_CHANNEL,
											   tkvx4780_SELECT_ALL_CHANNELS,
											   group1_attn)){
        	displayErr(sig_cond_hndl,result);
    	}
 		if (result = tkvx4780_setGain_grp1 (sig_cond_hndl,
										tkvx4780_SELECT_CHANNEL,
										tkvx4780_SELECT_CHANNEL,
										tkvx4780_SELECT_CHANNEL,
										tkvx4780_SELECT_CHANNEL,
										tkvx4780_SELECT_ALL_CHANNELS,
										group1_gain)){
        	displayErr(sig_cond_hndl,result);
   	 	}
 		if (result = tkvx4780_setFilterFreq_grp1 (sig_cond_hndl,
											  tkvx4780_SELECT_CHANNEL,
											  tkvx4780_SELECT_CHANNEL,
											  tkvx4780_SELECT_CHANNEL,
											  tkvx4780_SELECT_CHANNEL,
											  tkvx4780_SELECT_ALL_CHANNELS,
											  group1_cutoff)){
     		displayErr(sig_cond_hndl,result);
    	}
    }

//	Group 2 Settings
	if (group2_status==1) {
	
		if (group2_attn==0) group2_attn=tkvx4780_BYPASS_ATTEN;
 		if (group2_attn==10) group2_attn=tkvx4780_MIN_ATTEN;
 		if (group2_attn==100) group2_attn=tkvx4780_MAX_ATTEN;
 		
		if (group2_gain==1) group2_gain=tkvx4780_MIN_GAIN;
 		if (group2_gain==10) group2_gain=tkvx4780_GAIN_10;
 		if (group2_gain==100) group2_gain=tkvx4780_MAX_GAIN;
 		
  		if (result = tkvx4780_setInputState_grp2 (sig_cond_hndl,
											  tkvx4780_SELECT_CHANNEL,
											  tkvx4780_SELECT_CHANNEL,
											  tkvx4780_SELECT_CHANNEL,
											  tkvx4780_SELECT_CHANNEL,
											  tkvx4780_SELECT_ALL_CHANNELS,
											  tkvx4780_INPUT_STATE_ON)){
        	displayErr(sig_cond_hndl,result);
    	}
 		if (result = tkvx4780_setOutputState_grp2 (sig_cond_hndl,
											   tkvx4780_SELECT_CHANNEL,
											   tkvx4780_SELECT_CHANNEL,
											   tkvx4780_SELECT_CHANNEL,
											   tkvx4780_SELECT_CHANNEL,
											   tkvx4780_SELECT_ALL_CHANNELS,
											   tkvx4780_OUTPUT_STATE_ON)){
        	displayErr(sig_cond_hndl,result);
    	}
 		if (result = tkvx4780_setCoupling_grp2 (sig_cond_hndl,
												tkvx4780_SELECT_CHANNEL,
												tkvx4780_SELECT_CHANNEL,
												tkvx4780_SELECT_CHANNEL,
												tkvx4780_SELECT_CHANNEL,
												tkvx4780_SELECT_ALL_CHANNELS,
												tkvx4780_AC_COUPLE_STR)){
        	displayErr(sig_cond_hndl,result);
    	}
 		if (result = tkvx4780_setAttenuation_grp2 (sig_cond_hndl,
											   tkvx4780_SELECT_CHANNEL,
											   tkvx4780_SELECT_CHANNEL,
											   tkvx4780_SELECT_CHANNEL,
											   tkvx4780_SELECT_CHANNEL,
											   tkvx4780_SELECT_ALL_CHANNELS,
											   group2_attn)){
        	displayErr(sig_cond_hndl,result);
    	}
 		if (result = tkvx4780_setGain_grp2 (sig_cond_hndl,
										tkvx4780_SELECT_CHANNEL,
										tkvx4780_SELECT_CHANNEL,
										tkvx4780_SELECT_CHANNEL,
										tkvx4780_SELECT_CHANNEL,
										tkvx4780_SELECT_ALL_CHANNELS,
										group2_gain)){
        	displayErr(sig_cond_hndl,result);
   	 	}
 		if (result = tkvx4780_setFilterFreq_grp2 (sig_cond_hndl,
											  tkvx4780_SELECT_CHANNEL,
											  tkvx4780_SELECT_CHANNEL,
											  tkvx4780_SELECT_CHANNEL,
											  tkvx4780_SELECT_CHANNEL,
											  tkvx4780_SELECT_ALL_CHANNELS,
											  group2_cutoff)){
     		displayErr(sig_cond_hndl,result);
    	}
    }

//	Group 3 Settings
	if (group3_status==1) {
	
		if (group3_attn==0) group3_attn=tkvx4780_BYPASS_ATTEN;
 		if (group3_attn==10) group3_attn=tkvx4780_MIN_ATTEN;
 		if (group3_attn==100) group3_attn=tkvx4780_MAX_ATTEN;
 		
		if (group3_gain==1) group3_gain=tkvx4780_MIN_GAIN;
 		if (group3_gain==10) group3_gain=tkvx4780_GAIN_10;
 		if (group3_gain==100) group3_gain=tkvx4780_MAX_GAIN;
 		
  		if (result = tkvx4780_setInputState_grp3 (sig_cond_hndl,
											  tkvx4780_SELECT_CHANNEL,
											  tkvx4780_SELECT_CHANNEL,
											  tkvx4780_SELECT_CHANNEL,
											  tkvx4780_SELECT_CHANNEL,
											  tkvx4780_SELECT_ALL_CHANNELS,
											  tkvx4780_INPUT_STATE_ON)){
        	displayErr(sig_cond_hndl,result);
    	}
 		if (result = tkvx4780_setOutputState_grp3 (sig_cond_hndl,
											   tkvx4780_SELECT_CHANNEL,
											   tkvx4780_SELECT_CHANNEL,
											   tkvx4780_SELECT_CHANNEL,
											   tkvx4780_SELECT_CHANNEL,
											   tkvx4780_SELECT_ALL_CHANNELS,
											   tkvx4780_OUTPUT_STATE_ON)){
        	displayErr(sig_cond_hndl,result);
    	}
 		if (result = tkvx4780_setCoupling_grp3 (sig_cond_hndl, tkvx4780_SELECT_CHANNEL, tkvx4780_SELECT_CHANNEL, tkvx4780_SELECT_CHANNEL, tkvx4780_SELECT_CHANNEL, tkvx4780_SELECT_ALL_CHANNELS, tkvx4780_DC_COUPLE_STR)){
        	displayErr(sig_cond_hndl,result);
    	}
 		if (result = tkvx4780_setAttenuation_grp3 (sig_cond_hndl,
											   tkvx4780_SELECT_CHANNEL,
											   tkvx4780_SELECT_CHANNEL,
											   tkvx4780_SELECT_CHANNEL,
											   tkvx4780_SELECT_CHANNEL,
											   tkvx4780_SELECT_ALL_CHANNELS,
											   group3_attn)){
        	displayErr(sig_cond_hndl,result);
    	}
 		if (result = tkvx4780_setGain_grp3 (sig_cond_hndl,
										tkvx4780_SELECT_CHANNEL,
										tkvx4780_SELECT_CHANNEL,
										tkvx4780_SELECT_CHANNEL,
										tkvx4780_SELECT_CHANNEL,
										tkvx4780_SELECT_ALL_CHANNELS,
										group3_gain)){
        	displayErr(sig_cond_hndl,result);
   	 	}
 		if (result = tkvx4780_setFilterFreq_grp3 (sig_cond_hndl,
											  tkvx4780_SELECT_CHANNEL,
											  tkvx4780_SELECT_CHANNEL,
											  tkvx4780_SELECT_CHANNEL,
											  tkvx4780_SELECT_CHANNEL,
											  tkvx4780_SELECT_ALL_CHANNELS,
											  group3_cutoff)){
     		displayErr(sig_cond_hndl,result);
    	}
    }

//	Group 4 Settings
	if (group4_status==1) {
	
		if (group4_attn==0) group4_attn=tkvx4780_BYPASS_ATTEN;
 		if (group4_attn==10) group4_attn=tkvx4780_MIN_ATTEN;
 		if (group4_attn==100) group4_attn=tkvx4780_MAX_ATTEN;
 		
		if (group4_gain==1) group4_gain=tkvx4780_MIN_GAIN;
 		if (group4_gain==10) group4_gain=tkvx4780_GAIN_10;
 		if (group4_gain==100) group4_gain=tkvx4780_MAX_GAIN;
 		
  		if (result = tkvx4780_setInputState_grp4 (sig_cond_hndl,
											  tkvx4780_SELECT_CHANNEL,
											  tkvx4780_SELECT_CHANNEL,
											  tkvx4780_SELECT_CHANNEL,
											  tkvx4780_SELECT_CHANNEL,
											  tkvx4780_SELECT_ALL_CHANNELS,
											  tkvx4780_INPUT_STATE_ON)){
        	displayErr(sig_cond_hndl,result);
    	}
 		if (result = tkvx4780_setOutputState_grp4 (sig_cond_hndl,
											   tkvx4780_SELECT_CHANNEL,
											   tkvx4780_SELECT_CHANNEL,
											   tkvx4780_SELECT_CHANNEL,
											   tkvx4780_SELECT_CHANNEL,
											   tkvx4780_SELECT_ALL_CHANNELS,
											   tkvx4780_OUTPUT_STATE_ON)){
        	displayErr(sig_cond_hndl,result);
    	}
 		if (result = tkvx4780_setCoupling_grp4 (sig_cond_hndl,
												tkvx4780_SELECT_CHANNEL,
												tkvx4780_SELECT_CHANNEL,
												tkvx4780_SELECT_CHANNEL,
												tkvx4780_SELECT_CHANNEL,
												tkvx4780_SELECT_ALL_CHANNELS,
												tkvx4780_DC_COUPLE_STR)){
        	displayErr(sig_cond_hndl,result);
    	}
 		if (result = tkvx4780_setAttenuation_grp4 (sig_cond_hndl,
											   tkvx4780_SELECT_CHANNEL,
											   tkvx4780_SELECT_CHANNEL,
											   tkvx4780_SELECT_CHANNEL,
											   tkvx4780_SELECT_CHANNEL,
											   tkvx4780_SELECT_ALL_CHANNELS,
											   group4_attn)){
        	displayErr(sig_cond_hndl,result);
    	}
 		if (result = tkvx4780_setGain_grp4 (sig_cond_hndl,
										tkvx4780_SELECT_CHANNEL,
										tkvx4780_SELECT_CHANNEL,
										tkvx4780_SELECT_CHANNEL,
										tkvx4780_SELECT_CHANNEL,
										tkvx4780_SELECT_ALL_CHANNELS,
										group4_gain)){
        	displayErr(sig_cond_hndl,result);
   	 	}
 		if (result = tkvx4780_setFilterFreq_grp4 (sig_cond_hndl,
											  tkvx4780_SELECT_CHANNEL,
											  tkvx4780_SELECT_CHANNEL,
											  tkvx4780_SELECT_CHANNEL,
											  tkvx4780_SELECT_CHANNEL,
											  tkvx4780_SELECT_ALL_CHANNELS,
											  group4_cutoff)){
     		displayErr(sig_cond_hndl,result);
    	}
    }
}
