#include <utility.h>
#include <ansi_c.h>
#include <visa.h>    
#include <userint.h>
#include "GlobalVariables.h"
#include "ThomsonScatteringSystems.h"   
#include "PulsedLIFSystems.h"   
#include "MasterTriggerSettings.h"
#include "MasterTriggerSystem.h"


//Global Variables for settings panel
int			TriggerSettings_setup_state = 4;	//WARNING, if this gets changed here, it must also be changed in the mastercontrol loop repeat cycle subroutine
int			Trigger_panel;


void OpenTrigger_Settings (void)  
{
	float 	Laser_Warmup;
	float 	PerpTS_Offset;
	float 	ParaTS_Offset;

	
	//Open settings panel and load saved values
	Trigger_panel = LoadPanel (0, "MasterTriggerSettings.uir", Trigger);
	RecallPanelState (Trigger_panel, "Master_Control_Storage_File", TriggerSettings_setup_state);
	
	//Update master cycle time from global variable
	SetCtrlVal (Trigger_panel, Trigger_Total_Cycle,Master_cycle_time);
	
	//Update laser warmup time from laser interface
	Get_TS_Laser_Offsets(&PerpTS_Offset, &ParaTS_Offset, &Laser_Warmup);
	SetCtrlVal (Trigger_panel, Trigger_SRSDG1_ChannelA,Laser_Warmup);
	
	DisplayPanel(Trigger_panel);

	// Start interacting with user
    RunUserInterface ();

}

int CVICALLBACK Close_Trigger_Settings_Activate (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{

	switch (event)
		{
			case EVENT_COMMIT:
				//Save Settings
				SavePanelState (Trigger_panel, "Master_Control_Storage_File", TriggerSettings_setup_state);
				
				//Update values to instruments
				Trigger_Settings_Activate();
				
				//Discard panel
				DiscardPanel(panel);
				break;
		}
	return 0;
}


//The objective of this function is to store the
//timing settings from a user to a file of their choosing. These settings 
// can then be read into the timing panel so different
//people can run different experimental configurations more easily
int CVICALLBACK Save_Trigger_to_File (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	char 	filename_string[64];

	switch (event)
		{
			case EVENT_COMMIT:
				GetCtrlVal (Trigger_panel, Trigger_Settings,filename_string);
				SavePanelState (Trigger_panel, filename_string, 1);				//Use identifying state = 1 since a custom file
				break;
		}
	return 0;
}

//The objective of this function is to store the
//timing settings from a user to a file of their choosing. These settings 
// can then be read into the timing panel so different
//people can run different experimental configurations more easily
int CVICALLBACK Read_Trigger_from_File (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	char 	filename_string[260];
	int 	status;
	
	switch (event)
		{
			case EVENT_COMMIT:
				status = FileSelectPopupEx ("", "*.*", "", "", VAL_LOAD_BUTTON, 0, 0, filename_string);
				RecallPanelState (Trigger_panel, filename_string, 1); //Use identifying state = 1 since a custom file
				
				//Update the panel in use
				DisplayPanel(Trigger_panel);
				break;
		}
	return 0;
}

//Create the task that will be called to trigger the master trigger from the NI DAQ card	
void SetUpNI_Card_Trig(void)
{
	double	Pulse_length = 100.0;		//Define length of trigger pulse to be 100 ms
		
	//Shutdown any old tasks
	DAQmxClearTask (OutputCardTrigHandle);

	//Setup I/O card for creation of trigger signal
	DAQmxCreateTask ("", &OutputCardTrigHandle);

	//create a user defined TTL output pulse using DAQ card
	DAQmxCreateCOPulseChanFreq (OutputCardTrigHandle, "Dev1/ctr1", "", DAQmx_Val_Hz, DAQmx_Val_Low, 0.0, .01, 0.01*Pulse_length/1000.0);
	
	//Set up mode of timing to create a single pulse
	DAQmxCfgImplicitTiming (OutputCardTrigHandle, DAQmx_Val_FiniteSamps, 1);
}
			
int MasterTriggerArm(void)
{
	char		Trigger_string[64]="Null";
	char 		Trigger1_Port_string[36];
	char 		Trigger2_Port_string[36];
	char 		Trigger3_Port_string[36];
	
	ViSession 	Trigger1_handle;
	ViSession 	Trigger2_handle;
	ViSession 	Trigger3_handle;
	
	ViStatus 	result;
	ViUInt32 	count;

	//Open settings panel and load saved values
	Trigger_panel = LoadPanel (0, "MasterTriggerSettings.uir", Trigger);
	RecallPanelState (Trigger_panel, "Master_Control_Storage_File", TriggerSettings_setup_state);

	/***********************************************************************************************/
	//Get the port address of the first SRSDG645
	GetCtrlVal (Trigger_panel, Trigger_Trigger_IP_1,Trigger1_Port_string);

	/***********************************************************************************************/
	//Get the port address of the second SRSDG645
	GetCtrlVal (Trigger_panel, Trigger_Trigger_IP_2,Trigger2_Port_string);

	/***********************************************************************************************/
	//Get the port address of the third SRSDG645
	GetCtrlVal (Trigger_panel, Trigger_Trigger_IP_3,Trigger3_Port_string);

	/***********************************************************************************************/
	//Open Visa session to first Trigger unit
	result=viOpen (Global_Visa_Session_Handle, Trigger1_Port_string, VI_NULL, VI_NULL, &Trigger1_handle);
	
	if (!result) {
		//Send commands to set up each of the timing channels
		sprintf (Trigger_string, "*TRG\r");   //Arm the trigger unit to get ready for external trigger
		result = viWrite (Trigger1_handle, Trigger_string, (unsigned int)strlen(Trigger_string), &count);	
	} //End check to see if device session opened

	/***********************************************************************************************/
	//Open Visa session to second Trigger unit
	result=viOpen (Global_Visa_Session_Handle, Trigger2_Port_string, VI_NULL, VI_NULL, &Trigger2_handle);
				
	if (!result) {
		//Send commands to set up each of the timing channels
		sprintf (Trigger_string, "*TRG\r");   //Arm the trigger unit to get ready for external trigger
		result = viWrite (Trigger2_handle, Trigger_string, (unsigned int)strlen(Trigger_string), &count);
	} //End check to see if device session opened
	
	/***********************************************************************************************/
	//Open Visa session to third Trigger unit
	result=viOpen (Global_Visa_Session_Handle, Trigger3_Port_string, VI_NULL, VI_NULL, &Trigger3_handle);
	
	if (!result) {
		sprintf (Trigger_string, "*TRG\r");   //Arm the trigger unit to get ready for external trigger
		result = viWrite (Trigger3_handle, Trigger_string, (unsigned int)strlen(Trigger_string), &count);
	} //End check to see if device session opened
	
	//Discard panel
	DiscardPanel(Trigger_panel); 
	
	//Close all trigger units
	viClose(Trigger1_handle);
	viClose(Trigger2_handle);
	viClose(Trigger3_handle);
	
	return !result;
}

/*
For this instrument, the channel identifier and the corresponding channel are given below
		Delay channel: Value Channel 
					0 T0 
					1 T1 
					2 A 
					3 B 
					4 C 
					5 D 
					6 E 
					7 F 
					8 G 
					9 H 
So for example
	DLAY 2,0,10e-6 		Will set channel A delay to equal channel T0 plus 10 µs.

*/

void Trigger_Settings_Activate (void)
{
	char		Trigger_string[32]="Null";
	char 		Trigger1_Port_string[36];
	char 		Trigger2_Port_string[36];
	char 		Trigger3_Port_string[36];
	
	
	int			polarity;
	extern int 	PulsedLIFFlag;		//Sort of sketchy way to do this, but look at flags from master control system using an external variable declaration
	extern int 	TSFlag;				//Sort of sketchy way to do this, but look at flags from master control system using an external variable declaration
	
	float		SRSDG1_A;
	float		SRSDG1_B;
	float		SRSDG1_C;
	float		SRSDG1_D;
	float		SRSDG1_E;
	float		SRSDG1_F;
	float		SRSDG1_G;
	float		SRSDG1_H;
	float 		SRSDG1_AB_High;
	float 		SRSDG1_AB_Low;
	float 		SRSDG1_CD_High;
	float 		SRSDG1_CD_Low;
	float 		SRSDG1_EF_High;
	float 		SRSDG1_EF_Low;
	float 		SRSDG1_GH_High;
	float 		SRSDG1_GH_Low;
	float		SRSDG2_A;
	float		SRSDG2_B;
	float		SRSDG2_C;
	float		SRSDG2_D;
	float		SRSDG2_E;
	float		SRSDG2_F;
	float		SRSDG2_G;
	float		SRSDG2_H;
	float 		SRSDG2_AB_High;
	float 		SRSDG2_AB_Low;
	float 		SRSDG2_CD_High;
	float 		SRSDG2_CD_Low;
	float 		SRSDG2_EF_High;
	float 		SRSDG2_EF_Low;
	float 		SRSDG2_GH_High;
	float 		SRSDG2_GH_Low;
	float		SRSDG3_A;
	float		SRSDG3_B;
	float		SRSDG3_C;
	float		SRSDG3_D;
	float		SRSDG3_E;
	float		SRSDG3_F;
	float		SRSDG3_G;
	float		SRSDG3_H;
	float 		SRSDG3_AB_High;
	float 		SRSDG3_AB_Low;
	float 		SRSDG3_CD_High;
	float 		SRSDG3_CD_Low;
	float 		SRSDG3_EF_High;
	float 		SRSDG3_EF_Low;
	float 		SRSDG3_GH_High;
	float 		SRSDG3_GH_Low;
	
	float 		Total_Cycle_Time;
	float		Laser_Warmup_Pulse_Length;
	float		Laser_Warmup_High;
	float		Laser_Warmup_Low;
	float		Gun_Charging_Pulse_Length;
	float		Gun_Charging_High;
	float		Gun_Charging_Low;
	float		Astrella_Synch;
	float		Astrella_Synch_Pulse_Length;
	float		Astrella_Synch_High;
	float		Astrella_Synch_Low;
	float		iCCD_Shutter;
	float		iCCD_Shutter_Pulse_Length;
	float		iCCD_Shutter_High;
	float		iCCD_Shutter_Low;
	float		Gas_Puff;
	float		Gas_Puff_Pulse_Length;
	float		Gas_Puff_High;
	float		Gas_Puff_Low;
	float		Arc;
	float		Arc_Pulse_Length;
	float		Arc_High;
	float		Arc_Low;
	float		Bias1;
	float		Bias1_Pulse_Length;
	float		Bias1_High;
	float		Bias1_Low;
	float		Bias2;
	float		Bias2_Pulse_Length;
	float		Bias2_High;
	float		Bias2_Low;
	float		PerpTS;
	float		PerpTS_Pulse_Length;
	float		PerpTS_High;
	float		PerpTS_Low;
	float		ParaTS;
	float		ParaTS_Pulse_Length;
	float		ParaTS_High;
	float		ParaTS_Low;
	float		PLIF;
	float		PLIF_Pulse_Length;
	float		PLIF_High;
	float		PLIF_Low;
	float		Other;
	float		Other_Pulse_Length;
	float		Other_High;
	float		Other_Low;
	float 		Adjusted_Laser_Warmup=0.0;		//Default setting of 30 seconds
	float 		PerpTS_Offset;
	float 		ParaTS_Offset;
	float 		PLIF_Offset;
	float 		Other_Offset=0.0;
	float 		TSLaser_Warmup;
	float 		PLIF_Laser_Warmup;
	
	ViSession 	Trigger1_handle;
	ViSession 	Trigger2_handle;
	ViSession 	Trigger3_handle;
	
	ViStatus 	result;
	ViUInt32 	count;

	//Open settings panel and load saved values
	Trigger_panel = LoadPanel (0, "MasterTriggerSettings.uir", Trigger);
	RecallPanelState (Trigger_panel, "Master_Control_Storage_File", TriggerSettings_setup_state);

	//Get the overall cycle time from the front panel
	GetCtrlVal (Trigger_panel, Trigger_Total_Cycle,&Total_Cycle_Time);
	
	//Save this value of the cycle time to the global cycle time variable
	Master_cycle_time=Total_Cycle_Time;


	/***********************************************************************************************/
	//Get the port address of the first SRSDG645
	GetCtrlVal (Trigger_panel, Trigger_Trigger_IP_1,Trigger1_Port_string);

	//Get all the timing values associated with the first delay generator
	GetCtrlVal (Trigger_panel, Trigger_SRSDG1_ChannelA,&Laser_Warmup);
	GetCtrlVal (Trigger_panel, Trigger_SRSDG1_ChannelB,&Laser_Warmup_Pulse_Length);
	GetCtrlVal (Trigger_panel, Trigger_SRSDG1_V_High_AB,&Laser_Warmup_High);
	GetCtrlVal (Trigger_panel, Trigger_SRSDG1_V_Low_AB,&Laser_Warmup_Low);

	GetCtrlVal (Trigger_panel, Trigger_SRSDG1_ChannelC,&Gun_Charging);
	GetCtrlVal (Trigger_panel, Trigger_SRSDG1_ChannelD,&Gun_Charging_Pulse_Length);
	GetCtrlVal (Trigger_panel, Trigger_SRSDG1_V_High_CD,&Gun_Charging_High);
	GetCtrlVal (Trigger_panel, Trigger_SRSDG1_V_Low_CD,&Gun_Charging_Low);
	
	//If master cycle time is shorter than laser warmup, correct master cycle time.
	if (Laser_Warmup >= Total_Cycle_Time)   {
		Total_Cycle_Time=1.0+Laser_Warmup;
		//Save this value of the cycle time to the global cycle time variable
		Master_cycle_time=Total_Cycle_Time;
		//set the overall cycle time to the new value
		SetCtrlVal (Trigger_panel, Trigger_Total_Cycle,Total_Cycle_Time);
	}

	GetCtrlVal (Trigger_panel, Trigger_SRSDG1_ChannelE,&Astrella_Synch);
	GetCtrlVal (Trigger_panel, Trigger_SRSDG1_ChannelF,&Astrella_Synch_Pulse_Length);
	Astrella_Synch=Astrella_Synch/1.0E6;													//Convert from microseconds
	GetCtrlVal (Trigger_panel, Trigger_SRSDG1_V_High_EF,&Astrella_Synch_High);
	GetCtrlVal (Trigger_panel, Trigger_SRSDG1_V_Low_EF,&Astrella_Synch_Low);

	GetCtrlVal (Trigger_panel, Trigger_SRSDG1_ChannelG,&iCCD_Shutter);
	GetCtrlVal (Trigger_panel, Trigger_SRSDG1_ChannelH,&iCCD_Shutter_Pulse_Length);
	iCCD_Shutter_Pulse_Length=iCCD_Shutter_Pulse_Length/1.0E6;								//Convert from microseconds	
	GetCtrlVal (Trigger_panel, Trigger_SRSDG1_V_High_GH,&iCCD_Shutter_High);
	GetCtrlVal (Trigger_panel, Trigger_SRSDG1_V_Low_GH,&iCCD_Shutter_Low);

	/***********************************************************************************************/
	//Get the port address of the second SRSDG645
	GetCtrlVal (Trigger_panel, Trigger_Trigger_IP_2,Trigger2_Port_string);

	//Get all the timing values associated with the first delay generator
	GetCtrlVal (Trigger_panel, Trigger_SRSDG2_ChannelA,&Gas_Puff);
	Gas_Puff=Gas_Puff/1.0E6;
	GetCtrlVal (Trigger_panel, Trigger_SRSDG2_ChannelB,&Gas_Puff_Pulse_Length);
	Gas_Puff_Pulse_Length=Gas_Puff_Pulse_Length/1.0E6;
	GetCtrlVal (Trigger_panel, Trigger_SRSDG2_V_High_AB,&Gas_Puff_High);
	GetCtrlVal (Trigger_panel, Trigger_SRSDG2_V_Low_AB,&Gas_Puff_Low);

	GetCtrlVal (Trigger_panel, Trigger_SRSDG2_ChannelC,&Arc);
	Arc=Arc/1.0E6;
	GetCtrlVal (Trigger_panel, Trigger_SRSDG2_ChannelD,&Arc_Pulse_Length);
	Arc_Pulse_Length=Arc_Pulse_Length/1.0E6;
	GetCtrlVal (Trigger_panel, Trigger_SRSDG2_V_High_CD,&Arc_High);
	GetCtrlVal (Trigger_panel, Trigger_SRSDG2_V_Low_CD,&Arc_Low);

	GetCtrlVal (Trigger_panel, Trigger_SRSDG2_ChannelE,&Bias1);
	Bias1=Bias1/1.0E6;
	GetCtrlVal (Trigger_panel, Trigger_SRSDG2_ChannelF,&Bias1_Pulse_Length);
	Bias1_Pulse_Length=Bias1_Pulse_Length/1.0E6;
	GetCtrlVal (Trigger_panel, Trigger_SRSDG2_V_High_EF,&Bias1_High);
	GetCtrlVal (Trigger_panel, Trigger_SRSDG2_V_Low_EF,&Bias1_Low);

	GetCtrlVal (Trigger_panel, Trigger_SRSDG2_ChannelG,&Bias2);
	Bias2=Bias2/1.0E6;
	GetCtrlVal (Trigger_panel, Trigger_SRSDG2_ChannelH,&Bias2_Pulse_Length);
	Bias2_Pulse_Length=Bias2_Pulse_Length/1.0E6;
	GetCtrlVal (Trigger_panel, Trigger_SRSDG2_V_High_GH,&Bias2_High);
	GetCtrlVal (Trigger_panel, Trigger_SRSDG2_V_Low_GH,&Bias2_Low);

	
	/***********************************************************************************************/
	//Get the port address of the third SRSDG645
	GetCtrlVal (Trigger_panel, Trigger_Trigger_IP_3,Trigger3_Port_string);

	//Get all the timing values associated with the first delay generator
	GetCtrlVal (Trigger_panel, Trigger_SRSDG3_ChannelA,&PerpTS);
	PerpTS=PerpTS/1.0E6;
	GetCtrlVal (Trigger_panel, Trigger_SRSDG3_ChannelB,&PerpTS_Pulse_Length);
	PerpTS_Pulse_Length=PerpTS_Pulse_Length/1.0E6;
	GetCtrlVal (Trigger_panel, Trigger_SRSDG3_V_High_AB,&PerpTS_High);
	GetCtrlVal (Trigger_panel, Trigger_SRSDG3_V_Low_AB,&PerpTS_Low);

	GetCtrlVal (Trigger_panel, Trigger_SRSDG3_ChannelC,&ParaTS);
	ParaTS=ParaTS/1.0E6;
	GetCtrlVal (Trigger_panel, Trigger_SRSDG3_ChannelD,&ParaTS_Pulse_Length);
	ParaTS_Pulse_Length=ParaTS_Pulse_Length/1.0E6;
	GetCtrlVal (Trigger_panel, Trigger_SRSDG3_V_High_CD,&ParaTS_High);
	GetCtrlVal (Trigger_panel, Trigger_SRSDG3_V_Low_CD,&ParaTS_Low);

	GetCtrlVal (Trigger_panel, Trigger_SRSDG3_ChannelE,&PLIF);
	PLIF=PLIF/1.0E6;
	GetCtrlVal (Trigger_panel, Trigger_SRSDG3_ChannelF,&PLIF_Pulse_Length);
	PLIF_Pulse_Length=PLIF_Pulse_Length/1.0E6;
	GetCtrlVal (Trigger_panel, Trigger_SRSDG3_V_High_EF,&PLIF_High);
	GetCtrlVal (Trigger_panel, Trigger_SRSDG3_V_Low_EF,&PLIF_Low);

	GetCtrlVal (Trigger_panel, Trigger_SRSDG3_ChannelG,&Other);
	Other=Other/1.0E6;
	GetCtrlVal (Trigger_panel, Trigger_SRSDG3_ChannelH,&Other_Pulse_Length);
	Other_Pulse_Length=Other_Pulse_Length/1.0E6;
	GetCtrlVal (Trigger_panel, Trigger_SRSDG3_V_High_GH,&Other_High);
	GetCtrlVal (Trigger_panel, Trigger_SRSDG3_V_Low_GH,&Other_Low);

	//Based on the characteristics of the perpendicular Thomson Scattering laser, calculate the
	//optical laser warmup time that would yield exactly an integer number of YAG laser pulses 
	//if the requested warm up time is a multiple of the laser pulse period (e.g., 30 sec = 300 pulses). 
	
	Get_PLIF_Laser_Offsets(&PLIF_Offset,&PLIF_Laser_Warmup);																																										
	if (PulsedLIFFlag) {
		Adjusted_Laser_Warmup = PLIF_Laser_Warmup - (Astrella_Synch+Arc+Bias1+PLIF-PLIF_Offset);				//This uses the sum of the PLIF laser offsets .
	}
	
	Get_TS_Laser_Offsets(&PerpTS_Offset, &ParaTS_Offset, &TSLaser_Warmup);
	if (TSFlag) {
		if (Get_TS_Laser_Offsets(&PerpTS_Offset, &ParaTS_Offset, &TSLaser_Warmup)) {
			Adjusted_Laser_Warmup = TSLaser_Warmup - (Astrella_Synch+Arc+Bias1+PerpTS-PerpTS_Offset);		//This uses the sum of the TS laser offsets.
		}else {
			Adjusted_Laser_Warmup = TSLaser_Warmup - (Astrella_Synch+Arc+Bias1+ParaTS-ParaTS_Offset);		//This uses the sum of the TS laser offsets.
		}
	}

	//Since the perp and para TS trigger times can differ by 100 us at most, using only the perp TS trigger time here is not a major issue.
	/***********************************************************************************************/
	//Calculate all the timings to send to first trigger unit
	SRSDG1_A = Total_Cycle_Time-Adjusted_Laser_Warmup;
	SRSDG1_B = Total_Cycle_Time-Adjusted_Laser_Warmup+Laser_Warmup_Pulse_Length;
	SRSDG1_AB_High = Laser_Warmup_High;
	SRSDG1_AB_Low = Laser_Warmup_Low;
		
	SRSDG1_C = Total_Cycle_Time-Gun_Charging;												//Charging stops when Gun Charging time reached
	SRSDG1_D = Total_Cycle_Time+Gun_Charging_Pulse_Length;									//Charging begins after pulse by amount of charging pulse length
	SRSDG1_CD_High = Gun_Charging_High;
	SRSDG1_CD_Low = Gun_Charging_Low;
	
	SRSDG1_E = Total_Cycle_Time;															//Note, the Astrella_Synch time is added to everything AFTER the triggerings of the next two trigger units because
	SRSDG1_F = Total_Cycle_Time+Astrella_Synch+Astrella_Synch_Pulse_Length;					//it is meant to introduce an offset betweeen the Astrella internal clock and when the Astrella actually fires.
	SRSDG1_CD_High = Astrella_Synch_High;													//That way as the Astrella pulses at its merry 1 kHz rate, one of those pulses will appear at the target time in the
	SRSDG1_CD_Low = Astrella_Synch_Low;														//bias pulse.
	
	SRSDG1_G = Total_Cycle_Time+iCCD_Shutter;								
	SRSDG1_H = Total_Cycle_Time+iCCD_Shutter+iCCD_Shutter_Pulse_Length;
	SRSDG1_CD_High = iCCD_Shutter_High;
	SRSDG1_CD_Low = iCCD_Shutter_Low;
	

	/***********************************************************************************************/
	//Calculate all the timings to send to second trigger unit
	//SRSDG2_A = Total_Cycle_Time-Laser_Warmup+Gas_Puff+Astrella_Synch;
	//SRSDG2_B = Total_Cycle_Time-Laser_Warmup+Gas_Puff+Astrella_Synch+Gas_Puff_Pulse_Length;
	SRSDG2_A = Gas_Puff;
	SRSDG2_B = Gas_Puff+Gas_Puff_Pulse_Length;
	SRSDG2_AB_High = Gas_Puff_High;
	SRSDG2_AB_Low = Gas_Puff_Low;

	//SRSDG2_C = Total_Cycle_Time-Laser_Warmup+Astrella_Synch+Arc;
	//SRSDG2_D = Total_Cycle_Time-Laser_Warmup+Astrella_Synch+Arc+Arc_Pulse_Length;
	SRSDG2_C = Arc;
	SRSDG2_D = Arc+Arc_Pulse_Length;
	SRSDG2_CD_High = Arc_High;
	SRSDG2_CD_Low = Arc_Low;

	//SRSDG2_E = Total_Cycle_Time-Laser_Warmup+Astrella_Synch+Arc+Bias1;
	//SRSDG2_F = Total_Cycle_Time-Laser_Warmup+Astrella_Synch+Arc+Bias1+Bias1_Pulse_Length;
	SRSDG2_E = Arc+Bias1;
	SRSDG2_F = Arc+Bias1+Bias1_Pulse_Length;
	SRSDG2_EF_High = Bias1_High;
	SRSDG2_EF_Low = Bias1_Low;

	//SRSDG2_G = Total_Cycle_Time-Laser_Warmup+Astrella_Synch+Arc+Bias2;
	//SRSDG2_H = Total_Cycle_Time-Laser_Warmup+Astrella_Synch+Arc+Bias2+Bias2_Pulse_Length;
	SRSDG2_G = Arc+Bias2;
	SRSDG2_H = Arc+Bias2+Bias2_Pulse_Length;
	SRSDG2_GH_High = Bias2_High;
	SRSDG2_GH_Low = Bias2_Low;

	
	/***********************************************************************************************/
	//Calculate all the timings to send to third trigger unit
	//SRSDG3_A = Total_Cycle_Time-Laser_Warmup+Astrella_Synch+Arc+Bias1+PerpTS-PerpTS_Offset;
	//SRSDG3_B = Total_Cycle_Time-Laser_Warmup+Astrella_Synch+Arc+Bias1+PerpTS-PerpTS_Offset+PerpTS_Pulse_Length;
	SRSDG3_A = Arc+Bias1+PerpTS-PerpTS_Offset;
	SRSDG3_B = Arc+Bias1+PerpTS-PerpTS_Offset+PerpTS_Pulse_Length;
	SRSDG3_AB_High = PerpTS_High;
	SRSDG3_AB_Low = PerpTS_Low;

	//SRSDG3_C = Total_Cycle_Time-Laser_Warmup+Astrella_Synch+Arc+Bias1+ParaTS-ParaTS_Offset;
	//SRSDG3_D = Total_Cycle_Time-Laser_Warmup+Astrella_Synch+Arc+Bias1+ParaTS-ParaTS_Offset+ParaTS_Pulse_Length;
	SRSDG3_C = Arc+Bias1+ParaTS-ParaTS_Offset;
	SRSDG3_D = Arc+Bias1+ParaTS-ParaTS_Offset+ParaTS_Pulse_Length;
	SRSDG3_CD_High = ParaTS_High;
	SRSDG3_CD_Low = ParaTS_Low;

	//SRSDG3_E = Total_Cycle_Time-Laser_Warmup+Astrella_Synch+Arc+Bias1+PLIF-PLIF_Offset;
	//SRSDG3_F = Total_Cycle_Time-Laser_Warmup+Astrella_Synch+Arc+Bias1+PLIF-PLIF_Offset+PLIF_Pulse_Length;
	SRSDG3_E = Arc+Bias1+PLIF-PLIF_Offset;
	SRSDG3_F = Arc+Bias1+PLIF-PLIF_Offset+PLIF_Pulse_Length;
	SRSDG3_EF_High = PLIF_High;
	SRSDG3_EF_Low = PLIF_Low;

	//SRSDG3_G = Total_Cycle_Time-Laser_Warmup+Astrella_Synch+Arc+Bias1+Other-Other_Offset;
	//SRSDG3_H = Total_Cycle_Time-Laser_Warmup+Astrella_Synch+Arc+Bias1+Other-Other_Offset+Other_Pulse_Length;
	SRSDG3_G = Arc+Bias1+Other-Other_Offset;
	SRSDG3_H = Arc+Bias1+Other-Other_Offset+Other_Pulse_Length;
	SRSDG3_GH_High = Other_High;
	SRSDG3_GH_Low = Other_Low;


	/***********************************************************************************************/
	//Open Visa session to first Trigger unit
	result=viOpen (Global_Visa_Session_Handle, Trigger1_Port_string, VI_NULL, VI_NULL, &Trigger1_handle);
	
	if (!result) {
		//Send commands to set up each of the timing channels
		sprintf (Trigger_string, "DLAY 2,0,%f\r",SRSDG1_A);   							//Delay A relative to TO
		result = viWrite (Trigger1_handle, Trigger_string, (unsigned int)strlen(Trigger_string), &count);

		sprintf (Trigger_string, "DLAY 3,0,%f\r",SRSDG1_B);   							//Delay B relative to TO
		result = viWrite (Trigger1_handle, Trigger_string, (unsigned int)strlen(Trigger_string), &count);

		sprintf (Trigger_string, "LOFF 1,%f\r",Laser_Warmup_Low);   					//Set offset voltage of AB
		result = viWrite (Trigger1_handle, Trigger_string, (unsigned int)strlen(Trigger_string), &count);

		if (Laser_Warmup_Low > Laser_Warmup_High)	{									//Check polarity of timing pulse, set negative if needed
			polarity = 0;
			sprintf (Trigger_string, "LOFF 1,%f\r",Laser_Warmup_High);   					//Set offset voltage of AB
			result = viWrite (Trigger1_handle, Trigger_string, (unsigned int)strlen(Trigger_string), &count);
		} else {
			polarity = 1;
			sprintf (Trigger_string, "LOFF 1,%f\r",Laser_Warmup_Low);   					//Set offset voltage of AB
			result = viWrite (Trigger1_handle, Trigger_string, (unsigned int)strlen(Trigger_string), &count);
		}
		sprintf (Trigger_string, "LPOL 1,%d\r",polarity);   							//Set polarity of AB
		result = viWrite (Trigger1_handle, Trigger_string, (unsigned int)strlen(Trigger_string), &count);

		sprintf (Trigger_string, "LAMP 1,%f\r",fabs(Laser_Warmup_High-Laser_Warmup_Low));   //Set amplitude of AB
		result = viWrite (Trigger1_handle, Trigger_string, (unsigned int)strlen(Trigger_string), &count);
		
		
		sprintf (Trigger_string, "DLAY 4,0,%f\r",SRSDG1_C);   							//Delay C relative to TO
		result = viWrite (Trigger1_handle, Trigger_string, (unsigned int)strlen(Trigger_string), &count);

		sprintf (Trigger_string, "DLAY 5,0,%f\r",SRSDG1_D);   							//Delay D relative to TO
		result = viWrite (Trigger1_handle, Trigger_string, (unsigned int)strlen(Trigger_string), &count);

		if (Gun_Charging_Low > Gun_Charging_High)	{									//Check polarity of timing pulse, set negative if needed
			polarity = 0;
			sprintf (Trigger_string, "LOFF 2,%f\r",Gun_Charging_High);   						//Set offset voltage of CD
			result = viWrite (Trigger1_handle, Trigger_string, (unsigned int)strlen(Trigger_string), &count);
		} else {
			polarity = 1;
			sprintf (Trigger_string, "LOFF 2,%f\r",Gun_Charging_Low);   						//Set offset voltage of CD
			result = viWrite (Trigger1_handle, Trigger_string, (unsigned int)strlen(Trigger_string), &count);
		}
		
		sprintf (Trigger_string, "LPOL 2,%d\r",polarity);   							//Set polarity of CD
		result = viWrite (Trigger1_handle, Trigger_string, (unsigned int)strlen(Trigger_string), &count);

		sprintf (Trigger_string, "LAMP 2,%f\r",fabs(Gun_Charging_High-Gun_Charging_Low));   //Set amplitude of CD
		result = viWrite (Trigger1_handle, Trigger_string, (unsigned int)strlen(Trigger_string), &count);

		
		sprintf (Trigger_string, "DLAY 6,0,%f\r",SRSDG1_E);   							//Delay E relative to TO
		result = viWrite (Trigger1_handle, Trigger_string, (unsigned int)strlen(Trigger_string), &count);

		sprintf (Trigger_string, "DLAY 7,0,%f\r",SRSDG1_F);   							//Delay F relative to TO
		result = viWrite (Trigger1_handle, Trigger_string, (unsigned int)strlen(Trigger_string), &count);

		if (Astrella_Synch_Low > Astrella_Synch_High)	{								//Check polarity of timing pulse, set negative if needed
			polarity = 0;
			sprintf (Trigger_string, "LOFF 3,%f\r",Astrella_Synch_Low);   					//Set offset voltage of EF
			result = viWrite (Trigger1_handle, Trigger_string, (unsigned int)strlen(Trigger_string), &count);
		} else {
			polarity = 1;
			sprintf (Trigger_string, "LOFF 3,%f\r",Astrella_Synch_Low);   					//Set offset voltage of EF
			result = viWrite (Trigger1_handle, Trigger_string, (unsigned int)strlen(Trigger_string), &count);
		}
		sprintf (Trigger_string, "LPOL 3,%d\r",polarity);   							//Set polarity of EF
		result = viWrite (Trigger1_handle, Trigger_string, (unsigned int)strlen(Trigger_string), &count);

		sprintf (Trigger_string, "LAMP 3,%f\r",fabs(Astrella_Synch_High-Astrella_Synch_Low));   //Set amplitude voltage of EF
		result = viWrite (Trigger1_handle, Trigger_string, (unsigned int)strlen(Trigger_string), &count);
		
		sprintf (Trigger_string, "DLAY 8,0,%f\r",SRSDG1_G);   //Delay G relative to TO
		result = viWrite (Trigger1_handle, Trigger_string, (unsigned int)strlen(Trigger_string), &count);

		sprintf (Trigger_string, "DLAY 9,0,%f\r",SRSDG1_H);   //Delay H relative to TO
		result = viWrite (Trigger1_handle, Trigger_string, (unsigned int)strlen(Trigger_string), &count);

		if (iCCD_Shutter_Low > iCCD_Shutter_High)	{								//Check polarity of timing pulse, set negative if needed
			polarity = 0;
			sprintf (Trigger_string, "LOFF 4,%f\r",iCCD_Shutter_High);   					//Set offset voltage of GH
			result = viWrite (Trigger1_handle, Trigger_string, (unsigned int)strlen(Trigger_string), &count);
		} else {
			polarity = 1;
			sprintf (Trigger_string, "LOFF 4,%f\r",iCCD_Shutter_Low);   					//Set offset voltage of GH
			result = viWrite (Trigger1_handle, Trigger_string, (unsigned int)strlen(Trigger_string), &count);
		}
		sprintf (Trigger_string, "LPOL 4,%d\r",polarity);   							//Set polarity of GH
		result = viWrite (Trigger1_handle, Trigger_string, (unsigned int)strlen(Trigger_string), &count);

		sprintf (Trigger_string, "LAMP 4,%f\r",fabs(iCCD_Shutter_High-iCCD_Shutter_Low));   //Set amplitude voltage of GH
		result = viWrite (Trigger1_handle, Trigger_string, (unsigned int)strlen(Trigger_string), &count);
		
		sprintf (Trigger_string, "TSRC 3\r");   //Set up triggering to be rising edge of externally supplied trigger
		result = viWrite (Trigger1_handle, Trigger_string, (unsigned int)strlen(Trigger_string), &count);
		
		viClose(Trigger1_handle);
	} //End check to see if device session opened

	/***********************************************************************************************/
	//Open Visa session to second Trigger unit
	result=viOpen (Global_Visa_Session_Handle, Trigger2_Port_string, VI_NULL, VI_NULL, &Trigger2_handle);
				
	if (!result) {
		//Send commands to set up each of the timing channels
		sprintf (Trigger_string, "DLAY 2,0,%f\r",SRSDG2_A);   							//Delay A relative to TO
		result = viWrite (Trigger2_handle, Trigger_string, (unsigned int)strlen(Trigger_string), &count);

		sprintf (Trigger_string, "DLAY 3,0,%f\r",SRSDG2_B);   							//Delay B relative to TO
		result = viWrite (Trigger2_handle, Trigger_string, (unsigned int)strlen(Trigger_string), &count);

		if (Gas_Puff_Low > Gas_Puff_High)	{											//Check polarity of timing pulse, set negative if needed
			polarity = 0;
			sprintf (Trigger_string, "LOFF 1,%f\r",Gas_Puff_High);   					//Set offset voltage of AB
			result = viWrite (Trigger2_handle, Trigger_string, (unsigned int)strlen(Trigger_string), &count);	
		} else {
			polarity = 1;
			sprintf (Trigger_string, "LOFF 1,%f\r",Gas_Puff_Low);   					//Set offset voltage of AB
			result = viWrite (Trigger2_handle, Trigger_string, (unsigned int)strlen(Trigger_string), &count);	
		}
		
		sprintf (Trigger_string, "LPOL 1,%d\r",polarity);   							//Set polarity of AB
		result = viWrite (Trigger2_handle, Trigger_string, (unsigned int)strlen(Trigger_string), &count);

		sprintf (Trigger_string, "LAMP 1,%f\r",fabs(Gas_Puff_High-Gas_Puff_Low));   //Set amplitude voltage of AB
		result = viWrite (Trigger2_handle, Trigger_string, (unsigned int)strlen(Trigger_string), &count);
		
		sprintf (Trigger_string, "DLAY 4,0,%f\r",SRSDG2_C);   							//Delay C relative to TO
		result = viWrite (Trigger2_handle, Trigger_string, (unsigned int)strlen(Trigger_string), &count);

		sprintf (Trigger_string, "DLAY 5,0,%f\r",SRSDG2_D);   							//Delay D relative to TO
		result = viWrite (Trigger2_handle, Trigger_string, (unsigned int)strlen(Trigger_string), &count);

		if (Arc_Low > Arc_High)	{														//Check polarity of timing pulse, set negative if needed
			polarity = 0;
			sprintf (Trigger_string, "LOFF 2,%f\r",Arc_High);   								//Set offset voltage of CD
			result = viWrite (Trigger2_handle, Trigger_string, (unsigned int)strlen(Trigger_string), &count);
		} else {
			polarity = 1;
			sprintf (Trigger_string, "LOFF 2,%f\r",Arc_Low);   								//Set offset voltage of CD
			result = viWrite (Trigger2_handle, Trigger_string, (unsigned int)strlen(Trigger_string), &count);
		}
		sprintf (Trigger_string, "LPOL 2,%d\r",polarity);   							//Set polarity of CD
		result = viWrite (Trigger2_handle, Trigger_string, (unsigned int)strlen(Trigger_string), &count);

		sprintf (Trigger_string, "LAMP 2,%f\r",fabs(Arc_High-Arc_Low));   				//Set amplitude voltage of CD
		result = viWrite (Trigger2_handle, Trigger_string, (unsigned int)strlen(Trigger_string), &count);

		sprintf (Trigger_string, "DLAY 6,0,%f\r",SRSDG2_E);   							//Delay E relative to TO
		result = viWrite (Trigger2_handle, Trigger_string, (unsigned int)strlen(Trigger_string), &count);

		sprintf (Trigger_string, "DLAY 7,0,%f\r",SRSDG2_F);   							//Delay F relative to TO
		result = viWrite (Trigger2_handle, Trigger_string, (unsigned int)strlen(Trigger_string), &count);

		if (Bias1_Low > Bias1_High)	{													//Check polarity of timing pulse, set negative if needed
			polarity = 0;
			sprintf (Trigger_string, "LOFF 3,%f\r",Bias1_High);   							//Set offset voltage of EF
			result = viWrite (Trigger2_handle, Trigger_string, (unsigned int)strlen(Trigger_string), &count);
		} else {
			polarity = 1;
			sprintf (Trigger_string, "LOFF 3,%f\r",Bias1_Low);   							//Set offset voltage of EF
			result = viWrite (Trigger2_handle, Trigger_string, (unsigned int)strlen(Trigger_string), &count);
		}
		sprintf (Trigger_string, "LPOL 3,%d\r",polarity);   							//Set polarity of EF
		result = viWrite (Trigger2_handle, Trigger_string, (unsigned int)strlen(Trigger_string), &count);

		sprintf (Trigger_string, "LAMP 3,%f\r",fabs(Bias1_High-Bias1_Low));   				//Set amplitude voltage of EF
		result = viWrite (Trigger2_handle, Trigger_string, (unsigned int)strlen(Trigger_string), &count);
				
		sprintf (Trigger_string, "DLAY 8,0,%f\r",SRSDG2_G);   							//Delay G relative to TO
		result = viWrite (Trigger2_handle, Trigger_string, (unsigned int)strlen(Trigger_string), &count);

		sprintf (Trigger_string, "DLAY 9,0,%f\r",SRSDG2_H);   							//Delay H relative to TO
		result = viWrite (Trigger2_handle, Trigger_string, (unsigned int)strlen(Trigger_string), &count);

		if (Bias2_Low > Bias2_High)	{											//Check polarity of timing pulse, set negative if needed
			polarity = 0;
			sprintf (Trigger_string, "LOFF 4,%f\r",Bias2_High);   							//Set offset voltage of GH
			result = viWrite (Trigger2_handle, Trigger_string, (unsigned int)strlen(Trigger_string), &count);
		} else {
			polarity = 1;
			sprintf (Trigger_string, "LOFF 4,%f\r",Bias2_Low);   							//Set offset voltage of GH
			result = viWrite (Trigger2_handle, Trigger_string, (unsigned int)strlen(Trigger_string), &count);
		}
		sprintf (Trigger_string, "LPOL 4,%d\r",polarity);   							//Set polarity of GH
		result = viWrite (Trigger2_handle, Trigger_string, (unsigned int)strlen(Trigger_string), &count);

		sprintf (Trigger_string, "LAMP 4,%f\r",fabs(Bias2_High-Bias2_Low));   				//Set amplitude voltage of GH
		result = viWrite (Trigger2_handle, Trigger_string, (unsigned int)strlen(Trigger_string), &count);

		sprintf (Trigger_string, "TSRC 3\r");   //Set up triggering to be rising edge of externally supplied trigger
		result = viWrite (Trigger2_handle, Trigger_string, (unsigned int)strlen(Trigger_string), &count);

		viClose(Trigger2_handle);
	} //End check to see if device session opened

	
	/***********************************************************************************************/
	//Open Visa session to third Trigger unit
	result=viOpen (Global_Visa_Session_Handle, Trigger3_Port_string, VI_NULL, VI_NULL, &Trigger3_handle);
	
	if (!result) {
		//Send commands to set up each of the timing channels
		sprintf (Trigger_string, "DLAY 2,0,%f\r",SRSDG3_A);   							//Delay A relative to TO
		result = viWrite (Trigger3_handle, Trigger_string, (unsigned int)strlen(Trigger_string), &count);

		sprintf (Trigger_string, "DLAY 3,0,%f\r",SRSDG3_B);   							//Delay B relative to TO
		result = viWrite (Trigger3_handle, Trigger_string, (unsigned int)strlen(Trigger_string), &count);

		if (PerpTS_Low > PerpTS_High)	{											//Check polarity of timing pulse, set negative if needed
			polarity = 0;
			sprintf (Trigger_string, "LOFF 1,%f\r",PerpTS_High);   						//Set offset voltage of AB
			result = viWrite (Trigger3_handle, Trigger_string, (unsigned int)strlen(Trigger_string), &count);
		} else {
			polarity = 1;
			sprintf (Trigger_string, "LOFF 1,%f\r",PerpTS_Low);   						//Set offset voltage of AB
			result = viWrite (Trigger3_handle, Trigger_string, (unsigned int)strlen(Trigger_string), &count);
		}
		sprintf (Trigger_string, "LPOL 1,%d\r",polarity);   							//Set polarity of AB
		result = viWrite (Trigger3_handle, Trigger_string, (unsigned int)strlen(Trigger_string), &count);

		sprintf (Trigger_string, "LAMP 1,%f\r",fabs(PerpTS_High-PerpTS_Low));   //Set amplitude voltage of AB
		result = viWrite (Trigger3_handle, Trigger_string, (unsigned int)strlen(Trigger_string), &count);
		
		sprintf (Trigger_string, "DLAY 4,0,%f\r",SRSDG3_C);   							//Delay C relative to TO
		result = viWrite (Trigger3_handle, Trigger_string, (unsigned int)strlen(Trigger_string), &count);

		sprintf (Trigger_string, "DLAY 5,0,%f\r",SRSDG3_D);   							//Delay D relative to TO
		result = viWrite (Trigger3_handle, Trigger_string, (unsigned int)strlen(Trigger_string), &count);

		if (ParaTS_Low > ParaTS_High)	{												//Check polarity of timing pulse, set negative if needed
			polarity = 0;
			sprintf (Trigger_string, "LOFF 2,%f\r",ParaTS_High);   							//Set offset voltage of CD
			result = viWrite (Trigger3_handle, Trigger_string, (unsigned int)strlen(Trigger_string), &count);
		} else {
			polarity = 1;
			sprintf (Trigger_string, "LOFF 2,%f\r",ParaTS_Low);   							//Set offset voltage of CD
			result = viWrite (Trigger3_handle, Trigger_string, (unsigned int)strlen(Trigger_string), &count);
		}
		sprintf (Trigger_string, "LPOL 2,%d\r",polarity);   							//Set polarity of CD
		result = viWrite (Trigger3_handle, Trigger_string, (unsigned int)strlen(Trigger_string), &count);

		sprintf (Trigger_string, "LAMP 2,%f\r",fabs(ParaTS_High-ParaTS_Low));   			//Set amplitude voltage of CD
		result = viWrite (Trigger3_handle, Trigger_string, (unsigned int)strlen(Trigger_string), &count);
	
		sprintf (Trigger_string, "DLAY 6,0,%f\r",SRSDG3_E);   							//Delay E relative to TO
		result = viWrite (Trigger3_handle, Trigger_string, (unsigned int)strlen(Trigger_string), &count);

		sprintf (Trigger_string, "DLAY 7,0,%f\r",SRSDG3_F);   							//Delay F relative to TO
		result = viWrite (Trigger3_handle, Trigger_string, (unsigned int)strlen(Trigger_string), &count);

		if (PLIF_Low > PLIF_High)	{													//Check polarity of timing pulse, set negative if needed
			polarity = 0;
			sprintf (Trigger_string, "LOFF 3,%f\r",PLIF_High);   								//Set offset voltage of EF
			result = viWrite (Trigger3_handle, Trigger_string, (unsigned int)strlen(Trigger_string), &count);
		} else {
			polarity = 1;
			sprintf (Trigger_string, "LOFF 3,%f\r",PLIF_Low);   								//Set offset voltage of EF
			result = viWrite (Trigger3_handle, Trigger_string, (unsigned int)strlen(Trigger_string), &count);
		}
		sprintf (Trigger_string, "LPOL 3,%d\r",polarity);   							//Set polarity of EF
		result = viWrite (Trigger3_handle, Trigger_string, (unsigned int)strlen(Trigger_string), &count);

		sprintf (Trigger_string, "LAMP 3,%f\r",fabs(PLIF_High-PLIF_Low));   				//Set amplitude voltage of EF
		result = viWrite (Trigger3_handle, Trigger_string, (unsigned int)strlen(Trigger_string), &count);
		
			
		sprintf (Trigger_string, "DLAY 8,0,%f\r",SRSDG3_G);   							//Delay G relative to TO
		result = viWrite (Trigger3_handle, Trigger_string, (unsigned int)strlen(Trigger_string), &count);

		sprintf (Trigger_string, "DLAY 9,0,%f\r",SRSDG3_H);   							//Delay H relative to TO
		result = viWrite (Trigger3_handle, Trigger_string, (unsigned int)strlen(Trigger_string), &count);

		if (Other_Low > Other_High)	{													//Check polarity of timing pulse, set negative if needed
			polarity = 0;
			sprintf (Trigger_string, "LOFF 4,%f\r",Other_High);   							//Set offset voltage of GH
			result = viWrite (Trigger3_handle, Trigger_string, (unsigned int)strlen(Trigger_string), &count);
		} else {
			polarity = 1;
			sprintf (Trigger_string, "LOFF 4,%f\r",Other_Low);   							//Set offset voltage of GH
			result = viWrite (Trigger3_handle, Trigger_string, (unsigned int)strlen(Trigger_string), &count);
		}
		sprintf (Trigger_string, "LPOL 4,%d\r",polarity);   							//Set polarity of GH
		result = viWrite (Trigger3_handle, Trigger_string, (unsigned int)strlen(Trigger_string), &count);

		sprintf (Trigger_string, "LAMP 4,%f\r",fabs(Other_High-Other_Low));   				//Set amplitude voltage of GH
		result = viWrite (Trigger3_handle, Trigger_string, (unsigned int)strlen(Trigger_string), &count);

		sprintf (Trigger_string, "TSRC 3\r");   //Set up triggering to be rising edge of externally supplied trigger
		result = viWrite (Trigger3_handle, Trigger_string, (unsigned int)strlen(Trigger_string), &count);
		
		viClose(Trigger3_handle);
	} //End check to see if device session opened
	
	//Discard panel
	DiscardPanel(Trigger_panel); 	
}



//Send the master trigger from the NI DAQ card
void Send_Trigger (void)  
{
	int			result;
	int			count;
	char		Trigger_string[16]="Null";
	char 		Trigger1_Port_string[36];
	ViSession 	Trigger1_handle;
	
	//get NI DAQ card to send out a trigger pulse by starting up the task  
	DAQmxStartTask (OutputCardTrigHandle); 	  
		
	//Open settings panel and load saved values
	//Trigger_panel = LoadPanel (0, "MasterTriggerSettings.uir", Trigger);
	//RecallPanelState (Trigger_panel, "Master_Control_Storage_File", TriggerSettings_setup_state);
	//Open Visa session 
	//result = viOpenDefaultRM (&Global_Visa_Session_Handle);
	//Open Visa session to Trigger unit
	//Get the port address of the first SRSDG645
	//GetCtrlVal (Trigger_panel, Trigger_Trigger_IP_1,Trigger1_Port_string);
	//Open Visa session to first Trigger unit
	//result=viOpen (Global_Visa_Session_Handle, Trigger1_Port_string, VI_NULL, VI_NULL, &Trigger1_handle);
	//Send command to initiate trigger
	//sprintf (Trigger_string, "*TRG\r"); 
	//result = viWrite (Trigger1_handle, Trigger_string, (unsigned int)strlen(Trigger_string), &count);
	//Close ViSession
	//result = viClose (Global_Visa_Session_Handle);
	
	//Delay long enough for trigger
	Delay(0.1);
	
	//shut down trigger task  
	DAQmxStopTask (OutputCardTrigHandle); 
}
