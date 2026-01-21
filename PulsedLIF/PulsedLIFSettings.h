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
#define  PLIF_Current_Wavelength          6       /* control type: numeric, callback function: (none) */
#define  PLIF_Wavelength_Increment        7       /* control type: numeric, callback function: (none) */
#define  PLIF_Rigol_Address               8       /* control type: ring, callback function: (none) */
#define  PLIF_Agilent_Address             9       /* control type: ring, callback function: (none) */
#define  PLIF_Pulsed_CenterTime           10      /* control type: numeric, callback function: (none) */
#define  PLIF_Pulsed_TimeSpan             11      /* control type: numeric, callback function: (none) */
#define  PLIF_Pulsed_Chan2_Offset         12      /* control type: numeric, callback function: (none) */
#define  PLIF_Pulsed_Chan1_Offset         13      /* control type: numeric, callback function: (none) */
#define  PLIF_Pulsed_VoltageRange2        14      /* control type: numeric, callback function: (none) */
#define  PLIF_Pulsed_VoltageRange1        15      /* control type: numeric, callback function: (none) */
#define  PLIF_Pulsed_TriggerLevel         16      /* control type: numeric, callback function: (none) */
#define  PLIF_TEXTMSG_67                  17      /* control type: textMsg, callback function: (none) */
#define  PLIF_Wave_Current                18      /* control type: command, callback function: Wavelength_Current */
#define  PLIF_Wave_Stop                   19      /* control type: command, callback function: Wavelength_Move_to_Stop */
#define  PLIF_Wave_Start                  20      /* control type: command, callback function: Wavelength_Move_to_Start */
#define  PLIF_TEXTMSG_54                  21      /* control type: textMsg, callback function: (none) */
#define  PLIF_TEXTMSG_55                  22      /* control type: textMsg, callback function: (none) */
#define  PLIF_TEXTMSG_56                  23      /* control type: textMsg, callback function: (none) */
#define  PLIF_TEXTMSG_44                  24      /* control type: textMsg, callback function: (none) */
#define  PLIF_TEXTMSG_42                  25      /* control type: textMsg, callback function: (none) */
#define  PLIF_QSW                         26      /* control type: textMsg, callback function: (none) */
#define  PLIF_EXTTRIGGER                  27      /* control type: textMsg, callback function: (none) */
#define  PLIF_OUTPUT                      28      /* control type: textMsg, callback function: (none) */
#define  PLIF_QSYNCHOUT                   29      /* control type: textMsg, callback function: (none) */
#define  PLIF_CH1                         30      /* control type: textMsg, callback function: (none) */
#define  PLIF_Q850_QSW_Source_2           31      /* control type: binary, callback function: (none) */
#define  PLIF_Q850_QSW_Source             32      /* control type: binary, callback function: (none) */
#define  PLIF_Pulsed_RecordLength         33      /* control type: slide, callback function: (none) */
#define  PLIF_TEXTMSG_57                  34      /* control type: textMsg, callback function: (none) */
#define  PLIF_FINALPULSE                  35      /* control type: textMsg, callback function: (none) */
#define  PLIF_EXTTRIGGER_2                36      /* control type: textMsg, callback function: (none) */
#define  PLIF_T0                          37      /* control type: textMsg, callback function: (none) */
#define  PLIF_Wavelength_Return           38      /* control type: binary, callback function: (none) */
#define  PLIF_TEXTMSG_58                  39      /* control type: textMsg, callback function: (none) */
#define  PLIF_TEXTMSG_60                  40      /* control type: textMsg, callback function: (none) */
#define  PLIF_TEXTMSG_43                  41      /* control type: textMsg, callback function: (none) */
#define  PLIF_TEXTMSG_8                   42      /* control type: textMsg, callback function: (none) */
#define  PLIF_Q850_Circuit_Delay          43      /* control type: numeric, callback function: (none) */
#define  PLIF_Q850_InstrinsicDelay        44      /* control type: numeric, callback function: (none) */
#define  PLIF_Q850_Q_delay                45      /* control type: numeric, callback function: (none) */
#define  PLIF_Q850_Frequency              46      /* control type: numeric, callback function: (none) */
#define  PLIF_PLIF_Warmup                 47      /* control type: numeric, callback function: (none) */
#define  PLIF_Chan3Name                   48      /* control type: string, callback function: (none) */
#define  PLIF_Chan2Name                   49      /* control type: string, callback function: (none) */
#define  PLIF_Chan1Name                   50      /* control type: string, callback function: (none) */
#define  PLIF_GenTecPort                  51      /* control type: ring, callback function: (none) */
#define  PLIF_TEXTMSG_22                  52      /* control type: textMsg, callback function: (none) */
#define  PLIF_TEXTMSG_21                  53      /* control type: textMsg, callback function: (none) */
#define  PLIF_TEXTMSG_26                  54      /* control type: textMsg, callback function: (none) */
#define  PLIF_TEXTMSG_69                  55      /* control type: textMsg, callback function: (none) */
#define  PLIF_TEXTMSG_66                  56      /* control type: textMsg, callback function: (none) */
#define  PLIF_TEXTMSG_65                  57      /* control type: textMsg, callback function: (none) */
#define  PLIF_TEXTMSG_45                  58      /* control type: textMsg, callback function: (none) */
#define  PLIF_TEXTMSG_64                  59      /* control type: textMsg, callback function: (none) */
#define  PLIF_SAVE                        60      /* control type: command, callback function: Close_PulsedLIF_Settings */
#define  PLIF_Pulsed_TriggerSource        61      /* control type: ring, callback function: (none) */
#define  PLIF_SPLITTER_19                 62      /* control type: splitter, callback function: (none) */
#define  PLIF_SPLITTER_18                 63      /* control type: splitter, callback function: (none) */
#define  PLIF_SPLITTER_20                 64      /* control type: splitter, callback function: (none) */
#define  PLIF_TEXTMSG_71                  65      /* control type: textMsg, callback function: (none) */
#define  PLIF_TEXTMSG_70                  66      /* control type: textMsg, callback function: (none) */
#define  PLIF_TEXTMSG_68                  67      /* control type: textMsg, callback function: (none) */
#define  PLIF_SPLITTER_22                 68      /* control type: splitter, callback function: (none) */
#define  PLIF_SPLITTER_25                 69      /* control type: splitter, callback function: (none) */
#define  PLIF_SPLITTER_17                 70      /* control type: splitter, callback function: (none) */
#define  PLIF_DECORATION_5                71      /* control type: deco, callback function: (none) */
#define  PLIF_SPLITTER_31                 72      /* control type: splitter, callback function: (none) */
#define  PLIF_SPLITTER_32                 73      /* control type: splitter, callback function: (none) */
#define  PLIF_SPLITTER_16                 74      /* control type: splitter, callback function: (none) */
#define  PLIF_DECORATION_2                75      /* control type: deco, callback function: (none) */
#define  PLIF_DECORATION_4                76      /* control type: deco, callback function: (none) */
#define  PLIF_DECORATION_3                77      /* control type: deco, callback function: (none) */
#define  PLIF_DECORATION                  78      /* control type: deco, callback function: (none) */
#define  PLIF_SPLITTER_30                 79      /* control type: splitter, callback function: (none) */
#define  PLIF_SPLITTER_29                 80      /* control type: splitter, callback function: (none) */
#define  PLIF_SPLITTER_28                 81      /* control type: splitter, callback function: (none) */
#define  PLIF_SPLITTER_26                 82      /* control type: splitter, callback function: (none) */
#define  PLIF_SPLITTER_24                 83      /* control type: splitter, callback function: (none) */
#define  PLIF_SPLITTER_23                 84      /* control type: splitter, callback function: (none) */
#define  PLIF_SPLITTER_21                 85      /* control type: splitter, callback function: (none) */
#define  PLIF_SPLITTER_3                  86      /* control type: splitter, callback function: (none) */
#define  PLIF_Q850_SimmerDelay            87      /* control type: numeric, callback function: (none) */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK Close_PulsedLIF_Settings(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Wavelength_Current(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Wavelength_Move_to_Start(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Wavelength_Move_to_Stop(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif