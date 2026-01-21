#include 	<ansi_c.h>
#include	<visa.h>
#include 	<formatio.h>
#include	"PHASMA-MDS-Routines.h"
#include 	"RFEASystems.h"
#include 	"RFEASettings.h"  
#include	"GlobalVariables.h"
#include	"Rigol.h"
#include	"PHASMA_C_to_Python_convertor.h"

ViSession  	sig_cond_hndl;
ViSession 	digitizer_hndl;

int		RFEA_panel;
int		RFEA_setup_state = 11;
int		RFEA_RecordLength=8192;
float	RFEA_VoltageRange=10.0;
float	RFEA_TriggerLevel=1.0;
float	RFEA_TimeWindow=200;

ViSession 	Rigol_handle;


//Set up the Rigol Scope
void RFEAActivate(void)
{
	int 		result; 					//	Error-checking variable
	float		RFEA_VoltageRange=10;
	char		RFEA_string[32]="Null";
	char		IP_Address_string[64];  
	char		RFEA_TriggerSource[8];
	ViUInt32 	count;

	//Open up RFEA settings panel and get target digitizer rate and other settings
	RFEA_panel = LoadPanel (0, "RFEASettings.uir", RFEA);
	RecallPanelState (RFEA_panel, "Master_Control_Storage_File", RFEA_setup_state);
	   
	//Get sample rate, record length, and channel scales
	GetCtrlVal (RFEA_panel, RFEA_SettingsRecordLength,&RFEA_RecordLength);
	GetCtrlVal (RFEA_panel, RFEA_SettingsTimeWindow,&RFEA_TimeWindow);
	GetCtrlVal (RFEA_panel, RFEA_SettingsVoltageRange,&RFEA_VoltageRange);
	GetCtrlVal (RFEA_panel, RFEA_SettingsTriggerSource,RFEA_TriggerSource);
	GetCtrlVal (RFEA_panel, RFEA_SettingsTriggerLevel,&RFEA_TriggerLevel);
		
	//Discard RFEA panel
	DiscardPanel(RFEA_panel);
	
	//Get Rigol ip address from interface panel and open connection to the scope
	GetCtrlVal (RFEA_panel, RFEA_RIGOL_RFEAAddress, IP_Address_string);
 	result = viOpen (Global_Visa_Session_Handle, IP_Address_string, VI_NULL, VI_NULL, &Rigol_handle);

	if (!result) {
		//Make sure scope is running so commands will work
		sprintf (RFEA_string, ":RUN\r");   																							//Send the run command
		result = viWrite (Rigol_handle, RFEA_string, (unsigned int)strlen(RFEA_string), &count);

		//Setup the rate, record length, trigger system, and voltage range for the Rigol scope
	  	sprintf (RFEA_string, ":TIM:MAIN:SCAL %f\r",(RFEA_TimeWindow/10.0));   										//Set up timebase scaling based on target sample window and 10 divisions
		result = viWrite (Rigol_handle, RFEA_string, (unsigned int)strlen(RFEA_string), &count);
		sprintf (RFEA_string, ":ACQ:MDEP %d\r",RFEA_RecordLength);   												//Set the record length by setting the Memory Depth and upload the record length in k
		result = viWrite (Rigol_handle, RFEA_string, (unsigned int)strlen(RFEA_string), &count);
		sprintf (RFEA_string, ":CHAN1:SCAL  %f\r",RFEA_VoltageRange);   											//Set the voltage scale of the channel to the target value
		result = viWrite (Rigol_handle, RFEA_string, (unsigned int)strlen(RFEA_string), &count);
		sprintf (RFEA_string, ":CHAN2:SCAL  %f\r",RFEA_VoltageRange);   											//Set the voltage scale of the channel to the target value
		result = viWrite (Rigol_handle, RFEA_string, (unsigned int)strlen(RFEA_string), &count);
		sprintf (RFEA_string, ":CHAN3:SCAL  %f\r",RFEA_VoltageRange);   											//Set the voltage scale of the channel to the target value
		result = viWrite (Rigol_handle, RFEA_string, (unsigned int)strlen(RFEA_string), &count);
		sprintf (RFEA_string, ":CHAN4:SCAL  %f\r",RFEA_VoltageRange);   											//Set the voltage scale of the channel to the target value
		result = viWrite (Rigol_handle, RFEA_string, (unsigned int)strlen(RFEA_string), &count);
	 	sprintf (RFEA_string, ":TRIG:MODE EDGE\r");   
		result = viWrite (Rigol_handle, RFEA_string, (unsigned int)strlen(RFEA_string), &count);
		sprintf (RFEA_string, ":TRIG:COUP AC\r");   
		result = viWrite (Rigol_handle, RFEA_string, (unsigned int)strlen(RFEA_string), &count);
		sprintf (RFEA_string, ":TRIG:EDGE:SOUR %s\r",RFEA_TriggerSource);  														//Set the trigger source to the target value 
		result = viWrite (Rigol_handle, RFEA_string, (unsigned int)strlen(RFEA_string), &count);
	 	sprintf (RFEA_string, ":TRIG:EDGE:SLOP POS\r");   
		result = viWrite (Rigol_handle, RFEA_string, (unsigned int)strlen(RFEA_string), &count);
		sprintf (RFEA_string, ":TRIG:EDGE:LEV %f\r",RFEA_TriggerLevel);   
		result = viWrite (Rigol_handle, RFEA_string, (unsigned int)strlen(RFEA_string), &count);
		sprintf (RFEA_string, ":SYST:KEY:PRES MOFF\r");   												 	//Hide menu on scope screen
	 	result = viWrite (Rigol_handle, RFEA_string, (unsigned int)strlen(RFEA_string), &count);
	}
}


//Arm the Rigol oscilloscope
int RFEAArm(void)
{
	int		result;
	int		count;
	char	RFEA_string[32]="Null";
	
	//	Arm Digitizer with provision for error and only arms those groups in use
	sprintf (RFEA_string, ":TRIG:SWE SINGl\r");   												 		//Single Trigger Sweep
 	result = viWrite (Rigol_handle, RFEA_string, (unsigned int)strlen(RFEA_string), &count);

	return result;
}




//Take the acquired data from the instrument and print it to the common data folder and 
//also push it to the MDS database
void Write_RFEAData(void)
{
	char	ChannelNameString[36];
	char	outfilename[36];
	FILE*	outfile;
	int		j;
	float 	RFEA_timestep=0.0;
	double	*RFEA_Array1;
	double	*RFEA_Array2;
	double	*RFEA_Array3;
	double	*RFEA_Array4;
	double	*timebase;
		
	//Create temporary data arrays	
	RFEA_Array1 = malloc (RFEA_RecordLength);
	RFEA_Array2 = malloc (RFEA_RecordLength);
	RFEA_Array3 = malloc (RFEA_RecordLength);
	RFEA_Array4 = malloc (RFEA_RecordLength);
	timebase = malloc (RFEA_RecordLength);

	//Grab data from all four channels of the oscilloscope
	Rigol(Rigol_handle, RFEA_RecordLength, &RFEA_timestep, RFEA_Array1, RFEA_Array2, RFEA_Array3, RFEA_Array4);   
	
	//Dump results to a file
	sprintf(outfilename, RawDataPath);
	strcat(outfilename, ShotNumberString);
	strcat(outfilename, "_");
	strcat(outfilename, "RFEA.txt");
	
	outfile=fopen (outfilename, "w");
	fprintf(outfile,"t, RFEA1\n");
	for (j=0;j<RFEA_RecordLength;j++) {
		timebase[j]=(double)(j*RFEA_timestep);
		fprintf(outfile,"%f,%f,%f,%f,%f,%f,%f,%f,%f\n",timebase[j],RFEA_Array1[j]);
	}

 	//Close Pulsed LIF data file
	fclose (outfile);
	
	//Open up RFEA settings panel and get filenames for each measurement
	RFEA_panel = LoadPanel (0, "RFEASystems.uir", RFEA);
	RecallPanelState (RFEA_panel, "Master_Control_Storage_File", RFEA_setup_state);	
	
	//Get channel name string, store raw data on drive folder, and push data to MDSPlus for Channel 1
	GetCtrlVal (RFEA_panel, RFEA_Chan1Name,ChannelNameString);
	Write_to_PHASMA_MDS(timebase,RFEA_Array1,ChannelNameString);
}



//Launch this routine as window opens and it updates the window with the latest values from the 
//control system as it loads.
void OpenRFEA_Settings (void)  
{
	//Open settings panel and load saved values
	RFEA_panel = LoadPanel (0, "RFEASettings.uir", RFEA);
	SavePanelState (RFEA_panel, "Master_Control_Storage_File", RFEA_setup_state);
	DisplayPanel(RFEA_panel);
	
	//Update panel with 

	
	// Start interacting with user
    RunUserInterface ();

}

int CVICALLBACK Close_RFEA_Settings (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			//Save current state of panel and close interface
			RFEA_panel = LoadPanel (0, "RFEASettings.uir", RFEA);
			SavePanelState (RFEA_panel, "Master_Control_Storage_File", RFEA_setup_state);
			DiscardPanel(panel);
			break;
	}
	return 0;
}

int CVICALLBACK RFEA_Move_to_Start (int panel, int control, int event,
							   void *callbackData, int eventData1, int eventData2)
{

	float 	RFEA_RStart;
	float 	Current_Position;
	float  	RFEA_Max_Insertion;
	float 	RFEA_COUNTSPERMM;
	float 	RFEA_Offset;
	char 	RFEA_Probe_IP_Address[64];

	switch (event)
	{
		case EVENT_COMMIT:
			//Get current position of probe, starting position, end position, and what to do at end of scan range
			GetCtrlVal (RFEA, RFEA_R_Start,&RFEA_RStart);
			GetCtrlVal (RFEA, RFEA_Probe_IP,RFEA_Probe_IP_Address);
			GetCtrlVal (RFEA, RFEA_Probe_Max_Insertion,&RFEA_Max_Insertion);
			GetCtrlVal (RFEA, RFEA_Probe_COUNTSPERMM,&RFEA_COUNTSPERMM);
			GetCtrlVal (RFEA, RFEA_Probe_Offset,&RFEA_Offset);
			
			//Send move command to probe motor
			move_probe(RFEA_Probe_IP_Address, (RFEA_Offset-RFEA_RStart)*10.0, RFEA_COUNTSPERMM, RFEA_Max_Insertion);
			
			//Set current position to Start
			SetCtrlVal (RFEA, RFEA_R_Position,RFEA_RStart);


			break;
	}
	return 0;
}


int CVICALLBACK RFEA_Move_to_Stop (int panel, int control, int event,
							   void *callbackData, int eventData1, int eventData2)
{
	float 	RFEA_RStop;
	float 	Current_Position;
	float  	RFEA_Max_Insertion;
	float 	RFEA_COUNTSPERMM;
	float 	RFEA_Offset;
	char 	RFEA_Probe_IP_Address[64];

	switch (event)
	{
		case EVENT_COMMIT:
			//Get current position of probe, starting position, end position, and what to do at end of scan range
			GetCtrlVal (RFEA, RFEA_R_Stop,&RFEA_RStop);
			GetCtrlVal (RFEA, RFEA_Probe_IP,RFEA_Probe_IP_Address);
			GetCtrlVal (RFEA, RFEA_Probe_Max_Insertion,&RFEA_Max_Insertion);
			GetCtrlVal (RFEA, RFEA_Probe_COUNTSPERMM,&RFEA_COUNTSPERMM);
			GetCtrlVal (RFEA, RFEA_Probe_Offset,&RFEA_Offset);
			
			//Send move command to probe motor
			move_probe(RFEA_Probe_IP_Address, (RFEA_Offset-RFEA_RStop)*10.0, RFEA_COUNTSPERMM, RFEA_Max_Insertion);
			
			//Set current position to Start
			SetCtrlVal (RFEA, RFEA_R_Position,RFEA_RStop);


			break;
	}
	return 0;
}

