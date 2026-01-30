/*******************************************************************
*This file created using Driver Wizard version 1.8 and instrtmp.c
*/ 
/***************************************************************************
 * Tektronix VX4244 16-Channel Digitizer Module
 * Copyright Tektronix Inc. 1996, 1998
 * Originally written in C
 * Driver version 2.1.0
 * VXIPlug&Play WIN/WIN95/WINNT Frameworks Revision  4.0 
 * Compatible with Instrument Firmware Version 1.4 or later
 *
 */
/***************************************************************************
 * Revision History: Original release 2.0.1
 * Revision 2.1.0:Nv 1998.
 * 1. Wrote a new function named tkvx4244_getBinaryData to retrieve binary data. 
 * (solves the  LabView transfer problem). Maintained the old function
 * for backward compatibility. Old function is not in the function panel file.
 * 2. Wrote a new function named tkvx4244_getFDCData to retrieve FDC data. 
 * (solves the  LabView transfer problem). Maintained the old function
 * for backward compatibility. Old function is not in the function panel file.
 * 3. Eliminated all references to VTL or vtl in comments.
 * 4. Eliminated all references to VXIplug&play specifications.
 * 5. Exposed the tkvx4244_write and tkvx4244_read functions in the function panel.
 * 6. Corrected all signed/unsigned mismatch warnings.
 * 7. Removed all unnecessary line termination escape characters '\'.
 */

#ifndef _tkvx4244_H_
#define _tkvx4244_H_


#include <vpptype.h>

