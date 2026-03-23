// Controls 96-channel photodiode system (2x ACQ2106 48channel)

#include <userint.h>
#include <utility.h>
#include	"PHASMA_C_to_Python_convertor.h"
#include 	<ansi_c.h>
#include	<visa.h>
#include	<stdlib.h>
#include 	<formatio.h>
#include	"PHASMA-MDS-Routines.h"
#include 	"PhotodiodeSystems.h"
#include 	"PhotodiodeSettings.h"  
#include	"GlobalVariables.h"
#include    "HelperFunctions.h"

#define STRBUFFSIZE 512U

int PD_recall_from_master_conf = 0; // whether to use config settings in the master_control file or stick with what's currently in the UI

int acq2106_config(char* , int , int , int, char *);

int			Photodiode_setup_state = 91;
int 	    Photodiode_panel;


const char PD_PyShell[]  = "cmd.exe /c ";
const char PD_PyCall[] = "python ";
static char PD_PyEnvPath[STRBUFFSIZE];
static char PD_PyWdir[STRBUFFSIZE];
static char PD_PyScript[STRBUFFSIZE]; 


int panelHandle = Photodiode;

void PDRunPython(char * PyFunc, char ** PyArgs, int numargs)
{
	if(TryPython() != PyScriptFinished) WaitForPython(); // Blocking behavior; if PyHandle in use just wait until available. 
	
	char command[STRBUFFSIZE];
	int k;

	Photodiode_panel = LoadPanel (0, "PhotodiodeSettings.uir", Photodiode);
	
	GetCtrlVal (Photodiode_panel, Photodiode_ACQ2106_ENV_PATH,PD_PyEnvPath);
	GetCtrlVal (Photodiode_panel, Photodiode_ACQ2106_WDIR,PD_PyWdir);
	GetCtrlVal (Photodiode_panel, Photodiode_ACQ_PY_SCRIPT,PD_PyScript);
	
	snprintf(command, sizeof(command), "%s conda activate %s & cd %s & %s %s %s",  
			PD_PyShell, PD_PyEnvPath, PD_PyWdir, PD_PyCall, PD_PyScript, PyFunc);

	for (k=0; k<numargs; k++){
		strcat(command," ");
		strcat(command,PyArgs[k]);
		
	}
	
	DiscardPanel(Photodiode_panel);
	
	if (!LaunchExecutableEx(command, LE_HIDE, &PyHandle))
		{
			//SetCtrlVal (panelHandle, Photodiode_CONSOLE, command); // TODO: should have a plaintext readout in the main screen to display output from devices, external scripts, etc
		}
    else
		MessagePopup ("ERROR", "Failed to launch executable!");		
}

int PDTryPython()
{	
	switch (ExecutableHasTerminated(PyHandle) )
	{
		case PyScriptFinished:	
			return 0; // Ready to go
		case PyScriptNotFinished:
			return 1; // Python's still occupied
	}
	return -1; // Something weird happened
}


int PhotodiodeActivate(){
	int 		Photodiode_RecordLength;
	uint32_t 	Photodiode_Sample_Rate;
	
	char 		Photodiode_IP1[64];
	char 		Photodiode_IP2[64];
	
	int inZbool;
	
	char trig_source[16];
	
	Photodiode_panel = LoadPanel (0, "PhotodiodeSettings.uir", Photodiode);
	if (PD_recall_from_master_conf) RecallPanelState (Photodiode_panel, "Master_Control_Storage_File", Photodiode_setup_state);
	
	//Access the info from the interface
	GetCtrlVal (Photodiode_panel, Photodiode_acq2106_1_IP,Photodiode_IP1);
	GetCtrlVal (Photodiode_panel, Photodiode_acq2106_2_IP,Photodiode_IP2);
	
	GetCtrlVal (Photodiode_panel, Photodiode_ACQ2106_SAMPLE_RATE, &Photodiode_Sample_Rate);
	GetCtrlVal (Photodiode_panel, Photodiode_ACQ2106_NUM_SAMPLES ,&Photodiode_RecordLength );

	GetCtrlVal(Photodiode_panel, Photodiode_ACQ2106_TRG_SOURCE, trig_source);
	GetCtrlVal(Photodiode_panel, Photodiode_ACQ2106_INPUT_Z, &inZbool);
	
	//push configuration and arm both modules for the photodiodes
	acq2106_config(Photodiode_IP1, Photodiode_RecordLength, Photodiode_Sample_Rate, inZbool, trig_source  );
	acq2106_config(Photodiode_IP2, Photodiode_RecordLength, Photodiode_Sample_Rate, inZbool, trig_source  );

	DiscardPanel(Photodiode_panel);
	
	return 0;
		
}

int PhotodiodeArm(){
	
	char 		Photodiode_IP1[64];
	char 		Photodiode_IP2[64];
	
	Photodiode_panel = LoadPanel (0, "PhotodiodeSettings.uir", Photodiode);
	GetCtrlVal (Photodiode_panel, Photodiode_acq2106_1_IP,Photodiode_IP1);
	GetCtrlVal (Photodiode_panel, Photodiode_acq2106_2_IP,Photodiode_IP2);
	
	DiscardPanel(Photodiode_panel);
	
	char* args1[]  = {Photodiode_IP1};
	char* args2[]  = {Photodiode_IP2};
	
	PDRunPython("arm_transient", args1,1);
	PDRunPython("arm_transient", args2,1);
	
	return 1;	// return > 0 to indicate ready
}

//push configuration settings from UI to a single ac2106
int acq2106_config( char * IP, int numsamples, int samplerate, int inZbool, char * trig_source)
{
	char SRstr[16] ;
	char numsampstr[16]; 
	int InputImpedance;
	char Zarg[12] ;
	inZbool ? strcat(Zarg,"50") : strcat(Zarg,"1MEG");
	
	sprintf(SRstr, "%d", samplerate);
	sprintf(numsampstr, "%d", numsamples);
	char * args[] = {IP,numsampstr,SRstr,Zarg,trig_source};

	PDRunPython("config_master", args,5);
	
	return 0;
}

//read data from both acq2016 devices; save to local 
void Write_PhotodiodeData(void)
{
	int 		nchan;
	int         nsamples;
	int 	    Photodiode_RecordLength;
	int 		Photodiode_Sample_Rate;
	char 		Photodiode_IP1[64];
	char 		Photodiode_IP2[64];
	char 		Photodiode_Data_Path[64];
	char 		nsampstr[64];
	char 		nchannstr[64];
	char 		acq1nameStr[64];
	char 		acq2nameStr[64];
	
	Photodiode_panel = LoadPanel (0, "PhotodiodeSettings.uir", Photodiode);
	if (PD_recall_from_master_conf) RecallPanelState (Photodiode_panel, "Master_Control_Storage_File", Photodiode_setup_state);

	GetCtrlVal (Photodiode_panel, Photodiode_acq2106_1_IP,Photodiode_IP1);
	GetCtrlVal (Photodiode_panel, Photodiode_acq2106_2_IP,Photodiode_IP2);	
	GetCtrlVal (Photodiode_panel, Photodiode_ACQ2106_NCHANNELS, &nchan);		
	GetCtrlVal (Photodiode_panel, Photodiode_ACQ2106_NUM_SAMPLES, &nsamples);
	GetCtrlVal (Photodiode_panel, Photodiode_ACQ2106_ID1, acq1nameStr);
	GetCtrlVal (Photodiode_panel, Photodiode_ACQ2106_ID2, acq2nameStr);

	DiscardPanel(Photodiode_panel);
	
	sprintf(nchannstr,"%d",nchan);
	sprintf(nsampstr, "%d", nsamples);

	char * args1[] = {Photodiode_IP1, nchannstr, nsampstr, ShotNumberString ,RawDataPath,acq1nameStr, "TOMOGRAPHY"};
	char * args2[] = {Photodiode_IP2, nchannstr, nsampstr,ShotNumberString ,RawDataPath,acq2nameStr, "TOMOGRAPHY"};
	
	PDRunPython("readout_store",args1,7); 
	PDRunPython("readout_store",args2,7);
}


//Launch this routine as window opens and it updates the window with the latest values from the 
//control system as it loads.
void OpenPhotodiode_Settings (void)  
{
	//Open settings panel and load saved values
	Photodiode_panel = LoadPanel (0, "PhotodiodeSettings.uir", Photodiode);
	
	if (PD_recall_from_master_conf) RecallPanelState (Photodiode_panel, "Master_Control_Storage_File", Photodiode_setup_state);
	
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


int CVICALLBACK PyConsoleUpdate (int panel, int control, int event,
								 void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:

			break;
	}
	return 0;
}

int CVICALLBACK acq2106_set_abort (int panel, int control, int event,
								   void *callbackData, int eventData1, int eventData2)
{
	
	char 		Photodiode_IP1[64];
	char 		Photodiode_IP2[64];
	char * arg1[] = {Photodiode_IP1};
	char * arg2[] = {Photodiode_IP2};
	//Photodiode_panel = LoadPanel (0, "PhotodiodeSettings.uir", Photodiode);

	GetCtrlVal (panel, Photodiode_acq2106_1_IP,Photodiode_IP1);
	GetCtrlVal (panel, Photodiode_acq2106_2_IP,Photodiode_IP2);	
	//DiscardPanel(Photodiode_panel);
	
	switch (event)
	{
		case EVENT_COMMIT:
			PDRunPython("set_abort",arg1,1);
			PDRunPython("set_abort",arg2,1);
			break;
	}
	return 0;
}

int CVICALLBACK acq2106_manual_arm (int panel, int control, int event,
								   void *callbackData, int eventData1, int eventData2)
{
	
	char 		Photodiode_IP1[64];
	char 		Photodiode_IP2[64];
	char * arg1[] = {Photodiode_IP1};
	char * arg2[] = {Photodiode_IP2};
	//Photodiode_panel = LoadPanel (0, "PhotodiodeSettings.uir", Photodiode);

	GetCtrlVal (panel, Photodiode_acq2106_1_IP,Photodiode_IP1);
	GetCtrlVal (panel, Photodiode_acq2106_2_IP,Photodiode_IP2);	
	
	
	switch (event)
	{
		case EVENT_COMMIT:
			PDRunPython("set_arm",arg1,1);
			PDRunPython("set_arm",arg2,1);
			break;
	}
	return 0;
}

int CVICALLBACK acq2106_soft_trig (int panel, int control, int event,
								   void *callbackData, int eventData1, int eventData2)
{
	
	char 		Photodiode_IP1[64];
	char 		Photodiode_IP2[64];
	char * arg1[] = {Photodiode_IP1};
	char * arg2[] = {Photodiode_IP2};
	//Photodiode_panel = LoadPanel (0, "PhotodiodeSettings.uir", Photodiode);

	GetCtrlVal (panel, Photodiode_acq2106_1_IP,Photodiode_IP1);
	GetCtrlVal (panel, Photodiode_acq2106_2_IP,Photodiode_IP2);	
	//DiscardPanel(Photodiode_panel);
	
	switch (event)
	{
		case EVENT_COMMIT:
			PDRunPython("set_trig",arg1,1);
			PDRunPython("set_trig",arg2,1);
			break;
	}
	return 0;
}

int CVICALLBACK acq2106_pull_data (int panel, int control, int event,
								   void *callbackData, int eventData1, int eventData2)
{	
	switch (event)
	{
		case EVENT_COMMIT:
			Write_PhotodiodeData();
			
			break;
	}
	return 0;
}

int CVICALLBACK acq2106_push_config (int panel, int control, int event,
									 void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
				PhotodiodeActivate();
			break;
	}
	return 0;
}

int CVICALLBACK PD_ACQ2106_TRG_DELAY_SET (int panel, int control, int event,
									   void *callbackData, int eventData1, int eventData2)
{
	int 		delay_us;
	char 		Photodiode_IP1[64];
	char 		Photodiode_IP2[64];
	char 		delaystr[16];
	
	GetCtrlVal (panel, Photodiode_acq2106_1_IP,Photodiode_IP1);
	GetCtrlVal (panel, Photodiode_acq2106_2_IP,Photodiode_IP2);	
	GetCtrlVal (panel, Photodiode_ACQ2106_TRG_DELAY, &delay_us);
	sprintf(delaystr,"%d",delay_us);
	char * arg1[] = {Photodiode_IP1,delaystr};
	char * arg2[] = {Photodiode_IP2,delaystr};

	switch (event)
	{
		case EVENT_COMMIT:
				PDRunPython("set_delay",arg1,2);
				PDRunPython("set_delay",arg2,2);	
			break;
	}
	return 0;
}
