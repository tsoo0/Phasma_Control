#include 	<ansi_c.h>
#include	<visa.h>
#include	<stdlib.h>
#include 	<formatio.h>
#include	"PHASMA-MDS-Routines.h"
#include 	"QBSSystems.h"
#include 	"QuantumBeatSettings.h"  
#include	"GlobalVariables.h"
#include	"Lecroy.h"

int		QBS_panel;
int		QBS_setup_state = 7001;


//Set up the oscilloscopes
void QBSActivate(void)
{
	int 		result; 					//	Error-checking variable
	char		QBS_string[32]="Null";
	char		IP_Address_string[64];  
	ViUInt32 	count;

	//Open up Temp Diagnostic settings panel and get usage of either Rigol or LeCroy scope
	QBS_panel = LoadPanel (0, "QuantumBeatSettings.uir", QBS);
	RecallPanelState (QBS_panel, "Master_Control_Storage_File", QBS_setup_state);
	
	//Discard housekeeping panel
	DiscardPanel(QBS_panel);
	
}


//Arm the VX4244 digitizer
int QBSArm(void)
{
	int		result=0;
	int		count;
	char	QBS_string[16]="Null";
	
	//	Arm Rigol scope if in use
	
	return result;
}




//Take the acquired data from the instrument and print it to the common data folder and 
//also push it to the MDS database
void Write_QBSData(void)
{


}

//Launch this routine as window opens and it updates the window with the latest values from the 
//control system as it loads.
void OpenQBS_Settings (void)  
{
	//Open settings panel and load saved values
	QBS_panel = LoadPanel (0, "QuantumBeatSettings.uir", QBS);
	SavePanelState (QBS_panel, "Master_Control_Storage_File", QBS_setup_state);
	DisplayPanel(QBS_panel);
	
	//Update panel with 

	
	// Start interacting with user
    RunUserInterface ();

}

int CVICALLBACK Close_QBS_Settings (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			//Save current state of panel and close interface
			SavePanelState (QBS_panel, "Master_Control_Storage_File", QBS_setup_state);
			DiscardPanel(panel);
			break;
	}
	return 0;
}


