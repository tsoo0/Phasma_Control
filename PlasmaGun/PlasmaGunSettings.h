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
#define  PlasmaGun_Gun2_Offset            5       /* control type: numeric, callback function: (none) */
#define  PlasmaGun_Gun1_Offset            6       /* control type: numeric, callback function: (none) */
#define  PlasmaGun_Gun1_R                 7       /* control type: numeric, callback function: (none) */
#define  PlasmaGun_Gun2_Bias_COM          8       /* control type: ring, callback function: (none) */
#define  PlasmaGun_Gun1_Bias_COM          9       /* control type: ring, callback function: (none) */
#define  PlasmaGun_Gun2_Bias_Amps         10      /* control type: numeric, callback function: (none) */
#define  PlasmaGun_Gun2_Bias_Volts        11      /* control type: numeric, callback function: (none) */
#define  PlasmaGun_Gun2_Arc_Amps          12      /* control type: numeric, callback function: (none) */
#define  PlasmaGun_Gun2_Arc_Volts         13      /* control type: numeric, callback function: (none) */
#define  PlasmaGun_Gun1_Arc_Amps          14      /* control type: numeric, callback function: (none) */
#define  PlasmaGun_ClosePlasmaGun_2       15      /* control type: command, callback function: Close_PlasmaGun_Settings_nosave */
#define  PlasmaGun_ClosePlasmaGun         16      /* control type: command, callback function: Close_PlasmaGun_Settings */
#define  PlasmaGun_Gun1_Arc_Volts         17      /* control type: numeric, callback function: (none) */
#define  PlasmaGun_Gun1_Bias_Amps         18      /* control type: numeric, callback function: (none) */
#define  PlasmaGun_GunHome                19      /* control type: command, callback function: Gun_Home */
#define  PlasmaGun_Guns_motion            20      /* control type: command, callback function: Guns_Move */
#define  PlasmaGun_Gun1_Bias_Volts        21      /* control type: numeric, callback function: (none) */
#define  PlasmaGun_GunPressure_Target     22      /* control type: numeric, callback function: (none) */
#define  PlasmaGun_GunPressure            23      /* control type: numeric, callback function: (none) */
#define  PlasmaGun_TEXTMSG_2              24      /* control type: textMsg, callback function: (none) */
#define  PlasmaGun_Gun2_Power_State       25      /* control type: binary, callback function: (none) */
#define  PlasmaGun_Gun1_Power_State       26      /* control type: binary, callback function: (none) */
#define  PlasmaGun_Gun2_Arc_IP            27      /* control type: ring, callback function: (none) */
#define  PlasmaGun_Gun1_Arc_IP            28      /* control type: ring, callback function: (none) */
#define  PlasmaGun_Regulator_IP           29      /* control type: ring, callback function: (none) */
#define  PlasmaGun_TEXTMSG_6              30      /* control type: textMsg, callback function: (none) */
#define  PlasmaGun_SPLITTER               31      /* control type: splitter, callback function: (none) */
#define  PlasmaGun_Gun2_COUNTSPERMM       32      /* control type: numeric, callback function: (none) */
#define  PlasmaGun_TEXTMSG_7              33      /* control type: textMsg, callback function: (none) */
#define  PlasmaGun_TEXTMSG_20             34      /* control type: textMsg, callback function: (none) */
#define  PlasmaGun_TEXTMSG_3              35      /* control type: textMsg, callback function: (none) */
#define  PlasmaGun_Gun1_COUNTSPERMM       36      /* control type: numeric, callback function: (none) */
#define  PlasmaGun_TEXTMSG                37      /* control type: textMsg, callback function: (none) */
#define  PlasmaGun_GUN_PRESSURE_TIMER     38      /* control type: timer, callback function: Pressure_Refresh_Timer */


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