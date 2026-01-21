#include 	<ansi_c.h>
#include	<visa.h>
#include	<stdlib.h>
#include 	<formatio.h>
#include	"PHASMA-MDS-Routines.h"
#include 	"DoubleProbeSystems.h"
#include 	"DoubleProbeSettings.h"  
#include	"GlobalVariables.h"
#include	"Rigol.h"
#include	"PHASMA_C_to_Python_convertor.h"

int			DoubleProbe_panel;
int			DoubleProbe_setup_state = 30;
int			Double_Probe_RecordLength=8192;
float		Double_Probe_VoltageRange=10.0;
float		Double_Probe_TriggerLevel=1.0;
float		Double_Probe_TimeWindow=200;

ViSession 	DoubleProbeRigol_handle1;
ViSession 	DoubleProbeRigol_handle2;


//Set up the Rigol scope
void DoubleProbeActivate(void)
{
	int 		result; 					//	Error-checking variable
	float		Double_Probe_VoltageRange=10;
	char		DoubleProbe_string[32]="Null";
	char		IP_Address_string[64];  
	char		Double_Probe_TriggerSource[8];
	ViUInt32 	count;

	//Open up Double Probe settings panel and get target digitizer rate and other settings
	DoubleProbe_panel = LoadPanel (0, "DoubleProbeSettings.uir", DoublePrbe);
	RecallPanelState (DoubleProbe_panel, "Master_Control_Storage_File", DoubleProbe_setup_state);
	   
	//Get Rigol ip address from interface panel and open connection to the scope
	GetCtrlVal (DoubleProbe_panel, DoublePrbe_DBPRB_RIGOLAddress1, IP_Address_string);
 
	//Get sample rate, record length, and channel scales
	GetCtrlVal (DoubleProbe_panel, DoublePrbe_DP_RecordLength,&Double_Probe_RecordLength);
	GetCtrlVal (DoubleProbe_panel, DoublePrbe_DP_TimeWindow,&Double_Probe_TimeWindow);
	GetCtrlVal (DoubleProbe_panel, DoublePrbe_DP_VoltageRange,&Double_Probe_VoltageRange);
	GetCtrlVal (DoubleProbe_panel, DoublePrbe_DP_TriggerSource,Double_Probe_TriggerSource);
	GetCtrlVal (DoubleProbe_panel, DoublePrbe_DP_TriggerLevel,&Double_Probe_TriggerLevel);
		
	//Discard housekeeping panel
	DiscardPanel(DoubleProbe_panel);
	
	//Open session to 1st scope
	result=	viOpen (Global_Visa_Session_Handle, IP_Address_string, VI_NULL, VI_NULL, &DoubleProbeRigol_handle1);

	if (!result) {
		//Make sure scope is running so commands will work
		sprintf (DoubleProbe_string, ":RUN\r");   																							//Send the run command
		result = viWrite (DoubleProbeRigol_handle1, DoubleProbe_string, (unsigned int)strlen(DoubleProbe_string), &count);

		//Setup the rate, record length, trigger system, and voltage range for the Rigol scope
	  	sprintf (DoubleProbe_string, ":TIM:MAIN:SCAL %f\r",(Double_Probe_TimeWindow/10.0));   									//Set up timebase scaling based on target sample window and 10 divisions
		result = viWrite (DoubleProbeRigol_handle1, DoubleProbe_string, (unsigned int)strlen(DoubleProbe_string), &count);
		sprintf (DoubleProbe_string, ":ACQ:MDEP %d\r",Double_Probe_RecordLength);   											//Set the record length by setting the Memory Depth and upload the record length in k
		result = viWrite (DoubleProbeRigol_handle1, DoubleProbe_string, (unsigned int)strlen(DoubleProbe_string), &count);
		sprintf (DoubleProbe_string, ":CHAN1:SCAL  %f\r",Double_Probe_VoltageRange);   											//Set the voltage scale of the channel to the target value
		result = viWrite (DoubleProbeRigol_handle1, DoubleProbe_string, (unsigned int)strlen(DoubleProbe_string), &count);
		sprintf (DoubleProbe_string, ":CHAN2:SCAL  %f\r",Double_Probe_VoltageRange);   											//Set the voltage scale of the channel to the target value
		result = viWrite (DoubleProbeRigol_handle1, DoubleProbe_string, (unsigned int)strlen(DoubleProbe_string), &count);
		sprintf (DoubleProbe_string, ":CHAN3:SCAL  %f\r",Double_Probe_VoltageRange);   											//Set the voltage scale of the channel to the target value
		result = viWrite (DoubleProbeRigol_handle1, DoubleProbe_string, (unsigned int)strlen(DoubleProbe_string), &count);
		sprintf (DoubleProbe_string, ":CHAN4:SCAL  %f\r",Double_Probe_VoltageRange);   											//Set the voltage scale of the channel to the target value
		result = viWrite (DoubleProbeRigol_handle1, DoubleProbe_string, (unsigned int)strlen(DoubleProbe_string), &count);
	 	sprintf (DoubleProbe_string, ":TRIG:MODE EDGE\r");   
		result = viWrite (DoubleProbeRigol_handle1, DoubleProbe_string, (unsigned int)strlen(DoubleProbe_string), &count);
		sprintf (DoubleProbe_string, ":TRIG:COUP AC\r");   
		result = viWrite (DoubleProbeRigol_handle1, DoubleProbe_string, (unsigned int)strlen(DoubleProbe_string), &count);
		sprintf (DoubleProbe_string, ":TRIG:EDGE:SOUR %s\r",Double_Probe_TriggerSource);  														//Set the trigger source to the target value 
		result = viWrite (DoubleProbeRigol_handle1, DoubleProbe_string, (unsigned int)strlen(DoubleProbe_string), &count);
	 	sprintf (DoubleProbe_string, ":TRIG:EDGE:SLOP POS\r");   
		result = viWrite (DoubleProbeRigol_handle1, DoubleProbe_string, (unsigned int)strlen(DoubleProbe_string), &count);
		sprintf (DoubleProbe_string, ":TRIG:EDGE:LEV %f\r",Double_Probe_TriggerLevel);   
		result = viWrite (DoubleProbeRigol_handle1, DoubleProbe_string, (unsigned int)strlen(DoubleProbe_string), &count);
		sprintf (DoubleProbe_string, ":SYST:KEY:PRES MOFF\r");   												 	//Hide menu on scope screen
	 	result = viWrite (DoubleProbeRigol_handle1, DoubleProbe_string, (unsigned int)strlen(DoubleProbe_string), &count);
	}
	
	//Get 2nd Rigol ip address from interface panel and open connection to the scope
	GetCtrlVal (DoubleProbe_panel, DoublePrbe_DBPRB_RIGOLAddress2, IP_Address_string);

	//Open session to 2nd scope
	result=	viOpen (Global_Visa_Session_Handle, IP_Address_string, VI_NULL, VI_NULL, &DoubleProbeRigol_handle2);

	if (!result) {
		//Make sure scope is running so commands will work
		sprintf (DoubleProbe_string, ":RUN\r");   																							//Send the run command
		result = viWrite (DoubleProbeRigol_handle2, DoubleProbe_string, (unsigned int)strlen(DoubleProbe_string), &count);

		//Setup the rate, record length, trigger system, and voltage range for the Rigol scope
	  	sprintf (DoubleProbe_string, ":TIM:MAIN:SCAL %f\r",(Double_Probe_TimeWindow/10.0));   									//Set up timebase scaling based on target sample window and 10 divisions
		result = viWrite (DoubleProbeRigol_handle2, DoubleProbe_string, (unsigned int)strlen(DoubleProbe_string), &count);
		sprintf (DoubleProbe_string, ":ACQ:MDEP %d\r",Double_Probe_RecordLength);   											//Set the record length by setting the Memory Depth and upload the record length in k
		result = viWrite (DoubleProbeRigol_handle2, DoubleProbe_string, (unsigned int)strlen(DoubleProbe_string), &count);
		sprintf (DoubleProbe_string, ":CHAN1:SCAL  %f\r",Double_Probe_VoltageRange);   											//Set the voltage scale of the channel to the target value
		result = viWrite (DoubleProbeRigol_handle2, DoubleProbe_string, (unsigned int)strlen(DoubleProbe_string), &count);
		sprintf (DoubleProbe_string, ":CHAN2:SCAL  %f\r",Double_Probe_VoltageRange);   											//Set the voltage scale of the channel to the target value
		result = viWrite (DoubleProbeRigol_handle2, DoubleProbe_string, (unsigned int)strlen(DoubleProbe_string), &count);
		sprintf (DoubleProbe_string, ":CHAN3:SCAL  %f\r",Double_Probe_VoltageRange);   											//Set the voltage scale of the channel to the target value
		result = viWrite (DoubleProbeRigol_handle2, DoubleProbe_string, (unsigned int)strlen(DoubleProbe_string), &count);
		sprintf (DoubleProbe_string, ":CHAN4:SCAL  %f\r",Double_Probe_VoltageRange);   											//Set the voltage scale of the channel to the target value
		result = viWrite (DoubleProbeRigol_handle2, DoubleProbe_string, (unsigned int)strlen(DoubleProbe_string), &count);
	 	sprintf (DoubleProbe_string, ":TRIG:MODE EDGE\r");   
		result = viWrite (DoubleProbeRigol_handle2, DoubleProbe_string, (unsigned int)strlen(DoubleProbe_string), &count);
		sprintf (DoubleProbe_string, ":TRIG:COUP AC\r");   
		result = viWrite (DoubleProbeRigol_handle2, DoubleProbe_string, (unsigned int)strlen(DoubleProbe_string), &count);
		sprintf (DoubleProbe_string, ":TRIG:EDGE:SOUR %s\r",Double_Probe_TriggerSource);  														//Set the trigger source to the target value 
		result = viWrite (DoubleProbeRigol_handle2, DoubleProbe_string, (unsigned int)strlen(DoubleProbe_string), &count);
	 	sprintf (DoubleProbe_string, ":TRIG:EDGE:SLOP POS\r");   
		result = viWrite (DoubleProbeRigol_handle2, DoubleProbe_string, (unsigned int)strlen(DoubleProbe_string), &count);
		sprintf (DoubleProbe_string, ":TRIG:EDGE:LEV %f\r",Double_Probe_TriggerLevel);   
		result = viWrite (DoubleProbeRigol_handle2, DoubleProbe_string, (unsigned int)strlen(DoubleProbe_string), &count);
		sprintf (DoubleProbe_string, ":SYST:KEY:PRES MOFF\r");   												 	//Hide menu on scope screen
	 	result = viWrite (DoubleProbeRigol_handle2, DoubleProbe_string, (unsigned int)strlen(DoubleProbe_string), &count);
	}
	
}


//Arm the oscilloscope
int DoubleProbeArm(void)
{
	int		result;
	int		count;
	char	DoubleProbe_string[16]="Null";
	
	
	sprintf (DoubleProbe_string, ":TRIG:SWE SINGl\r");   					//Single Trigger Sweep
 	result = viWrite (DoubleProbeRigol_handle1, DoubleProbe_string, (unsigned int)strlen(DoubleProbe_string), &count);
 	result = viWrite (DoubleProbeRigol_handle2, DoubleProbe_string, (unsigned int)strlen(DoubleProbe_string), &count);

	return result;
}




//Take the acquired data from the instrument and print it to the common data folder and 
//also push it to the MDS database
void Write_DoubleProbeData(void)
{
	char	ChannelNameString[36];
	char	outfilename[36];
	FILE*	outfile;
	int		j;
	float 	DoubleProbe_timestep=0.0;
	double	*DoubleProbe_Array1;
	double	*DoubleProbe_Array2;
	double	*DoubleProbe_Array3;
	double	*DoubleProbe_Array4;
	double	*DoubleProbe_Array5;
	double	*DoubleProbe_Array6;
	double	*DoubleProbe_Array7;
	double	*DoubleProbe_Array8;
	double	*timebase;
		
	//Create temporary data arrays	
	DoubleProbe_Array1 = malloc (Double_Probe_RecordLength);
	DoubleProbe_Array2 = malloc (Double_Probe_RecordLength);
	DoubleProbe_Array3 = malloc (Double_Probe_RecordLength);
	DoubleProbe_Array4 = malloc (Double_Probe_RecordLength);
	DoubleProbe_Array5 = malloc (Double_Probe_RecordLength);
	DoubleProbe_Array6 = malloc (Double_Probe_RecordLength);
	DoubleProbe_Array7 = malloc (Double_Probe_RecordLength);
	DoubleProbe_Array8 = malloc (Double_Probe_RecordLength);
	timebase = malloc (Double_Probe_RecordLength);
	
	//Grab data from all four channels of the oscilloscopes
	Rigol(DoubleProbeRigol_handle1, Double_Probe_RecordLength, &DoubleProbe_timestep, DoubleProbe_Array1, DoubleProbe_Array2, DoubleProbe_Array3, DoubleProbe_Array4);   
	Rigol(DoubleProbeRigol_handle2, Double_Probe_RecordLength, &DoubleProbe_timestep, DoubleProbe_Array5, DoubleProbe_Array6, DoubleProbe_Array7, DoubleProbe_Array8);   
	
	//Dump results to a file
	sprintf(outfilename, RawDataPath);
	strcat(outfilename, ShotNumberString);
	strcat(outfilename, "_");
	strcat(outfilename, "DoubleProbe.txt");
	
	outfile=fopen (outfilename, "w");
	fprintf(outfile,"t, Chanel1, Channel2, Channel3,Channel4,Channel5,Channel6\n");
	for (j=0;j<Double_Probe_RecordLength;j++) {
		timebase[j]=(double)(j*DoubleProbe_timestep);
		fprintf(outfile,"%f,%f,%f,%f,%f,%f,%f,%f,%f\n",timebase[j],DoubleProbe_Array1[j],DoubleProbe_Array2[j],DoubleProbe_Array3[j],DoubleProbe_Array4[j],DoubleProbe_Array5[j],DoubleProbe_Array6[j]);
	}

 	//Close Housekeeping data file
	fclose (outfile);
	
	//Open up Housekeepings settings panel and get filenames for each measuremen
	DoubleProbe_panel = LoadPanel (0, "DoubleProbeSystems.uir", DoublePrbe);
	RecallPanelState (DoubleProbe_panel, "Master_Control_Storage_File", DoubleProbe_setup_state);
		
	//Get channel name string, store raw data on drive folder, and push data to MDSPlus for Channel 1
	GetCtrlVal (DoubleProbe_panel, DoublePrbe_Chan1Name,ChannelNameString);
	Write_to_PHASMA_MDS(timebase,DoubleProbe_Array1,ChannelNameString);

	//Get channel name string, store raw data on drive folder, and push data to MDSPlus for Channel 2
	GetCtrlVal (DoubleProbe_panel, DoublePrbe_Chan2Name,ChannelNameString);
	Write_to_PHASMA_MDS(timebase,DoubleProbe_Array2,ChannelNameString);

	//Get channel name string, store raw data on drive folder, and push data to MDSPlus for Channel 3
	GetCtrlVal (DoubleProbe_panel, DoublePrbe_Chan3Name,ChannelNameString);
	Write_to_PHASMA_MDS(timebase,DoubleProbe_Array3,ChannelNameString);
	
	//Get channel name string, store raw data on drive folder, and push data to MDSPlus for Channel 4
	GetCtrlVal (DoubleProbe_panel, DoublePrbe_Chan4Name,ChannelNameString);
	Write_to_PHASMA_MDS(timebase,DoubleProbe_Array4,ChannelNameString);

	//Get channel name string, store raw data on drive folder, and push data to MDSPlus for Channel 5
	GetCtrlVal (DoubleProbe_panel, DoublePrbe_Chan5Name,ChannelNameString);
	Write_to_PHASMA_MDS(timebase,DoubleProbe_Array5,ChannelNameString);

	//Get channel name string, store raw data on drive folder, and push data to MDSPlus for Channel 6
	GetCtrlVal (DoubleProbe_panel, DoublePrbe_Chan6Name,ChannelNameString);
	Write_to_PHASMA_MDS(timebase,DoubleProbe_Array6,ChannelNameString);
}



//Launch this routine as window opens and it updates the window with the latest values from the 
//control system as it loads.
void OpenDoubleProbe_Settings (void)  
{
	//Open settings panel and load saved values
	DoubleProbe_panel = LoadPanel (0, "DoubleProbeSettings.uir", DoublePrbe);
	SavePanelState (DoubleProbe_panel, "Master_Control_Storage_File", DoubleProbe_setup_state);
	DisplayPanel(DoubleProbe_panel);
	
	//Update panel with 

	
	// Start interacting with user
    RunUserInterface ();

}

int CVICALLBACK Close_DoubleProbe_Settings (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			//Save current state of panel and close interface
			SavePanelState (DoubleProbe_panel, "Master_Control_Storage_File", DoubleProbe_setup_state);
			DiscardPanel(panel);
			break;
	}
	return 0;
}

int CVICALLBACK DoubleProbe_Move_to_Start (int panel, int control, int event,
							   void *callbackData, int eventData1, int eventData2)
{

	float 	DoubleProbe_RStart;
	float 	Current_Position;
	float  	DoubleProbe_Max_Insertion;
	float 	DoubleProbe_COUNTSPERMM;
	float 	DoubleProbe_Offset;
	char 	DoubleProbe_Probe_IP_Address[64];

	switch (event)
	{
		case EVENT_COMMIT:
			//Get current position of probe, starting position, end position, and what to do at end of scan range
			GetCtrlVal (DoubleProbe_panel, DoublePrbe_R_Start,&DoubleProbe_RStart);
			GetCtrlVal (DoubleProbe_panel, DoublePrbe_Probe_IP,DoubleProbe_Probe_IP_Address);
			GetCtrlVal (DoubleProbe_panel, DoublePrbe_Probe_Max_Insertion,&DoubleProbe_Max_Insertion);
			GetCtrlVal (DoubleProbe_panel, DoublePrbe_Probe_COUNTSPERMM,&DoubleProbe_COUNTSPERMM);
			GetCtrlVal (DoubleProbe_panel, DoublePrbe_Probe_Offset,&DoubleProbe_Offset);
			
			//Send move command to probe motor
			move_probe(DoubleProbe_Probe_IP_Address, (DoubleProbe_Offset-DoubleProbe_RStart)*10.0, DoubleProbe_COUNTSPERMM, DoubleProbe_Max_Insertion);
			
			//Set current position to Start
			SetCtrlVal (DoubleProbe_panel, DoublePrbe_R_Position,DoubleProbe_RStart);


			break;
	}
	return 0;
}


