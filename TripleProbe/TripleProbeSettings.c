//#include	<pyconfig.h>
//#include	<Python.h>
#include 	<ansi_c.h>
#include	<visa.h>
#include	<stdlib.h>
#include 	<formatio.h>
#include	"PHASMA-MDS-Routines.h"
#include 	"TripleProbeSystems.h"
#include 	"TripleProbeSettings.h"  
#include	"GlobalVariables.h"
#include	"PHASMA_C_to_Python_convertor.h"


int			TripleProbe_panel;
int			TripleProbe_setup_state = 75;
int			TripleProbe_RecordLength=8192;



//Set up the T-Daq system using the Python scripts. This function configures and arms the system in one process
int TripleProbeArm(void)
{

	int 		TripleProbe_RecordLength;
	float 		TripleProbe_Sample_Rate;
	char 		TripleProbe_IP1[64];
	char 		TripleProbe_IP2[64];


	
	//First, open panel and get key parameters
	TripleProbe_panel = LoadPanel (0, "TripleProbeSettings.uir", TriplePrbe);
	RecallPanelState (TripleProbe_panel, "Master_Control_Storage_File", TripleProbe_setup_state);
	
	//Access the info from the interface
	GetCtrlVal (TripleProbe_panel, TriplePrbe_t_DAQ_IP1,TripleProbe_IP1);
	
	//Test python code functionality
	WritePrimesToFile();
	
	//Arm the two t_DAC modules for the TripleProbes
	t_DAQ_Arm(TripleProbe_IP1);
	
	//Discard housekeeping panel
	DiscardPanel(TripleProbe_panel);
	
	return 1;
}


//Load parameters of the system
void TripleProbeActivate(void)
{
	int 		TripleProbe_RecordLength;
	float 		TripleProbe_Sample_Rate;
	char 		TripleProbe_IP1[64];
	char 		TripleProbe_IP2[64];
	
	//First, open panel and get key parameters
	TripleProbe_panel = LoadPanel (0, "TripleProbeSettings.uir", TriplePrbe);
	RecallPanelState (TripleProbe_panel, "Master_Control_Storage_File", TripleProbe_setup_state);
	
	//Access the info from the interface
	GetCtrlVal (TripleProbe_panel, TriplePrbe_t_DAQ_Samples,&TripleProbe_RecordLength);
	GetCtrlVal (TripleProbe_panel, TriplePrbe_t_DAQ_Freq,&TripleProbe_Sample_Rate);
	GetCtrlVal (TripleProbe_panel, TriplePrbe_t_DAQ_IP1,TripleProbe_IP1);
		
	//Test python code functionality
	WritePrimesToFile();
	
	//Configure the two t_DAC modules for the TripleProbes
	t_DAQ_Config(TripleProbe_IP1, TripleProbe_Sample_Rate, TripleProbe_RecordLength);
}




//Calls the Python function that reads out the data from the T-daq System
//and pushes it to the raw data folder and the MDS database
void Write_TripleProbeData(void)
{
	int 		TripleProbe_RecordLength;
	float 		TripleProbe_Sample_Rate;
	char 		TripleProbe_IP1[64];
	char 		TripleProbe_Data_Path[64];
	
	//First, open panel and get key parameters
	TripleProbe_panel = LoadPanel (0, "TripleProbeSettings.uir", TriplePrbe);
	RecallPanelState (TripleProbe_panel, "Master_Control_Storage_File", TripleProbe_setup_state);

	GetCtrlVal (TripleProbe_panel, TriplePrbe_t_DAQ_IP1,TripleProbe_IP1);

	//Discard housekeeping panel
	DiscardPanel(TripleProbe_panel);
	
	//Call Python routine to write the data to a raw data file and the MDS tree
	t_DAQ_Data_Read(TripleProbe_IP1, RawDataPath);


}




//Launch this routine as window opens and it updates the window with the latest values from the 
//control system as it loads.
void OpenTripleProbe_Settings (void)  
{
	//Open settings panel and load saved values
	TripleProbe_panel = LoadPanel (0, "TripleProbeSettings.uir", TriplePrbe);
	RecallPanelState (TripleProbe_panel, "Master_Control_Storage_File", TripleProbe_setup_state);
	DisplayPanel(TripleProbe_panel);
	
	// Start interacting with user
    RunUserInterface ();

}

int CVICALLBACK Close_TripleProbe_Settings (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			//Save current state of panel and close interface
			SavePanelState (TripleProbe_panel, "Master_Control_Storage_File", TripleProbe_setup_state);
			DiscardPanel(panel);
			break;
	}
	return 0;
}


int CVICALLBACK TripleProbe_Move_to_Start (int panel, int control, int event,
							   void *callbackData, int eventData1, int eventData2)
{

	float 	TriplePrbe_RStart;
	float 	Current_Position;
	float  	TriplePrbe_Max_Insertion;
	float 	TriplePrbe_COUNTSPERMM;
	float 	TriplePrbe_Offset;
	char 	TriplePrbe_Probe_IP_Address[64];

	switch (event)
	{
		case EVENT_COMMIT:
			//Get current position of probe, starting position, end position, and what to do at end of scan range
			GetCtrlVal (TripleProbe_panel, TriplePrbe_R_Start,&TriplePrbe_RStart);
			GetCtrlVal (TripleProbe_panel, TriplePrbe_Probe_IP,TriplePrbe_Probe_IP_Address);
			GetCtrlVal (TripleProbe_panel, TriplePrbe_Probe_Max_Insertion,&TriplePrbe_Max_Insertion);
			GetCtrlVal (TripleProbe_panel, TriplePrbe_Probe_COUNTSPERMM,&TriplePrbe_COUNTSPERMM);
			GetCtrlVal (TripleProbe_panel, TriplePrbe_Probe_Offset,&TriplePrbe_Offset);
			
			//Send move command to probe motor
			move_probe(TriplePrbe_Probe_IP_Address, (TriplePrbe_Offset-TriplePrbe_RStart)*10.0, TriplePrbe_COUNTSPERMM, TriplePrbe_Max_Insertion);
			
			//Set current position to Start
			SetCtrlVal (TripleProbe_panel, TriplePrbe_R_Position,TriplePrbe_RStart);


			break;
	}
	return 0;
}


int CVICALLBACK TripleProbe_Move_to_Stop (int panel, int control, int event,
							   void *callbackData, int eventData1, int eventData2)
{
	float 	TriplePrbe_RStop;
	float 	Current_Position;
	float  	TriplePrbe_Max_Insertion;
	float 	TriplePrbe_COUNTSPERMM;
	float 	TriplePrbe_Offset;
	char 	TriplePrbe_Probe_IP_Address[64];

	switch (event)
	{
		case EVENT_COMMIT:
			//Get current position of probe, starting position, end position, and what to do at end of scan range
			GetCtrlVal (TripleProbe_panel, TriplePrbe_R_Stop,&TriplePrbe_RStop);
			GetCtrlVal (TripleProbe_panel, TriplePrbe_Probe_IP,TriplePrbe_Probe_IP_Address);
			GetCtrlVal (TripleProbe_panel, TriplePrbe_Probe_Max_Insertion,&TriplePrbe_Max_Insertion);
			GetCtrlVal (TripleProbe_panel, TriplePrbe_Probe_COUNTSPERMM,&TriplePrbe_COUNTSPERMM);
			GetCtrlVal (TripleProbe_panel, TriplePrbe_Probe_Offset,&TriplePrbe_Offset);
			
			//Send move command to probe motor
			move_probe(TriplePrbe_Probe_IP_Address, (TriplePrbe_Offset-TriplePrbe_RStop)*10.0, TriplePrbe_COUNTSPERMM, TriplePrbe_Max_Insertion);
			
			//Set current position to Start
			SetCtrlVal (TripleProbe_panel, TriplePrbe_R_Position,TriplePrbe_RStop);


			break;
	}
	return 0;
}

int CVICALLBACK TripleProbe_Home (int panel, int control, int event,
							   void *callbackData, int eventData1, int eventData2)
{

	char 	TriplePrbe_Probe_IP_Address[64];
	float 	TriplePrbe_Offset;

	switch (event)
	{
		case EVENT_COMMIT:
			//Get current position of probe, starting position, end position, and what to do at end of scan range
			GetCtrlVal (TripleProbe_panel, TriplePrbe_Probe_IP,TriplePrbe_Probe_IP_Address);
			GetCtrlVal (TripleProbe_panel, TriplePrbe_Probe_Offset,&TriplePrbe_Offset);
			
			//Send move command to probe motor
			home_probe(TriplePrbe_Probe_IP_Address);
			
			//Set current position to Offset
			SetCtrlVal (TripleProbe_panel, TriplePrbe_R_Position,TriplePrbe_Offset);


			break;
	}
	return 0;
}

float Advance_TripleProbe_Probe(void)
{
	int	 	Probe_Return_Flag;
	int 	Probe_Active_Move_Flag;
	float 	TriplePrbe_RStart;
	float 	TriplePrbe_RStop;
	float 	TriplePrbe_RIncrement;
	float 	TriplePrbe_Offset;
	float 	TriplePrbe_Return;
	float 	Current_Position;
	float 	Probe_Move;
	float  	TriplePrbe_Max_Insertion;
	float 	TriplePrbe_COUNTSPERMM;
	char 	TriplePrbe_Probe_IP_Address[64];
	
	//Open settings panel and load saved values
	TripleProbe_panel = LoadPanel (0, "BdotSettings.uir", TriplePrbe);
	RecallPanelState (TripleProbe_panel, "Master_Control_Storage_File", TripleProbe_setup_state);
	
	//Get current position of probe, starting position, end position, and what to do at end of scan range
	GetCtrlVal (TripleProbe_panel, TriplePrbe_R_Start,&TriplePrbe_RStart);
	GetCtrlVal (TripleProbe_panel, TriplePrbe_R_Stop,&TriplePrbe_RStop);
	GetCtrlVal (TripleProbe_panel, TriplePrbe_R_Increment,&TriplePrbe_RIncrement);
	GetCtrlVal (TripleProbe_panel, TriplePrbe_Probe_Offset,&TriplePrbe_Offset);
	GetCtrlVal (TripleProbe_panel, TriplePrbe_Probe_Return,&TriplePrbe_Return);
	GetCtrlVal (TripleProbe_panel, TriplePrbe_R_Position,&Current_Position);
	GetCtrlVal (TripleProbe_panel, TriplePrbe_Probe_Return,&Probe_Return_Flag);
	GetCtrlVal (TripleProbe_panel, TriplePrbe_Probe_Active_Move,&Probe_Active_Move_Flag);
	GetCtrlVal (TripleProbe_panel, TriplePrbe_Probe_IP,TriplePrbe_Probe_IP_Address);
	GetCtrlVal (TripleProbe_panel, TriplePrbe_Probe_Max_Insertion,&TriplePrbe_Max_Insertion);
	GetCtrlVal (TripleProbe_panel, TriplePrbe_Probe_COUNTSPERMM,&TriplePrbe_COUNTSPERMM);

	if (Probe_Active_Move_Flag) {
		
		//Get current position from motor controller
	
		//Calculate distance to move to next position
		Probe_Move=Current_Position+TriplePrbe_RIncrement;
		
		//If next position is beyond the stop position, either do nothing or send 
		//probe back to start position depending on settings in interface window
		if ( (TriplePrbe_RIncrement + Current_Position) > TriplePrbe_RStop) {
			return Current_Position;
		} else {
			//Command motor to move to next position	
			move_probe(TriplePrbe_Probe_IP_Address, (TriplePrbe_Offset-Probe_Move)*10.0, TriplePrbe_COUNTSPERMM, TriplePrbe_Max_Insertion);
		}

		
		//Update window with current position and return current position to master code
		SetCtrlVal (TripleProbe_panel, TriplePrbe_R_Position,(TriplePrbe_RIncrement + Current_Position));
		return TriplePrbe_RIncrement + Current_Position;
	} else {
		return Current_Position;
	}

	SavePanelState (TripleProbe_panel, "Master_Control_Storage_File", TripleProbe_setup_state);

}