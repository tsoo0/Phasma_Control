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

#define  ThomsonSct                       1
#define  ThomsonSct_SETWAVELEN            2       /* control type: command, callback function: Set_McPherson207 */
#define  ThomsonSct_MOVETOTARGETWAVELENGT 3       /* control type: command, callback function: Move_McPherson207 */
#define  ThomsonSct_RIGOL_PerpScope       4       /* control type: ring, callback function: (none) */
#define  ThomsonSct_RIGOL_ParaScope       5       /* control type: ring, callback function: (none) */
#define  ThomsonSct_TS_Perp_Rigol_Add     6       /* control type: ring, callback function: (none) */
#define  ThomsonSct_TS_Perp_Agilent_Add   7       /* control type: ring, callback function: (none) */
#define  ThomsonSct_TS_Para_Rigol_Add     8       /* control type: ring, callback function: (none) */
#define  ThomsonSct_TS_Para_Agilent_Add   9       /* control type: ring, callback function: (none) */
#define  ThomsonSct_Andor_Temp_Actual     10      /* control type: numeric, callback function: (none) */
#define  ThomsonSct_Andor_Temp            11      /* control type: numeric, callback function: (none) */
#define  ThomsonSct_Andor_Pre_Amp         12      /* control type: numeric, callback function: (none) */
#define  ThomsonSct_Andor_Gain            13      /* control type: numeric, callback function: (none) */
#define  ThomsonSct_Andor_V_bin           14      /* control type: numeric, callback function: (none) */
#define  ThomsonSct_Andor_H_bin           15      /* control type: numeric, callback function: (none) */
#define  ThomsonSct_Andor_Exp_Time        16      /* control type: numeric, callback function: (none) */
#define  ThomsonSct_Andor_GateWidth       17      /* control type: numeric, callback function: (none) */
#define  ThomsonSct_Q850_DDG_Delay        18      /* control type: numeric, callback function: (none) */
#define  ThomsonSct_Q1500_DDG_Delay       19      /* control type: numeric, callback function: (none) */
#define  ThomsonSct_TS_CenterTime         20      /* control type: numeric, callback function: (none) */
#define  ThomsonSct_TS_TimeSpan           21      /* control type: numeric, callback function: (none) */
#define  ThomsonSct_TS_Chan2_Offset       22      /* control type: numeric, callback function: (none) */
#define  ThomsonSct_TS_Chan1_Offset       23      /* control type: numeric, callback function: (none) */
#define  ThomsonSct_TS_VoltageRange2      24      /* control type: numeric, callback function: (none) */
#define  ThomsonSct_TS_VoltageRange1      25      /* control type: numeric, callback function: (none) */
#define  ThomsonSct_TS_TriggerLevel       26      /* control type: numeric, callback function: (none) */
#define  ThomsonSct_McP_Countspernm       27      /* control type: numeric, callback function: (none) */
#define  ThomsonSct_TS_McP_FocalLength    28      /* control type: numeric, callback function: (none) */
#define  ThomsonSct_TS_RecordLength       29      /* control type: slide, callback function: (none) */
#define  ThomsonSct_TS_McP_AngleDiff      30      /* control type: numeric, callback function: (none) */
#define  ThomsonSct_TS_McP_GrooveDen      31      /* control type: numeric, callback function: (none) */
#define  ThomsonSct_TS_McP_Current_Wlengt 32      /* control type: numeric, callback function: (none) */
#define  ThomsonSct_TS_McP_Center         33      /* control type: numeric, callback function: (none) */
#define  ThomsonSct_TEXTMSG_46            34      /* control type: textMsg, callback function: (none) */
#define  ThomsonSct_TEXTMSG_47            35      /* control type: textMsg, callback function: (none) */
#define  ThomsonSct_TEXTMSG_48            36      /* control type: textMsg, callback function: (none) */
#define  ThomsonSct_TEXTMSG_49            37      /* control type: textMsg, callback function: (none) */
#define  ThomsonSct_TS_McP_COM            38      /* control type: ring, callback function: (none) */
#define  ThomsonSct_SPLITTER_28           39      /* control type: splitter, callback function: (none) */
#define  ThomsonSct_SPLITTER_29           40      /* control type: splitter, callback function: (none) */
#define  ThomsonSct_SPLITTER_13           41      /* control type: splitter, callback function: (none) */
#define  ThomsonSct_TEXTMSG_38            42      /* control type: textMsg, callback function: (none) */
#define  ThomsonSct_TEXTMSG_39            43      /* control type: textMsg, callback function: (none) */
#define  ThomsonSct_TEXTMSG_40            44      /* control type: textMsg, callback function: (none) */
#define  ThomsonSct_TEXTMSG_41            45      /* control type: textMsg, callback function: (none) */
#define  ThomsonSct_TEXTMSG_44            46      /* control type: textMsg, callback function: (none) */
#define  ThomsonSct_TEXTMSG_42            47      /* control type: textMsg, callback function: (none) */
#define  ThomsonSct_QSW                   48      /* control type: textMsg, callback function: (none) */
#define  ThomsonSct_EXTTRIGGER            49      /* control type: textMsg, callback function: (none) */
#define  ThomsonSct_OUTPUT                50      /* control type: textMsg, callback function: (none) */
#define  ThomsonSct_QSYNCHOUT             51      /* control type: textMsg, callback function: (none) */
#define  ThomsonSct_CH1                   52      /* control type: textMsg, callback function: (none) */
#define  ThomsonSct_Q850_QSW_Source       53      /* control type: binary, callback function: (none) */
#define  ThomsonSct_TEXTMSG_57            54      /* control type: textMsg, callback function: (none) */
#define  ThomsonSct_ANDORSHUTTERTRIGGER   55      /* control type: textMsg, callback function: (none) */
#define  ThomsonSct_FINALPULSE            56      /* control type: textMsg, callback function: (none) */
#define  ThomsonSct_EXTTRIGGER_2          57      /* control type: textMsg, callback function: (none) */
#define  ThomsonSct_T0                    58      /* control type: textMsg, callback function: (none) */
#define  ThomsonSct_TEXTMSG_59            59      /* control type: textMsg, callback function: (none) */
#define  ThomsonSct_TEXTMSG_60            60      /* control type: textMsg, callback function: (none) */
#define  ThomsonSct_TEXTMSG_43            61      /* control type: textMsg, callback function: (none) */
#define  ThomsonSct_TEXTMSG_8             62      /* control type: textMsg, callback function: (none) */
#define  ThomsonSct_Q850_Circuit_Delay    63      /* control type: numeric, callback function: (none) */
#define  ThomsonSct_Q850_InstrinsicDelay  64      /* control type: numeric, callback function: (none) */
#define  ThomsonSct_Q850_Q_delay          65      /* control type: numeric, callback function: (none) */
#define  ThomsonSct_Q850_Frequency        66      /* control type: numeric, callback function: (none) */
#define  ThomsonSct_Chan2Name_para        67      /* control type: string, callback function: (none) */
#define  ThomsonSct_Chan1Name_para        68      /* control type: string, callback function: (none) */
#define  ThomsonSct_Andor_Camera_Cooler   69      /* control type: binary, callback function: TS_Andor_Temp_Control */
#define  ThomsonSct_TEXTMSG_61            70      /* control type: textMsg, callback function: (none) */
#define  ThomsonSct_TEXTMSG_22            71      /* control type: textMsg, callback function: (none) */
#define  ThomsonSct_TEXTMSG_23            72      /* control type: textMsg, callback function: (none) */
#define  ThomsonSct_TEXTMSG_21            73      /* control type: textMsg, callback function: (none) */
#define  ThomsonSct_CollectionOptics      74      /* control type: binary, callback function: (none) */
#define  ThomsonSct_OrientationSwitch     75      /* control type: binary, callback function: (none) */
#define  ThomsonSct_Q1500_QSW_Source      76      /* control type: binary, callback function: (none) */
#define  ThomsonSct_TEXTMSG_18            77      /* control type: textMsg, callback function: (none) */
#define  ThomsonSct_TEXTMSG_26            78      /* control type: textMsg, callback function: (none) */
#define  ThomsonSct_TEXTMSG_45            79      /* control type: textMsg, callback function: (none) */
#define  ThomsonSct_TEXTMSG_27            80      /* control type: textMsg, callback function: (none) */
#define  ThomsonSct_TEXTMSG_64            81      /* control type: textMsg, callback function: (none) */
#define  ThomsonSct_TEXTMSG_67            82      /* control type: textMsg, callback function: (none) */
#define  ThomsonSct_TEXTMSG_66            83      /* control type: textMsg, callback function: (none) */
#define  ThomsonSct_TEXTMSG_65            84      /* control type: textMsg, callback function: (none) */
#define  ThomsonSct_TEXTMSG_25            85      /* control type: textMsg, callback function: (none) */
#define  ThomsonSct_TEXTMSG_37            86      /* control type: textMsg, callback function: (none) */
#define  ThomsonSct_TEXTMSG_62            87      /* control type: textMsg, callback function: (none) */
#define  ThomsonSct_TEXTMSG_28            88      /* control type: textMsg, callback function: (none) */
#define  ThomsonSct_TEXTMSG_20            89      /* control type: textMsg, callback function: (none) */
#define  ThomsonSct_SAVE_2                90      /* control type: command, callback function: CloseSimmer_ThomsonScattering_Settings */
#define  ThomsonSct_SAVE                  91      /* control type: command, callback function: Close_ThomsonScattering_Settings */
#define  ThomsonSct_Q1500_Circuit_Delay   92      /* control type: numeric, callback function: (none) */
#define  ThomsonSct_Q1500_InstrinsicDelay 93      /* control type: numeric, callback function: (none) */
#define  ThomsonSct_Q850_Simmer_Delay     94      /* control type: numeric, callback function: (none) */
#define  ThomsonSct_Q1500_Simmer_Delay    95      /* control type: numeric, callback function: (none) */
#define  ThomsonSct_Q1500_Q_delay         96      /* control type: numeric, callback function: (none) */
#define  ThomsonSct_Q850_Warmup           97      /* control type: numeric, callback function: (none) */
#define  ThomsonSct_Q1500_Warmup          98      /* control type: numeric, callback function: (none) */
#define  ThomsonSct_Q1500_Frequency       99      /* control type: numeric, callback function: (none) */
#define  ThomsonSct_Chan2Name             100     /* control type: string, callback function: (none) */
#define  ThomsonSct_Chan1Name             101     /* control type: string, callback function: (none) */
#define  ThomsonSct_TS_TriggerSource      102     /* control type: ring, callback function: (none) */
#define  ThomsonSct_SPLITTER_19           103     /* control type: splitter, callback function: (none) */
#define  ThomsonSct_SPLITTER_18           104     /* control type: splitter, callback function: (none) */
#define  ThomsonSct_SPLITTER_20           105     /* control type: splitter, callback function: (none) */
#define  ThomsonSct_SPLITTER_22           106     /* control type: splitter, callback function: (none) */
#define  ThomsonSct_SPLITTER_25           107     /* control type: splitter, callback function: (none) */
#define  ThomsonSct_SPLITTER_17           108     /* control type: splitter, callback function: (none) */
#define  ThomsonSct_DECORATION_5          109     /* control type: deco, callback function: (none) */
#define  ThomsonSct_SPLITTER_27           110     /* control type: splitter, callback function: (none) */
#define  ThomsonSct_SPLITTER_16           111     /* control type: splitter, callback function: (none) */
#define  ThomsonSct_DECORATION_2          112     /* control type: deco, callback function: (none) */
#define  ThomsonSct_SPLITTER              113     /* control type: splitter, callback function: (none) */
#define  ThomsonSct_DECORATION_4          114     /* control type: deco, callback function: (none) */
#define  ThomsonSct_DECORATION_3          115     /* control type: deco, callback function: (none) */
#define  ThomsonSct_DECORATION            116     /* control type: deco, callback function: (none) */
#define  ThomsonSct_SPLITTER_26           117     /* control type: splitter, callback function: (none) */
#define  ThomsonSct_SPLITTER_3            118     /* control type: splitter, callback function: (none) */
#define  ThomsonSct_SPLITTER_21           119     /* control type: splitter, callback function: (none) */
#define  ThomsonSct_SPLITTER_23           120     /* control type: splitter, callback function: (none) */
#define  ThomsonSct_SPLITTER_24           121     /* control type: splitter, callback function: (none) */
#define  ThomsonSct_TIMER_ICCD_STATUS     122     /* control type: timer, callback function: TIMER_UPDATE_ICCD_STATUS */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK Close_ThomsonScattering_Settings(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK CloseSimmer_ThomsonScattering_Settings(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Move_McPherson207(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Set_McPherson207(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK TIMER_UPDATE_ICCD_STATUS(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK TS_Andor_Temp_Control(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif