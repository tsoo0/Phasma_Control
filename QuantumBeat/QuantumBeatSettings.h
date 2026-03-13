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
#define  QBS_MOVETOTARGETLOCATION         14      /* control type: command, callback function: QBS_XYZ_Move */
#define  QBS_CLOSEQBSSYSTEM               15      /* control type: command, callback function: Close_QBS_Settings */
#define  QBS_Chan1QBSName                 16      /* control type: string, callback function: (none) */
#define  QBS_SPLITTER_7                   17      /* control type: splitter, callback function: (none) */
#define  QBS_SPLITTER_6                   18      /* control type: splitter, callback function: (none) */
#define  QBS_SPLITTER_2                   19      /* control type: splitter, callback function: (none) */
#define  QBS_QBS_Laser_IP                 20      /* control type: ring, callback function: (none) */
#define  QBS_SPLITTER                     21      /* control type: splitter, callback function: (none) */
#define  QBS_SPLITTER_5                   22      /* control type: splitter, callback function: (none) */
#define  QBS_TEXTMSG_18                   23      /* control type: textMsg, callback function: (none) */
#define  QBS_TEXTMSG_19                   24      /* control type: textMsg, callback function: (none) */
#define  QBS_TEXTMSG_24                   25      /* control type: textMsg, callback function: (none) */
#define  QBS_TEXTMSG_25                   26      /* control type: textMsg, callback function: (none) */
#define  QBS_TEXTMSG_27                   27      /* control type: textMsg, callback function: (none) */
#define  QBS_TEXTMSG_26                   28      /* control type: textMsg, callback function: (none) */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK Close_QBS_Settings(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK QBS_XYZ_Move(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif