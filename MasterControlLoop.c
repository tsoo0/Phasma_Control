/*  PHASMA DAQ System Control Software
	
	Development started Jan 21, 2025
	by Earl Scime
	
	This program will set up all the instruments in the PHASMA lab, arm them, and acquire data from them
	after they receive a trigger from the master trigger system. The VXI components of the data system are
	hosted on a seperate computer and the instruments accessed through a VISA forwarding process. 
	
	
	*/

//General Include Statements
#include 	<windows.h>
#include 	<visa.h>
#include 	<ansi_c.h>
#include 	<utility.h>
#include 	<userint.h>
#include 	<stdio.h>   
#include 	<stdlib.h>
#include 	<string.h>
#include	"GlobalVariables.h"


//Include all the definitions from all the subpanel user interfaces
#include "MasterControlPanel.h"
#include "MasterControlSettings.h"

#include "MagField_Settings.h" 
#include "MagField_Functions.h"

#include "HeliconSourceSettings.h"
#include "Helicon_Settings.h"

#include "HousekeepingSettings.h"
#include "HousekeepingSystems.h"

#include "MasterTriggerSettings.h"
#include "MasterTriggerSystem.h"

#include "PlasmaGunSettings.h"
#include "PlasmaGunSystem.h"

#include "BdotSettings.h"
#include "BdotSystems.h"

#include "RFEASettings.h"
#include "RFEASystems.h"

#include "TempDiagSettings.h"
#include "TempDiagSystems.h"

#include "ContLIFSettings.h"  
#include "ContLIFSystems.h"

#include "LangmuirSettings.h"  
#include "LangmuirSystems.h"

#include "DoubleProbeSettings.h"  
#include "DoubleProbeSystems.h"

#include "PhotronCameraSettings.h"  
#include "PhotronCameraSystems.h"

#include "ThomsonScatteringSettings.h"  
#include "ThomsonScatteringSystems.h"

#include "PulsedLIFSettings.h"  
#include "PulsedLIFSystems.h"

#include "PhotodiodeSettings.h"  
#include "PhotodiodeSystems.h"

#include "TripleProbeSettings.h"  
#include "TripleProbeSystems.h"

#include "McPhersonSettings.h"  
#include "McPhersonSystems.h"

#include "OceanOpticsSettings.h"  
#include "OceanOpticsSystems.h"

#include "PHASMA-MDS-Routines.h"


//Include the MDS libraries
//#include <mdsobjects.h>
#include <mdslib.h>

//#include <mdsshr.h>

#define	Housekeeping_max_record_length 262144
	
//Variables for operating the Master Control Panel
int		Master_Control_Panel;
int		Master_Control_Panel_setup_state = 100; 	

//Flags that determine if a subsystem is active
int		Interval=30; 
int		Countdown=30;
int		CycleRepeat;
int		Cycle_Count;
int 	Auto_Advance_Probes_Flag=0;
int 	Countdown_Flag=0;
int		MagFieldFlag=0;
int		HeliconFlag=0;
int		HousekeepingFlag=0;
int		PlasmaGunFlag=0;
int		Write_PlasmaGun=0;
int		BdotFlag=0;
int		TripleProbeFlag=0;
int		DoubleProbeFlag=0;
int		TSFlag=0;
int		PulsedLIFFlag=0;
int		QuantumBeatFlag=0;
int		LangmuirProbeFlag=0;
int		PhotodiodeFlag=0;
int		PhotronCameraFlag=0;
int		OceanOpticsFlag=0;
int		McPherson209Flag=0;
int		LightwvIntferomFlag=0;
int		RFEAFlag=0;
int		TemporaryDiagFlag=0;
int 	MasterTriggerFlag=0;
int 	MasterSimmerFlag=0;
float 	master_trigger_start;

static int trigger_hours;
static int trigger_minutes;
static int trigger_seconds;

static int post_storage_hours;
static int post_storage_minutes;
static int post_storage_seconds;




/*********************************************************************************************************
*	This function loops through all the diagnostics and writes their data to the raw data txt files and
*	pushes them to the MDS database.
*
*	 TR 1/16/26: 
		This function will store data locally but the MDS storage logic is not located here; that's done in a seperate Python script 
        located at C:\PHASMA 2025 DAQ\PHASMA Python Routines\PHASMA_C_to_Python_convertor_Project\PhasmaMDS\mds_push_all.py
*********************************************************************************************************/
void	Store_Data (void)
{
	//Always store timing parameters
	//Write_Timing();
	
	//Go through diagnostics and store data
	if (MagFieldDataFlag) 		Write_MagField();
	if (HeliconDataFlag) 		Write_HeliconSettings();
	if (BdotDataFlag) 			Write_BdotData();
	if (PlasmaGunDataFlag) 		Write_PlasmaGunData();
	if (HousekeepingDataFlag) 	Write_HousekeepingData();
	if (TemporaryDiagDataFlag) 	Write_TempDiagData();
	if (RFEADataFlag) 			Write_RFEAData();
	if (DoubleProbeDataFlag) 	Write_DoubleProbeData(); 
	if (PhotronCameraDataFlag) 	Write_PhotronCameraData();   
	if (TSDataFlag) 			Write_ThomsonScatteringData();
	if (PulsedLIFDataFlag) 		Write_PulsedLIFData();
	if (PhotodiodeDataFlag) 	Write_PhotodiodeData();
	if (TripleProbeDataFlag) 	Write_TripleProbeData();
	if (McPherson209DataFlag) 	Write_McPherson209Data();
	
	/*
	if (QuantumBeatDataFlag) 	Write_QuantumBeatData();
	if (OceanOpticsDataFlag) 	Write_OceanOpticsData();
	if (LightwvIntferomDataFlag)Write_LightwvIntferomData() ;
	*/
	
	return;
}


/*********************************************************************************************************
*	This function arms all active subsystems but the laser delay generators
*
*
*********************************************************************************************************/
 void MasterArm(void)
{
	int		result;
	
	//Trip the individual arming lights
	if((HousekeepingFlag) && (HousekeepingArm()>0))   	SetCtrlVal (Master_Control_Panel, MasterCont_Housekeeping_Arm, 1);
	if((BdotFlag) && (BdotArm()>0))   					SetCtrlVal (Master_Control_Panel, MasterCont_Bdot_Arm, 1);
 	if((DoubleProbeFlag) && (DoubleProbeArm()>0))   	SetCtrlVal (Master_Control_Panel, MasterCont_DoubleProbe_Arm, 1);
 	if((PhotronCameraFlag) && (PhotronCameraArm()>0))   SetCtrlVal (Master_Control_Panel, MasterCont_PhotronCamera_Arm, 1);
 	if((RFEAFlag) && (RFEAArm()>0))   					SetCtrlVal (Master_Control_Panel, MasterCont_RFEA_Arm, 1);
	if((TemporaryDiagFlag) && (TempDiagArm()>0))   		SetCtrlVal (Master_Control_Panel, MasterCont_TempDiag_Arm, 1);
	if((PhotodiodeFlag) && (PhotodiodeArm()>0))   		SetCtrlVal (Master_Control_Panel, MasterCont_Photodiode_Arm, 1);
	if((TripleProbeFlag) && (TripleProbeArm()>0))   	SetCtrlVal (Master_Control_Panel, MasterCont_TripleProbe_Arm, 1);
	if((McPherson209Flag) && (McPherson209Arm()>0))   	SetCtrlVal (Master_Control_Panel, MasterCont_McPherson209_Arm, 1);

}
 
 
/*********************************************************************************************************
*	This function arms all active laser delay generators
*
*
*********************************************************************************************************/
 void LaserMasterArm(void)
{
	int		result;
	
	//The laser control is updates in each of the laser arming functions	
	
	//Trip the individual arming lights
	if((TSFlag) && (ThomsonScatteringArm()>0))   		SetCtrlVal (Master_Control_Panel, MasterCont_TS_Arm, 1);
	if((PulsedLIFFlag) && (PulsedLIFArm()>0))  {
		//Update current wavelength value on panel
		SetCtrlVal (Master_Control_Panel, MasterCont_PLIF_Laser_Wavelength, Get_PLIF_Wavelength());
		SetCtrlVal (Master_Control_Panel, MasterCont_PulsedLIF_Arm, 1);
	}
	
}

 
/*********************************************************************************************************
*	This function resets all activation and data flag switches to off when code boots up for safety
*
*
*********************************************************************************************************/
 void ClearSystemStatus(void)
{
	int		result;
	

	//Reset the individual activation switches
	SetCtrlVal (Master_Control_Panel, MasterCont_Housekeeping_Status, 0);
	SetCtrlVal (Master_Control_Panel, MasterCont_Bdot_Status, 0);
	SetCtrlVal (Master_Control_Panel, MasterCont_MagField_Status, 0);
	SetCtrlVal (Master_Control_Panel, MasterCont_TempDiag_Status, 0);
	SetCtrlVal (Master_Control_Panel, MasterCont_DoubleProbe_Status, 0);
	SetCtrlVal (Master_Control_Panel, MasterCont_PhotronCamera_Status, 0);
	SetCtrlVal (Master_Control_Panel, MasterCont_RFEA_Status, 0);
	SetCtrlVal (Master_Control_Panel, MasterCont_TS_Status, 0);
	SetCtrlVal (Master_Control_Panel, MasterCont_PulsedLIF_Status, 0);
	SetCtrlVal (Master_Control_Panel, MasterCont_Photodiode_Status, 0);
	SetCtrlVal (Master_Control_Panel, MasterCont_TripleProbe_Status, 0);
	SetCtrlVal (Master_Control_Panel, MasterCont_McPherson209_Status, 0);
	SetCtrlVal (Master_Control_Panel, MasterCont_MasterTrigger_Status, 0);
	SetCtrlVal (Master_Control_Panel, MasterCont_PlasmaGun_Status, 0);
	
	//Reset the individual data flag switchs
	SetCtrlVal (Master_Control_Panel, MasterCont_Housekeeping_Data, 0);
	SetCtrlVal (Master_Control_Panel, MasterCont_MagField_Data, 0);
	SetCtrlVal (Master_Control_Panel, MasterCont_MagProbe_Data, 0);
	SetCtrlVal (Master_Control_Panel, MasterCont_TempDiag_Data, 0);
	SetCtrlVal (Master_Control_Panel, MasterCont_DoubleProbe_Data, 0);
	SetCtrlVal (Master_Control_Panel, MasterCont_PhotronCamera_Data, 0);
	SetCtrlVal (Master_Control_Panel, MasterCont_RFEA_Data, 0);
	SetCtrlVal (Master_Control_Panel, MasterCont_TS_Data, 0);
	SetCtrlVal (Master_Control_Panel, MasterCont_PulsedLIF_Data, 0);
	SetCtrlVal (Master_Control_Panel, MasterCont_Photodiode_Data, 0);
	SetCtrlVal (Master_Control_Panel, MasterCont_TripleProbe_Data, 0);
	SetCtrlVal (Master_Control_Panel, MasterCont_McPherson209_Data, 0);
	
}

  
/*********************************************************************************************************
*	This function clears all transient arm state lights
*
*
*********************************************************************************************************/
 void ClearMasterArm(void)
{
	//Reset the individual arming lights and auto advance features
	SetCtrlVal (Master_Control_Panel, MasterCont_Housekeeping_Arm, 0);
	SetCtrlVal (Master_Control_Panel, MasterCont_Bdot_Arm, 0);
	SetCtrlVal (Master_Control_Panel, MasterCont_TempDiag_Arm, 0);
	SetCtrlVal (Master_Control_Panel, MasterCont_DoubleProbe_Arm, 0);
	SetCtrlVal (Master_Control_Panel, MasterCont_PhotronCamera_Arm, 0);
	SetCtrlVal (Master_Control_Panel, MasterCont_RFEA_Arm, 0);
	SetCtrlVal (Master_Control_Panel, MasterCont_ContinLIF_Arm, 0);
	SetCtrlVal (Master_Control_Panel, MasterCont_Lang_Arm, 0);
	SetCtrlVal (Master_Control_Panel, MasterCont_TS_Arm, 0);
	SetCtrlVal (Master_Control_Panel, MasterCont_PulsedLIF_Arm, 0);
	SetCtrlVal (Master_Control_Panel, MasterCont_Photodiode_Arm, 0);
	SetCtrlVal (Master_Control_Panel, MasterCont_TripleProbe_Arm, 0);	
	SetCtrlVal (Master_Control_Panel, MasterCont_McPherson209_Arm, 0);	
	SetCtrlVal (Master_Control_Panel, MasterCont_ProbeAdvance_Status,0);
	SetCtrlVal (Master_Control_Panel, MasterCont_MasterTrigger_Arm,0);
	if (!MagFieldFlag) 	SetCtrlVal (Master_Control_Panel, MasterCont_MagField_Arm,0);
	if (!PlasmaGunFlag) 	SetCtrlVal (Master_Control_Panel, MasterCont_PlasmaGun_Arm,0);
}

void ManualMatLab_Plotting(void)
{
	//run the individual plotting routines for raw data
	if((HousekeepingFlag) && (HousekeepingPlotFlag)) 	;//Launch raw bdot plotting code;
	if((BdotFlag) && (BdotPlotFlag))   					;//Launch raw bdot plotting code;
 	if((DoubleProbeFlag) && (DoubleProbePlotFlag))   	;//Launch raw bdot plotting code;
 	if((PhotronCameraFlag) && (PhotronCameraPlotFlag))  ;//Launch raw bdot plotting code;
 	if((RFEAFlag) && (RFEAPlotFlag))   					;//Launch raw bdot plotting code;
	if((TemporaryDiagFlag) && (TemporaryDiagPlotFlag))  ;//Launch raw bdot plotting code;
	if((PhotodiodeFlag) && (PhotodiodePlotFlag))   		;//Launch raw bdot plotting code;
	if((TripleProbeFlag) && (TripleProbePlotFlag))   	;//Launch raw bdot plotting code;
	if((McPherson209Flag) && (McPherson209PlotFlag))   	;//Launch raw bdot plotting code;
	if((TSFlag) && (TSPlotFlag))   						;//Launch raw bdot plotting code;
	if((PulsedLIFFlag) && (PulsedLIFPlotFlag))   		;//Launch raw bdot plotting code;
}



// test function for pushing data to mds server with Python
void push_mds(void)
{
	const char set_env[] = "conda activate \"C:\PHASMA 2025 DAQ\PHASMA Python Routines\PHASMA_C_to_Python_convertor_Project\PhasmaMDS\Python\"";

	const char script_path[] = "& python \"C:/PHASMA 2025 DAQ/PHASMA Python Routines/PHASMA_C_to_Python_convertor_Project/PhasmaMDS/";

	const char script_fid[] = "mds_push_all.py\"";
	int returnval;

	const char command[] = "cmd.exe /c conda activate \"C:/PHASMA 2025 DAQ/PHASMA Python Routines/PHASMA_C_to_Python_convertor_Project/PhasmaMDS/Python\" & python \"C:/PHASMA 2025 DAQ/PHASMA Python Routines/PHASMA_C_to_Python_convertor_Project/PhasmaMDS/mds_push_all.py\"";
	
	//char command[256];
	//strcpy(command,"cmd.exe /c ");
	//strcat(command, set_env);
	//strcat(command, script_path);
	//strcat(command, script_fid);
	
	
	LaunchExecutable(command);
	
}




/*********************************************************************************************************
*	This function advances probes or laser wavelength in all active subsystems that request probe or laser advance
*
*
*********************************************************************************************************/
 void SingleStepAdvance(void)
{
	float Bdot_position=0.0;
	float DoubleProbe_position=0.0;
	float TripleProbe_position=0.0;
	float RFEA_position=0.0;
	float Gun1_Pos=0.0;
	float Gun2_Pos=0.0;
	
	//Advance the individual systems and update the positions in the master panel
	if(PlasmaGunFlag)   	{
		GunPosition(&Gun1_Pos, &Gun2_Pos);
		SetCtrlVal (Master_Control_Panel, MasterCont_Gun1_Radial, Gun1_Pos);
		SetCtrlVal (Master_Control_Panel, MasterCont_Gun2_Radial, Gun2_Pos);
	}
	
	if(BdotFlag)   	{
		Bdot_position = Advance_Bdot_Probe();
		SetCtrlVal (Master_Control_Panel, MasterCont_Magnetic_Radial, Bdot_position);
	}
	
	if(DoubleProbeFlag)   	{
		DoubleProbe_position = Advance_DoubleProbe_Probe();
		SetCtrlVal (Master_Control_Panel, MasterCont_FluctProbe_Radial, DoubleProbe_position);
	}

	if(TripleProbeFlag)   	{
		TripleProbe_position = Advance_TripleProbe_Probe();
		SetCtrlVal (Master_Control_Panel, MasterCont_TripleProbe_Radial, TripleProbe_position);
	}

	if(RFEAFlag)   	{
		RFEA_position = Advance_RFEA_Probe();
		SetCtrlVal (Master_Control_Panel, MasterCont_RFEA_Radial, RFEA_position);
	}
	
	if(PulsedLIFFlag)   	{
		Advance_PLIF_Laser();
		SetCtrlVal (Master_Control_Panel, MasterCont_PLIF_Laser_Wavelength, Get_PLIF_Wavelength());
	}


}
 

//Added this for silly debug purposes - TR 1/19/2026
int CVICALLBACK manual_write(int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch(event)
	{
		case EVENT_COMMIT:
			Write_HousekeepingData();
			//Write_BdotData();
		
	}
	return 0;
}
 
/*********************************************************************************************************
*	Here are all the callback functions from the Main Control Loop Panel that launch the settings windows
*
*
*
*********************************************************************************************************/
int CVICALLBACK MagField_Settings (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	int 	Mag_Field_State;
	switch (event)
	{
		case EVENT_COMMIT:
			//Since Magnetic Field system can be updated in the settings panel, pass the value of the arming state of the system to the control system
			GetCtrlVal (Master_Control_Panel, MasterCont_MagField_Status,&Mag_Field_State);
			OpenMagField_Settings(Mag_Field_State);
			break;
	}
	return 0;
}

int CVICALLBACK MasterTrigger_Settings (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			OpenTrigger_Settings(); 
			break;
	}
	return 0;
}

int CVICALLBACK Helicon_Settings (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			HeliconSource_Settings(); 
			break;
	}
	return 0;
}

int CVICALLBACK Housekeeping_Settings (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			OpenHousekeeping_Settings ();
			break;
	}
	return 0;
}

int CVICALLBACK Gun_Settings (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			//Deactivate guns to change settings
			PlasmaGunFlag=0;
			PlasmaGun_ActivateSystem(0);
			SetCtrlVal (Master_Control_Panel, MasterCont_PlasmaGun_Arm, 0);	
			SetCtrlVal (Master_Control_Panel, MasterCont_PlasmaGun_Status, 0);				
			OpenPlasmaGun_Settings (); 
			break;
	}
	return 0;
}

int CVICALLBACK MagProbe_Settings (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			OpenBdot_Settings (); 
			break;
	}
	return 0;
}

int CVICALLBACK TripleProbe_Settings (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			OpenTripleProbe_Settings ();
			break;
	}
	return 0;
}

int CVICALLBACK DoubleProbe_Settings (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			OpenDoubleProbe_Settings ();
			break;
	}
	return 0;
}

int CVICALLBACK ThomsonsScattering_Settings (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			OpenThomsonScattering_Settings ();
			break;
	}
	return 0;
}

int CVICALLBACK PulsedLIF_Settings (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			OpenPulsedLIF_Settings ();
			break;
	}
	return 0;
}

int CVICALLBACK ContinLIF_Settings (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			SetCtrlVal (Master_Control_Panel, MasterCont_ContinLIF_Arm, 1);
			Delay(0.1);
			SetCtrlVal (Master_Control_Panel, MasterCont_ContinLIF_Arm, 0);
			OpenContinLIF_Settings();
			break;
	}
	return 0;
}

int CVICALLBACK Quantum_Settings (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:

			break;
	}
	return 0;
}

int CVICALLBACK SingleLang_Settings (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			SetCtrlVal (Master_Control_Panel, MasterCont_Lang_Arm, 1);
			OpenLang_Settings();
			break;
	}

	return 0;
}

int CVICALLBACK Photodiode_Settings (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			OpenPhotodiode_Settings();
			break;
	}
	return 0;
}

int CVICALLBACK Camera_Settings (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			OpenPhotronCamera_Settings (); 
			break;
	}
	return 0;
}

int CVICALLBACK OceanOptics_Settings (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			OpenOceanOptics_Settings();
			break;
	}
	return 0;
}

int CVICALLBACK McPherson209_Settings (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			OpenMcPherson209_Settings (); 
			break;
	}
	return 0;
}

int CVICALLBACK Lightwave_Settings (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:

			break;
	}
	return 0;
}

int CVICALLBACK RFEA_Settings (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			OpenRFEA_Settings ();
			break;
	}
	return 0;
}

int CVICALLBACK Temporary_Settings (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			OpenTempDiag_Settings ();
			break;
	}
	return 0;
}


/*********************************************************************************************************
*	Here are all the callback functions from the Main Control Loop Panel that turn on the 
*	various instruments.
*
*
*********************************************************************************************************/
int CVICALLBACK MagField_Activate_Switch (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	int		switchstate;
	
	switch (event)
	{
		case EVENT_COMMIT:

			//Obtain the state of the switch and either turn on or turn off the magnetic fields 
			GetCtrlVal (panel, control,&switchstate);
			if (switchstate) {
				SetCtrlVal (Master_Control_Panel, MasterCont_MagField_Arm, 1);
				MagFieldFlag=1;
				MagField_Activate (switchstate, 1);

			} else {	
				SetCtrlVal (Master_Control_Panel, MasterCont_MagField_Arm, 0);
				MagFieldFlag=0;
				MagField_Activate (switchstate, 0);
			}
			
			break;
	}
	return 0;
}

int CVICALLBACK MasterTrigger_Activate (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	int		switchstate;

	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlVal (panel, control,&switchstate);
			if (switchstate) {
				MasterTriggerFlag=1;
				Trigger_Settings_Activate();
			} else {
 				MasterTriggerFlag=0;
			}
			break;
	}
	return 0;
}

int CVICALLBACK HeliconSource_Activate (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	int		switchstate;

	switch (event)
	{
		case EVENT_COMMIT:
			//Obtain the state of the switch and either turn on or turn off the helicon system 
			GetCtrlVal (panel, control,&switchstate);
			if (switchstate) {
				HeliconFlag=1;
				Helicon_Activate (switchstate);
				SetCtrlVal (Master_Control_Panel, MasterCont_Helicon_Arm, 1);
			} else {	
				HeliconFlag=0;
				Helicon_Activate (switchstate);
				SetCtrlVal (Master_Control_Panel, MasterCont_Helicon_Arm, 0);
			}

			break;
	}
	return 0;
}

int CVICALLBACK Housekeeping_Activate (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	int		switchstate;
	
	switch (event)
	{
		case EVENT_COMMIT:
			//Obtain the state of the switch and either turn on or turn off the housekeeping data system
			GetCtrlVal (panel, control,&switchstate);
			if (switchstate) {
				HousekeepingFlag=1;
				HousekeepingActivate();
			} else {	
				HousekeepingFlag=0;
			}

			break;
	}
	return 0;
}

int CVICALLBACK PlasmaGun_Activate (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	int		switchstate;

	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlVal (panel, control,&switchstate);
			if (switchstate) {
				PlasmaGunFlag=1;
				PlasmaGun_ActivateSystem(1);
				SetCtrlVal (Master_Control_Panel, MasterCont_PlasmaGun_Arm, 1);
			} else {	
				PlasmaGunFlag=0;
				PlasmaGun_ActivateSystem(0);
				SetCtrlVal (Master_Control_Panel, MasterCont_PlasmaGun_Arm, 0);
			}
			break;
	}
	return 0;
}

int CVICALLBACK Bdot_Activate (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	int		switchstate;

	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlVal (panel, control,&switchstate);
			if (switchstate) {
				BdotFlag=1;
				BdotActivate();
			} else {	
				BdotFlag=0;
			}
			break;
	}
	return 0;
}

int CVICALLBACK TripleProbe_Activate (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	int		switchstate;

	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlVal (panel, control,&switchstate);
			if (switchstate) {
				TripleProbeFlag=1;
				TripleProbeActivate();
			} else {	
				TripleProbeFlag=0;
			}
			break;
	}
	return 0;
}

int CVICALLBACK DoubleProbe_Activate (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	int		switchstate;

	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlVal (panel, control,&switchstate);
			if (switchstate) {
				DoubleProbeFlag=1;
				DoubleProbeActivate();
			} else {	
				DoubleProbeFlag=0;
			}
			break;
	}
	return 0;
}

int CVICALLBACK TS_Activate (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	int		switchstate;

	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlVal (panel, control,&switchstate);
			if (switchstate) {
				TSFlag=1;
				Trigger_Settings_Activate();
				ThomsonScatteringActivate();
			} else {	
				TSFlag=0;
			}
			break;
	}
	return 0;
}


int CVICALLBACK PulsedLIF_Activate (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	int		switchstate;

	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlVal (panel, control,&switchstate);
			if (switchstate) {
				PulsedLIFFlag=1;
				Trigger_Settings_Activate();
				PulsedLIFActivate();
			} else {	
				TSFlag=0;
			}
			break;
	}
	return 0;
}

int CVICALLBACK Quantum_Activate (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:

			break;
	}
	return 0;
}

int CVICALLBACK Photodiode_Activate (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	int		switchstate;

	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlVal (panel, control,&switchstate);
			if (switchstate) {
				PhotodiodeFlag=1;
				PhotodiodeActivate();
			} else {	
				TSFlag=0;
			}
			break;
	}
	return 0;
}

int CVICALLBACK Camera_Activate (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	int		switchstate;

	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlVal (panel, control,&switchstate);
			if (switchstate) {
				PhotronCameraFlag=1;
				PhotronCameraActivate();
			} else {	
				PhotronCameraFlag=0;
			}
			break;
	}
	return 0;
}

int CVICALLBACK Ocean_Activate (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:

			break;
	}
	return 0;
}

int CVICALLBACK McPherson209_Activate (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	int		switchstate;

	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlVal (panel, control,&switchstate);
			if (switchstate) {
				McPherson209Flag=1;
				McPherson209Activate();
			} else {	
				McPherson209Flag=0;
			}
			break;
	}
	return 0;
}

int CVICALLBACK Lightwave_Activate (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:

			break;
	}
	return 0;
}

int CVICALLBACK RFEA_Activate (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	int		switchstate;

	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlVal (panel, control,&switchstate);
			if (switchstate) {
				RFEAFlag=1;
				RFEAActivate();
			} else {	
				RFEAFlag=0;
			}
			break;
	}
	return 0;
}

int CVICALLBACK Temporary_Activate (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	int		switchstate;

	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlVal (panel, control,&switchstate);
			if (switchstate) {
				TemporaryDiagFlag=1;
				TempDiagActivate();
			} else {	
				TemporaryDiagFlag=0;
			}
			break;
	}
	return 0;
}




/*********************************************************************************************************
*	Here are all the callback functions from the Main Control Loop Panel that manually or automatically
*   arm and trigger the data system and/or advance the probes to their next position.
*********************************************************************************************************/

int CVICALLBACK mdstest(int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch(event)
	{
		case EVENT_COMMIT:
			push_mds();
			break;
			
	}
	return 0;
}

int CVICALLBACK ManualArm (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			MasterArm();
			LaserMasterArm();
			break;
	}
	return 0;
}

int CVICALLBACK ManualTrigger (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			//Fire the master trigger which has a pretty long time so that the TS laser can warm up and the capacitor banks can charge
			SetCtrlVal (Master_Control_Panel, MasterCont_Firing_LED, 1); //Fire the master trigger on first time through
						
			//Fire the initial T0 trigger to start warming up laser and charging banks
			Send_Trigger();
			Delay(.1);
			SetCtrlVal (Master_Control_Panel, MasterCont_Firing_LED, 0); //Reset the firing LED
			break;
	}
	return 0;
}

int CVICALLBACK ManualProbe_Advance (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			SingleStepAdvance();
			break;
	}
	return 0;
}

int CVICALLBACK ManualMatLab (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			ManualMatLab_Plotting();
			break;
	}
	return 0;
}



int CVICALLBACK Auto_Advance_Probes (int panel, int control, int event,
									 void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlVal(Master_Control_Panel,MasterCont_ProbeAdvance_Status,&Auto_Advance_Probes_Flag);
			break;
	}
	return 0;
}


int CVICALLBACK Auto_Run_Matlab (int panel, int control, int event,
								 void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			MatlabStatusFlag=1;
			break;
	}
	return 0;
}




int CVICALLBACK AutoCycle (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:	
			Repeat_Cycle();
			break;
	}
	return 0;
}

/*********************************************************************************************************
*	Here is the callback to shut down the user interface
*
*
*********************************************************************************************************/
int CVICALLBACK PHASMA_ShutDown (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:

			//Save state of master control panel, close it, and then shut down user interace
			SavePanelState (Master_Control_Panel, "Master_Control_Storage_File", Master_Control_Panel_setup_state);
			DiscardPanel (Master_Control_Panel);
			QuitUserInterface (0);

			
			break;
	}
	return 0;
}


int		LaserSimmerMode (int mode)
{
	//Switch lasers from simmer to warmup ready for trigger mode based on user input	
	if (mode) {
		if (TSFlag) 		ThomsonScatteringLaserControl(1);   //Thomson scattering laser set to simmer mode
		if (PulsedLIFFlag) 	PLIFLaserControl(1);				//PLIF laser set to simmer mode
	} else {
		if (TSFlag) 		ThomsonScatteringLaserControl(0);   //Thomson scattering laser set to warmup mode
		if (PulsedLIFFlag)	PLIFLaserControl(0);				//PLIF laser set to warmup mode
	}		
			
	return 0;
}
 



int		Advance_Single_Cycle (void)
{
	//Play audible tone pattern to let user know the master trigger is about to fire	
	Delay(.2);
	Beep();	

	//Fire the master trigger which has a pretty long time so that the TS laser can warm up and the capacitor banks can charge
	SetCtrlVal (Master_Control_Panel, MasterCont_Firing_LED, 1); //Fire the master trigger on first time through
				
	//Fire the initial T0 trigger and then wait for the laser trigger signal to start warming up the laser. The capacitor banks are already chargning
	Send_Trigger();
	Delay(.1);
	SetCtrlVal (Master_Control_Panel, MasterCont_Firing_LED, 0); //Reset the firing LED
	
	return 0;
}


/*********************************************************************************************************
*	These two functions are the sequence that will move through the arm-trigger-data storage sequence
*
*
*********************************************************************************************************/
 int	Start_Single_Cycle (void)
{
	FILE*	outfile; 
	int		ShotNum;
	int		result;
	char 	*date = DateStr();
	char 	*time = TimeStr();
	
	//Clear previous Bias Current plot
	DeleteGraphPlot (Master_Control_Panel, MasterCont_HousekeepingPlot, -1,VAL_IMMEDIATE_DRAW);

	//Open MDS to new shot number
	result=	IncrementMDSCurrentShot();

	//Store the raw data file storage path in the global file storage variable
	GetCtrlVal (Master_Control_Panel, MasterCont_RawDataPath,RawDataPath);
	
	//Update shot number in main window
	//Print current MDS shot
	ShotNum = getMDSCurrentShot();
	SetCtrlVal (Master_Control_Panel, MasterCont_MDS_Shot,ShotNum);
	
	//Put the shot number into the global shot number string for all functions to use
	sprintf(ShotNumberString,"%d",ShotNum);
		
	//Write an array of all the instrument write-t-mds flags to a spreadsheet to maintain a permanent list
	//of which data are stored for each shot. Include the shot number, the date, and all the flags.
	outfile = fopen (PHASMA_LOG_String, "a");
	fprintf(outfile,"%d,%s,%s,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n",ShotNum,date,time,MagFieldDataFlag, HeliconDataFlag,HousekeepingDataFlag,PlasmaGunDataFlag,Write_PlasmaGunDataData,
		BdotDataFlag,TripleProbeDataFlag,DoubleProbeDataFlag,TSDataFlag,PulsedLIFDataFlag,QuantumBeatDataFlag,PhotodiodeDataFlag,PhotronCameraDataFlag,OceanOpticsDataFlag,McPherson209DataFlag,LightwvIntferomDataFlag,RFEADataFlag,TemporaryDiagDataFlag);

 	//Close PHASMA log file
	fclose (outfile);

	//If Pulsed LIF system active, spawn process to record the laser energy per pulse and the wavelength
	if(PulsedLIFFlag) {
		ReadPulsedLIF_WaveandEnergy();
	}
				 
	//Arm all instruments but the lasers and the trigger system
	MasterArm();
		
	//Since the auto cycle button was selected, enable the timer - this starts the countdown sequence so it is followed immediately by firing the master trigger!
	SetCtrlAttribute (Master_Control_Panel, MasterCont_COUNTDOWN_TIMER, ATTR_ENABLED, 1);

	//Update the interface window
	ProcessDrawEvents();
	ProcessSystemEvents();
	
	//First time through, take the lasers out of simmer mode, arm the laser controllers, and fire the master trigger
	if (Cycle_Count < 1) {
		//If laser systems active, get lasers ready for warm up process and arm Thomson and Pulsed LIF systems
		if( (TSFlag) || (PulsedLIFFlag)) {
			LaserSimmerMode(0);
			LaserMasterArm(); 
		}
		
		//Arm the triggering system
		if((MasterTriggerFlag) && (MasterTriggerArm()>0))   SetCtrlVal (Master_Control_Panel, MasterCont_MasterTrigger_Arm, 1);
		
		//Fire the master trigger
		Advance_Single_Cycle();
	}
	
	//Now wait for timer to elapse
	return 0;
}
 
 

int		Complete_Single_Cycle (void)
{
	int		record_length;
	int 	result;
	double	time[Housekeeping_max_record_length];
	double	BiasI_a[Housekeeping_max_record_length];
	double	BiasI_b[Housekeeping_max_record_length];
	 
	
	//Clear all the arm states
	ClearMasterArm();
	
	//Immediately begin next cycle of timing if doing multiple pulses if there will be another shot
	if ( (Cycle_Count > 0) && (Cycle_Count < CycleRepeat) ) {
		//Arm the laser controllers again to get them ready for the warmup trigger
		if( (TSFlag) || (PulsedLIFFlag)) {
			LaserMasterArm(); 
		}		
		
		//Arm the triggering system
		if((MasterTriggerFlag) && (MasterTriggerArm()>0))   SetCtrlVal (Master_Control_Panel, MasterCont_MasterTrigger_Arm, 1);
		
		//Record the absolute time so the countdown clock will be accurate
		GetSystemTime (&trigger_hours, &trigger_minutes, &trigger_seconds);

		//Fire the trigger which will start the lasers
		Advance_Single_Cycle();
	}

	//Store those data targeted for storage
	Delay (0.2); //delay to let digitizers record everything
	
	//Store data
	Store_Data();
	Delay(0.5); // wait a moment between disc write and mds push 
	push_mds();

	//Close the open MDS tree
	result= CloseMDSTree();
	
	//Plot the Housekeeping data to the main window by sending the plot info to a Housekeeping function
	if (HousekeepingFlag) {
		record_length=TransferHousekeepingData (time,BiasI_a,BiasI_b);
	
		//Plot Bias currents to housekeeping plot window with a time limit of 2 ms
		SetAxisRange (Master_Control_Panel, MasterCont_HousekeepingPlot, VAL_MANUAL, 1200, 1400, VAL_NO_CHANGE, 0.0, 1.0);
		
		//PlotXY (Master_Control_Panel, MasterCont_HousekeepingPlot, time, BiasI_a, record_length, VAL_DOUBLE, VAL_DOUBLE, VAL_FAT_LINE, VAL_SIMPLE_DOT, VAL_SOLID, 1, VAL_CYAN);
		//PlotXY (Master_Control_Panel, MasterCont_HousekeepingPlot, time, BiasI_b, record_length, VAL_DOUBLE, VAL_DOUBLE, VAL_FAT_LINE, VAL_SIMPLE_DOT, VAL_SOLID, 1, VAL_MAGENTA);
	}
	
	//Plot raw data to screen if requested
	if (MatlabStatusFlag) ManualMatLab_Plotting();

	//Move probes and lasers to the next position if subsystem is active and there is a motion stage associate with probe and auto advance has been activated
	if (Auto_Advance_Probes_Flag) SingleStepAdvance();
	
	//Determine how much time remains on trigger cycle and set the countdown interval to that
	GetSystemTime (&post_storage_hours, &post_storage_minutes, &post_storage_seconds);

	//Update value of Countdown clock
	Countdown = (Interval - (int)((post_storage_hours-trigger_hours)*24*60+(post_storage_minutes-trigger_minutes)*60+(post_storage_seconds-trigger_seconds)));
	
	return 0;
}


 
/*********************************************************************************************************
*	This function is the master sequence that
*
*
*********************************************************************************************************/
void	Repeat_Cycle (void)
{


	int		j;
	int 	Trigger_panel;
	int		TriggerSettings_setup_state = 4;		//WARNING, if this gets changed in the trigger code it must be changed here
	
	//For a requested number of cycles with a proscribed delay in between cycles, run the single cycle code repeatedly
	//Set the cycle counter to zero to start and the Countdown flag to indicate countdown has not yet been reached
	Cycle_Count = 0;
	Countdown_Flag =0;
	
	//Get the requested number of cycles from the interface
	GetCtrlVal (Master_Control_Panel, MasterCont_CycleRepeat,&CycleRepeat);
	
	//Make sure requested interval time is greater than or equal to the larger of laser warmup or charging time
	//Get the requested cycle time from the interface
	GetCtrlVal (Master_Control_Panel, MasterCont_Interval,&Interval);
	
	//Get laser warmup and charging times and save them to a global variable so they can be used everywhere
	//Open settings panel and load saved values
	Trigger_panel = LoadPanel (0, "MasterTriggerSettings.uir", Trigger);
	RecallPanelState (Trigger_panel, "Master_Control_Storage_File", TriggerSettings_setup_state);
	GetCtrlVal (Trigger_panel, Trigger_SRSDG1_ChannelA,&Laser_Warmup);
	GetCtrlVal (Trigger_panel, Trigger_SRSDG1_ChannelC,&Gun_Charging);
	//Discard panel
	DiscardPanel(Trigger_panel); 
	
	//Make sure the interval requested is long enough to warm up laser. Make sure interval is at least 1 seconds
	//longer than laser warm up time so that there is time to take the lasers out of simmer mode
	if (Interval < Laser_Warmup+1.0)  {
		Interval = Laser_Warmup+1.0; //Set time interval to be adjusted for laser simmer mode to switch to laser warmup mode
	}
	
	//Set the master cycle time global variables to the interval
	Master_cycle_time = Interval;
	Countdown = Interval;

	//Update the cycle time in the interface
	SetCtrlVal (Master_Control_Panel, MasterCont_Interval,Interval);

	//Set master cycle time global variable and if it has been updated by more than 0.1 s, update trigger settings
	if (MasterTriggerFlag) Trigger_Settings_Activate();			//Only update if Trigger System is active
	
	//Set the max value on the countdown clock to the requested interval
	SetCtrlAttribute (Master_Control_Panel, MasterCont_CountdownClock, ATTR_MAX_VALUE, Interval);

	//Set the master simmer flag to zero before launching the countdown
	MasterSimmerFlag=0;
	
	//Call the start cycle process. The timer will keep track of how many cycles to do.
	Start_Single_Cycle();
	
}


/*********************************************************************************************************
*	Here is the master control loop for the entire data system
*   Note, make sure the stack size is large enough to handle all the processes when running as an executable
*
*********************************************************************************************************/
void  main(void)
{
    int 	height,width,panel_height, panel_width; 
	int		result;
	int 	j;
	float 	interval;

	//	Launch all the visuals
	GetScreenSize(&height, &width);
	width=(int)(width*2.8/5);			//compensate for new extra wide monitor
	panel_height = 2.8*height/5-20;
	panel_width = width;
	
	Master_Control_Panel = LoadPanel (0, "MasterControlPanel.uir", MasterCont);
	//RecallPanelState (Master_Control_Panel, "Master_Control_Storage_File", Master_Control_Panel_setup_state);
	DisplayPanel (Master_Control_Panel);
																		 
	SetPanelAttribute (Master_Control_Panel, ATTR_HEIGHT, panel_height);
	SetPanelAttribute (Master_Control_Panel, ATTR_WIDTH, 4*panel_width/5);
	SetPanelAttribute (Master_Control_Panel, ATTR_TOP, 150);
	SetPanelAttribute (Master_Control_Panel, ATTR_LEFT, 75);
	
	//Define Standard I/O window options and size
	SetStdioWindowOptions (1, 1, 0);
	SetStdioWindowPosition (0, 0);
	SetStdioWindowSize (1, 1);

	//Clear all arm state indicators
	ClearMasterArm();
		
	//Clear all activation and data switches for safety. Never boot into an active system
	ClearSystemStatus();

	//Access MDS database
	OpenPHASMA_MDS();  
	
	//Open global VisaSession
	result = viOpenDefaultRM (&Global_Visa_Session_Handle);
	
	//Get the requested cycle time from the interface
	GetCtrlVal (Master_Control_Panel, MasterCont_Interval,&Interval);
	Master_cycle_time = Interval;

	//Store the raw data file storage path in the global file storage variable
	GetCtrlVal (Master_Control_Panel, MasterCont_RawDataPath,RawDataPath);

	//Store the MDS data file storage path in the global file storage variable
	GetCtrlVal (Master_Control_Panel, MasterCont_MDSDataPath,MDSDATAPATH);
	
	//Create the path and filename for the log file
	sprintf(PHASMA_LOG_String, RawDataPath);
	strcat(PHASMA_LOG_String, "PHASMA2025_Shot_Log.txt");

	//Print current MDS shot
	printf("[DEBUG] Current MDS shot is '%d'...\n", getMDSCurrentShot());  
	SetCtrlVal (Master_Control_Panel, MasterCont_MDS_Shot,getMDSCurrentShot());
	
	//Set up the NI card to function as the master trigger
	SetUpNI_Card_Trig();
		
	// Start interacting with user
    RunUserInterface ();

 	//Close MDS database
	ClosePHASMA_MDS();  
	
	//Close Global Visa session
	viClose(Global_Visa_Session_Handle);
	
}

//This function is what is called, every second. Use it to update the value on the
//clock as the timer counts down. We want the master trigger to fire at the time the 
//lasers and/or charging ssytem need to start warming up. Then the actually firing of the
//banks will happen later. 
int CVICALLBACK countdown_timer (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{

	switch (event)
	{
		case EVENT_TIMER_TICK:
			//Update the value on the countdown clock 
			SetCtrlAttribute (Master_Control_Panel, MasterCont_CountdownClock, ATTR_CTRL_VAL, Countdown);
		
			//When reaching zero on the timer, disable the timer
			if (Countdown < 1) {	
				//send out the warning tone that the main pulse is about to fire
				Delay (0.2);
				Beep();
				
				//Disable the timer since the countdown is complete
				SetCtrlAttribute (Master_Control_Panel, MasterCont_COUNTDOWN_TIMER, ATTR_ENABLED, 0);
				Cycle_Count=Cycle_Count+1;	
					
				//Finish the cycle 
				Complete_Single_Cycle();
				
				//Start the next cycle if more than one cycle called for
				if(Cycle_Count < (CycleRepeat)) {
					Start_Single_Cycle(); 
				} else {
					//Perform end of cycle actions
				
					//if lasers active, put lasers into simmer mode
					if( (TSFlag) || (PulsedLIFFlag)) {
						LaserSimmerMode(1);
					}
				}
			}
				
			//decrease the time by one second every clock tick
			Countdown=Countdown-1;
			
			break;
	}
	
	return 0;
}



int CVICALLBACK SetMasterClock (int panel, int control, int event,
								void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			//Get the requested cycle time from the interface
			GetCtrlVal (Master_Control_Panel, MasterCont_Interval,&Interval);
			Master_cycle_time = Interval;
			break;
	}
	return 0;
}


