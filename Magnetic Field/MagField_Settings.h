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

#define  MagField                         1
#define  MagField_SOURCE_1                2       /* control type: numeric, callback function: (none) */
#define  MagField_SOURCE_2                3       /* control type: numeric, callback function: (none) */
#define  MagField_PHASMA_1                4       /* control type: numeric, callback function: (none) */
#define  MagField_PHASMA_2                5       /* control type: numeric, callback function: (none) */
#define  MagField_PHASMA_3                6       /* control type: numeric, callback function: (none) */
#define  MagField_PHASMA_4                7       /* control type: numeric, callback function: (none) */
#define  MagField_Clear                   8       /* control type: command, callback function: clear_graph */
#define  MagField_Simulate                9       /* control type: command, callback function: run_simulation */
#define  MagField_CloseMagField2          10      /* control type: command, callback function: Close_MagField_Settings */
#define  MagField_CloseMagField           11      /* control type: command, callback function: Close_MagField_Settings_Activate */
#define  MagField_Scoil10                 12      /* control type: numeric, callback function: (none) */
#define  MagField_Pcoil11                 13      /* control type: numeric, callback function: (none) */
#define  MagField_Pcoil10                 14      /* control type: numeric, callback function: (none) */
#define  MagField_Pcoil12                 15      /* control type: numeric, callback function: (none) */
#define  MagField_Pcoil8                  16      /* control type: numeric, callback function: (none) */
#define  MagField_Pcoil7                  17      /* control type: numeric, callback function: (none) */
#define  MagField_Pcoil9                  18      /* control type: numeric, callback function: (none) */
#define  MagField_Pcoil6                  19      /* control type: numeric, callback function: (none) */
#define  MagField_Pcoil5                  20      /* control type: numeric, callback function: (none) */
#define  MagField_Scoil9                  21      /* control type: numeric, callback function: (none) */
#define  MagField_Pcoil4                  22      /* control type: numeric, callback function: (none) */
#define  MagField_Pcoil3                  23      /* control type: numeric, callback function: (none) */
#define  MagField_Pcoil2                  24      /* control type: numeric, callback function: (none) */
#define  MagField_Pcoil1                  25      /* control type: numeric, callback function: (none) */
#define  MagField_Scoil8                  26      /* control type: numeric, callback function: (none) */
#define  MagField_Scoil7                  27      /* control type: numeric, callback function: (none) */
#define  MagField_Scoil6                  28      /* control type: numeric, callback function: (none) */
#define  MagField_Scoil5                  29      /* control type: numeric, callback function: (none) */
#define  MagField_Scoil4                  30      /* control type: numeric, callback function: (none) */
#define  MagField_Scoil3                  31      /* control type: numeric, callback function: (none) */
#define  MagField_Scoil2                  32      /* control type: numeric, callback function: (none) */
#define  MagField_Scoil1                  33      /* control type: numeric, callback function: (none) */
#define  MagField_MagFieldGraph           34      /* control type: graph, callback function: (none) */
#define  MagField_Current_or_Field        35      /* control type: binary, callback function: (none) */
#define  MagField_SPLITTER_5              36      /* control type: splitter, callback function: (none) */
#define  MagField_SPLITTER_4              37      /* control type: splitter, callback function: (none) */
#define  MagField_SPLITTER_3              38      /* control type: splitter, callback function: (none) */
#define  MagField_SPLITTER                39      /* control type: splitter, callback function: (none) */
#define  MagField_SPLITTER_2              40      /* control type: splitter, callback function: (none) */
#define  MagField_Sorenson6_IP            41      /* control type: ring, callback function: (none) */
#define  MagField_Sorenson5_IP            42      /* control type: ring, callback function: (none) */
#define  MagField_Sorenson4_IP            43      /* control type: ring, callback function: (none) */
#define  MagField_Sorenson3_IP            44      /* control type: ring, callback function: (none) */
#define  MagField_Sorenson2_IP            45      /* control type: ring, callback function: (none) */
#define  MagField_Sorenson1_IP            46      /* control type: ring, callback function: (none) */
#define  MagField_XANTREX_COM             47      /* control type: ring, callback function: (none) */
#define  MagField_TEXTMSG                 48      /* control type: textMsg, callback function: (none) */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

#define  Parameters                       1
#define  Parameters_PHASMA                2       /* callback function: Parameter_choice */
#define  Parameters_LANGMUIR              3       /* callback function: Parameter_choice */
#define  Parameters_LIF                   4       /* callback function: Parameter_choice */
#define  Parameters_JOERGER               5       /* callback function: Parameter_choice */
#define  Parameters_ESTAT                 6       /* callback function: Parameter_choice */
#define  Parameters_EMISSIVE              7       /* callback function: Parameter_choice */
#define  Parameters_Microwave             8       /* callback function: Parameter_choice */
#define  Parameters_PlasmaGun             9       /* callback function: Parameter_choice */
#define  Parameters_PulsedConfig          10      /* callback function: Parameter_choice */
#define  Parameters_PhotronCamera         11      /* callback function: Parameter_choice */
#define  Parameters_DAQ                   12
#define  Parameters_DAQ_filenum           13      /* callback function: Parameter_choice */
#define  Parameters_DAQ_sweeps            14      /* callback function: Parameter_choice */
#define  Parameters_DAQ_Drate             15      /* callback function: Parameter_choice */


     /* Callback Prototypes: */

int  CVICALLBACK clear_graph(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Close_MagField_Settings(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Close_MagField_Settings_Activate(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
void CVICALLBACK Parameter_choice(int menubar, int menuItem, void *callbackData, int panel);
int  CVICALLBACK run_simulation(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif