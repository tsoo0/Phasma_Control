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
#define  PlasmaGun_ClosePlasmaGun         15      /* control type: command, callback function: Close_PlasmaGun_Settings */
#define  PlasmaGun_Gun1_Arc_Volts         16      /* control type: numeric, callback function: (none) */
#define  PlasmaGun_Gun1_Bias_Amps         17      /* control type: numeric, callback function: (none) */
#define  PlasmaGun_GunHome                18      /* control type: command, callback function: Gun_Home */
#define  PlasmaGun_Guns_motion            19      /* control type: command, callback function: Guns_Move */
#define  PlasmaGun_Gun1_Bias_Volts        20      /* control type: numeric, callback function: (none) */
#define  PlasmaGun_GunPressure_Target     21      /* control type: numeric, callback function: (none) */
#define  PlasmaGun_GunPressure            22      /* control type: numeric, callback function: (none) */
#define  PlasmaGun_TEXTMSG                23      /* control type: textMsg, callback function: (none) */
#define  PlasmaGun_TEXTMSG_3              24      /* control type: textMsg, callback function: (none) */
#define  PlasmaGun_TEXTMSG_2              25      /* control type: textMsg, callback function: (none) */
#define  PlasmaGun_Gun2_Power_State       26      /* control type: binary, callback function: (none) */
#define  PlasmaGun_Gun1_Power_State       27      /* control type: binary, callback function: (none) */
#define  PlasmaGun_Gun2_Arc_IP            28      /* control type: ring, callback function: (none) */
#define  PlasmaGun_Gun1_Arc_IP            29      /* control type: ring, callback function: (none) */
#define  PlasmaGun_Regulator_IP           30      /* control type: ring, callback function: (none) */
#define  PlasmaGun_TEXTMSG_6              31      /* control type: textMsg, callback function: (none) */
#define  PlasmaGun_SPLITTER               32      /* control type: splitter, callback function: (none) */
#define  PlasmaGun_Gun2_COUNTSPERMM       33      /* control type: numeric, callback function: (none) */
#define  PlasmaGun_TEXTMSG_7              34      /* control type: textMsg, callback function: (none) */
#define  PlasmaGun_Gun1_COUNTSPERMM       35      /* control type: numeric, callback function: (none) */
#define  PlasmaGun_TEXTMSG_20             36      /* control type: textMsg, callback function: (none) */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK Close_PlasmaGun_Settings(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Gun_Home(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Guns_Move(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif