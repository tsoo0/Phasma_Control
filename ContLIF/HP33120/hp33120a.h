/*= HP33120a Function Generator Include File ==============================*/

/*=========================================================================*/
/*  Please note that the use of global variables or arrays in the Include  */
/*  file is no longer allowed.                                             */
/*=========================================================================*/
#ifndef __Hp33120a_HEADER
#define __Hp33120a_HEADER

#define PUBLIC __stdcall

#if defined (__cplusplus) || defined(__cplusplus__)
extern "C" {
#endif

/*== HP33120a GLOBAL CONSTANT DECLARATIONS ================================*/
/* Replace 10 with the maximum number of devices of this type being used.  */
#define hp33120a_MAX_INSTR       10

/*== GLOBAL FUNCTION DECLARATIONS =========================================*/

short PUBLIC hp33120a_init (short GPIBAddress, short IDQuery, short resetDevice, 
                        short *instrumentID);
short PUBLIC hp33120a_wf_config (short instrumentID, short waveformShape, char userDefWFName[],
                        double frequencyHz, double amplitude, short amplUnit,
                        double DCOffsetV, short dutyCycle);
short PUBLIC hp33120a_wf_param (short instrumentID, short parameter, double value, 
                        short amplUnit);
short PUBLIC hp33120a_ampl_modulation (short instrumentID, short AMMode, short modulatingWfShape,
                        char userDefWFName[], double modulatingFreqHz,
                        short AMDepth);
short PUBLIC hp33120a_freq_modulation (short instrumentID, short FMMode, short modulatingWfShape,
                        char userDefWFName[], double modulatingFreqHz,
                        double deviationHz);
short PUBLIC hp33120a_burst_modulation (short instrumentID, short burstMode, short trigger,
                        long burstCount, double burstRateHz,
                        short burstPhasedeg);
short PUBLIC hp33120a_sweep_modulation (short instrumentID, short sweepMode, double startFreqHz,
                        double stopFreqHz, double sweepTimes,
                        short sweepSpacing);
short PUBLIC hp33120a_fsk_modulation (short instrumentID, short FSKMode, double hopFreqHz,
                        double FSKRateHz);
short PUBLIC hp33120a_sys_display (short instrumentID, short displayMode, short textMode,
                        char text[]);
short PUBLIC hp33120a_sys_beeper (short instrumentID);
short PUBLIC hp33120a_instr_setup (short instrumentID, short instrConfiguration,
                        short configurationNumber);
short PUBLIC hp33120a_out_impedance (short instrumentID, short outputImpedance);
short PUBLIC hp33120a_send_trig (short instrumentID);
short PUBLIC hp33120a_arb_float_point_wf (short instrumentID, char fileName[]);
short PUBLIC hp33120a_arb_bin_wf (short instrumentID, char fileName[]);
short PUBLIC hp33120a_arb_bin_block_wf (short instrumentID, short numberOfBytes,
                        short binaryBlock[]);
short PUBLIC hp33120a_arb_delete (short instrumentID, short Delete, char arbName[]);
short PUBLIC hp33120a_arb_copy (short instrumentID, char destinationArbName[]);
short PUBLIC hp33120a_arb_value (short instrumentID, short waveformParameter, char arbName[],
                        double *value);
short PUBLIC hp33120a_arb_catalog (short instrumentID, char catalog[]);
short PUBLIC hp33120a_arb_nv_catalog (short instrumentID, char catalog[]);
short PUBLIC hp33120a_arb_free (short instrumentID, short *numberofFreeSlots);
short PUBLIC hp33120a_reset (short instrumentID);
short PUBLIC hp33120a_self_test (short instrumentID, short *result);
short PUBLIC hp33120a_error (short instrumentID, short *errorNumber, char errorMessage[]);
short PUBLIC hp33120a_revision (short instrumentID, char VPPRev[], char instrDriverRev[],
                        char firmwareRev[], char minFirmwareRev[]);
short PUBLIC hp33120a_sys_version (short instrumentID, char version[]);
short PUBLIC hp33120a_write (short instrumentID, char writeBuffer[]);
short PUBLIC hp33120a_read (short instrumentID, short numberBytesToRead, char readBuffer[],
                        short *numBytesRead);
short PUBLIC hp33120a_close (short instrumentID);

#if defined (__cplusplus) || defined(__cplusplus__)
}
#endif
/*=== END INCLUDE FILE ====================================================*/

#endif

