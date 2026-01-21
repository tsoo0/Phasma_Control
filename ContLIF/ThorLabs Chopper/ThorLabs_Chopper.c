#include <visa.h>
#include <rs232.h>
#include <string.h> 
#include <stdio.h>
#include <userint.h>
#include <formatio.h>
#include <ansi_c.h>
#include <utility.h>
#include "MC2000CommandLib.h"
#include "GlobalVariables.h"


/* Codes to run the Thor Labs chopper
*/

void setup_Thor_Chopper(char COM_port_string[], int Chopper_Frequency)
{
	int 	Thor_Chopper_handle;
	ViStatus result;
	char 	serialport[64];
	//Open visa session to the Thorlabs Chopper Controller
	//result=viOpen (Global_Visa_Session_Handle, "ASRL23::INSTR", VI_NULL, VI_NULL, &Thor_Chopper_handle);


	
	//Old style communications
	//Use SRS library to open port to instrument at Chopper port with 115200 baud rate and a timeout of 3
	//Thor_Chopper_handle = Open(COM_port_string, 115200, 3);  
	Thor_Chopper_handle = Open("COM23", 19200, 3);  
 	result=IsOpen("COM23");
	//Thor_Chopper_handle = OpenComConfig (23, "COM23", 9600, 0, 8, 1, 512, 512);
	//OpenComConfig ("COM23", COM_string, 9600, 0, 8, 1, 512, 512);
	
	
	//Only try to talk to chopper if it exists
	if (Thor_Chopper_handle > -1 ) {
	
		//Set the frequency
		result = SetFrequency(Thor_Chopper_handle, Chopper_Frequency);
	
		//Turn chopper on by setting status to 1
		result = SetEnable(Thor_Chopper_handle, 1); 
	
	 	//Close COM port session
		result = Close(Thor_Chopper_handle);
	}
}

void disable_Thor_Chopper(char COM_port_string[])
{
	int 	Thor_Chopper_handle;
	ViStatus result;

	//Use SRS library to open port to instrument at Chopper port with 115200 baud rate and a timeout of 3
	Thor_Chopper_handle = Open(COM_port_string, 115200, 3);  

	//Only try to talk to chopper if it exists
	if (Thor_Chopper_handle > -1 ) {
	
		//Turn chopper off by setting status to 0
		SetEnable(Thor_Chopper_handle, 0); 
	
	 	//Close COM port session  
		result = Close(Thor_Chopper_handle);
	}
	
}
 
