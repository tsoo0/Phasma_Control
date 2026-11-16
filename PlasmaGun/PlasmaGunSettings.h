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

#define  PlasmaGun                        1
#define  PlasmaGun_Gun2_IP                2       /* control type: ring, callback function: (none) */
#define  PlasmaGun_Gun1_IP                3       /* control type: ring, callback function: (none) */
#define  PlasmaGun_Gun2_R                 4       /* control type: numeric, callback function: (none) */
#define  PlasmaGun_Gun2_Current           5       /* control type: numeric, callback function: (none) */
#define  PlasmaGun_Gun1_Current           6       /* control type: numeric, callback function: (none) */
#define  PlasmaGun_Gun2_Offset            7       /* control type: numeric, callback function: (none) */
#define  PlasmaGun_Gun1_Offset            8       /* control type: numeric, callback function: (none) */
#define  PlasmaGun_Gun1_R                 9       /* control type: numeric, callback function: (none) */
#define  PlasmaGun_Gun2_Bias_COM          10      /* control type: ring, callback function: (none) */
#define  PlasmaGun_Gun1_Bias_COM          11      /* control type: ring, callback function: (none) */
#define  PlasmaGun_Gun2_Bias_Amps         12      /* control type: numeric, callback function: (none) */
#define  PlasmaGun_Gun2_Bias_Volts        13      /* control type: numeric, callback function: (none) */
#define  PlasmaGun_Gun2_Arc_Amps          14      /* control type: numeric, callback function: (none) */
#define  PlasmaGun_Gun2_Arc_Volts         15      /* control type: numeric, callback function: (none) */
#define  PlasmaGun_Gun1_Arc_Amps          16      /* control type: numeric, callback function: (none) */
#define  PlasmaGun_ClosePlasmaGun_2       17      /* control type: command, callback function: Close_PlasmaGun_Settings_nosave */
#define  PlasmaGun_ClosePlasmaGun         18      /* control type: command, callback function: Close_PlasmaGun_Settings */
#define  PlasmaGun_Gun1_Arc_Volts         19      /* control type: numeric, callback function: (none) */
#define  PlasmaGun_Gun1_Bias_Amps         20      /* control type: numeric, callback function: (none) */
#define  PlasmaGun_GunHome                21      /* control type: command, callback function: Gun_Home */
#define  PlasmaGun_Guns_motion            22      /* control type: command, callback function: Guns_Move */
#define  PlasmaGun_Gun1_Bias_Volts        23      /* control type: numeric, callback function: (none) */
#define  PlasmaGun_GunPressure_Target     24      /* control type: numeric, callback function: (none) */
#define  PlasmaGun_GaugeCalibration       25      /* control type: numeric, callback function: (none) */
#define  PlasmaGun_GunPressure            26      /* control type: numeric, callback function: (none) */
#define  PlasmaGun_TEXTMSG_2              27      /* control type: textMsg, callback function: (none) */
#define  PlasmaGun_Gun2_Power_State       28      /* control type: binary, callback function: (none) */
#define  PlasmaGun_Gun1_Power_State       29      /* control type: binary, callback function: (none) */
#define  PlasmaGun_Gun2_Arc_IP            30      /* control type: ring, callback function: (none) */
#define  PlasmaGun_Gun1_Arc_IP            31      /* control type: ring, callback function: (none) */
#define  PlasmaGun_Regulator_IP           32      /* control type: ring, callback function: (none) */
#define  PlasmaGun_TEXTMSG_6              33      /* control type: textMsg, callback function: (none) */
#define  PlasmaGun_SPLITTER               34      /* control type: splitter, callback function: (none) */
#define  PlasmaGun_Gun2_COUNTSPERMM       35      /* control type: numeric, callback function: (none) */
#define  PlasmaGun_TEXTMSG_7              36      /* control type: textMsg, callback function: (none) */
#define  PlasmaGun_TEXTMSG_20             37      /* control type: textMsg, callback function: (none) */
#define  PlasmaGun_TEXTMSG_3              38      /* control type: textMsg, callback function: (none) */
#define  PlasmaGun_Gun1_COUNTSPERMM       39      /* control type: numeric, callback function: (none) */
#define  PlasmaGun_TEXTMSG                40      /* control type: textMsg, callback function: (none) */
#define  PlasmaGun_GUN_PRESSURE_TIMER     41      /* control type: timer, callback function: Pressure_Refresh_Timer */
#define  PlasmaGun_Gun2_Act               42      /* control type: binary, callback function: (none) */
#define  PlasmaGun_Gun1_Act               43      /* control type: binary, callback function: (none) */
#define  PlasmaGun_TEXTMSG_21             44      /* control type: textMsg, callback function: (none) */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK Close_PlasmaGun_Settings(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Close_PlasmaGun_Settings_nosave(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Gun_Home(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Guns_Move(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Pressure_Refresh_Timer(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif