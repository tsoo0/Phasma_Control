#include 	<utility.h>
#include 	<visa.h>
#include 	"HousekeepingSystems.h"
#include 	"HousekeepingSettings.h"  
#include 	<tkvx4244.h>
#include 	<ansi_c.h>
#include 	<formatio.h>
#include	"PHASMA-MDS-Routines.h"
#include 	"GlobalVariables.h"   
#include	<stdlib.h>  
#include	"Rigol.h"

#define		max_record_length 262144  

ViSession  	sig_cond_hndl;
ViSession 	digitizer_hndl;


int			bit_mask; 
int			Housekeeping_panel;
int			Housekeeping_setup_state = 3;
int			Housekeeping_RecordLength=8192;
int 		RIGOL_RecordLength;
ViInt16		Housekeeping_VoltageRange=10;

double  	Houskeeping_Signal0[max_record_length];
double  	Houskeeping_Signal1[max_record_length];
double  	Houskeeping_Signal2[max_record_length];
double  	Houskeeping_Signal3[max_record_length];
double  	Houskeeping_Signal4[max_record_length];
double  	Houskeeping_Signal5[max_record_length];
double  	Houskeeping_Signal6[max_record_length];
double  	Houskeeping_Signal7[max_record_length];
double  	Houskeeping_Signal9[max_record_length];
double  	Houskeeping_Signal10[max_record_length];
double  	Houskeeping_Signal11[max_record_length];
double  	Houskeeping_Signal12[max_record_length];
double  	time_array[max_record_length];
double		timebase[max_record_length];

double		RIGOL_Housekeeping_clockspeed;
double		SampleRate=2000000;
ViSession 	Housekeeping_RIGOL1_handle;  
ViInt16		LogicalAddress=56;			//	It should be at 56

char	RIGOL1_Housekeep_IP_Address_string[64];


//Set up the VXI 4244 digitizer
void HousekeepingActivate(void)
{
	int 	result=0; 					//	Error-checking variable
	int		count;
	int		RIGOL_Housekeeping_recordlength;
	float	RIGOL_Housekeeping_TimeWindow;
	float 	RIGOL_VoltageRange;
	char	LogicalAddressString[36];
	char	dummy[18];
	char 	idn[256];
	char	RIGOL_Housekeeping_string[32];	
	void 	displayErr(ViSession InstrHndl,ViStatus err); /* For Displaying Error Messages */

		//Open up Housekeepings settings panel and get target digitizer rate and other settings
		Housekeeping_panel = LoadPanel (0, "HousekeepingSystems.uir", Housekeep);
		RecallPanelState (Housekeeping_panel, "Master_Control_Storage_File", Housekeeping_setup_state);
		
		//Get logical address of VX4244 and other information
		GetCtrlVal (Housekeeping_panel, Housekeep_VX4244Address,LogicalAddressString);
		
		//Strip out address from string
		sscanf(LogicalAddressString,"%16s %hd",dummy, &LogicalAddress);
 
		//Get sample rate and voltage range
		GetCtrlVal (Housekeeping_panel, Housekeep_SampleRate,&SampleRate);
		GetCtrlVal (Housekeeping_panel, Housekeep_RecordLength,&Housekeeping_RecordLength);
		GetCtrlVal (Housekeeping_panel, Housekeep_VoltageRange,&Housekeeping_VoltageRange);
		
		// 	Find VX4244 Digitizer, Get Its ID, and Reset It
		if ((result=tkvx4244_autoConnectToLA (&digitizer_hndl, LogicalAddress))<0){
        	displayErr(digitizer_hndl,result);
    	}

	
		//	Setup Digitizer, turning on the first 8 channels as they are the only ones in use for housekeeping data
		setup_4244 (digitizer_hndl, Housekeeping_RecordLength, SampleRate, Housekeeping_RecordLength, SampleRate,
			Housekeeping_RecordLength, SampleRate, Housekeeping_RecordLength, SampleRate, Housekeeping_VoltageRange,
			Housekeeping_VoltageRange, Housekeeping_VoltageRange, Housekeeping_VoltageRange, 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1);		 
	 		
		//First two groups of digitizer are in use. A Bit Mask of 15 Means the First 4 bits (measurements 1, 2, 3 & 4) are Complete
		bit_mask=3;  
		
			//Get sample rate, record length, and channel scales
		GetCtrlVal (Housekeeping_panel, Housekeep_RIGOL_clockspeed, &RIGOL_Housekeeping_clockspeed);
		GetCtrlVal (Housekeeping_panel, Housekeep_RIGOL_RecordLength, &RIGOL_Housekeeping_recordlength);
		GetCtrlVal (Housekeeping_panel, Housekeep_RIGOL_VoltageRange, &RIGOL_VoltageRange);
	
		//Calculate rounded off time window to sue for setting Rigol scope
		RIGOL_Housekeeping_TimeWindow=(float)((RIGOL_Housekeeping_recordlength/RIGOL_Housekeeping_clockspeed/1000.0));

		//Set up RIGOL scope used to acquire high resolution housekeeping data
		//Get Rigol ip address from interface panel and open connection to the two scopes
		GetCtrlVal (Housekeeping_panel, Housekeep_RIGOL1Address, RIGOL1_Housekeep_IP_Address_string);
	 	result=viOpen (Global_Visa_Session_Handle, RIGOL1_Housekeep_IP_Address_string, VI_NULL, VI_NULL, &Housekeeping_RIGOL1_handle);

		if (!result) {
			//Make sure scope is running so commands will work
			sprintf (RIGOL_Housekeeping_string, ":RUN\r");   																							//Send the run command
			result = viWrite (Housekeeping_RIGOL1_handle, RIGOL_Housekeeping_string, (unsigned int)strlen(RIGOL_Housekeeping_string), &count);

			//Setup the rate, record length, trigger system, and voltage range for the 1st Rigol scope
			sprintf (RIGOL_Housekeeping_string, ":TIM:MAIN:SCAL %f\r",(RIGOL_Housekeeping_TimeWindow/10.0));  
			result = viWrite (Housekeeping_RIGOL1_handle, RIGOL_Housekeeping_string, (unsigned int)strlen(RIGOL_Housekeeping_string), &count);	
			sprintf (RIGOL_Housekeeping_string, ":ACQ:MDEP %d\r",RIGOL_Housekeeping_recordlength);   													//Set the record length by setting the Memory Depth and upload the record length in k
			result = viWrite (Housekeeping_RIGOL1_handle, RIGOL_Housekeeping_string, (unsigned int)strlen(RIGOL_Housekeeping_string), &count);	
			sprintf (RIGOL_Housekeeping_string, ":CHAN1:SCAL  %f\r", RIGOL_VoltageRange);   															//Set the voltage scale of the channel to 10 V to match Joerger
			result = viWrite (Housekeeping_RIGOL1_handle, RIGOL_Housekeeping_string, (unsigned int)strlen(RIGOL_Housekeeping_string), &count);
			sprintf (RIGOL_Housekeeping_string, ":CHAN2:SCAL  %f\r", RIGOL_VoltageRange);   															//Set the voltage scale of the channel to 10 V to match Joerger
			result = viWrite (Housekeeping_RIGOL1_handle, RIGOL_Housekeeping_string, (unsigned int)strlen(RIGOL_Housekeeping_string), &count);
			sprintf (RIGOL_Housekeeping_string, ":CHAN3:SCAL  %f\r", RIGOL_VoltageRange);  																//Set the voltage scale of the channel to 10 V to match Joerger
			result = viWrite (Housekeeping_RIGOL1_handle, RIGOL_Housekeeping_string, (unsigned int)strlen(RIGOL_Housekeeping_string), &count);
			sprintf (RIGOL_Housekeeping_string, ":CHAN4:SCAL  %f\r", RIGOL_VoltageRange);   															//Set the voltage scale of the channel to 10 V to match Joerger
			result = viWrite (Housekeeping_RIGOL1_handle, RIGOL_Housekeeping_string, (unsigned int)strlen(RIGOL_Housekeeping_string), &count);
		 	sprintf (RIGOL_Housekeeping_string, ":TRIG:MODE EDGE\r");   
			result = viWrite (Housekeeping_RIGOL1_handle, RIGOL_Housekeeping_string, (unsigned int)strlen(RIGOL_Housekeeping_string), &count);
			sprintf (RIGOL_Housekeeping_string, ":TRIG:COUP AC\r");   
			result = viWrite (Housekeeping_RIGOL1_handle, RIGOL_Housekeeping_string, (unsigned int)strlen(RIGOL_Housekeeping_string), &count);
			
			sprintf (RIGOL_Housekeeping_string, ":TRIG:EDGE:SOUR D0\r");   																				//Set the trigger source to the Digital 0 trigger since this is hardwired for this scope
			result = viWrite (Housekeeping_RIGOL1_handle, RIGOL_Housekeeping_string, (unsigned int)strlen(RIGOL_Housekeeping_string), &count);
		
		 	sprintf (RIGOL_Housekeeping_string, ":TRIG:EDGE:SLOP POS\r");   
			result = viWrite (Housekeeping_RIGOL1_handle, RIGOL_Housekeeping_string, (unsigned int)strlen(RIGOL_Housekeeping_string), &count);

			sprintf (RIGOL_Housekeeping_string, ":TIM:OFFS 0%f\r", (RIGOL_Housekeeping_TimeWindow));   																					//No offset of the trigger position
			result = viWrite (Housekeeping_RIGOL1_handle, RIGOL_Housekeeping_string, (unsigned int)strlen(RIGOL_Housekeeping_string), &count);

			Delay(0.1);																																	//Delay long enough for menu screen clearing to work
			
			sprintf (RIGOL_Housekeeping_string, ":SYST:KEY:PRES MOFF\r");   												 							//Hide menu on scope screen
		 	result = viWrite (Housekeeping_RIGOL1_handle, RIGOL_Housekeeping_string, (unsigned int)strlen(RIGOL_Housekeeping_string), &count);
			
			viClose(Housekeeping_RIGOL1_handle);
		}
		//Discard housekeeping panel
		DiscardPanel(Housekeeping_panel);
		
}


//Arm the VX4244 digitizer and Rigol scope
int HousekeepingArm(void)
{
	int		result;
	int		count;
	char	RIGOL_Housekeeping_string[32];
	
	//	Arm Digitizer with provision for error and only arms those groups in use
	if ((result = tkvx4244_initMeasurementCycle (digitizer_hndl, 0, tkvx4244_GROUP_ONE_ENABLE, tkvx4244_GROUP_TWO_ENABLE, tkvx4244_GROUP_THREE_ENABLE, tkvx4244_GROUP_FOUR_ENABLE, tkvx4244_ALL_GROUPS_ENABLE, tkvx4244_INIT_IMM)))
			  				displayErr(digitizer_hndl,result);

	//Arm the Rigol scope
	result=viOpen (Global_Visa_Session_Handle, RIGOL1_Housekeep_IP_Address_string, VI_NULL, VI_NULL, &Housekeeping_RIGOL1_handle);
	if (!result) {
		sprintf (RIGOL_Housekeeping_string,":TRIG:SWE SING\r");   												 		//Single Trigger Sweep
 		result = viWrite (Housekeeping_RIGOL1_handle, RIGOL_Housekeeping_string, (unsigned int)strlen(RIGOL_Housekeeping_string), &count);
		viClose(Housekeeping_RIGOL1_handle);
	}

	return !result;
}


