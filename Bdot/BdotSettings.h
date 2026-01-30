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
#define  Bdot_R_Start                     3       /* control type: numeric, callback function: (none) */
#define  Bdot_R_Stop                      4       /* control type: numeric, callback function: (none) */
#define  Bdot_Joerger2Address             5       /* control type: ring, callback function: (none) */
#define  Bdot_Probe_Offset                6       /* control type: numeric, callback function: (none) */
#define  Bdot_R_Position                  7       /* control type: numeric, callback function: (none) */
#define  Bdot_R_Increment                 8       /* control type: numeric, callback function: (none) */
#define  Bdot_Joerger5Address             9       /* control type: ring, callback function: (none) */
#define  Bdot_Joerger4Address             10      /* control type: ring, callback function: (none) */
#define  Bdot_Joerger5MemAddress          11      /* control type: ring, callback function: (none) */
#define  Bdot_Joerger3Address             12      /* control type: ring, callback function: (none) */
#define  Bdot_Joerger4MemAddress          13      /* control type: ring, callback function: (none) */
#define  Bdot_Joerger3MemAddress          14      /* control type: ring, callback function: (none) */
#define  Bdot_Joerger2MemAddress          15      /* control type: ring, callback function: (none) */
#define  Bdot_Joerger1MemAddress          16      /* control type: ring, callback function: (none) */
#define  Bdot_Joerger1Address             17      /* control type: ring, callback function: (none) */
#define  Bdot_TEXTMSG_18                  18      /* control type: textMsg, callback function: (none) */
#define  Bdot_SendHome                    19      /* control type: command, callback function: Bdot_Home */
#define  Bdot_Move_Stop                   20      /* control type: command, callback function: Bdot_Move_to_Stop */
#define  Bdot_Move_Start                  21      /* control type: command, callback function: Bdot_Move_to_Start */
#define  Bdot_SAVE                        22      /* control type: command, callback function: Close_Bdot_Settings */
#define  Bdot_Chan36Name                  23      /* control type: string, callback function: (none) */
#define  Bdot_Chan35Name                  24      /* control type: string, callback function: (none) */
#define  Bdot_Chan34Name                  25      /* control type: string, callback function: (none) */
#define  Bdot_Chan33Name                  26      /* control type: string, callback function: (none) */
#define  Bdot_Chan32Name                  27      /* control type: string, callback function: (none) */
#define  Bdot_Chan31Name                  28      /* control type: string, callback function: (none) */
#define  Bdot_Chan30Name                  29      /* control type: string, callback function: (none) */
#define  Bdot_Chan29Name                  30      /* control type: string, callback function: (none) */
#define  Bdot_Chan28Name                  31      /* control type: string, callback function: (none) */
#define  Bdot_Chan27Name                  32      /* control type: string, callback function: (none) */
#define  Bdot_Chan26Name                  33      /* control type: string, callback function: (none) */
#define  Bdot_Chan25Name                  34      /* control type: string, callback function: (none) */
#define  Bdot_Chan24Name                  35      /* control type: string, callback function: (none) */
#define  Bdot_Chan23Name                  36      /* control type: string, callback function: (none) */
#define  Bdot_Chan22Name                  37      /* control type: string, callback function: (none) */
#define  Bdot_Chan21Name                  38      /* control type: string, callback function: (none) */
#define  Bdot_Probe_Active_Move           39      /* control type: binary, callback function: (none) */
#define  Bdot_Probe_Return                40      /* control type: binary, callback function: (none) */
#define  Bdot_Chan20Name                  41      /* control type: string, callback function: (none) */
#define  Bdot_Chan19Name                  42      /* control type: string, callback function: (none) */
#define  Bdot_Chan18Name                  43      /* control type: string, callback function: (none) */
#define  Bdot_Chan17Name                  44      /* control type: string, callback function: (none) */
#define  Bdot_Chan16Name                  45      /* control type: string, callback function: (none) */
#define  Bdot_Chan15Name                  46      /* control type: string, callback function: (none) */
#define  Bdot_Chan14Name                  47      /* control type: string, callback function: (none) */
#define  Bdot_Chan13Name                  48      /* control type: string, callback function: (none) */
#define  Bdot_Chan12Name                  49      /* control type: string, callback function: (none) */
#define  Bdot_Chan11Name                  50      /* control type: string, callback function: (none) */
#define  Bdot_Chan10Name                  51      /* control type: string, callback function: (none) */
#define  Bdot_Chan9Name                   52      /* control type: string, callback function: (none) */
#define  Bdot_Chan8Name                   53      /* control type: string, callback function: (none) */
#define  Bdot_Chan7Name                   54      /* control type: string, callback function: (none) */
#define  Bdot_Chan6Name                   55      /* control type: string, callback function: (none) */
#define  Bdot_Chan5Name                   56      /* control type: string, callback function: (none) */
#define  Bdot_Chan4Name                   57      /* control type: string, callback function: (none) */
#define  Bdot_Chan3Name                   58      /* control type: string, callback function: (none) */
#define  Bdot_Chan2Name                   59      /* control type: string, callback function: (none) */
#define  Bdot_Chan1Name                   60      /* control type: string, callback function: (none) */
#define  Bdot_VTR10014_RECORDLENGTH       61      /* control type: slide, callback function: (none) */
#define  Bdot_VTR10012_RECORDLENGTH       62      /* control type: slide, callback function: (none) */
#define  Bdot_VTR10014_Clockspeed         63      /* control type: slide, callback function: (none) */
#define  Bdot_VTR10012_Clockspeed         64      /* control type: slide, callback function: (none) */
#define  Bdot_SPLITTER                    65      /* control type: splitter, callback function: (none) */
#define  Bdot_TEXTMSG_21                  66      /* control type: textMsg, callback function: (none) */
#define  Bdot_TEXTMSG_20                  67      /* control type: textMsg, callback function: (none) */
#define  Bdot_Probe_Max_Insertion         68      /* control type: numeric, callback function: (none) */
#define  Bdot_Probe_COUNTSPERMM           69      /* control type: numeric, callback function: (none) */


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