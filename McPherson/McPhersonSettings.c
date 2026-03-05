#include	"ATMCD32D.h"  
#include 	<rs232.h>
#include 	<utility.h>
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
int			McPherson209_setup_state = 651;
int			McPherson209_RecordLength=1024;
int			McP_Andor_H_bin;
int			McP_Andor_V_bin;
int 		McP_Andor_cameraIndex;			//select second camera, assume it is the McPherson camera and not the Thomson Scattering camera
int 		McP_Andor_cameraHandle= 0;   	//Handle for second camera
int			McP_Andor_Horiz_pixels;
int			McP_Andor_Vert_pixels;
int 		McP_iCoolerStatus=0;
float		McP209_Current_Wlength;
float		McP209_GrooveDen;
float		McP209_AngleDiff;
float		McP209_FocalLength;

//Arm the Andor camera set up on the spectrometer
int McPherson209Arm(void)
{
	int 	result;

	//Arm the Andor Camera but first make sure the code is talking to the McP camera
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

	
	float		McP_Andor_Temp;
	float		McP_Andor_Exp_Time;
	float		McP_Andor_DDGDelay;
	float		McP_Andor_GateWidth;
	
	//Open up Double Probe settings panel and get target digitizer rate and other settings
	McPherson209_panel = LoadPanel (0, "McPhersonSettings.uir", McP209);
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
	result = SetDDGGateTime((McP_Andor_DDGDelay/1000.0),(McP_Andor_GateWidth/1000.0) );							//Set time and width in picoseconds
	
	GetCtrlVal (McPherson209_panel, McP209_Andor_H_bin,&McP_Andor_H_bin);
	GetCtrlVal (McPherson209_panel, McP209_Andor_V_bin,&McP_Andor_V_bin);
	result = GetDetector(&McP_Andor_Horiz_pixels,&McP_Andor_Vert_pixels);										//Read how many pixels there are in H and V
	result = SetImage(McP_Andor_H_bin,McP_Andor_V_bin, 1, McP_Andor_Horiz_pixels, 1, McP_Andor_Vert_pixels);	//Bin the pixels of the iCCD from 1 to the full extent into the requested number of bins
	
	//Discard housekeeping panel
	DiscardPanel(McPherson209_panel);
}


//Use this routine to create the wavelength axis for the McPherson data
void Create_McP_Wavelength (double Wavelength[])  
{
	#define		pi 3.141592654	
	#define		Order 1
	#define		PixelDimensions 13		//in microns
	int			j;
	float		alpha;
	float		beta;
	float		linear_disp;
	float		Wavelength_step;
	
	//Open settings panel and load saved values
	McPherson209_panel = LoadPanel (0, "McPhersonSettings.uir", McP209);
	RecallPanelState (McPherson209_panel, "Master_Control_Storage_File", McPherson209_setup_state);

	//Get McPherson Spectrometer Parameters
 	GetCtrlVal (McPherson209_panel, McP209_McP209_Current_Wlengt,&McP209_Current_Wlength);	   //Wavelength in nm
	GetCtrlVal (McPherson209_panel, McP209_McP209_GrooveDen,&McP209_GrooveDen);
	GetCtrlVal (McPherson209_panel, McP209_McP209_AngleDiff,&McP209_AngleDiff);
	GetCtrlVal (McPherson209_panel, McP209_McP209_FocalLength,&McP209_FocalLength);
	GetCtrlVal (McPherson209_panel, McP209_Andor_H_bin,&McP_Andor_H_bin);

	alpha = (180.0/pi)*( asin( (Order*McP209_GrooveDen*1.0E-6/2.0)/(cos(McP209_AngleDiff*pi/2.0/180.0) ) ) - (McP209_AngleDiff*pi/2.0/180.0) );
	beta = McP209_AngleDiff+alpha;
	
	linear_disp=1.0E6*cos(beta*pi/180.0)/(McP209_FocalLength*Order*McP209_GrooveDen);

	Wavelength_step= McP_Andor_H_bin*PixelDimensions*0.001*linear_disp;
	
	for (j=0;j<(int)(McP_Andor_Horiz_pixels/McP_Andor_H_bin);j++) {
		Wavelength[j]= ((j-(McP_Andor_Horiz_pixels/McP_Andor_H_bin)/2.0)*Wavelength_step+McP209_Current_Wlength);		//This assumes no stretching of axis or offsets from calibration
	}
	
	//Close panel in background
	DiscardPanel(McPherson209_panel); 


}

//Pulls the image from the Andor camera on the spectrometer and pushes it to the raw data folder
void Write_McPherson209Data(void)
{
	int 		result;
	int 		i,j;
	char		Channel1NameString[64];
	char		outfilename[64];
	double		*VerticalIndex;
	double		*McPWavelength;
	double		*McPAndorImageDouble;
	WORD		*McPAndorImage;	
	FILE*		outfile;
	
	//Get data from the McP Andor Camera but first make sure the code is talking to the McP camera
	if (McP_Andor_cameraHandle > 0) {
		result = SetCurrentCamera(McP_Andor_cameraHandle);												//Choose to make the McP camera the default camera if more than one camera
	}

	result = GetDetector(&McP_Andor_Horiz_pixels,&McP_Andor_Vert_pixels);								//Read how many pixels there are in H and V

	McPAndorImage = malloc ((int)((2*McP_Andor_Horiz_pixels/McP_Andor_H_bin)*(McP_Andor_Vert_pixels/McP_Andor_V_bin)));	//Build an array equal to the size of the total number of image pixels after the image has been binned into a
																														//smaller set of pixels per how the camera was set up earlier.
																														//Increase the size to account for the data being 16 bits so it needs to be 2 words (8 bits). Malloc allocates memory in bytes
	//Create a double array from the image array
	McPAndorImageDouble = malloc ((int)((8*McP_Andor_Horiz_pixels/McP_Andor_H_bin)*(McP_Andor_Vert_pixels/McP_Andor_V_bin))); 					//Increase the size to account for splitting the image into 2 parts and the fact that a double is 64 bits (or 8 words).
	
	result = GetAcquiredData16(McPAndorImage, (int)((McP_Andor_Horiz_pixels/McP_Andor_H_bin)*(McP_Andor_Vert_pixels/McP_Andor_V_bin)));			//Retrieve the image data from the camera 

	//Put the image date into a double array for MDS
	for (j=0;j<(int)(McP_Andor_Vert_pixels/McP_Andor_V_bin);j++)  {  
		for (i=0;i<(int)(McP_Andor_Horiz_pixels/McP_Andor_H_bin);i++)  {  
			McPAndorImageDouble[j*(int)(McP_Andor_Horiz_pixels/McP_Andor_H_bin)+i]=(double)McPAndorImage[j*(int)(McP_Andor_Horiz_pixels/McP_Andor_H_bin)+i];
		}
	}
	
	VerticalIndex = malloc ((int)(8*McP_Andor_Vert_pixels/McP_Andor_V_bin));		//Since 8 bit intergaer
	//Create vertical index array
	for (j=0;j<(int)(McP_Andor_Vert_pixels/McP_Andor_V_bin);j++)  {
		VerticalIndex[j]=(double)j;		
	}
	
	McPWavelength = malloc ((int)(8*McP_Andor_Horiz_pixels/McP_Andor_H_bin));
	//Run McPherson Function to generate wavelength array
	Create_McP_Wavelength(McPWavelength);

	//Reopen settings panel and load saved values since wavelength generating function closed the panel
	McPherson209_panel = LoadPanel (0, "McPhersonSettings.uir", McP209);
	RecallPanelState (McPherson209_panel, "Master_Control_Storage_File", McPherson209_setup_state);

	//Get channel name strings for Andor image and photodiode data and push data to MDSPlus
	GetCtrlVal (McPherson209_panel, McP209_Chan1Name,Channel1NameString);

	//Dump results to a file
	sprintf(outfilename, RawDataPath);
	strcat(outfilename, ShotNumberString);
	strcat(outfilename, "_");
	strcat(outfilename, "McPhersonSpectrum.txt");
	outfile=fopen (outfilename, "w");
	

	//Write McP spectrum to McP file assuming it is a 1D array of signal versus wavelength.
	for (j=0;j<(int)(McP_Andor_Horiz_pixels/McP_Andor_H_bin);j++) {
		fprintf(outfile,"%f, %f, %f\n",McPWavelength[j],McPAndorImageDouble[j]);
	}

 	//Close TS data data file
	fclose (outfile);

	//Close panel in background
	DiscardPanel(McPherson209_panel); 
	
}



//Launch this routine as window opens and it updates the window with the latest values from the 
//control system as it loads.
void OpenMcPherson209_Settings (void)  
{
	int 	error;
	int		McP_Andor_Temp;
	long 	totalCameras;	
	
	//Open settings panel and load saved values
	McPherson209_panel = LoadPanel (0, "McPhersonSettings.uir", McP209);
	RecallPanelState (McPherson209_panel, "Master_Control_Storage_File", McPherson209_setup_state);
	DisplayPanel(McPherson209_panel);
	
	//Get number of available cameras
	GetAvailableCameras(&totalCameras);
	
	//If the TS camera is also on, there will be two cameras in the system and the McPherson canera shows up as the second camera (index = 1),
	//so check for number of cameras and set index accordingly
	if (totalCameras == 1) {
		McP_Andor_cameraIndex = 0;
	} else {
		McP_Andor_cameraIndex =1;
	}
	
	//Connect to McPherson camera and update panel with current camera temperature
	error = GetCameraHandle(McP_Andor_cameraIndex, &McP_Andor_cameraHandle);   	//Get Handle for first camera
    if (error != DRV_SUCCESS) {
        printf("Only one camera in system so step to find multiple cameras failed %u\n", error);
    }
		
	
	if (McP_Andor_cameraHandle > 0) {
		error = SetCurrentCamera(McP_Andor_cameraHandle);						//Choose to make the McP camera the default camera if more than one camera
	}
	
	//Regardless of if one or two cameras, at this point a default camera has been selected and all commands will go to that camera.
	error = Initialize("");														//Initialize the camera
	Delay(0.25);																//Delay to let camera initialize
	
	error = IsCoolerOn(&McP_iCoolerStatus);
   	if (McP_iCoolerStatus) {
		//Move control switch to on position if temperature system active
		SetCtrlVal (McPherson209_panel, McP209_Andor_Camera_Cooler,1);
		error = GetTemperature(&McP_Andor_Temp);												//Update camera temperature in window. Function call will fail if camera temp system not active
 		SetCtrlVal (McPherson209_panel, McP209_Andor_Temp_Actual,McP_Andor_Temp);				//Report value of camera temperature to panel
	} else {
		//Move control switch to off position if temperature system not active
		SetCtrlVal (McPherson209_panel, McP209_Andor_Camera_Cooler,0);
		SetCtrlVal (McPherson209_panel, McP209_Andor_Temp_Actual,1000);
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



/*  COMMANDS to operate the McPherson 789A controller
R (Reset): Stops motion, sets counter to "0", and enters Idle state.
F (Find Home): Searches for the home switch transition at 1000 pps.
V [sps] (Velocity): Sets scan speed in steps per second (Range: 36 sps to 60,000 sps).
+ / - [steps] (Index Scan): Moves the motor relative to the current position (e.g., +36000).
I [sps] (Initial Velocity): Sets starting and stopping speed.
K [factor] (Ramp Slope): Sets acceleration/deceleration factor.
S (Save): Stores current parameters to non-volatile memory.
X (Examine): Sends values of K, I, and V parameters.
^ (Read Status): Returns motor status (0=stop, 1=moving, 2=constant velocity, 16=ramping).
] (Read Limit Switch): Returns limit switch status (0=none, 32=home, 64=high, 128=low). 
*/

//Move McPherson Spectrometer
int CVICALLBACK Move_McPherson209 (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	int			result;
	int			counts_to_move;
	int 		count;
	float 		Target_Wavelength;
	float		delay_time=.01;	
	float 		Countspernm_209;
	float 		Current_Wavelength;
	char 		receive_Value[64];
 	char		COM_string[30];	
	char 		send_string[64];
	ViSession 	McPherson209_handle;
	
	switch (event)
	{
		case EVENT_COMMIT:
			
			//Get the target wavelength 
			GetCtrlVal (McPherson209_panel, McP209_McP209_Center,&Target_Wavelength);
			
			//Get McPherson 209 controller COM port string from settings panel
 			GetCtrlVal (McPherson209_panel, McP209_McP209_COM,COM_string);
			
			//Read the current position of the spectrometer
			GetCtrlVal (McPherson209_panel, McP209_McP209_Current_Wlengt,&Current_Wavelength);
			
			//read the calibration factor, counts per mm for this spectrometer
			GetCtrlVal (McPherson209_panel, McP209_McP209_Countspernm,&Countspernm_209);
			
			//Open VISA session to COM port
			result=viOpen (Global_Visa_Session_Handle, COM_string, VI_NULL, VI_NULL, &McPherson209_handle);
									
			//Open communications with controller
			sprintf (send_string, " \r");  					// write info to string to send to instrument
			result = viWrite (McPherson209_handle, (ViConstBuf)send_string, (ViUInt32)strlen(send_string), &count);
			
			//clear buffer
			result = viRead(McPherson209_handle, (ViConstBuf)receive_Value, 32, &count);
			
			//return without doing anything else if open command fails
			if (result < 0) {
				return(0.0);
			} else {
							
				//Based on current position in controller, current wavelength, and target wavelength, calculate how many steps
				//need to move the controller to the correct wavelength and then send that command as an integer
				counts_to_move=fabs((int)((Target_Wavelength-Current_Wavelength)*Countspernm_209));									
				Delay(0.2);
				
				if (Target_Wavelength > Current_Wavelength) {
					sprintf (send_string, "+%d\r", counts_to_move);  					// write info to string to send to instrument
				} else {
					sprintf (send_string, "-%d\r", counts_to_move);  					// write info to string to send to instrument
				}
				//result = ComWrt (McPherson209_COM_Port, send_string, 64);				// send move command
				result = viWrite (McPherson209_handle, (ViConstBuf)send_string, (ViUInt32)strlen(send_string), &count);
				
				//Update the stored current position of the spectrometer to be the new position (assume the move command was successful)
				SetCtrlVal (panel, McP209_McP209_Current_Wlengt,Target_Wavelength);				
			}
			Delay (delay_time);  
  			
			//Close session to instrument
			result=viClose (McPherson209_handle);	
				
			//Update the stored current position of the spectrometer to be the new position (assume the move command was successful)
			SetCtrlVal (McPherson209_panel, McP209_McP209_Current_Wlengt,Target_Wavelength);				

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
			
			//set the current wavelength in this code block so it is available
			GetCtrlVal (McPherson209_panel, McP209_McP209_Current_Wlengt,&Current_Wavelength);
			
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