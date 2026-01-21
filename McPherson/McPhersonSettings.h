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

#define  McP209                           1
#define  McP209_SETWAVELEN                2       /* control type: command, callback function: Set_McPherson209 */
#define  McP209_MOVETOTARGETWAVELEN_2     3       /* control type: command, callback function: Move_McPherson209 */
#define  McP209_SAVE                      4       /* control type: command, callback function: Close_McPherson209_Settings */
#define  McP209_McP209_FocalLength        5       /* control type: numeric, callback function: (none) */
#define  McP209_McP209_AngleDiff          6       /* control type: numeric, callback function: (none) */
#define  McP209_McP209_GrooveDen          7       /* control type: numeric, callback function: (none) */
#define  McP209_McP209_Current_Wlengt     8       /* control type: numeric, callback function: (none) */
#define  McP209_McP209_Center             9       /* control type: numeric, callback function: (none) */
#define  McP209_Andor_Temp_Actual         10      /* control type: numeric, callback function: (none) */
#define  McP209_McP209_COM                11      /* control type: ring, callback function: (none) */
#define  McP209_Andor_Temp                12      /* control type: numeric, callback function: (none) */
#define  McP209_Andor_Pre_Amp             13      /* control type: numeric, callback function: (none) */
#define  McP209_Andor_Gain                14      /* control type: numeric, callback function: (none) */
#define  McP209_Andor_V_bin               15      /* control type: numeric, callback function: (none) */
#define  McP209_Andor_H_bin               16      /* control type: numeric, callback function: (none) */
#define  McP209_Andor_Exp_Time            17      /* control type: numeric, callback function: (none) */
#define  McP209_DDG_Delay                 18      /* control type: numeric, callback function: (none) */
#define  McP209_Andor_GateWidth           19      /* control type: numeric, callback function: (none) */
#define  McP209_TEXTMSG_61                20      /* control type: textMsg, callback function: (none) */
#define  McP209_McPhersonGraph            21      /* control type: graph, callback function: (none) */
#define  McP209_Andor_Camera_Cooler       22      /* control type: binary, callback function: McP_Andor_Temp_Control */
#define  McP209_TEXTMSG_23                23      /* control type: textMsg, callback function: (none) */
#define  McP209_TEXTMSG_18                24      /* control type: textMsg, callback function: (none) */
#define  McP209_Chan1Name                 25      /* control type: string, callback function: (none) */
#define  McP209_TEXTMSG_39                26      /* control type: textMsg, callback function: (none) */
#define  McP209_TEXTMSG_40                27      /* control type: textMsg, callback function: (none) */
#define  McP209_TEXTMSG_41                28      /* control type: textMsg, callback function: (none) */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK Close_McPherson209_Settings(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK McP_Andor_Temp_Control(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Move_McPherson209(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Set_McPherson209(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif