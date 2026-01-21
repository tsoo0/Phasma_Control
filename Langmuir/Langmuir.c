/********************************************************************
	This file includes everything necessary for Langmuir measurements.
	The functions in this program are 
		open_LP_file
		zero_LP_arrays
		LP_acquire
		langmuir_analyze
		write_LP_data
		
		
	11/29/00  jk  Currently there is an offset included for voltage axis.  This allows 
				  the keithley to be sweeps from 0 to 70 volts while the langmuir probe
	              is sweeps from -50 to 30 volts.  This eliminates the error in the 
	              keithley when switching from negative to positive source voltages.
*********************************************************************/

#include 	<toolbox.h>
#include 	<utility.h>
#include 	<analysis.h>
#include 	<ansi_c.h>
#include 	<visa.h>
#include 	<userint.h>
#include 	<formatio.h>
#include 	<gpib.h>
#include 	<stdio.h>
#include 	<NIDAQmx.h>
#include	"GlobalVariables.h"
#include	"LangmuirSettings.h"
#include	"LangmuirSystems.h"
#include	"PHASMA-MDS-Routines.h"  

#define		pi 					3.141593 
#define		LP_recordlength   	1000
#define		LP_fit_pts 			50


int		Langmuir_panel=0;
int 	Langmuir_setup_state=20; 
int		Keithley1_port;
int		Keithley2_port;
int		Keithley_Source;
int		Keithley_PHASMA;
int		Langmuir_probe_num;
int		Lang_DataFlag;
int		Planar_Flag;
int		shotnum;
double  LP_voltageS[LP_recordlength];
double  LP_fit_voltageS[LP_recordlength];
double  LP_timeS[LP_recordlength];
double  LP_currentS[LP_recordlength];
double  LP_fit_currentS[LP_recordlength];
double  LP_voltageP[LP_recordlength];
double  LP_fit_voltageP[LP_recordlength];
double  LP_currentP[LP_recordlength];
double  LP_fit_currentP[LP_recordlength];
double  LP_timeP[LP_recordlength];
double  current_norm[LP_recordlength];
double	LP_fit_current[LP_fit_pts];
double	abs_current[LP_recordlength];
double	temp_current[LP_fit_pts];
double	LP_fit_voltage[LP_fit_pts];
double 	battery;
double	PlanarProbe_Angle;
double	PlanarProbe_Area;
double	Source_tiplength;
double	PHASMA_tiplength;
double	Source_tipradius;
double	PHASMA_tipradius;
double	Source_Floating;
double	PHASMA_Floating;
double	Source_Te;
double	PHASMA_Te;
double	Source_Ne;
double	PHASMA_Ne;
	
//Set up the Sourcemeter parameters
void set_keithley(int gpib_num, double min_volts, double max_volts, double max_current)
{
	double	step_volts;

	// This function changes the setting on the keithley      
       char string[80];
	
			//Communicate with instrument if opened properly
            if (gpib_num > 0) {
			   ibwrt (gpib_num, "*RST; *CLS",10 );
		       ibwrt (gpib_num, "*ESE 1; *SRE 32",15 );
	//	       ibwrt (gpib_num, "ROUT:TERM REAR", 15);
		       ibwrt (gpib_num, "SENS:FUNC:ON 'CURR:DC'",22 );
		       ibwrt (gpib_num, "SOUR:FUNC:MODE VOLT",19 );
		       ibwrt (gpib_num, "SOUR:SWE:SPAC LIN",17 );
	//	       ibwrt (gpib_num, "SOUR:DEL 0.1",13);
		       ibwrt (gpib_num, "SOUR:VOLT:STAR -50.0",20);
		       ibwrt (gpib_num, "SOUR:VOLT:STOP 50.1",19);
		       ibwrt (gpib_num, "SOUR:VOLT:STEP 0.1",18);
		       ibwrt (gpib_num, "SENS:VOLT:NPLC 0.1",19);
		       ibwrt (gpib_num, "SOUR:VOLT:RANG:AUTO OFF",23); 
		       ibwrt (gpib_num, "SOUR:VOLT:RANG 200",19);
		       ibwrt (gpib_num, "SOUR:SWE:RANG FIX",18);
		       ibwrt (gpib_num, "SENS:CURR:PROT:LEV 1.00000",24);
		       ibwrt (gpib_num, "SENS:CURR:RANG:AUTO OFF",23);
		       ibwrt (gpib_num, "SENS:CURR:RANG:UPP 0.10000",26);
		       ibwrt (gpib_num, "SENS:AVER:TCON MOV",18);//8
		       ibwrt (gpib_num, "SENS:AVER:STAT ON",17);//8
		       ibwrt (gpib_num, "SOUR:DEL:AUTO OFF",17);
		       ibwrt (gpib_num, "ARM:COUN 1", 10);
		       ibwrt (gpib_num, "FORM:DATA ASC",13);
		       ibwrt (gpib_num, "FORM:ELEM VOLT,CURR",19);
		       ibwrt (gpib_num, "TRIG:COUN 1001", 14);
		       ibwrt (gpib_num, "SOUR:VOLT:MODE SWE",18);
		       ibwrt (gpib_num, "OUTP ON", 7);
		       ibwrt (gpib_num, "SOUR:DEL 0.01",12);   //8
        
       
			   sprintf (string, "SOUR:VOLT:STAR %4.1f",min_volts);
			   ibwrt (gpib_num, string,strlen(string));
	   
			   sprintf (string, "SOUR:VOLT:STOP %4.1f",max_volts);
			   ibwrt (gpib_num, string,strlen(string));
       
		       step_volts = (max_volts - min_volts)/ LP_recordlength;
		       sprintf (string, "SOUR:VOLT:STEP %6.4f",step_volts);
		       ibwrt (gpib_num, string,strlen(string));
       
		       sprintf (string, "SENS:CURR:RANG:UPP %7.6f",max_current);
		       ibwrt (gpib_num, string,strlen(string));      
			}       
}


void keithley_trigger(void)
{

	   	switch (Langmuir_probe_num) {   
	 	 	case 0:
		    	ibwrt (Keithley_Source, ":READ?;*OPC",11);  //Trigger source plasma Langmuir probe
			break;

			case 1:
		    	ibwrt (Keithley_PHASMA, ":READ?;*OPC",11);  //Trigger PHASMA Langmuir probe
			break;
		
			case 2:
		    	ibwrt (Keithley_Source, ":READ?;*OPC",11);  //Trigger source plasma Langmuir probe
		    	ibwrt (Keithley_PHASMA, ":READ?;*OPC",11);  //Trigger PHASMA Langmuir probe
			break;
		}	   

}

int Langmuir_Activate(void) 

{ 
	short 		found=0;
	short 		found1=0;
	short 		found2=0;
	int 		result;
	double 		Smax_volts;
	double 		Smin_volts;
	double 		Pmax_volts;
	double 		Pmin_volts;
	double 		max_current;


    //Get settings paramets from fron tpanel  
	GetCtrlVal (Langmuir_panel, LANG_PANEL_SMAX_VOLTS, &Smax_volts);
	GetCtrlVal (Langmuir_panel, LANG_PANEL_SMIN_VOLTS, &Smin_volts);
	GetCtrlVal (Langmuir_panel, LANG_PANEL_PMAX_VOLTS, &Pmax_volts);
	GetCtrlVal (Langmuir_panel, LANG_PANEL_PMIN_VOLTS, &Pmin_volts);
	GetCtrlVal (Langmuir_panel, LANG_PANEL_MAX_CURRENT, &max_current);
	GetCtrlVal (Langmuir_panel, LANG_PANEL_PlanarProbe_flag, &Planar_Flag);
	GetCtrlVal (Langmuir_panel, LANG_PANEL_PlanarProbe_Angle, &PlanarProbe_Angle);
	GetCtrlVal (Langmuir_panel, LANG_PANEL_PlanarProbe_Area, &PlanarProbe_Area);
	GetCtrlVal (Langmuir_panel, LANG_PANEL_Source_tiplength, &Source_tiplength);
	GetCtrlVal (Langmuir_panel, LANG_PANEL_PHASMA_tiplength, &PHASMA_tiplength);
	GetCtrlVal (Langmuir_panel, LANG_PANEL_Source_tipradius, &Source_tipradius);
	GetCtrlVal (Langmuir_panel, LANG_PANEL_PHASMA_tipradius, &PHASMA_tipradius);
	GetCtrlVal (Langmuir_panel, LANG_PANEL_Langmuir_probe_num, &Langmuir_probe_num);
	GetCtrlVal (Langmuir_panel, LANG_PANEL_PHASMA_tipradius, &PHASMA_tipradius);
	GetCtrlVal (Langmuir_panel, LANG_PANEL_Langmuir_probe_num, &Langmuir_probe_num);
	GetCtrlVal (Langmuir_panel, LANG_PANEL_BATTERY, &battery);
	
	//Get GPIB port numbers
	GetCtrlVal (Langmuir_panel, LANG_PANEL_KEITHLEYGPIB1,&Keithley1_port);
	GetCtrlVal (Langmuir_panel, LANG_PANEL_KEITHLEYGPIB2,&Keithley2_port);
	
	//Clear the GPIB bus
	SendIFC (1);

	//Set up source, PHASMA, or both Langmuir probes
	switch (Langmuir_probe_num) {   
	 	case 0:
			//Check if device is on the GPIB bus
			result = ibln (1, Keithley1_port, 0, &found);
			
			if (found > 0) {
				//New syntax open command
		    	Keithley_Source = ibdev (1, Keithley1_port, NO_SAD, T10s, 1, 0); //Source plasma Langmuir probe
				//Set voltage range to proper Source or PHASMA option
				set_keithley(Keithley_Source, Smin_volts, Smax_volts, max_current);
			} else {
				Keithley_Source = 0;
			}
		break;

		case 1:
			//Check if device is on the GPIB bus
			result = ibln (1, Keithley2_port, 0, &found);
			
			if (found > 0) {
				//New syntax open command
		    	Keithley_PHASMA = ibdev (1, Keithley2_port, NO_SAD, T10s, 1, 0); //PHASMA plasma Langmuir probe
				//Set voltage range to proper Source or PHASMA option
				set_keithley(Keithley_PHASMA, Pmin_volts, Pmax_volts, max_current);
			} else {
				Keithley_PHASMA=0;
			}
		break;
		
		case 2:
			//Check if device is on the GPIB bus
			result = ibln (1, Keithley1_port, 0, &found1);
			if (found1 > 0) {
				//New syntax open command
		    	Keithley_Source = ibdev (1, Keithley1_port, NO_SAD, T10s, 1, 0); //Source plasma Langmuir probe
				//Set voltage range to proper Source or PHASMA option
				set_keithley(Keithley_Source, Smin_volts, Smax_volts, max_current);
			} else {
				Keithley_Source = 0;
			}

			//Check if device is on the GPIB bus
			result = ibln (1, Keithley2_port, 0, &found2);
			
			if (found2 > 0) {
				//New syntax open command
		    	Keithley_PHASMA = ibdev (1, Keithley2_port, NO_SAD, T10s, 1, 0); //PHASMA plasma Langmuir probe
				//Set voltage range to proper Source or PHASMA option
				set_keithley(Keithley_PHASMA, Pmin_volts, Pmax_volts, max_current);
			} else {
				Keithley_PHASMA=0;
			}
			found=found1*found2;
		break;
	}
	
	return found;
}	   
			   

	
void keithley_aquire(int keithley_gpibS, int keithley_gpibP, double *volt, double *current, double *voltP, double *currentP, double *Keithley_time,double *Keithley_timeP)
   {
	  int result;
	  double a[3003];
	  char buffer[42046];
      int i,j,k;
	  
 	  switch (Langmuir_probe_num) {   
	 	 	//Read I-V curve from sourcemeter
		  	case 0:
				ibrd (keithley_gpibS, buffer, 28030);
				result = Scan (buffer, "%s>%2002f[x]", a);
				j=0;
				for (i=2; i<=2000; i+=2){
				      *(volt+j) = a[i]-battery;
				      *(current+j) = a[i+1];
				      j++;
				}  
			break;

	 	 	//Read PHASMA I-V curve from sourcemeter
			case 1:
				ibrd (keithley_gpibP, buffer, 28030);
				result = Scan (buffer, "%s>%2002f[x]", a);
				j=0;
				for (i=2; i<=2000; i+=2){
				      *(voltP+j) = a[i];
				      *(currentP+j) = a[i+1];
				      j++;
				}  
			break;
		
	 	 	//Read both I-V curve from sourcemeter
			case 2:
				ibrd (keithley_gpibS, buffer, 28030);
				result = Scan (buffer, "%s>%2002f[x]", a);
				j=0;
				for (i=2; i<=2000; i+=2){
				      *(volt+j) = a[i]-battery;
				      *(current+j) = a[i+1];
				      j++;
				}  
				ibrd (keithley_gpibP, buffer, 28030);
				result = Scan (buffer, "%s>%2002f[x]", a);
				j=0;
				for (i=2; i<=2000; i+=2){
				      *(voltP+j) = a[i];
				      *(currentP+j) = a[i+1];
				      j++;
				}  
			 break;
		}	
}

void Langmuir_Analyze(void)		
/* 	location is probe location, 0 = source, 1 = PHASMA
	gas_num=gas species
	fit_pts=number of points to use in temperature fit
	rlength=record length to use
	ave_current=average probe current
	voltage=voltage axis for data
	temp_voltage=voltage from fit to characteristic slope part of trace
	fit_current=current from fit to characteristic slope part of trace
	etemp=electron temperature
	edensity=electron density
	floating_pot=floating potential */

{ 
    ssize_t	min_current_loc;
    ssize_t	max_current_loc;
	int     mu=0.0;
    int		j;
	double	mass;
	double	e_charge=1.602E-19;
	double	isat;
	double  lowest_current;
    double 	min_current;
    double 	max_current;
	double 	chisq;
	double 	intercept;
	double 	slope;
	double	probe_scaling;
	double	Source_probe_area;
	double	PHASMA_probe_area;
	double	LP_current_norm[LP_recordlength]={0};
	double	LP_temp_current[LP_recordlength]={0};
	
	
	//Set ion mass for analysis
	switch (species) {   
	 	 	//Read I-V curve from sourcemeter
		  	case 1:
				mu=40;
			break;
		  	case 2:
				mu=4;
			break;
		  	case 3:
				mu=14;
			break;
		  	case 4:
				mu=126.9;
			break;
		  	case 5:
				mu=131.29;
			break;
		  	case 6:
				mu=40;
			break;
	 }

	//Ion mass in real units		
    mass= mu * 1.627E-27;
 
	//Calculate Probe tip areas 
 	if (Planar_Flag) {
 		Source_probe_area=sin(PlanarProbe_Angle)*PlanarProbe_Area;  
 		PHASMA_probe_area=sin(PlanarProbe_Angle)*PlanarProbe_Area;  
 	} else {
		//Convert radii to proper units
		Source_tipradius= Source_tipradius/1000.0;
		PHASMA_tipradius=PHASMA_tipradius/1000.0;
 		Source_probe_area=Source_tipradius*Source_tipradius*pi+(2.0*Source_tipradius*Source_tiplength/1000.0)*pi;  
 		PHASMA_probe_area=PHASMA_tipradius*PHASMA_tipradius*pi+(2.0*PHASMA_tipradius*PHASMA_tiplength/1000.0)*pi;  
	} 
	

	switch (Langmuir_probe_num) {   
	 	 	//Analyzer first Langmuir probe data
		  	case 0:
				//Zero key variables
				isat=0.0;
	
				//Set up real units
				probe_scaling=(1/0.49)*sqrt(mass/e_charge)*(1.0/(Source_probe_area*e_charge));  
					
				// 	Determine Ion Saturation Current 	 
			    for (j = 0; j<20; j++)  
			     	isat=isat+LP_currentS[j];
				isat=fabs(isat/20.0);
				MaxMin1D (LP_currentS,LP_recordlength, &max_current, &max_current_loc, &lowest_current, &min_current_loc);

				// 	Determine Floating Potential 
				Abs1D (LP_currentS,LP_recordlength, abs_current);
				MaxMin1D (abs_current, LP_recordlength, &max_current, &max_current_loc,&min_current, &min_current_loc);
				if (min_current_loc > LP_recordlength-LP_fit_pts-10) min_current_loc=LP_recordlength-LP_fit_pts-11;
				Source_Floating=LP_voltageS[min_current_loc];

				//  Normalize Current Array by Adding Ion Saturation Current to Values 
				for (j = 0; j<LP_recordlength; j++) 
			    	LP_current_norm[j]=LP_currentS[j]+fabs(lowest_current);

				//  Fill Array for Temperature Determination and Do Linear Fit to fit_pts Points
				for (j=0; j<LP_fit_pts; j++) {
				 	LP_fit_voltageS[j]=LP_voltageS[min_current_loc+j+10];
				 	LP_temp_current[j]=log(LP_current_norm[min_current_loc+j+10]);
				}   
				LinFit (LP_fit_voltageS, LP_temp_current, LP_fit_pts, LP_fit_current, &slope,&intercept, &chisq);

				//	Convert fit routine points back into current 
				for (j=0; j<LP_fit_pts; j++) 
				 	LP_fit_currentS[j]=exp(LP_fit_current[j])-fabs(lowest_current);
	
				
				// 	Calculate Other Plasma Parameters with a check to make sure not dividing by zero or taking square root of negative number
				if (slope <= 0.0001) Source_Te = 0.0;
					Source_Te=(1.0/slope);
				if (Source_Te <= 0) Source_Te = 0.0000001;
					//if (IsNotANumber(etemp) || IsInfinity(etemp)) etemp = 0.0000001; // Added DST 12/3/16. Fixes error from sqrt(NaN)
				Source_Ne=isat*probe_scaling/sqrt(Source_Te)/1.0E18;			
			break;

			//Analyze PHASMA Langmuir probe
			case 1:
				//Zero key variables
				isat=0.0;
	
				//Set up real units
				probe_scaling=(1/0.49)*sqrt(mass/e_charge)*(1.0/(PHASMA_probe_area*e_charge));  
					
				// 	Determine Ion Saturation Current 	 
			    for (j = 0; j<20; j++)  
			     	isat=isat+LP_currentP[j];
				isat=fabs(isat/20.0);
				MaxMin1D (LP_currentP,LP_recordlength, &max_current, &max_current_loc, &lowest_current, &min_current_loc);

				// 	Determine Floating Potential 
				Abs1D (LP_currentP,LP_recordlength, abs_current);
				MaxMin1D (abs_current, LP_recordlength, &max_current, &max_current_loc,&min_current, &min_current_loc);
				if (min_current_loc > LP_recordlength-LP_fit_pts-10) min_current_loc=LP_recordlength-LP_fit_pts-11;
				PHASMA_Floating=LP_voltageP[min_current_loc];

				//  Normalize Current Array by Adding Ion Saturation Current to Values 
				for (j = 0; j<LP_recordlength; j++) 
			    	LP_current_norm[j]=LP_currentP[j]+fabs(lowest_current);

				//  Fill Array for Temperature Determination and Do Linear Fit to fit_pts Points
				for (j=0; j<LP_fit_pts; j++) {
				 	LP_fit_voltageP[j]=LP_voltageP[min_current_loc+j+10];
				 	LP_temp_current[j]=log(LP_current_norm[min_current_loc+j+10]);
				}   
				LinFit (LP_fit_voltageP, LP_temp_current, LP_fit_pts, LP_fit_current, &slope,&intercept, &chisq);

				//	Convert fit routine points back into current 
				for (j=0; j<LP_fit_pts; j++) 
				 	LP_fit_currentP[j]=exp(LP_fit_current[j])-fabs(lowest_current);
	
				
				// 	Calculate Other Plasma Parameters with a check to make sure not dividing by zero or taking square root of negative number
				if (slope <= 0.0001) PHASMA_Te = 0.0;
					PHASMA_Te=(1.0/slope);
				if (PHASMA_Te <= 0) PHASMA_Te = 0.0000001;
					//if (IsNotANumber(etemp) || IsInfinity(etemp)) etemp = 0.0000001; // Added DST 12/3/16. Fixes error from sqrt(NaN)
				PHASMA_Ne=isat*probe_scaling/sqrt(PHASMA_Te)/1.0E18;			
			break;

			//Analyze both Langmuir probes
			case 2:
				//Zero key variables
				isat=0.0;
	
				//Set up real units
				probe_scaling=(1/0.49)*sqrt(mass/e_charge)*(1.0/(Source_probe_area*e_charge));  
					
				// 	Determine Ion Saturation Current 	 
			    for (j = 0; j<20; j++)  
			     	isat=isat+LP_currentS[j];
				isat=fabs(isat/20.0);
				MaxMin1D (LP_currentS,LP_recordlength, &max_current, &max_current_loc, &lowest_current, &min_current_loc);

				// 	Determine Floating Potential 
				Abs1D (LP_currentS,LP_recordlength, abs_current);
				MaxMin1D (abs_current, LP_recordlength, &max_current, &max_current_loc,&min_current, &min_current_loc);
				if (min_current_loc > LP_recordlength-LP_fit_pts-10) min_current_loc=LP_recordlength-LP_fit_pts-11;
				Source_Floating=LP_voltageS[min_current_loc];

				//  Normalize Current Array by Adding Ion Saturation Current to Values 
				for (j = 0; j<LP_recordlength; j++) 
			    	LP_current_norm[j]=LP_currentS[j]+fabs(lowest_current);

				//  Fill Array for Temperature Determination and Do Linear Fit to fit_pts Points
				for (j=0; j<LP_fit_pts; j++) {
				 	LP_fit_voltageS[j]=LP_voltageS[min_current_loc+j+10];
				 	LP_temp_current[j]=log(LP_current_norm[min_current_loc+j+10]);
				}   
				LinFit (LP_fit_voltageS, LP_temp_current, LP_fit_pts, LP_fit_current, &slope,&intercept, &chisq);

				//	Convert fit routine points back into current 
				for (j=0; j<LP_fit_pts; j++) 
				 	LP_fit_currentS[j]=exp(LP_fit_current[j])-fabs(lowest_current);
	
				
				// 	Calculate Other Plasma Parameters with a check to make sure not dividing by zero or taking square root of negative number
				if (slope <= 0.0001) Source_Te = 0.0;
					Source_Te=(1.0/slope);
				if (Source_Te <= 0) Source_Te = 0.0000001;
					//if (IsNotANumber(etemp) || IsInfinity(etemp)) etemp = 0.0000001; // Added DST 12/3/16. Fixes error from sqrt(NaN)
				Source_Ne=isat*probe_scaling/sqrt(Source_Te)/1.0E18;			

								//Zero key variables
				isat=0.0;
	
				//Set up real units
				probe_scaling=(1/0.49)*sqrt(mass/e_charge)*(1.0/(PHASMA_probe_area*e_charge));  
					
				// 	Determine Ion Saturation Current 	 
			    for (j = 0; j<20; j++)  
			     	isat=isat+LP_currentP[j];
				isat=fabs(isat/20.0);
				MaxMin1D (LP_currentP,LP_recordlength, &max_current, &max_current_loc, &lowest_current, &min_current_loc);

				// 	Determine Floating Potential 
				Abs1D (LP_currentP,LP_recordlength, abs_current);
				MaxMin1D (abs_current, LP_recordlength, &max_current, &max_current_loc,&min_current, &min_current_loc);
				if (min_current_loc > LP_recordlength-LP_fit_pts-10) min_current_loc=LP_recordlength-LP_fit_pts-11;
				PHASMA_Floating=LP_voltageP[min_current_loc];

				//  Normalize Current Array by Adding Ion Saturation Current to Values 
				for (j = 0; j<LP_recordlength; j++) 
			    	LP_current_norm[j]=LP_currentP[j]+fabs(lowest_current);

				//  Fill Array for Temperature Determination and Do Linear Fit to fit_pts Points
				for (j=0; j<LP_fit_pts; j++) {
				 	LP_fit_voltageP[j]=LP_voltageP[min_current_loc+j+10];
				 	LP_temp_current[j]=log(LP_current_norm[min_current_loc+j+10]);
				}   
				LinFit (LP_fit_voltageP, LP_temp_current, LP_fit_pts, LP_fit_current, &slope,&intercept, &chisq);

				//	Convert fit routine points back into current 
				for (j=0; j<LP_fit_pts; j++) 
				 	LP_fit_currentP[j]=exp(LP_fit_current[j])-fabs(lowest_current);
	
				
				// 	Calculate Other Plasma Parameters with a check to make sure not dividing by zero or taking square root of negative number
				if (slope <= 0.0001) PHASMA_Te = 0.0;
					PHASMA_Te=(1.0/slope);
				if (PHASMA_Te <= 0) PHASMA_Te = 0.0000001;
					//if (IsNotANumber(etemp) || IsInfinity(etemp)) etemp = 0.0000001; // Added DST 12/3/16. Fixes error from sqrt(NaN)
				PHASMA_Ne=isat*probe_scaling/sqrt(PHASMA_Te)/1.0E18;			
		
			break;
	}

}  



void Langmuir_plot(void)
{ 	
    ssize_t min_index;
    ssize_t max_index;
    double 	min;
    double 	max;

	//Clear plot
	DeleteGraphPlot (Langmuir_panel, LANG_PANEL_LP_plot, -1,VAL_IMMEDIATE_DRAW);
	
	switch (Langmuir_probe_num) {   
	 	 	//Plot first Langmuir probe data
		  	case 0:
				//	Plot Langmuir Probe Data from first probe and Fit to Screen
				MaxMin1D (LP_voltageS, LP_recordlength, &max, &max_index, &min, &min_index);
			
				//Note battery offset included in axis scaling
				SetAxisRange (Langmuir_panel, LANG_PANEL_LP_plot, VAL_MANUAL, min, max,
															  VAL_AUTOSCALE, -5E-6, 1E-5);
				PlotXY (Langmuir_panel, LANG_PANEL_LP_plot, LP_voltageS,LP_currentS,
											LP_recordlength, VAL_DOUBLE, VAL_DOUBLE, VAL_THIN_LINE,
											VAL_NO_POINT, VAL_SOLID, 1, VAL_RED);
				PlotXY (Langmuir_panel, LANG_PANEL_LP_plot, LP_fit_voltageS, LP_fit_currentS,
											LP_fit_pts, VAL_DOUBLE, VAL_DOUBLE, VAL_THIN_LINE, 
											VAL_NO_POINT,VAL_SOLID, 1, VAL_BLUE);
	
				//Write fit results to the screen
				SetCtrlVal (Langmuir_panel, LANG_PANEL_Te1, Source_Te);
				SetCtrlVal (Langmuir_panel, LANG_PANEL_Ne1, Source_Ne);
				SetCtrlVal (Langmuir_panel, LANG_PANEL_FloatPot1, Source_Floating);
			break;
	 	 	//Plot second Langmuir probe data
		  	case 1:
				//	Plot Langmuir Probe Data from first probe and Fit to Screen
				MaxMin1D (LP_voltageP, LP_recordlength, &max, &max_index, &min, &min_index);
			
				//Note battery offset included in axis scaling
				SetAxisRange (Langmuir_panel, LANG_PANEL_LP_plot, VAL_MANUAL, min, max,
															  VAL_AUTOSCALE, -5E-6, 1E-5);
				PlotXY (Langmuir_panel, LANG_PANEL_LP_plot, LP_voltageP,LP_currentP,
											LP_recordlength, VAL_DOUBLE, VAL_DOUBLE, VAL_THIN_LINE,
											VAL_NO_POINT, VAL_SOLID, 1, VAL_RED);
				PlotXY (Langmuir_panel, LANG_PANEL_LP_plot, LP_fit_voltageP, LP_fit_currentP,
											LP_fit_pts, VAL_DOUBLE, VAL_DOUBLE, VAL_THIN_LINE, 
											VAL_NO_POINT,VAL_SOLID, 1, VAL_BLUE);
	
				//Write fit results to the screen
				SetCtrlVal (Langmuir_panel, LANG_PANEL_Te2, PHASMA_Te);
				SetCtrlVal (Langmuir_panel, LANG_PANEL_Ne2, PHASMA_Ne);
				SetCtrlVal (Langmuir_panel, LANG_PANEL_FloatPot2, PHASMA_Floating);
			break;
	 	 	//Plot both Langmuir probe data
		  	case 2:
				//	Plot Langmuir Probe Data from first probe and Fit to Screen
				MaxMin1D (LP_voltageS, LP_recordlength, &max, &max_index, &min, &min_index);
			
				//Note battery offset included in axis scaling
				SetAxisRange (Langmuir_panel, LANG_PANEL_LP_plot, VAL_MANUAL, min, max,
															  VAL_AUTOSCALE, -5E-6, 1E-5);
				PlotXY (Langmuir_panel, LANG_PANEL_LP_plot, LP_voltageS,LP_currentS,
											LP_recordlength, VAL_DOUBLE, VAL_DOUBLE, VAL_THIN_LINE,
											VAL_NO_POINT, VAL_SOLID, 1, VAL_RED);
				PlotXY (Langmuir_panel, LANG_PANEL_LP_plot, LP_fit_voltageS, LP_fit_currentS,
											LP_fit_pts, VAL_DOUBLE, VAL_DOUBLE, VAL_THIN_LINE, 
											VAL_NO_POINT,VAL_SOLID, 1, VAL_BLUE);
	
				//Write fit results to the screen
				SetCtrlVal (Langmuir_panel, LANG_PANEL_Te1, Source_Te);
				SetCtrlVal (Langmuir_panel, LANG_PANEL_Ne1, Source_Ne);
				SetCtrlVal (Langmuir_panel, LANG_PANEL_FloatPot1, Source_Floating);
			
				
				//	Plot Langmuir Probe Data from first probe and Fit to Screen
				MaxMin1D (LP_voltageP, LP_recordlength, &max, &max_index, &min, &min_index);
			
				//Note battery offset included in axis scaling
				SetAxisRange (Langmuir_panel, LANG_PANEL_LP_plot, VAL_MANUAL, min, max,
															  VAL_AUTOSCALE, -5E-6, 1E-5);
				PlotXY (Langmuir_panel, LANG_PANEL_LP_plot, LP_voltageP,LP_currentP,
											LP_recordlength, VAL_DOUBLE, VAL_DOUBLE, VAL_THIN_LINE,
											VAL_NO_POINT, VAL_SOLID, 1, VAL_RED);
				PlotXY (Langmuir_panel, LANG_PANEL_LP_plot, LP_fit_voltageP, LP_fit_currentP,
											LP_fit_pts, VAL_DOUBLE, VAL_DOUBLE, VAL_THIN_LINE, 
											VAL_NO_POINT,VAL_SOLID, 1, VAL_BLUE);
	
				//Write fit results to the screen
				SetCtrlVal (Langmuir_panel, LANG_PANEL_Te2, PHASMA_Te);
				SetCtrlVal (Langmuir_panel, LANG_PANEL_Ne2, PHASMA_Ne);
				SetCtrlVal (Langmuir_panel, LANG_PANEL_FloatPot2, PHASMA_Floating);

			break;
	 }	
	
}



void write_Langmuir_data(void)
{	

	int		j;
	char	ChannelNameString[36];
	char	outfilename[64];
	FILE*	outfile;


	//Dump LIF results to a file
	sprintf(outfilename, RawDataPath);
	strcat(outfilename, ShotNumberString);
	strcat(outfilename, "_");
	strcat(outfilename, "Langmuir.txt");
	outfile=fopen (outfilename, "w");

	//Store raw LIF signals
	for (j=0;j<LP_recordlength;j++) {
		fprintf(outfile,"%f,%f,%f,%f\n",LP_voltageS[j], LP_currentS[j],LP_voltageP[j],LP_currentP[j]);
	}

 	//Close Langmuir data file
	fclose (outfile);

	//Write an array of all the instrument write-t-mds flags to a spreadsheet to maintain a permanent list
	//of which data are stored for each shot. Include the shot number, the date, and all the flags.
	outfile = fopen (PHASMA_LOG_String, "a");
	fprintf(outfile,"%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n",shotnum,0, 0,0,0,0,
		0,0,0,0,0,0,0, 1,0,0,0,0,0,0,0);

 	//Close magnetic field profile data file
	fclose (outfile);

	//Get channel name string, store raw data on drive folder, and push data to MDSPlus for Channel 1
	GetCtrlVal (Langmuir_panel, LANG_PANEL_Chan1Name,ChannelNameString);
	Write_to_PHASMA_MDS(LP_voltageS, LP_currentS,ChannelNameString);

	//Get channel name string, store raw data on drive folder, and push data to MDSPlus for Channel 3
	GetCtrlVal (Langmuir_panel, LANG_PANEL_Chan2Name,ChannelNameString);
	Write_to_PHASMA_MDS(LP_voltageP, LP_currentP,ChannelNameString);

}		    


//Trigger the sourcemeters and read back the data     
void Langmuir_Acquire(void)  
{
	int j;

	
	//Trigger sweeps
	keithley_trigger(); 
		
	//Acquire data from sourcemeters	
	keithley_aquire(Keithley_Source, Keithley_PHASMA, &LP_voltageS[0], &LP_currentS[0],&LP_voltageP[0], &LP_currentP[0], &LP_timeS[0],&LP_timeP[0]);
	
	//Analyze the LIF data
	Langmuir_Analyze(); 
		
	//Plot the LIF data
	Langmuir_plot();		
		
	//Store the LIF data if flag set on user interface
	GetCtrlVal (Langmuir_panel, LANG_PANEL_Lang_Data,&Lang_DataFlag);
	if (Lang_DataFlag) {
		write_Langmuir_data();
	}
}


int CVICALLBACK Langmuir_Activate_Button (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
int	shotnum;
int result;

	switch (event)
	{
		case EVENT_COMMIT:
			//Open MDS to new shot number
			result=	IncrementMDSCurrentShot();
	
			//Get current shot from MDS database		
			shotnum= getMDSCurrentShot();
			
			//Update shot number in panel
			SetCtrlVal (Langmuir_panel, LANG_PANEL_MDS_Shot,shotnum);
			
			//run acquisition if and only if Keithley sourcemeters found on the GPIB bus
			if (Langmuir_Activate()) {
				Langmuir_Acquire();
			}
			break;
	}
	return 0;
	
	
}



void OpenLang_Settings (void)
{
	int	shotnum;
	
	Langmuir_panel = LoadPanel (0, "LangmuirSettings.uir", LANG_PANEL);
	RecallPanelState (Langmuir_panel, "Master_Control_Storage_File", Langmuir_setup_state);
	
	//Update MDS shot number
	shotnum=getMDSCurrentShot();
		
	//Display Panel
	DisplayPanel (Langmuir_panel);
	
	//Update shot number in panel
	SetCtrlVal (Langmuir_panel, LANG_PANEL_MDS_Shot,shotnum);
	
	// Start interacting with user
    RunUserInterface ();
}

void keithley_end(int gpib_num)

   {
     
    if (gpib_num > 0 ) {
	   	ibwrt (gpib_num, "OUTP OFF", 8);
	 	ibonl (gpib_num, 0);  	 	
	}
}

int CVICALLBACK Close_Langmuir_Settings (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			//Turn off Keithley instruments
			keithley_end(Keithley_Source);
			keithley_end(Keithley_PHASMA);
			
			
			//Save current state of panel and close interface
			SavePanelState (Langmuir_panel, "Master_Control_Storage_File", Langmuir_setup_state);
			DiscardPanel(panel);
			
			break;
	}
	return 0;
}
