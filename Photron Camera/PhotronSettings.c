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

unsigned long 	nDeviceNo;             	//Device number
unsigned long 	nChildNo = 1;			//Number of the camera head



//Photron Camera Global Variables



  

//Set up Photron Camera
void PhotronCameraActivate(void)
{
	char 			*endpointer;
	unsigned long 	IPList[PDC_MAX_DEVICE];   // IP address to be searched
	unsigned long 	pDeviceCode[PDC_MAX_DEVICE];
	unsigned long 	pTmpDeviceNo[PDC_MAX_DEVICE];
	unsigned long 	pInterfaceCode[PDC_MAX_DEVICE];
	unsigned long 	pList[PDC_MAX_DEVICE]; 
	unsigned long 	nRet;
	unsigned long 	nErrorCode;
	unsigned long 	nDetectParam;
	unsigned long 	pDeviceNum;
	unsigned long 	nAFrames;				//Frames per trigger 
	unsigned long 	nRFrames;				//Frames per trigger  
	unsigned long 	nRCount = 1;			//Record count limit 
	unsigned long 	nFrameNo = 1;			//Frame number at which to start save
	char			nDepth;

	unsigned long 	nWidth; 				//Image Width
	unsigned long 	nHeight; 				//Image height
	unsigned long 	frame_rate;				//Image frame rate
	unsigned long 	nPreLUTMode;  			//Pre-LUT mode 
	unsigned long 	pSize;  				//Pre-LUT mode 
	unsigned long 	nList;  				//Pre-LUT mode 
	unsigned long 	pFps;
	unsigned long 	pMode;
	unsigned long 	pDepth;
	char			PhotronIPAddress_String[16];
	
	PDC_DETECT_NUM_INFO DetectNumInfo;      // Search result
	PDC_LUT_PARAMS lutParams;   			// Pre-LUT parameters

	//Open panel in background to get values from interface
	PhotronCamera_panel = LoadPanel (0, "PhotronCameraSettings.uir", PhotronCam);
	RecallPanelState (PhotronCamera_panel, "Master_Control_Storage_File", PhotronCamera_setup_state);

	GetCtrlVal(PhotronCamera_panel, PhotronCam_FramesPerSec, &Camera_frame_rate);  
	GetCtrlVal(PhotronCamera_panel, PhotronCam_Height, &Camera_height);  
	GetCtrlVal(PhotronCamera_panel, PhotronCam_Width, &Camera_width);  
	GetCtrlVal(PhotronCamera_panel, PhotronCam_Shutter, &Camera_shutter_rate);  
	GetCtrlVal(PhotronCamera_panel, PhotronCam_Frames, &Camera_frames);  
	GetCtrlVal(PhotronCamera_panel, PhotronCam_Brightness, &nBrightness);  
	GetCtrlVal(PhotronCamera_panel, PhotronCam_Contrast, &nContrast);  
	GetCtrlVal(PhotronCamera_panel, PhotronCam_Gamma, &nGamma);  
	GetCtrlVal(PhotronCamera_panel, PhotronCam_Gain, &nGain);  
	GetCtrlVal(PhotronCamera_panel, PhotronCam_PhotronIPAddress, PhotronIPAddress_String);  
 	
	//Close panel
	DiscardPanel(PhotronCamera_panel);
	
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

	//Turn on shading correction
	nRet = PDC_SetShadingMode(nDeviceNo,nChildNo,PDC_SHADING_ON,&nErrorCode);
	
	//Set the shutter speed to 1/frame rate
	//nRet = PDC_SetAutoExposure(nDeviceNo,nChildNo, PDC_FUNCTION_ON,&nErrorCode);
	nRet = PDC_SetShutterSpeedFps(nDeviceNo,nChildNo, Camera_shutter_rate,&nErrorCode);   //sets the shutter speed to 1/frame_rate 
 	
	//Set the trigger mode to random
	nRet = PDC_SetTriggerMode(nDeviceNo,PDC_TRIGGER_RANDOM_MANUAL,nAFrames,nRFrames,nRCount,&nErrorCode);   //Set the trigger to give nRCount frames from before the trigger time
	
	//Set the frame rate to frame_rate in fps - if you do not do this close to last, it does not take
	nRet = PDC_SetRecordRate(nDeviceNo,nChildNo,frame_rate,&nErrorCode);

	//Set the resolution to the specified height and width
	nRet = PDC_SetResolution(nDeviceNo,nChildNo, nWidth, nHeight,&nErrorCode);	
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
    char			headerstring[50];
	unsigned long 	nRet;
	unsigned long 	nErrorCode;
	unsigned long 	pSize;
	char			PhotronFileName[54];
	
	
	shotnum= getMDSCurrentShot();
	//Create output filename to send to Photron camera based on current MDS shot number and set path to external hard
	//drive array
	sprintf(PhotronFileName,"Z:\\PhotronCameraData\\Scan\\%d.mraw", shotnum);		
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

			//Update camera with new values
			PhotronCameraActivate();

			//Save current state of panel and close interface
			SavePanelState (PhotronCamera_panel, "Master_Control_Storage_File", PhotronCamera_setup_state);
			DiscardPanel(panel);
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
