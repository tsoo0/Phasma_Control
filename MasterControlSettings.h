//Flags that determine if data records will be saved to plotted on each shot
int		HousekeepingPlotFlag=0;
int		BdotPlotFlag=0;
int		TripleProbePlotFlag=0;
int		DoubleProbePlotFlag=0;
int		TSPlotFlag=0;
int		PulsedLIFPlotFlag=0;
int		QuantumBeatPlotFlag=0;
int		PhotodiodePlotFlag=0;
int		PhotronCameraPlotFlag=0;
int		OceanOpticsPlotFlag=0;
int		McPherson209PlotFlag=0;
int		LightwvIntferomPlotFlag=0;
int		RFEAPlotFlag=0;
int		TemporaryDiagPlotFlag=0;
int		MatlabStatusFlag=0;

//Flags that determine if data records will be saved to MDSPlus on each shot
int		MagFieldDataFlag=0;
int		HeliconDataFlag=0;
int		HousekeepingDataFlag=0;
int		PlasmaGunDataFlag=0;
int		Write_PlasmaGunDataData=0;
int		BdotDataFlag=0;
int		TripleProbeDataFlag=0;
int		DoubleProbeDataFlag=0;
int		TSDataFlag=0;
int		PulsedLIFDataFlag=0;
int		QuantumBeatDataFlag=0;
int		LangmuirProbeDataFlag=0;
int		PhotodiodeDataFlag=0;
int		PhotronCameraDataFlag=0;
int		OceanOpticsDataFlag=0;
int		McPherson209DataFlag=0;
int		LightwvIntferomDataFlag=0;
int		RFEADataFlag=0;
int		TemporaryDiagDataFlag=0;





void	Repeat_Cycle (void);


/*********************************************************************************************************
*	Here are all the callback functions from the Main Control Loop Panel that determine if a particular 
*	data record will be sent to MDSPlus for storage
*
*
*********************************************************************************************************/
int CVICALLBACK MagField_Data_Switch (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			MagFieldDataFlag=1;
			break;
	}
	return 0;
}

int CVICALLBACK HeliconData_Switch (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			HeliconDataFlag=1; 
			break;
	}
	return 0;
}

int CVICALLBACK HousekeepingData_Switch (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			HousekeepingDataFlag=1; 
			break;
	}
	return 0;
}

int CVICALLBACK MagProbeData_Switch (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
 			BdotDataFlag=1; 

			break;
	}
	return 0;
}

int CVICALLBACK RFEAData_Switch (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			RFEADataFlag=1;
			break;
	}
	return 0;
}

int CVICALLBACK PhotronCameraData_Switch (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			PhotronCameraDataFlag=1;
			break;
	}
	return 0;
}


int CVICALLBACK TempDiagData_Switch (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			TemporaryDiagDataFlag=1; 
			break;
	}
	return 0;
}

int CVICALLBACK DPData_Switch (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			DoubleProbeDataFlag=1; 
			break;
	}
	return 0;
}

int CVICALLBACK TSData_Switch (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			TSDataFlag=1; 
			break;
	}
	return 0;
}

int CVICALLBACK PhotodiodeData_Switch (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			PhotodiodeDataFlag=1; 
			break;
	}
	return 0;
}

int CVICALLBACK TripleProbeData_Switch (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			TripleProbeDataFlag=1; 
			break;
	}
	return 0;
}

int CVICALLBACK McPherson209Data_Switch (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			McPherson209DataFlag=1; 
			break;
	}
	return 0;
}

int CVICALLBACK PulsedLIFData_Switch (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			PulsedLIFDataFlag=1; 
			break;
	}
	return 0;
}



/*********************************************************************************************************
*	Here are all the callback functions from the Main Control Loop Panel that determine if a particular 
*	diagnostic will have its raw data plotted
*
*
*********************************************************************************************************/


int CVICALLBACK HousekeepingPlot_Switch (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			HousekeepingPlotFlag=1; 
			break;
	}
	return 0;
}

int CVICALLBACK MagProbePlot_Switch (int panel, int control, int event,
		void *callbackPlot, int eventPlot1, int eventPlot2)
{
	switch (event)
	{
		case EVENT_COMMIT:
 			BdotPlotFlag=1; 
			break;
	}
	return 0;
}

int CVICALLBACK RFEAPlot_Switch (int panel, int control, int event,
		void *callbackPlot, int eventPlot1, int eventPlot2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			RFEAPlotFlag=1;
			break;
	}
	return 0;
}

int CVICALLBACK PhotronCameraPlot_Switch (int panel, int control, int event,
		void *callbackPlot, int eventPlot1, int eventPlot2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			PhotronCameraPlotFlag=1;
			break;
	}
	return 0;
}


int CVICALLBACK TempDiagPlot_Switch (int panel, int control, int event,
		void *callbackPlot, int eventPlot1, int eventPlot2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			TemporaryDiagPlotFlag=1; 
			break;
	}
	return 0;
}

int CVICALLBACK DPDPlot_Switch (int panel, int control, int event,
		void *callbackPlot, int eventPlot1, int eventPlot2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			DoubleProbePlotFlag=1; 
			break;
	}
	return 0;
}

int CVICALLBACK TSPlot_Switch (int panel, int control, int event,
		void *callbackPlot, int eventPlot1, int eventPlot2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			TSPlotFlag=1; 
			break;
	}
	return 0;
}

int CVICALLBACK PhotodiodePlot_Switch (int panel, int control, int event,
		void *callbackPlot, int eventPlot1, int eventPlot2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			PhotodiodePlotFlag=1; 
			break;
	}
	return 0;
}

int CVICALLBACK TripleProbePlot_Switch (int panel, int control, int event,
		void *callbackPlot, int eventPlot1, int eventPlot2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			TripleProbePlotFlag=1; 
			break;
	}
	return 0;
}

int CVICALLBACK McPherson209Plot_Switch (int panel, int control, int event,
		void *callbackPlot, int eventPlot1, int eventPlot2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			McPherson209PlotFlag=1; 
			break;
	}
	return 0;
}

int CVICALLBACK PulsedLIFPlot_Switch (int panel, int control, int event,
		void *callbackPlot, int eventPlot1, int eventPlot2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			PulsedLIFPlotFlag=1; 
			break;
	}
	return 0;
}
