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

#define  Triple                           1
#define  Triple_Probe_IP                  2       /* control type: ring, callback function: (none) */
#define  Triple_R_Start                   3       /* control type: numeric, callback function: (none) */
#define  Triple_R_Stop                    4       /* control type: numeric, callback function: (none) */
#define  Triple_Probe_Offset              5       /* control type: numeric, callback function: (none) */
#define  Triple_R_Position                6       /* control type: numeric, callback function: (none) */
#define  Triple_R_Increment               7       /* control type: numeric, callback function: (none) */
#define  Triple_Probe_Active_Move         8       /* control type: binary, callback function: (none) */
#define  Triple_Move_Stop                 9       /* control type: command, callback function: TripleProbe_Move_to_Stop */
#define  Triple_Probe_Return              10      /* control type: binary, callback function: (none) */
#define  Triple_Move_Start                11      /* control type: command, callback function: TripleProbe_Move_to_Start */
#define  Triple_SendHome                  12      /* control type: command, callback function: TripleProbe_Home */
#define  Triple_ACQ2106_NUM_SAMPLES       13      /* control type: numeric, callback function: (none) */
#define  Triple_TEXTMSG_22                14      /* control type: textMsg, callback function: (none) */
#define  Triple_TEXTMSG_23                15      /* control type: textMsg, callback function: (none) */
#define  Triple_Probe_Max_Insertion       16      /* control type: numeric, callback function: (none) */
#define  Triple_Probe_COUNTSPERMM         17      /* control type: numeric, callback function: (none) */
#define  Triple_TEXTMSG_20                18      /* control type: textMsg, callback function: (none) */
#define  Triple_TEXTMSG_19                19      /* control type: textMsg, callback function: (none) */
#define  Triple_SAVE                      20      /* control type: command, callback function: Close_Triple_Settings */
#define  Triple_Version                   21      /* control type: numeric, callback function: (none) */
#define  Triple_ACQ2106_INPUT_Z           22      /* control type: binary, callback function: (none) */
#define  Triple_acq2106_2_IP              23      /* control type: string, callback function: (none) */
#define  Triple_ACQ2106_ID2               24      /* control type: string, callback function: (none) */
#define  Triple_ACQ2106_ID1               25      /* control type: string, callback function: (none) */
#define  Triple_acq2106_1_IP              26      /* control type: string, callback function: (none) */
#define  Triple_ACQ2106_WDIR              27      /* control type: string, callback function: (none) */
#define  Triple_ACQ2106_ENV_PATH          28      /* control type: string, callback function: (none) */
#define  Triple_ACQ2106_SAMPLE_RATE       29      /* control type: slide, callback function: (none) */
#define  Triple_ACQ2106_TRG_DELAY         30      /* control type: numeric, callback function: TLP_ACQ2106_TRG_DELAY_SET */
#define  Triple_ACQ2106_NCHANNELS         31      /* control type: numeric, callback function: (none) */
#define  Triple_ACQ2106_DECIMATON         32      /* control type: scale, callback function: (none) */
#define  Triple_MANUALREAD                33      /* control type: command, callback function: TLP_acq2106_pull_data */
#define  Triple_MANUAL_TRIG               34      /* control type: command, callback function: TLP_acq2106_soft_trig */
#define  Triple_ACQ2106_PUSH_SETTINGS     35      /* control type: command, callback function: TLP_acq2106_push_config */
#define  Triple_MANUALARM                 36      /* control type: command, callback function: TLP_acq2106_manual_arm */
#define  Triple_MANUALABORT               37      /* control type: command, callback function: TLP_acq2106_set_abort */
#define  Triple_ACQ_PY_SCRIPT             38      /* control type: string, callback function: (none) */
#define  Triple_ACQ2106_TRG_SOURCE        39      /* control type: ring, callback function: (none) */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK Close_Triple_Settings(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK TLP_acq2106_manual_arm(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK TLP_acq2106_pull_data(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK TLP_acq2106_push_config(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK TLP_acq2106_set_abort(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK TLP_acq2106_soft_trig(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK TLP_ACQ2106_TRG_DELAY_SET(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK TripleProbe_Home(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK TripleProbe_Move_to_Start(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK TripleProbe_Move_to_Stop(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif