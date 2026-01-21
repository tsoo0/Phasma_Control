 /***************************************************************************
 * Code for aquiring all the parameters of the helicon source               *
 *																			*
 * Written by       Earl Scime												*
 *																			*
 * 				            												*
 *																			*
 *	5/1/2025		Code modified to integrate with new PHASMA DAQ code		*
 *  																		*
 ****************************************************************************/		

//General Include Files
#include <gpib.h>
#include <analysis.h>
#include <stdio.h>
#include <userint.h>
#include <formatio.h>
#include <ansi_c.h>
#include <utility.h>
#include <visa.h>
#include <rs232.h>
#include <string.h> 
#include <NIDAQmx.h>
#include <srds345.h> 
#include "GlobalVariables.h"   

//Settings Specific Include Files
#include "HeliconSourceSettings.h"
#include "Helicon_Settings.h"
#include "PHASMA-MDS-Routines.h"

//Global Variables for settings panel
int			Helicon_setup_state = 2;
int			Helicon_panel;
int			DS345_Active=1;			//Initialize to not finding instrument
int			rfsource_gpibdev=-1;		//initialize to not finding instrument  
char		SRS365_GPIB[18];  
short		found=0;					//initialize to not finding instrument
float		SCCM1, SCCM2;
float		species_mass;
double		rf_freq;
double		rf_amp;
double		Forward_rf;
double		Reflected_rf;
double		Upstream_Pressure;
double		Downstream_Pressure;
double		corrected_upstream_pressure;
double		corrected_downstream_pressure;




//Read the target RF source frequency and amplitude from the interface and
//send the values to function generator with corrections to account for
//peak-to-peak voltage and proper voltage units.
void 	rfsource_configure(int power_state)
{
	int		result;
 	double 	rf_amp_ptp;
 
 	//Open settings panel in the background
	Helicon_panel = LoadPanel (0, "Helicon_Settings.uir", Helicon);
	RecallPanelState (Helicon_panel, "Master_Control_Storage_File", Helicon_setup_state);

	//Get RF source frequency and amplitude from settings panel
 	GetCtrlVal (Helicon_panel,Helicon_RF_freq,&rf_freq);
 	GetCtrlVal (Helicon_panel,Helicon_RF_Amp,&rf_amp);

	//Close Panel
	//DiscardPanel(Helicon_panel);
	
	//Communicate with SRS Function Generator at GPIB 15 if properly opened
 	if (found > 0 ) {
		if (DS345_Active)	{  
			DS345_Active = srds345_init (SRS365_GPIB, VI_TRUE, VI_FALSE, &rfsource_gpibdev);
		}
	//Convert to Peak-to_peak voltage and convert to millivolts
	rf_amp_ptp=2.0*rf_amp*power_state/1000;
	
	//Convert frequency to MHz
	rf_freq=rf_freq*1.0E6;
	
	//Communicate with RF source if opening command successful
	if (!DS345_Active) {
		result=srds345_ConfigureOutputMode (rfsource_gpibdev, SRDS345_VAL_OUTPUT_FUNC);
		result=srds345_ConfigureStandardWaveform (rfsource_gpibdev, "1", SRDS345_VAL_WFM_SINE, rf_amp_ptp, 0, rf_freq, 0.00);
		result=srds345_ConfigureOperationMode (rfsource_gpibdev, "1", SRDS345_VAL_OPERATE_CONTINUOUS);
		result=srds345_ConfigureOutputImpedance (rfsource_gpibdev, "1", 50.00);
		result=srds345_ConfigureOutputEnabled (rfsource_gpibdev, "1", VI_TRUE); 
	}
	
	//delay to let rfsource get ready for next command
	Delay(.1);

	//Close Panel
	//DiscardPanel(Helicon_panel);

	//Close communications with rf source
	srds345_close (rfsource_gpibdev);
	DS345_Active=1;
 	}
}


//Read the current frequency and RF amplitude from the function
//generator and update the front panel with that information.
																	   
void rfsource_parameters(double *rfsource_freq, double *rfsource_amp)
{
 	double	dummy1,dummy2;
	int		GPIB_Address;
	int		result;
	char	dummy[8];
					  
   	//Open settings panel in the background
	//Helicon_panel = LoadPanel (0, "Helicon_Settings.uir", Helicon);
	//RecallPanelState (Helicon_panel, "Master_Control_Storage_File", Helicon_setup_state);

	//Get RF source GPIB address string from settings panel
 	GetCtrlVal (Helicon_panel, Helicon_SRS365_GPIB,SRS365_GPIB);
	
	//Strip out GPID address from string
	sscanf(SRS365_GPIB,"%7s %d",dummy, &GPIB_Address);
 
	//Clear the GPIB bus
	SendIFC (1);

	// Check to see if SRS Function Generator on bus at address #15
	result = ibln (1, GPIB_Address, 0, &found);
	
	//Only proceed if valid device found 
	if (found > 0) {
		
		if (DS345_Active)	{  
			DS345_Active = srds345_init (SRS365_GPIB, VI_TRUE, VI_FALSE, &rfsource_gpibdev);
		}
 
		if (rfsource_gpibdev > 0) {
		 	//Read current frequency and amplitude of the RF source
			result = srds345_GetAttributeViReal64 (rfsource_gpibdev, "1", SRDS345_ATTR_FUNC_AMPLITUDE, &dummy1);
			result = srds345_GetAttributeViReal64 (rfsource_gpibdev, "1", SRDS345_ATTR_FUNC_FREQUENCY, &dummy2);

			*rfsource_freq=dummy2/1.0E6;
			*rfsource_amp=dummy1*1000.0/2.0;
		} else {
			dummy1=0.0;
			dummy2=0.0;
		}

	//Send values to the front panel
	SetCtrlVal (Helicon_panel, Helicon_RF_freq,dummy2/1.0E6);
 	SetCtrlVal (Helicon_panel, Helicon_RF_Amp,dummy1*1000.0/2.0);
	
	//Close Panel
	//SavePanelState (Helicon_panel, "Master_Control_Storage_File", Helicon_setup_state);
	//DiscardPanel(Helicon_panel);
	
	//Close communications with rf source
	srds345_close (rfsource_gpibdev);
	DS345_Active=1;
	}
}


//Read RF power from ENI amplifier at target COM port
void EandI_meter(double *Forward, double *Reflected)
{
	int 	result;
	int		i;
 	int		EandI_Port=50; 
	int		holding_int1;
 	int		holding_int2;
 	char 	receive_string[512];
	char	dummy[6];
  	char	*colon=":";
	char	*string_piece_1="^";
 	char	COM_string[30];
	
	//Open settings panel in the background
	//Helicon_panel = LoadPanel (0, "Helicon_Settings.uir", Helicon);
	//RecallPanelState (Helicon_panel, "Master_Control_Storage_File", Helicon_setup_state);

	//Get RF source COM port string from settings panel
 	GetCtrlVal (Helicon_panel, Helicon_ENI_COM,COM_string);
	
	//Strip out GPID address from string
	sscanf(COM_string,"%3s %d",dummy,&EandI_Port);
	
	//open COM port to device and flush buffers
	result = OpenComConfig (EandI_Port, COM_string, 19200, 0, 8, 1, 128, 256);
	FlushInQ (EandI_Port);
	FlushOutQ (EandI_Port);
	
	//return without doing anything else if open command fails
	if (result < 0) {
		//Send values to the front panel
		SetCtrlVal (Helicon_panel, Helicon_RF_power_foward,0.0);
	 	SetCtrlVal (Helicon_panel, Helicon_RF_power_reflected,0.0);
	} else {
			
	 	//Set length of time to wait for reading to 0.5 second
	 	SetComTime (EandI_Port, 0.50);
	 
	 	//The instrument continously broadcasts, so just read the buffer
		ComRd (EandI_Port, receive_string, 256);  // read whatever is left on the buffer from the last command sent to instrument 

		//  Look for power information in the read string
		i=0;
		do  {
		   	sscanf (&receive_string[i], "%c",string_piece_1);
		   	i=i+1;
		} while (fabs(strcmp(string_piece_1,colon)>0)&& (i<4));
		Scan(&receive_string[i],"%d",&holding_int1); 	// first number is the forward power, second is the reverse power
		*Forward=(double)(holding_int1);
		
		i=i+5; //Advance to next number
		do  {
		   	sscanf (&receive_string[i], "%c",string_piece_1);
		   	i=i+1;
		} while (fabs(strcmp(string_piece_1,colon)>0)&& (i<12));
		Scan(&receive_string[i],"%d",&holding_int2); 	// first number is the forward power, second is the reverse power
	 	*Reflected=(double)(holding_int2);

		//Send values to the front panel
		SetCtrlVal (Helicon_panel, Helicon_RF_power_foward,(double)(holding_int1));
	 	SetCtrlVal (Helicon_panel, Helicon_RF_power_reflected,(double)(holding_int2));
		
		//Close Panel
		//SavePanelState (Helicon_panel, "Master_Control_Storage_File", Helicon_setup_state);
		//DiscardPanel(Helicon_panel);
		
		//Close COM port
		result = CloseCom (EandI_Port);
	}
}



// Read pressure from the Pfeiffer pressure gauge.
// Considerably more error checking added to try
// to solve COM port lockup problem. Last thing added
// is the clearing of the queues just before closing the port
// and reduction in baud rate to 4800
double read_pfeiffer_pressure(void)
{
	int		result;
	int		Pressure_Port=4;
	int		count=0;
	long	response;
	char 	receive_Value[64];
 	char	COM_string[30];
	char	dummy[6];
	double 	mbar[2];
	float	delay_time=.01;
 
	//Open settings panel in the background
	//Helicon_panel = LoadPanel (0, "Helicon_Settings.uir", Helicon);
	//RecallPanelState (Helicon_panel, "Master_Control_Storage_File", Helicon_setup_state);

	//Get RF source COM port string from settings panel
 	GetCtrlVal (Helicon_panel, Helicon_PfeifferGauge,COM_string);
	
	//Strip out GPID address from string
	sscanf(COM_string,"%3s %d",dummy,&Pressure_Port);
	
	//open COM port to device and flush buffers
   	result = OpenComConfig (Pressure_Port, COM_string, 4800, 0, 8, 1, 64, 64);
	
	//return without doing anything else if open command fails
	if (result < 0) return(0.0);
	Delay (delay_time);  
 
	//Set length of time to wait for reading to 0.2 second
	SetComTime (Pressure_Port, 0.2);
 
	ComWrt (Pressure_Port, "\x05\r\n", 3);		// request transmission
	Delay (delay_time);      
	ComRd (Pressure_Port, receive_Value, 64);  	// read whatever is left on the buffer from the last command sent to instrument 
	Delay (delay_time); 
	response=receive_Value[0];					// place first byte of buffer into "response" variable.

	ComWrt (Pressure_Port, "PR1\r\n", 5);		// transmit request for pressure data
	Delay (delay_time);  
 
	while ((response != 6) && (count <5)) {		// ask up to 5 times for data or until message received is confirmed
	     if (response == 21)   {
	     	ComWrt (Pressure_Port, "PR1\r\n", 5);   // ask again for pressure if negative acknowledgement received
	     	Delay (delay_time);
	     }
	     ComRd (Pressure_Port, receive_Value, 64);  // confirm message received
	 	 response=receive_Value[0];					// place first byte of buffer into "response" variable.
		 count=count+1;
	}
 
	ComWrt (Pressure_Port, "\x05\r\n", 3);			// request data transmission
	Delay (delay_time);
 
	ComRd (Pressure_Port, receive_Value, 64);  		// get pressure    
	Delay (delay_time) ; 
	Scan(receive_Value,"%s>%f[x]%f",&mbar[0],&mbar[1]); // mbar[0] is the Status, mbar[1] is the pressure
 
    count=0;
 	while ((mbar[1] < 1.0E-9)&&(count < 5)) {					// ask up to 5 times for pressure data until valid pressure data obtained
 		ComRd (Pressure_Port, receive_Value, 64);  			// get pressure    
		Delay (delay_time) ; 
		Scan(receive_Value,"%s>%f[x]%f",&mbar[0],&mbar[1]); // mbar[0] is the Status, mbar[1] is the pressure
		count=count+1;
 	}

 
 	ComWrt (Pressure_Port, "RES\r\n", 5);		// reset instrument before closing
 	Delay (delay_time) ;      
 	ComRd (Pressure_Port, receive_Value, 64);  // read whatever is on the buffer 
 	Delay (delay_time); 
 	response=receive_Value[0];					// place first byte of buffer into "response" variable.
 	count=0;
	while ((response != 6) && (count < 5)){		// keep asking for data until message received is confirmed
     	ComRd (Pressure_Port, receive_Value, 64);// confirm message received
 	 	response=receive_Value[0];				// place first byte of buffer into "response" variable.
		count=count+1;
 	}

 	ComWrt (Pressure_Port, "\x05\r\n", 3);		// request transmission
 	Delay (delay_time);      
 	ComRd (Pressure_Port, receive_Value, 64);  // read whatever is left on the buffer from the last command sent to instrument 
 	Delay (delay_time); 

									
 	//clear all buffers and try to reset all the com lines
 	FlushInQ (Pressure_Port); 
 	FlushOutQ (Pressure_Port);
 	ComSetEscape (Pressure_Port, 6);
 	ComSetEscape (Pressure_Port, 4);
 
 	result = CloseCom (Pressure_Port);
 	Delay (delay_time); 
 	count=0;
	while ((result < 0) && (count <5)){
 		result = CloseCom (Pressure_Port);		//Try closing Com port up to 5 times or until it is really closed
 		Delay (delay_time); 
 	}
 
 	if (mbar[0] != 0)
	{	
	  	MessagePopup ("warning", "please check your gauge");
      	return(-1);
    }
  
	//Get gas species from settings panel
 	GetCtrlVal (Helicon_panel, Helicon_Species,&species);
	
	switch (species) {   
 	 	case 1:
	    	//Argon Gas
	    	species_mass = 40.0;
			corrected_upstream_pressure=mbar[1]*750*0.80;
		break;

 	 	case 2:
	    	//Helium Gas
	    	species_mass = 4.0;
			corrected_upstream_pressure=mbar[1]*750*5.9;
		break;
		
 	 	case 3:
	    	//Nitrogen Gas
	    	species_mass = 14.0;
			corrected_upstream_pressure=mbar[1]*750*1.0;
		break;

		case 4:
	    	//Iodine Gas (assumes it acts like O2)
	   		species_mass = 126.9;
			corrected_upstream_pressure=mbar[1]*750*1.0;
		break;
		
		case 5:
			//Xenon Gas
			species_mass = 131.3;
			corrected_upstream_pressure=mbar[1]*750*0.40;
		
		case 6:
	    	//Argon Gas
	    	species_mass = 40.0;
			corrected_upstream_pressure=mbar[1]*750*0.80;
		break;
	} 


	//Close Panel
	//SavePanelState (Helicon_panel, "Master_Control_Storage_File", Helicon_setup_state);
	//DiscardPanel(Helicon_panel);
	
	//Send values to the front panel
	SetCtrlVal (Helicon_panel, Helicon_UpstreamPressure,mbar[1]*750);
	SetCtrlVal (Helicon_panel, Helicon_UpstreamCorrected,corrected_upstream_pressure);
 	
	return(mbar[1]*750);  //mbar --->mTorr

}





// Read pressure from the Lesker 300 high pressure gauge
// using channel 7 of the analog card
double readpressureLesker300(void)
{
 double Downstream_Pressure=0.0;
 int 	result;

	//Declare task handle for output card
	TaskHandle 		PressureReadTaskHandle;
 
 	//Create Task for measurement
 	result = DAQmxCreateTask ("", &PressureReadTaskHandle);
	
	//Use interlaced empty channels to take care of cross talk in between measurements as AtoD switching between very different values creates ghosting
	result = DAQmxCreateAIVoltageChan (PressureReadTaskHandle, "Dev1/ai7", "Channel7", DAQmx_Val_RSE, -5.0, 5.0, DAQmx_Val_Volts, "");
	
	//Finish setup if device found
	if (!result) {
		//Set sample timing of 1 kHz and acquire 10 samples
		result = DAQmxCfgSampClkTiming (PressureReadTaskHandle, "OnboardClock", 1000, DAQmx_Val_Rising, DAQmx_Val_FiniteSamps, 10);
		
		//Make the measurement
		result = DAQmxStartTask (PressureReadTaskHandle);
			
		//read the measurement with a timeout of 1 s. Looks like this function just reads one value
		DAQmxReadAnalogScalarF64 (PressureReadTaskHandle, 1.0, &Downstream_Pressure, 0);

		//Stop Task
		DAQmxStopTask (PressureReadTaskHandle);
	}
	
	//Covert the measured voltage into actual pressure in mTorr if a reasonable value is measured
	if (Downstream_Pressure < 4) {
		Downstream_Pressure=1000.0*pow(10.0,(1.667*Downstream_Pressure - 11.46));
	} else {
		Downstream_Pressure=0.0;
	}
		
	//Open settings panel in the background
	//Helicon_panel = LoadPanel (0, "Helicon_Settings.uir", Helicon);
	//RecallPanelState (Helicon_panel, "Master_Control_Storage_File", Helicon_setup_state);

	//Get RF source COM port string from settings panel
 	SetCtrlVal (Helicon_panel, Helicon_DownstreamPressure,Downstream_Pressure);
	
	//Get gas species from settings panel
 	GetCtrlVal (Helicon_panel, Helicon_Species,&species);

	switch (species) {   
 	 	case 1:
	    	//Argon Gas
	    	species_mass = 40.0;
			corrected_downstream_pressure=Downstream_Pressure;
		break;

 	 	case 2:
	    	//Helium Gas
	    	species_mass = 4.0;
			corrected_downstream_pressure=Downstream_Pressure;
		break;
		
 	 	case 3:
	    	//Nitrogen Gas
	    	species_mass = 14.0;
			corrected_downstream_pressure=Downstream_Pressure;
		break;

		case 4:
	   		species_mass = 126.9;
			corrected_downstream_pressure=Downstream_Pressure;
		break;
		
		case 5:
	   		species_mass = 131.3;
			corrected_downstream_pressure=Downstream_Pressure;
		
		case 6:
	    	species_mass = 40.0;
			corrected_downstream_pressure=Downstream_Pressure;
		break;
	} 
 	//Send corrected gas pressure to front panel
 	SetCtrlVal (Helicon_panel, Helicon_DownstreamCorrected,corrected_downstream_pressure);

	//Close Panel
	//SavePanelState (Helicon_panel, "Master_Control_Storage_File", Helicon_setup_state);
	//DiscardPanel(Helicon_panel);
 
	return(Downstream_Pressure);  
}






//Open communications with the PR4000 and read out the mass flow controller values.
void GetSCCM(float *SCCM1, float *SCCM2)
{
 	char 	receive_Value[11];
 	char	COM_string[30];
	char	dummy[6];
	int		PR4000_Port=3;
	int		result;
	int		Baratron_state;
	int		tries;
	double	setpoint1;
	double	setpoint2;
				
	//Open settings panel in the background
	//Helicon_panel = LoadPanel (0, "Helicon_Settings.uir", Helicon);
	//RecallPanelState (Helicon_panel, "Master_Control_Storage_File", Helicon_setup_state);

	//Get RF source COM port string from settings panel
 	GetCtrlVal (Helicon_panel, Helicon_PR4000_COM,COM_string);
	
	//Strip out COM address from string
	sscanf(COM_string,"%3s %d",dummy,&PR4000_Port);
	
	//open COM port to device and flush buffers
   	result = OpenComConfig (PR4000_Port, COM_string, 9600, 0, 8, 1, 512, 512);
 
	if (result < 0) {
		//No comunications so set everything to zero
		setpoint1=0.0;
		setpoint2=0.0;
	} else {
		//Set length of time to wait for reading to 0.3 second
	  	SetComTime (3, 0.3);

	  	// change mode to remote   
	  	ComWrt (PR4000_Port, "%1\r", 3); 
	  	Delay (.1);
	  	ComWrt (PR4000_Port, "RT,ON\r",6);
	  	Delay (.1);
  
	    //Read SCCM1 value
		ComWrt (PR4000_Port, "?AV1\r",5); 
		ComRd (PR4000_Port, receive_Value,11);
		sscanf(&receive_Value[0],"%6lf",&setpoint1);
		*SCCM1=setpoint1;
	
	   	//Read SCCM2 value
		ComWrt (PR4000_Port, "?AV2\r",5); 
		ComRd (PR4000_Port, receive_Value,11);
		sscanf(&receive_Value[0],"%6lf",&setpoint2);
		*SCCM2=setpoint2;
	
		//Turn off remote setting on PR4000
	 	tries=0;
	  	result = ComWrt (PR4000_Port, "RT,OFF\r",7);
		while ((result < 2) && (tries < 3)) {
		  	  result = ComWrt (PR4000_Port, "RT,OFF\r",7);
			  tries=tries+1;
		 }
  
		//Close Comport
		Delay (.1);
	  	result = CloseCom (PR4000_Port);
	  	tries=0;
	  	while ((result < 0) && (tries < 3)) {
	 		result = CloseCom (PR4000_Port);		//keep closing Com port until it is really closed
			tries=tries+1;
	  	}
	}
		
	//Check if Baratron in port #1 of PR4000
	GetCtrlVal (Helicon_panel, Helicon_Baratron,&Baratron_state);

	//Send values to the front panel
	if (Baratron_state) {
		SetCtrlVal (Helicon_panel, Helicon_BaratronPressure,SCCM1);
	}else{
		SetCtrlVal (Helicon_panel, Helicon_MassFlow1,setpoint1);
	}
 	SetCtrlVal (Helicon_panel, Helicon_MassFlow2,setpoint2);
	
	//Close Panel
	//DiscardPanel(Helicon_panel);

 	return;
}


//Using current target values for RF system, update function generator
void Helicon_Activate (int Helicon_Status)
{
	//Open settings panel in the background
	Helicon_panel = LoadPanel (0, "Helicon_Settings.uir", Helicon);
	RecallPanelState (Helicon_panel, "Master_Control_Storage_File", Helicon_setup_state);

	//Send updated values to the function generator running the RF amp or
	//shut system down if toggles off
	if(Helicon_Status) { 
		//run update with turning on system
		rfsource_configure(1);
	} else {

		//Set RF source frequency and amplitude in settings panel to turn down power
 		SetCtrlVal (Helicon_panel,Helicon_RF_Amp,10.0);

		//Close Panel
		DiscardPanel(Helicon_panel);

		//Run update with zero power requested for system
		rfsource_configure(0);
	}	
	return;  
}


//Launch this routine as window opens and it updates the window with the latest values from the 
//control system as it loads.
void HeliconSource_Settings (void)  
{
	//Open settings panel and load saved values
	Helicon_panel = LoadPanel (0, "Helicon_Settings.uir", Helicon);
	RecallPanelState (Helicon_panel, "Master_Control_Storage_File", Helicon_setup_state);
	DisplayPanel(Helicon_panel);
	
	//Update panel with latest values from rf source function generator
	rfsource_parameters(&rf_freq,&rf_amp);
	
	//Update panel with latest values from pressure system
	Upstream_Pressure=read_pfeiffer_pressure();
	Downstream_Pressure= readpressureLesker300();
	
	//Update panel with SCCM values from PR4000
	GetSCCM(&SCCM1,&SCCM2);
	
	//Update panel with latest RF power values from ENI amplifier
	EandI_meter(&Forward_rf,&Reflected_rf);
	
	// Start interacting with user
    RunUserInterface ();

}


void Write_HeliconSettings(void)
{
	int		j;
	double	Dummy_array[12];
	double	System_Parameters[12]={0};
	char	outfilename[36];
	FILE*	outfile;
	
	//Update panel with latest values from rf source function generator
	rfsource_parameters(&rf_freq,&rf_amp);
	
	//Update panel with latest RF power values from ENI amplifier
	EandI_meter(&Forward_rf,&Reflected_rf);

	//Update panel with latest values from pressure system
	Upstream_Pressure=read_pfeiffer_pressure();
	
	//Update panel with SCCM values from PR4000
	GetSCCM(&SCCM1,&SCCM2);

	//Read all the current values from the control system and write into an array that is then
	//written to the MDS database
	
	//Open settings panel and load saved values
	Helicon_panel = LoadPanel (0, "Helicon_Settings.uir", Helicon);
	RecallPanelState (Helicon_panel, "Master_Control_Storage_File", Helicon_setup_state);
	
	//Dump results to the arrays
	GetCtrlVal (Helicon_panel, Helicon_RF_freq,&System_Parameters[0]);
	GetCtrlVal (Helicon_panel, Helicon_RF_Amp,&System_Parameters[1]);
	GetCtrlVal (Helicon_panel, Helicon_RF_power_foward,&System_Parameters[2]);
	GetCtrlVal (Helicon_panel, Helicon_RF_power_reflected,&System_Parameters[3]);
	GetCtrlVal (Helicon_panel, Helicon_MassFlow1,&System_Parameters[4]);
	GetCtrlVal (Helicon_panel, Helicon_MassFlow2,&System_Parameters[5]);
	GetCtrlVal (Helicon_panel, Helicon_UpstreamPressure,&System_Parameters[6]);
	GetCtrlVal (Helicon_panel, Helicon_DownstreamPressure,&System_Parameters[7]);
	GetCtrlVal (Helicon_panel, Helicon_Baratron,&System_Parameters[8]);
	GetCtrlVal (Helicon_panel, Helicon_Species,&System_Parameters[10]);
 
	//Dump results to a file
	sprintf(outfilename, RawDataPath);
	strcat(outfilename, ShotNumberString);
	strcat(outfilename, "_");
	strcat(outfilename, "SystemParameters.txt");
	
	outfile=fopen (outfilename, "w");
	fprintf(outfile,"RF Freq\n");
	fprintf(outfile,"RF Amp\n");
	fprintf(outfile,"RF Freq\n");
	fprintf(outfile,"RF Power Forward\n");
	fprintf(outfile,"RF Power Reflected\n");
	fprintf(outfile,"Mass Flow 1\n");
	fprintf(outfile,"Mass Flow 2\n");
	fprintf(outfile,"Upstream Pressure\n");
	fprintf(outfile,"Downstream Pressure\n");
	fprintf(outfile,"Baratron Pressure\n");
	fprintf(outfile,"Ion Species\n");
	for (j=0;j<12;j++) {
		fprintf(outfile,"%f\n",System_Parameters[j]);
	}

 	//Close Housekeeping data file
	fclose (outfile);

	
	//Write to the MDS database a dummy array for timebase and the system parameters to reuse the standard
	//timeseries writting format for the MDS routine
	Write_to_PHASMA_MDS(Dummy_array,System_Parameters,"RAW_DATA.HELICON:HEL_PARAMS");
   	
	//Close Panel
	DiscardPanel(Helicon_panel);

}

int CVICALLBACK Close_Helicon_Settings (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			//Save current state of panel and close interface
			Helicon_panel = LoadPanel (0, "Helicon_Settings.uir", Helicon);
			SavePanelState (Helicon_panel, "Master_Control_Storage_File", Helicon_setup_state);
			DiscardPanel(panel);
			break;
	}
	return 0;
}

int CVICALLBACK Close_Helicon_Settings_Activate (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	int		result;
 	double 	rf_amp_ptp;
	
	switch (event)
	{
		case EVENT_COMMIT:
			
			//Get RF source frequency and amplitude from settings panel
		 	GetCtrlVal (Helicon_panel,Helicon_RF_freq,&rf_freq);
		 	GetCtrlVal (Helicon_panel,Helicon_RF_Amp,&rf_amp);

			//Communicate with SRS Function Generator at GPIB 15 if properly opened
		 	if (found > 0 ) {
				if (DS345_Active)	{  
					DS345_Active = srds345_init (SRS365_GPIB, VI_TRUE, VI_FALSE, &rfsource_gpibdev);
				}
				//Convert to Peak-to_peak voltage and convert to millivolts
				rf_amp_ptp=2.0*rf_amp/1000;
				
				//Convert frequency to MHz
				rf_freq=rf_freq*1.0E6;
				
				//Communicate with RF source if opening command successful
				if (!DS345_Active) {
					result=srds345_ConfigureOutputMode (rfsource_gpibdev, SRDS345_VAL_OUTPUT_FUNC);
					result=srds345_ConfigureStandardWaveform (rfsource_gpibdev, "1", SRDS345_VAL_WFM_SINE, rf_amp_ptp, 0, rf_freq, 0.00);
					result=srds345_ConfigureOperationMode (rfsource_gpibdev, "1", SRDS345_VAL_OPERATE_CONTINUOUS);
					result=srds345_ConfigureOutputImpedance (rfsource_gpibdev, "1", 50.00);
					result=srds345_ConfigureOutputEnabled (rfsource_gpibdev, "1", VI_TRUE); 
				}
			}
				
			//Save current state of parameters
			SavePanelState (Helicon_panel, "Master_Control_Storage_File", Helicon_setup_state);
			
			//Update RF function generator with new values
			Helicon_Activate(1);
			
			//Close Panel
			DiscardPanel(panel);
			break;
	}
	return 0;
}


int CVICALLBACK ReadPressure (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
		//Update panel with latest values from upstream pressure system
		Upstream_Pressure=read_pfeiffer_pressure();
			
		//Update panel with latest values from downstream pressure system
		readpressureLesker300();

		//Update panel with SCCM values from PR4000
		GetSCCM(&SCCM1,&SCCM2);

		break;
	}
	return 0;
}

int CVICALLBACK ReadRFPower (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			//Update panel with latest RF power values from ENI amplifier
			EandI_meter(&Forward_rf,&Reflected_rf);

			break;
	}
	return 0;
}

int CVICALLBACK PulsedSource (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	double 	Pulse_Freq;
	
	switch (event)
		{
		case EVENT_COMMIT:
		
			//Get settings from helicon interface panel
			GetCtrlVal (Helicon_panel, Helicon_Pulse_Freq,&Pulse_Freq);
	 			
			if (found > 0) {
				
				if (DS345_Active)	{  
					DS345_Active = srds345_init (SRS365_GPIB, VI_TRUE, VI_FALSE, &rfsource_gpibdev);
				}
		 
			 	//Turn on amplitude modulation on the function generator 
				srds345_ConfigureAMInternal (rfsource_gpibdev, 100, SRDS345_VAL_AM_INTERNAL_SQUARE, Pulse_Freq);
				srds345_ConfigureAMEnabled (rfsource_gpibdev, "1", VI_TRUE);
				
				//Turn on RF function generator output
				rfsource_configure(1);		
				
				//Close communications with rf source
				srds345_close (rfsource_gpibdev);
				DS345_Active=1;
			}
		}
	return 0;
}

int CVICALLBACK Stop_PulsedSource (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
		{
		case EVENT_COMMIT:
			if (found > 0) {
				
				if (DS345_Active)	{  
					DS345_Active = srds345_init (SRS365_GPIB, VI_TRUE, VI_FALSE, &rfsource_gpibdev);
				}
		 
				//Turn off AM modulation and RF function generator output
				srds345_ConfigureAMEnabled (rfsource_gpibdev, "0", VI_TRUE);
				rfsource_configure(1);		
				
				//Close communications with rf source
				srds345_close (rfsource_gpibdev);
				DS345_Active=1;

			}
		}
	return 0;
}

