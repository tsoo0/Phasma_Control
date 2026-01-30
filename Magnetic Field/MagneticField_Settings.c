 /***************************************************************************
 * Code for Modeling Axial Magnetic Field of PHASMA                         *
 *																			*
 * Written by       Earl Scime												*
 *																			*
 * 					This code takes the measured positions of all the coils *
 *					and the currents flowing through them and calculates    *
 *					the on axis magnetic field profile.						*
 *                  														*
 *																			*
 * 12/29/98			Code started											*
 *	6/8/07			Modified to account for four loops of LEIA coils		*
 *					and present positions of the coils						*
 *  7/8/2019        Code modified for the PHASMA experiment                 *
 *	1/22/2025		Code modified to integrate with new PHASMA DAQ code		*
 *  																		*
 ****************************************************************************/		

//General Include Files
#include <analysis.h>
#include <stdio.h>
#include <userint.h>
#include <formatio.h>
#include <ansi_c.h>
#include <utility.h>
#include <visa.h>
#include <rs232.h>
#include <string.h> 
#include "PHASMA-MDS-Routines.h"  
#include "GlobalVariables.h"

//Settings Specific Include Files
#include "MagField_Settings.h"
#include "MagField_Functions.h"


//Calibrations of Magnetic Field System to Convert Current in Supplies to Measured Fields
#define Source_Set_1_Conversion 	5.29	//Calibration from Aug 15, 2019	
#define Source_Set_2_Conversion 	4.92	//Calibration from Aug 15, 2019	
#define PHASMA_Set_1_Conversion 	1.10	//Calibration from Aug 15, 2019	
#define PHASMA_Set_2_Conversion 	1.01	//Calibration from Aug 15, 2019	
#define PHASMA_Set_3_Conversion 	1.05	//Calibration from Aug 15, 2019	
#define PHASMA_Set_4_Conversion 	1.10	//Calibration from Aug 15, 2019	
#define Source_Conversion 			6.23	//Calibration from Aug 15, 2019	
#define PHASMA_Conversion 			1.47	//Calibration from Aug 15, 2019	

//Details of the structure of the PHASMA Coils
#define NumPHASMA 12				//number of sets of coils for PHASMA contribution
#define NumPHASMAcoils 16			//number of loops for one set of PHASMA coils
#define NumPHASMAlayers 2			//number of layers in PHASMA coils
#define PHASMARadius 0.2667   		//the radius of the first PHASMA coil loop
#define PHASMALoopZSpacing .02  	//the spacing (z) of one PHASMA loop to the next
#define PHASMALoopRSpacing .02  	//the spacing (r) of one PHASMA loop to the next

//Details of the structure of the Source Coils  
#define NumSource 10				//number of sets of coils for the source contribution
#define NumSourcecoils 18			//number of current loops for one set of source coils
#define NumSourcelayers 4			//number of layers in source coils
#define SourceRadius 0.1524   		//the radius of the first PHASMA coil loop
#define SourceLoopZSpacing .01  	//the spacing (z) of one PHASMA loop to the next
#define SourceLoopRSpacing .01  	//the spacing (r) of one PHASMA loop to the next


//Parameters and constants used in magnetic field calculations
#define	num_integ_pts 1000			// Number if integration bins for distributions
#define	num_particles 10000			// Number of particles in each distribution
#define	num_bins 100 				// Number of Velocity Bins for Distributions 
#define num_steps 1000              // Sets the number of axial divisions
#define pi 3.141592654				// define pi
#define mu_not 1.25663706144E-6	    // defines value for free space permeability

//Global Variables for settings panel
int			MagField_setup_state = 1;
int			MagField_panel;
int 		Mag_Field_Arm_State=0;


// prototypes for global functions
double 		rf(double x, double y, double z) ;		//post calconeloop prototype 
double 		rd(double x, double y, double z);		// ""
double 		ellf(double phi, double ak);			// ""
double 		elle(double phi, double ak);			// ""this stuff is all needed for the numerical solutions

//Global Variables
double		rho_i[num_steps];
double		flux_tube_1cm[num_steps];
double		flux_tube_2cm[num_steps];
double		flux_tube_5cm[num_steps];
double		position[num_steps];
double		Bz[num_steps];
double		Brho[num_steps];
double		Ti_perp;
double		Ti_para;
double 		S_current1;				//current in source coil
double 		S_current2;				//current in source coil
double 		P_current1;				//current in phasma coil
double 		P_current2;				//current in phasma coil
double 		P_current3;				//current in phasma coil
double 		P_current4;				//current in phasma coil
double 		rho;					//rho component of point where field is considered
double 		z; 						//z component of point where field is considered
double 		BSource1,BSource2;		//rho and z B fields due to source coils, respectively
double 		Bcoup1,Bcoup2;			//B fields due to coupling coils
double 		BPHASMA1,BPHASMA2;		//B fields due to leia
double 		Bgen1;					//these are used in the CalcOneLoop function, many con-
double 		Bgen2;					//tributions must be summed, but the function is general 
double 		PosSource[NumSource];	//array of source coil z-positions measured from end of 80-20 source frame by pump
double 		PosPHASMA[NumPHASMA];	//array of PHASMA coil z-positions





//Communicate with Xantrex power supplies
void setup_xantrex(double current, int powerstate)
{
 int	rounded_current;
 int	result;
 int	Xantrex_COM;
 double supply_current;
 char	xantrex_string[30];
 char	COM_string[30];
 char	dummy[6];

 
  	//Open settings panel in the background
	MagField_panel = LoadPanel (0, "MagField_Settings.uir", MagField);
	RecallPanelState (MagField_panel, "Master_Control_Storage_File", MagField_setup_state);
	
	//Get the COM port of the Xantrex power supply	
  	GetCtrlVal (MagField_panel, MagField_XANTREX_COM,COM_string);

	//Close the panel		
	//DiscardPanel(MagField_panel);
	
	//Read the number out of the COM string in the panel
	sscanf(COM_string,"%3s %d",dummy,&Xantrex_COM);
	
    //Open communications with the device
 	result = OpenComConfig (Xantrex_COM, COM_string, 9600, 0, 8, 1, 512, 512);
 
	 //if open call successful, communicate with power supply
	 if (result == 0) {
		 ComWrt (Xantrex_COM, "SYST:REM:STAT REM\r", 18);		//  set power supply to remote mode
		 Delay (.3);

		 ComWrt (Xantrex_COM, "SOUR:VOLT 59.9\r", 15);		//  set power supply to remote mode
		 Delay (.3);

		  //calculate current needed since two supplies in parallel
		 supply_current=current/2.0;
		 rounded_current=supply_current*1.017;  //add 3% to correct for 3% less current from current control mode
		 if (supply_current < 0) supply_current = 0.0;  //Can't go below zero
		 sprintf (xantrex_string, "SOUR:CURR %f\r", supply_current*1.015); //round to nearest integer value
 
		 ComWrt (Xantrex_COM, xantrex_string, strlen(xantrex_string));		//  set power supply to remote mode
		 Delay (.3);

		 ComWrt (Xantrex_COM, "OUTP ON\r", 8);		//  set power supply to remote mode
		 Delay (.3);

		 //Turn off output if instructed to power down
		 if (!powerstate) {
		 	ComWrt (Xantrex_COM, "OUTP OFF\r", 9);		//  set power supply to remote mode
		 	Delay (.2);
		 }
 
		 ComWrt (Xantrex_COM, "SYST:REM:STAT LOC\r", 18);		//  set power supply to local mode
		 Delay (.3);
		 CloseCom (Xantrex_COM);
 	}

}


//Communicate with Sorensen power supplies
void setup_Sorensen(double current, int supply_num, int powerstate)
{
 	char		sorensen_string[30];
 	char		sorensen_read_string[30];
	char		Port_string[36];
	double		current_value;
	int			j;
	
	ViSession 	Sorensen_handle=0;
	ViStatus 	result=0;
	ViUInt32 	count;
    

	//Open settings panel in the background
	MagField_panel = LoadPanel (0, "MagField_Settings.uir", MagField);
	RecallPanelState (MagField_panel, "Master_Control_Storage_File", MagField_setup_state);

	//Select which supply with which to communicate
	switch (supply_num) {   
 	 	case 1:
			//Get the port address of the appropriate Sorensen power supply	
  			GetCtrlVal (MagField_panel, MagField_Sorenson1_IP,Port_string);

			//Open Visa session to Sorensen power supply
			result=viOpen (Global_Visa_Session_Handle, Port_string, VI_NULL, VI_NULL, &Sorensen_handle);
		break;
 	 	case 2:
			//Get the port address of the appropriate Sorensen power supply	
  			GetCtrlVal (MagField_panel, MagField_Sorenson2_IP,Port_string);

			//Open Visa session to Sorensen power supply
			result=viOpen (Global_Visa_Session_Handle, Port_string, VI_NULL, VI_NULL, &Sorensen_handle);
		break;
 	 	case 3:
			//Get the port address of the appropriate Sorensen power supply	
  			GetCtrlVal (MagField_panel, MagField_Sorenson3_IP,Port_string);

			//Open Visa session to Sorensen power supply
			result=viOpen (Global_Visa_Session_Handle, Port_string, VI_NULL, VI_NULL, &Sorensen_handle);
		break;
 	 	case 4:
			//Get the port address of the appropriate Sorensen power supply	
  			GetCtrlVal (MagField_panel, MagField_Sorenson4_IP,Port_string);

			//Open Visa session to Sorensen power supply
			result=viOpen (Global_Visa_Session_Handle, Port_string, VI_NULL, VI_NULL, &Sorensen_handle);
		break;
 	 	case 5:
			//Get the port address of the appropriate Sorensen power supply	
  			GetCtrlVal (MagField_panel, MagField_Sorenson5_IP,Port_string);

			//Open Visa session to Sorensen power supply
			result=viOpen (Global_Visa_Session_Handle, Port_string, VI_NULL, VI_NULL, &Sorensen_handle);
		break;
 	 	case 6:
			//Get the port address of the appropriate Sorensen power supply	
  			GetCtrlVal (MagField_panel, MagField_Sorenson6_IP,Port_string);

			//Open Visa session to Sorensen power supply
			result=viOpen (Global_Visa_Session_Handle, Port_string, VI_NULL, VI_NULL, &Sorensen_handle);
		break;
	} 

	//Close the panel		
	//DiscardPanel(MagField_panel);

	//Communicate with power supply if open statement successfull
	if (result == 0) {
		//Set max volt so that current is not limited by voltage settings
		if (supply_num < 5) {
			sprintf (sorensen_string, "SOUR:VOLT 60.0\r");   
			result = viWrite (Sorensen_handle, sorensen_string, (unsigned int)strlen(sorensen_string), VI_NULL);
		} else {
			sprintf (sorensen_string, "SOUR:VOLT 70.0\r");   
			result = viWrite (Sorensen_handle, sorensen_string, (unsigned int)strlen(sorensen_string), &count);
		}	
		
		//print requested current into string to send to power supply after setting upper bound for voltage
		sprintf (sorensen_string, "SOUR:CURR %f\r", current); 
		result = viWrite (Sorensen_handle, sorensen_string, (unsigned int)strlen(sorensen_string), &count);
		
		//Delay 1 sec to give supply time to get set up
		Delay(1);
		
		//Read current value of power supply current to confirm that it has been set correctly. If it has not worked after 3 more tries, jump out of loop
		j=0;
		do {
			sprintf (sorensen_string, "SOUR:CURR?\r"); 
			result = viWrite (Sorensen_handle, sorensen_string, (unsigned int)strlen(sorensen_string), &count);
			result = viRead (Sorensen_handle, sorensen_read_string, (unsigned int)strlen(sorensen_string), &count);
			sscanf (sorensen_read_string, "%lf", &current_value);
			if (fabs((current/current_value)-1) < 0.10) {		//check if withing 10%
			   	j = 5; 	//hop out of loop
			   	break;
			} else {
				Delay(.2);
				printf("SORENSEN NOT RESPONDING\n");
				sprintf (sorensen_string, "SOUR:CURR %f\r", current); 
				result = viWrite (Sorensen_handle, sorensen_string, (unsigned int)strlen(sorensen_string), &count);
	   		}
			j=j+1;
		} while (j<5);
	

		//Turn off output if instructed to power down
		if (powerstate) {
			//Turn power supply on
			sprintf (sorensen_string, "OUTP ON\r"); 
			result = viWrite (Sorensen_handle, sorensen_string, (unsigned int)strlen(sorensen_string), &count);	
		} else {
			 sprintf (sorensen_string, "OUTP OFF\r"); 
			 result = viWrite (Sorensen_handle, sorensen_string, (unsigned int)strlen(sorensen_string), &count);
		}

		//Close session to specific power supply since it was opened
		viClose(Sorensen_handle);
	}
}


double ELF(double XK)
/*this was written in fortran by dr. littleton, adapted and arranged here by me
elle and ellf are from numerical recipes, and they are included elsewhere*/

{
	double intrnlELF, f, e;
		
		f=ellf(pi/2,XK);
		e=elle(pi/2,XK);
		intrnlELF = ((2 - XK*XK)* f - 2* e)/(XK*XK);
		
	return(intrnlELF);
}


double ELFPK(double XK)
/*this function was written in fortran by dr. littleton, modified by me*/
{
	double intrnlELFPK;
	double XKP;
	double f;
	double e;
	double elf;
	
		f=ellf(pi/2,XK);
		e=elle(pi/2,XK);
		elf=ELF(XK);
		
		XKP = 1 - XK * XK;
		
		
		intrnlELFPK = ((f-e-(1+XKP)*elf)/XKP);
	return(intrnlELFPK);
}
		


void CalcOneLoop(double I, double zprime, double a, double *Bgen1, double *Bgen2)
/*this function calculates the B field for one general loop.  it's used in the calculations for
each of the contributors to the magnetic field*/

	
{
	double zdiff;			//distance between point of calculation and a coil
	double k;				//argument of elf, elfpk
	double k_squared;
	double con10;			//these are variables to make things unmessy
	double con11;
	double con12;
	double con13;
	double con14;
	double con15;
	double elf;
	double elfpk;
	
	/*these are constants by which elliptic ints are *'d */
	zdiff=z-zprime;
	con10 =  mu_not * I * a / pi;
	con11 = a * a + zdiff * zdiff;
	if (rho < .001)	{
		//this is the analytic, on-axis solution
		*Bgen1 = 0;
		*Bgen2 += (mu_not*I*a*a)/(2*con11*sqrt(con11));		//courtesy of jackson
	}	
	else {
		con12 = con11 + rho * rho + 2 * a * rho;
		con13 = con12 * sqrt( con12 );	
		con14 = con11+a*rho;
		con15 = .5*(con11-rho*rho);
		k_squared = 4*a*rho/con12;
		k = sqrt(k_squared);
		
		elf=ELF(k);
		elfpk=ELFPK(k);
		
		*Bgen1 += (con10*zdiff/con13) * ( elf + elfpk );
    	*Bgen2 += (con10 / ( rho * con13 ) ) * (con14 * elf + con15 * elfpk);
		
	}
	return;
}	


void CalcOnePHASMASet(double zprime, double current)
/*this finds B for one PHASMA magnet*/

{	int turn,layer;
	double a;
	
	

	for (layer =0; layer < NumPHASMAlayers; layer++)	//loop through the number of layers along the z axis
	{
		
		a = PHASMARadius;								//a remains constant for the PHASMA coils
		
		for(turn = 0; turn < NumPHASMAcoils; turn++)   //loop through the number of turns in radius
		{
		
			CalcOneLoop(current, (zprime+layer*PHASMALoopZSpacing), a, &BPHASMA1, &BPHASMA2);  
			a += PHASMALoopRSpacing;
		}
	}
	return;
}


void CalcBPHASMA()
/*this function finds the B field produced by all of the PHASMA magnets */

{	int count;
	double zprime, current=0.0;
	
	for(count = 0; count < NumPHASMA; count++)
	{
		
		zprime = PosPHASMA[count];			//this sets the position of the first loop for a 
											//set of PHASMA coils

		switch (count) {   					//Chose the correct current for that PHASMA coil set
 	 	case 0:
	    	current=P_current1;
		break;
 	 	case 1:
	    	current=P_current1;
		break;
 	 	case 2:
	    	current=P_current1;
		break;
 	 	case 3:
	    	current=P_current2;
		break;
 	 	case 4:
	    	current=P_current2;
		break;
 	 	case 5:
	    	current=P_current3;
		break;
 	 	case 6:
	    	current=P_current3;
		break;
 	 	case 7:
	    	current=P_current4;
		break;
 	 	case 8:
	    	current=P_current4;
		break;
 	 	case 9:
	    	current=P_current4;
		break;
 	 	case 10:
	    	current=P_current4;
		break;
 	 	case 11:
	    	current=P_current4;
		break;
		} 

		CalcOnePHASMASet(zprime, current);
	}
	return;
}


void CalcOneSourceSet(double zprime, double current)
/*this finds B for one Source magnet*/

{	int turn,layer;
	double a;
	
	

	for (layer =0; layer < NumSourcelayers; layer++)	//loop through the number of layers along the z axis
	{
		
		a = SourceRadius;								//a remains constant for the PHASMA coils
		
		for(turn = 0; turn < NumSourcecoils; turn++)   //loop through the number of turns in radius
		{
		
			CalcOneLoop(current, (zprime+layer*SourceLoopZSpacing), a, &BSource1, &BSource2);  
			a += SourceLoopRSpacing;
		}
	}
	return;
}


void CalcBSource()
/*this function finds the B field produced by all of the Source magnets */

{	int count;
	double zprime, current=0.0;
	
	for(count = 0; count < NumSource; count++)
	{
		
		zprime = PosSource[count];			//this sets the position of the first loop for a 
											//set of Source coils

		switch (count) {   					//Chose the correct current for that Source coil set
 	 	case 0:
	    	current=S_current1;
		break;
 	 	case 1:
	    	current=S_current1;
		break;
 	 	case 2:
	    	current=S_current1;
		break;
 	 	case 3:
	    	current=S_current1;
		break;
 	 	case 4:
	    	current=S_current1;
		break;
 	 	case 5:
	    	current=S_current2;
		break;
 	 	case 6:
	    	current=S_current2;
		break;
 	 	case 7:
	    	current=S_current2;
		break;
 	 	case 8:
	    	current=S_current2;
		break;
 	 	case 9:
	    	current=S_current2;
		break;
		} 

		CalcOneSourceSet(zprime, current);
	}
	return;
}




double AddB ( double h, double c)
/*  this function adds the contributions to the magnetic field from the two different sets of 
coils in one general dimension*/
{
	double Bfield;
	
	Bfield = h + c ;
	
	return(Bfield);
}
	


int CVICALLBACK End (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
//  This subroutine ends the program
	switch (event)
	{
		case EVENT_COMMIT:
			   QuitUserInterface (0);

			break;
	}
	return 0;
}


void make_graph(void)
  {
  	  /************************************************************************
  	   *																	  *
  	   *   This loop calculates the magnetic field vector at a given phyisical*
  	   *   location by adding together all the contributions from the 10      *
  	   *   HELIX coils, the 12 PHASMA coils, and the coupling coil.	
  	   *   The required information 
			is:
			1.  the cylindrical coordinates of the point
			2.  the z coordinate of each coil
			3.  the radius of each coil
			4.  the current in each set of coils

			this magnetic field vector will have a rho and z component, 
			but no phi component, as the current  coils are assumed to be 
			coaxial with rho=0.    											  *
  	   ************************************************************************/
	  
	double	Source_1;
	double	Source_2;
	double	PHASMA_1;
	double	PHASMA_2;
	double	PHASMA_3;
	double	PHASMA_4;
	int		j;
	int		result;
	int		bfield_control_flag;



//	The following set of lines get the values from the user window
	GetCtrlVal (MagField_panel, MagField_Scoil1, &PosSource[0]);
	GetCtrlVal (MagField_panel, MagField_Scoil2, &PosSource[1]);
	GetCtrlVal (MagField_panel, MagField_Scoil3, &PosSource[2]);
	GetCtrlVal (MagField_panel, MagField_Scoil4, &PosSource[3]);
	GetCtrlVal (MagField_panel, MagField_Scoil5, &PosSource[4]);
	GetCtrlVal (MagField_panel, MagField_Scoil6, &PosSource[5]);
	GetCtrlVal (MagField_panel, MagField_Scoil7, &PosSource[6]);
	GetCtrlVal (MagField_panel, MagField_Scoil8, &PosSource[7]);
	GetCtrlVal (MagField_panel, MagField_Scoil9, &PosSource[8]);
	GetCtrlVal (MagField_panel, MagField_Scoil10,&PosSource[9]);
	  
// 	Convert positions to meters
	for (j=0;j<NumSource;j++) PosSource[j]=PosSource[j]/100.0;
	
	GetCtrlVal (MagField_panel, MagField_Pcoil1, &PosPHASMA[0]);
	GetCtrlVal (MagField_panel, MagField_Pcoil2, &PosPHASMA[1]);
	GetCtrlVal (MagField_panel, MagField_Pcoil3, &PosPHASMA[2]);
	GetCtrlVal (MagField_panel, MagField_Pcoil4, &PosPHASMA[3]);
	GetCtrlVal (MagField_panel, MagField_Pcoil5, &PosPHASMA[4]);
	GetCtrlVal (MagField_panel, MagField_Pcoil6, &PosPHASMA[5]);
	GetCtrlVal (MagField_panel, MagField_Pcoil7, &PosPHASMA[6]);
	GetCtrlVal (MagField_panel, MagField_Pcoil8, &PosPHASMA[7]);
	GetCtrlVal (MagField_panel, MagField_Pcoil9, &PosPHASMA[8]);
	GetCtrlVal (MagField_panel, MagField_Pcoil10,&PosPHASMA[9]);
	GetCtrlVal (MagField_panel, MagField_Pcoil11,&PosPHASMA[10]);
	GetCtrlVal (MagField_panel, MagField_Pcoil12,&PosPHASMA[11]);
	  
// 	Convert positions to meters
	for (j=0;j<NumPHASMA;j++) PosPHASMA[j]=PosPHASMA[j]/100.0;
	  
// 	Get coil current settings from interface
	GetCtrlVal (MagField_panel, MagField_SOURCE_1, &S_current1);
	GetCtrlVal (MagField_panel, MagField_SOURCE_2, &S_current2);
	GetCtrlVal (MagField_panel, MagField_PHASMA_1, &P_current1);
	GetCtrlVal (MagField_panel, MagField_PHASMA_2, &P_current2);
	GetCtrlVal (MagField_panel, MagField_PHASMA_3, &P_current3);
	GetCtrlVal (MagField_panel, MagField_PHASMA_4, &P_current4);

	//if PHASMA Magnetic Field Control flag is set choose field, update current values based on desired field	
	GetCtrlVal (MagField_panel, MagField_Current_or_Field,&bfield_control_flag);
	if (bfield_control_flag) {		//set current values based on a target field
		
		//Loop through all fields and define the target current
		GetCtrlVal (MagField_panel, MagField_SOURCE_1,&Source_1);
		// Convert target field to current based on calibration
		S_current1=Source_1/Source_Set_1_Conversion;	  
		
		GetCtrlVal (MagField_panel, MagField_SOURCE_2,&Source_2);
		// Convert target field to current based on calibration
		S_current2=Source_2/Source_Set_2_Conversion;

		GetCtrlVal (MagField_panel, MagField_PHASMA_1,&PHASMA_1);
		// Convert target field to current based on calibration
		P_current1=PHASMA_1/PHASMA_Set_1_Conversion;

		GetCtrlVal (MagField_panel, MagField_PHASMA_2,&PHASMA_2);
		// Convert target field to current based on calibration
		P_current2=PHASMA_2/PHASMA_Set_2_Conversion;

		GetCtrlVal (MagField_panel, MagField_PHASMA_3,&PHASMA_3);
		// Convert target field to current based on calibration
		P_current3=PHASMA_3/PHASMA_Set_2_Conversion;

		GetCtrlVal (MagField_panel, MagField_PHASMA_4,&PHASMA_4);
		// Convert target field to current based on calibration
		P_current4=PHASMA_4/PHASMA_Set_2_Conversion;
	}
	else {  //set current values based on a targer current
		//Loop through all the currents. 
		GetCtrlVal (MagField_panel,MagField_SOURCE_1,&Source_1);
		// Convert target field to current based on calibration
		S_current1=Source_1;					
		
		GetCtrlVal (MagField_panel, MagField_SOURCE_2,&Source_2);
		// Convert target field to current based on calibration
		S_current2=Source_2;

		GetCtrlVal (MagField_panel, MagField_PHASMA_1,&PHASMA_1);
		// Convert target field to current based on calibration
		P_current1=PHASMA_1;

		GetCtrlVal (MagField_panel, MagField_PHASMA_2,&PHASMA_2);
		// Convert target field to current based on calibration
		P_current2=PHASMA_2;

		GetCtrlVal (MagField_panel, MagField_PHASMA_3,&PHASMA_3);
		// Convert target field to current based on calibration
		P_current3=PHASMA_3;

		GetCtrlVal (MagField_panel, MagField_PHASMA_4,&PHASMA_4);
		// Convert target field to current based on calibration
		P_current4=PHASMA_4;
	}
	  
//	  Calculate the expected field along the z axis
	  for (j=0;j<450;j++) {		 //step through 4.5 meters of the experiment
	  
	   	z=(float)(j)/100.0;	  //use position in meters in calculation
	   	position[j]=(float)(j);  //keep record of position in cm
	   	
//		At given position, add the fields of all the Source coils	   
	   	Bgen1 = 0;
		Bgen2 = 0;
		BSource1=0.0;
		BSource2=0.0;
		CalcBSource();

//		At given position, add the fields of all the PHASMA coils	   
		Bgen1 = 0;
		Bgen2 = 0;
		BPHASMA1=0.0;
		BPHASMA2=0.0;
		CalcBPHASMA();

	   	Brho[j] = 10000.0*AddB(BSource1, BPHASMA1);  //add fields and convert to Gauss
		Bz[j] = 10000.0*AddB(BSource2, BPHASMA2);	  //add fields and convert to Gauss 
	  }
	  
	  
//	  Graph Source field
	  SetAxisScalingMode (MagField_panel, MagField_MagFieldGraph, VAL_XAXIS, VAL_MANUAL, 0,400);
	  SetAxisScalingMode (MagField_panel, MagField_MagFieldGraph, VAL_LEFT_YAXIS, VAL_MANUAL,0, 1600);
	  PlotXY (MagField_panel, MagField_MagFieldGraph, position, Bz, 200, VAL_DOUBLE, VAL_DOUBLE, VAL_THIN_LINE, VAL_SIMPLE_DOT, VAL_SOLID, 1, VAL_RED);
			  
//	  Graph PHASMA field
	  SetAxisScalingMode (MagField_panel, MagField_MagFieldGraph, VAL_XAXIS, VAL_MANUAL, 0,400);
	  SetAxisScalingMode (MagField_panel, MagField_MagFieldGraph, VAL_LEFT_YAXIS, VAL_MANUAL,0, 1600);
	  PlotXY (MagField_panel, MagField_MagFieldGraph, position, Bz, 450, VAL_DOUBLE, VAL_DOUBLE, VAL_THIN_LINE, VAL_SIMPLE_DOT, VAL_SOLID, 1, VAL_RED);
	
}


int CVICALLBACK run_simulation (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
//  This runs the simulation with the input parameters in the window
	switch (event)
	{
		case EVENT_COMMIT:
 				make_graph();
			break;
	}
	return 0;
}


int CVICALLBACK clear_graph (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
//  This subroutine lets the user clear the graph
	switch (event)
	{
		case EVENT_COMMIT:
				DeleteGraphPlot (panel, MagField_MagFieldGraph, -1, VAL_IMMEDIATE_DRAW);
			break;
	}
	return 0;
}


//This routine takes the settings from the control panel and sends the commands to the power supplies
void MagField_Activate (int MagField_Status, int Master_Mag_Arm_State)
{

	double	Source_1;
	double	Source_2;
	double	PHASMA_1;
	double	PHASMA_2;
	double	PHASMA_3;
	double	PHASMA_4;
	double	Current_1;
	double	Current_2;
	double	Current_3;
	double	Current_4;
	double	Current_5;
	double	Current_6;
	
	int		Current_or_Field;
	//int		MagField_Status=0;
	
 	//Open settings panel in the background
	MagField_panel = LoadPanel (0, "MagField_Settings.uir", MagField);
	RecallPanelState (MagField_panel, "Master_Control_Storage_File", MagField_setup_state);
	
	//Get state of choice for using magnetic field or current to set up power supplies	
  	GetCtrlVal (MagField_panel, MagField_Current_or_Field,&Current_or_Field);

	//Transfer information about state of Master Arm state for magnetic field system
  	Mag_Field_Arm_State = Master_Mag_Arm_State;

	//Run the command to either turn on or turn off the electromagnets
	if (MagField_Status && Mag_Field_Arm_State) {

		if (Current_or_Field) {
			//Loop through all 6 power supplies and set them to the target field. 
			GetCtrlVal (MagField_panel, MagField_SOURCE_1,&Source_1);
			// Convert target field to current based on calibration
			Current_1=Source_1/Source_Set_1_Conversion/2;	  //Divide by 2 since two supplies in parallel
			//Send target current to power supply
			setup_Sorensen(Current_1,5,1);
			setup_Sorensen(Current_1,6,1);
			
			GetCtrlVal (MagField_panel, MagField_SOURCE_2,&Source_2);
			// Convert target field to current based on calibration
			Current_2=Source_2/Source_Set_2_Conversion;
			//Send target current to power supply
			setup_xantrex(Current_2,1);

			GetCtrlVal (MagField_panel, MagField_PHASMA_1,&PHASMA_1);
			// Convert target field to current based on calibration
			Current_3=PHASMA_1/PHASMA_Set_1_Conversion;
			setup_Sorensen(Current_3,1,1);

			GetCtrlVal (MagField_panel, MagField_PHASMA_2,&PHASMA_2);
			// Convert target field to current based on calibration
			Current_4=PHASMA_2/PHASMA_Set_2_Conversion;
			//Send target current to power supply
			setup_Sorensen(Current_4,2,1);
		
			GetCtrlVal (MagField_panel, MagField_PHASMA_3,&PHASMA_3);
			// Convert target field to current based on calibration
			Current_5=PHASMA_3/PHASMA_Set_3_Conversion;
			//Send target current to power supply
			setup_Sorensen(Current_5,3,1);

			GetCtrlVal (MagField_panel, MagField_PHASMA_4,&PHASMA_4);
			// Convert target field to current based on calibration
			Current_6=PHASMA_4/PHASMA_Set_4_Conversion;
			//Send target current to power supply
			setup_Sorensen(Current_6,4,1);
		}
		else {  //set Xantrex and Sorensen to target current instead
	
			//Loop through all 6 power supplies and set them to the target current 
			GetCtrlVal (MagField, MagField_SOURCE_1,&Source_1);
			Current_1=Source_1/2;					//Divide by 2 since supplies in parallel
			//Send target current to power supply
			setup_Sorensen(Current_1,5,1);
			setup_Sorensen(Current_1,6,1);

			GetCtrlVal (MagField, MagField_SOURCE_2,&Source_2);
			Current_2=Source_2;
			//Send target current to power supply
			setup_xantrex(Current_2,1);

			GetCtrlVal (MagField, MagField_PHASMA_1,&PHASMA_1);
			Current_3=PHASMA_1;
			//Send target current to power supply
			setup_Sorensen(Current_3,1,1);
			GetCtrlVal (MagField, MagField_PHASMA_2,&PHASMA_2);
	
			Current_4=PHASMA_2;
			//Send target current to power supply
			setup_Sorensen(Current_4,2,1);
			GetCtrlVal (MagField, MagField_PHASMA_3,&PHASMA_3);
			Current_5=PHASMA_3;
			//Send target current to power supply
			setup_Sorensen(Current_5,3,1);
		
			GetCtrlVal (MagField, MagField_PHASMA_4,&PHASMA_4);
			Current_6=PHASMA_4;
			//Send target current to power supply
			setup_Sorensen(Current_6,4,1);
		} //end setting supplies to field or current
	} 
	else {		//turn off magnet power supplies
		setup_Sorensen(0.0,5,0);
		setup_Sorensen(0.0,6,0);
		setup_xantrex(0.0,0);
		setup_Sorensen(0.0,1,0);
		setup_Sorensen(0.0,2,0);
		setup_Sorensen(0.0,3,0);
		setup_Sorensen(0.0,4,0);
	}
	
	//Close the panel		
	DiscardPanel(MagField_panel);
	return;  
}


//Function that returns the total fields in PHASMA
void MagField_Total (float *Source_bfield, float *PHASMA_bfield)  
{
	int		Current_or_Field;
	double	Source_1;
	double	Source_2;
	double	PHASMA_1;
	double	PHASMA_2;
	double	PHASMA_3;
	double	PHASMA_4;
	double	Current_1;
	double	Current_2;
	double	Current_3;
	double	Current_4;
	double	Current_5;
	double	Current_6;


		MagField_panel = LoadPanel (0, "MagField_Settings.uir", MagField);
		RecallPanelState (MagField_panel, "Master_Control_Storage_File", MagField_setup_state);

		//if PHASMA Magnetic Field Activate switch is turned on and Field Option is selected, update XANTREX and Sorensen supplies control value based on desired field	
  		GetCtrlVal (MagField_panel, MagField_Current_or_Field,&Current_or_Field);

		if (Current_or_Field) {
			GetCtrlVal (MagField_panel, MagField_SOURCE_1,&Source_1);
			// Convert target field to current based on calibration
			Current_1=Source_1/Source_Set_1_Conversion/2;	  //Divide by 2 since two supplies in parallel
				
			GetCtrlVal (MagField_panel, MagField_SOURCE_2,&Source_2);
			// Convert target field to current based on calibration
			Current_2=Source_2/Source_Set_2_Conversion;

			GetCtrlVal (MagField_panel, MagField_PHASMA_1,&PHASMA_1);
			// Convert target field to current based on calibration
			Current_3=PHASMA_1/PHASMA_Set_1_Conversion;
					
			GetCtrlVal (MagField_panel, MagField_PHASMA_2,&PHASMA_2);
			// Convert target field to current based on calibration
			Current_4=PHASMA_2/PHASMA_Set_2_Conversion;

			GetCtrlVal (MagField_panel, MagField_PHASMA_3,&PHASMA_3);
			// Convert target field to current based on calibration
			Current_5=PHASMA_3/PHASMA_Set_3_Conversion;

			GetCtrlVal (MagField_panel, MagField_PHASMA_4,&PHASMA_4);
			// Convert target field to current based on calibration
			Current_6=PHASMA_4/PHASMA_Set_4_Conversion;
		}
		else {  //set Xantrex and Sorensen to target current instead
		
			//Loop through all 6 power supplies and set them to the target current 
			GetCtrlVal (MagField_panel, MagField_SOURCE_1,&Source_1);
			Current_1=Source_1/2;					//Divide by 2 since supplies in parallel

			GetCtrlVal (MagField_panel, MagField_SOURCE_2,&Source_2);
			Current_2=Source_2;

			GetCtrlVal (MagField_panel, MagField_PHASMA_1,&PHASMA_1);
			Current_3=PHASMA_1;

			GetCtrlVal (MagField_panel, MagField_PHASMA_2,&PHASMA_2);
			Current_4=PHASMA_2;

			GetCtrlVal (MagField_panel, MagField_PHASMA_3,&PHASMA_3);
			Current_5=PHASMA_3;

			GetCtrlVal (MagField_panel, MagField_PHASMA_4,&PHASMA_4);
			Current_6=PHASMA_4;

		} //end setting supplies to field or current
	//Calculate total fields in source and PHASMA based on average current in each subsystem
	*Source_bfield=((Current_1*2+Current_2)/2.0)*Source_Conversion;
	*PHASMA_bfield=((Current_3+Current_4+Current_5+Current_6)/4.0)*PHASMA_Conversion;
	
	//Close the panel		
	DiscardPanel(MagField_panel);
}

void OpenMagField_Settings (int	Master_Mag_Arm_State)  
{
	//Open settings panel and load saved values
	MagField_panel = LoadPanel (0, "MagField_Settings.uir", MagField);
	RecallPanelState (MagField_panel, "Master_Control_Storage_File", MagField_setup_state);
	DisplayPanel(MagField_panel);

	Mag_Field_Arm_State = Master_Mag_Arm_State;
	// Start interacting with user
    RunUserInterface ();

}

void Write_MagField(void)
{
	int		j;
	char	outfilename[64];
	FILE*	outfile;

	
	//Open settings panel and load saved values
	MagField_panel = LoadPanel (0, "MagField_Settings.uir", MagField);
	RecallPanelState (MagField_panel, "Master_Control_Storage_File", MagField_setup_state);

	//Calculate field profile
	make_graph();
	
	//Dump results to a file
	sprintf(outfilename, RawDataPath);
	strcat(outfilename, ShotNumberString);
	strcat(outfilename, "_");
	strcat(outfilename, "MagneticField.txt");
	outfile=fopen (outfilename, "w");

	fprintf(outfile,"Z, Bz\n");
	for (j=0;j<600;j++) {
		fprintf(outfile,"%f,%f\n",position[j],Bz[j]);
	}

 	//Close magnetic field profile data file
	fclose (outfile);
	
	//Write data record to MDSPlus database
	//Write_to_PHASMA_MDS(position, Bz,"RAW_DATA.MAGFIELD:MAGPROFILE");

}

int CVICALLBACK Close_MagField_Settings (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			//Save current state of panel and close interface
			SavePanelState (MagField_panel, "Master_Control_Storage_File", MagField_setup_state);
			DiscardPanel(MagField_panel);
			//Write_MagField();
			break;
	}
	return 0;
}

int CVICALLBACK Close_MagField_Settings_Activate (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			//Save current state of panel and close interface
			SavePanelState (MagField_panel, "Master_Control_Storage_File", MagField_setup_state);
			DiscardPanel(MagField_panel);
			
			//Update magnetic field power supplies with new values
			MagField_Activate(1, Mag_Field_Arm_State);
			break;
	}
	return 0;
}


