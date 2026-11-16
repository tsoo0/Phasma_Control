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
#define  ThomsonSct_AndorPulseNum         17      /* control type: numeric, callback function: (none) */
#define  ThomsonSct_Andor_GateWidth       18      /* control type: numeric, callback function: (none) */
#define  ThomsonSct_Q850_DDG_Delay        19      /* control type: numeric, callback function: (none) */
#define  ThomsonSct_Q1500_DDG_Delay       20      /* control type: numeric, callback function: (none) */
#define  ThomsonSct_TS_CenterTime         21      /* control type: numeric, callback function: (none) */
#define  ThomsonSct_TS_TimeSpan           22      /* control type: numeric, callback function: (none) */
#define  ThomsonSct_TS_Chan2_Offset       23      /* control type: numeric, callback function: (none) */
#define  ThomsonSct_TS_Chan1_Offset       24      /* control type: numeric, callback function: (none) */
#define  ThomsonSct_TS_VoltageRange2      25      /* control type: numeric, callback function: (none) */
#define  ThomsonSct_TS_VoltageRange1      26      /* control type: numeric, callback function: (none) */
#define  ThomsonSct_TS_TriggerLevel       27      /* control type: numeric, callback function: (none) */
#define  ThomsonSct_McP_Countspernm       28      /* control type: numeric, callback function: (none) */
#define  ThomsonSct_TS_McP_FocalLength    29      /* control type: numeric, callback function: (none) */
#define  ThomsonSct_TS_RecordLength       30      /* control type: slide, callback function: (none) */
#define  ThomsonSct_TS_McP_AngleDiff      31      /* control type: numeric, callback function: (none) */
#define  ThomsonSct_TS_McP_GrooveDen      32      /* control type: numeric, callback function: (none) */
#define  ThomsonSct_TS_McP_Current_Wlengt 33      /* control type: numeric, callback function: (none) */
#define  ThomsonSct_TS_McP_Center         34      /* control type: numeric, callback function: (none) */
#define  ThomsonSct_TEXTMSG_46            35      /* control type: textMsg, callback function: (none) */
#define  ThomsonSct_TEXTMSG_47            36      /* control type: textMsg, callback function: (none) */
#define  ThomsonSct_TEXTMSG_48            37      /* control type: textMsg, callback function: (none) */
#define  ThomsonSct_TEXTMSG_49            38      /* control type: textMsg, callback function: (none) */
#define  ThomsonSct_TS_McP_COM            39      /* control type: ring, callback function: (none) */
#define  ThomsonSct_SPLITTER_28           40      /* control type: splitter, callback function: (none) */
#define  ThomsonSct_SPLITTER_29           41      /* control type: splitter, callback function: (none) */
#define  ThomsonSct_SPLITTER_13           42      /* control type: splitter, callback function: (none) */
#define  ThomsonSct_TEXTMSG_69            43      /* control type: textMsg, callback function: (none) */
#define  ThomsonSct_TEXTMSG_68            44      /* control type: textMsg, callback function: (none) */
#define  ThomsonSct_TEXTMSG_70            45      /* control type: textMsg, callback function: (none) */
#define  ThomsonSct_TEXTMSG_38            46      /* control type: textMsg, callback function: (none) */
#define  ThomsonSct_TEXTMSG_39            47      /* control type: textMsg, callback function: (none) */
#define  ThomsonSct_TEXTMSG_40            48      /* control type: textMsg, callback function: (none) */
#define  ThomsonSct_TEXTMSG_41            49      /* control type: textMsg, callback function: (none) */
#define  ThomsonSct_TEXTMSG_44            50      /* control type: textMsg, callback function: (none) */
#define  ThomsonSct_TEXTMSG_42            51      /* control type: textMsg, callback function: (none) */
#define  ThomsonSct_QSW                   52      /* control type: textMsg, callback function: (none) */
#define  ThomsonSct_EXTTRIGGER            53      /* control type: textMsg, callback function: (none) */
#define  ThomsonSct_OUTPUT                54      /* control type: textMsg, callback function: (none) */
#define  ThomsonSct_QSYNCHOUT             55      /* control type: textMsg, callback function: (none) */
#define  ThomsonSct_CH1                   56      /* control type: textMsg, callback function: (none) */
#define  ThomsonSct_Q850_QSW_Source       57      /* control type: binary, callback function: (none) */
#define  ThomsonSct_TEXTMSG_57            58      /* control type: textMsg, callback function: (none) */
#define  ThomsonSct_ANDORSHUTTERTRIGGER   59      /* control type: textMsg, callback function: (none) */
#define  ThomsonSct_FINALPULSE            60      /* control type: textMsg, callback function: (none) */
#define  ThomsonSct_EXTTRIGGER_2          61      /* control type: textMsg, callback function: (none) */
#define  ThomsonSct_T0                    62      /* control type: textMsg, callback function: (none) */
#define  ThomsonSct_TEXTMSG_59            63      /* control type: textMsg, callback function: (none) */
#define  ThomsonSct_TEXTMSG_60            64      /* control type: textMsg, callback function: (none) */
#define  ThomsonSct_TEXTMSG_43            65      /* control type: textMsg, callback function: (none) */
#define  ThomsonSct_TEXTMSG_8             66      /* control type: textMsg, callback function: (none) */
#define  ThomsonSct_Q850_Circuit_Delay    67      /* control type: numeric, callback function: (none) */
#define  ThomsonSct_Q850_InstrinsicDelay  68      /* control type: numeric, callback function: (none) */
#define  ThomsonSct_Q850_Q_delay          69      /* control type: numeric, callback function: (none) */
#define  ThomsonSct_Q850_Frequency        70      /* control type: numeric, callback function: (none) */
#define  ThomsonSct_Chan2Name_para        71      /* control type: string, callback function: (none) */
#define  ThomsonSct_Chan1Name_para        72      /* control type: string, callback function: (none) */
#define  ThomsonSct_Andor_Camera_Cooler   73      /* control type: binary, callback function: TS_Andor_Temp_Control */
#define  ThomsonSct_TEXTMSG_61            74      /* control type: textMsg, callback function: (none) */
#define  ThomsonSct_TEXTMSG_22            75      /* control type: textMsg, callback function: (none) */
#define  ThomsonSct_TEXTMSG_23            76      /* control type: textMsg, callback function: (none) */
#define  ThomsonSct_TEXTMSG_21            77      /* control type: textMsg, callback function: (none) */
#define  ThomsonSct_CollectionOptics      78      /* control type: binary, callback function: (none) */
#define  ThomsonSct_OrientationSwitch     79      /* control type: binary, callback function: (none) */
#define  ThomsonSct_AndorMultiPulse       80      /* control type: binary, callback function: (none) */
#define  ThomsonSct_Q1500_QSW_Source      81      /* control type: binary, callback function: (none) */
#define  ThomsonSct_TEXTMSG_18            82      /* control type: textMsg, callback function: (none) */
#define  ThomsonSct_TEXTMSG_26            83      /* control type: textMsg, callback function: (none) */
#define  ThomsonSct_TEXTMSG_45            84      /* control type: textMsg, callback function: (none) */
#define  ThomsonSct_TEXTMSG_27            85      /* control type: textMsg, callback function: (none) */
#define  ThomsonSct_TEXTMSG_64            86      /* control type: textMsg, callback function: (none) */
#define  ThomsonSct_TEXTMSG_67            87      /* control type: textMsg, callback function: (none) */
#define  ThomsonSct_TEXTMSG_66            88      /* control type: textMsg, callback function: (none) */
#define  ThomsonSct_TEXTMSG_65            89      /* control type: textMsg, callback function: (none) */
#define  ThomsonSct_TEXTMSG_25            90      /* control type: textMsg, callback function: (none) */
#define  ThomsonSct_TEXTMSG_37            91      /* control type: textMsg, callback function: (none) */
#define  ThomsonSct_TEXTMSG_62            92      /* control type: textMsg, callback function: (none) */
#define  ThomsonSct_TEXTMSG_28            93      /* control type: textMsg, callback function: (none) */
#define  ThomsonSct_TEXTMSG_20            94      /* control type: textMsg, callback function: (none) */
#define  ThomsonSct_SAVE_2                95      /* control type: command, callback function: CloseSimmer_ThomsonScattering_Settings */
#define  ThomsonSct_SAVE                  96      /* control type: command, callback function: Close_ThomsonScattering_Settings */
#define  ThomsonSct_Q1500_Circuit_Delay   97      /* control type: numeric, callback function: (none) */
#define  ThomsonSct_Q1500_InstrinsicDelay 98      /* control type: numeric, callback function: (none) */
#define  ThomsonSct_Q850_Simmer_Delay     99      /* control type: numeric, callback function: (none) */
#define  ThomsonSct_Q1500_Simmer_Delay    100     /* control type: numeric, callback function: (none) */
#define  ThomsonSct_Q1500_Q_delay         101     /* control type: numeric, callback function: (none) */
#define  ThomsonSct_Q850_Warmup           102     /* control type: numeric, callback function: (none) */
#define  ThomsonSct_Q1500_Warmup          103     /* control type: numeric, callback function: (none) */
#define  ThomsonSct_Q1500_Frequency       104     /* control type: numeric, callback function: (none) */
#define  ThomsonSct_Chan2Name             105     /* control type: string, callback function: (none) */
#define  ThomsonSct_Chan1Name             106     /* control type: string, callback function: (none) */
#define  ThomsonSct_TS_TriggerSource      107     /* control type: ring, callback function: (none) */
#define  ThomsonSct_SPLITTER_19           108     /* control type: splitter, callback function: (none) */
#define  ThomsonSct_SPLITTER_18           109     /* control type: splitter, callback function: (none) */
#define  ThomsonSct_SPLITTER_20           110     /* control type: splitter, callback function: (none) */
#define  ThomsonSct_SPLITTER_22           111     /* control type: splitter, callback function: (none) */
#define  ThomsonSct_SPLITTER_25           112     /* control type: splitter, callback function: (none) */
#define  ThomsonSct_SPLITTER_17           113     /* control type: splitter, callback function: (none) */
#define  ThomsonSct_DECORATION_5          114     /* control type: deco, callback function: (none) */
#define  ThomsonSct_SPLITTER_27           115     /* control type: splitter, callback function: (none) */
#define  ThomsonSct_SPLITTER_16           116     /* control type: splitter, callback function: (none) */
#define  ThomsonSct_DECORATION_2          117     /* control type: deco, callback function: (none) */
#define  ThomsonSct_SPLITTER              118     /* control type: splitter, callback function: (none) */
#define  ThomsonSct_DECORATION_4          119     /* control type: deco, callback function: (none) */
#define  ThomsonSct_DECORATION_3          120     /* control type: deco, callback function: (none) */
#define  ThomsonSct_DECORATION            121     /* control type: deco, callback function: (none) */
#define  ThomsonSct_SPLITTER_26           122     /* control type: splitter, callback function: (none) */
#define  ThomsonSct_SPLITTER_3            123     /* control type: splitter, callback function: (none) */
#define  ThomsonSct_SPLITTER_21           124     /* control type: splitter, callback function: (none) */
#define  ThomsonSct_SPLITTER_23           125     /* control type: splitter, callback function: (none) */
#define  ThomsonSct_SPLITTER_24           126     /* control type: splitter, callback function: (none) */
#define  ThomsonSct_TIMER_ICCD_STATUS     127     /* control type: timer, callback function: TIMER_UPDATE_ICCD_STATUS */


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