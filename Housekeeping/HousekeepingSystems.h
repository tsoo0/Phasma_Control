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

#define  Housekeep                        1
#define  Housekeep_VX4780Address          2       /* control type: ring, callback function: (none) */
#define  Housekeep_RIGOL1Address          3       /* control type: ring, callback function: (none) */
#define  Housekeep_VX4244Address          4       /* control type: ring, callback function: (none) */
#define  Housekeep_RIGOL_VoltageRange     5       /* control type: numeric, callback function: (none) */
#define  Housekeep_TEXTMSG_20             6       /* control type: textMsg, callback function: (none) */
#define  Housekeep_VoltageRange           7       /* control type: numeric, callback function: (none) */
#define  Housekeep_RecordLength           8       /* control type: numeric, callback function: (none) */
#define  Housekeep_RIGOL_clockspeed       9       /* control type: numeric, callback function: (none) */
#define  Housekeep_TEXTMSG_19             10      /* control type: textMsg, callback function: (none) */
#define  Housekeep_TEXTMSG_18             11      /* control type: textMsg, callback function: (none) */
#define  Housekeep_SAVE                   12      /* control type: command, callback function: Close_Housekeeping_Settings */
#define  Housekeep_SampleRate             13      /* control type: numeric, callback function: (none) */
#define  Housekeep_Chan12Name             14      /* control type: string, callback function: (none) */
#define  Housekeep_Chan11Name             15      /* control type: string, callback function: (none) */
#define  Housekeep_Chan10Name             16      /* control type: string, callback function: (none) */
#define  Housekeep_Chan9Name              17      /* control type: string, callback function: (none) */
#define  Housekeep_Chan7Name              18      /* control type: string, callback function: (none) */
#define  Housekeep_Chan6Name              19      /* control type: string, callback function: (none) */
#define  Housekeep_Chan5Name              20      /* control type: string, callback function: (none) */
#define  Housekeep_Chan4Name              21      /* control type: string, callback function: (none) */
#define  Housekeep_Chan3Name              22      /* control type: string, callback function: (none) */
#define  Housekeep_Chan2Name              23      /* control type: string, callback function: (none) */
#define  Housekeep_Chan1Name              24      /* control type: string, callback function: (none) */
#define  Housekeep_Chan0Name              25      /* control type: string, callback function: (none) */
#define  Housekeep_RIGOL_RecordLength     26      /* control type: slide, callback function: (none) */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK Close_Housekeeping_Settings(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif