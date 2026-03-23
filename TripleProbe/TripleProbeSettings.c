#include <userint.h>
#include <utility.h>
#include	"PHASMA_C_to_Python_convertor.h"
#include 	<ansi_c.h>
#include	<visa.h>
#include	<stdlib.h>
#include 	<formatio.h>
#include	"PHASMA-MDS-Routines.h"
#include 	"TripleProbeSystems.h"
#include 	"TripleProbeSettings.h"
#include	"GlobalVariables.h"
#include 	"TitanMotor.h"

#include    "HelperFunctions.h"

#define STRBUFFSIZE 512U

int TLP_acq2106_config(char* , int , int , int, char *);

int			Triple_panel;
int			Triple_setup_state = 75;
int			Triple_RecordLength=8192;


const char PyShell[]  = "cmd.exe /c ";
const char PyCall[] = "python ";
static char PyEnvPath[STRBUFFSIZE];
static char PyWdir[STRBUFFSIZE];
static char PyScript[STRBUFFSIZE]; 

void TLP_RunPython(char * PyFunc, char ** PyArgs, int numargs)
{
	if(TryPython() != PyScriptFinished) WaitForPython(); // Blocking behavior; if PyHandle in use just wait until available. 
	
	char command[STRBUFFSIZE];
	char func_call[STRBUFFSIZE];
	char format_str[STRBUFFSIZE];
	int k;

	Triple_panel = LoadPanel (0, "TripleProbeSettings.uir", Triple);
	
	GetCtrlVal (Triple_panel, Triple_ACQ2106_ENV_PATH,PyEnvPath);
	GetCtrlVal (Triple_panel, Triple_ACQ2106_WDIR,PyWdir);
	GetCtrlVal (Triple_panel, Triple_ACQ_PY_SCRIPT,PyScript);
	
	snprintf(command, sizeof(command), "%s conda activate %s & cd %s & %s %s %s",  
			PyShell, PyEnvPath, PyWdir, PyCall, PyScript, PyFunc);
	
	//strcat(command,PyArgs[0]);
	
	for (k=0; k<numargs; k++){
		strcat(command," ");
		strcat(command,PyArgs[k]);
		
	}
	
	DiscardPanel(Triple_panel);
	
	if (!LaunchExecutableEx(command, LE_HIDE, &PyHandle))
		{
			//SetCtrlVal (panelHandle, Triple_CONSOLE, command);
		}
    else
		MessagePopup ("ERROR", "Failed to launch executable!");		
}

void TripleProbeActivate(){
	int 		Triple_RecordLength;
	uint32_t 	Triple_Sample_Rate;
	
	char 		Triple_IP1[64];
	char 		Triple_IP2[64];
	
	int inZbool;
	
	char trig_source[16];
	
	Triple_panel = LoadPanel (0, "TripleProbeSettings.uir", Triple);
	//RecallPanelState (Triple_panel, "Master_Control_Storage_File", Triple_setup_state);
	
	//Access the info from the interface
	GetCtrlVal (Triple_panel, Triple_acq2106_1_IP,Triple_IP1);
	GetCtrlVal (Triple_panel, Triple_acq2106_2_IP,Triple_IP2);
	
	GetCtrlVal (Triple_panel, Triple_ACQ2106_SAMPLE_RATE, &Triple_Sample_Rate);
	GetCtrlVal (Triple_panel, Triple_ACQ2106_NUM_SAMPLES ,&Triple_RecordLength );

	GetCtrlVal(Triple_panel, Triple_ACQ2106_TRG_SOURCE, trig_source);
	GetCtrlVal(Triple_panel, Triple_ACQ2106_INPUT_Z, &inZbool);
	
	//push configuration and arm both modules for the Triples
	TLP_acq2106_config(Triple_IP1, Triple_RecordLength, Triple_Sample_Rate, inZbool, trig_source  );
	TLP_acq2106_config(Triple_IP2, Triple_RecordLength, Triple_Sample_Rate, inZbool, trig_source  );

	DiscardPanel(Triple_panel);

		
}

int TripleProbeArm(){
	
	char 		Triple_IP1[64];
	char 		Triple_IP2[64];
	
	Triple_panel = LoadPanel (0, "TripleProbeSettings.uir", Triple);
	GetCtrlVal (Triple_panel, Triple_acq2106_1_IP,Triple_IP1);
	GetCtrlVal (Triple_panel, Triple_acq2106_2_IP,Triple_IP2);
	
	DiscardPanel(Triple_panel);
	
	char* args1[]  = {Triple_IP1};
	char* args2[]  = {Triple_IP2};
	
	TLP_RunPython("arm_transient", args1,1);
	TLP_RunPython("arm_transient", args2,1);
	
	return 1;	// return > 0 to indicate ready
}

//configure a single ac2106
int TLP_acq2106_config( char * IP, int numsamples, int samplerate, int inZbool, char * trig_source)
{
	char SRstr[16] ;
	char numsampstr[16]; 
	int InputImpedance;
	int NChan;
	char Zarg[12] ;
	inZbool ? strcat(Zarg,"50") : strcat(Zarg,"1MEG");
	
	sprintf(SRstr, "%d", samplerate);
	sprintf(numsampstr, "%d", numsamples);
	char * args[] = {IP,numsampstr,SRstr,Zarg,trig_source};

	TLP_RunPython("config_master", args,5);
	
	return 0;
}

//read data from both acq2016 devices; save to local 
void Write_TripleProbeData(void)
{
	int 		nchan;
	int         nsamples;
	int 	    Triple_RecordLength;
	int 		Triple_Sample_Rate;
	char 		Triple_IP1[64];
	char 		Triple_IP2[64];
	char 		Triple_Data_Path[64];
	char 		nsampstr[64];
	char 		nchannstr[64];
	char 		acq1nameStr[64];
	char 		acq2nameStr[64];
	
	Triple_panel = LoadPanel (0, "TripleProbeSettings.uir", Triple);
	//RecallPanelState (Triple_panel, "Master_Control_Storage_File", Triple_setup_state);

	GetCtrlVal (Triple_panel, Triple_acq2106_1_IP,Triple_IP1);
	GetCtrlVal (Triple_panel, Triple_acq2106_2_IP,Triple_IP2);	
	GetCtrlVal (Triple_panel, Triple_ACQ2106_NCHANNELS, &nchan);		
	GetCtrlVal (Triple_panel, Triple_ACQ2106_NUM_SAMPLES, &nsamples);
	GetCtrlVal (Triple_panel, Triple_ACQ2106_ID1, acq1nameStr);
	GetCtrlVal (Triple_panel, Triple_ACQ2106_ID2, acq2nameStr);

	DiscardPanel(Triple_panel);
	
	sprintf(nchannstr,"%d",nchan);
	sprintf(nsampstr, "%d", nsamples);

	char * args1[] = {Triple_IP1, nchannstr, nsampstr, ShotNumberString ,RawDataPath,acq1nameStr, "TLP"};
	char * args2[] = {Triple_IP2, nchannstr, nsampstr,ShotNumberString ,RawDataPath,acq2nameStr, "TLP"};
	
	TLP_RunPython("readout_store",args1,7); 
	TLP_RunPython("readout_store",args2,7);
}

int CVICALLBACK Close_Triple_Settings (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			//Save current state of panel and close interface
			SavePanelState (Triple_panel, "Master_Control_Storage_File", Triple_setup_state); 
			
			DiscardPanel(panel);
			break;
	}
	return 0;
}

//Launch this routine as window opens and it updates the window with the latest values from the 
//control system as it loads.
void OpenTripleProbe_Settings (void)  
{
	//Open settings panel and load saved values
	Triple_panel = LoadPanel (0, "TripleProbeSettings.uir", Triple);
	//RecallPanelState (Triple_panel, "Master_Control_Storage_File", Triple_setup_state);
	DisplayPanel(Triple_panel);
	
	// Start interacting with user
    RunUserInterface ();

}

int CVICALLBACK TLP_acq2106_set_abort (int panel, int control, int event,
								   void *callbackData, int eventData1, int eventData2)
{
	
	char 		Triple_IP1[64];
	char 		Triple_IP2[64];
	char * arg1[] = {Triple_IP1};
	char * arg2[] = {Triple_IP2};
	//Triple_panel = LoadPanel (0, "TripleProbeSettings.uir", Triple);

	GetCtrlVal (panel, Triple_acq2106_1_IP,Triple_IP1);
	GetCtrlVal (panel, Triple_acq2106_2_IP,Triple_IP2);	
	//DiscardPanel(Triple_panel);
	
	switch (event)
	{
		case EVENT_COMMIT:
			TLP_RunPython("set_abort",arg1,1);
			TLP_RunPython("set_abort",arg2,1);
			break;
	}
	return 0;
}

int CVICALLBACK TLP_acq2106_manual_arm (int panel, int control, int event,
								   void *callbackData, int eventData1, int eventData2)
{
	
	char 		Triple_IP1[64];
	char 		Triple_IP2[64];
	char * arg1[] = {Triple_IP1};
	char * arg2[] = {Triple_IP2};
	//Triple_panel = LoadPanel (0, "TripleProbeSettings.uir", Triple);

	GetCtrlVal (panel, Triple_acq2106_1_IP,Triple_IP1);
	GetCtrlVal (panel, Triple_acq2106_2_IP,Triple_IP2);	
	
	
	switch (event)
	{
		case EVENT_COMMIT:
			TLP_RunPython("set_arm",arg1,1);
			TLP_RunPython("set_arm",arg2,1);
			break;
	}
	return 0;
}

int CVICALLBACK TLP_acq2106_soft_trig (int panel, int control, int event,
								   void *callbackData, int eventData1, int eventData2)
{
	
	char 		Triple_IP1[64];
	char 		Triple_IP2[64];
	char * arg1[] = {Triple_IP1};
	char * arg2[] = {Triple_IP2};
	//Triple_panel = LoadPanel (0, "TripleProbeSettings.uir", Triple);

	GetCtrlVal (panel, Triple_acq2106_1_IP,Triple_IP1);
	GetCtrlVal (panel, Triple_acq2106_2_IP,Triple_IP2);	
	//DiscardPanel(Triple_panel);
	
	switch (event)
	{
		case EVENT_COMMIT:
			TLP_RunPython("set_trig",arg1,1);
			TLP_RunPython("set_trig",arg2,1);
			break;
	}
	return 0;
}

int CVICALLBACK TLP_acq2106_pull_data (int panel, int control, int event,
								   void *callbackData, int eventData1, int eventData2)
{	
	switch (event)
	{
		case EVENT_COMMIT:
			Write_TripleProbeData();
			
			break;
	}
	return 0;
}

int CVICALLBACK TLP_acq2106_push_config (int panel, int control, int event,
									 void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
				TripleProbeActivate();
			break;
	}
	return 0;
}

int CVICALLBACK TLP_ACQ2106_TRG_DELAY_SET (int panel, int control, int event,
									   void *callbackData, int eventData1, int eventData2)
{
	int 		delay_us;
	char 		Triple_IP1[64];
	char 		Triple_IP2[64];
	char 		delaystr[16];
	
	
	GetCtrlVal (panel, Triple_acq2106_1_IP,Triple_IP1);
	GetCtrlVal (panel, Triple_acq2106_2_IP,Triple_IP2);	
	GetCtrlVal (panel, Triple_ACQ2106_TRG_DELAY, &delay_us);
	sprintf(delaystr,"%d",delay_us);
	char * arg1[] = {Triple_IP1,delaystr};
	char * arg2[] = {Triple_IP2,delaystr};

	switch (event)
	{
		case EVENT_COMMIT:
				TLP_RunPython("set_delay",arg1,2);
				TLP_RunPython("set_delay",arg2,2);	
			break;
	}
	return 0;
}



int CVICALLBACK TripleProbe_Move_to_Start (int panel, int control, int event,
							  void *callbackData, int eventData1, int eventData2)
{

	float 	TripleProbe_RStart;
	float 	TripleProbe_Max_Insertion;
	float 	TripleProbe_COUNTSPERMM;
	float 	TripleProbe_Offset;
	float 	TripleProbe_RPosition;
	char 	TripleProbe_Probe_IP_Address[64];

	switch (event)
	{
		case EVENT_COMMIT:
			//Get current position of probe, starting position, end position, and what to do at end of scan range
			GetCtrlVal (Triple_panel, Triple_R_Stop,&TripleProbe_RStart);
			GetCtrlVal (Triple_panel, Triple_Probe_IP,TripleProbe_Probe_IP_Address);
			GetCtrlVal (Triple_panel, Triple_Probe_Max_Insertion,&TripleProbe_Max_Insertion);
			GetCtrlVal (Triple_panel, Triple_Probe_COUNTSPERMM,&TripleProbe_COUNTSPERMM);
			GetCtrlVal (Triple_panel, Triple_Probe_Offset,&TripleProbe_Offset);
			GetCtrlVal (Triple_panel, Triple_R_Position,&TripleProbe_RPosition);
			
			//Send move command to probe motor
			TitanMotorMove(TripleProbe_Probe_IP_Address, (TripleProbe_Offset-TripleProbe_RPosition)*10.0, (TripleProbe_Offset-TripleProbe_RStart)*10.0, TripleProbe_COUNTSPERMM);		//Convert distances to mm from cm
			
			//Set current position to Start
			SetCtrlVal (Triple_panel, Triple_R_Position,TripleProbe_RStart);

			break;
	}
	return 0;
}


int CVICALLBACK TripleProbe_Move_to_Stop (int panel, int control, int event,
							  void *callbackData, int eventData1, int eventData2)
{
	float 	TripleProbe_RStop;
	float 	TripleProbe_Max_Insertion;
	float 	TripleProbe_COUNTSPERMM;
	float 	TripleProbe_Offset;
	float 	TripleProbe_RPosition;
	char 	TripleProbe_Probe_IP_Address[64];

	switch (event)
	{
		case EVENT_COMMIT:
			//Get current position of probe, starting position, end position, and what to do at end of scan range
			GetCtrlVal (Triple_panel, Triple_R_Stop,&TripleProbe_RStop);
			GetCtrlVal (Triple_panel, Triple_Probe_IP,TripleProbe_Probe_IP_Address);
			GetCtrlVal (Triple_panel, Triple_Probe_Max_Insertion,&TripleProbe_Max_Insertion);
			GetCtrlVal (Triple_panel, Triple_Probe_COUNTSPERMM,&TripleProbe_COUNTSPERMM);
			GetCtrlVal (Triple_panel, Triple_Probe_Offset,&TripleProbe_Offset);
			GetCtrlVal (Triple_panel, Triple_R_Position,&TripleProbe_RPosition);
			
			//Send move command to probe motor
			TitanMotorMove(TripleProbe_Probe_IP_Address, (TripleProbe_Offset-TripleProbe_RPosition)*10.0, (TripleProbe_Offset-TripleProbe_RStop)*10.0, TripleProbe_COUNTSPERMM);		//Convert distances to mm from cm
			
			//Set current position to Start
			SetCtrlVal (Triple_panel, Triple_R_Position,TripleProbe_RStop);

			break;
	}
	return 0;
}

int CVICALLBACK TripleProbe_Home (int panel, int control, int event,
							  void *callbackData, int eventData1, int eventData2)
{

	char 	TripleProbe_Probe_IP_Address[64];
	float 	TripleProbe_Offset;
	
	switch (event)
	{
		case EVENT_COMMIT:
			//Get current position of probe, starting position, end position, and what to do at end of scan range
			GetCtrlVal (Triple_panel, Triple_Probe_IP,TripleProbe_Probe_IP_Address);
			GetCtrlVal (Triple_panel, Triple_Probe_Offset,&TripleProbe_Offset);
			
			//Send move command to probe motor
			TitanMotorHome(TripleProbe_Probe_IP_Address);		//Convert distances to mm from cm
			
			//Set current position to Offset
			SetCtrlVal (Triple_panel, Triple_R_Position,TripleProbe_Offset);

			break;
	}
	return 0;
}

float Advance_TripleProbe_Probe(void)
{
	int	 	Probe_Return_Flag;
	int 	Probe_Active_Move_Flag;
	float 	Triple_RStart;
	float 	Triple_RStop;
	float 	Triple_RIncrement;
	float 	Triple_Offset;
	float 	Triple_Return;
	float 	Current_Position;
	float 	Probe_Move;
	float 	Triple_Max_Insertion;
	float 	Triple_COUNTSPERMM;
	char 	Triple_Probe_IP_Address[64];
	
	//Open settings panel and load saved values
	Triple_panel = LoadPanel (0, "TripleProbeSettings.uir", Triple);
	RecallPanelState (Triple_panel, "Master_Control_Storage_File", Triple_setup_state);
	
	//Get current position of probe, starting position, end position, and what to do at end of scan range
	GetCtrlVal (Triple_panel, Triple_R_Start,&Triple_RStart);
	GetCtrlVal (Triple_panel, Triple_R_Stop,&Triple_RStop);
	GetCtrlVal (Triple_panel, Triple_R_Increment,&Triple_RIncrement);
	GetCtrlVal (Triple_panel, Triple_Probe_Offset,&Triple_Offset);
	GetCtrlVal (Triple_panel, Triple_Probe_Return,&Triple_Return);
	GetCtrlVal (Triple_panel, Triple_R_Position,&Current_Position);
	GetCtrlVal (Triple_panel, Triple_Probe_Return,&Probe_Return_Flag);
	GetCtrlVal (Triple_panel, Triple_Probe_Active_Move,&Probe_Active_Move_Flag);
	GetCtrlVal (Triple_panel, Triple_Probe_IP,Triple_Probe_IP_Address);
	GetCtrlVal (Triple_panel, Triple_Probe_Max_Insertion,&Triple_Max_Insertion);
	GetCtrlVal (Triple_panel, Triple_Probe_COUNTSPERMM,&Triple_COUNTSPERMM);

	if (Probe_Active_Move_Flag) {
		
		//Get current position from motor controller
	
		//Calculate distance to move to next position
		Probe_Move=Current_Position+Triple_RIncrement;
		
		//If next position is beyond the stop position, either do nothing or send 
		//probe back to start position depending on settings in interface window
		if ( (Triple_RIncrement + Current_Position) > Triple_RStop) {
			return Current_Position;
		} else {
			//Command motor to move to next position	
			TitanMotorMove(Triple_Probe_IP_Address, (Triple_Offset-Probe_Move)*10.0, (Triple_Offset-Current_Position)*10.0, Triple_COUNTSPERMM);		//Convert distances to mm from cm
		}
		
		//Update window with current position and return current position to master code
		SetCtrlVal (Triple_panel, Triple_R_Position,(Triple_RIncrement + Current_Position));
		SavePanelState (Triple_panel, "Master_Control_Storage_File", Triple_setup_state);
		return Triple_RIncrement + Current_Position;
	} else {
		SavePanelState (Triple_panel, "Master_Control_Storage_File", Triple_setup_state);
		return Current_Position;
	}


}


