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

#define  Photodiode                       1
#define  Photodiode_t_DAQ_Samples         2       /* control type: numeric, callback function: (none) */
#define  Photodiode_TEXTMSG_21            3       /* control type: textMsg, callback function: (none) */
#define  Photodiode_t_DAQ_IP2             4       /* control type: ring, callback function: (none) */
#define  Photodiode_t_DAQ_IP1             5       /* control type: ring, callback function: (none) */
#define  Photodiode_TEXTMSG_20            6       /* control type: textMsg, callback function: (none) */
#define  Photodiode_TEXTMSG_19            7       /* control type: textMsg, callback function: (none) */
#define  Photodiode_SAVE                  8       /* control type: command, callback function: Close_Photodiode_Settings */
#define  Photodiode_t_DAQ_Freq            9       /* control type: numeric, callback function: (none) */
#define  Photodiode_Version               10      /* control type: numeric, callback function: (none) */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK Close_Photodiode_Settings(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif