#include <utility.h>
#include <ansi_c.h>
#include <rs232.h>
#include <visa.h>    
#include <userint.h>
#include "GlobalVariables.h"
#include "PHASMA-MDS-Routines.h"
#include "PlasmaGunSystem.h"
#include "PlasmaGunSettings.h"
#include "PHASMA_C_to_Python_convertor.h"

//Global Variables for settings panel
int			PlasmaGun_setup_state = 5;
int			PlasmaGun_panel;
int			Gun1_Bias_Port;
int			Gun2_Bias_Port;

char		Gun1_Bias_COM[30];
char		Gun2_Bias_COM[30];


void OpenPlasmaGun_Settings (void)  
{
	float gun_pressure;
	
	//Open settings panel and load saved values
	PlasmaGun_panel = LoadPanel (0, "PlasmaGunSettings.uir", PlasmaGun);
	RecallPanelState (PlasmaGun_panel, "Master_Control_Storage_File", PlasmaGun_setup_state);
	DisplayPanel(PlasmaGun_panel);

	gun_pressure=ReadGunPressure();
	SetCtrlVal (PlasmaGun_panel, PlasmaGun_GunPressure,gun_pressure);

	// Start interacting with user
    RunUserInterface ();

}

int CVICALLBACK Close_PlasmaGun_Settings (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	float 	target_gun_pressure;
	
	switch (event)
		{
			case EVENT_COMMIT:
				SavePanelState (PlasmaGun_panel, "Master_Control_Storage_File", PlasmaGun_setup_state);
				
				//Read the target gas bottle regulator pressure from window and set regulator to that value
				GetCtrlVal (PlasmaGun_panel, PlasmaGun_GunPressure_Target,&target_gun_pressure);
				SetGunPressure(target_gun_pressure);
								
				DiscardPanel(panel);
				break;
		}
	return 0;
}




//Send voltages to bias power supplies (no arc control yet) and command output state as instructed from master code
void PlasmaGun_ActivateSystem (int Guns_Power_State)  
{
	int		result;
	int		count=0;
	int		Gun1_Power_State;
	int		Gun2_Power_State;
	int 	Gun1_Bias_handle;
	int 	Gun2_Bias_handle;
	float	Gun1_Bias_Volts;
	float	Gun2_Bias_Volts;
	float	Gun1_Arc_Volts;
	float	Gun2_Arc_Volts;
	float	Gun1_Bias_Amps;
	float	Gun2_Bias_Amps;
	float	Gun1_Arc_Amps;
	float	Gun2_Arc_Amps;
	char	dummy[6]="NULL";
	char	PlasmaGun_string[36]="NULL";

	
	//Open settings panel and load saved values
	PlasmaGun_panel = LoadPanel (0, "PlasmaGunSettings.uir", PlasmaGun);
	RecallPanelState (PlasmaGun_panel, "Master_Control_Storage_File", PlasmaGun_setup_state);

	//Get the on/off state for both guns
	GetCtrlVal (PlasmaGun_panel, PlasmaGun_Gun1_Power_State,&Gun1_Power_State);
	GetCtrlVal (PlasmaGun_panel, PlasmaGun_Gun2_Power_State,&Gun2_Power_State);

	//Get all the gun setttings from the front panel
	GetCtrlVal (PlasmaGun_panel, PlasmaGun_Gun1_Bias_Volts,&Gun1_Bias_Volts);
	GetCtrlVal (PlasmaGun_panel, PlasmaGun_Gun2_Bias_Volts,&Gun2_Bias_Volts);
	GetCtrlVal (PlasmaGun_panel, PlasmaGun_Gun1_Arc_Volts,&Gun1_Arc_Volts);
	GetCtrlVal (PlasmaGun_panel, PlasmaGun_Gun2_Arc_Volts,&Gun2_Arc_Volts);
	GetCtrlVal (PlasmaGun_panel, PlasmaGun_Gun1_Bias_Amps,&Gun1_Bias_Amps);
	GetCtrlVal (PlasmaGun_panel, PlasmaGun_Gun2_Bias_Amps,&Gun2_Bias_Amps);
	GetCtrlVal (PlasmaGun_panel, PlasmaGun_Gun1_Arc_Amps,&Gun1_Arc_Amps);
	GetCtrlVal (PlasmaGun_panel, PlasmaGun_Gun2_Arc_Amps,&Gun2_Arc_Amps);

	//Gun 1 Bias
	//Get Gun1 Bias COM port string from settings panel
 	GetCtrlVal (PlasmaGun_panel, PlasmaGun_Gun1_Bias_COM,Gun1_Bias_COM);
	
	//Open communication to power supply using Visa calls
	result=viOpen (Global_Visa_Session_Handle, Gun1_Bias_COM, VI_NULL, VI_NULL, &Gun1_Bias_handle);
	
	//return without doing anything else if open command fails
	if (result >= 0) {

		//Set local address
		sprintf (PlasmaGun_string, "*ADR 20\r");   
		result = viWrite (Gun1_Bias_handle, PlasmaGun_string, (unsigned int)strlen(PlasmaGun_string), &count);
	
		//Let instrument catch up
		Delay (0.1);
		
		//Do IDN query
		//sprintf (PlasmaGun_string, "*IDN?\r");   
		//result = viWrite (Gun1_Bias_handle, PlasmaGun_string, (unsigned int)strlen(PlasmaGun_string), &count);
		//result = viRead (Gun1_Bias_handle, PlasmaGun_string, 36, &count);
		
		//Set overvoltage protection level
		sprintf (PlasmaGun_string, "SOURCE:VOLT:PROT 650\r");   
		result = viWrite (Gun1_Bias_handle, PlasmaGun_string, (unsigned int)strlen(PlasmaGun_string), &count);
		
		//Let instrument catch up
		Delay (0.1);

		//Set output state of Gun1
		if ((Guns_Power_State) && (Gun1_Power_State) ){
			
			//Set the requested voltage level
			sprintf (PlasmaGun_string, "SOURCE:VOLT %f\r",Gun1_Bias_Volts); 	
			result = viWrite (Gun1_Bias_handle, PlasmaGun_string, (unsigned int)strlen(PlasmaGun_string), &count);

			//Let instrument catch up
			Delay (0.1);
		
			//Set the requested current level
			sprintf (PlasmaGun_string, "SOURCE:CURR %f\r",Gun1_Bias_Amps); 	 
			result = viWrite (Gun1_Bias_handle, PlasmaGun_string, (unsigned int)strlen(PlasmaGun_string), &count);

			//Let instrument catch up
			Delay (0.1);
	
			sprintf (PlasmaGun_string, "OUTPUT:POWER:STATE ON\r");   
			result = viWrite (Gun1_Bias_handle, PlasmaGun_string, (unsigned int)strlen(PlasmaGun_string), &count);
		} else {
			//Set the requested voltage level
			sprintf (PlasmaGun_string, "SOURCE:VOLT 0.0\r"); 	
			result = viWrite (Gun1_Bias_handle, PlasmaGun_string, (unsigned int)strlen(PlasmaGun_string), &count);

			//Let instrument catch up
			Delay (0.1);

			sprintf (PlasmaGun_string, "OUTPUT:POWER:STATE OFF\r");   
			result = viWrite (Gun1_Bias_handle, PlasmaGun_string, (unsigned int)strlen(PlasmaGun_string), &count);
		}

		//Close COM port
		result = viClose(Gun1_Bias_handle);
	}
	
	//Gun 2 Bias
	//Get Gun2 Bias COM port string from settings panel
	GetCtrlVal (PlasmaGun_panel, PlasmaGun_Gun2_Bias_COM,Gun2_Bias_COM);
	
	//Open communication to power supply using Visa calls
	result=viOpen (Global_Visa_Session_Handle, Gun2_Bias_COM, VI_NULL, VI_NULL, &Gun2_Bias_handle);
	
	//return without doing anything else if open command fails
	if (result >= 0) {

		//Set local address
		sprintf (PlasmaGun_string, "*ADR 20\r");   
		result = viWrite (Gun2_Bias_handle, PlasmaGun_string, (unsigned int)strlen(PlasmaGun_string), &count);
		
		//Let instrument catch up
		Delay (0.1);
	
		//Do IDN query
		//sprintf (PlasmaGun_string, "*IDN?\r");   
		//result = viWrite (Gun2_Bias_handle, PlasmaGun_string, (unsigned int)strlen(PlasmaGun_string), &count);
		//result = viRead (Gun2_Bias_handle, PlasmaGun_string, 36, &count);
		
		//Set overvoltage protection level
		sprintf (PlasmaGun_string, "SOURCE:VOLT:PROT 650\r");   
		result = viWrite (Gun2_Bias_handle, PlasmaGun_string, (unsigned int)strlen(PlasmaGun_string), &count);
		
		//Let instrument catch up
		Delay (0.1);

		//Set output state of Gun2
		if ((Guns_Power_State) && (Gun2_Power_State) ){
			
			//Set the requested voltage level
			sprintf (PlasmaGun_string, "SOURCE:VOLT %f\r",Gun2_Bias_Volts); 	
			result = viWrite (Gun2_Bias_handle, PlasmaGun_string, (unsigned int)strlen(PlasmaGun_string), &count);

			//Let instrument catch up
			Delay (0.1);
		
			//Set the requested current level
			sprintf (PlasmaGun_string, "SOURCE:CURR %f\r",Gun2_Bias_Amps); 	 
			result = viWrite (Gun2_Bias_handle, PlasmaGun_string, (unsigned int)strlen(PlasmaGun_string), &count);

			//Let instrument catch up
			Delay (0.1);
	
			sprintf (PlasmaGun_string, "OUTPUT:POWER:STATE ON\r");   
			result = viWrite (Gun2_Bias_handle, PlasmaGun_string, (unsigned int)strlen(PlasmaGun_string), &count);
		} else {
			//Set the requested voltage level
			sprintf (PlasmaGun_string, "SOURCE:VOLT 0.0\r"); 	
			result = viWrite (Gun2_Bias_handle, PlasmaGun_string, (unsigned int)strlen(PlasmaGun_string), &count);

			//Let instrument catch up
			Delay (0.1);

			sprintf (PlasmaGun_string, "OUTPUT:POWER:STATE OFF\r");   
			result = viWrite (Gun2_Bias_handle, PlasmaGun_string, (unsigned int)strlen(PlasmaGun_string), &count);
		}

		//Close COM port
		result = viClose(Gun2_Bias_handle);
	}
	
	//Discard panel
	DiscardPanel(PlasmaGun_panel); 
}


//Upload to the MDS database and a text file, all the parameters of the plasma guns
void Write_PlasmaGunData(void)
{
	double	Dummy_array[12];
	double	GunSystem_Parameters[10];


	//Read all the current values from the control system and write into an array that is then
	//written to the MDS database
	
	//Open settings panel and load saved values
	PlasmaGun_panel = LoadPanel (0, "PlasmaGunSettings.uir", PlasmaGun);
	RecallPanelState (PlasmaGun_panel, "Master_Control_Storage_File", PlasmaGun_setup_state);

	//Get all the delays from the front panel
	GetCtrlVal (PlasmaGun_panel, PlasmaGun_Gun1_Bias_Volts,&GunSystem_Parameters[0]);
	GetCtrlVal (PlasmaGun_panel, PlasmaGun_Gun2_Bias_Volts,&GunSystem_Parameters[1]);
	GetCtrlVal (PlasmaGun_panel, PlasmaGun_Gun1_Arc_Volts,&GunSystem_Parameters[2]);
	GetCtrlVal (PlasmaGun_panel, PlasmaGun_Gun2_Arc_Volts,&GunSystem_Parameters[3]);
	GetCtrlVal (PlasmaGun_panel, PlasmaGun_Gun1_Bias_Amps,&GunSystem_Parameters[4]);
	GetCtrlVal (PlasmaGun_panel, PlasmaGun_Gun2_Bias_Amps,&GunSystem_Parameters[5]);
	GetCtrlVal (PlasmaGun_panel, PlasmaGun_Gun1_Arc_Amps,&GunSystem_Parameters[6]);
	GetCtrlVal (PlasmaGun_panel, PlasmaGun_Gun2_Arc_Amps,&GunSystem_Parameters[7]);
	
	//Write to the MDS database a dummy array for timebase and the system parameters to reuse the standard
	//timeseries writting format for the MDS routine
	Write_to_PHASMA_MDS(Dummy_array,GunSystem_Parameters,"RAW_DATA.PLASMAGUN:GUNPARAMS");
   	
	//Close Panel
	DiscardPanel(PlasmaGun_panel);

}

// Open communications with the plasma gun gas bottle regulator and read the pressure
// Code needs to be fixed to actually communicate with regulator
float ReadGunPressure(void)
{
 	char		Port_string[36]; 
	char		Regulator_string[32]="Null";
	int			result;
	int 		count;
	float		regulator_pressure;
	
	ViSession 	Regulator_handle;

	//Get the port address of the gas bottle regulator	
  	GetCtrlVal (PlasmaGun_panel, PlasmaGun_Regulator_IP,Port_string);
	
	//Open communicatin to pressure gauge
	result=viOpen (Global_Visa_Session_Handle, Port_string, VI_NULL, VI_NULL, &Regulator_handle);

	//Ask for regulator pressure
	sprintf (Regulator_string, "RP\r");   
	result = viWrite (Regulator_handle, Regulator_string, (unsigned int)strlen(Regulator_string), &count);

	//Read regulator pressure
	result = viRead (Regulator_handle, Regulator_string, 8 ,&count);
	sscanf(Regulator_string,"%f",&regulator_pressure);
	
	//Close communicatin to pressure gauge
	result=viClose (Regulator_handle);

	return(regulator_pressure);	
}

// Open communications with the plasma gun gas bottle regulator and set the pressure
// Code needs to be fixed to actually communicate with regulator
void SetGunPressure(float target_pressure)
{
 	char		Port_string[36]; 
	char		Regulator_string[32]="Null";
	int			result;
	int 		count;
	
	ViSession 	Regulator_handle;

	//Get the port address of the gas bottle regulator	
  	GetCtrlVal (PlasmaGun_panel, PlasmaGun_Regulator_IP,Port_string);

	//Open communicatin to pressure gauge
	result=viOpen (Global_Visa_Session_Handle, Port_string, VI_NULL, VI_NULL, &Regulator_handle);

	//Set regulator pressure
	sprintf (Regulator_string, "SP%f\r", target_pressure);   
	result = viWrite (Regulator_handle, Regulator_string, (unsigned int)strlen(Regulator_string), &count);

	//Close communicatin to pressure gauge
	result=viClose (Regulator_handle);
}
	

int CVICALLBACK Guns_Move (int panel, int control, int event,
						   void *callbackData, int eventData1, int eventData2)
{
	float 	Gun_1_R;
	float 	Gun_1_Offset;
	float  	Gun_1_COUNTSPERMM;
	float 	Gun_2_R;
	float 	Gun_2_Offset;
	float  	Gun_2_COUNTSPERMM;
	char 	Gun_1_IP_Address[64];
	char 	Gun_2_IP_Address[64];
	
	switch (event)
	{
		case EVENT_COMMIT:
			//Get current position of probe, starting position, end position, and what to do at end of scan range
			GetCtrlVal (PlasmaGun_panel, PlasmaGun_Gun1_R,&Gun_1_R);
			GetCtrlVal (PlasmaGun_panel, PlasmaGun_Gun2_R,&Gun_2_R);
			GetCtrlVal (PlasmaGun_panel, PlasmaGun_Gun1_Offset,&Gun_1_Offset);
			GetCtrlVal (PlasmaGun_panel, PlasmaGun_Gun2_Offset,&Gun_2_Offset);
			GetCtrlVal (PlasmaGun_panel, PlasmaGun_Gun1_IP,Gun_1_IP_Address);
			GetCtrlVal (PlasmaGun_panel, PlasmaGun_Gun2_IP,Gun_2_IP_Address);
			GetCtrlVal (PlasmaGun_panel, PlasmaGun_Gun1_COUNTSPERMM,&Gun_1_COUNTSPERMM);
			GetCtrlVal (PlasmaGun_panel, PlasmaGun_Gun2_COUNTSPERMM,&Gun_2_COUNTSPERMM);
			
			//Send move command to probe motor
			move_probe(Gun_1_IP_Address, (Gun_1_Offset-Gun_1_R)*10.0, Gun_1_COUNTSPERMM, 100.0);	//move gun with max insertation allowed of 100 mm
			move_probe(Gun_2_IP_Address, (Gun_2_Offset-Gun_2_R)*10.0, Gun_2_COUNTSPERMM, 100.0);	//move gun with max insertation allowed of 100 mm
			
			break;
	}
	return 0;
}

int CVICALLBACK Gun_Home (int panel, int control, int event,
							   void *callbackData, int eventData1, int eventData2)
{
	char 	Gun_1_IP_Address[64];
	char 	Gun_2_IP_Address[64];

	switch (event)
	{
		case EVENT_COMMIT:
			//Get current position of probe, starting position, end position, and what to do at end of scan range
			GetCtrlVal (PlasmaGun_panel, PlasmaGun_Gun1_IP,Gun_1_IP_Address);
			GetCtrlVal (PlasmaGun_panel, PlasmaGun_Gun2_IP,Gun_2_IP_Address);
			
			//Send move command to probe motor
			home_probe(Gun_1_IP_Address);
			home_probe(Gun_2_IP_Address);
			
			break;
	}
	return 0;
}

void GunPosition(float *Gun1_Position, float *Gun2_Position)
{
 	float 	Gun_1_R;
	float 	Gun_2_R;
	
	
	//Open settings panel in the background
	PlasmaGun_panel = LoadPanel (0, "PlasmaGunSettings.uir", PlasmaGun);
	RecallPanelState (PlasmaGun_panel, "Master_Control_Storage_File", PlasmaGun_setup_state);

	//Get the gun positions	
 	GetCtrlVal (PlasmaGun_panel, PlasmaGun_Gun1_R,&Gun_1_R);
	GetCtrlVal (PlasmaGun_panel, PlasmaGun_Gun2_R,&Gun_2_R);

	//return gun position values
	*Gun1_Position=Gun_1_R;
	*Gun2_Position=Gun_2_R;
	
}