int CVICALLBACK DoubleProbe_Move_to_Stop (int panel, int control, int event,
							   void *callbackData, int eventData1, int eventData2)
{
	float 	DoubleProbe_RStop;
	float 	Current_Position;
	float  	DoubleProbe_Max_Insertion;
	float 	DoubleProbe_COUNTSPERMM;
	float 	DoubleProbe_Offset;
	char 	DoubleProbe_Probe_IP_Address[64];

	switch (event)
	{
		case EVENT_COMMIT:
			//Get current position of probe, starting position, end position, and what to do at end of scan range
			GetCtrlVal (DoubleProbe_panel, DoublePrbe_R_Stop,&DoubleProbe_RStop);
			GetCtrlVal (DoubleProbe_panel, DoublePrbe_Probe_IP,DoubleProbe_Probe_IP_Address);
			GetCtrlVal (DoubleProbe_panel, DoublePrbe_Probe_Max_Insertion,&DoubleProbe_Max_Insertion);
			GetCtrlVal (DoubleProbe_panel, DoublePrbe_Probe_COUNTSPERMM,&DoubleProbe_COUNTSPERMM);
			GetCtrlVal (DoubleProbe_panel, DoublePrbe_Probe_Offset,&DoubleProbe_Offset);
			
			//Send move command to probe motor
			move_probe(DoubleProbe_Probe_IP_Address, (DoubleProbe_Offset-DoubleProbe_RStop)*10.0, DoubleProbe_COUNTSPERMM, DoubleProbe_Max_Insertion);
			
			//Set current position to Start
			SetCtrlVal (DoubleProbe_panel, DoublePrbe_R_Position,DoubleProbe_RStop);


			break;
	}
	return 0;
}

int CVICALLBACK DoubleProbe_Home (int panel, int control, int event,
							   void *callbackData, int eventData1, int eventData2)
{

	char 	DoubleProbe_Probe_IP_Address[64];
	float 	DoubleProbe_Offset;

	switch (event)
	{
		case EVENT_COMMIT:
			//Get current position of probe, starting position, end position, and what to do at end of scan range
			GetCtrlVal (DoubleProbe_panel, DoublePrbe_Probe_IP,DoubleProbe_Probe_IP_Address);
			GetCtrlVal (DoubleProbe_panel, DoublePrbe_Probe_Offset,&DoubleProbe_Offset);
			
			//Send move command to probe motor
			home_probe(DoubleProbe_Probe_IP_Address);
			
			//Set current position to Offset
			SetCtrlVal (DoubleProbe_panel, DoublePrbe_R_Position,DoubleProbe_Offset);


			break;
	}
	return 0;
}

float Advance_DoubleProbe_Probe(void)
{
	int	 	Probe_Return_Flag;
	int 	Probe_Active_Move_Flag;
	float 	DoublePrbe_RStart;
	float 	DoublePrbe_RStop;
	float 	DoublePrbe_RIncrement;
	float 	DoublePrbe_Offset;
	float 	DoublePrbe_Return;
	float 	Current_Position;
	float 	Probe_Move;
	float  	DoublePrbe_Max_Insertion;
	float 	DoublePrbe_COUNTSPERMM;
	char 	DoublePrbe_Probe_IP_Address[64];
	
	//Open settings panel and load saved values
	DoubleProbe_panel = LoadPanel (0, "BdotSettings.uir", DoublePrbe);
	RecallPanelState (DoubleProbe_panel, "Master_Control_Storage_File", DoubleProbe_setup_state);
	
	//Get current position of probe, starting position, end position, and what to do at end of scan range
	GetCtrlVal (DoubleProbe_panel, DoublePrbe_R_Start,&DoublePrbe_RStart);
	GetCtrlVal (DoubleProbe_panel, DoublePrbe_R_Stop,&DoublePrbe_RStop);
	GetCtrlVal (DoubleProbe_panel, DoublePrbe_R_Increment,&DoublePrbe_RIncrement);
	GetCtrlVal (DoubleProbe_panel, DoublePrbe_Probe_Offset,&DoublePrbe_Offset);
	GetCtrlVal (DoubleProbe_panel, DoublePrbe_Probe_Return,&DoublePrbe_Return);
	GetCtrlVal (DoubleProbe_panel, DoublePrbe_R_Position,&Current_Position);
	GetCtrlVal (DoubleProbe_panel, DoublePrbe_Probe_Return,&Probe_Return_Flag);
	GetCtrlVal (DoubleProbe_panel, DoublePrbe_Probe_Active_Move,&Probe_Active_Move_Flag);
	GetCtrlVal (DoubleProbe_panel, DoublePrbe_Probe_IP,DoublePrbe_Probe_IP_Address);
	GetCtrlVal (DoubleProbe_panel, DoublePrbe_Probe_Max_Insertion,&DoublePrbe_Max_Insertion);
	GetCtrlVal (DoubleProbe_panel, DoublePrbe_Probe_COUNTSPERMM,&DoublePrbe_COUNTSPERMM);

	if (Probe_Active_Move_Flag) {
		
		//Get current position from motor controller
	
		//Calculate distance to move to next position
		Probe_Move=Current_Position+DoublePrbe_RIncrement;
		
		//If next position is beyond the stop position, either do nothing or send 
		//probe back to start position depending on settings in interface window
		if ( (DoublePrbe_RIncrement + Current_Position) > DoublePrbe_RStop) {
			return Current_Position;
		} else {
			//Command motor to move to next position	
			move_probe(DoublePrbe_Probe_IP_Address, (DoublePrbe_Offset-Probe_Move)*10.0, DoublePrbe_COUNTSPERMM, DoublePrbe_Max_Insertion);
		}

		
		//Update window with current position and return current position to master code
		SetCtrlVal (DoubleProbe_panel, DoublePrbe_R_Position,(DoublePrbe_RIncrement + Current_Position));
		return DoublePrbe_RIncrement + Current_Position;
	} else {
		return Current_Position;
	}

	SavePanelState (DoubleProbe_panel, "Master_Control_Storage_File", DoubleProbe_setup_state);

}

