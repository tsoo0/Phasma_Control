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

#define  PhotronCam                       1
#define  PhotronCam_Width                 2       /* control type: slide, callback function: (none) */
#define  PhotronCam_Height                3       /* control type: slide, callback function: (none) */
#define  PhotronCam_Frames                4       /* control type: slide, callback function: (none) */
#define  PhotronCam_Shutter               5       /* control type: slide, callback function: (none) */
#define  PhotronCam_FramesPerSec          6       /* control type: slide, callback function: (none) */
#define  PhotronCam_Brightness            7       /* control type: scale, callback function: (none) */
#define  PhotronCam_Contrast              8       /* control type: scale, callback function: (none) */
#define  PhotronCam_Gamma                 9       /* control type: scale, callback function: (none) */
#define  PhotronCam_Gain                  10      /* control type: scale, callback function: (none) */
#define  PhotronCam_PhotronIPAddress      11      /* control type: ring, callback function: (none) */
#define  PhotronCam_TEXTMSG_19            12      /* control type: textMsg, callback function: (none) */
#define  PhotronCam_TEXTMSG_18            13      /* control type: textMsg, callback function: (none) */
#define  PhotronCam_SAVE                  14      /* control type: command, callback function: Close_PhotronCamera_Settings */
#define  PhotronCam_Chan1Name             15      /* control type: string, callback function: (none) */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK Close_PhotronCamera_Settings(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif