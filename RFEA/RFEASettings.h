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

#define  RFEA                             1
#define  RFEA_RIGOL_RFEAAddress           2       /* control type: ring, callback function: (none) */
#define  RFEA_Probe_IP                    3       /* control type: ring, callback function: (none) */
#define  RFEA_SettingsVoltageRange        4       /* control type: numeric, callback function: (none) */
#define  RFEA_SettingsTriggerLevel        5       /* control type: numeric, callback function: (none) */
#define  RFEA_R_Start                     6       /* control type: numeric, callback function: (none) */
#define  RFEA_TEXTMSG_20                  7       /* control type: textMsg, callback function: (none) */
#define  RFEA_TEXTMSG_19                  8       /* control type: textMsg, callback function: (none) */
#define  RFEA_R_Stop                      9       /* control type: numeric, callback function: (none) */
#define  RFEA_TEXTMSG_18                  10      /* control type: textMsg, callback function: (none) */
#define  RFEA_Probe_Offset                11      /* control type: numeric, callback function: (none) */
#define  RFEA_R_Position                  12      /* control type: numeric, callback function: (none) */
#define  RFEA_R_Increment                 13      /* control type: numeric, callback function: (none) */
#define  RFEA_SAVE                        14      /* control type: command, callback function: Close_RFEA_Settings */
#define  RFEA_Probe_Active_Move           15      /* control type: binary, callback function: (none) */
#define  RFEA_Move_Stop                   16      /* control type: command, callback function: Bdot_Move_to_Stop */
#define  RFEA_Probe_Return                17      /* control type: binary, callback function: (none) */
#define  RFEA_Move_Start                  18      /* control type: command, callback function: Bdot_Move_to_Start */
#define  RFEA_SendHome                    19      /* control type: command, callback function: Bdot_Home */
#define  RFEA_SPLITTER                    20      /* control type: splitter, callback function: (none) */
#define  RFEA_TEXTMSG_22                  21      /* control type: textMsg, callback function: (none) */
#define  RFEA_SettingsTimeWindow          22      /* control type: numeric, callback function: (none) */
#define  RFEA_TEXTMSG_23                  23      /* control type: textMsg, callback function: (none) */
#define  RFEA_Probe_Max_Insertion         24      /* control type: numeric, callback function: (none) */
#define  RFEA_SettingsRecordLength        25      /* control type: slide, callback function: (none) */
#define  RFEA_Probe_COUNTSPERMM           26      /* control type: numeric, callback function: (none) */
#define  RFEA_Chan1Name                   27      /* control type: string, callback function: (none) */
#define  RFEA_SettingsTriggerSource       28      /* control type: ring, callback function: (none) */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK Bdot_Home(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Bdot_Move_to_Start(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Bdot_Move_to_Stop(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Close_RFEA_Settings(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif