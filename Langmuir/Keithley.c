#include <userint.h>
#include <ansi_c.h>
#include <formatio.h>
#include <gpib.h>
#include <keithley2400.h> 
#include <keithley_setup.h>

int				keithley_setup_panel=0;
int 			keithley_setup_state=156;    
double 			max_volts;
double 			min_volts;
double 			Smax_volts;
double 			Smin_volts;
double 			Pmax_volts;
double 			Pmin_volts;
double 			step_volts;
double 			max_current;
extern int		Planar_Flag;
extern int		Pulsed_flag;
extern int		langmuir_pts;
extern double	Planar_Probe_Angle;













void keithley_aquire(double *volt, double *current, double *voltP, double *currentP, double *Keithley_time,double *Keithley_timeP)

   {
	  int result;
	  double a[3003];
	  char buffer[42046];
      int i,j,k;
	  double battery;
	  
	  GetCtrlVal (keithley_setup_panel, KEITHLEY_BATTERY, &battery);

 	  switch (Langmuir_probe_num) {   
	 	 	//Read I-V curve from sourcemeter
		  	case 0:
				if (!Pulsed_flag) {
					  ibrd (keithley_gpibS, buffer, 28030);
					  result = Scan (buffer, "%s>%2002f[x]", a);
					  j=0;
					  for (i=2; i<=2000; i+=2){
					       *(volt+j) = a[i]-battery;
					       *(current+j) = a[i+1];
					       j++;
					  }  
				} else {
					//read data from Keithley buffer
					ibrd (keithley_gpibS, buffer, 42046);		  
				
					//Sort measurements into arrays
					result = Scan (buffer, "%s>%3000f[x]", a);
					k=0;
					for (i=0; i<(int)(3*langmuir_pts); i+=3) {
						volt[k] = a[i];
						current[k] = a[i+1];
						Keithley_time[k]= a[i+2]-a[2];	  //adjust time values to include absolute time offset
						k++;
					} 
				}
			break;

	 	 	//Read PHASMA I-V curve from sourcemeter
			case 1:
				if (!Pulsed_flag) {
					  ibrd (keithley_gpibP, buffer, 28030);
					  result = Scan (buffer, "%s>%2002f[x]", a);
					  j=0;
					  for (i=2; i<=2000; i+=2){
					       *(voltP+j) = a[i];
					       *(currentP+j) = a[i+1];
					       j++;
					  }  
				} else {
					//read data from Keithley buffer
					ibrd (keithley_gpibP, buffer, 42046);		  
				
					//Sort measurements into arrays
					result = Scan (buffer, "%s>%3000f[x]", a);
					k=0;
					for (i=0; i<(int)(3*langmuir_pts); i+=3) {
						voltP[k] = a[i];
						currentP[k] = a[i+1];
						Keithley_time[k]= a[i+2]-a[2];	  //adjust time values to include absolute time offset
						k++;
					} 
				}
			break;
		
	 	 	//Read both I-V curve from sourcemeter
			case 2:
			  	if (!Pulsed_flag) {
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
				} else {
					//read data from Keithley buffer
					ibrd (keithley_gpibS, buffer, 42046);		  
				
					//Sort measurements into arrays
					result = Scan (buffer, "%s>%3000f[x]", a);
					k=0;
					for (i=0; i<(int)(3*langmuir_pts); i+=3) {
						volt[k] = a[i];
						current[k] = a[i+1];
						Keithley_time[k]= a[i+2]-a[2];	  //adjust time values to include absolute time offset
						k++;
					} 

					//read data from Keithley buffer
					ibrd (keithley_gpibP, buffer, 42046);		  
				
					//Sort measurements into arrays
					result = Scan (buffer, "%s>%3000f[x]", a);
					k=0;
					for (i=0; i<(int)(3*langmuir_pts); i+=3) {
						voltP[k] = a[i];
						currentP[k] = a[i+1];
						Keithley_timeP[k]= a[i+2]-a[2];	  //adjust time values to include absolute time offset
						k++;
					} 
				}
			  break;
		}	
}
	

   
