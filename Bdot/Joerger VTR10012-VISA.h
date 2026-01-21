/*
Note: In this code UINT32 has been replaced with ViUInt32 and UINT16 with ViUint16 data types.
All the function calls have been convereted from VXI library calls to Visa library calls.
In making the change from the VXI libraries to basic Visa calls, there is no need to add the instrument
base address to every register passing call as the base address is effectively buried in the instrument handdle.
In other words, previously the command was just sent out to a specific memory address regardless of the IP address
of the instrument. With this new approach, the instrument is specifically identified and then only the relevant 
register offset needs to be sent to that specific instrument.

Some of the function calls are the same for the VTR10012 instrument and the VTR10014 instrument, so those
are reused. However, some of the calls are different and so there are spefici VTR10014 versions at the end of
this file specifically for the VTR10014 instrument.

	- May 7, 2025
		E. Scime
*/
#include	<visa.h>
#include 	<formatio.h>
#define  	JoergerVTR10012_voltage_scale 1.0/2048.0  
#define  	JoergerVTR10014_voltage_scale 1.25/8192.0    //Note, the VTR10014 has a plus/minue 1.1 input range different from the VTR10012





void VTR10012_RESET(ViSession instrument_handle, ViUInt32 Base_address)
{
	ViUInt32	Total_address;

	//Create total address and then write to instrument reset register
	//Total_address=Base_address+0x0; //Add offset of 0x0 for reset register
	Total_address=0x0; //Add offset of 0x0 for reset register
	

	if ( viOut16 (instrument_handle, VI_A16_SPACE, Total_address, 0) )
	{
		printf("Unable to reset VTR10012 module\n");
		exit (0);
	}
}

void VTR10012_READ_ID(ViSession instrument_handle, ViUInt32 Base_address, int *Reg_value)
{
	ViUInt32	Total_address;
	ViUInt16	Register_value;

	//Create total address and then query instrument ID register
	//Total_address=Base_address+0xC; //Add offset of 0xC for ID register
	Total_address=0xC; //Add offset of 0xC for ID register

	if ( viIn16 (instrument_handle, VI_A16_SPACE, Total_address, &Register_value) )
	{
		printf("Unable to read VTR10012 module ID register\n");
		exit (0);
	}
	*Reg_value=Register_value;
}



void VTR10012_SET_A32_ADDRESS(ViSession instrument_handle,ViUInt32 Base_address, ViUInt32 address)
{
	ViUInt32	Total_address;

	//Create total address and then write to A32 base address to correct register
	//Total_address=Base_address+0x1C; //Add offset of 0x1C for writing A32 base address
	Total_address=0x1C; //Add offset of 0x1C for writing A32 base address
	
	//Setup A32 base address using user input 
	if ( viOut16 (instrument_handle, VI_A16_SPACE, Total_address, (ViUInt16)address) )
	{
		printf("Unable to setup A32 base address on VTR10012 module\n");
		exit (0);
	}
}


void VTR10012_SET_CLOCK(ViSession instrument_handle, ViUInt32 Base_address, int Rate_Choice)
{
	ViUInt32	Total_address;
	ViUInt32	Rate= 0x0;

	//Create total address and then write to instrument to initiate trigger
	//Total_address=Base_address+0x0A; //Add offset of 0x0A for setting clock rate
	Total_address=0x0A; //Add offset of 0x0A for setting clock rate

	//Use provided choice to set value of rate variable
	switch (Rate_Choice) {
    	  case 0: Rate = 0x0; //100 MHz
    	  		  break;
    	  case 1: Rate = 0x1; //50 MHz
    	  		  break;
    	  case 2: Rate = 0x2; //25 MHz
    	  		  break;
    	  case 3: Rate = 0x3; //10 MHz
    	  		  break;
    	  case 4: Rate = 0x4; //5 MHz
    	  		  break;
    	  case 5: Rate = 0x5; //2.5 MHz
    	  		  break;
    	  case 6: Rate = 0x6; //1 MHz
    	  		  break;
    	  default: printf("Invalid or non-internal clock rate choice \n");
    }	  

	if ( viOut16 (instrument_handle, VI_A16_SPACE, Total_address, (ViUInt16)Rate) )
	{
		printf("Unable to set VTR10012 digitation rate\n");
		exit (0);
	}
}


void VTR10012_SET_TRIGGER(ViSession instrument_handle, ViUInt32 Base_address)
{
	ViUInt32	Total_address;
	ViUInt16	Register_value;
	
	//Create total address and then write to instrument control register to setup trigger
	//Total_address=Base_address+0x4; //Add offset of 0x4 for setting up trigger
	Total_address=0x4; //Add offset of 0x4 for setting up trigger
	
	//Setup trigger state for software trigger or external trigger, no wrap mode, automatic disarm at end of cycle, and auto reset memory location counter
	//D0=1, D1=1, D2=1, D3=1, D4=1 for a total value of 31 in decimal and 0x1F
	if ( viOut16 (instrument_handle, VI_A16_SPACE, Total_address, 0x1F) )
	{
		printf("Unable to setup trigger state on VTR10012 module\n");
		exit (0);
	}


	//Create total address and then write to Comparator Threshold register to set threshold valid voltage
	//Total_address=Base_address+0x82; //Add offset of 0x82 for setting up comparator threshold register
	Total_address=0x82; //Add offset of 0x82 for setting up comparator threshold register
	
	//The threshold comparator uses 0.488mV per bit. Set threshold value of all channels to zero
	if ( viOut16 (instrument_handle, VI_A16_SPACE, Total_address, 0x0) )
	{
		printf("Unable to setup threshold comparator state on VTR10012 module\n");
		exit (0);
	}
}


void VTR10012_SET_RECORD_LENGTH(ViSession instrument_handle, ViUInt32 Base_address, int length)
{
	ViUInt32	Total_address;
	ViUInt32	samples=0;
	ViUInt32	lower_samples=0x0;

	if (length < 65536) {
		//Set high byte to zero and set low byte
		
		//Create total address for high byte and then write to gate duration register to setup record length
		//Total_address=Base_address+0x20; //Add offset of 0x20 for high byte on gate duration
		Total_address=0x20; //Add offset of 0x20 for high byte on gate duration
	
		//Set high byte
		if ( viOut16 (instrument_handle, VI_A16_SPACE, Total_address, 131071) )
		{
			printf("Unable to set high byte on gate duration register on VTR10012 module\n");
			exit (0);
		}
		
		//Create total address for low byte and then write to gate duration register to setup record length
		//Total_address=Base_address+0x22; //Add offset of 0x22 for low byte on gate duration
		Total_address=0x22; //Add offset of 0x22 for low byte on gate duration
	
		//Set low byte
		samples=length;
		if ( viOut16 (instrument_handle, VI_A16_SPACE, Total_address, (ViUInt16)samples) )
		{
			printf("Unable to set low byte on gate duration register on VTR10012 module\n");
			exit (0);
		}
	}
	else {
		//Set low byte to zero and set high byte
		
		//Create total address for high byte and then write to gate duration register to setup record length
		//Total_address=Base_address+0x20; //Add offset of 0x20 for high byte on gate duration
		Total_address=0x20; //Add offset of 0x20 for high byte on gate duration
	
		//Set high byte
		switch (length) {
    	  case 65536: 	samples = 0x1; //65536 samples
		  				lower_samples = 0x0;	
    	  	break;
    	  case 131072: 	samples = 0x2; //131072 samples 
 		  				lower_samples = 0x0;	
   	  		break;
    	  case 262142: 	samples = 0x3; //262114 samples
		  				lower_samples = 65534;	 //4 samples less than full 256K to allow for pre-trigger 
    	  	break;
     	  default: printf("Invalid record length choice \n");
    	}	  

		if ( viOut16 (instrument_handle, VI_A16_SPACE, Total_address,(ViUInt16)samples)  )
		{
			printf("Unable to set high byte on gate duration register on VTR10012 module\n");
			exit (0);
		}
		
		//Create total address for low byte and then write to gate duration register to setup record length
		//Total_address=Base_address+0x22; //Add offset of 0x22 for low byte on gate duration
		Total_address=0x22; //Add offset of 0x22 for low byte on gate duration
	
		//Set low byte
		if ( viOut16 (instrument_handle, VI_A16_SPACE, Total_address, (ViUInt16)lower_samples) )
		{
			printf("Unable to set low byte on gate duration register on VTR10012 module\n");
			exit (0);
		}
	}
}
 

void VTR10012_RESET_MEM_COUNTER(ViSession instrument_handle, ViUInt32 Base_address)
{
	ViUInt32	Total_address;

	//Create total address and then write to instrument memory counter reset register
	//Total_address=Base_address+0x18; //Add offset of 0x18 for reset register   (was 16 but P. Shi found error in May 2020)
	Total_address=0x18; //Add offset of 0x18 for reset register   (was 16 but P. Shi found error in May 2020)

	if ( viOut16 (instrument_handle, VI_A16_SPACE, Total_address, 0) )
	{
		printf("Unable to reset memory counter register for VTR10012 module\n");
		exit (0);
	}
}


void VTR10012_READ_STATUS(ViSession instrument_handle, ViUInt32 Base_address, int *Reg_value)
{
	ViUInt32	Total_address;
	ViUInt16	Register_value;

	
	//Create total address and then query instrument status register
	//Total_address=Base_address+0x2; //Add offset of 0x2 for status register
	Total_address=0x2; //Add offset of 0x2 for status register

	if ( viIn16 (instrument_handle, VI_A16_SPACE, Total_address, &Register_value)  )
	{
		printf("Unable to read VTR10012 module status register\n");
		exit (0);
	}
	*Reg_value=Register_value;
}




void VTR10012_ARM(ViSession instrument_handle, ViUInt32 Base_address)
{
	ViUInt32	Total_address;

	//Create total address and then write to instrument arming register
	//Total_address=Base_address+0x12; //Add offset of 0x12 for arming register
	Total_address=0x12; //Add offset of 0x12 for arming register

	if ( viOut16 (instrument_handle, VI_A16_SPACE, Total_address, 0)  )
	{
		printf("Unable to arm VTR10012 module\n");
		exit (0);
	}
}


void VTR10012_DISARM(ViSession instrument_handle, ViUInt32 Base_address)
{
	ViUInt32	Total_address;

	//Create total address and then write to instrument disarming register
	//Total_address=Base_address+0x14; //Add offset of 0x14 for disarming register
	Total_address=0x14; //Add offset of 0x14 for disarming register

	if ( viOut16 (instrument_handle, VI_A16_SPACE, Total_address, 0)  )
	{
		printf("Unable to disarm VTR10012 module\n");
		exit (0);
	}
	//Create total address and then write to instrument clearing IRQ source register
	//Total_address=Base_address+0x16; //Add offset of 0x16 for clearing IRQ source register
	Total_address=0x16; //Add offset of 0x16 for clearing IRQ source register

	if ( viOut16 (instrument_handle, VI_A16_SPACE, Total_address, 0)  )
	{
		printf("Unable to reset IRQ status for VTR10012 module\n");
		exit (0);
	}
}



void VTR10012_SOFTWARE_TRIGGER(ViSession instrument_handle, ViUInt32 Base_address)
{
	ViUInt32	Total_address;

	//Create total address and then write to instrument to initiate trigger
	//Total_address=Base_address+0x10; //Add offset of 0x10 for software trigger
	Total_address=0x10; //Add offset of 0x10 for software trigger

	if ( viOut16 (instrument_handle, VI_A16_SPACE, Total_address, 0))
	{
		printf("Unable to trigger VTR10012 module via software\n");
		exit (0);
	}
}


void JoergerVTR10012_complete (ViSession instrument_handle, ViUInt32 Base_address)
{
int status;
	do {
		Delay (.1); //wait before checking for completion of cycle
		VTR10012_READ_STATUS(instrument_handle, Base_address, &status);
	}while(status != 4);   //	when the status = recording cycle is done, exit
}


void JoergerVTR10012_trigger_status (ViSession instrument_handle, ViUInt32 Base_address, int *status)
{
	VTR10012_READ_STATUS(instrument_handle,Base_address, status);
}


void JoergerVTR10012_trigger (ViSession instrument_handle, ViUInt32 Base_address)
{
	VTR10012_SOFTWARE_TRIGGER(instrument_handle, Base_address);
}


 
void VTR10012_READ_A32_ADDRESS(ViSession instrument_handle, ViUInt32 Base_address, ViUInt32 *address)
{
	ViUInt32	Total_address;
	ViUInt16	Register_value;

	//Create total address and then read A32 base address from correct A16 register
	//Total_address=Base_address+0x1C; //Add offset of 0x1C for reading A32 base address
	Total_address=0x1C; //Add offset of 0x1C for reading A32 base address
	
	//Read A32 base address 
	if ( viIn16 (instrument_handle, VI_A16_SPACE, Total_address, &Register_value) )   //Read as a single byte
	{
		printf("Unable to read A32 base address from VTR10012 module\n");
		exit (0);
	}
	*address=Register_value;
}


 
void VTR10012_READ_CLOCK(ViSession instrument_handle, ViUInt32 Base_address, int *Rate)
{
	ViUInt32	Total_address;
	ViUInt16	Register_value; 
	
	//Create total address and then write to instrument to initiate trigger
	//Total_address=Base_address+0x0A; //Add offset of 0x0A for reading clock rate
	Total_address=0x0A; //Add offset of 0x0A for reading clock rate

	if ( viIn16 (instrument_handle, VI_A16_SPACE, Total_address, &Register_value)  )
	{
		printf("Unable to read VTR10012 digitation rate\n");
		exit (0);
	}
	
	//Use returned value to set value of rate variable
	switch (Register_value) {
    	  case 0: *Rate = 100;
    	  		  break;
    	  case 1: *Rate = 50;
    	  		  break;
    	  case 2: *Rate = 25;
    	  		  break;
    	  case 3: *Rate = 10;
    	  		  break;
    	  case 4: *Rate = 5;
    	  		  break;
    	  case 5: *Rate = 2.5;
    	  		  break;
    	  case 6: *Rate = 1;
    	  		  break;
    	  default: printf("Invalid or non-internal clock rate value read \n");
    }	  
}





void VTR10012_GET_DATA(ViSession instrument_handle, ViUInt32 srcaddr, int channel_status, int record_length, double data_record1[], 
						double data_record2[])
{
	//srcaddr is the address offset within the source address space
	
	//Channel status is a unique number that indicates which channels are to
	//be queryed.
	// 1 = channel 1 & 5
	// 2 = channel 2 & 6
	// 3 = channel 3 & 7
	// 4 = channel 4 & 8
	//
	//Since the data are brought over in pairs of records, we go ahead and bring
	//both over everytime and store them as a first record and second record.
	 


	//Record length is the length ofthe data record in each channel
	//
	// srcaddr:  address offset within the source address space.
	// srcparms: which address space to use as the source of the move operation.
	// dstparms: address space to use as the  destination of the move operation.
	// width:    size of each element to move.
	// record_length: number of elements to move in each record (will use twice this value)
	// dstaddr:  address offset within the destination address space.
	
	ViUInt32	srcparms;
	ViUInt32	dstparms;
	ViUInt32	Total_address;
	ViUInt32	dstaddr;
	int			Register_value; 
	char		*move_buffer;
	double		*real_buffer;
	int			width;
	int 		Status=0;
	int			j;

	//dynamically assign the memory for the records
	move_buffer = malloc ((record_length*32));
	real_buffer = malloc ((record_length*16));
		
	srcparms = 0x3;        //source is A32 space.
	dstparms = 0x0;        //destination is a local buffer.
	width = 4;			   //move each element as a long word. 
	dstaddr = (ViUInt32)move_buffer;	 //typecast pointer to UINT32 to
	                                 	//pass array beginning address.
 								
										
	//The data transfer is set up just like in the VXI version and then the LabView version - just
	//now it uses the ViMove32 command (as was in the LabView). One important step is that the
	//instrument has to have been triggered for this to work. Temporarily a software trigger has
	//been added to make sure the instrument is triggered.
	
	//Read the data from the VXI address space. It may be necessary to convert the binary data
	//to real numbers, but not sure yet.
	switch (channel_status) {
    	  case 1: //Total_address=srcaddr+0x0; //add offset for location of channel 1 & 5 data
    	  		  Total_address=0x0;			//The use of the vIMove command brings along the base address of the instrument when it finds it
    	  		  //Retrive data from module as two 2 byte numbers packed in a single 4 byte number	
    	  		  //Status = VXImove (srcparms, Total_address, dstparms, dstaddr,record_length, width);   //Original VXI function call 
				  //Status= viMove (instrument_handle, VI_A32_SPACE, Total_address, VI_WIDTH_32, VI_LOCAL_SPACE, move_buffer, VI_WIDTH_32, record_length);
				  Status= viMoveIn32(instrument_handle,VI_A32_SPACE,Total_address,record_length,(ViAUInt32)dstaddr);			  
				  break;
    	  case 2: //Total_address=//srcaddr+ 0x400000; //add offset for location of channel data
     	  		  Total_address=0x400000;			//The use of the vIMove command brings along the base address of the instrument when it finds it  
    	  		  //Retrive data from module as two 2 byte numbers packed in a single 4 byte number	
      	  		  //Status = VXImove (srcparms, Total_address, dstparms, dstaddr,record_length, width);   //Original VXI function call 
				  Status= viMoveIn32(instrument_handle,VI_A32_SPACE,Total_address,record_length,(ViAUInt32)dstaddr);
				  break;
    	  case 3:// Total_address=//srcaddr+ 0x800000; //add offset for location of channel data
     	  		  Total_address=0x800000;			//The use of the vIMove command brings along the base address of the instrument when it finds it 
    	  		  //Retrive data from module as two 2 byte numbers packed in a single 4 byte number	
     	  		  //Status = VXImove (srcparms, Total_address, dstparms, dstaddr,record_length, width);   //Original VXI function call 
				  Status= viMoveIn32(instrument_handle,VI_A32_SPACE,Total_address,record_length,(ViAUInt32)dstaddr);
				  break;
    	  case 4: //Total_address=//srcaddr+ 0xC00000; //add offset for location of channel data
    	  		  Total_address=0xC00000;			//The use of the vIMove command brings along the base address of the instrument when it finds it  	  
    	  		  //Retrive data from module as two 2 byte numbers packed in a single 4 byte number	
    	  		  //Status = VXImove (srcparms, Total_address, dstparms, dstaddr,record_length, width);   //Original VXI function call 
				  Status= viMoveIn32(instrument_handle,VI_A32_SPACE,Total_address,record_length,(ViAUInt32)dstaddr);
				  break;
    	  default: printf("Invalid channel choice \n");
    }
    if (Status)
	{		// An error occurred
		printf ("Error in viMove.  Status = %d\n", Status);	
	}
				  
	//Scan data buffer of 4 byte numbers and extract record_length elements into twice that
	//many 2 byte integer numbers.Bytes are in correct high/low byte order
	Scan(move_buffer,"%*i[b2u]>%*f[]",2*record_length,2*record_length,real_buffer);
				  
	//	Move Data into Correct Arrays and Scale Data Values						
	for (j = 0; j<record_length; j++) {
	    data_record1[j]=(real_buffer[2*j]-2047.0)*(JoergerVTR10012_voltage_scale);   
	    data_record2[j]=(real_buffer[2*j+1]-2047.0)*(JoergerVTR10012_voltage_scale);
	}
	//FreeAnalysisMem(move_buffer);
	//FreeAnalysisMem(real_buffer);
 	free(move_buffer);
	free(real_buffer);
}








//This function assumes that the Joerger module is set up to acquire single ended signals from each channel
void JoergerVTR10012_acquire(ViSession instrument_handle, ViUInt32 A32_address, int channel,int Joerger_rlength, double signal_array1[], double signal_array2[])
{
	int		source_channel=1;
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
	VTR10012_GET_DATA(instrument_handle, A32_address,source_channel, Joerger_rlength,data_array1,data_array2);

//  Try using the array copying routines from CVI
	Copy1D (data_array1, Joerger_rlength, signal_array1);
	Copy1D (data_array2, Joerger_rlength, signal_array2);

/*	FreeAnalysisMem(data_array1);
	FreeAnalysisMem(data_array2);
*/
	free(data_array1);
	free(data_array2);
}


//*************************************************************************************************************************
//Begin here the specific VTR10014 function calls

void VTR10014_GET_DATA(ViSession instrument_handle, ViUInt32 srcaddr, int channel_status, int record_length, double data_record1[], 
						double data_record2[])
{
	//srcaddr is the address offset within the source address space
	
	//Channel status is a unique number that indicates which channels are to
	//be queryed.
	// 1 = channel 1 & 5
	// 2 = channel 2 & 6
	// 3 = channel 3 & 7
	// 4 = channel 4 & 8
	//
	//Since the data are brought over in pairs of records, we go ahead and bring
	//both over everytime and store them as a first record and second record.
	 


	//Record length is the length ofthe data record in each channel
	//
	// srcaddr:  address offset within the source address space.
	// srcparms: which address space to use as the source of the move operation.
	// dstparms: address space to use as the  destination of the move operation.
	// width:    size of each element to move.
	// record_length: number of elements to move in each record (will use twice this value)
	// dstaddr:  address offset within the destination address space.
	
	ViUInt32	srcparms;
	ViUInt32	dstparms;
	ViUInt32	Total_address;
	ViUInt32	dstaddr;
	int			Register_value; 
	char		*move_buffer;
	double		*real_buffer;
	int			width;
	int 		Status=0;
	int			j;

	//dynamically assign the memory for the records
	move_buffer = malloc ((record_length*32));
	real_buffer = malloc ((record_length*16));
		
	srcparms = 0x3;        //source is A32 space.
	dstparms = 0x0;        //destination is a local buffer.
	width = 4;			   //move each element as a long word. 
	dstaddr = (ViUInt32)move_buffer;	 //typecast pointer to UINT32 to
	                                 	//pass array beginning address.
 								
										
	//The data transfer is set up just like in the VXI version and then the LabView version - just
	//now it uses the ViMove32 command (as was in the LabView). One important step is that the
	//instrument has to have been triggered for this to work. Temporarily a software trigger has
	//been added to make sure the instrument is triggered.
	
	//Read the data from the VXI address space. It may be necessary to convert the binary data
	//to real numbers, but not sure yet.
	switch (channel_status) {
    	  case 1: //Total_address=srcaddr+0x0; //add offset for location of channel 1 & 5 data
    	  		  Total_address=0x0;			//The use of the vIMove command brings along the base address of the instrument when it finds it
    	  		  //Retrive data from module as two 2 byte numbers packed in a single 4 byte number	
    	  		  //Status = VXImove (srcparms, Total_address, dstparms, dstaddr,record_length, width);   //Original VXI function call 
				  //Status= viMove (instrument_handle, VI_A32_SPACE, Total_address, VI_WIDTH_32, VI_LOCAL_SPACE, move_buffer, VI_WIDTH_32, record_length);
				  Status= viMoveIn32(instrument_handle,VI_A32_SPACE,Total_address,record_length,(ViAUInt32)dstaddr);			  
				  break;
    	  case 2: //Total_address=//srcaddr+ 0x400000; //add offset for location of channel data
     	  		  Total_address=0x400000;			//The use of the vIMove command brings along the base address of the instrument when it finds it  
    	  		  //Retrive data from module as two 2 byte numbers packed in a single 4 byte number	
      	  		  //Status = VXImove (srcparms, Total_address, dstparms, dstaddr,record_length, width);   //Original VXI function call 
				  Status= viMoveIn32(instrument_handle,VI_A32_SPACE,Total_address,record_length,(ViAUInt32)dstaddr);
				  break;
    	  case 3:// Total_address=//srcaddr+ 0x800000; //add offset for location of channel data
     	  		  Total_address=0x800000;			//The use of the vIMove command brings along the base address of the instrument when it finds it 
    	  		  //Retrive data from module as two 2 byte numbers packed in a single 4 byte number	
     	  		  //Status = VXImove (srcparms, Total_address, dstparms, dstaddr,record_length, width);   //Original VXI function call 
				  Status= viMoveIn32(instrument_handle,VI_A32_SPACE,Total_address,record_length,(ViAUInt32)dstaddr);
				  break;
    	  case 4: //Total_address=//srcaddr+ 0xC00000; //add offset for location of channel data
    	  		  Total_address=0xC00000;			//The use of the vIMove command brings along the base address of the instrument when it finds it  	  
    	  		  //Retrive data from module as two 2 byte numbers packed in a single 4 byte number	
    	  		  //Status = VXImove (srcparms, Total_address, dstparms, dstaddr,record_length, width);   //Original VXI function call 
				  Status= viMoveIn32(instrument_handle,VI_A32_SPACE,Total_address,record_length,(ViAUInt32)dstaddr);
				  break;
    	  default: printf("Invalid channel choice \n");
    }
    if (Status)
	{		// An error occurred
		printf ("Error in viMove.  Status = %d\n", Status);	
	}
				  
	//Scan data buffer of 4 byte numbers and extract record_length elements into twice that
	//many 2 byte integer numbers.Bytes are in correct high/low byte order
	Scan(move_buffer,"%*i[b2u]>%*f[]",2*record_length,2*record_length,real_buffer);
				  
	//	Move Data into Correct Arrays and Scale Data Values						
	for (j = 0; j<record_length; j++) {
	    data_record1[j]=(real_buffer[2*j]-8191.0)*(JoergerVTR10014_voltage_scale);   
	    data_record2[j]=(real_buffer[2*j+1]-8191.0)*(JoergerVTR10014_voltage_scale);
	}
	//FreeAnalysisMem(move_buffer);
	//FreeAnalysisMem(real_buffer);
 	free(move_buffer);
	free(real_buffer);
}




//This function assumes that the Joerger module is set up to acquire single ended signals from each channel
void JoergerVTR10014_acquire(ViSession instrument_handle, ViUInt32 A32_address, int channel,int Joerger_rlength, double signal_array1[], double signal_array2[])
{
	int		source_channel=1;
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
	VTR10014_GET_DATA(instrument_handle, A32_address,source_channel, Joerger_rlength,data_array1,data_array2);

//  Try using the array copying routines from CVI
	Copy1D (data_array1, Joerger_rlength, signal_array1);
	Copy1D (data_array2, Joerger_rlength, signal_array2);

/*	FreeAnalysisMem(data_array1);
	FreeAnalysisMem(data_array2);
*/
	free(data_array1);
	free(data_array2);
}


void VTR10014_SET_RECORD_LENGTH(ViSession instrument_handle, ViUInt32 Base_address, int length)
{
	ViUInt32	Total_address;
	ViUInt32	samples=0.0;
	ViUInt32	lower_samples=0.0;
		
	if (length < 65536) {
		//Set high byte to zero and set low byte
		
		//Create total address for high byte and then write to gate duration register to setup record length
		//Total_address=VTR10014_Base_address0+0x20; //Add offset of 0x20 for high byte on gate duration 
		Total_address=0x20; //Add offset of 0x20 for high byte on gate duration 
	
		//Set high byte
		if ( viOut16 (instrument_handle, VI_A16_SPACE, Total_address, 0) )
		{
			printf("Unable to set high byte on gate duration register on VTR10014 module\n");
			exit (0);
		}
		
		//Create total address for low byte and then write to gate duration register to setup record length
		//Total_address=VTR10014_Base_address0+0x22; //Add offset of 0x20 for high byte on gate duration  
		Total_address=0x22; //Add offset of 0x20 for high byte on gate duration  
	
		//Set low byte
		samples=length;
		if ( viOut16 (instrument_handle, VI_A16_SPACE, Total_address, (ViUInt16)samples) )
		{
			printf("Unable to set low byte on gate duration register on VTR10014 module\n");
			exit (0);
		}
	}
	else {
		//Set low byte to zero and set high byte
		
		//Create total address for high byte and then write to gate duration register to setup record length
		//Total_address=VTR10014_Base_address0+0x20; //Add offset of 0x20 for high byte on gate duration  
		Total_address=0x20; //Add offset of 0x20 for high byte on gate duration  
	
		//Set high byte
		switch (length) {
    	  case 65536: 	samples = 0x1; //65536 samples
		  				lower_samples=0x0;
    	  		  		break;
    	  case 131072: 	samples = 0x2; //131072 samples 
  		  				lower_samples=0x0;
  	  		  			break;
    	  case 262144: 	samples = 0x3; //262142 samples
		  				lower_samples=65534;
    	  		  		break;
     	  default: printf("Invalid record length choice \n");
    	}	  

		if ( viOut16 (instrument_handle, VI_A16_SPACE, Total_address, (ViUInt16)samples) )
		{
			printf("Unable to set high byte on gate duration register on VTR10014 module\n");
			exit (0);
		}
		
		//Create total address for low byte and then write to gate duration register to setup record length
		//Total_address=VTR10014_Base_address0+0x22; //Add offset of 0x20 for high byte on gate duration  
		Total_address=0x22; //Add offset of 0x20 for high byte on gate duration  
	
		//Set low byte
		if ( viOut16 (instrument_handle, VI_A16_SPACE, Total_address, (ViUInt16)lower_samples) )
		{
			printf("Unable to set low byte on gate duration register on VTR10014 module\n");
			exit (0);
		}
	}
}
 


void VTR10014_SET_CLOCK(ViSession instrument_handle, ViUInt32 Base_address, int Rate_Choice)
{
	ViUInt32	Total_address;
	ViUInt32	Rate=0;

	//Create total address and then write to instrument to initiate trigger
 	//Total_address=VTR10014_Base_address0+0x0A; //Add offset of 0x0A for setting clock rate   
 	Total_address=0x0A; //Add offset of 0x0A for setting clock rate   
 	
	//Use provided choice to set value of rate variable
	switch (Rate_Choice) {
    	  case 0: Rate = 0x0; //100 MHz
    	  		  break;
    	  case 1: Rate = 0x1; //50 MHz
    	  		  break;
     	  default: printf("Invalid or non-internal clock rate choice \n");
    }	  

	if ( viOut16 (instrument_handle, VI_A16_SPACE, Total_address, (ViUInt16)Rate) )
	{
		printf("Unable to set VTR10014 digitation rate\n");
		exit (0);
	}
}

