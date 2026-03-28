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
#define  LANG_PANEL_Probe2_IP             2       /* control type: ring, callback function: (none) */
#define  LANG_PANEL_Probe1_IP             3       /* control type: ring, callback function: (none) */
#define  LANG_PANEL_PHASMA_tipradius      4       /* control type: numeric, callback function: (none) */
#define  LANG_PANEL_Source_tipradius      5       /* control type: numeric, callback function: (none) */
#define  LANG_PANEL_PHASMA_tiplength      6       /* control type: numeric, callback function: (none) */
#define  LANG_PANEL_R_Start_2             7       /* control type: numeric, callback function: (none) */
#define  LANG_PANEL_R_Stop_2              8       /* control type: numeric, callback function: (none) */
#define  LANG_PANEL_R_Start               9       /* control type: numeric, callback function: (none) */
#define  LANG_PANEL_Probe2_Offset         10      /* control type: numeric, callback function: (none) */
#define  LANG_PANEL_R_Position_2          11      /* control type: numeric, callback function: (none) */
#define  LANG_PANEL_R_Increment_2         12      /* control type: numeric, callback function: (none) */
#define  LANG_PANEL_R_Stop                13      /* control type: numeric, callback function: (none) */
#define  LANG_PANEL_Source_tiplength      14      /* control type: numeric, callback function: (none) */
#define  LANG_PANEL_Probe1_Offset         15      /* control type: numeric, callback function: (none) */
#define  LANG_PANEL_R_Position            16      /* control type: numeric, callback function: (none) */
#define  LANG_PANEL_Probe2_Active_Move    17      /* control type: binary, callback function: (none) */
#define  LANG_PANEL_Move_Stop_2           18      /* control type: command, callback function: Langmuir_Move_to_Stop2 */
#define  LANG_PANEL_Probe_Return_2        19      /* control type: binary, callback function: (none) */
#define  LANG_PANEL_Move_Start_2          20      /* control type: command, callback function: Langmuir_Move_to_Start2 */
#define  LANG_PANEL_R_Increment           21      /* control type: numeric, callback function: (none) */
#define  LANG_PANEL_TEXTMSG_25            22      /* control type: textMsg, callback function: (none) */
#define  LANG_PANEL_PMAX_VOLTS            23      /* control type: numeric, callback function: (none) */
#define  LANG_PANEL_TEXTMSG_26            24      /* control type: textMsg, callback function: (none) */
#define  LANG_PANEL_PMIN_VOLTS            25      /* control type: numeric, callback function: (none) */
#define  LANG_PANEL_SMAX_VOLTS            26      /* control type: numeric, callback function: (none) */
#define  LANG_PANEL_SMIN_VOLTS            27      /* control type: numeric, callback function: (none) */
#define  LANG_PANEL_Probe1_Active_Move    28      /* control type: binary, callback function: (none) */
#define  LANG_PANEL_Move_Stop             29      /* control type: command, callback function: Langmuir_Move_to_Stop1 */
#define  LANG_PANEL_Probe_Return          30      /* control type: binary, callback function: (none) */
#define  LANG_PANEL_Move_Start            31      /* control type: command, callback function: Langmuir_Move_to_Start1 */
#define  LANG_PANEL_SPLITTER              32      /* control type: splitter, callback function: (none) */
#define  LANG_PANEL_TEXTMSG_23            33      /* control type: textMsg, callback function: (none) */
#define  LANG_PANEL_CLOSELANGSYSTEM       34      /* control type: command, callback function: Close_Langmuir_Settings */
#define  LANG_PANEL_TEXTMSG_24            35      /* control type: textMsg, callback function: (none) */
#define  LANG_PANEL_PERFORMLANGSCAN       36      /* control type: command, callback function: Langmuir_Activate_Button */
#define  LANG_PANEL_Chan2Name             37      /* control type: string, callback function: (none) */
#define  LANG_PANEL_Chan1Name             38      /* control type: string, callback function: (none) */
#define  LANG_PANEL_MAX_CURRENT           39      /* control type: numeric, callback function: (none) */
#define  LANG_PANEL_PlanarProbe_Area      40      /* control type: numeric, callback function: (none) */
#define  LANG_PANEL_PlanarProbe_Angle     41      /* control type: numeric, callback function: (none) */
#define  LANG_PANEL_BATTERY               42      /* control type: numeric, callback function: (none) */
#define  LANG_PANEL_PlanarProbe_flag      43      /* control type: binary, callback function: (none) */
#define  LANG_PANEL_PROBE                 44      /* control type: binary, callback function: (none) */
#define  LANG_PANEL_Keithley_float        45      /* control type: binary, callback function: (none) */
#define  LANG_PANEL_TEXTMSG               46      /* control type: textMsg, callback function: (none) */
#define  LANG_PANEL_Lang_Data             47      /* control type: binary, callback function: (none) */
#define  LANG_PANEL_Langmuir_probe_num    48      /* control type: slide, callback function: (none) */
#define  LANG_PANEL_LP_plot               49      /* control type: graph, callback function: (none) */
#define  LANG_PANEL_MDS_Shot              50      /* control type: numeric, callback function: (none) */
#define  LANG_PANEL_Te2                   51      /* control type: numeric, callback function: (none) */
#define  LANG_PANEL_Ne2                   52      /* control type: numeric, callback function: (none) */
#define  LANG_PANEL_Ne1                   53      /* control type: numeric, callback function: (none) */
#define  LANG_PANEL_Te1                   54      /* control type: numeric, callback function: (none) */
#define  LANG_PANEL_FloatPot2             55      /* control type: numeric, callback function: (none) */
#define  LANG_PANEL_FloatPot1             56      /* control type: numeric, callback function: (none) */
#define  LANG_PANEL_TEXTMSG_20            57      /* control type: textMsg, callback function: (none) */
#define  LANG_PANEL_KEITHLEYGPIB2         58      /* control type: ring, callback function: (none) */
#define  LANG_PANEL_SendHome_2            59      /* control type: command, callback function: Langmuir_Home2 */
#define  LANG_PANEL_SendHome              60      /* control type: command, callback function: Langmuir_Home1 */
#define  LANG_PANEL_KEITHLEYGPIB1         61      /* control type: ring, callback function: (none) */
#define  LANG_PANEL_TEXTMSG_18            62      /* control type: textMsg, callback function: (none) */
#define  LANG_PANEL_TEXTMSG_22            63      /* control type: textMsg, callback function: (none) */
#define  LANG_PANEL_TEXTMSG_21            64      /* control type: textMsg, callback function: (none) */
#define  LANG_PANEL_TEXTMSG_19            65      /* control type: textMsg, callback function: (none) */
#define  LANG_PANEL_SPLITTER_4            66      /* control type: splitter, callback function: (none) */
#define  LANG_PANEL_SPLITTER_3            67      /* control type: splitter, callback function: (none) */
#define  LANG_PANEL_Probe1_Max_Insertion  68      /* control type: numeric, callback function: (none) */
#define  LANG_PANEL_Probe1_COUNTSPERMM    69      /* control type: numeric, callback function: (none) */
#define  LANG_PANEL_Probe2_Max_Insertion  70      /* control type: numeric, callback function: (none) */
#define  LANG_PANEL_Probe2_COUNTSPERMM    71      /* control type: numeric, callback function: (none) */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK Close_Langmuir_Settings(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Langmuir_Activate_Button(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Langmuir_Home1(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Langmuir_Home2(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Langmuir_Move_to_Start1(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Langmuir_Move_to_Start2(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Langmuir_Move_to_Stop1(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Langmuir_Move_to_Stop2(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif