/****************************************************************************
 *                       SRS DS345 Function Generator                           
 *---------------------------------------------------------------------------
 *   © 1998-2010, National Instruments, Corporation.  All Rights Reserved.         
 *---------------------------------------------------------------------------
 *                                                                          
 * Title:    srds345.h                                        
 * Purpose:  SRS DS345 Function Generator                                       
 *           instrument driver declarations.                                
 *                                                                          
 ****************************************************************************/

#ifndef __SRDS345_HEADER
#define __SRDS345_HEADER

#include <ivi.h>
#include <ivifgen.h>

#if defined(__cplusplus) || defined(__cplusplus__)
extern "C" {
#endif

/****************************************************************************
 *----------------- Instrument Driver Revision Information -----------------*
 ****************************************************************************/
   
#define SRDS345_MAJOR_VERSION                2     /* Instrument driver major version   */
#define SRDS345_MINOR_VERSION              111     /* Instrument driver minor version   */

#define SRDS345_CLASS_SPEC_MAJOR_VERSION     3     /* Class specification major version */
#define SRDS345_CLASS_SPEC_MINOR_VERSION     0     /* Class specification minor version */

#define SRDS345_SUPPORTED_INSTRUMENT_MODELS  "SRDS345"

#define SRDS345_DRIVER_VENDOR                "National Instruments"
#ifdef _IVI_mswin64_
#define SRDS345_DRIVER_DESCRIPTION           "Stanford Research Synthesized Function Generator DS345 [Compiled for 64-bit]"
#else
#define SRDS345_DRIVER_DESCRIPTION           "Stanford Research Synthesized Function Generator DS345"
#endif

#define SRDS345_IO_SESSION_TYPE              "VISA"

/**************************************************************************** 
 *------------------------------ Useful Macros -----------------------------* 
 ****************************************************************************/
  
#define SRDS345_ATTR_CHANNEL_COUNT                 IVI_ATTR_NUM_CHANNELS                   /* ViInt32,  read-only  */
#define SRDS345_ATTR_GROUP_CAPABILITIES            IVI_ATTR_GROUP_CAPABILITIES             /* ViString, read-only */
/****************************************************************************
 *---------------------------- Attribute Defines ---------------------------*
 ****************************************************************************/

    /*- IVI Inherent Instrument Attributes ---------------------------------*/

        /* User Options */
#define SRDS345_ATTR_RANGE_CHECK                   IVI_ATTR_RANGE_CHECK                    /* ViBoolean */
#define SRDS345_ATTR_QUERY_INSTRUMENT_STATUS       IVI_ATTR_QUERY_INSTR_STATUS             /* ViBoolean */
#define SRDS345_ATTR_CACHE                         IVI_ATTR_CACHE                          /* ViBoolean */
#define SRDS345_ATTR_SIMULATE                      IVI_ATTR_SIMULATE                       /* ViBoolean */
#define SRDS345_ATTR_RECORD_COERCIONS              IVI_ATTR_RECORD_COERCIONS               /* ViBoolean */

        /* Driver Information  */
#define SRDS345_ATTR_SPECIFIC_DRIVER_PREFIX        IVI_ATTR_SPECIFIC_DRIVER_PREFIX         /* ViString, read-only  */
#define SRDS345_ATTR_SUPPORTED_INSTRUMENT_MODELS   IVI_ATTR_SUPPORTED_INSTRUMENT_MODELS    /* ViString, read-only  */
#define SRDS345_ATTR_INSTRUMENT_MANUFACTURER       IVI_ATTR_INSTRUMENT_MANUFACTURER        /* ViString, read-only  */
#define SRDS345_ATTR_INSTRUMENT_MODEL              IVI_ATTR_INSTRUMENT_MODEL               /* ViString, read-only  */
#define SRDS345_ATTR_INSTRUMENT_FIRMWARE_REVISION  IVI_ATTR_INSTRUMENT_FIRMWARE_REVISION              /* ViString, read-only  */
#define SRDS345_ATTR_SPECIFIC_DRIVER_REVISION      IVI_ATTR_SPECIFIC_DRIVER_REVISION       /* ViString, read-only  */
#define SRDS345_ATTR_SPECIFIC_DRIVER_VENDOR        IVI_ATTR_SPECIFIC_DRIVER_VENDOR         /* ViString, read-only  */
#define SRDS345_ATTR_SPECIFIC_DRIVER_DESCRIPTION   IVI_ATTR_SPECIFIC_DRIVER_DESCRIPTION    /* ViString, read-only  */
#define SRDS345_ATTR_SPECIFIC_DRIVER_CLASS_SPEC_MAJOR_VERSION IVI_ATTR_SPECIFIC_DRIVER_CLASS_SPEC_MAJOR_VERSION /* ViInt32, read-only */
#define SRDS345_ATTR_SPECIFIC_DRIVER_CLASS_SPEC_MINOR_VERSION IVI_ATTR_SPECIFIC_DRIVER_CLASS_SPEC_MINOR_VERSION /* ViInt32, read-only */
#define SRDS345_ATTR_DRIVER_SETUP                  IVI_ATTR_DRIVER_SETUP
#define SRDS345_ATTR_INTERCHANGE_CHECK             IVI_ATTR_INTERCHANGE_CHECK


        /* Error Info */

        /* Advanced Session Information */
#define SRDS345_ATTR_LOGICAL_NAME                  IVI_ATTR_LOGICAL_NAME                   /* ViString, read-only  */
#define SRDS345_ATTR_IO_RESOURCE_DESCRIPTOR        IVI_ATTR_RESOURCE_DESCRIPTOR            /* ViString, read-only  */
    
    /*- Instrument-Specific Attributes --------------------------------------*/
    /*- Basic Instrument Operation ------------------------------------------*/
#define SRDS345_ATTR_OPERATION_MODE            IVIFGEN_ATTR_OPERATION_MODE
#define SRDS345_ATTR_OUTPUT_MODE               IVIFGEN_ATTR_OUTPUT_MODE             /* ViInt32                         */
#define SRDS345_ATTR_REF_CLOCK_SOURCE          IVIFGEN_ATTR_REF_CLOCK_SOURCE        /* ViInt32                         */
#define SRDS345_ATTR_OUTPUT_ENABLED            IVIFGEN_ATTR_OUTPUT_ENABLED          /* ViBoolean, multi-channel        */
#define SRDS345_ATTR_OUTPUT_IMPEDANCE          IVIFGEN_ATTR_OUTPUT_IMPEDANCE        /* ViReal64,  multi-channel, ohms  */
        
    /*- Standard Waveform Generation ----------------------------------------*/
#define SRDS345_ATTR_FUNC_WAVEFORM             IVIFGEN_ATTR_FUNC_WAVEFORM           /* ViInt32,  multi-channel           */
#define SRDS345_ATTR_FUNC_AMPLITUDE            IVIFGEN_ATTR_FUNC_AMPLITUDE          /* ViReal64, multi-channel, volts    */
#define SRDS345_ATTR_FUNC_DC_OFFSET            IVIFGEN_ATTR_FUNC_DC_OFFSET          /* ViReal64, multi-channel, volts    */
#define SRDS345_ATTR_FUNC_FREQUENCY            IVIFGEN_ATTR_FUNC_FREQUENCY          /* ViReal64, multi-channel, hertz    */
#define SRDS345_ATTR_FUNC_START_PHASE          IVIFGEN_ATTR_FUNC_START_PHASE        /* ViReal64, multi-channel, degrees  */
#define SRDS345_ATTR_FUNC_DUTY_CYCLE_HIGH      IVIFGEN_ATTR_FUNC_DUTY_CYCLE_HIGH    /* ViReal64, multi-channel, percent  */

    /*- Arbitrary Waveform Generation ---------------------------------------*/
#define SRDS345_ATTR_ARB_WAVEFORM_HANDLE       IVIFGEN_ATTR_ARB_WAVEFORM_HANDLE    /* ViInt32,  multi-channel         */
#define SRDS345_ATTR_ARB_GAIN                  IVIFGEN_ATTR_ARB_GAIN               /* ViReal64, multi-channel         */
#define SRDS345_ATTR_ARB_OFFSET                IVIFGEN_ATTR_ARB_OFFSET             /* ViReal64, multi-channel, volts  */
#define SRDS345_ATTR_ARB_SAMPLE_RATE           IVIFGEN_ATTR_ARB_SAMPLE_RATE        /* ViReal64, samples-per-second    */

#define SRDS345_ATTR_MAX_NUM_WAVEFORMS         IVIFGEN_ATTR_MAX_NUM_WAVEFORMS      /* ViInt32,  read-only             */
#define SRDS345_ATTR_WAVEFORM_QUANTUM          IVIFGEN_ATTR_WAVEFORM_QUANTUM       /* ViInt32,  read-only             */
#define SRDS345_ATTR_MIN_WAVEFORM_SIZE         IVIFGEN_ATTR_MIN_WAVEFORM_SIZE      /* ViInt32,  read-only             */
#define SRDS345_ATTR_MAX_WAVEFORM_SIZE         IVIFGEN_ATTR_MAX_WAVEFORM_SIZE      /* ViInt32,  read-only             */

    /*- Trigger Configuration ----------------------------------------------*/
#define SRDS345_ATTR_TRIGGER_SOURCE            IVIFGEN_ATTR_TRIGGER_SOURCE         /* ViInt32, multi-channel */   

    /*- Internal Trigger Configuration -------------------------------------*/                                                                        
#define SRDS345_ATTR_INTERNAL_TRIGGER_RATE     IVIFGEN_ATTR_INTERNAL_TRIGGER_RATE  /* ViReal64 */

    /*- Burst Configuration ------------------------------------------------*/     /* ViInt32, multi-channel */
#define SRDS345_ATTR_BURST_COUNT               IVIFGEN_ATTR_BURST_COUNT

    /*- Ampitude Modulation Generation --------------------------------------*/
#define SRDS345_ATTR_AM_ENABLED                IVIFGEN_ATTR_AM_ENABLED             /* ViBoolean, multi-channel */
#define SRDS345_ATTR_AM_SOURCE                 IVIFGEN_ATTR_AM_SOURCE              /* ViInt32,   multi-channel */
#define SRDS345_ATTR_AM_INTERNAL_DEPTH         IVIFGEN_ATTR_AM_INTERNAL_DEPTH      /* ViReal64,  percent       */
#define SRDS345_ATTR_AM_INTERNAL_WAVEFORM      IVIFGEN_ATTR_AM_INTERNAL_WAVEFORM   /* ViInt32                  */
#define SRDS345_ATTR_AM_INTERNAL_FREQUENCY     IVIFGEN_ATTR_AM_INTERNAL_FREQUENCY  /* ViReal64,  hertz         */

    /*- Frequency Modulation Generation -------------------------------------*/
#define SRDS345_ATTR_FM_ENABLED                IVIFGEN_ATTR_FM_ENABLED             /* ViBoolean, multi-channel */
#define SRDS345_ATTR_FM_SOURCE                 IVIFGEN_ATTR_FM_SOURCE              /* ViInt32,   multi-channel */
#define SRDS345_ATTR_FM_INTERNAL_DEVIATION     IVIFGEN_ATTR_FM_INTERNAL_DEVIATION  /* ViReal64,  hertz         */
#define SRDS345_ATTR_FM_INTERNAL_WAVEFORM      IVIFGEN_ATTR_FM_INTERNAL_WAVEFORM   /* ViInt32                  */
#define SRDS345_ATTR_FM_INTERNAL_FREQUENCY     IVIFGEN_ATTR_FM_INTERNAL_FREQUENCY  /* ViReal64,  hertz         */

#define SRDS345_ATTR_ID_QUERY_RESPONSE         (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 1L)    /* ViString (Read Only) */
    
    /*- Phase Modulation Generation -----------------------------------------*/
#define SRDS345_ATTR_PM_ENABLED                (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 3L)
#define SRDS345_ATTR_PM_INTERNAL_WAVEFORM      (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 4L)
#define SRDS345_ATTR_PM_INTERNAL_FREQUENCY     (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 5L)
#define SRDS345_ATTR_PM_INTERNAL_SPAN          (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 6L)

#define SRDS345_ATTR_ARB_MOD_RATE              (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 8L)

    /*- Sweep Modulation Generation -----------------------------------------*/
#define SRDS345_ATTR_SWEEP_DIRECTION           (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 11L)
#define SRDS345_ATTR_SWEEP_START_FREQ          (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 13L)
#define SRDS345_ATTR_SWEEP_STOP_FREQ           (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 14L)
    

    /*- Trigger Modulation Generation ---------------------------------------*/
#define SRDS345_ATTR_SWEEP_MOD_RATE            (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 20L)
    
/***************************************************************************** 
 *------------------------ Attribute Value Defines --------------------------* 
 *****************************************************************************/

    /*- Defined values for attribute GPIBFGEN_ATTR_OPERATION_MODE --------------*/
#define SRDS345_VAL_OPERATE_CONTINUOUS         IVIFGEN_VAL_OPERATE_CONTINUOUS
#define SRDS345_VAL_OPERATE_BURST              IVIFGEN_VAL_OPERATE_BURST
#define SRDS345_VAL_OPERATE_SWEEP_LIN          (IVIFGEN_VAL_OP_MODE_SPECIFIC_EXT_BASE + 1L)
#define SRDS345_VAL_OPERATE_SWEEP_LOG          (IVIFGEN_VAL_OP_MODE_SPECIFIC_EXT_BASE + 2L)


    /*- Defined values for attribute SRDS345_ATTR_FUNCTION ------------------*/
#define SRDS345_VAL_OUTPUT_FUNC                IVIFGEN_VAL_OUTPUT_FUNC
#define SRDS345_VAL_OUTPUT_ARB                 IVIFGEN_VAL_OUTPUT_ARB

    /*- Defined values for attribute SRDS345_ATTR_REF_CLOCK_SOURCE ----------*/
#define SRDS345_VAL_REF_CLOCK_INTERNAL         IVIFGEN_VAL_REF_CLOCK_INTERNAL
#define SRDS345_VAL_REF_CLOCK_EXTERNAL         IVIFGEN_VAL_REF_CLOCK_EXTERNAL

    /*- Defined values for attribute SRDS345_ATTR_FUNC_WAVEFORM -------------*/
#define SRDS345_VAL_WFM_SINE                   IVIFGEN_VAL_WFM_SINE     
#define SRDS345_VAL_WFM_SQUARE                 IVIFGEN_VAL_WFM_SQUARE   
#define SRDS345_VAL_WFM_TRIANGLE               IVIFGEN_VAL_WFM_TRIANGLE 
#define SRDS345_VAL_WFM_RAMP_UP                IVIFGEN_VAL_WFM_RAMP_UP  
#define SRDS345_VAL_WFM_NOISE                  (IVIFGEN_VAL_WFM_SPECIFIC_EXT_BASE + 1L)

    /*- Defined values for attribute SRDS345_ATTR_ARB_WAVEFORM_HANDLE -------*/
#define SRDS345_VAL_ARB_WAVEFORM               (10000L)
#define SRDS345_VAL_ARB_NO_WAVEFORM            (1L)

    /*- Defined values for attribute SRDS345_ATTR_TRIGGER_SOURCE ------------*/
#define SRDS345_VAL_INTERNAL_TRIGGER           IVIFGEN_VAL_INTERNAL_TRIGGER
#define SRDS345_VAL_EXTERNAL                   IVIFGEN_VAL_EXTERNAL
#define SRDS345_VAL_SOFTWARE_TRIG              IVIFGEN_VAL_SOFTWARE_TRIG
#define SRDS345_VAL_LINE_TRIGGER               (IVIFGEN_VAL_TRIG_SRC_SPECIFIC_EXT_BASE + 1L) 
#define SRDS345_VAL_EXTERNAL_NEG               (IVIFGEN_VAL_TRIG_SRC_SPECIFIC_EXT_BASE + 2L)

    /*- Defined values for attribute SRDS345_ATTR_AM_SOURCE -----------------*/
#define SRDS345_VAL_AM_INTERNAL                IVIFGEN_VAL_AM_INTERNAL

    /*- Defined values for attribute SRDS345_ATTR_FM_SOURCE -----------------*/
#define SRDS345_VAL_FM_INTERNAL                IVIFGEN_VAL_FM_INTERNAL

    /*- Defined values for attribute SRDS345_ATTR_AM_INTERNAL_WAVEFORM ------*/
#define SRDS345_VAL_AM_INTERNAL_SINE           IVIFGEN_VAL_AM_INTERNAL_SINE    
#define SRDS345_VAL_AM_INTERNAL_SQUARE         IVIFGEN_VAL_AM_INTERNAL_SQUARE    
#define SRDS345_VAL_AM_INTERNAL_TRIANGLE       IVIFGEN_VAL_AM_INTERNAL_TRIANGLE    
#define SRDS345_VAL_AM_INTERNAL_RAMP_UP        IVIFGEN_VAL_AM_INTERNAL_RAMP_UP     

    /*- Defined values for attribute SRDS345_ATTR_FM_INTERNAL_WAVEFORM ------*/
#define SRDS345_VAL_FM_INTERNAL_SINE           IVIFGEN_VAL_FM_INTERNAL_SINE    
#define SRDS345_VAL_FM_INTERNAL_SQUARE         IVIFGEN_VAL_FM_INTERNAL_SQUARE    
#define SRDS345_VAL_FM_INTERNAL_TRIANGLE       IVIFGEN_VAL_FM_INTERNAL_TRIANGLE    
#define SRDS345_VAL_FM_INTERNAL_RAMP_UP        IVIFGEN_VAL_FM_INTERNAL_RAMP_UP     

    /*- Defined values for attribute SRDS345_ATTR_PM_INTERNAL_WAVEFORM ------*/
#define SRDS345_VAL_PM_INTERNAL_SINE           (1L)
#define SRDS345_VAL_PM_INTERNAL_SQUARE         (2L)  
#define SRDS345_VAL_PM_INTERNAL_TRIANGLE       (3L)    
#define SRDS345_VAL_PM_INTERNAL_RAMP_UP        (4L)    

    /*- Defined values for attribute SRDS345_ATTR_SWEEP_DIRECTION -----------*/
#define SRDS345_VAL_SWEEP_SINGLE               (0L)
#define SRDS345_VAL_SWEEP_UP                   (1L)
#define SRDS345_VAL_SWEEP_DOWN                 (2L)

   /*- Defined values for waveformHandle parameter for function srds345_ClearArbWaveform -*/
#define SRDS345_VAL_ALL_WAVEFORMS              (-1L)

/***************************************************************************** 
 *---------------- Instrument Driver Function Declarations ------------------* 
 *****************************************************************************/
    /*- Init and Close Functions --------------------------------------------*/
ViStatus _VI_FUNC srds345_init                     (ViRsrc resourceName, ViBoolean IDQuery,
                                                    ViBoolean resetDevice, ViSession *vi);
ViStatus _VI_FUNC srds345_InitWithOptions          (ViRsrc resourceName, ViBoolean IDQuery, ViBoolean resetDevice, 
                                                    ViConstString optionString, ViSession *newVi);
ViStatus _VI_FUNC srds345_close                    (ViSession vi);   

    /*- Coercion Functions --------------------------------------------------*/
ViStatus _VI_FUNC srds345_GetNextCoercionRecord    (ViSession vi, ViInt32 bufferSize, ViChar record[]);

    /*- Interchangeability Checking Functions ------------------------------*/
ViStatus _VI_FUNC srds345_GetNextInterchangeWarning (ViSession vi, 
                                                      ViInt32 bufferSize, 
                                                      ViChar warnString[]);
ViStatus _VI_FUNC srds345_ResetInterchangeCheck (ViSession vi);
ViStatus _VI_FUNC srds345_ClearInterchangeWarnings (ViSession vi);
    
    /*- Locking Functions ---------------------------------------------------*/
ViStatus _VI_FUNC srds345_LockSession              (ViSession vi, ViBoolean *callerHasLock);   
ViStatus _VI_FUNC srds345_UnlockSession            (ViSession vi, ViBoolean *callerHasLock);

    /*- Basic Instrument Operation ------------------------------------------*/
ViStatus _VI_FUNC srds345_ConfigureOperationMode   (ViSession vi, ViConstString channelName, ViInt32 outputMode);
ViStatus _VI_FUNC srds345_ConfigureOutputMode      (ViSession vi, ViInt32 outputMode);
ViStatus _VI_FUNC srds345_ConfigureRefClockSource  (ViSession vi, ViInt32 refClockSource);
ViStatus _VI_FUNC srds345_ConfigureOutputImpedance (ViSession vi, ViConstString channelName, ViReal64 outputImpedance);
ViStatus _VI_FUNC srds345_ConfigureOutputEnabled   (ViSession vi, ViConstString channelName, ViBoolean enabled);
ViStatus _VI_FUNC srds345_InitiateGeneration       (ViSession vi);
ViStatus _VI_FUNC srds345_AbortGeneration          (ViSession vi);

    /*- Standard Function Output --------------------------------------------*/
ViStatus _VI_FUNC srds345_ConfigureStandardWaveform(ViSession vi, ViConstString channelName,
                                                    ViInt32 waveform, ViReal64 amplitude, ViReal64 dcOffset,
                                                    ViReal64 frequency, ViReal64 startPhase);
ViStatus _VI_FUNC srds345_GenerateStandardWaveform (ViSession vi, ViConstString channelName,
                                                    ViInt32 waveform, ViReal64 amplitude, ViReal64 dcOffset,
                                                    ViReal64 frequency,  ViReal64 startPhase);
                                                    
    /*- Arbitrary Waveform Output -------------------------------------------*/
ViStatus _VI_FUNC srds345_QueryArbWfmCapabilities (ViSession vi, ViInt32 *maxNumWfms, ViInt32 *wfmQuantum,
                                                   ViInt32 *minWfmSize, ViInt32 *maxWfmSize);
ViStatus _VI_FUNC srds345_CreateArbWaveform       (ViSession vi, ViInt32 wfmSize,  ViReal64 wfmData[], ViInt32 *wfmHandle);
ViStatus _VI_FUNC srds345_ConfigureSampleRate     (ViSession vi, ViReal64 sampleRate);
ViStatus _VI_FUNC srds345_ConfigureArbWaveform    (ViSession vi, ViConstString channelName, 
                                                   ViInt32 wfmHandle, ViReal64 arbGain, ViReal64 arbOffset);
ViStatus _VI_FUNC srds345_GenerateArbWaveform     (ViSession vi, ViConstString channelName,
                                                   ViInt32 wfmHandle,  ViReal64 arbGain, ViReal64 arbOffset);
ViStatus _VI_FUNC srds345_ClearArbWaveform        (ViSession vi, ViInt32 wfmHandle);

    /*- Triggering ----------------------------------------------------------*/
ViStatus _VI_FUNC srds345_ConfigureTriggerSource        (ViSession vi, ViConstString channelName, ViInt32 source);
ViStatus _VI_FUNC srds345_ConfigureInternalTriggerRate  (ViSession vi, ViReal64 rate);
ViStatus _VI_FUNC srds345_ConfigureBurstCount           (ViSession vi, ViConstString channelName, ViInt32 count);
ViStatus _VI_FUNC srds345_SendSoftwareTrigger           (ViSession vi);                                             

    /*- Amplitude Modulation ------------------------------------------------*/
ViStatus _VI_FUNC srds345_ConfigureAMEnabled        (ViSession vi, ViConstString channelName, ViBoolean enabled);
ViStatus _VI_FUNC srds345_ConfigureAMSource         (ViSession vi, ViConstString channelName, ViInt32 source);
ViStatus _VI_FUNC srds345_ConfigureAMInternal       (ViSession vi, ViReal64 amDepth, ViInt32 amWaveform, ViReal64 amFrequency);
ViStatus _VI_FUNC srds345_ConfigureAMModArbWaveform (ViSession vi, ViConstString channelName,
                                                     ViInt32 wfmSize, ViReal64  wfmData[], ViInt32 rateDivider);
    
    /*- Frequency Modulation ------------------------------------------------*/
ViStatus _VI_FUNC srds345_ConfigureFMEnabled        (ViSession vi, ViConstString channelName, ViBoolean enabled);
ViStatus _VI_FUNC srds345_ConfigureFMSource         (ViSession vi, ViConstString channelName,  ViInt32 source);
ViStatus _VI_FUNC  srds345_ConfigureFMInternal      (ViSession vi, ViReal64 fmDeviation, ViInt32 fmWaveform, ViReal64 fmFrequency);
ViStatus _VI_FUNC srds345_ConfigureFMModArbWaveform (ViSession vi, ViConstString channelName,
                                                     ViInt32 wfmSize, ViReal64 wfmData[], ViInt32 rateDivider);

    /*- Phase Modulation ----------------------------------------------------*/
ViStatus _VI_FUNC srds345_ConfigurePMEnabled        (ViSession vi, ViConstString channelName, ViBoolean enabled);
ViStatus _VI_FUNC srds345_ConfigurePMInternal       (ViSession vi, ViReal64 spanDegrees,
                                                     ViInt32 modulationWaveform, ViReal64 modulationFrequencyHz);
ViStatus _VI_FUNC srds345_ConfigurePMModArbWaveform (ViSession vi, ViConstString channelName,
                                                     ViInt32 wfmSize, ViReal64 wfmData[], ViInt32 rateDivider);

    /*- Sweeping ------------------------------------------------------------*/
ViStatus _VI_FUNC srds345_ConfigureSweep  (ViSession vi,ViInt32 direction, ViReal64 modRate,
                                           ViReal64 start,ViReal64 stop);

    /*- Channel Info Functions ---------------------------------------------*/
ViStatus _VI_FUNC srds345_GetChannelName (ViSession vi, ViInt32 index, 
                                   ViInt32 bufferSize, ViChar name[]);

    /*- Error Functions -----------------------------------------------------*/
ViStatus _VI_FUNC  srds345_error_query    (ViSession vi, ViInt32 *errorCode, ViChar errorMessage[]);
ViStatus _VI_FUNC  srds345_error_message  (ViSession vi, ViStatus errorCode, ViChar errorMessage[256]);
ViStatus _VI_FUNC  srds345_GetError (ViSession vi, ViStatus *errorCode, 
                                     ViInt32 bufferSize, ViChar description[]);  
ViStatus _VI_FUNC  srds345_ClearError (ViSession vi);
    
    /*- Utility Functions --------------------------------------------------*/
ViStatus _VI_FUNC  srds345_reset          (ViSession vi);
ViStatus _VI_FUNC  srds345_self_test      (ViSession vi, ViInt16 *selfTestResult, ViChar selfTestMessage[]);
ViStatus _VI_FUNC  srds345_revision_query (ViSession vi, ViChar instrumentDriverRevision[], ViChar firmwareRevision[]);
ViStatus _VI_FUNC  srds345_WriteInstrData (ViSession vi, ViConstString writeBuffer); 
ViStatus _VI_FUNC  srds345_ReadInstrData  (ViSession vi, ViInt32 numBytes, ViChar rdBuf[], ViInt32 *bytesRead);
ViStatus _VI_FUNC srds345_InvalidateAllAttributes (ViSession vi);
ViStatus _VI_FUNC srds345_ResetWithDefaults (ViSession vi);
ViStatus _VI_FUNC srds345_Disable (ViSession vi);

    /*- Set, Get, and Check Attribute Functions ----------------------------*/
ViStatus _VI_FUNC  srds345_GetAttributeViInt32   (ViSession vi, ViConstString channelName, ViAttr attribute, ViInt32 *value);
ViStatus _VI_FUNC  srds345_GetAttributeViReal64  (ViSession vi, ViConstString channelName, ViAttr attribute, ViReal64 *value);
ViStatus _VI_FUNC  srds345_GetAttributeViString  (ViSession vi, ViConstString channelName, ViAttr attribute, ViInt32 bufSize, ViChar value[]); 
ViStatus _VI_FUNC  srds345_GetAttributeViSession (ViSession vi, ViConstString channelName, ViAttr attribute, ViSession *value);
ViStatus _VI_FUNC  srds345_GetAttributeViBoolean (ViSession vi, ViConstString channelName, ViAttr attribute, ViBoolean *value);

ViStatus _VI_FUNC  srds345_SetAttributeViInt32   (ViSession vi, ViConstString channelName, ViAttr attribute, ViInt32 value);
ViStatus _VI_FUNC  srds345_SetAttributeViReal64  (ViSession vi, ViConstString channelName, ViAttr attribute, ViReal64 value);
ViStatus _VI_FUNC  srds345_SetAttributeViString  (ViSession vi, ViConstString channelName, ViAttr attribute, ViConstString value); 
ViStatus _VI_FUNC  srds345_SetAttributeViSession (ViSession vi, ViConstString channelName, ViAttr attribute, ViSession value);
ViStatus _VI_FUNC  srds345_SetAttributeViBoolean (ViSession vi, ViConstString channelName, ViAttr attribute, ViBoolean value);

ViStatus _VI_FUNC  srds345_CheckAttributeViInt32   (ViSession vi, ViConstString channelName, ViAttr attribute, ViInt32 value);
ViStatus _VI_FUNC  srds345_CheckAttributeViReal64  (ViSession vi, ViConstString channelName, ViAttr attribute, ViReal64 value);
ViStatus _VI_FUNC  srds345_CheckAttributeViString  (ViSession vi, ViConstString channelName, ViAttr attribute, ViConstString value); 
ViStatus _VI_FUNC  srds345_CheckAttributeViSession (ViSession vi, ViConstString channelName, ViAttr attribute, ViSession value);
ViStatus _VI_FUNC  srds345_CheckAttributeViBoolean (ViSession vi, ViConstString channelName, ViAttr attribute, ViBoolean value);

    /*********************************************************
        Functions reserved for class driver use only.
        End-users should not call these functions.  
     *********************************************************/
ViStatus _VI_FUNC  srds345_IviInit  (ViRsrc resourceName, ViBoolean IDQuery, ViBoolean reset, ViSession vi);
ViStatus _VI_FUNC  srds345_IviClose (ViSession vi);   

/****************************************************************************
 *------------------------ Error And Completion Codes ----------------------*
 ****************************************************************************/
#define SRDS345_ERROR_NO_WFMS_AVAILABLE         IVIFGEN_ERROR_NO_WFMS_AVAILABLE
#define SRDS345_ERROR_TRIGGER_NOT_SOFTWARE      IVIFGEN_ERROR_TRIGGER_NOT_SOFTWARE
#define SRDS345_ERROR_NOT_FUNC_MODE             (IVI_SPECIFIC_ERROR_BASE + 1) 
#define SRDS345_ERROR_NOT_ARB_MODE              (IVI_SPECIFIC_ERROR_BASE + 2) 
#define SRDS345_ERROR_INVALID_WFM_LENGTH        (IVI_SPECIFIC_ERROR_BASE + 3) 
#define SRDS345_ERROR_INVALID_WFM_ELEMENT       (IVI_SPECIFIC_ERROR_BASE + 4) 
#define SRDS345_ERROR_INVALID_WAVEFORM          (IVI_SPECIFIC_ERROR_BASE + 5) 

    /*- Obsolete Inherent Instrument Attributes and functions -*/
    /*- These attributes and functions have been deprecated and may not
        be supported in future versions of this driver. 
    -*/
        /*- Driver Information -*/
#define SRDS345_ATTR_SPECIFIC_DRIVER_MAJOR_VERSION IVI_ATTR_SPECIFIC_DRIVER_MAJOR_VERSION  // ViInt32,  read-only  
#define SRDS345_ATTR_SPECIFIC_DRIVER_MINOR_VERSION IVI_ATTR_SPECIFIC_DRIVER_MINOR_VERSION  // ViInt32,  read-only  

        /*- Error Info -*/
#define SRDS345_ATTR_PRIMARY_ERROR                 IVI_ATTR_PRIMARY_ERROR                  // ViInt32 
#define SRDS345_ATTR_SECONDARY_ERROR               IVI_ATTR_SECONDARY_ERROR                // ViInt32
#define SRDS345_ATTR_ERROR_ELABORATION             IVI_ATTR_ERROR_ELABORATION              // ViString

        /*- Advanced Session Information -*/
#define SRDS345_ATTR_IO_SESSION_TYPE               IVI_ATTR_IO_SESSION_TYPE                // ViString, read-only  
#define SRDS345_ATTR_IO_SESSION                    IVI_ATTR_IO_SESSION                     // ViSession, read-only

        /*- Deprecated Attribute Identifiers for Renamed Attributes -*/
#define SRDS345_ATTR_NUM_CHANNELS                  SRDS345_ATTR_CHANNEL_COUNT
#define SRDS345_ATTR_QUERY_INSTR_STATUS            SRDS345_ATTR_QUERY_INSTRUMENT_STATUS            
#define SRDS345_ATTR_RESOURCE_DESCRIPTOR           SRDS345_ATTR_IO_RESOURCE_DESCRIPTOR

        /*- Deprecated Error Information functions -*/
ViStatus _VI_FUNC  srds345_GetErrorInfo (ViSession vi, ViStatus *primaryError, 
                                          ViStatus *secondaryError, 
                                          ViChar errorElaboration[256]);
ViStatus _VI_FUNC  srds345_ClearErrorInfo (ViSession vi);

/**************************************************************************** 
 *---------------------------- End Include File ----------------------------* 
 ****************************************************************************/
#if defined(__cplusplus) || defined(__cplusplus__)
}
#endif
#endif /* __SRDS345_HEADER */




