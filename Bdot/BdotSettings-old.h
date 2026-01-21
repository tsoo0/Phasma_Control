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

#define  Bdot                             1
#define  Bdot_Probe_IP                    2       /* control type: ring, callback function: (none) */
#define  Bdot_RIGOL2Address               3       /* control type: ring, callback function: (none) */
#define  Bdot_RIGOL1Address               4       /* control type: ring, callback function: (none) */
#define  Bdot_TriggerLevel                5       /* control type: numeric, callback function: (none) */
#define  Bdot_R_Start                     6       /* control type: numeric, callback function: (none) */
#define  Bdot_R_Stop                      7       /* control type: numeric, callback function: (none) */
#define  Bdot_Joerger2Address             8       /* control type: ring, callback function: (none) */
#define  Bdot_Probe_Offset                9       /* control type: numeric, callback function: (none) */
#define  Bdot_R_Position                  10      /* control type: numeric, callback function: (none) */
#define  Bdot_R_Increment                 11      /* control type: numeric, callback function: (none) */
#define  Bdot_Joerger5Address             12      /* control type: ring, callback function: (none) */
#define  Bdot_Joerger4Address             13      /* control type: ring, callback function: (none) */
#define  Bdot_Joerger5MemAddress          14      /* control type: ring, callback function: (none) */
#define  Bdot_Joerger3Address             15      /* control type: ring, callback function: (none) */
#define  Bdot_Joerger4MemAddress          16      /* control type: ring, callback function: (none) */
#define  Bdot_Joerger3MemAddress          17      /* control type: ring, callback function: (none) */
#define  Bdot_Joerger2MemAddress          18      /* control type: ring, callback function: (none) */
#define  Bdot_Joerger1MemAddress          19      /* control type: ring, callback function: (none) */
#define  Bdot_Joerger1Address             20      /* control type: ring, callback function: (none) */
#define  Bdot_TEXTMSG_18                  21      /* control type: textMsg, callback function: (none) */
#define  Bdot_SendHome                    22      /* control type: command, callback function: Bdot_Home */
#define  Bdot_Move_Stop                   23      /* control type: command, callback function: Bdot_Move_to_Stop */
#define  Bdot_Move_Start                  24      /* control type: command, callback function: Bdot_Move_to_Start */
#define  Bdot_SAVE                        25      /* control type: command, callback function: Close_Bdot_Settings */
#define  Bdot_Chan36Name                  26      /* control type: string, callback function: (none) */
#define  Bdot_TriggerSource               27      /* control type: ring, callback function: (none) */
#define  Bdot_RIGOL_RECORDLENGTH          28      /* control type: slide, callback function: (none) */
#define  Bdot_Chan35Name                  29      /* control type: string, callback function: (none) */
#define  Bdot_Chan34Name                  30      /* control type: string, callback function: (none) */
#define  Bdot_Chan33Name                  31      /* control type: string, callback function: (none) */
#define  Bdot_Chan32Name                  32      /* control type: string, callback function: (none) */
#define  Bdot_Chan31Name                  33      /* control type: string, callback function: (none) */
#define  Bdot_Chan30Name                  34      /* control type: string, callback function: (none) */
#define  Bdot_Chan29Name                  35      /* control type: string, callback function: (none) */
#define  Bdot_Chan28Name                  36      /* control type: string, callback function: (none) */
#define  Bdot_Chan27Name                  37      /* control type: string, callback function: (none) */
#define  Bdot_Chan26Name                  38      /* control type: string, callback function: (none) */
#define  Bdot_Chan25Name                  39      /* control type: string, callback function: (none) */
#define  Bdot_Chan24Name                  40      /* control type: string, callback function: (none) */
#define  Bdot_Chan23Name                  41      /* control type: string, callback function: (none) */
#define  Bdot_Chan22Name                  42      /* control type: string, callback function: (none) */
#define  Bdot_Chan21Name                  43      /* control type: string, callback function: (none) */
#define  Bdot_Probe_Active_Move           44      /* control type: binary, callback function: (none) */
#define  Bdot_Probe_Return                45      /* control type: binary, callback function: (none) */
#define  Bdot_Chan20Name                  46      /* control type: string, callback function: (none) */
#define  Bdot_Chan19Name                  47      /* control type: string, callback function: (none) */
#define  Bdot_Chan18Name                  48      /* control type: string, callback function: (none) */
#define  Bdot_Chan17Name                  49      /* control type: string, callback function: (none) */
#define  Bdot_Chan16Name                  50      /* control type: string, callback function: (none) */
#define  Bdot_Chan15Name                  51      /* control type: string, callback function: (none) */
#define  Bdot_Chan14Name                  52      /* control type: string, callback function: (none) */
#define  Bdot_Chan13Name                  53      /* control type: string, callback function: (none) */
#define  Bdot_Chan12Name                  54      /* control type: string, callback function: (none) */
#define  Bdot_Chan11Name                  55      /* control type: string, callback function: (none) */
#define  Bdot_Chan10Name                  56      /* control type: string, callback function: (none) */
#define  Bdot_Chan9Name                   57      /* control type: string, callback function: (none) */
#define  Bdot_Chan8Name                   58      /* control type: string, callback function: (none) */
#define  Bdot_Chan7Name                   59      /* control type: string, callback function: (none) */
#define  Bdot_Chan6Name                   60      /* control type: string, callback function: (none) */
#define  Bdot_Chan5Name                   61      /* control type: string, callback function: (none) */
#define  Bdot_Chan4Name                   62      /* control type: string, callback function: (none) */
#define  Bdot_Chan3Name                   63      /* control type: string, callback function: (none) */
#define  Bdot_Chan2Name                   64      /* control type: string, callback function: (none) */
#define  Bdot_Chan1Name                   65      /* control type: string, callback function: (none) */
#define  Bdot_VTR10014_RECORDLENGTH       66      /* control type: slide, callback function: (none) */
#define  Bdot_VTR10012_RECORDLENGTH       67      /* control type: slide, callback function: (none) */
#define  Bdot_RIGOL_Clockspeed            68      /* control type: slide, callback function: (none) */
#define  Bdot_VTR10014_Clockspeed         69      /* control type: slide, callback function: (none) */
#define  Bdot_VTR10012_Clockspeed         70      /* control type: slide, callback function: (none) */
#define  Bdot_SPLITTER                    71      /* control type: splitter, callback function: (none) */
#define  Bdot_TEXTMSG_21                  72      /* control type: textMsg, callback function: (none) */
#define  Bdot_TEXTMSG_20                  73      /* control type: textMsg, callback function: (none) */
#define  Bdot_Probe_Max_Insertion         74      /* control type: numeric, callback function: (none) */
#define  Bdot_Probe_COUNTSPERMM           75      /* control type: numeric, callback function: (none) */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK Bdot_Home(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Bdot_Move_to_Start(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Bdot_Move_to_Stop(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Close_Bdot_Settings(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif