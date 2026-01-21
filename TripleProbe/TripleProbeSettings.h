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

#define  TriplePrbe                       1
#define  TriplePrbe_t_DAQ_Samples         2       /* control type: numeric, callback function: (none) */
#define  TriplePrbe_Probe_IP              3       /* control type: ring, callback function: (none) */
#define  TriplePrbe_t_DAQ_IP2             4       /* control type: ring, callback function: (none) */
#define  TriplePrbe_t_DAQ_IP1             5       /* control type: ring, callback function: (none) */
#define  TriplePrbe_TEXTMSG_21            6       /* control type: textMsg, callback function: (none) */
#define  TriplePrbe_TEXTMSG_20            7       /* control type: textMsg, callback function: (none) */
#define  TriplePrbe_R_Start               8       /* control type: numeric, callback function: (none) */
#define  TriplePrbe_t_DAQ_Freq            9       /* control type: numeric, callback function: (none) */
#define  TriplePrbe_Version_2             10      /* control type: numeric, callback function: (none) */
#define  TriplePrbe_R_Stop                11      /* control type: numeric, callback function: (none) */
#define  TriplePrbe_TEXTMSG_19            12      /* control type: textMsg, callback function: (none) */
#define  TriplePrbe_Probe_Offset          13      /* control type: numeric, callback function: (none) */
#define  TriplePrbe_R_Position            14      /* control type: numeric, callback function: (none) */
#define  TriplePrbe_R_Increment           15      /* control type: numeric, callback function: (none) */
#define  TriplePrbe_SAVE                  16      /* control type: command, callback function: Close_TripleProbe_Settings */
#define  TriplePrbe_Probe_Active_Move     17      /* control type: binary, callback function: (none) */
#define  TriplePrbe_Move_Stop             18      /* control type: command, callback function: Bdot_Move_to_Stop */
#define  TriplePrbe_Probe_Return          19      /* control type: binary, callback function: (none) */
#define  TriplePrbe_Move_Start            20      /* control type: command, callback function: Bdot_Move_to_Start */
#define  TriplePrbe_SendHome              21      /* control type: command, callback function: Bdot_Home */
#define  TriplePrbe_SPLITTER              22      /* control type: splitter, callback function: (none) */
#define  TriplePrbe_TEXTMSG_22            23      /* control type: textMsg, callback function: (none) */
#define  TriplePrbe_TEXTMSG_24            24      /* control type: textMsg, callback function: (none) */
#define  TriplePrbe_TEXTMSG_23            25      /* control type: textMsg, callback function: (none) */
#define  TriplePrbe_Probe_Max_Insertion   26      /* control type: numeric, callback function: (none) */
#define  TriplePrbe_Probe_COUNTSPERMM     27      /* control type: numeric, callback function: (none) */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK Bdot_Home(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Bdot_Move_to_Start(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Bdot_Move_to_Stop(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Close_TripleProbe_Settings(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif