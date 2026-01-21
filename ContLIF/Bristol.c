
 /***************************************************************************		
 * WVU Software to Communicate with the Bristol Wavemeter                   *
 *																			*
 * Written by       Earl Scime												*
 *																			*
 * 					This code is to obtain wavelength data from the Bristol *
 *                  621A-VIS wavemeter									    *
 *                  														*
 *																			*
 * 2/15/2010		Code started											*
 * 3/21/2012		Complete rewrite of code to use autobroadcast feature   *
 *					and multithreading.  E. Scime and J. Carr Jr.			*
 * 11/23/2015		Modified to include an "uber counter" to stop infinite  *
 *					loops when the laser is very unstable. Count of 100     *
 *  																		*
 ****************************************************************************/		
 
 /***************************************************************************		
 * To deal with the lack of 64-bit libraries from Bristol for the 621 		*
 * wavemeter, we use some help from LabWindows and use Explicit linking to 	*
 * the 64-bit .dll file that is available. The technique is described below.*
 
 Calling a DLL using Explicit Linking in LabWindows/CVI
In order to call a DLL dynamically, we use the Windows SDK functions. 
This process is very similar in other languages/environments as well.

Define the function pointer using typedef:
First, we typedef the function pointer to match the prototype (parameters 
and return type) of the exported function. This lets us call the function 
with parameters later on.

For more information on typedef, please refer to: Wikipedia: typedef
Load the DLL into memory:
Use the LoadLibrary function (part of the Windows SDK, prototyped in windows.h) 
to load the DLL into memory. LoadLibrary returns a handle to the DLL.
Note: Loading a DLL using LoadLibrary will cause the DLLMain function in the 
DLL to execute. 

Get a reference to the exported function:
Use the GetProcAddress function (part of the Windows SDK, prototyped 
in windows.h) to get the memory address of the function in the loaded 
DLL. We assign this to a function pointer.

Call the function using the function pointer:
Use the function pointer that you assigned in the previous step to call 
the exported function. You can pass in parameters and get return values 
as with a regular function since we typedef'ed it earlier.

#include <ansi_c.h>
//Include windows.h to call the DLL dynamically. windows.h contains LoadLibrary 
//and GetProcAddress .
#include <windows.h>

//Typedef the pointer to the exported function so you can call it easily later
//The function returns an int and has two inputs, an int and a string
typedef int (*MYPROC)(int, char*);

int main ()
{

int number = 5;
char* string = "Hello";
int returnValue;

HINSTANCE hinstLib; //Handle to the DLL
MYPROC ProcAddress; //Pointer to the function

hinstLib = LoadLibrary("CLDevlFace.dll");
//The the pointer to the exported function and typecast it so that we can easily call it
//MYPROC is typedef'ed above
//GetProcAddress is part of the Windows SDK and is declared in windows.h
ProcAddress = (MYPROC) GetProcAddress(hinstLib, "ShowMyNumberAndString");

//Call the function using the function pointer
returnValue = (ProcAddress)(number, string);

return 0;

}
 
 *  																		*
 ****************************************************************************/		

/****************************************************************************
If this approach does not work, eliminate the spawning of the thread and
just revert to doing direct calls to the wavemeter to read values and
use those in the PID routines.
****************************************************************************/



#include <windows.h>  // for Sleep() -- Windows-specific 
#include <ansi_c.h>
#include <utility.h>
#include <stdio.h>
#include "Bristol.h"


float	 	update_power;
double	 	update_wavelength;
int		 	update_index;
int		 	start_index;
int			BristolHandle; 

//Define the 3 functions in the DLL that will be needed
typedef int (*CLOpenUSBSerialDevice)(int);
typedef int (*CLSetMeasHBCallback) (int, void *);
typedef int (*CLSetAutoSend) (int, int);
typedef int (*CLCloseDevice) (int);

//Define the thread function that gets spawned to run in the background
static int 	CVICALLBACK Bristol_ThreadFunction1 (void *functionData);

HINSTANCE hinstLib; //Handle to the DLL
	
CLOpenUSBSerialDevice ProcAddress1; //Pointer to the 1st function
CLSetMeasHBCallback ProcAddress2; //Pointer to the 2nd function
CLSetAutoSend ProcAddress3; //Pointer to the 3rd function
CLCloseDevice ProcAddress4; //Pointer to the 4th function

typedef struct
{
    unsigned int    ScanIndex;
    unsigned int    Status;
    unsigned int    RefFringeCnt;
    unsigned int    InpFringeCnt;
    float           StartPhase;
    float           EndPhase;
    float           Temperature;
    float           Pressure;
    unsigned short  RefPower1;
    unsigned short  RefPower2;
    float           InputPower;
    double          Wavelength;
} tsMeasurementDataType;


// Measurement callback
void HandleMeasurementData(tsMeasurementDataType  *data)
{
   printf("Scan Index: %d, Status %08x, RefFringeCnt: %d, InpFringeCnt: %d\n",
         data->ScanIndex, data->Status, data->RefFringeCnt, data->InpFringeCnt);
   printf("\tStartPhase: %3.3e, EndPhase: %3.3f, Temperature: %3.3f, "
         "Pressure %3.3f\n",
         data->StartPhase, data->EndPhase, data->Temperature, data->Pressure);
   printf("\tRefPower1: %d, RefPower2: %d, InputPower: %3.3f, "
         "Wavelength: %9.4lf\n",
         data->RefPower1, data->RefPower2, data->InputPower, data->Wavelength);
   update_power = data->InputPower;
   update_wavelength = data->Wavelength;
   update_index = data->ScanIndex;
   
}
  

void CVICALLBACK Bristol_shutdown (CmtThreadPoolHandle poolHandle, CmtThreadFunctionID functionID, unsigned int event, int value, void *callbackData)
{
int	result;
	//Close Wavemeter when shutdown
	//CLCloseDevice(BristolHandle);	
	result = (ProcAddress4)(BristolHandle);		//Call the function using the function pointer 
       
	// Don't forget to free the library
  	FreeLibrary(hinstLib);
}




void Bristol_Wavelength(int	CommPort, int	averages, double *Wavelen, float *Power) 
{
   	int 	result;
    int		i;
	int		counter, second_counter; 
	int		modehop_flag;
	int		BristolCommFlag=0;	
 	float	Bristol_power;
	double	Bristol_wavelength;
	double	stored_wavelength=0.0;

	
	//Dynamically load in the 64-bit Bristol dll  
	SetLastError(0);
	hinstLib = LoadLibrary("CLDevIFace.dll");
	
	//check that the loading of the dll happened correctly
	if (hinstLib == NULL) {
        DWORD errCode = GetLastError();
        printf("Failed to load DLL. Error code: %lu\n", errCode);
    } else {
        printf("DLL loaded successfully.\n");
    }

	//The the pointer to the exported function and typecast it so that we can easily call it
	//MYPROC is typedef'ed above
	//GetProcAddress is part of the Windows SDK and is declared in windows.h
	ProcAddress1 = (CLOpenUSBSerialDevice) GetProcAddress(hinstLib, "CLOpenUSBSerialDevice");
	ProcAddress2 = (CLSetMeasHBCallback) GetProcAddress(hinstLib, "CLSetMeasHBCallback");
	ProcAddress3 = (CLSetAutoSend) GetProcAddress(hinstLib, "CLSetAutoSend");
	ProcAddress4 = (CLCloseDevice) GetProcAddress(hinstLib, "CLCloseDevice");

	//reset start index to check if updates being done
	start_index = update_index = -1;  


	//Open communications to the wavemeter
	if ( BristolCommFlag < 1) {
		
		//Open comm port to wavemeter if not already opened
		//BristolHandle = CLOpenUSBSerialDevice(CommPort);
		BristolHandle = (ProcAddress1)(CommPort);		//Call the function using the function pointer 
	
		//Turn on Comm Port Flag
		BristolCommFlag = 1;
		
		//Turn on thread completion variable
		Bristol_gExiting=0;
		
		//Spawn the thread to start up the wavemeter to run in broadcast mode and define a function to call when the thread ends
		CmtScheduleThreadPoolFunctionAdv (DEFAULT_THREAD_POOL_HANDLE, Bristol_ThreadFunction1, NULL, THREAD_PRIORITY_NORMAL, Bristol_shutdown,
										  EVENT_TP_THREAD_FUNCTION_END, NULL, CmtGetCurrentThreadID(), NULL);
	
		//Wait a little bit to let spawned thread start up
		Sleep(.1);
   	}
	
	//If Wavemeter communications set up properly above, query for data
	if (BristolHandle > -1) {
	   
		//reset global values to zero to avoid errors
		Bristol_wavelength = 0.0;
	    Bristol_power = 0.0;
	
		// wait to confirm that updates of the wavemeter values are occuring, then proceed with measurements
		while (update_index == -1)
	    		start_index = update_index;

	  	//flag to control loop resetting
		modehop_flag=0;
		
		//restart counter that looks for getting really trapped
		second_counter=0;
		
		//If auto measurement process started properly, start looking at scan index and every time it
		//increments by at least one, use the new wavelength measurement in our averaging process
		//If measurements change too much between measurements, restart measurement process
		for (i=0;i<averages;i++) {
	
			//Check to see if recent, but not too large hop happened. If so, reset measurement cycle and try again.
			if (modehop_flag==-1) {
				modehop_flag = 0;
				i=0;
			}
			
			//If really trapped in this measurment, kick all the way out after 100 cycles
			second_counter=second_counter+1;
			if (second_counter > 100) { 
				i=averages-1;
				Bristol_wavelength = 0.0;
		    	Bristol_power = 0.0;
			}
			
			//One first measurement or not?
			if (i > 0)   {
				
				//If a mode hop, check to see how big.
				if (fabs(update_wavelength-stored_wavelength) > 0.003)   {
					
					//if not a massive hop and unstable restart averaging cycle and try again, otherwise kick all the way out
					if (fabs(update_wavelength-stored_wavelength) < 0.1) {
						modehop_flag=-1;
						Bristol_wavelength = 0.0;
		    			Bristol_power = 0.0; 
						stored_wavelength = update_wavelength;						 
						i=0;
					}
					else {   //for large hop, kick out to end of loop
						i=averages-1;
						Bristol_wavelength = 0.0;
		    			Bristol_power = 0.0; 
					}
				} else {  //Do the averaging	
					Bristol_wavelength += update_wavelength;
					Bristol_power += update_power;
					stored_wavelength = update_wavelength;
				}
			} else  {   //first time through
				Bristol_wavelength += update_wavelength;
				Bristol_power += update_power;
				stored_wavelength = update_wavelength;
			}	
				
			//Make sure next measurement available	
			counter = 0;
			do  {
				//do absolutely nothing until new measurement has occurred but make sure code doesn't get into infinite loop
				Delay(.05);			 //Delay for 100 ms to reflect maximum update rate of 250 ms of Wavemeter
				counter = counter +1;
			} while ((update_index < start_index+1) && (counter < 20));
  		
			//update index to be newest value for checking progress later
			start_index=update_index;
		}  //end of for loop for averaging
                        
		//Take sums of values and average them
		Bristol_wavelength = Bristol_wavelength/averages;
	    Bristol_power = Bristol_power/averages;

	    //Transfer values to pointers
		*Wavelen = Bristol_wavelength;
		*Power = Bristol_power;
	}
	else {
		//Transfer zero values to pointers if communications did not open
		*Wavelen = 0.0;
		*Power = 0.0;
	}
	
	//Shut down parallel Bristol thread
	Bristol_gExiting=1;
}



/* Second thread function */
static int CVICALLBACK Bristol_ThreadFunction1 (void *functionData)
{   

	/* 	This function just starts up a thread for the Bristol wavemeter to work in while the other thread monitors the
	    measured values for a set number of averages
		
		Created 	3/21/2012	E. Scime	*/

    int		result;
	
	//Define function for wavemeter data to call each time wavemeter broadcasts data
 	//CLSetMeasHBCallback(BristolHandle, HandleMeasurementData);
 	result = (ProcAddress2)(BristolHandle, HandleMeasurementData);		//Call the function using the function pointer 

	
	//Turn on broadcasting
	//result = CLSetAutoSend(BristolHandle, 2);
 	result = (ProcAddress3)(BristolHandle, 2);		//Call the function using the function pointer 

	while (!Bristol_gExiting)
    {
		//Do nothing, just keep broadcasting until thread told to end by main code
	}
		
	//Stop broadcasting
	//result = CLSetAutoSend(BristolHandle, 0);
	result = (ProcAddress3)(BristolHandle, 0);		//Call the function using the function pointer 

	//return to Bristol code
    return 0;
}



