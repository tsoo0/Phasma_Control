	/*Thomson Scattering Code Process
	Step 0
	If and only if the activate switch is flipped on the main control panel, launch the LabView executable 
	that runs the lasers in the background on a slow simmer. The delay between the Q-switch and the Rigol 
	is stretched out so the lasers are firing at pretty low power. When the activate switch is turned off, 
	turn off the background simmer sequence? Will this return the lasers to full power? Probably not if 
	deactivating them does not turn on any external pulse sequencing. Better make sure an errant master trigger 
	pulse cannot turn the lasers back on. Perhaps overwrite the flashlamp and Qswitch trigger amplitudes to zero 
	Volts at this step? If so, then when activated, have to return them to 1 volt.
	
	Step 1
	When the TS settings window is opened and then the values saved, read all the pulse frequencies, delay times, 
	etc. from the front panel. The time to fire the laser in the middle of the bias pulse could be set here or 
	in the master trigger panel. In the code, we will subtract off the flashlamp-to-Q-switch delay, the intrinsic 
	Fire to Qswitch delay, and the circuit response delay. So the actual trigger time gives us a Q-switch pulse at 
	the desired time.  This trigger time will also appear in the master timing trigger plan. The plan will be to set 
	them in both places and no matter what, the code updates them in both places. We will call the laser firing time 
	"time from bias trigger for laser firing." We will also define a new trigger time which will be the Andor camera 
	shutter trigger time. By creating this third trigger, we separate out the firing and gas puff sequence of PHASMA 
	from the camera shutter. At this step in the code we will also calculate the total number of pulses that the 
	Agilent will produce based on the time from the master trigger panel to the gas puff and then the time from the 
	gas puff trigger to the bias pulse trigger time. For example, if those values are 30 seconds plus 36.2 ms, we would 
	program the Agilent to generate 300 pulses. That would leave a .0362 second gap between the last flashlamp pulse and 
	the bias pulse. Note, need to make sure that the last of the pulse train and the "final" pulse are still about 100 
	ms apart to give capacitors time to charge. This may mean backing up or advancing the T0 trigger to get the right 
	number of pulses to line up. Here also read the visa addresses of all the instruments and set up the acquisition 
	parameters for the Rigol Scope that records the laser trigger signal and the laser power from the photodiodes on the 
	optical table and on top of pumping station. Set this scope to trigger using a digital D0 trigger. Also set up the 
	parameters for the iCCD camera including different DDG delays for the parallel and perpendicular TS systems based on 
	how far the Q-synch signals have to travel to the camera. Note here in the panel remind the user to physically swap 
	the Q-synch cables until they are permanently coupled through diodes (will need to check that combining the Q-synch 
	signals does not introduce an unacceptable delay in triggering the Andor camera)
	
	Here also have the option to move the central wavelength of the small McPherson spectrometer so you can look at 
	different parts of the TS spectrum if desired.

	Step 2
	In the arming step of the PHASMA cycle sequence send the timing information to the Agilent and Rigol delay generators 
	for the laser chosen (parallel or perp). Could set them both as a matter of course and then it just matters which laser 
	is turned on for which one fires. Essentially these commands will be to set up the number of pulses to be created by the 
	Agilent system and at what frequency, the delay for the Rigol DG1022Z between an external trigger it receives and the 
	pulse it sends out the front (this will be the optimal delay to maximize the laser power).

	Still in the arming step, write a string to the file read by the other TS code to tell the laser to switch from background 
	simmer mode to be in the "experimental mode" In-EXP. This other code runs continuously in the background and changes the 
	timing settings on the Agilent and Rigol devices on both the parallel and perpendicular TS lasers. The timing pulses for 
	the QSmart1500 are teed off to the LIF Q850 laser on the same table
	
	Also still in the arming step, write all the TS system parameters to the MDS system using the same tree paths as Peiyun set up.

	Step 3
	Arm the Agilent and the DG1022Z (no arming of the SRS DG645 as we are assuming here that it has been pulled out of the system). 
	Arm the Rigol Oscilloscope and the Andor camera.
	
	Step 4
	Now in the primary trigger sequence of the PHASMA cycle. Switch the mode of the laser by updating the string in the file 
	to tell the background TS code to stop. Then fire the master trigger T0. This will start the many hundred pulse sequence 
	of the Agilent. The laser stops firing when the last of the pulse train hits. Concurrent with the gas puff trigger, send a 
	different trigger we will call "Andor Camera Shutter Trigger" that lasts for 150 ms (long enough to cover the gas puff and 
	the entire plasma gun sequence). This same trigger could be used to trigger the photron camera shutter. Fire the gas puff, 
	fire the arc pulse, and then fire the "Laser-Firing" trigger, and then fire the bias pulse trigger. Given all the delays 
	that have to be built into the laser firing time trigger (274+60+1=335 microseconds), it will have to come before the roughly 
	100 microsecond long bias pulse begins. When the laser actually fires (the Q-synch signal), that trigger is combined with the 
	Andor Camera Shutter Trigger (which is still held high) and the iCCD camera fires after the programmed DDG delay of hundreds 
	of ns and then is held open for 50 ns or so.
	
	Step 4
	After the pulse ends, write the string to the file again so the laser simmer code starts running again.
	ove the Rigol oscilloscope data to the PC and store a plaintext version and push the Rigol data to MDS.
	Create a wavelength array based on the parameters of the small McPherson spectrometer. Move the Andor data to the PC. 
	Store the wavelength array and the Andor data in a plaintext file and push those data to MDS.

	Step 5
	TS process ends.
*/


#include	"ATMCD32D.h"  
#include 	<utility.h>
#include 	<ansi_c.h>
#include	<visa.h>
#include	<stdlib.h>
#include 	<formatio.h>
#include	"PHASMA-MDS-Routines.h"
#include 	"ThomsonScatteringSystems.h"
#include 	"ThomsonScatteringSettings.h"  
#include	"GlobalVariables.h"
#include	"Rigol.h"
#include 	"MasterTriggerSystem.h"
#include 	<userint.h>
#include	"PHASMA_C_to_Python_convertor.h"

#define		agilent_highlevel 5.0   	//Set square wave pulse amplitude to 5 V	
#define		agilent_lowlevel 0.0		//Set square wave pulse low to 0 V	
#define		agilent_pulsewidth 350.0	//Set square wave pulse length to 350 microseconds based on Peiyun's LabView code		

#define		rigol_pulsewidth 120.0		//Set Rigol frequency to 4.1666 kHz		
#define		rigol_amplitude 4.0					
#define		rigol_offset 0.0				


int			ThomsonScattering_panel;
int			ThomsonScattering_setup_state = 55;
int			TS_Para_Perp_flag=0; 
int			TS_RecordLength; 
int			Andor_H_bin;
int			Andor_V_bin;
int			Q1500_QSW_Source;
int			Q850_QSW_Source;
int			Andor_Gain;
int			Andor_Pre_Amp;

float		Andor_Exp_Time;
float		Andor_DDGDelay;
float		Andor_GateWidth;
float		Q1500_DDG_Delay;
float		Q850_DDG_Delay;
float		Laser_Firing_Time;
float		Q1500_Frequency;
float		Q1500_Warmup;
float 		Q1500_InstrinsicDelay;
float		Q1500_Q_delay;
float		Q1500_Simmer_delay;
float		Q1500_Circuit_Delay;
float		Q1500_Trigger_Time;
float		Q850_Frequency;
float		Q850_Warmup;
float 		Q850_InstrinsicDelay;
float		Q850_Q_delay;
float		Q850_Simmer_delay;
float		Q850_Circuit_Delay;
float		Q850_Trigger_Time;
float		TS_TimeSpan;
float		McP_Current_Wlength;
float		McP_GrooveDen;
float		McP_AngleDiff;
float		McP_FocalLength;

char		agilent_string[30];
char		rigol_string[30];
char		TS_Agilent_Add[64]; 
char		TS_Rigol_Add[64]; 
char		TS_IP_Address_string[64];

ViSession 	ThomsonScatteringRigol_handle;
ViSession	Agilent_handle;
ViSession	Rigol_handle;


//Set up the TS system infrastructure
void ThomsonScatteringLaserControl(int simmermode)
{
	int 		result; 					//	Error-checking variable
	int			num_pulses;
	float 		SimmerSwitchDelay;
	float		duty_cycle;
	char		QSmart1500_Lamp_IP_Address_string[64]; 
	char		QSmart1500_Qsw_IP_Address_string[64]; 
	char		TS_Perp_Agilent_Add[64]; 
	char		TS_Perp_Rigol_Add[64]; 
	char		QSmart850_Lamp_IP_Address_string[64]; 
	char		QSmart850_Qsw_IP_Address_string[64]; 
	char		ThomsonScattering_string[32]="Null";

	ViUInt32 	count;

	//Open up TS settings panel and get settings
	ThomsonScattering_panel = LoadPanel (0, "ThomsonScatteringSettings.uir", ThomsonSct);
	RecallPanelState (ThomsonScattering_panel, "Master_Control_Storage_File", ThomsonScattering_setup_state);
	   
	//Get parallel or perp choice state
 	GetCtrlVal (ThomsonScattering_panel, ThomsonSct_OrientationSwitch,&TS_Para_Perp_flag);

	//Get simmer swithc delay time
 	GetCtrlVal (ThomsonScattering_panel, ThomsonSct_SimmerSwitch,&SimmerSwitchDelay);

	//Depending on orientation choice, get relevant settings from interface and set up Agilent function generator and Rigol Delay generator
	if (TS_Para_Perp_flag) {   //Perp choice
		
		//Get Laser warmup time
	 	GetCtrlVal (ThomsonScattering_panel, ThomsonSct_Q1500_Warmup,&Q1500_Warmup);
		
		//Get Laser pulse frequency
	 	GetCtrlVal (ThomsonScattering_panel, ThomsonSct_Q1500_Frequency,&Q1500_Frequency);
		
		//Get Laser intrinsic delay
	 	GetCtrlVal (ThomsonScattering_panel, ThomsonSct_Q1500_InstrinsicDelay,&Q1500_InstrinsicDelay);
		
		//Get Laser Q switch delay
	 	GetCtrlVal (ThomsonScattering_panel, ThomsonSct_Q1500_Q_delay,&Q1500_Q_delay);
		
		//Get Laser Simmer delay
	 	GetCtrlVal (ThomsonScattering_panel, ThomsonSct_Q1500_Simmer_Delay,&Q1500_Simmer_delay);
		
		//Get Laser circuit delay
	 	GetCtrlVal (ThomsonScattering_panel, ThomsonSct_Q1500_Circuit_Delay,&Q1500_Circuit_Delay);
		
		//Get Laser trigger source flag
	 	GetCtrlVal (ThomsonScattering_panel, ThomsonSct_Q1500_QSW_Source,&Q1500_QSW_Source);
			
		//Calculate laser firing time to be used in various places
		Laser_Firing_Time = (Q1500_Trigger_Time- Q1500_InstrinsicDelay- Q1500_Q_delay -Q1500_Circuit_Delay)/1.0E6;

		
		/***********************************************************************************************/
		//Set up Rigol for laser Q-switch control 

		//Get Rigol Address String
	 	GetCtrlVal (ThomsonScattering_panel, ThomsonSct_TS_Perp_Rigol_Add,TS_Perp_Rigol_Add);

		//Open visa session to Rigol delay Generator
		result=viOpen (Global_Visa_Session_Handle, TS_Perp_Rigol_Add, VI_NULL, VI_NULL, &Rigol_handle);
		
		if (!result) {
			//Turn on channel 1
			sprintf (rigol_string, ":OUTP1 ON\r");   
			result = viWrite (Rigol_handle, rigol_string, (unsigned int)strlen(rigol_string), VI_NULL);
			
			//Turn on channel 2
			sprintf (rigol_string, ":OUTP2 ON\r");   
			result = viWrite (Rigol_handle, rigol_string, (unsigned int)strlen(rigol_string), VI_NULL);
			
			//Set the frequency, peak to peak ammplitude, and offset for channel 1. If Vpp=4, offset by 2 volts to go 4 V to 0V
			sprintf (rigol_string, ":SOUR1:APPL:SQU %f,%f,%f\r", (1.0/(2*rigol_pulsewidth/1000000.0)), rigol_amplitude, rigol_offset);   
			result = viWrite (Rigol_handle, rigol_string, (unsigned int)strlen(rigol_string), VI_NULL);
			
			//Set the frequency, peak to peak ammplitude, and offset for channel 2. If Vpp=4, offset by 2 volts to go 4 V to 0V
			sprintf (rigol_string, ":SOUR2:APPL:SQU %f,%f,%f\r", (1.0/(2*rigol_pulsewidth/1000000.0)), rigol_amplitude, rigol_offset);   
			result = viWrite (Rigol_handle, rigol_string, (unsigned int)strlen(rigol_string), VI_NULL);
			
			//Turn on burst mode on channel 1
			sprintf (rigol_string, ":SOUR1:BURS ON\r");   
			result = viWrite (Rigol_handle, rigol_string, (unsigned int)strlen(rigol_string), VI_NULL);
			
			//Turn on burst mode on channel 2
			sprintf (rigol_string, ":SOUR2:BURS ON\r");   
			result = viWrite (Rigol_handle, rigol_string, (unsigned int)strlen(rigol_string), VI_NULL);
			
			//Turn on burst mode on channel 1 to be triggered
			sprintf (rigol_string, ":SOUR1:BURS:MODE TRIG\r");   
			result = viWrite (Rigol_handle, rigol_string, (unsigned int)strlen(rigol_string), VI_NULL);
			
			//Turn on burst mode on channel 2 to be triggered
			sprintf (rigol_string, ":SOUR2:BURS:MODE TRIG\r");   
			result = viWrite (Rigol_handle, rigol_string, (unsigned int)strlen(rigol_string), VI_NULL);
			
			//Set number of cycles on burst mode on channel 1 to be 1 so one pulse generated for every trigger pulse
			sprintf (rigol_string, ":SOUR1:BURS:NCYC 1\r");   
			result = viWrite (Rigol_handle, rigol_string, (unsigned int)strlen(rigol_string), VI_NULL);
			
			//Set number of cycles on burst mode on channel 2 to be 1 so one pulse generated for every trigger pulse
			sprintf (rigol_string, ":SOUR2:BURS:NCYC 1\r");   
			result = viWrite (Rigol_handle, rigol_string, (unsigned int)strlen(rigol_string), VI_NULL);
			
			//Turn on trigger source on channel 1 to be external
			sprintf (rigol_string, ":SOUR1:BURS:TRIG:SOUR EXT\r");   
			result = viWrite (Rigol_handle, rigol_string, (unsigned int)strlen(rigol_string), VI_NULL);
			
			//Turn on trigger source on channel 2 to be external
			sprintf (rigol_string, ":SOUR2:BURS:TRIG:SOUR EXT\r");   
			result = viWrite (Rigol_handle, rigol_string, (unsigned int)strlen(rigol_string), VI_NULL);

			//Set trigger slope on channel 1 to be positive
			sprintf (rigol_string, ":SOUR1:BURS:TRIG:TRIGO POS\r");   
			result = viWrite (Rigol_handle, rigol_string, (unsigned int)strlen(rigol_string), VI_NULL);
			
			//Set trigger slope on channel 2 to be positive
			sprintf (rigol_string, ":SOUR2:BURS:TRIG:TRIGO POS\r");   
			result = viWrite (Rigol_handle, rigol_string, (unsigned int)strlen(rigol_string), VI_NULL);
			
			//Set time delay between external trigger received and output signal for channel 2 to be the sum of the intrinsic
			//laser trigger time plus the user requested delay to optimize laser power if in regular power mode. If
			//not, switch to simmer mode time delay. Time in seconds.
			if (!simmermode) {
				sprintf (rigol_string, ":SOUR1:BURS:TDEL %f\r",((Q1500_InstrinsicDelay+Q1500_Q_delay)/1000000.0));   
				result = viWrite (Rigol_handle, rigol_string, (unsigned int)strlen(rigol_string), VI_NULL);
			} else {
				sprintf (rigol_string, ":SOUR1:BURS:TDEL %f\r",((Q1500_InstrinsicDelay+Q1500_Simmer_delay)/1000000.0));   
				result = viWrite (Rigol_handle, rigol_string, (unsigned int)strlen(rigol_string), VI_NULL);
			}	
			
			//Set time delay between external trigger received and output signal for channel 2 to be the sum of the intrinsic
			//laser trigger time plus the user requested delay to optimize laser power if in regular power mode. If
			//not, switch to simmer mode time delay. Time in seconds.
			if (!simmermode) {
				sprintf (rigol_string, ":SOUR2:BURS:TDEL %f\r",((Q1500_InstrinsicDelay+Q1500_Q_delay)/1000000.0));   
				result = viWrite (Rigol_handle, rigol_string, (unsigned int)strlen(rigol_string), VI_NULL);
			} else {
				sprintf (rigol_string, ":SOUR2:BURS:TDEL %f\r",((Q1500_InstrinsicDelay+Q1500_Simmer_delay)/1000000.0));   
				result = viWrite (Rigol_handle, rigol_string, (unsigned int)strlen(rigol_string), VI_NULL);
			}	
			
			//Make double sure that channel 1 is on
			sprintf (rigol_string, ":OUTP1 ON\r");   
			result = viWrite (Rigol_handle, rigol_string, (unsigned int)strlen(rigol_string), VI_NULL);
			
			//Make double sure that channel 2 is on 
			sprintf (rigol_string, ":OUTP2 ON\r");   
			result = viWrite (Rigol_handle, rigol_string, (unsigned int)strlen(rigol_string), VI_NULL);
			
		}
		
		/***********************************************************************************************/
		//Set up Agilent for laser flashlamp control
	 	
		//Get Agilent Address String
	 	GetCtrlVal (ThomsonScattering_panel, ThomsonSct_TS_Perp_Agilent_Add,TS_Agilent_Add);

		//Open visa session to Agilent Function Generator
		result=viOpen (Global_Visa_Session_Handle, TS_Perp_Agilent_Add, VI_NULL, VI_NULL, &Agilent_handle);
		
		if(!result) {		
			//Set square wave high
			sprintf (agilent_string, ":VOLT:HIGH %f\r", agilent_highlevel);   
			result = viWrite (Agilent_handle, agilent_string, (unsigned int)strlen(agilent_string), VI_NULL);

			//Set square wave low
			sprintf (agilent_string, ":VOLT:LOW %f\r", agilent_lowlevel);   
			result = viWrite (Agilent_handle, agilent_string, (unsigned int)strlen(agilent_string), VI_NULL);

			//Set square wave frequency
			sprintf (agilent_string, ":FREQ %f\r", Q1500_Frequency);   
			result = viWrite (Agilent_handle, agilent_string, (unsigned int)strlen(agilent_string), VI_NULL);

			//Set square wave duty cycle
			duty_cycle=(agilent_pulsewidth/1000000.0)*Q1500_Frequency*100;
			sprintf (agilent_string, "FUNC:PULSe:DCYCLe %f\r", duty_cycle);   
			result = viWrite (Agilent_handle, agilent_string, (unsigned int)strlen(agilent_string), VI_NULL);

			//Set trigger to external
			sprintf (agilent_string, "TRIG:SOUR EXT\r");   
			result = viWrite (Agilent_handle, agilent_string, (unsigned int)strlen(agilent_string), VI_NULL);

	 		//Set number of laser pulses to create for warm up phase
			num_pulses=(int)Q1500_Frequency*Q1500_Warmup;
			sprintf (agilent_string, "BURST:NCYC %d\r", num_pulses);   
			result = viWrite (Agilent_handle, agilent_string, (unsigned int)strlen(agilent_string), VI_NULL);

	 		//Turn on Burst state if not in simmermode
			if (!simmermode) {
				sprintf (agilent_string, "BURST:STATE ON\r");   
				result = viWrite (Agilent_handle, agilent_string, (unsigned int)strlen(agilent_string), VI_NULL);
			} else {
				Delay(SimmerSwitchDelay);	//Delay switch over to give flashlamps time to decay
				sprintf (agilent_string, "BURST:STATE OFF\r");   
				result = viWrite (Agilent_handle, agilent_string, (unsigned int)strlen(agilent_string), VI_NULL);
			}
			
			//Get laser specific Rigol Scope address
			GetCtrlVal (ThomsonScattering_panel, ThomsonSct_RIGOL_PerpScope, TS_IP_Address_string);
		}

	} else { //Parallel choice
		
		//Get Laser pulse frequency
	 	GetCtrlVal (ThomsonScattering_panel, ThomsonSct_Q850_Warmup,&Q850_Warmup);
		
		//Get Laser pulse frequency
	 	GetCtrlVal (ThomsonScattering_panel, ThomsonSct_Q850_Frequency,&Q850_Frequency);
		
		//Get Laser intrinsic delay
	 	GetCtrlVal (ThomsonScattering_panel, ThomsonSct_Q850_InstrinsicDelay,&Q850_InstrinsicDelay);
		
		//Get Laser Q switch delay
	 	GetCtrlVal (ThomsonScattering_panel, ThomsonSct_Q850_Q_delay,&Q850_Q_delay);
		
		//Get Laser Simmer delay
	 	GetCtrlVal (ThomsonScattering_panel, ThomsonSct_Q850_Simmer_Delay,&Q850_Simmer_delay);

		//Get Laser circuit delay
	 	GetCtrlVal (ThomsonScattering_panel, ThomsonSct_Q850_Circuit_Delay,&Q850_Circuit_Delay);
		
		//Get Laser trigger source flag
	 	GetCtrlVal (ThomsonScattering_panel, ThomsonSct_Q850_QSW_Source,&Q850_QSW_Source);
		
		//Get Andor camera delay time
	 	GetCtrlVal (ThomsonScattering_panel, ThomsonSct_Q850_DDG_Delay,&Q850_DDG_Delay);
		
		//Calculate laser firing time to be used in various places
		Laser_Firing_Time = (Q850_Trigger_Time- Q850_InstrinsicDelay- Q850_Q_delay -Q850_Circuit_Delay)/1.0E6;

		
		/***********************************************************************************************/
		//Set up Rigol for laser Q-switch control 
 		
		//Get Rigol Address String
	 	GetCtrlVal (ThomsonScattering_panel, ThomsonSct_TS_Para_Rigol_Add,TS_Rigol_Add);

		//Open visa session to Rigol delay Generator
		result=viOpen (Global_Visa_Session_Handle, TS_Rigol_Add, VI_NULL, VI_NULL, &Rigol_handle);
		
		if(!result) {
			//Turn on channel 1
			sprintf (rigol_string, ":OUTP1 ON\r");   
			result = viWrite (Rigol_handle, rigol_string, (unsigned int)strlen(rigol_string), VI_NULL);
			
			//Turn on channel 2
			sprintf (rigol_string, ":OUTP2 ON\r");   
			result = viWrite (Rigol_handle, rigol_string, (unsigned int)strlen(rigol_string), VI_NULL);
			
			//Set the frequency, peak to peak ammplitude, and offset for channel 1. If Vpp=4, offset by 2 volts to go 4 V to 0V
			sprintf (rigol_string, ":SOUR1:APPL:SQU %f,%f,%f\r", (1.0/(2*rigol_pulsewidth/1000000.0)), rigol_amplitude, rigol_offset);   
			result = viWrite (Rigol_handle, rigol_string, (unsigned int)strlen(rigol_string), VI_NULL);
			
			//Set the frequency, peak to peak ammplitude, and offset for channel 2. If Vpp=4, offset by 2 volts to go 4 V to 0V
			sprintf (rigol_string, ":SOUR2:APPL:SQU %f,%f,%f\r", (1.0/(2*rigol_pulsewidth/1000000.0)), rigol_amplitude, rigol_offset);   
			result = viWrite (Rigol_handle, rigol_string, (unsigned int)strlen(rigol_string), VI_NULL);
			
			//Turn on burst mode on channel 1
			sprintf (rigol_string, ":SOUR1:BURS ON\r");   
			result = viWrite (Rigol_handle, rigol_string, (unsigned int)strlen(rigol_string), VI_NULL);
			
			//Turn on burst mode on channel 2
			sprintf (rigol_string, ":SOUR2:BURS ON\r");   
			result = viWrite (Rigol_handle, rigol_string, (unsigned int)strlen(rigol_string), VI_NULL);
			
			//Turn on burst mode on channel 1 to be triggered
			sprintf (rigol_string, ":SOUR1:BURS:MODE TRIG\r");   
			result = viWrite (Rigol_handle, rigol_string, (unsigned int)strlen(rigol_string), VI_NULL);
			
			//Turn on burst mode on channel 2 to be triggered
			sprintf (rigol_string, ":SOUR2:BURS:MODE TRIG\r");   
			result = viWrite (Rigol_handle, rigol_string, (unsigned int)strlen(rigol_string), VI_NULL);
			
			//Set number of cycles on burst mode on channel 1 to be 1 so one pulse generated for every trigger pulse
			sprintf (rigol_string, ":SOUR1:BURS:NCYC 1\r");   
			result = viWrite (Rigol_handle, rigol_string, (unsigned int)strlen(rigol_string), VI_NULL);
			
			//Set number of cycles on burst mode on channel 2 to be 1 so one pulse generated for every trigger pulse
			sprintf (rigol_string, ":SOUR2:BURS:NCYC 1\r");   
			result = viWrite (Rigol_handle, rigol_string, (unsigned int)strlen(rigol_string), VI_NULL);
			
			//Turn on trigger source on channel 1 to be external
			sprintf (rigol_string, ":SOUR1:BURS:TRIG:SOUR EXT\r");   
			result = viWrite (Rigol_handle, rigol_string, (unsigned int)strlen(rigol_string), VI_NULL);
			
			//Turn on trigger source on channel 2 to be external
			sprintf (rigol_string, ":SOUR2:BURS:TRIG:SOUR EXT\r");   
			result = viWrite (Rigol_handle, rigol_string, (unsigned int)strlen(rigol_string), VI_NULL);

			//Set trigger slope on channel 1 to be positive
			sprintf (rigol_string, ":SOUR1:BURS:TRIG:TRIGO POS\r");   
			result = viWrite (Rigol_handle, rigol_string, (unsigned int)strlen(rigol_string), VI_NULL);
			
			//Set trigger slope on channel 2 to be positive
			sprintf (rigol_string, ":SOUR2:BURS:TRIG:TRIGO POS\r");   
			result = viWrite (Rigol_handle, rigol_string, (unsigned int)strlen(rigol_string), VI_NULL);
			
			//Set time delay between external trigger received and output signal for channel 2 to be the sum of the intrinsic
			//laser trigger time plus the user requested delay to optimize laser power if in regular power mode. If
			//not, switch to simmer mode time delay. Time in seconds.
			if (!simmermode) {
				sprintf (rigol_string, ":SOUR1:BURS:TDEL %f\r",((Q850_InstrinsicDelay+Q850_Q_delay)/1000000.0));   
				result = viWrite (Rigol_handle, rigol_string, (unsigned int)strlen(rigol_string), VI_NULL);
			} else {
				sprintf (rigol_string, ":SOUR1:BURS:TDEL %f\r",((Q850_InstrinsicDelay+Q850_Simmer_delay)/1000000.0));   
				result = viWrite (Rigol_handle, rigol_string, (unsigned int)strlen(rigol_string), VI_NULL);
			}	

			//Set time delay between external trigger received and output signal for channel 2 to be the sum of the intrinsic
			//laser trigger time plus the user requested delay to optimize laser power if in regular power mode. If
			//not, switch to simmer mode time delay. Time in seconds.
			if (!simmermode) {
				sprintf (rigol_string, ":SOUR2:BURS:TDEL %f\r",((Q850_InstrinsicDelay+Q850_Q_delay)/1000000.0));   
				result = viWrite (Rigol_handle, rigol_string, (unsigned int)strlen(rigol_string), VI_NULL);
			} else {
				sprintf (rigol_string, ":SOUR2:BURS:TDEL %f\r",((Q850_InstrinsicDelay+Q850_Simmer_delay)/1000000.0));   
				result = viWrite (Rigol_handle, rigol_string, (unsigned int)strlen(rigol_string), VI_NULL);
			}	
						
			//Make double sure that channel 1 is on
			sprintf (rigol_string, ":OUTP1 ON\r");   
			result = viWrite (Rigol_handle, rigol_string, (unsigned int)strlen(rigol_string), VI_NULL);
			
			//Make double sure that channel 2 is on 
			sprintf (rigol_string, ":OUTP2 ON\r");   
			result = viWrite (Rigol_handle, rigol_string, (unsigned int)strlen(rigol_string), VI_NULL);
		}		
		/***********************************************************************************************/
		//Set up Agilent for laser flashlamp control

		//Get Agilent Address String
	 	GetCtrlVal (ThomsonScattering_panel, ThomsonSct_TS_Para_Agilent_Add,TS_Agilent_Add);
		
		//Open visa session to Agilent Function Generator
		result=viOpen (Global_Visa_Session_Handle, TS_Agilent_Add, VI_NULL, VI_NULL, &Agilent_handle);
		
		if (!result) {
			//Set square wave high
			sprintf (agilent_string, ":VOLT:HIGH %f\r", agilent_highlevel);   
			result = viWrite (Agilent_handle, agilent_string, (unsigned int)strlen(agilent_string), VI_NULL);

			//Set square wave low
			sprintf (agilent_string, ":VOLT:LOW %f\r", agilent_lowlevel);   
			result = viWrite (Agilent_handle, agilent_string, (unsigned int)strlen(agilent_string), VI_NULL);

			//Set square wave frequency
			sprintf (agilent_string, ":FREQ %f\r", Q850_Frequency);   
			result = viWrite (Agilent_handle, agilent_string, (unsigned int)strlen(agilent_string), VI_NULL);

			//Set square wave duty cycle
			duty_cycle=(agilent_pulsewidth/1000000.0)*Q850_Frequency*100;
			sprintf (agilent_string, "FUNC:PULSE:DCYCLE %f\r", duty_cycle);   
			result = viWrite (Agilent_handle, agilent_string, (unsigned int)strlen(agilent_string), VI_NULL);

			//Set trigger to external
			sprintf (agilent_string, "TRIG:SOUR EXT\r");   
			result = viWrite (Agilent_handle, agilent_string, (unsigned int)strlen(agilent_string), VI_NULL);

			//Set number of laser pulses to create for warm up phase
			num_pulses=(int)Q850_Frequency*Q850_Warmup;
			sprintf (agilent_string, "BURST:NCYC %d\r", num_pulses);   
			result = viWrite (Agilent_handle, agilent_string, (unsigned int)strlen(agilent_string), VI_NULL);

	 		//Turn on Burst state if not in simmermode
			if (!simmermode) {
				sprintf (agilent_string, "BURST:STATE ON\r");   
				result = viWrite (Agilent_handle, agilent_string, (unsigned int)strlen(agilent_string), VI_NULL);
			} else {
				Delay(SimmerSwitchDelay);	//Delay switch over to give flashlamps time to decay
				sprintf (agilent_string, "BURST:STATE OFF\r");   
				result = viWrite (Agilent_handle, agilent_string, (unsigned int)strlen(agilent_string), VI_NULL);
			}
		}

		//Get laser specific Rigol Scope address
		GetCtrlVal (ThomsonScattering_panel, ThomsonSct_RIGOL_ParaScope, TS_IP_Address_string);
	}	
	//Discard housekeeping panel
	DiscardPanel(ThomsonScattering_panel);
	
	//Close sessions to the devices
	viClose(Agilent_handle);
	viClose(Rigol_handle);
}	
	
