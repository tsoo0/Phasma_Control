#include	<stdlib.h>  
#include 	<analysis.h>
#include 	<utility.h>
#include 	<ansi_c.h>
#include 	<formatio.h>
#include	<visa.h>
#include	"PHASMA-MDS-Routines.h"
#include	"GlobalVariables.h"
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
#define  	VTR10014_4_Base_address 	0xCC00  		//Joerger 4 byte base address of EE00 in hexadecimal
#define	 	VTR10014_1_A32_address 		0x04			//Joerger A32 memory address (2 byte max) in hexadecimal for bits 24 to 32 in the 16 bit register, must match the full address and be mutltiple of 4
#define	 	VTR10014_2_A32_address 		0x08			//Joerger A32 memory address (2 byte max) in hexadecimal for bits 24 to 32 in the 16 bit register, must match the full address and be mutltiple of 4 
#define	 	VTR10014_3_A32_address 		0x0C			//Joerger A32 memory address (2 byte max) in hexadecimal for bits 24 to 32 in the 16 bit register, must match the full address and be mutltiple of 4 
#define	 	VTR10014_4_A32_address 		0x0E			//Joerger A32 memory address (2 byte max) in hexadecimal for bits 24 to 32 in the 16 bit register, must match the full address and be mutltiple of 4 
#define	 	VTR10014_1_A32_addressFull 	0x04000000		//Joerger A32 memory address set in the VME device parameters
#define	 	VTR10014_2_A32_addressFull 	0x08000000		//Joerger A32 memory address set in the VME device parameters 
#define	 	VTR10014_3_A32_addressFull 	0x0C000000		//Joerger A32 memory address set in the VME device parameters 
#define	 	VTR10014_4_A32_addressFull 	0x0E000000		//Joerger A32 memory address set in the VME device parameters 

		
int			Bdot_panel;
int			Bdot_setup_state = 6;
int 		JoergerVTR10012_clockspeed;
int 		JoergerVTR10014_clockspeed;
long 		JoergerVTR10012_recordlength; 		// --changed from int to long 5/15/07 by AKH
long 		JoergerVTR10014_recordlength; 		// --changed from int to long 5/15/07 by AKH
double 		JoergerVTR10012_digitation_rate; 
double 		JoergerVTR10014_digitation_rate; 
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
	char	IP_Address_string[64];


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

	
	//Temporarily software trigger all five Joergers so that data transfer can occur
	//VTR10012_SOFTWARE_TRIGGER(VTR10012_handle, 0);
	//VTR10012_SOFTWARE_TRIGGER(VTR10014_1_handle, 0);
	//VTR10012_SOFTWARE_TRIGGER(VTR10014_2_handle, 0);
	//VTR10012_SOFTWARE_TRIGGER(VTR10014_3_handle, 0);
	//VTR10012_SOFTWARE_TRIGGER(VTR10014_4_handle, 0);
	

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
	int 	ch;
	double  Bdot_signal1[max_record_length];
	double  Bdot_signal2[max_record_length];
	double  Bdot_signal3[max_record_length];
	double  Bdot_signal4[max_record_length];
	double  Bdot_signal5[max_record_length];
	double  Bdot_signal6[max_record_length];
	double  Bdot_signal7[max_record_length];
	double  Bdot_signal8[max_record_length];
	double  time_array[max_record_length];	
	
	char fileheader[256];
	
	
	int bdot_channels[] = {
		 Bdot_Chan1Name 	
		 , Bdot_Chan2Name                  	
		 , Bdot_Chan3Name                  	
		 , Bdot_Chan4Name                   	
		 , Bdot_Chan5Name                   	
		 , Bdot_Chan6Name                   	
		 , Bdot_Chan7Name                   	
		 , Bdot_Chan8Name                  	
		 , Bdot_Chan9Name                   	
		 , Bdot_Chan10Name                 	
		 , Bdot_Chan11Name                 	
		 , Bdot_Chan12Name                  	
		 , Bdot_Chan13Name                  	
		 , Bdot_Chan14Name                  	
		 , Bdot_Chan15Name                  	
		 , Bdot_Chan16Name                  	
		 , Bdot_Chan17Name                  	
		 , Bdot_Chan18Name                 	
		 , Bdot_Chan19Name                  	
		 , Bdot_Chan20Name                  	
		 , Bdot_Chan21Name                 	
		 , Bdot_Chan22Name 	
		 , Bdot_Chan23Name 	
		 , Bdot_Chan24Name 	
		 , Bdot_Chan25Name	
		 , Bdot_Chan26Name	
		 , Bdot_Chan27Name	
		 , Bdot_Chan28Name 	
		 , Bdot_Chan29Name	
		 , Bdot_Chan30Name	
		 , Bdot_Chan31Name	
		 , Bdot_Chan32Name 	
		 , Bdot_Chan33Name	
		 , Bdot_Chan34Name	
		 , Bdot_Chan35Name 
		 , Bdot_Chan36Name
                 
		};

	//Wait for Joerger completion indication before trying to transfer data
	JoergerVTR10012_complete (VTR10012_handle, VTR10012_Base_address);
	
	//Open settings panel and load saved values
	Bdot_panel = LoadPanel (0, "BdotSettings.uir", Bdot);
	RecallPanelState (Bdot_panel, "Master_Control_Storage_File", Bdot_setup_state);

	//Retrieve 8 timeseries from the first VTR10012 instrument and write them to raw files 
	JoergerVTR10012_acquire(VTR10012_memory_handle, VTR10012_A32_address, 1,JoergerVTR10012_recordlength,Bdot_signal1,Bdot_signal5);
	JoergerVTR10012_acquire(VTR10012_memory_handle, VTR10012_A32_address, 2,JoergerVTR10012_recordlength,Bdot_signal2,Bdot_signal6);
	JoergerVTR10012_acquire(VTR10012_memory_handle, VTR10012_A32_address, 3,JoergerVTR10012_recordlength,Bdot_signal3,Bdot_signal7);
	JoergerVTR10012_acquire(VTR10012_memory_handle, VTR10012_A32_address, 4,JoergerVTR10012_recordlength,Bdot_signal4,Bdot_signal8);
	
	//Dump results to a file
	sprintf(outfilename, RawDataPath);
	strcat(outfilename, ShotNumberString);
	strcat(outfilename, "_");
	strcat(outfilename, "JOERGER01.txt");
	outfile=fopen (outfilename, "w");
	
	//Write the header for the file
	strcpy(fileheader, "Time");
	for (ch = 0; ch<8; ch++) {
		GetCtrlVal (Bdot_panel, bdot_channels[ch],ChannelNameString);
		strcat(fileheader,",");
		strcat(fileheader,ChannelNameString);
		
	}
	strcat(fileheader,"\n");

	fprintf(outfile,fileheader);
	for (j=0;j<JoergerVTR10012_recordlength;j++) {
		time_array[j]=(double)(1.0E6*j/JoergerVTR10012_digitation_rate);
		fprintf(outfile,"%f,%f,%f,%f,%f,%f,%f,%f,%f\n",time_array[j],Bdot_signal1[j],Bdot_signal2[j],Bdot_signal3[j],Bdot_signal4[j],
			Bdot_signal5[j],Bdot_signal6[j],Bdot_signal7[j],Bdot_signal8[j]);
	}
	
 	//Close Bdot data file
	fclose (outfile);

	
	//Get channel name string, store raw data on drive folder, and push data to MDSPlus for 8 channels
	
	//Retrieve data from the first VTR10014 instrument in pairs and write them to raw files  
 	JoergerVTR10014_acquire(VTR10014_1_memory_handle, VTR10014_1_A32_addressFull, 1,JoergerVTR10014_recordlength,Bdot_signal1,Bdot_signal5);
 	JoergerVTR10014_acquire(VTR10014_1_memory_handle, VTR10014_1_A32_addressFull, 2,JoergerVTR10014_recordlength,Bdot_signal2,Bdot_signal6);
 	JoergerVTR10014_acquire(VTR10014_1_memory_handle, VTR10014_1_A32_addressFull, 3,JoergerVTR10014_recordlength,Bdot_signal3,Bdot_signal7);
 	JoergerVTR10014_acquire(VTR10014_1_memory_handle, VTR10014_1_A32_addressFull, 4,JoergerVTR10014_recordlength,Bdot_signal4,Bdot_signal8);
	
	
	//Dump results to a file
	sprintf(outfilename, RawDataPath);
	strcat(outfilename, ShotNumberString);
	strcat(outfilename, "_");
	strcat(outfilename, "JOERGER02.txt");
	outfile=fopen (outfilename, "w");
	
	strcpy(fileheader, "Time");
	for (ch = 8; ch<16; ch++) {
		GetCtrlVal (Bdot_panel, bdot_channels[ch],ChannelNameString);
		strcat(fileheader,",");
		strcat(fileheader,ChannelNameString);
		
	}
	strcat(fileheader,"\n");
	
	fprintf(outfile,fileheader);
	for (j=0;j<JoergerVTR10014_recordlength;j++) {
		time_array[j]=(double)(1.0E6*j/JoergerVTR10014_digitation_rate);
		fprintf(outfile,"%f,%f,%f,%f,%f,%f,%f,%f,%f\n",time_array[j],Bdot_signal1[j],Bdot_signal2[j],Bdot_signal3[j],Bdot_signal4[j],
			Bdot_signal5[j],Bdot_signal6[j],Bdot_signal7[j],Bdot_signal8[j]);
	}
	
 	//Close Bdot data file
	fclose (outfile);
	
	//Retrieve data from the second VTR10014 instrument in pairs and write them to raw files 
 	JoergerVTR10014_acquire(VTR10014_2_memory_handle, VTR10014_2_A32_addressFull, 1,JoergerVTR10014_recordlength,Bdot_signal1,Bdot_signal5);
 	JoergerVTR10014_acquire(VTR10014_2_memory_handle, VTR10014_2_A32_addressFull, 2,JoergerVTR10014_recordlength,Bdot_signal2,Bdot_signal6);
 	JoergerVTR10014_acquire(VTR10014_2_memory_handle, VTR10014_2_A32_addressFull, 3,JoergerVTR10014_recordlength,Bdot_signal3,Bdot_signal7);
 	JoergerVTR10014_acquire(VTR10014_2_memory_handle, VTR10014_2_A32_addressFull, 4,JoergerVTR10014_recordlength,Bdot_signal4,Bdot_signal8);

	//Dump results to a file
	sprintf(outfilename, RawDataPath);
	strcat(outfilename, ShotNumberString);
	strcat(outfilename, "_");
	strcat(outfilename, "JOERGER03.txt");
	outfile=fopen (outfilename, "w");
	
	//Write the header for the file
	strcpy(fileheader, "Time");
	for (ch = 16; ch<24; ch++) {
		GetCtrlVal (Bdot_panel, bdot_channels[ch],ChannelNameString);
		strcat(fileheader,",");
		strcat(fileheader,ChannelNameString);
		
	}
	strcat(fileheader,"\n");
	
	fprintf(outfile,fileheader);
	for (j=0;j<JoergerVTR10014_recordlength;j++) {
		time_array[j]=(double)(1.0E6*j/JoergerVTR10014_digitation_rate);
		fprintf(outfile,"%f,%f,%f,%f,%f,%f,%f,%f,%f\n",time_array[j],Bdot_signal1[j],Bdot_signal2[j],Bdot_signal3[j],Bdot_signal4[j],
			Bdot_signal5[j],Bdot_signal6[j],Bdot_signal7[j],Bdot_signal8[j]);
	}
	
 	//Close Bdot data file
	fclose (outfile);

	//Retrieve data from the third VTR10014 instrument in pairs and write them to raw files 
 	JoergerVTR10014_acquire(VTR10014_3_memory_handle, VTR10014_3_A32_addressFull, 1,JoergerVTR10014_recordlength,Bdot_signal1,Bdot_signal5);
 	JoergerVTR10014_acquire(VTR10014_3_memory_handle, VTR10014_3_A32_addressFull, 2,JoergerVTR10014_recordlength,Bdot_signal2,Bdot_signal6);
 	JoergerVTR10014_acquire(VTR10014_3_memory_handle, VTR10014_3_A32_addressFull, 3,JoergerVTR10014_recordlength,Bdot_signal3,Bdot_signal7);
 	JoergerVTR10014_acquire(VTR10014_3_memory_handle, VTR10014_3_A32_addressFull, 4,JoergerVTR10014_recordlength,Bdot_signal4,Bdot_signal8);
	
	//Dump results to a file
	sprintf(outfilename, RawDataPath);
	strcat(outfilename, ShotNumberString);
	strcat(outfilename, "_");
	strcat(outfilename, "JOERGER04.txt");
	outfile=fopen (outfilename, "w");
	
	//Write the header for the file
	strcpy(fileheader, "Time");
	for (ch = 24; ch<32; ch++) {
		GetCtrlVal (Bdot_panel, bdot_channels[ch],ChannelNameString);
		strcat(fileheader,",");
		strcat(fileheader,ChannelNameString);
		
	}
	strcat(fileheader,"\n");
	
	fprintf(outfile,fileheader);
	for (j=0;j<JoergerVTR10014_recordlength;j++) {
		time_array[j]=(double)(1.0E6*j/JoergerVTR10014_digitation_rate);
		fprintf(outfile,"%f,%f,%f,%f,%f,%f,%f,%f,%f\n",time_array[j],Bdot_signal1[j],Bdot_signal2[j],Bdot_signal3[j],Bdot_signal4[j],
			Bdot_signal5[j],Bdot_signal6[j],Bdot_signal7[j],Bdot_signal8[j]);
	}
	
 	//Close Bdot data file
	fclose (outfile);
	
	//Retrieve data from the fourth VTR10014 instrument in pairs and write them to raw files. Don't need the last few channels as there are no bdot coils past 36 total
 	JoergerVTR10014_acquire(VTR10014_4_memory_handle, VTR10014_4_A32_addressFull, 1,JoergerVTR10014_recordlength,Bdot_signal1,Bdot_signal5);
 	JoergerVTR10014_acquire(VTR10014_4_memory_handle, VTR10014_4_A32_addressFull, 2,JoergerVTR10014_recordlength,Bdot_signal2,Bdot_signal6);
 	//JoergerVTR10014_acquire(VTR10014_4_memory_handle, VTR10014_4_A32_addressFull, 3,JoergerVTR10014_recordlength,Bdot_signal3,Bdot_signal7);
 	//JoergerVTR10014_acquire(VTR10014_4_memory_handle, VTR10014_4_A32_addressFull, 4,JoergerVTR10014_recordlength,Bdot_signal4,Bdot_signal8);
	
	//Dump results to a file
	sprintf(outfilename, RawDataPath);
	strcat(outfilename, ShotNumberString);
	strcat(outfilename, "_");
	strcat(outfilename, "JOERGER05.txt");
	outfile=fopen (outfilename, "w");
	
	//Write the header for the file
	strcpy(fileheader, "Time");
	for (ch = 32; ch<36; ch++) {
		GetCtrlVal (Bdot_panel, bdot_channels[ch],ChannelNameString);
		strcat(fileheader,",");
		strcat(fileheader,ChannelNameString);
		
	}
	strcat(fileheader,"\n");
	fprintf(outfile,fileheader);
	
	for (j=0;j<JoergerVTR10014_recordlength;j++) {
		time_array[j]=(double)(1.0E6*j/JoergerVTR10014_digitation_rate);
		fprintf(outfile,"%f,%f,%f,%f,%f\n",time_array[j],Bdot_signal1[j],Bdot_signal2[j],Bdot_signal3[j],Bdot_signal4[j]);
	}
	
 	//Close Bdot data file
	fclose (outfile);
	
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


