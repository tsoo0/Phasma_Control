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

#define  Helicon                          1
#define  Helicon_Species                  2       /* control type: slide, callback function: (none) */
#define  Helicon_Baratron                 3       /* control type: radioButton, callback function: (none) */
#define  Helicon_RF_Amp                   4       /* control type: numeric, callback function: (none) */
#define  Helicon_READCURRENTPRESSURES     5       /* control type: command, callback function: ReadPressure */
#define  Helicon_PULSED_2                 6       /* control type: command, callback function: Stop_PulsedSource */
#define  Helicon_PULSED                   7       /* control type: command, callback function: PulsedSource */
#define  Helicon_READRFPOWER              8       /* control type: command, callback function: ReadRFPower */
#define  Helicon_SAVE                     9       /* control type: command, callback function: Close_Helicon_Settings */
#define  Helicon_SAVEANDUPDATEINSTRUME    10      /* control type: command, callback function: Close_Helicon_Settings_Activate */
#define  Helicon_RF_power_reflected       11      /* control type: numeric, callback function: (none) */
#define  Helicon_MassFlow2                12      /* control type: numeric, callback function: (none) */
#define  Helicon_Pulse_Freq               13      /* control type: numeric, callback function: (none) */
#define  Helicon_MassFlow1                14      /* control type: numeric, callback function: (none) */
#define  Helicon_BaratronPressure         15      /* control type: numeric, callback function: (none) */
#define  Helicon_DownstreamCorrected      16      /* control type: numeric, callback function: (none) */
#define  Helicon_UpstreamCorrected        17      /* control type: numeric, callback function: (none) */
#define  Helicon_DownstreamPressure       18      /* control type: numeric, callback function: (none) */
#define  Helicon_UpstreamPressure         19      /* control type: numeric, callback function: (none) */
#define  Helicon_RF_power_foward          20      /* control type: numeric, callback function: (none) */
#define  Helicon_RF_freq                  21      /* control type: numeric, callback function: (none) */
#define  Helicon_TEXTMSG3_2               22      /* control type: textMsg, callback function: (none) */
#define  Helicon_TEXTMSG3                 23      /* control type: textMsg, callback function: (none) */
#define  Helicon_TEXTMSG2_3               24      /* control type: textMsg, callback function: (none) */
#define  Helicon_TEXTMSG2_2               25      /* control type: textMsg, callback function: (none) */
#define  Helicon_TEXTMSG2                 26      /* control type: textMsg, callback function: (none) */
#define  Helicon_TEXTMSG1                 27      /* control type: textMsg, callback function: (none) */
#define  Helicon_TEXTMSG_3                28      /* control type: textMsg, callback function: (none) */
#define  Helicon_TEXTMSG_6                29      /* control type: textMsg, callback function: (none) */
#define  Helicon_ENI_COM                  30      /* control type: ring, callback function: (none) */
#define  Helicon_PR4000_COM               31      /* control type: ring, callback function: (none) */
#define  Helicon_LeskerGauge              32      /* control type: ring, callback function: (none) */
#define  Helicon_PfeifferGauge            33      /* control type: ring, callback function: (none) */
#define  Helicon_SRS365_GPIB              34      /* control type: ring, callback function: (none) */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK Close_Helicon_Settings(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Close_Helicon_Settings_Activate(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PulsedSource(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK ReadPressure(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK ReadRFPower(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Stop_PulsedSource(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif