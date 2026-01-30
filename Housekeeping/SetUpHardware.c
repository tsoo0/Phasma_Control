#include 	<NIDAQmx.h>
#include 	<nivxi.h>
#include 	"TKTVS600.H"
#include 	"lif.h"
#include 	"Tkvx4780.h"			//	need this and its matching .c file for hardware		
#include 	"Tkvx4244.h"			//	ditto
#include 	"global_defines.h"
#include	"Joerger_routines.h"
#include	"keithley2400.h"
#include	"ep.h"
#include	"mwave.h"

void 		Set_PHASMA_Parameters(void);
void		Set_LEIA_Parameters(void);
void		Drate_parameters(void);
void 		displayErr(ViSession InstrHndl,ViStatus err); 	/* For Displaying Error Messages */ 
void        setup_4244(int digitizer_hndl,int group1rlength, double group1drate,int group2rlength, double group2drate,int group3rlength, double group3drate,int group4rlength, double group4drate, ViInt16 group1vltrange, ViInt16  group2vltrange,
			ViInt16 group3vltrange,ViInt16  group4vltrange,ViInt16 channel_one_status,ViInt16 channel_two_status, ViInt16 channel_three_status, ViInt16 channel_four_status, ViInt16 channel_five_status, ViInt16 channel_six_status, ViInt16 channel_seven_status,
			ViInt16 channel_eight_status, ViInt16 channel_nine_status, ViInt16 channel_ten_status, ViInt16 channel_eleven_status, ViInt16 channel_twelve_status, ViInt16 channel_thirteen_status, ViInt16 channel_fourteen_status, ViInt16 channel_fifteen_status,
			ViInt16 channel_sixteen_status);
void        setup_4780(int sig_cond_hndl,ViInt16 group1_status, ViInt32 group1_attn, ViInt32 group1_gain,ViInt32 group1_cutoff,ViInt16 group2_status, ViInt32 group2_attn,ViInt32 group2_gain,ViInt32 group2_cutoff,
			ViInt16 group3_status, ViInt32 group3_attn,ViInt32 group3_gain, ViInt32 group3_cutoff, ViInt16 group4_status, ViInt32 group4_attn, ViInt32 group4_gain,ViInt32 group4_cutoff);
extern int 	DeltaN_flag, LP_flag,LIF_flag,LIF_cont_flag,Mwave_flag,Bdot_flag,SuperProbe_flag,Estat_flag,EP_flag, Pulsed_flag,Joerger_flag,DyeLaserSwitch, PlasmaGun_flag,group4_flag;
extern int	bit_mask;
extern int	Mapping_result;
extern	ViSession  	sig_cond_hndl;
extern	ViSession 	digitizer_hndl;
extern	ViSession 	Second_digitizer_hndl;
extern int		 	TVS600_Handle;
extern double		group1_drate;
extern double		group2_drate;
extern double		group3_drate;
extern double		group4_drate;
extern double  		source_press;
extern void			Setup_TVS641(double, int, double, double); 

//Declare task handle for output card
TaskHandle 		OutputCardLIFTaskHandle;
TaskHandle 		OutputCardTrigHandle;

//This function sets up all the hardware flagged for use in a data acquisition sequence.

