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

#define  LANG_PANEL                       1
#define  LANG_PANEL_PHASMA_tipradius      2       /* control type: numeric, callback function: (none) */
#define  LANG_PANEL_Source_tipradius      3       /* control type: numeric, callback function: (none) */
#define  LANG_PANEL_PHASMA_tiplength      4       /* control type: numeric, callback function: (none) */
#define  LANG_PANEL_R_Start_2             5       /* control type: numeric, callback function: (none) */
#define  LANG_PANEL_R_Stop_2              6       /* control type: numeric, callback function: (none) */
#define  LANG_PANEL_R_Start               7       /* control type: numeric, callback function: (none) */
#define  LANG_PANEL_Probe_Offset_2        8       /* control type: numeric, callback function: (none) */
#define  LANG_PANEL_R_Position_2          9       /* control type: numeric, callback function: (none) */
#define  LANG_PANEL_R_Increment_2         10      /* control type: numeric, callback function: (none) */
#define  LANG_PANEL_R_Stop                11      /* control type: numeric, callback function: (none) */
#define  LANG_PANEL_Source_tiplength      12      /* control type: numeric, callback function: (none) */
#define  LANG_PANEL_Probe_Offset          13      /* control type: numeric, callback function: (none) */
#define  LANG_PANEL_R_Position            14      /* control type: numeric, callback function: (none) */
#define  LANG_PANEL_Probe_Active_Move_2   15      /* control type: binary, callback function: (none) */
#define  LANG_PANEL_Move_Stop_2           16      /* control type: command, callback function: Bdot_Move_to_Stop */
#define  LANG_PANEL_Probe_Return_2        17      /* control type: binary, callback function: (none) */
#define  LANG_PANEL_Move_Start_2          18      /* control type: command, callback function: Bdot_Move_to_Start */
#define  LANG_PANEL_R_Increment           19      /* control type: numeric, callback function: (none) */
#define  LANG_PANEL_TEXTMSG_25            20      /* control type: textMsg, callback function: (none) */
#define  LANG_PANEL_PMAX_VOLTS            21      /* control type: numeric, callback function: (none) */
#define  LANG_PANEL_TEXTMSG_26            22      /* control type: textMsg, callback function: (none) */
#define  LANG_PANEL_PMIN_VOLTS            23      /* control type: numeric, callback function: (none) */
#define  LANG_PANEL_SMAX_VOLTS            24      /* control type: numeric, callback function: (none) */
#define  LANG_PANEL_SMIN_VOLTS            25      /* control type: numeric, callback function: (none) */
#define  LANG_PANEL_Probe_Active_Move     26      /* control type: binary, callback function: (none) */
#define  LANG_PANEL_Move_Stop             27      /* control type: command, callback function: Bdot_Move_to_Stop */
#define  LANG_PANEL_Probe_Return          28      /* control type: binary, callback function: (none) */
#define  LANG_PANEL_Move_Start            29      /* control type: command, callback function: Bdot_Move_to_Start */
#define  LANG_PANEL_SPLITTER              30      /* control type: splitter, callback function: (none) */
#define  LANG_PANEL_TEXTMSG_23            31      /* control type: textMsg, callback function: (none) */
#define  LANG_PANEL_CLOSELANGSYSTEM       32      /* control type: command, callback function: Close_Langmuir_Settings */
#define  LANG_PANEL_TEXTMSG_24            33      /* control type: textMsg, callback function: (none) */
#define  LANG_PANEL_PERFORMLANGSCAN       34      /* control type: command, callback function: Langmuir_Activate_Button */
#define  LANG_PANEL_Chan2Name             35      /* control type: string, callback function: (none) */
#define  LANG_PANEL_Chan1Name             36      /* control type: string, callback function: (none) */
#define  LANG_PANEL_MAX_CURRENT           37      /* control type: numeric, callback function: (none) */
#define  LANG_PANEL_PlanarProbe_Area      38      /* control type: numeric, callback function: (none) */
#define  LANG_PANEL_PlanarProbe_Angle     39      /* control type: numeric, callback function: (none) */
#define  LANG_PANEL_BATTERY               40      /* control type: numeric, callback function: (none) */
#define  LANG_PANEL_PlanarProbe_flag      41      /* control type: binary, callback function: (none) */
#define  LANG_PANEL_PROBE                 42      /* control type: binary, callback function: (none) */
#define  LANG_PANEL_Keithley_float        43      /* control type: binary, callback function: (none) */
#define  LANG_PANEL_TEXTMSG               44      /* control type: textMsg, callback function: (none) */
#define  LANG_PANEL_Lang_Data             45      /* control type: binary, callback function: (none) */
#define  LANG_PANEL_Langmuir_probe_num    46      /* control type: slide, callback function: (none) */
#define  LANG_PANEL_LP_plot               47      /* control type: graph, callback function: (none) */
#define  LANG_PANEL_MDS_Shot              48      /* control type: numeric, callback function: (none) */
#define  LANG_PANEL_Te2                   49      /* control type: numeric, callback function: (none) */
#define  LANG_PANEL_Ne2                   50      /* control type: numeric, callback function: (none) */
#define  LANG_PANEL_Ne1                   51      /* control type: numeric, callback function: (none) */
#define  LANG_PANEL_Te1                   52      /* control type: numeric, callback function: (none) */
#define  LANG_PANEL_FloatPot2             53      /* control type: numeric, callback function: (none) */
#define  LANG_PANEL_FloatPot1             54      /* control type: numeric, callback function: (none) */
#define  LANG_PANEL_TEXTMSG_20            55      /* control type: textMsg, callback function: (none) */
#define  LANG_PANEL_KEITHLEYGPIB2         56      /* control type: ring, callback function: (none) */
#define  LANG_PANEL_KEITHLEYGPIB1         57      /* control type: ring, callback function: (none) */
#define  LANG_PANEL_TEXTMSG_18            58      /* control type: textMsg, callback function: (none) */
#define  LANG_PANEL_TEXTMSG_22            59      /* control type: textMsg, callback function: (none) */
#define  LANG_PANEL_TEXTMSG_21            60      /* control type: textMsg, callback function: (none) */
#define  LANG_PANEL_TEXTMSG_19            61      /* control type: textMsg, callback function: (none) */
#define  LANG_PANEL_SPLITTER_4            62      /* control type: splitter, callback function: (none) */
#define  LANG_PANEL_SPLITTER_3            63      /* control type: splitter, callback function: (none) */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK Bdot_Move_to_Start(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Bdot_Move_to_Stop(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Close_Langmuir_Settings(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Langmuir_Activate_Button(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif