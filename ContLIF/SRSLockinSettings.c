#include <string.h> 
#include <stdio.h>
#include <formatio.h>
#include <ansi_c.h>
#include <utility.h>
#include <gpib.h>


int setLIAintTime (int timeconst, int tcmult, char tcu[3], int sensitivity, int sens_multiplier, char sens_units[3], int Lockin_reserve, int Lockin_slope)
{
    int	result;                 
    int tcsetting;
	int	Lockin_sens_setting;
    int itcu;
    int sunits;
    int gpibdev;
    int gpibdev2;
    char sendstring[11];
	extern int	DualLIFSwitch;    
	extern int	LIF_diode_flag;    
		
			
    if (result = !strcmp (tcu, "us")) itcu = 1;
    if (result = !strcmp (tcu, "ms")) itcu = 2; 
    if (result = !strcmp (tcu, "s"))  itcu = 3; 
    if (result = !strcmp (tcu, "ks")) itcu = 4; 
    

//  Determine time constant setting

    if ( timeconst == 1 )
       {
          if ( tcmult == 1)
            {
               switch (itcu)
                 {
                 
                    case 1:
                       // This setting can not be done;
                    break;
                    
                    case 2:
                       tcsetting = 4;
                    break;  
            		   
            		case 3:
                       tcsetting = 10;
                    break;  
            
            		case 4:
                       tcsetting = 16;
                    break;  
            	 }
            }
          else if ( tcmult == 10 )
            {
               switch (itcu)
                 {
                 
                    case 1:
                       tcsetting = 0;
                    break;
                    
                    case 2:
                       tcsetting = 6;
                    break;  
            		   
            		case 3:
                       tcsetting = 12;
                    break;  
            
            		case 4:
                       tcsetting = 18;
                    break;  
            	 }
            }
          else
            {
            	switch (itcu)
                 {
                 
                    case 1:
                       tcsetting = 2;
                    break;
                    
                    case 2:
                       tcsetting = 8;
                    break;  
            		   
            		case 3:
                       tcsetting = 14;
                    break;  
            
            		case 4:
                       // This setting can not be done;
                    break;
                 }  
            }
       }
     else  
       {
          if ( tcmult == 1)
            {
            	switch (itcu)
                 {
                 
                    case 1:
                       // This setting can not be done.
                    break;
                    
                    case 2:
                       tcsetting = 5;
                    break;  
            		   
            		case 3:
                       tcsetting = 11;
                    break;  
            
            		case 4:
                       tcsetting = 17;
                    break;
                 }   
            } 
          else if ( tcmult == 10 )
            {
            	switch (itcu)
                 {
                    case 1:
                       tcsetting = 1;
                    break;
                    
                    case 2:
                       tcsetting = 7;
                    break;  
            		   
            		case 3:
                       tcsetting = 13;
                    break;  
            
            		case 4:
                       tcsetting = 19;
                    break;
                  }  
            }
          else
            {
            	switch (itcu)
                 {
                 
                    case 1:
                       tcsetting = 3;
                    break;
                    
                    case 2:
                       tcsetting = 9;
                    break;  
            		   
            		case 3:
                       tcsetting = 15;
                    break;  
            
            		case 4:
                       // This setting can not be done;
                    break;
                 }   
            }
        }
        
        
	//  Determine sensitivity setting
    if (result = !strcmp (sens_units, "V"))  sunits = 4;
    if (result = !strcmp (sens_units, "mV")) sunits = 3; 
    if (result = !strcmp (sens_units, "uV")) sunits = 2; 
    if (result = !strcmp (sens_units, "nV")) sunits = 1; 
    

    switch (sunits )			  //sensitivity units
       {
           case 1:
           		switch (sens_multiplier) 
				{
					case 1:
						switch (sensitivity)
						{
							case 1:
								//this setting cannot be done
							break;
							case 2:
								Lockin_sens_setting = 0;
							break;
							case 5:
								Lockin_sens_setting = 1;
							break;
						}
					break;
					
					case 10:
						switch (sensitivity)
						{
							case 1:
								Lockin_sens_setting = 2;
							break;
							case 2:
								Lockin_sens_setting = 3;
							break;
							case 5:
								Lockin_sens_setting = 4;
							break;
						}
					break;
					
					case 100:
						switch (sensitivity)
						{
							case 1:
								Lockin_sens_setting = 5;
							break;
							case 2:
								Lockin_sens_setting = 6;
							break;
							case 5:
								Lockin_sens_setting = 7;
							break;
						}
					break;
				}	
           break;
                    
           case 2:
          		switch (sens_multiplier) 
				{
					case 1:
						switch (sensitivity)
						{
							case 1:
								Lockin_sens_setting = 8;
							break;
							case 2:
								Lockin_sens_setting = 9;
							break;
							case 5:
								Lockin_sens_setting = 10;
							break;
						}
					break;
					
					case 10:
						switch (sensitivity)
						{
							case 1:
								Lockin_sens_setting = 11;
							break;
							case 2:
								Lockin_sens_setting = 12;
							break;
							case 5:
								Lockin_sens_setting = 13;
							break;
						}
					break;
					
					case 100:
						switch (sensitivity)
						{
							case 1:
								Lockin_sens_setting = 14;
							break;
							case 2:
								Lockin_sens_setting = 15;
							break;
							case 5:
								Lockin_sens_setting = 16;
							break;
						}
					break;
				}	
           break;  
            		   
           case 3:
           		switch (sens_multiplier) 
				{
					case 1:
						switch (sensitivity)
						{
							case 1:
								Lockin_sens_setting = 17;
							break;
							case 2:
								Lockin_sens_setting = 18;
							break;
							case 5:
								Lockin_sens_setting = 19;
							break;
						}
					break;
					
					case 10:
						switch (sensitivity)
						{
							case 1:
								Lockin_sens_setting = 20;
							break;
							case 2:
								Lockin_sens_setting = 21;
							break;
							case 5:
								Lockin_sens_setting = 22;
							break;
						}
					break;
					
					case 100:
						switch (sensitivity)
						{
							case 1:
								Lockin_sens_setting = 23;
							break;
							case 2:
								Lockin_sens_setting = 24;
							break;
							case 5:
								Lockin_sens_setting = 25;
							break;
						}
					break;
				}	
           break;  
            
           case 4:
              	Lockin_sens_setting = 26;
           break;  
     }

	//  Open Lockin Amplifier #1
 	//New syntax open command
	gpibdev = ibdev (0, 13, NO_SAD, T10s, 1, 0);
	
	//set parameters if valid device found
	if (gpibdev > 0) {
		// set timeconstant
		sprintf(sendstring, "OFLT %d\n", tcsetting);  
		result = ibwrt (gpibdev, sendstring, strlen (sendstring));
	
		// set Sensitivity
		sprintf(sendstring, "SENS %d\n", Lockin_sens_setting);  
		result = ibwrt (gpibdev, sendstring, strlen (sendstring));
	
		// set reserve
		sprintf(sendstring, "RMOD %d\n", Lockin_reserve);  
		result = ibwrt (gpibdev, sendstring, strlen (sendstring));
	
		// set slope
		sprintf(sendstring, "OFSL %d\n", Lockin_slope);  
		result = ibwrt (gpibdev, sendstring, strlen (sendstring));
	
	  	// Close LOCKIN GPIB port using new open/close process
	 	ibonl (gpibdev, 0);
	}
	 
	//  Open Lockin Amplifier #2 if in use
	if (DualLIFSwitch||LIF_diode_flag) {
	 	//New syntax open command
		gpibdev2 = ibdev (0, 19, NO_SAD, T10s, 1, 0);

		//set parameters if valid device found
		if (gpibdev2 > 0) {
			// set timeconstant
			sprintf(sendstring, "OFLT %d\n", tcsetting);  
			result = ibwrt (gpibdev2, sendstring, strlen (sendstring));
	
			// set Sensitivity
			sprintf(sendstring, "SENS %d\n", Lockin_sens_setting);  
			result = ibwrt (gpibdev2, sendstring, strlen (sendstring));
	
			// set reserve
			sprintf(sendstring, "RMOD %d\n", Lockin_reserve);  
			result = ibwrt (gpibdev2, sendstring, strlen (sendstring));
	
			// set slope
			sprintf(sendstring, "OFSL %d\n", Lockin_slope);  
			result = ibwrt (gpibdev2, sendstring, strlen (sendstring));
	 	
		 	// Close LOCKIN2 GPIB port using new open/close process
		 	ibonl (gpibdev2, 0);
		}
		
	}
	return(0);
  
  }
