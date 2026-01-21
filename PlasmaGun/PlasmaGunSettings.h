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
#define  PlasmaGun_Gun2_COM               4       /* control type: ring, callback function: (none) */
#define  PlasmaGun_Gun2_R                 5       /* control type: numeric, callback function: (none) */
#define  PlasmaGun_Gun2_Offset            6       /* control type: numeric, callback function: (none) */
#define  PlasmaGun_Gun1_Offset            7       /* control type: numeric, callback function: (none) */
#define  PlasmaGun_Gun1_R                 8       /* control type: numeric, callback function: (none) */
#define  PlasmaGun_Gun1_Arc_COM           9       /* control type: ring, callback function: (none) */
#define  PlasmaGun_Gun2_Bias_COM          10      /* control type: ring, callback function: (none) */
#define  PlasmaGun_Gun1_Bias_COM          11      /* control type: ring, callback function: (none) */
#define  PlasmaGun_Gun2_Bias_Amps         12      /* control type: numeric, callback function: (none) */
#define  PlasmaGun_Gun2_Bias_Volts        13      /* control type: numeric, callback function: (none) */
#define  PlasmaGun_Gun2_Arc_Amps          14      /* control type: numeric, callback function: (none) */
#define  PlasmaGun_Gun2_Arc_Volts         15      /* control type: numeric, callback function: (none) */
#define  PlasmaGun_Gun1_Arc_Amps          16      /* control type: numeric, callback function: (none) */
#define  PlasmaGun_ClosePlasmaGun         17      /* control type: command, callback function: Close_PlasmaGun_Settings */
#define  PlasmaGun_Gun1_Arc_Volts         18      /* control type: numeric, callback function: (none) */
#define  PlasmaGun_Gun1_Bias_Amps         19      /* control type: numeric, callback function: (none) */
#define  PlasmaGun_GunHome                20      /* control type: command, callback function: Gun_Home */
#define  PlasmaGun_Guns_motion            21      /* control type: command, callback function: Guns_Move */
#define  PlasmaGun_Gun1_Bias_Volts        22      /* control type: numeric, callback function: (none) */
#define  PlasmaGun_GunPressure_Target     23      /* control type: numeric, callback function: (none) */
#define  PlasmaGun_GunPressure            24      /* control type: numeric, callback function: (none) */
#define  PlasmaGun_TEXTMSG                25      /* control type: textMsg, callback function: (none) */
#define  PlasmaGun_TEXTMSG_3              26      /* control type: textMsg, callback function: (none) */
#define  PlasmaGun_TEXTMSG_5              27      /* control type: textMsg, callback function: (none) */
#define  PlasmaGun_TEXTMSG_4              28      /* control type: textMsg, callback function: (none) */
#define  PlasmaGun_TEXTMSG_2              29      /* control type: textMsg, callback function: (none) */
#define  PlasmaGun_Gun2_Power_State       30      /* control type: binary, callback function: (none) */
#define  PlasmaGun_Gun1_Power_State       31      /* control type: binary, callback function: (none) */
#define  PlasmaGun_Regulator_IP           32      /* control type: ring, callback function: (none) */
#define  PlasmaGun_TEXTMSG_6              33      /* control type: textMsg, callback function: (none) */
#define  PlasmaGun_SPLITTER               34      /* control type: splitter, callback function: (none) */
#define  PlasmaGun_Gun2_COUNTSPERMM       35      /* control type: numeric, callback function: (none) */
#define  PlasmaGun_TEXTMSG_7              36      /* control type: textMsg, callback function: (none) */
#define  PlasmaGun_Gun1_COUNTSPERMM       37      /* control type: numeric, callback function: (none) */
#define  PlasmaGun_TEXTMSG_20             38      /* control type: textMsg, callback function: (none) */


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