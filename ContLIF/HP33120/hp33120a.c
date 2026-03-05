/***************************************************************************/
/*  Copyright 1994 National Instruments Corporation.  All Rights Reserved. */
/***************************************************************************/

#include <gpib.h>
#include <formatio.h>
#include <hp33120a.h>


/*= HP 33120A 15 MHz Function / Arbitrary Waveform Generator ==============*/
/* LabWindows/CVI 3.1 Instrument Driver                                    */
/* Original Release: August 1994                                           */
/* By: MH, Technical University Ostrava, Czech Republic,                   */
/*              Ph +(042) 69-699-1507                                      */
/* For LabWindows Technical Support in the United States call:             */
/*              National Instruments, Austin, Texas                        */
/*              Ph (800) 433-3488    Fax (512) 794-5678                    */
/* Originally written in C                                                 */
/* Modification History:                                                   */
/*                                                                         */
/*      November 1994:  Added code necessary for creation of DLL           */
/*                      This change not done to the LW/DOS version.        */
/*                      Modified by: SA, NI, Austin TX.                    */
/*                                                                         */
/*      January 1996:   Changed the order of the commands sent to the      */
/*                      instrument in the wf_config function so that the   */
/*                      square wave duty cycle is set correctly, previously*/
/*                      the duty cycle was incorrectly forced to 50%. Also */
/*                      changed all datatypes that were ints to shorts or  */
/*                      longs and added [b2] or [b4] to all Format and     */
/*                      Scan specifiers of type %d and %i. Also in the CVI */
/*                      driver fp file changed all controls that were ints */
/*                      to shorts or longs, this change was not possible   */
/*                      in the LW/DOS driver as the fp controls do not have*/
/*                      the datatype short and the default for an int in   */
/*                      LW/DOS is a short.                                 */
/*                      Modified by: JRO, NI, Austin TX.                   */
/*                                                                         */
/*         June 1996:   Modified for MDLL compatibility.                   */
/*                      Modified by:  TKN, National Instruments            */
/*                      Phone (800) 433-3488  Fax (512) 794-5678           */    
/*                                                                         */
/*     December 1996:   Modified function wf_config - waveform shapes set  */
/*                      incorrectly for predefined shapes. Catalog         */
/*                      functions - corrected                              */
/*                      Modified by: M.Cejka, Technical University Brno    */
/*                      Czech Republic. Phone +(042) 5-7275-125            */
/*                      E-MAIL : cejka@dame.fee.vutbr.cz                   */  
/*                                                                         */
/*=========================================================================*/

/*== CONSTANT DECLARATIONS ================================================*/
#define BIN_BUF_LENGTH           32500
#define VXIPP_REVISION           "1.0"
#define DRIVER_REVISION          "Rev 1.3, 12/96, CVI 4.0.1"
#define MINFW_REVISION           "1.0-2.0-1.0"

/*= INSTRUMENT TABLE ======================================================*/
/* address array: contains the GPIB addresses of opened instruments.       */
/* bd array: contains the device descriptors returned by OpenDev.          */
/* instr_cnt: contains the number of instruments open of this model type.  */
/*=========================================================================*/
static short address[hp33120a_MAX_INSTR + 1];
static short bd[hp33120a_MAX_INSTR + 1];
static short instr_cnt;

/*= STATIC VARIABLES ======================================================*/
/* cmd is a buffer for GPIB I/O strings.                                   */
/* hp33120a_err: the error variable for the instrument module              */
/* ibcnt: contains the number of bytes transferred by GPIB reads and       */
/*        writes.  See the GPIB library I/O Class for more information     */
/*=========================================================================*/
static char cmd[BIN_BUF_LENGTH];
static char bin_buf[BIN_BUF_LENGTH];
static short hp33120a_err;
static short count;

/*= INSTRUMENT-DEPENDENT COMMAND ARRAYS ===================================*/
static char *shape_arr[13];
static char *unit_arr[3];
static char *out_z_arr[2];
static char *am_mode_arr[3];
static char *sour_arr[3];
static char *trig_arr[4];
static char *fs_spacing_arr[4];
static char *instr_config_arr[3];
static char *arb_func_arr[4];
static char *wf_parameter_arr[3];

/*= UTILITY ROUTINES ======================================================*/
short hp33120a_open_instr (short, short *);
short hp33120a_close_instr (short);
short hp33120a_invalid_integer_range (short, short, short, short);
short hp33120a_invalid_longint_range (long, long, long, short);
short hp33120a_invalid_real_range (double, double, double, short);
short hp33120a_device_closed (short);
short hp33120a_read_data (short, char *, short);
short hp33120a_write_data (short, char *, short);
short hp33120a_write_data_file (short, char *);
short hp33120a_set_timeout (short, short, short *);
short hp33120a_sys_status (short);
short hp33120a_check_shape (short);
void hp33120a_setup_arrays (void);

/*=========================================================================*/
/* Function: Initialize                                                    */
/* Purpose:  This function opens the instrument, queries the instrument    */
/*           for its ID, and initializes the instrument to a known state.  */
/*=========================================================================*/
short PUBLIC hp33120a_init (short addr, short id_query, short rest, short *instrID)
{
    short  ID, old_timeout;

    if (hp33120a_invalid_integer_range (addr, 0, 30, -1) != 0)
        return hp33120a_err;
    if (hp33120a_invalid_integer_range (id_query, 0, 1, -2) != 0)
        return hp33120a_err;
    if (hp33120a_invalid_integer_range (rest, 0, 1, -3) != 0)
        return hp33120a_err;

    if (hp33120a_open_instr (addr, &ID) != 0)
        return hp33120a_err;

    if (id_query) {
        if (hp33120a_write_data (ID, "*IDN?", 5) != 0) {
            hp33120a_close_instr (ID);
            return hp33120a_err;
        }
        if (hp33120a_read_data (ID, cmd, 150) != 0) {
            hp33120a_close_instr (ID);
            return hp33120a_err;
        }

        Scan (cmd, "HEWLETT-PACKARD,33120");
        if ((short)NumFmtdBytes () != 21) {
            hp33120a_err = 223;
            hp33120a_close_instr (ID);
            return hp33120a_err;
        }
    }

    if (rest) {
        if (hp33120a_write_data (ID, "*RST", 4) != 0) {
            hp33120a_close_instr (ID);
            return hp33120a_err;
        }
    }
    hp33120a_setup_arrays ();
    *instrID = ID;

    if (hp33120a_set_timeout (ID, 13, &old_timeout))
        return hp33120a_err;

    return hp33120a_err;
}

/*=========================================================================*/
/* Function: Config Waveform                                               */
/* Purpose:  This function sets parameters of the waveform.                */
/*=========================================================================*/
short PUBLIC hp33120a_wf_config (short instrID, short shape, char user_name[], 
                        double freq, double ampl, short unit, double offset, 
                        short d_cycle)
{
    if (hp33120a_invalid_integer_range (instrID, 1, hp33120a_MAX_INSTR, -1) != 0)
        return hp33120a_err;
    if (hp33120a_invalid_integer_range (shape, 0, 12, -2) != 0)
        return hp33120a_err;
    if (hp33120a_invalid_real_range (freq, 1E-4, 1.5E7, -3) != 0)
        return hp33120a_err;
    if (unit == 0) {
        if (hp33120a_invalid_real_range (ampl, 5E-2, 20.0, -4) != 0)
            return hp33120a_err;
    }
    if (hp33120a_invalid_integer_range (unit, 0, 2, -5) != 0)
        return hp33120a_err;
    if (hp33120a_invalid_real_range (offset, -10.0, 10.0, -6) != 0)
        return hp33120a_err;
    if (freq > 5E6)
        if (hp33120a_invalid_integer_range (d_cycle, 40, 60, -7) != 0)
            return hp33120a_err;
    else
        if (hp33120a_invalid_integer_range (d_cycle, 20, 80, -7) != 0)
            return hp33120a_err;
    if (hp33120a_device_closed(instrID) != 0)
        return hp33120a_err;

    if (shape > 5) {
        if (shape == 12) {
            Fmt (cmd, "%s<SOUR:FUNC:USER %s;:", user_name);
            count = (short)(short)NumFmtdBytes();
        }
        else {
            Fmt (cmd, "%s<SOUR:FUNC:USER %s;:", shape_arr[shape]);
            count = (short)NumFmtdBytes();
        }

        Fmt (cmd, "%s[a]<SOUR:VOLT:UNIT %s;:APPL:USER %f, %f, %f ;:SOUR:PULS:DCYC %d[b2]",
            unit_arr[unit], freq, ampl, offset, d_cycle); 
        count += (short)NumFmtdBytes();
        if (hp33120a_write_data (instrID, cmd, count) != 0)
            return hp33120a_err;
    }
    else {
        Fmt (cmd, "%s<SOUR:VOLT:UNIT %s;:APPL:%s %f, %f, %f ;:SOUR:PULS:DCYC %d[b2]",
            unit_arr[unit], shape_arr[shape], freq, ampl, offset, d_cycle); 
        if (hp33120a_write_data (instrID, cmd, (short)NumFmtdBytes()) != 0)
            return hp33120a_err;
    }

    hp33120a_sys_status (instrID);
    return hp33120a_err;
}

/*=========================================================================*/
/* Function: Waveform Parameters                                           */
/* Purpose:  This function sets separately amplitude, frequency, and       */
/*           offset.                                                       */
/*=========================================================================*/
short PUBLIC hp33120a_wf_param (short instrID, short wf_parameter, double wf_value, short unit)
{
    if (hp33120a_invalid_integer_range (instrID, 1, hp33120a_MAX_INSTR, -1) != 0)
        return hp33120a_err;
    if (hp33120a_invalid_integer_range (wf_parameter, 0, 2, -2) != 0)
        return hp33120a_err;
    switch (wf_parameter) {
        case 0: if (hp33120a_invalid_real_range (wf_value, 1E-4, 1.5E7, -3) != 0)
                    return hp33120a_err;
                break;
        case 1: if (unit == 0)
                    if (hp33120a_invalid_real_range (wf_value, 5E-2, 20.0, -4) != 0)
                        return hp33120a_err;
                break;
        case 2: if (hp33120a_invalid_real_range (wf_value, -10.0, 10.0, -6) != 0)
                    return hp33120a_err;
                break;
    }
    if (hp33120a_invalid_integer_range (unit, 0, 2, -5) != 0)
        return hp33120a_err;
    if (hp33120a_device_closed(instrID) != 0)
        return hp33120a_err;

    if (wf_parameter == 1) {
        Fmt (cmd, "%s<SOUR:VOLT:UNIT %s", unit_arr[unit]);
        if (hp33120a_write_data (instrID, cmd, (short)NumFmtdBytes()) != 0)
            return hp33120a_err;
    }

    Fmt (cmd, "%s<SOUR:%s %f", wf_parameter_arr[wf_parameter], wf_value);
    if (hp33120a_write_data (instrID, cmd, (short)NumFmtdBytes()) != 0)
        return hp33120a_err;

    hp33120a_sys_status (instrID);
    return hp33120a_err;
}

/*=========================================================================*/
/* Function: Amplitude Modulation                                          */
/* Purpose:  This function allowes amplitude modulation and sets its       */
/*           parameters.                                                   */
/*=========================================================================*/
short PUBLIC hp33120a_ampl_modulation (short instrID, short am_mode, short am_shape,
                              char user_name[], double am_freq, short am_depth)
{
    if (hp33120a_invalid_integer_range (instrID, 1, hp33120a_MAX_INSTR, -1) != 0)
        return hp33120a_err;
    if (hp33120a_invalid_integer_range (am_mode, 0, 2, -2) != 0)
        return hp33120a_err;
    if (hp33120a_invalid_integer_range (am_shape, 1, 12, -3) != 0)
        return hp33120a_err;
    if (hp33120a_invalid_real_range (am_freq, 1E-2, 2E4, -4) != 0)
        return hp33120a_err;
    if (hp33120a_invalid_integer_range (am_depth, 0, 120, -5) != 0)
        return hp33120a_err;
    if (hp33120a_device_closed(instrID) != 0)
        return hp33120a_err;

    if (am_mode == 0) {
        Fmt (cmd, "%s<SOUR:AM:STAT OFF");
        if (hp33120a_write_data (instrID, cmd, (short)NumFmtdBytes()) != 0)
            return hp33120a_err;
    }
    else {
        if (hp33120a_check_shape (instrID))
            return hp33120a_err;

        if (am_shape > 5) {
            if (am_shape == 12) {
                Fmt (cmd, "%s<SOUR:FUNC:USER %s;:AM:INT:FUNC USER", user_name);
            }
            else
                Fmt (cmd, "%s<SOUR:FUNC:USER %s;:AM:INT:FUNC USER",
                     shape_arr[am_shape]);
            if (hp33120a_write_data (instrID, cmd, (short)NumFmtdBytes()) != 0)
                return hp33120a_err;

            hp33120a_sys_status (instrID);
            if (hp33120a_err != 0)
                return hp33120a_err;
        }
        else {
            Fmt (cmd, "%s<SOUR:AM:INT:FUNC %s", shape_arr[am_shape]);
            if (hp33120a_write_data (instrID, cmd, (short)NumFmtdBytes()) != 0)
                return hp33120a_err;
        }

        Fmt (cmd, "%s<SOUR:AM:INT:FREQ %f;:AM:DEPT %d[b2];SOUR %s;STAT ON",
             am_freq, am_depth, am_mode_arr[am_mode]);
        if (hp33120a_write_data (instrID, cmd, (short)NumFmtdBytes()) != 0)
            return hp33120a_err;

        hp33120a_sys_status (instrID);
        if (hp33120a_err != 0)
            return hp33120a_err;
    }

    return hp33120a_err;
}

/*=========================================================================*/
/* Function: Frequency Modulation                                          */
/* Purpose:  This function allowes frequency modulation and sets its       */
/*           parameters.                                                   */
/*=========================================================================*/
short PUBLIC hp33120a_freq_modulation (short instrID, short fm_mode, short fm_shape,
                         char user_name[], double fm_freq, double fm_deviation)
{
    if (hp33120a_invalid_integer_range (instrID, 1, hp33120a_MAX_INSTR, -1) != 0)
        return hp33120a_err;
    if (hp33120a_invalid_integer_range (fm_mode, 0, 2, -2) != 0)
        return hp33120a_err;
    if (hp33120a_invalid_integer_range (fm_shape, 1, 12, -3) != 0)
        return hp33120a_err;
    if (hp33120a_invalid_real_range (fm_freq, 1E-2, 1E4, -4) != 0)
        return hp33120a_err;
    if (hp33120a_invalid_real_range (fm_deviation, 1E-2, 7.5E6, -5) != 0)
        return hp33120a_err;
    if (hp33120a_device_closed(instrID) != 0)
        return hp33120a_err;

    if (fm_mode == 0) {
        Fmt (cmd, "%s<SOUR:FM:STAT OFF");
        if (hp33120a_write_data (instrID, cmd, (short)NumFmtdBytes()) != 0)
            return hp33120a_err;
    }
    else {
        if (hp33120a_check_shape (instrID))
            return hp33120a_err;

        if (fm_shape > 5) {
            if (fm_shape == 12)
                Fmt (cmd, "%s<SOUR:FUNC:USER %s;:FM:INT:FUNC USER", user_name);
            else
                Fmt (cmd, "%s<SOUR:FUNC:USER %s;:FM:INT:FUNC USER",
                     shape_arr[fm_shape]);

            if (hp33120a_write_data (instrID, cmd, (short)NumFmtdBytes()) != 0)
                return hp33120a_err;

            hp33120a_sys_status (instrID);
            if (hp33120a_err != 0)
                return hp33120a_err;
        }
        else {
            Fmt (cmd, "%s<SOUR:FM:INT:FUNC %s", shape_arr[fm_shape]);
            if (hp33120a_write_data (instrID, cmd, (short)(short)NumFmtdBytes()) != 0)
                return hp33120a_err;
        }

        Fmt (cmd, "%s<SOUR:FM:INT:FREQ %f;:FM:DEV %f;STAT ON",
             fm_freq, fm_deviation);
        if (hp33120a_write_data (instrID, cmd, (short)NumFmtdBytes()) != 0)
            return hp33120a_err;

        hp33120a_sys_status (instrID);
        if (hp33120a_err != 0)
            return hp33120a_err;
    }

    return hp33120a_err;
}

/*=========================================================================*/
/* Function: Burst Modulation                                              */
/* Purpose:  This function allows burst modulation and sets its parameters.*/
/*=========================================================================*/
short PUBLIC hp33120a_burst_modulation (short instrID, short bm_mode, short bm_ext_func,
                               long bm_count, double bm_rate, short bm_phase)
{
    if (hp33120a_invalid_integer_range (instrID, 1, hp33120a_MAX_INSTR, -1) != 0)
        return hp33120a_err;
    if (hp33120a_invalid_integer_range (bm_mode, 0, 2, -2) != 0)
        return hp33120a_err;
    if (hp33120a_invalid_integer_range (bm_ext_func, 1, 3, -3) != 0)
        return hp33120a_err;
    if (hp33120a_invalid_longint_range (bm_count, 1, 50000, -4) != 0)
        return hp33120a_err;
    if (hp33120a_invalid_real_range (bm_rate, 1E-2, 5E4, -5) != 0)
        return hp33120a_err;
    if (hp33120a_invalid_integer_range (bm_phase, -360, 360, -6) != 0)
        return hp33120a_err;
    if (hp33120a_device_closed(instrID) != 0)
        return hp33120a_err;

    if (bm_mode == 0) {
        Fmt (cmd, "%s<SOUR:BM:STAT OFF");
        if (hp33120a_write_data (instrID, cmd, (short)NumFmtdBytes()) != 0)
            return hp33120a_err;
    }
    else {
        if (hp33120a_check_shape (instrID))
            return hp33120a_err;

        Fmt (cmd, "%s<SOUR:BM:NCYC %i[b4];PHAS %d[b2];SOUR %s;:TRIG:SOUR %s;:SOUR:BM:INT:RATE %f;:BM:STAT ON",
              bm_count, bm_phase, sour_arr[bm_mode], trig_arr[bm_ext_func],
              bm_rate);
        if (hp33120a_write_data (instrID, cmd, (short)NumFmtdBytes()) != 0)
            return hp33120a_err;
    }

    hp33120a_sys_status (instrID);
    return hp33120a_err;
}

/*=========================================================================*/
/* Function: Frequency Sweep                                               */
/* Purpose:  This function allows frequency sweep and sets its parameters. */
/*=========================================================================*/
short PUBLIC hp33120a_sweep_modulation (short instrID, short fs_mode, double fs_start_freq,
                          double fs_stop_freq, double fs_time, short fs_spacing)
{
    double change;

    if (hp33120a_invalid_integer_range (instrID, 1, hp33120a_MAX_INSTR, -1) != 0)
        return hp33120a_err;
    if (hp33120a_invalid_integer_range (fs_mode, 0, 3, -2) != 0)
        return hp33120a_err;
    if (hp33120a_invalid_real_range (fs_start_freq, 1E-2, 1.5E7, -3) != 0)
        return hp33120a_err;
    if (hp33120a_invalid_real_range (fs_stop_freq, 1E-2, 1.5E7, -4) != 0)
        return hp33120a_err;
    if (hp33120a_invalid_real_range (fs_time, 1E-3, 5E2, -5) != 0)
        return hp33120a_err;
    if (hp33120a_invalid_integer_range (fs_spacing, 0, 3, -6) != 0)
        return hp33120a_err;
    if (fs_start_freq >= fs_stop_freq) {
        hp33120a_err = -7;
        return hp33120a_err;
    }
    if (hp33120a_device_closed(instrID) != 0)
        return hp33120a_err;

    if (fs_mode == 0) {
        Fmt (cmd, "%s<SOUR:SWE:STAT OFF");
        if (hp33120a_write_data (instrID, cmd, (short)NumFmtdBytes()) != 0)
            return hp33120a_err;
    }
    else {
        if (hp33120a_check_shape (instrID))
            return hp33120a_err;

        if (fs_spacing > 1) {
            change = fs_start_freq;
            fs_start_freq = fs_stop_freq;
            fs_stop_freq = change;
        }

        Fmt (cmd, "%s<TRIG:SOUR %s;:SOUR:SWE:SPAC %s;TIME %f;:SOUR:FREQ:STAR %f;STOP %f;:SWE:STAT ON",
             trig_arr[fs_mode], fs_spacing_arr[fs_spacing], fs_time,
             fs_start_freq, fs_stop_freq);
        if (hp33120a_write_data (instrID, cmd, (short)NumFmtdBytes()) != 0)
            return hp33120a_err;
    }

    hp33120a_sys_status (instrID);
    return hp33120a_err;
}

/*=========================================================================*/
/* Function: FSK Modulation                                                */
/* Purpose:  This function allows FSK modulation and sets its parameters.  */
/*=========================================================================*/
short PUBLIC hp33120a_fsk_modulation (short instrID, short fsk_mode, double hop_freq,
                             double fsk_rate)
{
    if (hp33120a_invalid_integer_range (instrID, 1, hp33120a_MAX_INSTR, -1) != 0)
        return hp33120a_err;
    if (hp33120a_invalid_integer_range (fsk_mode, 0, 2, -2) != 0)
        return hp33120a_err;
    if (hp33120a_invalid_real_range (hop_freq, 1E-2, 1.5E7, -3) != 0)
        return hp33120a_err;
    if (hp33120a_invalid_real_range (fsk_rate, 1E-2, 5E4, -4) != 0)
        return hp33120a_err;
    if (hp33120a_device_closed(instrID) != 0)
        return hp33120a_err;

    if (fsk_mode == 0) {
        Fmt (cmd, "%s<SOUR:FSK:STAT OFF");
        if (hp33120a_write_data (instrID, cmd, (short)NumFmtdBytes()) != 0)
            return hp33120a_err;
    }
    else {
        if (hp33120a_check_shape (instrID))
            return hp33120a_err;

        Fmt (cmd, "%s<SOUR:FSK:FREQ %f;SOUR %s;INT:RATE %f;:FSK:STAT ON",
             hop_freq, sour_arr[fsk_mode], fsk_rate);
        if (hp33120a_write_data (instrID, cmd, (short)NumFmtdBytes()) != 0)
            return hp33120a_err;
    }

    hp33120a_sys_status (instrID);
    return hp33120a_err;
}

/*=========================================================================*/
/* Function: Display                                                       */
/* Purpose:  This function allows  to display a message on the front-panel */
/*           display of the instrument.                                    */
/*=========================================================================*/
short PUBLIC hp33120a_sys_display (short instrID, short disp_mode, short txt_mode, char text[])
{
    if (hp33120a_invalid_integer_range (instrID, 1, hp33120a_MAX_INSTR, -1) != 0)
        return hp33120a_err;
    if (hp33120a_invalid_integer_range (disp_mode, 0, 1, -2) != 0)
        return hp33120a_err;
    if (hp33120a_invalid_integer_range (txt_mode, 0, 1, -3) != 0)
        return hp33120a_err;
    if (hp33120a_device_closed(instrID) != 0)
        return hp33120a_err;

    if (disp_mode) {
        Fmt (cmd, "%s<DISP ON");
        count = (short)NumFmtdBytes ();
    }
    else {
        Fmt (cmd, "%s<DISP OFF");
        count = (short)NumFmtdBytes ();
    }
    if (txt_mode) {
        Fmt (cmd, "%s[a]<;:DISP:TEXT '%s'", text);
        if ((short)NumFmtdBytes () > 25) {
            hp33120a_err = -4;
            return hp33120a_err;
        }
        count += (short)NumFmtdBytes ();
    }
    else {
        Fmt (cmd, "%s[a]<;:DISP:TEXT:CLE");
        count += (short)NumFmtdBytes ();
    }
    if (hp33120a_write_data (instrID, cmd, count) != 0)
        return hp33120a_err;

    return hp33120a_err;
}

/*=========================================================================*/
/* Function: Beeper                                                        */
/* Purpose:  This function issues a single beep immediately.               */
/*=========================================================================*/
short PUBLIC hp33120a_sys_beeper (short instrID)
{
    if (hp33120a_invalid_integer_range (instrID, 1, hp33120a_MAX_INSTR, -1) != 0)
        return hp33120a_err;
    if (hp33120a_device_closed(instrID) != 0)
        return hp33120a_err;

    Fmt (cmd, "%s<SYST:BEEP");
    if (hp33120a_write_data (instrID, cmd, (short)NumFmtdBytes()) != 0)
        return hp33120a_err;

    return hp33120a_err;
}

/*=========================================================================*/
/* Function: Instrument Setup                                              */
/* Purpose:  This function stores and recalls instrument configuration.    */
/*=========================================================================*/
short PUBLIC hp33120a_instr_setup (short instrID, short instr_config, short config_number)
{
    if (hp33120a_invalid_integer_range (instrID, 1, hp33120a_MAX_INSTR, -1) != 0)
        return hp33120a_err;
    if (hp33120a_invalid_integer_range (instr_config, 0, 2, -2) != 0)
        return hp33120a_err;
    if (hp33120a_invalid_integer_range (config_number, 0, 3, -3) != 0)
        return hp33120a_err;
    if (hp33120a_device_closed(instrID) != 0)
        return hp33120a_err;

    Fmt (cmd, "%s<%s %d[b2]", instr_config_arr[instr_config], config_number);
    if (hp33120a_write_data (instrID, cmd, (short)NumFmtdBytes()) != 0)
        return hp33120a_err;

    return hp33120a_err;
}

/*=========================================================================*/
/* Function: Output Impedance                                              */
/* Purpose:  This function sets the output impedance.                      */
/*=========================================================================*/
short PUBLIC hp33120a_out_impedance (short instrID, short out_z)
{
    if (hp33120a_invalid_integer_range (instrID, 1, hp33120a_MAX_INSTR, -1) != 0)
        return hp33120a_err;
    if (hp33120a_invalid_integer_range (out_z, 0, 1, -2) != 0)
        return hp33120a_err;
    if (hp33120a_device_closed(instrID) != 0)
        return hp33120a_err;

    Fmt (cmd, "%s<OUTP:LOAD %s", out_z_arr[out_z]);
    if (hp33120a_write_data (instrID, cmd, (short)NumFmtdBytes()) != 0)
        return hp33120a_err;

    return hp33120a_err;
}

/*=========================================================================*/
/* Function: Send Trigger                                                  */
/* Purpose:  This function sends one single trigger to the instrument.     */
/*=========================================================================*/
short PUBLIC hp33120a_send_trig (short instrID)
{
    if (hp33120a_invalid_integer_range (instrID, 1, hp33120a_MAX_INSTR, -1) != 0)
        return hp33120a_err;
    if (hp33120a_device_closed(instrID) != 0)
        return hp33120a_err;

    Fmt (cmd, "%s<*TRG");
    if (hp33120a_write_data (instrID, cmd, (short)NumFmtdBytes()) != 0)
        return hp33120a_err;

    hp33120a_sys_status (instrID);
    return hp33120a_err;
}

/*=========================================================================*/
/* Function: Load Float-Pt Waveform                                        */
/* Purpose:  This function sends floating-point values into instrument's   */
/*           volatile memory.                                              */
/*=========================================================================*/
short PUBLIC hp33120a_arb_float_point_wf (short instrID, char file_name[])
{
    if (hp33120a_invalid_integer_range (instrID, 1, hp33120a_MAX_INSTR, -1) != 0)
        return hp33120a_err;
    if (hp33120a_device_closed(instrID) != 0)
        return hp33120a_err;

    /* Disable sending END message (EOI) */
    if (ibeot (bd[instrID], 0) & 0x8000) {
        hp33120a_err = 301;
        return hp33120a_err;
    }

    /* Send the header for downloading floating-point values */
    Fmt (cmd, "%s<DATA VOLATILE, ");
    if (hp33120a_write_data (instrID, cmd, (short)NumFmtdBytes()) != 0) {
        ibeot (bd[instrID], 1);
        return hp33120a_err;
    }

    /* Enable sending END message (EOI) */
    if (ibeot (bd[instrID], 1) & 0x8000) {
        hp33120a_err = 301;
        return hp33120a_err;
    }

    /* Send the file "file_name" containing waveform points */
    if (hp33120a_write_data_file (instrID, file_name) != 0)
        return hp33120a_err;

    hp33120a_sys_status (instrID);
    return hp33120a_err;
}

/*=========================================================================*/
/* Function: Load Binary Integer Waveform                                  */
/* Purpose:  This function sends binary integer values into instrument's   */
/*           volatile memory.                                              */
/*=========================================================================*/
short PUBLIC hp33120a_arb_bin_wf (short instrID, char file_name[])
{
    if (hp33120a_invalid_integer_range (instrID, 1, hp33120a_MAX_INSTR, -1) != 0)
        return hp33120a_err;
    if (hp33120a_device_closed(instrID) != 0)
        return hp33120a_err;

    /* Disable sending END message (EOI) */
    if (ibeot (bd[instrID], 0) & 0x8000) {
        hp33120a_err = 301;
        return hp33120a_err;
    }

    /* Send the header for uploading binary integer values */
    Fmt (cmd, "%s<DATA:DAC VOLATILE, ");
    if (hp33120a_write_data (instrID, cmd, (short)NumFmtdBytes()) != 0) {
        ibeot (bd[instrID], 1);
        return hp33120a_err;
    }

    /* Enable sending END message (EOI) */
    if (ibeot (bd[instrID], 1) & 0x8000) {
        hp33120a_err = 301;
        return hp33120a_err;
    }

    /* Send the file "file_name" containing waveform points */
    if (hp33120a_write_data_file (instrID, file_name) != 0)
        return hp33120a_err;

    hp33120a_sys_status (instrID);
    return hp33120a_err;
}

/*=========================================================================*/
/* Function: Load Binary Block                                             */
/* Purpose:  This function sends binary integer values into instrument's   */
/*           volatile memory.                                              */
/*=========================================================================*/
short PUBLIC hp33120a_arb_bin_block_wf (short instrID, short numb_of_bytes, short bin_block[])
{
    short numb_of_points;
    long numb_length;

    if (hp33120a_invalid_integer_range (instrID, 1, hp33120a_MAX_INSTR, -1) != 0)
        return hp33120a_err;
    if (hp33120a_invalid_integer_range (numb_of_bytes, 0, 32000, -2) != 0)
        return hp33120a_err;
    if (hp33120a_device_closed(instrID) != 0)
        return hp33120a_err;

    numb_of_points = numb_of_bytes / 2;

    Fmt (cmd, "%s<%d[b2]", numb_of_bytes);
    numb_length = (short)NumFmtdBytes();

    Fmt (bin_buf, "%s<FORM:BORD SWAP;:DATA:DAC VOLATILE, #%d[b4]%d[b2]" ,
         numb_length, numb_of_bytes);
    count = (short)NumFmtdBytes ();
    Fmt (bin_buf, "%*i[b2i*z]<%*i[b2]", numb_of_points, (short)NumFmtdBytes (),
         numb_of_points, bin_block);
    count += (short)NumFmtdBytes ();
    if (hp33120a_write_data (instrID, bin_buf, count) != 0)
        return hp33120a_err;

    hp33120a_sys_status (instrID);
    return hp33120a_err;
}

/*=========================================================================*/
/* Function: Delete Waveform                                               */
/* Purpose:  This function deletes specified arbitrary waveform from       */
/*           instrument's memory.                                          */
/*=========================================================================*/
short PUBLIC hp33120a_arb_delete (short instrID, short del_mode, char arb_name[])
{
    if (hp33120a_invalid_integer_range (instrID, 1, hp33120a_MAX_INSTR, -1) != 0)
        return hp33120a_err;
    if (hp33120a_invalid_integer_range (del_mode, 0, 1, -2) != 0)
        return hp33120a_err;
    if (hp33120a_device_closed(instrID) != 0)
        return hp33120a_err;

    if (del_mode)
        Fmt (cmd, "%s<DATA:DEL:ALL");
    else
        Fmt (cmd, "%s<DATA:DEL %s", arb_name);
    if (hp33120a_write_data (instrID, cmd, (short)NumFmtdBytes()) != 0)
        return hp33120a_err;

    hp33120a_sys_status (instrID);
    return hp33120a_err;
}

/*=========================================================================*/
/* Function: Copy Waveform                                                 */
/* Purpose:  This function copies the waveform from volatile memory to     */
/*           the specified name in non_volatile memory.                    */
/*=========================================================================*/
short PUBLIC hp33120a_arb_copy (short instrID, char dest_name[])
{
    if (hp33120a_invalid_integer_range (instrID, 1, hp33120a_MAX_INSTR, -1) != 0)
        return hp33120a_err;
    if (hp33120a_device_closed(instrID) != 0)
        return hp33120a_err;

    Fmt (cmd, "%s<DATA:COPY %s", dest_name);
    if (hp33120a_write_data (instrID, cmd, (short)NumFmtdBytes()) != 0)
        return hp33120a_err;

    hp33120a_sys_status (instrID);
    return hp33120a_err;
}

/*=========================================================================*/
/* Function: Arbitrary Waveform Value                                      */
/* Purpose:  This function queries the instrument for arbitrary waveform   */
/*           specific values.                                              */
/*=========================================================================*/
short PUBLIC hp33120a_arb_value (short instrID, short arb_func, char arb_name[], double *value)
{
    if (hp33120a_invalid_integer_range (instrID, 1, hp33120a_MAX_INSTR, -1) != 0)
        return hp33120a_err;
    if (hp33120a_invalid_integer_range (arb_func, 0, 3, -2) != 0)
        return hp33120a_err;
    if (hp33120a_device_closed(instrID) != 0)
        return hp33120a_err;

    Fmt (cmd, "%s<DATA:ATTR:%s? %s", arb_func_arr[arb_func], arb_name);
    if (hp33120a_write_data (instrID, cmd, (short)NumFmtdBytes()) != 0)
        return hp33120a_err;
    if (hp33120a_read_data (instrID, cmd, 150) != 0)
        return hp33120a_err;

    if (Scan (cmd, "%s>%f", value) != 1) {
        hp33120a_err = 236;
        return hp33120a_err;
    }

    hp33120a_sys_status (instrID);
    return hp33120a_err;
}

/*=========================================================================*/
/* Function: Catalog                                                       */
/* Purpose:  This function queries the list of all waveforms currently     */
/*           loaded to memory.                                             */
/*=========================================================================*/
short PUBLIC hp33120a_arb_catalog (short instrID, char catalog[])
{
    if (hp33120a_invalid_integer_range (instrID, 1, hp33120a_MAX_INSTR, -1) != 0)
        return hp33120a_err;
    if (hp33120a_device_closed(instrID) != 0)
        return hp33120a_err;

    Fmt (cmd, "%s<DATA:CAT?");
    if (hp33120a_write_data (instrID, cmd, (short)NumFmtdBytes()) != 0)
        return hp33120a_err;
    if (hp33120a_read_data (instrID, cmd, 100) != 0)
        return hp33120a_err;
    if (Scan (cmd, "%s>%s[t10]", catalog) != 1) {
        hp33120a_err = 236;
        return hp33120a_err;
    }

    return hp33120a_err;
}

/*=========================================================================*/
/* Function: Non-Volatile Catalog                                          */
/* Purpose:  This function queries the list of all user-defined arbitrary  */
/*           waveforms currently loaded to non-volatile memory.            */
/*=========================================================================*/
short PUBLIC hp33120a_arb_nv_catalog (short instrID, char catalog[])
{
    if (hp33120a_invalid_integer_range (instrID, 1, hp33120a_MAX_INSTR, -1) != 0)
        return hp33120a_err;
    if (hp33120a_device_closed(instrID) != 0)
        return hp33120a_err;

    Fmt (cmd, "%s<DATA:NVOL:CAT?");
    if (hp33120a_write_data (instrID, cmd, (short)NumFmtdBytes()) != 0)
        return hp33120a_err;
    if (hp33120a_read_data (instrID, cmd, 100) != 0)
        return hp33120a_err;
    if (Scan (cmd, "%s>%s[t10]", catalog) != 1) {
        hp33120a_err = 236;
        return hp33120a_err;
    }
    
    return hp33120a_err;
}

/*=========================================================================*/
/* Function: Free Memory Slots                                             */
/* Purpose:  This function queries the number of non-volatile memory slots */
/*           available to store user-defined waveforms.                    */
/*=========================================================================*/
short PUBLIC hp33120a_arb_free (short instrID, short *free)
{
    if (hp33120a_invalid_integer_range (instrID, 1, hp33120a_MAX_INSTR, -1) != 0)
        return hp33120a_err;
    if (hp33120a_device_closed(instrID) != 0)
        return hp33120a_err;

    Fmt (cmd, "%s<DATA:NVOL:FREE?");
    if (hp33120a_write_data (instrID, cmd, (short)NumFmtdBytes()) != 0)
        return hp33120a_err;
    if (hp33120a_read_data (instrID, cmd, 150) != 0)
        return hp33120a_err;

    if (Scan (cmd, "%s>%d[b2]", free) != 1) {
        hp33120a_err = 236;
        return hp33120a_err;
    }

    return hp33120a_err;
}

/*=========================================================================*/
/* Function: Reset                                                         */
/* Purpose:  This function resets the instrument and places it in default  */
/*           state.                                                        */
/*=========================================================================*/
short PUBLIC hp33120a_reset (short instrID)
{
    if (hp33120a_invalid_integer_range (instrID, 1, hp33120a_MAX_INSTR, -1) != 0)
        return hp33120a_err;
    if (hp33120a_device_closed(instrID) != 0)
        return hp33120a_err;

    Fmt (cmd, "%s<*RST");
    if (hp33120a_write_data (instrID, cmd, (short)NumFmtdBytes()) != 0)
        return hp33120a_err;

    return hp33120a_err;
}

/*=========================================================================*/
/* Function: Self-Test                                                     */
/* Purpose:  This function function causes the instrument to perform       */
/*           a self-test and returns the result of that self-test.         */
/*=========================================================================*/
short PUBLIC hp33120a_self_test (short instrID, short *result)
{
    if (hp33120a_invalid_integer_range (instrID, 1, hp33120a_MAX_INSTR, -1) != 0)
        return hp33120a_err;
    if (hp33120a_device_closed(instrID) != 0)
        return hp33120a_err;

    Fmt (cmd, "%s<*TST?");
    if (hp33120a_write_data (instrID, cmd, (short)NumFmtdBytes()) != 0)
        return hp33120a_err;
    if (hp33120a_read_data (instrID, cmd, 150) != 0)
        return hp33120a_err;

    if (Scan (cmd, "%s>%d[b2]", result) != 1) {
        hp33120a_err = 236;
        return hp33120a_err;
    }

    hp33120a_sys_status (instrID);
    return hp33120a_err;
}

/*=========================================================================*/
/* Function: Error Query                                                   */
/* Purpose:  This function reads one error message from the instrument's   */
/*           error queue.                                                  */
/*=========================================================================*/
short PUBLIC hp33120a_error (short instrID, short *err_number, char err_message[])
{
    if (hp33120a_invalid_integer_range (instrID, 1, hp33120a_MAX_INSTR, -1) != 0)
        return hp33120a_err;
    if (hp33120a_device_closed(instrID) != 0)
        return hp33120a_err;

    Fmt (cmd, "%s<SYST:ERR?");
    if (hp33120a_write_data (instrID, cmd, (short)NumFmtdBytes()) != 0)
        return hp33120a_err;
    if (hp33120a_read_data (instrID, cmd, 150) != 0)
        return hp33120a_err;

    if (Scan (cmd, "%s>%d[b2],%s[t10]", err_number, err_message) != 2) {
        hp33120a_err = 236;
        return hp33120a_err;
    }

    return hp33120a_err;
}

/*=========================================================================*/
/* Function: Revision                                                      */
/* Purpose:  This function returns the driver and instrument revision      */
/*           number.                                                       */
/*=========================================================================*/
short PUBLIC hp33120a_revision (short instrID, char vxipp_rev[], char driver_rev[],
                   char instr_rev[], char minfw_rev[])
{
    if (hp33120a_invalid_integer_range (instrID, 1, hp33120a_MAX_INSTR, -1) != 0)
        return hp33120a_err;
    if (hp33120a_device_closed(instrID) != 0)
        return hp33120a_err;

    Fmt (cmd, "%s<*IDN?");
    if (hp33120a_write_data (instrID, cmd, (short)NumFmtdBytes()) != 0)
        return hp33120a_err;
    if (hp33120a_read_data (instrID, cmd, 150) != 0)
        return hp33120a_err;
    if (Scan (cmd, "%s[i25]>%s", instr_rev) != 1) {
        hp33120a_err = 236;
        return hp33120a_err;
    }

    Fmt (vxipp_rev, "%s<%s", VXIPP_REVISION);
    Fmt (driver_rev, "%s<%s", DRIVER_REVISION);
    Fmt (minfw_rev, "%s<%s", MINFW_REVISION);

    return hp33120a_err;
}

/*=========================================================================*/
/* Function: SCPI Version                                                  */
/* Purpose:  This function queries the instrument to determine the present */
/*           SCPI version.                                                 */
/*=========================================================================*/
short PUBLIC hp33120a_sys_version (short instrID, char version[])
{
    if (hp33120a_invalid_integer_range (instrID, 1, hp33120a_MAX_INSTR, -1) != 0)
        return hp33120a_err;
    if (hp33120a_device_closed(instrID) != 0)
        return hp33120a_err;

    Fmt (cmd, "%s<SYST:VERS?");
    if (hp33120a_write_data (instrID, cmd, (short)NumFmtdBytes()) != 0)
        return hp33120a_err;
    if (hp33120a_read_data (instrID, version, 100) != 0)
        return hp33120a_err;

    return hp33120a_err;
}

/*=========================================================================*/
/* Function: Write To Instrument                                           */
/* Purpose:  This function writes a command string to the instrument.      */
/*=========================================================================*/
short PUBLIC hp33120a_write (short instrID, char cmd_string[])
{
    if (hp33120a_invalid_integer_range (instrID, 1, hp33120a_MAX_INSTR, -1) != 0)
        return hp33120a_err;
    if (hp33120a_device_closed(instrID) != 0)
        return hp33120a_err;

    Fmt (cmd, "%s<%s", cmd_string);
    if (hp33120a_write_data (instrID, cmd, (short)NumFmtdBytes()) != 0)
        return hp33120a_err;

    return hp33120a_err;
}

/*=========================================================================*/
/* Function: Read Instrument Buffer                                        */
/* Purpose:  This function reads the output buffer of the instrument.      */
/*=========================================================================*/
short PUBLIC hp33120a_read (short instrID, short numbytes, char in_buff[], short *bytes_read)
{
    if (hp33120a_invalid_integer_range (instrID, 1, hp33120a_MAX_INSTR, -1) != 0)
        return hp33120a_err;
    if (hp33120a_device_closed(instrID) != 0)
        return hp33120a_err;

    *bytes_read = 0;
    if (hp33120a_read_data (instrID, in_buff, numbytes) != 0)
        return hp33120a_err;

    *bytes_read =(short) ibcnt;

    return hp33120a_err;
}

/*=========================================================================*/
/* Function: Close                                                         */
/* Purpose:  This function closes the instrument.                          */
/*=========================================================================*/
short PUBLIC hp33120a_close (short instrID)
{
    if (hp33120a_invalid_integer_range (instrID, 1, hp33120a_MAX_INSTR, -1) != 0)
        return hp33120a_err;
    if (hp33120a_device_closed (instrID))
        return hp33120a_err;

    hp33120a_close_instr (instrID);

    return hp33120a_err;
}

/*= UTILITY ROUTINES ======================================================*/

/*=========================================================================*/
/* Function: Open Instrument                                               */
/* Purpose:  This function locates and initializes an entry in the         */
/*           Instrument Table and the GPIB device table for the            */
/*           instrument.  The size of the Instrument Table can be changed  */
/*           in the include file by altering the constant                  */
/*           hp33120a_MAX_INSTR.  The return value of this function is equal*/
/*           to the global error variable.                                 */
/*=========================================================================*/
short hp33120a_open_instr (short addr, short *ID)
{
    short i, instrID;

    instrID = 0;
    hp33120a_err = 0;

/* Check to see if the instrument is already in the Instrument Table. */

    for (i = 1; i <= hp33120a_MAX_INSTR; i++)
        if (address[i] == addr) {
            instrID = i;
            i = hp33120a_MAX_INSTR;
         }

/* If it is not in the instrument table, open an entry for the instrument. */

    if (instrID <= 0)
        for (i = 1; i <= hp33120a_MAX_INSTR; i++)
            if (address[i] == 0) {
                instrID = i;
                i = hp33120a_MAX_INSTR;
             }

/* If an entry could not be opened in the Instrument Table, return an error.*/

    if (instrID <= 0) {
        hp33120a_err = 220;
        return hp33120a_err;
    }

/* If the device has not been opened in the GPIB device table (bd[ID] = 0),*/
/*  then open it.                                                          */

    if (bd[instrID] <= 0) {
        if (instr_cnt <= 0)
            CloseInstrDevs("hp33120a");
        bd[instrID] = (short)OpenDev ("", "hp33120a");
        if (bd[instrID] <= 0) {
            hp33120a_err = 220;
            return hp33120a_err;
        }
        instr_cnt += 1;
        address[instrID] = addr;
     }

/*  Change the primary address of the device    */

    if (ibpad (bd[instrID], addr) & 0x8000) {
        hp33120a_err = 233;
        return hp33120a_err;
    }

    *ID = instrID;
    return hp33120a_err;
}

/*=========================================================================*/
/* Function: Close Instrument                                              */
/* Purpose:  This function closes the instrument by removing it from the   */
/*           GPIB device table and setting the address and bd[instrID] to  */
/*           zero in the Instrument Table.  The return value is equal to   */
/*           the global error variable.                                    */
/*=========================================================================*/
short hp33120a_close_instr (short instrID)
{
    if (bd[instrID] != 0) {
        CloseDev (bd[instrID]);
        bd[instrID] = 0;
        address[instrID] = 0;
        instr_cnt -= 1;
    }
    else
        hp33120a_err = 221;

    return hp33120a_err;
}

/*=========================================================================*/
/* Function: Invalid Integer Range                                         */
/* Purpose:  This function checks an integer to see if it lies between a   */
/*           minimum and maximum value.  If the value is out of range, set */
/*           the global error variable to the value err_code.  If the      */
/*           value is OK, error = 0.  The return value is equal to the     */
/*           global error value.                                           */
/*=========================================================================*/
short hp33120a_invalid_integer_range (short val, short min, short max, short err_code)
{
    if (val < min || val > max)
        hp33120a_err = err_code;
    else
        hp33120a_err = 0;

    return hp33120a_err;
}

/*=========================================================================*/
/* Function: Invalid Long Integer Range                                    */
/* Purpose:  This function checks a long integer to see if it lies between */
/*           a minimum and maximum value.  If the value is out of range,   */
/*           set the global error variable to the value err_code.  If the  */
/*           value is OK, error = 0.  The return value is equal to the     */
/*           global error value.                                           */
/*=========================================================================*/
short hp33120a_invalid_longint_range (long val, long min, long max,
                                    short err_code)
{
    if (val < min || val > max)
        hp33120a_err = err_code;
    else
        hp33120a_err = 0;

    return hp33120a_err;
}

/*=========================================================================*/
/* Function: Invalid Real Range                                            */
/* Purpose:  This function checks a real number to see if it lies between  */
/*           a minimum and maximum value.  If the value is out of range,   */
/*           set the global error variable to the value err_code.  If the  */
/*           value is OK, error = 0.  The return value is equal to the     */
/*           global error value.                                           */
/*=========================================================================*/
short hp33120a_invalid_real_range (double val, double min, double max, short err_code)
{
    if (val < min || val > max)
        hp33120a_err = err_code;
    else
        hp33120a_err = 0;

    return hp33120a_err;
}

/*=========================================================================*/
/* Function: Device Closed                                                 */
/* Purpose:  This function checks to see if the module has been            */
/*           initialized.  If the device has not been opened, set the      */
/*           global error variable to 232, 0 otherwise. The return value   */
/*           is equal to the global error value.                           */
/*=========================================================================*/
short hp33120a_device_closed (short instrID)

{
    if (bd[instrID] <= 0)
        hp33120a_err = 232;
    else
        hp33120a_err = 0;

    return hp33120a_err;
}

/*=========================================================================*/
/* Function: Read Data                                                     */
/* Purpose:  This function reads a buffer of data from the instrument. The */
/*           return value is equal to the global error variable.           */
/*=========================================================================*/
short hp33120a_read_data (short instrID, char *buf, short cnt)
{
    if (ibrd(bd[instrID], buf, (long)cnt) & 0x8000)
        hp33120a_err = 231;
    else
        hp33120a_err = 0;

    return hp33120a_err;
}

