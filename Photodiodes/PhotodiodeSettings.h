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
#define  Photodiode_ACQ2106_NUM_SAMPLES   2       /* control type: numeric, callback function: (none) */
#define  Photodiode_TEXTMSG_20            3       /* control type: textMsg, callback function: (none) */
#define  Photodiode_TEXTMSG_19            4       /* control type: textMsg, callback function: (none) */
#define  Photodiode_SAVE                  5       /* control type: command, callback function: Close_Photodiode_Settings */
#define  Photodiode_Version               6       /* control type: numeric, callback function: (none) */
#define  Photodiode_ACQ2106_INPUT_Z       7       /* control type: binary, callback function: (none) */
#define  Photodiode_acq2106_2_IP          8       /* control type: string, callback function: (none) */
#define  Photodiode_ACQ2106_ID2           9       /* control type: string, callback function: (none) */
#define  Photodiode_ACQ2106_ID1           10      /* control type: string, callback function: (none) */
#define  Photodiode_acq2106_1_IP          11      /* control type: string, callback function: (none) */
#define  Photodiode_ACQ2106_WDIR          12      /* control type: string, callback function: (none) */
#define  Photodiode_ACQ2106_ENV_PATH      13      /* control type: string, callback function: (none) */
#define  Photodiode_ACQ2106_SAMPLE_RATE   14      /* control type: slide, callback function: (none) */
#define  Photodiode_ACQ2106_TRG_DELAY     15      /* control type: numeric, callback function: PD_ACQ2106_TRG_DELAY_SET */
#define  Photodiode_ACQ2106_NCHANNELS     16      /* control type: numeric, callback function: (none) */
#define  Photodiode_ACQ2106_DECIMATON     17      /* control type: scale, callback function: (none) */
#define  Photodiode_PyConsole             18      /* control type: textBox, callback function: PyConsoleUpdate */
#define  Photodiode_MANUALREAD            19      /* control type: command, callback function: acq2106_pull_data */
#define  Photodiode_MANUAL_TRIG           20      /* control type: command, callback function: acq2106_soft_trig */
#define  Photodiode_ACQ2106_PUSH_SETTINGS 21      /* control type: command, callback function: acq2106_push_config */
#define  Photodiode_MANUALARM             22      /* control type: command, callback function: acq2106_manual_arm */
#define  Photodiode_MANUALABORT           23      /* control type: command, callback function: acq2106_set_abort */
#define  Photodiode_ACQ_PY_SCRIPT         24      /* control type: string, callback function: (none) */
#define  Photodiode_ACQ2106_TRG_SOURCE    25      /* control type: ring, callback function: (none) */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK acq2106_manual_arm(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK acq2106_pull_data(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK acq2106_push_config(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK acq2106_set_abort(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK acq2106_soft_trig(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Close_Photodiode_Settings(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PD_ACQ2106_TRG_DELAY_SET(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PyConsoleUpdate(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif