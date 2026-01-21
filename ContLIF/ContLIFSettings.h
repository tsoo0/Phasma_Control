/**************************************************************************/
/* LabWindows/CVI User Interface Resource (UIR) Include File              */
/*                                                                        */
/* WARNING: Do not add to, delete from, or otherwise modify the contents  */
/*          of this include file.                                         */
/**************************************************************************/

#include <userint.h>

#ifdef __cplusplus
    extern "C" {
#endif

     /* Panels and Controls: */

#define  CLIF_PANEL                       1
#define  CLIF_PANEL_COMMANDBUTTON_3       2       /* control type: command, callback function: Laser_Shift_Down */
#define  CLIF_PANEL_COMMANDBUTTON_2       3       /* control type: command, callback function: Laser_Shift */
#define  CLIF_PANEL_sens_units            4       /* control type: ring, callback function: (none) */
#define  CLIF_PANEL_Z_Target              5       /* control type: numeric, callback function: (none) */
#define  CLIF_PANEL_Y_Target              6       /* control type: numeric, callback function: (none) */
#define  CLIF_PANEL_Z_Offset              7       /* control type: numeric, callback function: (none) */
#define  CLIF_PANEL_Z_Current             8       /* control type: numeric, callback function: (none) */
#define  CLIF_PANEL_Z_COM                 9       /* control type: ring, callback function: (none) */
#define  CLIF_PANEL_Y_COM                 10      /* control type: ring, callback function: (none) */
#define  CLIF_PANEL_X_COM                 11      /* control type: ring, callback function: (none) */
#define  CLIF_PANEL_X_Target              12      /* control type: numeric, callback function: (none) */
#define  CLIF_PANEL_Y_Offset              13      /* control type: numeric, callback function: (none) */
#define  CLIF_PANEL_Y_Current             14      /* control type: numeric, callback function: (none) */
#define  CLIF_PANEL_DAQ_RecordLength      15      /* control type: numeric, callback function: (none) */
#define  CLIF_PANEL_X_Offset              16      /* control type: numeric, callback function: (none) */
#define  CLIF_PANEL_X_Current             17      /* control type: numeric, callback function: (none) */
#define  CLIF_PANEL_sens_multiplier       18      /* control type: ring, callback function: (none) */
#define  CLIF_PANEL_units                 19      /* control type: ring, callback function: (none) */
#define  CLIF_PANEL_multiplier            20      /* control type: ring, callback function: (none) */
#define  CLIF_PANEL_Orientation           21      /* control type: ring, callback function: (none) */
#define  CLIF_PANEL_Home                  22      /* control type: command, callback function: XYZ_Home */
#define  CLIF_PANEL_MOVETOTARGETLOCATION  23      /* control type: command, callback function: XYZ_Move */
#define  CLIF_PANEL_CLOSELIFSYSTEM        24      /* control type: command, callback function: Close_CLIF_Settings */
#define  CLIF_PANEL_PERFORMLIFSCAN        25      /* control type: command, callback function: CLIF_Activate_Button */
#define  CLIF_PANEL_Chan3Name             26      /* control type: string, callback function: (none) */
#define  CLIF_PANEL_Chan2Name             27      /* control type: string, callback function: (none) */
#define  CLIF_PANEL_Chan1Name             28      /* control type: string, callback function: (none) */
#define  CLIF_PANEL_Machine               29      /* control type: ring, callback function: (none) */
#define  CLIF_PANEL_ChopperFreq           30      /* control type: numeric, callback function: (none) */
#define  CLIF_PANEL_Local_Bfield          31      /* control type: numeric, callback function: (none) */
#define  CLIF_PANEL_PMT                   32      /* control type: numeric, callback function: (none) */
#define  CLIF_PANEL_SweepTime             33      /* control type: numeric, callback function: (none) */
#define  CLIF_PANEL_timeconstant          34      /* control type: numeric, callback function: (none) */
#define  CLIF_PANEL_PIDModule_gain        35      /* control type: numeric, callback function: (none) */
#define  CLIF_PANEL_PIDModule_offset      36      /* control type: numeric, callback function: (none) */
#define  CLIF_PANEL_ContinLIF_Data        37      /* control type: binary, callback function: (none) */
#define  CLIF_PANEL_Burleigh_setpoint     38      /* control type: numeric, callback function: (none) */
#define  CLIF_PANEL_PIDModule_setpoint    39      /* control type: numeric, callback function: (none) */
#define  CLIF_PANEL_Sirah_Calibration     40      /* control type: numeric, callback function: (none) */
#define  CLIF_PANEL_SweepRange            41      /* control type: numeric, callback function: (none) */
#define  CLIF_PANEL_Deconvolve_active     42      /* control type: radioButton, callback function: (none) */
#define  CLIF_PANEL_DualLIFSwitch         43      /* control type: binary, callback function: (none) */
#define  CLIF_PANEL_WaveMeterSwitch       44      /* control type: binary, callback function: (none) */
#define  CLIF_PANEL_TEXTMSG_3             45      /* control type: textMsg, callback function: (none) */
#define  CLIF_PANEL_TEXTMSG_4             46      /* control type: textMsg, callback function: (none) */
#define  CLIF_PANEL_TEXTMSG_2             47      /* control type: textMsg, callback function: (none) */
#define  CLIF_PANEL_TEXTMSG_5             48      /* control type: textMsg, callback function: (none) */
#define  CLIF_PANEL_TEXTMSG_17            49      /* control type: textMsg, callback function: (none) */
#define  CLIF_PANEL_TEXTMSG_9             50      /* control type: textMsg, callback function: (none) */
#define  CLIF_PANEL_TEXTMSG_8             51      /* control type: textMsg, callback function: (none) */
#define  CLIF_PANEL_LIF_PLOT              52      /* control type: graph, callback function: (none) */
#define  CLIF_PANEL_Reserve               53      /* control type: slide, callback function: (none) */
#define  CLIF_PANEL_Slope                 54      /* control type: slide, callback function: (none) */
#define  CLIF_PANEL_sensitivity           55      /* control type: slide, callback function: (none) */
#define  CLIF_PANEL_TEXTMSG_16            56      /* control type: textMsg, callback function: (none) */
#define  CLIF_PANEL_TEXTMSG               57      /* control type: textMsg, callback function: (none) */
#define  CLIF_PANEL_InfoString            58      /* control type: textMsg, callback function: (none) */
#define  CLIF_PANEL_TEXTMSG_11            59      /* control type: textMsg, callback function: (none) */
#define  CLIF_PANEL_TEXTMSG_12            60      /* control type: textMsg, callback function: (none) */
#define  CLIF_PANEL_TEXTMSG_13            61      /* control type: textMsg, callback function: (none) */
#define  CLIF_PANEL_TEXTMSG_14            62      /* control type: textMsg, callback function: (none) */
#define  CLIF_PANEL_SPLITTER_7            63      /* control type: splitter, callback function: (none) */
#define  CLIF_PANEL_SPLITTER_6            64      /* control type: splitter, callback function: (none) */
#define  CLIF_PANEL_SPLITTER_2            65      /* control type: splitter, callback function: (none) */
#define  CLIF_PANEL_SIRAH_IP              66      /* control type: ring, callback function: (none) */
#define  CLIF_PANEL_SPLITTER              67      /* control type: splitter, callback function: (none) */
#define  CLIF_PANEL_LockInGPIB2           68      /* control type: ring, callback function: (none) */
#define  CLIF_PANEL_LockInGPIB1           69      /* control type: ring, callback function: (none) */
#define  CLIF_PANEL_Wavemeter_COM         70      /* control type: ring, callback function: (none) */
#define  CLIF_PANEL_RigolFunct            71      /* control type: ring, callback function: (none) */
#define  CLIF_PANEL_ThorChopperCOM        72      /* control type: ring, callback function: (none) */
#define  CLIF_PANEL_SPLITTER_4            73      /* control type: splitter, callback function: (none) */
#define  CLIF_PANEL_SPLITTER_5            74      /* control type: splitter, callback function: (none) */
#define  CLIF_PANEL_SPLITTER_3            75      /* control type: splitter, callback function: (none) */
#define  CLIF_PANEL_AOMSwitch             76      /* control type: binary, callback function: (none) */
#define  CLIF_PANEL_COMMANDBUTTON_4       77      /* control type: command, callback function: Wavemeter_Call */
#define  CLIF_PANEL_MDS_Shot              78      /* control type: numeric, callback function: (none) */
#define  CLIF_PANEL_TEXTMSG_6             79      /* control type: textMsg, callback function: (none) */
#define  CLIF_PANEL_Ti2                   80      /* control type: numeric, callback function: (none) */
#define  CLIF_PANEL_CenterShift2          81      /* control type: numeric, callback function: (none) */
#define  CLIF_PANEL_CenterShift1          82      /* control type: numeric, callback function: (none) */
#define  CLIF_PANEL_Ti1                   83      /* control type: numeric, callback function: (none) */
#define  CLIF_PANEL_Zeeman_Shift          84      /* control type: numeric, callback function: (none) */
#define  CLIF_PANEL_TEXTMSG_10            85      /* control type: textMsg, callback function: (none) */
#define  CLIF_PANEL_TEXTMSG_18            86      /* control type: textMsg, callback function: (none) */
#define  CLIF_PANEL_TEXTMSG_19            87      /* control type: textMsg, callback function: (none) */
#define  CLIF_PANEL_TEXTMSG_20            88      /* control type: textMsg, callback function: (none) */
#define  CLIF_PANEL_TEXTMSG_21            89      /* control type: textMsg, callback function: (none) */
#define  CLIF_PANEL_TEXTMSG_24            90      /* control type: textMsg, callback function: (none) */
#define  CLIF_PANEL_TEXTMSG_25            91      /* control type: textMsg, callback function: (none) */
#define  CLIF_PANEL_TEXTMSG_27            92      /* control type: textMsg, callback function: (none) */
#define  CLIF_PANEL_TEXTMSG_26            93      /* control type: textMsg, callback function: (none) */
#define  CLIF_PANEL_WAVEMETER_STRING      94      /* control type: textBox, callback function: (none) */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK CLIF_Activate_Button(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Close_CLIF_Settings(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Laser_Shift(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Laser_Shift_Down(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Wavemeter_Call(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK XYZ_Home(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK XYZ_Move(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif