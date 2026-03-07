/********************************************************************
	This file includes everything necessary for communicating with and
	storing data from the Photron camera. To be able to talk to the Photron camera, 
	go into Windows' firewall advanced settings, go into the Windows Defender settings 
	and then into the Public settings tab, then select the customize protections 
	options and turn off the firewall for the Slow and Fast Lab networks. That should
	turn off the firewall blocking to the camera that prevents these functions from working.
	
	
											Earl E. Scime 6/23/2020
											.
	The functions in this program include 
		open_Photron_file - creates both a standard DAQ header file and the .AVI for the camera output
		Photron_arm		  - Sets the camera to be ready for an external trigger
		write_Photron_data- Writes to the header file and the AVI file after the shot
		Photron_fclose	  - Closes the data files
		Photron_printf    - Prints a string to the open header file
		Photron_parameters- Loads all the settings onto the camera
		Photron_Close_Camera- Shuts down communications with the camera
	
*********************************************************************/
#include 	<windows.h> 
#include <ansi_c.h>
#include	<PDCLIB.h> 
#include 	<ansi_c.h>
#include	<visa.h>
#include	<stdlib.h>
#include 	<formatio.h>
#include	"PHASMA-MDS-Routines.h"
#include 	"PhotronCameraSystems.h"
#include 	"PhotronCameraSettings.h"  
#include	"GlobalVariables.h"
#include 	<userint.h>
#include 	<utility.h>
#include 	<analysis.h>

#define		pi 3.141593
//#define		Camera_frame_rate 125
//#define		Camera_frames 100
//#define		Camera_height 512
//#define		Camera_width 512
#define		Camera_IP_address  0xC0A8000A    // 192.168.0.10	 (now read in from interface)



int			Camera_frame_rate;  
int			FrameCase;  
int			Camera_height;  
int			Camera_width;  
int			Camera_shutter_rate;  
int			Camera_frames = 1000;		//Default to 1000 frames
int			Camera_found_flag = 0;		//Start with camera not found
int			PhotronCamera_panel;
int			PhotronCamera_setup_state = 40;
double   	nBrightness=0.00;         	//Brightness 
double  	nContrast=0.00;           	//Contrast
double 		nGamma=0.6;              	//Gamma
double 		nGain=16.0;              	//Gamma
float 		micro_per_pixel;			//User defined mapping of length scale to pixels
float		Trigger_Delay;  

unsigned long 	nDeviceNo;             	//Device number
unsigned long 	nChildNo = 1;			//Number of the camera head



//Photron Camera Global Variables



  

//Set up Photron Camera
void PhotronCameraActivate(void)
{
	char 			*endpointer;
	unsigned long 	IPList[PDC_MAX_DEVICE];   // IP address to be searched

	unsigned long 	nRet;
	unsigned long 	nErrorCode;
	unsigned long 	nAFrames;				//Frames per trigger 
	unsigned long 	nRFrames;				//Frames per trigger  
	unsigned long 	nRCount = 1;			//Record count limit 
	unsigned long 	delayCounts=0;			//Frames to skip after trigger

	unsigned long 	nWidth; 				//Image Width
	unsigned long 	nHeight; 				//Image height
	unsigned long 	frame_rate;				//Image frame rate
	unsigned long 	nPreLUTMode;  			//Pre-LUT mode 

	char			PhotronIPAddress_String[16];
	
	PDC_DETECT_NUM_INFO DetectNumInfo;      // Search result
	PDC_LUT_PARAMS lutParams;   			// Pre-LUT parameters

	//Open panel in background to get values from interface
	PhotronCamera_panel = LoadPanel (0, "PhotronCameraSettings.uir", PhotronCam);
	RecallPanelState (PhotronCamera_panel, "Master_Control_Storage_File", PhotronCamera_setup_state);

	GetCtrlVal(PhotronCamera_panel, PhotronCam_PhotronFrameCase, &FrameCase);  
	GetCtrlVal(PhotronCamera_panel, PhotronCam_Shutter, &Camera_shutter_rate);  
	GetCtrlVal(PhotronCamera_panel, PhotronCam_Frames, &Camera_frames);  
	GetCtrlVal(PhotronCamera_panel, PhotronCam_Brightness, &nBrightness);  
	GetCtrlVal(PhotronCamera_panel, PhotronCam_Contrast, &nContrast);  
	GetCtrlVal(PhotronCamera_panel, PhotronCam_Gamma, &nGamma);  
	GetCtrlVal(PhotronCamera_panel, PhotronCam_Gain, &nGain);  
	GetCtrlVal(PhotronCamera_panel, PhotronCam_Trigger_Delay, &Trigger_Delay);  
	GetCtrlVal(PhotronCamera_panel, PhotronCam_PhotronIPAddress, PhotronIPAddress_String);  
	GetCtrlVal(PhotronCamera_panel, PhotronCam_micro_per_pixel, &micro_per_pixel);  
	
	
 	
	//Close panel
	DiscardPanel(PhotronCamera_panel);
	
	//Set up frame rate and resolutions based on selection
	switch(FrameCase)
		{
		case 0:
			Camera_frame_rate= 9000;  
			Camera_height = 1024;  
			Camera_width = 1024;
			break;
		case 1:
			Camera_frame_rate= 12000;  
			Camera_height = 1024;  
			Camera_width = 768;
			break;
		case 2:
			Camera_frame_rate= 18000;  
			Camera_height = 1024;  
			Camera_width = 512;
			break;
		case 3:
			Camera_frame_rate= 10000;  
			Camera_height = 896;  
			Camera_width = 896;
			break;
		case 4:
			Camera_frame_rate= 15000;  
			Camera_height = 768;  
			Camera_width = 768;
			break;
		case 5:
			Camera_frame_rate= 25000;  
			Camera_height = 640;  
			Camera_width = 480;
			break;
		case 6:
			Camera_frame_rate= 30000;  
			Camera_height = 512;  
			Camera_width = 512;
			break;
		case 7:
			Camera_frame_rate= 38400;  
			Camera_height = 512;  
			Camera_width = 384;
			break;
		case 8:
			Camera_frame_rate= 45000;  
			Camera_height = 384;  
			Camera_width = 384;
			break;
		case 9:
			Camera_frame_rate= 57600;  
			Camera_height = 384;  
			Camera_width = 256;
			break;
		case 10:
			Camera_frame_rate= 80000;  
			Camera_height = 256;  
			Camera_width = 256;
			break;
		case 11:
			Camera_frame_rate= 160000;  
			Camera_height = 256;  
			Camera_width = 128;
			break;
		case 12:
			Camera_frame_rate= 200000;  
			Camera_height = 128;  
			Camera_width = 128;
			break;
		case 13:
			Camera_frame_rate= 250000;  
			Camera_height = 128;  
			Camera_width = 96;
			break;
		case 14:
			Camera_frame_rate= 400000;  
			Camera_height = 128;  
			Camera_width = 64;
			break;
		case 15:
			Camera_frame_rate= 480000;  
			Camera_height = 128;  
			Camera_width = 48;
			break;
		case 16:
			Camera_frame_rate= 576000;  
			Camera_height = 128;  
			Camera_width = 32;
			break;
		case 17:
			Camera_frame_rate= 900000;  
			Camera_height = 128;  
			Camera_width = 16;
			break;
	}
 
	
	//Set up parameters based on the defines
	nAFrames = Camera_frames;
	nRFrames = Camera_frames;
	frame_rate = Camera_frame_rate;
	nWidth=Camera_width;
	nHeight=Camera_height;

	//If camera has already been found, do not go through the process of finding the camera again
	if (!Camera_found_flag) {
		
		//Initalize PDC library
		nRet = PDC_Init(&nErrorCode);
		if (nRet == PDC_FAILED) {
		    printf("Photron Camera Error: PDC_Init Error %d\n", nErrorCode);
		}

		//Set the camera IP address
		//IPList[0] = Camera_IP_address; 
		IPList[0] = strtoul (PhotronIPAddress_String, &endpointer, 16); 
	
		//Detect Photron camera at specific IP address
		nRet = PDC_DetectDevice(PDC_INTTYPE_G_ETHER,IPList,1,PDC_DETECT_NORMAL,&DetectNumInfo,&nErrorCode);
	
		//Check of detection failed
		if (nRet == PDC_FAILED) {
		    printf("PDC_DetectDevice Error %d\n", nErrorCode);
		    return;
		}

		// When the search result is 0 give up
		if (DetectNumInfo.m_nDeviceNum == 0) {
		 return;
		}

		// Quit if the search result IP address is different //
		if (DetectNumInfo.m_DetectInfo[0].m_nTmpDeviceNo != IPList[0]) {
		   return;
		}

		//Open device that was detected
		nRet = PDC_OpenDevice(&(DetectNumInfo.m_DetectInfo[0]),&nDeviceNo,&nErrorCode);
	 
		//Quit if device open failed, or set flag that device was found and is ready to go
		if (nRet == PDC_FAILED) {
		    printf("PDC_OpenDeviceError %d\n", nErrorCode);
			return;
		} else {
			Camera_found_flag = 1;
		}
	}
	
	//Set the LUT values for the camera
	nPreLUTMode = PDC_LUT_USER1;
		
	// Changes Pre-LUT parameters ( In the case of monochrome device, same parameters are used for each RGB plane ) 
	lutParams.m_nBrightnessR = nBrightness;
	lutParams.m_nBrightnessG = nBrightness;
	lutParams.m_nBrightnessB = nBrightness;

	lutParams.m_nContrastR = nContrast;
	lutParams.m_nContrastG = nContrast;
	lutParams.m_nContrastB = nContrast;

	lutParams.m_nGammaR = nGamma;
	lutParams.m_nGammaG = nGamma;
	lutParams.m_nGammaB = nGamma;

	lutParams.m_nGainR = nGain;
	lutParams.m_nGainG = nGain;
	lutParams.m_nGainB = nGain;
		
	lutParams.m_nPosiNegaR = FALSE;
	lutParams.m_nPosiNegaG = FALSE;
	lutParams.m_nPosiNegaB = FALSE;

	lutParams.m_nFreeMode = FALSE;  // Disable Pre-LUT direct setting function 

	// Sets Pre-LUT parameters ( At this point, this set of Pre-LUT parameters is not applied to the specified device ) 
	nRet = PDC_SetLUTUserParams(nDeviceNo,nChildNo,nPreLUTMode,&lutParams,&nErrorCode);

	//Set the camera status to live so future commands to change settings work
	nRet = PDC_SetStatus(nDeviceNo,PDC_STATUS_LIVE,&nErrorCode);
	
	// Uploads LUT data to the specified device 
	nRet = PDC_SetLUTUser(nDeviceNo,nChildNo,nPreLUTMode,&nErrorCode);

	//Use the pre-defined set of LuT values stored in "USER1" instead of uploading new values (which takes a lot of time and doesn't seem to work)
	//nRet = PDC_SetLUTUser(nDeviceNo,nChildNo,PDC_LUT_USER1,&nErrorCode);
		
	//Set the camera recording mode to autosave after a trigger. This might need to be turned off since the file name is not opened. Might need to move it right before the trigger
	//nRet = PDC_SetRecordingType(nDeviceNo, PDC_RECORDING_TYPE_DIRECT_TRIG,&nErrorCode);	
	
	//Set port #2 to be a negative polarity trigger pulse
	nRet = PDC_SetExternalInMode(nDeviceNo, 2,PDC_EXT_IN_TRIGGER_NEGA, &nErrorCode);

	//Inputs a switching signal (READY ON/OFF) for recording standby state. (Negative polarity)
	//nRet = PDC_SetExternalInMode(nDeviceNo, 2,PDC_EXT_IN_READY_NEGA, &nErrorCode);
 
	//Set the camera to partition #1. This is really critical
	nRet = PDC_SetCurrentPartition(nDeviceNo, nChildNo, 1,&nErrorCode);

	//Set the shutter speed to 1/frame rate
	//nRet = PDC_SetAutoExposure(nDeviceNo,nChildNo, PDC_FUNCTION_ON,&nErrorCode);
	nRet = PDC_SetShutterSpeedFps(nDeviceNo,nChildNo, Camera_shutter_rate,&nErrorCode);   
 	
	//Set the trigger mode to random
	nRet = PDC_SetTriggerMode(nDeviceNo,PDC_TRIGGER_RANDOM_MANUAL,nAFrames,nRFrames,nRCount,&nErrorCode);   //Set the trigger to give nRCount frames from before the trigger time
	
	//Set the trigger delay
	delayCounts = (int)(Trigger_Delay*1.0E-6/100e-9);
	nRet = PDC_SetDelay(nDeviceNo, PDC_DELAY_TRIGGER_IN, delayCounts, &nErrorCode);
	
	//Set the frame rate to frame_rate in fps - if you do not do this close to last, it does not take
	nRet = PDC_SetRecordRate(nDeviceNo,nChildNo,frame_rate,&nErrorCode);

	//Set the resolution to the specified height and width
	nRet = PDC_SetResolution(nDeviceNo,nChildNo, nWidth, nHeight,&nErrorCode);	
	
	//Turn on shading correction, this does the calibration
	nRet = PDC_SetShadingMode(nDeviceNo,nChildNo,PDC_SHADING_ON,&nErrorCode);
	
}


//Arm the Photron Camera
int PhotronCameraArm(void)
{
	unsigned long 	nRet;
	unsigned long 	nErrorCode;

	//Set the camera status to live
	nRet = PDC_SetStatus(nDeviceNo,PDC_STATUS_LIVE,&nErrorCode);

	//Set the camera to be ready for a trigger
	nRet = PDC_SetRecReady(nDeviceNo,&nErrorCode);

	//Manually trigger the camera (this is here temporarily for testing)
	//nRet = PDC_TriggerIn(nDeviceNo,&nErrorCode);
	
	return 0;
}




//Take the acquired data from the instrument and print it to the common data folder and 
//also push it to the MDS database
void Write_PhotronCameraData(void)
{
	int 			j;
	int				shotnum;
    char			PhotronData_String[64];
	unsigned long 	nRet;
	unsigned long 	nErrorCode;
	char			PhotronFileName[64];
	char			outfilename[64];
	FILE*			outfile;	
	
	shotnum= getMDSCurrentShot();
	//Create output filename to send to Photron camera based on current MDS shot number and set path to external hard
	//drive array
	
	//Open panel in background to get values from interface
	PhotronCamera_panel = LoadPanel (0, "PhotronCameraSettings.uir", PhotronCam);
	RecallPanelState (PhotronCamera_panel, "Master_Control_Storage_File", PhotronCamera_setup_state);

	//Get raw data file paty
	GetCtrlVal(PhotronCamera_panel, PhotronCam_PhotronDataPath, PhotronData_String);  
 	
	//Close panel
	DiscardPanel(PhotronCamera_panel);

	sprintf(PhotronFileName,"%sPhotronImage_%d.mraw", PhotronData_String,shotnum);		
 	//strcat(outname,".avi");		//for use if making avi files instead of mraw

	//Set the camera to memory mode
	nRet = PDC_SetStatus(nDeviceNo,PDC_STATUS_PLAYBACK,&nErrorCode);
 	
	//Set data transfer mode - is this needed?
	//nRet = PDC_SetTransferOption(nDeviceNo, nChildNo, PDC_8BITSEL_12UPPER,PDC_FUNCTION_ON, PDC_COLORDATA_INTERLEAVE_RGB, &nErrorCode);

	//Open the avi file to which the camera data will be sent
	//nRet = PDC_AVIFileSaveOpen(nDeviceNo,nChildNo,outname,30,PDC_COMPRESS_DIALOG_HIDE,&nErrorCode);

	//Open the MRAW file to which the camera data will be sent
	nRet = PDC_MRAWFileSaveOpen(nDeviceNo,nChildNo,PhotronFileName,PDC_MRAW_BITDEPTH_16,Camera_frames,&nErrorCode);

	//Send the images from the camera to the AVI file
	for (j=1;j<Camera_frames; j++) {
		//nRet = PDC_AVIFileSave(nDeviceNo,nChildNo,j,&pSize,&nErrorCode);
		
		//Use MRAW format instead
		nRet = PDC_MRAWFileSave(nDeviceNo,nChildNo,j,&nErrorCode);
	}
	
	//Close the photron camera avi file
	//nRet = PDC_AVIFileSaveClose(nDeviceNo,nChildNo,&nErrorCode);
	
	//Close the photon camera MRAW file
	nRet = PDC_MRAWFileSaveClose(nDeviceNo,nChildNo,&nErrorCode);
	
	//Set the camera status to live so future commands to change settings work
	nRet = PDC_SetStatus(nDeviceNo,PDC_STATUS_LIVE,&nErrorCode);
	
	//Dump settings to a file
	sprintf(outfilename, RawDataPath);
	strcat(outfilename, ShotNumberString);
	strcat(outfilename, "_");
	strcat(outfilename, "PhotronSettings.txt");
	outfile=fopen (outfilename, "w");
	
	fprintf(outfile, "Spatial Calibration = %f\r", micro_per_pixel);
	fprintf(outfile, "Camera frame rate = %d\r",Camera_frame_rate);
	fprintf(outfile, "Camera shutter rate = %d\r",Camera_shutter_rate);
	fprintf(outfile, "Trigger Delay (micro_s) = %f\r",Trigger_Delay);
	
	fclose(outfile);
	
}





//Launch this routine as window opens and it updates the window with the latest values from the 
//control system as it loads.
void OpenPhotronCamera_Settings (void)  
{
	//Open settings panel and load saved values
	PhotronCamera_panel = LoadPanel (0, "PhotronCameraSettings.uir", PhotronCam);
	RecallPanelState (PhotronCamera_panel, "Master_Control_Storage_File", PhotronCamera_setup_state);
	DisplayPanel(PhotronCamera_panel);
	
	//Update panel with 

	
	// Start interacting with user
    RunUserInterface ();

}

int CVICALLBACK Close_PhotronCamera_Settings (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:

			//Save current state of panel and close interface
			SavePanelState (panel, "Master_Control_Storage_File", PhotronCamera_setup_state);
			DiscardPanel(panel);
			
			//Update camera with new values
			PhotronCameraActivate();
			break;
	}
	return 0;
}


void Photron_Close_Camera(void)	
{
	unsigned long 	nRet;
	unsigned long 	nErrorCode;

	//Close the camera interface
	nRet = PDC_CloseDevice(nDeviceNo,&nErrorCode);
}
