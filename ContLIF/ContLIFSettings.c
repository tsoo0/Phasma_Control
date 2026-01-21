/********************************************************************
	This file includes everything necessary for LIF measurements.
	The functions in this program are 
		open_LIF_file
		open_tomog_file
		LIF_wavemeter_acquire
		zero_LIF_arrays
		LIF_acquire
		LIF_analyze
		ShiftedMax--for analysis
		ShiftedMax2--
		ShiftedMax3-- "
		write_LIF_data
		SetLIAintTime
*********************************************************************/

//	Modification History
//
//  Changed to permit fitting of iodine cell data or weavemeter refernce
//	for center wavelength.   7/31/01   E.S. and J.K.
//
//	Modified velocity calculation for velocity axis of LIF data to correct 
//	for double counting of center shift in data.  3/17/05  E.S. and X.S.
//
//	Modified difference in iodine and argon line center values based on
//  data from 7/29/05 injecting laser from HELIX and superprobe. E.S. and C.B.
//
//	Modified difference in iodine and argon ion center value based on new information
//	that iodine reference tables were in error by -0.0056 cm^-1. New line difference based
//	entirely on values from standard tables including error correction.
//
//	Modified to use Bristol wavemeter to determine range of laser scan by reading
//	wavemeter pre and post trigger. Then assume a linear extrapolation of wavelength
//	and power scans. E.S. and M. G.  2/17/2010
//
//  Modified to include laser scan up and down buttons E.S. and J.C.J 12/15/10
//	
//	Modified to use location of peak of measured LIF signal as initial guess for fitting E.S. 2/15/2011
//
//	Modified to use new DAQ-MX code for 64-bit compatibility.  E.S. 3/1/2012
//
//	Modified to use either the Bristol or the Burleigh 1500 wavemeter E.E.S. 1/21/2014
//
//	Modified so that when a continuous scan is recorded, there is a final adjustment using the iodine
//	cell to correct the absolute wavelength data record.				E.E.S. 8/11/2016
//
//	Modified to integrate with new PHASMA code structure		E.S. 5/9/2025


#include 	<rs232.h>   
#include 	"hp33120a.h"
#include 	<NIDAQmx.h>
#include 	<gpib.h>
#include 	<userint.h>
#include 	<utility.h>
#include 	<ansi_c.h>
#include 	<analysis.h>
#include 	<string.h> 
#include 	<stdio.h>
#include 	<formatio.h>
#include 	<visa.h>
#include	<Sirah.h>   
#include    "Bristol.h" 
#include	"ContLIFSettings.h"  
#include	"ContLIFSystems.h"  
#include	"GlobalVariables.h"
#include	"PHASMA-MDS-Routines.h" 
#include	"ThorLabsChopperSettings.h"
#include	"MasterTriggerSystem.h"

#define	 	speed_o_light 2.99792458E8 
#define		pi 3.141593   
#define     IFIT_PTS_CONT  100			  		// number of points to fit in iodine signal around peak when using a continuous scan
#define		Argon_wavelength    611.6616E-9		//Argon ion wavelength updated per NIST database on 10/14/2004 by E.Scime
#define		Argon_I_wavelength  696.7352E-9		//Argon neutral wavelength updated per NIST database on 9/30/2017 by E.Scime Commented 10/2/2017 for 706 nm scheme T.E.S.
//#define		Argon_I_wavelength  706.9167E-9	//Argon neutral wavelength updated per NIST database on 9/30/2017 by E.Scime
//#define		Argon_I_wavelength  714.9012E-9	//Argon neutral wavelength updated per NIST database on 9/30/2017 by E.Scime Commented 10/2/2017 for 706 nm scheme T.E.S.
#define		Helium_wavelength   587.7254E-9		//Neutral helium wavelength updated per NIST database on 7/07/2006 by E.Scime
#define		Iodine_wavelength   696.0694E-9		//Iodine ion wavelength updated per NIST database on 9/5/2017 by E.Scime
#define		Xenon_wavelength    605.2780E-9		//Xenon ion wavelength updated per NIST database on 9/5/2017 by E.Scime
#define		Iodine_to_Helium_shift 5.203  		//Based on Knockel shift correction to iodine atlas data
										  		//with large iodine peak at left of line (in frequency scan) 
										  		//centered at 587.7314 nm and the peak to the right centered at
										  		//587.7144 nm.
#define		Iodine_to_Argon_shift  1.0013 		//corrected for known frequency difference of iodine and
		    							 		//Ar lines. According to page 37 of iodine cell tables and a subsequent
		    							 		//published correction of -0.0056 cm^-1, the
		    							 		//three iodine lines typically seen around the Ar II vacuum
				    							//wavelength of 611.6616 nm (16348.9093 cm^-1) are at wave-
				    							//numbers (intensities) of 16348.7051 (23), 16348.8249 (25), 
				    							//and 16348.9427 (63) cm^-1. Converting the wavenumber difference
				    							//between the vacuum Ar II line and the iodine line to frequency
				    							//gives a frequency difference of 1.0013 GHz, not the 0.88 GHz used
				    							//in previous versions of this code.  8/23/2006 --  E.E.S.
#define		Iodine_to_Iodine_shift  0.0  		//no line identified yet  9/5/2017 --  E.E.S.
#define		Iodine_to_Xenon_shift   0.0  		//no line identified yet  9/5/2017 --  E.E.S.
#define		Iodine_to_Argon_I_shift 0.0	 		//no line identified yet  9/30/2017 --  E.E.S.
#define		LIF_max_points 4096			 		//Maximum possible array size for LIF data. For timeseries this can be quite large
#define		Wavemeter_Averages 5				//Number of measurement averages to use for wavemeter measurement calls

//Declare task handle for output card
TaskHandle OutputCardLIFTaskHandle;

int		ContinLIF_panel;
int		ContinLIF_setup_state = 14;		//unique identifier for settings of setup panel
int		Deconvolve_flag;				//Set to deconvolve timeseries
int		CLIF_RecordLength;
int		DualCLIFSwitch; 				//Flag to choose between single or dual LIF measurements
int		IFIT_PTS;
int		Wavemeter_COM_Port;
int		ShotNum; 
float	RC; 
float	PMT;
float	Source_Bfield;
float  	zeeman_freq;   // Frequency Shift due to Zeeman splitting 
double	CLIF_sweep_range;
double	CLIF_timespan;
double 	Sirah_Calibration;
double  CLIF_frequency[LIF_max_points];
double  CLIF_frequency_relative[LIF_max_points];
double  CLIF_laser_power[LIF_max_points];
double  CLIF_signal1[LIF_max_points];
double  CLIF_signal2[LIF_max_points];
double  CLIF_iodine_signal[LIF_max_points];
double  CLIF_imaginary[LIF_max_points]; 
double  CLIF_absolute_frequency[LIF_max_points];
double  CLIF_fit[LIF_max_points];
double  CLIF_fit2[LIF_max_points];
double	signal_amp;
double	signal_amp2;
double	center_freq;
double	center_freq2;
double	ion_temp;
double	ion_temp2;
double	fit_error;
double	fit_error2;	
double	center_wavelength;
double  Ifit_freq[IFIT_PTS_CONT];	// Iodine cell
double  Ifit_signal[IFIT_PTS_CONT];	// Iodine cell
double  Ifit[IFIT_PTS_CONT];		// Iodine cell
double	PIDModule_setpoint;
double	PIDModule_gain;
double	PIDModule_offset;
char	Sirah_Address[64];

int setLIAintTime (int timeconst, int tcmult, char tcu[3], int sensitivity, int sens_multiplier, char sens_units[3], int Lockin_reserve, int Lockin_slope)
{
    int	result;                 
    int tcsetting=0.0;
	int	Lockin_sens_setting=0.0;
    int itcu=0.0;
    int sunits=0.0;
    int LockIn_gpibdev1_port;
    int LockIn_gpibdev1;
    int LockIn_gpibdev2_port;
    int LockIn_gpibdev2;
   	char sendstring[11];
  
    if (result = !strcmp (tcu, "us")) itcu = 1;
    if (result = !strcmp (tcu, "ms")) itcu = 2; 
    if (result = !strcmp (tcu, "s"))  itcu = 3; 
    if (result = !strcmp (tcu, "ks")) itcu = 4; 
    

//  Determine time constant setting

    if ( timeconst == 1 )
       {
          if ( tcmult == 1)
            {
               switch (itcu)
                 {
                 
                    case 1:
                       // This setting can not be done;
                    break;
                    
                    case 2:
                       tcsetting = 4;
                    break;  
            		   
            		case 3:
                       tcsetting = 10;
                    break;  
            
            		case 4:
                       tcsetting = 16;
                    break;  
            	 }
            }
          else if ( tcmult == 10 )
            {
               switch (itcu)
                 {
                 
                    case 1:
                       tcsetting = 0;
                    break;
                    
                    case 2:
                       tcsetting = 6;
                    break;  
            		   
            		case 3:
                       tcsetting = 12;
                    break;  
            
            		case 4:
                       tcsetting = 18;
                    break;  
            	 }
            }
          else
            {
            	switch (itcu)
                 {
                 
                    case 1:
                       tcsetting = 2;
                    break;
                    
                    case 2:
                       tcsetting = 8;
                    break;  
            		   
            		case 3:
                       tcsetting = 14;
                    break;  
            
            		case 4:
                       // This setting can not be done;
                    break;
                 }  
            }
       }
     else  
       {
          if ( tcmult == 1)
            {
            	switch (itcu)
                 {
                 
                    case 1:
                       // This setting can not be done.
                    break;
                    
                    case 2:
                       tcsetting = 5;
                    break;  
            		   
            		case 3:
                       tcsetting = 11;
                    break;  
            
            		case 4:
                       tcsetting = 17;
                    break;
                 }   
            } 
          else if ( tcmult == 10 )
            {
            	switch (itcu)
                 {
                    case 1:
                       tcsetting = 1;
                    break;
                    
                    case 2:
                       tcsetting = 7;
                    break;  
            		   
            		case 3:
                       tcsetting = 13;
                    break;  
            
            		case 4:
                       tcsetting = 19;
                    break;
                  }  
            }
          else
            {
            	switch (itcu)
                 {
                 
                    case 1:
                       tcsetting = 3;
                    break;
                    
                    case 2:
                       tcsetting = 9;
                    break;  
            		   
            		case 3:
                       tcsetting = 15;
                    break;  
            
            		case 4:
                       // This setting can not be done;
                    break;
                 }   
            }
        }
        
        
	//  Determine sensitivity setting
    if (result = !strcmp (sens_units, "V"))  sunits = 4;
    if (result = !strcmp (sens_units, "mV")) sunits = 3; 
    if (result = !strcmp (sens_units, "uV")) sunits = 2; 
    if (result = !strcmp (sens_units, "nV")) sunits = 1; 
    

    switch (sunits )			  //sensitivity units
       {
           case 1:
           		switch (sens_multiplier) 
				{
					case 1:
						switch (sensitivity)
						{
							case 1:
								//this setting cannot be done
							break;
							case 2:
								Lockin_sens_setting = 0;
							break;
							case 5:
								Lockin_sens_setting = 1;
							break;
						}
					break;
					
					case 10:
						switch (sensitivity)
						{
							case 1:
								Lockin_sens_setting = 2;
							break;
							case 2:
								Lockin_sens_setting = 3;
							break;
							case 5:
								Lockin_sens_setting = 4;
							break;
						}
					break;
					
					case 100:
						switch (sensitivity)
						{
							case 1:
								Lockin_sens_setting = 5;
							break;
							case 2:
								Lockin_sens_setting = 6;
							break;
							case 5:
								Lockin_sens_setting = 7;
							break;
						}
					break;
				}	
           break;
                    
           case 2:
          		switch (sens_multiplier) 
				{
					case 1:
						switch (sensitivity)
						{
							case 1:
								Lockin_sens_setting = 8;
							break;
							case 2:
								Lockin_sens_setting = 9;
							break;
							case 5:
								Lockin_sens_setting = 10;
							break;
						}
					break;
					
					case 10:
						switch (sensitivity)
						{
							case 1:
								Lockin_sens_setting = 11;
							break;
							case 2:
								Lockin_sens_setting = 12;
							break;
							case 5:
								Lockin_sens_setting = 13;
							break;
						}
					break;
					
					case 100:
						switch (sensitivity)
						{
							case 1:
								Lockin_sens_setting = 14;
							break;
							case 2:
								Lockin_sens_setting = 15;
							break;
							case 5:
								Lockin_sens_setting = 16;
							break;
						}
					break;
				}	
           break;  
            		   
           case 3:
           		switch (sens_multiplier) 
				{
					case 1:
						switch (sensitivity)
						{
							case 1:
								Lockin_sens_setting = 17;
							break;
							case 2:
								Lockin_sens_setting = 18;
							break;
							case 5:
								Lockin_sens_setting = 19;
							break;
						}
					break;
					
					case 10:
						switch (sensitivity)
						{
							case 1:
								Lockin_sens_setting = 20;
							break;
							case 2:
								Lockin_sens_setting = 21;
							break;
							case 5:
								Lockin_sens_setting = 22;
							break;
						}
					break;
					
					case 100:
						switch (sensitivity)
						{
							case 1:
								Lockin_sens_setting = 23;
							break;
							case 2:
								Lockin_sens_setting = 24;
							break;
							case 5:
								Lockin_sens_setting = 25;
							break;
						}
					break;
				}	
           break;  
            
           case 4:
              	Lockin_sens_setting = 26;
           break;  
     }

	//Get GPIB port numbers
	GetCtrlVal (ContinLIF_panel, CLIF_PANEL_LockInGPIB1,&LockIn_gpibdev1_port);
	GetCtrlVal (ContinLIF_panel, CLIF_PANEL_LockInGPIB2,&LockIn_gpibdev2_port);

	
	//  Open Lockin Amplifier #1
 	//New syntax open command
	LockIn_gpibdev1 = ibdev (0, LockIn_gpibdev1_port, NO_SAD, T10s, 1, 0);
	
	//set parameters if valid device found
	if (LockIn_gpibdev1 > 0) {
		// set timeconstant
		sprintf(sendstring, "OFLT %d\n", tcsetting);  
		result = ibwrt (LockIn_gpibdev1, sendstring, strlen (sendstring));
	
		// set Sensitivity
		sprintf(sendstring, "SENS %d\n", Lockin_sens_setting);  
		result = ibwrt (LockIn_gpibdev1, sendstring, strlen (sendstring));
	
		// set reserve
		sprintf(sendstring, "RMOD %d\n", Lockin_reserve);  
		result = ibwrt (LockIn_gpibdev1, sendstring, strlen (sendstring));
	
		// set slope
		sprintf(sendstring, "OFSL %d\n", Lockin_slope);  
		result = ibwrt (LockIn_gpibdev1, sendstring, strlen (sendstring));
	
	  	// Close LOCKIN GPIB port using new open/close process
	 	ibonl (LockIn_gpibdev1, 0);
	}
	 
	//  Open Lockin Amplifier #2 if in use
	if (DualCLIFSwitch) {
	 	//New syntax open command
		LockIn_gpibdev2 = ibdev (0, LockIn_gpibdev2_port, NO_SAD, T10s, 1, 0);

		//set parameters if valid device found
		if (LockIn_gpibdev2 > 0) {
			// set timeconstant
			sprintf(sendstring, "OFLT %d\n", tcsetting);  
			result = ibwrt (LockIn_gpibdev2, sendstring, strlen (sendstring));
	
			// set Sensitivity
			sprintf(sendstring, "SENS %d\n", Lockin_sens_setting);  
			result = ibwrt (LockIn_gpibdev2, sendstring, strlen (sendstring));
	
			// set reserve
			sprintf(sendstring, "RMOD %d\n", Lockin_reserve);  
			result = ibwrt (LockIn_gpibdev2, sendstring, strlen (sendstring));
	
			// set slope
			sprintf(sendstring, "OFSL %d\n", Lockin_slope);  
			result = ibwrt (LockIn_gpibdev2, sendstring, strlen (sendstring));
	 	
		 	// Close LOCKIN2 GPIB port using new open/close process
		 	ibonl (LockIn_gpibdev2, 0);
		}
		
	}
	return(0);
  
  }

void OpenContinLIF_Settings (void)
{
	int	shotnum;
	
	ContinLIF_panel = LoadPanel (0, "ContLIFSettings.uir", CLIF_PANEL);
	RecallPanelState (ContinLIF_panel, "Master_Control_Storage_File", ContinLIF_setup_state);
	
	//Update MDS shot number
	shotnum=getMDSCurrentShot();
		
	//Display Panel
	DisplayPanel (ContinLIF_panel);
	
	//Update shot number in panel
	SetCtrlVal (ContinLIF_panel, CLIF_PANEL_MDS_Shot,shotnum);
	
	// Start interacting with user
    RunUserInterface ();
}


int CVICALLBACK Close_CLIF_Settings (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	int 	result;
	int		AOM_Switch;
	short	HP_GPIB_port;
	char	ThorChopperCOM_port[64];
	short	HP33120_GPIB;

	switch (event)
	{
		case EVENT_COMMIT:
			//Shut down any instruments started up for LIF
			
			//Get AOM Switch setting
			GetCtrlVal (ContinLIF_panel, CLIF_PANEL_AOMSwitch,&AOM_Switch);
			
			//Shut down AOM or Mechanical Chopper
			if (AOM_Switch) {
				//Get GPIB address for HP33120a function generator
				GetCtrlVal (ContinLIF_panel, CLIF_PANEL_RigolFunct,&HP_GPIB_port);
			
				//Open function generator
				result = hp33120a_init (HP_GPIB_port, 0, 0, &HP33120_GPIB);

				//Set up device
				result = hp33120a_wf_config (HP33120_GPIB, 2, "Chopper", 1.0, 0.0, 0, 0.0, 50);  // modified by Peiyun, Nov 30, 2020

				// Close device
				result = hp33120a_close (HP33120_GPIB);
				}
			else {
				//Get COM port Address for ThorLabs Chopper
				GetCtrlVal (ContinLIF_panel, CLIF_PANEL_ThorChopperCOM,ThorChopperCOM_port);

				//Set Chopper Frequency
				disable_Thor_Chopper(ThorChopperCOM_port); 
			}
			
			//Restore master trigger capability to NI DAQ card
			SetUpNI_Card_Trig();
	
			//Save current state of panel and close interface
			SavePanelState (ContinLIF_panel, "Master_Control_Storage_File", ContinLIF_setup_state);
			DiscardPanel(panel);
			break;
	}
	return 0;
}


void ContinLIF_Activate(void)
{
	int		Lockin_reserve;		   			// SRS lock in reserve 
	int		Lockin_slope;		   			// SRS lock in slope
	int		sensitivity;					// SRS lock in sensitivity parameter 
	int		sens_multiplier;				// SRS lock in sensitivity parameter 
	int		AOM_Switch;
	int		result;
	short	HP_GPIB_port;
	char	ThorChopperCOM_port[64];
	char	Wavemeter_COM_string[64];
	char	tcu[5];							// Default time base 
	char	sens_units[5];					// Default sensitivty units
	char    machine_string[16];
	char 	orient_string[30];
	char	dummy[8];
	short	HP33120_GPIB;
	float   timeconst;						// Default time const
	float	tcmult;							// Default time const multiplier 
	double	Chopper_Freq;
	double	LIF_scanning_drate;

	// Set 1V or 2V switch to indicate single or dual LIF measurements
	GetCtrlVal (ContinLIF_panel, CLIF_PANEL_DualLIFSwitch,&DualCLIFSwitch);

	//	SetUp Lockin Amplifier 
	GetCtrlVal (ContinLIF_panel, CLIF_PANEL_timeconstant,&timeconst);
	GetCtrlVal (ContinLIF_panel, CLIF_PANEL_multiplier,&tcmult);
	GetCtrlVal (ContinLIF_panel, CLIF_PANEL_units,tcu);
	GetCtrlVal (ContinLIF_panel, CLIF_PANEL_Reserve,&Lockin_reserve);
	GetCtrlVal (ContinLIF_panel, CLIF_PANEL_Slope,&Lockin_slope);
	GetCtrlVal (ContinLIF_panel, CLIF_PANEL_sensitivity,&sensitivity);
	GetCtrlVal (ContinLIF_panel, CLIF_PANEL_sens_multiplier,&sens_multiplier);
	GetCtrlVal (ContinLIF_panel, CLIF_PANEL_sens_units,sens_units);
	
	setLIAintTime (timeconst, tcmult, tcu, sensitivity,sens_multiplier, sens_units,Lockin_reserve, Lockin_slope);
	
	//Define timeconstant for use later
	switch(tcu[0])
		{
		case 'k':
			RC=timeconst*tcmult*1000;
			break;
		case 's':
			RC=timeconst*tcmult*1;
			break;
		case 'm':
			RC=timeconst*tcmult*1.0E-3;
			break;
		case 'u':
			RC=timeconst*tcmult*1.0E-6;
			break;
		}

	//Get local magnetic field supplied by user
	GetCtrlVal (ContinLIF_panel, CLIF_PANEL_Local_Bfield,&Source_Bfield);

	//Get Chopper Frequency
	GetCtrlVal (ContinLIF_panel, CLIF_PANEL_ChopperFreq,&Chopper_Freq);
	
	//Get AOM Switch setting
	GetCtrlVal (ContinLIF_panel, CLIF_PANEL_AOMSwitch,&AOM_Switch);
	
	//Set up AOM or Mechanical Chopper
	if (AOM_Switch) {
		//Get GPIB address for HP33120a function generator
		GetCtrlVal (ContinLIF_panel, CLIF_PANEL_RigolFunct,&HP_GPIB_port);
	
		//Open function generator
		result = hp33120a_init (HP_GPIB_port, 0, 0, &HP33120_GPIB);

		//Set up device
		result = hp33120a_wf_config (HP33120_GPIB, 2, "Chopper", Chopper_Freq, 3.0, 0, 1.5, 50);  // modified by Peiyun, Nov 30, 2020

		// Close device
		result = hp33120a_close (HP33120_GPIB);
		}
	else {
		//Get COM port Address for ThorLabs Chopper
		GetCtrlVal (ContinLIF_panel, CLIF_PANEL_ThorChopperCOM,ThorChopperCOM_port);

		//Set Chopper Frequency
		setup_Thor_Chopper(ThorChopperCOM_port, Chopper_Freq); 
	}

	//	Get Sweep Range for LIF	 
	GetCtrlVal (ContinLIF_panel, CLIF_PANEL_SweepRange,&CLIF_sweep_range);
	 
	//	Get Record Length	 
	GetCtrlVal (ContinLIF_panel, CLIF_PANEL_DAQ_RecordLength,&CLIF_RecordLength);

	//	Set up Length of Sweep Time for Laser and calculate sample rate	 
	GetCtrlVal (ContinLIF_panel, CLIF_PANEL_SweepTime,&CLIF_timespan);

	//Set the deconvolution flag to be what is displayed on the panel
	GetCtrlVal (ContinLIF_panel, CLIF_PANEL_Deconvolve_active, &Deconvolve_flag);
	
	//	Get laser conversion factor	 
	GetCtrlVal (ContinLIF_panel, CLIF_PANEL_Sirah_Calibration,&Sirah_Calibration);
		
	//	Get Sirah Laser IP address	 
	GetCtrlVal (ContinLIF_panel, CLIF_PANEL_SIRAH_IP,Sirah_Address);
		
	//Set Sirah laser scan parameters
	Sirah_Laser_SetScan(Sirah_Address,CLIF_timespan,CLIF_sweep_range,Sirah_Calibration);

	//Create Internal DAQ Card Task for continuous LIF measurement
	DAQmxCreateTask ("", &OutputCardLIFTaskHandle);
	DAQmxCreateAIVoltageChan (OutputCardLIFTaskHandle, "Dev1/ai0:7", "Channel0,Channel1,Channel2,Channel3,Channel4,Channel5,Channel6,Channel7", DAQmx_Val_RSE, -5.0, 5.0, DAQmx_Val_Volts, "");
		
	//Use onboard clock for digitization and set rate based on LIF continuous scan recording time
	LIF_scanning_drate=(double)CLIF_RecordLength/CLIF_timespan;
	DAQmxCfgSampClkTiming (OutputCardLIFTaskHandle, "OnboardClock", LIF_scanning_drate, DAQmx_Val_Rising, DAQmx_Val_FiniteSamps, CLIF_RecordLength);   
		
	//To eliminate need for VXI to create a trigger pulse, code updated to initiate measurement immediately upon start of 
	//DAQ card task. 2/14/2017 - E.E.S.
	DAQmxDisableStartTrig (OutputCardLIFTaskHandle);
	
	//Get Wavemeter COM Port number string 
	GetCtrlVal (ContinLIF_panel, CLIF_PANEL_Wavemeter_COM,Wavemeter_COM_string);
	
	//Strip out COM address from string
	sscanf(Wavemeter_COM_string,"%3s %d",dummy,&Wavemeter_COM_Port);
	

}


double ShiftedMaxArII(double x, double coef[], int ncoef)
// Fit for dye laser Argon ion line
{
	double y;
	
	y=coef[3]+coef[0]*exp(-0.07794*(x-coef[1])*(x-coef[1])/coef[2]);
		
    return y;
}


double ShiftedMaxHeI(double x, double coef[], int ncoef)
// Fit for Helium Neutral line 
{
	double y;
	
	y=coef[3]+coef[0]*exp(-0.00718*(x-coef[1])*(x-coef[1])/coef[2]);
	
    return y;
}


double ShiftedMaxII(double x, double coef[], int ncoef)
// Fit for molecular iodine line for fitting iodine spectrum (width is an arbitrary guess)
{
	double y;
	
	y=coef[3]+coef[0]*exp(-0.003*(x-coef[1])*(x-coef[1])/coef[2]);
	
    return y;
}

double ShiftedMaxIII(double x, double coef[], int ncoef)
// Fit for iodine ion line
{
	double y;
	
	y=coef[3]+coef[0]*exp(-0.31864*(x-coef[1])*(x-coef[1])/coef[2]);
	
    return y;
}

double ShiftedMaxXeII(double x, double coef[], int ncoef)
// Fit for xenon ion line
{
	double y;
	
	y=coef[3]+coef[0]*exp(-0.24927*(x-coef[1])*(x-coef[1])/coef[2]);
	
    return y;
}

double ShiftedMaxArI(double x, double coef[], int ncoef)
// Fit for argon neutral line at whichever line used
{
	double y;
	double scaling;
	
	scaling = 0.000000207017284*Argon_I_wavelength*Argon_I_wavelength*1.0E18;
	
	y=coef[3]+coef[0]*exp(-scaling*(x-coef[1])*(x-coef[1])/coef[2]);
	
    return y;
}

void CLIF_plot(void)
{
	ssize_t    	Imax_index,Imin_index; 
	int			j;  
   	ssize_t    	LIFmax_index,LIFmin_index;
	double  	LIFmax=0.0, LIF2max=0.0,LIFmin=0.0;
	double  	Freqmax, Freqmin;
  	double		scaled_iodine[LIF_max_points];
	double  	Imax, Imin;
  
	//Clear previous LIF plot
	DeleteGraphPlot (ContinLIF_panel, CLIF_PANEL_LIF_PLOT, -1,VAL_IMMEDIATE_DRAW);

	//Find frequency range
	MaxMin1D (CLIF_frequency_relative, CLIF_RecordLength, &Freqmax, &LIFmax_index, &Freqmin,&LIFmin_index);	
		
	//Plot LIF data and fit plot to screen
	if (DualCLIFSwitch) {   //If doing 2V LIF
				
		//scale 2nd LIF measurement to match first measurement, thereby making visualization easier
		MaxMin1D (CLIF_signal2, CLIF_RecordLength, &LIF2max, &LIFmax_index, &LIFmin,&LIFmin_index);				
		MaxMin1D (CLIF_signal1, CLIF_RecordLength, &LIFmax, &LIFmax_index, &LIFmin,&LIFmin_index);				
		for (j=0;j<CLIF_RecordLength;j++) {
			CLIF_signal2[j]=CLIF_signal2[j]*(LIFmax/LIF2max);
		}
		SetAxisRange (ContinLIF_panel, CLIF_PANEL_LIF_PLOT, VAL_MANUAL, Freqmin, Freqmax,VAL_MANUAL, 0.0, 1.25*LIFmax);
		PlotXY (ContinLIF_panel, CLIF_PANEL_LIF_PLOT, CLIF_frequency_relative, CLIF_signal1, CLIF_RecordLength, VAL_DOUBLE,
			VAL_DOUBLE, VAL_SCATTER, VAL_SOLID_CIRCLE, VAL_SOLID, 1, VAL_DK_BLUE);
		PlotXY (ContinLIF_panel, CLIF_PANEL_LIF_PLOT, CLIF_frequency_relative, CLIF_signal2, CLIF_RecordLength, VAL_DOUBLE, 
			VAL_DOUBLE, VAL_SCATTER, VAL_SOLID_SQUARE, VAL_SOLID,1, VAL_DK_CYAN);
	} else { //if not doing 2V LIF
				
		MaxMin1D (CLIF_signal1, CLIF_RecordLength, &LIFmax, &LIFmax_index, &LIFmin,&LIFmin_index);
		if (LIFmax > 0) {
			SetAxisRange (ContinLIF_panel, CLIF_PANEL_LIF_PLOT, VAL_MANUAL, Freqmin, Freqmax,VAL_MANUAL, 0.0, 1.25*LIFmax);
		} else {
			SetAxisRange (ContinLIF_panel, CLIF_PANEL_LIF_PLOT, VAL_MANUAL, Freqmin, Freqmax, VAL_AUTOSCALE, 0.0, 0.0);
		}	
		PlotXY (ContinLIF_panel, CLIF_PANEL_LIF_PLOT, CLIF_frequency_relative, CLIF_signal1, CLIF_RecordLength, VAL_DOUBLE,
			VAL_DOUBLE, VAL_SCATTER, VAL_SOLID_CIRCLE, VAL_SOLID, 1, VAL_DK_BLUE);
	}
	//Plot the fit to the LIF data
	PlotXY (ContinLIF_panel, CLIF_PANEL_LIF_PLOT, CLIF_frequency_relative, CLIF_fit, CLIF_RecordLength, VAL_DOUBLE, VAL_DOUBLE,
		VAL_FAT_LINE, VAL_SIMPLE_DOT, VAL_SOLID, 1, VAL_RED);
		
 	//If doing 2V LIF plot the second fit
	if (DualCLIFSwitch) {   
		//scale 2nd LIF measurement to match first measurement, thereby making visualization easier
		for (j=0;j<CLIF_RecordLength;j++) {
			CLIF_fit2[j]=CLIF_fit2[j]*(LIFmax/LIF2max);
		}
		PlotXY (ContinLIF_panel, CLIF_PANEL_LIF_PLOT, CLIF_frequency_relative, CLIF_fit2, CLIF_RecordLength, VAL_DOUBLE, 
			VAL_DOUBLE, VAL_FAT_LINE, VAL_SIMPLE_DOT, VAL_SOLID, 1, VAL_CYAN);
	}
			
	//Plot iodine signal and the fit
	MaxMin1D (CLIF_iodine_signal,CLIF_RecordLength, &Imax, &Imax_index, &Imin,&Imin_index);				
	for (j = 0; j<CLIF_RecordLength; j++) {
		if ((LIFmax>0) && (Imax > 0)) {
			scaled_iodine[j]=CLIF_iodine_signal[j]*(LIFmax/Imax);
		}
		else {
			scaled_iodine[j]=CLIF_iodine_signal[j];
		}
	}
	PlotXY (ContinLIF_panel, CLIF_PANEL_LIF_PLOT, CLIF_frequency_relative, scaled_iodine, CLIF_RecordLength, VAL_DOUBLE,
		VAL_DOUBLE, VAL_THIN_LINE, VAL_SOLID_SQUARE, VAL_SOLID, 1, VAL_GREEN);
	for (j = 0; j<IFIT_PTS; j++) {
		if ((LIFmax>0) && (Imax > 0)) {
			Ifit_signal[j]=Ifit_signal[j]*(LIFmax/Imax);
		}
	}
	PlotXY (ContinLIF_panel, CLIF_PANEL_LIF_PLOT, Ifit_freq, Ifit_signal, IFIT_PTS, VAL_DOUBLE, VAL_DOUBLE,
			VAL_FAT_LINE, VAL_SOLID_SQUARE, VAL_SOLID, 1, VAL_DK_GRAY);
}



void CLIF_analyze(void)

/*	2/4/98  - Changed nonlinear fit routines to include background as a free parameter
	2/16/98 - Changed routine to use a manual instrument handle search to be 
				consistent with other subroutines
	2/16/98	- Changed routine to average signals onboard the instrument
	6/29/98	- Completely rewrote to just analyze data as acquired from 16 bit digitizer
	6/24/99 - Allowed for Zeeman correction if LEIA parallel data acquired in argon
	7/07/06 - Updated flow analysis and iodine cell fits for He I LIF measurements
	5/9/25	- Removed all Zeeman shift and F1 analysis and updated to be compatible with PHASMA DAQ code
*/


{
   	int		j;
	int		i;
 	double 	Icoef[4];				  	// Iodine cell
 	double	mean_sq_err;
	double 	coef[4];
   	double  Imax, Imin;
   	double  LIFmax, LIFmin;
	double  S1max, S1min;
   	double  S2max, S2min;
   	double  S3max, S3min;
   	double	Middle_Frequency=0.0;
	ssize_t Imax_index,Imin_index;
	ssize_t LIFmax_index,LIFmin_index;
	ssize_t S1max_index,S1min_index;
	ssize_t S2max_index,S2min_index;
	ssize_t S3max_index,S3min_index;
	char	headerstring[50];  


	// Calculate center frequency to then subtract from frequency scan so fit is in relative units to center
	switch (species) {
		case 1:   //calculate center wavelength for argon ions   
			Middle_Frequency=speed_o_light/Argon_wavelength/1.0E9;
			break;
		case 2:   //calculate center wavelength for helium neutrals   
			Middle_Frequency=speed_o_light/Helium_wavelength/1.0E9;
			break;
		case 4:	   //calculate center wavelength for iodine ions  
			break;
		case 5:	   //calculate center wavelength for xenon ions  
			Middle_Frequency=speed_o_light/Xenon_wavelength/1.0E9;
			break;
		case 6:	   //calculate center wavelength for neutral argon  
			Middle_Frequency=speed_o_light/Argon_I_wavelength/1.0E9;
			break;
		default: Middle_Frequency=(CLIF_frequency[CLIF_RecordLength/2+1]+CLIF_frequency[CLIF_RecordLength/2])/2.0;
	}  //end case

	//Find the lowest value in the LIF signal arrays to subtract off DC level in signal
	MaxMin1D (CLIF_signal1, CLIF_RecordLength, &S1max, &S1max_index, &S1min,&S1min_index);
	MaxMin1D (CLIF_signal2,CLIF_RecordLength, &S2max, &S2max_index, &S2min,&S2min_index);
	
	//Subtract any DC offset from LIF signals and create frequency array centered on rest frame frequency of relevant line 
	for (j=0; j<CLIF_RecordLength; j++) {
	   	CLIF_frequency_relative[j]=CLIF_frequency[j]- Middle_Frequency;  //subtract off reference frequency for the scan
		CLIF_signal1[j]=CLIF_signal1[j]-S1min;
		CLIF_signal2[j]=CLIF_signal2[j]-S2min;
	}
		
 	//  Fit iodine cell data
	//	Iodine cell
	MaxMin1D (CLIF_iodine_signal,CLIF_RecordLength, &Imax, &Imax_index, &Imin,&Imin_index);

	//Update the number of points to use in iodine fit based on discrete or continuous data record
	IFIT_PTS= IFIT_PTS_CONT;
			 
	// Create array to fit around peak
	for( i=0; i<IFIT_PTS_CONT; i++) {
		if (Imax_index-IFIT_PTS_CONT > 0) {
			if ( (Imax_index-IFIT_PTS_CONT/2+i) > CLIF_RecordLength-1) {
				IFIT_PTS=i;
			}  else {
			  	Ifit_freq[i]=CLIF_frequency_relative[Imax_index-IFIT_PTS_CONT/2+i]; 
	    		Ifit_signal[i]=CLIF_iodine_signal[Imax_index-IFIT_PTS_CONT/2+i];
			}
		} else {
			Ifit_freq[i]=0.0; 
	    	Ifit_signal[i]=0.0;
		}	
	}
			
	// Make initial guesses	
	Icoef[0]=Imax;						   // Amplitude 
	Icoef[1]=Ifit_freq[IFIT_PTS_CONT/2+1];  // Center 
	Icoef[2]=0.05;						   // Temperature 
	Icoef[3]=0.0;						   // Background Level 
		
	// Fit to peak
	NonLinearFit (Ifit_freq, Ifit_signal, Ifit, IFIT_PTS_CONT, ShiftedMaxII, Icoef, 4, &mean_sq_err);

	switch (species) {
		case 1:   //Argon fit

			//Correct frequency axis based on location of peak of iodine cell using tabulated values
			//Icoef[1] is the center of the fit in relative frequency coordinate. So location of peak
			//is Icoef[1]+Middle_Frequency. The true value is 490128.97177 GHz. Any difference needs to
			//be added to the absolute frequency data record get as close to perfect as we can.
			//So LIF_absolute_frequency=LIF_absolute_frequency - (Icoef[1]+Middle_frequency-490128.97177)
			for (j=0; j<CLIF_RecordLength; j++) {
				CLIF_absolute_frequency[j]=CLIF_frequency_relative[j]-Icoef[1]+490128.97177;  //store absolute frequency info for printing
	    	}
			break;
		case 2:   //For He, 
			for (j=0; j<CLIF_RecordLength; j++) {
				CLIF_absolute_frequency[j]=CLIF_frequency[j]+ Middle_Frequency;;   //go back to actual measurement since no reference molecular iodine line
		   	}
		    break;
		case 4:	   //Iodine Fit  
			for (j=0; j<CLIF_RecordLength; j++) {
				CLIF_absolute_frequency[j]=CLIF_frequency[j]+ Middle_Frequency;;   //go back to actual measurement since no reference molecular iodine line
		   	}
		    break;
		case 5:	   //Xenon Fit  
			for (j=0; j<CLIF_RecordLength; j++) {
				CLIF_absolute_frequency[j]=CLIF_frequency[j]+ Middle_Frequency;;   //go back to actual measurement since no reference molecular iodine line
		   	}
		    break;
		case 6:	   //Neutral Argon Fit  
			for (j=0; j<CLIF_RecordLength; j++) {
				CLIF_absolute_frequency[j]=CLIF_frequency[j]+ Middle_Frequency;;   //go back to actual measurement since no reference molecular iodine line
		   	}
		    break;
	    default: center_wavelength=0.0;
	}  //end case
	
     // Find max of LIF single to begin fitting LIF signal
	MaxMin1D (CLIF_signal1, CLIF_RecordLength, &LIFmax, &LIFmax_index, &LIFmin,&LIFmin_index);

	// 	Perform Shifted Maxwellian Fit to Data
	switch (species) {
		case 1: //Argon fit
			// Create initial guesses and perform fit
			coef[0]=LIFmax;						   				// Amplitude 
			coef[1]=CLIF_frequency_relative[LIFmax_index];	   	// Center 
			coef[2]=0.2;						   				// Temperature 
			coef[3]=0.0;						   				// Background Level 
			NonLinearFit (CLIF_frequency_relative, CLIF_signal1, CLIF_fit, CLIF_RecordLength, ShiftedMaxArII, 
			    	coef, 4, &mean_sq_err);
		    center_freq=(coef[1]-Icoef[1])+Iodine_to_Argon_shift;		   //caculate the difference in iodine and argon peaks in frequency, then add in known offset
			sprintf(headerstring,"Dye Frequency Difference %f GHz\n",center_freq); 
			
			// 	Store Data in Variables	
		    signal_amp=coef[0];
			ion_temp=coef[2];
			fit_error=mean_sq_err;
	
			//if doing 2V LIF, redo the entire analysis for second measurement array
			if (DualCLIFSwitch) {  
				//	Argon fit
				//  Use same variables used to find max of iodine so naming is stupid
				MaxMin1D (CLIF_signal2, CLIF_RecordLength, &LIFmax, &LIFmax_index, &LIFmin,&LIFmin_index);
				coef[0]=LIFmax;						   				// Amplitude 
				coef[1]=CLIF_frequency_relative[LIFmax_index];	   	// Center 
				coef[2]=0.2;						   				// Temperature 
				coef[3]=0.0;						   				// Background Level 
				NonLinearFit (CLIF_frequency_relative, CLIF_signal2, CLIF_fit2, CLIF_RecordLength, ShiftedMaxArII, 
				    	coef, 4, &mean_sq_err);
			    center_freq=(coef[1]-Icoef[1])+Iodine_to_Argon_shift;		   //caculate the difference in iodine and argon peaks in frequency, then add in known offset
				sprintf(headerstring,"Argon ivdf Frequency Difference %f GHz\n",center_freq); 

			    signal_amp2=coef[0];
				ion_temp2=coef[2];
				fit_error2=mean_sq_err;
			}
			break;
		case 2:   //He I fit 
			coef[0]=LIFmax;						   				// Amplitude 
			coef[1]=CLIF_frequency_relative[LIFmax_index];	   	// Center 
			coef[2]=0.05;						   				// Temperature 
			coef[3]=0.0;						   				// Background Level 
			NonLinearFit (CLIF_frequency_relative, CLIF_signal1, CLIF_fit, CLIF_RecordLength, ShiftedMaxHeI, 
			    	coef, 4, &mean_sq_err);

		    // 	Store Data in Variables	
			signal_amp=coef[0];
			ion_temp=coef[2];
			fit_error=mean_sq_err;
			center_freq=(coef[1]-Icoef[1])+Iodine_to_Helium_shift;
			sprintf(headerstring,"Helium Frequency Difference %f GHz\n",center_freq); 
		   break;
		case 4:	   //Iodine Fit  
			coef[0]=LIFmax;						   				// Amplitude 
			coef[1]=CLIF_frequency_relative[LIFmax_index];	   	// Center 
			coef[2]=0.05;						   				// Temperature 
			coef[3]=0.0;						   				// Background Level 
			NonLinearFit (CLIF_frequency_relative, CLIF_signal1, CLIF_fit, CLIF_RecordLength, ShiftedMaxArII, 
			    	coef, 4, &mean_sq_err);

		    // 	Store Data in Variables	
			signal_amp=coef[0];
			ion_temp=coef[2];
			fit_error=mean_sq_err;
			center_freq=coef[1];
			sprintf(headerstring,"Iodine Frequency Difference %f GHz\n",center_freq); 

		   break;
		case 5:	   //Xenon Fit  
			coef[0]=LIFmax;						   				// Amplitude 
			coef[1]=CLIF_frequency_relative[LIFmax_index];	   	// Center 
			coef[2]=0.05;						   				// Temperature 
			coef[3]=0.0;						   				// Background Level 
			NonLinearFit (CLIF_frequency_relative, CLIF_signal1, CLIF_fit, CLIF_RecordLength, ShiftedMaxArII, 
			    	coef, 4, &mean_sq_err);

	       // 	Store Data in Variables	
		   signal_amp=coef[0];
		   ion_temp=coef[2];
		   fit_error=mean_sq_err;
		   center_freq=coef[1];
		   sprintf(headerstring,"Xenon Frequency Difference %f GHz\n",center_freq); 
		   break;
		case 6:	   //Neutral Argon Fit  
			coef[0]=LIFmax;						   				// Amplitude 
			coef[1]=CLIF_frequency_relative[LIFmax_index];	   	// Center 
			coef[2]=0.05;						   				// Temperature 
			coef[3]=0.0;						   				// Background Level 
			NonLinearFit (CLIF_frequency_relative, CLIF_signal1, CLIF_fit, CLIF_RecordLength, ShiftedMaxArII, 
			    	coef, 4, &mean_sq_err);

	       // 	Store Data in Variables	
		   signal_amp=coef[0];
		   ion_temp=coef[2];
		   fit_error=mean_sq_err;
		   center_freq=coef[1];
		   sprintf(headerstring,"Argon Neutral Frequency Difference %f GHz\n",center_freq); 
		   break;
		default: center_wavelength=0.0;
	}  //end case
		
	// 	Calculate the argon Zeeman splitting in the Parallel Direction for the magnetic field specified by the user
	//	Convert SOURCE magnetic field to kilogauss 
	zeeman_freq = 1.399 * (double)(Source_Bfield)/1000.0;			  //correction for Zeeman at 611.6616nm is 1.399 GHz/kGauss
	
	//Print Zeeman offset and fit results into LIF panel
	SetCtrlVal (ContinLIF_panel, CLIF_PANEL_Zeeman_Shift,zeeman_freq); 
	SetCtrlVal (ContinLIF_panel, CLIF_PANEL_CenterShift1,center_freq);  
	SetCtrlVal (ContinLIF_panel, CLIF_PANEL_Ti1,ion_temp);  
	SetCtrlVal (ContinLIF_panel, CLIF_PANEL_CenterShift2,center_freq2);  
	SetCtrlVal (ContinLIF_panel, CLIF_PANEL_Ti2,ion_temp2);  
}



void write_CLIF_data(void)
{
	int		j;
	char	ChannelNameString[36];
	char	outfilename[36];
	double	System_Parameters[36]={0};
	FILE*	outfile;

	//Store PID module info into system parameters array
	GetCtrlVal (ContinLIF_panel, CLIF_PANEL_PIDModule_setpoint,&System_Parameters[0]);
	GetCtrlVal (ContinLIF_panel, CLIF_PANEL_PIDModule_offset,&System_Parameters[1]);
	GetCtrlVal (ContinLIF_panel, CLIF_PANEL_PIDModule_gain,&System_Parameters[2]);
	GetCtrlVal (ContinLIF_panel, CLIF_PANEL_Machine,&System_Parameters[3]);
	GetCtrlVal (ContinLIF_panel, CLIF_PANEL_PMT,&System_Parameters[4]);
  	GetCtrlVal (ContinLIF_panel, CLIF_PANEL_Orientation,&System_Parameters[5]);
	GetCtrlVal (ContinLIF_panel, CLIF_PANEL_timeconstant,&System_Parameters[6]);
	GetCtrlVal (ContinLIF_panel, CLIF_PANEL_multiplier,&System_Parameters[7]);
	GetCtrlVal (ContinLIF_panel, CLIF_PANEL_Reserve,&System_Parameters[9]);
	GetCtrlVal (ContinLIF_panel, CLIF_PANEL_Slope,&System_Parameters[10]);
	GetCtrlVal (ContinLIF_panel, CLIF_PANEL_sensitivity,&System_Parameters[11]);
	GetCtrlVal (ContinLIF_panel, CLIF_PANEL_sens_multiplier,&System_Parameters[12]);
	GetCtrlVal (ContinLIF_panel, CLIF_PANEL_sens_units,&System_Parameters[13]);
 	GetCtrlVal (ContinLIF_panel, CLIF_PANEL_SweepRange,&System_Parameters[14]);
	GetCtrlVal (ContinLIF_panel, CLIF_PANEL_DAQ_RecordLength,&System_Parameters[15]);
 	GetCtrlVal (ContinLIF_panel, CLIF_PANEL_SweepTime,&System_Parameters[16]);
 	GetCtrlVal (ContinLIF_panel, CLIF_PANEL_Deconvolve_active, &System_Parameters[17]);
	System_Parameters[18]= signal_amp;
	System_Parameters[19]= signal_amp2;
	System_Parameters[20]= center_freq;
	System_Parameters[21]= center_freq2;
	System_Parameters[22]= ion_temp; 
	System_Parameters[23]= ion_temp2;
	System_Parameters[24]= fit_error;
	System_Parameters[25]= fit_error2;
	System_Parameters[26]= center_wavelength;
	GetCtrlVal (ContinLIF_panel, CLIF_PANEL_Local_Bfield,&System_Parameters[27]);
  	System_Parameters[28]= zeeman_freq;
	
	
	//Get current shot number
	ShotNum=getMDSCurrentShot();

		
	//Dump LIF results to a file
	sprintf(outfilename, RawDataPath);
	strcat(outfilename, ShotNumberString);
	sprintf(outfilename, ShotNumberString);
	strcat(outfilename, "_");
	strcat(outfilename, "CLIF.txt");
	
	outfile=fopen (outfilename, "w");
	//Store system parameters in first 36 lines
	for (j=0;j<36;j++) {
		fprintf(outfile,"%f,\n",System_Parameters[j]);
	}
	
	//Store raw LIF signals
	for (j=0;j<CLIF_RecordLength;j++) {
		fprintf(outfile,"%f,%f,%f,%f,%f,%f\n",CLIF_frequency_relative[j], CLIF_signal1[j],CLIF_signal2[j],CLIF_fit[j],CLIF_fit2[j], CLIF_iodine_signal[j]);
	}


}		    	

 void write_CLIF_data_MDS(void)
{
	char	ChannelNameString[36];
	FILE*	outfile;

	//Get current shot number
	ShotNum=getMDSCurrentShot();
	
	//Write an array of all the instrument write-t-mds flags to a spreadsheet to maintain a permanent list
	//of which data are stored for each shot. Include the shot number, the date, and all the flags.
	outfile = fopen (PHASMA_LOG_String, "a");
	fprintf(outfile,"%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n",ShotNum,0, 0,0,0,0,
		0,0,0,0,0, 1,0, 0,0,0,0,0,0,0,0);

 	//Close magnetic field profile data file
	fclose (outfile);

	//Get channel name string, store raw data on drive folder, and push data to MDSPlus for Channel 1
	GetCtrlVal (ContinLIF_panel, CLIF_PANEL_Chan1Name,ChannelNameString);
	Write_to_PHASMA_MDS(CLIF_frequency_relative, CLIF_signal1,ChannelNameString);

	//Get channel name string, store raw data on drive folder, and push data to MDSPlus for Channel 3
	GetCtrlVal (ContinLIF_panel, CLIF_PANEL_Chan2Name,ChannelNameString);
	Write_to_PHASMA_MDS(CLIF_frequency_relative, CLIF_signal2,ChannelNameString);

	//Get channel name string, store raw data on drive folder, and push data to MDSPlus for Channel 3
	GetCtrlVal (ContinLIF_panel, CLIF_PANEL_Chan3Name,ChannelNameString);
	Write_to_PHASMA_MDS(CLIF_frequency_relative, CLIF_iodine_signal,ChannelNameString);
}		    	

 
void CLIF_acquire(void)
//August 20, 2015	Radical rewrite of routine. This routine now goes back to using a continously recorded timeseries
//					measurement of the iodine cell, the wavelength from the wavemeter PID module, and the lockin signal.
//					The Bristol or Burleigh wavemeter is ued for the wavelength measurement. 
//
//May 9, 2025		Rewritten entirely to work with new PHASMA DAQ system

{
	int 	j;
 	int		actual_read;
	int		max_loc,min_loc;
	int		i,k;
	int		result;
	int		ContinLIF_DataFlag;
	double	Hold_array[8*LIF_max_points];
	double	freq_time_axis[LIF_max_points]={0};
	double 	Fitted_CLIF_Frequency[LIF_max_points];
	double	Freq_fit_slope;
	double	Freq_fit_intercept;
	double	Freq_fit_error;
    char	headerstring[50];  	
	double	omega;
	double	filter_real;
	double 	filter_imag;
 	double  power_max,power_min;
  	double  Iodine_max, Iodine_min;
 	double  Imax, Imin;
	ssize_t Imax_index,Imin_index;

	//zero arrays from previous measurements
	for (j=0;j<CLIF_RecordLength;j++) {
 		CLIF_frequency[j]=0.0;
		CLIF_laser_power[j]=0.0;
		CLIF_signal1[j]=0.0;	
		CLIF_signal2[j]=0.0;	
		CLIF_iodine_signal[j]=0.0;
		freq_time_axis[j]=j*1.0;
	}

	//Trigger LIF Scan and start up DAQ card recording at same time
	Sirah_Laser_Scan (Sirah_Address);
	DAQmxStartTask (OutputCardLIFTaskHandle);
	
	 //Get LIF signals from channels 0,2,4,6,8 of I/O card using taskhandle. Code holds here until measurement is complete plus 1 second
	result = DAQmxReadAnalogF64(OutputCardLIFTaskHandle, CLIF_RecordLength, (CLIF_timespan+1), DAQmx_Val_GroupByChannel, Hold_array, 8*CLIF_RecordLength, &actual_read, 0);
	//Stop Task
	DAQmxStopTask (OutputCardLIFTaskHandle);
	//Sort data into correct signal arrays, notice real values interwoven with the empty channels to suppress cross-over read error
	for (j=0;j<CLIF_RecordLength;j++) {
		CLIF_frequency[j]=Hold_array[j];	
		CLIF_signal1[j]=Hold_array[j+2*CLIF_RecordLength];	
		CLIF_iodine_signal[j]=Hold_array[j+4*CLIF_RecordLength];	
		CLIF_signal2[j]=Hold_array[j+6*CLIF_RecordLength];	
	}

	// Do a linear fit to the wavelength data here and using a recursive method to eliminate outliers.
	// First fit, then remove points more than 10% from fit by replacing them with fit values, then fit again
	// and remove points more than a few percent from fit by replacing them with fit. Then fit again and use the fit
	// to overwrite the wavelength data to create a new and smoothed wavelength array.
	LinFit (freq_time_axis, CLIF_frequency, CLIF_RecordLength, Fitted_CLIF_Frequency, &Freq_fit_slope, &Freq_fit_intercept, &Freq_fit_error);
	for (j=0;j<CLIF_RecordLength;j++) {
		if ( (fabs(Fitted_CLIF_Frequency[j])) > 0) {
			if ( (fabs((CLIF_frequency[j]-Fitted_CLIF_Frequency[j])/Fitted_CLIF_Frequency[j])) > 0.005) {    //If deviation from fit is more than .5%, replace value
				   CLIF_frequency[j]=Fitted_CLIF_Frequency[j];
			}
		}
	}
	LinFit (freq_time_axis, CLIF_frequency, CLIF_RecordLength, Fitted_CLIF_Frequency, &Freq_fit_slope, &Freq_fit_intercept, &Freq_fit_error);
	for (j=0;j<CLIF_RecordLength;j++) {
		if ( (fabs(Fitted_CLIF_Frequency[j])) > 0) {
			if ( (fabs((CLIF_frequency[j]-Fitted_CLIF_Frequency[j])/Fitted_CLIF_Frequency[j])) > 0.001) {    //If deviation from fit is more than .1%, replace value
				   CLIF_frequency[j]=Fitted_CLIF_Frequency[j];
			}
		}
	}

	//	Get PID Module setpoint, gain, and offset
	GetCtrlVal (ContinLIF_panel, CLIF_PANEL_PIDModule_setpoint,&PIDModule_setpoint);
	GetCtrlVal (ContinLIF_panel, CLIF_PANEL_PIDModule_offset,&PIDModule_offset);
	GetCtrlVal (ContinLIF_panel, CLIF_PANEL_PIDModule_gain,&PIDModule_gain);

	//Convert measured timeseries into real units
	for (j=0;j<CLIF_RecordLength;j++) {
		CLIF_frequency[j]=speed_o_light/( (CLIF_frequency[j]-PIDModule_offset)/PIDModule_gain +PIDModule_setpoint);
		CLIF_iodine_signal[j]=-1.0*CLIF_iodine_signal[j];
	}
		
	// Print Wavelength into Textbox if valid frequency measurement value
	if (CLIF_frequency[CLIF_RecordLength-1] > 10.0) {
		sprintf(headerstring, "%f\n",speed_o_light/CLIF_frequency[CLIF_RecordLength-1]);
		InsertTextBoxLine(ContinLIF_panel, CLIF_PANEL_WAVEMETER_STRING, 0, headerstring);
	}
		
	//Subtract off dc signal from iodine trace
	MaxMin1D (CLIF_iodine_signal, CLIF_RecordLength, &Imax, &Imax_index, &Imin,&Imin_index);
	for( j = 0; j < CLIF_RecordLength; j++) {
	   	CLIF_iodine_signal[j]=CLIF_iodine_signal[j]-Imin;		 
	}

	//Deconvolve LIF timeseries based Lock-in timeconstant
	if (Deconvolve_flag) {
	
		ReFFT(CLIF_signal1, CLIF_imaginary, CLIF_RecordLength);
	    // Deconvolve out to 10% of Nyquist frequency, i.e., drate/20
	    for( j = 1; j < CLIF_RecordLength/20; j++) {
    	
	    	//calculate filter value at each frequency. Need to guess at an effective frequency for the actual LIF measurement
			//For now use 1/(2*pi*RC) as a proxy.
	    	omega=j*2*pi*(1/(2.0*pi*RC))/CLIF_RecordLength;
	    	filter_real=1./(1+omega*RC*omega*RC);
	    	filter_imag=-omega*RC/(1+omega*RC*omega*RC);
		
			//correct FFT of timeseries by proper complex division
			CLIF_signal1[j]=(CLIF_signal1[j]*filter_real+CLIF_imaginary[j]*filter_imag)/
							(filter_real*filter_real+filter_imag*filter_imag);				
			CLIF_signal1[CLIF_RecordLength-j]=(CLIF_signal1[CLIF_RecordLength-j]*filter_real+
								CLIF_imaginary[CLIF_RecordLength-j]*filter_imag)/
							(filter_real*filter_real+filter_imag*filter_imag);
						
			CLIF_imaginary[j]=(-1.*CLIF_signal1[j]*filter_imag+CLIF_imaginary[j]*filter_real)/
							(filter_real*filter_real+filter_imag*filter_imag);				
			CLIF_imaginary[CLIF_RecordLength-j]=(-1.*CLIF_signal1[CLIF_RecordLength-j]*filter_imag+
							CLIF_imaginary[CLIF_RecordLength-j]*filter_real)/
							(filter_real*filter_real+filter_imag*filter_imag);
		}
	
		// inverse fft to recover deconvolved times series
		ReInvFFT (CLIF_signal1, CLIF_imaginary, CLIF_RecordLength);
	} //Deconvolve check
						
	//Analyze the LIF data
	CLIF_analyze(); 
		
	//Plot the LIF data
	CLIF_plot();		
	
	//Always store the raw data to the raw data folder
	write_CLIF_data();
	
	//Store the LIF data in the MDS database if flag set on user interface
	GetCtrlVal (ContinLIF_panel, CLIF_PANEL_ContinLIF_Data,&ContinLIF_DataFlag);
	if (ContinLIF_DataFlag) {
		write_CLIF_data_MDS();
		//Update MDS Shot Number in window
		ShotNum=IncrementMDSCurrentShot();
	}
	
	
} //end of continuous LIF scan routine		







//Shift laser frequency up by 1 GHz
int CVICALLBACK Laser_Shift (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	double	Shift_Time;
	
	switch (event)
		{
		case EVENT_COMMIT:
			
			Shift_Time = (1.0/10.0)*90.0;

			//	Get Sirah Laser IP address	 
			GetCtrlVal (ContinLIF_panel, CLIF_PANEL_SIRAH_IP,Sirah_Address);			
				
			Sirah_Laser_SetScan(Sirah_Address,Shift_Time,1.0, Sirah_Calibration);
			
			//Trigger laser shift
			Sirah_Laser_Scan (Sirah_Address);
			Delay(Shift_Time);  		//wait number of seconds laser should be taking to move to next wavelength
			
			break;
		}
	return 0;
}


//Shift laser frequency down by 1 GHz
int CVICALLBACK Laser_Shift_Down (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	double	Shift_Time;
	
	switch (event)
		{
		case EVENT_COMMIT:
			
			Shift_Time = (1.0/10.0)*90.0;
			Sirah_Laser_SetScan_Down(Sirah_Address,Shift_Time, 1.0, Sirah_Calibration); 
				
			//	Get Sirah Laser IP address	 
			GetCtrlVal (ContinLIF_panel, CLIF_PANEL_SIRAH_IP,Sirah_Address);
			
			//Trigger laser shift
			Sirah_Laser_Scan (Sirah_Address);
			Delay(Shift_Time);  		//wait number of seconds laser should be taking to move to next wavelength
			
			break;
		}
	return 0;
}


void ContLIF_Laser_Reset(double wavelength)
{
    char			headerstring[50];
	extern int		control_panel;


	//Set laser scan time and scan range to selected values based on which laser in use
	//	Get laser conversion factor	 
	GetCtrlVal (ContinLIF_panel, CLIF_PANEL_Sirah_Calibration,&Sirah_Calibration);
		
	//	Get laser timespan for scan	 
	GetCtrlVal (ContinLIF_panel, CLIF_PANEL_SweepTime,&CLIF_timespan);
		
	//	Get Sweep Range for LIF	 
	GetCtrlVal (ContinLIF_panel, CLIF_PANEL_SweepRange,&CLIF_sweep_range);

	//	Get Sirah Laser IP address	 
	GetCtrlVal (ContinLIF_panel, CLIF_PANEL_SIRAH_IP,Sirah_Address);

	//Reset laser to beginning of scan range using only a 30 second return time
	Sirah_Laser_SetScan_Down(Sirah_Address,30.0,CLIF_sweep_range,Sirah_Calibration);
	Sirah_Laser_Scan(Sirah_Address);

}

int CVICALLBACK Wavemeter_Call (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
    double	Display_Wavelength;
    float	Laser_Power;
    char	headerstring[50];  
	char	Wavemeter_COM_string[8];
	char	dummy[8];

	
	switch (event)
		{
		case EVENT_COMMIT:
			//Get Wavemeter COM Port number string 
			GetCtrlVal (ContinLIF_panel, CLIF_PANEL_Wavemeter_COM,Wavemeter_COM_string);
	
			//Strip out COM address from string
			sscanf(Wavemeter_COM_string,"%3s %d",dummy,&Wavemeter_COM_Port);

			// Get Wavelength from wavemeter and request an average over given number of measurements
			Bristol_Wavelength(Wavemeter_COM_Port, Wavemeter_Averages, &Display_Wavelength, &Laser_Power);
			// Print Wavelength into Textbox
			sprintf(headerstring, "%f\n",Display_Wavelength);
			InsertTextBoxLine(ContinLIF_panel, CLIF_PANEL_WAVEMETER_STRING, 0, headerstring);
		break;
		}
	return 0;
}

int CVICALLBACK CLIF_Activate_Button (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
int	shotnum;
int result;

	switch (event)
	{
		case EVENT_COMMIT:
			//Open MDS to new shot number
			result=	IncrementMDSCurrentShot();
			
			shotnum= getMDSCurrentShot();  
			ContinLIF_Activate();
			CLIF_acquire(); 
			break;
	}
	return 0;
}



/**********************************************************************************
 //																				  
 //	  Code	             :  2D LIF Ti & Ni measurements 						  
 //   Writen By          :  Rick Soulsby, John Kline, Brian Kent				  
 //   Date Alpha version :  7/18/2000											  							  
 //																				  
 //   Codes to move the XYZ stages for LIF measurements  			  
 //																			  
 //   MODIFICATION: Brian Kent, 7/25/2000										  
 //						The file was modified with a function at the end of the   
 //							file to test the input values			  			  
 //							of x and y to see that LIF stage will not enter		  
 //							a region of x < -0.20 and x > 3.2					  
 //																				  
 //	  MODIFICATION: Rick Soulsby, 7/26/2000										  
 //						File modified to actively control position area to avoid  
 //							stage of x < -0.20   and x > 3.2					  
 //	  MODIFICATION: E.E.S. 3/19/2020
 //						Completely rewriten to use 2D and 3D stages in the source
 //						and PHASMA. Also rewritten to acquire data from camera
 //						and pulsed operation.
 //
 //   Modification: E.E.S. 6/19/2025
 //						Completely rewritten to integrate with new PHASMA DAQ code.
 //
 *********************************************************************************/

int CVICALLBACK XYZ_Move (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	//Physical Properties of the lead screws set this value
	#define Step_to_cm 0.000004


	int		X_steps;
	int		Y_steps;
	int		Z_steps;
	
	int		result;
	int		count;
	int		X_COM_Handle;
	int		Y_COM_Handle;
	int		Z_COM_Handle;
	float	X_Current;
	float	X_Target;
	float	Y_Current;
	float	Y_Target;
	float	Z_Current;
	float	Z_Target;
	char 	dummy_string[8];
	char 	X_COM_string[8];
	char 	Y_COM_string[8];
	char 	Z_COM_string[8];
	char	send_string[30]=" ";
	
	ViSession X_stage_handle;
	ViSession Y_stage_handle;
	ViSession Z_stage_handle;
	
	switch (event)
		{
		case EVENT_COMMIT:

			//Get the COM ports for the motor controllers	
  			GetCtrlVal (ContinLIF_panel, CLIF_PANEL_X_COM,X_COM_string);
 			GetCtrlVal (ContinLIF_panel, CLIF_PANEL_Y_COM,Y_COM_string);
 			GetCtrlVal (ContinLIF_panel, CLIF_PANEL_Z_COM,Z_COM_string);

	   		//Open communications with the X, Y, and Z stages
			result=viOpen (Global_Visa_Session_Handle, X_COM_string, VI_NULL, VI_NULL, &X_stage_handle);
			result=viOpen (Global_Visa_Session_Handle, Y_COM_string, VI_NULL, VI_NULL, &Y_stage_handle);
			result=viOpen (Global_Visa_Session_Handle, Z_COM_string, VI_NULL, VI_NULL, &Z_stage_handle);
	 
			//Get current positions
			GetCtrlVal (ContinLIF_panel, CLIF_PANEL_X_Current,&X_Current);
			GetCtrlVal (ContinLIF_panel, CLIF_PANEL_Y_Current,&Y_Current);
			GetCtrlVal (ContinLIF_panel, CLIF_PANEL_Z_Current,&Z_Current);
			
			//Get target positions
			GetCtrlVal (ContinLIF_panel, CLIF_PANEL_X_Target,&X_Target);
			GetCtrlVal (ContinLIF_panel, CLIF_PANEL_Y_Target,&Y_Target);
			GetCtrlVal (ContinLIF_panel, CLIF_PANEL_Z_Target,&Z_Target);

			//Enable the stages
			sprintf (send_string,"DE=1\r\n");
			result = viWrite (X_stage_handle, send_string, (ViUInt32)strlen(send_string), &count);
			result = viWrite (Y_stage_handle, send_string, (ViUInt32)strlen(send_string), &count);
			result = viWrite (Z_stage_handle, send_string, (ViUInt32)strlen(send_string), &count);

			//Calculate move steps in absolute units
			X_steps=(int)((X_Target-X_Current)/Step_to_cm);
			Y_steps=(int)((Y_Target-Y_Current)/Step_to_cm);
			Z_steps=(int)((Z_Target-Z_Current)/Step_to_cm);
	
			//Move x stage with a relative move
			sprintf (send_string,"MR %d\r\n",X_steps);
			result = viWrite (X_stage_handle, send_string, (ViUInt32)strlen(send_string), &count);
 	
			//Move y stage with a relative move
			sprintf (send_string,"MR %d\r\n",Y_steps);
			result = viWrite (Y_stage_handle, send_string, (ViUInt32)strlen(send_string), &count);
	
			//Move z stage with a relative move if in that mode
			sprintf (send_string,"MR %d\r\n",Z_steps);
			result = viWrite (Z_stage_handle, send_string, (ViUInt32)strlen(send_string), &count);
	
			//Hold program from advancing while stage moves based on largest travel
			//if ((fabs(X_steps) > fabs(Y_steps)) && (fabs(X_steps) > fabs(Z_steps))) {
			//	Delay(fabs((X_steps-last_steps)*Step_to_cm/2));
			//	last_steps=X_steps;
			//} else {
			//	if (fabs(Y_steps) > fabs(Z_steps)) {
			//		Delay(fabs((Y_steps-last_steps)*Step_to_cm/2));
			//		last_steps=Y_steps;
			//	} else {
			//		Delay(fabs((Z_steps-last_steps)*Step_to_cm/2));
			//		last_steps=Z_steps;
			//	}	
			
			//Set current positions
			GetCtrlVal (ContinLIF_panel, CLIF_PANEL_X_Current,&X_Target);
			GetCtrlVal (ContinLIF_panel, CLIF_PANEL_Y_Current,&Y_Target);
			GetCtrlVal (ContinLIF_panel, CLIF_PANEL_Z_Current,&Z_Target);
			
			//Close the ports to each motion drive
			result=viClose (X_stage_handle);
			result=viClose (Y_stage_handle);
			result=viClose (Z_stage_handle);
		
			break;
		}
	return 0;
}

int CVICALLBACK XYZ_Home (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	//Physical Properties of the lead screws set this value
	#define Step_to_cm 0.000004

	int		X_steps;
	int		Y_steps;
	int		Z_steps;
	
	int		result;
	int		count;
	int		X_COM_Handle;
	int		Y_COM_Handle;
	int		Z_COM_Handle;
	float	X_Offset=0.0;
	float	Y_Offset=0.0;
	float	Z_Offset=0.0;
	char 	dummy_string[8];
	char 	X_COM_string[8];
	char 	Y_COM_string[8];
	char 	Z_COM_string[8];
	char	send_string[30]=" ";
	
	ViSession X_stage_handle;
	ViSession Y_stage_handle;
	ViSession Z_stage_handle;
	
	switch (event)
		{
		case EVENT_COMMIT:

			//Get the COM ports for the motor controllers	
  			GetCtrlVal (ContinLIF_panel, CLIF_PANEL_X_COM,X_COM_string);
 			GetCtrlVal (ContinLIF_panel, CLIF_PANEL_Y_COM,Y_COM_string);
 			GetCtrlVal (ContinLIF_panel, CLIF_PANEL_Z_COM,Z_COM_string);

		   	//Open communications with the X, Y, and Z stages
			result=viOpen (Global_Visa_Session_Handle, X_COM_string, VI_NULL, VI_NULL, &X_stage_handle);
			result=viOpen (Global_Visa_Session_Handle, Y_COM_string, VI_NULL, VI_NULL, &Y_stage_handle);
			result=viOpen (Global_Visa_Session_Handle, Z_COM_string, VI_NULL, VI_NULL, &Z_stage_handle);
	 
			//Enable the stages
			sprintf (send_string,"DE=1\r\n");
			result = viWrite (X_stage_handle, send_string, (ViUInt32)strlen(send_string), &count);
			result = viWrite (Y_stage_handle, send_string, (ViUInt32)strlen(send_string), &count);
			result = viWrite (Z_stage_handle, send_string, (ViUInt32)strlen(send_string), &count);

			//Move back to zero by going to limit switch
			X_steps=(int)(-10/Step_to_cm);
			Y_steps=(int)(-10/Step_to_cm);
			Z_steps=(int)(-10/Step_to_cm);
	
			//Move x stage with a relative move
			sprintf (send_string,"MR %d\r\n",X_steps);
			result = viWrite (X_stage_handle, send_string, (ViUInt32)strlen(send_string), &count);
 	
			//Move y stage with a relative move
			sprintf (send_string,"MR %d\r\n",Y_steps);
			result = viWrite (Y_stage_handle, send_string, (ViUInt32)strlen(send_string), &count);
	
			//Move z stage with a relative move if in that mode
			sprintf (send_string,"MR %d\r\n",Z_steps);
			result = viWrite (Z_stage_handle, send_string, (ViUInt32)strlen(send_string), &count);
	
			//Move to center by going to offsets
			X_steps=(int)(X_Offset/Step_to_cm);
			Y_steps=(int)(Y_Offset/Step_to_cm);
			Z_steps=(int)(Z_Offset/Step_to_cm);
	
			//Move x stage with a relative move
			sprintf (send_string,"MR %d\r\n",X_steps);
			result = viWrite (X_stage_handle, send_string, (ViUInt32)strlen(send_string), &count);
 	
			//Move y stage with a relative move
			sprintf (send_string,"MR %d\r\n",Y_steps);
			result = viWrite (Y_stage_handle, send_string, (ViUInt32)strlen(send_string), &count);
	
			//Move z stage with a relative move if in that mode
			sprintf (send_string,"MR %d\r\n",Z_steps);
			result = viWrite (Z_stage_handle, send_string, (ViUInt32)strlen(send_string), &count);
	
			//Set current positions
			SetCtrlVal (ContinLIF_panel, CLIF_PANEL_X_Current,0.0);
			SetCtrlVal (ContinLIF_panel, CLIF_PANEL_Y_Current,0.0);
			SetCtrlVal (ContinLIF_panel, CLIF_PANEL_Z_Current,0.0);
			
			//Close the ports to each motion drive
			result=viClose (X_stage_handle);
			result=viClose (Y_stage_handle);
			result=viClose (Z_stage_handle);
		
			break;
		}
	return 0;
}