void ThomsonScatteringScopeandCamera(void)
{
	int 		result;
	int			Andor_cameraIndex;
	int			Andor_cameraHandle;
	int			Andor_Camera_Cooler;
	int			Andor_Horiz_pixels;
	int			Andor_Vert_pixels;
	
	float		Andor_Temp;
	float		TS_CenterTime;
	float		TS_VoltageRange1;
	float		TS_Chan1_Offset;
	float		TS_VoltageRange2;
	float		TS_Chan2_Offset;
	float		TS_TriggerLevel;
	char		TS_TriggerSource[8];
	char		DetectorIniPath[64];
	char		ThomsonScattering_string[32]="Null";
	
	ViUInt32 	count;
	
	//Open up TS settings panel and get settings
	ThomsonScattering_panel = LoadPanel (0, "ThomsonScatteringSettings.uir", ThomsonSct);
	RecallPanelState (ThomsonScattering_panel, "Master_Control_Storage_File", ThomsonScattering_setup_state);

	//Get parallel or perp choice state
 	GetCtrlVal (ThomsonScattering_panel, ThomsonSct_OrientationSwitch,&TS_Para_Perp_flag);

	//Get Andor camera delay time
	GetCtrlVal (ThomsonScattering_panel, ThomsonSct_Q1500_DDG_Delay,&Q1500_DDG_Delay);
	

	/******************************************************************************/
	//Set up Rigol Scope for photodiode measurements
	
	//Get sample rate, record length, and channel scales
	GetCtrlVal (ThomsonScattering_panel, ThomsonSct_TS_RecordLength,&TS_RecordLength);
	GetCtrlVal (ThomsonScattering_panel, ThomsonSct_TS_CenterTime,&TS_CenterTime);
	GetCtrlVal (ThomsonScattering_panel, ThomsonSct_TS_VoltageRange1,&TS_VoltageRange1);
	GetCtrlVal (ThomsonScattering_panel, ThomsonSct_TS_Chan1_Offset,&TS_Chan1_Offset);
	GetCtrlVal (ThomsonScattering_panel, ThomsonSct_TS_VoltageRange2,&TS_VoltageRange2);
	GetCtrlVal (ThomsonScattering_panel, ThomsonSct_TS_Chan2_Offset,&TS_Chan2_Offset);
	GetCtrlVal (ThomsonScattering_panel, ThomsonSct_TS_TriggerSource,TS_TriggerSource);
	GetCtrlVal (ThomsonScattering_panel, ThomsonSct_TS_TriggerLevel,&TS_TriggerLevel);
	GetCtrlVal (ThomsonScattering_panel, ThomsonSct_TS_TimeSpan,&TS_TimeSpan);

	//Depending on orientation choice, get relevant settings from interface and set up Agilent function generator and Rigol Delay generator
	if (TS_Para_Perp_flag) {   //Perp choice
		GetCtrlVal (ThomsonScattering_panel, ThomsonSct_RIGOL_PerpScope,TS_IP_Address_string);
	} else {
		GetCtrlVal (ThomsonScattering_panel, ThomsonSct_RIGOL_ParaScope,TS_IP_Address_string);
	}	
	
	//Open connection to the scope for photodiode measurements
	result=viOpen (Global_Visa_Session_Handle, TS_IP_Address_string, VI_NULL, VI_NULL, &ThomsonScatteringRigol_handle);

	if(!result) {
		//Make sure scope is running so commands will work
		sprintf (ThomsonScattering_string, ":RUN\r");   																							//Send the run command
		result = viWrite (ThomsonScatteringRigol_handle, ThomsonScattering_string, (unsigned int)strlen(ThomsonScattering_string), &count);

		//Setup the rate, record length, trigger system, and voltage range for the Rigol scope 
		sprintf (ThomsonScattering_string, ":CHAN1:DISP ON\r");   
		result = viWrite (ThomsonScatteringRigol_handle, ThomsonScattering_string, (unsigned int)strlen(ThomsonScattering_string), &count);

		sprintf (ThomsonScattering_string, ":CHAN2:DISP ON\r");   
		result = viWrite (ThomsonScatteringRigol_handle, ThomsonScattering_string, (unsigned int)strlen(ThomsonScattering_string), &count);

		sprintf (ThomsonScattering_string, ":CHAN1:COUP AC\r");   
		result = viWrite (ThomsonScatteringRigol_handle, ThomsonScattering_string, (unsigned int)strlen(ThomsonScattering_string), &count);

		sprintf (ThomsonScattering_string, ":CHAN2:COUP AC\r");   
		result = viWrite (ThomsonScatteringRigol_handle, ThomsonScattering_string, (unsigned int)strlen(ThomsonScattering_string), &count);

		sprintf (ThomsonScattering_string, ":SINGLE\r");   
		result = viWrite (ThomsonScatteringRigol_handle, ThomsonScattering_string, (unsigned int)strlen(ThomsonScattering_string), &count);
			
		sprintf (ThomsonScattering_string, ":ACQ:MDEP %d\r",TS_RecordLength);   																//Set the record length by setting the Memory Depth and upload the record length in k
		result = viWrite (ThomsonScatteringRigol_handle, ThomsonScattering_string, (unsigned int)strlen(ThomsonScattering_string), &count);

	  	sprintf (ThomsonScattering_string, ":TIM:MAIN:SCAL %f\r",(TS_TimeSpan/10.0/1.0E9));   													//Set up timebase scaling based on target sample window and 10 divisions
		result = viWrite (ThomsonScatteringRigol_handle, ThomsonScattering_string, (unsigned int)strlen(ThomsonScattering_string), &count);
		
		sprintf (ThomsonScattering_string, ":TIM:OFFS %f\r",TS_CenterTime);   															   		//Set center of trigger window
		result = viWrite (ThomsonScatteringRigol_handle, ThomsonScattering_string, (unsigned int)strlen(ThomsonScattering_string), &count);
			
		sprintf (ThomsonScattering_string, ":CHAN1:SCAL  %f\r",TS_VoltageRange1);   															//Set the voltage scale of the channel to the target value
		result = viWrite (ThomsonScatteringRigol_handle, ThomsonScattering_string, (unsigned int)strlen(ThomsonScattering_string), &count);

		sprintf (ThomsonScattering_string, ":CHAN2:SCAL  %f\r",TS_VoltageRange2);   															//Set the voltage scale of the channel to the target value
		result = viWrite (ThomsonScatteringRigol_handle, ThomsonScattering_string, (unsigned int)strlen(ThomsonScattering_string), &count);
			
		sprintf (ThomsonScattering_string, ":CHAN1:OFFS  %f\r",TS_Chan1_Offset);   																//Set the voltage offset of the channel to the target value
		result = viWrite (ThomsonScatteringRigol_handle, ThomsonScattering_string, (unsigned int)strlen(ThomsonScattering_string), &count);

		sprintf (ThomsonScattering_string, ":CHAN2:OFFS  %f\r",TS_Chan2_Offset);   																//Set the voltage offset of the channel to the target value
		result = viWrite (ThomsonScatteringRigol_handle, ThomsonScattering_string, (unsigned int)strlen(ThomsonScattering_string), &count);
			
		sprintf (ThomsonScattering_string, ":TRIG:MODE EDGE\r");   
		result = viWrite (ThomsonScatteringRigol_handle, ThomsonScattering_string, (unsigned int)strlen(ThomsonScattering_string), &count);

		sprintf (ThomsonScattering_string, ":TRIG:COUP AC\r");   
		result = viWrite (ThomsonScatteringRigol_handle, ThomsonScattering_string, (unsigned int)strlen(ThomsonScattering_string), &count);

		sprintf (ThomsonScattering_string, ":TRIG:EDGE:SOUR %s\r",TS_TriggerSource);  															//Set the trigger source to the target value 
		result = viWrite (ThomsonScatteringRigol_handle, ThomsonScattering_string, (unsigned int)strlen(ThomsonScattering_string), &count);

		sprintf (ThomsonScattering_string, ":TRIG:EDGE:SLOP POS\r");   
		result = viWrite (ThomsonScatteringRigol_handle, ThomsonScattering_string, (unsigned int)strlen(ThomsonScattering_string), &count);

		sprintf (ThomsonScattering_string, ":TRIG:EDGE:LEV %f\r",TS_TriggerLevel);   
		result = viWrite (ThomsonScatteringRigol_handle, ThomsonScattering_string, (unsigned int)strlen(ThomsonScattering_string), &count);
		
		sprintf (ThomsonScattering_string, ":SYST:KEY:PRES MOFF\r");   												 							//Hide menu on scope screen
	 	result = viWrite (ThomsonScatteringRigol_handle, ThomsonScattering_string, (unsigned int)strlen(ThomsonScattering_string), &count);	
	}
	
	
	/******************************************************************************/
	//Set up Andor Camera
				
	Andor_cameraIndex = 1;													//select first camera, assume it is the Thomson camera and not the McPherson camera
	result = GetCameraHandle(Andor_cameraIndex, &Andor_cameraHandle);   	//Get Handle for first camera
	result = SetCurrentCamera(Andor_cameraHandle);							//Choose to make default the camera
	result = Initialize(DetectorIniPath);									//Initialize the camera
	
	GetCtrlVal (ThomsonScattering_panel, ThomsonSct_Andor_Camera_Cooler,&Andor_Camera_Cooler);
	if (Andor_Camera_Cooler) {
		GetCtrlVal (ThomsonScattering_panel, ThomsonSct_Andor_Temp,&Andor_Temp);
		CoolerON();
		SetTemperature(Andor_Temp);
	} else {
		CoolerOFF();
		SetCoolerMode(0);
		ShutDown();
	}
	result = SetAcquisitionMode(1);		//Single acquisition mode
	result = SetReadMode(4);			//Record an image
	result = SetTriggerMode(1);			//External Trigger
	result = SetGateMode(5);			//Use DDG mode for trigger
	
	GetCtrlVal (ThomsonScattering_panel, ThomsonSct_Andor_Gain,&Andor_Gain);
	result = SetGain(Andor_Gain);
	
	result = SetVerticalSpeed(0);
	result = SetHorizontalSpeed(0);
	
	GetCtrlVal (ThomsonScattering_panel, ThomsonSct_Andor_Pre_Amp,&Andor_Pre_Amp);
	result = SetPreAmpGain(Andor_Pre_Amp);
	
	result = SetDDGInsertionDelay(0);	//Set DDG insertion delay to normal
	result = SetDDGIntelligate(1);		//Set to "yes"
	
	GetCtrlVal (ThomsonScattering_panel, ThomsonSct_Andor_Exp_Time,&Andor_Exp_Time);
	result = SetExposureTime(Andor_Exp_Time/1000000.0);
	
	if (TS_Para_Perp_flag) {   //Perp choice
		GetCtrlVal (ThomsonScattering_panel, ThomsonSct_Q1500_DDG_Delay,&Andor_DDGDelay);
	} else {
		GetCtrlVal (ThomsonScattering_panel, ThomsonSct_Q850_DDG_Delay,&Andor_DDGDelay);
	}
	GetCtrlVal (ThomsonScattering_panel, ThomsonSct_Andor_GateWidth,&Andor_GateWidth);
	result = SetDDGGateTime((Andor_DDGDelay/1000.0),(Andor_GateWidth/1000.0) );						//Set time and width in picoseconds
	
	GetCtrlVal (ThomsonScattering_panel, ThomsonSct_Andor_H_bin,&Andor_H_bin);
	GetCtrlVal (ThomsonScattering_panel, ThomsonSct_Andor_V_bin,&Andor_V_bin);
	result = GetDetector(&Andor_Horiz_pixels,&Andor_Vert_pixels);									//Read how many pixels there are in H and V
	result = SetImage(Andor_H_bin,Andor_V_bin, 1, Andor_Horiz_pixels, 1, Andor_Vert_pixels);		//Bin the pixels of the iCCD from 1 to the full extent into the requested number of bins
	
	//Discard housekeeping panel
	DiscardPanel(ThomsonScattering_panel);
	
	viClose(ThomsonScatteringRigol_handle);
}


//Function to calculate the laser firing offset times so they can be accessed by the master trigger system
//Also send the preferred laser warmup times
void Get_TS_Laser_Offsets (float *Perp_offset,float *Para_offset, float *Laser_Warmup)
{
	float time1;
	float time2;
	float warmup1;
	float warmup2;
	
	//Open up TS settings panel and get settings
	ThomsonScattering_panel = LoadPanel (0, "ThomsonScatteringSettings.uir", ThomsonSct);
	RecallPanelState (ThomsonScattering_panel, "Master_Control_Storage_File", ThomsonScattering_setup_state);
	   
	//Get parallel or perp choice state and warmup times
 	GetCtrlVal (ThomsonScattering_panel, ThomsonSct_OrientationSwitch,&TS_Para_Perp_flag);
	GetCtrlVal (ThomsonScattering_panel, ThomsonSct_Q1500_Warmup,&warmup1);
	GetCtrlVal (ThomsonScattering_panel, ThomsonSct_Q850_Warmup,&warmup2);
		
	//Get TS perp delays
	GetCtrlVal (ThomsonScattering_panel, ThomsonSct_Q1500_InstrinsicDelay,&Q1500_InstrinsicDelay);
	GetCtrlVal (ThomsonScattering_panel, ThomsonSct_Q1500_Q_delay,&Q1500_Q_delay);
	GetCtrlVal (ThomsonScattering_panel, ThomsonSct_Q1500_Circuit_Delay,&Q1500_Circuit_Delay);

	//Get TS para delays
 	GetCtrlVal (ThomsonScattering_panel, ThomsonSct_Q850_InstrinsicDelay,&Q850_InstrinsicDelay);
 	GetCtrlVal (ThomsonScattering_panel, ThomsonSct_Q850_Q_delay,&Q850_Q_delay);
	GetCtrlVal (ThomsonScattering_panel, ThomsonSct_Q850_Circuit_Delay,&Q850_Circuit_Delay);

	//Update values that will be sent to master trigger interface
	time1 = (Q850_InstrinsicDelay+Q850_Q_delay+Q850_Circuit_Delay)/1.0E6;
	time2 = (Q1500_InstrinsicDelay+Q1500_Q_delay+Q1500_Circuit_Delay)/1.0E6;

	
	//Send laser warmup times back to master trigger panel and make it that the offset time of the laser not in use is zero
	if (TS_Para_Perp_flag) {
		*Laser_Warmup = warmup1;
		*Para_offset = time1;
		*Perp_offset = 0.0;
	} else {
		*Laser_Warmup = warmup2;
		*Para_offset = 0.0;
		*Perp_offset = time2;
	}	

	//Close panel in background
	DiscardPanel(ThomsonScattering_panel); 
}



//Activate Thomson Scattering System
int ThomsonScatteringActivate(void)
{
	int		result;
	int		count;
	char	SimmerCodePath[256]="Null";
	
	//Call laser control code and place laser in simmer mode
	ThomsonScatteringLaserControl(1);
	
	//Set up Rigol scope and camera
	ThomsonScatteringScopeandCamera();
	
	return 0;
}




//Arm the timing systems for the lasers, the Andor camera, and the Rigol oscilloscope
int ThomsonScatteringArm(void)
{
	int		result;
	int		count;
	char	ThomsonScattering_string[32]="Null";
	
	//Take lasers out of simmer mode and get them ready for a trigger
	ThomsonScatteringLaserControl(0);

	//Open connection to the scope for photodiode measurements
	result=viOpen (Global_Visa_Session_Handle, TS_IP_Address_string, VI_NULL, VI_NULL, &ThomsonScatteringRigol_handle);

	//Arm the Rigol Oscilloscope
	sprintf (ThomsonScattering_string, ":TRIG:SWE SINGl\r");   					//Single Trigger Sweep
 	result = viWrite (ThomsonScatteringRigol_handle, ThomsonScattering_string, (unsigned int)strlen(ThomsonScattering_string), &count);
	
	//Arm the Andor Camera
	result = StartAcquisition();

	//Close communications with TS devices
	viClose(ThomsonScatteringRigol_handle);
	
	return result;
}


//Launch this routine as window opens and it updates the window with the latest values from the 
//control system as it loads.
void Create_McPherson_Wavelength (double Wavelength[])  
{
	#define		pi 3.141592654	
	#define		Order 1
	#define		PixelDimensions 13		//in microns
	#define		Horiz_Bins 1			//The binning of the grating axis in the image. 1 = no binning
	#define		Horiz_Pixels 1024		//Size of the iCCD camera chip
	int			j;
	float		alpha;
	float		beta;
	float		linear_disp;
	float		Wavelength_step;
	
	//Open settings panel and load saved values
	ThomsonScattering_panel = LoadPanel (0, "ThomsonScatteringSettings.uir", ThomsonSct);
	SavePanelState (ThomsonScattering_panel, "Master_Control_Storage_File", ThomsonScattering_setup_state);

	//Get McPherson Spectrometer Parameters
 	GetCtrlVal (ThomsonScattering_panel, ThomsonSct_TS_McP_Current_Wlengt,&McP_Current_Wlength);	   //Wavelength in nm
	GetCtrlVal (ThomsonScattering_panel, ThomsonSct_TS_McP_GrooveDen,&McP_GrooveDen);
	GetCtrlVal (ThomsonScattering_panel, ThomsonSct_TS_McP_AngleDiff,&McP_AngleDiff);
	GetCtrlVal (ThomsonScattering_panel, ThomsonSct_TS_McP_FocalLength,&McP_FocalLength);
	
	alpha = (180.0/pi)*( asin( (Order*McP_GrooveDen*1.0E-6/2.0)/(cos(McP_AngleDiff*pi/2.0/180.0) ) ) - (McP_AngleDiff*pi/2.0/180.0) );
	beta = McP_AngleDiff+alpha;
	
	linear_disp=1.0E6*cos(beta*pi/180.0)/(McP_FocalLength*Order*McP_GrooveDen);

	Wavelength_step= Horiz_Bins*PixelDimensions*0.001*linear_disp;
	
	for (j=0;j<(int)(Horiz_Pixels/Horiz_Bins);j++) {
		Wavelength[j]= ((j-Horiz_Pixels/Horiz_Bins)*Wavelength_step+McP_Current_Wlength);			//This assumes no stretching of axis or offsets from calibration
	}


}


//Take the acquired data from the instrument and print it to the common data folder and 
//also push it to the MDS database
void Write_ThomsonScatteringData(void)
{
	char	ChannelNameString[36];
	char	outfilename[36];
	FILE*	outfile;
	int		i,j;
	int		result;
	int		Andor_Horiz_pixels;
	int		Andor_Vert_pixels;
	int 	CollectionOptics_Flag;
	float	TS_timestep=0.0;
	double	*ThomsonScattering_Array1;
	double	*ThomsonScattering_Array2;
	double	*ThomsonScattering_Array3;
	double	*ThomsonScattering_Array4;
	double	*timebase;
	double	*VerticalIndex;
	double	*TSWavelength;
	double	*AndorImageDouble;
	WORD	*AndorImage;	

	//Create temporary data arrays	
	ThomsonScattering_Array1 = malloc (TS_RecordLength);
	ThomsonScattering_Array2 = malloc (TS_RecordLength);
	ThomsonScattering_Array3 = malloc (TS_RecordLength);
	ThomsonScattering_Array4 = malloc (TS_RecordLength);
	timebase = malloc (TS_RecordLength);

	//Grab data from all four channels of the oscilloscope
	Rigol(ThomsonScatteringRigol_handle, TS_RecordLength, &TS_timestep, ThomsonScattering_Array1, ThomsonScattering_Array2, ThomsonScattering_Array3, ThomsonScattering_Array4);   
	
	//Dump results to a file
	sprintf(outfilename, RawDataPath);
	strcat(outfilename, ShotNumberString);
	strcat(outfilename, "_");
	strcat(outfilename, "ThomsonScattering.txt");
	outfile=fopen (outfilename, "w");
	
	//Store all the parameters of the lasers and the collection optics choices
	GetCtrlVal (ThomsonScattering_panel, ThomsonSct_CollectionOptics,&CollectionOptics_Flag);
	fprintf(outfile, "Type of COllection Optics %d",CollectionOptics_Flag);
	fprintf(outfile, "Q1500 Frequency %f",Q1500_Frequency);
	fprintf(outfile, "Q1500_InstrinsicDelay %f",Q1500_InstrinsicDelay);
	fprintf(outfile, "Q1500_Q_delay %f",Q1500_Q_delay);
	fprintf(outfile, "Q1500_Circuit_Delay %f", Q1500_Circuit_Delay);
	fprintf(outfile, "Q1500_QSW_Sourcey %f", Q1500_QSW_Source);
	fprintf(outfile, "Q1500_DDG_Delay %f", Q1500_DDG_Delay);
	fprintf(outfile, "Laser Firing Time %f",(Q1500_Trigger_Time- Q1500_InstrinsicDelay- Q1500_Q_delay -Q1500_Circuit_Delay)/1.0E6);
	fprintf(outfile, "Q850 Frequency %f",Q850_Frequency);
	fprintf(outfile, "Q850_InstrinsicDelay %f",Q850_InstrinsicDelay);
	fprintf(outfile, "Q850_Q_delay %f",Q850_Q_delay);
	fprintf(outfile, "Q850_Circuit_Delay %f", Q850_Circuit_Delay);
	fprintf(outfile, "Q850_QSW_Sourcey %f", Q850_QSW_Source);
	fprintf(outfile, "Q850_DDG_Delay %f", Q850_DDG_Delay);
	fprintf(outfile, "Laser Firing Time %f",(Q850_Trigger_Time- Q850_InstrinsicDelay- Q850_Q_delay -Q850_Circuit_Delay)/1.0E6);
	fprintf(outfile, "Andor_Gain %f", Andor_Gain);
	fprintf(outfile, "Andor_Pre_Amp %f", Andor_Pre_Amp);
	fprintf(outfile, "Andor_Exp_Time %f", Andor_Exp_Time);
	fprintf(outfile, "Andor_DDGDelay %f", Andor_DDGDelay);
	fprintf(outfile, "Andor_GateWidth %f", Andor_GateWidth);
	fprintf(outfile, "Andor_H_bin %f", Andor_H_bin);
	fprintf(outfile, "Andor_V_bin %f", Andor_V_bin);
	fprintf(outfile, "McP_Current_Wlength %f", McP_Current_Wlength);
	fprintf(outfile, "McP_GrooveDen %f", McP_GrooveDen);
	fprintf(outfile, "McP_AngleDiff %f", McP_AngleDiff);
	fprintf(outfile, "McP_FocalLength %f", McP_FocalLength);

	fprintf(outfile,"t, Photodiode 1, Photodiode 2\n");
	for (j=0;j<TS_RecordLength;j++) {
		timebase[j]=(double)(j*TS_timestep);
		fprintf(outfile,"%f,%f,%f\n",timebase[j],ThomsonScattering_Array1[j],ThomsonScattering_Array2[j]);
	}

 	//Close Housekeeping data file
	fclose (outfile);
	
	
	//Move Andor Camera Data to MDS
	result = GetDetector(&Andor_Horiz_pixels,&Andor_Vert_pixels);									//Read how many pixels there are in H and V
 	AndorImage = malloc ((int)((Andor_Horiz_pixels/Andor_H_bin)*(Andor_Vert_pixels/Andor_V_bin)));	//Build an array equal to the size of the total number of image pixels after the image has been binned into a
																									//smaller set of pixels per how the camera was set up earlier.
																									//Might have to increase the size to be the pixles TIMES the size of a 16 bit number
	//Create a double array from the image array
	AndorImageDouble = malloc ((int)((Andor_Horiz_pixels/Andor_H_bin)*(Andor_Vert_pixels/Andor_V_bin)));
	
	result = GetAcquiredData16(AndorImage, (int)((Andor_Horiz_pixels/Andor_H_bin)*(Andor_Vert_pixels/Andor_V_bin)));			//Retrieve the image data from the camera

	//Put the image date into a double array for MDS
	for (j=0;j<(int)(Andor_Horiz_pixels/Andor_H_bin);j++)  {  
		for (i=0;i<(int)(Andor_Vert_pixels/Andor_V_bin);i++)  {  
			AndorImageDouble[j+i]=(double)AndorImage[j+i];
		}
	}
	
	VerticalIndex = malloc ((int)(Andor_Vert_pixels/Andor_V_bin));
	//Create vertical index array
	for (j=0;j<(int)(Andor_Vert_pixels/Andor_V_bin);j++)  {
		VerticalIndex[j]=(double)j;		
	}
	
	TSWavelength = malloc ((int)(Andor_Horiz_pixels/Andor_H_bin));
	//Run McPherson Function to generate wavelength array
	Create_McPherson_Wavelength(TSWavelength);
	
	//Open up Housekeepings settings panel and get filenames for each measuremen
	ThomsonScattering_panel = LoadPanel (0, "ThomsonScatteringSystems.uir", ThomsonSct);
	RecallPanelState (ThomsonScattering_panel, "Master_Control_Storage_File", ThomsonScattering_setup_state);

	//Get channel name string for Andor image and push data to MDSPlus
	GetCtrlVal (ThomsonScattering_panel, ThomsonSct_Chan1Name,ChannelNameString);

	//Push 2D to MDS database
	Write_to_PHASMA_2D_MDS(AndorImageDouble, TSWavelength, VerticalIndex, ChannelNameString);

	//Get channel name string, store raw photodiode data on drive folder, and push data to MDSPlus for Channel 1
	GetCtrlVal (ThomsonScattering_panel, ThomsonSct_Chan2Name,ChannelNameString);
	Write_to_PHASMA_MDS(timebase,ThomsonScattering_Array2,ChannelNameString);
}



//Launch this routine as window opens and it updates the window with the latest values from the 
//control system as it loads.
void OpenThomsonScattering_Settings (void)  
{
	//Open settings panel and load saved values
	ThomsonScattering_panel = LoadPanel (0, "ThomsonScatteringSettings.uir", ThomsonSct);
	RecallPanelState (ThomsonScattering_panel, "Master_Control_Storage_File", ThomsonScattering_setup_state);
	DisplayPanel(ThomsonScattering_panel);
	
	//Update panel with 

	
	// Start interacting with user
    RunUserInterface ();

}

int CVICALLBACK Close_ThomsonScattering_Settings (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			//Save current state of panel and close interface
			SavePanelState (panel, "Master_Control_Storage_File", ThomsonScattering_setup_state);
			DiscardPanel(panel);
			
			//Update the values to the instruments but with the lasers in simmer mode
			ThomsonScatteringLaserControl(1);
			
			break;
	}
	return 0;
}



//Routine that sets the YAG lasers back to simmering mode by switching the Agilent function generators for each laser to regular mode (turn off burst mode) and lengthen the Rigol delay generator pulse delay. 
//This will run after every shot. Make sure to wait a bit of time before starting up simmering mode or shutting down simmering mode so as not to trigger the laser out of sequence.
void Laser_SimmerMode(void)
{

	//Put lasers in simmer mode
	ThomsonScatteringLaserControl(1);

}

//Move McPherson Spectrometer
int CVICALLBACK Move_McPherson207 (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	float 	Target_Wavelength;
	
	switch (event)
	{
		case EVENT_COMMIT:
			ThomsonScattering_panel = LoadPanel (0, "ThomsonScatteringSettings.uir", ThomsonSct);
			RecallPanelState (ThomsonScattering_panel, "Master_Control_Storage_File", ThomsonScattering_setup_state);
			
			//Get the target wavelength 
			GetCtrlVal (ThomsonScattering_panel, ThomsonSct_TS_McP_Center,&Target_Wavelength);
		
			//Move McPherson to target wavelength
			move_207(Target_Wavelength);
			
			break;
	}
	return 0;
}

int CVICALLBACK Set_McPherson207 (int panel, int control, int event,
								  void *callbackData, int eventData1, int eventData2)
{
	float 	Current_Wavelength;
	
	switch (event)
	{
		case EVENT_COMMIT:
			ThomsonScattering_panel = LoadPanel (0, "ThomsonScatteringSettings.uir", ThomsonSct);
			RecallPanelState (ThomsonScattering_panel, "Master_Control_Storage_File", ThomsonScattering_setup_state);
			
			//Get the current wavelength
			GetCtrlVal (ThomsonScattering_panel, ThomsonSct_TS_McP_Current_Wlengt,&Current_Wavelength);

			//Set current wavelength in controller software
			overwriteLocation_207(Current_Wavelength);
			
			break;
	}
	return 0;
}