/*=========================================================================*/
/* Function: Write Data                                                    */
/* Purpose:  This function writes a buffer of data to the instrument. The  */
/*           return value is equal to the global error variable.           */
/*=========================================================================*/
short hp33120a_write_data (short instrID, char *buf, short cnt)
{
    if (ibwrt(bd[instrID], buf, (long)cnt) & 0x8000)
        hp33120a_err = 230;
    else
        hp33120a_err = 0;

    return hp33120a_err;
}

/*=========================================================================*/
/* Function: Write Data File                                               */
/* Purpose:  This function writes a buffer of data from the file specified */
/*           by "filename" to the instrument. Filename must either be a    */
/*           string, such as "C:\\lw\\instr\\file" or a pointer to such a  */
/*           string.  The return value is equal to the global error        */
/*           variable.                                                     */
/*=========================================================================*/
short hp33120a_write_data_file (short instrID, char *filename)
{
    if (ibwrtf (bd[instrID], filename) & 0x8000)
        hp33120a_err = 228;
    else
        hp33120a_err = 0;

    return hp33120a_err;
}

/*=========================================================================*/
/* Function: Set Timeout                                                   */
/* Purpose:  This function changes or disables the timeout of the device.  */
/*           Refer to the LabWindows Standard Libraries Reference Manual   */
/*           for timeout codes. The return value is equal to the global    */
/*           error variable.                                               */
/*=========================================================================*/
short hp33120a_set_timeout (short instrID, short tmo_code, short *old_timeout)
{
    if (ibtmo (bd[instrID], tmo_code) & 0x8000)
        hp33120a_err = 239;
    else {
        *old_timeout = (short)iberr;
        hp33120a_err = 0;
    }
    return hp33120a_err;
}

/*=========================================================================*/
/* Function: Status                                                        */
/* Purpose:  Function reads the Standard Event Register and checks for     */
/*           error.                                                        */
/*=========================================================================*/
short hp33120a_sys_status (short instrID)
{
    short esr;

    if (hp33120a_invalid_integer_range (instrID, 1, hp33120a_MAX_INSTR, -1) != 0)
        return hp33120a_err;
    if (hp33120a_device_closed (instrID))
        return hp33120a_err;

    hp33120a_err = 0;

    /* Read the Standard Event Register */
    Fmt (cmd, "%s<*ESR?");
    if (hp33120a_write_data (instrID, cmd, (short)NumFmtdBytes()) != 0)
        return hp33120a_err;
    if (hp33120a_read_data (instrID, cmd, 150) != 0)
        return hp33120a_err;
    if (Scan (cmd, "%s>%d[b2]", &esr) != 1) {
        hp33120a_err = 236;
        return hp33120a_err;
    }

    /* Check if any error bit was asserted */
    if ((esr & 0x3C) != 0)
        hp33120a_err = 300;

    return hp33120a_err;
}

/*=========================================================================*/
/* Function: Check Shape                                                   */
/* Purpose:  This function queries the instrument for the current          */
/*           waveform shape and determines whether this shape is allowed   */
/*           as a carrier waveform.                                        */
/*=========================================================================*/
short hp33120a_check_shape (short instrID)
{
    Fmt (cmd, "%s<SOUR:FUNC:SHAP?");
    if (hp33120a_write_data (instrID, cmd, (short)NumFmtdBytes()) != 0)
        return hp33120a_err;
    if (hp33120a_read_data (instrID, cmd, 150) != 0)
        return hp33120a_err;

    Scan (cmd, "NOIS");
    if ((short)NumFmtdBytes() == 4)
        hp33120a_err = 100;

    Scan (cmd, "DC");
    if ((short)NumFmtdBytes() == 2)
        hp33120a_err = 100;

    return hp33120a_err;
}

/*=========================================================================*/
/* Function: Setup Arrays                                                  */
/* Purpose:  This function is called by the init routine to initialize     */
/*           static arrays.                                                */
/*=========================================================================*/
void hp33120a_setup_arrays ()
{
    /* Basic Output Function */
    shape_arr[0] = "DC";                    /* DC                    */
    shape_arr[1] = "SIN";                   /* Sine                  */
    shape_arr[2] = "SQU";                   /* Square                */
    shape_arr[3] = "TRI";                   /* Triangle              */
    shape_arr[4] = "RAMP";                  /* Ramp                  */
    shape_arr[5] = "NOIS";                  /* Noise                 */
    shape_arr[6] = "VOLATILE";              /* Wf stored in Volatile */
    shape_arr[7] = "SINC";                  /* Sinc                  */
    shape_arr[8] = "NEG_RAMP";              /* Negative Ramp         */
    shape_arr[9] = "EXP_RISE";              /* Exponential Rise      */
    shape_arr[10] = "EXP_FALL";             /* Exponential Fall      */
    shape_arr[11] = "CARDIAC";              /* Cardiac               */

    /* Waveform Parameters */
    wf_parameter_arr[0] = "FREQ";           /* Frequency             */
    wf_parameter_arr[1] = "VOLT";           /* Amplitude             */
    wf_parameter_arr[2] = "VOLT:OFFS";      /* Offset                */

    /* Output Amplitude Unit */
    unit_arr[0] = "VPP";                    /* Volts Peak-To-Peak    */
    unit_arr[1] = "VRMS";                   /* Volts rms             */
    unit_arr[2] = "DBM";                    /* dBm                   */

    /* Output Impedance */
    out_z_arr[0] = "MIN";                   /* 50 Ohms               */
    out_z_arr[1] = "MAX";                   /* Open Circuit          */

    /* Amplitude Modulation - Modulation Source */
    am_mode_arr[1] = "EXT";                 /* External              */
    am_mode_arr[2] = "BOTH";                /* External and Internal */

    /* FSK Modulation and Burst Modulation - Gate Signal Source */
    sour_arr[1] = "INT";                    /* Internal              */
    sour_arr[2] = "EXT";                    /* External              */

    /* Trigger Source */
    trig_arr[1] = "IMM";                    /* Immediate             */
    trig_arr[2] = "EXT";                    /* External              */
    trig_arr[3] = "BUS";                    /* Bus                   */

    /* Frequency Sweep - Spacing Type */
    fs_spacing_arr[0] = "LIN";              /* Linear                */
    fs_spacing_arr[1] = "LOG";              /* Logarithmic           */
    fs_spacing_arr[2] = "LIN";              /* Linear                */
    fs_spacing_arr[3] = "LOG";              /* Logarithmic           */

    /* Instrument Configuration */
    instr_config_arr[0] = "*SAV";           /* Save                  */
    instr_config_arr[1] = "*RCL";           /* Recall                */
    instr_config_arr[2] = "MEM:STAT:DEL";   /* Delete                */

    /* Arbitrary Waveforms Parameters */
    arb_func_arr[0] = "AVER";               /* Average               */
    arb_func_arr[1] = "CFAC";               /* Crest Factor          */
    arb_func_arr[2] = "PTP";                /* Peak-To-Peak          */
    arb_func_arr[3] = "POIN";               /* Points                */

}

/*=== THE END =============================================================*/
