
	/*Plused LIF Code Process
	Step 0
	If and only if the activate switch is flipped on the main control panel, launch the LabView executable 
	that runs the lasers in the background on a slow simmer. The delay between the Q-switch and the Rigol 
	is stretched out so the lasers are firing at pretty low power. When the activate switch is turned off, 
	turn off the background simmer sequence? Will this return the lasers to full power? Probably not if 
	deactivating them does not turn on any external pulse sequencing. Better make sure an errant master trigger 
	pulse cannot turn the lasers back on. Perhaps overwrite the flashlamp and Qswitch trigger amplitudes to zero 
	VolPLIF at this step? If so, then when activated, have to return them to 1 volt.
	
	Step 1
	When the PLIF settings window is opened and then the values saved, read all the pulse frequencies, delay times, 
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
	number of pulses to line up. Here also read the visa addresses of all the instrumenPLIF and set up the acquisition 
	parameters for the Rigol Scope that records the laser trigger signal and the laser power from the photodiodes on the 
	optical table and on top of pumping station. Set this scope to trigger using a digital D0 trigger. Also set up the 
	parameters for the iCCD camera including different DDG delays for the parallel and perpendicular PLIF systems based on 
	how far the Q-synch signals have to travel to the camera. Note here in the panel remind the user to physically swap 
	the Q-synch cables until they are permanently coupled through diodes (will need to check that combining the Q-synch 
	signals does not introduce an unacceptable delay in triggering the Andor camera)
	
	Here also have the option to move the central wavelength of the small McPherson spectrometer so you can look at 
	different parPLIF of the PLIF spectrum if desired.

	Step 2
	In the arming step of the PHASMA cycle sequence send the timing information to the Agilent and Rigol delay generators 
	for the laser chosen (parallel or perp). Could set them both as a matter of course and then it just matters which laser 
	is turned on for which one fires. Essentially these commands will be to set up the number of pulses to be created by the 
	Agilent system and at what frequency, the delay for the Rigol DG1022Z between an external trigger it receives and the 
	pulse it sends out the front (this will be the optimal delay to maximize the laser power).

	Still in the arming step, write a string to the file read by the other PLIF code to tell the laser to switch from background 
	simmer mode to be in the "experimental mode" In-EXP. This other code runs continuously in the background and changes the 
	timing settings on the Agilent and Rigol devices on both the parallel and perpendicular PLIF lasers. The timing pulses for 
	the QSmart1500 are teed off to the LIF Q850 laser on the same table
	
	Also still in the arming step, write all the PLIF system parameters to the MDS system using the same tree paths as Peiyun set up.

	Step 3
	Arm the Agilent and the DG1022Z (no arming of the SRS DG645 as we are assuming here that it has been pulled out of the system). 
	Arm the Rigol Oscilloscope and the Andor camera.
	
	Step 4
	Now in the primary trigger sequence of the PHASMA cycle. Switch the mode of the laser by updating the string in the file 
	to tell the background PLIF code to stop. Then fire the master trigger T0. This will start the many hundred pulse sequence 
	of the Agilent. The laser stops firing when the last of the pulse train hiPLIF. Concurrent with the gas puff trigger, send a 
	different trigger we will call "Andor Camera Shutter Trigger" that lasPLIF for 150 ms (long enough to cover the gas puff and 
	the entire plasma gun sequence). This same trigger could be used to trigger the photron camera shutter. Fire the gas puff, 
	fire the arc pulse, and then fire the "Laser-Firing" trigger, and then fire the bias pulse trigger. Given all the delays 
	that have to be built into the laser firing time trigger (274+60+1=335 microseconds), it will have to come before the roughly 
	100 microsecond long bias pulse begins. When the laser actually fires (the Q-synch signal), that trigger is combined with the 
	Andor Camera Shutter Trigger (which is still held high) and the iCCD camera fires after the programmed DDG delay of hundreds 
	of ns and then is held open for 50 ns or so.
	
	Step 4
	After the pulse ends, write the string to the file again so the laser simmer code starPLIF running again.
	ove the Rigol oscilloscope data to the PC and store a plaintext version and push the Rigol data to MDS.
	Create a wavelength array based on the parameters of the small McPherson spectrometer. Move the Andor data to the PC. 
	Store the wavelength array and the Andor data in a plaintext file and push those data to MDS.

	Step 5
	PLIF process ends.
*/

#include 	"windows.h"
#include 	"wlmData.h"
#include 	<ansi_c.h>
#include 	<utility.h>
#include	<visa.h>
#include	<stdlib.h>
#include 	<formatio.h>
#include	"PHASMA-MDS-Routines.h"
#include 	"PulsedLIFSystems.h"
#include 	"PulsedLIFSettings.h"  
#include	"GlobalVariables.h"
#include	"Rigol.h"
#include 	"MasterTriggerSystem.h"
#include 	"ThomsonScatteringSystems.h"



#define		agilent_highlevel 5.0   	//Set square wave pulse amplitude to 3 V	
#define		agilent_lowlevel 0.0		//Set square wave pulse amplitude to 3 V	
#define		agilent_pulsewidth 350.0	//Set square wave pulse length to 120 microseconds based on Peiyun's LabView code		
#define		agilent_burstphase 55.1		//Set square wave burst phase to start burst in synch with external trigger. For a 20% duty cycle, this phase delay
										//gives a 40-60 microsecond delay in the start of the agilent burst signal.
#define		rigol_pulsewidth 120.0			
#define		rigol_amplitude 4.0			
#define		rigol_offset 2.0			


int			PulsedLIF_panel;
int			PulsedLIF_setup_state = 61;
int			PLIF_Para_Perp_flag; 
int			PLIF_RecordLength; 
int			GenTec_handle;
int			Angstrom_gExiting;
int			AngstromReadFunctionThread_ID;
	
float		Laser_Firing_Time;
float		PLIF_Frequency;
float		PLIF_Warmup;
float 		PLIF_InstrinsicDelay;
float		PLIF_Simmer_Delay;
float		PLIF_Q_delay;
float		PLIF_Circuit_Delay;
float		PLIF_Trigger_Time;
float		PLIF_TimeSpan;

double		PLIF_Wavelength[100];
double		Laser_Power[100];

char		agilent_string[64];
char		rigol_string[64];
char		PLIF_IP_Address_string[64];

ViSession 	PLIF_Scope_Rigol_handle;
ViSession	PLIF_Agilent_handle;
ViSession	PLIF_Rigol_handle;


//Set up the PLIF system infrastructure
void PLIFLaserControl(int simmermode)
{
	int 		result; 					//	Error-checking variable
	int			num_pulses;
	int			PLIF_QSW_Source;
	float		PLIF_DDG_Delay;

	float		duty_cycle;
	char		PLIF_Agilent_Add[64]; 
	char		PLIF_Rigol_Add[64]; 
	char		PLIF_QSmart850_Lamp_IP_Address_string[64]; 
	char		PLIF_QSmart850_Qsw_IP_Address_string[64]; 
	char		PLIF_IP_Address_string[64];
	char		PulsedLIF_string[32]="Null";
	ViUInt32 	count;

	//Open up PLIF settings panel and get settings
	PulsedLIF_panel = LoadPanel (0, "PulsedLIFSettings.uir", PLIF);
	RecallPanelState (PulsedLIF_panel, "Master_Control_Storage_File", PulsedLIF_setup_state);
	   
	//Get Laser pulse frequency
 	GetCtrlVal (PulsedLIF_panel, PLIF_Q850_Frequency,&PLIF_Frequency);
		
	//Get Laser intrinsic delay
 	GetCtrlVal (PulsedLIF_panel, PLIF_Q850_InstrinsicDelay,&PLIF_InstrinsicDelay);
		
	//Get Laser Q switch delay
 	GetCtrlVal (PulsedLIF_panel, PLIF_Q850_Q_delay,&PLIF_Q_delay);
		
	//Get Laser circuit delay
 	GetCtrlVal (PulsedLIF_panel, PLIF_Q850_Circuit_Delay,&PLIF_Circuit_Delay);
		
	//Get Laser warm up time
 	GetCtrlVal (PulsedLIF_panel, PLIF_PLIF_Warmup,&PLIF_Warmup);
				
	//Get Laser trigger source flag
 	GetCtrlVal (PulsedLIF_panel, PLIF_Q850_QSW_Source,&PLIF_QSW_Source);
	
	//Calculate laser firing time to be used in various places
	Laser_Firing_Time = (PLIF_Trigger_Time- PLIF_InstrinsicDelay- PLIF_Q_delay - PLIF_Circuit_Delay)/1.0E6;
	
	//Get Laser Simmer delay
	GetCtrlVal (PulsedLIF_panel, PLIF_Q850_SimmerDelay,&PLIF_Simmer_Delay);
		

	
	/***********************************************************************************************/
	//Set up Rigol for laser Q-switch control 

	//Get Rigol Address String
	GetCtrlVal (PulsedLIF_panel, PLIF_Rigol_Address,PLIF_Rigol_Add);

	//Open visa session to Rigol delay Generator
	result=viOpen (Global_Visa_Session_Handle, PLIF_Rigol_Add, VI_NULL, VI_NULL, &PLIF_Rigol_handle);
		
	if(!result) {

		//Set the frequency, peak to peak ammplitude, and offset for channel 1. If Vpp=4, offset by 2 volts to go 4 V to 0V
		sprintf (rigol_string, ":SOUR1:APPL:SQU %f,%f,%f\r", (1.0/(2*rigol_pulsewidth/1000000.0)), rigol_amplitude, rigol_offset);   
		result = viWrite (PLIF_Rigol_handle, rigol_string, (unsigned int)strlen(rigol_string), VI_NULL);

		//Set time delay between external trigger received and output signal for channel 2 to be the sum of the intrinsic
		//laser trigger time plus the user requested delay to optimize laser power if in regular power mode. If
		//not, switch to simmer mode time delay. Time in seconds.
		if (!simmermode) {
			sprintf (rigol_string, ":SOUR1:BURS:TDEL %f\r",((PLIF_InstrinsicDelay+PLIF_Q_delay)/1000000.0));   
			result = viWrite (PLIF_Rigol_handle, rigol_string, (unsigned int)strlen(rigol_string), VI_NULL);
		} else {
			sprintf (rigol_string, ":SOUR1:BURS:TDEL %f\r",((PLIF_InstrinsicDelay+PLIF_Simmer_Delay)/1000000.0));   
			result = viWrite (PLIF_Rigol_handle, rigol_string, (unsigned int)strlen(rigol_string), VI_NULL);
		}	

		//Delay to let Rigol process command			
		Delay (0.1);
		
		//Turn on channel 1
		sprintf (rigol_string, ":OUTP1 ON\r");   
		result = viWrite (PLIF_Rigol_handle, rigol_string, (unsigned int)strlen(rigol_string), VI_NULL);
			

		//Turn on burst mode on channel 1
		//sprintf (rigol_string, ":SOUR1:BURS ON\r");   
		//result = viWrite (PLIF_Rigol_handle, rigol_string, (unsigned int)strlen(rigol_string), VI_NULL);
		
						
		//Set number of cycles on burst mode on channel 1 to be 1 so one pulse generated for every trigger pulse
		sprintf (rigol_string, ":SOUR1:BURS:NCYC 1\r");   
		result = viWrite (PLIF_Rigol_handle, rigol_string, (unsigned int)strlen(rigol_string), VI_NULL);
			
		//Turn on trigger source on channel 1 to be external
		sprintf (rigol_string, ":SOUR1:BURS:TRIG:SOUR EXT\r");   
		result = viWrite (PLIF_Rigol_handle, rigol_string, (unsigned int)strlen(rigol_string), VI_NULL);
			
		//Set Trigger 1 phase to 359.9
		sprintf (rigol_string, ":SOUR1:BURS:PHAS 359.9\r");   
		result = viWrite (PLIF_Rigol_handle, rigol_string, (unsigned int)strlen(rigol_string), VI_NULL);		
			
		//Set trigger slope on channel 1 to be positive
		sprintf (rigol_string, ":SOUR1:BURS:TRIG:TRIGO POS\r");   
		result = viWrite (PLIF_Rigol_handle, rigol_string, (unsigned int)strlen(rigol_string), VI_NULL);
			
		//Turn on burst mode on channel 1 to be triggered
		sprintf (rigol_string, ":SOUR1:BURS:MODE TRIG\r");   
		result = viWrite (PLIF_Rigol_handle, rigol_string, (unsigned int)strlen(rigol_string), VI_NULL);
		
		//Turn on burst mode on channel 1
		sprintf (rigol_string, ":SOUR1:BURS ON\r");   
		result = viWrite (PLIF_Rigol_handle, rigol_string, (unsigned int)strlen(rigol_string), VI_NULL);

		//Make double sure that channel 1 is on
		sprintf (rigol_string, ":OUTP1 ON\r");   
		result = viWrite (PLIF_Rigol_handle, rigol_string, (unsigned int)strlen(rigol_string), VI_NULL);
	}
	
		
		
	/***********************************************************************************************/
	//Set up Agilent for laser flashlamp control

	//Get Agilent Address String
 	GetCtrlVal (PulsedLIF_panel, PLIF_Agilent_Address,PLIF_Agilent_Add);
		
	//Open visa session to Agilent Function Generator
	result=viOpen (Global_Visa_Session_Handle, PLIF_Agilent_Add, VI_NULL, VI_NULL, &PLIF_Agilent_handle);
		
	if (!result) {
		sprintf (agilent_string, "OUTP ON\r");   
		result = viWrite (PLIF_Agilent_handle, agilent_string, (unsigned int)strlen(agilent_string), VI_NULL);

		//Set square wave high
		sprintf (agilent_string, ":VOLT:HIGH %f\r", agilent_highlevel);   
		result = viWrite (PLIF_Agilent_handle, agilent_string, (unsigned int)strlen(agilent_string), VI_NULL);

		//Set square wave low
		sprintf (agilent_string, ":VOLT:LOW %f\r", agilent_lowlevel);   
		result = viWrite (PLIF_Agilent_handle, agilent_string, (unsigned int)strlen(agilent_string), VI_NULL);

		//Set square wave frequency
		sprintf (agilent_string, ":FREQ %f\r", PLIF_Frequency);   
		result = viWrite (PLIF_Agilent_handle, agilent_string, (unsigned int)strlen(agilent_string), VI_NULL);

		//Set square wave duty cycle
		duty_cycle=(agilent_pulsewidth/1000000.0)*PLIF_Frequency*100;
		if (duty_cycle < 20) duty_cycle=20;	//minumum duty cycle is 20%
		sprintf (agilent_string, "FUNC:PULSE:DCYCLE %f\r", duty_cycle);   
		result = viWrite (PLIF_Agilent_handle, agilent_string, (unsigned int)strlen(agilent_string), VI_NULL);

		//Turn on Burst state if not in simmermode
		if (!simmermode) {
				
			//Turn on burst mode
			sprintf (agilent_string, "BURST:STATE ON\r");   
			result = viWrite (PLIF_Agilent_handle, agilent_string, (unsigned int)strlen(agilent_string), VI_NULL);

			//Set number of laser pulses to create for warm up phase
			num_pulses=(int)PLIF_Frequency*PLIF_Warmup;
			sprintf (agilent_string, "BURST:NCYC %d\r", num_pulses);   
			result = viWrite (PLIF_Agilent_handle, agilent_string, (unsigned int)strlen(agilent_string), VI_NULL);				
	
			//Set phase of burst so that first rising edge of pulse train lines up with external trigger as closely as possible
			sprintf (agilent_string, "BURST:PHASe %f\r", agilent_burstphase);   
			result = viWrite (PLIF_Agilent_handle, agilent_string, (unsigned int)strlen(agilent_string), VI_NULL);

			//Set trigger to external
			sprintf (agilent_string, "TRIG:SOUR EXT\r");   
			result = viWrite (PLIF_Agilent_handle, agilent_string, (unsigned int)strlen(agilent_string), VI_NULL);
				
			//Set trigger to be on rising edge
			sprintf (agilent_string, "TRIG:SLOP POS\r");   
			result = viWrite (PLIF_Agilent_handle, agilent_string, (unsigned int)strlen(agilent_string), VI_NULL);
				
		} else {
				//Turn output off before going to simmer to avoid a pulse at the wrong frequency
				sprintf (agilent_string, "OUTP OFF\r");   
				result = viWrite (PLIF_Agilent_handle, agilent_string, (unsigned int)strlen(agilent_string), VI_NULL);

				//Turn voltage on low before restarting simmer, then turn back up
				sprintf (agilent_string, ":VOLT:HIGH 0.0\r");   
				result = viWrite (PLIF_Agilent_handle, agilent_string, (unsigned int)strlen(agilent_string), VI_NULL);

				//Turn burst back on	
				sprintf (agilent_string, "BURST:STATE OFF\r");   
				result = viWrite (PLIF_Agilent_handle, agilent_string, (unsigned int)strlen(agilent_string), VI_NULL);			
				
				//Turn voltage on low before restarting simmer, then turn back up
				sprintf (agilent_string, ":VOLT:HIGH %f\r", agilent_highlevel);   
				result = viWrite (PLIF_Agilent_handle, agilent_string, (unsigned int)strlen(agilent_string), VI_NULL);
				
				//Turn output back on
				sprintf (agilent_string, "OUTP ON\r");   
				result = viWrite (PLIF_Agilent_handle, agilent_string, (unsigned int)strlen(agilent_string), VI_NULL);			
			}
	}
	
	//Discard housekeeping panel
	DiscardPanel(PulsedLIF_panel);
	
	//Close sessions to the devices
	viClose(PLIF_Agilent_handle);
	viClose(PLIF_Rigol_handle);
}	



/******************************************************************************/
//Set up Rigol Scope for photodiode measurement for PLIF
void PLIFScope(void)	
{
	int 		result; 					//	Error-checking variable
	float		PLIF_CenterTime;
	float		PLIF_VoltageRange1;
	float		PLIF_Chan1_Offset;
	float		PLIF_VoltageRange2;
	float		PLIF_Chan2_Offset;
	float		PLIF_TriggerLevel;
	char		PLIF_TriggerSource[8];
	char		PulsedLIF_string[32]="Null";
	ViUInt32 	count;
	
	//Open up PLIF settings panel and get settings
	PulsedLIF_panel = LoadPanel (0, "PulsedLIFSettings.uir", PLIF);
	RecallPanelState (PulsedLIF_panel, "Master_Control_Storage_File", PulsedLIF_setup_state);
		
	//Get sample rate, record length, and channel scales
	GetCtrlVal (PulsedLIF_panel, PLIF_Pulsed_RecordLength,&PLIF_RecordLength);
	GetCtrlVal (PulsedLIF_panel, PLIF_Pulsed_CenterTime,&PLIF_CenterTime);
	GetCtrlVal (PulsedLIF_panel, PLIF_Pulsed_VoltageRange1,&PLIF_VoltageRange1);
	GetCtrlVal (PulsedLIF_panel, PLIF_Pulsed_Chan1_Offset,&PLIF_Chan1_Offset);
	GetCtrlVal (PulsedLIF_panel, PLIF_Pulsed_VoltageRange2,&PLIF_VoltageRange2);
	GetCtrlVal (PulsedLIF_panel, PLIF_Pulsed_Chan2_Offset,&PLIF_Chan2_Offset);
	GetCtrlVal (PulsedLIF_panel, PLIF_Pulsed_TriggerSource,PLIF_TriggerSource);
	GetCtrlVal (PulsedLIF_panel, PLIF_Pulsed_TriggerLevel,&PLIF_TriggerLevel);
	GetCtrlVal (PulsedLIF_panel, PLIF_Pulsed_TimeSpan,&PLIF_TimeSpan);

	//Get Rigol ip address from interface panel and open connection to the scope
	GetCtrlVal (PulsedLIF_panel, PLIF_RIGOLAddress, PLIF_IP_Address_string);
 	result=viOpen (Global_Visa_Session_Handle, PLIF_IP_Address_string, VI_NULL, VI_NULL, &PLIF_Scope_Rigol_handle);

	if (!result) {
		//Make sure scope is running so commands will work
		sprintf (PulsedLIF_string, ":RUN\r");   																							//Send the run command
		result = viWrite (PLIF_Scope_Rigol_handle, PulsedLIF_string, (unsigned int)strlen(PulsedLIF_string), &count);

		//Setup the rate, record length, trigger system, and voltage range for the Rigol scope 
		sprintf (PulsedLIF_string, ":CHAN1:DISP ON\r");   
		result = viWrite (PLIF_Scope_Rigol_handle, PulsedLIF_string, (unsigned int)strlen(PulsedLIF_string), &count);

		sprintf (PulsedLIF_string, ":CHAN2:DISP ON\r");   
		result = viWrite (PLIF_Scope_Rigol_handle, PulsedLIF_string, (unsigned int)strlen(PulsedLIF_string), &count);

		sprintf (PulsedLIF_string, ":CHAN1:COUP AC\r");   
		result = viWrite (PLIF_Scope_Rigol_handle, PulsedLIF_string, (unsigned int)strlen(PulsedLIF_string), &count);

		sprintf (PulsedLIF_string, ":CHAN2:COUP AC\r");   
		result = viWrite (PLIF_Scope_Rigol_handle, PulsedLIF_string, (unsigned int)strlen(PulsedLIF_string), &count);

		sprintf (PulsedLIF_string, ":SINGLE\r");   
		result = viWrite (PLIF_Scope_Rigol_handle, PulsedLIF_string, (unsigned int)strlen(PulsedLIF_string), &count);

		sprintf (PulsedLIF_string, ":ACQ:MDEP %d\r",PLIF_RecordLength);   															//Set the record length by setting the Memory Depth and upload the record length in k
		result = viWrite (PLIF_Scope_Rigol_handle, PulsedLIF_string, (unsigned int)strlen(PulsedLIF_string), &count);

	  	sprintf (PulsedLIF_string, ":TIM:MAIN:SCAL %f\r",(PLIF_TimeSpan/10.0/1.0E9));   											//Set up timebase scaling based on target sample window and 10 divisions
		result = viWrite (PLIF_Scope_Rigol_handle, PulsedLIF_string, (unsigned int)strlen(PulsedLIF_string), &count);
		
		sprintf (PulsedLIF_string, ":TIM:OFFS %f\r",PLIF_CenterTime);   															//Set center of trigger window
		result = viWrite (PLIF_Scope_Rigol_handle, PulsedLIF_string, (unsigned int)strlen(PulsedLIF_string), &count);
			
		sprintf (PulsedLIF_string, ":CHAN1:SCAL  %f\r",PLIF_VoltageRange1);   														//Set the voltage scale of the channel to the target value
		result = viWrite (PLIF_Scope_Rigol_handle, PulsedLIF_string, (unsigned int)strlen(PulsedLIF_string), &count);

		sprintf (PulsedLIF_string, ":CHAN2:SCAL  %f\r",PLIF_VoltageRange2);   														//Set the voltage scale of the channel to the target value
		result = viWrite (PLIF_Scope_Rigol_handle, PulsedLIF_string, (unsigned int)strlen(PulsedLIF_string), &count);
			
		sprintf (PulsedLIF_string, ":CHAN1:OFFS  %f\r",PLIF_Chan1_Offset);   														//Set the voltage offset of the channel to the target value
		result = viWrite (PLIF_Scope_Rigol_handle, PulsedLIF_string, (unsigned int)strlen(PulsedLIF_string), &count);

		sprintf (PulsedLIF_string, ":CHAN2:OFFS  %f\r",PLIF_Chan2_Offset);   														//Set the voltage offset of the channel to the target value
		result = viWrite (PLIF_Scope_Rigol_handle, PulsedLIF_string, (unsigned int)strlen(PulsedLIF_string), &count);
			
		sprintf (PulsedLIF_string, ":TRIG:MODE EDGE\r");   
		result = viWrite (PLIF_Scope_Rigol_handle, PulsedLIF_string, (unsigned int)strlen(PulsedLIF_string), &count);

		sprintf (PulsedLIF_string, ":TRIG:COUP AC\r");   
		result = viWrite (PLIF_Scope_Rigol_handle, PulsedLIF_string, (unsigned int)strlen(PulsedLIF_string), &count);

		sprintf (PulsedLIF_string, ":TRIG:EDGE:SOUR %s\r",PLIF_TriggerSource);  															//Set the trigger source to the target value 
		result = viWrite (PLIF_Scope_Rigol_handle, PulsedLIF_string, (unsigned int)strlen(PulsedLIF_string), &count);

		sprintf (PulsedLIF_string, ":TRIG:EDGE:SLOP POS\r");   
		result = viWrite (PLIF_Scope_Rigol_handle, PulsedLIF_string, (unsigned int)strlen(PulsedLIF_string), &count);

		sprintf (PulsedLIF_string, ":TRIG:EDGE:LEV %f\r",PLIF_TriggerLevel);   
		result = viWrite (PLIF_Scope_Rigol_handle, PulsedLIF_string, (unsigned int)strlen(PulsedLIF_string), &count);
		
		sprintf (PulsedLIF_string, ":SYST:KEY:PRES MOFF\r");   												 							//Hide menu on scope screen
	 	result = viWrite (PLIF_Scope_Rigol_handle, PulsedLIF_string, (unsigned int)strlen(PulsedLIF_string), &count);	
	}	
	
	//Discard Pulsed panel
	DiscardPanel(PulsedLIF_panel);
	
	//Close sessions to the devices
	viClose(PLIF_Scope_Rigol_handle);

}


//Function to calculate the actual target laser firing times so they can be accessed by the master trigger system
void Get_PLIF_Laser_Offsets (float *PLIF_offset, float *PLIF_Laser_Warmup)
{
	float time1;
	float warmup;
	
	//Open up PLIF settings panel and get settings
	PulsedLIF_panel = LoadPanel (0, "PulsedLIFSettings.uir", PLIF);
	RecallPanelState (PulsedLIF_panel, "Master_Control_Storage_File", PulsedLIF_setup_state);
	   
	//Get Laser delays
	GetCtrlVal (PulsedLIF_panel, PLIF_Q850_InstrinsicDelay,&PLIF_InstrinsicDelay);
	GetCtrlVal (PulsedLIF_panel, PLIF_Q850_Q_delay,&PLIF_Q_delay);
	GetCtrlVal (PulsedLIF_panel, PLIF_Q850_Circuit_Delay,&PLIF_Circuit_Delay);

	//Get PLIF warmup times
 	GetCtrlVal (PulsedLIF_panel, PLIF_PLIF_Warmup,&warmup);
	
	//Update values that will be sent to master trigger interface
	time1 = (PLIF_InstrinsicDelay+PLIF_Q_delay+PLIF_Circuit_Delay)/1.0E6;
	*PLIF_offset = time1;
	*PLIF_Laser_Warmup = warmup;

	//Close panel in background
	DiscardPanel(PulsedLIF_panel); 
		
}



//Activate The LIF System
int PulsedLIFActivate(void)
{
	int		result;
	int		count;
	char	PulsedLIF_string[16]="Null";
	
	//Call laser control code and place laser in simmer mode
	PLIFLaserControl(1);
	
	//Set up Rigol scope 
	PLIFScope();
	return 0;
}




//Arm the timing systems for the lasers, the Andor camera, and the Rigol oscilloscope
int PulsedLIFArm(void)
{
	int		result;
	int		count;
	char	PulsedLIF_string[64]="Null";
		
	//Open connection to the scope for photodiode measurements
	result=viOpen (Global_Visa_Session_Handle, PLIF_IP_Address_string, VI_NULL, VI_NULL, &PLIF_Scope_Rigol_handle);

	//Arm the Rigol Oscilloscope if the scope is there
	if (result > 0) {
		sprintf (PulsedLIF_string, ":TRIG:SWE SINGl\r");   					//Single Trigger Sweep
 		result = viWrite (PLIF_Scope_Rigol_handle, PulsedLIF_string, (unsigned int)strlen(PulsedLIF_string), &count);
	}
	
	//Close communications with PLIF devices
	viClose(PLIF_Scope_Rigol_handle);
	
	return result;
}




//Take the acquired data from the instrument and print it to the common data folder and 
//also push it to the MDS database
void Write_PulsedLIFData(void)
{
	char	ChannelNameString[36];
	char	outfilename[36];
	FILE*	outfile;
	int		j;
	float 	PulsedLIF_timestep=0.0;
	double	*PulsedLIF_Array1;
	double	*PulsedLIF_Array2;
	double	*PulsedLIF_Array3;
	double	*PulsedLIF_Array4;
	double	*timebase;

	//Create temporary data arrays	
	PulsedLIF_Array1 = malloc (PLIF_RecordLength);
	PulsedLIF_Array2 = malloc (PLIF_RecordLength);
	PulsedLIF_Array3 = malloc (PLIF_RecordLength);
	PulsedLIF_Array4 = malloc (PLIF_RecordLength);
	timebase = malloc (PLIF_RecordLength);
	
	//Grab data from all four channels of the oscilloscope
	Rigol(PLIF_Scope_Rigol_handle, PLIF_RecordLength, &PulsedLIF_timestep, PulsedLIF_Array1, PulsedLIF_Array2, PulsedLIF_Array3, PulsedLIF_Array4);   
	
	//Dump resulPLIF to a file
	sprintf(outfilename, RawDataPath);
	strcat(outfilename, ShotNumberString);
	strcat(outfilename, "_");
	strcat(outfilename, "PulsedLIF.txt");
	
	outfile=fopen (outfilename, "w");
	fprintf(outfile,"t, PLIF Signal, PLIF Photodiode \n");
	for (j=0;j<PLIF_RecordLength;j++) {
		timebase[j]=(double)(j*PulsedLIF_timestep);
		fprintf(outfile,"%f,%f,%f\n",timebase[j],PulsedLIF_Array1[j],PulsedLIF_Array2[j]);
	}

	//Append the laser power and wavelength values to the end of the file
	for (j=0;j<75;j++) {
		fprintf(outfile,"%f,%f\n",Laser_Power[j],PLIF_Wavelength[j]);
	}
	
 	//Close Housekeeping data file
	fclose (outfile);
	
	//Open up Housekeepings settings panel and get filenames for each measuremen
	PulsedLIF_panel = LoadPanel (0, "PulsedLIFSystems.uir", PLIF);
	RecallPanelState (PulsedLIF_panel, "Master_Control_Storage_File", PulsedLIF_setup_state);
		
	
	//Get channel name string, store raw data on drive folder, and push data to MDSPlus for Channel 0
	GetCtrlVal (PulsedLIF_panel, PLIF_Chan1Name,ChannelNameString);
	Write_to_PHASMA_MDS(timebase,PulsedLIF_Array1,ChannelNameString);

	//Get channel name string, store raw data on drive folder, and push data to MDSPlus for Channel 1
	GetCtrlVal (PulsedLIF_panel, PLIF_Chan2Name,ChannelNameString);
	Write_to_PHASMA_MDS(timebase,PulsedLIF_Array2,ChannelNameString);

	//Get channel name string,  and push data to MDSPlus for Channel 1
	GetCtrlVal (PulsedLIF_panel, PLIF_Chan3Name,ChannelNameString);
	Write_to_PHASMA_MDS(Laser_Power,PLIF_Wavelength,ChannelNameString);
}



//Launch this routine as window opens and it updates the window with the latest values from the 
//control system as it loads.
void OpenPulsedLIF_Settings (void)  
{
	//Open settings panel and load saved values
	PulsedLIF_panel = LoadPanel (0, "PulsedLIFSettings.uir", PLIF);
	RecallPanelState (PulsedLIF_panel, "Master_Control_Storage_File", PulsedLIF_setup_state);
	DisplayPanel(PulsedLIF_panel);
	
	//Update panel with 

	
	// Start interacting with user
    RunUserInterface ();

}




int CVICALLBACK Close_PulsedLIF_Settings (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			//Update the values to the instrumenPLIF
			PulsedLIFActivate();
			
			//Save current state of panel and close interface
			SavePanelState (panel, "Master_Control_Storage_File", PulsedLIF_setup_state);
			DiscardPanel(panel);
			break;
	}
	return 0;
}							

int CVICALLBACK AngstromReadFunction (void *functionData)
{   

	/* 	This function just starts up a thread for the wavemeter and power meter readings 
	    measured values for a set number of averages
		
		Created 	5/22/2025	E. Scime	*/

    int		result;
	int		count;
	int		pulse_count;
	char	GenTec_string[16];
	
	
	pulse_count=0;
	
	while (!Angstrom_gExiting)
    {
		//Delay 100 ms
		Delay(0.1);
		
		//Read 75 pulses at the 10 Hz laser frequency, store them in a global variable, then end the thread.
		sprintf (GenTec_string, "*CVU?\r");
		result = viWrite (GenTec_handle, GenTec_string, (unsigned int)strlen(GenTec_string), &count);
		result = viRead (GenTec_handle, GenTec_string, 16, &count);
		Laser_Power[pulse_count] = strtod (GenTec_string, NULL);
		
		result = GetWavelength(PLIF_Wavelength[pulse_count]);
		
		
		pulse_count=pulse_count+1;
		if(pulse_count>74) {
			Angstrom_gExiting=1;
		}

	}
		

	//return to primary code
    return 0;
}



void ReadPulsedLIF_WaveandEnergy (void)  
{
//Read laser pulse energy from power meter (need to make sure last power measurement is what is read)
	//set buad rate to 115200, send command *CVU?, then read buffer get laser power. Store it in first row of 
	//PLIF file
	
	//Read laser wavelength from Angstrom pulsed wavemeter. Store this information as well.

	int			result;
	int 		AngstromWavemeter_handle;
	char		GenTecPort_String[64];
	char		AngstromWavemeterAdd_String[64];
	
	//Open communication with the Gentex power meter and the Angstrom Wavelength meter
	//Open settings panel in the background
	//Open up PLIF settings panel and get settings
	PulsedLIF_panel = LoadPanel (0, "PulsedLIFSettings.uir", PLIF);
	RecallPanelState (PulsedLIF_panel, "Master_Control_Storage_File", PulsedLIF_setup_state);

	//Get GenTec Address
	GetCtrlVal (PulsedLIF_panel, PLIF_GenTecPort,GenTecPort_String);

	//Open Visa session to GenTec Power meter
	result=viOpen (Global_Visa_Session_Handle,GenTecPort_String, VI_NULL, VI_NULL, &GenTec_handle);
	
	//Get Angstrom Wavemeter Address
	//GetCtrlVal (PulsedLIF_panel, PLIF_AngstromWavemeterAdd,AngstromWavemeterAdd_String);

	//Open Visa session to Angstrom Wavemeter (not needed given how the wavemeter works?)
	//result=viOpen (Global_Visa_Session_Handle,AngstromWavemeterAdd_String, VI_NULL, VI_NULL, &AngstromWavemeter_handle);

	//if (!result) {
		//Open communication with Angstrom wavemeter
		result = (int) Instantiate(0,0,0,0);
		
		//Set to operate in pulsed mode
		result = SetPulseMode(1);
		
		//Set to read a wide bandwidth laser
		result = SetWideMode(1);
		
		//Set exposure to default level
		result = SetExposure(0);
		
		//Turn on thread completion variable
		Angstrom_gExiting=0;
	
		//Spawn the thread to read 75 pulses of the pulsed laser power and the wavelength. Also define the function to call when the thread ends
		CmtScheduleThreadPoolFunction (DEFAULT_THREAD_POOL_HANDLE, AngstromReadFunction, NULL, NULL);
	//}
	//Close panel
	DiscardPanel(PulsedLIF_panel);
	
	
}

int CVICALLBACK Wavelength_Current (int panel, int control, int event,
							   void *callbackData, int eventData1, int eventData2)
{
	int 		AngstromWavemeter_handle;
	int 		result=0;
	ViPUInt32	count=0;
	double 		wavelength=0.0;
	char		AngstromWavemeterAdd_String[64];
	char		Angstrom_String[64];

	switch (event)
	{
		case EVENT_COMMIT:
			//Get Angstrom Wavemeter Address
			//GetCtrlVal (PulsedLIF_panel, PLIF_AngstromWavemeterAdd,AngstromWavemeterAdd_String);

			//Open Visa session to Angstrom Wavemeter (not needed given how the wavemeter works?)
			//result=viOpen (Global_Visa_Session_Handle,AngstromWavemeterAdd_String, VI_NULL, VI_NULL, &AngstromWavemeter_handle);

			//if (!result) {
				//Open communication with Angstrom wavemeter
				result = (int)Instantiate(0,0,0,0);
				
				
				//Set to operate in pulsed mode
				result = SetPulseMode(1);
				
				//Set to read a wide bandwidth laser
				result = SetWideMode(1);
				
				//Set exposure to default level
				result = SetExposure(0);
				
				//Get current laser wavelength from Angstrom
				GetWavelength(wavelength);			
			//}
			
			//Set current wavelength in panel
			SetCtrlVal (PulsedLIF_panel, PLIF_Current_Wavelength,wavelength);
			break;
	}
	return 0;
}

int CVICALLBACK Wavelength_Move_to_Start (int panel, int control, int event,
							   void *callbackData, int eventData1, int eventData2)
{
	int 	result;
	int 	count;
	int 	Cobra_handle;
	float 	Wavelength_Start;
	char	CobraPort_String[64];
	char	Cobra_String[64];

	switch (event)
	{
		case EVENT_COMMIT:
			//Get current position of probe, starting position, end position, and what to do at end of scan range
			GetCtrlVal (PulsedLIF_panel, PLIF_Wavelength_Start,&Wavelength_Start);
			
			//Open Visa session to Cobra stretch laster
			GetCtrlVal (PulsedLIF_panel, PLIF_Cobra_Port,CobraPort_String);
			result=viOpen (Global_Visa_Session_Handle,Cobra_String, VI_NULL, VI_NULL, &Cobra_handle);
				
			//Send command to move laser
			sprintf (Cobra_String, "LASER:WAVELENGTH %f\r",Wavelength_Start);   																										
			result = viWrite (Cobra_handle, Cobra_String, (unsigned int)strlen(Cobra_String), VI_NULL);
				
			//update current laser wavelength value
			Wavelength_Current (panel, control, event,&callbackData, eventData1,eventData2);

			break;
	}
	return 0;
}


int CVICALLBACK Wavelength_Move_to_Stop (int panel, int control, int event,
							   void *callbackData, int eventData1, int eventData2)
{
	int 	result;
	int 	count;
	int 	Cobra_handle;
	float 	Wavelength_Stop;
	char	CobraPort_String[64];
	char	Cobra_String[64];

	switch (event)
	{
		case EVENT_COMMIT:
			//Get current position of probe, starting position, end position, and what to do at end of scan range
			GetCtrlVal (PulsedLIF_panel, PLIF_Wavelength_Stop,&Wavelength_Stop);
			
			//Open Visa session to Cobra stretch laster
			GetCtrlVal (PulsedLIF_panel, PLIF_Cobra_Port,CobraPort_String);
			result=viOpen (Global_Visa_Session_Handle,Cobra_String, VI_NULL, VI_NULL, &Cobra_handle);
				
			//Send command to move laseresult = viWrite (Cobra_handle, Cobra_String, (unsigned int)strlen(Cobra_String), &count);
			sprintf (Cobra_String, "LASER:WAVELENGTH %f\r",Wavelength_Stop);   																										
			result = viWrite (Cobra_handle, Cobra_String, (unsigned int)strlen(Cobra_String), VI_NULL);
				
			//update current laser wavelength value
			Wavelength_Current (panel, control, event,&callbackData, eventData1,eventData2);

			break;
	}
	return 0;
}

float Get_PLIF_Wavelength (void)  
{
	//Read laser wavelegnth from the Angstrom wavelength meter
	int 		AngstromWavemeter_handle;
	int 		result=0;
	ViPUInt32	count=0;
	float 		wavelength=0.0;
	char		AngstromWavemeterAdd_String[64];
	char		Angstrom_String[64];

	//Open up PLIF settings panel and get settings
	PulsedLIF_panel = LoadPanel (0, "PulsedLIFSettings.uir", PLIF);
	RecallPanelState (PulsedLIF_panel, "Master_Control_Storage_File", PulsedLIF_setup_state);

	//Get Angstrom Wavemeter Address
	//GetCtrlVal (PulsedLIF_panel, PLIF_AngstromWavemeterAdd,AngstromWavemeterAdd_String);

	//Open Visa session to Angstrom Wavemeter (not needed given how the wavemeter works?)
	//result=viOpen (Global_Visa_Session_Handle,AngstromWavemeterAdd_String, VI_NULL, VI_NULL, &AngstromWavemeter_handle);

	//if (!result) {
		//Open communication with Angstrom wavemeter
		Instantiate(0,0,0,0);
				
		//Set to operate in pulsed mode
		result = SetPulseMode(1);
				
		//Set to read a wide bandwidth laser
		result = SetWideMode(1);
				
		//Set exposure to default level
		result = SetExposure(0);
				
		//Get current laser wavelength from Angstrom
		GetWavelength(wavelength);			
	//}

	//Close panel
	DiscardPanel(PulsedLIF_panel);

	return wavelength;	
}

//This function advances the Bdot probe a single step per the spacing requested in the interface
void Advance_PLIF_Laser(void)
{
	int 	result=0;
	int 	Cobra_handle;
	float 	Current_Wavelength;
	float 	Wavelength_Start;
	float 	Wavelength_Stop;
	float 	Wavelength_Inc;
	float 	New_Wavelength;
	char	CobraPort_String[64];
	char	Cobra_String[64];
	
	//Open up PLIF settings panel and get settings
	PulsedLIF_panel = LoadPanel (0, "PulsedLIFSettings.uir", PLIF);
	RecallPanelState (PulsedLIF_panel, "Master_Control_Storage_File", PulsedLIF_setup_state);
	
	//Get current position of probe, starting position, end position, and what to do at end of scan range
	GetCtrlVal (PulsedLIF_panel, PLIF_Wavelength_Start,&Wavelength_Start);
	GetCtrlVal (PulsedLIF_panel, PLIF_Wavelength_Stop,&Wavelength_Stop);
	GetCtrlVal (PulsedLIF_panel, PLIF_Wavelength_Increment,&Wavelength_Inc);
	
	//Get current wavelength
	Current_Wavelength = Get_PLIF_Wavelength();
	
	//Calculate next wavelength
	New_Wavelength=Current_Wavelength + Wavelength_Inc;
	
	//Only move laser if still within range of scan
	if (New_Wavelength < Wavelength_Stop) {
		//Open Visa session to Cobra stretch laster
		GetCtrlVal (PulsedLIF_panel, PLIF_Cobra_Port,CobraPort_String);
		result=viOpen (Global_Visa_Session_Handle,Cobra_String, VI_NULL, VI_NULL, &Cobra_handle);
					
		if (!result) {
			//Send command to move laser
			sprintf (Cobra_String, "LASER:WAVELENGTH %f\r",New_Wavelength);   																										
			result = viWrite (Cobra_handle, Cobra_String, (unsigned int)strlen(Cobra_String), VI_NULL);
		}
	}

}

