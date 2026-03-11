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
#define  QBS_LIF_X_START                  5       /* control type: numeric, callback function: (none) */
#define  QBS_LIF_X_STOP                   6       /* control type: numeric, callback function: (none) */
#define  QBS_Z_Current                    7       /* control type: numeric, callback function: (none) */
#define  QBS_LIF_X_INCREMENT              8       /* control type: numeric, callback function: (none) */
#define  QBS_LIF_X_Not                    9       /* control type: numeric, callback function: (none) */
#define  QBS_LIF_Y_Not                    10      /* control type: numeric, callback function: (none) */
#define  QBS_LIF_Z_Not                    11      /* control type: numeric, callback function: (none) */
#define  QBS_LIF_Z_START                  12      /* control type: numeric, callback function: (none) */
#define  QBS_LIF_Z_STOP                   13      /* control type: numeric, callback function: (none) */
#define  QBS_LIF_Z_INCREMENT              14      /* control type: numeric, callback function: (none) */
#define  QBS_LIF_Y_START                  15      /* control type: numeric, callback function: (none) */
#define  QBS_LIF_Y_STOP                   16      /* control type: numeric, callback function: (none) */
#define  QBS_LIF_Y_INCREMENT              17      /* control type: numeric, callback function: (none) */
#define  QBS_Z_COM                        18      /* control type: ring, callback function: (none) */
#define  QBS_Y_COM                        19      /* control type: ring, callback function: (none) */
#define  QBS_X_COM                        20      /* control type: ring, callback function: (none) */
#define  QBS_X_Target                     21      /* control type: numeric, callback function: (none) */
#define  QBS_Y_Offset                     22      /* control type: numeric, callback function: (none) */
#define  QBS_Y_Current                    23      /* control type: numeric, callback function: (none) */
#define  QBS_X_Offset                     24      /* control type: numeric, callback function: (none) */
#define  QBS_X_Current                    25      /* control type: numeric, callback function: (none) */
#define  QBS_Home                         26      /* control type: command, callback function: QBS_XYZ_Home */
#define  QBS_MOVETOTARGETLOCATION         27      /* control type: command, callback function: QBS_XYZ_Move */
#define  QBS_UpdateQBSSettings            28      /* control type: command, callback function: UpdateQBSSettings */
#define  QBS_CLOSEQBSSYSTEM               29      /* control type: command, callback function: Close_QBS_Settings */
#define  QBS_Chan1QBSName                 30      /* control type: string, callback function: (none) */
#define  QBS_SPLITTER_7                   31      /* control type: splitter, callback function: (none) */
#define  QBS_SPLITTER_6                   32      /* control type: splitter, callback function: (none) */
#define  QBS_SPLITTER_2                   33      /* control type: splitter, callback function: (none) */
#define  QBS_QBS_Laser_IP                 34      /* control type: ring, callback function: (none) */
#define  QBS_SPLITTER                     35      /* control type: splitter, callback function: (none) */
#define  QBS_SPLITTER_5                   36      /* control type: splitter, callback function: (none) */
#define  QBS_TEXTMSG_18                   37      /* control type: textMsg, callback function: (none) */
#define  QBS_TEXTMSG_19                   38      /* control type: textMsg, callback function: (none) */
#define  QBS_TEXTMSG_24                   39      /* control type: textMsg, callback function: (none) */
#define  QBS_TEXTMSG_25                   40      /* control type: textMsg, callback function: (none) */
#define  QBS_TEXTMSG_27                   41      /* control type: textMsg, callback function: (none) */
#define  QBS_TEXTMSG_26                   42      /* control type: textMsg, callback function: (none) */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK Close_QBS_Settings(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK QBS_XYZ_Home(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK QBS_XYZ_Move(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK UpdateQBSSettings(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif