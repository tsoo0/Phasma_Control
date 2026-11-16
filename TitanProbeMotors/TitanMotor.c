/**********************************************************************************
 //																				  
 //	  Code	             :  Titan motor moving code 						  
 //   Writen By          :  Earl Scime				  
 //   Date Alpha version : Feb 11, 2026											  							  
 //																				  
 //   Codes to move the Titan motors on probes and the plasma gun		
 //   Note that the Titan motors should all have different IP addresses
 //   and they should all be set to network ID=1. This code assumes the 
 //   network ID for each motor has been set to ID=1.
 //   Flashing blue light on motor means that the motor has a fault and
 //   the fault must be cleared - probably with the Titan 6.0 software
 //																			  
 //   MODIFICATION: 								  
 //
 *********************************************************************************/

#include 	<ansi_c.h>
#include 	<utility.h>
#include	<visa.h>
#include 	<formatio.h>
#include 	"GlobalVariables.h"
#include 	<stdio.h>



int TitanMotorMove(char ip_address[64], float current_position, float target_position, int stepspermm)
{
#define 	Titan_delay 0.3
	int		steps;
	int 	target_steps;
	int		result;
	int		count;
	int 	current_counts=0;
	int		trap_count=0;
	char	send_string[256]=" ";
	char	read_string[256]=" ";
	char 	dummy_string[16];
		
	ViSession Titan_handle;

	
	//Open communications with the Titan motor
	result=viOpen (Global_Visa_Session_Handle, ip_address, VI_NULL, VI_NULL, &Titan_handle);

	if (!result) {
		//Reset encoders and clear faults
		sprintf (send_string,"@01:SVRST\r\n");
		result = viWrite (Titan_handle, (ViConstBuf)send_string, (ViUInt32)strlen(send_string), &count);
		Delay(Titan_delay);		//Give device time to respond
		sprintf (send_string,"@01:ECLEARX\r\n");
		result = viWrite (Titan_handle, (ViConstBuf)send_string, (ViUInt32)strlen(send_string), &count);

		//clear reply buffer from motor by reading it all out
		result = viRead(Titan_handle, (ViConstBuf)read_string, 256, &count);
		//Delay(0.2);		//Give device time to respond
		
		//Find out the current motor encoder value as the first thing to do
		sprintf (send_string,"@01:EX\r\n");
		result = viWrite (Titan_handle, (ViConstBuf)send_string, (ViUInt32)strlen(send_string), &count);
		Delay(Titan_delay);	//Give device time to respond
		
		//read the response from the motor
		result = viRead(Titan_handle, (ViConstBuf)read_string, 256, &count);
		//Delay(0.2);		//Give device time to respond
		
		//Strip out the counts value from the encoder
		sscanf(read_string,"%7s %d",dummy_string,&current_counts);
		
		//Set the current limit to 2000 mA
		sprintf (send_string,"@01:CEVAL=2000\r\n");
		result = viWrite (Titan_handle, (ViConstBuf)send_string, (ViUInt32)strlen(send_string), &count);
		Delay(Titan_delay);	//Give device time to respond
				
		//Set the target speed to 200
		sprintf (send_string,"@01:HSPD=200\r\n");
		result = viWrite (Titan_handle, (ViConstBuf)send_string, (ViUInt32)strlen(send_string), &count);
		Delay(Titan_delay);		//Give device time to respond
				
		//read the response from the motor
		//result = viRead(Titan_handle, (ViConstBuf)send_string, 32, &count);
		
		//Set the target acceleration of 6000
		sprintf (send_string,"@01:ACC=6000\r\n");
		result = viWrite (Titan_handle, (ViConstBuf)send_string, (ViUInt32)strlen(send_string), &count);
		Delay(Titan_delay);		//Give device time to respond
				
		//read the response from the motor
		//result = viRead(Titan_handle, (ViConstBuf)send_string, 32, &count);
		
		//Calculate move steps in absolute units. Use negative sign to account for gearing direction
		steps=-1.0*(int)((target_position- current_position)*stepspermm);
		
		//Calculate target position in absolute steps
		target_steps=current_counts+steps;
		
		//read the response from the motor to clear out the buffer
		result = viRead(Titan_handle, (ViConstBuf)read_string, 32, &count);
		
		//Move to the target position in absolute counts if a move is called for
		if (fabs(target_steps)>0) {
	
			//Enable the motor
			sprintf(send_string,"@01:SVON\r\n");
			result = viWrite (Titan_handle, (ViConstBuf)send_string, (ViUInt32)strlen(send_string), &count);
			Delay(3.0);		//Give device extra time to enable, seems like an entire second is needed

			//Send command to move motor
			sprintf (send_string,"@01:X=%d\r\n", target_steps);
			result = viWrite (Titan_handle, (ViConstBuf)send_string, (ViUInt32)strlen(send_string), &count);
			Delay(Titan_delay);  //Give motor time to move
			  
			//read the response from the motor
			//result = viRead(Titan_handle, (ViConstBuf)send_string, 32, &count);
			
			//clear reply buffer from motor by reading it all out
			result = viRead(Titan_handle, (ViConstBuf)read_string, 256, &count);
		
			//limit number of checks to 10 for motor position
			while ( (fabs((current_counts - target_steps))>50) && (trap_count < 10) ) {
				
				//increment trapped counts
				trap_count=trap_count+1;
				
				//Check to see if motor reached goal
				sprintf (send_string,"@01:EX\r\n");
				result = viWrite (Titan_handle, (ViConstBuf)send_string, (ViUInt32)strlen(send_string), &count);
				//Delay(0.2);		//Give device time to respond
				
				//read the response from the motor
				result = viRead(Titan_handle, (ViConstBuf)read_string, 256, &count);
				
				//Strip out the counts value from the encoder
				sscanf(read_string,"%7s %d",dummy_string,&current_counts);
			}
		}	
		
		//Turn motor off
		sprintf (send_string,"@01:SVOFF\r\n");
		result = viWrite (Titan_handle, (ViConstBuf)send_string, (ViUInt32)strlen(send_string), &count);
				
		//read the response from the motor
		//result = viRead(Titan_handle, (ViConstBuf)send_string, 32, &count);
		
		//Close motor
		result = viClose(Titan_handle);
	
	}
	
	if (trap_count > 5) {
		return -1;
	} else {
		return result;
	}
}


int TitanMotorHome(char ip_address[64])
{
#define 	Titan_delay 0.2
	int		motor_state;
	int		result;
	int		count;
	int 	increment=0;
	char	send_string[256]=" ";
	char 	dummy[16];
	
	ViSession Titan_handle;

	
	//Open communications with the Titan motor
	result=viOpen (Global_Visa_Session_Handle, ip_address, VI_NULL, VI_NULL, &Titan_handle);

	if (!result) {
		//Enable the motor
		sprintf(send_string,"@01:SVON\r\n");
		result = viWrite (Titan_handle, (ViConstBuf)send_string, (ViUInt32)strlen(send_string), &count);
				
		//read the response from the motor
		//result = viRead(Titan_handle, (ViConstBuf)send_string, 32, &count);
		
		//Set the target speed to 12
		sprintf (send_string,"@01:HSPD=12\r\n");
		result = viWrite (Titan_handle, (ViConstBuf)send_string, (ViUInt32)strlen(send_string), &count);
				
		//read the response from the motor
		//result = viRead(Titan_handle, (ViConstBuf)send_string, 32, &count);
		
		//Set the target acceleration of 6000
		sprintf (send_string,"@01:ACC=6000\r\n");
		result = viWrite (Titan_handle, (ViConstBuf)send_string, (ViUInt32)strlen(send_string), &count);
				
		//read the response from the motor
		//result = viRead(Titan_handle, (ViConstBuf)send_string, 32, &count);
			
		//clear reply buffer from motor by reading it all out
		result = viRead(Titan_handle, (ViConstBuf)send_string, 256, &count);
		
		//Set the current limit to 350 mA
		sprintf (send_string,"@01:CEVAL=350\r\n");
		result = viWrite (Titan_handle, (ViConstBuf)send_string, (ViUInt32)strlen(send_string), &count);
				
		//Turn on error checking on current limit
		sprintf (send_string,"@01:ENAFC=4\r\n");
		result = viWrite (Titan_handle, (ViConstBuf)send_string, (ViUInt32)strlen(send_string), &count);
				
		//Set the home process to positive
		sprintf (send_string,"@01:HMODE=0\r\n");
		result = viWrite (Titan_handle, (ViConstBuf)send_string, (ViUInt32)strlen(send_string), &count);
				
		//Home the motor
		sprintf (send_string,"@01:HOMEX\r\n");
		result = viWrite (Titan_handle, (ViConstBuf)send_string, (ViUInt32)strlen(send_string), &count);
				
		//read the response from the motor
		result = viRead(Titan_handle, (ViConstBuf)send_string, 32, &count);
			
		//If motor home complete, reset encoder and shut down servo. If not, wait a second and check again.
		
		//Strip out motor state from string
		sscanf(send_string,"%10s %d",dummy, &motor_state);
		
		//Check if motor still moving and limit checking to 20 seconds
		while ((motor_state==0) && (increment < 20)) {
			increment=increment+1;
			Delay(Titan_delay);		//give it another second to move
					
			//read the response from the motor
			result = viRead(Titan_handle, (ViConstBuf)send_string, 32, &count);
			
			//Strip out motor state from string
			sscanf(send_string,"%10s %d",dummy, &motor_state);
		}
				
		//If motor home complete or fault indicated, reset encoder and shut down servo. 
		if (motor_state > 1) {
			//Reset encoders and clear faults
			sprintf (send_string,"@01:SVRST\r\n");
			result = viWrite (Titan_handle, (ViConstBuf)send_string, (ViUInt32)strlen(send_string), &count);
			sprintf (send_string,"@01:ECLEARX\r\n");
			result = viWrite (Titan_handle, (ViConstBuf)send_string, (ViUInt32)strlen(send_string), &count);
		}
				
		//Turn motor off
		sprintf (send_string,"@01:SVOFF\r\n");
		result = viWrite (Titan_handle, (ViConstBuf)send_string, (ViUInt32)strlen(send_string), &count);
				
		//read the response from the motor
		result = viRead(Titan_handle, (ViConstBuf)send_string, 32, &count);
		
		//Close motor
		result = viClose(Titan_handle);
	}	
	return result;
}