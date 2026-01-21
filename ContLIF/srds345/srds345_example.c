/***************************************************************************** 
 *  Copyright 1998-2010 National Instruments Corporation.  All Rights Reserved* 
 *****************************************************************************/

/*****************************************************************************
 *  FilePrefix Instrument Driver Sample Program                              
 *  Developed with LabWindows/CVI 5.1                                     
 *  Original Release: 12/15/98                                  
 *
 *  Purpose:  This sample program initializes the function generator, configures
 *            it to output a standard waveform, and displays a message 
 *            pop-up dialog.
 *
 *            To run this sample program, do the following:
 *            1) Create a new project in LabWindows/CVI.
 *            2) Add this file to the project.  To add files to the project,
 *               select the Add Files To Project from the Edit menu of the 
 *               Project window.
 *            3) Add the .fp file for the srds345 instrument driver to the 
 *               project.
 *            4) Check that the resource name used in srds345_InitWithOptions()
 *               is correct. If simulating, change flag to simulate.
 *            5) Run the project.
 * [PP] Modified 02/22/00 to comply with Driver v2.0
 *****************************************************************************/

#include <ansi_c.h>
#include <userint.h>
#include "srds345.h"

main ()

{
    ViSession   srds345;
    ViStatus    error = VI_SUCCESS;
    ViString    popUpMsg = "The function generator is outputting the waveform "  
                           "that you configured.\n\nPress <Enter> to continue";

        /* 
            If you want to run this sample program and the instrument is not
            present, set the Simulate flag to 1.  (Example:  "Simulate = 1")
        */
    checkErr( srds345_InitWithOptions ("GPIB::19::INSTR", VI_TRUE, VI_TRUE,
                                        "Simulate=0,RangeCheck=1,QueryInstrStatus=0,Cache=1",
                                        &srds345));

    checkErr( srds345_ConfigureOutputMode (srds345, SRDS345_VAL_OUTPUT_FUNC));
    checkErr( srds345_ConfigureRefClockSource (srds345, SRDS345_VAL_REF_CLOCK_INTERNAL));
    checkErr( srds345_ConfigureOutputImpedance (srds345, "1", 50));
    checkErr( srds345_ConfigureStandardWaveform (srds345, "1", SRDS345_VAL_WFM_SINE,
                                                 1.0, 0.0, 1.00e3, 0.0));
                                                  
    checkErr( srds345_ConfigureOutputEnabled (srds345, "1", VI_TRUE));
    checkErr( srds345_InitiateGeneration (srds345));
                                                  
    MessagePopup ("Message", popUpMsg);
                  
    checkErr( srds345_AbortGeneration (srds345));
                                        
Error:
    if (error != VI_SUCCESS)
        {
            ViChar   errStr[2048];

            srds345_GetError (srds345, &error, 2048, errStr);
            MessagePopup ("Error!", errStr);
        }

    if (srds345)
        srds345_close (srds345);
}
