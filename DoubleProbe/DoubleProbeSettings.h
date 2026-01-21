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

#define  DoublePrbe                       1
#define  DoublePrbe_DBPRB_RIGOLAddress2   2       /* control type: ring, callback function: (none) */
#define  DoublePrbe_DBPRB_RIGOLAddress1   3       /* control type: ring, callback function: (none) */
#define  DoublePrbe_Probe_IP              4       /* control type: ring, callback function: (none) */
#define  DoublePrbe_DP_VoltageRange       5       /* control type: numeric, callback function: (none) */
#define  DoublePrbe_DP_TriggerLevel       6       /* control type: numeric, callback function: (none) */
#define  DoublePrbe_R_Start               7       /* control type: numeric, callback function: (none) */
#define  DoublePrbe_TEXTMSG_21            8       /* control type: textMsg, callback function: (none) */
#define  DoublePrbe_TEXTMSG_20            9       /* control type: textMsg, callback function: (none) */
#define  DoublePrbe_R_Stop                10      /* control type: numeric, callback function: (none) */
#define  DoublePrbe_TEXTMSG_19            11      /* control type: textMsg, callback function: (none) */
#define  DoublePrbe_Probe_Offset          12      /* control type: numeric, callback function: (none) */
#define  DoublePrbe_R_Position            13      /* control type: numeric, callback function: (none) */
#define  DoublePrbe_R_Increment           14      /* control type: numeric, callback function: (none) */
#define  DoublePrbe_TEXTMSG_18            15      /* control type: textMsg, callback function: (none) */
#define  DoublePrbe_Probe_Active_Move     16      /* control type: binary, callback function: (none) */
#define  DoublePrbe_Move_Stop             17      /* control type: command, callback function: Bdot_Move_to_Stop */
#define  DoublePrbe_Probe_Return          18      /* control type: binary, callback function: (none) */
#define  DoublePrbe_Move_Start            19      /* control type: command, callback function: Bdot_Move_to_Start */
#define  DoublePrbe_SendHome              20      /* control type: command, callback function: Bdot_Home */
#define  DoublePrbe_SPLITTER              21      /* control type: splitter, callback function: (none) */
#define  DoublePrbe_TEXTMSG_23            22      /* control type: textMsg, callback function: (none) */
#define  DoublePrbe_TEXTMSG_22            23      /* control type: textMsg, callback function: (none) */
#define  DoublePrbe_TEXTMSG_24            24      /* control type: textMsg, callback function: (none) */
#define  DoublePrbe_DP_RecordLength       25      /* control type: slide, callback function: (none) */
#define  DoublePrbe_Probe_Max_Insertion   26      /* control type: numeric, callback function: (none) */
#define  DoublePrbe_Probe_COUNTSPERMM     27      /* control type: numeric, callback function: (none) */
#define  DoublePrbe_SAVE                  28      /* control type: command, callback function: Close_DoubleProbe_Settings */
#define  DoublePrbe_DP_TimeWindow         29      /* control type: numeric, callback function: (none) */
#define  DoublePrbe_Chan6Name             30      /* control type: string, callback function: (none) */
#define  DoublePrbe_Chan5Name             31      /* control type: string, callback function: (none) */
#define  DoublePrbe_Chan4Name             32      /* control type: string, callback function: (none) */
#define  DoublePrbe_Chan3Name             33      /* control type: string, callback function: (none) */
#define  DoublePrbe_Chan2Name             34      /* control type: string, callback function: (none) */
#define  DoublePrbe_Chan1Name             35      /* control type: string, callback function: (none) */
#define  DoublePrbe_DP_TriggerSource      36      /* control type: ring, callback function: (none) */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK Bdot_Home(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Bdot_Move_to_Start(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Bdot_Move_to_Stop(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Close_DoubleProbe_Settings(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif