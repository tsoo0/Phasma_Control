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

#define  TempDiag                         1
#define  TempDiag_LeCroy_temp_Address     2       /* control type: ring, callback function: (none) */
#define  TempDiag_RIGOL_temp_Address      3       /* control type: ring, callback function: (none) */
#define  TempDiag_VoltageRange_2          4       /* control type: numeric, callback function: (none) */
#define  TempDiag_TriggerLevel_2          5       /* control type: numeric, callback function: (none) */
#define  TempDiag_VoltageRange            6       /* control type: numeric, callback function: (none) */
#define  TempDiag_TriggerLevel            7       /* control type: numeric, callback function: (none) */
#define  TempDiag_TEXTMSG_21              8       /* control type: textMsg, callback function: (none) */
#define  TempDiag_TEXTMSG_19              9       /* control type: textMsg, callback function: (none) */
#define  TempDiag_TimeWindow_2            10      /* control type: numeric, callback function: (none) */
#define  TempDiag_TEXTMSG_18              11      /* control type: textMsg, callback function: (none) */
#define  TempDiag_SAVE                    12      /* control type: command, callback function: Close_TempDiag_Settings */
#define  TempDiag_TimeWindow              13      /* control type: numeric, callback function: (none) */
#define  TempDiag_Chan8Name               14      /* control type: string, callback function: (none) */
#define  TempDiag_Chan7Name               15      /* control type: string, callback function: (none) */
#define  TempDiag_Chan6Name               16      /* control type: string, callback function: (none) */
#define  TempDiag_Chan5Name               17      /* control type: string, callback function: (none) */
#define  TempDiag_Chan4Name               18      /* control type: string, callback function: (none) */
#define  TempDiag_TriggerSource_2         19      /* control type: ring, callback function: (none) */
#define  TempDiag_RecordLength_2          20      /* control type: slide, callback function: (none) */
#define  TempDiag_Chan3Name               21      /* control type: string, callback function: (none) */
#define  TempDiag_Chan2Name               22      /* control type: string, callback function: (none) */
#define  TempDiag_Chan1Name               23      /* control type: string, callback function: (none) */
#define  TempDiag_TriggerSource           24      /* control type: ring, callback function: (none) */
#define  TempDiag_RecordLength            25      /* control type: slide, callback function: (none) */
#define  TempDiag_LECROY                  26      /* control type: binary, callback function: (none) */
#define  TempDiag_RIGOL                   27      /* control type: binary, callback function: (none) */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK Close_TempDiag_Settings(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif