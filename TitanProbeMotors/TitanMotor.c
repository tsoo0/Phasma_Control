/**********************************************************************************
 //																				  
 //	  Code	             :  Titan motor moving code 						  
 //   Writen By          :  Earl Scime				  
 //   Date Alpha version : Feb 11, 2026											  							  
 //																				  
 //   Codes to move the Titan motors on probes and the plasma gun			  
 //																			  
 //   MODIFICATION: 								  
 //
 *********************************************************************************/

#include <ansi_c.h>
#include <utility.h>
#include	<visa.h>
#include 	<formatio.h>
#include 	"GlobalVariables.h"
#include 	<stdio.h>



int TitanMotorMove(char ip_address[64], float current_position, float target_position, int stepspermm)
{
#define 	Titan_delay 0.2
	int		steps;
	int		result;
	int		count;
	int 	current_counts=0;
	int		trap_count=0;
	char	send_string[256]=" ";
	char 	dummy_string[16];
		
	ViSession Titan_handle;

	
	//Open communications with the Titan motor
	result=viOpen (Global_Visa_Session_Handle, ip_address, VI_NULL, VI_NULL, &Titan_handle);

	if (!result) {
		//Set encoder position to zero by restting motor
		//sprintf (send_string,"@01:SVRST");
		//result = viWrite (Titan_handle, (ViConstBuf)send_string, (ViUInt32)strlen(send_string), &count);
		//Delay(0.2);		//Give device time to respond
		
		//clear reply buffer from motor by reading it all out
		result = viRead(Titan_handle, (ViConstBuf)send_string, 256, &count);
		//Delay(0.2);		//Give device time to respond
		
		//Find out the current motor encoder value as the first thing to do
		sprintf (send_string,"@01:EX\r\n");
		result = viWrite (Titan_handle, (ViConstBuf)send_string, (ViUInt32)strlen(send_string), &count);
		Delay(Titan_delay);		//Give device time to respond
		
		//read the response from the motor
		result = viRead(Titan_handle, (ViConstBuf)send_string, 256, &count);
		//Delay(0.2);		//Give device time to respond
		
		//Strip out the counts value from the encoder
		sscanf(send_string,"%7s %d",dummy_string,&current_counts);
		
		//Enable the motor
		sprintf(send_string,"@01:SVON\r\n");
		result = viWrite (Titan_handle, (ViConstBuf)send_string, (ViUInt32)strlen(send_string), &count);
		Delay(Titan_delay);		//Give device time to respond
				
		//read the response from the motor
		//result = viRead(Titan_handle, (ViConstBuf)send_string, 32, &count);
		
		//Set the target speed to 12
		sprintf (send_string,"@01:HSPD=12\r\n");
		result = viWrite (Titan_handle, (ViConstBuf)send_string, (ViUInt32)strlen(send_string), &count);
		Delay(Titan_delay);		//Give device time to respond
				
		//read the response from the motor
		//result = viRead(Titan_handle, (ViConstBuf)send_string, 32, &count);
		
		//Set the target acceleration of 2000
		//sprintf (send_string,"@01:ACC=2000\r\n");
		//result = viWrite (Titan_handle, (ViConstBuf)send_string, (ViUInt32)strlen(send_string), &count);
		//Delay(0.2);		//Give device time to respond
				
		//read the response from the motor
		//result = viRead(Titan_handle, (ViConstBuf)send_string, 32, &count);
		
		//Calculate move steps in absolute units
		steps=(int)((target_position- current_position)*stepspermm);
		
		//Calculate target position in absolute steps
		steps=current_counts+steps;
		
		//Move to the target position in absolute counts if a move is called for
		if (fabs(steps)>0) {
			sprintf (send_string,"@01:X=%d\r\n", steps);
			result = viWrite (Titan_handle, (ViConstBuf)send_string, (ViUInt32)strlen(send_string), &count);
			Delay(Titan_delay);
			  
			//read the response from the motor
			//result = viRead(Titan_handle, (ViConstBuf)send_string, 32, &count);
			
			//clear reply buffer from motor by reading it all out
			result = viRead(Titan_handle, (ViConstBuf)send_string, 256, &count);
		
			//limit number of checks to 20 for motor position
			while ( (fabs((current_counts - steps))>5) && (trap_count < 20) ) {
				
				//increment trapped counts
				trap_count=trap_count+1;
				
				//Check to see if motor reached goal
				sprintf (send_string,"@01:EX\r\n");
				result = viWrite (Titan_handle, (ViConstBuf)send_string, (ViUInt32)strlen(send_string), &count);
				//Delay(0.2);		//Give device time to respond
				
				//read the response from the motor
				result = viRead(Titan_handle, (ViConstBuf)send_string, 256, &count);
				
				//Strip out the counts value from the encoder
				sscanf(send_string,"%7s %d",dummy_string,&current_counts);
			}
		}	
		
		//Turn motor off
		//sprintf (send_string,"@01:SVOFF\r\n");
		result = viWrite (Titan_handle, (ViConstBuf)send_string, (ViUInt32)strlen(send_string), &count);
				
		//read the response from the motor
		//result = viRead(Titan_handle, (ViConstBuf)send_string, 32, &count);
		
		//Close motor
		result = viClose(Titan_handle);
	
	}
	return result;
}


int TitanMotorHome(char ip_address[64])
{
#define 	Titan_delay 0.2
	int		motor_state;
	int		result;
	int		count;
	int 	increment=0;
	char	send_string[30]=" ";
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
		
		//Set the target speed to 500
		sprintf (send_string,"@01:HSPD=500\r\n");
		result = viWrite (Titan_handle, (ViConstBuf)send_string, (ViUInt32)strlen(send_string), &count);
				
		//read the response from the motor
		//result = viRead(Titan_handle, (ViConstBuf)send_string, 32, &count);
		
		//Set the target acceleration of 2000
		sprintf (send_string,"@01:ACC=2000\r\n");
		result = viWrite (Titan_handle, (ViConstBuf)send_string, (ViUInt32)strlen(send_string), &count);
				
		//read the response from the motor
		//result = viRead(Titan_handle, (ViConstBuf)send_string, 32, &count);
			
		//clear reply buffer from motor by reading it all out
		result = viRead(Titan_handle, (ViConstBuf)send_string, 256, &count);
		
		//Home the motor
		sprintf (send_string,"@01:HOMEX\r\n");
		result = viWrite (Titan_handle, (ViConstBuf)send_string, (ViUInt32)strlen(send_string), &count);
				
		//read the response from the motor
		result = viRead(Titan_handle, (ViConstBuf)send_string, 32, &count);
			
		//If motor home complete, reset encoder and shut down servo. If not, wait a second and check again.
		
		//Strip out motor state from string
		sscanf(send_string,"%10s %d",dummy, &motor_state);
		
		//Check if motor still moving and limit checking to 10 seconds
		while ((motor_state!=0) && (increment < 10)) {
			increment=increment+1;
			Delay(Titan_delay);		//give it another second to move
			
			//Home the motor
			sprintf (send_string,"@01:HOMEX\r\n");
			result = viWrite (Titan_handle, (ViConstBuf)send_string, (ViUInt32)strlen(send_string), &count);
			
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