//Routine to acquire data from the VX4244 digitizer
void acquire_data(ViSession handle, int channel,double voltage_scale, int record_length,
				double output_array[])
{

	void 		displayErr(ViSession InstrHndl,ViStatus err); /* For Displaying Error Messages */
	ViStatus   	result;            		/* Function return status */
	ViChar 		binary_buffer[20000];	/* Large Array for Character Buffer of binary points */
	double		sampleValues[10000];	/* Large array for reading from binary buffer*/
	int			source_channel=tkvx4244_ANALYZE_CH1;
	int 		start_address;
	int			acquire_points;
	int			j;

	switch (channel)
		{
                 
	    case 1:
	       	source_channel=tkvx4244_ANALYZE_CH1;
	       	break;
                    
	   	case 2:
	    	source_channel=tkvx4244_ANALYZE_CH2;
	     	break;
                    		 
	   	case 3:
	      	source_channel=tkvx4244_ANALYZE_CH3;
	        break;
                    
	   	case 4:
	      	source_channel=tkvx4244_ANALYZE_CH4;
	        break;
                    
	   	case 5:
	      	source_channel=tkvx4244_ANALYZE_CH5;
	        break;
                    
	   	case 6:
	      	source_channel=tkvx4244_ANALYZE_CH6;
	        break;
                    
	   	case 7:
	      	source_channel=tkvx4244_ANALYZE_CH7;
	        break;
                    
	   	case 8:
	      	source_channel=tkvx4244_ANALYZE_CH8;
	        break;
                    
	   	case 9:
	      	source_channel=tkvx4244_ANALYZE_CH9;
	        break;
                    
	   	case 10:
	      	source_channel=tkvx4244_ANALYZE_CH10;
	        break;
                    
	   	case 11:
	      	source_channel=tkvx4244_ANALYZE_CH11;
	        break;
                    
	   	case 12:
	      	source_channel=tkvx4244_ANALYZE_CH12;
	        break;
                    
	   	case 13:
	      	source_channel=tkvx4244_ANALYZE_CH13;
	        break;
                    
	   	case 14:
	      	source_channel=tkvx4244_ANALYZE_CH14;
	        break;
                    
	   	case 15:
	      	source_channel=tkvx4244_ANALYZE_CH15;
	        break;
                    
	   	case 16:
	      	source_channel=tkvx4244_ANALYZE_CH16;
	        break;
	}
						
	start_address=0;
	acquire_points = record_length-5000;
	if (acquire_points < 0) {
		acquire_points = record_length;
		}
	else {
		acquire_points = 5000;
	}
	
//	Start loop to acquire data record	
	do {
		binary_buffer[0]='\0';
	
//		Read Binary data from Appropriate Channel into Character Buffer in 5000 point or less increments
		if ((result = tkvx4244_readBinaryData (handle, (ViInt16)acquire_points, start_address,
				(ViInt16)source_channel, binary_buffer))){
	        displayErr(handle,result);
	        break;
	    }
    						
//		Move Binary  Data from Buffer into Array (take care of first of two bytes)		
		Scan(binary_buffer,"%*i[b2o10]>%*f[]",(int)(2*acquire_points),(int) acquire_points,sampleValues);

//		Move Data into Correct Array and Scale Data						
	  	for (j = 0; j<acquire_points; j++) {
	    	output_array[start_address+j]=(float)sampleValues[j]*(voltage_scale/32768.);
	    }	

		start_address=start_address+5000;
		acquire_points = record_length-start_address-5000;
		if (acquire_points > 0) {
			acquire_points = 5000;
		}
		else {
			acquire_points = record_length-start_address;
		}

    						
	} while (start_address < record_length); /* end of do-while loop for acquisition up to record length points*/

} 


//Acquire data from the VXI 4244 device in 2 groups of 4 and save all the data to global variables in this subsystem
void Plasma_Gun_housekeeping_acquire(void)	

{
	int i;
	int result;
	double	vltrange= 10;

	
	//Plasma Gun Housekeeping data from first 8 channels of digitizer.
	switch (Housekeeping_VoltageRange) {
	    case 0: vltrange = 0.5;
	    	  		  break;
	    case 1: vltrange = 1;
	    	  		  break;
	    case 2: vltrange = 2;
	    	  		  break;
	    case 5: vltrange = 5;
	    	  		  break;
	    case 10:vltrange = 10;
	    	  		  break;
	    case 20:vltrange = 20;
	    	  		  break;
	    default: printf("Invalid voltage range selected for Group 1! \n");
	}	  
	
	//Transfer four core housekeeping measurements from the digitzer to the arrays
	acquire_data(digitizer_hndl,1,(double)(vltrange), Housekeeping_RecordLength,Houskeeping_Signal0);
	acquire_data(digitizer_hndl,2,(double)(vltrange), Housekeeping_RecordLength,Houskeeping_Signal1);	
	acquire_data(digitizer_hndl,3,(double)(vltrange), Housekeeping_RecordLength,Houskeeping_Signal2);
	acquire_data(digitizer_hndl,4,(double)(vltrange), Housekeeping_RecordLength,Houskeeping_Signal3);	
	
	//Create group1 time_array 
	for (i=0;i<Housekeeping_RecordLength;i++) { 
		time_array[i]=(double)i/group1_drate;
	}
	
	switch (Housekeeping_VoltageRange) {
	    case 0: vltrange = 0.5;
	    	  		  break;
	    case 1: vltrange = 1;
	    	  		  break;
	    case 2: vltrange = 2;
	    	  		  break;
	    case 5: vltrange = 5;
	    	  		  break;
	    case 10:vltrange = 10;
	    	  		  break;
	    case 20:vltrange = 20;
	    	  		  break;
	    default: printf("Invalid voltage range selected for Group 2! \n");
	}	  
	
	//Transfer four core housekeeping measurements from the digitzer to the arrays
	acquire_data(digitizer_hndl,5,(double)(vltrange),Housekeeping_RecordLength,Houskeeping_Signal4);	
	acquire_data(digitizer_hndl,6,(double)(vltrange),Housekeeping_RecordLength,Houskeeping_Signal5);	
	acquire_data(digitizer_hndl,7,(double)(vltrange),Housekeeping_RecordLength,Houskeeping_Signal6);	
	acquire_data(digitizer_hndl,8,(double)(vltrange),Housekeeping_RecordLength,Houskeeping_Signal7);	
	
}	


void displayErr(ViSession gInstrHndl,ViStatus err)
{
	char szErrMsg[256];

    tkvx4244_error_message (gInstrHndl, err, szErrMsg);
    printf("%s\n", szErrMsg);
}  



//Take the acquired data from the instrument and print it to the common data folder and 
//also push it to the MDS database
void Write_HousekeepingData(void)
{
	char	ChannelNameString[64];
	char	outfilename[64];
	FILE*	outfile;
	int		j;
	int  	result;
	float 	RIGOL_timestep=0.0;
	
	//Acquire the housekeeping data from the digitizer using temporary arrays that are discarded every shot. 
	//Ideally this process would run in parallel with other data transfer ones....?
	Plasma_Gun_housekeeping_acquire();
	
	//Dump results to a file
	sprintf(outfilename, RawDataPath);
	strcat(outfilename, ShotNumberString);
	strcat(outfilename, "_");
	strcat(outfilename, "Housekeeping.txt");
	
	outfile=fopen (outfilename, "w");
	fprintf(outfile,"t, Arc1_I, Arc1_V, Bias1_I, XX, Arc2_I, Arc2_V, Bias2_I, XX\n");
	for (j=0;j<Housekeeping_RecordLength;j++) {
		timebase[j]=(double)(j/SampleRate);
		fprintf(outfile,"%f,%f, %f,%f,%f,%f,%f,%f,%f\n",timebase[j],Houskeeping_Signal0[j],Houskeeping_Signal1[j],Houskeeping_Signal2[j],
			Houskeeping_Signal3[j],Houskeeping_Signal4[j],Houskeeping_Signal5[j],Houskeeping_Signal6[j],Houskeeping_Signal7[j]);
	}

 	//Close Housekeeping data file
	fclose (outfile);
	
	//Open up Housekeepings settings panel and get filenames for each measurement
	Housekeeping_panel = LoadPanel (0, "HousekeepingSystems.uir", Housekeep);
	RecallPanelState (Housekeeping_panel, "Master_Control_Storage_File", Housekeeping_setup_state);
		
	//Get channel name string, store raw data on drive folder, and push data to MDSPlus for Channel 0
	GetCtrlVal (Housekeeping_panel, Housekeep_Chan0Name,ChannelNameString);
	Write_to_PHASMA_MDS(timebase,Houskeeping_Signal0,ChannelNameString);

	//Get channel name string, store raw data on drive folder, and push data to MDSPlus for Channel 1
	GetCtrlVal (Housekeeping_panel, Housekeep_Chan1Name,ChannelNameString);
	Write_to_PHASMA_MDS(timebase,Houskeeping_Signal1,ChannelNameString);

	//Get channel name string, store raw data on drive folder, and push data to MDSPlus for Channel 2
	GetCtrlVal (Housekeeping_panel, Housekeep_Chan2Name,ChannelNameString);
	Write_to_PHASMA_MDS(timebase,Houskeeping_Signal2,ChannelNameString);
	
	//Get channel name string, store raw data on drive folder, and push data to MDSPlus for Channel 3
	GetCtrlVal (Housekeeping_panel, Housekeep_Chan3Name,ChannelNameString);
	Write_to_PHASMA_MDS(timebase,Houskeeping_Signal3,ChannelNameString);
		
	//Get channel name string, store raw data on drive folder, and push data to MDSPlus for Channel 4
	GetCtrlVal (Housekeeping_panel, Housekeep_Chan4Name,ChannelNameString);
	Write_to_PHASMA_MDS(timebase,Houskeeping_Signal4,ChannelNameString);
	
	//Get channel name string, store raw data on drive folder, and push data to MDSPlus for Channel 5
	GetCtrlVal (Housekeeping_panel, Housekeep_Chan5Name,ChannelNameString);
	Write_to_PHASMA_MDS(timebase,Houskeeping_Signal5,ChannelNameString);

	//Get channel name string, store raw data on drive folder, and push data to MDSPlus for Channel 6
	GetCtrlVal (Housekeeping_panel, Housekeep_Chan6Name,ChannelNameString);
	Write_to_PHASMA_MDS(timebase,Houskeeping_Signal6,ChannelNameString);
	
	//Get channel name string, store raw data on drive folder, and push data to MDSPlus for Channel 7
	GetCtrlVal (Housekeeping_panel, Housekeep_Chan7Name,ChannelNameString);
	Write_to_PHASMA_MDS(timebase,Houskeeping_Signal7,ChannelNameString);

	//Retrieve data from the RIGOL oscilloscope and store the data
	//Get RIGOL Record Length
	GetCtrlVal (Housekeeping_panel, Housekeep_RIGOL_RecordLength,&RIGOL_RecordLength);

	//Grab data from all four channels of the oscilloscope
	result=viOpen (Global_Visa_Session_Handle, RIGOL1_Housekeep_IP_Address_string, VI_NULL, VI_NULL, &Housekeeping_RIGOL1_handle);
	if (!result) {
		Rigol(Housekeeping_RIGOL1_handle, RIGOL_RecordLength, &RIGOL_timestep, Houskeeping_Signal9, Houskeeping_Signal10, Houskeeping_Signal11, Houskeeping_Signal12);   
		viClose(Housekeeping_RIGOL1_handle);
	}
	
	//Dump results to a file
	sprintf(outfilename, RawDataPath);
	strcat(outfilename, ShotNumberString);
	strcat(outfilename, "_");
	strcat(outfilename, "Housekeeping_HighRes.txt");
	
	outfile=fopen (outfilename, "w");
	fprintf(outfile,"t, Bias1_I, Bias2_I, TSperp, TSpara\n");
	for (j=0;j<RIGOL_RecordLength;j++) {
		timebase[j]=(double)(j*RIGOL_timestep);
		fprintf(outfile,"%f,%f,%f,%f,%f\n",timebase[j],Houskeeping_Signal9[j],Houskeeping_Signal10[j],Houskeeping_Signal11[j],Houskeeping_Signal12[j]);
	}

 	//Close high res housekeeping data file
	fclose (outfile);
	
	//Get channel name string, store raw data on drive folder, and push data to MDSPlus for Channel 9
	GetCtrlVal (Housekeeping_panel, Housekeep_Chan9Name,ChannelNameString);
	Write_to_PHASMA_MDS(timebase,Houskeeping_Signal9,ChannelNameString);
		
	//Get channel name string, store raw data on drive folder, and push data to MDSPlus for Channel 10
	GetCtrlVal (Housekeeping_panel, Housekeep_Chan10Name,ChannelNameString);
	Write_to_PHASMA_MDS(timebase,Houskeeping_Signal10,ChannelNameString);

	//Get channel name string, store raw data on drive folder, and push data to MDSPlus for Channel 11
	GetCtrlVal (Housekeeping_panel, Housekeep_Chan11Name,ChannelNameString);
	Write_to_PHASMA_MDS(timebase,Houskeeping_Signal11,ChannelNameString);
	
	//Get channel name string, store raw data on drive folder, and push data to MDSPlus for Channel 12
	GetCtrlVal (Housekeeping_panel, Housekeep_Chan12Name,ChannelNameString);
	Write_to_PHASMA_MDS(timebase,Houskeeping_Signal12,ChannelNameString);
	
}



//Launch this routine as window opens and it updates the window with the latest values from the 
//control system as it loads.
void OpenHousekeeping_Settings (void)  
{
	//Open settings panel and load saved values
	Housekeeping_panel = LoadPanel (0, "HousekeepingSystems.uir", Housekeep);
	RecallPanelState (Housekeeping_panel, "Master_Control_Storage_File", Housekeeping_setup_state);
	DisplayPanel(Housekeeping_panel);
	
	//Update panel with 

	
	// Start interacting with user
    RunUserInterface ();

}

int CVICALLBACK Close_Housekeeping_Settings (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			//Save current state of panel and close interface
			SavePanelState (Housekeeping_panel, "Master_Control_Storage_File", Housekeeping_setup_state);
			DiscardPanel(Housekeeping_panel);
			QuitUserInterface (0);
			break;
	}
	return 0;
}


int TransferHousekeepingData (double transfer_array1[], double transfer_array2[], double transfer_array3[])  
{
int	j;

	//Update panel with 
	for (j=0;j<RIGOL_RecordLength;j++) {
		transfer_array1[j]=timebase[j]*1.0E6;			//put time in microseconds
		transfer_array2[j]=Houskeeping_Signal9[j];
		transfer_array3[j]=Houskeeping_Signal10[j];
	}

	return RIGOL_RecordLength;
}
