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
#define  PhotronCam_Frames                2       /* control type: slide, callback function: (none) */
#define  PhotronCam_Shutter               3       /* control type: slide, callback function: (none) */
#define  PhotronCam_Brightness            4       /* control type: scale, callback function: (none) */
#define  PhotronCam_Contrast              5       /* control type: scale, callback function: (none) */
#define  PhotronCam_Gamma                 6       /* control type: scale, callback function: (none) */
#define  PhotronCam_Gain                  7       /* control type: scale, callback function: (none) */
#define  PhotronCam_PhotronFrameCase      8       /* control type: ring, callback function: (none) */
#define  PhotronCam_PhotronIPAddress      9       /* control type: ring, callback function: (none) */
#define  PhotronCam_TEXTMSG_19            10      /* control type: textMsg, callback function: (none) */
#define  PhotronCam_TEXTMSG_18            11      /* control type: textMsg, callback function: (none) */
#define  PhotronCam_SAVE                  12      /* control type: command, callback function: Close_PhotronCamera_Settings */
#define  PhotronCam_Chan1Name             13      /* control type: string, callback function: (none) */
#define  PhotronCam_PhotronDataPath       14      /* control type: string, callback function: (none) */
#define  PhotronCam_micro_per_pixel       15      /* control type: numeric, callback function: (none) */
#define  PhotronCam_Trigger_Delay         16      /* control type: numeric, callback function: (none) */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK Close_PhotronCamera_Settings(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif