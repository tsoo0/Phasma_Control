#include 		<NIDAQmx.h>				//Neeeded so that the Taskhandle variable can be defined
int				Global_Visa_Session_Handle;
int				species; 
float 			Master_cycle_time;
float			Laser_Warmup;
float 			Gun_Charging;
char 			RawDataPath[128];  // Location of raw data in local file system
char 			PHASMA_LOG_String[128];
char			MDSDATAPATH[128];
char 			ShotNumberString[8];
TaskHandle 		OutputCardTrigHandle;	//Global variable that defines the master trigger call for the NI DAQ card