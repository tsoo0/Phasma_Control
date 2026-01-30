/*
Note: In this code UINT32 has been replaced with long int and UINT16 with int data types
*/

#include	<nivxi.h>	
#include 	<formatio.h>
#define  	JoergerVTR10014_voltage_scale 1.0/8192.0  




void VTR10014_RESET(int INST_NUM)
{
	long int	Total_address;

	//Create total address and then write to instrument reset register
	switch (INST_NUM) {
    	  case 0: Total_address=VTR10014_Base_address0+0x0; //Add offset of 0x0 for reset register
    	  		  break;
    	  case 1: Total_address=VTR10014_Base_address1+0x0; //Add offset of 0x0 for reset register
    	  		  break;
    	  case 2: Total_address=VTR10014_Base_address2+0x0; //Add offset of 0x0 for reset register
    	  		  break;
    	  default: printf("Invalid instrument ID choice \n");
    }	  

	if ( VXIout (1, Total_address, 2, 0) )
	{
		printf("Unable to reset VTR10014 module\n");
		exit (0);
	}
}


void VTR10014_CLOSE()
{
	//This function must be called to close VXI library. 
	if ( CloseVXIlibrary() )	
	{
		printf ("Unable to Close VXI library... exiting");
		exit (0);
	}
}


void VTR10014_RESET_MEM_COUNTER(int	INST_NUM)
{
	long int	Total_address;

	//Create total address and then write to instrument memory counter reset register
 	switch (INST_NUM) {
    	  case 0: Total_address=VTR10014_Base_address0+0x18; //Add offset of 0x18 for reset register  
    	  		  break;
    	  case 1: Total_address=VTR10014_Base_address1+0x18; //Add offset of 0x18 for reset register   
    	  		  break;
    	  case 2: Total_address=VTR10014_Base_address2+0x18; //Add offset of 0x18 for reset register   
    	  		  break;
    	  default: printf("Invalid instrument ID choice \n");
    }	  

	if ( VXIout (1, Total_address, 2, 0) )
	{
		printf("Unable to reset memory counter register for VTR10014 module\n");
		exit (0);
	}
}


void VTR10014_READ_ID(int INST_NUM, int *Reg_value)
{
	long int	Total_address;
	int	Register_value;

	//Create total address and then query instrument ID register
 	switch (INST_NUM) {
    	  case 0: Total_address=VTR10014_Base_address0+0xC; //Add offset of 0xC for ID register   
    	  		  break;
    	  case 1: Total_address=VTR10014_Base_address1+0xC; //Add offset of 0xC for ID register    
    	  		  break;
    	  case 2: Total_address=VTR10014_Base_address2+0xC; //Add offset of 0xC for ID register    
    	  		  break;
    	  default: printf("Invalid instrument ID choice \n");
    }	  

	if ( VXIin (1, Total_address, 2, &Register_value) )
	{
		printf("Unable to read VTR10014 module ID register\n");
		exit (0);
	}
	*Reg_value=Register_value;
}



void VTR10014_READ_STATUS(int INST_NUM,int *Reg_value)
{
	long int	Total_address;
	int	Register_value;

	//Create total address and then query instrument status register
	switch (INST_NUM) {
    	  case 0: Total_address=VTR10014_Base_address0+0x2; //Add offset of 0x2 for status register    
    	  		  break;
    	  case 1: Total_address=VTR10014_Base_address1+0x2; //Add offset of 0x2 for status register      
    	  		  break;
    	  case 2: Total_address=VTR10014_Base_address2+0x2; //Add offset of 0x2 for status register     
    	  		  break;
    	  default: printf("Invalid instrument ID choice \n");
    }	  

	if ( VXIin (1, Total_address, 2, &Register_value) )
	{
		printf("Unable to read VTR10014 module status register\n");
		exit (0);
	}
	*Reg_value=Register_value;
}


void VTR10014_SET_TRIGGER(int INST_NUM)
{
	long int	Total_address;
	int	Register_value;
	
	//Create total address and then write to instrument control register to setup trigger
	switch (INST_NUM) {
    	  case 0: Total_address=VTR10014_Base_address0+0x4; //Add offset of 0x4 for setting up trigger     
    	  		  break;
    	  case 1: Total_address=VTR10014_Base_address1+0x4; //Add offset of 0x4 for setting up trigger         
    	  		  break;
    	  case 2: Total_address=VTR10014_Base_address2+0x4; //Add offset of 0x4 for setting up trigger       
    	  		  break;
    	  default: printf("Invalid instrument ID choice \n");
    }	  

	//Setup trigger state for software trigger or external trigger, wrap mode, automatic disarm at end of cycle, and auto reset memory location counter
	//D0=1, D1=1, D2=1, D3=1, D4=1, D6 =1, for a total value of 31 in decimal and 0x1F 
	if ( VXIout (1, Total_address, 2, 0x1F) )
	{
		printf("Unable to setup trigger state on VTR10014 module\n");
		exit (0);
	}

	//Create total address and then write to Comparator Threshold register to set threshold valid voltage
	switch (INST_NUM) {
    	  case 0: Total_address=VTR10014_Base_address0+0x82; //Add offset of 0x82 for setting up comparator threshold register      
    	  		  break;
    	  case 1: Total_address=VTR10014_Base_address1+0x82; //Add offset of 0x82 for setting up comparator threshold register          
    	  		  break;
    	  case 2: Total_address=VTR10014_Base_address2+0x82; //Add offset of 0x82 for setting up comparator threshold register         
    	  		  break;
    	  default: printf("Invalid instrument ID choice \n");
    }	  
	
	//The threshold comparator uses 0.488mV per bit. Set threshold value of all channels to zero
	if ( VXIout (1, Total_address, 2, 0x0) )
	{
		printf("Unable to setup threshold comparator state on VTR10014 module\n");
		exit (0);
	}
}


void VTR10014_SET_A32_ADDRESS(int INST_NUM, long int address)
//This function sets the register in the 16 bit address space for bits from 24 to 32 to identify where the A32 data start
{
	long int	Total_address;

	//Create total address and then write to A32 base address to correct register
	switch (INST_NUM) {
    	  case 0: Total_address=VTR10014_Base_address0+0x1C; //Add offset of 0x1C for writing A32 base address  
    	  		  break;
    	  case 1: Total_address=VTR10014_Base_address1+0x1C; //Add offset of 0x1C for writing A32 base address        
    	  		  break;
    	  case 2: Total_address=VTR10014_Base_address2+0x1C; //Add offset of 0x1C for writing A32 base address       
    	  		  break;
    	  default: printf("Invalid instrument ID choice \n");
    }	  
	
	//Setup A32 base address using user input 
	if ( VXIout (1, Total_address, 2,address) )
	{
		printf("Unable to setup A32 base address on VTR10014 module\n");
		exit (0);
	}
}


void VTR10014_READ_A32_ADDRESS(int INST_NUM,long int *address)
{
	long int	Total_address;
	int	Register_value;

	//Create total address and then read A32 base address from correct A16 register
	switch (INST_NUM) {
    	  case 0: Total_address=VTR10014_Base_address0+0x1C; //Add offset of 0x1C for reading A32 base address
    	  		  break;
    	  case 1: Total_address=VTR10014_Base_address1+0x1C; //Add offset of 0x1C for reading A32 base address       
    	  		  break;
    	  case 2: Total_address=VTR10014_Base_address2+0x1C; //Add offset of 0x1C for reading A32 base address    
    	  		  break;
    	  default: printf("Invalid instrument ID choice \n");
    }	  
	
	//Read A32 base address 
	if ( VXIin (1, Total_address, 2, &Register_value) )   //Read as a single byte
	{
		printf("Unable to read A32 base address from VTR10014 module\n");
		exit (0);
	}
	*address=Register_value;
}


void VTR10014_SET_RECORD_LENGTH(int INST_NUM,int length)
{
	long int	Total_address;
	long int	samples;
	long int	lower_samples;
		
	if (length < 65536) {
		//Set high byte to zero and set low byte
		
		//Create total address for high byte and then write to gate duration register to setup record length
		switch (INST_NUM) {
	    	  case 0: Total_address=VTR10014_Base_address0+0x20; //Add offset of 0x20 for high byte on gate duration  
	    	  		  break;
	    	  case 1: Total_address=VTR10014_Base_address1+0x20; //Add offset of 0x20 for high byte on gate duration    
	    	  		  break;
	    	  case 2: Total_address=VTR10014_Base_address2+0x20; //Add offset of 0x20 for high byte on gate duration     
	    	  		  break;
	    	  default: printf("Invalid instrument ID choice \n");
	    }	  
	
		//Set high byte
		if ( VXIout (1, Total_address, 2, 0) )
		{
			printf("Unable to set high byte on gate duration register on VTR10014 module\n");
			exit (0);
		}
		
		//Create total address for low byte and then write to gate duration register to setup record length
		switch (INST_NUM) {
	    	  case 0: Total_address=VTR10014_Base_address0+0x22; //Add offset of 0x20 for high byte on gate duration  
	    	  		  break;
	    	  case 1: Total_address=VTR10014_Base_address1+0x22; //Add offset of 0x20 for high byte on gate duration    
	    	  		  break;
	    	  case 2: Total_address=VTR10014_Base_address2+0x22; //Add offset of 0x20 for high byte on gate duration     
	    	  		  break;
	    	  default: printf("Invalid instrument ID choice \n");
	    }	  
	
		//Set low byte
		samples=length;
		if ( VXIout (1, Total_address, 2, samples) )
		{
			printf("Unable to set low byte on gate duration register on VTR10014 module\n");
			exit (0);
		}
	}
	else {
		//Set low byte to zero and set high byte
		
		//Create total address for high byte and then write to gate duration register to setup record length
		switch (INST_NUM) {
	    	  case 0: Total_address=VTR10014_Base_address0+0x20; //Add offset of 0x20 for high byte on gate duration  
	    	  		  break;
	    	  case 1: Total_address=VTR10014_Base_address1+0x20; //Add offset of 0x20 for high byte on gate duration    
	    	  		  break;
	    	  case 2: Total_address=VTR10014_Base_address2+0x20; //Add offset of 0x20 for high byte on gate duration     
	    	  		  break;
	    	  default: printf("Invalid instrument ID choice \n");
	    }	  
	
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

		if ( VXIout (1, Total_address, 2, samples) )
		{
			printf("Unable to set high byte on gate duration register on VTR10014 module\n");
			exit (0);
		}
		
		//Create total address for low byte and then write to gate duration register to setup record length
		switch (INST_NUM) {
	    	  case 0: Total_address=VTR10014_Base_address0+0x22; //Add offset of 0x20 for high byte on gate duration  
	    	  		  break;
	    	  case 1: Total_address=VTR10014_Base_address1+0x22; //Add offset of 0x20 for high byte on gate duration    
	    	  		  break;
	    	  case 2: Total_address=VTR10014_Base_address2+0x22; //Add offset of 0x20 for high byte on gate duration     
	    	  		  break;
	    	  default: printf("Invalid instrument ID choice \n");
	    }	  
	
		//Set low byte
		if ( VXIout (1, Total_address, 2, lower_samples) )
		{
			printf("Unable to set low byte on gate duration register on VTR10014 module\n");
			exit (0);
		}
	}
}
 


void VTR10014_ARM(int INST_NUM)
{
	long int	Total_address;

	//Create total address and then write to instrument arming register
	switch (INST_NUM) {
    	  case 0: Total_address=VTR10014_Base_address0+0x12; //Add offset of 0x12 for arming register 
    	  		  break;
    	  case 1: Total_address=VTR10014_Base_address1+0x12; //Add offset of 0x12 for arming register        
    	  		  break;
    	  case 2: Total_address=VTR10014_Base_address2+0x12; //Add offset of 0x12 for arming register     
    	  		  break;
    	  default: printf("Invalid instrument ID choice \n");
    }	  

	if ( VXIout (1, Total_address, 2, 0) )
	{
		printf("Unable to arm VTR10014 module\n");
		exit (0);
	}
}


void VTR10014_DISARM(int INST_NUM)
{
	long int	Total_address;

	//Create total address and then write to instrument disarming register
	switch (INST_NUM) {
    	  case 0: Total_address=VTR10014_Base_address0+0x14; //Add offset of 0x14 for diarming register
    	  		  break;
    	  case 1: Total_address=VTR10014_Base_address1+0x14; //Add offset of 0x14 for diarming register     
    	  		  break;
    	  case 2: Total_address=VTR10014_Base_address2+0x14; //Add offset of 0x14 for diarming register   
    	  		  break;
    	  default: printf("Invalid instrument ID choice \n");
    }	  
	if ( VXIout (1, Total_address, 2, 0) )
	{
		printf("Unable to disarm VTR10014 module\n");
		exit (0);
	}

 	//Create total address and then write to instrument disarming register
	switch (INST_NUM) {
    	  case 0: Total_address=VTR10014_Base_address0+0x16; //Add offset of 0x16 for clearing IRQ source register 
    	  		  break;
    	  case 1: Total_address=VTR10014_Base_address1+0x16; //Add offset of 0x16 for clearing IRQ source register      
    	  		  break;
    	  case 2: Total_address=VTR10014_Base_address2+0x16; //Add offset of 0x16 for clearing IRQ source register    
    	  		  break;
    	  default: printf("Invalid instrument ID choice \n");
    }	  
	if ( VXIout (1, Total_address, 2, 0) )
	{
		printf("Unable to reset IRQ status for VTR10014 module\n");
		exit (0);
	}
}


void VTR10014_SOFTWARE_TRIGGER(int INST_NUM)
{
	long int	Total_address;

	//Create total address and then write to instrument to initiate trigger
	switch (INST_NUM) {
    	  case 0: Total_address=VTR10014_Base_address0+0x10; //Add offset of 0x10 for software trigger
    	  		  break;
    	  case 1: Total_address=VTR10014_Base_address1+0x10; //Add offset of 0x10 for software trigger    
    	  		  break;
    	  case 2: Total_address=VTR10014_Base_address2+0x10; //Add offset of 0x10 for software trigger
    	  		  break;
    	  default: printf("Invalid instrument ID choice \n");
    }	  
	if ( VXIout (1, Total_address, 2, 0) )
	{
		printf("Unable to trigger VTR10014 module via software\n");
		exit (0);
	}
}


void VTR10014_SET_CLOCK(int INST_NUM, int Rate_Choice)
{
	long int	Total_address;
	long int	Rate;

	//Create total address and then write to instrument to initiate trigger
 	switch (INST_NUM) {
    	  case 0: Total_address=VTR10014_Base_address0+0x0A; //Add offset of 0x0A for setting clock rate   
    	  		  break;
    	  case 1: Total_address=VTR10014_Base_address1+0x0A; //Add offset of 0x0A for setting clock rate      
    	  		  break;
    	  case 2: Total_address=VTR10014_Base_address2+0x0A; //Add offset of 0x0A for setting clock rate   
    	  		  break;
    	  default: printf("Invalid instrument ID choice \n");
    }	  

	//Use provided choice to set value of rate variable
	switch (Rate_Choice) {
    	  case 0: Rate = 0x0; //100 MHz
    	  		  break;
    	  case 1: Rate = 0x1; //50 MHz
    	  		  break;
     	  default: printf("Invalid or non-internal clock rate choice \n");
    }	  

	if ( VXIout (1, Total_address, 2, Rate) )
	{
		printf("Unable to set VTR10014 digitation rate\n");
		exit (0);
	}
}


void VTR10014_READ_CLOCK(int INST_NUM,int *Rate)
{
	long int	Total_address;
	int	Register_value; 
	
	//Create total address and then write to instrument to initiate trigger
 	switch (INST_NUM) {
    	  case 0: Total_address=VTR10014_Base_address0+0x0A; //Add offset of 0x0A for reading clock rate   
    	  		  break;
    	  case 1: Total_address=VTR10014_Base_address1+0x0A; //Add offset of 0x0A for reading clock rate        
    	  		  break;
    	  case 2: Total_address=VTR10014_Base_address2+0x0A; //Add offset of 0x0A for reading clock rate    
    	  		  break;
    	  default: printf("Invalid instrument ID choice \n");
    }	  

	if ( VXIin (1, Total_address, 2, &Register_value) )
	{
		printf("Unable to read VTR10014 digitation rate\n");
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


void VTR10014_GET_DATA(int INST_NUM, int channel_status, int record_length, double data_record1[], 
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
	
	long int srcaddr;
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
  	switch (INST_NUM) {
    	  case 0: srcaddr=VTR10014_A32_addressFull0 ; //Set first bit of A32 data to full A32 address 
    	  		  break;
    	  case 1: srcaddr=VTR10014_A32_addressFull1 ; //Set first bit of A32 data to full A32 address        
    	  		  break;
    	  case 2: srcaddr=VTR10014_A32_addressFull2 ; //Set first bit of A32 data to full A32 address    
    	  		  break;
    	  default: printf("Invalid instrument ID choice \n");
    }	  

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
				  
	//	Move Data into Correct Arrays and Scale Data Values	after shifting for range of input values					
	for (j = 0; j<record_length; j++) {
	    data_record1[j]=(real_buffer[2*j]-8191.0)*(JoergerVTR10014_voltage_scale);   
	    data_record2[j]=(real_buffer[2*j+1]-8191.0)*(JoergerVTR10014_voltage_scale);
	}
	FreeAnalysisMem(move_buffer);
	FreeAnalysisMem(real_buffer);
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
