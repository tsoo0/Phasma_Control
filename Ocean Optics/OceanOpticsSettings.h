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

#define  OceanOptic                       1
#define  OceanOptic_Acquire               2       /* control type: command, callback function: OceanOpticsAcquire */
#define  OceanOptic_Close                 3       /* control type: command, callback function: Close_OceanOptics_Settings */
#define  OceanOptic_IntergrationTime      4       /* control type: numeric, callback function: (none) */
#define  OceanOptic_HR4000_COM            5       /* control type: ring, callback function: (none) */
#define  OceanOptic_OceanOpticsGraph      6       /* control type: graph, callback function: (none) */
#define  OceanOptic_TEXTMSG_18            7       /* control type: textMsg, callback function: (none) */
#define  OceanOptic_Chan1Name             8       /* control type: string, callback function: (none) */
#define  OceanOptic_Trigger               9       /* control type: binary, callback function: (none) */
#define  OceanOptic_MDS_Shot              10      /* control type: numeric, callback function: (none) */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK Close_OceanOptics_Settings(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OceanOpticsAcquire(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif