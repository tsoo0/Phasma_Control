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
#define  Housekeep_TEXTMSG_21             11      /* control type: textMsg, callback function: (none) */
#define  Housekeep_TEXTMSG_18             12      /* control type: textMsg, callback function: (none) */
#define  Housekeep_SAVE                   13      /* control type: command, callback function: Close_Housekeeping_Settings */
#define  Housekeep_SampleRate             14      /* control type: numeric, callback function: (none) */
#define  Housekeep_Chan12Name             15      /* control type: string, callback function: (none) */
#define  Housekeep_Chan11Name             16      /* control type: string, callback function: (none) */
#define  Housekeep_Chan10Name             17      /* control type: string, callback function: (none) */
#define  Housekeep_Chan9Name              18      /* control type: string, callback function: (none) */
#define  Housekeep_Chan7Name              19      /* control type: string, callback function: (none) */
#define  Housekeep_Chan6Name              20      /* control type: string, callback function: (none) */
#define  Housekeep_Chan5Name              21      /* control type: string, callback function: (none) */
#define  Housekeep_Chan4Name              22      /* control type: string, callback function: (none) */
#define  Housekeep_Chan3Name              23      /* control type: string, callback function: (none) */
#define  Housekeep_Chan2Name              24      /* control type: string, callback function: (none) */
#define  Housekeep_Chan1Name              25      /* control type: string, callback function: (none) */
#define  Housekeep_PD1_Chan1              26      /* control type: string, callback function: (none) */
#define  Housekeep_Chan0Name              27      /* control type: string, callback function: (none) */
#define  Housekeep_RIGOL_RecordLength     28      /* control type: slide, callback function: (none) */
#define  Housekeep_HOUSE_PD_SYSTEM_ONOFF  29      /* control type: binary, callback function: (none) */
#define  Housekeep_House_PD_Base_IP       30      /* control type: string, callback function: (none) */
#define  Housekeep_House_PD_Python_Path   31      /* control type: string, callback function: (none) */
#define  Housekeep_PD_CONFIG_STORE        32      /* control type: command, callback function: PD_CONFIG_STORE_CB */
#define  Housekeep_PD_CONFIG_ARM          33      /* control type: command, callback function: PD_CONFIG_ARM_CB */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK Close_Housekeeping_Settings(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PD_CONFIG_ARM_CB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PD_CONFIG_STORE_CB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif