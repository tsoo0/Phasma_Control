#include "ACQ2106.h"

int CVICALLBACK PyConsoleUpdate (int panel, int control, int event,
								 void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:

			break;
	}
	return 0;
}

int CVICALLBACK acq2106_set_abort (int panel, int control, int event,
								   void *callbackData, int eventData1, int eventData2)
{
	
	char 		Triple_IP1[64];
	char 		Triple_IP2[64];
	char * arg1[] = {Triple_IP1};
	char * arg2[] = {Triple_IP2};
	//Triple_panel = LoadPanel (0, "TripleSettings.uir", Triple);

	GetCtrlVal (panel, Triple_acq2106_1_IP,Triple_IP1);
	GetCtrlVal (panel, Triple_acq2106_2_IP,Triple_IP2);	
	//DiscardPanel(Triple_panel);
	
	switch (event)
	{
		case EVENT_COMMIT:
			RunPython("set_abort",arg1,1);
			RunPython("set_abort",arg2,1);
			break;
	}
	return 0;
}

int CVICALLBACK acq2106_manual_arm (int panel, int control, int event,
								   void *callbackData, int eventData1, int eventData2)
{
	
	char 		Triple_IP1[64];
	char 		Triple_IP2[64];
	char * arg1[] = {Triple_IP1};
	char * arg2[] = {Triple_IP2};
	//Triple_panel = LoadPanel (0, "TripleSettings.uir", Triple);

	GetCtrlVal (panel, Triple_acq2106_1_IP,Triple_IP1);
	GetCtrlVal (panel, Triple_acq2106_2_IP,Triple_IP2);	
	
	
	switch (event)
	{
		case EVENT_COMMIT:
			RunPython("set_arm",arg1,1);
			RunPython("set_arm",arg2,1);
			break;
	}
	return 0;
}

int CVICALLBACK acq2106_soft_trig (int panel, int control, int event,
								   void *callbackData, int eventData1, int eventData2)
{
	
	char 		Triple_IP1[64];
	char 		Triple_IP2[64];
	char * arg1[] = {Triple_IP1};
	char * arg2[] = {Triple_IP2};
	//Triple_panel = LoadPanel (0, "TripleSettings.uir", Triple);

	GetCtrlVal (panel, Triple_acq2106_1_IP,Triple_IP1);
	GetCtrlVal (panel, Triple_acq2106_2_IP,Triple_IP2);	
	//DiscardPanel(Triple_panel);
	
	switch (event)
	{
		case EVENT_COMMIT:
			RunPython("set_trig",arg1,1);
			RunPython("set_trig",arg2,1);
			break;
	}
	return 0;
}

int CVICALLBACK acq2106_pull_data (int panel, int control, int event,
								   void *callbackData, int eventData1, int eventData2)
{	
	switch (event)
	{
		case EVENT_COMMIT:
			Write_TripleData();
			
			break;
	}
	return 0;
}

int CVICALLBACK acq2106_push_config (int panel, int control, int event,
									 void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
				TripleActivate();
			break;
	}
	return 0;
}

int CVICALLBACK ACQ2106_TRG_DELAY_SET (int panel, int control, int event,
									   void *callbackData, int eventData1, int eventData2)
{
	int 		delay_us;
	char 		Triple_IP1[64];
	char 		Triple_IP2[64];
	char 		delaystr[16];
	
	
	GetCtrlVal (panel, Triple_acq2106_1_IP,Triple_IP1);
	GetCtrlVal (panel, Triple_acq2106_2_IP,Triple_IP2);	
	GetCtrlVal (panel, Triple_ACQ2106_TRG_DELAY, &delay_us);
	sprintf(delaystr,"%d",delay_us);
	char * arg1[] = {Triple_IP1,delaystr};
	char * arg2[] = {Triple_IP2,delaystr};

	switch (event)
	{
		case EVENT_COMMIT:
				RunPython("set_delay",arg1,2);
				RunPython("set_delay",arg2,2);	
			break;
	}
	return 0;
}