#if defined(__cplusplus) || defined(__cplusplus__)
   extern "C" {
#endif

#if defined(_CVI_)
#define tkvx4244_VI_PTR	*
#else
#define tkvx4244_VI_PTR	_VI_FAR *
#endif

#ifndef VI_VERSION_MAJOR
#define VI_VERSION_MAJOR(ver)       ((((ViVersion)ver) & 0xFFF00000UL) >> 20)
#define VI_VERSION_MINOR(ver)       ((((ViVersion)ver) & 0x000FFF00UL) >>  8)
#define VI_VERSION_SUBMINOR(ver)    ((((ViVersion)ver) & 0x000000FFUL)		)
#endif

/*VXI p&p required function defines*/ 
#define INIT_DO_QUERY 1
#define INIT_SKIP_QUERY 0
#define INIT_DO_RESET 1
#define INIT_DONT_RESET 0
/*Place instrument specific defines here*/
#define tkvx4244_CH_ONE_ENABLE 1
#define tkvx4244_CH_ONE_DISABLE 0
#define tkvx4244_CH_TWO_ENABLE 1
#define tkvx4244_CH_TWO_DISABLE 0
#define tkvx4244_CH_THREE_ENABLE 1
#define tkvx4244_CH_THREE_DISABLE 0
#define tkvx4244_CH_FOUR_ENABLE 1
#define tkvx4244_CH_FOUR_DISABLE 0
#define tkvx4244_CH_FIVE_ENABLE 1
#define tkvx4244_CH_FIVE_DISABLE 0
#define tkvx4244_CH_SIX_ENABLE 1
#define tkvx4244_CH_SIX_DISABLE 0
#define tkvx4244_CH_SEVEN_ENABLE 1
#define tkvx4244_CH_SEVEN_DISABLE 0
#define tkvx4244_CH_EIGHT_ENABLE 1
#define tkvx4244_CH_EIGHT_DISABLE 0
#define tkvx4244_CH_NINE_ENABLE 1
#define tkvx4244_CH_NINE_DISABLE 0
#define tkvx4244_CH_TEN_ENABLE 1
#define tkvx4244_CH_TEN_DISABLE 0
#define tkvx4244_CH_ELEVEN_ENABLE 1
#define tkvx4244_CH_ELEVEN_DISABLE 0
#define tkvx4244_CH_TWELVE_ENABLE 1
#define tkvx4244_CH_TWELVE_DISABLE 0
#define tkvx4244_CH_THIRTEEN_ENABLE 1
#define tkvx4244_CH_THIRTEEN_DISABLE 0
#define tkvx4244_CH_FOURTEEN_ENABLE 1
#define tkvx4244_CH_FOURTEEN_DISABLE 0
#define tkvx4244_CH_FIFTEEN_ENABLE 1
#define tkvx4244_CH_FIFTEEN_DISABLE 0
#define tkvx4244_CH_SIXTEEN_ENABLE 1
#define tkvx4244_CH_SIXTEEN_DISABLE 0
#define tkvx4244_SELECT_ALL_CHANNELS 1
#define tkvx4244_DONT_SELECT_ALL_CHANNELS 0
#define tkvx4244_SELECT_ENABLE 1
#define tkvx4244_SELECT_DISABLE 0 
#define tkvx4244_VOLT_RANGE_200MV 0
#define tkvx4244_VOLT_RANGE_500MV 1
#define tkvx4244_VOLT_RANGE_1VOLT 2
#define tkvx4244_VOLT_RANGE_2VOLT 3
#define tkvx4244_VOLT_RANGE_5VOLT 4
#define tkvx4244_VOLT_RANGE_10VOLT 5
#define tkvx4244_VOLT_RANGE_20VOLT 6
#define tkvx4244_VOLT_RANGE_UPPER 0
#define tkvx4244_VOLT_RANGE_LOWER 1
#define tkvx4244_CLOCK_INTERNAL 1
#define tkvx4244_CLOCK_EXTERNAL 0
#define tkvx4244_INTERVAL 0
#define tkvx4244_FREQUENCY 1
#define tkvx4244_GROUP_ONE_ENABLE 1
#define tkvx4244_GROUP_ONE_DISABLE 0
#define tkvx4244_GROUP_TWO_ENABLE 1
#define tkvx4244_GROUP_TWO_DISABLE 0 
#define tkvx4244_GROUP_THREE_ENABLE 1
#define tkvx4244_GROUP_THREE_DISABLE 0 
#define tkvx4244_GROUP_FOUR_ENABLE 1 
#define tkvx4244_GROUP_FOUR_DISABLE 0
#define tkvx4244_ALL_GROUPS_ENABLE  1
#define tkvx4244_ALL_GROUPS_DISABLE  0
#define tkvx4244_TRIGGER_LOGIC_OR   1
#define tkvx4244_TRIGGER_LOGIC_AND   0
#define tkvx4244_TRIGGER_EXT_ENABLE  1
#define tkvx4244_TRIGGER_EXT_DISABLE  0
#define tkvx4244_TRIGGER_IMM_ENABLE 1
#define tkvx4244_TRIGGER_IMM_DISABLE 0
#define tkvx4244_TRIGGER_VXI_ENABLE  1
#define tkvx4244_TRIGGER_VXI_DISABLE 0
#define tkvx4244_TRIGGER_THRESH_ENABLE 1
#define tkvx4244_TRIGGER_THRESH_DISABLE 0
#define tkvx4244_TRIGGER_TTL0_ENABLE 1
#define tkvx4244_TRIGGER_TTL0_DISABLE 0
#define tkvx4244_TRIGGER_TTL1_ENABLE 1
#define tkvx4244_TRIGGER_TTL1_DISABLE 0
#define tkvx4244_TRIGGER_TTL2_ENABLE 1
#define tkvx4244_TRIGGER_TTL2_DISABLE 0
#define tkvx4244_TRIGGER_TTL3_ENABLE 1
#define tkvx4244_TRIGGER_TTL3_DISABLE 0
#define tkvx4244_TRIGGER_TTL4_ENABLE 1
#define tkvx4244_TRIGGER_TTL4_DISABLE 0
#define tkvx4244_TRIGGER_TTL5_ENABLE 1
#define tkvx4244_TRIGGER_TTL5_DISABLE 0
#define tkvx4244_TRIGGER_TTL6_ENABLE 1
#define tkvx4244_TRIGGER_TTL6_DISABLE 0
#define tkvx4244_TRIGGER_TTL7_ENABLE 1
#define tkvx4244_TRIGGER_TTL7_DISABLE 0
#define tkvx4244_EXTERNAL_ARM  0
#define tkvx4244_IMMEDIATE_ARM  1
#define tkvx4244_MEMORY_ZERO_ON  2
#define tkvx4244_MEMORY_ZERO_OFF  3
#define tkvx4244_SLOPE_POS  0
#define tkvx4244_SLOPE_NEG  1
#define tkvx4244_TTLTRIG0 0
#define tkvx4244_TTLTRIG1 1
#define tkvx4244_TTLTRIG2 2
#define tkvx4244_TTLTRIG3 3
#define tkvx4244_TTLTRIG4 4
#define tkvx4244_TTLTRIG5 5
#define tkvx4244_TTLTRIG6 6
#define tkvx4244_TTLTRIG7 7
#define tkvx4244_TTL_TRIGGER_OFF 8
#define tkvx4244_GROUP_CHAN_ONE 0
#define tkvx4244_GROUP_CHAN_TWO 1 
#define tkvx4244_GROUP_CHAN_THREE 2
#define tkvx4244_GROUP_CHAN_FOUR 3
#define tkvx4244_GROUP_CHAN_OFF 4
#define tkvx4244_THRESH_POS_SLOPE 0
#define tkvx4244_THRESH_NEG_SLOPE 1
#define tkvx4244_THRESH_GREATER_THAN 2
#define tkvx4244_THRESH_LESS_THAN 3
#define tkvx4244_CLK_SYNC_POS  0
#define tkvx4244_CLK_SYNC_NEG  1
#define tkvx4244_CLK_SYNC_IMM  2
#define tkvx4244_CLK_SYNC_TTLOFF  3
#define tkvx4244_CLK_SYNC_TTL1  4
#define tkvx4244_CLK_SYNC_TTL2  5
#define tkvx4244_CLK_SYNC_TTL3  6
#define tkvx4244_CLK_SYNC_TTL4  7
#define tkvx4244_CLK_SYNC_TTL5  8
#define tkvx4244_CLK_SYNC_TTL6  9
#define tkvx4244_CLK_SYNC_TTL7  10
#define tkvx4244_INIT_IMM  0
#define tkvx4244_INIT_DELAY  1
#define tkvx4244_INIT_CONT  2
#define tkvx4244_FDC_IMM  0
#define tkvx4244_FDC_AUTO  1
#define tkvx4244_FDC_OFF  2
#define tkvx4244_ANALYZE_MIN 0
#define tkvx4244_ANALYZE_MAX 1
#define tkvx4244_ANALYZE_AVE 2
#define tkvx4244_ANALYZE_POS_TRANS 3
#define tkvx4244_ANALYZE_NEG_TRANS 4
#define tkvx4244_ANALYZE_TRMS 5
#define	tkvx4244_ANALYZE_CH1  1
#define tkvx4244_ANALYZE_CH2  2
#define tkvx4244_ANALYZE_CH3  3
#define tkvx4244_ANALYZE_CH4  4
#define tkvx4244_ANALYZE_CH5  5
#define tkvx4244_ANALYZE_CH6  6
#define tkvx4244_ANALYZE_CH7  7
#define tkvx4244_ANALYZE_CH8  8
#define tkvx4244_ANALYZE_CH9  9
#define tkvx4244_ANALYZE_CH10  10
#define tkvx4244_ANALYZE_CH11  11
#define tkvx4244_ANALYZE_CH12  12
#define tkvx4244_ANALYZE_CH13  13
#define tkvx4244_ANALYZE_CH14  14
#define tkvx4244_ANALYZE_CH15  15
#define tkvx4244_ANALYZE_CH16  16
#define tkvx4244_CHAN_QUERY_OPEN 0
#define tkvx4244_CHAN_QUERY_CLOSED 1
#define tkvx4244_CHAN_QUERY_STATE 2
#define tkvx4244_RATE_QUERY_INTERVAL  0
#define tkvx4244_RATE_QUERY_FREQ  1
#define tkvx4244_RATE_QUERY_SRC 2
#define tkvx4244_ENABLE_SRE  0
#define tkvx4244_ENABLE_ESE  1
#define tkvx4244_QUERY_IDN  0
#define tkvx4244_QUERY_STB  1
#define tkvx4244_QUERY_SRE  2
#define tkvx4244_QUERY_ESR  3
#define tkvx4244_QUERY_ESE  4
#define tkvx4244_QUERY_ARM_SRC  0
#define tkvx4244_QUERY_MEM_ZERO  1
#define tkvx4244_TRIG_QUERY_LOGIC  0
#define tkvx4244_TRIG_QUERY_MASK  1
#define tkvx4244_TRIG_QUERY_SOURCE  2
#define tkvx4244_OUT_QUERY_SLOPE  0
#define tkvx4244_OUT_QUERY_TTLTRG 1
#define tkvx4244_OUT_QUERY_LOGIC  2
#define tkvx4244_OUT_QUERY_MASK 3
#define tkvx4244_TT_CLK10  2
#define tkvx4244_TT_ROSC  3
#define tkvx4244_GROUP_ONE  1
#define tkvx4244_GROUP_TWO  2
#define tkvx4244_GROUP_THREE  3
#define tkvx4244_GROUP_FOUR  4
#define tkvx4244_SYNC_QUERY_LEVEL 1
#define tkvx4244_SYNC_QUERY_TTL 0
#define tkvx4244_FDC_BUF_2K  2048L
#define tkvx4244_FDC_BUF_4K  4096L
#define tkvx4244_FDC_BUF_8K  8192L
#define tkvx4244_FDC_BUF_16K  16384L
#define tkvx4244_FDC_BUF_32K  32768
#define tkvx4244_FDC_BUF_64K  65536
#define tkvx4244_FDC_BUF_128K  131072
#define tkvx4244_FDC_BUF_256K  262144
#define tkvx4244_FDC_QUERY_MODE 0
#define tkvx4244_FDC_QUERY_TRANS 1
#define tkvx4244_FDC_QUERY_QUE 2

/*= GLOBAL FUNCTION DECLARATIONS ==========================================*/
/***************************************************************************
 * Function: tkvx4244_confGpOneTrigSrcLogic
 *
 * Purpose:
 *  This function captures data on a selected channel and returns 
 *  the data in a character array.
 * Parameters:
 *	ViSession instrumentHandle: VISA instrument handle used to 
 *  access instrument specific data. Must be initialized by 
 *  tkvx4244_init() prior to use.
 *  ViInt16 channel:
 *	ViInt16 voltageRange:
 *  ViInt16 intervalFrequency:
 *  ViReal64 samplingRate:
 *  ViInt32 startingAddress:
 *	ViInt16 numberOfSamples:
 *	ViChar dataArray[]:
 *  Returns:
 *		VI_SUCCESS if no errors occurred, error code otherwise
 */
ViStatus _VI_FUNC 
tkvx4244_measCaptureReturnData (ViSession instrumentHandle, ViInt16 channel,
					ViInt16 voltageRange,ViInt16 intervalFrequency,
					ViReal64 samplingRate,ViInt32 startingAddress,
					ViInt16 numberOfSamples,ViChar dataArray[]);
/***************************************************************************
 * Function: tkvx4244_confGroupOneChannels
 *
 * Purpose:
 *    This function enables or disables selected channels
 *    in group 1.
 *
 * Parameters:
 *	ViSession instrumentHandle: VISA instrument handle used to access instrument
 *		specific data. Must be initialized by tkvx4244_init()
 *		prior to use.
 *		ViInt16 select, ch1-ch4
 *  Returns:
 *		VI_SUCCESS if no errors occurred, error code otherwise
 */
ViStatus _VI_FUNC 
tkvx4244_confGroupOneChannels (ViSession instrumentHandle,
ViInt16 ch1,ViInt16 ch2,ViInt16 ch3,ViInt16 ch4,ViInt16 select);
/***************************************************************************
 * Function: tkvx4244_confGroupTwoChannels
 *
 * Purpose:
 *    This function enables or disables selected channels
 *    in group 2.
 *
 * Parameters:
 *	ViSession instrumentHandle: VISA instrument handle used to access instrument
 *		specific data. Must be initialized by tkvx4244_init()
 *		prior to use.
 *		ViInt16 select, ch5-ch8
 *  Returns:
 *		VI_SUCCESS if no errors occurred, error code otherwise
 */
ViStatus _VI_FUNC 
tkvx4244_confGroupTwoChannels (ViSession instrumentHandle,
ViInt16 ch5,ViInt16 ch6,ViInt16 ch7,ViInt16 ch8,ViInt16 select);
/***************************************************************************
 * Function: tkvx4244_confGroupThreeChannels
 *
 * Purpose:
 *    This function enables or disables selected channels
 *    in group 3.
 *
 * Parameters:
 *	ViSession instrumentHandle: VISA instrument handle used to access instrument
 *		specific data. Must be initialized by tkvx4244_init()
 *		prior to use.
 *		ViInt16 select, ch9-ch12
 *  Returns:
 *		VI_SUCCESS if no errors occurred, error code otherwise
 */
ViStatus _VI_FUNC 
tkvx4244_confGroupThreeChannels (ViSession instrumentHandle,
ViInt16 ch9,ViInt16 ch10,ViInt16 ch11,ViInt16 ch12,ViInt16 select);
/***************************************************************************
 * Function: tkvx4244_confGroupFourChannels
 *
 * Purpose:
 *    This function enables or disables selected channels
 *    in group 4.
 *
 * Parameters:
 *	ViSession instrumentHandle: VISA instrument handle used to access instrument
 *		specific data. Must be initialized by tkvx4244_init()
 *		prior to use.
 *		ViInt16 select, ch13-ch16
 *  Returns:
 *		VI_SUCCESS if no errors occurred, error code otherwise
 */
ViStatus _VI_FUNC 
tkvx4244_confGroupFourChannels (ViSession instrumentHandle,
ViInt16 ch13,ViInt16 ch14,ViInt16 ch15,ViInt16 ch16,ViInt16 select);
/***************************************************************************
 * Function: tkvx4244_chanlist
 *
 * Purpose:
 *	 NOTE: This Function is an "Exception Function" and is not
 *compliant with VXIplu&play standards because of the number of
 *parameters in the function. This function is being maintained in
 *this version of the VXIplug&play driver to provide compatibility
 *with existing applications. New functions have been written to
 *replace this function.  There will not be a function panel for
 *this function in WIN Framework 4.0 compliant drivers.
 *
 *    This function enables or disables selected channels.
 *    This function is an 'Exception Function'.
 *    Maintain for compatibility with existing applications.
 *    include in dll but not in function panel file.
 *
 * Parameters:
 *	ViSession instrumentHandle: VISA instrument handle used to access instrument
 *		specific data. Must be initialized by tkvx4244_init()
 *		prior to use.
 *		ViInt16 opn_cls, c1-c16, ca
 *  Returns:
 *		VI_SUCCESS if no errors occurred, error code otherwise
 */
ViStatus _VI_FUNC tkvx4244_chanlist (ViSession instrumentHandle,ViInt16 select,ViInt16 ch1,ViInt16 ch2,ViInt16 ch3,ViInt16 ch4,ViInt16 ch5,ViInt16 ch6,
ViInt16 ch7,ViInt16 ch8,ViInt16 ch9,ViInt16 ch10,ViInt16 ch11,ViInt16 ch12,ViInt16 ch13,ViInt16 ch14,ViInt16 ch15,ViInt16 ch16,ViInt16 all);
	/*For compatibility with old applications*/ 
#define TKVX4244_chanlist(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s)  tkvx4244_chanlist((a),(b),(c),(d),(e),(f),(g),(h),(i),(j),(k),(l),(m),(n),(o),(p),(q),(r),(s))
/***************************************************************************
 * Function: tkvx4244_initMeasurementCycle
 *
 * Purpose:
 *  This function Begins the collection process on the designated
 *  groups.
 * Parameters:
 *	ViSession instrumentHandle: VISA instrument handle used to access instrument
 *		specific data. Must be initialized by tkvx4244_init()
 *		prior to use.
 *      ViReal64 delay - sets a delay before digitizing if desired.
 *		ViInt16 gp1-gp4,all,initiate
 *  Returns:
 *		VI_SUCCESS if no errors occurred, error code otherwise
 */
ViStatus _VI_FUNC 
tkvx4244_initMeasurementCycle (ViSession instrumentHandle,
ViReal64 delay,ViInt16 gp1,ViInt16 gp2,ViInt16 gp3,ViInt16 gp4,
ViInt16 all,ViInt16 initiate);
	/*For compatibility with old applications*/ 
#define TKVX4244_start_collection(a,b,c,d,e,f,g,h)  tkvx4244_initMeasurementCycle((a),(b),(c),(d),(e),(f),(g),(h))
/***************************************************************************
 * Function: tkvx4244_confDataCollectCount
 *
 * Purpose:
 *  This function sets up various parameters relating to the collection of
 *  data. The number of samples to collect and the group this number applies
 *  to is defined. Long count is the number of samples and char gplst
 *  the group or groups.
 * Parameters:
 *	ViSession instrumentHandle: VISA instrument handle used to access instrument
 *		specific data. Must be initialized by tkvx4244_init()
 *		prior to use.
 *  ViInt32 numberOfSamples-number of samples.
 *  ViInt16 gp1-gp4,allGroups
 *  Returns:
 *		VI_SUCCESS if no errors occurred, error code otherwise
 */
ViStatus _VI_FUNC 
tkvx4244_confDataCollectCount (ViSession instrumentHandle,
ViInt32 numberOfSamples,ViInt16 gp1,ViInt16 gp2,ViInt16 gp3,
ViInt16 gp4,ViInt16 allGroups);
	/*For compatibility with old applications*/ 
#define TKVX4244_data_collect(a,b,c,d,e,f,g)  tkvx4244_confDataCollectCount((a),(b),(c),(d),(e),(f),(g))
/***************************************************************************
 * Function: tkvx4244_confExternalArmSlope
 *
 * Purpose:
 *  This function programs the active edge of the external arm input.
 * Parameters:
 *	ViSession instrumentHandle: VISA instrument handle used to access instrument
 *		specific data. Must be initialized by tkvx4244_init()
 *		prior to use.
 *  ViInt16 slope
 *  ViChar slopeVerification[]
 *  Returns:
 *		VI_SUCCESS if no errors occurred, error code otherwise
 */
ViStatus _VI_FUNC 
tkvx4244_confExternalArmSlope (ViSession instrumentHandle,
ViInt16 slope,ViChar slopeVerification[]);
	/*For compatibility with old applications*/ 
#define TKVX4244_ext_arm_parms(a,b,c)  tkvx4244_confExternalArmSlope((a),(b),(c))
/***************************************************************************
 * Function: tkvx4244_readMasterOutTrigger
 *
 * Purpose:
 *  This function queries the instrument for output trig setups.
 * Parameters:
 *	ViSession instrumentHandle: VISA instrument handle used to access instrument
 *		specific data. Must be initialized by tkvx4244_init()
 *		prior to use.
 *  ViInt16 outputTrigger
 *  ViChar masterOutputtrigger[]
 *  Returns:
 *		VI_SUCCESS if no errors occurred, error code otherwise
 */
ViStatus _VI_FUNC 
tkvx4244_readMasterOutTrigger (ViSession instrumentHandle,
ViInt16 outputTrigger,ViChar masterOutputtrigger[]);
	/*For compatibility with old applications*/ 
#define TKVX4244_out_trig_query(a,b,c)  tkvx4244_readMasterOutTrigger((a),(b),(c))
/***************************************************************************
 * Function: tkvx4244_confMasterClock
 *
 * Purpose:
 *  This function defines the master sample rate clock
 * Parameters:
 *	ViSession instrumentHandle: VISA instrument handle used to access instrument
 *		specific data. Must be initialized by tkvx4244_init()
 *		prior to use.
 *  ViReal64 frequency
 *  ViInt16 source
 *  ViChar masterClockSource[]
 *  ViChar masterClockFrequency[]
 *  Returns:
 *		VI_SUCCESS if no errors occurred, error code otherwise
 */
ViStatus _VI_FUNC 
tkvx4244_confMasterClock (ViSession instrumentHandle,
ViReal64 frequency,ViInt16 source,ViChar masterClockSource[],
ViChar masterClockFrequency[]);
/*For compatibility with old applications*/ 
#define TKVX4244_mastr_clk_source(a,b,c,d,e)  tkvx4244_confMasterClock((a),(b),(c),(d),(e))
/***************************************************************************
 * Function: tkvx4244_readTrigThreshold
 *
 * Purpose:
 *  This function queries trigger threshold settings.
 * Parameters:
 *	ViSession instrumentHandle: VISA instrument handle used to access instrument
 *		specific data. Must be initialized by tkvx4244_init()
 *		prior to use.
 *
 *  ViChar threshold[]
 *  Returns:
 *		VI_SUCCESS if no errors occurred, error code otherwise
 */
ViStatus _VI_FUNC 
tkvx4244_readTrigThreshold (ViSession instrumentHandle,
ViChar threshold[]);
	/*For compatibility with old applications*/ 
#define TKVX4244_trig_thresh_query(a,b)  tkvx4244_readTrigThreshold((a),(b))
/***************************************************************************
 * Function: tkvx4244_readTrigSrcAndLogic
 *
 * Purpose:
 *  This function queries various trigger parameters for logic and source.
 * Parameters:
 *	ViSession instrumentHandle: VISA instrument handle used to access instrument
 *		specific data. Must be initialized by tkvx4244_init()
 *		prior to use.
 *  ViInt16 sourceAndLogic
 *  ViInt16 gp1-gp4,all
 *  ViChar triggerSettings[]
 *  Returns:
 *		VI_SUCCESS if no errors occurred, error code otherwise
 */
ViStatus _VI_FUNC 
tkvx4244_readTrigSrcAndLogic (ViSession instrumentHandle,
ViInt16 sourceAndLogic,ViInt16 gp1,ViInt16 gp2,ViInt16 gp3,
ViInt16 gp4,ViInt16 all,ViChar triggerSettings[]);
/*For compatibility with old applications*/ 
#define TKVX4244_trig_src_log_query(a,b,c,d,e,f,g,h)  tkvx4244_readTrigSrcAndLogic((a),(b),(c),(d),(e),(f),(g),(h))
/***************************************************************************
 * Function: tkvx4244_confTriggerSlope
 *
 * Purpose:
 *  This function queries the instrument for the trigger slope settings.
 * Parameters:
 *	ViSession instrumentHandle: VISA instrument handle used to access instrument
 *		specific data. Must be initialized by tkvx4244_init()
 *		prior to use.
 *  ViInt16 acttiveEdge
 *  ViChar slope[]
 *  Returns:
 *		VI_SUCCESS if no errors occurred, error code otherwise
 */
ViStatus _VI_FUNC 
tkvx4244_confTriggerSlope (ViSession instrumentHandle,
ViInt16 activeEdge,ViChar slope[]);
/*For compatibility with old applications*/ 
#define TKVX4244_trig_slope(a,b,c)  tkvx4244_confTriggerSlope((a),(b),(c))
/***************************************************************************
 * Function: tkvx4244_confTriggerSrcAndLogic
 *
 * Purpose:
 *NOTE: This Function is an "Exception Function" and is not
 *compliant with VXIplu&play standards because of the number of
 *parameters in the function. This function is being maintained in
 *this version of the VXIplug&play driver to provide compatibility
 *with existing applications. New functions have been written to
 *replace this function.  There will not be a function panel for
 *this function in WIN Framework 4.0 compliant drivers.
 *  This function sets various trigger parameters for logic and source.
 * Parameters:
 *	ViSession instrumentHandle: VISA instrument handle used to access instrument
 *		specific data. Must be initialized by tkvx4244_init()
 *		prior to use.
 *  ViInt16 ext,imm,thre,vxi-- ttl0,ttl1,ttl2,ttl3,ttl4,ttl5,ttl6,ttl7
 *  ViInt16 gp1,gp2,gp3,gp4,gpa
 *  ViInt16 logic
 *  Returns:
 *		VI_SUCCESS if no errors occurred, error code otherwise
 */
ViStatus _VI_FUNC tkvx4244_confTriggerSrcAndLogic (ViSession instrumentHandle,ViInt16 logic,ViInt16 ext,ViInt16 imm,ViInt16 thre,ViInt16 vxi,
ViInt16 ttl0,ViInt16 ttl1,ViInt16 ttl2,ViInt16 ttl3,ViInt16 ttl4,ViInt16 ttl5,ViInt16 ttl6,ViInt16 ttl7,
ViInt16 gp1,ViInt16 gp2,ViInt16 gp3,ViInt16 gp4,ViInt16 gpa);
	/*For compatibility with old applications*/ 
#define TKVX4244_trig_src_log(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s)  tkvx4244_confTriggerSrcAndLogic((a),(b),(c),(d),(e),(f),(g),(h),(i),(j),(k),(l),(m),(n),(o),(p),(q),(r),(s))
/***************************************************************************
 * Function: tkvx4244_confGpOneTrigSrcLogic
 *
 * Purpose:
 *  This function sets various trigger parameters for logic 
 *  and source for Group 1.
 * Parameters:
 *	ViSession instrumentHandle: VISA instrument handle used to access instrument
 *		specific data. Must be initialized by tkvx4244_init()
 *		prior to use.
 *  ViInt16 external,immediate,threshold,vxicmd-- ttl0,ttl1,ttl2,ttl3,ttl4,ttl5,ttl6,ttl7
 *  ViInt16 logic
 *  Returns:
 *		VI_SUCCESS if no errors occurred, error code otherwise
 */
ViStatus _VI_FUNC 
tkvx4244_confGpOneTrigSrcLogic (ViSession instrumentHandle,
ViInt16 logic,ViInt16 external,ViInt16 immediate,ViInt16 threshold,
ViInt16 vxicmd,ViInt16 ttl0,ViInt16 ttl1,ViInt16 ttl2,ViInt16 ttl3,
ViInt16 ttl4,ViInt16 ttl5,ViInt16 ttl6,ViInt16 ttl7);
/***************************************************************************
 * Function: tkvx4244_confGpTwoTrigSrcLogic
 *
 * Purpose:
 *  This function sets various trigger parameters for logic 
 *  and source for Group 2.
 * Parameters:
 *	ViSession instrumentHandle: VISA instrument handle used to access instrument
 *		specific data. Must be initialized by tkvx4244_init()
 *		prior to use.
 *  ViInt16 external,immediate,threshold,vxicmd-- ttl0,ttl1,ttl2,ttl3,ttl4,ttl5,ttl6,ttl7
 *  ViInt16 logic
 *  Returns:
 *		VI_SUCCESS if no errors occurred, error code otherwise
 */
ViStatus _VI_FUNC 
tkvx4244_confGpTwoTrigSrcLogic (ViSession instrumentHandle,
ViInt16 logic,ViInt16 external,ViInt16 immediate,ViInt16 threshold,
ViInt16 vxicmd,ViInt16 ttl0,ViInt16 ttl1,ViInt16 ttl2,ViInt16 ttl3,
ViInt16 ttl4,ViInt16 ttl5,ViInt16 ttl6,ViInt16 ttl7);
/***************************************************************************
 * Function: tkvx4244_confGpThreeTrigSrcLogic
 *
 * Purpose:
 *  This function sets various trigger parameters for logic 
 *  and source for Group 3.
 * Parameters:
 *	ViSession instrumentHandle: VISA instrument handle used to access instrument
 *		specific data. Must be initialized by tkvx4244_init()
 *		prior to use.
 *  ViInt16 external,immediate,threshold,vxicmd-- ttl0,ttl1,ttl2,ttl3,ttl4,ttl5,ttl6,ttl7
 *  ViInt16 logic
 *  Returns:
 *		VI_SUCCESS if no errors occurred, error code otherwise
 */
ViStatus _VI_FUNC 
tkvx4244_confGpThreeTrigSrcLogic (ViSession instrumentHandle,
ViInt16 logic,ViInt16 external,ViInt16 immediate,ViInt16 threshold,
ViInt16 vxicmd,ViInt16 ttl0,ViInt16 ttl1,ViInt16 ttl2,ViInt16 ttl3,
ViInt16 ttl4,ViInt16 ttl5,ViInt16 ttl6,ViInt16 ttl7);
/***************************************************************************
 * Function: tkvx4244_confGpFourTrigSrcLogic
 *
 * Purpose:
 *  This function sets various trigger parameters for logic 
 *  and source for Group 4.
 * Parameters:
 *	ViSession instrumentHandle: VISA instrument handle used to access instrument
 *		specific data. Must be initialized by tkvx4244_init()
 *		prior to use.
 *  ViInt16 external,immediate,threshold,vxicmd-- ttl0,ttl1,ttl2,ttl3,ttl4,ttl5,ttl6,ttl7
 *  ViInt16 logic
 *  Returns:
 *		VI_SUCCESS if no errors occurred, error code otherwise
 */
ViStatus _VI_FUNC 
tkvx4244_confGpFourTrigSrcLogic (ViSession instrumentHandle,
ViInt16 logic,ViInt16 external,ViInt16 immediate,ViInt16 threshold,
ViInt16 vxicmd,ViInt16 ttl0,ViInt16 ttl1,ViInt16 ttl2,ViInt16 ttl3,
ViInt16 ttl4,ViInt16 ttl5,ViInt16 ttl6,ViInt16 ttl7);
/***************************************************************************
 * Function: tkvx4244_confSynchronousClocks
 *
 * Purpose:
 *  This function synchronizes the clocks for the channel groups.
 * Parameters:
 *	ViSession instrumentHandle: VISA instrument handle used to access instrument
 *		specific data. Must be initialized by tkvx4244_init()
 *		prior to use.
 *  ViInt16 source, tTLxSource
 *  Returns:
 *		VI_SUCCESS if no errors occurred, error code otherwise
 */
ViStatus _VI_FUNC tkvx4244_confSynchronousClocks (ViSession instrumentHandle,ViInt16 source,ViInt16 tTLxSource);
	/*For compatibility with old applications*/ 
#define TKVX4244_synch_clks(a,b,c)  tkvx4244_confSynchronousClocks((a),(b),(c))
/***************************************************************************
 * Function: tkvx4244_confFDCTransfer
 *
 * Purpose:
 *    This function sets up the digitizer for FDC protocol.
 *
 * Parameters:
 *	ViSession instrumentHandle: VISA instrument handle used to access instrument
 *		specific data. Must be initialized by tkvx4244_init()
 *		prior to use.
 *		ViInt16 FDCmode
 *		ViInt16 gp1,ViInt16 gp2,ViInt16 gp3,ViInt16 gp4,ViInt16 allGroups
 *		ViInt32 fdcBufferSize
 *      ViInt32 startingAddress
 *  Returns:
 *		VI_SUCCESS if no errors occurred, error code otherwise
 */
ViStatus _VI_FUNC tkvx4244_confFDCTransfer (ViSession instrumentHandle,
ViInt16 fdcMode,ViInt16 gp1,ViInt16 gp2,ViInt16 gp3,ViInt16 gp4,
ViInt16 allGroups,ViInt32 fdcBufferSize,ViInt32 startingAddress);
/***************************************************************************
 * Function: tkvx4244_getFDCData
 *
 * Purpose:
 *    This function reads FDC data into a short integer array.
 *
 * Parameters:
 *	ViSession instrumentHandle: VISA instrument handle used to access instrument
 *		specific data. Must be initialized by tkvx4244_init()
 *		prior to use.
 *  ViInt16 fdcGroup:
 *  ViInt32 numberOfSamples:
 *  ViInt16 dataArray[]:
 *  Returns:
 *		VI_SUCCESS if no errors occurred, error code otherwise
 */
ViStatus _VI_FUNC
tkvx4244_getFDCData (ViSession instrumentHandle,ViInt16 fdcGroup,
						ViInt32 numberOfSamples,ViInt16 dataArray[]);
/***************************************************************************
 * Function: tkvx4244_readFDCData
 *
 * Purpose:
 *    This function reads FDC data.
 *
 * Parameters:
 *	ViSession instrumentHandle: VISA instrument handle used to access instrument
 *		specific data. Must be initialized by tkvx4244_init()
 *		prior to use.
 *  ViInt16 fdcGroup:
 *  ViInt32 numberOfSamples:
 *  ViChar dataArray[]:
 *  Returns:
 *		VI_SUCCESS if no errors occurred, error code otherwise
 */
ViStatus _VI_FUNC
tkvx4244_readFDCData (ViSession instrumentHandle,ViInt16 fdcGroup,
ViInt32 numberOfSamples,ViChar dataArray[]);
/***************************************************************************
 * Function: tkvx4244_readFDCState
 *
 * Purpose:
 *    This function reads FDC state information.
 *
 * Parameters:
 *	ViSession instrumentHandle: VISA instrument handle used to access instrument
 *		specific data. Must be initialized by tkvx4244_init()
 *		prior to use.
 *  ViInt16 stateQuery, gp1,gp2,gp3,gp4,all:
 *  ViChar fdcQueryResponse[]:
 *  Returns:
 *		VI_SUCCESS if no errors occurred, error code otherwise
 */
ViStatus _VI_FUNC
tkvx4244_readFDCState (ViSession instrumentHandle,ViInt16 stateQuery,ViInt16 gp1,
ViInt16 gp2,ViInt16 gp3,ViInt16 gp4,ViInt16 all,
ViChar fdcQueryResponse[]);
/***************************************************************************
 * Function: tkvx4244_readASCIIData
 *
 * Purpose:
 *  This function allows retrieval of raw data in ASCII.
 * Parameters:
 *	ViSession instrumentHandle: VISA instrument handle used to access instrument
 *		specific data. Must be initialized by tkvx4244_init()
 *		prior to use.
 *  ViInt32 startingAddress
 *  ViInt16 numberOfSamples, channel
 *  ViChar dataArray
 *  Returns:
 *		VI_SUCCESS if no errors occurred, error code otherwise
 */
ViStatus _VI_FUNC tkvx4244_readASCIIData (ViSession instrumentHandle,ViInt16 numberOfSamples,ViInt32 startingAddress,ViInt16 channel,ViChar dataArray[]);
	/*For compatibility with old applications*/ 
#define TKVX4244_inpt_raw_data(a,b,c,d,e)  tkvx4244_readASCIIData((a),(b),(c),(d),(e))
/***************************************************************************
 * Function: tkvx4244_readBinaryData
 * NOTE:This function has been replaced by tkvx4244_getBinaryData. The use 
 * of a ViChar array to transfer data causes major problems when using
 * LabView. Therefore, this function is being kept for backward compatibility
 * only.
 * Purpose:
 *  This function allows retrieval of raw data in Binary.
 * Parameters:
 *	ViSession instrumentHandle: VISA instrument handle used to access instrument
 *		specific data. Must be initialized by tkvx4244_init()
 *		prior to use.
 *  ViInt16 numberOfSamples:
 *  ViInt32 startingAddress:
 *  ViInt16 channel:
 *  ViChar response[]:
 *  Returns:
 *		VI_SUCCESS if no errors occurred, error code otherwise
 */
ViStatus _VI_FUNC 
tkvx4244_readBinaryData (ViSession instrumentHandle,ViInt16 numberOfSamples,
ViInt32 startingAddress,ViInt16 channel,ViChar response[]);
	/*For compatibility with old applications*/ 
#define TKVX4244_inpt_bin(a,b,c,d,e)  tkvx4244_readBinaryData((a),(b),(c),(d),(e))
/***************************************************************************
 * Function: tkvx4244_getBinaryData
 *
 * Purpose:
 *  This function allows retrieval of raw data in Binary.
 * Parameters:
 *	ViSession instrumentHandle: VISA instrument handle used to access instrument
 *		specific data. Must be initialized by tkvx4244_init()
 *		prior to use.
 *  ViInt16 numberOfSamples:
 *  ViInt32 startingAddress:
 *  ViInt16 channel:
 *  ViInt16 response[]:
 *  Returns:
 *		VI_SUCCESS if no errors occurred, error code otherwise
 */
ViStatus _VI_FUNC tkvx4244_getBinaryData (ViSession instrumentHandle,	
						ViInt16 numberOfSamples,ViInt32 startingAddress,
										ViInt16 channel,ViInt16 response[]);
/***************************************************************************
 * Function: tkvx4244_getBinaryDataSFP
 *
 * Purpose:
 *  This function allows retrieval of raw data in Binary put into 
 *  proper order to use in the SFP.
 * Parameters:
 *	ViSession instrumentHandle: VISA instrument handle used to access instrument
 *		specific data. Must be initialized by tkvx4244_init()
 *		prior to use.
 *  ViInt16 numberOfSamples:
 *  ViInt32 startingAddress:
 *  ViInt16 channel:
 *  ViInt16 response[]:
 *  Returns:
 *		VI_SUCCESS if no errors occurred, error code otherwise
 */
ViStatus _VI_FUNC tkvx4244_getBinaryDataSFP (ViSession instrumentHandle,
							ViInt16 numberOfSamples,ViInt32 startingAddress,
										ViInt16 channel, ViInt16 response[]);
/***************************************************************************
 * Function: tkvx4244_readTimetag
 *
 * Purpose:
 *  This function reads the time tag values of the specified group(s).
 * Parameters:
 *	ViSession instrumentHandle: VISA instrument handle used to access instrument
 *		specific data. Must be initialized by tkvx4244_init()
 *		prior to use.
 *  ViInt16 gp1-gp4,all
 *  ViChar timeTag[]
 *  Returns:
 *		VI_SUCCESS if no errors occurred, error code otherwise
 */
ViStatus _VI_FUNC 
tkvx4244_readTimetag (ViSession instrumentHandle,ViInt16 gp1,ViInt16 gp2,
ViInt16 gp3,ViInt16 gp4,ViInt16 all,ViChar timeTag[]);
	/*For compatibility with old applications*/ 
#define TKVX4244_read_timetag(a,b,c,d,e,f,g)  tkvx4244_readTimetag((a),(b),(c),(d),(e),(f),(g))
/***************************************************************************
 * Function: tkvx4244_confFrontPanelOutTrig
 *
 * Purpose:
 *  This function controls which group output is routed to the trig and
 *  clock outputs.
 *
 * Parameters:
 *	ViSession instrumentHandle: VISA instrument handle used to access instrument
 *		specific data. Must be initialized by tkvx4244_init()
 *		prior to use.
 *  ViInt16 groupNumber
 *  ViChar groupOutput[]
 *  Returns:
 *		VI_SUCCESS if no errors occurred, error code otherwise
 */
ViStatus _VI_FUNC tkvx4244_confFrontPanelOutTrig (ViSession instrumentHandle,ViInt16 groupNumber,ViChar groupOutput[]);
/*For compatibility with old applications*/ 
#define TKVX4244_output_gp(a,b,c)  tkvx4244_confFrontPanelOutTrig((a),(b),(c))
/***************************************************************************
 * Function: tkvx4244_readSyncClkSource
 *
 * Purpose:
 * This function queries the instrument for synch clock setups.
 *
 * Parameters:
 *	ViSession instrumentHandle: VISA instrument handle used to access instrument
 *		specific data. Must be initialized by tkvx4244_init()
 *		prior to use.
 *  ViInt16 synchronize
 *  ViChar activeLevel[]
 *  Returns:
 *		VI_SUCCESS if no errors occurred, error code otherwise
 */
ViStatus _VI_FUNC 
tkvx4244_readSyncClkSource (ViSession instrumentHandle,
ViInt16 synchronize,ViChar activeLevel[]);
	/*For compatibility with old applications*/ 
#define TKVX4244_synch_clks_query(a,b,c)  tkvx4244_readSyncClkSource((a),(b),(c))
/***************************************************************************
 * Function: tkvx4244_readActiveChannels
 *
 * Purpose:
 * This function queries the instrument for the active channels.
 *
 * Parameters:
 *	ViSession instrumentHandle: VISA instrument handle used to access instrument
 *		specific data. Must be initialized by tkvx4244_init()
 *		prior to use.
 *  ViInt16 queryCondition
 *  ViChar list[]
 *  Returns:
 *		VI_SUCCESS if no errors occurred, error code otherwise
 */
ViStatus _VI_FUNC 
tkvx4244_readActiveChannels (ViSession instrumentHandle,
ViInt16 queryCondition,ViChar list[]);
	/*For compatibility with old applications*/ 
#define TKVX4244_act_ch_query(a,b,c)  tkvx4244_readActiveChannels((a),(b),(c))
/***************************************************************************
 * Function: tkvx4244_readSampleRates
 *
 * Purpose:
 * This function reads the sample rate setup of the specified group(s).
 *
 * Parameters:
 *	ViSession instrumentHandle: VISA instrument handle used to
 *  access instrument specific data. Must be initialized by
 *  tkvx4244_init() prior to use.
 *  ViInt16 definitions: Type of query.
 *  ViChar response[]: buffer for response.
 *  ViInt16 gp1-gp4,all: Select group(s) for query.
 *  Returns:
 *		VI_SUCCESS if no errors occurred, error code otherwise
 */
ViStatus _VI_FUNC 
tkvx4244_readSampleRates (ViSession instrumentHandle,
ViInt16 definitions,ViInt16 gp1,ViInt16 gp2,ViInt16 gp3,ViInt16 gp4,
ViInt16 all,ViChar response[]);
	/*For compatibility with old applications*/ 
#define TKVX4244_samp_rate_query(a,b,c,d,e,f,g,h)  tkvx4244_readSampleRates((a),(b),(c),(d),(e),(f),(g),(h))
/***************************************************************************
 * Function: tkvx4244_readCollectionCount
 *
 * Purpose:
 * This function reads the collection setup of the specified group(s).
 *
 * Parameters:
 *	ViSession instrumentHandle: VISA instrument handle used to access instrument
 *		specific data. Must be initialized by tkvx4244_init()
 *		prior to use.
 *  ViChar count[]
 *  ViInt16 gp1-gp4,all
 *  Returns:
 *		VI_SUCCESS if no errors occurred, error code otherwise
 */
ViStatus _VI_FUNC 
tkvx4244_readCollectionCount (ViSession instrumentHandle,
ViInt16 gp1,ViInt16 gp2,ViInt16 gp3,ViInt16 gp4,ViInt16 all,
ViChar count[]);
	/*For compatibility with old applications*/ 
#define TKVX4244_col_count_query(a,b,c,d,e,f,g)  tkvx4244_readCollectionCount((a),(b),(c),(d),(e),(f),(g))
/***************************************************************************
 * Function: tkvx4244_readArmSrcZero
 *
 * Purpose:
 *  This function queries setup of arm source and memory zero for the
 *  specified group(s).
 *
 * Parameters:
 *	ViSession instrumentHandle: VISA instrument handle used to access instrument
 *		specific data. Must be initialized by tkvx4244_init()
 *		prior to use.
 *  ViInt16 armSourceOrMemoryZero
 *  ViChar armOrMemoryZeroResponse[]
 *  ViInt16 gp1-gp4,all
 *  Returns:
 *		VI_SUCCESS if no errors occurred, error code otherwise
 */
ViStatus _VI_FUNC 
tkvx4244_readArmSrcZero (ViSession instrumentHandle,
ViInt16 armSourceOrMemoryZero,ViInt16 gp1,ViInt16 gp2,ViInt16 gp3,
ViInt16 gp4,ViInt16 all,ViChar armOrMemoryZeroResponse[]);
/*For compatibility with old applications*/ 
#define TKVX4244_arm_source_query(a,b,c,d,e,f,g,h)  tkvx4244_readArmSrcZero((a),(b),(c),(d),(e),(f),(g),(h))
/***************************************************************************
 * Function: tkvx4244_readInputVoltageSettings
 *
 * Purpose:
 *  This function queries the input voltage ranges of the channels.
 *
 * Parameters:
 *	ViSession instrumentHandle: VISA instrument handle used to access instrument
 *		specific data. Must be initialized by tkvx4244_init()
 *		prior to use.
 *  ViInt16 rangeQuery
 *  ViChar inputVoltage[]
 *  Returns:
 *		VI_SUCCESS if no errors occurred, error code otherwise
 */
ViStatus _VI_FUNC 
tkvx4244_readInputVoltageSettings (ViSession instrumentHandle,
ViInt16 rangeQuery,ViChar inputVoltage[] );
/***************************************************************************
 * Function: tkvx4244_volt_rang_query
 *
 * Purpose:
 *	 NOTE: This Function is an "Exception Function" and is not
 *compliant with VXIplu&play standards because of the number of
 *parameters in the function. This function is being maintained in
 *this version of the VXIplug&play driver to provide compatibility
 *with existing applications. New functions have been written to
 *replace this function.  There will not be a function panel for
 *this function in WIN Framework 4.0 compliant drivers.
 *
 *  This function queries the input voltage ranges of the channels.
 *
 * Parameters:
 *	ViSession instrumentHandle: VISA instrument handle used to access instrument
 *		specific data. Must be initialized by tkvx4244_init()
 *		prior to use.
 *  ViInt16 rang
 *  ViChar answer[]
 *  ViInt16 c1-c16,ca
 *  Returns:
 *		VI_SUCCESS if no errors occurred, error code otherwise
 */
ViStatus _VI_FUNC tkvx4244_volt_rang_query (ViSession instrumentHandle,ViInt16 rang,ViInt16 c1,ViInt16 c2,ViInt16 c3,ViInt16 c4,ViInt16 c5
,ViInt16 c6,ViInt16 c7,ViInt16 c8,ViInt16 c9,ViInt16 c10,ViInt16 c11,ViInt16 c12,ViInt16 c13,ViInt16 c14
,ViInt16 c15,ViInt16 c16,ViInt16 ca,ViChar answer[]);
	/*For compatibility with old applications*/ 
#define TKVX4244_volt_rang_query(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t)  tkvx4244_volt_rang_query((a),(b),(c),(d),(e),(f),(g),(h),(i),(j),(k),(l),(m),(n),(o),(p),(q),(r),(s),(t))
/***************************************************************************
 * Function: tkvx4244_confExtClkSlopAndTtag
 *
 * Purpose:
 * This function sets the active edge of the external clock to either
 * pos true or neg true.  It also sets the source of the Timetag clk.
 * Parameters:
 *	ViSession instrumentHandle: VISA instrument handle used to access instrument
 *		specific data. Must be initialized by tkvx4244_init()
 *		prior to use.
 *  ViInt16 slope
 *  ViInt16 timeTag
 *  ViChar clockSlope[],
 *  ViChar timeTagSource[]
 *  Returns:
 *		VI_SUCCESS if no errors occurred, error code otherwise
 */
ViStatus _VI_FUNC 
tkvx4244_confExtClkSlopAndTtag (ViSession instrumentHandle,
ViInt16 slope,ViInt16 timeTag,ViChar clockSlope[],
ViChar timeTagSource[]);
	/*For compatibility with old applications*/ 
#define TKVX4244_clk_slop_ttag(a,b,c,d,e)  tkvx4244_confExtClkSlopAndTtag((a),(b),(c),(d),(e))
/***************************************************************************
 * Function: tkvx4244_eventReqEnable
 *
 * Purpose:
 *  This function defines the mask for event status reporting and for
 *  generating VXI Request True interrupts.
 * Parameters:
 *	ViSession instrumentHandle: VISA instrument handle used to access instrument
 *		specific data. Must be initialized by tkvx4244_init()
 *		prior to use.
 *  ViInt16 requestEventEnab
 *  ViInt16 maskDefinition
 *  Returns:
 *		VI_SUCCESS if no errors occurred, error code otherwise
 */
ViStatus _VI_FUNC 
tkvx4244_eventReqEnable (ViSession instrumentHandle,
ViInt16 requestEventEnab,ViInt16 maskDefinition);
/*For compatibility with old applications*/ 
#define TKVX4244_even_req_enable(a,b,c)  tkvx4244_eventReqEnable((a),(b),(c))
/***************************************************************************
 * Function: tkvx4244_readStatus
 *
 * Purpose:
 *	This function returns the results of some standard 488.2 queries.
 *
 * Parameters:
 *	ViSession instrumentHandle: VISA instrument handle used to access instrument
 *		specific data. Must be initialized by tkvx4244_init()
 *		prior to use.
 *  ViInt16 select4882Command
 *  ViChar get4882Status[]
 *  Returns:
 *		VI_SUCCESS if no errors occurred, error code otherwise
 */
ViStatus _VI_FUNC 
tkvx4244_readStatus (ViSession instrumentHandle,
ViInt16 select4882Command,ViChar get4882Status[]);
	/*For compatibility with old applications*/ 
#define TKVX4244_4882_queries(a,b,c)  tkvx4244_readStatus((a),(b),(c))
/***************************************************************************
 * Function: tkvx4244_readPostprocessCalc
 *
 * Purpose:
 *  This function allows retrieval of some basic signal parameters.  The
 *  parameters are maximum, minimum, average, positive transition and
 *  negative transition.
 * Parameters:
 *	ViSession instrumentHandle: VISA instrument handle used to access instrument
 *		specific data. Must be initialized by tkvx4244_init()
 *		prior to use.
 *  ViInt16 measurement:
 *  ViInt32 numberOfSamples:
 *  ViInt32 startingAddress:
 *  ViInt16 channel:
 *  ViChar response[]:
 *  Returns:
 *		VI_SUCCESS if no errors occurred, error code otherwise
 */
ViStatus _VI_FUNC 
tkvx4244_readPostprocessCalc (ViSession instrumentHandle,
ViInt16 measurement,ViInt32 numberOfSamples,ViInt32 startingAddress,
ViInt16 channel,ViChar response[]);
	/*For compatibility with old applications*/ 
#define TKVX4244_rddata(a,b,c,d,e,f)  tkvx4244_readPostprocessCalc((a),(b),(c),(d),(e),(f))
/***************************************************************************
 * Function: tkvx4244_initStopMeasurementCycle
 *
 * Purpose:
 *  This function aborts the collection process on the designated groups.
 * Parameters:
 *	ViSession instrumentHandle: VISA instrument handle used to access instrument
 *		specific data. Must be initialized by tkvx4244_init()
 *		prior to use.
 *  ViInt16 gp1-gp4,all
 *  Returns:
 *		VI_SUCCESS if no errors occurred, error code otherwise
 */
ViStatus _VI_FUNC 
tkvx4244_initStopMeasurementCycle (ViSession instrumentHandle,
ViInt16 gp1,ViInt16 gp2,ViInt16 gp3,ViInt16 gp4,ViInt16 all);
	/*For compatibility with old applications*/ 
#define TKVX4244_abort_samp(a,b,c,d,e,f)  tkvx4244_initStopMeasurementCycle((a),(b),(c),(d),(e),(f))
/***************************************************************************
 * Function: tkvx4244_confMastertrigOut
 *
 * Purpose:
 *  This function controls the master trig output of the module.
 * Parameters:
 *	ViSession instrumentHandle: VISA instrument handle used to
 *  access instrument specific data. Must be initialized by 
 *  tkvx4244_init()	prior to use.
 *  ViInt16 outputSelect:Which TTLtrg line to use.
 *  ViInt16 logic: Logic AND or OR.
 *  ViInt16 slope: Pos or Neg.
 *  ViInt16 gp1,gp2,gp3,gp4: Select group(s).
 *  Returns:
 *		VI_SUCCESS if no errors occurred, error code otherwise
 */
ViStatus _VI_FUNC 
tkvx4244_confMastertrigOut (ViSession instrumentHandle,
ViInt16 outputSelect,ViInt16 logic,ViInt16 slope,ViInt16 gp1,
ViInt16 gp2,ViInt16 gp3,ViInt16 gp4);
	/*For compatibility with old applications*/ 
#define TKVX4244_out_trig_parms(a,b,c,d,e,f,g,h)  tkvx4244_confMastertrigOut((a),(b),(c),(d),(e),(f),(g),(h))
/***************************************************************************
 * Function: tkvx4244_confTriggerThreshold
 *
 * Purpose:
 *  This function sets trigger threshold settings.
 * Parameters:
 *	ViSession instrumentHandle: VISA instrument handle used to
 *  access instrument specific data. Must be initialized by 
 *  tkvx4244_init()	prior to use.
 *  ViInt16 thresholdDescriptor
 *  ViReal64 voltageLevel
 *  ViInt16 group1
 *  ViInt16 group2
 *  ViInt16 group3
 *  ViInt16 group4
 *  Returns:
 *		VI_SUCCESS if no errors occurred, error code otherwise
 */
ViStatus _VI_FUNC 
tkvx4244_confTriggerThreshold (ViSession instrumentHandle,ViInt16 thresholdDescriptor,
ViReal64 voltageLevel,ViInt16 group1,ViInt16 group2,ViInt16 group3,
ViInt16 group4);
	/*For compatibility with old applications*/ 
#define TKVX4244_trig_thresh(a,b,c,d,e,f,g)  tkvx4244_confTriggerThreshold((a),(b),(c),(d),(e),(f),(g))
/***************************************************************************
 * Function: tkvx4244_confArmSrcMemZero
 *
 * Purpose:
 *  This function defines various conditions for arming the group triggers .
 * Parameters:
 *	ViSession instrumentHandle: VISA instrument handle used to access instrument
 *		specific data. Must be initialized by tkvx4244_init()
 *		prior to use.
 *  ViInt16 source
 *  ViInt16 memoryZero
 *  ViInt16 gp1-gp4,allGroups
 *  Returns:
 *		VI_SUCCESS if no errors occurred, error code otherwise
 */
ViStatus _VI_FUNC 
tkvx4244_confArmSrcMemZero (ViSession instrumentHandle,ViInt16 source,
ViInt16 gp1,ViInt16 gp2,ViInt16 gp3,ViInt16 gp4,ViInt16 allGroups,
ViInt16 memoryZero);
#define TKVX4244_ext_arm(a,b,c,d,e,f,g,h)  tkvx4244_confArmSrcMemZero((a),(b),(c),(d),(e),(f),(g),(h))
/***************************************************************************
 * Function: tkvx4244_confFrontPanelDisplay
 *
 * Purpose:
 *  This function controls the LED display on the face plate of the card.
 * Parameters:
 *	ViSession instrumentHandle: VISA instrument handle used to access instrument
 *		specific data. Must be initialized by tkvx4244_init()
 *		prior to use.
 *  ViChar groupDisplayed[]
 *  ViInt16 groupNumber
 *  Returns:
 *		VI_SUCCESS if no errors occurred, error code otherwise
 */
ViStatus _VI_FUNC 
tkvx4244_confFrontPanelDisplay (ViSession instrumentHandle,
ViInt16 groupNumber,ViChar groupDisplayed[]);
	/*For compatibility with old applications*/ 
#define TKVX4244_dis_grp(a,b,c)  tkvx4244_confFrontPanelDisplay((a),(b),(c))
/***************************************************************************
 * Function: tkvx4244_confGpOneVoltageRange
 *
 * Purpose:
 *  This function sets the input voltage ranges of the channels
 *  in Group 1.
 * Parameters:
 *	ViSession instrumentHandle: VISA instrument handle used to access instrument
 *		specific data. Must be initialized by tkvx4244_init()
 *		prior to use.
 *  ViInt16 range,ch1-ch4
 *  Returns:
 *		VI_SUCCESS if no errors occurred, error code otherwise
 */
ViStatus _VI_FUNC 
tkvx4244_confGpOneVoltageRange (ViSession instrumentHandle,
ViInt16 ch1,ViInt16 ch2,ViInt16 ch3,ViInt16 ch4,ViInt16 range);
/***************************************************************************
 * Function: tkvx4244_confGpTwoVoltageRange
 *
 * Purpose:
 *  This function sets the input voltage ranges of the channels
 *  in Group 2.
 * Parameters:
 *	ViSession instrumentHandle: VISA instrument handle used to access instrument
 *		specific data. Must be initialized by tkvx4244_init()
 *		prior to use.
 *  ViInt16 range,ch5-ch8
 *  Returns:
 *		VI_SUCCESS if no errors occurred, error code otherwise
 */
ViStatus _VI_FUNC 
tkvx4244_confGpTwoVoltageRange (ViSession instrumentHandle,
ViInt16 ch5,ViInt16 ch6,ViInt16 ch7, ViInt16 ch8,ViInt16 range);
/***************************************************************************
 * Function: tkvx4244_confGpThreeVoltageRange
 *
 * Purpose:
 *  This function sets the input voltage ranges of the channels
 *  in Group 3.
 * Parameters:
 *	ViSession instrumentHandle: VISA instrument handle used to access instrument
 *		specific data. Must be initialized by tkvx4244_init()
 *		prior to use.
 *  ViInt16 range,ch9-ch12
 *  Returns:
 *		VI_SUCCESS if no errors occurred, error code otherwise
 */
ViStatus _VI_FUNC 
tkvx4244_confGpThreeVoltageRange (ViSession instrumentHandle,
ViInt16 ch9,ViInt16 ch10,ViInt16 ch11,ViInt16 ch12,ViInt16 range);
/***************************************************************************
 * Function: tkvx4244_confGpFourVoltageRange
 *
 * Purpose:
 *  This function sets the input voltage ranges of the channels
 *  in Group 4.
 * Parameters:
 *	ViSession instrumentHandle: VISA instrument handle used to access instrument
 *		specific data. Must be initialized by tkvx4244_init()
 *		prior to use.
 *  ViInt16 range,ch13-ch16
 *  Returns:
 *		VI_SUCCESS if no errors occurred, error code otherwise
 */
ViStatus _VI_FUNC 
tkvx4244_confGpFourVoltageRange (ViSession instrumentHandle,
ViInt16 ch13,ViInt16 ch14,ViInt16 ch15,ViInt16 ch16,ViInt16 range);
/***************************************************************************
 * Function: tkvx4244_volt_rang
 *
 * Purpose:
 *	 NOTE: This Function is an "Exception Function" and is not
 *compliant with VXIplu&play standards because of the number of
 *parameters in the function. This function is being maintained in
 *this version of the VXIplug&play driver to provide compatibility
 *with existing applications. New functions have been written to
 *replace this function.  There will not be a function panel for
 *this function in WIN Framework 4.0 compliant drivers.
 *  This function sets the input voltage ranges of the channels.
 * Parameters:
 *	ViSession instrumentHandle: VISA instrument handle used to access instrument
 *		specific data. Must be initialized by tkvx4244_init()
 *		prior to use.
 *  ViInt16 vlts,c1-c16,ca
 *  Returns:
 *		VI_SUCCESS if no errors occurred, error code otherwise
 */
ViStatus _VI_FUNC tkvx4244_volt_rang (ViSession instrumentHandle,ViInt16 vlts,ViInt16 c1,ViInt16 c2,ViInt16 c3,ViInt16 c4
,ViInt16 c5,ViInt16 c6,ViInt16 c7,ViInt16 c8,ViInt16 c9,ViInt16 c10,ViInt16 c11,ViInt16 c12,ViInt16 c13
,ViInt16 c14,ViInt16 c15,ViInt16 c16,ViInt16 ca);
	/*For compatibility with old applications*/ 
#define TKVX4244_volt_rang(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s)  tkvx4244_volt_rang((a),(b),(c),(d),(e),(f),(g),(h),(i),(j),(k),(l),(m),(n),(o),(p),(q),(r),(s))
/***************************************************************************
 * Function: tkvx4244_confSamplingInterval
 *
 * Purpose:
 *  This function sets the Interval or frequency for the selected groups.
 * Parameters:
 *	ViSession instrumentHandle: VISA instrument handle used to access instrument
 *		specific data. Must be initialized by tkvx4244_init()
 *		prior to use.
 *  ViInt16 source:
 *  ViInt16 intervalFrequency:
 *  ViReal64 samplingRate:
 *  ViInt16 gp1,gp2, gp3, gp4,allGroups
 *  Returns:
 *		VI_SUCCESS if no errors occurred, error code otherwise
 */
ViStatus _VI_FUNC 
tkvx4244_confSamplingInterval (ViSession instrumentHandle,
ViInt16 source,ViInt16 intervalFrequency,ViReal64 samplingRate,ViInt16 gp1,
ViInt16 gp2,ViInt16 gp3,ViInt16 gp4,ViInt16 allGroups);
/*For compatibility with old applications*/ 
#define TKVX4244_samp_freq(a,b,c,d,e,f,g,h,i)   tkvx4244_confSamplingInterval((a),(b),(c),(d),(e),(f),(g),(h),(i))
/***************************************************************************
 * Function: tkvx4244_autoConnectToAll
 *
 * Purpose:
 *	Connect to all tkvx4244 instruments found.
 *
 * Parameters:
 *	ViSession instrumentArray[]: Array of VISA instrument handles used to access
 *		instrument specific data. One handle initialized by this routine
 *		for each instrument found.
 *	ViInt16 arrayLength: Number of entries which can be put in instrArray
 *	ViPInt16 numberConnected: Number of valid entries in instrArray upon return
 *
 * Returns:
 *	VI_SUCCESS if no errors occurred, error code otherwise
 */

ViStatus _VI_FUNC tkvx4244_autoConnectToAll(ViSession instrumentArray[],
			ViInt16 arrayLength, ViPInt16 numberConnected);
	/*define for backward compatibility*/
#define TKVX4244_autoConnectToAll(a,b,c) tkvx4244_autoConnectToAll((a),(b),(c))
/***************************************************************************
 * Function: tkvx4244_autoConnectToFirst
 *
 * Purpose:
 *		Connect to first tkvx4244 instrument found
 *
 * Parameters:
 *	ViPSession instrumentHandle: Address of VISA instrument handle used to
 *						access instrument specific data. Initialized by
 *						this routine.
 *
 * Returns:
 *	VI_SUCCESS if no errors occurred, error code otherwise
 */

ViStatus _VI_FUNC tkvx4244_autoConnectToFirst(ViPSession instrumentHandle);
	/*define for backward compatibility*/
#define TKVX4244_autoConnectToFirst(a) tkvx4244_autoConnectToFirst((a))
/***************************************************************************
 * Function: tkvx4244_autoConnectToLA
 *
 * Purpose:
 *	Connect to instrument in selected slot.
 *
 * Parameters:
 *	ViPSession instrumentHandle: Address of VISA instrument handle used to
 *						access instrument specific data. Initialized by
 *						this routine.
 *	ViInt16 logicalAddress: Logical address to connect to
 *
 * Returns:
 *	VI_SUCCESS if no errors occurred, error code otherwise
 */

ViStatus _VI_FUNC tkvx4244_autoConnectToLA(ViPSession instrumentHandle,
			ViInt16 logicalAddress);
	/*define for backward compatibility*/
#define TKVX4244_autoConnectToLA(a,b) tkvx4244_autoConnectToLA((a),(b))
/***************************************************************************
 * Function: tkvx4244_autoConnectToSlot
 *
 * Purpose:
 *	Connect to instrument in selected slot.
 *
 * Parameters:
 *	ViSession instrumentArray[]: 	Array of VISA instrument handles used
 *		to access instrument specific data. One handle initialized
 *		by this routine for each instrument found.
 *	ViInt16 arrayLength: 	Number of entries which can be put in
 *      instrumentArray
 *	ViPInt16 numberConnected: Number of valid entries in instrumentArray
 *		upon return
 *	ViInt16 slot: 		Slot to connect to
 *
 * Returns:
 *	VI_SUCCESS if no errors occurred, error code otherwise
 */

ViStatus _VI_FUNC tkvx4244_autoConnectToSlot(ViSession instrumentArray[],
		ViInt16 arrayLength, ViPInt16 numberConnected, ViInt16 slot);
	/*define for backward compatibility*/
#define TKVX4244_autoConnectToSlot(a,b,c,d) tkvx4244_autoConnectToSlot((a),(b),(c),(d))
/***************************************************************************
 * Function: tkvx4244_close
 *
 * Purpose:
 *	This function closes the instrument and returns memory allocated for
 *	instrument specific data storage.
 *
 * CHANGE: add deletion mechanisms here for any dynamically allocated
 *	data structures added to the driver structure.
 *
 * Parameters:
 *	ViSession instrumentHandle: VISA instrument handle used to access instrument
 *						specific data.
 *
 * Returns:
 *	VI_SUCCESS if no errors occurred, error code otherwise
 *
 * 	NOTE!!!! instrumentHandle is no longer valid, but cannot be changed by
 *	this routine. It is important that users do not use this handle
 *	after calling this routine.
 */

ViStatus _VI_FUNC tkvx4244_close (ViSession instrumentHandle);
	/*define for backward compatibility*/
#define TKVX4244_close(a) tkvx4244_close((a))
/***************************************************************************
 * Function: tkvx4244_init
 *
 * Purpose:
 *	This function opens the instrument, and depending upon flags passed
 *	in, queries for ID and initializes the instrument to a known state.
 *
 * Parameters:
 *	ViRsrc resourceName: Instrument search expression, see VPP-3.3, 3.2.1.1
 *	ViBoolean IDQuery: Boolean flag, VI_TRUE -> verify correct instrument
 *	ViBoolean resetDevice: Boolean flag, VI_TRUE -> reset instrument
 *	ViPSession instrumentHandle: VISA instrument handle used to access instrument
 *						 specific data. The handle is initialized by
 *						 this routine.
 *
 * Returns:
 *	VI_SUCCESS if no errors occurred, error code otherwise
 *	If no error, instrumentHandle is initialized for use in rest of driver
 */

ViStatus _VI_FUNC tkvx4244_init (ViRsrc resourceName, ViBoolean IDQuery,
			ViBoolean resetDevice, ViPSession instrumentHandle); /* VPP 3.1 Rule 5.6 - 5.10 */
 	/*define for backward compatibility*/
#define TKVX4244_init(a,b,c,d) tkvx4244_init((a),(b),(c),(d))
/***************************************************************************
 * Function: tkvx4244_error_message
 *
 * Purpose:
 *	This function returns a text message for a corresponding instrument
 *	driver error code.
 *
 * Parameters:
 *	ViSession instrumentHandle: VISA instrument handle used to access instrument
 *		specific data.
 *	ViStatus errorCode: error code to check
 *	ViChar errorMessage[]: Returned error string
 *
 * Returns:
 *	VI_SUCCESS if no errors occurred, error code otherwise
 */

ViStatus _VI_FUNC tkvx4244_error_message (ViSession instrumentHandle, ViStatus errorCode,
			ViChar errorMessage[]);
	/*define for backward compatibility*/
#define TKVX4244_error_message(a,b,c) tkvx4244_error_message((a),(b),(c))
/***************************************************************************
 * Function: tkvx4244_error_query
 *
 * Purpose:
 *	This function queries the instrument for errors.
 *
 * Parameters:
 *	ViSession instrumentHandle: VISA instrument handle used to access instrument
 *			specific data.
 *	ViPStatus errorCode: Instrument error code returned
 *	ViChar errorMessage[]: Textual version of returned error code
 *
 * Returns:
 *	VI_SUCCESS if no errors occurred, error code otherwise
 */

ViStatus _VI_FUNC tkvx4244_error_query (ViSession instrumentHandle, ViPInt32 errorCode,
			ViChar errorMessage[]);		/* VPP 3.1 Rule 5.14 */
	/*define for backward compatibility*/
#define TKVX4244_error_query(a,b,c) tkvx4244_error_query((a),(b),(c))
/***************************************************************************
 * Function: tkvx4244_getInstrDesc
 *
 * Purpose:
 *	Return instrument descriptor string of instrument
 *
 * Parameters:
 *	ViSession instrumentHandle: VISA instrument handle used to access instrument
 *		specific data.
 *	ViChar instrumentDescriptor[]: Storage for returned instrument descriptor
 *
 * Returns:
 *	VI_SUCCESS if no errors occurred, error code otherwise
 */

ViStatus _VI_FUNC tkvx4244_getInstrDesc(ViSession instrumentHandle,
			ViChar instrumentDescriptor[]);
	/*define for backward compatibility*/
#define TKVX4244_getInstrDesc(a,b) tkvx4244_getInstrDesc((a),(b))
/***************************************************************************
 * Function: tkvx4244_getLogicalAddress
 *
 * Purpose:
 *	Return logical address of instrument
 *
 * Parameters:
 *	ViSession instrumentHandle: VISA instrument handle used to access instrument
 *		specific data.
 *	ViPInt16 logicalAddress: Storage for returned logical address #
 *
 * Returns:
 *	VI_SUCCESS if no errors occurred, error code otherwise
 */

ViStatus _VI_FUNC tkvx4244_getLogicalAddress(ViSession instrumentHandle, ViPInt16 logicalAddress);
	/*define for backward compatibility*/
#define TKVX4244_getLogicalAddress(a,b) tkvx4244_getLogicalAddress((a),(b))
/***************************************************************************
 * Function: tkvx4244_getManufacturerID
 *
 * Purpose:
 *	Returns manufacturer ID of instrument
 *
 * Parameters:
 *	ViSession instrumentHandle: VISA instrument handle used to access instrument
 *		specific data.
 *	ViPInt16 manufacturerIdentification: pointer to storage for ID
 *
 * Returns:
 *	VI_SUCCESS if no errors occurred, error code otherwise
 */

ViStatus _VI_FUNC tkvx4244_getManufacturerID (ViSession instrumentHandle, ViPInt16 manufacturerIdentification);
	/*define for backward compatibility*/
#define TKVX4244_getManufacturerID(a,b) tkvx4244_getManufacturerID((a),(b))
/***************************************************************************
 * Function: tkvx4244_getModelCode
 *
 * Purpose:
 *	Returns model code of instrument
 *
 * Parameters:
 *	ViSession instrumentHandle: VISA instrument handle used to access instrument
 *		specific data.
 *	ViPInt16 modelCode: pointer to storage for code
 *
 * Returns:
 *	VI_SUCCESS if no errors occurred, error code otherwise
 */

ViStatus _VI_FUNC tkvx4244_getModelCode (ViSession instrumentHandle, ViPInt16 modelCode);
	/*define for backward compatibility*/
#define TKVX4244_getModelCode(a,b) tkvx4244_getModelCode((a),(b))
/***************************************************************************
 * Function: tkvx4244_getSlotandLAList
 *
 * Purpose:
 *	Return lists of slot numbers and logical address for all tkvx4244's.
 *
 * Parameters:
 *	ViInt16 slotArray[]: array of slot #s
 *	ViInt16 logicalAddressArray[]: array of logical addresses
 *	ViInt16  arrayLength: length of these arrays
 *	ViPInt16 numberFound: Return value for number of matching instruments found
 *
 * Returns:
 *	VI_SUCCESS if no errors occurred, error code otherwise
 */

ViStatus _VI_FUNC tkvx4244_getSlotandLAList(ViInt16 slotArray[],
			ViInt16 logicalAddressArray[], ViInt16 arrayLength, ViPInt16 numberFound);
	/*define for backward compatibility*/
#define TKVX4244_getSlotandLAList(a,b,c,d) tkvx4244_getSlotandLAList((a),(b),(c),(d))
/***************************************************************************
 * Function: tkvx4244_getSlotNumber
 *
 * Purpose:
 *	Return slot number of instrument
 *
 * Parameters:
 *	ViSession instrumentHandle: VISA instrument handle used to access instrument
 *		specific data. Must be initialized by tkvx4244_init()
 *		prior to use.
 *	ViPInt16 slot: Storage for returned slot #
 *
 * Returns:
 *	VI_SUCCESS if no errors occurred, error code otherwise
 */

ViStatus _VI_FUNC tkvx4244_getSlotNumber(ViSession instrumentHandle, ViPInt16 slot);
	/*define for backward compatibility*/
#define TKVX4244_getSlotNumber(a,b) tkvx4244_getSlotNumber((a),(b))
/***************************************************************************
 * Function: tkvx4244_getSlotList
 *
 * Purpose:
 *	Return lists of slot numbers for all tkvx4244's.
 *
 * Parameters:
 *	ViInt16 slotArray[]: array of slot #s
 *	ViInt16  slotArrayLength: length of these arrays
 *	ViPInt16 numberFound: Return value for number of matching instruments found
 *
 * Returns:
 *	VI_SUCCESS if no errors occurred, error code otherwise
 */

ViStatus _VI_FUNC tkvx4244_getSlotList(ViInt16 slotArray[], ViInt16 slotArrayLength,
			ViPInt16 numberFound);
	/*define for backward compatibility*/
#define TKVX4244_getSlotList(a,b,c) tkvx4244_getSlotList((a),(b),(c))
/***************************************************************************
 * Function: tkvx4244_getVisaRev
 *
 * Purpose:
 *	Return visa revison as an ASCII string
 *
 * Parameters:
 *	ViSession instrumentHandle: VISA instrument handle used to access instrument
 *		specific data. Must be initialized by tkvx4244_init()
 *		prior to use.
 *	ViChar revision[]: Storage for returned instrument descriptor
 *
 * Returns:
 *	VI_SUCCESS if no errors occurred, error code otherwise
 */

ViStatus _VI_FUNC tkvx4244_getVisaRev(ViSession instrumentHandle,
			ViChar revision[]);
	/*define for backward compatibility*/
#define TKVX4244_getVisaRev(a,b) tkvx4244_getVisaRev((a),(b))
/***************************************************************************
 * Function: tkvx4244_reset
 *
 * Purpose:
 *	This function resets the instrument.
 *
 * Parameters:
 *	ViSession instrumentHandle: VISA instrument handle used to access instrument
 *		specific data.
 *
 * Returns:
 *	VI_SUCCESS if no errors occurred, error code otherwise
 */

ViStatus _VI_FUNC tkvx4244_reset (ViSession instrumentHandle);
	/*define for backward compatibility*/
#define TKVX4244_reset(a) tkvx4244_reset((a))
/***************************************************************************
 * Function: tkvx4244_revision_query
 *
 * Purpose:
 *	This function returns the driver and instrument revisions.
 *
 * Parameters:
 *	ViSession instrumentHandle: VISA instrument handle used to access instrument
 *		specific data.
 *	ViChar driverRevision[]: Revision string for driver
 *	ViChar instrumentRevision[]: Revison string for instrument firmware
 *
 * Returns:
 *	VI_SUCCESS if no errors occurred, error code otherwise
 */

ViStatus _VI_FUNC tkvx4244_revision_query (ViSession instrumentHandle, ViChar driverRevision[],
			ViChar instrumentRevision[]);
	/*define for backward compatibility*/
#define TKVX4244_revision(a,b,c) tkvx4244_revision_query((a),(b),(c))
/***************************************************************************
 * Function: tkvx4244_self_test
 *
 * Purpose:
 *	This function executes the instrument self-test and returns the result.
 *
 * Parameters:
 *	ViSession instrumentHandle: VISA instrument handle used to access instrument
 *		specific data.
 *	ViPInt16 selfTestResult: Boolean flag specifying self-test status
 *	ViChar selfTestMessage[]: Textual description of self-test status
 *
 * Returns:
 *	VI_SUCCESS if no errors occurred, error code otherwise
 */

ViStatus _VI_FUNC tkvx4244_self_test (ViSession instrumentHandle, ViPInt16 selfTestResult,
			ViChar selfTestMessage[]);		/* VPP 3.1 Rule 5.13 */
/***************************************************************************
 * Function: tkvx4244_perform_self_test
 *
 * Purpose:
 *	This function executes the instrument self-test and returns the result.
 *
 * Parameters:
 *	ViSession instrumentHandle: VISA instrument handle used to access instrument
 *		specific data.
 *	ViPStatus selfTestResult: Boolean flag specifying self-test status
 *	ViChar selfTestMessage[]: Textual description of self-test status
 *
 * Returns:
 *	VI_SUCCESS if no errors occurred, error code otherwise
 */

ViStatus _VI_FUNC tkvx4244_perform_self_test (ViSession instrumentHandle, ViPStatus selfTestResult,
			ViChar selfTestMessage[]);		/* VPP 3.1 Rule 5.13 */
#define TKVX4244_self_test(a,b,c)  tkvx4244_perform_self_test((a),(b),(c))
/***************************************************************************
 * Function: tkvx4244_sleep
 *
 * Purpose:
 *	Stop processing for specified # of seconds.
 *
 * Parameters:
 *	ViInt32 secondsToDelay: # of seconds to delay for
 *
 * Returns:
 *	ViStatus VI_SUCCESS
 */

ViStatus _VI_FUNC tkvx4244_sleep (ViInt32 secondsToDelay);
	/*define for backward compatibility*/
#define TKVX4244_Sleep(a) tkvx4244_sleep((a))
/***************************************************************************
 * Function: tkvx4244_write
 *
 * Purpose:
 *	This function writes to an instrument.
 *
 * Parameters:
 *	ViSession instrumentHandle: VISA instrument handle used to access instrument
 *		specific data.
 *	ViString command:	String that is passed to an instrument.
 *
 * Returns:
 *	VI_SUCCESS if no errors occurred, error code otherwise
 */

ViStatus _VI_FUNC tkvx4244_write (ViSession instrumentHandle,ViString command);
	/*define for backward compatibility*/
#define TKVX4244_write(a,b) tkvx4244_write((a),(b))
/***************************************************************************
 * Function: tkvx4244_read
 *
 * Purpose:
 *	This function executes a read.
 *
 * Parameters:
 *	ViSession instrumentHandle: VISA instrument handle used to access instrument
 *		specific data.
 *	ViChar message[]:	Returns the read string.
 *
 * Returns:
 *	VI_SUCCESS if no errors occurred, error code otherwise
 */

ViStatus _VI_FUNC tkvx4244_read (ViSession instrumentHandle,ViChar message[]);
	/*define for backward compatibility*/
#define TKVX4244_read(a,b) tkvx4244_read((a),(b))
#if defined(__cplusplus) || defined(__cplusplus__)
  }
#endif

#endif

