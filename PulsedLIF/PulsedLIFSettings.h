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

#define  PLIF                             1
#define  PLIF_Cobra_Port                  2       /* control type: ring, callback function: (none) */
#define  PLIF_Wavelength_Start            3       /* control type: numeric, callback function: (none) */
#define  PLIF_Wavelength_Stop             4       /* control type: numeric, callback function: (none) */
#define  PLIF_RIGOLAddress                5       /* control type: ring, callback function: (none) */
#define  PLIF_Wavelength_Increment        6       /* control type: numeric, callback function: (none) */
#define  PLIF_Rigol_Address               7       /* control type: ring, callback function: (none) */
#define  PLIF_Agilent_Address             8       /* control type: ring, callback function: (none) */
#define  PLIF_Pulsed_CenterTime           9       /* control type: numeric, callback function: (none) */
#define  PLIF_Pulsed_TimeSpan             10      /* control type: numeric, callback function: (none) */
#define  PLIF_Pulsed_Chan2_Offset         11      /* control type: numeric, callback function: (none) */
#define  PLIF_Pulsed_Chan1_Offset         12      /* control type: numeric, callback function: (none) */
#define  PLIF_Pulsed_VoltageRange2        13      /* control type: numeric, callback function: (none) */
#define  PLIF_Pulsed_VoltageRange1        14      /* control type: numeric, callback function: (none) */
#define  PLIF_Pulsed_TriggerLevel         15      /* control type: numeric, callback function: (none) */
#define  PLIF_TEXTMSG_67                  16      /* control type: textMsg, callback function: (none) */
#define  PLIF_Wave_Stop                   17      /* control type: command, callback function: Wavelength_Move_to_Stop */
#define  PLIF_Wave_Start                  18      /* control type: command, callback function: Wavelength_Move_to_Start */
#define  PLIF_TEXTMSG_54                  19      /* control type: textMsg, callback function: (none) */
#define  PLIF_TEXTMSG_55                  20      /* control type: textMsg, callback function: (none) */
#define  PLIF_TEXTMSG_56                  21      /* control type: textMsg, callback function: (none) */
#define  PLIF_TEXTMSG_44                  22      /* control type: textMsg, callback function: (none) */
#define  PLIF_TEXTMSG_42                  23      /* control type: textMsg, callback function: (none) */
#define  PLIF_QSW                         24      /* control type: textMsg, callback function: (none) */
#define  PLIF_EXTTRIGGER                  25      /* control type: textMsg, callback function: (none) */
#define  PLIF_OUTPUT                      26      /* control type: textMsg, callback function: (none) */
#define  PLIF_QSYNCHOUT                   27      /* control type: textMsg, callback function: (none) */
#define  PLIF_CH1                         28      /* control type: textMsg, callback function: (none) */
#define  PLIF_Q850_QSW_Source_2           29      /* control type: binary, callback function: (none) */
#define  PLIF_Q850_QSW_Source             30      /* control type: binary, callback function: (none) */
#define  PLIF_Pulsed_RecordLength         31      /* control type: slide, callback function: (none) */
#define  PLIF_TEXTMSG_57                  32      /* control type: textMsg, callback function: (none) */
#define  PLIF_FINALPULSE                  33      /* control type: textMsg, callback function: (none) */
#define  PLIF_EXTTRIGGER_2                34      /* control type: textMsg, callback function: (none) */
#define  PLIF_T0                          35      /* control type: textMsg, callback function: (none) */
#define  PLIF_Wavelength_Return           36      /* control type: binary, callback function: (none) */
#define  PLIF_TEXTMSG_58                  37      /* control type: textMsg, callback function: (none) */
#define  PLIF_TEXTMSG_60                  38      /* control type: textMsg, callback function: (none) */
#define  PLIF_TEXTMSG_43                  39      /* control type: textMsg, callback function: (none) */
#define  PLIF_TEXTMSG_8                   40      /* control type: textMsg, callback function: (none) */
#define  PLIF_Meter_Delay                 41      /* control type: numeric, callback function: (none) */
#define  PLIF_Q850_Circuit_Delay          42      /* control type: numeric, callback function: (none) */
#define  PLIF_Q850_InstrinsicDelay        43      /* control type: numeric, callback function: (none) */
#define  PLIF_Q850_Q_delay                44      /* control type: numeric, callback function: (none) */
#define  PLIF_Q850_Frequency              45      /* control type: numeric, callback function: (none) */
#define  PLIF_PLIF_Warmup                 46      /* control type: numeric, callback function: (none) */
#define  PLIF_Chan3Name                   47      /* control type: string, callback function: (none) */
#define  PLIF_Chan2Name                   48      /* control type: string, callback function: (none) */
#define  PLIF_Chan1Name                   49      /* control type: string, callback function: (none) */
#define  PLIF_GenTecPort                  50      /* control type: ring, callback function: (none) */
#define  PLIF_TEXTMSG_22                  51      /* control type: textMsg, callback function: (none) */
#define  PLIF_TEXTMSG_21                  52      /* control type: textMsg, callback function: (none) */
#define  PLIF_TEXTMSG_26                  53      /* control type: textMsg, callback function: (none) */
#define  PLIF_TEXTMSG_70                  54      /* control type: textMsg, callback function: (none) */
#define  PLIF_TEXTMSG_69                  55      /* control type: textMsg, callback function: (none) */
#define  PLIF_TEXTMSG_65                  56      /* control type: textMsg, callback function: (none) */
#define  PLIF_TEXTMSG_45                  57      /* control type: textMsg, callback function: (none) */
#define  PLIF_TEXTMSG_64                  58      /* control type: textMsg, callback function: (none) */
#define  PLIF_SAVE                        59      /* control type: command, callback function: Close_PulsedLIF_Settings */
#define  PLIF_Pulsed_TriggerSource        60      /* control type: ring, callback function: (none) */
#define  PLIF_SPLITTER_19                 61      /* control type: splitter, callback function: (none) */
#define  PLIF_SPLITTER_18                 62      /* control type: splitter, callback function: (none) */
#define  PLIF_SPLITTER_20                 63      /* control type: splitter, callback function: (none) */
#define  PLIF_TEXTMSG_68                  64      /* control type: textMsg, callback function: (none) */
#define  PLIF_SPLITTER_22                 65      /* control type: splitter, callback function: (none) */
#define  PLIF_SPLITTER_25                 66      /* control type: splitter, callback function: (none) */
#define  PLIF_SPLITTER_17                 67      /* control type: splitter, callback function: (none) */
#define  PLIF_DECORATION_5                68      /* control type: deco, callback function: (none) */
#define  PLIF_SPLITTER_31                 69      /* control type: splitter, callback function: (none) */
#define  PLIF_SPLITTER_16                 70      /* control type: splitter, callback function: (none) */
#define  PLIF_DECORATION_2                71      /* control type: deco, callback function: (none) */
#define  PLIF_DECORATION_4                72      /* control type: deco, callback function: (none) */
#define  PLIF_DECORATION_3                73      /* control type: deco, callback function: (none) */
#define  PLIF_DECORATION                  74      /* control type: deco, callback function: (none) */
#define  PLIF_COMMANDBUTTON_4             75      /* control type: command, callback function: PLIF_Wavemeter_Call */
#define  PLIF_SPLITTER_29                 76      /* control type: splitter, callback function: (none) */
#define  PLIF_SPLITTER_32                 77      /* control type: splitter, callback function: (none) */
#define  PLIF_SPLITTER_28                 78      /* control type: splitter, callback function: (none) */
#define  PLIF_SPLITTER_26                 79      /* control type: splitter, callback function: (none) */
#define  PLIF_SPLITTER_24                 80      /* control type: splitter, callback function: (none) */
#define  PLIF_SPLITTER_23                 81      /* control type: splitter, callback function: (none) */
#define  PLIF_SPLITTER_21                 82      /* control type: splitter, callback function: (none) */
#define  PLIF_SPLITTER_3                  83      /* control type: splitter, callback function: (none) */
#define  PLIF_Q850_SimmerDelay            84      /* control type: numeric, callback function: (none) */
#define  PLIF_WAVEMETER_VALUE             85      /* control type: numeric, callback function: (none) */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK Close_PulsedLIF_Settings(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PLIF_Wavemeter_Call(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Wavelength_Move_to_Start(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Wavelength_Move_to_Stop(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif