#include 	<NIDAQmx.h>
#include 	<utility.h>
#include 	<ansi_c.h>
#include 	<nivxi.h>
#include 	"Tkvx4780.h"
#include 	"tkvx4244.h"
#include 	"lif.h"
#include 	"global_defines.h"
#include	"Joerger_routines.h"
#include	"keithley2400.h"
#include	"ep.h"
#include	<Sirah.h> 
#include	<toptica.h> 

extern int 			DeltaN_flag, LP_flag,LIF_flag,LIF_cont_flag, LIF_diode_flag,LIF_dye_flag,Mwave_flag,Bdot_flag,SuperProbe_flag,Estat_flag,EP_flag, Joerger_flag,DyeLaserSwitch, PlasmaGun_flag, Pulsed_flag;
extern	ViSession 	digitizer_hndl;
extern int			bit_mask;
extern TaskHandle	OutputCardLIFTaskHandle;  
extern TaskHandle	OutputCardTrigHandle;  

//This function triggers all the hardware flagged for use in the setup sequence.
void trigger_hardware(void)
{
	int 			result;
	int				Reg_value;
	int				poll_value;
	ViChar 			status_byte[10]; 
	extern double	Pulse_length;
	
	poll_value = 0;  //Gotta initialize this, or only first trigger will work
	Reg_value = 0;  //Initialize this
	
	//If emissive probe in use, set up emissive probe function generator to operate in burst mode and to wait for a trigger
	if (EP_flag) {
		EP_Burst_Mode();
	}
	
	//Trigger LIF laser if continuous scan LIF 
	if (LIF_cont_flag)  {
		if (LIF_diode_flag) 	toptica_Laser_Scan ();
		if (LIF_dye_flag ) 		Sirah_Laser_Scan ();
		DAQmxStartTask (OutputCardLIFTaskHandle); 	//immediate start of task to begin recording data
	}
	
	//	Trigger Keithly Sourcemeter (do this first to catch any pulsed activity)   
	if (LP_flag) keithley_trigger();
	
	//	Send TTL trigger from NI card and it is mapped to TTL0 to trigger digitizers  
	if (Estat_flag || PlasmaGun_flag || Mwave_flag || LIF_flag || Pulsed_flag || LIF_cont_flag || Joerger_flag)  {
		
		// Send a trigger out on TTL 1 to trigger armed digitizers 
		//SrcTrig (1, 1, 4, 100);

		//get I/o card to send out a trigger pulse by starting up the task  
		DAQmxStartTask (OutputCardTrigHandle); 	  	
		
	}
	
	//Trigger Joerger Instrument with software trigger just in case external trigger didn't work.May not be needed
	if (Joerger_flag == 1)  {
		Joerger_trigger();   			//use software trigger to fire digitizer
		JoergerVTR10014_trigger();   	//use software trigger to fire digitizer
	}

	//Delay 25% longer than the pulse length
	Delay(1.25*Pulse_length/1000.0);		
	
	//	This loop delays until acqusition is complete
   	if (Estat_flag || PlasmaGun_flag || Mwave_flag) {
		do {
			tkvx4244_sleep (1); //	wait 1 sec, set according to maximum efficiency
			tkvx4244_readStatus (digitizer_hndl, tkvx4244_QUERY_STB, status_byte);
			poll_value=atoi(status_byte); 
			if( KeyHit()) {	//automatically exits if a key is hit
	 			GetKey();
	     		poll_value=bit_mask;
		 	}
			SetStdioWindowVisibility (0);
		}while((poll_value & bit_mask) != bit_mask);   //	when the status byte = bit_mask, exit
	}
	
	//	This loop delays until Joerger acqusition is complete
	if (Joerger_flag == 1)  {
		Joerger_complete ();
		JoergerVTR10014_complete ();
	}

	// Shut down trigger out on TTL one					
	if (Estat_flag || PlasmaGun_flag || Mwave_flag || LIF_flag || Pulsed_flag || Joerger_flag)   {
		//SrcTrig (1, 1, 3, 100);
	}
	
	//If emissive probe in use, restore emissive probe function generator to operate in continuous heating mode
	if (EP_flag) {
		Emissive_Probe_setup();
	}
	
	//Shut down trigger task	
	DAQmxStopTask (OutputCardTrigHandle); 	  	

}                    
