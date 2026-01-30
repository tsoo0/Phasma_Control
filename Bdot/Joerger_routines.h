void JoergerVTR10012_setup (void);
void JoergerVTR10012_trigger (void);
void JoergerVTR10012_complete (void);
void JoergerVTR10012_display (void);
void JoergerVTR10012_acquire(int,int,double channel1[],double channel2[]);
void JoergerVTR10012_ARM(void);
void JoergerVTR10012_trigger_status (int *);

void JoergerVTR10014_setup (void);
void JoergerVTR10014_trigger (void);
void JoergerVTR10014_complete (void);
void JoergerVTR10014_display (void);
void JoergerVTR10014_acquire(int,int,double channel1[],double channel2[]);
void JoergerVTR10014_ARM(void);
void JoergerVTR10014_trigger_status (int *);

#include <utility.h>
#include <userint.h>


/*

void JoergerVTR10012_ARM()
{
int status;

	//Disarm the digitizer and clear the IRQ source
	VTR10012_DISARM();
	VTR10012_RESET_MEM_COUNTER();
	VTR10012_ARM();
	do {
		Delay (.1); //wait before checking for completion of cycle
		VTR10012_READ_STATUS(&status);
	}while(status != 1);   //	when the status = armed, exit
}			


void JoergerVTR10014_ARM()
{
int status;

	//Disarm the digitizer and clear the IRQ source
	VTR10014_DISARM(0);
	VTR10014_DISARM(1);
	VTR10014_DISARM(2);
	VTR10014_RESET_MEM_COUNTER(0);
	VTR10014_RESET_MEM_COUNTER(1);
	VTR10014_RESET_MEM_COUNTER(2);
	   
	//Arm all three VTR10014 instruments
	VTR10014_ARM(0);
	do {
		Delay (.1); //wait before checking for completion of cycle
		VTR10014_READ_STATUS(0,&status);
	}while(status != 1);   //	when the status = armed, exit
 
	VTR10014_ARM(1);
	do {
		Delay (.1); //wait before checking for completion of cycle
		VTR10014_READ_STATUS(1,&status);
	}while(status != 1);   //	when the status = armed, exit

	VTR10014_ARM(2);
	do {
		Delay (.1); //wait before checking for completion of cycle
		VTR10014_READ_STATUS(2,&status);
	}while(status != 1);   //	when the status = armed, exit
}			

*/



void JoergerVTR10012_complete ()
{
int status;
	do {
		Delay (.1); //wait before checking for completion of cycle
		VTR10012_READ_STATUS(&status);
	}while(status != 4);   //	when the status = recording cycle is done, exit
}


void JoergerVTR10012_trigger_status (int *status)
{
	VTR10012_READ_STATUS(status);
}

void JoergerVTR10012_trigger ()
{
	VTR10012_SOFTWARE_TRIGGER();
}

//This function assumes that the Joerger module is set up to acquire single ended signals from each channel
void JoergerVTR10012_acquire(int channel,int Joerger_rlength, double signal_array1[], double signal_array2[])
{
	int		source_channel;
	int		i;
	double 	*data_array1;
	double	*data_array2;
	
	//Allot space to read data records
	data_array1 = malloc ((Joerger_rlength*8));
	data_array2 = malloc ((Joerger_rlength*8));

	switch (channel)
		{
                 
	    case 1:
	       	source_channel=1;
	       	break;
                    
	   	case 2:
	    	source_channel=2;
	     	break;
                    		 
	   	case 3:
	      	source_channel=3;
	        break;
                    
	   	case 4:
	      	source_channel=4;
	        break;
                    
	   	case 5:
	      	source_channel=1;
	        break;
                    
	   	case 6:
	      	source_channel=2;
	        break;
                    
	   	case 7:
	      	source_channel=3;
	        break;
                    
	   	case 8:
	      	source_channel=4;
	        break;
	}

	//Read data
	VTR10012_GET_DATA(VTR10012_A32_address,source_channel, Joerger_rlength,data_array1,data_array2);

//  Try using the array copying routines from CVI
	Copy1D (data_array1, Joerger_rlength, signal_array1);
	Copy1D (data_array2, Joerger_rlength, signal_array2);

/*	FreeAnalysisMem(data_array1);
	FreeAnalysisMem(data_array2);
*/
	free(data_array1);
	free(data_array2);
}

void JoergerVTR10014_complete ()
{
int status;
    //Check all three VTR10014 instruments
	do {
		Delay (.1); //wait before checking for completion of cycle
		VTR10014_READ_STATUS(0,&status);
	}while(status != 4);   //	when the status = recording cycle is done, exit
	do {
		Delay (.1); //wait before checking for completion of cycle
		VTR10014_READ_STATUS(1,&status);
	}while(status != 4);   //	when the status = recording cycle is done, exit
	do {;
		Delay (.1); //wait before checking for completion of cycle
		VTR10014_READ_STATUS(2,&status);
	}while(status != 4);   //	when the status = recording cycle is done, exit
}



void JoergerVTR10014_trigger_status (int *status)
{
	//Right now, just read the status of the last of the three VTR10014s. This assumes what happened to one happened to all
	VTR10014_READ_STATUS(2,status);
}




void JoergerVTR10014_trigger ()
{
	//Trigger all 3 VTR10014 instruments
	VTR10014_SOFTWARE_TRIGGER(0);
	VTR10014_SOFTWARE_TRIGGER(1);
	VTR10014_SOFTWARE_TRIGGER(2);
}

void JoergerVTR10014_acquire(int channel,int Joerger_rlength, double signal_array1[], double signal_array2[])
{
	int		source_channel;
	int		i;
	int		INSTR;		//which of the 3 VTR10014s in in use
	double 	*data_array1;
	double	*data_array2;
	
	//Allot space to read data records
	data_array1 = malloc ((Joerger_rlength*8));
	data_array2 = malloc ((Joerger_rlength*8));

	switch (channel)
		{
                 
	    case 1:
	       	source_channel=1;
			INSTR=0;
	       	break;
                    
	   	case 2:
	    	source_channel=2;
			INSTR=0;
	     	break;
                    		 
	   	case 3:
	      	source_channel=3;
			INSTR=0;
	        break;
                    
	   	case 4:
	      	source_channel=4;
			INSTR=0;
	        break;
                    
	   	case 5:
	      	source_channel=1;
			INSTR=0;
	        break;
                    
	   	case 6:
	      	source_channel=2;
			INSTR=0;
	        break;
                    
	   	case 7:
	      	source_channel=3;
			INSTR=0;
	        break;
                    
	   	case 8:
	      	source_channel=4;
			INSTR=1;
	        break;
	    case 9:
	       	source_channel=1;
			INSTR=1;
	       	break;
                    
	   	case 10:
	    	source_channel=2;
			INSTR=1;
	     	break;
                    		 
	   	case 11:
	      	source_channel=3;
			INSTR=1;
	        break;
                    
	   	case 12:
	      	source_channel=4;
			INSTR=1;
	        break;
                    
	   	case 13:
	      	source_channel=1;
			INSTR=1;
	        break;
                    
	   	case 14:
	      	source_channel=2;
			INSTR=1;
	        break;
                    
	   	case 15:
	      	source_channel=3;
			INSTR=1;
	        break;
                    
	   	case 16:
	      	source_channel=4;
			INSTR=1;
	        break;
	    case 17:
	       	source_channel=1;
			INSTR=2;
	       	break;
                    
	   	case 18:
	    	source_channel=2;
			INSTR=2;
	     	break;
                    		 
	   	case 19:
	      	source_channel=3;
			INSTR=2;
	        break;
                    
	   	case 20:
	      	source_channel=4;
			INSTR=2;
	        break;
                    
	   	case 21:
	      	source_channel=1;
			INSTR=2;
	        break;
                    
	   	case 22:
	      	source_channel=2;
			INSTR=2;
	        break;
                    
	   	case 23:
	      	source_channel=3;
			INSTR=2;
	        break;
                    
	   	case 24:
	      	source_channel=4;
			INSTR=2;
	        break;
	}

	//Read data
	VTR10014_GET_DATA(INSTR,source_channel, Joerger_rlength,data_array1,data_array2);

//  Try using the array copying routines from CVI
	Copy1D (data_array1, Joerger_rlength, signal_array1);
	Copy1D (data_array2, Joerger_rlength, signal_array2);

/*	FreeAnalysisMem(data_array1);
	FreeAnalysisMem(data_array2);
*/
	free(data_array1);
	free(data_array2);
}

/*
int CVICALLBACK close_Joerger_panel (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
void Joerger_setup(void);
void JoergerVTR10014_setup(void);

	switch (event)
		{
		case EVENT_COMMIT:
				//Get digitizer rate and record length  
				GetCtrlVal (Joerger_setup_panel, JOERGER_CLOCKSPEED, &Joerger_clockspeed);
				GetCtrlVal (Joerger_setup_panel, JOERGER_CLOCKSPEED_VTR14, &JoergerVTR14_clockspeed);
				GetCtrlVal (Joerger_setup_panel, JOERGER_RLENGTH, &Joerger_rlength);
				
				GetCtrlVal (Joerger_setup_panel, JOERGER_KNOB_AMP_1, &Mst_amp_gain_1);
				GetCtrlVal (Joerger_setup_panel, JOERGER_KNOB_AMP_2, &Mst_amp_gain_2);
				GetCtrlVal (Joerger_setup_panel, JOERGER_KNOB_AMP_3, &Mst_amp_gain_3);
				GetCtrlVal (Joerger_setup_panel, JOERGER_KNOB_AMP_4, &Mst_amp_gain_4);
				GetCtrlVal (Joerger_setup_panel, JOERGER_KNOB_AMP_5, &Mst_amp_gain_5);
				GetCtrlVal (Joerger_setup_panel, JOERGER_KNOB_AMP_6, &Mst_amp_gain_6);
				GetCtrlVal (Joerger_setup_panel, JOERGER_KNOB_AMP_7, &Mst_amp_gain_7);
				GetCtrlVal (Joerger_setup_panel, JOERGER_KNOB_AMP_8, &Mst_amp_gain_8);

				GetCtrlVal (Joerger_setup_panel, JOERGER_PROBE1POS, &Bdot_Probe1_pos);
				GetCtrlVal (Joerger_setup_panel, JOERGER_PROBE2POS, &Bdot_Probe2_pos);
				GetCtrlVal (Joerger_setup_panel, JOERGER_PROBE3POS, &Bdot_Probe3_pos);
				GetCtrlVal (Joerger_setup_panel, JOERGER_PROBE4POS, &Bdot_Probe4_pos);
				GetCtrlVal (Joerger_setup_panel, JOERGER_PROBE5POS, &Bdot_Probe5_pos);
				GetCtrlVal (Joerger_setup_panel, JOERGER_PROBE6POS, &Bdot_Probe6_pos);
				GetCtrlVal (Joerger_setup_panel, JOERGER_PROBE7POS, &Bdot_Probe7_pos);
				GetCtrlVal (Joerger_setup_panel, JOERGER_PROBE8POS, &Bdot_Probe8_pos);

				SavePanelState (Joerger_setup_panel, "preferences", Joerger_setup_state);
				Joerger_setup();
				JoergerVTR10014_setup();
				DiscardPanel (panel);
				Joerger_setup_panel=0;
			break;
		}
	return 0;
}
*/
/*
void JoergerVTR10012_setup ()
{
    int Joerger_id_value;
    int return_value; 
    int status;
     
	//load panel in background to set default values
	if(Joerger_setup_panel == 0) {
		Joerger_setup_panel = LoadPanel (0, "Joerger_settings.uir", JOERGER);
		RecallPanelState (Joerger_setup_panel, "preferences", Joerger_setup_state);
	}
	   
	//Get digitizer rate and record length  
	GetCtrlVal (Joerger_setup_panel, JOERGER_CLOCKSPEED, &Joerger_clockspeed);
	GetCtrlVal (Joerger_setup_panel, JOERGER_RLENGTH, &Joerger_rlength);
    
    //Define Joerger Digitation rate for use later
	switch (Joerger_clockspeed)
		{
                 
	    case 0:
	       	Joerger_digitation_rate=100.0E6;
	       	break;
                    
	   	case 1:
	       	Joerger_digitation_rate=50.0E6;
	     	break;
                    		 
	   	case 2:
	      	Joerger_digitation_rate=25.0E6;
	        break;
                    
	   	case 3:
	      	Joerger_digitation_rate=10.0E6;
	        break;
                    
	   	case 4:
	      	Joerger_digitation_rate=5.0E6;
	        break;
                    
	   	case 5:
	      	Joerger_digitation_rate=2.5E6;
	        break;
                    
	   	case 6:
	      	Joerger_digitation_rate=1.0E6;
	        break;
    }
    
	//Close Panel now done
    HidePanel(Joerger_setup_panel);
    
    //Reset VTR10012 and Check its ID
	VTR10012_RESET ();
	VTR10012_READ_ID(&Joerger_id_value);

	//Set A32 base address to same value as A16 base address of instrument
	VTR10012_SET_A32_ADDRESS(VTR10012_A32_address);

	//Setup clock rate, trigger style, reset memory counter and length of data records
	VTR10012_SET_CLOCK(Joerger_clockspeed);
	VTR10012_SET_TRIGGER();
	VTR10012_SET_RECORD_LENGTH(Joerger_rlength);

	VTR10012_RESET_MEM_COUNTER();
	Delay(.1);   //wait a bit for instrument to settle
			
	//Check Digitizer
	Delay(.1);  //wait a bit for instrument to settle
	VTR10012_READ_STATUS(&status);
}
*/

/*void JoergerVTR10014_setup ()
{
    int Joerger_id_value;
    int return_value; 
    int status;

	//This routine sets the VTR10014 instruments to the same settings as the VTR10012 instrument. 
	//This can be modified later if different settings are desired.
	
	//load panel in background to set default values
	if(Joerger_setup_panel == 0) {
		Joerger_setup_panel = LoadPanel (0, "Joerger_settings.uir", JOERGER);
		RecallPanelState (Joerger_setup_panel, "preferences", Joerger_setup_state);
	}
	   
	//Get digitizer rate and record length  
	GetCtrlVal (Joerger_setup_panel, JOERGER_CLOCKSPEED_VTR14, &JoergerVTR14_clockspeed);
	GetCtrlVal (Joerger_setup_panel, JOERGER_RLENGTH_VTR14, &JoergerVTR14_rlength);
    
    //Define Joerger Digitation rate for use later
	switch (JoergerVTR14_clockspeed)
		{
                 
	    case 0:
	       	JoergerVTR14_digitation_rate=100.0E6;
	       	break;
                    
	   	case 1:
	       	JoergerVTR14_digitation_rate=50.0E6;
	     	break;
    }
    
	//Close Panel now done
    HidePanel(Joerger_setup_panel);
    
    //Reset VTR10014s and Check their ID
	VTR10014_RESET (0);
	VTR10014_READ_ID(0,&Joerger_id_value);
	VTR10014_RESET (1);
	VTR10014_READ_ID(1,&Joerger_id_value);
	VTR10014_RESET (2);
	VTR10014_READ_ID(2,&Joerger_id_value);

	//Set A32 base address to same value as A16 base address of instruments
	VTR10014_SET_A32_ADDRESS(0,VTR10014_A32_address0);
	VTR10014_SET_A32_ADDRESS(1,VTR10014_A32_address1);
	VTR10014_SET_A32_ADDRESS(2,VTR10014_A32_address2);

	//Setup clock rate, trigger style, reset memory counter and length of data records on all threee instruments
	VTR10014_SET_CLOCK(0,JoergerVTR14_clockspeed);
	VTR10014_SET_CLOCK(1,JoergerVTR14_clockspeed);
	VTR10014_SET_CLOCK(2,JoergerVTR14_clockspeed);

	VTR10014_SET_TRIGGER(0);
	VTR10014_SET_TRIGGER(1);
	VTR10014_SET_TRIGGER(2);

	VTR10014_SET_RECORD_LENGTH(0,JoergerVTR14_rlength);
	VTR10014_SET_RECORD_LENGTH(1,JoergerVTR14_rlength);
	VTR10014_SET_RECORD_LENGTH(2,JoergerVTR14_rlength);

	VTR10014_RESET_MEM_COUNTER(0);
	VTR10014_RESET_MEM_COUNTER(1);
	VTR10014_RESET_MEM_COUNTER(2);
	Delay(.1);   //wait a bit for instrument to settle
			
	//Check Digitizer
	Delay(.1);  //wait a bit for instrument to settle
	VTR10014_READ_STATUS(0,&status);
	VTR10014_READ_STATUS(1,&status);
	VTR10014_READ_STATUS(2,&status);
}
*/