void SetUpNI_Card_LIF(void)
{
	double			LIF_scanning_drate;
	extern	double	LIF_timespan;
	extern  int		DAQ_System_Choice;

	//Create Task for measurement
	DAQmxCreateTask ("", &OutputCardLIFTaskHandle);
	
	//Choose which DAQ card to use for LIF
	if (DAQ_System_Choice) {
		//Use interlaced empty channels to take care of cross talk in between measurements as AtoD switching between very different values creates ghosting
		DAQmxCreateAIVoltageChan (OutputCardLIFTaskHandle, "Dev1/ai0:7", "Channel0,Channel1,Channel2,Channel3,Channel4,Channel5,Channel6,Channel7", DAQmx_Val_RSE, -5.0, 5.0, DAQmx_Val_Volts, "");
	} else {
		//Use interlaced empty channels to take care of cross talk in between measurements as AtoD switching between very different values creates ghosting
		DAQmxCreateAIVoltageChan (OutputCardLIFTaskHandle, "Dev2/ai0:7", "Channel0,Channel1,Channel2,Channel3,Channel4,Channel5,Channel6,Channel7", DAQmx_Val_RSE, -5.0, 5.0, DAQmx_Val_Volts, "");
	}
		
	//Set the timing between measurements to largest value possible to reduce ghosting effect. Not sure if this is working
	//DAQmxSetTimingAttribute (OutputCardLIFTaskHandle, DAQmx_DelayFromSampClk_DelayUnits, DAQmx_Val_Seconds);
	//DAQmxSetTimingAttribute (OutputCardLIFTaskHandle, DAQmx_DelayFromSampClk_Delay, (1.0/(group4_drate*8*4)));
		
	//Use onboard clock for digitization and set rate based on LIF continuous scan recording time
	LIF_scanning_drate=(double)LIF_cont_rlength/LIF_timespan;
	DAQmxCfgSampClkTiming (OutputCardLIFTaskHandle, "OnboardClock", LIF_scanning_drate, DAQmx_Val_Rising, DAQmx_Val_FiniteSamps, group4_rlength);   
		
	//To eliminate need for VXI to create a trigger pulse, code updated to initiate measurement immediately upon start of 
	//DAQ card task. 2/14/2017 - E.E.S.
	DAQmxDisableStartTrig (OutputCardLIFTaskHandle);
		
	//This code sets up DAQ card task to wait for external trigger. Removed for now to try immediate start of task 2/14/2017 - E.E.S.
	//DAQmxCfgDigEdgeStartTrig (OutputCardLIFTaskHandle, "/Dev1/PFI0", DAQmx_Val_Rising);
	//DAQmxSetTrigAttribute (OutputCardLIFTaskHandle, DAQmx_StartTrig_Type, DAQmx_Val_DigEdge);
}

void SetUpNI_Card_Trig(void)
{
	extern double	Pulse_length;		//Pulse length from pulsed operation control window
	extern  int		DAQ_System_Choice;
	
	if (Pulse_length < 0.001) Pulse_length = 100;  	//If pulse length not set, default it to 100 ms
		
	//Shutdown any old tasks
	DAQmxClearTask (OutputCardTrigHandle);

	//Setup I/O card for creation of trigger signal
	DAQmxCreateTask ("", &OutputCardTrigHandle);

	//Choose which DAQ card to use for trigger
	if (DAQ_System_Choice) {
		//create a user defined TTL output pulse using DAQ card
		DAQmxCreateCOPulseChanFreq (OutputCardTrigHandle, "Dev1/ctr1", "", DAQmx_Val_Hz, DAQmx_Val_Low, 0.0, .01, 0.01*Pulse_length/1000.0);
	} else {
		//create a user defined TTL output pulse using DAQ card
		DAQmxCreateCOPulseChanFreq (OutputCardTrigHandle, "Dev2/ctr1", "", DAQmx_Val_Hz, DAQmx_Val_Low, 0.0, .01, 0.01*Pulse_length/1000.0);
	}

	
	//Set up mode of timing to create a single pulse
	DAQmxCfgImplicitTiming (OutputCardTrigHandle, DAQmx_Val_FiniteSamps, 1);
}

void SetUpHardware(void)
{
	int result; 		//	Error-checking variable
	extern double 	TVS600_Amp;
	extern double	TVS600_digitization_freq;
	extern double	TVS600_trigger_pos;
		
	//Set operational parameters (defaults if not set already)
	Set_PHASMA_Parameters();

	//Set up microwave source and DAQ rate if selected
	if (Mwave_flag) {
		Mwave_setup();
	}
	
	//  Setup the LIF system with the default values.
	if ( LIF_flag || LIF_cont_flag)  { 

			//Set up LIF parameters including output card timestep
			LIF_Parameter_Setup(1);
		
			//Set up Lockin amplifier
			LIF_Lockin_Setup();
	}	

	//Read in DAQ rates for digitizers from panel
	Drate_parameters();

	//Setup Nat Instrum card for global trigger creation
	SetUpNI_Card_Trig();	

	//Setup Nat Instrum card for continuous LIF since we want to digitize slowly and remap internal trigger line to external port on controller
	if (LIF_cont_flag) {
		SetUpNI_Card_LIF();	
	}
	
//	Set up hardware that is not already setup	
	if (Estat_flag || PlasmaGun_flag || Mwave_flag) {
		
		// 	Find VX4780 Signal Conditioner, Get Its ID and Reset It
		if (!sig_cond_hndl) {
			if ((result = tkvx4780_autoConnectToFirst (&sig_cond_hndl))<0){
         		displayErr(sig_cond_hndl,result);
    		}
    	}

		// 	Find VX4244 Digitizer, Get Its ID, and Reset It
		if (!digitizer_hndl) {
			if ((result=tkvx4244_autoConnectToLA (&digitizer_hndl, 56))<0){
        		displayErr(digitizer_hndl,result);
    		}
    	}

		// Set up second VX4244 Digitizer since it is in the crate, get its ID, and Reset It
		if (!Second_digitizer_hndl) {
			if ((result=tkvx4244_autoConnectToLA (&Second_digitizer_hndl, 7))<0){
        		displayErr(Second_digitizer_hndl,result);
    		}
    	}
		
		//  Setup Signal Conditioner Inputs, Outputs,Frequency Corners, etc. for all 
		setup_4780 (sig_cond_hndl, 1, group1_filter_atten, group1_filter_gain,
			group1_filter_cutoff, 1, group2_filter_atten, group2_filter_gain,
			group2_filter_cutoff, 1, group3_filter_atten, group3_filter_gain,
			group3_filter_cutoff, 1, group4_filter_atten, group4_filter_gain,
			group4_filter_cutoff);

		//	Setup Digitizer, turning on all channels for respective channels in use
		setup_4244 (digitizer_hndl, group1_rlength, group1_drate, group2_rlength, group2_drate,
			group3_rlength, group3_drate, group4_rlength, group4_drate, group1_vltrange,
			group2_vltrange, group3_vltrange, group4_vltrange, PlasmaGun_flag, PlasmaGun_flag, 
			PlasmaGun_flag, PlasmaGun_flag, PlasmaGun_flag,PlasmaGun_flag, PlasmaGun_flag, 
			PlasmaGun_flag, Mwave_flag,Mwave_flag,Estat_flag, Estat_flag,0,0,0,0);		 

		//	Setup Second Digitizer, turning on all channels 
		setup_4244 (Second_digitizer_hndl, group1_rlength, group1_drate, group2_rlength, group2_drate,
			group3_rlength, group3_drate, group4_rlength, group4_drate, group1_vltrange,
			group2_vltrange, group3_vltrange, group4_vltrange, 1, 1, 
			1, 1, 1,1, 1, 1, 1,1,1, 1,0,0,0,0);		 

		bit_mask=0;
		if (PlasmaGun_flag) bit_mask=bit_mask+1;	   //Check if first group is complete
		if (PlasmaGun_flag) bit_mask=bit_mask+2;	   //Check if second group is complete
		if ((Estat_flag) || (Mwave_flag) ) bit_mask=bit_mask+4;  //If Estat or Microwave in use, check if third group is complete
		if (group4_flag) bit_mask=bit_mask+8;	// A Bit Mask of 15 Means the First 4 bits (measurements 1, 2, 3 & 4) are Complete
	}
	
	//Set up Joerger Instrument
	if (Joerger_flag) {
		Joerger_setup();
		JoergerVTR10014_setup();
	}
	
	//	Setup Sourcemeter 
    if (LP_flag) {
		setup_keithley ();
	}
	
    if (DeltaN_flag) {
		setup_keithley ();
	}
	
	//unmap trigger to avoid error then map if necessary
	//UnMapTrigToTrig (1, 1, 41 );   //was used when VXI crate created the trigger
	//UnMapTrigToTrig (1, 40, 0);
	
	//Take external trigger from NI card wired into the input trigger on the VXI crate controller and use that to trigger all the instruments
	if (PlasmaGun_flag || Estat_flag || Mwave_flag ||Joerger_flag) {
		
		//MapTrigToTrig (1, 1, 41, 0);  //was used to map internal trigger to outside
		
		//Map external trigger to internal trigger line TTL 1
		//if (!(Mapping_result==0)) {
			//Mapping_result = MapTrigToTrig (1, 40, 0, 0);
		//}
		
		//Set ext trigger port on controller to low
		//SrcTrig (1, 41, 1, 100); 
	}
}

