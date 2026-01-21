/*
Note: In this code UINT32 has been replaced with long int and UINT16 with int data types
*/
#include	<nivxi.h>	
#include 	<formatio.h>
#define  	VTR10012_Base_address 0xAA00   //Joerger 4 byte base address of AA00 in hexadecimal
#define	 	VTR10012_A32_address 0x0		//Joerger A32 memory address (2 byte max) in hexadecimal
#define  	Joerger_voltage_scale 1.0/2048.0  

void VTR10012_RESET()
{
	long int	Total_address;

	//Create total address and then write to instrument reset register
	Total_address=VTR10012_Base_address+0x0; //Add offset of 0x0 for reset register
	if ( VXIout (1, Total_address, 2, 0) )
	{
		printf("Unable to reset VTR10012 module\n");
		exit (0);
	}
}


void VTR10012_CLOSE()
{
	//This function must be called to close VXI library. 
	if ( CloseVXIlibrary() )	
	{
		printf ("Unable to Close VXI library... exiting");
		exit (0);
	}
}


void VTR10012_RESET_MEM_COUNTER()
{
	long int	Total_address;

	//Create total address and then write to instrument memory counter reset register
	Total_address=VTR10012_Base_address+0x18; //Add offset of 0x18 for reset register   (was 16 but P. Shi found error in May 2020)
	if ( VXIout (1, Total_address, 2, 0) )
	{
		printf("Unable to reset memory counter register for VTR10012 module\n");
		exit (0);
	}
}


void VTR10012_READ_ID(int *Reg_value)
{
	long int	Total_address;
	int	Register_value;

	//Create total address and then query instrument ID register
	Total_address=VTR10012_Base_address+0xC; //Add offset of 0xC for ID register
	if ( VXIin (1, Total_address, 2, &Register_value) )
	{
		printf("Unable to read VTR10012 module ID register\n");
		exit (0);
	}
	*Reg_value=Register_value;
}



void VTR10012_READ_STATUS(int *Reg_value)
{
	long int	Total_address;
	int	Register_value;

	//Create total address and then query instrument status register
	Total_address=VTR10012_Base_address+0x2; //Add offset of 0x2 for status register
	if ( VXIin (1, Total_address, 2, &Register_value) )
	{
		printf("Unable to read VTR10012 module status register\n");
		exit (0);
	}
	*Reg_value=Register_value;
}


void VTR10012_SET_TRIGGER()
{
	long int	Total_address;
	int	Register_value;
	
	//Create total address and then write to instrument control register to setup trigger
	Total_address=VTR10012_Base_address+0x4; //Add offset of 0x4 for setting up trigger
	
	//Setup trigger state for software trigger or external trigger, no wrap mode, automatic disarm at end of cycle, and auto reset memory location counter
	//D0=1, D1=1, D2=1, D3=1, D4=1 for a total value of 31 in decimal and 0x1F
	if ( VXIout (1, Total_address, 2, 0x1F) )
	{
		printf("Unable to setup trigger state on VTR10012 module\n");
		exit (0);
	}


	//Create total address and then write to Comparator Threshold register to set threshold valid voltage
	Total_address=VTR10012_Base_address+0x82; //Add offset of 0x82 for setting up comparator threshold register
	
	//The threshold comparator uses 0.488mV per bit. Set threshold value of all channels to zero
	if ( VXIout (1, Total_address, 2, 0x0) )
	{
		printf("Unable to setup threshold comparator state on VTR10012 module\n");
		exit (0);
	}
}


void VTR10012_SET_A32_ADDRESS(long int address)
{
	long int	Total_address;

	//Create total address and then write to A32 base address to correct register
	Total_address=VTR10012_Base_address+0x1C; //Add offset of 0x1C for writing A32 base address
	
	//Setup A32 base address using user input 
	if ( VXIout (1, Total_address, 2,address) )
	{
		printf("Unable to setup A32 base address on VTR10012 module\n");
		exit (0);
	}
}


void VTR10012_READ_A32_ADDRESS(long int *address)
{
	long int	Total_address;
	int	Register_value;

	//Create total address and then read A32 base address from correct A16 register
	Total_address=VTR10012_Base_address+0x1C; //Add offset of 0x1C for reading A32 base address
	
	//Read A32 base address 
	if ( VXIin (1, Total_address, 2, &Register_value) )   //Read as a single byte
	{
		printf("Unable to read A32 base address from VTR10012 module\n");
		exit (0);
	}
	*address=Register_value;
}


void VTR10012_SET_RECORD_LENGTH(int length)
{
	long int	Total_address;
	long int	samples;
	long int	lower_samples;

	if (length < 65536) {
		//Set high byte to zero and set low byte
		
		//Create total address for high byte and then write to gate duration register to setup record length
		Total_address=VTR10012_Base_address+0x20; //Add offset of 0x20 for high byte on gate duration
	
		//Set high byte
		if ( VXIout (1, Total_address, 2, 131071) )
		{
			printf("Unable to set high byte on gate duration register on VTR10012 module\n");
			exit (0);
		}
		
		//Create total address for low byte and then write to gate duration register to setup record length
		Total_address=VTR10012_Base_address+0x22; //Add offset of 0x22 for low byte on gate duration
	
		//Set low byte
		samples=length;
		if ( VXIout (1, Total_address, 2, samples) )
		{
			printf("Unable to set low byte on gate duration register on VTR10012 module\n");
			exit (0);
		}
	}
	else {
		//Set low byte to zero and set high byte
		
		//Create total address for high byte and then write to gate duration register to setup record length
		Total_address=VTR10012_Base_address+0x20; //Add offset of 0x20 for high byte on gate duration
	
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

		if ( VXIout (1, Total_address, 2, samples) )
		{
			printf("Unable to set high byte on gate duration register on VTR10012 module\n");
			exit (0);
		}
		
		//Create total address for low byte and then write to gate duration register to setup record length
		Total_address=VTR10012_Base_address+0x22; //Add offset of 0x22 for low byte on gate duration
	
		//Set low byte
		if ( VXIout (1, Total_address, 2, lower_samples) )
		{
			printf("Unable to set low byte on gate duration register on VTR10012 module\n");
			exit (0);
		}
	}
}
 


void VTR10012_ARM()
{
	long int	Total_address;

	//Create total address and then write to instrument arming register
	Total_address=VTR10012_Base_address+0x12; //Add offset of 0x12 for arming register
	if ( VXIout (1, Total_address, 2, 0) )
	{
		printf("Unable to arm VTR10012 module\n");
		exit (0);
	}
}


void VTR10012_DISARM()
{
	long int	Total_address;

	//Create total address and then write to instrument disarming register
	Total_address=VTR10012_Base_address+0x14; //Add offset of 0x14 for disarming register
	if ( VXIout (1, Total_address, 2, 0) )
	{
		printf("Unable to disarm VTR10012 module\n");
		exit (0);
	}
	//Create total address and then write to instrument clearing IRQ source register
	Total_address=VTR10012_Base_address+0x16; //Add offset of 0x16 for clearing IRQ source register
	if ( VXIout (1, Total_address, 2, 0) )
	{
		printf("Unable to reset IRQ status for VTR10012 module\n");
		exit (0);
	}
}


void VTR10012_SOFTWARE_TRIGGER()
{
	long int	Total_address;

	//Create total address and then write to instrument to initiate trigger
	Total_address=VTR10012_Base_address+0x10; //Add offset of 0x10 for software trigger
	if ( VXIout (1, Total_address, 2, 0) )
	{
		printf("Unable to trigger VTR10012 module via software\n");
		exit (0);
	}
}


void VTR10012_SET_CLOCK(int Rate_Choice)
{
	long int	Total_address;
	long int	Rate;

	//Create total address and then write to instrument to initiate trigger
	Total_address=VTR10012_Base_address+0x0A; //Add offset of 0x0A for setting clock rate

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

	if ( VXIout (1, Total_address, 2, Rate) )
	{
		printf("Unable to set VTR10012 digitation rate\n");
		exit (0);
	}
}


void VTR10012_READ_CLOCK(int *Rate)
{
	long int	Total_address;
	int	Register_value; 
	
	//Create total address and then write to instrument to initiate trigger
	Total_address=VTR10012_Base_address+0x0A; //Add offset of 0x0A for reading clock rate

	if ( VXIin (1, Total_address, 2, &Register_value) )
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


void VTR10012_GET_DATA(long int srcaddr, int channel_status, int record_length, double data_record1[], 
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
	
	long int	srcparms;
	long int	dstparms;
	long int	Total_address;
	long int	dstaddr;
	int	Register_value; 
	char	*move_buffer;
	double	*real_buffer;
	int		width;
	int 	Status;
	int		j;

	//dynamically assign the memory for the records
	move_buffer = malloc ((record_length*32));
	real_buffer = malloc ((record_length*16));

		
	srcparms = 0x3;        //source is A32 space.
	dstparms = 0x0;        //destination is a local buffer.
	width = 4;			   //move each element as a word. 
	dstaddr = (long int)move_buffer;	 //typecast pointer to long int to
	                                 //pass array beginning address.

	//Read the data from the VXI address space. It may be necessary to convert the binary data
	//to real numbers, but not sure yet.
	switch (channel_status) {
    	  case 1: Total_address=srcaddr+0x0; //add offset for location of channel 1 & 5 data
    	  
    	  		  //Retrive data from module as two 2 byte numbers packed in a single 4 byte number	
    	  		  Status = VXImove (srcparms, Total_address, dstparms, dstaddr,
					  record_length, width);  
				  break;
    	  case 2: Total_address=srcaddr+ 0x400000; //add offset for location of channel data
    	  
    	  		  //Retrive data from module as two 2 byte numbers packed in a single 4 byte number	
    	  		  Status = VXImove (srcparms, Total_address, dstparms, dstaddr,
					  record_length, width);  
    	  		  break;
    	  case 3: Total_address=srcaddr+ 0x800000; //add offset for location of channel data
    	  
    	  		  //Retrive data from module as two 2 byte numbers packed in a single 4 byte number	
    	  		  Status = VXImove (srcparms, Total_address, dstparms, dstaddr,
					  record_length, width);  
    	  		  break;
    	  case 4: Total_address=srcaddr+ 0xC00000; //add offset for location of channel data
    	  
    	  		  //Retrive data from module as two 2 byte numbers packed in a single 4 byte number	
    	  		  Status = VXImove (srcparms, Total_address, dstparms, dstaddr,
					  record_length, width);  
    	  		  break;
    	  default: printf("Invalid channel choice \n");
    }
    if (Status)
	{		// An error occurred
		printf ("Error in VXImove.  Status = %d\n", Status);	
	}
				  
	//Scan data buffer of 4 byte numbers and extract record_length elements into twice that
	//many 2 byte integer numbers.Bytes are in correct high/low byte order
	Scan(move_buffer,"%*i[b2u]>%*f[]",2*record_length,2*record_length,real_buffer);
				  
	//	Move Data into Correct Arrays and Scale Data Values						
	for (j = 0; j<record_length; j++) {
	    data_record1[j]=(real_buffer[2*j]-2047.0)*(Joerger_voltage_scale);   
	    data_record2[j]=(real_buffer[2*j+1]-2047.0)*(Joerger_voltage_scale);
	}
	FreeAnalysisMem(move_buffer);
	FreeAnalysisMem(real_buffer);
 }



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

