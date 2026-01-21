 /***************************************************************************		
 * WVU Software to Control Sirah Matisse Dye Laser                          *
 *																			*
 * Written by       Earl Scime												*
 *																			*
 * 					This code is to remotely sweep the Matiesse dye laser   *
 *                  over a specified frequency range and for a specified    *
 *				    length of time. 										*
 *                  														*
 *																			*
 * 12/23/2009		Code started											*
 *																			*
 * 12/15/2010       Modified by Jerry Carr Jr.and Earl Scime to include 	*
 *					Sirah_Laser_SetScan_Down for scanning down in 			*
 *					voltage (up in wavelength). Used for the "-1 GHz" 		*
 *					button on the LIF Paramters Panel						*
 *  																		*
 ****************************************************************************/		




#include <stdio.h>
#include <userint.h>
#include <formatio.h>
#include <ansi_c.h>
#include <visa.h>
#include <utility.h>
#include "Bristol.h"
#include "GlobalVariables.h"


#define	 Laser_return_wavemeter_averages 10			//when laser is returning, use large number of averages in control loop
#define	 Wavemeter_Averages 5				//Number of measurement averages to use for wavemeter measurement calls
#define	 speed_o_light 2.99792458E8  

void Sirah_Laser_Scan(char Sirah_COM_String[])
{
	
	/* 	This function assumes that the Sirah Matisse Dye Laser has been properly aligned, that the 
		PID controls have been turned on for the thin and thick etalon, and that the setpoint and
		direction of the scanning mirror scan has been set
		
		Created 	1/2/2010	E. Scime															*/
	
	//ViSession Global_Visa_Session_Handle=0;
	ViSession SIRAH_handle=0;
	ViStatus result;
	ViUInt32 count;
    
    char	rtn_string[128];
    char	send_string[32];

	//Open Visa session 
	//result = viOpenDefaultRM (&Global_Visa_Session_Handle);
	
	//Open Visa session to Sirah Dye Laser
	result=viOpen (Global_Visa_Session_Handle, (ViRsrc)Sirah_COM_String, VI_NULL, VI_NULL, &SIRAH_handle);
	
	//Only try to talk to Sirah laser if port opens successfully
	if (result > -1) {
		
		//Clear any errors
		sprintf (send_string,"ERR:CL\r\n");
		result = viWrite (SIRAH_handle, send_string, (ViUInt32)strlen(send_string), &count);

		//Print scan command into transmission string and send string to laser
		sprintf (send_string,"SCAN:STA RUN\r\n");
		result = viWrite (SIRAH_handle, send_string, (ViUInt32)strlen(send_string), &count);
	
	}
	
	//Close Visa session 
	//result = viClose (Global_Visa_Session_Handle);
	  
}   /*end Sirah_Laser_Scan*/


void Sirah_Laser_SetScan(char Sirah_COM_String[],double Timespan,double Range, double Conversion)
{
	
	/* 	This function assumes that the Sirah Matisse Dye Laser has been properly aligned, that the 
		PID controls have been turned on for the thin and thick etalon, and that the setpoint and
		direction of the scanning mirror scan has been set. It then reads the current scanning mirror
		voltage and calculates an upper limit, sets the system to scan up to the upper limit, and sets the scan
		rate to yield a scan length equal to the global LIF_timespan variable
		
		Created 	2/17/2010	E. Scime															*/
	
	//ViSession Global_Visa_Session_Handle=0;
	ViSession SIRAH_handle=0;
	ViStatus result, comms_result;
	ViUInt32 count;
    
    char			rtn_string[128];
    char			send_string[32];
    char			dummystring[10];
    double			upper_limit, current_value,scan_speed;
    
 	//Open Visa session 
	//result = viOpenDefaultRM (&Global_Visa_Session_Handle);
	
	//Open Visa session to Sirah Dye Laser
	comms_result=viOpen (Global_Visa_Session_Handle, (ViRsrc)Sirah_COM_String, VI_NULL, VI_NULL, &SIRAH_handle);
	
 	//Only try to talk to Sirah laser if port opens successfully
	if (comms_result > -1) {

	//Clear any errors
	sprintf (send_string,"ERR:CL\r\n");
	result = viWrite (SIRAH_handle, send_string, (ViUInt32)strlen(send_string), &count);

		//Read the current scanning mirror value
		sprintf (send_string,"SCAN:NOW?\r\n");
		result = viWrite (SIRAH_handle, send_string, (ViUInt32)strlen(send_string), &count);
		result = viRead (SIRAH_handle, rtn_string, 81, &count);
		Scan(rtn_string,"%s[i*]>%f",10,&current_value);	   //read floating point number after 10th character in string

		//Set the lower limit for the scan to the current scan value
		sprintf (send_string,"SCAN:LLM %f\r\n",current_value);
		result = viWrite (SIRAH_handle, send_string, (ViUInt32)strlen(send_string), &count);

		//Calculate the new upper limit for the scan
		upper_limit = current_value+(Range/Conversion);    //Range in GHz and Conversion in GHz from interface window
		if (upper_limit > 0.69) {    //limit upper value to valid range
			upper_limit = 0.69;
		}
		sprintf (send_string,"SCAN:ULM %f\r\n",upper_limit);
		result = viWrite (SIRAH_handle, send_string, (ViUInt32)strlen(send_string), &count);

		//Set the scanning mirror mode to scan from up current value to upper limit and then stop at upper limit 
		//Information is coded into the bits of the value. 
		//	First bit is up or down (0 or 1)
		//  Second bit is scan stops at lower limit (0 = no, 1 = yes)
		//  Third bit is scan stops at upper limit (0 = no, 1 = yes)
		sprintf (send_string,"SCAN:MODE 4\r\n");			//scan with increasing voltage and stop at upper value
		result = viWrite (SIRAH_handle, send_string, (ViUInt32)strlen(send_string), &count);

		//Set the scanning mirror scan rate to increase frequency so total scan equals data acquisition time
		scan_speed=(upper_limit - current_value)/Timespan;
		sprintf (send_string,"SCAN:RSPD %f\r\n",scan_speed);
		result = viWrite (SIRAH_handle, send_string, (ViUInt32)strlen(send_string), &count);
	}
	
	//Close Visa session 
	//result = viClose (Global_Visa_Session_Handle);
}   /*end Sirah_Laser_Scan*/


void Sirah_Laser_SetScan_Down(char Sirah_COM_String[],double Timespan, double Range, double Conversion)
{
	
	/* 	This function assumes that the Sirah Matisse Dye Laser has been properly aligned, that the 
		PID controls have been turned on for the thin and thick etalon, and that the setpoint and
		direction of the scanning mirror scan has been set. It then reads the current scanning mirror
		voltage and calculates a lower limit, sets the system to scan up to the lower limit, and sets the scan
		rate to yield a scan length equal to the global LIF_timespan variable
		
		Created 	2/17/2010	E. Scime															*/
	
	//ViSession Global_Visa_Session_Handle=0;
	ViSession SIRAH_handle;
	ViStatus result;
	ViUInt32 count;
    
    char			rtn_string[128];
    char			send_string[32];
    char			dummystring[10];
    double			lower_limit, current_value,scan_speed;
    
 	//Open Visa session 
	//result = viOpenDefaultRM (&Global_Visa_Session_Handle);
	
	//Open Visa session to Sirah Dye Laser
	result=viOpen (Global_Visa_Session_Handle, (ViRsrc)Sirah_COM_String, VI_NULL, VI_NULL, &SIRAH_handle);
	
	//Only try to talk to Sirah laser if port opens successfully
	if (result > -1) {

		//Clear any errors
		sprintf (send_string,"ERR:CL\r\n");
		result = viWrite (SIRAH_handle, send_string, (ViUInt32)strlen(send_string), &count);

		//Read the current scanning mirror value
		sprintf (send_string,"SCAN:NOW?\r\n");
		result = viWrite (SIRAH_handle, send_string, (ViUInt32)strlen(send_string), &count);
		result = viRead (SIRAH_handle, rtn_string, 81, &count);
		Scan(rtn_string,"%s[i*]>%f",10,&current_value);	   //read floating point number after 10th character in string

		//Set the upper limit for the scan to the current scan value
		sprintf (send_string,"SCAN:ULM %f\r\n",current_value);
		result = viWrite (SIRAH_handle, send_string, (ViUInt32)strlen(send_string), &count);

		//Calculate the new lower limit for the scan
		lower_limit = current_value-(Range/Conversion);    //Range in GHz and Conversion in GHz from interface window
		if (lower_limit < 0.05) {    //limit lower value to valid range
			lower_limit = 0.05;
		}
		sprintf (send_string,"SCAN:LLM %f\r\n",lower_limit);
		result = viWrite (SIRAH_handle, send_string, (ViUInt32)strlen(send_string), &count);

		//Set the scanning mirror mode to scan from down current value to lower limit and then stop at lower limit 
		//Information is coded into the bits of the value. 
		//	First bit is up or down (0 or 1)
		//  Second bit is scan stops at lower limit (0 = no, 1 = yes)
		//  Third bit is scan stops at upper limit (0 = no, 1 = yes)
		sprintf (send_string,"SCAN:MODE 3\r\n");			//scan with decreasing voltage and stop at lower value
		result = viWrite (SIRAH_handle, send_string, (ViUInt32)strlen(send_string), &count);

		//Set the scanning mirror scan rate to increase frequency so total scan equals data acquisition time
		scan_speed=(current_value - lower_limit)/Timespan;
		sprintf (send_string,"SCAN:FSPD %f\r\n",scan_speed);
		result = viWrite (SIRAH_handle, send_string, (ViUInt32)strlen(send_string), &count);
	}
	
	//Close Visa session 
	//result = viClose (Global_Visa_Session_Handle);
	  
}   /*end Sirah_Laser_Scan*/



void Sirah_Laser_ScanReset(int	Wavemeter_COM, char Sirah_COM_String[], double Range, double Conversion, double Timespan, double Begin_Wavelength, int panel_for_button, int WAVEMETER_STRING)
{
	
	/* 	This function assumes that the Sirah Matisse Dye Laser has been properly aligned, that the 
		PID controls have been turned on for the thin and thick etalon, and the laser has just completed
		a scan and now it needs to return to it original starting value.
		
		Created 	2/17/2010	E. Scime															*/
	
	#define			stop_char "STOP"
	//ViSession Global_Visa_Session_Handle=0;
	ViSession SIRAH_handle;
	ViStatus result;
	ViUInt32 count;
    
    int				poll_value=1;		 //initialize with poll value to zero
    int				loop_count;
    char			rtn_string[128];
    char			send_string[32];
    char			dummystring[10];
	char			poll_char[5];
	char			headerstring[50];
	float			current_Power;
	double			current_Wavelength;
    double			lower_limit, current_value,scan_speed;
	double			difference_Wavelength;
	double			voltage_step;
	double			Current_Frequency;
	double			Target_Frequency;
	double			Frequency_step;
	double			Time_step;
	
	//Reset laser position if a single scan, otherwise set up laser for reverse scan

	//Open Visa session 
	//result = viOpenDefaultRM (&Global_Visa_Session_Handle);
	
	//Open Visa session to Sirah Dye Laser
	result=viOpen (Global_Visa_Session_Handle, (ViRsrc)Sirah_COM_String, VI_NULL, VI_NULL, &SIRAH_handle);
	
	//Only try to talk to Sirah laser if port opens successfully
	if (result > -1) {

		//Clear any errors
		sprintf (send_string,"ERR:CL\r\n");
		result = viWrite (SIRAH_handle, send_string, (ViUInt32)strlen(send_string), &count);

		//Read the current scanning mirror value
		sprintf (send_string,"SCAN:NOW?\r\n");
		result = viWrite (SIRAH_handle, send_string, (ViUInt32)strlen(send_string), &count);
		result = viRead (SIRAH_handle, rtn_string, 81, &count);
		Scan(rtn_string,"%s[i*]>%f",10,&current_value);	   //read floating point number after 10th character in string

	
		//If Sirah controller returns crazy control values because it has an error, try again
		if ((current_value < 0.04) || (current_value > 0.65))  {
	
			//Clear any errors
			sprintf (send_string,"ERR:CL\r\n");
			result = viWrite (SIRAH_handle, send_string, (ViUInt32)strlen(send_string), &count);

			//Close ViSession
			//result = viClose (Global_Visa_Session_Handle);

			//Open Visa session 
			//result = viOpenDefaultRM (&Global_Visa_Session_Handle);

			//Open Visa session to Sirah Dye Laser
			viOpen (Global_Visa_Session_Handle, "USB0::0x17E7::0x0104::09-44-26::INSTR", VI_NULL, VI_NULL, &SIRAH_handle);
	
			//Read current laser position
			sprintf (send_string,"SCAN:NOW?\r\n");
			result = viWrite (SIRAH_handle, send_string, (ViUInt32)strlen(send_string), &count);
			result = viRead (SIRAH_handle, rtn_string, 81, &count);
			Scan(rtn_string,"%s[i*]>%f",10,&current_value);	   //read floating point number after 10th character in string
		}
	
		//Calculate the new lower limit for the scan
		lower_limit = current_value-(Range/Conversion);    	//Range in GHz and Conversion in GHz from interface window
		if (lower_limit < 0.05) lower_limit=0.05;			//Make sure negative control value is not sent to Sirah laser
		sprintf (send_string,"SCAN:LLM %f\r\n",lower_limit);
		result = viWrite (SIRAH_handle, send_string, (ViUInt32)strlen(send_string), &count);

		//Set the upper limit for the scan to the current scan value
		sprintf (send_string,"SCAN:ULM %f\r\n",current_value);
		result = viWrite (SIRAH_handle, send_string, (ViUInt32)strlen(send_string), &count);

		//Set the scanning mirror mode to scan from up current value to upper limit and then stop at lower limit 
		//Information is coded into the bits of the value. 
		//	First bit is up or down (0 or 1)
		//  Second bit is scan stops at lower limit (0 = no, 1 = yes)
		//  Third bit is scan stops at upper limit (0 = no, 1 = yes)
		sprintf (send_string,"SCAN:MODE 3\r\n");			//scan with decreasing voltage and stop at lower value
		result = viWrite (SIRAH_handle, send_string, (ViUInt32)strlen(send_string), &count);

		//Set the scanning mirror scan rate to increase frequency at so total scan equals data acquisition time
		scan_speed=(current_value - lower_limit)/(Range/0.5);		 	//have return sweep happen in 5 s for every 10 GHz, changed from 2 to 0.5, M.G.  
		sprintf (send_string,"SCAN:FSPD %f\r\n",scan_speed);
		result = viWrite (SIRAH_handle, send_string, (ViUInt32)strlen(send_string), &count);

		//Now Sirah to execute reset command
		//Print scan command into transmission string and send string to laser
		sprintf (send_string,"SCAN:STA RUN\r\n");
		result = viWrite (SIRAH_handle, send_string, (ViUInt32)strlen(send_string), &count);
	
		//wait until scan is completed before going onto next operation
		loop_count=0;	//initialize counter variable
		do {
			sprintf (send_string,"SCAN:STA?\r\n");
			result = viWrite (SIRAH_handle, send_string, (ViUInt32)strlen(send_string), &count);
			result = viRead (SIRAH_handle, rtn_string, 81, &count);
			Scan(rtn_string,"%s[i*]>%s[w4]",10,poll_char);	   //read string number after 10th character in string
			poll_value = strcmp (poll_char, stop_char);
			if(KeyHit()) {	//automatically exits if a key is hit
		 		GetKey();
		     	poll_value=0;
			}
			loop_count += 1;
			if (loop_count > 40000) {	 //emergency escape from stuck loop if laser won't return to original control voltage
				poll_value=0;
				//Also force Sirah laser to stop any active scan
				//Print scan command into transmission string and send string to laser
				sprintf (send_string,"SCAN:STA STOP\r\n");
				result = viWrite (SIRAH_handle, send_string, (ViUInt32)strlen(send_string), &count);
			}
		}while(poll_value != 0);   //	end waiting when laser has stopped scanning

	
		// Check to see if the laser has returned to the wavelength at which it started. If not, start
		// loop to get wavelength back to within 0.2 GHz (0.0002 nm), which is the accuracy limit of the wavemeter
		Bristol_Wavelength(Wavemeter_COM, Wavemeter_Averages, &current_Wavelength, &current_Power); 		
	
		//Print Wavelength into Textbox
		sprintf(headerstring, "%f\n",current_Wavelength);
		InsertTextBoxLine(panel_for_button, WAVEMETER_STRING, 0, headerstring);
		ProcessDrawEvents ();

		loop_count=0;	//initialize counter variable
		do {
		
			//Use adaptive method to try and return laser all the way back to original wavelength
			//This process added March 27, 2012
			Current_Frequency=speed_o_light/current_Wavelength;
			Target_Frequency=speed_o_light/Begin_Wavelength;
			Frequency_step = Target_Frequency-Current_Frequency;	
			Time_step = (fabs(Frequency_step)/10.0)*90.0;
			if ( Frequency_step > 0 ) {
				Sirah_Laser_SetScan(Sirah_COM_String,Time_step,Frequency_step, Conversion);
			} else {
				Sirah_Laser_SetScan_Down(Sirah_COM_String,Time_step,fabs(Frequency_step), Conversion);
			}	
	
			//Trigger laser shift
			Sirah_Laser_Scan (Sirah_COM_String);
			Delay(Time_step);  		//wait number of seconds laser should be taking to move to next wavelength
	
	
			Bristol_Wavelength(Wavemeter_COM,Laser_return_wavemeter_averages, &current_Wavelength, &current_Power); 
		
			//Print Wavelength into Textbox
			sprintf(headerstring, "%f\n",current_Wavelength);
			InsertTextBoxLine(panel_for_button, WAVEMETER_STRING, 0, headerstring);
			ProcessDrawEvents ();
		
			//increment loop counter to give automatic exit
			loop_count += 1;
			if (loop_count > 100) {	 //emergency escape from stuck loop if laser won't return to original control voltage
				current_Wavelength = Begin_Wavelength;
				//Also force Sirah laser to stop any active scan
				//Print scan command into transmission string and send string to laser
				sprintf (send_string,"SCAN:STA STOP\r\n");
				result = viWrite (SIRAH_handle, send_string, (ViUInt32)strlen(send_string), &count);
			}

			// if caught in this loop, set the current value to the target to escape
			if(KeyHit()) {	//automatically exits if a key is hit
		 		GetKey();
		     	current_Wavelength = Begin_Wavelength;
			}
	
		} while (fabs(current_Wavelength - Begin_Wavelength) > 0.0006);  //end while loop. For 611.6616, 1 GHz ~ 0.0012 nm, so set return threshold to ~0.5 GHz
 
	
		//Prepare laser for next upward going scan after the reset scan is complete
		//Set the scanning mirror mode to scan from up current value to upper limit and then stop at lower limit 
		//Information is coded into the bits of the value. 
		//	First bit is up or down (0 or 1)
		//  Second bit is scan stops at lower limit (0 = no, 1 = yes)
		//  Third bit is scan stops at upper limit (0 = no, 1 = yes)
		sprintf (send_string,"SCAN:MODE 4\r\n");			//scan with increasing voltage and stop at upper value
		result = viWrite (SIRAH_handle, send_string, (ViUInt32)strlen(send_string), &count);
	
		//Set the scanning mirror scan rate to increase frequency so total scan equals data acquisition time for the next scan
		scan_speed=(current_value - lower_limit)/Timespan;
		sprintf (send_string,"SCAN:RSPD %f\r\n",scan_speed);
		result = viWrite (SIRAH_handle, send_string, (ViUInt32)strlen(send_string), &count);
	}
	
	//Close Visa session 
	//result = viClose (Global_Visa_Session_Handle);
		
}   /*end Sirah_Laser_ScanReset*/



void Sirah_Laser_Stop(char Sirah_COM_String[])
{
	
	/* 	This function assumes that the Sirah Matisse Dye Laser has been properly aligned, that the 
		PID controls have been turned on for the thin and thick etalon, and that the setpoint and
		direction of the scanning mirror scan has been set
		
		Created 	1/2/2010	E. Scime															*/
	
	//ViSession Global_Visa_Session_Handle=0;
	ViSession SIRAH_handle;
	ViStatus result;
	ViUInt32 count;
    
    char	rtn_string[128];
    char	send_string[32];

	//Open Visa session 
	//result = viOpenDefaultRM (&Global_Visa_Session_Handle);
	
	//Open Visa session to Sirah Dye Laser
	result=viOpen (Global_Visa_Session_Handle, (ViRsrc)Sirah_COM_String, VI_NULL, VI_NULL, &SIRAH_handle);
	
	//Only try to talk to Sirah laser if port opens successfully
	if (result > -1) {

		//Clear any errors
		sprintf (send_string,"ERR:CL\r\n");
		result = viWrite (SIRAH_handle, send_string, (ViUInt32)strlen(send_string), &count);

		//Print scan stop command into transmission string and send string to laser
		sprintf (send_string,"SCAN:STA STOP\r\n");
		result = viWrite (SIRAH_handle, send_string, (ViUInt32)strlen(send_string), &count);
	}
	
	//Close Visa session 
	//result = viClose (Global_Visa_Session_Handle);
	  
}   /*end Sirah_Laser_Scan*/


