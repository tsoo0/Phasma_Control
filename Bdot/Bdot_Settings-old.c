#include	<stdlib.h>  
#include 	<analysis.h>
#include 	<utility.h>
#include 	<ansi_c.h>
#include 	<formatio.h>
#include	<visa.h>
#include	"PHASMA-MDS-Routines.h"
#include	"GlobalVariables.h"
#include	"Rigol.h"  
#include	"PHASMA_C_to_Python_convertor.h"
#include 	<Joerger VTR10012-VISA.h>

#include 	"BdotSystems.h"
#include 	"BdotSettings.h" 

#define		max_record_length 262144  

#define  	VTR10012_Base_address 		0xAA00   		//Joerger 4 byte base address of AA00 in hexadecimal
#define	 	VTR10012_A32_address 		0x0				//Joerger A32 memory address (2 byte max) in hexadecimal
#define  	VTR10014_1_Base_address 	0xBB00   		//Joerger 4 byte base address of BB00 in hexadecimal
#define  	VTR10014_2_Base_address 	0xEE00   		//Joerger 4 byte base address of DD00 in hexadecimal
#define  	VTR10014_3_Base_address 	0xDD00  		//Joerger 4 byte base address of EE00 in hexadecimal
#define  	VTR10014_4_Base_address 	0x9900  		//Joerger 4 byte base address of EE00 in hexadecimal
#define	 	VTR10014_1_A32_address 		0x04			//Joerger A32 memory address (2 byte max) in hexadecimal for bits 24 to 32 in the 16 bit register, must match the full address and be mutltiple of 4
#define	 	VTR10014_2_A32_address 		0x08			//Joerger A32 memory address (2 byte max) in hexadecimal for bits 24 to 32 in the 16 bit register, must match the full address and be mutltiple of 4 
#define	 	VTR10014_3_A32_address 		0x0C			//Joerger A32 memory address (2 byte max) in hexadecimal for bits 24 to 32 in the 16 bit register, must match the full address and be mutltiple of 4 
#define	 	VTR10014_4_A32_address 		0x10			//Joerger A32 memory address (2 byte max) in hexadecimal for bits 24 to 32 in the 16 bit register, must match the full address and be mutltiple of 4 
#define	 	VTR10014_1_A32_addressFull 	0x04000000		//Joerger A32 memory address set in the VME device parameters
#define	 	VTR10014_2_A32_addressFull 	0x08000000		//Joerger A32 memory address set in the VME device parameters 
#define	 	VTR10014_3_A32_addressFull 	0x0C000000		//Joerger A32 memory address set in the VME device parameters 
#define	 	VTR10014_4_A32_addressFull 	0x10000000		//Joerger A32 memory address set in the VME device parameters 

		
int			Bdot_panel;
int			Bdot_setup_state = 6;
int 		JoergerVTR10012_clockspeed;
int 		JoergerVTR10014_clockspeed;
int 		RIGOL_Bdot_clockspeed;
long 		JoergerVTR10012_recordlength; 		// --changed from int to long 5/15/07 by AKH
long 		RIGOL_Bdot_recordlength; 		
long 		JoergerVTR10014_recordlength; 		// --changed from int to long 5/15/07 by AKH
double 		JoergerVTR10012_digitation_rate; 
double 		JoergerVTR10014_digitation_rate; 
double 		RIGOL_Bdot_digitation_rate; 
ViSession 	VTR10012_handle;
ViSession 	VTR10012_memory_handle;
ViSession 	VTR10014_1_handle;
ViSession 	VTR10014_1_memory_handle;
ViSession 	VTR10014_2_handle;
ViSession 	VTR10014_2_memory_handle;
ViSession 	VTR10014_3_handle;
ViSession 	VTR10014_3_memory_handle;
ViSession 	VTR10014_4_handle;
ViSession 	VTR10014_4_memory_handle;
ViSession 	Bdot_RIGOL1_handle;
ViSession 	Bdot_RIGOL2_handle;
int 		JoergerVTR10012_id_value;
int 		JoergerVTR10014_1_id_value;
int 		JoergerVTR10014_2_id_value;
int 		JoergerVTR10014_3_id_value;
int 		JoergerVTR10014_4_id_value;


//Set up the magnetic probe digitizers
void BdotActivate(void)
{
	int 	result; 					//	Error-checking variable
    int 	status;
	int		count;
	float	RIGOL_Bdot_TriggerLevel;
	float	RIGOL_Bdot_TimeWindow;
	char	IP_Address_string[64];
	char	RIGOL_Bdot_TriggerSource[8];
	char	RIGOL1_Bdot_IP_Address_string[64];
	char	RIGOL2_Bdot_IP_Address_string[64];
	char	RIGOL_Bdot_string[32];

	//Open up Bdots settings panel and get target digitizer rate and other settings
	Bdot_panel = LoadPanel (0, "BdotSettings.uir", Bdot);
	RecallPanelState (Bdot_panel, "Master_Control_Storage_File", Bdot_setup_state);
		

	//Setup VTR10012 Joerger Digitizer
	//Get digitizer rate and record length  
	GetCtrlVal (Bdot_panel, Bdot_VTR10012_Clockspeed, &JoergerVTR10012_clockspeed);
	GetCtrlVal (Bdot_panel, Bdot_VTR10012_RECORDLENGTH, &JoergerVTR10012_recordlength);
    
	//Define Joerger Digitation rate for use later
	switch (JoergerVTR10012_clockspeed)
			{
                 
		    case 0:
		       	JoergerVTR10012_digitation_rate=100.0E6;
		       	break;
                    
		   	case 1:
		       	JoergerVTR10012_digitation_rate=50.0E6;
		     	break;
                    		 
		   	case 2:
		      	JoergerVTR10012_digitation_rate=25.0E6;
		        break;
                    
		   	case 3:
		      	JoergerVTR10012_digitation_rate=10.0E6;
		        break;
                    
		   	case 4:
		      	JoergerVTR10012_digitation_rate=5.0E6;
		        break;
                    
		   	case 5:
		      	JoergerVTR10012_digitation_rate=2.5E6;
		        break;
                    
		   	case 6:
		      	JoergerVTR10012_digitation_rate=1.0E6;
		        break;
	}

    //Maybe there is a need to open a session to the VXI controller and reset it because
	//opening sessions to the Joerger instruments is irreproducible.
	
	
	//Get VTR10012 ip address from interface panel
	GetCtrlVal (Bdot_panel, Bdot_Joerger1Address, IP_Address_string);
 	result=viOpen(Global_Visa_Session_Handle, IP_Address_string, VI_NULL, VI_NULL, &VTR10012_handle);
	if ( !result) {
		GetCtrlVal (Bdot_panel, Bdot_Joerger1MemAddress, IP_Address_string);
 		viOpen (Global_Visa_Session_Handle, IP_Address_string, VI_NULL, VI_NULL, &VTR10012_memory_handle);
   
		//Reset VTR10012 and Check its ID
		VTR10012_RESET (VTR10012_handle, VTR10012_Base_address);
		VTR10012_READ_ID(VTR10012_handle, VTR10012_Base_address, &JoergerVTR10012_id_value);

		//Set A32 base address to same value as A16 base address of instrument
		VTR10012_SET_A32_ADDRESS(VTR10012_handle,VTR10012_Base_address, VTR10012_A32_address);

		//Setup clock rate, trigger style, reset memory counter and length of data records
		VTR10012_SET_CLOCK(VTR10012_handle,VTR10012_Base_address, JoergerVTR10012_clockspeed);
		VTR10012_SET_TRIGGER(VTR10012_handle, VTR10012_Base_address);
		VTR10012_SET_RECORD_LENGTH(VTR10012_handle,VTR10012_Base_address, JoergerVTR10012_recordlength);

		VTR10012_RESET_MEM_COUNTER(VTR10012_handle, VTR10012_Base_address);
		Delay(.1);   //wait a bit for instrument to settle
				
		//Check Digitizer
		Delay(.1);  //wait a bit for instrument to settle
		VTR10012_READ_STATUS(VTR10012_handle, VTR10012_Base_address, &status);
	}
	
	//Get digitizer rate and record length  
	GetCtrlVal (Bdot_panel, Bdot_VTR10014_Clockspeed, &JoergerVTR10014_clockspeed);
	GetCtrlVal (Bdot_panel, Bdot_VTR10014_RECORDLENGTH, &JoergerVTR10014_recordlength);
	
	//Define Joerger Digitation rate for use later
	switch (JoergerVTR10014_clockspeed)
		{
                 
	    case 0:
	       	JoergerVTR10014_digitation_rate=100.0E6;
	       	break;
                    
	   	case 1:
	       	JoergerVTR10014_digitation_rate=50.0E6;
	     	break;
    }

	//Get #1 VTR10014 ip address from interface panel
	GetCtrlVal (Bdot_panel, Bdot_Joerger2Address, IP_Address_string);
 	result = viOpen (Global_Visa_Session_Handle, IP_Address_string, VI_NULL, VI_NULL, &VTR10014_1_handle);
	if ( !result) {
		GetCtrlVal (Bdot_panel, Bdot_Joerger2MemAddress, IP_Address_string);
 		viOpen (Global_Visa_Session_Handle, IP_Address_string, VI_NULL, VI_NULL, &VTR10014_1_memory_handle);
	   
		//Reset 1st VTR10014 and Check ID using VTR10012 routines
		VTR10012_RESET (VTR10014_1_handle, VTR10014_1_Base_address);
		VTR10012_READ_ID(VTR10014_1_handle, VTR10014_1_Base_address, &JoergerVTR10014_1_id_value);

		//Set A32 base address to same value as A16 base address of instrument
		VTR10012_SET_A32_ADDRESS(VTR10014_1_handle,VTR10014_1_Base_address, VTR10014_1_A32_address);

		//Setup clock rate, trigger style, reset memory counter and length of data records
		VTR10014_SET_CLOCK(VTR10014_1_handle,VTR10014_1_Base_address, JoergerVTR10014_clockspeed);
		VTR10012_SET_TRIGGER(VTR10014_1_handle,VTR10014_1_Base_address);
		VTR10014_SET_RECORD_LENGTH(VTR10014_1_handle,VTR10014_1_Base_address, JoergerVTR10014_recordlength);
		VTR10012_RESET_MEM_COUNTER(VTR10014_1_handle,VTR10014_1_Base_address);
		
		//Check Digitizer
		Delay(.1);
		VTR10012_READ_STATUS(VTR10014_1_handle,VTR10014_1_Base_address, &status);
	}
	
	//Get #2 VTR10014 ip address from interface panel
	GetCtrlVal (Bdot_panel, Bdot_Joerger3Address, IP_Address_string);
 	result=viOpen (Global_Visa_Session_Handle, IP_Address_string, VI_NULL, VI_NULL, &VTR10014_2_handle);
	if ( !result) {
		GetCtrlVal (Bdot_panel, Bdot_Joerger3MemAddress, IP_Address_string);
 		viOpen (Global_Visa_Session_Handle, IP_Address_string, VI_NULL, VI_NULL, &VTR10014_2_memory_handle);

	    //Reset 2nd VTR10014 and Check ID using VTR10012 routines
		VTR10012_RESET (VTR10014_2_handle, VTR10014_2_Base_address);
		VTR10012_READ_ID(VTR10014_2_handle, VTR10014_2_Base_address, &JoergerVTR10014_2_id_value);

		//Set A32 base address to same value as A16 base address of instrument
		VTR10012_SET_A32_ADDRESS(VTR10014_2_handle,VTR10014_2_Base_address, VTR10014_2_A32_address);

		//Setup clock rate, trigger style, reset memory counter and length of data records
		VTR10014_SET_CLOCK(VTR10014_2_handle,VTR10014_2_Base_address, JoergerVTR10014_clockspeed);
		VTR10012_SET_TRIGGER(VTR10014_2_handle,VTR10014_2_Base_address);
		VTR10014_SET_RECORD_LENGTH(VTR10014_2_handle,VTR10014_2_Base_address, JoergerVTR10014_recordlength);
		VTR10012_RESET_MEM_COUNTER(VTR10014_2_handle,VTR10014_2_Base_address);
		
		//Check Digitizer
		Delay(.1);
		VTR10012_READ_STATUS(VTR10014_2_handle,VTR10014_2_Base_address, &status);
	}
	

	//Get #3 VTR10014 ip address from interface panel
	GetCtrlVal (Bdot_panel, Bdot_Joerger4Address, IP_Address_string);
 	result=viOpen (Global_Visa_Session_Handle, IP_Address_string, VI_NULL, VI_NULL, &VTR10014_3_handle);
	if ( !result) {
		GetCtrlVal (Bdot_panel, Bdot_Joerger4MemAddress, IP_Address_string);
 		viOpen (Global_Visa_Session_Handle, IP_Address_string, VI_NULL, VI_NULL, &VTR10014_3_memory_handle);

	   	//Reset 3rd VTR10014 and Check ID using VTR10012 routines
		VTR10012_RESET (VTR10014_3_handle, VTR10014_3_Base_address);
		VTR10012_READ_ID(VTR10014_3_handle, VTR10014_3_Base_address, &JoergerVTR10014_3_id_value);

		//Set A32 base address to same value as A16 base address of instrument
		VTR10012_SET_A32_ADDRESS(VTR10014_3_handle,VTR10014_3_Base_address, VTR10014_3_A32_address);

		//Setup clock rate, trigger style, reset memory counter and length of data records
		VTR10014_SET_CLOCK(VTR10014_3_handle,VTR10014_3_Base_address, JoergerVTR10014_clockspeed);
		VTR10012_SET_TRIGGER(VTR10014_3_handle,VTR10014_3_Base_address);
		VTR10014_SET_RECORD_LENGTH(VTR10014_3_handle,VTR10014_3_Base_address, JoergerVTR10014_recordlength);
		VTR10012_RESET_MEM_COUNTER(VTR10014_3_handle,VTR10014_3_Base_address);
		
		//Check Digitizer
		Delay(.1);
		VTR10012_READ_STATUS(VTR10014_3_handle,VTR10014_3_Base_address, &status);
	}
	
 	//Get #4 VTR10014 ip address from interface panel
	GetCtrlVal (Bdot_panel, Bdot_Joerger5Address, IP_Address_string);
 	result=viOpen (Global_Visa_Session_Handle, IP_Address_string, VI_NULL, VI_NULL, &VTR10014_4_handle);
	if ( !result) {
		GetCtrlVal (Bdot_panel, Bdot_Joerger5MemAddress, IP_Address_string);
 		viOpen (Global_Visa_Session_Handle, IP_Address_string, VI_NULL, VI_NULL, &VTR10014_4_memory_handle);

	   	//Reset 3rd VTR10014 and Check ID using VTR10012 routines
		VTR10012_RESET (VTR10014_4_handle, VTR10014_4_Base_address);
		VTR10012_READ_ID(VTR10014_4_handle, VTR10014_4_Base_address, &JoergerVTR10014_4_id_value);

		//Set A32 base address to same value as A16 base address of instrument
		VTR10012_SET_A32_ADDRESS(VTR10014_4_handle,VTR10014_4_Base_address, VTR10014_4_A32_address);

		//Setup clock rate, trigger style, reset memory counter and length of data records
		VTR10014_SET_CLOCK(VTR10014_4_handle,VTR10014_4_Base_address, JoergerVTR10014_clockspeed);
		VTR10012_SET_TRIGGER(VTR10014_4_handle,VTR10014_4_Base_address);
		VTR10014_SET_RECORD_LENGTH(VTR10014_4_handle,VTR10014_4_Base_address, JoergerVTR10014_recordlength);
		VTR10012_RESET_MEM_COUNTER(VTR10014_4_handle,VTR10014_4_Base_address);
		
		//Check Digitizer
		Delay(.1);
		VTR10012_READ_STATUS(VTR10014_4_handle,VTR10014_4_Base_address, &status);
	}
	
	//Set up the two Rigol Oscilloscopes that also take Bdot data
	//Get sample rate, record length, and channel scales
	GetCtrlVal (Bdot_panel, Bdot_RIGOL_Clockspeed, &RIGOL_Bdot_clockspeed);
	GetCtrlVal (Bdot_panel, Bdot_RIGOL_RECORDLENGTH, &RIGOL_Bdot_recordlength);
	GetCtrlVal (Bdot_panel, Bdot_TriggerSource,RIGOL_Bdot_TriggerSource);
	GetCtrlVal (Bdot_panel, Bdot_TriggerLevel,&RIGOL_Bdot_TriggerLevel);
	
	//Define Rigol Digitation rate for use later
	switch (RIGOL_Bdot_clockspeed)
			{
                 
		    case 0:
		       	RIGOL_Bdot_digitation_rate=100.0E6;
		       	break;
                    
		   	case 1:
		       	RIGOL_Bdot_digitation_rate=50.0E6;
		     	break;
                    		 
		   	case 2:
		      	RIGOL_Bdot_digitation_rate=25.0E6;
		        break;
                    
		   	case 3:
		      	RIGOL_Bdot_digitation_rate=10.0E6;
		        break;
                    
		   	case 4:
		      	RIGOL_Bdot_digitation_rate=5.0E6;
		        break;
                    
		   	case 5:
		      	RIGOL_Bdot_digitation_rate=2.5E6;
		        break;
                    
		   	case 6:
		      	RIGOL_Bdot_digitation_rate=1.0E6;
		        break;
	}

	//Calculate rounded off time window to sue for setting Rigol scope
	RIGOL_Bdot_TimeWindow=(float)(((int)(1000.0*RIGOL_Bdot_recordlength/RIGOL_Bdot_digitation_rate))/1000.0);

	//Get Rigol ip address from interface panel and open connection to the two scopes
	GetCtrlVal (Bdot_panel, Bdot_RIGOL1Address, RIGOL1_Bdot_IP_Address_string);
 	result=viOpen (Global_Visa_Session_Handle, RIGOL1_Bdot_IP_Address_string, VI_NULL, VI_NULL, &Bdot_RIGOL1_handle);
	if (!result) {
		//Make sure scope is running so commands will work
		sprintf (RIGOL_Bdot_string, ":RUN\r");   																							//Send the run command
		result = viWrite (Bdot_RIGOL1_handle, RIGOL_Bdot_string, (unsigned int)strlen(RIGOL_Bdot_string), &count);
	
		//Setup the rate, record length, trigger system, and voltage range for the 1st Rigol scope
	  	sprintf (RIGOL_Bdot_string, ":TIM:MAIN:SCAL %f\r",(RIGOL_Bdot_TimeWindow/10.0));   										//Set up timebase scaling based on target sample window and 10 divisions
		result = viWrite (Bdot_RIGOL1_handle, RIGOL_Bdot_string, (unsigned int)strlen(RIGOL_Bdot_string), &count);
		sprintf (RIGOL_Bdot_string, ":ACQ:MDEP %d\r",RIGOL_Bdot_recordlength);   												//Set the record length by setting the Memory Depth and upload the record length in k
		result = viWrite (Bdot_RIGOL1_handle, RIGOL_Bdot_string, (unsigned int)strlen(RIGOL_Bdot_string), &count);
		sprintf (RIGOL_Bdot_string, ":CHAN1:SCAL  10.0\r");   																	//Set the voltage scale of the channel to 10 V to match Joerger
		result = viWrite (Bdot_RIGOL1_handle, RIGOL_Bdot_string, (unsigned int)strlen(RIGOL_Bdot_string), &count);
		sprintf (RIGOL_Bdot_string, ":CHAN2:SCAL  10.0\r");   																	//Set the voltage scale of the channel to 10 V to match Joerger
		result = viWrite (Bdot_RIGOL1_handle, RIGOL_Bdot_string, (unsigned int)strlen(RIGOL_Bdot_string), &count);
		sprintf (RIGOL_Bdot_string, ":CHAN3:SCAL  10.0\r");   																	//Set the voltage scale of the channel to 10 V to match Joerger
		result = viWrite (Bdot_RIGOL1_handle, RIGOL_Bdot_string, (unsigned int)strlen(RIGOL_Bdot_string), &count);
		sprintf (RIGOL_Bdot_string, ":CHAN4:SCAL  10.0\r");   																	//Set the voltage scale of the channel to 10 V to match Joerger
		result = viWrite (Bdot_RIGOL1_handle, RIGOL_Bdot_string, (unsigned int)strlen(RIGOL_Bdot_string), &count);
	 	sprintf (RIGOL_Bdot_string, ":TRIG:MODE EDGE\r");   
		result = viWrite (Bdot_RIGOL1_handle, RIGOL_Bdot_string, (unsigned int)strlen(RIGOL_Bdot_string), &count);
		sprintf (RIGOL_Bdot_string, ":TRIG:COUP AC\r");   
		result = viWrite (Bdot_RIGOL1_handle, RIGOL_Bdot_string, (unsigned int)strlen(RIGOL_Bdot_string), &count);
		sprintf (RIGOL_Bdot_string, ":TRIG:EDGE:SOUR %s\r",RIGOL_Bdot_TriggerSource);   										//Set the trigger source to the target value 
		result = viWrite (Bdot_RIGOL1_handle, RIGOL_Bdot_string, (unsigned int)strlen(RIGOL_Bdot_string), &count);
	 	sprintf (RIGOL_Bdot_string, ":TRIG:EDGE:SLOP POS\r");   
		result = viWrite (Bdot_RIGOL1_handle, RIGOL_Bdot_string, (unsigned int)strlen(RIGOL_Bdot_string), &count);
		sprintf (RIGOL_Bdot_string, ":TRIG:EDGE:LEV %f\r",RIGOL_Bdot_TriggerLevel);   
		result = viWrite (Bdot_RIGOL1_handle, RIGOL_Bdot_string, (unsigned int)strlen(RIGOL_Bdot_string), &count);
		sprintf (RIGOL_Bdot_string, ":SYST:KEY:PRES MOFF\r");   												 				//Hide menu on scope screen
	 	result = viWrite (Bdot_RIGOL1_handle, RIGOL_Bdot_string, (unsigned int)strlen(RIGOL_Bdot_string), &count);
	}

	//Get Rigol ip address from interface panel and open connection to the two scopes
	GetCtrlVal (Bdot_panel, Bdot_RIGOL2Address, RIGOL2_Bdot_IP_Address_string);
	result = viOpen (Global_Visa_Session_Handle, RIGOL2_Bdot_IP_Address_string, VI_NULL, VI_NULL, &Bdot_RIGOL2_handle);
	if(!result) {
		//Make sure scope is running so commands will work
		sprintf (RIGOL_Bdot_string, ":RUN\r");   																							//Send the run command
		result = viWrite (Bdot_RIGOL2_handle, RIGOL_Bdot_string, (unsigned int)strlen(RIGOL_Bdot_string), &count);

		//Setup the rate, record length, trigger system, and voltage range for the 2nd Rigol scope
	  	sprintf (RIGOL_Bdot_string, ":TIM:MAIN:SCAL %f\r",(RIGOL_Bdot_TimeWindow/10.0));   										//Set up timebase scaling based on target sample window and 10 divisions
		result = viWrite (Bdot_RIGOL2_handle, RIGOL_Bdot_string, (unsigned int)strlen(RIGOL_Bdot_string), &count);
		sprintf (RIGOL_Bdot_string, ":ACQ:MDEP %d\r",RIGOL_Bdot_recordlength);   												//Set the record length by setting the Memory Depth and upload the record length in k
		result = viWrite (Bdot_RIGOL2_handle, RIGOL_Bdot_string, (unsigned int)strlen(RIGOL_Bdot_string), &count);
		sprintf (RIGOL_Bdot_string, ":CHAN1:SCAL  10.0\r");   																	//Set the voltage scale of the channel to 10 V to match Joerger
		result = viWrite (Bdot_RIGOL2_handle, RIGOL_Bdot_string, (unsigned int)strlen(RIGOL_Bdot_string), &count);
		sprintf (RIGOL_Bdot_string, ":CHAN2:SCAL  10.0\r");   																	//Set the voltage scale of the channel to 10 V to match Joerger
		result = viWrite (Bdot_RIGOL2_handle, RIGOL_Bdot_string, (unsigned int)strlen(RIGOL_Bdot_string), &count);
		sprintf (RIGOL_Bdot_string, ":CHAN3:SCAL  10.0\r");   																	//Set the voltage scale of the channel to 10 V to match Joerger
		result = viWrite (Bdot_RIGOL2_handle, RIGOL_Bdot_string, (unsigned int)strlen(RIGOL_Bdot_string), &count);
		sprintf (RIGOL_Bdot_string, ":CHAN4:SCAL  10.0\r");   																	//Set the voltage scale of the channel to 10 V to match Joerger
		result = viWrite (Bdot_RIGOL2_handle, RIGOL_Bdot_string, (unsigned int)strlen(RIGOL_Bdot_string), &count);
	 	sprintf (RIGOL_Bdot_string, ":TRIG:MODE EDGE\r");   
		result = viWrite (Bdot_RIGOL2_handle, RIGOL_Bdot_string, (unsigned int)strlen(RIGOL_Bdot_string), &count);
		sprintf (RIGOL_Bdot_string, ":TRIG:COUP AC\r");   
		result = viWrite (Bdot_RIGOL2_handle, RIGOL_Bdot_string, (unsigned int)strlen(RIGOL_Bdot_string), &count);
		sprintf (RIGOL_Bdot_string, ":TRIG:EDGE:SOUR %s\r",RIGOL_Bdot_TriggerSource);   										//Set the trigger source to the target value 
		result = viWrite (Bdot_RIGOL2_handle, RIGOL_Bdot_string, (unsigned int)strlen(RIGOL_Bdot_string), &count);
	 	sprintf (RIGOL_Bdot_string, ":TRIG:EDGE:SLOP POS\r");   
		result = viWrite (Bdot_RIGOL2_handle, RIGOL_Bdot_string, (unsigned int)strlen(RIGOL_Bdot_string), &count);
		sprintf (RIGOL_Bdot_string, ":TRIG:EDGE:LEV %f\r",RIGOL_Bdot_TriggerLevel);   
		result = viWrite (Bdot_RIGOL2_handle, RIGOL_Bdot_string, (unsigned int)strlen(RIGOL_Bdot_string), &count);
		sprintf (RIGOL_Bdot_string, ":SYST:KEY:PRES MOFF\r");   												 				//Hide menu on scope screen
	 	result = viWrite (Bdot_RIGOL2_handle, RIGOL_Bdot_string, (unsigned int)strlen(RIGOL_Bdot_string), &count);
	}
		 
	//Close Panel now done
    HidePanel(Bdot_panel);
}


//Arm the Joerger digitizers
int BdotArm(void)
{
	int		status;
	int		count;
	int		result;
	char	RIGOL_Bdot_string[32];

	//Disarm the VTR10012 digitizer and clear the IRQ source
	VTR10012_DISARM(VTR10012_handle, VTR10012_Base_address);
	VTR10012_RESET_MEM_COUNTER(VTR10012_handle, VTR10012_Base_address);
	VTR10012_ARM(VTR10012_handle, VTR10012_Base_address);
	do {
		Delay (.1); //wait before checking for completion of cycle
		VTR10012_READ_STATUS(VTR10012_handle, VTR10012_Base_address,&status);
	}while(status != 1);   //	when the status = armed, exit
	
	//Disarm the 1st VTR10014 digitizer and clear the IRQ source
	VTR10012_DISARM(VTR10014_1_handle,VTR10014_1_Base_address);
	VTR10012_RESET_MEM_COUNTER(VTR10014_1_handle,VTR10014_1_Base_address);
	VTR10012_ARM(VTR10014_1_handle,VTR10014_1_Base_address);
	do {
		Delay (.1); //wait before checking for completion of cycle
		VTR10012_READ_STATUS(VTR10014_1_handle,VTR10014_1_Base_address,&status);
	}while(status != 1);   //	when the status = armed, exit


	//Disarm the 2nd VTR10014 digitizer and clear the IRQ source
	VTR10012_DISARM(VTR10014_2_handle,VTR10014_2_Base_address);
	VTR10012_RESET_MEM_COUNTER(VTR10014_2_handle,VTR10014_2_Base_address);
	VTR10012_ARM(VTR10014_2_handle,VTR10014_2_Base_address);
	do {
		Delay (.1); //wait before checking for completion of cycle
		VTR10012_READ_STATUS(VTR10014_2_handle,VTR10014_2_Base_address,&status);
	}while(status != 1);   //	when the status = armed, exit


	//Disarm the 3rd VTR10014 digitizer and clear the IRQ source
	VTR10012_DISARM(VTR10014_3_handle,VTR10014_3_Base_address);
	VTR10012_RESET_MEM_COUNTER(VTR10014_3_handle,VTR10014_3_Base_address);
	VTR10012_ARM(VTR10014_3_handle,VTR10014_3_Base_address);
	do {
		Delay (.1); //wait before checking for completion of cycle
		VTR10012_READ_STATUS(VTR10014_3_handle,VTR10014_3_Base_address,&status);
	}while(status != 1);   //	when the status = armed, exit

	//Disarm the 4th VTR10014 digitizer and clear the IRQ source
	VTR10012_DISARM(VTR10014_4_handle,VTR10014_4_Base_address);
	VTR10012_RESET_MEM_COUNTER(VTR10014_4_handle,VTR10014_4_Base_address);
	VTR10012_ARM(VTR10014_4_handle,VTR10014_4_Base_address);
	do {
		Delay (.1); //wait before checking for completion of cycle
		VTR10012_READ_STATUS(VTR10014_4_handle,VTR10014_4_Base_address,&status);
	}while(status != 1);   //	when the status = armed, exit

	
	//Arm the two Rigol scopes
	sprintf (RIGOL_Bdot_string,":TRIG:SWE SINGl\r");   												 		//Single Trigger Sweep
 	result = viWrite (Bdot_RIGOL1_handle, RIGOL_Bdot_string, (unsigned int)strlen(RIGOL_Bdot_string), &count);
 	result = viWrite (Bdot_RIGOL2_handle, RIGOL_Bdot_string, (unsigned int)strlen(RIGOL_Bdot_string), &count);

	//Temporarily software trigger all four Joergers so that data transfer can occur
	VTR10012_SOFTWARE_TRIGGER(VTR10012_handle, 0);
	VTR10012_SOFTWARE_TRIGGER(VTR10014_1_handle, 0);
	VTR10012_SOFTWARE_TRIGGER(VTR10014_2_handle, 0);
	VTR10012_SOFTWARE_TRIGGER(VTR10014_3_handle, 0);
	VTR10012_SOFTWARE_TRIGGER(VTR10014_4_handle, 0);


	return status;
}


//Take the acquired data from the Bdot digitizers and print it to the common data folder and 
//also push it to the MDS database
void Write_BdotData(void)
{

	char	ChannelNameString[36];
	char	outfilename[64];
	FILE*	outfile;
	int		j;
	float 	Bdot_Rigol_timestep=0.0;
	double  Bdot_signal1[max_record_length];
	double  Bdot_signal2[max_record_length];
	double  Bdot_signal3[max_record_length];
	double  Bdot_signal4[max_record_length];
	double  Bdot_signal5[max_record_length];
	double  Bdot_signal6[max_record_length];
	double  Bdot_signal7[max_record_length];
	double  Bdot_signal8[max_record_length];
	double  time_array[max_record_length];	
	
	//Open settings panel and load saved values
	Bdot_panel = LoadPanel (0, "BdotSettings.uir", Bdot);
	RecallPanelState (Bdot_panel, "Master_Control_Storage_File", Bdot_setup_state);

	//Retrieve 8 timeseries from the first VTR10012 instrument and write them to MDS system and raw files 
	JoergerVTR10012_acquire(VTR10012_memory_handle, VTR10012_A32_address, 1,JoergerVTR10012_recordlength,Bdot_signal1,Bdot_signal5);
	JoergerVTR10012_acquire(VTR10012_memory_handle, VTR10012_A32_address, 2,JoergerVTR10012_recordlength,Bdot_signal2,Bdot_signal6);
	JoergerVTR10012_acquire(VTR10012_memory_handle, VTR10012_A32_address, 3,JoergerVTR10012_recordlength,Bdot_signal3,Bdot_signal7);
	JoergerVTR10012_acquire(VTR10012_memory_handle, VTR10012_A32_address, 4,JoergerVTR10012_recordlength,Bdot_signal4,Bdot_signal8);
	
	//Dump results to a file
	sprintf(outfilename, RawDataPath);
	strcat(outfilename, ShotNumberString);
	strcat(outfilename, "_");
	strcat(outfilename, "Bdot1-8.txt");
	
	outfile=fopen (outfilename, "w");
	fprintf(outfile,"t(us), 1, 2, 3,4, 5, 6, 7, 8\n");
	for (j=0;j<JoergerVTR10012_recordlength;j++) {
		time_array[j]=(double)(1.0E6*j/JoergerVTR10012_digitation_rate);
		fprintf(outfile,"%f,%f,%f,%f,%f,%f,%f,%f,%f\n",time_array[j],Bdot_signal1[j],Bdot_signal2[j],Bdot_signal3[j],Bdot_signal4[j],
			Bdot_signal5[j],Bdot_signal6[j],Bdot_signal7[j],Bdot_signal8[j]);
	}
	
 	//Close Bdot data file
	fclose (outfile);

	
	//Get channel name string, store raw data on drive folder, and push data to MDSPlus for 8 channels
	GetCtrlVal (Bdot_panel, Bdot_Chan1Name,ChannelNameString);
	Write_to_PHASMA_MDS(time_array,Bdot_signal1,ChannelNameString);
	
	GetCtrlVal (Bdot_panel, Bdot_Chan2Name,ChannelNameString);
	Write_to_PHASMA_MDS(time_array,Bdot_signal2,ChannelNameString);

	GetCtrlVal (Bdot_panel, Bdot_Chan3Name,ChannelNameString);
	Write_to_PHASMA_MDS(time_array,Bdot_signal3,ChannelNameString);

	GetCtrlVal (Bdot_panel, Bdot_Chan4Name,ChannelNameString);
	Write_to_PHASMA_MDS(time_array,Bdot_signal4,ChannelNameString);
	
	GetCtrlVal (Bdot_panel, Bdot_Chan5Name,ChannelNameString);
	Write_to_PHASMA_MDS(time_array,Bdot_signal5,ChannelNameString);

	GetCtrlVal (Bdot_panel, Bdot_Chan6Name,ChannelNameString);
	Write_to_PHASMA_MDS(time_array,Bdot_signal6,ChannelNameString);
	
	GetCtrlVal (Bdot_panel, Bdot_Chan7Name,ChannelNameString);
	Write_to_PHASMA_MDS(time_array,Bdot_signal7,ChannelNameString);

	GetCtrlVal (Bdot_panel, Bdot_Chan8Name,ChannelNameString);
	Write_to_PHASMA_MDS(time_array,Bdot_signal8,ChannelNameString);
	
		
	//Retrieve data from the first VTR10014 instrumentin pairs and write them to MDS system and raw files  
 	JoergerVTR10014_acquire(VTR10014_1_memory_handle, VTR10014_1_A32_addressFull, 1,JoergerVTR10014_recordlength,Bdot_signal1,Bdot_signal5);
 	JoergerVTR10014_acquire(VTR10014_1_memory_handle, VTR10014_1_A32_addressFull, 2,JoergerVTR10014_recordlength,Bdot_signal2,Bdot_signal6);
 	JoergerVTR10014_acquire(VTR10014_1_memory_handle, VTR10014_1_A32_addressFull, 3,JoergerVTR10014_recordlength,Bdot_signal3,Bdot_signal7);
 	JoergerVTR10014_acquire(VTR10014_1_memory_handle, VTR10014_1_A32_addressFull, 4,JoergerVTR10014_recordlength,Bdot_signal4,Bdot_signal8);

	
	//Dump results to a file
	sprintf(outfilename, RawDataPath);
	strcat(outfilename, ShotNumberString);
	strcat(outfilename, "_");
	strcat(outfilename, "Bdot9-16.txt");
	
	outfile=fopen (outfilename, "w");
	fprintf(outfile,"t(us), 1, 2, 3,4, 5, 6, 7, 8\n");
	for (j=0;j<JoergerVTR10014_recordlength;j++) {
		time_array[j]=(double)(1.0E6*j/JoergerVTR10014_digitation_rate);
		fprintf(outfile,"%f,%f,%f,%f,%f,%f,%f,%f,%f\n",time_array[j],Bdot_signal1[j],Bdot_signal2[j],Bdot_signal3[j],Bdot_signal4[j],
			Bdot_signal5[j],Bdot_signal6[j],Bdot_signal7[j],Bdot_signal8[j]);
	}
	
 	//Close Bdot data file
	fclose (outfile);

	
	//Get channel name string, store raw data on drive folder, and push data to MDSPlus for 8 channels
	GetCtrlVal (Bdot_panel, Bdot_Chan9Name,ChannelNameString);
	Write_to_PHASMA_MDS(time_array,Bdot_signal1,ChannelNameString);
	
	GetCtrlVal (Bdot_panel, Bdot_Chan10Name,ChannelNameString);
	Write_to_PHASMA_MDS(time_array,Bdot_signal2,ChannelNameString);

	GetCtrlVal (Bdot_panel, Bdot_Chan11Name,ChannelNameString);
	Write_to_PHASMA_MDS(time_array,Bdot_signal3,ChannelNameString);

	GetCtrlVal (Bdot_panel, Bdot_Chan12Name,ChannelNameString);
	Write_to_PHASMA_MDS(time_array,Bdot_signal4,ChannelNameString);
	
	GetCtrlVal (Bdot_panel, Bdot_Chan13Name,ChannelNameString);
	Write_to_PHASMA_MDS(time_array,Bdot_signal5,ChannelNameString);

	GetCtrlVal (Bdot_panel, Bdot_Chan14Name,ChannelNameString);
	Write_to_PHASMA_MDS(time_array,Bdot_signal6,ChannelNameString);
	
	GetCtrlVal (Bdot_panel, Bdot_Chan15Name,ChannelNameString);
	Write_to_PHASMA_MDS(time_array,Bdot_signal7,ChannelNameString);

	GetCtrlVal (Bdot_panel, Bdot_Chan16Name,ChannelNameString);
	Write_to_PHASMA_MDS(time_array,Bdot_signal8,ChannelNameString);
	
	
	
	
	
	//Retrieve data from the second VTR10014 instrument
 	JoergerVTR10014_acquire(VTR10014_2_memory_handle, VTR10014_2_A32_addressFull, 1,JoergerVTR10014_recordlength,Bdot_signal1,Bdot_signal5);
 	JoergerVTR10014_acquire(VTR10014_2_memory_handle, VTR10014_2_A32_addressFull, 2,JoergerVTR10014_recordlength,Bdot_signal2,Bdot_signal6);
 	JoergerVTR10014_acquire(VTR10014_2_memory_handle, VTR10014_2_A32_addressFull, 3,JoergerVTR10014_recordlength,Bdot_signal3,Bdot_signal7);
 	JoergerVTR10014_acquire(VTR10014_2_memory_handle, VTR10014_2_A32_addressFull, 4,JoergerVTR10014_recordlength,Bdot_signal4,Bdot_signal8);

	//Dump results to a file
	sprintf(outfilename, RawDataPath);
	strcat(outfilename, ShotNumberString);
	strcat(outfilename, "_");
	strcat(outfilename, "Bdot17-24.txt");
	
	outfile=fopen (outfilename, "w");
	fprintf(outfile,"t(us), 1, 2, 3,4, 5, 6, 7, 8\n");
	for (j=0;j<JoergerVTR10014_recordlength;j++) {
		time_array[j]=(double)(1.0E6*j/JoergerVTR10014_digitation_rate);
		fprintf(outfile,"%f,%f,%f,%f,%f,%f,%f,%f,%f\n",time_array[j],Bdot_signal1[j],Bdot_signal2[j],Bdot_signal3[j],Bdot_signal4[j],
			Bdot_signal5[j],Bdot_signal6[j],Bdot_signal7[j],Bdot_signal8[j]);
	}
	
 	//Close Bdot data file
	fclose (outfile);

	
	//Get channel name string, store raw data on drive folder, and push data to MDSPlus for 8 channels
	GetCtrlVal (Bdot_panel, Bdot_Chan17Name,ChannelNameString);
	Write_to_PHASMA_MDS(time_array,Bdot_signal1,ChannelNameString);
	
	GetCtrlVal (Bdot_panel, Bdot_Chan18Name,ChannelNameString);
	Write_to_PHASMA_MDS(time_array,Bdot_signal2,ChannelNameString);

	GetCtrlVal (Bdot_panel, Bdot_Chan19Name,ChannelNameString);
	Write_to_PHASMA_MDS(time_array,Bdot_signal3,ChannelNameString);

	GetCtrlVal (Bdot_panel, Bdot_Chan20Name,ChannelNameString);
	Write_to_PHASMA_MDS(time_array,Bdot_signal4,ChannelNameString);
	
	GetCtrlVal (Bdot_panel, Bdot_Chan21Name,ChannelNameString);
	Write_to_PHASMA_MDS(time_array,Bdot_signal5,ChannelNameString);

	GetCtrlVal (Bdot_panel, Bdot_Chan22Name,ChannelNameString);
	Write_to_PHASMA_MDS(time_array,Bdot_signal6,ChannelNameString);
	
	GetCtrlVal (Bdot_panel, Bdot_Chan23Name,ChannelNameString);
	Write_to_PHASMA_MDS(time_array,Bdot_signal7,ChannelNameString);

	GetCtrlVal (Bdot_panel, Bdot_Chan24Name,ChannelNameString);
	Write_to_PHASMA_MDS(time_array,Bdot_signal8,ChannelNameString);
	
	
	
	
	//Retrieve data from the third VTR10014 instrument
 	JoergerVTR10014_acquire(VTR10014_3_memory_handle, VTR10014_3_A32_addressFull, 1,JoergerVTR10014_recordlength,Bdot_signal1,Bdot_signal5);
 	JoergerVTR10014_acquire(VTR10014_3_memory_handle, VTR10014_3_A32_addressFull, 2,JoergerVTR10014_recordlength,Bdot_signal2,Bdot_signal6);
 	JoergerVTR10014_acquire(VTR10014_3_memory_handle, VTR10014_3_A32_addressFull, 3,JoergerVTR10014_recordlength,Bdot_signal3,Bdot_signal7);
 	JoergerVTR10014_acquire(VTR10014_3_memory_handle, VTR10014_3_A32_addressFull, 4,JoergerVTR10014_recordlength,Bdot_signal4,Bdot_signal8);
	
	//Dump results to a file
	sprintf(outfilename, RawDataPath);
	strcat(outfilename, ShotNumberString);
	strcat(outfilename, "_");
	strcat(outfilename, "Bdot25-32.txt");
	
	outfile=fopen (outfilename, "w");
	fprintf(outfile,"t(us), 1, 2, 3,4, 5, 6, 7, 8\n");
	for (j=0;j<JoergerVTR10014_recordlength;j++) {
		time_array[j]=(double)(1.0E6*j/JoergerVTR10014_digitation_rate);
		fprintf(outfile,"%f,%f,%f,%f,%f,%f,%f,%f,%f\n",time_array[j],Bdot_signal1[j],Bdot_signal2[j],Bdot_signal3[j],Bdot_signal4[j],
			Bdot_signal5[j],Bdot_signal6[j],Bdot_signal7[j],Bdot_signal8[j]);
	}
	
 	//Close Bdot data file
	fclose (outfile);

	
	//Get channel name string, store raw data on drive folder, and push data to MDSPlus for 8 channels
	GetCtrlVal (Bdot_panel, Bdot_Chan25Name,ChannelNameString);
	Write_to_PHASMA_MDS(time_array,Bdot_signal1,ChannelNameString);
	
	GetCtrlVal (Bdot_panel, Bdot_Chan26Name,ChannelNameString);
	Write_to_PHASMA_MDS(time_array,Bdot_signal2,ChannelNameString);

	GetCtrlVal (Bdot_panel, Bdot_Chan27Name,ChannelNameString);
	Write_to_PHASMA_MDS(time_array,Bdot_signal3,ChannelNameString);

	GetCtrlVal (Bdot_panel, Bdot_Chan28Name,ChannelNameString);
	Write_to_PHASMA_MDS(time_array,Bdot_signal4,ChannelNameString);
	
	GetCtrlVal (Bdot_panel, Bdot_Chan29Name,ChannelNameString);
	Write_to_PHASMA_MDS(time_array,Bdot_signal5,ChannelNameString);

	GetCtrlVal (Bdot_panel, Bdot_Chan30Name,ChannelNameString);
	Write_to_PHASMA_MDS(time_array,Bdot_signal6,ChannelNameString);
	
	GetCtrlVal (Bdot_panel, Bdot_Chan31Name,ChannelNameString);
	Write_to_PHASMA_MDS(time_array,Bdot_signal7,ChannelNameString);

	GetCtrlVal (Bdot_panel, Bdot_Chan32Name,ChannelNameString);
	Write_to_PHASMA_MDS(time_array,Bdot_signal8,ChannelNameString);
	
	//Retrieve data from the fouth VTR10014 instrument
 	JoergerVTR10014_acquire(VTR10014_4_memory_handle, VTR10014_4_A32_addressFull, 1,JoergerVTR10014_recordlength,Bdot_signal1,Bdot_signal5);
 	JoergerVTR10014_acquire(VTR10014_4_memory_handle, VTR10014_4_A32_addressFull, 2,JoergerVTR10014_recordlength,Bdot_signal2,Bdot_signal6);
 	JoergerVTR10014_acquire(VTR10014_4_memory_handle, VTR10014_4_A32_addressFull, 3,JoergerVTR10014_recordlength,Bdot_signal3,Bdot_signal7);
 	JoergerVTR10014_acquire(VTR10014_4_memory_handle, VTR10014_4_A32_addressFull, 4,JoergerVTR10014_recordlength,Bdot_signal4,Bdot_signal8);
	
	//Dump results to a file
	sprintf(outfilename, RawDataPath);
	strcat(outfilename, ShotNumberString);
	strcat(outfilename, "_");
	strcat(outfilename, "Bdot25-32.txt");
	
	outfile=fopen (outfilename, "w");
	fprintf(outfile,"t(us), 1, 2, 3,4, 5, 6, 7, 8\n");
	for (j=0;j<JoergerVTR10014_recordlength;j++) {
		time_array[j]=(double)(1.0E6*j/JoergerVTR10014_digitation_rate);
		fprintf(outfile,"%f,%f,%f,%f,%f,%f,%f,%f,%f\n",time_array[j],Bdot_signal1[j],Bdot_signal2[j],Bdot_signal3[j],Bdot_signal4[j],
			Bdot_signal5[j],Bdot_signal6[j],Bdot_signal7[j],Bdot_signal8[j]);
	}
	
 	//Close Bdot data file
	fclose (outfile);

	
	//Get channel name string, store raw data on drive folder, and push data to MDSPlus for 8 channels
	GetCtrlVal (Bdot_panel, Bdot_Chan25Name,ChannelNameString);
	Write_to_PHASMA_MDS(time_array,Bdot_signal1,ChannelNameString);
	
	GetCtrlVal (Bdot_panel, Bdot_Chan26Name,ChannelNameString);
	Write_to_PHASMA_MDS(time_array,Bdot_signal2,ChannelNameString);

	GetCtrlVal (Bdot_panel, Bdot_Chan27Name,ChannelNameString);
	Write_to_PHASMA_MDS(time_array,Bdot_signal3,ChannelNameString);

	GetCtrlVal (Bdot_panel, Bdot_Chan28Name,ChannelNameString);
	Write_to_PHASMA_MDS(time_array,Bdot_signal4,ChannelNameString);
	
	GetCtrlVal (Bdot_panel, Bdot_Chan29Name,ChannelNameString);
	Write_to_PHASMA_MDS(time_array,Bdot_signal5,ChannelNameString);

	GetCtrlVal (Bdot_panel, Bdot_Chan30Name,ChannelNameString);
	Write_to_PHASMA_MDS(time_array,Bdot_signal6,ChannelNameString);
	
	GetCtrlVal (Bdot_panel, Bdot_Chan31Name,ChannelNameString);
	Write_to_PHASMA_MDS(time_array,Bdot_signal7,ChannelNameString);

	GetCtrlVal (Bdot_panel, Bdot_Chan32Name,ChannelNameString);
	Write_to_PHASMA_MDS(time_array,Bdot_signal8,ChannelNameString);
	
	
	//Retrieve data from the two RIGOL oscilloscopes and store the data
	/*
	//Grab data from all four channels of the oscilloscope
	Rigol(Bdot_RIGOL1_handle, RIGOL_Bdot_recordlength, Bdot_Rigol_timestep, Bdot_signal1, Bdot_signal2, Bdot_signal3, Bdot_signal4);   
	Rigol(Bdot_RIGOL2_handle, RIGOL_Bdot_recordlength,Bdot_Rigol_timestep, Bdot_signal5, Bdot_signal6, Bdot_signal7, Bdot_signal8);   
	
	//Dump results to a file
	sprintf(outfilename, RawDataPath);
	strcat(outfilename, ShotNumberString);
	strcat(outfilename, "_");
	strcat(outfilename, "Bdot33-36.txt");
	
	outfile=fopen (outfilename, "w");
	fprintf(outfile,"t(us), 31, 33, 34, 35, 36\n");
	for (j=0;j<RIGOL_Bdot_recordlength;j++) {
		time_array[j]=(double)(j*Bdot_Rigol_timestep);
		fprintf(outfile,"%f,%f,%f,%f,%f,%f\n",time_array[j],Bdot_signal1[j],Bdot_signal2[j],Bdot_signal3[j],Bdot_signal4[j],
			Bdot_signal5[j]);
	}

 	//Close Bdot data file
	fclose (outfile);
	
	//Overwrite channel 31 since that's the one that is dead on the Joerger
	GetCtrlVal (Bdot_panel, Bdot_Chan31Name,ChannelNameString);
	Write_to_PHASMA_MDS(time_array,Bdot_signal1,ChannelNameString);
		
	//Get channel name string, store raw data on drive folder, and push data to MDSPlus for Channel 33
	GetCtrlVal (Bdot_panel, Bdot_Chan33Name,ChannelNameString);
	Write_to_PHASMA_MDS(time_array,Bdot_signal2,ChannelNameString);

	//Get channel name string, store raw data on drive folder, and push data to MDSPlus for Channel 33
	GetCtrlVal (Bdot_panel, Bdot_Chan34Name,ChannelNameString);
	Write_to_PHASMA_MDS(time_array,Bdot_signal3,ChannelNameString);
	
	//Get channel name string, store raw data on drive folder, and push data to MDSPlus for Channel 33
	GetCtrlVal (Bdot_panel, Bdot_Chan35Name,ChannelNameString);
	Write_to_PHASMA_MDS(time_array,Bdot_signal4,ChannelNameString);
	
	//Get channel name string, store raw data on drive folder, and push data to MDSPlus for Channel 33
	GetCtrlVal (Bdot_panel, Bdot_Chan36Name,ChannelNameString);
	Write_to_PHASMA_MDS(time_array,Bdot_signal5,ChannelNameString);
	*/
	//Close interface panel
   	DiscardPanel(Bdot_panel);  
}

//This function advances the Bdot probe a single step per the spacing requested in the interface
float Advance_Bdot_Probe(void)
{
	int	 	Probe_Return_Flag;
	int 	Probe_Active_Move_Flag;
	float 	Bdot_RStart;
	float 	Bdot_RStop;
	float 	Bdot_RIncrement;
	float 	Bdot_Offset;
	float 	Bdot_Return;
	float 	Current_Position;
	float 	Probe_Move;
	float  	Bdot_Max_Insertion;
	float 	Bdot_COUNTSPERMM;
	char 	Bdot_Probe_IP_Address[64];
	
	//Open settings panel and load saved values
	Bdot_panel = LoadPanel (0, "BdotSettings.uir", Bdot);
	RecallPanelState (Bdot_panel, "Master_Control_Storage_File", Bdot_setup_state);
	
	//Get current position of probe, starting position, end position, and what to do at end of scan range
	GetCtrlVal (Bdot_panel, Bdot_R_Start,&Bdot_RStart);
	GetCtrlVal (Bdot_panel, Bdot_R_Stop,&Bdot_RStop);
	GetCtrlVal (Bdot_panel, Bdot_R_Increment,&Bdot_RIncrement);
	GetCtrlVal (Bdot_panel, Bdot_Probe_Offset,&Bdot_Offset);
	GetCtrlVal (Bdot_panel, Bdot_Probe_Return,&Bdot_Return);
	GetCtrlVal (Bdot_panel, Bdot_R_Position,&Current_Position);
	GetCtrlVal (Bdot_panel, Bdot_Probe_Return,&Probe_Return_Flag);
	GetCtrlVal (Bdot_panel, Bdot_Probe_Active_Move,&Probe_Active_Move_Flag);
	GetCtrlVal (Bdot_panel, Bdot_Probe_IP,Bdot_Probe_IP_Address);
	GetCtrlVal (Bdot_panel, Bdot_Probe_Max_Insertion,&Bdot_Max_Insertion);
	GetCtrlVal (Bdot_panel, Bdot_Probe_COUNTSPERMM,&Bdot_COUNTSPERMM);

	if (Probe_Active_Move_Flag) {
		
		//Get current position from motor controller
	
		//Calculate distance to move to next position
		Probe_Move=Current_Position+Bdot_RIncrement;
		
		//If next position is beyond the stop position, either do nothing or send 
		//probe back to start position depending on settings in interface window
		if ( (Bdot_RIncrement + Current_Position) > Bdot_RStop) {
			return Current_Position;
		} else {
			//Command motor to move to next position	
			move_probe(Bdot_Probe_IP_Address, (Bdot_Offset-Probe_Move)*10.0, Bdot_COUNTSPERMM, Bdot_Max_Insertion);
		}

		
		//Update window with current position and return current position to master code
		SetCtrlVal (Bdot_panel, Bdot_R_Position,(Bdot_RIncrement + Current_Position));
		return Bdot_RIncrement + Current_Position;
	} else {
		return Current_Position;
	}

	SavePanelState (Bdot_panel, "Master_Control_Storage_File", Bdot_setup_state);

}





//Launch this routine as window opens and it updates the window with the latest values from the 
//control system as it loads.
void OpenBdot_Settings (void)  
{

	//Open settings panel and load saved values
	Bdot_panel = LoadPanel (0, "BdotSettings.uir", Bdot);
	RecallPanelState (Bdot_panel, "Master_Control_Storage_File", Bdot_setup_state);
	DisplayPanel(Bdot_panel);
	
	//Update panel with 

	
	// Start interacting with user
    RunUserInterface ();

}

int CVICALLBACK Close_Bdot_Settings (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			//Save current state of panel and close interface
			SavePanelState (Bdot_panel, "Master_Control_Storage_File", Bdot_setup_state);
			
			//Set up Joerger digitizers
			BdotActivate();
			DiscardPanel(panel);
			break;
	}
	return 0;
}




int CVICALLBACK Bdot_Move_to_Start (int panel, int control, int event,
							   void *callbackData, int eventData1, int eventData2)
{

	float 	Bdot_RStart;
	float 	Current_Position;
	float  	Bdot_Max_Insertion;
	float 	Bdot_COUNTSPERMM;
	float 	Bdot_Offset;
	char 	Bdot_Probe_IP_Address[64];

	switch (event)
	{
		case EVENT_COMMIT:
			//Get current position of probe, starting position, end position, and what to do at end of scan range
			GetCtrlVal (Bdot_panel, Bdot_R_Start,&Bdot_RStart);
			GetCtrlVal (Bdot_panel, Bdot_Probe_IP,Bdot_Probe_IP_Address);
			GetCtrlVal (Bdot_panel, Bdot_Probe_Max_Insertion,&Bdot_Max_Insertion);
			GetCtrlVal (Bdot_panel, Bdot_Probe_COUNTSPERMM,&Bdot_COUNTSPERMM);
			GetCtrlVal (Bdot_panel, Bdot_Probe_Offset,&Bdot_Offset);
			
			//Send move command to probe motor
			move_probe(Bdot_Probe_IP_Address, (Bdot_Offset-Bdot_RStart)*10.0, Bdot_COUNTSPERMM, Bdot_Max_Insertion);
			
			//Set current position to Start
			SetCtrlVal (Bdot_panel, Bdot_R_Position,Bdot_RStart);


			break;
	}
	return 0;
}


int CVICALLBACK Bdot_Move_to_Stop (int panel, int control, int event,
							   void *callbackData, int eventData1, int eventData2)
{
	float 	Bdot_RStop;
	float 	Current_Position;
	float  	Bdot_Max_Insertion;
	float 	Bdot_COUNTSPERMM;
	float 	Bdot_Offset;
	char 	Bdot_Probe_IP_Address[64];

	switch (event)
	{
		case EVENT_COMMIT:
			//Get current position of probe, starting position, end position, and what to do at end of scan range
			GetCtrlVal (Bdot_panel, Bdot_R_Stop,&Bdot_RStop);
			GetCtrlVal (Bdot_panel, Bdot_Probe_IP,Bdot_Probe_IP_Address);
			GetCtrlVal (Bdot_panel, Bdot_Probe_Max_Insertion,&Bdot_Max_Insertion);
			GetCtrlVal (Bdot_panel, Bdot_Probe_COUNTSPERMM,&Bdot_COUNTSPERMM);
			GetCtrlVal (Bdot_panel, Bdot_Probe_Offset,&Bdot_Offset);
			
			//Send move command to probe motor
			move_probe(Bdot_Probe_IP_Address, (Bdot_Offset-Bdot_RStop)*10.0, Bdot_COUNTSPERMM, Bdot_Max_Insertion);
			
			//Set current position to Start
			SetCtrlVal (Bdot_panel, Bdot_R_Position,Bdot_RStop);


			break;
	}
	return 0;
}

int CVICALLBACK Bdot_Home (int panel, int control, int event,
							   void *callbackData, int eventData1, int eventData2)
{

	char 	Bdot_Probe_IP_Address[64];
	float 	Bdot_Offset;

	switch (event)
	{
		case EVENT_COMMIT:
			//Get current position of probe, starting position, end position, and what to do at end of scan range
			GetCtrlVal (Bdot_panel, Bdot_Probe_IP,Bdot_Probe_IP_Address);
			GetCtrlVal (Bdot_panel, Bdot_Probe_Offset,&Bdot_Offset);
			
			//Send move command to probe motor
			home_probe(Bdot_Probe_IP_Address);
			
			//Set current position to Offset
			SetCtrlVal (Bdot_panel, Bdot_R_Position,Bdot_Offset);


			break;
	}
	return 0;
}


