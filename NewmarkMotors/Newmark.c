

/**********************************************************************************
 //																				  
 //	  Program            :  2D LIF Ti & Ni measurements 						  
 //    Writen By          :  Rick Soulsby, John Kline, Brian Kent				  
 //    Date Alpha version :  7/18/2000											  
 //	  File               :  XYZ_scan.c 											  
 //																				  
 //   This file contains procedures for plotting the various data.  			  
 //																			  
 //   MODIFICATION: Brian Kent, 7/25/2000										  
 //						The file was modified with a function at the end of the   
 //							file to test the input values			  			  
 //							of x and y to see that LIF stage will not enter		  
 //							a region of x < -0.20 and x > 3.2					  
 //																				  
 //	  MODIFICATION: Rick Soulsby, 7/26/2000										  
 //						File modified to actively control position area to avoid  
 //							stage of x < -0.20   and x > 3.2					  
 //	  MODIFICATION: E.E.S. 3/19/2020
 //						Completely rewriten to use 2D and 3D stages in the source
 //						and PHASMA. Also rewritten to acquire data from camera
 //						and pulsed operation.
 //
 //   Modification: E.E.S. 6/19/2025
//						Completely rewritten to integrate with new PHASMA DAQ code.
 //
 *********************************************************************************/



#include <userint.h>  
#include <utility.h>
#include <ansi_c.h>
#include <formatio.h>
#include <rs232.h>  
#include <visa.h>

								//Calibration of 2D stage by K. Stevenson 3/1/2022
#define Xport "COM8"
#define Yport "COM19"
#define Zport "COM21"
#define X3port "COM31" 		  
#define Y3port "COM32" 
#define Z3port "COM33" 

#define	  xsteps_to_center 1.2	//Steps to center for X stage from positive limit switch
#define	  ysteps_to_center 5.33	//Steps to center for Y stage from positive limit switch
#define	  zsteps_to_center 5.0	//Steps to center for Z stage from positive limit switch


int 	Location=1.0;					//Default to source
int 	Mode=1.0;						//Default to steady_state   
int 	Z_mode=1.0;						//Default to z axis scanning   
int		Error_Mode=1.0;					//Default to not having user check each shot
int		Stage_Zeroed_Flag=0;			//Default to stages not zeroed at beginnning of program
int		ThreeD_Stage_Zeroed_Flag=0;		//Default to stages not zeroed at beginnning of program
int		last_steps=0; 					//Storage place for last value of steps moved
int		ThreeD_last_steps=0; 			//Storage place for last value of steps moved
	

void TwoD_move(double, double);  
void ThreeD_move(double, double, double);  



int CVICALLBACK XYZ_Move (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{

	#define Step_to_cm 0.000004

	char	send_string[30]=" ";
	int		result;
	int		count;
	int		X_steps;
	int		Y_steps;
	int		Z_steps;
	float	x_not=0;
	float	y_not=0;
	float	z_not=0;
	
	ViSession Motion_visa_handle;
	ViSession X_stage_handle;
	ViSession Y_stage_handle;
	ViSession Z_stage_handle;
	
	switch (event)
		{
		case EVENT_COMMIT:
			
			//Get target position
			GetCtrlVal (panel, CLIF_PANEL_LIF_X_Target,&X_goal);
			GetCtrlVal (panel, XYZ_PANEL_LIF_Y_Not,&y_not);
			GetCtrlVal (panel, XYZ_PANEL_LIF_Z_Not,&z_not);

			//Get motion system location information from interface. 1=source, 0 = PHASMA
			GetCtrlVal (panel, XYZ_PANEL_Location,&Location);

			//Open Visa session 
			result = viOpenDefaultRM (&Motion_visa_handle);
	
			//Select which drives to talk to based on location
			if (Location) {
				//Open Visa link to X, Y, and Z stages
				result=viOpen (Motion_visa_handle, Xport, VI_NULL, VI_NULL, &X_stage_handle);
				result=viOpen (Motion_visa_handle, Yport, VI_NULL, VI_NULL, &Y_stage_handle);
				result=viOpen (Motion_visa_handle, Zport, VI_NULL, VI_NULL, &Z_stage_handle);
			} else {
				//Open Visa link to X stage
				result=viOpen (Motion_visa_handle, X3port, VI_NULL, VI_NULL, &X_stage_handle);
				result=viOpen (Motion_visa_handle, Y3port, VI_NULL, VI_NULL, &Y_stage_handle);
				result=viOpen (Motion_visa_handle, Z3port, VI_NULL, VI_NULL, &Z_stage_handle);
			}
	
			//Enable the stages
			sprintf (send_string,"DE=1\r\n");
			result = viWrite (X_stage_handle, send_string, strlen(send_string), &count);
			result = viWrite (Y_stage_handle, send_string, strlen(send_string), &count);
			result = viWrite (Z_stage_handle, send_string, strlen(send_string), &count);

			//Calculate move steps
			X_steps=(int)(x_not/Step_to_cm);
			Y_steps=(int)(y_not/Step_to_cm);
			Z_steps=(int)(z_not/Step_to_cm);
	
			//Move x stage with a relative move
			sprintf (send_string,"MR %d\r\n",X_steps);
			result = viWrite (X_stage_handle, send_string, strlen(send_string), &count);
 	
			//Move y stage with a relative move
			sprintf (send_string,"MR %d\r\n",Y_steps);
			result = viWrite (Y_stage_handle, send_string, strlen(send_string), &count);
	
			//Move z stage with a relative move if in that mode
			sprintf (send_string,"MR %d\r\n",Z_steps);
			result = viWrite (Z_stage_handle, send_string, strlen(send_string), &count);
	
			//Hold progam from advancing while stage moves based on largest travel
			if ((fabs(X_steps) > fabs(Y_steps)) && (fabs(X_steps) > fabs(Z_steps))) {
				Delay(fabs((X_steps-last_steps)*Step_to_cm/2));
				last_steps=X_steps;
			} else {
				if (fabs(Y_steps) > fabs(Z_steps)) {
					Delay(fabs((Y_steps-last_steps)*Step_to_cm/2));
					last_steps=Y_steps;
				} else {
					Delay(fabs((Z_steps-last_steps)*Step_to_cm/2));
					last_steps=Z_steps;
				}	
			}

			//Close the ports to each motion drive
			result=viClose (X_stage_handle);
			result=viClose (Y_stage_handle);
			result=viClose (Z_stage_handle);

			//Close Visa session 
			result = viClose (Motion_visa_handle);
			
			break;
		}
	return 0;
}
