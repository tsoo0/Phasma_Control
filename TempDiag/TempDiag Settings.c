#include 	<ansi_c.h>
#include	<visa.h>
#include	<stdlib.h>
#include 	<formatio.h>
#include	"PHASMA-MDS-Routines.h"
#include 	"TempDiagSystems.h"
#include 	"TempDiagSettings.h"  
#include	"GlobalVariables.h"
#include	"Rigol.h"
#include	"Lecroy.h"

int		TempDiag_panel;
int		TempDiag_setup_state = 7;
int 	Rigol_flag=0;
int 	Lecroy_flag=0;
int		TemporaryDiag_RecordLength=8192;
int		TemporaryDiag_RecordLength_lecroy=8192;
float	TemporaryDiag_VoltageRange=10.0;
float	TriggerLevel=1.0;
float	TimeWindow=200;
float	TemporaryDiag_VoltageRange_lecroy=10.0;
float	TriggerLevel_lecroy=1.0;
float	TimeWindow_lecroy=200;

ViSession 	Rigol_handle;
ViSession 	Lecroy_handle;


//Set up the oscilloscopes
void TempDiagActivate(void)
{
	int 		result; 					//	Error-checking variable
	float		TemporaryDiag_VoltageRange=10;
	char		TempDiag_string[32]="Null";
	char		IP_Address_string[64];  
	char		TriggerSource[8];
	char		TriggerSource_lecroy[8];
	ViUInt32 	count;

	//Open up Temp Diagnostic settings panel and get usage of either Rigol or LeCroy scope
	TempDiag_panel = LoadPanel (0, "TempDiagSettings.uir", TempDiag);
	RecallPanelState (TempDiag_panel, "Master_Control_Storage_File", TempDiag_setup_state);
	
	//Get status of both scopes
	GetCtrlVal (TempDiag_panel, TempDiag_RIGOL,&Rigol_flag);
	GetCtrlVal (TempDiag_panel, TempDiag_LECROY,&Lecroy_flag);
	  
 	if (Rigol_flag) {
	//Get sample rate, record length, and channel scales for Rigol scope
		GetCtrlVal (TempDiag_panel, TempDiag_RecordLength,&TemporaryDiag_RecordLength);
		GetCtrlVal (TempDiag_panel, TempDiag_TimeWindow,&TimeWindow);
		GetCtrlVal (TempDiag_panel, TempDiag_VoltageRange,&TemporaryDiag_VoltageRange);
		GetCtrlVal (TempDiag_panel, TempDiag_TriggerSource,TriggerSource);
		GetCtrlVal (TempDiag_panel, TempDiag_TriggerLevel,&TriggerLevel);

		//Get Rigol ip address from interface panel and open connection to the scope
		GetCtrlVal (TempDiag_panel, TempDiag_RIGOL_temp_Address, IP_Address_string);
	 	result = viOpen (Global_Visa_Session_Handle, IP_Address_string, VI_NULL, VI_NULL, &Rigol_handle);
			
		if (!result) {
			//Make sure scope is running so commands will work
			sprintf (TempDiag_string, ":RUN\r");   																							//Send the run command
			result = viWrite (Rigol_handle, TempDiag_string, (unsigned int)strlen(TempDiag_string), &count);

			//Setup the rate, record length, trigger system, and voltage range for the Rigol scope
		  	sprintf (TempDiag_string, ":TIM:MAIN:SCAL %f\r",(TimeWindow/10.0));   													//Set up timebase scaling based on target sample window and 10 divisions
			result = viWrite (Rigol_handle, TempDiag_string, (unsigned int)strlen(TempDiag_string), &count);
			sprintf (TempDiag_string, ":ACQ:MDEP %d\r",TemporaryDiag_RecordLength);   												//Set the record length by setting the Memory Depth and upload the record length in k
			result = viWrite (Rigol_handle, TempDiag_string, (unsigned int)strlen(TempDiag_string), &count);
			sprintf (TempDiag_string, ":CHAN1:SCAL  %f\r",TemporaryDiag_VoltageRange);   											//Set the voltage scale of the channel to the target value
			result = viWrite (Rigol_handle, TempDiag_string, (unsigned int)strlen(TempDiag_string), &count);
			sprintf (TempDiag_string, ":CHAN2:SCAL  %f\r",TemporaryDiag_VoltageRange);   											//Set the voltage scale of the channel to the target value
			result = viWrite (Rigol_handle, TempDiag_string, (unsigned int)strlen(TempDiag_string), &count);
			sprintf (TempDiag_string, ":CHAN3:SCAL  %f\r",TemporaryDiag_VoltageRange);   											//Set the voltage scale of the channel to the target value
			result = viWrite (Rigol_handle, TempDiag_string, (unsigned int)strlen(TempDiag_string), &count);
			sprintf (TempDiag_string, ":CHAN4:SCAL  %f\r",TemporaryDiag_VoltageRange);   											//Set the voltage scale of the channel to the target value
			result = viWrite (Rigol_handle, TempDiag_string, (unsigned int)strlen(TempDiag_string), &count);
		 	sprintf (TempDiag_string, ":TRIG:MODE EDGE\r");   
			result = viWrite (Rigol_handle, TempDiag_string, (unsigned int)strlen(TempDiag_string), &count);
			sprintf (TempDiag_string, ":TRIG:COUP AC\r");   
			result = viWrite (Rigol_handle, TempDiag_string, (unsigned int)strlen(TempDiag_string), &count);
			sprintf (TempDiag_string, ":TRIG:EDGE:SOUR %s\r",TriggerSource);  														//Set the trigger source to the target value 
			result = viWrite (Rigol_handle, TempDiag_string, (unsigned int)strlen(TempDiag_string), &count);
		 	sprintf (TempDiag_string, ":TRIG:EDGE:SLOP POS\r");   
			result = viWrite (Rigol_handle, TempDiag_string, (unsigned int)strlen(TempDiag_string), &count);
			sprintf (TempDiag_string, ":TRIG:EDGE:LEV %f\r",TriggerLevel);   
			result = viWrite (Rigol_handle, TempDiag_string, (unsigned int)strlen(TempDiag_string), &count);
			sprintf (TempDiag_string, ":SYST:KEY:PRES MOFF\r");   												 	//Hide menu on scope screen
		 	result = viWrite (Rigol_handle, TempDiag_string, (unsigned int)strlen(TempDiag_string), &count);
		}
	}
	
	if (Lecroy_flag) {
		//Get sample rate, record length, and channel scales for LeCroy scope
		GetCtrlVal (TempDiag_panel, TempDiag_RecordLength,&TemporaryDiag_RecordLength_lecroy);
		GetCtrlVal (TempDiag_panel, TempDiag_TimeWindow,&TimeWindow_lecroy);
		GetCtrlVal (TempDiag_panel, TempDiag_VoltageRange,&TemporaryDiag_VoltageRange_lecroy);
		GetCtrlVal (TempDiag_panel, TempDiag_TriggerSource,TriggerSource_lecroy);
		GetCtrlVal (TempDiag_panel, TempDiag_TriggerLevel,&TriggerLevel_lecroy);

		//Get Rigol ip address from interface panel and open connection to the scope
		GetCtrlVal (TempDiag_panel, TempDiag_LeCroy_temp_Address, IP_Address_string);
	 	result = viOpen (Global_Visa_Session_Handle, IP_Address_string, VI_NULL, VI_NULL, &Lecroy_handle);
			
		if (!result) {
			//Following example set by Peiyun for LeCroy scope operation, send a series of commands to set up the scope
			sprintf (TempDiag_string, "MSG Pushing Data to MDS\r");   														//not quite the command he used, but it should push a message to the screen
			result = viWrite (Lecroy_handle, TempDiag_string, (unsigned int)strlen(TempDiag_string), &count);	
			
			//Setup the rate, record length, trigger system, and voltage range for the Rigol scope
		  	sprintf (TempDiag_string, "COMM_HEADER OFF\r");   																//Tell scope not to send back responses
			result = viWrite (Lecroy_handle, TempDiag_string, (unsigned int)strlen(TempDiag_string), &count);
		  	sprintf (TempDiag_string, "COMM_FORMAT DEF9,WORD,BIN\r");   													//Set up the format of the data records (maybe change BIN to ASCII?)
			result = viWrite (Lecroy_handle, TempDiag_string, (unsigned int)strlen(TempDiag_string), &count);
			sprintf (TempDiag_string, "MSIZ %d\r",TemporaryDiag_RecordLength);   											//Set the record length
			result = viWrite (Lecroy_handle, TempDiag_string, (unsigned int)strlen(TempDiag_string), &count);
			sprintf (TempDiag_string, "TRIG_SELECT EDGE,SR %s\r",TriggerSource_lecroy);   									//Set the trigger source
			result = viWrite (Lecroy_handle, TempDiag_string, (unsigned int)strlen(TempDiag_string), &count);
			sprintf (TempDiag_string, "EX:TRLV %fV\r",TriggerLevel_lecroy);   												//Set external trigger voltage level
			result = viWrite (Lecroy_handle, TempDiag_string, (unsigned int)strlen(TempDiag_string), &count);
			sprintf (TempDiag_string, "EX:TRCP LFREJ\r");   																//Set external trigger to low frequency reject
			result = viWrite (Lecroy_handle, TempDiag_string, (unsigned int)strlen(TempDiag_string), &count);
			sprintf (TempDiag_string, "EX:TRSL POS\r");   																	//Set external trigger to postitive slope for trigger
			result = viWrite (Lecroy_handle, TempDiag_string, (unsigned int)strlen(TempDiag_string), &count);
			sprintf (TempDiag_string, "TDIV %fUS\r",TimeWindow_lecroy);   													//Set the timebase in microseconds
			result = viWrite (Lecroy_handle, TempDiag_string, (unsigned int)strlen(TempDiag_string), &count);
			sprintf (TempDiag_string, "C1:VDIV  %fV\r",TemporaryDiag_VoltageRange_lecroy);   								//Set the voltage scale of the channel to the target value
			result = viWrite (Lecroy_handle, TempDiag_string, (unsigned int)strlen(TempDiag_string), &count);
			sprintf (TempDiag_string, "C2:VDIV  %fV\r",TemporaryDiag_VoltageRange_lecroy);   								//Set the voltage scale of the channel to the target value
			result = viWrite (Lecroy_handle, TempDiag_string, (unsigned int)strlen(TempDiag_string), &count);
			sprintf (TempDiag_string, "C3:VDIV  %fV\r",TemporaryDiag_VoltageRange_lecroy);   								//Set the voltage scale of the channel to the target value
			result = viWrite (Lecroy_handle, TempDiag_string, (unsigned int)strlen(TempDiag_string), &count);
			sprintf (TempDiag_string, "C4:VDIV  %fV\r",TemporaryDiag_VoltageRange_lecroy);   								//Set the voltage scale of the channel to the target value
			result = viWrite (Lecroy_handle, TempDiag_string, (unsigned int)strlen(TempDiag_string), &count);
		}
	}

	//Discard housekeeping panel
	DiscardPanel(TempDiag_panel);
	
}


//Arm the VX4244 digitizer
int TempDiagArm(void)
{
	int		result=0;
	int		count;
	char	TempDiag_string[16]="Null";
	
	//	Arm Rigol scope if in use
	if (Rigol_flag) {
		sprintf (TempDiag_string, ":TRIG:SWE SINGl\r");   												 					//Single Trigger Sweep
	 	result = viWrite (Rigol_handle, TempDiag_string, (unsigned int)strlen(TempDiag_string), &count);
	}
	
	//	Arm LeCroy scope if in use
	if (Lecroy_flag) {
		sprintf (TempDiag_string, "TRMD SINGLE\r");   												 						//Single Trigger Sweep
	 	result = viWrite (Lecroy_handle, TempDiag_string, (unsigned int)strlen(TempDiag_string), &count);
		sprintf (TempDiag_string, "ARM\r");   												 								//ARM Scope
	 	result = viWrite (Lecroy_handle, TempDiag_string, (unsigned int)strlen(TempDiag_string), &count);
		sprintf (TempDiag_string, "Armed, waiting for shot\r");   															//Put armed message on screen of scope
		result = viWrite (Lecroy_handle, TempDiag_string, (unsigned int)strlen(TempDiag_string), &count);	
	}
	
	return result;
}




//Take the acquired data from the instrument and print it to the common data folder and 
//also push it to the MDS database
void Write_TempDiagData(void)
{
	char	ChannelNameString[36];
	char	outfilename[36];
	FILE*	outfile;
	int		j;
	float 	TempDiag_timestep=0.0;
	float 	TempDiag_timestep_lecroy=0.0;
	double	*TempDiag_Array1;
	double	*TempDiag_Array2;
	double	*TempDiag_Array3;
	double	*TempDiag_Array4;
	double	*TempDiag_Array5;
	double	*TempDiag_Array6;
	double	*TempDiag_Array7;
	double	*TempDiag_Array8;
	double	*timebase;
	double	*timebase_lecroy;
		
	//Create temporary data arrays	
	TempDiag_Array1 = malloc (TemporaryDiag_RecordLength);
	TempDiag_Array2 = malloc (TemporaryDiag_RecordLength);
	TempDiag_Array3 = malloc (TemporaryDiag_RecordLength);
	TempDiag_Array4 = malloc (TemporaryDiag_RecordLength);
	TempDiag_Array5 = malloc (TemporaryDiag_RecordLength_lecroy);
	TempDiag_Array6 = malloc (TemporaryDiag_RecordLength_lecroy);
	TempDiag_Array7 = malloc (TemporaryDiag_RecordLength_lecroy);
	TempDiag_Array8 = malloc (TemporaryDiag_RecordLength_lecroy);
	timebase = malloc (TemporaryDiag_RecordLength);
	timebase_lecroy = malloc (TemporaryDiag_RecordLength_lecroy);
	
	//Grab data from all four channels of the oscilloscope
	if (Rigol_flag) Rigol(Rigol_handle, TemporaryDiag_RecordLength, &TempDiag_timestep, TempDiag_Array1, TempDiag_Array2, TempDiag_Array3, TempDiag_Array4);   
	if (Lecroy_flag) Rigol(Rigol_handle, TemporaryDiag_RecordLength_lecroy, &TempDiag_timestep_lecroy, TempDiag_Array5, TempDiag_Array6, TempDiag_Array7, TempDiag_Array8);   
	
	//Dump results to a file
	sprintf(outfilename, RawDataPath);
	strcat(outfilename, ShotNumberString);
	strcat(outfilename, "_");
	strcat(outfilename, "TempDiag.txt");
	
	outfile=fopen (outfilename, "w");
	
	if (Rigol_flag) {
		fprintf(outfile,"t, Channel1, Channel2, Channel3,Channel4\n");
		for (j=0;j<TemporaryDiag_RecordLength;j++) {
			timebase[j]=(double)(j*TempDiag_timestep);
			fprintf(outfile,"%f,%f,%f,%f,%f,%f,%f,%f,%f\n",timebase[j],TempDiag_Array1[j],TempDiag_Array2[j],TempDiag_Array3[j],TempDiag_Array4[j]);
		}
	}
	
	if (Lecroy_flag) {
		fprintf(outfile,"t, Channel5, Channel6, Channel7,Channel8\n");
		for (j=0;j<TemporaryDiag_RecordLength_lecroy;j++) {
			timebase[j]=(double)(j*TempDiag_timestep_lecroy);
			fprintf(outfile,"%f,%f,%f,%f,%f,%f,%f,%f,%f\n",timebase[j],TempDiag_Array1[j],TempDiag_Array2[j],TempDiag_Array3[j],TempDiag_Array4[j]);
		}
	}
	
 	//Close Housekeeping data file
	fclose (outfile);
	
	//Open up Housekeepings settings panel and get filenames for each measuremen
	TempDiag_panel = LoadPanel (0, "TempDiagSystems.uir", TempDiag);
	RecallPanelState (TempDiag_panel, "Master_Control_Storage_File", TempDiag_setup_state);
	
	if (Rigol_flag) {
		//Get channel name string, store raw data on drive folder, and push data to MDSPlus for Channel 0
		GetCtrlVal (TempDiag_panel, TempDiag_Chan1Name,ChannelNameString);
		Write_to_PHASMA_MDS(timebase,TempDiag_Array1,ChannelNameString);

		//Get channel name string, store raw data on drive folder, and push data to MDSPlus for Channel 1
		GetCtrlVal (TempDiag_panel, TempDiag_Chan2Name,ChannelNameString);
		Write_to_PHASMA_MDS(timebase,TempDiag_Array2,ChannelNameString);

		//Get channel name string, store raw data on drive folder, and push data to MDSPlus for Channel 2
		GetCtrlVal (TempDiag_panel, TempDiag_Chan3Name,ChannelNameString);
		Write_to_PHASMA_MDS(timebase,TempDiag_Array3,ChannelNameString);
		
		//Get channel name string, store raw data on drive folder, and push data to MDSPlus for Channel 3
		GetCtrlVal (TempDiag_panel, TempDiag_Chan4Name,ChannelNameString);
		Write_to_PHASMA_MDS(timebase,TempDiag_Array4,ChannelNameString);
	}
	if (Lecroy_flag) {
		//Get channel name string, store raw data on drive folder, and push data to MDSPlus for Channel 0
		GetCtrlVal (TempDiag_panel, TempDiag_Chan1Name,ChannelNameString);
		Write_to_PHASMA_MDS(timebase_lecroy,TempDiag_Array5,ChannelNameString);

		//Get channel name string, store raw data on drive folder, and push data to MDSPlus for Channel 1
		GetCtrlVal (TempDiag_panel, TempDiag_Chan2Name,ChannelNameString);
		Write_to_PHASMA_MDS(timebase_lecroy,TempDiag_Array6,ChannelNameString);

		//Get channel name string, store raw data on drive folder, and push data to MDSPlus for Channel 2
		GetCtrlVal (TempDiag_panel, TempDiag_Chan3Name,ChannelNameString);
		Write_to_PHASMA_MDS(timebase_lecroy,TempDiag_Array7,ChannelNameString);
		
		//Get channel name string, store raw data on drive folder, and push data to MDSPlus for Channel 3
		GetCtrlVal (TempDiag_panel, TempDiag_Chan4Name,ChannelNameString);
		Write_to_PHASMA_MDS(timebase_lecroy,TempDiag_Array8,ChannelNameString);
	}

}

//Launch this routine as window opens and it updates the window with the latest values from the 
//control system as it loads.
void OpenTempDiag_Settings (void)  
{
	//Open settings panel and load saved values
	TempDiag_panel = LoadPanel (0, "TempDiagSettings.uir", TempDiag);
	SavePanelState (TempDiag_panel, "Master_Control_Storage_File", TempDiag_setup_state);
	DisplayPanel(TempDiag_panel);
	
	//Update panel with 

	
	// Start interacting with user
    RunUserInterface ();

}

int CVICALLBACK Close_TempDiag_Settings (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			//Save current state of panel and close interface
			SavePanelState (TempDiag_panel, "Master_Control_Storage_File", TempDiag_setup_state);
			DiscardPanel(panel);
			break;
	}
	return 0;
}


