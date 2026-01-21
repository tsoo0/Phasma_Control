#include	"ATMCD32D.h"  
#include 	<ansi_c.h>
#include	<visa.h>
#include	<stdlib.h>
#include 	<formatio.h>
#include	"PHASMA-MDS-Routines.h"
#include 	"McPhersonSystems.h"
#include 	"McPhersonSettings.h"  
#include	"GlobalVariables.h"
#include	"PHASMA_C_to_Python_convertor.h"


int			McPherson209_panel;
int			McPherson209_setup_state = 65;
int			McPherson209_RecordLength=8192;
int			McP_Andor_H_bin;
int			McP_Andor_V_bin;
int 		McP_Andor_cameraIndex = 1;		//select second camera, assume it is the Thomson camera and not the McPherson camera
int 		McP_Andor_cameraHandle= 0;   	//Handle for second camera



//Arm the Andor camera set up on the spectrometer
int McPherson209Arm(void)
{
	int 	result;

	//Arm the Andor Camera but first make sure the code is talking to the TS camera
	if (McP_Andor_cameraHandle > 0) {
		result = SetCurrentCamera(McP_Andor_cameraHandle);						//Choose to make the TS camera the default camera if more than one camera
	}

	//Arm the Andor Camera
	result = StartAcquisition();
	
	return 1;
}


//Set up the Andor Camera for the McPherson 209 spectrometer
void McPherson209Activate(void)
{
	int 		result; 					//	Error-checking variable
	int			McP_Andor_Camera_Cooler;
	int			McP_Andor_Gain;
	int			McP_Andor_Pre_Amp;
	int			McP_Andor_Horiz_pixels;
	int			McP_Andor_Vert_pixels;
	
	float		McP_Andor_Temp;
	float		McP_Andor_Exp_Time;
	float		McP_Andor_DDGDelay;
	float		McP_Andor_GateWidth;
	char		DetectorIniPath[64];
	
	//Open up Double Probe settings panel and get target digitizer rate and other settings
	McPherson209_panel = LoadPanel (0, "McPherson209Settings.uir", McP209);
	RecallPanelState (McPherson209_panel, "Master_Control_Storage_File", McPherson209_setup_state);
	   

	/******************************************************************************/
	//Set up McP_Andor Camera
				
	//Set up Andor Camera
	if (McP_Andor_cameraHandle > 0) {
		result = SetCurrentCamera(McP_Andor_cameraHandle);						//Choose to make the McP camera the default camera if more than one camera
	}
			
	GetCtrlVal (McPherson209_panel, McP209_Andor_Camera_Cooler,&McP_Andor_Camera_Cooler);
	if (McP_Andor_Camera_Cooler) {
		GetCtrlVal (McPherson209_panel, McP209_Andor_Temp,&McP_Andor_Temp);
		CoolerON();
		SetTemperature(McP_Andor_Temp);
	} else {
		CoolerOFF();
		SetCoolerMode(0);
		ShutDown();
	}
	result = SetAcquisitionMode(1);		//Single acquisition mode
	result = SetReadMode(4);			//Record an image
	result = SetTriggerMode(1);			//External Trigger
	result = SetGateMode(5);			//Use DDG mode for trigger
	
	GetCtrlVal (McPherson209_panel, McP209_Andor_Gain,&McP_Andor_Gain);
	result = SetGain(McP_Andor_Gain);
	
	result = SetVerticalSpeed(0);
	result = SetHorizontalSpeed(0);
	
	GetCtrlVal (McPherson209_panel, McP209_Andor_Pre_Amp,&McP_Andor_Pre_Amp);
	result = SetPreAmpGain(McP_Andor_Pre_Amp);
	
	result = SetDDGInsertionDelay(0);	//Set DDG insertion delay to normal
	result = SetDDGIntelligate(1);		//Set to "yes"
	
	GetCtrlVal (McPherson209_panel, McP209_Andor_Exp_Time,&McP_Andor_Exp_Time);
	result = SetExposureTime(McP_Andor_Exp_Time/1000000.0);
	
	GetCtrlVal (McPherson209_panel, McP209_DDG_Delay,&McP_Andor_DDGDelay);
	GetCtrlVal (McPherson209_panel, McP209_Andor_GateWidth,&McP_Andor_GateWidth);
	result = SetDDGGateTime((McP_Andor_DDGDelay/1000.0),(McP_Andor_GateWidth/1000.0) );						//Set time and width in picoseconds
	
	GetCtrlVal (McPherson209_panel, McP209_Andor_H_bin,&McP_Andor_H_bin);
	GetCtrlVal (McPherson209_panel, McP209_Andor_V_bin,&McP_Andor_V_bin);
	result = GetDetector(&McP_Andor_Horiz_pixels,&McP_Andor_Vert_pixels);									//Read how many pixels there are in H and V
	result = SetImage(McP_Andor_H_bin,McP_Andor_V_bin, 1, McP_Andor_Horiz_pixels, 1, McP_Andor_Vert_pixels);		//Bin the pixels of the iCCD from 1 to the full extent into the requested number of bins
	
	//Discard housekeeping panel
	DiscardPanel(McPherson209_panel);
}




//Calls the Python function that reads out the data from the T-daq System
//and pushes it to the raw data folder and the MDS database
void Write_McPherson209Data(void)
{
	int 		result;
	float		McPherson209_Time_Offset;
	float 		PythonVersion;
	char		Library_string[64];
	char 		Python_Command_String[64];

	//Get data from the TS Andor Camera but first make sure the code is talking to the TS camera
	if (McP_Andor_cameraHandle > 0) {
		result = SetCurrentCamera(McP_Andor_cameraHandle);						//Choose to make the TS camera the default camera if more than one camera
	}

}



//Launch this routine as window opens and it updates the window with the latest values from the 
//control system as it loads.
void OpenMcPherson209_Settings (void)  
{
	int 	error;
	int		McP_Andor_Temp;

	
	//Open settings panel and load saved values
	McPherson209_panel = LoadPanel (0, "McPhersonSettings.uir", McP209);
	RecallPanelState (McPherson209_panel, "Master_Control_Storage_File", McPherson209_setup_state);
	DisplayPanel(McPherson209_panel);
	
	//Connect to McPherson camera and update panel with current camera temperature
	error = GetCameraHandle(McP_Andor_cameraIndex, &McP_Andor_cameraHandle);   	//Get Handle for first camera
    if (error != DRV_SUCCESS) {
        printf("Only one camera in system so step to find multiple cameras failed %u\n", error);
    }
		
	
	if (McP_Andor_cameraHandle > 0) {
		error = SetCurrentCamera(McP_Andor_cameraHandle);						//Choose to make the McP camera the default camera if more than one camera
	}
	
	//Regardless of if one or two cameras, at this point a default camera has been selected and all commands will go to that camera.
	error = Initialize("");									//Initialize the camera
	error = GetTemperature(&McP_Andor_Temp);				//Update camera temperature in window. Function call will fail if camera temp system not active
    if ( (error = DRV_TEMPERATURE_OFF) ) {
		//Move control switch to off position if temperature system not active
		SetCtrlVal (McPherson209_panel, McP209_Andor_Camera_Cooler,0);
		SetCtrlVal (McPherson209_panel, McP209_Andor_Temp_Actual,McP_Andor_Temp);	//If temp call failed, set temperature in window to 1000 degrees
	} else {
		SetCtrlVal (McPherson209_panel, McP209_Andor_Temp_Actual,McP_Andor_Temp);
    }

	
	// Start interacting with user
    RunUserInterface ();

}

int CVICALLBACK Close_McPherson209_Settings (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			//Save current state of panel and close interface
			SavePanelState (McPherson209_panel, "Master_Control_Storage_File", McPherson209_setup_state);
			DiscardPanel(panel);
			break;
	}
	return 0;
}


//Move McPherson Spectrometer
int CVICALLBACK Move_McPherson209 (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	float 	Target_Wavelength;
	
	switch (event)
	{
		case EVENT_COMMIT:
			
			//Get the target wavelength 
			GetCtrlVal (McPherson209_panel, McP209_McP209_Center,&Target_Wavelength);
		
			//Move McPherson to target wavelength
			move_209(Target_Wavelength);
			
			break;
	}
	return 0;
}

int CVICALLBACK Set_McPherson209 (int panel, int control, int event,
								  void *callbackData, int eventData1, int eventData2)
{
	float 	Current_Wavelength;
	
	switch (event)
	{
		case EVENT_COMMIT:
			
			//Get the current wavelength
			GetCtrlVal (McPherson209_panel, McP209_McP209_Current_Wlengt,&Current_Wavelength);

			//Set current wavelength in controller software
			overwriteLocation_209(Current_Wavelength);
			
			break;
	}
	return 0;
}

int CVICALLBACK McP_Andor_Temp_Control (int panel, int control, int event,
								  void *callbackData, int eventData1, int eventData2)
{
	int 	result;
	int		McP_Andor_Temp;
	int		Actual_Temp;
	
	switch (event)
	{
			case EVENT_COMMIT:
				//Set up Andor Camera
				if (McP_Andor_cameraHandle > 0) {
					result = SetCurrentCamera(McP_Andor_cameraHandle);						//Choose to make the TS camera the default camera if more than one camera
				}
						
				GetCtrlVal (panel, McP209_Andor_Camera_Cooler,&result);

				if (result) {
					GetCtrlVal (panel, McP209_Andor_Temp,&McP_Andor_Temp);
					CoolerON();
					GetTemperature(&Actual_Temp);
					SetCtrlVal (panel, McP209_Andor_Temp_Actual,Actual_Temp);
					SetTemperature(McP_Andor_Temp);
				} else {
					GetTemperature(&Actual_Temp);
					SetCtrlVal (panel, McP209_Andor_Temp_Actual,Actual_Temp);
					CoolerOFF();
					SetCoolerMode(0);
					ShutDown();
				}
						
			break;
	}
	return 0;
}	