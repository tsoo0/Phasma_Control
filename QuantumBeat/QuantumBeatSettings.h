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

#define  QBS                              1
#define  QBS_Z_Target                     2       /* control type: numeric, callback function: (none) */
#define  QBS_Y_Target                     3       /* control type: numeric, callback function: (none) */
#define  QBS_Z_Offset                     4       /* control type: numeric, callback function: (none) */
#define  QBS_Z_Current                    5       /* control type: numeric, callback function: (none) */
#define  QBS_Z_COM                        6       /* control type: ring, callback function: (none) */
#define  QBS_Y_COM                        7       /* control type: ring, callback function: (none) */
#define  QBS_X_COM                        8       /* control type: ring, callback function: (none) */
#define  QBS_X_Target                     9       /* control type: numeric, callback function: (none) */
#define  QBS_Y_Offset                     10      /* control type: numeric, callback function: (none) */
#define  QBS_Y_Current                    11      /* control type: numeric, callback function: (none) */
#define  QBS_X_Offset                     12      /* control type: numeric, callback function: (none) */
#define  QBS_X_Current                    13      /* control type: numeric, callback function: (none) */
#define  QBS_Home                         14      /* control type: command, callback function: XYZ_Home */
#define  QBS_MOVETOTARGETLOCATION         15      /* control type: command, callback function: XYZ_Move */
#define  QBS_UpdateSettings               16      /* control type: command, callback function: UpdateSettings */
#define  QBS_CLOSELIFSYSTEM               17      /* control type: command, callback function: Close_CLIF_Settings */
#define  QBS_PERFORMLIFSCAN               18      /* control type: command, callback function: CLIF_Activate_Button */
#define  QBS_Chan3Name                    19      /* control type: string, callback function: (none) */
#define  QBS_Chan2Name                    20      /* control type: string, callback function: (none) */
#define  QBS_Chan1Name                    21      /* control type: string, callback function: (none) */
#define  QBS_ContinLIF_Data               22      /* control type: binary, callback function: (none) */
#define  QBS_LIF_PLOT                     23      /* control type: graph, callback function: (none) */
#define  QBS_SPLITTER_7                   24      /* control type: splitter, callback function: (none) */
#define  QBS_SPLITTER_6                   25      /* control type: splitter, callback function: (none) */
#define  QBS_SPLITTER_2                   26      /* control type: splitter, callback function: (none) */
#define  QBS_SIRAH_IP                     27      /* control type: ring, callback function: (none) */
#define  QBS_SPLITTER                     28      /* control type: splitter, callback function: (none) */
#define  QBS_LockInGPIB2                  29      /* control type: ring, callback function: (none) */
#define  QBS_LockInGPIB1                  30      /* control type: ring, callback function: (none) */
#define  QBS_Wavemeter_COM                31      /* control type: ring, callback function: (none) */
#define  QBS_RigolFunct                   32      /* control type: ring, callback function: (none) */
#define  QBS_ThorChopperCOM               33      /* control type: ring, callback function: (none) */
#define  QBS_SPLITTER_5                   34      /* control type: splitter, callback function: (none) */
#define  QBS_MDS_Shot                     35      /* control type: numeric, callback function: (none) */
#define  QBS_TEXTMSG_18                   36      /* control type: textMsg, callback function: (none) */
#define  QBS_TEXTMSG_19                   37      /* control type: textMsg, callback function: (none) */
#define  QBS_TEXTMSG_24                   38      /* control type: textMsg, callback function: (none) */
#define  QBS_TEXTMSG_25                   39      /* control type: textMsg, callback function: (none) */
#define  QBS_TEXTMSG_27                   40      /* control type: textMsg, callback function: (none) */
#define  QBS_TEXTMSG_26                   41      /* control type: textMsg, callback function: (none) */
#define  QBS_SPLITTER_10                  42      /* control type: splitter, callback function: (none) */
#define  QBS_SPLITTER_9                   43      /* control type: splitter, callback function: (none) */
#define  QBS_Iodine                       44      /* control type: textMsg, callback function: (none) */
#define  QBS_LIF1                         45      /* control type: textMsg, callback function: (none) */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK CLIF_Activate_Button(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Close_CLIF_Settings(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK UpdateSettings(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK XYZ_Home(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK XYZ_Move(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif