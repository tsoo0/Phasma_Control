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

#define  MasterCont                       1
#define  MasterCont_ShutDown              2       /* control type: command, callback function: PHASMA_ShutDown */
#define  MasterCont_Master_Settings_But   3       /* control type: command, callback function: MasterTrigger_Settings */
#define  MasterCont_MasterTrigger_Status  4       /* control type: binary, callback function: MasterTrigger_Activate */
#define  MasterCont_TempDiag_Plot         5       /* control type: binary, callback function: TempDiagPlot_Switch */
#define  MasterCont_TempDiag_Data         6       /* control type: binary, callback function: TempDiagData_Switch */
#define  MasterCont_RFEA_DPlot            7       /* control type: binary, callback function: RFEAPlot_Switch */
#define  MasterCont_Temporary_Settings    8       /* control type: command, callback function: Temporary_Settings */
#define  MasterCont_RFEA_Data             9       /* control type: binary, callback function: RFEAData_Switch */
#define  MasterCont_Lightwave_Plot        10      /* control type: binary, callback function: (none) */
#define  MasterCont_TempDiag_Status       11      /* control type: binary, callback function: Temporary_Activate */
#define  MasterCont_RFEA_Settings         12      /* control type: command, callback function: RFEA_Settings */
#define  MasterCont_McPherson209Plot      13      /* control type: binary, callback function: McPherson209Plot_Switch */
#define  MasterCont_Lightwave_Data        14      /* control type: binary, callback function: (none) */
#define  MasterCont_Ocean_Plot            15      /* control type: binary, callback function: (none) */
#define  MasterCont_MDSDataPath           16      /* control type: string, callback function: (none) */
#define  MasterCont_RawDataPath           17      /* control type: string, callback function: (none) */
#define  MasterCont_PhotronCamera_Plot    18      /* control type: binary, callback function: PhotronCameraPlot_Switch */
#define  MasterCont_RFEA_Status           19      /* control type: binary, callback function: RFEA_Activate */
#define  MasterCont_Photodiode_Plot       20      /* control type: binary, callback function: PhotodiodePlot_Switch */
#define  MasterCont_McPherson209_Data     21      /* control type: binary, callback function: (none) */
#define  MasterCont_Lightwave_Settings    22      /* control type: command, callback function: Lightwave_Settings */
#define  MasterCont_Quantum_Plot          23      /* control type: binary, callback function: (none) */
#define  MasterCont_Ocean_Data            24      /* control type: binary, callback function: (none) */
#define  MasterCont_Lightwave_Status      25      /* control type: binary, callback function: Lightwave_Activate */
#define  MasterCont_PulsedLIF_Plot        26      /* control type: binary, callback function: PulsedLIFPlot_Switch */
#define  MasterCont_PhotronCamera_Data    27      /* control type: binary, callback function: PhotronCameraData_Switch */
#define  MasterCont_McPherson209_Setting  28      /* control type: command, callback function: McPherson209_Settings */
#define  MasterCont_MagProbe_Plot         29      /* control type: binary, callback function: MagProbePlot_Switch */
#define  MasterCont_TripleProbe_Plot      30      /* control type: binary, callback function: TripleProbePlot_Switch */
#define  MasterCont_DoubleProbe_Plot      31      /* control type: binary, callback function: DPDPlot_Switch */
#define  MasterCont_TS_Plot               32      /* control type: binary, callback function: TSPlot_Switch */
#define  MasterCont_Photodiode_Data       33      /* control type: binary, callback function: PhotodiodeData_Switch */
#define  MasterCont_McPherson209_Status   34      /* control type: binary, callback function: McPherson209_Activate */
#define  MasterCont_Housekeeping_Plot     35      /* control type: binary, callback function: HousekeepingPlot_Switch */
#define  MasterCont_OceanOptics_Settings  36      /* control type: command, callback function: OceanOptics_Settings */
#define  MasterCont_Quantum_Data          37      /* control type: binary, callback function: (none) */
#define  MasterCont_Ocean_Status          38      /* control type: binary, callback function: Ocean_Activate */
#define  MasterCont_Camera_Settings       39      /* control type: command, callback function: Camera_Settings */
#define  MasterCont_PulsedLIF_Data        40      /* control type: binary, callback function: PulsedLIFData_Switch */
#define  MasterCont_PhotronCamera_Status  41      /* control type: binary, callback function: Camera_Activate */
#define  MasterCont_Photodiode_Settings   42      /* control type: command, callback function: Photodiode_Settings */
#define  MasterCont_MagProbe_Data         43      /* control type: binary, callback function: MagProbeData_Switch */
#define  MasterCont_TripleProbe_Data      44      /* control type: binary, callback function: TripleProbeData_Switch */
#define  MasterCont_DoubleProbe_Data      45      /* control type: binary, callback function: DPData_Switch */
#define  MasterCont_TS_Data               46      /* control type: binary, callback function: TSData_Switch */
#define  MasterCont_Photodiode_Status     47      /* control type: binary, callback function: Photodiode_Activate */
#define  MasterCont_HeliconSource_Data    48      /* control type: binary, callback function: HeliconData_Switch */
#define  MasterCont_SingleLang_Settings   49      /* control type: command, callback function: SingleLang_Settings */
#define  MasterCont_MagField_Data         50      /* control type: binary, callback function: MagField_Data_Switch */
#define  MasterCont_Housekeeping_Data     51      /* control type: binary, callback function: HousekeepingData_Switch */
#define  MasterCont_Quantum_Settings      52      /* control type: command, callback function: Quantum_Settings */
#define  MasterCont_PlasmaGun_Data        53      /* control type: binary, callback function: (none) */
#define  MasterCont_Quantum_Status        54      /* control type: binary, callback function: Quantum_Activate */
#define  MasterCont_ContinLIF_Settings    55      /* control type: command, callback function: ContinLIF_Settings */
#define  MasterCont_PulsedLIF_Settings    56      /* control type: command, callback function: PulsedLIF_Settings */
#define  MasterCont_MagProbe_Settings     57      /* control type: command, callback function: MagProbe_Settings */
#define  MasterCont_Bdot_Status           58      /* control type: binary, callback function: Bdot_Activate */
#define  MasterCont_SAVEDATATOMDS_2       59      /* control type: textMsg, callback function: (none) */
#define  MasterCont_PulsedLIF_Status      60      /* control type: binary, callback function: PulsedLIF_Activate */
#define  MasterCont_TripleProbe_Settings  61      /* control type: command, callback function: TripleProbe_Settings */
#define  MasterCont_TripleProbe_Status    62      /* control type: binary, callback function: TripleProbe_Activate */
#define  MasterCont_DoubleProbet_Settings 63      /* control type: command, callback function: DoubleProbe_Settings */
#define  MasterCont_DoubleProbe_Status    64      /* control type: binary, callback function: DoubleProbe_Activate */
#define  MasterCont_TS_Settings           65      /* control type: command, callback function: ThomsonsScattering_Settings */
#define  MasterCont_TS_Status             66      /* control type: binary, callback function: TS_Activate */
#define  MasterCont_Helicon_Settings_But  67      /* control type: command, callback function: Helicon_Settings */
#define  MasterCont_HeliconSource_Status  68      /* control type: binary, callback function: HeliconSource_Activate */
#define  MasterCont_MagField_Settings_But 69      /* control type: command, callback function: MagField_Settings */
#define  MasterCont_MagField_Status       70      /* control type: binary, callback function: MagField_Activate_Switch */
#define  MasterCont_House_Settings_But    71      /* control type: command, callback function: Housekeeping_Settings */
#define  MasterCont_Housekeeping_Status   72      /* control type: binary, callback function: Housekeeping_Activate */
#define  MasterCont_Gun_Settings_But      73      /* control type: command, callback function: Gun_Settings */
#define  MasterCont_PlasmaGun_Status      74      /* control type: binary, callback function: PlasmaGun_Activate */
#define  MasterCont_CURRENTDIAGNOSTICPO_2 75      /* control type: textMsg, callback function: (none) */
#define  MasterCont_TextMessage6_2        76      /* control type: textMsg, callback function: (none) */
#define  MasterCont_TextMessage6          77      /* control type: textMsg, callback function: (none) */
#define  MasterCont_TextMessage5          78      /* control type: textMsg, callback function: (none) */
#define  MasterCont_CURRENTDIAGNOSTICPOSI 79      /* control type: textMsg, callback function: (none) */
#define  MasterCont_ARMED                 80      /* control type: textMsg, callback function: (none) */
#define  MasterCont_SAVEDATATOMDS         81      /* control type: textMsg, callback function: (none) */
#define  MasterCont_POWERUPSYSTEM         82      /* control type: textMsg, callback function: (none) */
#define  MasterCont_AutoCycle_Status      83      /* control type: command, callback function: AutoCycle */
#define  MasterCont_TempDiag_Arm          84      /* control type: LED, callback function: (none) */
#define  MasterCont_ManualMatLab          85      /* control type: command, callback function: ManualMatLab */
#define  MasterCont_ManualProbe_Advance   86      /* control type: command, callback function: ManualProbe_Advance */
#define  MasterCont_ManualTrigger_Status  87      /* control type: command, callback function: ManualTrigger */
#define  MasterCont_ManualArm_Status      88      /* control type: command, callback function: ManualArm */
#define  MasterCont_RFEA_Arm              89      /* control type: LED, callback function: (none) */
#define  MasterCont_Lightwave_Arm         90      /* control type: LED, callback function: (none) */
#define  MasterCont_Bdot_Arm              91      /* control type: LED, callback function: (none) */
#define  MasterCont_McPherson209_Arm      92      /* control type: LED, callback function: (none) */
#define  MasterCont_Ocean_Arm             93      /* control type: LED, callback function: (none) */
#define  MasterCont_SPLITTER_8            94      /* control type: splitter, callback function: (none) */
#define  MasterCont_PhotronCamera_Arm     95      /* control type: LED, callback function: (none) */
#define  MasterCont_TripleProbe_Arm       96      /* control type: LED, callback function: (none) */
#define  MasterCont_Photodiode_Arm        97      /* control type: LED, callback function: (none) */
#define  MasterCont_Lang_Arm              98      /* control type: LED, callback function: (none) */
#define  MasterCont_Quantum_Arm           99      /* control type: LED, callback function: (none) */
#define  MasterCont_DoubleProbe_Arm       100     /* control type: LED, callback function: (none) */
#define  MasterCont_ContinLIF_Arm         101     /* control type: LED, callback function: (none) */
#define  MasterCont_PulsedLIF_Arm         102     /* control type: LED, callback function: (none) */
#define  MasterCont_TS_Arm                103     /* control type: LED, callback function: (none) */
#define  MasterCont_PlasmaGun_Arm         104     /* control type: LED, callback function: (none) */
#define  MasterCont_MasterTrigger_Arm     105     /* control type: LED, callback function: (none) */
#define  MasterCont_MagField_Arm          106     /* control type: LED, callback function: (none) */
#define  MasterCont_Helicon_Arm           107     /* control type: LED, callback function: (none) */
#define  MasterCont_Housekeeping_Arm      108     /* control type: LED, callback function: (none) */
#define  MasterCont_CycleRepeat           109     /* control type: numeric, callback function: (none) */
#define  MasterCont_MatLab_Status         110     /* control type: binary, callback function: Auto_Run_Matlab */
#define  MasterCont_ProbeAdvance_Status   111     /* control type: binary, callback function: Auto_Advance_Probes */
#define  MasterCont_SPLITTER              112     /* control type: splitter, callback function: (none) */
#define  MasterCont_SPLITTER_5            113     /* control type: splitter, callback function: (none) */
#define  MasterCont_SPLITTER_6            114     /* control type: splitter, callback function: (none) */
#define  MasterCont_SPLITTER_4            115     /* control type: splitter, callback function: (none) */
#define  MasterCont_SPLITTER_3            116     /* control type: splitter, callback function: (none) */
#define  MasterCont_SPLITTER_2            117     /* control type: splitter, callback function: (none) */
#define  MasterCont_HousekeepingPlot      118     /* control type: graph, callback function: (none) */
#define  MasterCont_MDS_Shot              119     /* control type: numeric, callback function: (none) */
#define  MasterCont_RFEA_Radial           120     /* control type: numeric, callback function: (none) */
#define  MasterCont_QBS_Z                 121     /* control type: numeric, callback function: (none) */
#define  MasterCont_Quantum_Y             122     /* control type: numeric, callback function: (none) */
#define  MasterCont_Quantum_X             123     /* control type: numeric, callback function: (none) */
#define  MasterCont_PLIF_Laser_Wavelength 124     /* control type: numeric, callback function: (none) */
#define  MasterCont_FluctProbe_Radial     125     /* control type: numeric, callback function: (none) */
#define  MasterCont_TripleProbe_Radial    126     /* control type: numeric, callback function: (none) */
#define  MasterCont_Gun2_Radial           127     /* control type: numeric, callback function: (none) */
#define  MasterCont_Gun1_Radial           128     /* control type: numeric, callback function: (none) */
#define  MasterCont_Magnetic_Radial       129     /* control type: numeric, callback function: (none) */
#define  MasterCont_Interval              130     /* control type: numeric, callback function: SetMasterClock */
#define  MasterCont_CountdownClock        131     /* control type: scale, callback function: (none) */
#define  MasterCont_Firing_LED            132     /* control type: LED, callback function: (none) */
#define  MasterCont_Trigger               133     /* control type: textMsg, callback function: (none) */
#define  MasterCont_COUNTDOWN_TIMER       134     /* control type: timer, callback function: countdown_timer */
#define  MasterCont_SPLITTER_10           135     /* control type: splitter, callback function: (none) */
#define  MasterCont_SPLITTER_9            136     /* control type: splitter, callback function: (none) */
#define  MasterCont_TEXTMSG               137     /* control type: textMsg, callback function: (none) */
#define  MasterCont_mdstest               138     /* control type: command, callback function: mdstest */
#define  MasterCont_TEXTMSG_2             139     /* control type: textMsg, callback function: (none) */


     /* Control Arrays: */

#define  CTRLARRAY                        1

     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK Auto_Advance_Probes(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Auto_Run_Matlab(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK AutoCycle(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Bdot_Activate(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Camera_Activate(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Camera_Settings(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK ContinLIF_Settings(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK countdown_timer(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK DoubleProbe_Activate(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK DoubleProbe_Settings(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK DPData_Switch(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK DPDPlot_Switch(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Gun_Settings(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Helicon_Settings(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK HeliconData_Switch(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK HeliconSource_Activate(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Housekeeping_Activate(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Housekeeping_Settings(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK HousekeepingData_Switch(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK HousekeepingPlot_Switch(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Lightwave_Activate(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Lightwave_Settings(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK MagField_Activate_Switch(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK MagField_Data_Switch(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK MagField_Settings(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK MagProbe_Settings(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK MagProbeData_Switch(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK MagProbePlot_Switch(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK ManualArm(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK ManualMatLab(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK ManualProbe_Advance(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK ManualTrigger(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK MasterTrigger_Activate(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK MasterTrigger_Settings(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK McPherson209_Activate(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK McPherson209_Settings(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK McPherson209Plot_Switch(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK mdstest(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Ocean_Activate(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OceanOptics_Settings(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PHASMA_ShutDown(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Photodiode_Activate(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Photodiode_Settings(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PhotodiodeData_Switch(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PhotodiodePlot_Switch(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PhotronCameraData_Switch(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PhotronCameraPlot_Switch(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PlasmaGun_Activate(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PulsedLIF_Activate(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PulsedLIF_Settings(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PulsedLIFData_Switch(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PulsedLIFPlot_Switch(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Quantum_Activate(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Quantum_Settings(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK RFEA_Activate(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK RFEA_Settings(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK RFEAData_Switch(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK RFEAPlot_Switch(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK SetMasterClock(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK SingleLang_Settings(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK TempDiagData_Switch(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK TempDiagPlot_Switch(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Temporary_Activate(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Temporary_Settings(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK ThomsonsScattering_Settings(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK TripleProbe_Activate(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK TripleProbe_Settings(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK TripleProbeData_Switch(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK TripleProbePlot_Switch(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK TS_Activate(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK TSData_Switch(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK TSPlot_Switch(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif