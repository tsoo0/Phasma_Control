#include <userint.h>
#include "BdotSettings.h"

/*******************************************************************************
These sets of functions use a new hybrid implementation of C written in LabWindows,
C written in Microsoft Visual C (MSVC) and Python. The reason for this process is that
the C compiler in LabWindows is incompatible with the native C Python API - which assumes
that MVSC is the version of C that is being used. Therefore, to be able to call Python
commands from LabWindows, we first write C code in MSVC that calls the Python routines and
then export those C commands to LabWindows C by creating a header file, a .dll file, and
a library file. Then from LabWindows we can call these C functions that call the Python.
One of the challenges was setting up up the paths in the MVSC C code to properly call the
Python libraries and headers which are set up in Anaconda. Right now the implementation has 
very fixed paths that must be modified to match the paths on the machine being used. Eventually
it would be good to make these relative paths so that the code is more self-contained and portable.
To this end, we will assume for now that the primary Python installation is on the C drive in 
an anaconda folder and the DAQ code is on the C drive in a PHASMA_DAQ folder. The process is as 
follows:

Step 1: Create the Python code with a set of modules that will be called by the MVSC code.

Step 2: Create the MVSC C code and make sure it calls the Python functions properly - use the template of
the existing code to get the initialization and Python paths correct.

Step 3: Open the x64 Native Tools Command Prompt for Visual Studio and run the following commands
	cd C:\\path to MVSC code (to change the directory to this path)
	cl /LD python_bridge.c /I"C:\ProgramData\anaconda3\include" /link /LIBPATH:"C:\ProgramData\anaconda3\libs" python312.lib /OUT:python_bridge.dll  (to compile and build the MVSC code)

Step 4: Move a copy of the python_bridge.dll to same location as the LabWindows executable 

Step 5: Create a python_bridge.h file with all the C function calls that were mapped to the Python modules

Step 5: Add the python_bridge.lib and python_bridge.h files to the LabWindows code

Step 6: Call the MVSC code from the LabWindows code using the MVSC C functions that have been created.


*******************************************************************************/

#include	"PHASMA_C_to_Python_convertor.h"
#include 	<ansi_c.h>
#include	<visa.h>
#include	<stdlib.h>
#include 	<formatio.h>
#include	"PHASMA-MDS-Routines.h"
#include 	"PhotodiodeSystems.h"
#include 	"PhotodiodeSettings.h"  
#include	"GlobalVariables.h"

int			Photodiode_panel;
int			Photodiode_setup_state = 91;

//Set up the T-Daq system using the Python scripts. This function configures and arms the system in one process
int PhotodiodeArm(void)
{

	int 		Photodiode_RecordLength;
	float 		Photodiode_Sample_Rate;
	char 		Photodiode_IP1[64];
	char 		Photodiode_IP2[64];


	
	//First, open panel and get key parameters
	Photodiode_panel = LoadPanel (0, "PhotodiodeSettings.uir", Photodiode);
	RecallPanelState (Photodiode_panel, "Master_Control_Storage_File", Photodiode_setup_state);
	
	//Access the info from the interface
	GetCtrlVal (Photodiode_panel, Photodiode_t_DAQ_IP1,Photodiode_IP1);
	GetCtrlVal (Photodiode_panel, Photodiode_t_DAQ_IP2,Photodiode_IP2);
	
	//Test python code functionality
	WritePrimesToFile();
	
	//Arm the two t_DAC modules for the photodiodes
	t_DAQ_Arm(Photodiode_IP1);
	t_DAQ_Arm(Photodiode_IP1);
	
	//Discard housekeeping panel
	DiscardPanel(Photodiode_panel);
	
	return 1;
}


//Load parameters of the system
void PhotodiodeActivate(void)
{
	int 		Photodiode_RecordLength;
	float 		Photodiode_Sample_Rate;
	char 		Photodiode_IP1[64];
	char 		Photodiode_IP2[64];
	
	//First, open panel and get key parameters
	Photodiode_panel = LoadPanel (0, "PhotodiodeSettings.uir", Photodiode);
	RecallPanelState (Photodiode_panel, "Master_Control_Storage_File", Photodiode_setup_state);
	
	//Access the info from the interface
	GetCtrlVal (Photodiode_panel, Photodiode_t_DAQ_Samples,&Photodiode_RecordLength);
	GetCtrlVal (Photodiode_panel, Photodiode_t_DAQ_Freq,&Photodiode_Sample_Rate);
	GetCtrlVal (Photodiode_panel, Photodiode_t_DAQ_IP1,Photodiode_IP1);
	GetCtrlVal (Photodiode_panel, Photodiode_t_DAQ_IP2,Photodiode_IP2);
	
	//Test python code functionality
	WritePrimesToFile();
	
	//Configure the two t_DAC modules for the photodiodes
	t_DAQ_Config(Photodiode_IP1, Photodiode_Sample_Rate, Photodiode_RecordLength);
	t_DAQ_Config(Photodiode_IP2, Photodiode_Sample_Rate, Photodiode_RecordLength);
}




//Calls the Python function that reads out the data from the T-daq System
//and pushes it to the raw data folder and the MDS database
void Write_PhotodiodeData(void)
{
	int 		Photodiode_RecordLength;
	float 		Photodiode_Sample_Rate;
	char 		Photodiode_IP1[64];
	char 		Photodiode_IP2[64];
	char 		Photodiode_Data_Path[64];
	
	//First, open panel and get key parameters
	Photodiode_panel = LoadPanel (0, "PhotodiodeSettings.uir", Photodiode);
	RecallPanelState (Photodiode_panel, "Master_Control_Storage_File", Photodiode_setup_state);

	GetCtrlVal (Photodiode_panel, Photodiode_t_DAQ_IP1,Photodiode_IP1);
	GetCtrlVal (Photodiode_panel, Photodiode_t_DAQ_IP2,Photodiode_IP2);

	//Discard housekeeping panel
	DiscardPanel(Photodiode_panel);
	
	//Call Python routine to write the data to a raw data file and the MDS tree
	t_DAQ_Data_Read(Photodiode_IP1, RawDataPath);
	t_DAQ_Data_Read(Photodiode_IP2, RawDataPath);

}



//Launch this routine as window opens and it updates the window with the latest values from the 
//control system as it loads.
void OpenPhotodiode_Settings (void)  
{
	//Open settings panel and load saved values
	Photodiode_panel = LoadPanel (0, "PhotodiodeSettings.uir", Photodiode);
	RecallPanelState (Photodiode_panel, "Master_Control_Storage_File", Photodiode_setup_state);
	DisplayPanel(Photodiode_panel);
	
	// Start interacting with user
    RunUserInterface ();

}

int CVICALLBACK Close_Photodiode_Settings (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			//Save current state of panel and close interface
			SavePanelState (Photodiode_panel, "Master_Control_Storage_File", Photodiode_setup_state);
			DiscardPanel(panel);
			break;
	}
	return 0;
}

