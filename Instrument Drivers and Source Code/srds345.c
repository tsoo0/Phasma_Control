/***************************************************************************** 
 *  © 1998-2010, National Instruments, Corporation.  All Rights Reserved.    * 
 *****************************************************************************/
/*****************************************************************************
 *  SRS DS345 Function Generator Instrument Driver
 *  LabWindows/CVI 5.0 Instrument Driver
 *  Original Release: 10/23/98
 *  By: Zdenek Rykala, National Instruments
 *      PH. ++ 1-512-683-8459   Fax ++ 1-512-683-8661
 *
 *  Modification History:                                                    
 *
 *       10/23/98 - Instrument Driver Created.                  
 *       03/06/00 - [pp] Made inherent changes and other changes as per CAR 1UH9RHNV.
 *       03/28/00 - [pp] This Driver IS compliant with IVI Class Spec ver 2.0
 *       04/09/01 - [KP] Driver updated to IVI-3.2 specification:
 *                      Added functions:
 *                         srds345_InvalidateAllAttributes
 *                         srds345_ResetWithDefaults
 *                         srds345_Disable
 *                         srds345_GetNextInterchangeWarning
 *                         srds345_ResetInterchangeCheck
 *                         srds345_ClearInterchangeWarning
 *                         srds345_GetError
 *                         srds345_ClearError
 *                      Added attributes:
 *                         SRDS345_ATTR_DRIVER_SETUP
 *                         SRDS345_ATTR_INTERCHANGE_CHECK
 *                      Renamed Attributes
 *                         SRDS345_ATTR_QUERY_INSTR_STATUS to SRDS345_ATTR_QUERY_INSTRUMENT_STATUS
 *                         SRDS345_ATTR_RESOURCE_DESCRIPTOR to SRDS345_ATTR_IO_RESOURCE_DESCRIPTOR
 *                         SRDS345_ATTR_NUM_CHANNELS to SRDS345_ATTR_CHANNEL_COUNT
 *                      Changed:
 *                         srds345_InitWithOptions changed type of optionsString
 *                                                parameter to ViConstString
 *       01.29.02 - [BB] Fixed
 *                          attrAmInternalWaveformRangeTable
 *                          attrFmInternalWaveformRangeTable
 *                          attrPmInternalWaveformRangeTable
 *                              (Added Cmd Value)
 *                          srds345AttrAMFMPMWaveform_WriteCallback
 *                          srds345_CreateArbWaveform
 *                          CAR 2C7GRNBW
 *       03-13-02 - [BB] Removed CVI 6.0 compiler warnings. 
 *       05-28-2003 - [AL] Set attribute IVI_ATTR_IO_RESOURCE_DESCRIPTOR 
 *                        in initialize time.
 *       09/07/10 - [BS] Added portability for 64-bit
 *****************************************************************************/

#include <string.h>
#include <stdio.h>  
#include <formatio.h>
#include <utility.h>
#include <stdlib.h>
#include <math.h>

#include "srds345.h"

/*****************************************************************************
 *--------------------- Hidden Attribute Declarations -----------------------*
 *****************************************************************************/

#define SRDS345_ATTR_AMPLITUDE         (IVI_SPECIFIC_PRIVATE_ATTR_BASE + 104L)      /* ViReal64 */
#define SRDS345_ATTR_OFFSET            (IVI_SPECIFIC_PRIVATE_ATTR_BASE + 105L)      /* ViReal64 */
#define SRDS345_ATTR_WAVEFORM          (IVI_SPECIFIC_PRIVATE_ATTR_BASE + 110L)      /* ViInt32 */
#define SRDS345_ATTR_MOD_TYPE          (IVI_SPECIFIC_PRIVATE_ATTR_BASE + 107L)      /* ViInt32 */
#define SRDS345_ATTR_MOD_STATE         (IVI_SPECIFIC_PRIVATE_ATTR_BASE + 108L)      /* ViBoolean */
#define SRDS345_ATTR_MOD_WAVEFORM      (IVI_SPECIFIC_PRIVATE_ATTR_BASE + 109L)      /* ViInt32 */
#define SRDS345_ATTR_MOD_FREQ          (IVI_SPECIFIC_PRIVATE_ATTR_BASE + 111L)      /* ViReal64 */
#define SRDS345_ATTR_NUM_ARB_POINTS    (IVI_SPECIFIC_PRIVATE_ATTR_BASE + 112L)      /* ViInt32 */
#define SRDS345_ATTR_WFM_EXISTS        (IVI_SPECIFIC_PRIVATE_ATTR_BASE + 113L)      /* ViBoolean */

    /*- Defined values for Hidden attributes --------------------------------*/
    /*- Defined values for attribute SRDS345_ATTR_WAVEFORM ------------*/
#define  SRDS345_VAL_FGEN_SINE            (0L)
#define  SRDS345_VAL_FGEN_SQUARE          (1L)
#define  SRDS345_VAL_FGEN_TRIANGLE        (2L)
#define  SRDS345_VAL_FGEN_RAMP_UP         (3L)
#define  SRDS345_VAL_FGEN_NOISE           (4L)
#define  SRDS345_VAL_FGEN_ARB             (5L)

    /*- Defined values for attribute SRDS345_ATTR_MOD_TYPE -----------------*/
#define SRDS345_VAL_MOD_LIN               (0L)  
#define SRDS345_VAL_MOD_LOG               (1L)  
#define SRDS345_VAL_MOD_AM                (2L)  
#define SRDS345_VAL_MOD_FM                (3L)  
#define SRDS345_VAL_MOD_PM                (4L)  
#define SRDS345_VAL_MOD_BURST             (5L)  

    /*- Defined values for attribute SRDS345_ATTR_MOD_WAVEFORM -------------*/
#define  SRDS345_VAL_MOD_SING             (0L)
#define  SRDS345_VAL_MOD_RAMP_UP          (1L)
#define  SRDS345_VAL_MOD_TRIANGLE         (2L)
#define  SRDS345_VAL_MOD_SINE             (3L)
#define  SRDS345_VAL_MOD_SQUARE           (4L)
#define  SRDS345_VAL_MOD_ARB              (5L)
#define  SRDS345_VAL_MOD_NONE             (6L)

/*****************************************************************************
 *---------------------------- Useful Macros --------------------------------*
 *****************************************************************************/
    /*- I/O buffer size -----------------------------------------------------*/
#define BUFFER_SIZE                                 512L        

    /*- 488.2 Event Status Register (ESR) Bits ------------------------------*/
    /*- tests for query, device, execution, and command errors --------------*/
#define IEEE_488_2_ERROR_MASK                       (0x3C)

    /*- Constants for arbitrary waveofrms  ----------------------------------*/
#define SRDS345_VAL_MIN_WFM_SIZE                    8
#define SRDS345_VAL_MAX_WFM_SIZE                    16300   

#define SRDS345_VAL_MAX_BURST_TIME                  500.0
#define SRDS345_VAL_MAX_FREQ_TRIANGLE               100000.0
#define SRDS345_VAL_MAX_FREQ_SINE                   30.2e6

    /*- Constatnt for modulation arbitrary waveform -------------------------*/
#define SRDS345_VAL_MIN_MOD_SIZE                    8
#define SRDS345_VAL_MAX_AM_MOD_SIZE                 10000
#define SRDS345_VAL_MAX_FM_MOD_SIZE                 1500
#define SRDS345_VAL_MAX_PM_MOD_SIZE                 4000

    /*- List of channels passed to the Ivi_BuildChannelTable function -------*/ 
#define CHAN1                                       "1"

/*****************************************************************************
 *-------------- Utility Function Declarations (Non-Exported) ---------------*
 *****************************************************************************/
static ViStatus srds345_InitAttributes (ViSession vi);

static ViStatus srds345_DefaultInstrSetup (ViSession vi);

static ViStatus srds345_CheckStatus (ViSession vi);

static ViStatus srds345_VerifyFuncMode (ViSession vi);

static ViStatus srds345_VerifyArbMode (ViSession vi);

static ViStatus srds345_CoerceToResolution (ViSession vi, 
                                            ViReal64 value, 
                                            ViReal64 res, 
                                            ViReal64 *coercedValue);
                                                 
static ViStatus srds345_ModEnabled (ViSession vi,
                                    ViInt32 modType,
                                    ViBoolean* enabled);

static ViStatus srds345_VerifyOtherModDisabled (ViSession vi,
                                                ViInt32 modType);

static ViStatus srds345_VerifyContinuous (ViSession vi);

static ViStatus srds345_VerifyNotArbNoise (ViSession vi);

static ViStatus srds345_VerifyNotNoise (ViSession vi);

static ViStatus srds345_VerifyOpMode (ViSession vi, ViInt32 OpMode, ViBoolean enabled); 

static ViStatus srds345_EnableMod (ViSession vi, 
                                   ViInt32 modType, 
                                   ViBoolean enable);

static ViStatus srds345_QueryMod (ViSession vi, 
                                  ViBoolean* modState, 
                                  ViInt32* modType);
                                  
static ViStatus srds345_ReadFloat (ViSession vi,
                                   ViSession io,
                                   ViString cmd,
                                   ViReal64* v);

static ViStatus srds345_SendFloat (ViSession vi,
                                   ViSession io,
                                   ViString cmd,
                                   ViReal64 v);
                                   
static ViStatus srds345_ForceFloatToMax (ViSession vi,
                                         ViConstString channelName,
                                         ViAttr attrId,
                                         ViReal64 max);

static ViStatus srds345_ForceWfmFromNoise (ViSession vi);

static ViStatus srds345_GetMaxFrequency (ViSession vi, ViReal64* max);

static ViStatus srds345_VerifyBurstTime (ViSession vi, 
                                         ViInt32 count, 
                                         ViReal64 cycleTime);

static ViStatus srds345_GetFrequency (ViSession vi, ViReal64* freq);

/*****************************************************************************
 *----------------- Callback Declarations (Non-Exported) --------------------*
 *****************************************************************************/

    /*- Global Session Callbacks --------------------------------------------*/
static ViStatus _VI_FUNC srds345_CheckStatusCallback (ViSession vi, ViSession io);

    /*- Attribute callbacks -------------------------------------------------*/
static ViStatus _VI_FUNC srds345AttrSpecificDriverRevision_ReadCallback  (ViSession vi,
                                                                  ViSession io, 
                                                                  ViConstString channelName,
                                                                  ViAttr attributeId, 
                                                                  const ViConstString cacheValue);

    /*- Shared Callbacks ----------------------------------------------------*/
static ViStatus _VI_FUNC srds345AttrFrequency_CoerceCallback (ViSession vi,
                                                              ViConstString channelName,
                                                              ViAttr attributeId,
                                                              ViReal64 value,
                                                              ViReal64* coercedValue);
                                                              
static ViStatus _VI_FUNC srds345AttrAMFMPMWaveform_WriteCallback (ViSession vi,
                                                                  ViSession io,
                                                                  ViConstString channelName,
                                                                  ViAttr attributeId,
                                                                  ViInt32 value);
static ViStatus _VI_FUNC srds345AttrAMFMPMWaveform_ReadCallback (ViSession vi,
                                                                 ViSession io,
                                                                 ViConstString channelName,
                                                                 ViAttr attributeId,
                                                                 ViInt32  *value);

static ViStatus _VI_FUNC srds345AttrAMFMPMSwpFreq_ReadCallback (ViSession vi,
                                                                ViSession io,
                                                                ViConstString channelName,
                                                                ViAttr attributeId,
                                                                ViReal64 *value);
static ViStatus _VI_FUNC srds345AttrAMFMPMSwpFreq_WriteCallback (ViSession vi,
                                                                 ViSession io,
                                                                 ViConstString channelName,
                                                                 ViAttr attributeId,
                                                                 ViReal64 value);

    /*- Basic Operation -----------------------------------------------------*/
static ViStatus _VI_FUNC srds345AttrOperationMode_WriteCallback (ViSession vi,
                                                                 ViSession io,
                                                                 ViConstString channelName,
                                                                 ViAttr attributeId,
                                                                 ViInt32 value);
static ViStatus _VI_FUNC srds345AttrOperationMode_ReadCallback  (ViSession vi,
                                                                 ViSession io,
                                                                 ViConstString channelName,
                                                                 ViAttr attributeId,
                                                                 ViInt32 *value);
static ViStatus _VI_FUNC srds345AttrOutputMode_WriteCallback (ViSession vi,
                                                              ViSession io,
                                                              ViConstString channelName,
                                                              ViAttr attributeId,
                                                              ViInt32 value);
static ViStatus _VI_FUNC srds345AttrOutputMode_CheckCallback (ViSession vi,
                                                              ViConstString channelName,
                                                              ViAttr attributeId,
                                                              ViInt32 value);

static ViStatus _VI_FUNC srds345AttrOutputEnabled_CheckCallback (ViSession vi,
                                                                 ViConstString channelName,
                                                                 ViAttr attributeId,
                                                                 ViBoolean value);

    /*- Standard Function Output ---------------------------------------------------------*/
static ViStatus _VI_FUNC srds345AttrFuncWaveform_CheckCallback (ViSession vi,
                                                                ViConstString channelName,
                                                                ViAttr attributeId,
                                                                ViInt32 value);
static ViStatus _VI_FUNC srds345AttrFuncWaveform_WriteCallback (ViSession vi,
                                                                ViSession io,
                                                                ViConstString channelName,
                                                                ViAttr attributeId,
                                                                ViInt32 value);
static ViStatus _VI_FUNC srds345AttrFuncWaveform_ReadCallback  (ViSession vi,
                                                                ViSession io,
                                                                ViConstString channelName,
                                                                ViAttr attributeId,
                                                                ViInt32 *value);

static ViStatus _VI_FUNC srds345AttrFuncAmplitude_CheckCallback (ViSession vi,
                                                                 ViConstString channelName,
                                                                 ViAttr attributeId,
                                                                 ViReal64 value);
static ViStatus _VI_FUNC srds345AttrFuncAmplitude_WriteCallback (ViSession vi,
                                                                 ViSession io,
                                                                 ViConstString channelName,
                                                                 ViAttr attributeId,
                                                                 ViReal64 value);
static ViStatus _VI_FUNC srds345AttrFuncAmplitude_ReadCallback  (ViSession vi,
                                                                 ViSession io,
                                                                 ViConstString channelName,
                                                                 ViAttr attributeId,
                                                                 ViReal64 *value);

static ViStatus _VI_FUNC srds345AttrFuncDCOffset_CheckCallback  (ViSession vi,
                                                                 ViConstString channelName,
                                                                 ViAttr attributeId,
                                                                 ViReal64 value);
static ViStatus _VI_FUNC srds345AttrFuncDCOffset_WriteCallback  (ViSession vi,
                                                                 ViSession io,
                                                                 ViConstString channelName,
                                                                 ViAttr attributeId,
                                                                 ViReal64 value);
static ViStatus _VI_FUNC srds345AttrFuncDCOffset_ReadCallback   (ViSession vi,
                                                                 ViSession io,
                                                                 ViConstString channelName,
                                                                 ViAttr attributeId,
                                                                 ViReal64 *value);

static ViStatus _VI_FUNC srds345AttrFuncFrequency_RangeTableCallback (ViSession vi,
                                                                      ViConstString channelName,
                                                                      ViAttr attributeId,
                                                                      IviRangeTablePtr *rangeTablePtr);
static ViStatus _VI_FUNC srds345AttrFuncFrequency_CheckCallback  (ViSession vi,
                                                                  ViConstString channelName,
                                                                  ViAttr attributeId,
                                                                  ViReal64 value);
static ViStatus _VI_FUNC srds345AttrFuncFrequency_CoerceCallback (ViSession vi,
                                                                  ViConstString channelName,
                                                                  ViAttr attributeId,
                                                                  ViReal64 value,
                                                                  ViReal64 *coercedValue);
static ViStatus _VI_FUNC srds345AttrFuncFrequency_WriteCallback  (ViSession vi,
                                                                  ViSession io,
                                                                  ViConstString channelName,
                                                                  ViAttr attributeId,
                                                                  ViReal64 value);
static ViStatus _VI_FUNC srds345AttrFuncFrequency_ReadCallback   (ViSession vi,
                                                                  ViSession io,
                                                                  ViConstString channelName,
                                                                  ViAttr attributeId,
                                                                  ViReal64 *value);

static ViStatus _VI_FUNC srds345AttrFuncStartPhase_CheckCallback  (ViSession vi,
                                                                   ViConstString channelName,
                                                                   ViAttr attributeId,
                                                                   ViReal64 value);
static ViStatus _VI_FUNC srds345AttrFuncStartPhase_CoerceCallback (ViSession vi,
                                                                   ViConstString channelName,
                                                                   ViAttr attributeId,
                                                                   ViReal64 value,
                                                                   ViReal64 *coercedValue);
static ViStatus _VI_FUNC srds345AttrFuncStartPhase_WriteCallback  (ViSession vi,
                                                                   ViSession io,
                                                                   ViConstString channelName,
                                                                   ViAttr attributeId,
                                                                   ViReal64 value);
static ViStatus _VI_FUNC srds345AttrFuncStartPhase_ReadCallback   (ViSession vi,
                                                                   ViSession io,
                                                                   ViConstString channelName,
                                                                   ViAttr attributeId,
                                                                   ViReal64 *value);

/*- Arbitrary Waveform Output ---------------------------------------------------------*/
static ViStatus _VI_FUNC srds345AttrArbWaveformHandle_CheckCallback (ViSession vi,
                                                                     ViConstString channelName,
                                                                     ViAttr attributeId,
                                                                     ViInt32 value);
static ViStatus _VI_FUNC srds345AttrArbWaveformHandle_WriteCallback (ViSession vi,
                                                                     ViSession io,
                                                                     ViConstString channelName,
                                                                     ViAttr attributeId,
                                                                     ViInt32 value);
static ViStatus _VI_FUNC srds345AttrArbWaveformHandle_ReadCallback (ViSession vi,
                                                                    ViSession io,
                                                                    ViConstString channelName,
                                                                    ViAttr attributeId,
                                                                    ViInt32* value);

static ViStatus _VI_FUNC srds345AttrArbGain_CheckCallback  (ViSession vi,
                                                            ViConstString channelName,
                                                            ViAttr attributeId,
                                                            ViReal64 value);
static ViStatus _VI_FUNC srds345AttrArbGain_WriteCallback  (ViSession vi,
                                                            ViSession io,
                                                            ViConstString channelName,
                                                            ViAttr attributeId,
                                                            ViReal64 value);
static ViStatus _VI_FUNC srds345AttrArbGain_ReadCallback   (ViSession vi,
                                                            ViSession io,
                                                            ViConstString channelName,
                                                            ViAttr attributeId,
                                                            ViReal64 *value);

static ViStatus _VI_FUNC srds345AttrArbOffset_CheckCallback  (ViSession vi,
                                                              ViConstString channelName,
                                                              ViAttr attributeId,
                                                              ViReal64 value);
static ViStatus _VI_FUNC srds345AttrArbOffset_ReadCallback   (ViSession vi,
                                                              ViSession io,
                                                              ViConstString channelName,
                                                              ViAttr attributeId,
                                                              ViReal64 *value);

static ViStatus _VI_FUNC srds345AttrArbSampleRate_CheckCallback  (ViSession vi,
                                                                  ViConstString channelName,
                                                                  ViAttr attributeId,
                                                                  ViReal64 value);
static ViStatus _VI_FUNC srds345AttrArbSampleRate_CoerceCallback (ViSession vi,
                                                                  ViConstString channelName,
                                                                  ViAttr attributeId,
                                                                  ViReal64 value,
                                                                  ViReal64 *coercedValue);
static ViStatus _VI_FUNC srds345AttrArbSampleRate_WriteCallback  (ViSession vi,
                                                                  ViSession io,
                                                                  ViConstString channelName,
                                                                  ViAttr attributeId,
                                                                  ViReal64 value);
static ViStatus _VI_FUNC srds345AttrArbSampleRate_ReadCallback   (ViSession vi,
                                                                  ViSession io,
                                                                  ViConstString channelName,
                                                                  ViAttr attributeId,
                                                                  ViReal64 *value);

    /*- Triggering ---------------------------------------------------------------------------------*/
static ViStatus _VI_FUNC srds345AttrTriggerSource_WriteCallback (ViSession vi,
                                                                 ViSession io,
                                                                 ViConstString channelName,
                                                                 ViAttr attributeId,
                                                                 ViInt32 value);
static ViStatus _VI_FUNC srds345AttrTriggerSource_ReadCallback  (ViSession vi,
                                                                 ViSession io,
                                                                 ViConstString channelName,
                                                                 ViAttr attributeId,
                                                                 ViInt32 *value);

static ViStatus _VI_FUNC srds345AttrInternalTriggerRate_WriteCallback  (ViSession vi,
                                                                        ViSession io,
                                                                        ViConstString channelName,
                                                                        ViAttr attributeId,
                                                                        ViReal64 value);
static ViStatus _VI_FUNC srds345AttrInternalTriggerRate_ReadCallback   (ViSession vi,
                                                                        ViSession io,
                                                                        ViConstString channelName,
                                                                        ViAttr attributeId,
                                                                        ViReal64 *value);

static ViStatus _VI_FUNC srds345AttrBurstCount_CheckCallback (ViSession vi,
                                                              ViConstString channelName,
                                                              ViAttr attributeId,
                                                              ViInt32 value);
static ViStatus _VI_FUNC srds345AttrBurstCount_WriteCallback (ViSession vi,
                                                              ViSession io,
                                                              ViConstString channelName,
                                                              ViAttr attributeId,
                                                              ViInt32 value);
static ViStatus _VI_FUNC srds345AttrBurstCount_ReadCallback  (ViSession vi,
                                                              ViSession io,
                                                              ViConstString channelName,
                                                              ViAttr attributeId,
                                                              ViInt32 *value);

    /*- SRDS345_ATTR_ARB_MOD_RATE ---------------------------------------------------------------*/
static ViStatus _VI_FUNC srds345AttrArbModRate_WriteCallback (ViSession vi,
                                                              ViSession io,
                                                              ViConstString channelName,
                                                              ViAttr attributeId,
                                                              ViInt32 value);
static ViStatus _VI_FUNC srds345AttrArbModRate_ReadCallback  (ViSession vi,
                                                              ViSession io,
                                                              ViConstString channelName,
                                                              ViAttr attributeId,
                                                              ViInt32 *value);

    /*- Amplitude Modulation Attributes ------------------------------------------------------*/
static ViStatus _VI_FUNC srds345AttrAMEnabled_CheckCallback (ViSession vi,
                                                             ViConstString channelName,
                                                             ViAttr attributeId,
                                                             ViBoolean value);
static ViStatus _VI_FUNC srds345AttrAMEnabled_WriteCallback (ViSession vi,
                                                             ViSession io,
                                                             ViConstString channelName,
                                                             ViAttr attributeId,
                                                             ViBoolean value);
static ViStatus _VI_FUNC srds345AttrAMEnabled_ReadCallback  (ViSession vi,
                                                             ViSession io,
                                                             ViConstString channelName,
                                                             ViAttr attributeId,
                                                             ViBoolean *value);

static ViStatus _VI_FUNC srds345AttrAMInternalDepth_CoerceCallback (ViSession vi,
                                                                    ViConstString channelName,
                                                                    ViAttr attributeId,
                                                                    ViReal64 value,
                                                                    ViReal64 *coercedValue);
static ViStatus _VI_FUNC srds345AttrAMInternalDepth_WriteCallback  (ViSession vi,
                                                                    ViSession io,
                                                                    ViConstString channelName,
                                                                    ViAttr attributeId,
                                                                    ViReal64 value);
static ViStatus _VI_FUNC srds345AttrAMInternalDepth_ReadCallback   (ViSession vi,
                                                                    ViSession io,
                                                                    ViConstString channelName,
                                                                    ViAttr attributeId,
                                                                    ViReal64 *value);

    /*- Frequency Modulation Attributes ---------------------------------------------------------*/  
static ViStatus _VI_FUNC srds345AttrFMEnabled_CheckCallback (ViSession vi,
                                                             ViConstString channelName,
                                                             ViAttr attributeId,
                                                             ViBoolean value);
static ViStatus _VI_FUNC srds345AttrFMEnabled_WriteCallback (ViSession vi,
                                                             ViSession io,
                                                             ViConstString channelName,
                                                             ViAttr attributeId,
                                                             ViBoolean value);
static ViStatus _VI_FUNC srds345AttrFMEnabled_ReadCallback  (ViSession vi,
                                                             ViSession io,
                                                             ViConstString channelName,
                                                             ViAttr attributeId,
                                                             ViBoolean *value);

static ViStatus _VI_FUNC srds345AttrFMInternalDeviation_CheckCallback  (ViSession vi,
                                                                        ViConstString channelName,
                                                                        ViAttr attributeId,
                                                                        ViReal64 value);
static ViStatus _VI_FUNC srds345AttrFMInternalDeviation_CoerceCallback (ViSession vi,
                                                                        ViConstString channelName,
                                                                        ViAttr attributeId,
                                                                        ViReal64 value,
                                                                        ViReal64 *coercedValue);
static ViStatus _VI_FUNC srds345AttrFMInternalDeviation_WriteCallback  (ViSession vi,
                                                                        ViSession io,
                                                                        ViConstString channelName,
                                                                        ViAttr attributeId,
                                                                        ViReal64 value);
static ViStatus _VI_FUNC srds345AttrFMInternalDeviation_ReadCallback   (ViSession vi,
                                                                        ViSession io,
                                                                        ViConstString channelName,
                                                                        ViAttr attributeId,
                                                                        ViReal64 *value);

    /*- Phase Modulation Attributes -------------------------------------------------------------------*/
static ViStatus _VI_FUNC srds345AttrPmEnabled_CheckCallback (ViSession vi,
                                                             ViConstString channelName,
                                                             ViAttr attributeId,
                                                             ViBoolean value);
static ViStatus _VI_FUNC srds345AttrPmEnabled_WriteCallback (ViSession vi,
                                                             ViSession io,
                                                             ViConstString channelName,
                                                             ViAttr attributeId,
                                                             ViBoolean value);
static ViStatus _VI_FUNC srds345AttrPmEnabled_ReadCallback  (ViSession vi,
                                                             ViSession io,
                                                             ViConstString channelName,
                                                             ViAttr attributeId,
                                                             ViBoolean *value);

static ViStatus _VI_FUNC srds345AttrPmInternalSpan_CoerceCallback (ViSession vi,
                                                                   ViConstString channelName,
                                                                   ViAttr attributeId,
                                                                   ViReal64 value,
                                                                   ViReal64 *coercedValue);
static ViStatus _VI_FUNC srds345AttrPmInternalSpan_WriteCallback  (ViSession vi,
                                                                   ViSession io,
                                                                   ViConstString channelName,
                                                                   ViAttr attributeId,
                                                                   ViReal64 value);
static ViStatus _VI_FUNC srds345AttrPmInternalSpan_ReadCallback   (ViSession vi,
                                                                   ViSession io,
                                                                   ViConstString channelName,
                                                                   ViAttr attributeId,
                                                                   ViReal64 *value);
    
    /*- Sweeping --------------------------------------------------------------------------------------*/
static ViStatus _VI_FUNC srds345AttrSweepDirection_WriteCallback (ViSession vi,
                                                                  ViSession io,
                                                                  ViConstString channelName,
                                                                  ViAttr attributeId,
                                                                  ViInt32 value);
static ViStatus _VI_FUNC srds345AttrSweepDirection_ReadCallback  (ViSession vi,
                                                                  ViSession io,
                                                                  ViConstString channelName,
                                                                  ViAttr attributeId,
                                                                  ViInt32 *value);

static ViStatus _VI_FUNC srds345AttrSweFreq_CheckCallback        (ViSession vi,
                                                                  ViConstString channelName,
                                                                  ViAttr attributeId,
                                                                  ViReal64 value);
static ViStatus _VI_FUNC srds345AttrSweFreq_CoerceCallback       (ViSession vi,
                                                                  ViConstString channelName,
                                                                  ViAttr attributeId,
                                                                  ViReal64 value,
                                                                  ViReal64 *coercedValue);
static ViStatus _VI_FUNC srds345AttrSweepStartFreq_WriteCallback (ViSession vi,
                                                                  ViSession io,
                                                                  ViConstString channelName,
                                                                  ViAttr attributeId,
                                                                  ViReal64 value);
static ViStatus _VI_FUNC srds345AttrSweepStartFreq_ReadCallback  (ViSession vi,
                                                                  ViSession io,
                                                                  ViConstString channelName,
                                                                  ViAttr attributeId,
                                                                  ViReal64 *value);

static ViStatus _VI_FUNC srds345AttrSweepStopFreq_WriteCallback (ViSession vi,
                                                                 ViSession io,
                                                                 ViConstString channelName,
                                                                 ViAttr attributeId,
                                                                 ViReal64 value);
static ViStatus _VI_FUNC srds345AttrSweepStopFreq_ReadCallback  (ViSession vi,
                                                                 ViSession io,
                                                                 ViConstString channelName,
                                                                 ViAttr attributeId,
                                                                 ViReal64 *value);

static ViStatus _VI_FUNC srds345AttrSweepModRate_WriteCallback  (ViSession vi,
                                                                 ViSession io,
                                                                 ViConstString channelName,
                                                                 ViAttr attributeId,
                                                                 ViReal64 value);
static ViStatus _VI_FUNC srds345AttrSweepModRate_ReadCallback   (ViSession vi,
                                                                 ViSession io,
                                                                 ViConstString channelName,
                                                                 ViAttr attributeId,
                                                                 ViReal64 *value);

    /*- Miscellaneous Attributes ----------------------------------------------------------------------*/
static ViStatus _VI_FUNC srds345AttrIdQueryResponse_ReadCallback (ViSession vi,
                                                                  ViSession io, 
                                                                  ViConstString channelName, 
                                                                  ViAttr attributeId, 
                                                                  const ViConstString cacheValue);

    /*- Hidden Attributes -----------------------------------------------------------------------------*/
static ViStatus _VI_FUNC srds345AttrAmplitude_CoerceCallback (ViSession vi,
                                                              ViConstString channelName,
                                                              ViAttr attributeId,
                                                              ViReal64 value,
                                                              ViReal64 *coercedValue);
static ViStatus _VI_FUNC srds345AttrAmplitude_WriteCallback (ViSession vi,
                                                                 ViSession io,
                                                                 ViConstString channelName,
                                                                 ViAttr attributeId,
                                                                 ViReal64 value);
static ViStatus _VI_FUNC srds345AttrAmplitude_ReadCallback  (ViSession vi,
                                                                 ViSession io,
                                                                 ViConstString channelName,
                                                                 ViAttr attributeId,
                                                                 ViReal64 *value);

static ViStatus _VI_FUNC srds345AttrOffset_CoerceCallback (ViSession vi,
                                                           ViConstString channelName,
                                                           ViAttr attributeId,
                                                           ViReal64 value,
                                                           ViReal64 *coercedValue);
static ViStatus _VI_FUNC srds345AttrOffset_CheckCallback (ViSession vi,
                                                          ViConstString channelName,
                                                          ViAttr attributeId,
                                                          ViReal64 value);
static ViStatus _VI_FUNC srds345AttrOffset_WriteCallback (ViSession vi,
                                                              ViSession io,
                                                              ViConstString channelName,
                                                              ViAttr attributeId,
                                                              ViReal64 value);
static ViStatus _VI_FUNC srds345AttrOffset_ReadCallback  (ViSession vi,
                                                              ViSession io,
                                                              ViConstString channelName,
                                                              ViAttr attributeId,
                                                              ViReal64 *value);

static ViStatus _VI_FUNC srds345AttrFunction_CheckCallback (ViSession vi,
                                                                ViConstString channelName,
                                                                ViAttr attributeId,
                                                                ViInt32 value);
static ViStatus _VI_FUNC srds345AttrFunction_WriteCallback (ViSession vi,
                                                                ViSession io,
                                                                ViConstString channelName,
                                                                ViAttr attributeId,
                                                                ViInt32 value);
static ViStatus _VI_FUNC srds345AttrFunction_ReadCallback  (ViSession vi,
                                                                ViSession io,
                                                                ViConstString channelName,
                                                                ViAttr attributeId,
                                                                ViInt32 *value);


static ViStatus _VI_FUNC srds345AttrModState_WriteCallback (ViSession vi,
                                                            ViSession io,
                                                            ViConstString channelName,
                                                            ViAttr attributeId,
                                                            ViBoolean value);
static ViStatus _VI_FUNC srds345AttrModState_ReadCallback  (ViSession vi,
                                                            ViSession io,
                                                            ViConstString channelName,
                                                            ViAttr attributeId,
                                                            ViBoolean *value);

static ViStatus _VI_FUNC srds345AttrModType_CheckCallback (ViSession vi,
                                                           ViConstString channelName,
                                                           ViAttr attributeId,
                                                           ViInt32 value);
static ViStatus _VI_FUNC srds345AttrModType_WriteCallback (ViSession vi,
                                                           ViSession io,
                                                           ViConstString channelName,
                                                           ViAttr attributeId,
                                                           ViInt32 value);
static ViStatus _VI_FUNC srds345AttrModType_ReadCallback  (ViSession vi,
                                                           ViSession io,
                                                           ViConstString channelName,
                                                           ViAttr attributeId,
                                                           ViInt32 *value);

static ViStatus _VI_FUNC srds345AttrModWaveform_WriteCallback (ViSession vi,
                                                               ViSession io,
                                                               ViConstString channelName,
                                                               ViAttr attributeId,
                                                               ViInt32 value);
static ViStatus _VI_FUNC srds345AttrModWaveform_ReadCallback  (ViSession vi,
                                                               ViSession io,
                                                               ViConstString channelName,
                                                               ViAttr attributeId,
                                                               ViInt32 *value);

static ViStatus _VI_FUNC srds345AttrModFreq_WriteCallback (ViSession vi,
                                                           ViSession io,
                                                           ViConstString channelName,
                                                           ViAttr attributeId,
                                                           ViReal64 value);
static ViStatus _VI_FUNC srds345AttrModFreq_ReadCallback  (ViSession vi,
                                                           ViSession io,
                                                           ViConstString channelName,
                                                           ViAttr attributeId,
                                                           ViReal64 *value);

/*****************************************************************************
 *------------ User-Callable Functions (Exportable Functions) ---------------*
 *****************************************************************************/

/*****************************************************************************
 * Function: srds345_init   
 * Purpose:  VXIplug&play required function.  Calls the   
 *           srds345_InitWithOptions function.   
 *****************************************************************************/
ViStatus _VI_FUNC srds345_init (ViRsrc resourceName, ViBoolean IDQuery,
                                 ViBoolean resetDevice, ViSession *newVi)
{   
    ViStatus    error = VI_SUCCESS;

    if (newVi == VI_NULL)
        {
        Ivi_SetErrorInfo (VI_NULL, VI_FALSE, IVI_ERROR_INVALID_PARAMETER, 
                          VI_ERROR_PARAMETER4, "Null address for Instrument Handle");
        checkErr( IVI_ERROR_INVALID_PARAMETER);
        }

    checkErr( srds345_InitWithOptions (resourceName, IDQuery, resetDevice, 
                                        "", newVi));

Error:  
    return error;
}

/*****************************************************************************
 * Function: srds345_InitWithOptions                                       
 * Purpose:  This function creates a new IVI session and calls the 
 *           IviInit function.                                     
 *****************************************************************************/
ViStatus _VI_FUNC srds345_InitWithOptions (ViRsrc resourceName, ViBoolean IDQuery,
                                           ViBoolean resetDevice, ViConstString optionString, 
                                           ViSession *newVi)
{   
    ViStatus    error = VI_SUCCESS;
    ViSession   vi = VI_NULL;
    ViChar      newResourceName[IVI_MAX_MESSAGE_BUF_SIZE];
    ViChar      newOptionString[IVI_MAX_MESSAGE_BUF_SIZE];
    ViBoolean   isLogicalName;
    
    if (newVi == VI_NULL)
        {
        Ivi_SetErrorInfo (VI_NULL, VI_FALSE, IVI_ERROR_INVALID_PARAMETER, 
                          VI_ERROR_PARAMETER5, "Null address for Instrument Handle");
        checkErr( IVI_ERROR_INVALID_PARAMETER);
        }

    *newVi = VI_NULL;
    checkErr( Ivi_GetInfoFromResourceName (resourceName, (ViString) optionString, newResourceName,
                                           newOptionString, &isLogicalName));
    
        /* create a new interchangeable driver */
    checkErr( Ivi_SpecificDriverNew ("srds345", newOptionString, &vi));  
    if (!isLogicalName)
    {
        ViInt32 oldFlag = 0;
        
        checkErr (Ivi_GetAttributeFlags (vi, IVI_ATTR_IO_RESOURCE_DESCRIPTOR, &oldFlag));
        checkErr (Ivi_SetAttributeFlags (vi, IVI_ATTR_IO_RESOURCE_DESCRIPTOR, oldFlag & 0xfffb | 0x0010));
        checkErr (Ivi_SetAttributeViString (vi, "", IVI_ATTR_IO_RESOURCE_DESCRIPTOR, 0, newResourceName));
    }
        /* init the driver */
    checkErr( srds345_IviInit (newResourceName, IDQuery, resetDevice, vi)); 
    if (isLogicalName)
        checkErr( Ivi_ApplyDefaultSetup (vi));
    *newVi = vi;
    
Error:
    if (error < VI_SUCCESS) 
        Ivi_Dispose (vi);
        
    return error;
}

/*****************************************************************************
 * Function: srds345_IviInit                                                       
 * Purpose:  This function is called by srds345_InitWithOptions  
 *           or by an IVI class driver.  This function initializes the I/O 
 *           interface, optionally resets the device, optionally performs an
 *           ID query, and sends a default setup to the instrument.                
 *****************************************************************************/
ViStatus _VI_FUNC srds345_IviInit (ViRsrc resourceName, ViBoolean IDQuery,
                                   ViBoolean reset, ViSession vi)
{
    ViStatus    error = VI_SUCCESS;
    ViSession   io = VI_NULL;
    
    checkErr( Ivi_BuildChannelTable (vi, CHAN1, VI_FALSE, VI_NULL));
    
     /* Add attributes */
    checkErr( srds345_InitAttributes (vi));

    if (!Ivi_Simulating(vi))
        {
        ViSession   rmSession = VI_NULL;

        /* Open instrument session */
        checkErr( Ivi_GetAttributeViSession (vi, VI_NULL, IVI_ATTR_VISA_RM_SESSION, 0,
                                             &rmSession)); 
        viCheckErr( viOpen (rmSession, resourceName, VI_NULL, VI_NULL, &io));
        /* io session owned by driver now */
        checkErr( Ivi_SetAttributeViSession (vi, VI_NULL, IVI_ATTR_IO_SESSION, 0, io));  

        /* Configure VISA Formatted I/O */
        viCheckErr( viSetAttribute (io, VI_ATTR_TMO_VALUE, 5000 ));
        viCheckErr( viSetBuf       (io, VI_READ_BUF | VI_WRITE_BUF, 4000));
        viCheckErr( viSetAttribute (io, VI_ATTR_WR_BUF_OPER_MODE, VI_FLUSH_ON_ACCESS));
        viCheckErr( viSetAttribute (io, VI_ATTR_RD_BUF_OPER_MODE, VI_FLUSH_ON_ACCESS));
        }
        
    /*- Reset instrument ----------------------------------------------------*/
    if (reset) 
        checkErr( srds345_reset (vi));
    else  /*- Send Default Instrument Setup ---------------------------------*/
        checkErr( srds345_DefaultInstrSetup (vi));
    
    /*- Identification Query ------------------------------------------------*/
    if (IDQuery)                               
        {
        ViChar rdBuffer[BUFFER_SIZE];
    
        #define VALID_RESPONSE_STRING_START        "StanfordResearchSystems,DS345"
    
        checkErr( Ivi_GetAttributeViString (vi, VI_NULL, SRDS345_ATTR_ID_QUERY_RESPONSE, 
                                            0, BUFFER_SIZE, rdBuffer));
        
        if (strncmp (rdBuffer, VALID_RESPONSE_STRING_START, strlen(VALID_RESPONSE_STRING_START)) != 0)
            viCheckErr( VI_ERROR_FAIL_ID_QUERY);
        }

    checkErr( srds345_CheckStatus (vi));
Error:
    if (error < VI_SUCCESS)
        {
        if (!Ivi_Simulating (vi) && io)
            viClose (io);
        }
    return error;
}

/*****************************************************************************
 * Function: srds345_close                                                           
 * Purpose:  This function closes the instrument.                            
 *
 *           Note:  This function must unlock the session before calling
 *           Ivi_Dispose.
 *****************************************************************************/
ViStatus _VI_FUNC srds345_close (ViSession vi)
{
    ViStatus    error = VI_SUCCESS;
    
    checkErr( Ivi_LockSession (vi, VI_NULL));
    
    checkErr( srds345_IviClose (vi));

Error:    
    Ivi_UnlockSession (vi, VI_NULL);
    Ivi_Dispose (vi);  

    return error;
}

/*****************************************************************************
 * Function: srds345_IviClose                                                        
 * Purpose:  This function performs all of the drivers clean-up operations   
 *           except for closing the IVI session.  This function is called by 
 *           srds345_close or by an IVI class driver. 
 *
 *           Note:  This function must close the I/O session and set 
 *           IVI_ATTR_IO_SESSION to 0.
 *****************************************************************************/
ViStatus _VI_FUNC srds345_IviClose (ViSession vi)
{
    ViStatus error = VI_SUCCESS;
    ViSession io = VI_NULL;

        /* Do not lock here.  The caller manages the lock. */

    checkErr( Ivi_GetAttributeViSession (vi, VI_NULL, IVI_ATTR_IO_SESSION, 0, &io));

Error:
    Ivi_SetAttributeViSession (vi, VI_NULL, IVI_ATTR_IO_SESSION, 0, VI_NULL);
    if(io)                                                      
        {
        viClose (io);
        }
    return error;   
}

/*****************************************************************************
 * Function: srds345_ConfigureOperationMode
 * Purpose:  This function configures the operation mode of the function
 *           generator.  The operation mode determines how the function
 *           generator produces waveforms.  For example, you can choose to
 *           continuously generate waveform, or to generate a discrete
 *           burst of waveform cycles based on a trigger event.
 *****************************************************************************/
ViStatus _VI_FUNC srds345_ConfigureOperationMode (ViSession vi,
                                                  ViConstString channelName,
                                                  ViInt32 operationMode)
{
    return Ivi_SetAttributeViInt32 (vi, channelName, SRDS345_ATTR_OPERATION_MODE,
                                    IVI_VAL_DIRECT_USER_CALL, operationMode);
}

/*****************************************************************************
 * Function: srds345_ConfigureOutputMode
 * Purpose:  This function configures the output mode of the function
 *           generator.  The output mode determines how the function
 *           generator produces waveforms.  For example, you can choose to
 *           output a standard waveform, an arbitrary waveform, or a sequence
 *           of arbitrary waveforms.
 *****************************************************************************/
ViStatus _VI_FUNC srds345_ConfigureOutputMode (ViSession vi, ViInt32 outputMode)
{
    return Ivi_SetAttributeViInt32 (vi, VI_NULL, SRDS345_ATTR_OUTPUT_MODE,
                                    IVI_VAL_DIRECT_USER_CALL, outputMode);
}

/*****************************************************************************
 * Function: srds345_ConfigureRefClockSource
 * Purpose:  This function configures the function generator's reference
 *           clock source.  The function generator uses the reference clock
 *           to derive frequencies and sample rates for signal generation.
 *****************************************************************************/
ViStatus _VI_FUNC srds345_ConfigureRefClockSource (ViSession vi,
                                                   ViInt32 refClockSource)
{
    return Ivi_SetAttributeViInt32 (vi, VI_NULL,SRDS345_ATTR_REF_CLOCK_SOURCE,
                                    IVI_VAL_DIRECT_USER_CALL, refClockSource);
}

/*****************************************************************************
 * Function: srds345_ConfigureOutputImpedance
 * Purpose:  This function configures the output impedance for the channel
 *           you specify.
 *****************************************************************************/
ViStatus _VI_FUNC srds345_ConfigureOutputImpedance (ViSession vi, ViConstString channelName,
                                                    ViReal64 outputImpedance)
{
    return Ivi_SetAttributeViReal64 (vi, channelName, SRDS345_ATTR_OUTPUT_IMPEDANCE,
                                     IVI_VAL_DIRECT_USER_CALL, outputImpedance);
}
                                            
/*****************************************************************************
 * Function: srds345_ConfigureOutputEnabled
 * Purpose:  This function configures whether the signal the function 
 *           generator produces appears at the channel's output connector.  
 *****************************************************************************/
ViStatus _VI_FUNC srds345_ConfigureOutputEnabled (ViSession vi,
                                                  ViConstString channelName,
                                                  ViBoolean enabled)
{
    return Ivi_SetAttributeViBoolean (vi, channelName, SRDS345_ATTR_OUTPUT_ENABLED,
                                      IVI_VAL_DIRECT_USER_CALL, enabled);
}

/*****************************************************************************
 * Function: srds345_ConfigureStandardWaveform
 * Purpose:  This function configures the attributes of the function
 *           generator that affect standard waveform generation.  These
 *           attributes are the waveform, amplitude, DC offset, frequency,
 *           and start phase.
 *****************************************************************************/
ViStatus _VI_FUNC srds345_ConfigureStandardWaveform (ViSession vi,
                                                     ViConstString channelName,
                                                     ViInt32 waveform,
                                                     ViReal64 amplitude,
                                                     ViReal64 dcOffset,
                                                     ViReal64 frequency,
                                                     ViReal64 startPhase)
{
    ViStatus  error = VI_SUCCESS;
    
    checkErr( Ivi_LockSession (vi, VI_NULL));

        /* Set attributes: */
    viCheckParm( Ivi_SetAttributeViInt32  (vi, channelName, SRDS345_ATTR_FUNC_WAVEFORM, 
                                           0, waveform), 3, "Standard Waveform");
    viCheckParm( Ivi_SetAttributeViReal64 (vi, channelName, SRDS345_ATTR_FUNC_AMPLITUDE, 
                                           0, amplitude), 4, "Amplitude");
    viCheckParm( Ivi_SetAttributeViReal64 (vi, channelName, SRDS345_ATTR_FUNC_DC_OFFSET, 
                                           0, dcOffset), 5, "DC Offset");
   
    if (waveform != SRDS345_VAL_WFM_NOISE)
        {
        ViBoolean state;
        ViInt32 type;
        
        checkErr( srds345_QueryMod (vi, &state, &type));
        if (!state || type == SRDS345_VAL_MOD_AM || type == SRDS345_VAL_MOD_BURST)
            {
            viCheckParm( Ivi_SetAttributeViReal64 (vi, channelName, 
                                                   SRDS345_ATTR_FUNC_START_PHASE, 
                                                   0, startPhase), 7, "Start Phase");
            }
        viCheckParm( Ivi_SetAttributeViReal64 (vi, channelName, SRDS345_ATTR_FUNC_FREQUENCY, 
                                               0, frequency), 6, "Frequency");
        }
    checkErr( srds345_CheckStatus (vi));
Error:
    Ivi_UnlockSession (vi, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: srds345_GenerateStandardWaveform
 * Purpose:  This application function configures the attributes of the 
 *           function generator that affect standard waveform generation,
 *           enables an output channel, and initiates signal generation.  This
 *           behavior is identical to calling the
 *           srds345_ConfigureArbWaveform,
 *           srds345_ConfigureOutputEnbled, and
 *           srds345_InitiateGeneration functions.
 *****************************************************************************/
ViStatus _VI_FUNC  srds345_GenerateStandardWaveform (ViSession vi,
                                                     ViConstString channelName,
                                                     ViInt32 waveform,
                                                     ViReal64 amplitude,
                                                     ViReal64 dcOffset,
                                                     ViReal64 frequency,
                                                     ViReal64 startPhase)
{
    ViStatus error = VI_SUCCESS;
    
    checkErr( Ivi_LockSession (vi, VI_NULL));

        /* Call functions */
    checkErr( srds345_ConfigureOperationMode (vi, channelName,
                                              SRDS345_VAL_OPERATE_CONTINUOUS));
    checkErr( srds345_ConfigureOutputMode (vi, SRDS345_VAL_OUTPUT_FUNC));                                              
    checkErr( srds345_ConfigureStandardWaveform (vi, channelName, waveform, amplitude, 
                                                 dcOffset, frequency, startPhase));
Error:
    Ivi_UnlockSession (vi, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: srds345_ConfigureSampleRate
 * Purpose:  This function configures the sample rate attribute, which
 *           determines the rate at which the function generator produces
 *           arbitrary waveforms.
 *****************************************************************************/
ViStatus _VI_FUNC srds345_ConfigureSampleRate (ViSession vi, ViReal64 sampleRate)
{
    return Ivi_SetAttributeViReal64 (vi, VI_NULL, SRDS345_ATTR_ARB_SAMPLE_RATE,
                                     IVI_VAL_DIRECT_USER_CALL, sampleRate);
}

/*****************************************************************************
 * Function: srds345_QueryArbWfmCapabilities
 * Purpose:  This function returns the attributes of the function generator
 *           that are related to creating arbitrary waveforms.  These
 *           attributes are the maximum number of waveforms, waveform
 *           quantum, minimum waveform size, and maximum waveform size.
 *****************************************************************************/
ViStatus _VI_FUNC  srds345_QueryArbWfmCapabilities (ViSession vi,
                                                    ViInt32 *maxNumWfms,
                                                    ViInt32 *wfmQuantum,
                                                    ViInt32 *minWfmSize,
                                                    ViInt32 *maxWfmSize) 
{
    ViStatus error = VI_SUCCESS;
    
    checkErr( Ivi_LockSession (vi, VI_NULL));
    
    if (maxNumWfms) *maxNumWfms = 1;
    if (wfmQuantum) *wfmQuantum = 1;
    if (minWfmSize) *minWfmSize = SRDS345_VAL_MIN_WFM_SIZE;
    if (maxWfmSize) *maxWfmSize = SRDS345_VAL_MAX_WFM_SIZE;

Error:
    Ivi_UnlockSession (vi, VI_NULL);
    return error;
}


/*****************************************************************************
 * Function: srds345_CreateArbWaveform
 * Purpose:  This function creates an arbitrary waveform and returns a handle
 *           that identifies that waveform.  You use the handles this function
 *           returns to specify what waveform to generate with the
 *           srds345_ConfigureArbWaveform function.
 *****************************************************************************/
ViStatus _VI_FUNC srds345_CreateArbWaveform (ViSession vi, ViInt32 wfmSize,
                                             ViReal64 wfmData[],
                                             ViInt32 *wfmHandle)
{
    ViStatus    error = VI_SUCCESS;
    ViInt16     *dataBuffer = VI_NULL;
    ViBoolean   exists;
    
    checkErr( Ivi_LockSession (vi, VI_NULL));
    
    /* Check that data array and handle pointer are not null. */
    if (!wfmData)
        viCheckParm( IVI_ERROR_INVALID_PARAMETER,
                     3, "NULL address for wfmData");
    if (!wfmHandle)
        viCheckParm ( IVI_ERROR_INVALID_PARAMETER,
                      4, "NULL address for wfmHandle");
                      
    if ((wfmSize > SRDS345_VAL_MAX_WFM_SIZE) || (wfmSize < SRDS345_VAL_MIN_WFM_SIZE ))
        viCheckErr( SRDS345_ERROR_INVALID_WFM_LENGTH);

    checkErr( srds345_VerifyArbMode  (vi));
    checkErr( Ivi_GetAttributeViBoolean (vi, VI_NULL, SRDS345_ATTR_WFM_EXISTS, 0, &exists));
    
    if (exists)
        viCheckErrElab(SRDS345_ERROR_NO_WFMS_AVAILABLE, "No waveforms available for creation.");
        
    checkErr( Ivi_SetAttributeViInt32( vi, VI_NULL, SRDS345_ATTR_NUM_ARB_POINTS, 0, wfmSize));

    if (!Ivi_Simulating(vi))                /* call only when locked */
    {
        ViSession   io = Ivi_IOSession(vi); /* call only when locked */
        ViInt32     dataIndex;
        ViInt16    mathValue, tmpValue = 0;
        ViUInt16    *checksum;
        
        /* Build buffer of data points in array wfmData */
        checkErr( Ivi_Alloc (vi, (((ViInt32)sizeof(ViInt16)*(wfmSize + 1))), (ViAddr *)&dataBuffer));
        
        /* Fill buffer with array values */
        checksum = &dataBuffer[wfmSize];
        *checksum = 0;
        for (dataIndex = 0; dataIndex < wfmSize; dataIndex++)
            {
            /* each value must be in the range -2047 to +2047 */
            mathValue = RoundRealToNearestInteger(wfmData[dataIndex] * 2047.0);
            if (abs(mathValue) > 2047)
                {
                ViChar tmp[512];
                Fmt(tmp, "The data element at index #%ld in the waveform array is"
                                " not normalized.", dataIndex);
                viCheckErrElab(SRDS345_ERROR_INVALID_WFM_ELEMENT,tmp);
                }
            dataBuffer[dataIndex] = mathValue; 
            *checksum += mathValue;
            }

        /* check if the instrument is ready to receive waveform data */
        checkErr( Ivi_SetNeedToCheckStatus (vi, VI_TRUE));
        viCheckErr( viQueryf (io, "LDWF?0,%ld", "%ld", wfmSize, &tmpValue));
        if (tmpValue == 1)
            viCheckErr(viPrintf(io, "%*!olhy", wfmSize + 1, dataBuffer));
    }

    checkErr( srds345_CheckStatus (vi));
    checkErr( Ivi_SetAttributeViBoolean (vi, VI_NULL, SRDS345_ATTR_WFM_EXISTS, 0, VI_TRUE));

Error:
    /* if there is an error, do not return a handle. */
    if (dataBuffer)
        Ivi_Free (vi, dataBuffer);
        
    if (error < VI_SUCCESS)
        {
        if (wfmHandle)
            *wfmHandle = VI_NULL;
        }
    else
        *wfmHandle = SRDS345_VAL_ARB_WAVEFORM;

    Ivi_UnlockSession (vi, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: srds345_ConfigureArbWaveform
 * Purpose:  This function configures the attributes of the function
 *           generator that affect arbitrary waveform generation.  These
 *           attributes are the arbitrary waveform handle, gain, and offset.
 *****************************************************************************/
ViStatus _VI_FUNC  srds345_ConfigureArbWaveform (ViSession vi,
                                                 ViConstString channelName,
                                                 ViInt32 wfmHandle,
                                                 ViReal64 arbGain,
                                                 ViReal64 arbOffset)
{
    ViStatus error = VI_SUCCESS;
    
    checkErr( Ivi_LockSession (vi, VI_NULL));

        /* Set attributes */
    viCheckParm( Ivi_SetAttributeViInt32 (vi, channelName, SRDS345_ATTR_ARB_WAVEFORM_HANDLE,
                                          0, wfmHandle), 3, "Waveform Handle");
    viCheckParm( Ivi_SetAttributeViReal64 (vi, channelName, SRDS345_ATTR_ARB_GAIN,
                                           0, arbGain), 4, "Arbitrary Gain");
    viCheckParm( Ivi_SetAttributeViReal64 (vi, channelName, SRDS345_ATTR_ARB_OFFSET,
                                           0, arbOffset), 5, "Arbitrary Offset");
    checkErr( srds345_CheckStatus (vi));
Error:
    Ivi_UnlockSession (vi, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: srds345_ClearArbWaveform
 * Purpose:  This function removes a previously created arbitrary waveform
 *           from the function generator's memory and invalidates the
 *           waveform's handle.
 *****************************************************************************/
ViStatus _VI_FUNC  srds345_ClearArbWaveform (ViSession vi, ViInt32 wfmHandle)
{
    ViStatus  error = VI_SUCCESS;
    ViBoolean wfmExists;

    checkErr( Ivi_LockSession (vi, VI_NULL));
    
    checkErr ( srds345_VerifyArbMode (vi));
    
    if (wfmHandle != SRDS345_VAL_ALL_WAVEFORMS)
        {
        checkErr ( Ivi_GetAttributeViBoolean (vi, VI_NULL, SRDS345_ATTR_WFM_EXISTS,0, &wfmExists));
        if (!wfmExists)
            viCheckErr(SRDS345_ERROR_INVALID_WAVEFORM);
        
        checkErr ( Ivi_SetAttributeViInt32   (vi, CHAN1, SRDS345_ATTR_ARB_WAVEFORM_HANDLE, 0, wfmHandle));
        }
    checkErr ( Ivi_SetAttributeViBoolean (vi, VI_NULL, SRDS345_ATTR_WFM_EXISTS, 0, VI_FALSE));
    checkErr ( Ivi_SetAttributeViInt32   (vi, VI_NULL, SRDS345_ATTR_NUM_ARB_POINTS, 0, 0));

Error:
    Ivi_UnlockSession (vi, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: srds345_GenerateArbWaveform
 * Purpose:  This function configures the attributes of the function generator
 *           that affect arbitrary waveform generation, enables an output
 *           channel, and initiates signal generation.  This
 *           behavior is identical to calling the
 *           srds345_ConfigureArbWaveform,
 *           srds345_ConfigureOutputEnabled, and
 *           srds345_InitiateGeneration functions.
 *****************************************************************************/
ViStatus _VI_FUNC srds345_GenerateArbWaveform (ViSession vi,
                                               ViConstString channelName,
                                               ViInt32 wfmHandle,
                                               ViReal64 arbGain,
                                               ViReal64 arbOffset)
{
    ViStatus    error = VI_SUCCESS;
    ViBoolean   outputActive = VI_FALSE;
    
    checkErr( Ivi_LockSession (vi, VI_NULL));
    
        /* Call functions */
    checkErr( srds345_ConfigureOperationMode (vi, channelName,
                                              SRDS345_VAL_OPERATE_CONTINUOUS));
    checkErr( srds345_ConfigureOutputMode (vi, SRDS345_VAL_OUTPUT_ARB));
    checkErr( srds345_ConfigureArbWaveform (vi, channelName, wfmHandle, arbGain, arbOffset));
Error:
    Ivi_UnlockSession (vi, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: srds345_ConfigureTriggerSource
 * Purpose:  This function configures the trigger source.
 *****************************************************************************/
ViStatus _VI_FUNC  srds345_ConfigureTriggerSource (ViSession vi,
                                                   ViConstString channelName,
                                                   ViInt32 source)
{
    return Ivi_SetAttributeViInt32 (vi, channelName, SRDS345_ATTR_TRIGGER_SOURCE,
                                    IVI_VAL_DIRECT_USER_CALL, source);
}                                       

/*****************************************************************************
 * Function: srds345_ConfigureInternalTriggerRate
 * Purpose:  This function configures the internal trigger rate.
 *****************************************************************************/
ViStatus _VI_FUNC srds345_ConfigureInternalTriggerRate (ViSession vi,
                                                        ViReal64 rate)
{
    return Ivi_SetAttributeViReal64 (vi, VI_NULL, SRDS345_ATTR_INTERNAL_TRIGGER_RATE,
                                     IVI_VAL_DIRECT_USER_CALL, rate);
}                                       

/*****************************************************************************
 * Function: srds345_ConfigureBurstCount
 * Purpose:  This function configures the burst count.  When you set the
 *           operation mode to burst, the burst count determines how many
 *           waveform cycles the function generator produces after each trigger
 *           event.
 *****************************************************************************/
ViStatus _VI_FUNC srds345_ConfigureBurstCount (ViSession vi,
                                               ViConstString channelName,
                                               ViInt32 count)
{
    return Ivi_SetAttributeViInt32 (vi, channelName, SRDS345_ATTR_BURST_COUNT,
                                    IVI_VAL_DIRECT_USER_CALL, count);
}                                       

/*****************************************************************************
 * Function: srds345_ConfigureAMEnabled
 * Purpose:  This function configures whether the function generator applies
 *           amplitude modulation to a channel.
 *****************************************************************************/
ViStatus _VI_FUNC srds345_ConfigureAMEnabled (ViSession vi,
                                              ViConstString channelName,
                                              ViBoolean enabled)
{
    return Ivi_SetAttributeViBoolean (vi, channelName, SRDS345_ATTR_AM_ENABLED,
                                      IVI_VAL_DIRECT_USER_CALL, enabled);
}
 
/*****************************************************************************
 * Function: srds345_ConfigureAMSource
 * Purpose:  This function specifies the source of the modulating signal the 
 *           function generator uses for the channel when amplitude modulation
 *           is enabled.
 *****************************************************************************/
ViStatus _VI_FUNC srds345_ConfigureAMSource (ViSession vi, 
                                             ViConstString channelName,
                                             ViInt32 source)
{
    return Ivi_SetAttributeViInt32 (vi, channelName, SRDS345_ATTR_AM_SOURCE,
                                    IVI_VAL_DIRECT_USER_CALL, source);
}
                                           
/*****************************************************************************
 * Function: srds345_ConfigureAMInternal
 * Purpose:  This function configures the attributes that control the
 *           function generator's internal amplitude modulation source.
 *           These attributes are the modulation depth, waveform, and
 *           frequency.
 *****************************************************************************/
ViStatus _VI_FUNC srds345_ConfigureAMInternal (ViSession vi, ViReal64 amDepth,
                                               ViInt32 amWaveform, ViReal64 amFrequency)
{
    ViStatus error = VI_SUCCESS;
    
    checkErr( Ivi_LockSession (vi, VI_NULL));

        /* Set attributes */
    viCheckParm( Ivi_SetAttributeViInt32  (vi, VI_NULL, SRDS345_ATTR_AM_INTERNAL_WAVEFORM, 
                                           0, amWaveform), 3, "AM Waveform");
    viCheckParm( Ivi_SetAttributeViReal64 (vi, VI_NULL, SRDS345_ATTR_AM_INTERNAL_FREQUENCY, 
                                           0, amFrequency), 4, "AM Frequency");
    viCheckParm( Ivi_SetAttributeViReal64 (vi, VI_NULL, SRDS345_ATTR_AM_INTERNAL_DEPTH, 
                                           0, amDepth), 2, "AM Depth");
    checkErr( srds345_CheckStatus (vi));
Error:
    Ivi_UnlockSession (vi, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: srds345_ConfigureAMModArbWaveform
 * Purpose:  This function creates an AM modulation arbitrary waveform and
 *           enable AM modulation.
 *****************************************************************************/
ViStatus _VI_FUNC srds345_ConfigureAMModArbWaveform (ViSession vi,
                                                     ViConstString channelName,
                                                     ViInt32 wfmSize,
                                                     ViReal64 wfmData[],
                                                     ViInt32 rateDivider)
{
    ViStatus  error = VI_SUCCESS;
    ViInt16   *dataBuffer = VI_NULL;

    checkErr( Ivi_LockSession (vi, VI_NULL));

    if ((wfmSize > SRDS345_VAL_MAX_AM_MOD_SIZE) && (wfmSize < SRDS345_VAL_MIN_MOD_SIZE))
        viCheckParm( SRDS345_ERROR_INVALID_WFM_LENGTH, 2, "Waveform Size");

    if (!wfmData)
        viCheckParm ( IVI_ERROR_INVALID_PARAMETER,3, "NULL address for Waveform Data");

    checkErr( srds345_VerifyOtherModDisabled (vi, SRDS345_VAL_MOD_AM));
    
    checkErr( Ivi_SetAttributeViInt32 (vi, VI_NULL, SRDS345_ATTR_MOD_TYPE,
                                       0, SRDS345_VAL_MOD_AM));
    checkErr( Ivi_SetAttributeViInt32 (vi, VI_NULL, SRDS345_ATTR_MOD_WAVEFORM,
                                       0, SRDS345_VAL_MOD_ARB));

    if (!Ivi_Simulating(vi))                /* call only when locked */
        {
        ViSession   io = Ivi_IOSession(vi); /* call only when locked */
        ViInt32     dataIndex;
        ViInt16     *checksum, mathValue, tmpValue = 0;
                
        /* Build buffer of data points in array wfmData */
        checkErr( Ivi_Alloc (vi, ((ViInt32)sizeof(ViInt16)*(wfmSize+1)), (ViAddr *)&dataBuffer));

        /* Fill buffer with array values */
        checksum = &dataBuffer[wfmSize];
        *checksum =0;
        for (dataIndex = 0; dataIndex < wfmSize; dataIndex++)
            {
            mathValue = (ViInt16)RoundRealToNearestInteger(wfmData[dataIndex]*32767.0);
            if (abs(mathValue) > 32767)
                {
                ViChar tmp[512];
                Fmt(tmp, "The data element at index #%ld in the"
                         " waveform array is not normalized.",dataIndex);
                viCheckErrElab(SRDS345_ERROR_INVALID_WFM_ELEMENT,tmp);
                }
            dataBuffer[dataIndex] = mathValue;
            *checksum += mathValue;
            }
    
        checkErr( Ivi_SetNeedToCheckStatus (vi, VI_TRUE));
        viCheckErr( viQueryf (io, "AMOD?%ld", "%ld", wfmSize, &tmpValue));
    
        if (tmpValue == 1)
            {
            viCheckErr(viPrintf (io, "%*hy", wfmSize+1, dataBuffer));
            }
        }
    viCheckParm( Ivi_SetAttributeViInt32 (vi, VI_NULL, SRDS345_ATTR_ARB_MOD_RATE,
                                          0, rateDivider), 4, "Rate Divider");
    checkErr( Ivi_SetAttributeViBoolean (vi, VI_NULL, SRDS345_ATTR_MOD_STATE,
                                         0, VI_TRUE));
    checkErr( srds345_CheckStatus (vi));
Error:
    Ivi_Free (vi, dataBuffer);
    Ivi_UnlockSession (vi, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: srds345_ConfigureFMEnabled
 * Purpose:  This function configures whether the function generator applies
 *           frequency modulation to a channel.
 *****************************************************************************/
ViStatus _VI_FUNC srds345_ConfigureFMEnabled (ViSession vi,
                                              ViConstString channelName,
                                              ViBoolean enabled)
{
    return Ivi_SetAttributeViBoolean (vi, channelName, SRDS345_ATTR_FM_ENABLED,
                                      IVI_VAL_DIRECT_USER_CALL, enabled);
}
 
/*****************************************************************************
 * Function: srds345_ConfigureFMSource
 * Purpose:  This function specifies the source of the modulating signal the 
 *           function generator uses for the channel when frequency modulation
 *           is enabled.
 *****************************************************************************/
ViStatus _VI_FUNC srds345_ConfigureFMSource (ViSession vi, 
                                             ViConstString channelName,
                                             ViInt32 source)
{
    return Ivi_SetAttributeViInt32 (vi, channelName, SRDS345_ATTR_FM_SOURCE,
                                    IVI_VAL_DIRECT_USER_CALL, source);
}
                                           
/*****************************************************************************
 * Function: srds345_ConfigureFMInternal
 * Purpose:  This function configures the attributes that control the
 *           function generator's internal frequency modulation source.
 *           These attributes are the peak deviation, waveform, and
 *           frequency.
 *****************************************************************************/
ViStatus _VI_FUNC  srds345_ConfigureFMInternal (ViSession vi, ViReal64 fmDeviance,
                                                ViInt32 fmWaveform, ViReal64 fmFrequency)
{
    ViStatus error = VI_SUCCESS;
    
    checkErr( Ivi_LockSession (vi, VI_NULL));

        /* Set attributes */
    viCheckParm( Ivi_SetAttributeViInt32 (vi, VI_NULL,
                                          SRDS345_ATTR_FM_INTERNAL_WAVEFORM, 0,
                                          fmWaveform), 3, "FM Waveform");
    viCheckParm( Ivi_SetAttributeViReal64 (vi, VI_NULL,
                                           SRDS345_ATTR_FM_INTERNAL_FREQUENCY, 0,
                                           fmFrequency), 4, "FM Frequency");
    viCheckParm( Ivi_SetAttributeViReal64 (vi, VI_NULL,
                                           SRDS345_ATTR_FM_INTERNAL_DEVIATION, 0,
                                           fmDeviance), 2, "FM Peak Deviance");

    checkErr( srds345_CheckStatus (vi));
Error:
    Ivi_UnlockSession (vi, VI_NULL);
    return error;
} 

/*****************************************************************************
 * Function: srds345_ConfigureFMModArbWaveform
 * Purpose:  This function creates an FM modulation frequency list and
 *           enable FM modulation.
 *****************************************************************************/
ViStatus _VI_FUNC srds345_ConfigureFMModArbWaveform (ViSession vi,
                                                     ViConstString channelName,
                                                     ViInt32 wfmSize,
                                                     ViReal64 wfmData[],
                                                     ViInt32 rateDivider)
{
    ViStatus  error = VI_SUCCESS;
    ViInt32   *dataBuffer = VI_NULL;
    
    checkErr( Ivi_LockSession (vi, VI_NULL));

    if ((wfmSize > SRDS345_VAL_MAX_FM_MOD_SIZE) && (wfmSize < SRDS345_VAL_MIN_MOD_SIZE))
        viCheckParm( IVI_ERROR_INVALID_PARAMETER, 2, "Waveform Size");

    if (!wfmData)
        viCheckParm ( IVI_ERROR_INVALID_PARAMETER,3, "NULL address for Waveform Data");

    checkErr( srds345_VerifyOtherModDisabled (vi, SRDS345_VAL_MOD_FM));
    
    checkErr( Ivi_SetAttributeViInt32 (vi, VI_NULL, SRDS345_ATTR_MOD_TYPE,
                                       0, SRDS345_VAL_MOD_FM));
    checkErr( Ivi_SetAttributeViInt32 (vi, VI_NULL, SRDS345_ATTR_MOD_WAVEFORM,
                                       0, SRDS345_VAL_MOD_ARB));

    if (!Ivi_Simulating(vi))                /* call only when locked */
        {
        ViSession   io = Ivi_IOSession(vi); /* call only when locked */
        ViInt32     dataIndex;
        ViInt32     *checksum, mathValue, tmpValue = 0;
        ViInt32     tmpConst;
        
        /* Build buffer of data points in array wfmData */
        checkErr( Ivi_Alloc (vi, ((ViInt32)sizeof(ViUInt32)*(wfmSize+1)), (ViAddr *)&dataBuffer));
        
        /* Fill buffer with array values */
        checksum = &dataBuffer[wfmSize];
        *checksum = 0;
        tmpConst = 2^32;
        
        for (dataIndex = 0; dataIndex < wfmSize; dataIndex++)
            {
            if ((wfmData[dataIndex] < 0) || (wfmData[dataIndex] > 40e6))
                {
                ViChar tmp[512];
                Fmt(tmp, "The data element at index #%ld in the waveform array is"
                                " out of range.",dataIndex);
                viCheckErrElab(SRDS345_ERROR_INVALID_WFM_ELEMENT,tmp);
                }
            mathValue = (ViInt32)RoundRealToNearestInteger(tmpConst*(wfmData[dataIndex]/40.0e6));
            dataBuffer[dataIndex] = mathValue;
            *checksum += mathValue;
            }

        checkErr( Ivi_SetNeedToCheckStatus (vi, VI_TRUE));
        viCheckErr( viQueryf (io, "AMOD?%ld","%ld", wfmSize, &tmpValue));
    
        if (tmpValue == 1)
            {
            viCheckErr(viPrintf (io, "%*ly", wfmSize+1, dataBuffer)); 
            }
        }
    
    viCheckParm( Ivi_SetAttributeViInt32 (vi, VI_NULL, SRDS345_ATTR_ARB_MOD_RATE,
                                          0, rateDivider), 4, "Rate Divider");
    checkErr( Ivi_SetAttributeViBoolean (vi, VI_NULL, SRDS345_ATTR_MOD_STATE,
                                         0, VI_TRUE));
    checkErr( srds345_CheckStatus (vi));

Error:
    Ivi_Free (vi, dataBuffer);
    Ivi_UnlockSession (vi, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: srds345_ConfigurePMEnabled
 * Purpose:  This function configures whether the function generator applies
 *           frequency modulation to a channel.
 *****************************************************************************/
ViStatus _VI_FUNC srds345_ConfigurePMEnabled (ViSession vi,
                                              ViConstString channelName,
                                              ViBoolean enabled)
{
    return Ivi_SetAttributeViBoolean (vi, channelName, SRDS345_ATTR_PM_ENABLED,
                                      IVI_VAL_DIRECT_USER_CALL, enabled);
}
 
/*****************************************************************************
 * Function: srds345_ConfigurePMInternal
 * Purpose:  This function configures the attributes that control the
 *           function generator's internal phase modulation source.
 *           These attributes are the span, waveform, and
 *           frequency.
 *****************************************************************************/
ViStatus _VI_FUNC  srds345_ConfigurePMInternal (ViSession vi, ViReal64 pmSpan,
                                                ViInt32 pmWaveform, ViReal64 pmFrequency)
{
    ViStatus error = VI_SUCCESS;
    
    checkErr( Ivi_LockSession (vi, VI_NULL));

        /* Set attributes */
    viCheckParm( Ivi_SetAttributeViInt32 (vi, VI_NULL,
                                          SRDS345_ATTR_PM_INTERNAL_WAVEFORM, 0,
                                          pmWaveform), 3, "PM Waveform");
    viCheckParm( Ivi_SetAttributeViReal64 (vi, VI_NULL,
                                           SRDS345_ATTR_PM_INTERNAL_FREQUENCY, 0,
                                           pmFrequency), 4, "PM Frequency");
    viCheckParm( Ivi_SetAttributeViReal64 (vi, VI_NULL,
                                           SRDS345_ATTR_PM_INTERNAL_SPAN, 0,
                                           pmSpan), 2, "PM Span");

    checkErr( srds345_CheckStatus (vi));
    
Error:                                      
    Ivi_UnlockSession (vi, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: srds345_ConfigurePMModArbWaveform
 * Purpose:  This function creates an PM modulation phase list and
 *           enable FM modulation.
 *****************************************************************************/
ViStatus _VI_FUNC srds345_ConfigurePMModArbWaveform (ViSession vi,
                                                     ViConstString channelName,
                                                     ViInt32 wfmSize,
                                                     ViReal64 wfmData[],
                                                     ViInt32 rateDivider)
{
    ViStatus  error = VI_SUCCESS;
    ViInt32   *dataBuffer = VI_NULL;

    checkErr( Ivi_LockSession (vi, VI_NULL));

    if ((wfmSize > SRDS345_VAL_MAX_PM_MOD_SIZE) && (wfmSize < SRDS345_VAL_MIN_MOD_SIZE))
        viCheckParm( IVI_ERROR_INVALID_PARAMETER, 2, "Waveform Size");

    if (!wfmData)
        viCheckParm ( IVI_ERROR_INVALID_PARAMETER,3, "NULL address for Waveform Data");

    checkErr( srds345_VerifyOtherModDisabled (vi, SRDS345_VAL_MOD_PM));
    
    checkErr( Ivi_SetAttributeViInt32 (vi, VI_NULL, SRDS345_ATTR_MOD_TYPE,
                                       0, SRDS345_VAL_MOD_PM));
    checkErr( Ivi_SetAttributeViInt32 (vi, VI_NULL, SRDS345_ATTR_MOD_WAVEFORM,
                                       0, SRDS345_VAL_MOD_ARB));

    if (!Ivi_Simulating(vi))                /* call only when locked */
        {
        ViSession   io = Ivi_IOSession(vi); /* call only when locked */
        ViInt32     dataIndex;
        ViInt32     mathValue, tmpValue = 0;
        ViInt32     *checksum;
        ViInt32     tmpConst;
        
        /* Build buffer of data points in array wfmData */
        checkErr( Ivi_Alloc (vi, ((ViInt32)sizeof(ViInt32)*(wfmSize+1)), (ViAddr *)&dataBuffer));
        /* Fill buffer with array values */
        checksum = &dataBuffer[wfmSize];
        *checksum = 0;
        tmpConst = 2^31;
        
        for (dataIndex = 0; dataIndex < wfmSize; dataIndex++)
            {
            if (abs(wfmData[dataIndex]) > 180)
                {
                ViChar tmp[512];
                Fmt(tmp, "The data element at index #%ld in the waveform array is"
                                " out of range.",dataIndex);
                viCheckErrElab(SRDS345_ERROR_INVALID_WFM_ELEMENT,tmp);
                }
            mathValue = (ViInt32)floor(tmpConst*(wfmData[dataIndex]/180.0));
            dataBuffer[dataIndex] = mathValue;
            *checksum += mathValue;
            }

        checkErr( Ivi_SetNeedToCheckStatus (vi, VI_TRUE));
        viCheckErr( viQueryf (io, "AMOD?%ld", "%ld", wfmSize, &tmpValue));
        
        if (tmpValue == 1)
            viCheckErr(viPrintf (io, "%*ly", wfmSize+1, dataBuffer));
        }
    
    viCheckParm( Ivi_SetAttributeViInt32 (vi, VI_NULL, SRDS345_ATTR_ARB_MOD_RATE,
                                          0, rateDivider), 4, "Rate Divider");
    checkErr( Ivi_SetAttributeViBoolean (vi, VI_NULL, SRDS345_ATTR_MOD_STATE,
                                         0, VI_TRUE));

    checkErr( srds345_CheckStatus (vi));

Error:
    Ivi_Free (vi, dataBuffer);
    Ivi_UnlockSession (vi, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: srds345_ConfigureSweep
 * Purpose:  This function configure sweep modulation of the output signal.
 *           The driver sets these attributes:
 *                SRDS345_ATTR_SWEEP_DIRECTION
 *                SRDS345_ATTR_SWEEP_MOD_RATE
 *                SRDS345_ATTR_SWEEP_START_FREQ
 *                SRDS345_ATTR_SWEEP_STOP_FREQ
 *****************************************************************************/
ViStatus _VI_FUNC srds345_ConfigureSweep (ViSession vi,
                                          ViInt32 direction,
                                          ViReal64 modRate,
                                          ViReal64 start,
                                          ViReal64 stop)
{
    ViStatus error = VI_SUCCESS;

    checkErr( Ivi_LockSession (vi, VI_NULL));

        /* Set attribute */
    viCheckParm( Ivi_SetAttributeViInt32 (vi, VI_NULL, SRDS345_ATTR_SWEEP_DIRECTION,
                                          0, direction), 3, "Direction");
    viCheckParm( Ivi_SetAttributeViReal64 (vi, VI_NULL, SRDS345_ATTR_SWEEP_MOD_RATE,
                                           0, modRate), 4, "Sweep Modulation Frequency");
    viCheckParm( Ivi_SetAttributeViReal64 (vi, VI_NULL, SRDS345_ATTR_SWEEP_START_FREQ,
                                           0, start), 4, "Start Frequency");
    viCheckParm( Ivi_SetAttributeViReal64 (vi, VI_NULL, SRDS345_ATTR_SWEEP_STOP_FREQ,
                                           0, stop), 5, "Stop Frequency");

    checkErr( srds345_CheckStatus (vi));
Error:
    Ivi_UnlockSession (vi, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: srds345_GetAttribute<type> Functions                                    
 * Purpose:  These functions enable the instrument driver user to get 
 *           attribute values directly.  There are typesafe versions for 
 *           ViInt32, ViReal64, ViString, ViBoolean, and ViSession attributes.                                         
 *****************************************************************************/
ViStatus _VI_FUNC srds345_GetAttributeViInt32 (ViSession vi, ViConstString channelName, 
                                               ViAttr attributeId, ViInt32 *value)
{                                                                                                           
    return Ivi_GetAttributeViInt32 (vi, channelName, attributeId, IVI_VAL_DIRECT_USER_CALL, 
                                    value);
}                                                                                                           

ViStatus _VI_FUNC srds345_GetAttributeViReal64 (ViSession vi, ViConstString channelName, 
                                                ViAttr attributeId, ViReal64 *value)
{                                                                                                           
    return Ivi_GetAttributeViReal64 (vi, channelName, attributeId, IVI_VAL_DIRECT_USER_CALL, 
                                     value);
}                                                                                                           
ViStatus _VI_FUNC srds345_GetAttributeViString (ViSession vi, ViConstString channelName, 
                                                ViAttr attributeId, ViInt32 bufSize, 
                                                ViChar value[]) 
{   
    return Ivi_GetAttributeViString (vi, channelName, attributeId, IVI_VAL_DIRECT_USER_CALL, 
                                     bufSize, value);
}   
ViStatus _VI_FUNC srds345_GetAttributeViBoolean (ViSession vi, ViConstString channelName, 
                                                 ViAttr attributeId, ViBoolean *value)
{                                                                                                           
    return Ivi_GetAttributeViBoolean (vi, channelName, attributeId, IVI_VAL_DIRECT_USER_CALL, 
                                      value);
}                                                                                                           
ViStatus _VI_FUNC srds345_GetAttributeViSession (ViSession vi, ViConstString channelName, 
                                                 ViAttr attributeId, ViSession *value)
{                                                                                                           
    return Ivi_GetAttributeViSession (vi, channelName, attributeId, IVI_VAL_DIRECT_USER_CALL, 
                                      value);
}                                                                                                           

/*****************************************************************************
 * Function: srds345_SetAttribute<type> Functions                                    
 * Purpose:  These functions enable the instrument driver user to set 
 *           attribute values directly.  There are typesafe versions for 
 *           ViInt32, ViReal64, ViString, ViBoolean, and ViSession datatypes.                                         
 *****************************************************************************/
ViStatus _VI_FUNC srds345_SetAttributeViInt32 (ViSession vi, ViConstString channelName, 
                                               ViAttr attributeId, ViInt32 value)
{                                                                                                           
    return Ivi_SetAttributeViInt32 (vi, channelName, attributeId, IVI_VAL_DIRECT_USER_CALL, 
                                    value);
}                                                                                                           
ViStatus _VI_FUNC srds345_SetAttributeViReal64 (ViSession vi, ViConstString channelName, 
                                                ViAttr attributeId, ViReal64 value)
{                                                                                                           
    return Ivi_SetAttributeViReal64 (vi, channelName, attributeId, IVI_VAL_DIRECT_USER_CALL, 
                                     value);
}                                                                                                           
ViStatus _VI_FUNC srds345_SetAttributeViString (ViSession vi, ViConstString channelName, 
                                                ViAttr attributeId, ViConstString value) 
{   
    return Ivi_SetAttributeViString (vi, channelName, attributeId, IVI_VAL_DIRECT_USER_CALL, 
                                     value);
}   
ViStatus _VI_FUNC srds345_SetAttributeViBoolean (ViSession vi, ViConstString channelName, 
                                                 ViAttr attributeId, ViBoolean value)
{                                                                                                           
    return Ivi_SetAttributeViBoolean (vi, channelName, attributeId, IVI_VAL_DIRECT_USER_CALL, 
                                      value);
}                                                                                                           
ViStatus _VI_FUNC srds345_SetAttributeViSession (ViSession vi, ViConstString channelName, 
                                                 ViAttr attributeId, ViSession value)
{                                                                                                           
    return Ivi_SetAttributeViSession (vi, channelName, attributeId, IVI_VAL_DIRECT_USER_CALL, 
                                      value);
}                                                                                                           

/*****************************************************************************
 * Function: srds345_CheckAttribute<type> Functions                                  
 * Purpose:  These functions enable the instrument driver user to check  
 *           attribute values directly.  These functions check the value you
 *           specify even if you set the SRDS345_ATTR_RANGE_CHECK 
 *           attribute to VI_FALSE.  There are typesafe versions for ViInt32, 
 *           ViReal64, ViString, ViBoolean, and ViSession datatypes.                         
 *****************************************************************************/
ViStatus _VI_FUNC srds345_CheckAttributeViInt32 (ViSession vi, ViConstString channelName, 
                                                 ViAttr attributeId, ViInt32 value)
{                                                                                                           
    return Ivi_CheckAttributeViInt32 (vi, channelName, attributeId, IVI_VAL_DIRECT_USER_CALL, 
                                      value);
}
ViStatus _VI_FUNC srds345_CheckAttributeViReal64 (ViSession vi, ViConstString channelName, 
                                                  ViAttr attributeId, ViReal64 value)
{                                                                                                           
    return Ivi_CheckAttributeViReal64 (vi, channelName, attributeId, IVI_VAL_DIRECT_USER_CALL, 
                                       value);
}                                                                                                           
ViStatus _VI_FUNC srds345_CheckAttributeViString (ViSession vi, ViConstString channelName, 
                                                  ViAttr attributeId, ViConstString value)  
{   
    return Ivi_CheckAttributeViString (vi, channelName, attributeId, IVI_VAL_DIRECT_USER_CALL, 
                                       value);
}   
ViStatus _VI_FUNC srds345_CheckAttributeViBoolean (ViSession vi, ViConstString channelName, 
                                                   ViAttr attributeId, ViBoolean value)
{                                                                                                           
    return Ivi_CheckAttributeViBoolean (vi, channelName, attributeId, IVI_VAL_DIRECT_USER_CALL, 
                                        value);
}                                                                                                           
ViStatus _VI_FUNC srds345_CheckAttributeViSession (ViSession vi, ViConstString channelName, 
                                                   ViAttr attributeId, ViSession value)
{                                                                                                           
    return Ivi_CheckAttributeViSession (vi, channelName, attributeId, IVI_VAL_DIRECT_USER_CALL, 
                                        value);
}                                                                                                           

/*****************************************************************************
 * Function: srds345_InitiateGeneration
 * Purpose:  This function does not perform any operations.  It is present for
 *           IviFgen class driver compliance.
 *****************************************************************************/
ViStatus _VI_FUNC  srds345_InitiateGeneration (ViSession vi)
{
    IviFgen_InterchangeCheck(vi, "srds345_InitiateGeneration");
    return VI_SUCCESS;
}
    
/*****************************************************************************
 * Function: srds345_AbortGeneration
 * Purpose:  This function does not perform any operations.  It is present for
 *           IviFgen class driver compliance.
 *****************************************************************************/
ViStatus _VI_FUNC  srds345_AbortGeneration (ViSession vi)
{
    return VI_SUCCESS;
}

/*****************************************************************************
 * Function: srds345_SendSoftwareTrigger                                                   
 * Purpose:  Sends a command to trigger the instrument.                      
 *****************************************************************************/
ViStatus _VI_FUNC srds345_SendSoftwareTrigger (ViSession vi)
{
    ViStatus    error = VI_SUCCESS;

    checkErr( Ivi_LockSession(vi, VI_NULL));

    if (Ivi_RangeChecking (vi))
        {
        ViInt32 trigSrc;
        checkErr( Ivi_GetAttributeViInt32 (vi, CHAN1, SRDS345_ATTR_TRIGGER_SOURCE,
                                           0, &trigSrc));
        if (trigSrc != SRDS345_VAL_SOFTWARE_TRIG)
            viCheckErr( SRDS345_ERROR_TRIGGER_NOT_SOFTWARE);
        }

    if (!Ivi_Simulating(vi))                /* call only when locked */
        {
        ViSession   io = Ivi_IOSession(vi); /* call only when locked */
    
        checkErr( Ivi_SetNeedToCheckStatus (vi, VI_TRUE));

        viCheckErr( viWrite (io, "*TRG;", 5, VI_NULL));                     
        }

        /* 
            Do not invoke the srds345_CheckStatus function here.  
            After the user calls this function, the user can check 
            for errors by calling the srds345_error_query function.
        */   

Error:    
    Ivi_UnlockSession(vi, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: srds345_reset                                                         
 * Purpose:  This function resets the instrument.                          
 *****************************************************************************/
ViStatus _VI_FUNC srds345_reset (ViSession vi)
{
    ViStatus    error = VI_SUCCESS;

    checkErr( Ivi_LockSession (vi, VI_NULL));

    if (!Ivi_Simulating(vi))                /* call only when locked */
        {
        ViSession   io = Ivi_IOSession(vi); /* call only when locked */
        
        checkErr( Ivi_SetNeedToCheckStatus (vi, VI_TRUE));
        viCheckErr( viPrintf (io, "*RST"));
        }
    
    checkErr( srds345_DefaultInstrSetup (vi));                                
    
    checkErr( srds345_CheckStatus (vi));                                      

Error:
    Ivi_UnlockSession (vi, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: srds345_self_test                                                       
 * Purpose:  This function executes the instrument self-test and returns the 
 *           result.                                                         
 *****************************************************************************/
ViStatus _VI_FUNC srds345_self_test (ViSession vi, ViInt16 *testResult, 
                                      ViChar testMessage[])
{
    ViStatus    error = VI_SUCCESS;

    checkErr( Ivi_LockSession (vi, VI_NULL));

    if (testResult == VI_NULL)
        viCheckParm( IVI_ERROR_INVALID_PARAMETER, 2, "Null address for Test Result");
    if (testMessage == VI_NULL)
        viCheckParm( IVI_ERROR_INVALID_PARAMETER, 3, "Null address for Test Message");

    if (!Ivi_Simulating(vi))                /* call only when locked */
        {
        ViSession   io = Ivi_IOSession(vi); /* call only when locked */
    
        checkErr( Ivi_SetNeedToCheckStatus (vi, VI_TRUE));
        viCheckErr( viQueryf (io, "*TST?", "%hd", testResult));
        
        testMessage[0] = 0;
    
        }
    else 
        {
            /* Simulate Self Test */
        *testResult = 0;
        strcpy (testMessage, "No error.");
        }
    
    checkErr( srds345_CheckStatus (vi));

Error:
    Ivi_UnlockSession(vi, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: srds345_revision_query                                                  
 * Purpose:  This function returns the driver and instrument revisions.      
 *****************************************************************************/
ViStatus _VI_FUNC srds345_revision_query (ViSession vi, ViChar driverRev[], 
                                           ViChar instrRev[])
{
    ViStatus    error = VI_SUCCESS;
    
    checkErr( Ivi_LockSession (vi, VI_NULL));

    if (driverRev == VI_NULL)
        viCheckParm( IVI_ERROR_INVALID_PARAMETER, 2, "Null address for Driver Revision");
    if (instrRev == VI_NULL)
        viCheckParm( IVI_ERROR_INVALID_PARAMETER, 3, "Null address for Instrument Revision");

    checkErr( Ivi_GetAttributeViString (vi, VI_NULL, SRDS345_ATTR_SPECIFIC_DRIVER_REVISION, 
                                        0, 256, driverRev));
    checkErr( Ivi_GetAttributeViString (vi, VI_NULL, SRDS345_ATTR_INSTRUMENT_FIRMWARE_REVISION, 
                                        0, 256, instrRev));
    checkErr( srds345_CheckStatus (vi));

Error:    
    Ivi_UnlockSession(vi, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: srds345_error_query                                                     
 * Purpose:  This function queries the instrument error queue and returns   
 *           the result.                                                     
 *****************************************************************************/
ViStatus _VI_FUNC srds345_error_query (ViSession vi, ViInt32 *errCode, 
                                       ViChar errMessage[])
{
    ViStatus    error = VI_SUCCESS;
    
    checkErr( Ivi_LockSession (vi, VI_NULL));
    
    if (errCode == VI_NULL)
        viCheckParm( IVI_ERROR_INVALID_PARAMETER, 2, "Null address for Error Code");
    if (errMessage == VI_NULL)
        viCheckParm( IVI_ERROR_INVALID_PARAMETER, 3, "Null address for Error Message");

    viCheckWarn( VI_WARN_NSUP_ERROR_QUERY);

Error:
    Ivi_UnlockSession(vi, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: srds345_error_message                                                  
 * Purpose:  This function translates the error codes returned by this       
 *           instrument driver into user-readable strings.  
 *
 *           Note:  The caller can pass VI_NULL for the vi parameter.  This 
 *           is useful if one of the init functions fail.
 *****************************************************************************/
ViStatus _VI_FUNC srds345_error_message (ViSession vi, ViStatus errorCode,
                                         ViChar errorMessage[256])
{
    ViStatus    error = VI_SUCCESS;
    
    static      IviStringValueTable errorTable = 
        {
            {SRDS345_ERROR_NOT_FUNC_MODE,        "The instrument must be in the Standard Function mode."},   
            {SRDS345_ERROR_NOT_ARB_MODE,         "The instrument must be in the Arbitrary Waveform mode."},
            {SRDS345_ERROR_INVALID_WFM_LENGTH,   "Waveform size must be between 8 and 16300."},
            {SRDS345_ERROR_INVALID_WFM_ELEMENT,  "The data element in the waveform array is not normalized."},
            {SRDS345_ERROR_INVALID_WAVEFORM,     "The specified waveform is invalid."},
            IVIFGEN_ERROR_CODES_AND_MSGS,
            {VI_NULL,                               VI_NULL}
        };
        
    if (vi)
        Ivi_LockSession(vi, VI_NULL);

        /* all VISA and IVI error codes are handled as well as codes in the table */
    if (errorMessage == VI_NULL)
        viCheckParm( IVI_ERROR_INVALID_PARAMETER, 3, "Null address for Error Message");

    checkErr( Ivi_GetSpecificDriverStatusDesc(vi, errorCode, errorMessage, errorTable));

Error:
    if (vi)
        Ivi_UnlockSession(vi, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: srds345_GetErrorInfo and srds345_ClearErrorInfo Functions                       
 * Purpose:  These functions enable the instrument driver user to  
 *           get or clear the error information the driver associates with the
 *           IVI session.                                                        
 *****************************************************************************/
ViStatus _VI_FUNC srds345_GetErrorInfo (ViSession vi, ViStatus *primaryError, 
                                        ViStatus *secondaryError, ViChar errorElaboration[256])  
{                                                                                                           
    return Ivi_GetErrorInfo(vi, primaryError, secondaryError, errorElaboration);                                
}                                                                                                           
ViStatus _VI_FUNC srds345_ClearErrorInfo (ViSession vi)                                                        
{                                                                                                           
    return Ivi_ClearErrorInfo (vi);                                                                             
}

/**************************************************************************** 
 *  Function: srds345_GetNextCoercionRecord                            
 *  Purpose:  Get the next coercion record from the queue.           
 ****************************************************************************/
ViStatus _VI_FUNC srds345_GetNextCoercionRecord (ViSession vi, ViInt32 bufferSize, ViChar record[])
{
    return Ivi_GetNextCoercionString (vi, bufferSize, record);
}

/*****************************************************************************
 * Function: srds345_LockSession and srds345_UnlockSession Functions                        
 * Purpose:  These functions enable the instrument driver user to lock the 
 *           session around a sequence of driver calls during which other
 *           execution threads must not disturb the instrument state.
 *                                                                          
 *           NOTE:  The callerHasLock parameter must be a local variable 
 *           initialized to VI_FALSE and passed by reference, or you can pass 
 *           VI_NULL.                     
 *****************************************************************************/
ViStatus _VI_FUNC srds345_LockSession (ViSession vi, ViBoolean *callerHasLock)  
{                                              
    return Ivi_LockSession(vi,callerHasLock);      
}                                              
ViStatus _VI_FUNC srds345_UnlockSession (ViSession vi, ViBoolean *callerHasLock) 
{                                              
    return Ivi_UnlockSession(vi,callerHasLock);    
}   

/*****************************************************************************
 * Function: WriteInstrData and ReadInstrData Functions                      
 * Purpose:  These functions enable the instrument driver user to  
 *           write and read commands directly to and from the instrument.            
 *                                                                           
 *           Note:  These functions bypass the IVI attribute state caching.  
 *                  WriteInstrData invalidates the cached values for all 
 *                  attributes.
 *****************************************************************************/
ViStatus _VI_FUNC srds345_WriteInstrData (ViSession vi, ViConstString writeBuffer)   
{   
    return Ivi_WriteInstrData (vi, writeBuffer);    
}   
ViStatus _VI_FUNC srds345_ReadInstrData (ViSession vi, ViInt32 numBytes, 
                                         ViChar rdBuf[], ViInt32 *bytesRead)  
{   
    return Ivi_ReadInstrData (vi, numBytes, rdBuf, bytesRead);   
}

/*****************************************************************************
 * Function: srds345_GetError and srds345_ClearError Functions                       
 * Purpose:  These functions enable the instrument driver user to  
 *           get or clear the error information the driver associates with the
 *           IVI session.                                                        
 *****************************************************************************/
ViStatus _VI_FUNC srds345_GetError (ViSession vi, 
                                     ViStatus *errorCode, 
                                     ViInt32 bufferSize,
                                     ViChar description[])  
{
    ViStatus error = VI_SUCCESS;
    ViStatus primary = VI_SUCCESS,
             secondary = VI_SUCCESS;
    ViChar   elaboration[256] = "";
    ViChar   errorMessage[1024] = "";
    ViChar  *appendPoint = errorMessage;
    ViInt32  actualSize = 0;
    ViBoolean locked = VI_FALSE;

    /* Lock Session */
    if (vi != 0)
        {
        checkErr( Ivi_LockSession(vi, &locked));
        }

    /* Test for nulls and acquire error data */
    if (bufferSize != 0)
        {
        if (errorCode == VI_NULL)
            {
            viCheckParm( IVI_ERROR_INVALID_PARAMETER, 2, "Null address for Error");
            }
        if (description == VI_NULL)
            {
            viCheckParm( IVI_ERROR_INVALID_PARAMETER, 4, "Null address for Description");
            }
        checkErr( Ivi_GetErrorInfo (vi, &primary, &secondary, elaboration));
        }

    else
        {
        checkErr( Ivi_GetAttributeViString(vi, VI_NULL, IVI_ATTR_ERROR_ELABORATION, 0, 256, elaboration));
        checkErr( Ivi_GetAttributeViInt32(vi, VI_NULL, IVI_ATTR_SECONDARY_ERROR, 0, &secondary));
        checkErr( Ivi_GetAttributeViInt32(vi, VI_NULL, IVI_ATTR_PRIMARY_ERROR, 0, &primary));
        }
        
    /* Format data */
    if (primary != VI_SUCCESS)
        {
        ViChar msg[256] = "";
        checkErr( srds345_error_message (vi, primary, msg));
        appendPoint += sprintf(appendPoint, "Primary Error: (Hex 0x%08X) %s\n", primary, msg);
        }
    
    if (secondary != VI_SUCCESS)
        {
        ViChar msg[256] = "";
        checkErr( srds345_error_message (vi, secondary, msg));
        appendPoint += sprintf(appendPoint, "Secondary Error: (Hex 0x%08X) %s\n", secondary, msg);
        }
    
    if (elaboration[0])
        {
        sprintf(appendPoint, "Elaboration: %s", elaboration);
        }
    
    actualSize = (ViInt32)strlen(errorMessage) + 1;
    
    /* Prepare return values */
    if (bufferSize == 0)
        {
        error = actualSize;
        }
    else 
        {
        if (bufferSize > 0)
            {
            if (actualSize > bufferSize)
                {
                error = actualSize;
                actualSize = bufferSize;
                }
            }
        memcpy(description, errorMessage, actualSize-1);
        description[actualSize-1] = '\0';
        }
    
    if (errorCode) 
        {
        *errorCode = primary;
        }
    
Error:
    /* Unlock Session */
    Ivi_UnlockSession (vi, &locked);
    return error;
}   

ViStatus _VI_FUNC srds345_ClearError (ViSession vi)                                                        
{                                                                                                           
    return Ivi_ClearErrorInfo (vi);                                                                             
}

/**************************************************************************** 
 *  Function: srds345_GetNextInterchangeWarning,
 *            srds345_ResetInterchangeCheck, and
 *            srds345_ClearInterchangeWarnings
 *  Purpose:  These functions allow the user to retrieve interchangeability
 *            warnings, reset the driver's interchangeability checking 
 *            state, and clear all previously logged interchangeability warnings.
 ****************************************************************************/
ViStatus _VI_FUNC srds345_GetNextInterchangeWarning (ViSession vi, 
                                                      ViInt32 bufferSize, 
                                                      ViChar warnString[])
{
    return Ivi_GetNextInterchangeCheckString (vi, bufferSize, warnString);
}

ViStatus _VI_FUNC srds345_ResetInterchangeCheck (ViSession vi)
{
    return Ivi_ResetInterchangeCheck (vi);
}

ViStatus _VI_FUNC srds345_ClearInterchangeWarnings (ViSession vi)
{
    return Ivi_ClearInterchangeWarnings (vi);
}

/*****************************************************************************
 * Function: srds345_InvalidateAllAttributes
 * Purpose:  This function invalidates the cached value of all attributes.
 *****************************************************************************/
ViStatus _VI_FUNC srds345_InvalidateAllAttributes (ViSession vi)
{
    return Ivi_InvalidateAllAttributes(vi);
}

/**************************************************************************** 
 *  Function: srds345_GetChannelName
 *  Purpose:  This function returns the highest-level channel name that
 *            corresponds to the specific driver channel string that is in
 *            the channel table at an index you specify.
 ****************************************************************************/
ViStatus _VI_FUNC srds345_GetChannelName (ViSession vi, ViInt32 index,
                                           ViInt32 bufferSize,
                                           ViChar name[])
{
   ViStatus         error = VI_SUCCESS;
   ViConstString    channelName;
   ViInt32          actualSize = 0;

    checkErr( Ivi_LockSession (vi, VI_NULL));
    
    if (bufferSize != 0)
        {
        if (name == VI_NULL)
            {
            viCheckParm( IVI_ERROR_INVALID_PARAMETER, 4, "Null address for Channel Name");
            }
        }
    
    /* Get channel name */
    viCheckParm( Ivi_GetNthChannelString (vi, index, &channelName), 2, "Index");
    
    actualSize = (ViInt32)strlen(channelName) + 1;
    
    /* Prepare return values */
    if (bufferSize == 0)
        {
        error = actualSize;
        }
    else 
        {
        if (bufferSize > 0)
            {
            if (actualSize > bufferSize)
                {
                error = actualSize;
                actualSize = bufferSize;
                }
            }
        memcpy(name, channelName, actualSize-1);
        name[actualSize-1] = '\0';
        }

Error:    
    Ivi_UnlockSession(vi, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: srds345_ResetWithDefaults
 * Purpose:  This function resets the instrument and applies default settings
 *           from the IVI Configuration Store based on the logical name
 *           from which the session was created.
 *****************************************************************************/
ViStatus _VI_FUNC srds345_ResetWithDefaults (ViSession vi)
{
    ViStatus error = VI_SUCCESS;
    
    checkErr( Ivi_LockSession (vi, VI_NULL));
    checkErr( srds345_reset(vi));
    checkErr( Ivi_ApplyDefaultSetup(vi));
    
Error:
    Ivi_UnlockSession(vi, VI_NULL);
    return error;
}

/**************************************************************************** 
 *  Function: srds345_Disable
 *  Purpose:  This function places the instrument in a quiescent state as 
 *            quickly as possible.
 ****************************************************************************/
ViStatus _VI_FUNC srds345_Disable (ViSession vi)
{
    return VI_SUCCESS;
}

/*****************************************************************************
 *-------------------- Utility Functions (Not Exported) ---------------------*
 *****************************************************************************/
/*****************************************************************************
 * Function: srds345_Verify<OutputMode>Mode
 * Purpose:  Returns an error if the mode is not correct.
 *****************************************************************************/
static ViStatus srds345_VerifyFuncMode (ViSession vi)
{
    ViStatus error = VI_SUCCESS;
    ViInt32  mode;
    checkErr( Ivi_GetAttributeViInt32 (vi, VI_NULL, SRDS345_ATTR_OUTPUT_MODE,
                                       0, &mode));
    if (mode != SRDS345_VAL_OUTPUT_FUNC) viCheckErr (SRDS345_ERROR_NOT_FUNC_MODE);
Error:
    return error;
}

static ViStatus srds345_VerifyArbMode (ViSession vi)
{
    ViStatus error = VI_SUCCESS;
    ViInt32  mode;
    checkErr( Ivi_GetAttributeViInt32 (vi, VI_NULL, SRDS345_ATTR_OUTPUT_MODE,
                                       0, &mode));
    if (mode != SRDS345_VAL_OUTPUT_ARB) viCheckErr (SRDS345_ERROR_NOT_ARB_MODE);
Error:
    return error;
}

/*****************************************************************************
 * Function: srds345_CheckStatus                                                 
 * Purpose:  This function checks the status of the instrument to detect 
 *           whether the instrument has encountered an error.  This function  
 *           is called at the end of most exported driver functions.  If the    
 *           instrument reports an error, this function returns      
 *           IVI_ERROR_INSTR_SPECIFIC.  The user can set the 
 *           IVI_ATTR_QUERY_INSTR_STATUS attribute to VI_FALSE to disable this 
 *           check and increase execution speed.                                   
 *
 *           Note:  Call this function only when the session is locked.
 *****************************************************************************/
static ViStatus srds345_CheckStatus (ViSession vi)
{
    ViStatus    error = VI_SUCCESS;

    if (Ivi_QueryInstrStatus (vi) && Ivi_NeedToCheckStatus (vi) && !Ivi_Simulating (vi))
        {
        checkErr( srds345_CheckStatusCallback (vi, Ivi_IOSession(vi)));
        checkErr( Ivi_SetNeedToCheckStatus (vi, VI_FALSE));
        }
Error:
    return error;
}

/*****************************************************************************
 * Function: srds345_DefaultInstrSetup                                               
 * Purpose:  This function sends a default setup to the instrument.  The    
 *           srds345_reset function calls this function.  The 
 *           srds345_IviInit function calls this function when the
 *           user passes VI_FALSE for the reset parameter.  This function is 
 *           useful for configuring settings that other instrument driver 
 *           functions require.    
 *
 *           Note:  Call this function only when the session is locked.
 *****************************************************************************/
static ViStatus srds345_DefaultInstrSetup (ViSession vi)
{
    ViStatus    error = VI_SUCCESS;
        
    /* Invalidate all attributes */
    checkErr( Ivi_InvalidateAllAttributes (vi));

    if (!Ivi_Simulating(vi))
    {
    ViSession   io = Ivi_IOSession(vi); /* call only when locked */
    checkErr( Ivi_SetNeedToCheckStatus (vi, VI_TRUE));
    
    /* clear instrument */
    viCheckErr( viPrintf (io, "*CLS;*ESE 1;*SRE 32"));
    
    /* set up default output */
    }
    
    /* Disable AM and FM class extension groups */            
    checkErr( Ivi_SetAttributeViBoolean (vi, "", SRDS345_ATTR_MOD_STATE,
                                       0, VI_FALSE));
  
    /* Set default state to be Standard output mode */
    checkErr( Ivi_SetAttributeViInt32(vi ,VI_NULL, SRDS345_ATTR_OUTPUT_MODE, 
                IVI_VAL_DONT_MARK_AS_SET_BY_USER, SRDS345_VAL_OUTPUT_FUNC));
    
Error:
    return error;
}

/*****************************************************************************
 * Function: srds345_CoerceToResolution                                     
 * Purpose:  Coerce a given value to the next highest step using a given step 
 *           value.                                                                                                    
 *****************************************************************************/
static ViStatus srds345_CoerceToResolution (ViSession vi, 
                                            ViReal64 value, 
                                            ViReal64 res, 
                                            ViReal64 *coercedValue)
{
    ViReal64 absValue = fabs (value);
    ViReal64 div;
    ViReal64 mult;
    
    div = (res < 1.00) ? (1.0/res) * absValue : absValue/res;
    mult = floor(div+0.5);
    *coercedValue = res * ((value > 0.0) ? mult : -mult);
    return VI_SUCCESS;
}

/*****************************************************************************
 * Function: srds345_ModEnabled                                     
 * Purpose:  Determines if a particular Modulation type is enabled.                                                                                                    
 *****************************************************************************/
static ViStatus srds345_ModEnabled (ViSession vi,
                                    ViInt32 modType,
                                    ViBoolean* enabled)
{
    ViStatus  error = VI_SUCCESS;
    ViBoolean state;
    
    checkErr( Ivi_GetAttributeViBoolean (vi, VI_NULL, SRDS345_ATTR_MOD_STATE, 
                                         0, &state));
    if (state)
        {
        ViInt32   actualType;
        
        checkErr( Ivi_GetAttributeViInt32 (vi, VI_NULL, SRDS345_ATTR_MOD_TYPE,
                                           0, &actualType));
        state = (modType == actualType);
        }
Error:
    *enabled = state;
    return error;
}

/*****************************************************************************
 * Function: srds345_VerifyOtherModDisabled                                     
 * Purpose:  Returns an error if a modulation type other than the one
 *           specified is enabled.
 *****************************************************************************/
static ViStatus srds345_VerifyOtherModDisabled (ViSession vi, ViInt32 modType)
{
    ViStatus  error = VI_SUCCESS;
    ViBoolean state;
    
    checkErr( Ivi_GetAttributeViBoolean (vi, VI_NULL, SRDS345_ATTR_MOD_STATE,
                                         0, &state));
    if (state)
        {
        ViInt32 actualType;
        
        checkErr( Ivi_GetAttributeViInt32 (vi, VI_NULL, SRDS345_ATTR_MOD_TYPE,
                                           0, &actualType));
        if (actualType != modType)
            {
            viCheckErrElab( IVI_ERROR_INVALID_CONFIGURATION, 
                            "Another modulation type is currently enabled.");
            }
        }

Error:
    return error;
}

/*****************************************************************************
 * Function: srds345_VerifyContinuous                                     
 * Purpose:  Returns an error if the operation mode is not continuous.
 *****************************************************************************/
static ViStatus srds345_VerifyContinuous (ViSession vi)
{
    ViStatus error = VI_SUCCESS;
    ViInt32 opMode;
    
    checkErr( Ivi_GetAttributeViInt32 (vi, CHAN1, SRDS345_ATTR_OPERATION_MODE,
                                       0, &opMode));
    if (opMode != SRDS345_VAL_OPERATE_CONTINUOUS)
        {
        viCheckErrElab( IVI_ERROR_INVALID_CONFIGURATION,
                        "Operation mode is not set to continuous.");
        }
Error:
    return error;
}

/*****************************************************************************
 * Function: srds345_VerifyNotArbNoise
 * Purpose:  Returns an error if the waveform is set to noise or arb.
 *****************************************************************************/
static ViStatus srds345_VerifyNotArbNoise (ViSession vi)
{
    ViStatus error = VI_SUCCESS;
    checkErr( srds345_VerifyFuncMode (vi));
    checkErr( srds345_VerifyNotNoise(vi));
Error:
    return error;
}    

static ViStatus srds345_VerifyNotNoise (ViSession vi)
{
    ViStatus error = VI_SUCCESS;
    ViInt32  wfm;
    
    checkErr( Ivi_GetAttributeViInt32 (vi, VI_NULL, SRDS345_ATTR_WAVEFORM,
                                       0, &wfm));
    if (wfm == SRDS345_VAL_FGEN_NOISE)
        {
        viCheckErrElab( IVI_ERROR_INVALID_CONFIGURATION,
                        "Cannot perform operation while waveform is set to noise.");
        }
Error:
    return error;
}    


/*****************************************************************************
 * Function: srds345_VerifyOpMode
 * Purpose:  Vefiries desired Operation Mode is set/not set.
 *****************************************************************************/
static ViStatus srds345_VerifyOpMode (ViSession vi, 
                                      ViInt32 OpMode, 
                                      ViBoolean enabled)
{
    ViStatus error = VI_SUCCESS;
    ViInt32  operationMode;
    
    checkErr( Ivi_GetAttributeViInt32 (vi, CHAN1, SRDS345_ATTR_OPERATION_MODE, 0,
                                       &operationMode));
    if (enabled)
        if (operationMode != OpMode)
            viCheckErrElab( IVI_ERROR_INVALID_CONFIGURATION,
                        "Cannot perform operation in current operation mode.");
    else if (operationMode == OpMode)
        viCheckErrElab( IVI_ERROR_INVALID_CONFIGURATION,
                        "Cannot perform operation in current operation mode.");
                        
Error:
    return error;
}
    
/*****************************************************************************
 * Function: srds345_EnableMod
 * Purpose:  Enables a particular modulation type.
 *****************************************************************************/
static ViStatus srds345_EnableMod (ViSession vi, 
                                   ViInt32 modType, 
                                   ViBoolean enable)
{
    ViStatus error = VI_SUCCESS;
    ViBoolean setState = VI_TRUE;
    
    if (enable)
        {
        checkErr( Ivi_SetAttributeViInt32 (vi, VI_NULL, SRDS345_ATTR_MOD_TYPE,
                                           0, modType));
        }
    else
        {
        ViInt32  actualType;
        
        checkErr( Ivi_GetAttributeViInt32 (vi, VI_NULL, SRDS345_ATTR_MOD_TYPE,
                                           0, &actualType));
        setState = (actualType == modType);
        }
    if (setState)
        {
        checkErr( Ivi_SetAttributeViBoolean (vi, VI_NULL, SRDS345_ATTR_MOD_STATE,
                                             0, enable));
        }
Error:
    return error;
    
}

/*****************************************************************************
 * Function: srds345_QueryMod
 * Purpose:  Returns whether modulation is enabled, and if so, what kind.
 *****************************************************************************/
static ViStatus srds345_QueryMod (ViSession vi, 
                                  ViBoolean* modState, 
                                  ViInt32* modType)
{
    ViStatus error = VI_SUCCESS;
    
    checkErr( Ivi_GetAttributeViBoolean (vi, VI_NULL, SRDS345_ATTR_MOD_STATE,
                                         0, modState));
    if (modState)
        {
        checkErr( Ivi_GetAttributeViInt32 (vi, VI_NULL, SRDS345_ATTR_MOD_TYPE,
                                           0, modType));
        }
Error:
    return error;
}

/*****************************************************************************
 * Function: srds345_<Read/Send>FloatMod
 * Purpose:  Performs IO to set or get a Real64 value.
 *****************************************************************************/
static ViStatus srds345_ReadFloat (ViSession vi, ViSession io, ViString cmd, ViReal64* v)
{
    ViStatus error = VI_SUCCESS;
    viCheckErr (viQueryf (io, cmd, "%lf", v));
Error: 
    return error;
}

static ViStatus srds345_SendFloat (ViSession vi, ViSession io, ViString cmd, ViReal64 v)
{
    ViStatus error = VI_SUCCESS;
    viCheckErr (viPrintf (io, cmd, v));
Error: 
    return error;
}

/*****************************************************************************
 * Function: srds345_ForceFloatToMax
 * Purpose:  If the attribute's current value is above max, this function
 *           sets the attribute to max.
 *****************************************************************************/
static ViStatus srds345_ForceFloatToMax (ViSession vi,
                                         ViConstString chan,
                                         ViAttr attrId,
                                         ViReal64 max)
{
    ViStatus error = VI_SUCCESS;
    ViReal64 val;
    checkErr( Ivi_GetAttributeViReal64 (vi, chan, attrId, 0, &val));
    if (val > max)
        {
        checkErr( Ivi_SetAttributeViReal64 (vi, chan, attrId, 
                                            IVI_VAL_DONT_MARK_AS_SET_BY_USER,
                                            max));
        }
Error:
    return error;
}    

/*****************************************************************************
 * Function: srds345_ForceWfmFromNoise
 * Purpose:  If the hidden waveform attribute is set to noise, this forces it
 *           to sine.
 *****************************************************************************/
static ViStatus srds345_ForceWfmFromNoise (ViSession vi)
{
    ViStatus error = VI_SUCCESS;
    ViInt32 wfm;                                                               
                                                                           
    checkErr( Ivi_GetAttributeViInt32 (vi, VI_NULL, SRDS345_ATTR_WAVEFORM,     
                                       0, &wfm));                              
    if (wfm == SRDS345_VAL_FGEN_NOISE)                                         
        checkErr( Ivi_SetAttributeViInt32 (vi, VI_NULL, SRDS345_ATTR_WAVEFORM, 
                                           0, SRDS345_VAL_FGEN_SINE));
Error:
    return error;
}    
                                                                              
/*****************************************************************************
 * Function: srds345_GetMaxFrequency
 * Purpose:  Returns the max frequency for the current waveform.
 *****************************************************************************/
static ViStatus srds345_GetMaxFrequency (ViSession vi, ViReal64* max)
{
    ViStatus error = VI_SUCCESS;
    ViInt32  wfm;
    
    checkErr(Ivi_GetAttributeViInt32  (vi, VI_NULL, SRDS345_ATTR_WAVEFORM, 0, &wfm));
    *max = (wfm == SRDS345_VAL_FGEN_TRIANGLE || wfm == SRDS345_VAL_FGEN_RAMP_UP) ?
            SRDS345_VAL_MAX_FREQ_TRIANGLE : SRDS345_VAL_MAX_FREQ_SINE;
Error:
    return error;
}    


/*****************************************************************************
 * Function: srds345_VerifyBurstTime
 * Purpose:  Verifies that the total burst time is within range.  Passing 0
 *           for the burst count causes the function to get the count.
 *****************************************************************************/
static ViStatus srds345_VerifyBurstTime (ViSession vi, 
                                         ViInt32 count, 
                                         ViReal64 freq)
{
    ViStatus error = VI_SUCCESS;
    if (count == 0)
        {
        checkErr( Ivi_GetAttributeViInt32 (vi, CHAN1, SRDS345_ATTR_BURST_COUNT,
                                           0, &count));
        }
    if (count / freq > SRDS345_VAL_MAX_BURST_TIME)
        viCheckErrElab(IVI_ERROR_INVALID_CONFIGURATION, "Total burst time must be less than 500s");
Error:
    return error;
}

/*****************************************************************************
 * Function: srds345_GetFrequency
 * Purpose:  returns the equivalent arb frequency.
 *****************************************************************************/
static ViStatus srds345_GetFrequency (ViSession vi, ViReal64* freq)
{
    ViStatus error = VI_SUCCESS;
    ViInt32  wfm;
    
    checkErr( Ivi_GetAttributeViInt32 (vi, VI_NULL, SRDS345_ATTR_WAVEFORM, 0, &wfm));
    if (wfm == SRDS345_VAL_FGEN_ARB)
        {
        ViReal64 rate;
        ViInt32 pts;
        checkErr( Ivi_GetAttributeViInt32 (vi, VI_NULL, SRDS345_ATTR_NUM_ARB_POINTS,
                                           0, &pts));
        checkErr( Ivi_GetAttributeViReal64 (vi, VI_NULL, SRDS345_ATTR_ARB_SAMPLE_RATE,
                                            0, &rate));
        *freq = pts/rate;                                        
        }
    else
        {
        checkErr( Ivi_GetAttributeViReal64 (vi, CHAN1, SRDS345_ATTR_FUNC_FREQUENCY, 
                                            0, freq));
        }                                            
Error:
    return error;
}    

/*****************************************************************************
 * Function: srds345AttrFrequency_CoerceCallback                                     
 * Purpose:  Callback for several frequency attributes.                                                                                                    
 *****************************************************************************/
static ViStatus _VI_FUNC srds345AttrFrequency_CoerceCallback (ViSession vi,
                                                              ViConstString channelName,
                                                              ViAttr attributeId,
                                                              ViReal64 value,
                                                              ViReal64* coercedValue)
{
    ViStatus error = VI_SUCCESS;
    ViReal64 res = 0.0;    
    
    if (value < .01)
        res = 0.001;
    else if (value < 1.0)
        res = 0.01;
    else if (value < 10.00)
        res = 0.1;
    else if (value < 100.00)
        res = 1.0;
    else if (value < 1000.00)
        res = 10.0;
    else if (value < 10000.00)
        res = 100.0;
    else
        return VI_SUCCESS;
    return srds345_CoerceToResolution (vi, value, res, coercedValue);
}


/*****************************************************************************
 * Function: srds345AttrAMFMPMWaveform_<Read/Write>Callback                                     
 * Purpose:  Callback for AM, FM, PM internal waveforms.                                                                                                    
 *****************************************************************************/
static ViStatus _VI_FUNC srds345AttrAMFMPMWaveform_ReadCallback (ViSession vi,
                                                                 ViSession io,
                                                                 ViConstString channelName,
                                                                 ViAttr attributeId,
                                                                 ViInt32  *value)
{
    return Ivi_GetAttributeViInt32 (vi, VI_NULL, SRDS345_ATTR_MOD_WAVEFORM, 0, value);
}

static ViStatus _VI_FUNC srds345AttrAMFMPMWaveform_WriteCallback (ViSession vi,
                                                                  ViSession io,
                                                                  ViConstString channelName,
                                                                  ViAttr attributeId,
                                                                  ViInt32 value)
{
    ViInt32 cmdVal;
    ViStatus error = VI_SUCCESS;
    IviRangeTablePtr attrRangeTable;
    
    checkErr (Ivi_GetAttrRangeTable(vi,VI_NULL, attributeId, &attrRangeTable));
    checkErr (Ivi_GetViInt32EntryFromValue(value, attrRangeTable, VI_NULL, VI_NULL, VI_NULL, VI_NULL, VI_NULL, &cmdVal));
    checkErr (Ivi_SetAttributeViInt32 (vi, VI_NULL, SRDS345_ATTR_MOD_WAVEFORM, 0, cmdVal));

Error:           
    return error;
}

/*****************************************************************************
 * Function: srds345AttrAMFMPMSwpFreq_<Read/Write>Callback                                     
 * Purpose:  Callback for AM, FM, PM, and sweep frequencies.
 *****************************************************************************/
static ViStatus _VI_FUNC srds345AttrAMFMPMSwpFreq_ReadCallback (ViSession vi,
                                                                ViSession io,
                                                                ViConstString channelName,
                                                                ViAttr attributeId,
                                                                ViReal64 *value)
{
    return Ivi_GetAttributeViReal64 (vi, VI_NULL, SRDS345_ATTR_MOD_FREQ, 0, value);
}

static ViStatus _VI_FUNC srds345AttrAMFMPMSwpFreq_WriteCallback (ViSession vi,
                                                                 ViSession io,
                                                                 ViConstString channelName,
                                                                 ViAttr attributeId,
                                                                 ViReal64 value)
{
    return Ivi_SetAttributeViReal64 (vi, VI_NULL, SRDS345_ATTR_MOD_FREQ, 0, value);
}

/*****************************************************************************
 *------------------------ Global Session Callbacks -------------------------*
 *****************************************************************************/

/*****************************************************************************
 * Function: srds345_CheckStatusCallback                                               
 * Purpose:  This function queries the instrument to determine if it has 
 *           encountered an error.  If the instrument has encountered an 
 *           error, this function returns the IVI_ERROR_INSTRUMENT_SPECIFIC 
 *           error code.  This function is called by the 
 *           srds345_CheckStatus utility function.  The 
 *           Ivi_SetAttribute and Ivi_GetAttribute functions invoke this 
 *           function when the optionFlags parameter includes the
 *           IVI_VAL_DIRECT_USER_CALL flag.
 *           
 *           The user can disable calls to this function by setting the 
 *           IVI_ATTR_QUERY_INSTR_STATUS attribute to VI_FALSE.  The driver can 
 *           disable the check status callback for a particular attribute by 
 *           setting the IVI_VAL_DONT_CHECK_STATUS flag.
 *****************************************************************************/
static ViStatus _VI_FUNC srds345_CheckStatusCallback (ViSession vi, ViSession io)
{
    ViStatus    error = VI_SUCCESS;
    ViInt16     esr = 0;     
    
    viCheckErr( viQueryf (io, "*ESR?", "%hd", &esr));
    if (esr & IEEE_488_2_ERROR_MASK) viCheckErr( IVI_ERROR_INSTR_SPECIFIC);

Error:
    return error;
}

/*****************************************************************************
 *----------------- Attribute Range Tables and Callbacks --------------------*
 *****************************************************************************/

/*- SRDS345_ATTR_ID_QUERY_RESPONSE -*/
static ViStatus _VI_FUNC srds345AttrIdQueryResponse_ReadCallback (ViSession vi, 
                                                                  ViSession io,
                                                                  ViConstString channelName, 
                                                                  ViAttr attributeId,
                                                                  const ViConstString cacheValue)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      rdBuffer[BUFFER_SIZE];
    ViUInt32    retCnt;
    
    viCheckErr( viPrintf (io, "*IDN?"));
    viCheckErr( viRead (io, rdBuffer, BUFFER_SIZE-1, &retCnt));
    rdBuffer[retCnt] = 0;

    checkErr( Ivi_SetValInStringCallback (vi, attributeId, rdBuffer));
    
Error:
    return error;
}


/*- SRDS345_ATTR_INSTRUMENT_FIRMWARE_REVISION -*/

static ViStatus _VI_FUNC srds345AttrFirmwareRevision_ReadCallback (ViSession vi, 
                                                                    ViSession io,
                                                                    ViConstString channelName, 
                                                                    ViAttr attributeId,
                                                                    const ViConstString cacheValue)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      idQ[BUFFER_SIZE], rdBuffer[BUFFER_SIZE];
    ViUInt32    retCnt;
    
    checkErr( Ivi_GetAttributeViString (vi, VI_NULL, SRDS345_ATTR_ID_QUERY_RESPONSE,
                                        0, BUFFER_SIZE, idQ));

    sscanf (idQ, "%*[^,],%*[^,],%*[^,],%256[^\n]", rdBuffer);

    checkErr( Ivi_SetValInStringCallback (vi, attributeId, rdBuffer));
    
Error:
    return error;
}
    
/*- SRDS345_ATTR_INSTRUMENT_MANUFACTURER -*/

static ViStatus _VI_FUNC srds345AttrInstrumentManufacturer_ReadCallback (ViSession vi, 
                                                                          ViSession io,
                                                                          ViConstString channelName, 
                                                                          ViAttr attributeId,
                                                                          const ViConstString cacheValue)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      idQ[BUFFER_SIZE], rdBuffer[BUFFER_SIZE];
    ViUInt32    retCnt;
    
    checkErr( Ivi_GetAttributeViString (vi, VI_NULL, SRDS345_ATTR_ID_QUERY_RESPONSE,
                                        0, BUFFER_SIZE, idQ));

    sscanf (idQ, "%256[^,]", rdBuffer);

    checkErr( Ivi_SetValInStringCallback (vi, attributeId, rdBuffer));
    
Error:
    return error;
}
    
/*- SRDS345_ATTR_INSTRUMENT_MODEL -*/

static ViStatus _VI_FUNC srds345AttrInstrumentModel_ReadCallback (ViSession vi, 
                                                                   ViSession io,
                                                                   ViConstString channelName, 
                                                                   ViAttr attributeId,
                                                                   const ViConstString cacheValue)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      idQ[BUFFER_SIZE], rdBuffer[BUFFER_SIZE];
    ViUInt32    retCnt;
    
    checkErr( Ivi_GetAttributeViString (vi, VI_NULL, SRDS345_ATTR_ID_QUERY_RESPONSE,
                                        0, BUFFER_SIZE, idQ));

    sscanf (idQ, "%*[^,],%256[^,]", rdBuffer);

    checkErr( Ivi_SetValInStringCallback (vi, attributeId, rdBuffer));
    
Error:
    return error;
}

/*- SRDS345_ATTR_SPECIFIC_DRIVER_REVISION -*/
static ViStatus _VI_FUNC srds345AttrSpecificDriverRevision_ReadCallback (ViSession vi, 
                                                                 ViSession io,
                                                                 ViConstString channelName, 
                                                                 ViAttr attributeId,
                                                                 const ViConstString cacheValue)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      driverRevision[256];
    
    
    sprintf (driverRevision, 
             "Driver: srds345 %d.%d, Compiler: %s %3.2f, "
             "Components: IVIEngine %.2f, VISA-Spec %.2f",
             SRDS345_MAJOR_VERSION , SRDS345_MINOR_VERSION, 
             IVI_COMPILER_NAME, IVI_COMPILER_VER_NUM, 
             IVI_ENGINE_MAJOR_VERSION + IVI_ENGINE_MINOR_VERSION/1000.0, 
             Ivi_ConvertVISAVer(VI_SPEC_VERSION));

    checkErr( Ivi_SetValInStringCallback (vi, attributeId, driverRevision));    
Error:
    return error;
}

/*- SRDS345_ATTR_OPERATION_MODE -*/
static IviRangeTableEntry attrOperationModeRangeTableEntries[] =
    {
        {SRDS345_VAL_OPERATE_CONTINUOUS, 0, 0, "", 0},
        {SRDS345_VAL_OPERATE_BURST,      0, 0, "", SRDS345_VAL_MOD_BURST},
        {SRDS345_VAL_OPERATE_SWEEP_LIN,  0, 0, "", SRDS345_VAL_MOD_LIN},
        {SRDS345_VAL_OPERATE_SWEEP_LOG,  0, 0, "", SRDS345_VAL_MOD_LOG},
        {IVI_RANGE_TABLE_LAST_ENTRY}
    };
static IviRangeTable attrOperationModeRangeTable =
    {
        IVI_VAL_DISCRETE,
        VI_FALSE,
        VI_FALSE,
        VI_NULL,
        attrOperationModeRangeTableEntries,
    };

static ViStatus _VI_FUNC srds345AttrOperationMode_WriteCallback (ViSession vi,
                                                                 ViSession io,
                                                                 ViConstString channelName,
                                                                 ViAttr attributeId,
                                                                 ViInt32 value)
{
    ViStatus  error = VI_SUCCESS;
    ViInt32   modVal;
    
    switch (value)
    {
    case SRDS345_VAL_OPERATE_CONTINUOUS:
        {
        /* turn of burst and sweeps. */
        ViBoolean state;
        ViInt32 type;

        checkErr( srds345_QueryMod (vi, &state, &type));
        if (state)
            {
            if (type == SRDS345_VAL_MOD_BURST ||
                type == SRDS345_VAL_MOD_LIN ||
                type == SRDS345_VAL_MOD_LOG)
                {
                checkErr( Ivi_SetAttributeViBoolean (vi, VI_NULL, 
                                                     SRDS345_ATTR_MOD_STATE,
                                                     0, VI_FALSE));
                }                                                     
            }
        }
        break;
    case SRDS345_VAL_OPERATE_BURST:
        {
        checkErr( srds345_ForceWfmFromNoise(vi));
        }
        break;
    case SRDS345_VAL_OPERATE_SWEEP_LIN:
    case SRDS345_VAL_OPERATE_SWEEP_LOG:
    default:
        {
        ViInt32 outMode;
        
        checkErr( Ivi_GetAttributeViInt32 (vi, VI_NULL, SRDS345_ATTR_OUTPUT_MODE,
                                           0, &outMode));
        /* force output mode to func*/
        if (outMode == SRDS345_VAL_OUTPUT_ARB)
            {
            checkErr( Ivi_SetAttributeViInt32 (vi, VI_NULL, SRDS345_ATTR_OUTPUT_MODE,
                                               IVI_VAL_DONT_MARK_AS_SET_BY_USER,
                                               SRDS345_VAL_OUTPUT_FUNC));
            }
        else 
            {
            checkErr( srds345_ForceWfmFromNoise(vi));
            }
        }
        break;
    }
    if (value != SRDS345_VAL_OPERATE_CONTINUOUS)
        {
        viCheckErr( Ivi_GetViInt32EntryFromValue (value, &attrOperationModeRangeTable,
                                                  VI_NULL, VI_NULL, VI_NULL, VI_NULL,
                                                  VI_NULL, &modVal));
        checkErr( srds345_EnableMod (vi, modVal, VI_TRUE));
        }                                                  
Error:
    return error;
}

static ViStatus _VI_FUNC srds345AttrOperationMode_ReadCallback (ViSession vi,
                                                                ViSession io,
                                                                ViConstString channelName,
                                                                ViAttr attributeId,
                                                                ViInt32 *value)
{
    ViStatus    error = VI_SUCCESS;
    ViInt32     opMode = SRDS345_VAL_OPERATE_CONTINUOUS;
    ViBoolean   state;
    ViInt32     type;
    
    checkErr( srds345_QueryMod (vi, &state, &type));
    if (state)
        {
        if (type == SRDS345_VAL_MOD_BURST)
            opMode = SRDS345_VAL_OPERATE_BURST;
        else if (type == SRDS345_VAL_MOD_LIN)
            opMode = SRDS345_VAL_OPERATE_SWEEP_LIN;
        else if (type == SRDS345_VAL_MOD_LOG)
            opMode = SRDS345_VAL_OPERATE_SWEEP_LOG;
        }
Error:
    *value = opMode;
    return error;
}

/*- SRDS345_ATTR_OUTPUT_MODE -*/
static IviRangeTableEntry attrOutputModeRangeTableEntries[] =
    {
        {SRDS345_VAL_OUTPUT_FUNC, 0, 0, "", 0},
        {SRDS345_VAL_OUTPUT_ARB,  0, 0, "", 0},
        {IVI_RANGE_TABLE_LAST_ENTRY}
    };
static IviRangeTable attrOutputModeRangeTable =
    {
        IVI_VAL_DISCRETE,
        VI_FALSE,
        VI_FALSE,
        VI_NULL,
        attrOutputModeRangeTableEntries,
    };

static ViStatus _VI_FUNC srds345AttrOutputMode_CheckCallback (ViSession vi,
                                                              ViConstString channelName,
                                                              ViAttr attributeId,
                                                              ViInt32 value)
{
    ViStatus error = VI_SUCCESS;
    ViInt32  opMode;
    
    checkErr( Ivi_GetAttributeViInt32 (vi, CHAN1, SRDS345_ATTR_OPERATION_MODE,
                                       0, &opMode));
    if (value == SRDS345_VAL_OUTPUT_ARB)
        {
        checkErr( srds345_VerifyOpMode (vi, SRDS345_VAL_OPERATE_SWEEP_LIN, VI_FALSE));
        checkErr( srds345_VerifyOpMode (vi, SRDS345_VAL_OPERATE_SWEEP_LOG, VI_FALSE));
        }
    else if (value == SRDS345_VAL_WFM_NOISE)
        checkErr( srds345_VerifyOpMode (vi, SRDS345_VAL_OPERATE_CONTINUOUS, VI_TRUE)); 
        
Error:
    return error;
}

static ViStatus _VI_FUNC srds345AttrOutputMode_WriteCallback (ViSession vi,
                                                              ViSession io,
                                                              ViConstString channelName,
                                                              ViAttr attributeId,
                                                              ViInt32 value)
{
    ViStatus error = VI_SUCCESS;
    ViInt32  modType;
    
    checkErr( Ivi_SetAttributeViInt32 (vi, VI_NULL, SRDS345_ATTR_WAVEFORM, 0, SRDS345_VAL_FGEN_SINE));

    if (value == SRDS345_VAL_OUTPUT_ARB)
    {
        checkErr( srds345_EnableMod (vi, SRDS345_VAL_MOD_FM, VI_FALSE));
        checkErr( srds345_EnableMod (vi, SRDS345_VAL_MOD_PM, VI_FALSE));
    }
    
Error:
    return error;
}

        
/*- SRDS345_ATTR_REF_CLOCK_SOURCE -*/
static IviRangeTableEntry attrRefClockSourceRangeTableEntries[] =
    {
        {SRDS345_VAL_REF_CLOCK_INTERNAL, 0, 0, "", 0},
        {SRDS345_VAL_REF_CLOCK_EXTERNAL, 0, 0, "", 0},
        {IVI_RANGE_TABLE_LAST_ENTRY}
    };
static IviRangeTable attrRefClockSourceRangeTable =
    {
        IVI_VAL_DISCRETE,
        VI_FALSE,
        VI_FALSE,
        VI_NULL,
        attrRefClockSourceRangeTableEntries,
    };

/*- SRDS345_ATTR_OUTPUT_IMPEDANCE -*/
static IviRangeTableEntry attrOutputImpedanceRangeTableEntries[] =
    {
        {50.00, 0, 0, VI_NULL, 0},
        {IVI_RANGE_TABLE_LAST_ENTRY}
    };
    
static IviRangeTable attrOutputImpedanceRangeTable =
    {
        IVI_VAL_DISCRETE,
        VI_FALSE,
        VI_FALSE,
        VI_NULL,
        attrOutputImpedanceRangeTableEntries,
    };

/*- SRDS345_ATTR_OUTPUT_ENABLED -*/
static ViStatus _VI_FUNC srds345AttrOutputEnabled_CheckCallback (ViSession vi,
                                                                 ViConstString channelName,
                                                                 ViAttr attributeId,
                                                                 ViBoolean value)
{
    ViStatus    error = VI_SUCCESS;
    if (!value) viCheckErr(IVI_ERROR_INVALID_VALUE);
Error:
    return error;
}

/********************************** Standard Function Output *******************************/
/*- SRDS345_ATTR_FUNC_WAVEFORM -*/
static IviRangeTableEntry attrFuncWaveformRangeTableEntries[] =
    {
        {SRDS345_VAL_WFM_SINE,     0, 0, "", SRDS345_VAL_FGEN_SINE},
        {SRDS345_VAL_WFM_SQUARE,   0, 0, "", SRDS345_VAL_FGEN_SQUARE},
        {SRDS345_VAL_WFM_TRIANGLE, 0, 0, "", SRDS345_VAL_FGEN_TRIANGLE},
        {SRDS345_VAL_WFM_RAMP_UP,  0, 0, "", SRDS345_VAL_FGEN_RAMP_UP},
        {SRDS345_VAL_WFM_NOISE,    0, 0, "", SRDS345_VAL_FGEN_NOISE},
        {IVI_RANGE_TABLE_LAST_ENTRY}
    };
static IviRangeTable attrFuncWaveformRangeTable =
    {
        IVI_VAL_DISCRETE,
        VI_FALSE,
        VI_FALSE,
        VI_NULL,
        attrFuncWaveformRangeTableEntries,
    };

static ViStatus _VI_FUNC srds345AttrFuncWaveform_CheckCallback (ViSession vi,
                                                                ViConstString channelName,
                                                                ViAttr attributeId,
                                                                ViInt32 value)
{
    ViStatus error = VI_SUCCESS;
    ViInt32 wfmVal;
    
    checkErr( srds345_VerifyFuncMode (vi));
    viCheckErr( Ivi_GetViInt32EntryFromValue (value, &attrFuncWaveformRangeTable,
                                              VI_NULL, VI_NULL, VI_NULL, VI_NULL,
                                              VI_NULL, &wfmVal));
    checkErr( Ivi_CheckAttributeViInt32 (vi, VI_NULL, SRDS345_ATTR_WAVEFORM, 0, wfmVal));
                                    
Error:
    return error;
}

static ViStatus _VI_FUNC srds345AttrFuncWaveform_WriteCallback (ViSession vi,
                                                                ViSession io,
                                                                ViConstString channelName,
                                                                ViAttr attributeId,
                                                                ViInt32 value)
{
    ViStatus error = VI_SUCCESS;
    ViInt32 wfmVal;

    viCheckErr( Ivi_GetViInt32EntryFromValue (value, &attrFuncWaveformRangeTable,
                                              VI_NULL, VI_NULL, VI_NULL, VI_NULL,
                                              VI_NULL, &wfmVal));
    checkErr(Ivi_SetAttributeViInt32 (vi, VI_NULL, SRDS345_ATTR_WAVEFORM, 0, wfmVal));
Error:
    return error;
}

static ViStatus _VI_FUNC srds345AttrFuncWaveform_ReadCallback (ViSession vi,
                                                               ViSession io,
                                                               ViConstString channelName,
                                                               ViAttr attributeId,
                                                               ViInt32 *value)
{
    ViStatus error = VI_SUCCESS;
    ViInt32  wfmVal;
    
    checkErr( srds345_VerifyFuncMode (vi));
    checkErr(Ivi_GetAttributeViInt32 (vi,VI_NULL, SRDS345_ATTR_WAVEFORM, 0, &wfmVal));
    viCheckErr( Ivi_GetViInt32EntryFromCmdValue (wfmVal, &attrFuncWaveformRangeTable, 
                                                 value, VI_NULL, VI_NULL, 
                                                 VI_NULL, VI_NULL));
Error:
    return error;
}

/*- SRDS345_ATTR_FUNC_AMPLITUDE -*/
static ViStatus _VI_FUNC srds345AttrFuncAmplitude_CheckCallback (ViSession vi,
                                                                 ViConstString channelName,
                                                                 ViAttr attributeId,
                                                                 ViReal64 value)
{
    ViStatus error = VI_SUCCESS;
    checkErr( srds345_VerifyFuncMode (vi));
    checkErr( Ivi_CheckAttributeViReal64 (vi, VI_NULL, SRDS345_ATTR_AMPLITUDE, 0, value));
Error:
    return error;
}

static ViStatus _VI_FUNC srds345AttrFuncAmplitude_WriteCallback (ViSession vi,
                                                                 ViSession io,
                                                                 ViConstString channelName,
                                                                 ViAttr attributeId,
                                                                 ViReal64 value)
{
    return Ivi_SetAttributeViReal64 (vi, VI_NULL, SRDS345_ATTR_AMPLITUDE, 0, value);
}

static ViStatus _VI_FUNC srds345AttrFuncAmplitude_ReadCallback (ViSession vi,
                                                                ViSession io,
                                                                ViConstString channelName,
                                                                ViAttr attributeId,
                                                                ViReal64 *value)
{
    ViStatus error = VI_SUCCESS;
    
    checkErr( srds345_VerifyFuncMode (vi));
    checkErr (Ivi_GetAttributeViReal64 (vi, VI_NULL, SRDS345_ATTR_AMPLITUDE, 0, value));

Error:
    return error;
}

/*- SRDS345_ATTR_FUNC_DC_OFFSET -*/
static ViStatus _VI_FUNC srds345AttrFuncDCOffset_CheckCallback (ViSession vi,
                                                                ViConstString channelName,
                                                                ViAttr attributeId,
                                                                ViReal64 value)
{
    ViStatus error = VI_SUCCESS;
    checkErr( srds345_VerifyFuncMode (vi));
    checkErr( Ivi_CheckAttributeViReal64 (vi, VI_NULL, SRDS345_ATTR_OFFSET, 0, value));
Error:
    return error;
}

static ViStatus _VI_FUNC srds345AttrFuncDCOffset_WriteCallback (ViSession vi,
                                                                ViSession io,
                                                                ViConstString channelName,
                                                                ViAttr attributeId,
                                                                ViReal64 value)
{
    return Ivi_SetAttributeViReal64 (vi, VI_NULL, SRDS345_ATTR_OFFSET, 0, value);
}

static ViStatus _VI_FUNC srds345AttrFuncDCOffset_ReadCallback (ViSession vi,
                                                               ViSession io,
                                                               ViConstString channelName,
                                                               ViAttr attributeId,
                                                               ViReal64 *value)
{
    ViStatus error = VI_SUCCESS;

    checkErr( srds345_VerifyFuncMode (vi));
    checkErr (Ivi_GetAttributeViReal64 (vi, VI_NULL, SRDS345_ATTR_OFFSET, 0, value));

Error:
    return error;
}

/*- SRDS345_ATTR_FUNC_FREQUENCY -*/
    /*- Sine and Square Waveform -*/
static IviRangeTableEntry attrFuncSineFrequencyRangeTableEntries[] = 
    {
        {1.0e-6, 30.2e6, 0, "", 0},
        {IVI_RANGE_TABLE_LAST_ENTRY}
    };
static IviRangeTable attrFuncSineFrequencyRangeTable =
    {
        IVI_VAL_RANGED,
        VI_TRUE,
        VI_TRUE,
        VI_NULL,
        attrFuncSineFrequencyRangeTableEntries,
    };

    /*- Triangle and Ramp-up Waveform -*/
static IviRangeTableEntry attrFuncDfltFrequencyRangeTableEntries[] =
    {
        {1.0e-6, 100.0e3, 0, "", 0},
        {IVI_RANGE_TABLE_LAST_ENTRY}
    };
static IviRangeTable attrFuncDfltFrequencyRangeTable =
    {
        IVI_VAL_RANGED,
        VI_TRUE,
        VI_TRUE,
        VI_NULL,
        attrFuncDfltFrequencyRangeTableEntries,
    };

static ViStatus _VI_FUNC srds345AttrFuncFrequency_CoerceCallback (ViSession vi,
                                                                  ViConstString channelName,
                                                                  ViAttr attributeId,
                                                                  ViReal64 value,
                                                                  ViReal64 *coercedValue)
{
    return srds345_CoerceToResolution (vi, value, .001, coercedValue);
}

static ViStatus _VI_FUNC srds345AttrFuncFrequency_RangeTableCallback (ViSession vi,
                                                                      ViConstString channelName,
                                                                      ViAttr attributeId,
                                                                      IviRangeTablePtr *rangeTablePtr)    
{
    ViStatus         error = VI_SUCCESS;
    ViInt32          wfm;
    
    checkErr( Ivi_GetAttributeViInt32 (vi, channelName, SRDS345_ATTR_FUNC_WAVEFORM,
                                       0, &wfm));
    *rangeTablePtr = (wfm == SRDS345_VAL_WFM_SINE || wfm == SRDS345_VAL_WFM_SQUARE) ?
                      &attrFuncSineFrequencyRangeTable :
                      &attrFuncDfltFrequencyRangeTable;
Error:
    return error;
}

static ViStatus _VI_FUNC srds345AttrFuncFrequency_CheckCallback (ViSession vi,
                                                                 ViConstString channelName,
                                                                 ViAttr attributeId,
                                                                 ViReal64 value)
{
    ViStatus    error = VI_SUCCESS;
    ViInt32     opMode;
    
    checkErr( srds345_VerifyNotArbNoise (vi));
    
    checkErr( Ivi_GetAttributeViInt32 (vi, channelName, SRDS345_ATTR_OPERATION_MODE, 0, &opMode));
    if (opMode == SRDS345_VAL_OPERATE_BURST)
        {
        checkErr( srds345_VerifyBurstTime (vi, 0, value));
        }
    else if (opMode == SRDS345_VAL_OPERATE_SWEEP_LIN || opMode == SRDS345_VAL_OPERATE_SWEEP_LOG)
        {
        ViReal64  span, max = 0;
        
        checkErr( Ivi_GetAttributeViReal64 (vi, VI_NULL, SRDS345_ATTR_FM_INTERNAL_DEVIATION, 0, &span));
    
        if ((value < (span/2.0)) || (value > (max- span/2.0)))
            viCheckErrElab(IVI_ERROR_INVALID_VALUE, "The sum of frequency and (internal deviation/2.0) or "
                "(-internal deviation/2.0) is out of range.");
        }
    checkErr( Ivi_DefaultCheckCallbackViReal64 (vi, channelName, attributeId, value));
    
Error:
    return error;
}


static ViStatus _VI_FUNC srds345AttrFuncFrequency_WriteCallback (ViSession vi,
                                                                 ViSession io,
                                                                 ViConstString channelName,
                                                                 ViAttr attributeId,
                                                                 ViReal64 value)
{return srds345_SendFloat (vi, io, "FREQ %lf", value);}

static ViStatus _VI_FUNC srds345AttrFuncFrequency_ReadCallback (ViSession vi,
                                                                ViSession io,
                                                                ViConstString channelName,
                                                                ViAttr attributeId,
                                                                ViReal64 *value)
{
    ViStatus error = VI_SUCCESS;
    checkErr( srds345_VerifyNotArbNoise (vi));
    checkErr( srds345_ReadFloat (vi, io, "FREQ?", value));
Error:
    return error;
}

/*- SRDS345_ATTR_FUNC_START_PHASE -*/
static IviRangeTableEntry attrFuncStartPhaseRangeTableEntries[] =
    {
        {-7199.999, 7199.999, 0, "", 0},
        {IVI_RANGE_TABLE_LAST_ENTRY}
    };
static IviRangeTable attrFuncStartPhaseRangeTable =
    {
        IVI_VAL_RANGED,
        VI_TRUE,
        VI_TRUE,
        VI_NULL,
        attrFuncStartPhaseRangeTableEntries,
    };

static ViStatus _VI_FUNC srds345AttrFuncStartPhase_CoerceCallback (ViSession vi,
                                                                   ViConstString channelName,
                                                                   ViAttr attributeId,
                                                                   ViReal64 value,
                                                                   ViReal64 *coercedValue)
{
    return srds345_CoerceToResolution (vi, value, .001, coercedValue);
}

static ViStatus _VI_FUNC srds345AttrFuncStartPhase_CheckCallback (ViSession vi,
                                                                  ViConstString channelName,
                                                                  ViAttr attributeId,
                                                                  ViReal64 value)
{
    ViStatus  error = VI_SUCCESS;
    ViBoolean state;
    ViInt32   type;
    
    checkErr( srds345_VerifyNotArbNoise (vi));
    checkErr( srds345_QueryMod (vi, &state, &type));
    if (state)
    {
        if ((type != SRDS345_VAL_MOD_AM) && (type != SRDS345_VAL_MOD_BURST))
            viCheckErrElab(IVI_ERROR_INVALID_CONFIGURATION, "Cannot set Phase when FM, PM or Sweep modulation is enabled.");
    }
    checkErr( Ivi_DefaultCheckCallbackViReal64 (vi, channelName, attributeId, value));

Error:
    return error;
}

static ViStatus _VI_FUNC srds345AttrFuncStartPhase_WriteCallback (ViSession vi,
                                                                  ViSession io,
                                                                  ViConstString channelName,
                                                                  ViAttr attributeId,
                                                                  ViReal64 value)
{return srds345_SendFloat (vi, io, "PHSE %lf", value);}

static ViStatus _VI_FUNC srds345AttrFuncStartPhase_ReadCallback (ViSession vi,
                                                                 ViSession io,
                                                                 ViConstString channelName,
                                                                 ViAttr attributeId,
                                                                 ViReal64 *value)
{
    ViStatus error = VI_SUCCESS;

    checkErr( srds345_VerifyNotArbNoise (vi));
    checkErr( srds345_ReadFloat (vi, io, "PHSE?", value));
Error:
    return error;
}

/*- SRDS345_ATTR_FUNC_DUTY_CYCLE_HIGH -*/ 
static IviRangeTableEntry attrFuncDutyCycleHighRangeTableEntries[] =
    {
        {50.00, 50.00, 0, "", 0},
        {IVI_RANGE_TABLE_LAST_ENTRY}
    };
static IviRangeTable attrFuncDutyCycleHighRangeTable =
    {
        IVI_VAL_RANGED,
        VI_TRUE,
        VI_TRUE,
        VI_NULL,
        attrFuncDutyCycleHighRangeTableEntries,
    };

/*- SRDS345_ATTR_ARB_WAVEFORM_HANDLE -*/
static ViStatus _VI_FUNC srds345AttrArbWaveformHandle_CheckCallback (ViSession vi,
                                                                     ViConstString channelName,
                                                                     ViAttr attributeId,
                                                                     ViInt32 value)
{
    ViStatus  error = VI_SUCCESS;
    ViInt32   wfmVal;
    
    checkErr( srds345_VerifyArbMode (vi));
    if (value == SRDS345_VAL_ARB_WAVEFORM)
        wfmVal = SRDS345_VAL_FGEN_ARB;
    else if (value == SRDS345_VAL_ARB_NO_WAVEFORM)
        wfmVal = SRDS345_VAL_FGEN_SINE;
    else
        viCheckErr( IVI_ERROR_INVALID_VALUE);
    checkErr( Ivi_CheckAttributeViInt32 (vi, VI_NULL, SRDS345_ATTR_WAVEFORM, 
                                         0, wfmVal));
Error:
    return error;
}

static ViStatus _VI_FUNC srds345AttrArbWaveformHandle_WriteCallback (ViSession vi,
                                                                     ViSession io,
                                                                     ViConstString channelName,
                                                                     ViAttr attributeId,
                                                                     ViInt32 value)
{
    ViInt32 wfmVal = (value == SRDS345_VAL_ARB_WAVEFORM) ?
                      SRDS345_VAL_FGEN_ARB :
                      SRDS345_VAL_FGEN_SINE;
    return Ivi_SetAttributeViInt32 (vi, VI_NULL, SRDS345_ATTR_WAVEFORM, 0, wfmVal);
}

static ViStatus _VI_FUNC srds345AttrArbWaveformHandle_ReadCallback (ViSession vi,
                                                                    ViSession io,
                                                                    ViConstString channelName,
                                                                    ViAttr attributeId,
                                                                    ViInt32* value)
{
    ViStatus error = VI_SUCCESS;
    ViInt32  wfmVal;
    checkErr( Ivi_GetAttributeViInt32 (vi, VI_NULL, SRDS345_ATTR_WAVEFORM, 0, &wfmVal));
    *value = (wfmVal == SRDS345_VAL_FGEN_ARB) ? 
              SRDS345_VAL_ARB_WAVEFORM : 
              SRDS345_VAL_ARB_NO_WAVEFORM;
Error:
    return error;
}

/*- SRDS345_ATTR_ARB_GAIN -*/
static ViStatus _VI_FUNC srds345AttrArbGain_CheckCallback (ViSession vi,
                                                           ViConstString channelName,
                                                           ViAttr attributeId,
                                                           ViReal64 value)
{
    ViStatus error = VI_SUCCESS;
    checkErr( srds345_VerifyArbMode (vi));
    checkErr( Ivi_CheckAttributeViReal64 (vi, VI_NULL, SRDS345_ATTR_AMPLITUDE, 0, (2.0 * value)));
 Error:
    return error;
}

static ViStatus _VI_FUNC srds345AttrArbGain_WriteCallback (ViSession vi,
                                                           ViSession io,
                                                           ViConstString channelName,
                                                           ViAttr attributeId,
                                                           ViReal64 value)
{   
    return Ivi_SetAttributeViReal64 (vi, VI_NULL, SRDS345_ATTR_AMPLITUDE, 0, (2.0 * value));
}

static ViStatus _VI_FUNC srds345AttrArbGain_ReadCallback (ViSession vi,
                                                          ViSession io,
                                                          ViConstString channelName,
                                                          ViAttr attributeId,
                                                          ViReal64 *value)
{
    ViStatus error = VI_SUCCESS;
    
    checkErr( srds345_VerifyArbMode (vi));
    checkErr (Ivi_GetAttributeViReal64 (vi, VI_NULL, SRDS345_ATTR_AMPLITUDE, 0, value));
    *value /= 2.0;
Error:
    return error;
}

/*- SRDS345_ATTR_ARB_OFFSET -*/
static ViStatus _VI_FUNC srds345AttrArbOffset_CheckCallback (ViSession vi,
                                                             ViConstString channelName,
                                                             ViAttr attributeId,
                                                             ViReal64 value)
{
    ViStatus error = VI_SUCCESS;
    checkErr( srds345_VerifyArbMode (vi));
    checkErr( Ivi_CheckAttributeViReal64 (vi, VI_NULL, SRDS345_ATTR_OFFSET, 0, value));
Error:
    return error;
}

static ViStatus _VI_FUNC srds345AttrArbOffset_ReadCallback (ViSession vi,
                                                            ViSession io,
                                                            ViConstString channelName,
                                                            ViAttr attributeId,
                                                            ViReal64 *value)
{
    ViStatus error = VI_SUCCESS;
    
    checkErr( srds345_VerifyArbMode (vi));
    checkErr (Ivi_GetAttributeViReal64 (vi, VI_NULL, SRDS345_ATTR_OFFSET, 0, value));
Error:
    return error;
}

/*- SRDS345_ATTR_ARB_SAMPLE_RATE -*/
static IviRangeTableEntry attrArbSampleRateRangeTableEntries[] =
    {
        {0.002329, 40000000.0, 0, "", 0},
        {IVI_RANGE_TABLE_LAST_ENTRY}
    };
static IviRangeTable attrArbSampleRateRangeTable =
    {
        IVI_VAL_RANGED,
        VI_TRUE,
        VI_TRUE,
        VI_NULL,
        attrArbSampleRateRangeTableEntries,
    };

static ViStatus _VI_FUNC srds345AttrArbSampleRate_CheckCallback (ViSession vi,
                                                                 ViConstString channelName,
                                                                 ViAttr attributeId,
                                                                 ViReal64 value)
{
    ViStatus    error = VI_SUCCESS;
    ViBoolean   burstOn;
    
    checkErr( srds345_VerifyArbMode (vi));
    checkErr( Ivi_DefaultCheckCallbackViReal64 (vi, VI_NULL, attributeId, value));
    checkErr( srds345_ModEnabled (vi, SRDS345_VAL_MOD_BURST, &burstOn));
    
    if (burstOn)
        {
        ViReal64 freq;
        checkErr( srds345_GetFrequency (vi, &freq));
        checkErr( srds345_VerifyBurstTime (vi, 0, freq));
        }
Error:
    return error;
}

static ViStatus _VI_FUNC srds345AttrArbSampleRate_CoerceCallback (ViSession vi,
                                                                  ViConstString channelName,
                                                                  ViAttr attributeId,
                                                                  ViReal64 value,
                                                                  ViReal64 *coercedValue)
{
    ViReal64 newValue = (40.0e6/floor(40.0e6/value + 0.5));
    ViReal64 res = (newValue < 1000000.00) ? 1.0e-6 : 1.0e-3;
    return srds345_CoerceToResolution (vi, newValue, res, coercedValue);
}

static ViStatus _VI_FUNC srds345AttrArbSampleRate_WriteCallback (ViSession vi,
                                                                 ViSession io,
                                                                 ViConstString channelName,
                                                                 ViAttr attributeId,
                                                                 ViReal64 value)
{return srds345_SendFloat (vi, io, "FSMP %Lf", value);}

static ViStatus _VI_FUNC srds345AttrArbSampleRate_ReadCallback (ViSession vi,
                                                                ViSession io,
                                                                ViConstString channelName,
                                                                ViAttr attributeId,
                                                                ViReal64 *value)
{
    ViStatus error = VI_SUCCESS;
    checkErr( srds345_VerifyArbMode (vi));
    checkErr( srds345_ReadFloat (vi, io, "FSMP?", value));
Error:
    return error;
}

/************************* Triggering ********************************************/
/*- SRDS345_ATTR_TRIGGER_SOURCE -*/
static IviRangeTableEntry attrTriggerSourceRangeTableEntries[] =
    {
        {SRDS345_VAL_SOFTWARE_TRIG,    0, 0, "", 0},
        {SRDS345_VAL_INTERNAL_TRIGGER, 0, 0, "", 1},
        {SRDS345_VAL_EXTERNAL,         0, 0, "", 2},
        {SRDS345_VAL_EXTERNAL_NEG,     0, 0, "", 3},
        {SRDS345_VAL_LINE_TRIGGER,     0, 0, "", 4},
        {IVI_RANGE_TABLE_LAST_ENTRY}
    };

static IviRangeTable attrTriggerSourceRangeTable =
    {
        IVI_VAL_DISCRETE,
        VI_TRUE,
        VI_TRUE,
        VI_NULL,
        attrTriggerSourceRangeTableEntries,
    };

static ViStatus _VI_FUNC srds345AttrTriggerSource_WriteCallback (ViSession vi,
                                                                 ViSession io,
                                                                 ViConstString channelName,
                                                                 ViAttr attributeId,
                                                                 ViInt32 value)
{
    ViStatus    error = VI_SUCCESS;
    ViInt32     cmdValue;

    viCheckErr( Ivi_GetViInt32EntryFromValue( value, &attrTriggerSourceRangeTable,
                                              VI_NULL, VI_NULL, VI_NULL, VI_NULL,
                                              VI_NULL, &cmdValue));
    viCheckErr( viPrintf (io, "TSRC%ld", cmdValue));

Error:
    return error;
}

static ViStatus _VI_FUNC srds345AttrTriggerSource_ReadCallback (ViSession vi,
                                                                ViSession io,
                                                                ViConstString channelName,
                                                                ViAttr attributeId,
                                                                ViInt32 *value)
{
    ViStatus    error = VI_SUCCESS;
    ViInt32     rdValue;

    viCheckErr( viQueryf (io, "TSRC?", "%ld", &rdValue));
    viCheckErr( Ivi_GetViInt32EntryFromCmdValue( rdValue, &attrTriggerSourceRangeTable,
                                                 value, VI_NULL, VI_NULL, VI_NULL, VI_NULL));
Error:
    return error;
}

/*- SRDS345_ATTR_INTERNAL_TRIGGER_RATE -*/
static IviRangeTableEntry attrInternalTriggerRateRangeTableEntries[] =
    {
        {0.001, 10000.0, 0, "", 0},
        {IVI_RANGE_TABLE_LAST_ENTRY}
    };

static IviRangeTable attrInternalTriggerRateRangeTable =
    {
        IVI_VAL_RANGED,
        VI_TRUE,
        VI_TRUE,
        VI_NULL,
        attrInternalTriggerRateRangeTableEntries,
    };

static ViStatus _VI_FUNC srds345AttrInternalTriggerRate_WriteCallback (ViSession vi,
                                                                       ViSession io,
                                                                       ViConstString channelName,
                                                                       ViAttr attributeId,
                                                                       ViReal64 value)
{return srds345_SendFloat (vi, io, "TRAT%lf", value);}

static ViStatus _VI_FUNC srds345AttrInternalTriggerRate_ReadCallback (ViSession vi,
                                                                      ViSession io,
                                                                      ViConstString channelName,
                                                                      ViAttr attributeId,
                                                                      ViReal64 *value)
{return srds345_ReadFloat (vi, io, "TRAT?", value);}

/*- SRDS345_ATTR_BURST_COUNT -*/
static IviRangeTableEntry attrBurstCountRangeTableEntries[] =
    {
        {1, 30000, 0, "", 0},
        {IVI_RANGE_TABLE_LAST_ENTRY}
    };

static IviRangeTable attrBurstCountRangeTable =
    {
        IVI_VAL_RANGED,
        VI_TRUE,
        VI_TRUE,
        VI_NULL,
        attrBurstCountRangeTableEntries,
    };

static ViStatus _VI_FUNC srds345AttrBurstCount_CheckCallback (ViSession vi,
                                                              ViConstString channelName,
                                                              ViAttr attributeId,
                                                              ViInt32 value)
{
    ViStatus    error = VI_SUCCESS;
    ViReal64    freq;

    checkErr( srds345_VerifyNotNoise (vi));
    checkErr( Ivi_DefaultCheckCallbackViInt32 (vi, channelName, attributeId, value));
    checkErr( srds345_GetFrequency (vi, &freq));
    checkErr( srds345_VerifyBurstTime (vi, value, freq));

Error:
    return error;
}

static ViStatus _VI_FUNC srds345AttrBurstCount_ReadCallback (ViSession vi,
                                                             ViSession io,
                                                             ViConstString channelName,
                                                             ViAttr attributeId,
                                                             ViInt32 *value)
{
    ViStatus    error = VI_SUCCESS;
    checkErr( srds345_VerifyNotNoise (vi));
    viCheckErr( viQueryf (io, "BCNT?", "%ld", value));
Error:
    return error;
}

static ViStatus _VI_FUNC srds345AttrBurstCount_WriteCallback (ViSession vi,
                                                              ViSession io,
                                                              ViConstString channelName,
                                                              ViAttr attributeId,
                                                              ViInt32 value)
{
    ViStatus    error = VI_SUCCESS;
    viCheckErr( viPrintf (io, "BCNT%ld", value));
Error:
    return error;
}

/*********************************** Amplitude Modulation ********************************************/
/*- SRDS345_ATTR_AM_ENABLED -*/
static ViStatus _VI_FUNC srds345AttrAMEnabled_CheckCallback (ViSession vi,
                                                             ViConstString channelName,
                                                             ViAttr attributeId,
                                                             ViBoolean value)
{
    ViStatus    error = VI_SUCCESS;
    
    checkErr( srds345_VerifyContinuous (vi));
    checkErr( srds345_VerifyOtherModDisabled (vi, SRDS345_VAL_MOD_AM));
    if (value) 
        {
        checkErr( srds345_VerifyNotNoise (vi));
        }
Error:
    return error;
}

static ViStatus _VI_FUNC srds345AttrAMEnabled_WriteCallback (ViSession vi,
                                                             ViSession io,
                                                             ViConstString channelName,
                                                             ViAttr attributeId,
                                                             ViBoolean value)
{
    return srds345_EnableMod (vi, SRDS345_VAL_MOD_AM, value);
}

static ViStatus _VI_FUNC srds345AttrAMEnabled_ReadCallback (ViSession vi,
                                                            ViSession io,
                                                            ViConstString channelName,
                                                            ViAttr attributeId,
                                                            ViBoolean *value)
{
    return srds345_ModEnabled (vi, SRDS345_VAL_MOD_AM, value);
}

/*- SRDS345_ATTR_AM_SOURCE -*/
static IviRangeTableEntry attrAmSourceRangeTableEntries[] = 
    {
        {SRDS345_VAL_AM_INTERNAL, 0, 0, "", 0},
        {IVI_RANGE_TABLE_LAST_ENTRY}
    };
static IviRangeTable attrAmSourceRangeTable = 
    {
        IVI_VAL_DISCRETE,
        VI_FALSE,
        VI_FALSE,
        VI_NULL,
        attrAmSourceRangeTableEntries,
    };

/*- SRDS345_ATTR_AM_INTERNAL_DEPTH -*/
static IviRangeTableEntry attrAmInternalDepthRangeTableEntries[] =
    {
        {0.00, 100.00, 0, "", 0},
        {IVI_RANGE_TABLE_LAST_ENTRY}
    };
static IviRangeTable attrAmInternalDepthRangeTable =
    {
        IVI_VAL_RANGED,
        VI_TRUE,
        VI_TRUE,
        VI_NULL,
        attrAmInternalDepthRangeTableEntries,
    };

static ViStatus _VI_FUNC srds345AttrAMInternalDepth_CoerceCallback (ViSession vi,
                                                                    ViConstString channelName,
                                                                    ViAttr attributeId,
                                                                    ViReal64 value,
                                                                    ViReal64 *coercedValue)
{
    *coercedValue = (ViReal64) RoundRealToNearestInteger(value);
    return VI_SUCCESS;
}

static ViStatus _VI_FUNC srds345AttrAMInternalDepth_WriteCallback (ViSession vi,
                                                                   ViSession io,
                                                                   ViConstString channelName,
                                                                   ViAttr attributeId,
                                                                   ViReal64 value)
{
    ViStatus error = VI_SUCCESS;
    
    viCheckErr( viPrintf (io, "DPTH %Ld", (ViInt32) value));
Error:
    return error;
}

static ViStatus _VI_FUNC srds345AttrAMInternalDepth_ReadCallback (ViSession vi,
                                                                  ViSession io,
                                                                  ViConstString channelName,
                                                                  ViAttr attributeId,
                                                                  ViReal64 *value)
{return srds345_ReadFloat (vi, io, "DPTH?", value);}

/*- SRDS345_ATTR_AM_INTERNAL_WAVEFORM -*/
static IviRangeTableEntry attrAmInternalWaveformRangeTableEntries[] =
    {
        {SRDS345_VAL_AM_INTERNAL_SINE,     0, 0, "", SRDS345_VAL_MOD_SINE},
        {SRDS345_VAL_AM_INTERNAL_SQUARE,   0, 0, "", SRDS345_VAL_MOD_SQUARE},
        {SRDS345_VAL_AM_INTERNAL_TRIANGLE, 0, 0, "", SRDS345_VAL_MOD_TRIANGLE},
        {SRDS345_VAL_AM_INTERNAL_RAMP_UP,  0, 0, "", SRDS345_VAL_MOD_RAMP_UP},
        {IVI_RANGE_TABLE_LAST_ENTRY}
    };
static IviRangeTable attrAmInternalWaveformRangeTable =
    {
        IVI_VAL_DISCRETE,
        VI_FALSE,
        VI_FALSE,
        VI_NULL,
        attrAmInternalWaveformRangeTableEntries,
    };

/*********************************** Frequency Modulation ******************************************/
/*- SRDS345_ATTR_FM_ENABLED -*/
static ViStatus _VI_FUNC srds345AttrFMEnabled_CheckCallback (ViSession vi,
                                                             ViConstString channelName,
                                                             ViAttr attributeId,
                                                             ViBoolean value)
{
    ViStatus    error = VI_SUCCESS;
    
    checkErr( srds345_VerifyOtherModDisabled (vi, SRDS345_VAL_MOD_FM));
    checkErr( Ivi_CheckAttributeViInt32 (vi, VI_NULL, SRDS345_ATTR_MOD_TYPE,
                                         0, SRDS345_VAL_MOD_FM));
Error:
    return error;
}

static ViStatus _VI_FUNC srds345AttrFMEnabled_WriteCallback (ViSession vi,
                                                             ViSession io,
                                                             ViConstString channelName,
                                                             ViAttr attributeId,
                                                             ViBoolean value)
{return srds345_EnableMod (vi, SRDS345_VAL_MOD_FM, value);}

static ViStatus _VI_FUNC srds345AttrFMEnabled_ReadCallback (ViSession vi,
                                                            ViSession io,
                                                            ViConstString channelName,
                                                            ViAttr attributeId,
                                                            ViBoolean *value)
{return srds345_ModEnabled (vi, SRDS345_VAL_MOD_FM, value);}

/*- SRDS345_ATTR_FM_SOURCE -*/
static IviRangeTableEntry attrFmSourceRangeTableEntries[] = 
    {
        {SRDS345_VAL_FM_INTERNAL, 0, 0, "", 0},
        {IVI_RANGE_TABLE_LAST_ENTRY}
    };
static IviRangeTable attrFmSourceRangeTable = 
    {
        IVI_VAL_DISCRETE,
        VI_FALSE,
        VI_FALSE,
        VI_NULL,
        attrFmSourceRangeTableEntries,
    };

/*- SRDS345_ATTR_FM_INTERNAL_DEVIATION -*/
static ViStatus _VI_FUNC srds345AttrFMInternalDeviation_CoerceCallback (ViSession vi,
                                                                        ViConstString channelName,
                                                                        ViAttr attributeId,
                                                                        ViReal64 value,
                                                                        ViReal64 *coercedValue)
{
    return srds345_CoerceToResolution (vi, value, .000001, coercedValue);
}

static ViStatus _VI_FUNC srds345AttrFMInternalDeviation_CheckCallback (ViSession vi,
                                                                       ViConstString channelName,
                                                                       ViAttr attributeId,
                                                                       ViReal64 value)
{
    ViStatus    error = VI_SUCCESS;
    ViReal64    freq, max;
    
    checkErr(srds345_VerifyNotArbNoise (vi));
    
    checkErr(Ivi_GetAttributeViReal64 (vi, CHAN1, SRDS345_ATTR_FUNC_FREQUENCY, 0, &freq));
    checkErr( srds345_GetMaxFrequency (vi, &max));
    if ((freq < (value/2.0)) || (freq > (max- value/2.0)))
            viCheckErrElab(IVI_ERROR_INVALID_VALUE, "The sum of frequency and (internal deviation/2.0) or "
                                                    "(-internal deviation/2.0) is out of range.");
Error:
    return error;
}

static ViStatus _VI_FUNC srds345AttrFMInternalDeviation_WriteCallback (ViSession vi,
                                                                       ViSession io,
                                                                       ViConstString channelName,
                                                                       ViAttr attributeId,
                                                                       ViReal64 value)
{return srds345_SendFloat (vi, io, "FDEV%lf", value);}

static ViStatus _VI_FUNC srds345AttrFMInternalDeviation_ReadCallback (ViSession vi,
                                                                      ViSession io,
                                                                      ViConstString channelName,
                                                                      ViAttr attributeId,
                                                                      ViReal64 *value)
{return srds345_ReadFloat (vi, io, "FDEV?", value);}

/*- SRDS345_ATTR_FM_INTERNAL_WAVEFORM -*/
static IviRangeTableEntry attrFmInternalWaveformRangeTableEntries[] =
    {
        {SRDS345_VAL_FM_INTERNAL_SINE,     0, 0, "", SRDS345_VAL_MOD_SINE},
        {SRDS345_VAL_FM_INTERNAL_SQUARE,   0, 0, "", SRDS345_VAL_MOD_SQUARE},
        {SRDS345_VAL_FM_INTERNAL_TRIANGLE, 0, 0, "", SRDS345_VAL_MOD_TRIANGLE},
        {SRDS345_VAL_FM_INTERNAL_RAMP_UP,  0, 0, "", SRDS345_VAL_MOD_RAMP_UP},
        {IVI_RANGE_TABLE_LAST_ENTRY}
    };
static IviRangeTable attrFmInternalWaveformRangeTable =
    {
        IVI_VAL_DISCRETE,
        VI_FALSE,
        VI_FALSE,
        VI_NULL,
        attrFmInternalWaveformRangeTableEntries,
    };

/********************************** Phase Modulation **********************************************/
/*- SRDS345_ATTR_PM_ENABLED -*/
static ViStatus _VI_FUNC srds345AttrPmEnabled_CheckCallback (ViSession vi,
                                                             ViConstString channelName,
                                                             ViAttr attributeId,
                                                             ViBoolean value)
{
    ViStatus    error = VI_SUCCESS;
    
    checkErr( srds345_VerifyContinuous (vi));
    checkErr( srds345_VerifyOtherModDisabled (vi, SRDS345_VAL_MOD_PM));
    
    if (value)
        {
        checkErr( srds345_VerifyNotArbNoise (vi));
        }

Error:
    return error;
}

static ViStatus _VI_FUNC srds345AttrPmEnabled_WriteCallback (ViSession vi,
                                                             ViSession io,
                                                             ViConstString channelName,
                                                             ViAttr attributeId,
                                                             ViBoolean value)
{
    return srds345_EnableMod (vi, SRDS345_VAL_MOD_PM, value);
}

static ViStatus _VI_FUNC srds345AttrPmEnabled_ReadCallback (ViSession vi,
                                                            ViSession io,
                                                            ViConstString channelName,
                                                            ViAttr attributeId,
                                                            ViBoolean *value)
{
    return srds345_ModEnabled (vi, SRDS345_VAL_MOD_PM, value);
}

/*- SRDS345_ATTR_PM_INTERNAL_WAVEFORM -*/
static IviRangeTableEntry attrPmInternalWaveformRangeTableEntries[] =
    {
        {SRDS345_VAL_PM_INTERNAL_SINE,     0, 0, "", SRDS345_VAL_MOD_SINE},
        {SRDS345_VAL_PM_INTERNAL_SQUARE,   0, 0, "", SRDS345_VAL_MOD_SQUARE},
        {SRDS345_VAL_PM_INTERNAL_TRIANGLE, 0, 0, "", SRDS345_VAL_MOD_TRIANGLE},
        {SRDS345_VAL_PM_INTERNAL_RAMP_UP,  0, 0, "", SRDS345_VAL_MOD_RAMP_UP},
        {IVI_RANGE_TABLE_LAST_ENTRY}
    };
static IviRangeTable attrPmInternalWaveformRangeTable =
    {
        IVI_VAL_DISCRETE,
        VI_FALSE,
        VI_FALSE,
        VI_NULL,
        attrPmInternalWaveformRangeTableEntries,
    };

/*- SRDS345_ATTR_PM_INTERNAL_SPAN -*/
static IviRangeTableEntry attrPMInternalSpanRangeTableEntries[] =
    {
        {0, 7199.999, 0, "", 0},
        {IVI_RANGE_TABLE_LAST_ENTRY}
    };

static IviRangeTable attrPMInternalSpanRangeTable =
    {
        IVI_VAL_RANGED,
        VI_TRUE,
        VI_TRUE,
        VI_NULL,
        attrPMInternalSpanRangeTableEntries,
    };

static ViStatus _VI_FUNC srds345AttrPmInternalSpan_CoerceCallback (ViSession vi,
                                                                   ViConstString channelName,
                                                                   ViAttr attributeId,
                                                                   ViReal64 value,
                                                                   ViReal64 *coercedValue)
{
    return srds345_CoerceToResolution (vi, value, .001, coercedValue);
}

static ViStatus _VI_FUNC srds345AttrPmInternalSpan_ReadCallback (ViSession vi,
                                                                 ViSession io,
                                                                 ViConstString channelName,
                                                                 ViAttr attributeId,
                                                                 ViReal64 *value)
{return srds345_ReadFloat (vi, io, "PDEV?", value);}

static ViStatus _VI_FUNC srds345AttrPmInternalSpan_WriteCallback (ViSession vi,
                                                                  ViSession io,
                                                                  ViConstString channelName,
                                                                  ViAttr attributeId,
                                                                  ViReal64 value)
{return srds345_SendFloat (vi, io, "PDEV%lf", value);}

/************************************ Sweep Waveform *****************************************/
/*- SRDS345_ATTR_SWEEP_DIRECTION -*/
static IviRangeTableEntry attrSweDirectionRangeTableEntries[] =
    {
        {SRDS345_VAL_SWEEP_SINGLE, 0, 0, "", SRDS345_VAL_MOD_SING    },
        {SRDS345_VAL_SWEEP_UP,     0, 0, "", SRDS345_VAL_MOD_RAMP_UP },
        {SRDS345_VAL_SWEEP_DOWN,   0, 0, "", SRDS345_VAL_MOD_TRIANGLE},
        {IVI_RANGE_TABLE_LAST_ENTRY}
    };

static IviRangeTable attrSweDirectionRangeTable =
    {
        IVI_VAL_DISCRETE,
        VI_TRUE,
        VI_TRUE,
        VI_NULL,
        attrSweDirectionRangeTableEntries,
    };

static ViStatus _VI_FUNC srds345AttrSweepDirection_WriteCallback (ViSession vi,
                                                                  ViSession io,
                                                                  ViConstString channelName,
                                                                  ViAttr attributeId,
                                                                  ViInt32 value)
{
    ViStatus    error = VI_SUCCESS;
    ViInt32     modWaveform;

    viCheckErr( Ivi_GetViInt32EntryFromValue (value, &attrSweDirectionRangeTable, VI_NULL,
                                              VI_NULL, VI_NULL, VI_NULL, VI_NULL, &modWaveform));
    checkErr( Ivi_SetAttributeViInt32 (vi, VI_NULL, SRDS345_ATTR_MOD_WAVEFORM, 0, modWaveform));
Error:
    return error;
}

static ViStatus _VI_FUNC srds345AttrSweepDirection_ReadCallback (ViSession vi,
                                                                 ViSession io,
                                                                 ViConstString channelName,
                                                                 ViAttr attributeId,
                                                                 ViInt32 *value)
{
    ViStatus    error = VI_SUCCESS;
    ViInt32     modWaveform, opMode;
    checkErr( Ivi_GetAttributeViInt32 (vi, CHAN1, SRDS345_ATTR_OPERATION_MODE, 0, &opMode));
    if ((opMode != SRDS345_VAL_OPERATE_SWEEP_LIN) || (opMode != SRDS345_VAL_OPERATE_SWEEP_LOG))
        viCheckErrElab( IVI_ERROR_INVALID_CONFIGURATION,
                        "Cannot perform operation in current operation mode.");
    checkErr( Ivi_GetAttributeViInt32 (vi, VI_NULL, SRDS345_ATTR_MOD_WAVEFORM, 0, &modWaveform));
    viCheckErr( Ivi_GetViInt32EntryFromCmdValue (modWaveform, &attrSweDirectionRangeTable,
                                                 value, VI_NULL, VI_NULL, VI_NULL, VI_NULL));
Error:
    return error;
}

/*- SRDS345_ATTR_SWEEP_START_FREQ -*/
static ViStatus _VI_FUNC srds345AttrSweFreq_CoerceCallback (ViSession vi,
                                                            ViConstString channelName,
                                                            ViAttr attributeId,
                                                            ViReal64 value,
                                                            ViReal64 *coercedValue)
{
    ViReal64 res = (value < 1.0e+6) ? 1.0e-6 : 1.0e-3;
    return srds345_CoerceToResolution (vi, value, res, coercedValue);
}

static ViStatus _VI_FUNC srds345AttrSweFreq_CheckCallback (ViSession vi,
                                                           ViConstString channelName,
                                                           ViAttr attributeId,
                                                           ViReal64 value)
{
    ViStatus    error = VI_SUCCESS;
    ViReal64    max;
    
    checkErr( srds345_VerifyNotArbNoise (vi));
    checkErr( srds345_GetMaxFrequency (vi, &max));
    if (value < 1.0e-6 || value > max) 
        viCheckErr( IVI_ERROR_INVALID_VALUE);

Error:
    return error;
}

static ViStatus _VI_FUNC srds345AttrSweepStartFreq_ReadCallback (ViSession vi,
                                                                 ViSession io,
                                                                 ViConstString channelName,
                                                                 ViAttr attributeId,
                                                                 ViReal64 *value)
{return srds345_ReadFloat (vi, io, "STFR?", value);}

static ViStatus _VI_FUNC srds345AttrSweepStartFreq_WriteCallback (ViSession vi,
                                                                  ViSession io,
                                                                  ViConstString channelName,
                                                                  ViAttr attributeId,
                                                                  ViReal64 value)
{return srds345_SendFloat (vi, io, "STFR%lf", value);}

/*- SRDS345_ATTR_SWEEP_STOP_FREQ -*/
static ViStatus _VI_FUNC srds345AttrSweepStopFreq_ReadCallback (ViSession vi,
                                                                ViSession io,
                                                                ViConstString channelName,
                                                                ViAttr attributeId,
                                                                ViReal64 *value)
{return srds345_ReadFloat (vi, io, "SPFR?", value);}

static ViStatus _VI_FUNC srds345AttrSweepStopFreq_WriteCallback (ViSession vi,
                                                                 ViSession io,
                                                                 ViConstString channelName,
                                                                 ViAttr attributeId,
                                                                 ViReal64 value)
{return srds345_SendFloat (vi, io, "SPFR%lf", value);}

/*- SRDS345_ATTR_SWEEP_MOD_RATE -*/
static IviRangeTableEntry attrSweModRateRangeTableEntries[] =
    {
        {1e-3, 1000.00, 0, "", 0},
        {IVI_RANGE_TABLE_LAST_ENTRY}
    };

static IviRangeTable attrSweModRateRangeTable =
    {
        IVI_VAL_RANGED,
        VI_TRUE,
        VI_TRUE,
        VI_NULL,
        attrSweModRateRangeTableEntries,
    };

static ViStatus _VI_FUNC srds345AttrSweepModRate_ReadCallback (ViSession vi,
                                                               ViSession io,
                                                               ViConstString channelName,
                                                               ViAttr attributeId,
                                                               ViReal64 *value)
{
    return Ivi_GetAttributeViReal64 (vi, VI_NULL, SRDS345_ATTR_MOD_FREQ, 0, value);
}

static ViStatus _VI_FUNC srds345AttrSweepModRate_WriteCallback (ViSession vi,
                                                                ViSession io,
                                                                ViConstString channelName,
                                                                ViAttr attributeId,
                                                                ViReal64 value)
{
    return Ivi_SetAttributeViReal64 (vi, VI_NULL, SRDS345_ATTR_MOD_FREQ, 0, value);
}

/******************************************** Hidden Attributes *******************************/
/*- SRDS345_ATTR_AMPLITUDE (hidden) -*/
static IviRangeTableEntry attrAmplitudeRangeTableEntries[] =
    {
        {0.0, 10.00, 0, "", 0},
        {IVI_RANGE_TABLE_LAST_ENTRY}
    };
static IviRangeTable attrAmplitudeRangeTable =
    {
        IVI_VAL_RANGED,
        VI_TRUE,
        VI_TRUE,
        VI_NULL,
        attrAmplitudeRangeTableEntries,
    };

static ViStatus _VI_FUNC srds345AttrAmplitude_CoerceCallback (ViSession vi,
                                                              ViConstString channelName,
                                                              ViAttr attributeId,
                                                              ViReal64 value,
                                                              ViReal64 *coercedValue)
{
    ViReal64 res = (value < 1.00) ? 0.001 : 0.01;
    return srds345_CoerceToResolution(vi, value, res, coercedValue);
}

static ViStatus _VI_FUNC srds345AttrAmplitude_ReadCallback (ViSession vi,
                                                                ViSession io,
                                                                ViConstString channelName,
                                                                ViAttr attributeId,
                                                                ViReal64 *value)
{return srds345_ReadFloat (vi, io, "AMPL?", value);}

static ViStatus _VI_FUNC srds345AttrAmplitude_WriteCallback (ViSession vi,
                                                                 ViSession io,
                                                                 ViConstString channelName,
                                                                 ViAttr attributeId,
                                                                 ViReal64 value)
{
    ViStatus error = VI_SUCCESS;
    ViReal64 offs;
    
    checkErr( Ivi_GetAttributeViReal64 (vi, channelName, SRDS345_ATTR_OFFSET, 0, &offs));
    if ((value/2.0 + fabs(offs)) > 5.0 )
        {
        checkErr(Ivi_SetAttributeViReal64 (vi, channelName, SRDS345_ATTR_OFFSET, 0, 0.0));
        }
    if (!Ivi_Simulating (vi))
        {
        checkErr( srds345_SendFloat (vi, io, "AMPL %LfVP", value));
        }
Error:
    return error;
}

/*- SRDS345_ATTR_OFFSET (hidden) -*/
static ViStatus _VI_FUNC srds345AttrOffset_CoerceCallback (ViSession vi,
                                                           ViConstString channelName,
                                                           ViAttr attributeId,
                                                           ViReal64 value,
                                                           ViReal64 *coercedValue)
{
    ViStatus error = VI_SUCCESS;
    ViReal64 res = (fabs(value) < 1.00) ? 0.001 : 0.01;
    checkErr (srds345_CoerceToResolution (vi, value, res, coercedValue));

Error:
    return error;
}

static ViStatus _VI_FUNC srds345AttrOffset_CheckCallback (ViSession vi,
                                                          ViConstString channelName,
                                                          ViAttr attributeId,
                                                          ViReal64 value)
{
    ViStatus error = VI_SUCCESS;
    ViReal64 absVal = fabs(value);
    ViReal64 amp;
    
    if (absVal > 5.0) viCheckErr( IVI_ERROR_INVALID_VALUE);
    checkErr( Ivi_GetAttributeViReal64 (vi, channelName, SRDS345_ATTR_AMPLITUDE,
                                        0, &amp));
    if ((amp/2.0 + absVal) > 5.0)
        {
        viCheckErrElab( IVI_ERROR_INVALID_CONFIGURATION,
                        "Combination of amplitude and DC offset exceeds output limits.");
        }
Error:
    return error;
}

static ViStatus _VI_FUNC srds345AttrOffset_ReadCallback (ViSession vi,
                                                         ViSession io,
                                                         ViConstString channelName,
                                                         ViAttr attributeId,
                                                         ViReal64 *value)
{return srds345_ReadFloat (vi, io, "OFFS?", value);}

static ViStatus _VI_FUNC srds345AttrOffset_WriteCallback (ViSession vi,
                                                              ViSession io,
                                                              ViConstString channelName,
                                                              ViAttr attributeId,
                                                              ViReal64 value)
{return srds345_SendFloat (vi, io, "OFFS %Lf", value);}

/*- SRDS345_ATTR_MOD_TYPE (hidden) -*/
static ViStatus _VI_FUNC srds345AttrModType_CheckCallback (ViSession vi,
                                                           ViConstString channelName,
                                                           ViAttr attributeId,
                                                           ViInt32 value)
{
    ViStatus error = VI_SUCCESS;
    
    switch (value)
        {
        case SRDS345_VAL_MOD_BURST:
        case SRDS345_VAL_MOD_AM:
            error =  srds345_VerifyNotNoise (vi);
            break;
        case SRDS345_VAL_MOD_FM:
        case SRDS345_VAL_MOD_LIN:
        case SRDS345_VAL_MOD_LOG:
        case SRDS345_VAL_MOD_PM:
            error = srds345_VerifyNotArbNoise (vi);
            break;
        default:
            error = IVI_ERROR_INVALID_VALUE;
            break;
        }
    viCheckErr( error);        

Error:
    return error;
}

static ViStatus _VI_FUNC srds345AttrModType_ReadCallback (ViSession vi,
                                                          ViSession io,
                                                          ViConstString channelName,
                                                          ViAttr attributeId,
                                                          ViInt32 *value)
{
    ViStatus    error = VI_SUCCESS;
    viCheckErr (viQueryf (io, "MTYP?", "%ld", value));
    checkErr( Ivi_InvalidateAttribute (vi, VI_NULL, SRDS345_ATTR_MOD_WAVEFORM));

Error:
    return error;
}

static ViStatus _VI_FUNC srds345AttrModType_WriteCallback (ViSession vi,
                                                           ViSession io,
                                                           ViConstString channelName,
                                                           ViAttr attributeId,
                                                           ViInt32 value)
{
    ViStatus    error = VI_SUCCESS;

    if (value == SRDS345_VAL_MOD_BURST)
        {
        ViReal64 freq;
        ViInt32  max;
        ViInt32  count;

        checkErr( srds345_GetFrequency (vi, &freq));
        max = SRDS345_VAL_MAX_BURST_TIME * freq;

        checkErr( Ivi_GetAttributeViInt32 (vi, CHAN1, SRDS345_ATTR_BURST_COUNT,
                                           0, &count));
        if (count > max)
            {
            checkErr( Ivi_SetAttributeViInt32 (vi, CHAN1, SRDS345_ATTR_BURST_COUNT,
                                               IVI_VAL_DONT_MARK_AS_SET_BY_USER, max));
            }
        }    
    else if (value == SRDS345_VAL_MOD_LIN || value == SRDS345_VAL_MOD_LOG)
        {
        ViReal64 max;
        
        checkErr( srds345_GetMaxFrequency (vi, &max));
        checkErr( srds345_ForceFloatToMax (vi, VI_NULL, SRDS345_ATTR_SWEEP_START_FREQ, max));
        checkErr( srds345_ForceFloatToMax (vi, VI_NULL, SRDS345_ATTR_SWEEP_STOP_FREQ, max));
        checkErr( srds345_ForceFloatToMax (vi, VI_NULL, SRDS345_ATTR_MOD_FREQ, 1000.0));
        }
    else if (value == SRDS345_VAL_MOD_FM)
        {
        ViReal64  max;
        ViReal64  freq;
        ViReal64  span;
        
        checkErr( Ivi_GetAttributeViReal64 (vi, CHAN1, SRDS345_ATTR_FUNC_FREQUENCY, 0, &freq));
        checkErr( Ivi_GetAttributeViReal64 (vi, VI_NULL, SRDS345_ATTR_FM_INTERNAL_DEVIATION, 0, &span));
        checkErr( srds345_GetMaxFrequency (vi, &max));
        if ((freq < (span/2.0)) || (freq > (max- span/2.0)))
            {
            checkErr ( Ivi_SetAttributeViReal64 (vi, VI_NULL, SRDS345_ATTR_FM_INTERNAL_DEVIATION, 0, 0.0));
            }
            
        }
    viCheckErr (viPrintf (io, "MTYP%ld", value));
Error:
    return error;
}

/*- SRDS345_ATTR_MOD_STATE (hidden) -*/
static ViStatus _VI_FUNC srds345AttrModState_ReadCallback (ViSession vi,
                                                           ViSession io,
                                                           ViConstString channelName,
                                                           ViAttr attributeId,
                                                           ViBoolean *value)
{
    ViStatus    error = VI_SUCCESS;
    viCheckErr (viQueryf (io, "MENA?", "%hd", value));
Error:
    return error;
}

static ViStatus _VI_FUNC srds345AttrModState_WriteCallback (ViSession vi,
                                                            ViSession io,
                                                            ViConstString channelName,
                                                            ViAttr attributeId,
                                                            ViBoolean value)
{
    ViStatus    error = VI_SUCCESS;
    viCheckErr (viPrintf (io, "MENA%hd", (value != VI_FALSE)));
Error:
    return error;
}

/*- SRDS345_ATTR_MOD_WAVEFORM (hidden) -*/
static IviRangeTableEntry attrModWaveformRangeTableEntries[] =
    {
        {SRDS345_VAL_MOD_SING,      0, 0, "", 0},
        {SRDS345_VAL_MOD_RAMP_UP,   0, 0, "", 0},
        {SRDS345_VAL_MOD_TRIANGLE,  0, 0, "", 0},
        {SRDS345_VAL_MOD_SINE,      0, 0, "", 0},
        {SRDS345_VAL_MOD_SQUARE,    0, 0, "", 0},
        {SRDS345_VAL_MOD_ARB,       0, 0, "", 0},
        {SRDS345_VAL_MOD_NONE,      0, 0, "", 0},
        {IVI_RANGE_TABLE_LAST_ENTRY}
    };

static IviRangeTable attrModWaveformRangeTable =
    {
        IVI_VAL_DISCRETE,
        VI_TRUE,
        VI_TRUE,
        VI_NULL,
        attrModWaveformRangeTableEntries,
    };

static ViStatus _VI_FUNC srds345AttrModWaveform_ReadCallback (ViSession vi,
                                                              ViSession io,
                                                              ViConstString channelName,
                                                              ViAttr attributeId,
                                                              ViInt32 *value)
{
    ViStatus    error = VI_SUCCESS;
    viCheckErr (viQueryf (io, "MDWF?", "%ld", value));
Error:
    return error;
}

static ViStatus _VI_FUNC srds345AttrModWaveform_WriteCallback (ViSession vi,
                                                               ViSession io,
                                                               ViConstString channelName,
                                                               ViAttr attributeId,
                                                               ViInt32 value)
{
    ViStatus    error = VI_SUCCESS;
    viCheckErr (viPrintf (io, "MDWF%ld", value));
Error:
    return error;
}

/*- SRDS345_ATTR_MOD_FREQ (hidden) -*/
static IviRangeTableEntry attrModFreqRangeTableEntries[] =
    {
        {0.001, 10000.0, 0, "", 0},
        {IVI_RANGE_TABLE_LAST_ENTRY}
    };
static IviRangeTable attrModFreqRangeTable =
    {
        IVI_VAL_RANGED,
        VI_TRUE,
        VI_TRUE,
        VI_NULL,
        attrModFreqRangeTableEntries,
    };

static ViStatus _VI_FUNC srds345AttrModFreq_ReadCallback (ViSession vi,
                                                          ViSession io,
                                                          ViConstString channelName,
                                                          ViAttr attributeId,
                                                          ViReal64 *value)
{return srds345_ReadFloat (vi, io, "RATE?", value);}

static ViStatus _VI_FUNC srds345AttrModFreq_WriteCallback (ViSession vi,
                                                           ViSession io,
                                                           ViConstString channelName,
                                                           ViAttr attributeId,
                                                           ViReal64 value)
{return srds345_SendFloat (vi, io, "RATE%Lf", value);}

/*- SRDS345_ATTR_ARB_MOD_RATE -*/
static IviRangeTableEntry attrArbModRateRangeTableEntries[] =
    {
        {1, 8388607, 0, "", 0},
        {IVI_RANGE_TABLE_LAST_ENTRY}
    };

static IviRangeTable attrArbModRateRangeTable =
    {
        IVI_VAL_RANGED,
        VI_TRUE,
        VI_TRUE,
        VI_NULL,
        attrArbModRateRangeTableEntries,
    };

static ViStatus _VI_FUNC srds345AttrArbModRate_ReadCallback (ViSession vi,
                                                             ViSession io,
                                                             ViConstString channelName,
                                                             ViAttr attributeId,
                                                             ViInt32 *value)
{
    ViStatus    error = VI_SUCCESS;

    viCheckErr (viQueryf (io, "AMRT?", "%ld", value));
Error:
    return error;
}

static ViStatus _VI_FUNC srds345AttrArbModRate_WriteCallback (ViSession vi,
                                                              ViSession io,
                                                              ViConstString channelName,
                                                              ViAttr attributeId,
                                                              ViInt32 value)
{
    ViStatus    error = VI_SUCCESS;
    viCheckErr (viPrintf (io, "AMRT%ld", value));
Error:
    return error;
}

/*- SRDS345_ATTR_WAVEFORM (hidden) -*/
static IviRangeTableEntry AttrFunctionRangeTableEntries[] =
    {
        {SRDS345_VAL_FGEN_SINE,      0, 0, "", 0},
        {SRDS345_VAL_FGEN_SQUARE,    0, 0, "", 0},
        {SRDS345_VAL_FGEN_TRIANGLE,  0, 0, "", 0},
        {SRDS345_VAL_FGEN_RAMP_UP,   0, 0, "", 0},
        {SRDS345_VAL_FGEN_NOISE,     0, 0, "", 0},
        {SRDS345_VAL_FGEN_ARB,       0, 0, "", 0},
        {IVI_RANGE_TABLE_LAST_ENTRY}
    };

static IviRangeTable AttrFunctionRangeTable =
    {
        IVI_VAL_DISCRETE,
        VI_TRUE,
        VI_TRUE,
        VI_NULL,
        AttrFunctionRangeTableEntries,
    };

static ViStatus _VI_FUNC srds345AttrFunction_CheckCallback (ViSession vi,
                                                                ViConstString channelName,
                                                                ViAttr attributeId,
                                                                ViInt32 value)
{
    ViStatus  error = VI_SUCCESS;
    ViBoolean state;
    ViInt32   type;
    
    checkErr( srds345_QueryMod (vi, &state, &type));
    if (state)
    {
        switch (value) 
        {
        case SRDS345_VAL_FGEN_NOISE:
            viCheckErrElab(IVI_ERROR_INVALID_CONFIGURATION, "You cannot select noise waveform,"
                                                            " when modulation is enabled.");
            break;
        case SRDS345_VAL_FGEN_TRIANGLE:
        case SRDS345_VAL_FGEN_RAMP_UP:
            if (type == SRDS345_VAL_MOD_LIN || type == SRDS345_VAL_MOD_LOG)
                {
                ViReal64 start, stop;
            
                checkErr( Ivi_GetAttributeViReal64 (vi, VI_NULL, SRDS345_ATTR_SWEEP_START_FREQ, 0, &start));
                checkErr( Ivi_GetAttributeViReal64 (vi, VI_NULL, SRDS345_ATTR_SWEEP_STOP_FREQ,  0, &stop));

                if ((start > SRDS345_VAL_MAX_FREQ_TRIANGLE) || (stop > SRDS345_VAL_MAX_FREQ_TRIANGLE))
                    viCheckErrElab(IVI_ERROR_INVALID_CONFIGURATION,"You cannot enable sweep modulation, when start or"
                                                                   " stop frequency is greater than 100 kHz.");
                }
            break;
        case SRDS345_VAL_FGEN_ARB:
            {
            ViBoolean exists;
            checkErr( Ivi_GetAttributeViBoolean (vi, VI_NULL, SRDS345_ATTR_WFM_EXISTS,0, &exists));
            if (!exists)
                viCheckErr(SRDS345_ERROR_INVALID_WAVEFORM);
            
            if (type == SRDS345_VAL_MOD_FM || type == SRDS345_VAL_MOD_PM ||
                type == SRDS345_VAL_MOD_LIN || type == SRDS345_VAL_MOD_LOG)
                viCheckErrElab(IVI_ERROR_INVALID_CONFIGURATION, "You cannot select arbitrary waveform,"
                                                                " when FM,PM or Sweep modulation is enabled.");
            }
        default:
            break;
        }
    }
Error:
    return error;
}

static ViStatus _VI_FUNC srds345AttrFunction_WriteCallback (ViSession vi,
                                                                ViSession io,
                                                                ViConstString channelName,
                                                                ViAttr attributeId,
                                                                ViInt32 value)
{
    ViStatus    error = VI_SUCCESS;
    ViBoolean state;
    ViInt32   type;
    ViReal64  max;
    
    checkErr( srds345_QueryMod (vi, &state, &type));
    if (state)
    {
        switch (value) 
        {
        case SRDS345_VAL_FGEN_TRIANGLE:
        case SRDS345_VAL_FGEN_RAMP_UP:
            if (type == SRDS345_VAL_MOD_LIN || type == SRDS345_VAL_MOD_LOG)
                {
                ViReal64 max = SRDS345_VAL_MAX_FREQ_TRIANGLE;
                checkErr( srds345_ForceFloatToMax (vi, VI_NULL, SRDS345_ATTR_SWEEP_START_FREQ, max));
                checkErr( srds345_ForceFloatToMax (vi, VI_NULL, SRDS345_ATTR_SWEEP_STOP_FREQ, max));
                }
            break;
        case SRDS345_VAL_FGEN_ARB:
            {
            if (type != SRDS345_VAL_MOD_BURST && type != SRDS345_VAL_MOD_AM)
                viCheckErrElab(IVI_ERROR_INVALID_CONFIGURATION, "You cannot select arbitrary waveform,"
                                                                " when FM,PM or Sweep modulation is enabled.");
            }
        case SRDS345_VAL_FGEN_NOISE:
            checkErr( srds345_EnableMod (vi, type, VI_FALSE));
            break;
        default:
            break;
        }
    }
    checkErr( srds345_GetMaxFrequency (vi, &max));
    checkErr( srds345_ForceFloatToMax (vi, VI_NULL, SRDS345_ATTR_FUNC_FREQUENCY, max));
    
    viCheckErr( viPrintf (io, "FUNC%ld", value));
Error:
    return error;
}

static ViStatus _VI_FUNC srds345AttrFunction_ReadCallback (ViSession vi,
                                                               ViSession io,
                                                               ViConstString channelName,
                                                               ViAttr attributeId,
                                                               ViInt32 *value)
{
    ViStatus    error = VI_SUCCESS;
    viCheckErr( viQueryf (io, "FUNC?", "%ld", value));
Error:
    return error;
}

/*****************************************************************************
 * Function: srds345_InitAttributes                                                  
 * Purpose:  This function adds attributes to the IVI session, initializes   
 *           instrument attributes, and sets attribute invalidation          
 *           dependencies.                                                   
 *****************************************************************************/
static ViStatus srds345_InitAttributes (ViSession vi)
{
    ViStatus    error = VI_SUCCESS;
        
        /*- Initialize instrument attributes --------------------------------*/            

    /*  These attributes are created with use-callbacks-for-simulation flag by  */
    /*  the IVI engine, so make sure you do not perform any kind of I/O in the  */
    /*  callbacks. If you have to, then change the flag on the attribute using  */
    /*  Ivi_SetAttributeFlag.                                                   */

    checkErr( Ivi_SetAttributeViInt32 (vi, VI_NULL, SRDS345_ATTR_SPECIFIC_DRIVER_MAJOR_VERSION,
                                       0, SRDS345_MAJOR_VERSION));
    checkErr( Ivi_SetAttributeViInt32 (vi, VI_NULL, SRDS345_ATTR_SPECIFIC_DRIVER_MINOR_VERSION,
                                       0, SRDS345_MINOR_VERSION));
    checkErr( Ivi_SetAttrReadCallbackViString (vi, SRDS345_ATTR_SPECIFIC_DRIVER_REVISION,
                                               srds345AttrSpecificDriverRevision_ReadCallback));
    checkErr( Ivi_SetAttributeViInt32 (vi, VI_NULL, SRDS345_ATTR_SPECIFIC_DRIVER_CLASS_SPEC_MAJOR_VERSION,
                                       0, SRDS345_CLASS_SPEC_MAJOR_VERSION));
    checkErr( Ivi_SetAttributeViInt32 (vi, VI_NULL, SRDS345_ATTR_SPECIFIC_DRIVER_CLASS_SPEC_MINOR_VERSION,
                                       0, SRDS345_CLASS_SPEC_MINOR_VERSION));
    checkErr( Ivi_SetAttributeViString (vi, VI_NULL, SRDS345_ATTR_IO_SESSION_TYPE,
                                        0, SRDS345_IO_SESSION_TYPE));
    checkErr( Ivi_SetAttributeViString (vi, VI_NULL, SRDS345_ATTR_SUPPORTED_INSTRUMENT_MODELS,
                                        0, SRDS345_SUPPORTED_INSTRUMENT_MODELS));
    checkErr( Ivi_SetAttributeFlags (vi, SRDS345_ATTR_INSTRUMENT_FIRMWARE_REVISION,
                                     IVI_VAL_USE_CALLBACKS_FOR_SIMULATION));
    checkErr( Ivi_SetAttributeFlags (vi, SRDS345_ATTR_INSTRUMENT_MANUFACTURER,
                                     IVI_VAL_USE_CALLBACKS_FOR_SIMULATION));
    checkErr( Ivi_SetAttributeFlags (vi, SRDS345_ATTR_INSTRUMENT_MODEL,
                                     IVI_VAL_USE_CALLBACKS_FOR_SIMULATION));
    checkErr( Ivi_SetAttrReadCallbackViString (vi, SRDS345_ATTR_INSTRUMENT_FIRMWARE_REVISION,
                                               srds345AttrFirmwareRevision_ReadCallback));
    checkErr( Ivi_SetAttrReadCallbackViString (vi, SRDS345_ATTR_INSTRUMENT_MANUFACTURER,
                                               srds345AttrInstrumentManufacturer_ReadCallback));
    checkErr( Ivi_SetAttrReadCallbackViString (vi, SRDS345_ATTR_INSTRUMENT_MODEL,
                                               srds345AttrInstrumentModel_ReadCallback));
    checkErr( Ivi_SetAttributeViString (vi, VI_NULL, SRDS345_ATTR_SPECIFIC_DRIVER_VENDOR,
                                        0, SRDS345_DRIVER_VENDOR));
    checkErr( Ivi_SetAttributeViString (vi, VI_NULL, SRDS345_ATTR_SPECIFIC_DRIVER_DESCRIPTION,
                                        0, SRDS345_DRIVER_DESCRIPTION));
                                  

    checkErr( Ivi_SetAttributeViAddr (vi, VI_NULL, IVI_ATTR_OPC_CALLBACK, 0, VI_NULL));
    checkErr( Ivi_SetAttributeViAddr (vi, VI_NULL, IVI_ATTR_CHECK_STATUS_CALLBACK, 0,
                                      srds345_CheckStatusCallback));
    checkErr( Ivi_SetAttributeViBoolean (vi, VI_NULL, IVI_ATTR_SUPPORTS_WR_BUF_OPER_MODE, 
                                         0, VI_TRUE));

    checkErr( Ivi_SetAttributeViString (vi, VI_NULL, SRDS345_ATTR_GROUP_CAPABILITIES, IVI_VAL_DONT_MARK_AS_SET_BY_USER,
                                    "IviFgenBase,"
                                    "IviFgenStdFunc,"
                                    "IviFgenArbWfm,"
                                    "IviFgenTrigger,"
                                    "IviFgenInternalTrigger,"
                                    "IviFgenSoftwareTrigger,"
                                    "IviFgenBurst,"
                                    "IviFgenModulateAM,"
                                    "IviFgenModulateFM"));
                                    

        /*- Add instrument-specific attributes ------------------------------*/            
/*- Basic Operation ---------------------------------------------------------*/
    checkErr (Ivi_AddAttributeViInt32 (vi, SRDS345_ATTR_OPERATION_MODE,
                                       "SRDS345_ATTR_OPERATION_MODE",
                                       SRDS345_VAL_OPERATE_CONTINUOUS,
                                       IVI_VAL_MULTI_CHANNEL | IVI_VAL_USE_CALLBACKS_FOR_SIMULATION,
                                       srds345AttrOperationMode_ReadCallback,
                                       srds345AttrOperationMode_WriteCallback,
                                       &attrOperationModeRangeTable));
    checkErr (Ivi_AddAttributeViInt32 (vi, SRDS345_ATTR_OUTPUT_MODE,
                                       "SRDS345_ATTR_OUTPUT_MODE",
                                       SRDS345_VAL_OUTPUT_FUNC,
                                       IVI_VAL_USE_CALLBACKS_FOR_SIMULATION, VI_NULL, 
                                       srds345AttrOutputMode_WriteCallback,
                                       &attrOutputModeRangeTable));
    checkErr (Ivi_SetAttrCheckCallbackViInt32 (vi, SRDS345_ATTR_OUTPUT_MODE,
                                               srds345AttrOutputMode_CheckCallback));
    checkErr (Ivi_AddAttributeViInt32 (vi, SRDS345_ATTR_REF_CLOCK_SOURCE,
                                       "SRDS345_ATTR_REF_CLOCK_SOURCE",
                                       SRDS345_VAL_REF_CLOCK_INTERNAL, 0, VI_NULL, VI_NULL,
                                       &attrRefClockSourceRangeTable));
    
    checkErr (Ivi_AddAttributeViBoolean (vi, SRDS345_ATTR_OUTPUT_ENABLED,
                                         "SRDS345_ATTR_OUTPUT_ENABLED", VI_TRUE,
                                         IVI_VAL_MULTI_CHANNEL, VI_NULL, VI_NULL));
    checkErr (Ivi_SetAttrCheckCallbackViBoolean (vi, SRDS345_ATTR_OUTPUT_ENABLED,
                                                 srds345AttrOutputEnabled_CheckCallback));
    
    checkErr (Ivi_AddAttributeViReal64 (vi, SRDS345_ATTR_OUTPUT_IMPEDANCE,
                                        "SRDS345_ATTR_OUTPUT_IMPEDANCE", 50.00,
                                        IVI_VAL_MULTI_CHANNEL, VI_NULL, VI_NULL,
                                        &attrOutputImpedanceRangeTable, 0));

/*- Standard Function Output ---------------------------------------------------------*/
    checkErr (Ivi_AddAttributeViInt32 (vi, SRDS345_ATTR_FUNC_WAVEFORM,
                                       "SRDS345_ATTR_FUNC_WAVEFORM",
                                       SRDS345_VAL_WFM_SINE,
                                       IVI_VAL_MULTI_CHANNEL | IVI_VAL_NEVER_CACHE | IVI_VAL_USE_CALLBACKS_FOR_SIMULATION | IVI_VAL_DONT_CHECK_STATUS,
                                       srds345AttrFuncWaveform_ReadCallback,
                                       srds345AttrFuncWaveform_WriteCallback,
                                       &attrFuncWaveformRangeTable));
    checkErr( Ivi_SetAttrCheckCallbackViInt32 (vi, SRDS345_ATTR_FUNC_WAVEFORM,
                                               srds345AttrFuncWaveform_CheckCallback));
    
    checkErr (Ivi_AddAttributeViReal64 (vi, SRDS345_ATTR_FUNC_AMPLITUDE,
                                        "SRDS345_ATTR_FUNC_AMPLITUDE", 2.00,
                                        IVI_VAL_MULTI_CHANNEL | IVI_VAL_NEVER_CACHE | IVI_VAL_USE_CALLBACKS_FOR_SIMULATION | IVI_VAL_DONT_CHECK_STATUS,
                                        srds345AttrFuncAmplitude_ReadCallback,
                                        srds345AttrFuncAmplitude_WriteCallback,
                                        VI_NULL, 0));
    checkErr( Ivi_SetAttrCheckCallbackViReal64 (vi, SRDS345_ATTR_FUNC_AMPLITUDE,
                                                srds345AttrFuncAmplitude_CheckCallback));
    
    checkErr (Ivi_AddAttributeViReal64 (vi, SRDS345_ATTR_FUNC_DC_OFFSET,
                                        "SRDS345_ATTR_FUNC_DC_OFFSET", 0.00,
                                        IVI_VAL_MULTI_CHANNEL | IVI_VAL_NEVER_CACHE | IVI_VAL_USE_CALLBACKS_FOR_SIMULATION | IVI_VAL_DONT_CHECK_STATUS,
                                        srds345AttrFuncDCOffset_ReadCallback,
                                        srds345AttrFuncDCOffset_WriteCallback,
                                        VI_NULL, 0));
    checkErr( Ivi_SetAttrCheckCallbackViReal64 (vi, SRDS345_ATTR_FUNC_DC_OFFSET,
                                                srds345AttrFuncDCOffset_CheckCallback));
    
    checkErr( Ivi_AddAttributeViReal64 (vi, SRDS345_ATTR_FUNC_FREQUENCY,
                                        "SRDS345_ATTR_FUNC_FREQUENCY",
                                        1000.00, IVI_VAL_MULTI_CHANNEL,
                                        srds345AttrFuncFrequency_ReadCallback,
                                        srds345AttrFuncFrequency_WriteCallback,
                                        VI_NULL, 0));
    checkErr( Ivi_SetAttrCheckCallbackViReal64 (vi, SRDS345_ATTR_FUNC_FREQUENCY,
                                                srds345AttrFuncFrequency_CheckCallback));
    checkErr( Ivi_SetAttrRangeTableCallback (vi, SRDS345_ATTR_FUNC_FREQUENCY,
                                                     srds345AttrFuncFrequency_RangeTableCallback));
    checkErr( Ivi_SetAttrCoerceCallbackViReal64 (vi, SRDS345_ATTR_FUNC_FREQUENCY, 
                                                     srds345AttrFuncFrequency_CoerceCallback));
    
    checkErr( Ivi_AddAttributeViReal64 (vi, SRDS345_ATTR_FUNC_START_PHASE,
                                        "SRDS345_ATTR_FUNC_START_PHASE",
                                        0.00, IVI_VAL_MULTI_CHANNEL,
                                        srds345AttrFuncStartPhase_ReadCallback,
                                        srds345AttrFuncStartPhase_WriteCallback,
                                        &attrFuncStartPhaseRangeTable, 0));
    checkErr( Ivi_SetAttrCheckCallbackViReal64 (vi, SRDS345_ATTR_FUNC_START_PHASE,
                                                srds345AttrFuncStartPhase_CheckCallback));
    checkErr( Ivi_SetAttrCoerceCallbackViReal64 (vi, SRDS345_ATTR_FUNC_START_PHASE, 
                                                srds345AttrFuncStartPhase_CoerceCallback)); 

    checkErr( Ivi_AddAttributeViReal64 (vi, SRDS345_ATTR_FUNC_DUTY_CYCLE_HIGH,
                                        "SRDS345_ATTR_FUNC_DUTY_CYCLE_HIGH",
                                        50.00, IVI_VAL_MULTI_CHANNEL,
                                        VI_NULL, VI_NULL,
                                        &attrFuncDutyCycleHighRangeTable, 0));

/*- Arbitrary Waveform Output ---------------------------------------------------------*/
    checkErr (Ivi_AddAttributeViInt32 (vi, SRDS345_ATTR_ARB_WAVEFORM_HANDLE,
                                       "SRDS345_ATTR_ARB_WAVEFORM_HANDLE", 0,
                                       IVI_VAL_MULTI_CHANNEL | IVI_VAL_USE_CALLBACKS_FOR_SIMULATION | IVI_VAL_NEVER_CACHE, 
                                       srds345AttrArbWaveformHandle_ReadCallback, 
                                       srds345AttrArbWaveformHandle_WriteCallback,
                                       VI_NULL));
    checkErr( Ivi_SetAttrCheckCallbackViInt32 (vi, SRDS345_ATTR_ARB_WAVEFORM_HANDLE,
                                               srds345AttrArbWaveformHandle_CheckCallback));

    checkErr (Ivi_AddAttributeViReal64 (vi, SRDS345_ATTR_ARB_GAIN,
                                        "SRDS345_ATTR_ARB_GAIN", 1.00,
                                        IVI_VAL_MULTI_CHANNEL | IVI_VAL_NEVER_CACHE | IVI_VAL_USE_CALLBACKS_FOR_SIMULATION | IVI_VAL_DONT_CHECK_STATUS,
                                        srds345AttrArbGain_ReadCallback,
                                        srds345AttrArbGain_WriteCallback,
                                        VI_NULL, 0));
    checkErr( Ivi_SetAttrCheckCallbackViReal64 (vi, SRDS345_ATTR_ARB_GAIN,
                                                srds345AttrArbGain_CheckCallback));
    
    checkErr (Ivi_AddAttributeViReal64 (vi, SRDS345_ATTR_ARB_OFFSET,
                                        "SRDS345_ATTR_ARB_OFFSET", 0.00,
                                        IVI_VAL_MULTI_CHANNEL | IVI_VAL_NEVER_CACHE | IVI_VAL_USE_CALLBACKS_FOR_SIMULATION | IVI_VAL_DONT_CHECK_STATUS,
                                        srds345AttrArbOffset_ReadCallback,
                                        srds345AttrFuncDCOffset_WriteCallback,
                                        VI_NULL, 0));
    checkErr( Ivi_SetAttrCheckCallbackViReal64 (vi, SRDS345_ATTR_ARB_OFFSET,
                                                srds345AttrArbOffset_CheckCallback));
    
    checkErr( Ivi_AddAttributeViReal64 (vi, SRDS345_ATTR_ARB_SAMPLE_RATE,
                                        "SRDS345_ATTR_ARB_SAMPLE_RATE",
                                        0.00, 0,
                                        srds345AttrArbSampleRate_ReadCallback,
                                        srds345AttrArbSampleRate_WriteCallback,
                                        &attrArbSampleRateRangeTable, 0));
    checkErr (Ivi_SetAttrCoerceCallbackViReal64 (vi, SRDS345_ATTR_ARB_SAMPLE_RATE,
                                                 srds345AttrArbSampleRate_CoerceCallback));
    checkErr( Ivi_SetAttrCheckCallbackViReal64 (vi, SRDS345_ATTR_ARB_SAMPLE_RATE,
                                                srds345AttrArbSampleRate_CheckCallback));

    checkErr (Ivi_AddAttributeViInt32 (vi, SRDS345_ATTR_MAX_NUM_WAVEFORMS,
                                       "SRDS345_ATTR_MAX_NUM_WAVEFORMS", 1,
                                       IVI_VAL_NOT_USER_WRITABLE,
                                       VI_NULL, VI_NULL, VI_NULL));
    checkErr (Ivi_AddAttributeViInt32 (vi, SRDS345_ATTR_WAVEFORM_QUANTUM,
                                       "SRDS345_ATTR_WAVEFORM_QUANTUM", 1,
                                       IVI_VAL_NOT_USER_WRITABLE,
                                       VI_NULL, VI_NULL, VI_NULL));
    checkErr (Ivi_AddAttributeViInt32 (vi, SRDS345_ATTR_MIN_WAVEFORM_SIZE,
                                       "SRDS345_ATTR_MIN_WAVEFORM_SIZE", SRDS345_VAL_MIN_WFM_SIZE,
                                       IVI_VAL_NOT_USER_WRITABLE,
                                       VI_NULL, VI_NULL, VI_NULL));
    checkErr (Ivi_AddAttributeViInt32 (vi, SRDS345_ATTR_MAX_WAVEFORM_SIZE,
                                       "SRDS345_ATTR_MAX_WAVEFORM_SIZE", SRDS345_VAL_MAX_WFM_SIZE,
                                       IVI_VAL_NOT_USER_WRITABLE,
                                       VI_NULL, VI_NULL, VI_NULL));

/*- Triggering ---------------------------------------------------------------------------------*/
    checkErr (Ivi_AddAttributeViInt32 (vi, SRDS345_ATTR_TRIGGER_SOURCE,
                                       "SRDS345_ATTR_TRIGGER_SOURCE",
                                       SRDS345_VAL_INTERNAL_TRIGGER,
                                       IVI_VAL_MULTI_CHANNEL,
                                       srds345AttrTriggerSource_ReadCallback,
                                       srds345AttrTriggerSource_WriteCallback,
                                       &attrTriggerSourceRangeTable));

    checkErr (Ivi_AddAttributeViReal64 (vi, SRDS345_ATTR_INTERNAL_TRIGGER_RATE,
                                        "SRDS345_ATTR_INTERNAL_TRIGGER_RATE", 1000.0, 
                                        0,
                                        srds345AttrInternalTriggerRate_ReadCallback,
                                        srds345AttrInternalTriggerRate_WriteCallback,
                                        &attrInternalTriggerRateRangeTable, 0));
    checkErr( Ivi_SetAttrCoerceCallbackViReal64 (vi, SRDS345_ATTR_INTERNAL_TRIGGER_RATE, 
                                                     srds345AttrFrequency_CoerceCallback));
    
    checkErr (Ivi_AddAttributeViInt32 (vi, SRDS345_ATTR_BURST_COUNT,
                                       "SRDS345_ATTR_BURST_COUNT", 1,
                                       IVI_VAL_MULTI_CHANNEL,
                                       srds345AttrBurstCount_ReadCallback,
                                       srds345AttrBurstCount_WriteCallback,
                                       &attrBurstCountRangeTable));
    checkErr (Ivi_SetAttrCheckCallbackViInt32 (vi, SRDS345_ATTR_BURST_COUNT,
                                               srds345AttrBurstCount_CheckCallback));

/*- SRDS345_ATTR_ARB_MOD_RATE ---------------------------------------------------------------*/
    checkErr (Ivi_AddAttributeViInt32 (vi, SRDS345_ATTR_ARB_MOD_RATE,
                                       "SRDS345_ATTR_ARB_MOD_RATE", 1, 0,
                                       srds345AttrArbModRate_ReadCallback,
                                       srds345AttrArbModRate_WriteCallback,
                                       &attrArbModRateRangeTable));

/*- Amplitude Modulation Attributes ---------------------------------------------------------*/
    checkErr (Ivi_AddAttributeViBoolean (vi, SRDS345_ATTR_AM_ENABLED,
                                         "SRDS345_ATTR_AM_ENABLED", VI_FALSE,
                                         IVI_VAL_MULTI_CHANNEL | IVI_VAL_NEVER_CACHE | IVI_VAL_USE_CALLBACKS_FOR_SIMULATION | IVI_VAL_DONT_CHECK_STATUS,
                                         srds345AttrAMEnabled_ReadCallback,
                                         srds345AttrAMEnabled_WriteCallback));
    checkErr( Ivi_SetAttrCheckCallbackViBoolean (vi, SRDS345_ATTR_AM_ENABLED,
                                                 srds345AttrAMEnabled_CheckCallback));

    checkErr (Ivi_AddAttributeViInt32 (vi, SRDS345_ATTR_AM_SOURCE,
                                       "SRDS345_ATTR_AM_SOURCE",
                                       SRDS345_VAL_AM_INTERNAL,
                                       IVI_VAL_MULTI_CHANNEL,
                                       VI_NULL, VI_NULL, &attrAmSourceRangeTable));

    checkErr( Ivi_AddAttributeViReal64 (vi, SRDS345_ATTR_AM_INTERNAL_DEPTH,
                                        "SRDS345_ATTR_AM_INTERNAL_DEPTH",
                                        100.00, 0,
                                        srds345AttrAMInternalDepth_ReadCallback,
                                        srds345AttrAMInternalDepth_WriteCallback,
                                        &attrAmInternalDepthRangeTable, 0));
    checkErr (Ivi_SetAttrCoerceCallbackViReal64 (vi, SRDS345_ATTR_AM_INTERNAL_DEPTH,
                                                 srds345AttrAMInternalDepth_CoerceCallback));
                                                
    checkErr (Ivi_AddAttributeViInt32 (vi, SRDS345_ATTR_AM_INTERNAL_WAVEFORM,
                                       "SRDS345_ATTR_AM_INTERNAL_WAVEFORM",
                                       SRDS345_VAL_AM_INTERNAL_SINE,
                                       IVI_VAL_NEVER_CACHE | IVI_VAL_USE_CALLBACKS_FOR_SIMULATION | IVI_VAL_DONT_CHECK_STATUS,
                                       srds345AttrAMFMPMWaveform_ReadCallback,
                                       srds345AttrAMFMPMWaveform_WriteCallback,
                                       &attrAmInternalWaveformRangeTable));

    checkErr (Ivi_AddAttributeViReal64 (vi, SRDS345_ATTR_AM_INTERNAL_FREQUENCY,
                                        "SRDS345_ATTR_AM_INTERNAL_FREQUENCY",
                                        1000.00,
                                        IVI_VAL_NEVER_CACHE | IVI_VAL_USE_CALLBACKS_FOR_SIMULATION | IVI_VAL_DONT_CHECK_STATUS,
                                        srds345AttrAMFMPMSwpFreq_ReadCallback,
                                        srds345AttrAMFMPMSwpFreq_WriteCallback,
                                        &attrModFreqRangeTable, 0));
    checkErr (Ivi_SetAttrCoerceCallbackViReal64 (vi, SRDS345_ATTR_AM_INTERNAL_FREQUENCY,
                                                 srds345AttrFrequency_CoerceCallback));

/*- Frequency Modulation Attributes ---------------------------------------------------------*/
    checkErr (Ivi_AddAttributeViBoolean (vi, SRDS345_ATTR_FM_ENABLED,
                                         "SRDS345_ATTR_FM_ENABLED", VI_FALSE,
                                         IVI_VAL_MULTI_CHANNEL | IVI_VAL_NEVER_CACHE | IVI_VAL_USE_CALLBACKS_FOR_SIMULATION | IVI_VAL_DONT_CHECK_STATUS,
                                         srds345AttrFMEnabled_ReadCallback,
                                         srds345AttrFMEnabled_WriteCallback));
    checkErr( Ivi_SetAttrCheckCallbackViBoolean (vi, SRDS345_ATTR_FM_ENABLED,
                                                 srds345AttrFMEnabled_CheckCallback));

    checkErr (Ivi_AddAttributeViInt32 (vi, SRDS345_ATTR_FM_SOURCE,
                                       "SRDS345_ATTR_FM_SOURCE",
                                       SRDS345_VAL_FM_INTERNAL,
                                       IVI_VAL_MULTI_CHANNEL,
                                       VI_NULL, VI_NULL, &attrFmSourceRangeTable));

    checkErr (Ivi_AddAttributeViReal64 (vi, SRDS345_ATTR_FM_INTERNAL_DEVIATION,
                                        "SRDS345_ATTR_FM_INTERNAL_DEVIATION", 0.00,0,
                                        srds345AttrFMInternalDeviation_ReadCallback,
                                        srds345AttrFMInternalDeviation_WriteCallback,
                                        VI_NULL, 0));
    checkErr (Ivi_SetAttrCheckCallbackViReal64 (vi, SRDS345_ATTR_FM_INTERNAL_DEVIATION,
                                                srds345AttrFMInternalDeviation_CheckCallback));
    checkErr (Ivi_SetAttrCoerceCallbackViReal64 (vi, SRDS345_ATTR_FM_INTERNAL_DEVIATION,
                                                srds345AttrFMInternalDeviation_CoerceCallback));

    checkErr (Ivi_AddAttributeViInt32 (vi, SRDS345_ATTR_FM_INTERNAL_WAVEFORM,
                                       "SRDS345_ATTR_FM_INTERNAL_WAVEFORM",
                                       SRDS345_VAL_AM_INTERNAL_SINE,
                                       IVI_VAL_NEVER_CACHE | IVI_VAL_USE_CALLBACKS_FOR_SIMULATION | IVI_VAL_DONT_CHECK_STATUS,
                                       srds345AttrAMFMPMWaveform_ReadCallback,
                                       srds345AttrAMFMPMWaveform_WriteCallback,
                                       &attrFmInternalWaveformRangeTable));
    
    checkErr (Ivi_AddAttributeViReal64 (vi, SRDS345_ATTR_FM_INTERNAL_FREQUENCY,
                                        "SRDS345_ATTR_FM_INTERNAL_FREQUENCY",
                                        1000.00,
                                        IVI_VAL_NEVER_CACHE | IVI_VAL_USE_CALLBACKS_FOR_SIMULATION | IVI_VAL_DONT_CHECK_STATUS,
                                        srds345AttrAMFMPMSwpFreq_ReadCallback,
                                        srds345AttrAMFMPMSwpFreq_WriteCallback,
                                        &attrModFreqRangeTable, 0));
    checkErr (Ivi_SetAttrCoerceCallbackViReal64 (vi,SRDS345_ATTR_FM_INTERNAL_FREQUENCY,
                                                srds345AttrFrequency_CoerceCallback));
    
/*- Phase Modulation Attributes -------------------------------------------------------------------*/
    checkErr (Ivi_AddAttributeViBoolean (vi, SRDS345_ATTR_PM_ENABLED,
                                         "SRDS345_ATTR_PM_ENABLED", VI_FALSE,
                                         IVI_VAL_MULTI_CHANNEL | IVI_VAL_NEVER_CACHE | IVI_VAL_USE_CALLBACKS_FOR_SIMULATION | IVI_VAL_DONT_CHECK_STATUS,
                                         srds345AttrPmEnabled_ReadCallback,
                                         srds345AttrPmEnabled_WriteCallback));
    checkErr (Ivi_SetAttrCheckCallbackViBoolean (vi, SRDS345_ATTR_PM_ENABLED,
                                                 srds345AttrPmEnabled_CheckCallback));

    checkErr (Ivi_AddAttributeViInt32 (vi, SRDS345_ATTR_PM_INTERNAL_WAVEFORM,
                                       "SRDS345_ATTR_PM_INTERNAL_WAVEFORM",
                                       SRDS345_VAL_PM_INTERNAL_SINE,
                                       IVI_VAL_NEVER_CACHE | IVI_VAL_USE_CALLBACKS_FOR_SIMULATION | IVI_VAL_DONT_CHECK_STATUS,
                                       srds345AttrAMFMPMWaveform_ReadCallback,
                                       srds345AttrAMFMPMWaveform_WriteCallback,
                                       &attrPmInternalWaveformRangeTable));
    
    checkErr (Ivi_AddAttributeViReal64 (vi, SRDS345_ATTR_PM_INTERNAL_FREQUENCY,
                                        "SRDS345_ATTR_PM_INTERNAL_FREQUENCY", 1000.0,
                                        IVI_VAL_NEVER_CACHE | IVI_VAL_USE_CALLBACKS_FOR_SIMULATION | IVI_VAL_DONT_CHECK_STATUS,
                                        srds345AttrAMFMPMSwpFreq_ReadCallback,
                                        srds345AttrAMFMPMSwpFreq_WriteCallback,
                                        &attrModFreqRangeTable, 0));
    checkErr (Ivi_SetAttrCoerceCallbackViReal64 (vi,SRDS345_ATTR_PM_INTERNAL_FREQUENCY,
                                                 srds345AttrFrequency_CoerceCallback));
 
    checkErr (Ivi_AddAttributeViReal64 (vi, SRDS345_ATTR_PM_INTERNAL_SPAN,
                                        "SRDS345_ATTR_PM_INTERNAL_SPAN", 45.00, 0,
                                        srds345AttrPmInternalSpan_ReadCallback,
                                        srds345AttrPmInternalSpan_WriteCallback,
                                        &attrPMInternalSpanRangeTable, 0));
    checkErr (Ivi_SetAttrCoerceCallbackViReal64 (vi,SRDS345_ATTR_PM_INTERNAL_SPAN,
                                                srds345AttrPmInternalSpan_CoerceCallback));
    
/*- Sweeping --------------------------------------------------------------------------------------*/
    checkErr (Ivi_AddAttributeViInt32 (vi, SRDS345_ATTR_SWEEP_DIRECTION,
                                       "SRDS345_ATTR_SWEEP_DIRECTION",
                                       SRDS345_VAL_SWEEP_SINGLE,
                                       IVI_VAL_NEVER_CACHE | IVI_VAL_USE_CALLBACKS_FOR_SIMULATION | IVI_VAL_DONT_CHECK_STATUS,
                                       srds345AttrSweepDirection_ReadCallback,
                                       srds345AttrSweepDirection_WriteCallback,
                                       &attrSweDirectionRangeTable));

    checkErr (Ivi_AddAttributeViReal64 (vi, SRDS345_ATTR_SWEEP_START_FREQ,
                                        "SRDS345_ATTR_SWEEP_START_FREQ", 1.0e-6, 0,
                                        srds345AttrSweepStartFreq_ReadCallback,
                                        srds345AttrSweepStartFreq_WriteCallback,
                                        VI_NULL, 0));
    checkErr (Ivi_SetAttrCheckCallbackViReal64 (vi, SRDS345_ATTR_SWEEP_START_FREQ,
                                                srds345AttrSweFreq_CheckCallback));
    checkErr (Ivi_SetAttrCoerceCallbackViReal64 (vi, SRDS345_ATTR_SWEEP_START_FREQ,
                                                 srds345AttrSweFreq_CoerceCallback));
    
    checkErr (Ivi_AddAttributeViReal64 (vi, SRDS345_ATTR_SWEEP_STOP_FREQ,
                                        "SRDS345_ATTR_SWEEP_STOP_FREQ", 100.0e3, 0,
                                        srds345AttrSweepStopFreq_ReadCallback,
                                        srds345AttrSweepStopFreq_WriteCallback,
                                        VI_NULL, 0));
    checkErr (Ivi_SetAttrCheckCallbackViReal64 (vi, SRDS345_ATTR_SWEEP_STOP_FREQ,
                                                srds345AttrSweFreq_CheckCallback));
    checkErr (Ivi_SetAttrCoerceCallbackViReal64 (vi, SRDS345_ATTR_SWEEP_STOP_FREQ,
                                                 srds345AttrSweFreq_CoerceCallback));

    checkErr (Ivi_AddAttributeViReal64 (vi, SRDS345_ATTR_SWEEP_MOD_RATE,
                                        "SRDS345_ATTR_SWEEP_MOD_RATE", 1000.00,
                                        IVI_VAL_NEVER_CACHE | IVI_VAL_USE_CALLBACKS_FOR_SIMULATION,
                                        srds345AttrSweepModRate_ReadCallback,
                                        srds345AttrSweepModRate_WriteCallback,
                                        &attrSweModRateRangeTable, 0));
    checkErr (Ivi_SetAttrCoerceCallbackViReal64 (vi, SRDS345_ATTR_SWEEP_MOD_RATE,
                                                 srds345AttrFrequency_CoerceCallback));
    
/*- Miscellaneous Attributes ----------------------------------------------------------------------*/
    checkErr( Ivi_AddAttributeViString (vi, SRDS345_ATTR_ID_QUERY_RESPONSE,
                                        "SRDS345_ATTR_ID_QUERY_RESPONSE", 
                                        "StanfordResearchSystems,DS345",
                                        IVI_VAL_NOT_USER_WRITABLE,
                                        srds345AttrIdQueryResponse_ReadCallback,
                                        VI_NULL));
    
/*- Hidden Attributes -----------------------------------------------------------------------------*/
    checkErr (Ivi_AddAttributeViReal64 (vi, SRDS345_ATTR_AMPLITUDE,
                                        "SRDS345_ATTR_AMPLITUDE", 1.0,
                                        IVI_VAL_HIDDEN,
                                        srds345AttrAmplitude_ReadCallback,
                                        srds345AttrAmplitude_WriteCallback,
                                        &attrAmplitudeRangeTable, 0));
    checkErr( Ivi_SetAttrCoerceCallbackViReal64 (vi, SRDS345_ATTR_AMPLITUDE, 
                                                     srds345AttrAmplitude_CoerceCallback));
    
    checkErr (Ivi_AddAttributeViReal64 (vi, SRDS345_ATTR_OFFSET,
                                        "SRDS345_ATTR_OFFSET", 0.0,
                                        IVI_VAL_HIDDEN,
                                        srds345AttrOffset_ReadCallback,
                                        srds345AttrOffset_WriteCallback,
                                        VI_NULL, 0));
    checkErr( Ivi_SetAttrCoerceCallbackViReal64 (vi, SRDS345_ATTR_OFFSET, 
                                                     srds345AttrOffset_CoerceCallback));
    checkErr (Ivi_SetAttrCheckCallbackViReal64 (vi, SRDS345_ATTR_OFFSET,
                                                srds345AttrOffset_CheckCallback));
    
    checkErr (Ivi_AddAttributeViInt32 (vi, SRDS345_ATTR_WAVEFORM,
                                       "SRDS345_ATTR_WAVEFORM",
                                       SRDS345_VAL_FGEN_SINE, IVI_VAL_HIDDEN,
                                       srds345AttrFunction_ReadCallback,
                                       srds345AttrFunction_WriteCallback,
                                       &AttrFunctionRangeTable));
    checkErr (Ivi_SetAttrCheckCallbackViInt32 (vi, SRDS345_ATTR_WAVEFORM,
                                               srds345AttrFunction_CheckCallback));

    checkErr (Ivi_AddAttributeViInt32 (vi, SRDS345_ATTR_MOD_TYPE,
                                       "SRDS345_ATTR_MOD_TYPE", 0, IVI_VAL_HIDDEN,
                                       srds345AttrModType_ReadCallback,
                                       srds345AttrModType_WriteCallback,
                                       VI_NULL));
    checkErr (Ivi_SetAttrCheckCallbackViInt32 (vi, SRDS345_ATTR_MOD_TYPE,
                                               srds345AttrModType_CheckCallback));
    checkErr (Ivi_AddAttributeViBoolean (vi, SRDS345_ATTR_MOD_STATE,
                                         "SRDS345_ATTR_MOD_STATE", VI_FALSE,
                                         IVI_VAL_HIDDEN,
                                         srds345AttrModState_ReadCallback,
                                         srds345AttrModState_WriteCallback));
    checkErr (Ivi_AddAttributeViInt32 (vi, SRDS345_ATTR_MOD_WAVEFORM,
                                       "SRDS345_ATTR_MOD_WAVEFORM", 0,
                                       IVI_VAL_HIDDEN,
                                       srds345AttrModWaveform_ReadCallback,
                                       srds345AttrModWaveform_WriteCallback,
                                       &attrModWaveformRangeTable));
    checkErr (Ivi_AddAttributeViReal64 (vi, SRDS345_ATTR_MOD_FREQ,
                                        "SRDS345_ATTR_MOD_FREQ", 1000.0,
                                        IVI_VAL_HIDDEN,
                                        srds345AttrModFreq_ReadCallback,
                                        srds345AttrModFreq_WriteCallback,
                                        &attrModFreqRangeTable, 0));

    checkErr (Ivi_AddAttributeViInt32 (vi, SRDS345_ATTR_NUM_ARB_POINTS,
                                       "SRDS345_ATTR_NUM_ARB_POINTS", 16000,
                                       IVI_VAL_ALWAYS_CACHE | IVI_VAL_HIDDEN,
                                       VI_NULL, VI_NULL, VI_NULL));
    checkErr (Ivi_AddAttributeViBoolean(vi, SRDS345_ATTR_WFM_EXISTS,
                                       "SRDS345_ATTR_WFM_EXISTS", VI_FALSE,
                                       IVI_VAL_ALWAYS_CACHE | IVI_VAL_HIDDEN,
                                       VI_NULL, VI_NULL));
    
/*- Setup attribute invalidations -------------------------------------------------------------*/            
    checkErr (Ivi_AddAttributeInvalidation (vi, SRDS345_ATTR_MOD_TYPE,
                                            SRDS345_ATTR_MOD_WAVEFORM, VI_TRUE));
                                            
Error:
    return error;
}


/*****************************************************************************
 *------------------- End Instrument Driver Source Code ---------------------*
 *****************************************************************************/
