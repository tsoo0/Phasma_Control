#include 	<utility.h>
#include 	<ansi_c.h>
#include	<visa.h>
#include	<stdlib.h>
#include 	<formatio.h>
#include	"PHASMA-MDS-Routines.h"
#include 	"QBSSystems.h"
#include 	"QuantumBeatSettings.h"  
#include	"GlobalVariables.h"
#include	"Lecroy.h"

#define X3port "COM31" 		  
#define Y3port "COM32" 
#define Z3port "COM33" 

#define	  xsteps_to_center 1.2	//Steps to center for X stage from positive limit switch
#define	  ysteps_to_center 5.33	//Steps to center for Y stage from positive limit switch
#define	  zsteps_to_center 5.0	//Steps to center for Z stage from positive limit switch


int		QBS_panel;
int		QBS_setup_state = 7001;


//Set up the oscilloscopes
void QBSActivate(void)
{
	int 		result; 					//	Error-checking variable
	char		QBS_string[32]="Null";
	char		IP_Address_string[64];  
	ViUInt32 	count;

	//Open up Temp Diagnostic settings panel and get usage of either Rigol or LeCroy scope
	QBS_panel = LoadPanel (0, "QuantumBeatSettings.uir", QBS);
	RecallPanelState (QBS_panel, "Master_Control_Storage_File", QBS_setup_state);
	
	//Discard housekeeping panel
	DiscardPanel(QBS_panel);
	
}


//Arm the VX4244 digitizer
int QBSArm(void)
{
	int		result=0;
	int		count;
	char	QBS_string[16]="Null";
	
	//	Arm Rigol scope if in use
	
	return result;
}




//Take the acquired data from the instrument and print it to the common data folder and 
//also push it to the MDS database
void Write_QBSData(void)
{


}

//Launch this routine as window opens and it updates the window with the latest values from the 
//control system as it loads.
void OpenQBS_Settings (void)  
{
	//Open settings panel and load saved values
	QBS_panel = LoadPanel (0, "QuantumBeatSettings.uir", QBS);
	SavePanelState (QBS_panel, "Master_Control_Storage_File", QBS_setup_state);
	DisplayPanel(QBS_panel);
	
	//Update panel with 

	
	// Start interacting with user
    RunUserInterface ();

}

int CVICALLBACK Close_QBS_Settings (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			//Save current state of panel and close interface
			SavePanelState (QBS_panel, "Master_Control_Storage_File", QBS_setup_state);
			DiscardPanel(panel);
			break;
	}
	return 0;
}

int CVICALLBACK QBS_XYZ_Move (int panel, int control, int event,
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
	int		last_steps=0; 					//Storage place for last value of steps moved
	char	QBS_Xcom[30];
	char	QBS_Ycom[30];
	char	QBS_Zcom[30];
	
	ViSession X_stage_handle;
	ViSession Y_stage_handle;
	ViSession Z_stage_handle;
	
	switch (event)
		{
		case EVENT_COMMIT:
		
			//Get COM ports
			GetCtrlVal (QBS_panel, QBS_X_COM,QBS_Xcom);
			GetCtrlVal (QBS_panel, QBS_Y_COM,QBS_Ycom);
			GetCtrlVal (QBS_panel, QBS_Z_COM,QBS_Zcom);

			//Get target position
			GetCtrlVal (QBS_panel, QBS_X_Target,&x_not);
			GetCtrlVal (QBS_panel, QBS_Y_Target,&y_not);
			GetCtrlVal (QBS_panel, QBS_Z_Target,&z_not);

	
			//Open Visa session 
			//result = viOpenDefaultRM (&Motion_visa_handle);
	
			//Open Visa link to XYZ stages
			result=viOpen (Global_Visa_Session_Handle, QBS_Xcom, VI_NULL, VI_NULL, &X_stage_handle);
			result=viOpen (Global_Visa_Session_Handle, QBS_Ycom, VI_NULL, VI_NULL, &Y_stage_handle);
			result=viOpen (Global_Visa_Session_Handle, QBS_Zcom, VI_NULL, VI_NULL, &Z_stage_handle);
			
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

			
			break;
		}
	return 0;
}