int CVICALLBACK RFEA_Home (int panel, int control, int event,
							   void *callbackData, int eventData1, int eventData2)
{

	char 	RFEA_Probe_IP_Address[64];
	float 	RFEA_Offset;

	switch (event)
	{
		case EVENT_COMMIT:
			//Get current position of probe, starting position, end position, and what to do at end of scan range
			GetCtrlVal (RFEA, RFEA_Probe_IP,RFEA_Probe_IP_Address);
			GetCtrlVal (RFEA, RFEA_Probe_Offset,&RFEA_Offset);
			
			//Send move command to probe motor
			home_probe(RFEA_Probe_IP_Address);
			
			//Set current position to Offset
			SetCtrlVal (RFEA, RFEA_R_Position,RFEA_Offset);


			break;
	}
	return 0;
}

float Advance_RFEA_Probe(void)
{
	int	 	Probe_Return_Flag;
	int 	Probe_Active_Move_Flag;
	float 	RFEA_RStart;
	float 	RFEA_RStop;
	float 	RFEA_RIncrement;
	float 	RFEA_Offset;
	float 	RFEA_Return;
	float 	Current_Position;
	float 	Probe_Move;
	float  	RFEA_Max_Insertion;
	float 	RFEA_COUNTSPERMM;
	char 	RFEA_Probe_IP_Address[64];
	
	//Open settings panel and load saved values
	RFEA_panel = LoadPanel (0, "RFEASettings.uir", RFEA);
	RecallPanelState (RFEA_panel, "Master_Control_Storage_File", RFEA_setup_state);
	
	//Get current position of probe, starting position, end position, and what to do at end of scan range
	GetCtrlVal (RFEA_panel, RFEA_R_Start,&RFEA_RStart);
	GetCtrlVal (RFEA_panel, RFEA_R_Stop,&RFEA_RStop);
	GetCtrlVal (RFEA_panel, RFEA_R_Increment,&RFEA_RIncrement);
	GetCtrlVal (RFEA_panel, RFEA_Probe_Offset,&RFEA_Offset);
	GetCtrlVal (RFEA_panel, RFEA_Probe_Return,&RFEA_Return);
	GetCtrlVal (RFEA_panel, RFEA_R_Position,&Current_Position);
	GetCtrlVal (RFEA_panel, RFEA_Probe_Return,&Probe_Return_Flag);
	GetCtrlVal (RFEA_panel, RFEA_Probe_Active_Move,&Probe_Active_Move_Flag);
	GetCtrlVal (RFEA_panel, RFEA_Probe_IP,RFEA_Probe_IP_Address);
	GetCtrlVal (RFEA_panel, RFEA_Probe_Max_Insertion,&RFEA_Max_Insertion);
	GetCtrlVal (RFEA_panel, RFEA_Probe_COUNTSPERMM,&RFEA_COUNTSPERMM);

	if (Probe_Active_Move_Flag) {
		
		//Get current position from motor controller
	
		//Calculate distance to move to next position
		Probe_Move=Current_Position+RFEA_RIncrement;
		
		//If next position is beyond the stop position, either do nothing or send 
		//probe back to start position depending on settings in interface window
		if ( (RFEA_RIncrement + Current_Position) > RFEA_RStop) {
			return Current_Position;
		} else {
			//Command motor to move to next position	
			move_probe(RFEA_Probe_IP_Address, (RFEA_Offset-Probe_Move)*10.0, RFEA_COUNTSPERMM, RFEA_Max_Insertion);
		}

		
		//Update window with current position and return current position to master code
		SetCtrlVal (RFEA_panel, RFEA_R_Position,(RFEA_RIncrement + Current_Position));
		return RFEA_RIncrement + Current_Position;
	} else {
		return Current_Position;
	}

	SavePanelState (RFEA_panel, "Master_Control_Storage_File", RFEA_setup_state);

}

