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
#define  CLIF_PANEL_PlasmaSpecies         2       /* control type: slide, callback function: (none) */
#define  CLIF_PANEL_COMMANDBUTTON_3       3       /* control type: command, callback function: Laser_Shift_Down */
#define  CLIF_PANEL_COMMANDBUTTON_2       4       /* control type: command, callback function: Laser_Shift */
#define  CLIF_PANEL_sens_units            5       /* control type: ring, callback function: (none) */
#define  CLIF_PANEL_Z_Target              6       /* control type: numeric, callback function: (none) */
#define  CLIF_PANEL_Y_Target              7       /* control type: numeric, callback function: (none) */
#define  CLIF_PANEL_Z_Offset              8       /* control type: numeric, callback function: (none) */
#define  CLIF_PANEL_Z_Current             9       /* control type: numeric, callback function: (none) */
#define  CLIF_PANEL_Z_COM                 10      /* control type: ring, callback function: (none) */
#define  CLIF_PANEL_Y_COM                 11      /* control type: ring, callback function: (none) */
#define  CLIF_PANEL_X_COM                 12      /* control type: ring, callback function: (none) */
#define  CLIF_PANEL_X_Target              13      /* control type: numeric, callback function: (none) */
#define  CLIF_PANEL_Y_Offset              14      /* control type: numeric, callback function: (none) */
#define  CLIF_PANEL_Y_Current             15      /* control type: numeric, callback function: (none) */
#define  CLIF_PANEL_DAQ_RecordLength      16      /* control type: numeric, callback function: (none) */
#define  CLIF_PANEL_X_Offset              17      /* control type: numeric, callback function: (none) */
#define  CLIF_PANEL_X_Current             18      /* control type: numeric, callback function: (none) */
#define  CLIF_PANEL_sens_multiplier       19      /* control type: ring, callback function: (none) */
#define  CLIF_PANEL_units                 20      /* control type: ring, callback function: (none) */
#define  CLIF_PANEL_multiplier            21      /* control type: ring, callback function: (none) */
#define  CLIF_PANEL_Orientation           22      /* control type: ring, callback function: (none) */
#define  CLIF_PANEL_Home                  23      /* control type: command, callback function: XYZ_Home */
#define  CLIF_PANEL_MOVETOTARGETLOCATION  24      /* control type: command, callback function: XYZ_Move */
#define  CLIF_PANEL_UpdateSettings        25      /* control type: command, callback function: UpdateSettings */
#define  CLIF_PANEL_TEXTMSG2              26      /* control type: textMsg, callback function: (none) */
#define  CLIF_PANEL_CLOSELIFSYSTEM        27      /* control type: command, callback function: Close_CLIF_Settings */
#define  CLIF_PANEL_PERFORMLIFSCAN        28      /* control type: command, callback function: CLIF_Activate_Button */
#define  CLIF_PANEL_Chan3Name             29      /* control type: string, callback function: (none) */
#define  CLIF_PANEL_Chan2Name             30      /* control type: string, callback function: (none) */
#define  CLIF_PANEL_Chan1Name             31      /* control type: string, callback function: (none) */
#define  CLIF_PANEL_Machine               32      /* control type: ring, callback function: (none) */
#define  CLIF_PANEL_ChopperFreq           33      /* control type: numeric, callback function: (none) */
#define  CLIF_PANEL_Local_Bfield          34      /* control type: numeric, callback function: (none) */
#define  CLIF_PANEL_PMT                   35      /* control type: numeric, callback function: (none) */
#define  CLIF_PANEL_SweepTime             36      /* control type: numeric, callback function: (none) */
#define  CLIF_PANEL_timeconstant          37      /* control type: numeric, callback function: (none) */
#define  CLIF_PANEL_PIDModule_gain        38      /* control type: numeric, callback function: (none) */
#define  CLIF_PANEL_PIDModule_offset      39      /* control type: numeric, callback function: (none) */
#define  CLIF_PANEL_ContinLIF_Data        40      /* control type: binary, callback function: (none) */
#define  CLIF_PANEL_Burleigh_setpoint     41      /* control type: numeric, callback function: (none) */
#define  CLIF_PANEL_PIDModule_setpoint    42      /* control type: numeric, callback function: (none) */
#define  CLIF_PANEL_Sirah_Calibration     43      /* control type: numeric, callback function: (none) */
#define  CLIF_PANEL_SweepRange            44      /* control type: numeric, callback function: (none) */
#define  CLIF_PANEL_Deconvolve_active     45      /* control type: radioButton, callback function: (none) */
#define  CLIF_PANEL_DualLIFSwitch         46      /* control type: binary, callback function: (none) */
#define  CLIF_PANEL_WaveMeterSwitch       47      /* control type: binary, callback function: (none) */
#define  CLIF_PANEL_TEXTMSG_3             48      /* control type: textMsg, callback function: (none) */
#define  CLIF_PANEL_TEXTMSG_4             49      /* control type: textMsg, callback function: (none) */
#define  CLIF_PANEL_TEXTMSG_2             50      /* control type: textMsg, callback function: (none) */
#define  CLIF_PANEL_TEXTMSG_5             51      /* control type: textMsg, callback function: (none) */
#define  CLIF_PANEL_TEXTMSG_17            52      /* control type: textMsg, callback function: (none) */
#define  CLIF_PANEL_TEXTMSG_9             53      /* control type: textMsg, callback function: (none) */
#define  CLIF_PANEL_TEXTMSG_8             54      /* control type: textMsg, callback function: (none) */
#define  CLIF_PANEL_LIF_PLOT              55      /* control type: graph, callback function: (none) */
#define  CLIF_PANEL_Reserve               56      /* control type: slide, callback function: (none) */
#define  CLIF_PANEL_Slope                 57      /* control type: slide, callback function: (none) */
#define  CLIF_PANEL_sensitivity           58      /* control type: slide, callback function: (none) */
#define  CLIF_PANEL_TEXTMSG_16            59      /* control type: textMsg, callback function: (none) */
#define  CLIF_PANEL_TEXTMSG               60      /* control type: textMsg, callback function: (none) */
#define  CLIF_PANEL_InfoString            61      /* control type: textMsg, callback function: (none) */
#define  CLIF_PANEL_TEXTMSG_11            62      /* control type: textMsg, callback function: (none) */
#define  CLIF_PANEL_TEXTMSG_12            63      /* control type: textMsg, callback function: (none) */
#define  CLIF_PANEL_TEXTMSG_13            64      /* control type: textMsg, callback function: (none) */
#define  CLIF_PANEL_TEXTMSG_14            65      /* control type: textMsg, callback function: (none) */
#define  CLIF_PANEL_SPLITTER_7            66      /* control type: splitter, callback function: (none) */
#define  CLIF_PANEL_SPLITTER_6            67      /* control type: splitter, callback function: (none) */
#define  CLIF_PANEL_SPLITTER_2            68      /* control type: splitter, callback function: (none) */
#define  CLIF_PANEL_SIRAH_IP              69      /* control type: ring, callback function: (none) */
#define  CLIF_PANEL_SPLITTER              70      /* control type: splitter, callback function: (none) */
#define  CLIF_PANEL_LockInGPIB2           71      /* control type: ring, callback function: (none) */
#define  CLIF_PANEL_LockInGPIB1           72      /* control type: ring, callback function: (none) */
#define  CLIF_PANEL_Wavemeter_COM         73      /* control type: ring, callback function: (none) */
#define  CLIF_PANEL_RigolFunct            74      /* control type: ring, callback function: (none) */
#define  CLIF_PANEL_ThorChopperCOM        75      /* control type: ring, callback function: (none) */
#define  CLIF_PANEL_SPLITTER_4            76      /* control type: splitter, callback function: (none) */
#define  CLIF_PANEL_SPLITTER_5            77      /* control type: splitter, callback function: (none) */
#define  CLIF_PANEL_SPLITTER_3            78      /* control type: splitter, callback function: (none) */
#define  CLIF_PANEL_AOMSwitch             79      /* control type: binary, callback function: (none) */
#define  CLIF_PANEL_COMMANDBUTTON_4       80      /* control type: command, callback function: Wavemeter_Call */
#define  CLIF_PANEL_MDS_Shot              81      /* control type: numeric, callback function: (none) */
#define  CLIF_PANEL_TEXTMSG_6             82      /* control type: textMsg, callback function: (none) */
#define  CLIF_PANEL_Ti2                   83      /* control type: numeric, callback function: (none) */
#define  CLIF_PANEL_CenterShift2          84      /* control type: numeric, callback function: (none) */
#define  CLIF_PANEL_CenterShift1          85      /* control type: numeric, callback function: (none) */
#define  CLIF_PANEL_Ti1                   86      /* control type: numeric, callback function: (none) */
#define  CLIF_PANEL_Zeeman_Shift          87      /* control type: numeric, callback function: (none) */
#define  CLIF_PANEL_TEXTMSG_10            88      /* control type: textMsg, callback function: (none) */
#define  CLIF_PANEL_TEXTMSG_18            89      /* control type: textMsg, callback function: (none) */
#define  CLIF_PANEL_TEXTMSG_19            90      /* control type: textMsg, callback function: (none) */
#define  CLIF_PANEL_TEXTMSG_20            91      /* control type: textMsg, callback function: (none) */
#define  CLIF_PANEL_TEXTMSG_21            92      /* control type: textMsg, callback function: (none) */
#define  CLIF_PANEL_TEXTMSG_24            93      /* control type: textMsg, callback function: (none) */
#define  CLIF_PANEL_TEXTMSG_25            94      /* control type: textMsg, callback function: (none) */
#define  CLIF_PANEL_TEXTMSG_27            95      /* control type: textMsg, callback function: (none) */
#define  CLIF_PANEL_WAVEMETER_STRING      96      /* control type: textBox, callback function: (none) */
#define  CLIF_PANEL_TEXTMSG_26            97      /* control type: textMsg, callback function: (none) */
#define  CLIF_PANEL_SPLITTER_10           98      /* control type: splitter, callback function: (none) */
#define  CLIF_PANEL_SPLITTER_9            99      /* control type: splitter, callback function: (none) */
#define  CLIF_PANEL_Iodine                100     /* control type: textMsg, callback function: (none) */
#define  CLIF_PANEL_LIF1                  101     /* control type: textMsg, callback function: (none) */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK CLIF_Activate_Button(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Close_CLIF_Settings(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Laser_Shift(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Laser_Shift_Down(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK UpdateSettings(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Wavemeter_Call(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK XYZ_Home(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK XYZ_Move(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif