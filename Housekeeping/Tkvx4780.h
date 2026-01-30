/*******************************************************************
*This file created using Driver Wizard version 2.0 and instrtmp.c
*/ 
/***************************************************************************
 * Tektronix tkvx4780  16-Channel Signal Conditioner
 * Driver Revision 2.1.0
 * VXIPlug&Play WIN/WIN95/WINNT Frameworks Revision  4.0
 * Compatible with Instrument Firmware Version 1.0 or later
 * Copyright Tektronix 1995-1996, all rights reserved.
 */

#ifndef _tkvx4780_H_
#define _tkvx4780_H_


#include <vpptype.h>

#if defined(__cplusplus) || defined(__cplusplus__)
   extern "C" {
#endif

#if defined(_CVI_)
#define tkvx4780_VI_PTR	*
#else
#define tkvx4780_VI_PTR	_VI_FAR *
#endif

#ifndef VI_VERSION_MAJOR
#define VI_VERSION_MAJOR(ver)    ((((ViVersion)ver) & 0xFFF00000UL)>>20)
#define VI_VERSION_MINOR(ver)    ((((ViVersion)ver) & 0x000FFF00UL)>>8)
#define VI_VERSION_SUBMINOR(ver) ((((ViVersion)ver) & 0x000000FFUL))
#endif

/*VXI p&p required function defines*/
#define INIT_DO_QUERY 1
#define INIT_SKIP_QUERY 0
#define INIT_DO_RESET 1
#define INIT_DONT_RESET 0

/*
 *  Instrument specific macros
 */

#define	tkvx4780_INPUT_STATE_OFF	    0
#define	tkvx4780_INPUT_STATE_ON	        1
#define	tkvx4780_OUTPUT_STATE_OFF	    0
#define	tkvx4780_OUTPUT_STATE_ON	    1
#define	tkvx4780_MULTI_CHANNELS	    	0
#define	tkvx4780_MIN_CHANNELS	    	1
#define	tkvx4780_MAX_CHANNELS	    	16
#define	tkvx4780_MIN_GAIN	    	    1
#define	tkvx4780_GAIN_2	    	        2
#define	tkvx4780_GAIN_5	    	        5
#define	tkvx4780_GAIN_10	    	    10
#define	tkvx4780_GAIN_20	    	    20
#define	tkvx4780_GAIN_50	    	    50
#define	tkvx4780_MAX_GAIN	        	100
#define	tkvx4780_MIN_ATTEN	    	    10
#define	tkvx4780_MAX_ATTEN	        	100
#define	tkvx4780_BYPASS_ATTEN	        1
#define	tkvx4780_MIN_COUPLE	    	    0
#define	tkvx4780_MAX_COUPLE	        	2
#define	tkvx4780_AC_COUPLE	    	    0
#define	tkvx4780_DC_COUPLE	        	1
#define	tkvx4780_GND_COUPLE	        	2
#define	tkvx4780_MIN_STGAIN	    	    1
#define	tkvx4780_STGAIN_2	    	    2
#define	tkvx4780_STGAIN_5	    	    5
#define	tkvx4780_MAX_STGAIN	    	    10
#define	tkvx4780_MIN_IAGAIN	    	    1
#define	tkvx4780_MAX_IAGAIN	    	    10
#define	tkvx4780_MIN_CALADR	    	    0
#define	tkvx4780_MAX_CALADR	        	1023
#define	tkvx4780_MIN_CALDATA    	    0
#define	tkvx4780_MAX_CALDATA        	65535
#define	tkvx4780_MIN_LOOPCNT    	    1
#define	tkvx4780_MAX_LOOPCNT        	65535
#define	tkvx4780_MIN_GAINDAC	    	0
#define	tkvx4780_MAX_GAINDAC	    	4095
#define	tkvx4780_MIN_MSOFFSET	        0
#define	tkvx4780_MAX_MSOFFSET	        128
#define	tkvx4780_MIN_LSOFFSET	        0
#define	tkvx4780_MAX_LSOFFSET   	    255
#define	tkvx4780_MIN_OFFSETTRIM	        -0.2F
#define	tkvx4780_MAX_OFFSETTRIM   	    0.2F
#define	tkvx4780_MIN_GAINTRIM	        -10000
#define	tkvx4780_MAX_GAINTRIM   	    10000
#define	tkvx4780_CH1_ENABLE	    	    1
#define	tkvx4780_CH1_DISABLE	    	0
#define	tkvx4780_CH2_ENABLE	    	    1
#define	tkvx4780_CH2_DISABLE	    	0
#define	tkvx4780_CH3_ENABLE	    	    1
#define	tkvx4780_CH3_DISABLE	    	0
#define	tkvx4780_CH4_ENABLE	    	    1
#define	tkvx4780_CH4_DISABLE	    	0
#define	tkvx4780_CH5_ENABLE	    	    1
#define	tkvx4780_CH5_DISABLE	    	0
#define	tkvx4780_CH6_ENABLE	    	    1
#define	tkvx4780_CH6_DISABLE	    	0
#define	tkvx4780_CH7_ENABLE	    	    1
#define	tkvx4780_CH7_DISABLE	    	0
#define	tkvx4780_CH8_ENABLE	    	    1
#define	tkvx4780_CH8_DISABLE	    	0
#define	tkvx4780_CH9_ENABLE	    	    1
#define	tkvx4780_CH9_DISABLE	    	0
#define	tkvx4780_CH10_ENABLE	    	1
#define	tkvx4780_CH10_DISABLE	    	0
#define	tkvx4780_CH11_ENABLE	    	1
#define	tkvx4780_CH11_DISABLE	    	0
#define	tkvx4780_CH12_ENABLE	    	1
#define	tkvx4780_CH12_DISABLE	    	0
#define	tkvx4780_CH13_ENABLE	    	1
#define	tkvx4780_CH13_DISABLE	    	0
#define	tkvx4780_CH14_ENABLE	    	1
#define	tkvx4780_CH14_DISABLE	    	0
#define	tkvx4780_CH15_ENABLE	    	1
#define	tkvx4780_CH15_DISABLE	    	0
#define	tkvx4780_CH16_ENABLE	    	1
#define	tkvx4780_CH16_DISABLE	    	0
#define	tkvx4780_ALLCH_ENABLE	    	1
#define	tkvx4780_ALLCH_DISABLE	    	0
#define tkvx4780_ATTR_NOT_VISIBLE		0
#define tkvx4780_ATTR_VISIBLE		    1
#define tkvx4780_MIN_STEP			    468
#define tkvx4780_MAX_STEP				7133
#define tkvx4780_MIN_LOW_STEP_FREQ		468
#define tkvx4780_MAX_LOW_STEP_FREQ		7020
#define tkvx4780_MIN_HIGH_STEP_FREQ		7133
#define tkvx4780_MAX_HIGH_STEP_FREQ		107000
#define	tkvx4780_MIN_FREQ	    	    468
#define tkvx4780_FREQ_468				468
#define tkvx4780_FREQ_936				936
#define tkvx4780_FREQ_1404				1404
#define tkvx4780_FREQ_1872				1872
#define tkvx4780_FREQ_2340				2340
#define tkvx4780_FREQ_2808				2808
#define tkvx4780_FREQ_3276				3276
#define tkvx4780_FREQ_3744				3744
#define tkvx4780_FREQ_4212				4212
#define tkvx4780_FREQ_4680				4680
#define tkvx4780_FREQ_5148				5148
#define tkvx4780_FREQ_5616				5616
#define tkvx4780_FREQ_6084				6084
#define tkvx4780_FREQ_6552				6552
#define tkvx4780_FREQ_7020				7020
#define tkvx4780_FREQ_7133				7133
#define tkvx4780_FREQ_14267				14267
#define tkvx4780_FREQ_21400				21400
#define tkvx4780_FREQ_28533				28533
#define tkvx4780_FREQ_35667				35667
#define tkvx4780_FREQ_42800				42800
#define tkvx4780_FREQ_49933				49933
#define tkvx4780_FREQ_57067				57067
#define tkvx4780_FREQ_64200				64200
#define tkvx4780_FREQ_71333				71333
#define tkvx4780_FREQ_78467				78467
#define tkvx4780_FREQ_85600				85600
#define tkvx4780_FREQ_92733				92733
#define tkvx4780_FREQ_99867				99867
#define tkvx4780_FREQ_107000			107000
#define	tkvx4780_MAX_FREQ	        	107000

#define tkvx4780_SELFTEST_OFF           0

#define tkvx4780_CHANNEL1				1
#define tkvx4780_CHANNEL2				2
#define tkvx4780_CHANNEL3				3
#define tkvx4780_CHANNEL4				4
#define tkvx4780_CHANNEL5				5
#define tkvx4780_CHANNEL6				6
#define tkvx4780_CHANNEL7				7
#define tkvx4780_CHANNEL8				8
#define tkvx4780_CHANNEL9				9
#define tkvx4780_CHANNEL10				10
#define tkvx4780_CHANNEL11				11
#define tkvx4780_CHANNEL12				12
#define tkvx4780_CHANNEL13				13
#define tkvx4780_CHANNEL14				14
#define tkvx4780_CHANNEL15				15
#define tkvx4780_CHANNEL16				16

#define	tkvx4780_CH1_CLOSE	    	    1
#define	tkvx4780_CH1_OPEN	    	    0
#define	tkvx4780_CH2_CLOSE	    	    1
#define	tkvx4780_CH2_OPEN	        	0
#define	tkvx4780_CH3_CLOSE	    	    1
#define	tkvx4780_CH3_OPEN	        	0
#define	tkvx4780_CH4_CLOSE	    	    1
#define	tkvx4780_CH4_OPEN	        	0
#define	tkvx4780_CH5_CLOSE	    	    1
#define	tkvx4780_CH5_OPEN	        	0
#define	tkvx4780_CH6_CLOSE	    	    1
#define	tkvx4780_CH6_OPEN	        	0
#define	tkvx4780_CH7_CLOSE	    	    1
#define	tkvx4780_CH7_OPEN	        	0
#define	tkvx4780_CH8_CLOSE	    	    1
#define	tkvx4780_CH8_OPEN	        	0
#define	tkvx4780_CH9_CLOSE	    	    1
#define	tkvx4780_CH9_OPEN	        	0
#define	tkvx4780_CH10_CLOSE	        	1
#define	tkvx4780_CH10_OPEN	        	0
#define	tkvx4780_CH11_CLOSE	        	1
#define	tkvx4780_CH11_OPEN	        	0
#define	tkvx4780_CH12_CLOSE	        	1
#define	tkvx4780_CH12_OPEN	        	0
#define	tkvx4780_CH13_CLOSE	        	1
#define	tkvx4780_CH13_OPEN	        	0
#define	tkvx4780_CH14_CLOSE	        	1
#define	tkvx4780_CH14_OPEN	    	    0
#define	tkvx4780_CH15_CLOSE	        	1
#define	tkvx4780_CH15_OPEN	    	    0
#define	tkvx4780_CH16_CLOSE	    	    1
#define	tkvx4780_CH16_OPEN	        	0
#define	tkvx4780_ALLCH_CLOSE	    	1
#define	tkvx4780_ALLCH_OPEN	        	0

/*
 *  Channel selection macros.
 */

#define tkvx4780_SELECT_CHANNEL			1
#define tkvx4780_IGNORE_CHANNEL			0
#define tkvx4780_SELECT_ALL_CHANNELS	1
#define tkvx4780_IGNORE_ALL_CHANNELS	0

/*
 *  Coupling macros to define string types
 */

#define tkvx4780_AC_COUPLE_STR			"AC"
#define tkvx4780_DC_COUPLE_STR			"DC"
#define tkvx4780_GND_COUPLE_STR			"GRO"

/*
 *  Query definitions.  These macros are used as indices into the
 *  query command array.  The order of these macros must match the
 *  text for instrument queries stored in the query command array.
 *  The last query macro is used to control loops:  0 to "last query #".
 */

#define tkvx4780_INPUT_STATE_QUERY  0
#define tkvx4780_OUTPUT_STATE_QUERY	1
#define tkvx4780_COUPLING_QUERY		2
#define tkvx4780_ATTEN_STATE_QUERY  3
#define tkvx4780_ATTENUATION_QUERY  4
#define tkvx4780_GAIN_QUERY  		5
#define tkvx4780_GAIN_TRIM_QUERY	6
#define tkvx4780_OFFSET_TRIM_QUERY  7
#define tkvx4780_FILTER_FREQ_QUERY  8
#define tkvx4780_LAST_QUERY			8

/*
 *  Macros to maintain backward compatibility.
 */

#define	TKVX4780_INPUT_STATE_OFF	    tkvx4780_INPUT_STATE_OFF
#define	TKVX4780_INPUT_STATE_ON	        tkvx4780_INPUT_STATE_ON
#define	TKVX4780_OUTPUT_STATE_OFF	    tkvx4780_OUTPUT_STATE_OFF
#define	TKVX4780_OUTPUT_STATE_ON	    tkvx4780_OUTPUT_STATE_ON
#define	TKVX4780_MULTI_CHANNELS	    	tkvx4780_MULTI_CHANNELS
#define	TKVX4780_MIN_CHANNELS	    	tkvx4780_MIN_CHANNELS
#define	TKVX4780_MAX_CHANNELS	    	tkvx4780_MAX_CHANNELS
#define	TKVX4780_MIN_GAIN	    	    tkvx4780_MIN_GAIN
#define	TKVX4780_GAIN_2	    	        tkvx4780_GAIN_2
#define	TKVX4780_GAIN_5	    	        tkvx4780_GAIN_5
#define	TKVX4780_GAIN_10	    	    tkvx4780_GAIN_10
#define	TKVX4780_GAIN_20	    	    tkvx4780_GAIN_20
#define	TKVX4780_GAIN_55	    	    tkvx4780_GAIN_50
#define	TKVX4780_MAX_GAIN	        	tkvx4780_MAX_GAIN
#define	TKVX4780_MIN_ATTEN	    	    tkvx4780_MIN_ATTEN
#define	TKVX4780_MAX_ATTEN	        	tkvx4780_MAX_ATTEN
#define	TKVX4780_BYPASS_ATTEN	        tkvx4780_BYPASS_ATTEN
#define	TKVX4780_MIN_COUPLE	    	    tkvx4780_MIN_COUPLE
#define	TKVX4780_MAX_COUPLE	        	tkvx4780_MAX_COUPLE
#define	TKVX4780_AC_COUPLE	    	    tkvx4780_AC_COUPLE
#define	TKVX4780_DC_COUPLE	        	tkvx4780_DC_COUPLE
#define	TKVX4780_GND_COUPLE	        	tkvx4780_GND_COUPLE
#define	TKVX4780_MIN_FREQ	    	    tkvx4780_MIN_FREQ
#define	TKVX4780_MAX_FREQ	        	tkvx4780_MAX_FREQ
#define	TKVX4780_MIN_STGAIN	    	    tkvx4780_MIN_STGAIN
#define	TKVX4780_STGAIN_2	    	    tkvx4780_STGAIN_2
#define	TKVX4780_STGAIN_5	    	    tkvx4780_STGAIN_5
#define	TKVX4780_MAX_STGAIN	    	    tkvx4780_MAX_STGAIN
#define	TKVX4780_MIN_IAGAIN	    	    tkvx4780_MIN_IAGAIN
#define	TKVX4780_MAX_IAGAIN	    	    tkvx4780_MAX_IAGAIN
#define	TKVX4780_MIN_CALADR	    	    tkvx4780_MIN_CALADR
#define	TKVX4780_MAX_CALADR	        	tkvx4780_MAX_CALADR
#define	TKVX4780_MIN_CALDATA    	    tkvx4780_MIN_CALDATA
#define	TKVX4780_MAX_CALDATA        	tkvx4780_MAX_CALDATA
#define	TKVX4780_MIN_LOOPCNT    	    tkvx4780_MIN_LOOPCNT
#define	TKVX4780_MAX_LOOPCNT        	tkvx4780_MAX_LOOPCNT
#define	TKVX4780_MIN_GAINDAC	    	tkvx4780_MIN_GAINDAC
#define	TKVX4780_MAX_GAINDAC	    	tkvx4780_MAX_GAINDAC
#define	TKVX4780_MIN_MSOFFSET	        tkvx4780_MIN_MSOFFSET
#define	TKVX4780_MAX_MSOFFSET	        tkvx4780_MAX_MSOFFSET
#define	TKVX4780_MIN_LSOFFSET	        tkvx4780_MIN_LSOFFSET
#define	TKVX4780_MAX_LSOFFSET   	    tkvx4780_MAX_LSOFFSET
#define	TKVX4780_MIN_OFFSETTRIM	        tkvx4780_MIN_OFFSETTRIM
#define	TKVX4780_MAX_OFFSETTRIM   	    tkvx4780_MAX_OFFSETTRIM
#define	TKVX4780_MIN_GAINTRIM	        tkvx4780_MIN_GAINTRIM
#define	TKVX4780_MAX_GAINTRIM   	    tkvx4780_MAX_GAINTRIM
#define	TKVX4780_CH1_ENABLE	    	    tkvx4780_CH1_ENABLE
#define	TKVX4780_CH1_DISABLE	    	tkvx4780_CH1_DISABLE
#define	TKVX4780_CH2_ENABLE	    	    tkvx4780_CH2_ENABLE
#define	TKVX4780_CH2_DISABLE	    	tkvx4780_CH2_DISABLE
#define	TKVX4780_CH3_ENABLE	    	    tkvx4780_CH3_ENABLE
#define	TKVX4780_CH3_DISABLE	    	tkvx4780_CH3_DISABLE
#define	TKVX4780_CH4_ENABLE	    	    tkvx4780_CH4_ENABLE
#define	TKVX4780_CH4_DISABLE	    	tkvx4780_CH4_DISABLE
#define	TKVX4780_CH5_ENABLE	    	    tkvx4780_CH5_ENABLE
#define	TKVX4780_CH5_DISABLE	    	tkvx4780_CH5_DISABLE
#define	TKVX4780_CH6_ENABLE	    	    tkvx4780_CH6_ENABLE
#define	TKVX4780_CH6_DISABLE	    	tkvx4780_CH6_DISABLE
#define	TKVX4780_CH7_ENABLE	    	    tkvx4780_CH7_ENABLE
#define	TKVX4780_CH7_DISABLE	    	tkvx4780_CH7_DISABLE
#define	TKVX4780_CH8_ENABLE	    	    tkvx4780_CH8_ENABLE
#define	TKVX4780_CH8_DISABLE	    	tkvx4780_CH8_DISABLE
#define	TKVX4780_CH9_ENABLE	    	    tkvx4780_CH9_ENABLE
#define	TKVX4780_CH9_DISABLE	    	tkvx4780_CH9_DISABLE
#define	TKVX4780_CH10_ENABLE	    	tkvx4780_CH10_ENABLE
#define	TKVX4780_CH10_DISABLE	    	tkvx4780_CH10_DISABLE
#define	TKVX4780_CH11_ENABLE	    	tkvx4780_CH11_ENABLE
#define	TKVX4780_CH11_DISABLE	    	tkvx4780_CH11_DISABLE
#define	TKVX4780_CH12_ENABLE	    	tkvx4780_CH12_ENABLE
#define	TKVX4780_CH12_DISABLE	    	tkvx4780_CH12_DISABLE
#define	TKVX4780_CH13_ENABLE	    	tkvx4780_CH13_ENABLE
#define	TKVX4780_CH13_DISABLE	    	tkvx4780_CH13_DISABLE
#define	TKVX4780_CH14_ENABLE	    	tkvx4780_CH14_ENABLE
#define	TKVX4780_CH14_DISABLE	    	tkvx4780_CH14_DISABLE
#define	TKVX4780_CH15_ENABLE	    	tkvx4780_CH15_ENABLE
#define	TKVX4780_CH15_DISABLE	    	tkvx4780_CH15_DISABLE
#define	TKVX4780_CH16_ENABLE	    	tkvx4780_CH16_ENABLE
#define	TKVX4780_CH16_DISABLE	    	tkvx4780_CH16_DISABLE
#define	TKVX4780_ALLCH_ENABLE	    	tkvx4780_ALLCH_ENABLE
#define	TKVX4780_ALLCH_DISABLE	    	tkvx4780_ALLCH_DISABLE
#define TKVX4780_ATTR_NOT_VISIBLE		tkvx4780_ATTR_NOT_VISIBLE
#define TKVX4780_ATTR_VISIBLE		    tkvx4780_ATTR_VISIBLE
#define TKVX4780_MIN_STEP			    tkvx4780_MIN_STEP
#define TKVX4780_MAX_STEP				tkvx4780_MAX_STEP
#define TKVX4780_MIN_LOW_STEP_FREQ		tkvx4780_MIN_LOW_STEP_FREQ
#define TKVX4780_MAX_LOW_STEP_FREQ		tkvx4780_MAX_LOW_STEP_FREQ
#define TKVX4780_MIN_HIGH_STEP_FREQ		tkvx4780_MIN_HIGH_STEP_FREQ
#define TKVX4780_MAX_HIGH_STEP_FREQ		tkvx4780_MAX_HIGH_STEP_FREQ
#define TKVX4780_SELFTEST_OFF           tkvx4780_SELFTEST_OFF

#define	TKVX4780_CH1_CLOSE	    	    tkvx4780_CH1_CLOSE
#define	TKVX4780_CH1_OPEN	    	    tkvx4780_CH1_OPEN
#define	TKVX4780_CH2_CLOSE	    	    tkvx4780_CH2_CLOSE
#define	TKVX4780_CH2_OPEN	        	tkvx4780_CH2_OPEN
#define	TKVX4780_CH3_CLOSE	    	    tkvx4780_CH3_CLOSE
#define	TKVX4780_CH3_OPEN	        	tkvx4780_CH3_OPEN
#define	TKVX4780_CH4_CLOSE	    	    tkvx4780_CH4_CLOSE
#define	TKVX4780_CH4_OPEN	        	tkvx4780_CH4_OPEN
#define	TKVX4780_CH5_CLOSE	    	    tkvx4780_CH5_CLOSE
#define	TKVX4780_CH5_OPEN	        	tkvx4780_CH5_OPEN
#define	TKVX4780_CH6_CLOSE	    	    tkvx4780_CH6_CLOSE
#define	TKVX4780_CH6_OPEN	        	tkvx4780_CH6_OPEN
#define	TKVX4780_CH7_CLOSE	    	    tkvx4780_CH7_CLOSE
#define	TKVX4780_CH7_OPEN	        	tkvx4780_CH7_OPEN
#define	TKVX4780_CH8_CLOSE	    	    tkvx4780_CH8_CLOSE
#define	TKVX4780_CH8_OPEN	        	tkvx4780_CH8_OPEN
#define	TKVX4780_CH9_CLOSE	    	    tkvx4780_CH9_CLOSE
#define	TKVX4780_CH9_OPEN	        	tkvx4780_CH9_OPEN
#define	TKVX4780_CH10_CLOSE	        	tkvx4780_CH10_CLOSE
#define	TKVX4780_CH10_OPEN	        	tkvx4780_CH10_OPEN
#define	TKVX4780_CH11_CLOSE	        	tkvx4780_CH11_CLOSE
#define	TKVX4780_CH11_OPEN	        	tkvx4780_CH11_OPEN
#define	TKVX4780_CH12_CLOSE	        	tkvx4780_CH12_CLOSE
#define	TKVX4780_CH12_OPEN	        	tkvx4780_CH12_OPEN
#define	TKVX4780_CH13_CLOSE	        	tkvx4780_CH13_CLOSE
#define	TKVX4780_CH13_OPEN	        	tkvx4780_CH13_OPEN
#define	TKVX4780_CH14_CLOSE	        	tkvx4780_CH14_CLOSE
#define	TKVX4780_CH14_OPEN	    	    tkvx4780_CH14_OPEN
#define	TKVX4780_CH15_CLOSE	        	tkvx4780_CH15_CLOSE
#define	TKVX4780_CH15_OPEN	    	    tkvx4780_CH15_OPEN
#define	TKVX4780_CH16_CLOSE	    	    tkvx4780_CH16_CLOSE
#define	TKVX4780_CH16_OPEN	        	tkvx4780_CH16_OPEN
#define	TKVX4780_ALLCH_CLOSE	    	tkvx4780_ALLCH_CLOSE
#define	TKVX4780_ALLCH_OPEN	        	tkvx4780_ALLCH_OPEN


/*
 *  Backwards compatability for function declarations.
 */

#define TKVX4780_autoConnectToAll(A,B,C) \
		tkvx4780_autoConnectToAll((A),(B),(C))

#define TKVX4780_autoConnectToFirst(A) \
		tkvx4780_autoConnectToFirst(A)

#define TKVX4780_autoConnectToLA(A,B) \
		tkvx4780_autoConnectToLA((A),(B))

#define TKVX4780_autoConnectToSlot(A,B,C,D) \
		tkvx4780_autoConnectToSlot((A),(B),(C),(D))

#define TKVX4780_close(A) \
		tkvx4780_close(A)

#define TKVX4780_init(A,B,C,D) \
		tkvx4780_init((A),(B),(C),(D))

#define TKVX4780_error_message(A,B,C) \
		tkvx4780_error_message((A),(B),(C))

#define TKVX4780_error_query(A,B,C) \
		tkvx4780_error_query((A),(B),(C))

#define TKVX4780_getInstrDesc(A,B) \
		tkvx4780_getInstrDesc((A),(B))

#define TKVX4780_getLogicalAddress(A,B) \
		tkvx4780_getLogicalAddress((A),(B))

#define TKVX4780_getManufacturerID(A,B) \
		tkvx4780_getManufacturerID((A),(B))

#define TKVX4780_getModelCode(A,B) \
		tkvx4780_getModelCode((A),(B))

#define TKVX4780_getSlotandLAList(A,B,C,D) \
		tkvx4780_getSlotandLAList((A),(B),(C),(D))

#define TKVX4780_getSlotNumber(A,B) \
		tkvx4780_getSlotNumber((A),(B))

#define TKVX4780_getSlotList(A,B,C) \
		tkvx4780_getSlotList((A),(B),(C))

#define TKVX4780_getVisaRev(A,B) \
		tkvx4780_getVisaRev((A),(B))

#define TKVX4780_reset(A) \
		tkvx4780_reset(A)

#define TKVX4780_revision_query(A,B,C) \
		tkvx4780_revision_query((A),(B),(C))

#define TKVX4780_self_test(A,B,C) \
		tkvx4780_self_test((A),(B),(C))

#define TKVX4780_Sleep(A) \
		tkvx4780_sleep(A)

#define TKVX4780_write(A,B) \
		tkvx4780_write((A),(B))

#define TKVX4780_read(A,B) \
		tkvx4780_read((A),(B))

#define TKVX4780_gain(A,B,C) \
		tkvx4780_setGain((A),(B),(C))

#define TKVX4780_gain_grp1(A,B,C,D,E,F,G) \
		tkvx4780_setGain_grp1((A),(B),(C),(D),(E),(F),(G))

#define TKVX4780_gain_grp2(A,B,C,D,E,F,G) \
		tkvx4780_setGain_grp2((A),(B),(C),(D),(E),(F),(G))

#define TKVX4780_gain_grp3(A,B,C,D,E,F,G) \
		tkvx4780_setGain_grp3((A),(B),(C),(D),(E),(F),(G))

#define TKVX4780_gain_grp4(A,B,C,D,E,F,G) \
		tkvx4780_setGain_grp4((A),(B),(C),(D),(E),(F),(G))

#define TKVX4780_attenuation(A,B,C) \
		tkvx4780_setAttenuation((A),(B),(C))

#define TKVX4780_attenuation_grp1(A,B,C,D,E,F,G) \
		tkvx4780_setAttenuation_grp1((A),(B),(C),(D),(E),(F),(G))

#define TKVX4780_attenuation_grp2(A,B,C,D,E,F,G) \
		tkvx4780_setAttenuation_grp2((A),(B),(C),(D),(E),(F),(G))

#define TKVX4780_attenuation_grp3(A,B,C,D,E,F,G) \
		tkvx4780_setAttenuation_grp3((A),(B),(C),(D),(E),(F),(G))

#define TKVX4780_attenuation_grp4(A,B,C,D,E,F,G) \
		tkvx4780_setAttenuation_grp4((A),(B),(C),(D),(E),(F),(G))

#define TKVX4780_coupling(A,B,C) \
		tkvx4780_setCoupling((A),(B),(C))

#define TKVX4780_coupling_grp1(A,B,C,D,E,F,G) \
		tkvx4780_setCoupling_grp1((A),(B),(C),(D),(E),(F),(G))

#define TKVX4780_coupling_grp2(A,B,C,D,E,F,G) \
		tkvx4780_setCoupling_grp2((A),(B),(C),(D),(E),(F),(G))

#define TKVX4780_coupling_grp3(A,B,C,D,E,F,G) \
		tkvx4780_setCoupling_grp3((A),(B),(C),(D),(E),(F),(G))

#define TKVX4780_coupling_grp4(A,B,C,D,E,F,G) \
		tkvx4780_setCoupling_grp4((A),(B),(C),(D),(E),(F),(G))

#define TKVX4780_input_state(A,B,C) \
		tkvx4780_setInputState((A),(B),(C))

#define TKVX4780_input_state_grp1(A,B,C,D,E,F,G) \
		tkvx4780_setInputState_grp1((A),(B),(C),(D),(E),(F),(G))

#define TKVX4780_input_state_grp2(A,B,C,D,E,F,G) \
		tkvx4780_setInputState_grp2((A),(B),(C),(D),(E),(F),(G))

#define TKVX4780_input_state_grp3(A,B,C,D,E,F,G) \
		tkvx4780_setInputState_grp3((A),(B),(C),(D),(E),(F),(G))

#define TKVX4780_input_state_grp4(A,B,C,D,E,F,G) \
		tkvx4780_setInputState_grp4((A),(B),(C),(D),(E),(F),(G))

#define TKVX4780_output_state(A,B,C) \
		tkvx4780_setOutputState((A),(B),(C))

#define TKVX4780_output_state_grp1(A,B,C,D,E,F,G) \
		tkvx4780_setOutputState_grp1((A),(B),(C),(D),(E),(F),(G))

#define TKVX4780_output_state_grp2(A,B,C,D,E,F,G) \
		tkvx4780_setOutputState_grp2((A),(B),(C),(D),(E),(F),(G))

#define TKVX4780_output_state_grp3(A,B,C,D,E,F,G) \
		tkvx4780_setOutputState_grp3((A),(B),(C),(D),(E),(F),(G))

#define TKVX4780_output_state_grp4(A,B,C,D,E,F,G) \
		tkvx4780_setOutputState_grp4((A),(B),(C),(D),(E),(F),(G))

#define TKVX4780_filter_freq(A,B,C) \
		tkvx4780_setFilterFreq((A),(B),(C))

#define TKVX4780_filter_freq_grp1(A,B,C,D,E,F,G) \
		tkvx4780_setFilterFreq_grp1((A),(B),(C),(D),(E),(F),(G))

#define TKVX4780_filter_freq_grp2(A,B,C,D,E,F,G) \
		tkvx4780_setFilterFreq_grp2((A),(B),(C),(D),(E),(F),(G))

#define TKVX4780_filter_freq_grp3(A,B,C,D,E,F,G) \
		tkvx4780_setFilterFreq_grp3((A),(B),(C),(D),(E),(F),(G))

#define TKVX4780_filter_freq_grp4(A,B,C,D,E,F,G) \
		tkvx4780_setFilterFreq_grp4((A),(B),(C),(D),(E),(F),(G))

#define TKVX4780_gain_trim(A,B,C) \
		tkvx4780_setGainTrim((A),(B),(C))

#define TKVX4780_gain_trim_grp1(A,B,C,D,E,F,G) \
		tkvx4780_setGainTrim_grp1((A),(B),(C),(D),(E),(F),(G))

#define TKVX4780_gain_trim_grp2(A,B,C,D,E,F,G) \
		tkvx4780_setGainTrim_grp2((A),(B),(C),(D),(E),(F),(G))

#define TKVX4780_gain_trim_grp3(A,B,C,D,E,F,G) \
		tkvx4780_setGainTrim_grp3((A),(B),(C),(D),(E),(F),(G))

#define TKVX4780_gain_trim_grp4(A,B,C,D,E,F,G) \
		tkvx4780_setGainTrim_grp4((A),(B),(C),(D),(E),(F),(G))

#define TKVX4780_offset_trim(A,B,C) \
		tkvx4780_setOffsetTrim((A),(B),(C))

#define TKVX4780_offset_trim_grp1(A,B,C,D,E,F,G) \
		tkvx4780_setOffsetTrim_grp1((A),(B),(C),(D),(E),(F),(G))

#define TKVX4780_offset_trim_grp2(A,B,C,D,E,F,G) \
		tkvx4780_setOffsetTrim_grp2((A),(B),(C),(D),(E),(F),(G))

#define TKVX4780_offset_trim_grp3(A,B,C,D,E,F,G) \
		tkvx4780_setOffsetTrim_grp3((A),(B),(C),(D),(E),(F),(G))

#define TKVX4780_offset_trim_grp4(A,B,C,D,E,F,G) \
		tkvx4780_setOffsetTrim_grp4((A),(B),(C),(D),(E),(F),(G))

#define TKVX4780_attenuation_on(A,B) \
		tkvx4780_attenuation_on((A),(B))

#define TKVX4780_attenuation_off(A,B) \
		tkvx4780_attenuation_off((A),(B))

#define TKVX4780_query_input_state(A,B,C) \
		tkvx4780_queryInputState((A),(B),(C))

#define TKVX4780_query_output_state(A,B,C) \
		tkvx4780_queryOutputState((A),(B),(C))

#define TKVX4780_query_coupling(A,B,C) \
		tkvx4780_queryCoupling((A),(B),(C))

#define TKVX4780_query_atten(A,B,C) \
		tkvx4780_queryAttenuation((A),(B),(C))

#define TKVX4780_query_atten_state(A,B,C) \
		tkvx4780_queryAttenuationState((A),(B),(C))

#define TKVX4780_query_gain(A,B,C) \
		tkvx4780_queryGain((A),(B),(C))

#define TKVX4780_query_gain_trim(A,B,C) \
		tkvx4780_queryGainTrim((A),(B),(C))

#define TKVX4780_query_offset_trim(A,B,C) \
		tkvx4780_queryOffsetTrim((A),(B),(C))

#define TKVX4780_query_filter_freq(A,B,C) \
		tkvx4780_queryFilterFreq((A),(B),(C))

#define TKVX4780_channellist(A,B,C,D,E,F,G,H,I,J,K,L,M,N,O,P,Q,R,S) \
		tkvx4780_channellist((A),(B),(C),(D),(E),(F),(G),(H),(I),(J),\
							 (K),(L),(M),(N),(O),(P),(Q),(R),(S))

#define TKVX4780_build_chlist(A,B,C,D,E,F,G,H,I,J,K,L,M,N,O,P,Q,R,S) \
		tkvx4780_build_chlist((A),(B),(C),(D),(E),(F),(G),(H),(I),(J),\
							  (K),(L),(M),(N),(O),(P),(Q),(R),(S))

#define TKVX4780_channellist_grp1(A,B,C,D,E,F,G) \
		tkvx4780_channellist_grp1((A),(B),(C),(D),(E),(F),(G))

#define TKVX4780_build_chlist_grp1(A,B,C,D,E,F,G) \
		tkvx4780_build_chlist_grp1((A),(B),(C),(D),(E),(F),(G))

#define TKVX4780_channellist_grp2(A,B,C,D,E,F,G) \
		tkvx4780_channellist_grp2((A),(B),(C),(D),(E),(F),(G))

#define TKVX4780_build_chlist_grp2(A,B,C,D,E,F,G) \
		tkvx4780_build_chlist_grp2((A),(B),(C),(D),(E),(F),(G))

#define TKVX4780_channellist_grp3(A,B,C,D,E,F,G) \
		tkvx4780_channellist_grp3((A),(B),(C),(D),(E),(F),(G))

#define TKVX4780_build_chlist_grp3(A,B,C,D,E,F,G) \
		tkvx4780_build_chlist_grp3((A),(B),(C),(D),(E),(F),(G))

#define TKVX4780_channellist_grp4(A,B,C,D,E,F,G) \
		tkvx4780_channellist_grp4((A),(B),(C),(D),(E),(F),(G))

#define TKVX4780_build_chlist_grp4(A,B,C,D,E,F,G) \
		tkvx4780_build_chlist_grp4((A),(B),(C),(D),(E),(F),(G))

#define TKVX4780_diag_ad_read(A,B,C) \
		tkvx4780_diagADCRead((A),(B),(C))

#define TKVX4780_diag_dc(A,B) \
		tkvx4780_diagDCVolt((A),(B))

#define TKVX4780_diag_input_sqrwave(A,B) \
		tkvx4780_diagInputSqrwave((A),(B))

#define TKVX4780_diag_offset_grp1(A,B,C,D,E,F,G,H) \
		tkvx4780_diagOffsetTrim_grp1((A),(B),(C),(D),(E),(F),(G),(H))

#define TKVX4780_diag_offset_grp2(A,B,C,D,E,F,G,H) \
		tkvx4780_diagOffsetTrim_grp2((A),(B),(C),(D),(E),(F),(G),(H))

#define TKVX4780_diag_offset_grp3(A,B,C,D,E,F,G,H) \
		tkvx4780_diagOffsetTrim_grp3((A),(B),(C),(D),(E),(F),(G),(H))

#define TKVX4780_diag_offset_grp4(A,B,C,D,E,F,G,H) \
		tkvx4780_diagOffsetTrim_grp4((A),(B),(C),(D),(E),(F),(G),(H))

#define TKVX4780_diag_gain_grp1(A,B,C,D,E,F,G) \
		tkvx4780_diagGainTrim_grp1((A),(B),(C),(D),(E),(F),(G))

#define TKVX4780_diag_gain_grp2(A,B,C,D,E,F,G) \
		tkvx4780_diagGainTrim_grp2((A),(B),(C),(D),(E),(F),(G))

#define TKVX4780_diag_gain_grp3(A,B,C,D,E,F,G) \
		tkvx4780_diagGainTrim_grp3((A),(B),(C),(D),(E),(F),(G))

#define TKVX4780_diag_gain_grp4(A,B,C,D,E,F,G) \
		tkvx4780_diagGainTrim_grp4((A),(B),(C),(D),(E),(F),(G))

#define TKVX4780_diag_iagain_grp1(A,B,C,D,E,F,G) \
		tkvx4780_diagIAgain_grp1((A),(B),(C),(D),(E),(F),(G))

#define TKVX4780_diag_iagain_grp2(A,B,C,D,E,F,G) \
		tkvx4780_diagIAgain_grp2((A),(B),(C),(D),(E),(F),(G))

#define TKVX4780_diag_iagain_grp3(A,B,C,D,E,F,G) \
		tkvx4780_diagIAgain_grp3((A),(B),(C),(D),(E),(F),(G))

#define TKVX4780_diag_iagain_grp4(A,B,C,D,E,F,G) \
		tkvx4780_diagIAgain_grp4((A),(B),(C),(D),(E),(F),(G))

#define TKVX4780_diag_stgain_grp1(A,B,C,D,E,F,G) \
		tkvx4780_diagSTgain_grp1((A),(B),(C),(D),(E),(F),(G))

#define TKVX4780_diag_stgain_grp2(A,B,C,D,E,F,G) \
		tkvx4780_diagSTgain_grp2((A),(B),(C),(D),(E),(F),(G))

#define TKVX4780_diag_stgain_grp3(A,B,C,D,E,F,G) \
		tkvx4780_diagSTgain_grp3((A),(B),(C),(D),(E),(F),(G))

#define TKVX4780_diag_stgain_grp4(A,B,C,D,E,F,G) \
		tkvx4780_diagSTgain_grp4((A),(B),(C),(D),(E),(F),(G))

#define TKVX4780_diag_CAL_query(A,B,C) \
		tkvx4780_diagCALQuery((A),(B),(C))

#define TKVX4780_diag_CAL(A,B,C) \
		tkvx4780_diagCAL((A),(B),(C))

#define TKVX4780_VXI_CLEAR(A) \
		tkvx4780_VXI_CLEAR(A)



/*= GLOBAL FUNCTION DECLARATIONS ==========================================*/

/***************************************************************************
 * Function: tkvx4780_autoConnectToAll
 *
 * Purpose:
 *	Connect to all tkvx4780 instruments found.
 *
 * Parameters:
 *	ViSession instrumentArray[]: Array of VISA instrument handles
 *      used to access instrument specific data. One handle initialized by
 *      this routine for each instrument found.
 *	ViInt16 arrayLength: Number of entries which can be put in instrArray
 *	ViPInt16 numberConnected: Number of valid entries in instrArray upon
 *      return
 *
 * Returns:
 *	VI_SUCCESS if no errors occurred, error code otherwise
 */

ViStatus _VI_FUNC tkvx4780_autoConnectToAll(ViSession instrumentArray[],
			ViInt16 arrayLength, ViPInt16 numberConnected);

/***************************************************************************
 * Function: tkvx4780_autoConnectToFirst
 *
 * Purpose:
 *		Connect to first tkvx4780 instrument found
 *
 * Parameters:
 *	ViPSession instrumentHandle: Address of VISA instrument handle 
 *      used to	access instrument specific data. Initialized by this routine.
 *
 * Returns:
 *	VI_SUCCESS if no errors occurred, error code otherwise
 */

ViStatus _VI_FUNC tkvx4780_autoConnectToFirst(ViPSession instrumentHandle);

/***************************************************************************
 * Function: tkvx4780_autoConnectToLA
 *
 * Purpose:
 *	Connect to instrument in selected slot.
 *
 * Parameters:
 *	ViPSession instrumentHandle: Address of VISA instrument handle 
 *      used to access instrument specific data. Initialized by	this routine.
 *
 *	ViInt16 logicalAddress: Logical address to connect to
 *
 * Returns:
 *	VI_SUCCESS if no errors occurred, error code otherwise
 */

ViStatus _VI_FUNC tkvx4780_autoConnectToLA(ViPSession instrumentHandle,
			ViInt16 logicalAddress);

/***************************************************************************
 * Function: tkvx4780_autoConnectToSlot
 *
 * Purpose:
 *	Connect to instrument in selected slot.
 *
 * Parameters:
 *	ViSession instrumentArray[]: 	Array of VISA instrument handles 
 *      used to access instrument specific data. One handle initialized
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

ViStatus _VI_FUNC tkvx4780_autoConnectToSlot(ViSession instrumentArray[],
											 ViInt16 arrayLength,
											 ViPInt16 numberConnected,
											 ViInt16 slot);

/***************************************************************************
 * Function: tkvx4780_close
 *
 * Purpose:
 *	This function closes the instrument and returns memory allocated for
 *	instrument specific data storage.
 *
 * Parameters:
 *	ViSession instrumentHandle: VISA instrument handle used to 
 *      access instrument specific data.
 *
 * Returns:
 *	VI_SUCCESS if no errors occurred, error code otherwise
 *
 * 	NOTE!!!! instrumentHandle is no longer valid, but cannot be changed by
 *	this routine. It is important that users do not use this handle
 *	after calling this routine.
 */

ViStatus _VI_FUNC tkvx4780_close (ViSession instrumentHandle);

/***************************************************************************
 * Function: tkvx4780_init
 *
 * Purpose:
 *	This function opens the instrument, and depending upon flags passed
 *	in, queries for ID and initializes the instrument to a known state.
 *
 * Parameters:
 *	ViRsrc resourceName: Instrument search expression, see VPP-3.3, 3.2.1.1
 *	ViBoolean IDQuery: Boolean flag, VI_TRUE -> verify correct instrument
 *	ViBoolean resetDevice: Boolean flag, VI_TRUE -> reset instrument
 *	ViPSession instrumentHandle: VISA instrument handle used to 
 *      access instrument specific data. The handle is initialized by
 *		this routine.
 *
 * Returns:
 *	VI_SUCCESS if no errors occurred, error code otherwise
 *	If no error, instrumentHandle is initialized for use in rest of driver
 */

ViStatus _VI_FUNC tkvx4780_init (ViRsrc resourceName,
								 ViBoolean IDQuery,
								 ViBoolean resetDevice,
								 ViPSession instrumentHandle);

/***************************************************************************
 * Function: tkvx4780_error_message
 *
 * Purpose:
 *	This function returns a text message for a corresponding instrument
 *	driver error code.
 *
 * Parameters:
 *	ViSession instrumentHandle: VISA instrument handle used to
 *      access instrument specific data.
 *	ViStatus errorCode: error code to check
 *	ViChar errorMessage[]: Returned error string
 *
 * Returns:
 *	VI_SUCCESS if no errors occurred, error code otherwise
 */

ViStatus _VI_FUNC tkvx4780_error_message (ViSession instrumentHandle,
										  ViStatus errorCode,
										  ViChar errorMessage[]);

/***************************************************************************
 * Function: tkvx4780_error_query
 *
 * Purpose:
 *	This function queries the instrument for errors.
 *
 * Parameters:
 *	ViSession instrumentHandle: VISA instrument handle used to 
 *      access instrument specific data.
 *	ViPInt32 errorCode: Instrument error code returned
 *	ViChar errorMessage[]: Textual version of returned error code
 *
 * Returns:
 *	VI_SUCCESS if no errors occurred, error code otherwise
 */

ViStatus _VI_FUNC tkvx4780_error_query (ViSession instrumentHandle,
										ViPInt32 errorCode,
										ViChar errorMessage[]);

/***************************************************************************
 * Function: tkvx4780_getInstrDesc
 *
 * Purpose:
 *	Return instrument descriptor string of instrument
 *
 * Parameters:
 *	ViSession instrumentHandle: VISA instrument handle used to
 *      access instrument specific data.
 *	ViChar instrumentDescriptor[]: Storage for returned instrument
 *      descriptor.
 *
 * Returns:
 *	VI_SUCCESS if no errors occurred, error code otherwise
 */

ViStatus _VI_FUNC tkvx4780_getInstrDesc(ViSession instrumentHandle,
			ViChar instrumentDescriptor[]);

/***************************************************************************
 * Function: tkvx4780_getLogicalAddress
 *
 * Purpose:
 *	Return logical address of instrument
 *
 * Parameters:
 *	ViSession instrumentHandle: VISA instrument handle used to 
 *      access instrument specific data.
 *	ViPInt16 logicalAddress: Storage for returned logical address #
 *
 * Returns:
 *	VI_SUCCESS if no errors occurred, error code otherwise
 */

ViStatus _VI_FUNC tkvx4780_getLogicalAddress(ViSession instrumentHandle,
											 ViPInt16 logicalAddress);

/***************************************************************************
 * Function: tkvx4780_getManufacturerID
 *
 * Purpose:
 *	Returns manufacturer ID of instrument
 *
 * Parameters:
 *	ViSession instrumentHandle: VISA instrument handle used to
 *      access instrument specific data.
 *	ViPInt16 manufacturerIdentification: pointer to storage for ID
 *
 * Returns:
 *	VI_SUCCESS if no errors occurred, error code otherwise
 */

ViStatus _VI_FUNC tkvx4780_getManufacturerID (ViSession instrumentHandle,
										ViPInt16 manufacturerIdentification);

/***************************************************************************
 * Function: tkvx4780_getModelCode
 *
 * Purpose:
 *	Returns model code of instrument
 *
 * Parameters:
 *	ViSession instrumentHandle: VISA instrument handle used to 
 *      access instrument specific data.
 *	ViPInt16 modelCode: pointer to storage for code
 *
 * Returns:
 *	VI_SUCCESS if no errors occurred, error code otherwise
 */

ViStatus _VI_FUNC tkvx4780_getModelCode (ViSession instrumentHandle,
										 ViPInt16 modelCode);

/***************************************************************************
 * Function: tkvx4780_getSlotandLAList
 *
 * Purpose:
 *	Return lists of slot numbers and logical address for all tkvx4780's.
 *
 * Parameters:
 *	ViInt16 slotArray[]: array of slot #s
 *	ViInt16 logicalAddressArray[]: array of logical addresses
 *	ViInt16  arrayLength: length of these arrays
 *	ViPInt16 numberFound: Return value for number of matching instruments
 *      found
 *
 * Returns:
 *	VI_SUCCESS if no errors occurred, error code otherwise
 */

ViStatus _VI_FUNC tkvx4780_getSlotandLAList(ViInt16 slotArray[],
											ViInt16 logicalAddressArray[],
											ViInt16 arrayLength,
											ViPInt16 numberFound);

/***************************************************************************
 * Function: tkvx4780_getSlotNumber
 *
 * Purpose:
 *	Return slot number of instrument
 *
 * Parameters:
 *	ViSession instrumentHandle: VISA instrument handle used to 
 *      access instrument specific data. Must be initialized by
 *      tkvx4780_init() prior to use.
 *	ViPInt16 slot: Storage for returned slot #
 *
 * Returns:
 *	VI_SUCCESS if no errors occurred, error code otherwise
 */

ViStatus _VI_FUNC tkvx4780_getSlotNumber(ViSession instrumentHandle,
														ViPInt16 slot);

/***************************************************************************
 * Function: tkvx4780_getSlotList
 *
 * Purpose:
 *	Return lists of slot numbers for all tkvx4780's.
 *
 * Parameters:
 *	ViInt16 slotArray[]: array of slot #s
 *	ViInt16  slotArrayLength: length of these arrays
 *	ViPInt16 numberFound: Return value for number of matching instruments 
 *      found
 *
 * Returns:
 *	VI_SUCCESS if no errors occurred, error code otherwise
 */

ViStatus _VI_FUNC tkvx4780_getSlotList(ViInt16 slotArray[],
									   ViInt16 slotArrayLength,
									   ViPInt16 numberFound);

/***************************************************************************
 * Function: tkvx4780_getVisaRev
 *
 * Purpose:
 *	Return visa revison as an ASCII string
 *
 * Parameters:
 *	ViSession instrumentHandle: VISA instrument handle used to
 *      access instrument specific data. Must be initialized by 
 *      tkvx4780_init() prior to use.
 *	ViChar revision[]: Storage for returned instrument descriptor
 *
 * Returns:
 *	VI_SUCCESS if no errors occurred, error code otherwise
 */

ViStatus _VI_FUNC tkvx4780_getVisaRev(ViSession instrumentHandle,
			ViChar revision[]);

/***************************************************************************
 * Function: tkvx4780_reset
 *
 * Purpose:
 *	This function resets the instrument.
 *
 * Parameters:
 *	ViSession instrumentHandle: VISA instrument handle used to
 *      access instrument specific data.
 *
 * Returns:
 *	VI_SUCCESS if no errors occurred, error code otherwise
 */

ViStatus _VI_FUNC tkvx4780_reset (ViSession instrumentHandle);  

/***************************************************************************
 * Function: tkvx4780_revision_query
 *
 * Purpose:
 *	This function returns the driver and instrument revisions.
 *
 * Parameters:
 *	ViSession instrumentHandle: VISA instrument handle used to 
 *      access instrument specific data.
 *	ViChar driverRevision[]: Revision string for driver
 *	ViChar instrumentRevision[]: Revison string for instrument firmware
 *
 * Returns:
 *	VI_SUCCESS if no errors occurred, error code otherwise
 */

ViStatus _VI_FUNC tkvx4780_revision_query (ViSession instrumentHandle,
										   ViChar driverRevision[],
										   ViChar instrumentRevision[]);

/***************************************************************************
 * Function: tkvx4780_self_test
 *
 * Purpose:
 *	This function executes the instrument self-test and returns the result.
 *
 * Parameters:
 *	ViSession instrumentHandle: VISA instrument handle used to 
 *      access instrument specific data.
 *	ViPStatus selfTestResult: Boolean flag specifying self-test status
 *	ViChar selfTestMessage[]: Textual description of self-test status
 *
 * Returns:
 *	VI_SUCCESS if no errors occurred, error code otherwise
 */

ViStatus _VI_FUNC tkvx4780_self_test (ViSession instrumentHandle,
									  ViPInt16 selfTestResult,
									  ViChar selfTestMessage[]);

/***************************************************************************
 * Function: tkvx4780_sleep
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

ViStatus _VI_FUNC tkvx4780_sleep (ViInt32 secondsToDelay);

/***************************************************************************
 * Function: tkvx4780_write
 *
 * Purpose:
 *	This function writes to an instrument.
 *
 * Parameters:
 *	ViSession instrumentHandle: VISA instrument handle used to
 *      access instrument specific data.
 *	ViString command: String that is passed to an instrument.
 *
 * Returns:
 *	VI_SUCCESS if no errors occurred, error code otherwise
 */

ViStatus _VI_FUNC tkvx4780_write (ViSession instrumentHandle, ViString command);

/***************************************************************************
 * Function: tkvx4780_read
 *
 * Purpose:
 *	This function executes a read.
 *
 * Parameters:
 *	ViSession instrumentHandle: VISA instrument handle used to
 *      access instrument specific data.
 *	ViChar message[]:	Returns the read string.
 *
 * Returns:
 *	VI_SUCCESS if no errors occurred, error code otherwise
 */

ViStatus _VI_FUNC tkvx4780_read (ViSession instrumentHandle, ViChar message[]);

/***************************************************************************
 ***************************************************************************
 *	Utility routines follow. 
 ***************************************************************************
 ***************************************************************************
 */

/***************************************************************************
 * Function: tkvx4780_channellist
 *
 * Purpose:
 *  This function enables or disables the selected channel(s)
 *
 *  Note:  This is an EXCEPTION function (too many parameters) retained
 *         for backward compatibility.
 *
 * Parameters:
 *	ViSession instrumentHandle: VISA instrument handle used to access
 *      instrument specific data. Must be initialized by
 *      tkvx4780_init() prior to use.
 *  ViStatus retError: error code to check.
 *  ViInt32 ch1,...,ch16,allch: channels to enable
 *  ViChar ch_list[]: channel list
 *
 * Returns:
 *	VI_SUCCESS if no errors occurred, error code otherwise
 ***************************************************************************/

ViStatus _VI_FUNC tkvx4780_channellist (ViSession instrumentHandle,
										ViInt32 ch1, ViInt32 ch2,
										ViInt32 ch3, ViInt32 ch4,
										ViInt32 ch5, ViInt32 ch6,
										ViInt32 ch7, ViInt32 ch8,
										ViInt32 ch9, ViInt32 ch10,
										ViInt32 ch11, ViInt32 ch12,
										ViInt32 ch13, ViInt32 ch14,
										ViInt32 ch15, ViInt32 ch16,
										ViInt32 allch,
										ViChar ch_list[]);

/***************************************************************************
 * Function: tkvx4780_build_chlist
 *
 * Purpose:
 *  This function builds the channel list from the channel(s) enabled
 *
 *  Note:  This is an EXCEPTION function (too many parameters) retained
 *         for backward compatibility.
 *
 * Parameters:
 *	ViSession instrumentHandle: VISA instrument handle used to access
 *      instrument specific data. Must be initialized by
 *      tkvx4780_init() prior to use.
 *  ViStatus retError: error code to check.
 *  ViInt32 ch1,...,ch16,allch: channels to enable
 *  ViString buf: holds channel list
 *
 * Returns:
 *	VI_SUCCESS if no errors occurred, error code otherwise
 ***************************************************************************/

ViStatus _VI_FUNC tkvx4780_build_chlist (ViSession instrumentHandle,
										 ViInt32 ch1, ViInt32 ch2,
										 ViInt32 ch3, ViInt32 ch4,
										 ViInt32 ch5, ViInt32 ch6,
										 ViInt32 ch7, ViInt32 ch8,
										 ViInt32 ch9, ViInt32 ch10,
										 ViInt32 ch11, ViInt32 ch12,
										 ViInt32 ch13, ViInt32 ch14,
										 ViInt32 ch15, ViInt32 ch16,
										 ViInt32 allch, ViChar buf[]);

/***************************************************************************
 * Function: tkvx4780_channellist_grp1
 *
 * Purpose:
 *  This function enables or disables the selected channel(s).
 *
 *  Note:  This function is now an EXCEPTION function because it is 
 *         obsolete with the new instrument setup and query commands.
 *         It is included for backwards compatibility and is not in 
 *         the function panel.
 *
 * Parameters:
 *	ViSession instrumentHandle: VISA instrument handle used to access
 *      instrument specific data. Must be initialized by
 *      tkvx4780_init() prior to use.
 *  ViStatus retError: error code to check.
 *  ViInt32 ch1,ch2,ch3,ch4,allChannelsInGroup: channels to enable
 *  ViChar ch_list[]: channel list
 *
 * Returns:
 *	VI_SUCCESS if no errors occurred, error code otherwise
 ***************************************************************************/

ViStatus _VI_FUNC tkvx4780_channellist_grp1 (ViSession instrumentHandle,
											 ViInt32 ch1,   ViInt32 ch2,
											 ViInt32 ch3,   ViInt32 ch4,
											 ViInt32 allChannelsInGroup, ViChar ch_list[]);

/***************************************************************************
 * Function: tkvx4780_build_chlist_grp1
 *
 * Purpose:
 *  This function builds the channel list from the channel(s) enabled
 *
 *  Note:  This function is now an EXCEPTION function because it is 
 *         obsolete with the new instrument setup and query commands.
 *         It is included for backwards compatibility and is not in 
 *         the function panel.
 *
 * Parameters:
 *	ViSession instrumentHandle: VISA instrument handle used to access
 *      instrument specific data. Must be initialized by
 *      tkvx4780_init() prior to use.
 *  ViStatus retError: error code to check.
 *  ViInt32 ch1,ch2,ch3,ch4,allChannelsInGroup: channels to enable
 *  ViString buf: holds channel list
 *
 * Returns:
 *	VI_SUCCESS if no errors occurred, error code otherwise
 ***************************************************************************/

ViStatus _VI_FUNC tkvx4780_build_chlist_grp1 (ViSession instrumentHandle,
											  ViInt32 ch1,   ViInt32 ch2,
											  ViInt32 ch3,   ViInt32 ch4,
											  ViInt32 allChannelsInGroup, ViChar buf[]);

/***************************************************************************
 * Function: tkvx4780_channellist_grp2
 *
 * Purpose:
 *  This function enables or disables the selected channel(s)
 *
 *  Note:  This function is now an EXCEPTION function because it is 
 *         obsolete with the new instrument setup and query commands.
 *         It is included for backwards compatibility and is not in 
 *         the function panel.
 *
 * Parameters:
 *	ViSession instrumentHandle: VISA instrument handle used to access
 *      instrument specific data. Must be initialized by
 *      tkvx4780_init() prior to use.
 *  ViStatus retError: error code to check.
 *  ViInt32 ch5,ch6,ch7,ch8,allChannelsInGroup: channels to enable
 *  ViChar ch_list[]: channel list
 *
 * Returns:
 *	VI_SUCCESS if no errors occurred, error code otherwise
 ***************************************************************************/

ViStatus _VI_FUNC tkvx4780_channellist_grp2 (ViSession instrumentHandle,
											 ViInt32 ch5,   ViInt32 ch6,
											 ViInt32 ch7,   ViInt32 ch8,
											 ViInt32 allChannelsInGroup, ViChar ch_list[]);

/***************************************************************************
 * Function: tkvx4780_build_chlist_grp2
 *
 * Purpose:
 *  This function builds the channel list from the channel(s) enabled
 *
 *  Note:  This function is now an EXCEPTION function because it is 
 *         obsolete with the new instrument setup and query commands.
 *         It is included for backwards compatibility and is not in 
 *         the function panel.
 *
 * Parameters:
 *	ViSession instrumentHandle: VISA instrument handle used to access
 *      instrument specific data. Must be initialized by
 *      tkvx4780_init() prior to use.
 *  ViStatus retError: error code to check.
 *  ViInt32 ch5,ch6,ch7,ch8,allChannelsInGroup: channels to enable
 *  ViString buf: holds channel list
 *
 * Returns:
 *	VI_SUCCESS if no errors occurred, error code otherwise
 ***************************************************************************/

ViStatus _VI_FUNC tkvx4780_build_chlist_grp2 (ViSession instrumentHandle,
											  ViInt32 ch5,   ViInt32 ch6,
											  ViInt32 ch7,   ViInt32 ch8,
											  ViInt32 allChannelsInGroup, ViChar buf[]);

/***************************************************************************
 * Function: tkvx4780_channellist_grp3
 *
 * Purpose:
 *  This function enables or disables the selected channel(s)
 *
 *  Note:  This function is now an EXCEPTION function because it is 
 *         obsolete with the new instrument setup and query commands.
 *         It is included for backwards compatibility and is not in 
 *         the function panel.
 *
 * Parameters:
 *	ViSession instrumentHandle: VISA instrument handle used to access
 *      instrument specific data. Must be initialized by
 *      tkvx4780_init() prior to use.
 *  ViStatus retError: error code to check.
 *  ViInt32 ch9,ch10,ch11,ch12,allChannelsInGroup: channels to enable
 *  ViChar ch_list[]: channel list
 *
 * Returns:
 *	VI_SUCCESS if no errors occurred, error code otherwise
 ***************************************************************************/

ViStatus _VI_FUNC tkvx4780_channellist_grp3 (ViSession instrumentHandle,
											 ViInt32 ch9,   ViInt32 ch10,
											 ViInt32 ch11,  ViInt32 ch12,
											 ViInt32 allChannelsInGroup, ViChar ch_list[]);

/***************************************************************************
 * Function: tkvx4780_build_chlist_grp3
 *
 * Purpose:
 *  This function builds the channel list from the channel(s) enabled
 *
 *  Note:  This function is now an EXCEPTION function because it is 
 *         obsolete with the new instrument setup and query commands.
 *         It is included for backwards compatibility and is not in 
 *         the function panel.
 *
 * Parameters:
 *	ViSession instrumentHandle: VISA instrument handle used to access
 *      instrument specific data. Must be initialized by
 *      tkvx4780_init() prior to use.
 *  ViStatus retError: error code to check.
 *  ViInt32 ch9,ch10,ch11,ch12,allChannelsInGroup: channels to enable
 *  ViString buf: holds channel list
 *
 * Returns:
 *	VI_SUCCESS if no errors occurred, error code otherwise
 ***************************************************************************/

ViStatus _VI_FUNC tkvx4780_build_chlist_grp3 (ViSession instrumentHandle,
											  ViInt32 ch9,   ViInt32 ch10,
											  ViInt32 ch11,  ViInt32 ch12,
											  ViInt32 allChannelsInGroup, ViChar buf[]);

/***************************************************************************
 * Function: tkvx4780_channellist_grp4
 *
 * Purpose:
 *  This function enables or disables the selected channel(s)
 *
 *  Note:  This function is now an EXCEPTION function because it is 
 *         obsolete with the new instrument setup and query commands.
 *         It is included for backwards compatibility and is not in 
 *         the function panel.
 *
 * Parameters:
 *	ViSession instrumentHandle: VISA instrument handle used to access
 *      instrument specific data. Must be initialized by
 *      tkvx4780_init() prior to use.
 *  ViStatus retError: error code to check.
 *  ViInt32 ch13,ch14,ch15,ch16,allChannelsInGroup: channels to enable
 *  ViChar ch_list[]: channel list
 *
 * Returns:
 *	VI_SUCCESS if no errors occurred, error code otherwise
 ***************************************************************************/

ViStatus _VI_FUNC tkvx4780_channellist_grp4 (ViSession instrumentHandle,
											 ViInt32 ch13,  ViInt32 ch14,
											 ViInt32 ch15,  ViInt32 ch16,
											 ViInt32 allChannelsInGroup, ViChar ch_list[]);

/***************************************************************************
 * Function: tkvx4780_build_chlist_grp4
 *
 * Purpose:
 *  This function builds the channel list from the channel(s) enabled
 *
 *  Note:  This function is now an EXCEPTION function because it is 
 *         obsolete with the new instrument setup and query commands.
 *         It is included for backwards compatibility and is not in 
 *         the function panel.
 *
 * Parameters:
 *	ViSession instrumentHandle: VISA instrument handle used to access
 *      instrument specific data. Must be initialized by
 *      tkvx4780_init() prior to use.
 *  ViStatus retError: error code to check.
 *  ViInt32 ch13,ch14,ch15,ch16,allChannelsInGroup: channels to enable
 *  ViString buf: holds channel list
 *
 * Returns:
 *	VI_SUCCESS if no errors occurred, error code otherwise
 ***************************************************************************/

ViStatus _VI_FUNC tkvx4780_build_chlist_grp4 (ViSession instrumentHandle,
											  ViInt32 ch13,  ViInt32 ch14,
											  ViInt32 ch15,  ViInt32 ch16,
											  ViInt32 allChannelsInGroup, ViChar buf[]);

/***************************************************************************
 * Function: tkvx4780_setGain
 *
 * Purpose:
 *      This function sets the gain for the selected channel(s)
 *
 *  Note:  This is an EXCEPTION function (unable to create function panel
 *         with variable array length) retained for backward compatibility.
 *
 * Parameters:
 *	ViSession instrumentHandle: VISA instrument handle used to access
 *      instrument specific data. Must be initialized by
 *      tkvx4780_init() prior to use.
 *  ViStatus retError: error code to check.
 *  ViChar ch_list[]: channels to enable
 *  ViInt32 gain: gain
 *
 * Returns:
 *	VI_SUCCESS if no errors occurred, error code otherwise
 ***************************************************************************/

ViStatus _VI_FUNC tkvx4780_setGain (ViSession instrumentHandle,
								 ViChar ch_list[],
								 ViInt32 gain);

/***************************************************************************
 * Function: tkvx4780_setGain_grp1
 *
 * Purpose:
 *      This function sets the gain for the selected channel(s)
 *
 * Parameters:
 *	ViSession instrumentHandle: VISA instrument handle used to access
 *      instrument specific data. Must be initialized by
 *      tkvx4780_init() prior to use.
 *  ViStatus retError: error code to check.
 *  ViInt32 ch1,ch2,ch3,ch4,allChannelsInGroup: channels to enable
 *  ViInt32 gain: gain
 *
 * Returns:
 *	VI_SUCCESS if no errors occurred, error code otherwise
 ***************************************************************************/

ViStatus _VI_FUNC tkvx4780_setGain_grp1 (ViSession instrumentHandle,
									  ViInt32 ch1,   ViInt32 ch2,
									  ViInt32 ch3,   ViInt32 ch4,
									  ViInt32 allChannelsInGroup, ViInt32 gain);

/***************************************************************************
 * Function: tkvx4780_setGain_grp2
 *
 * Purpose:
 *      This function sets the gain for the selected channel(s)
 *
 * Parameters:
 *	ViSession instrumentHandle: VISA instrument handle used to access
 *      instrument specific data. Must be initialized by
 *      tkvx4780_init() prior to use.
 *  ViStatus retError: error code to check.
 *  ViInt32 ch5,ch6,ch7,ch8,allChannelsInGroup: channels to enable
 *  ViInt32 gain: gain
 *
 * Returns:
 *	VI_SUCCESS if no errors occurred, error code otherwise
 ***************************************************************************/

ViStatus _VI_FUNC tkvx4780_setGain_grp2 (ViSession instrumentHandle,
									  ViInt32 ch5,   ViInt32 ch6,
									  ViInt32 ch7,   ViInt32 ch8,
									  ViInt32 allChannelsInGroup, ViInt32 gain);

/***************************************************************************
 * Function: tkvx4780_setGain_grp3
 *
 * Purpose:
 *      This function sets the gain for the selected channel(s)
 *
 * Parameters:
 *	ViSession instrumentHandle: VISA instrument handle used to access
 *      instrument specific data. Must be initialized by
 *      tkvx4780_init() prior to use.
 *  ViStatus retError: error code to check.
 *  ViInt32 ch9,ch10,ch11,ch12,allChannelsInGroup: channels to enable
 *  ViInt32 gain: gain
 *
 * Returns:
 *	VI_SUCCESS if no errors occurred, error code otherwise
 ***************************************************************************/

ViStatus _VI_FUNC tkvx4780_setGain_grp3 (ViSession instrumentHandle,
									  ViInt32 ch9,   ViInt32 ch10,
									  ViInt32 ch11,  ViInt32 ch12,
									  ViInt32 allChannelsInGroup, ViInt32 gain);

/***************************************************************************
 * Function: tkvx4780_setGain_grp4
 *
 * Purpose:
 *      This function sets the gain for the selected channel(s)
 *
 * Parameters:
 *	ViSession instrumentHandle: VISA instrument handle used to access
 *      instrument specific data. Must be initialized by
 *      tkvx4780_init() prior to use.
 *  ViStatus retError: error code to check.
 *  ViInt32 ch13,ch14,ch15,ch16,allChannelsInGroup: channels to enable
 *  ViInt32 gain: gain
 *
 * Returns:
 *	VI_SUCCESS if no errors occurred, error code otherwise
 ***************************************************************************/

ViStatus _VI_FUNC tkvx4780_setGain_grp4 (ViSession instrumentHandle,
									  ViInt32 ch13,  ViInt32 ch14,
									  ViInt32 ch15,  ViInt32 ch16,
									  ViInt32 allChannelsInGroup, ViInt32 gain);

/***************************************************************************
 * Function: tkvx4780_attenuation_off
 *
 * Purpose:
 *      This function bypasses the attenuator
 *      for the selected channel(s)
 *
 *  Note:  This is an EXCEPTION function (unable to create function panel
 *         with variable array length) retained for backward compatibility.
 *
 * Parameters:
 *	ViSession instrumentHandle: VISA instrument handle used to access
 *      instrument specific data. Must be initialized by
 *      tkvx4780_init() prior to use.
 *  ViStatus retError: error code to check.
 *  ViChar ch_list[]: channels to disable attenuation.
 *
 * Returns:
 *	VI_SUCCESS if no errors occurred, error code otherwise
 ***************************************************************************/

ViStatus _VI_FUNC tkvx4780_attenuation_off (ViSession instrumentHandle,
											ViChar ch_list[]);

/***************************************************************************
 * Function: tkvx4780_attenuation_on
 *
 * Purpose:
 *      This function inserts the attenuator into the signal path
 *      for the selected channel(s)
 *
 *  Note:  This is an EXCEPTION function (unable to create function panel
 *         with variable array length) retained for backward compatibility.
 *
 * Parameters:
 *	ViSession instrumentHandle: VISA instrument handle used to access
 *      instrument specific data. Must be initialized by
 *      tkvx4780_init() prior to use.
 *  ViStatus retError: error code to check.
 *  ViChar ch_list[]: channels to enable the attenuator.
 *
 * Returns:
 *	VI_SUCCESS if no errors occurred, error code otherwise
 ***************************************************************************/

ViStatus _VI_FUNC tkvx4780_attenuation_on (ViSession instrumentHandle,
										   ViChar ch_list[]);

/***************************************************************************
 * Function: tkvx4780_setAttenuation
 *
 * Purpose:
 *      This function sets the attenuation for the selected channel(s)
 *
 *  Note:  This is an EXCEPTION function (unable to create function panel
 *         with variable array length) retained for backward compatibility.
 *
 * Parameters:
 *	ViSession instrumentHandle: VISA instrument handle used to access
 *      instrument specific data. Must be initialized by
 *      tkvx4780_init() prior to use.
 *  ViStatus retError: error code to check.
 *  ViChar ch_list[]: channels to enable
 *  ViInt32 attenuation: attenuation
 *
 * Returns:
 *	VI_SUCCESS if no errors occurred, error code otherwise
 ***************************************************************************/

ViStatus _VI_FUNC tkvx4780_setAttenuation (ViSession instrumentHandle,
										ViChar ch_list[],
										ViInt32 attenuation);

/***************************************************************************
 * Function: tkvx4780_setAttenuation_grp1
 *
 * Purpose:
 *      This function sets the attenuation for the selected channel(s)
 *
 * Parameters:
 *	ViSession instrumentHandle: VISA instrument handle used to access
 *      instrument specific data. Must be initialized by
 *      tkvx4780_init() prior to use.
 *  ViStatus retError: error code to check.
 *  ViInt32 ch1,ch2,ch3,ch4,allChannelsInGroup: channels to enable/modify.
 *  ViInt32 attenuation: attenuation level.
 *
 * Returns:
 *	VI_SUCCESS if no errors occurred, error code otherwise
 ***************************************************************************/

ViStatus _VI_FUNC tkvx4780_setAttenuation_grp1 (ViSession instrumentHandle,
											 ViInt32 ch1,   ViInt32 ch2,
											 ViInt32 ch3,   ViInt32 ch4,
											 ViInt32 allChannelsInGroup, ViInt32 attenuation);

/***************************************************************************
 * Function: tkvx4780_setAttenuation_grp2
 *
 * Purpose:
 *      This function sets the attenuation for the selected channel(s)
 *
 * Parameters:
 *	ViSession instrumentHandle: VISA instrument handle used to access
 *      instrument specific data. Must be initialized by
 *      tkvx4780_init() prior to use.
 *  ViStatus retError: error code to check.
 *  ViInt32 ch5,ch6,ch7,ch8,allChannelsInGroup: channels to enable/modify.
 *  ViInt32 attenuation: attenuation level.
 *
 * Returns:
 *	VI_SUCCESS if no errors occurred, error code otherwise
 ***************************************************************************/

ViStatus _VI_FUNC tkvx4780_setAttenuation_grp2 (ViSession instrumentHandle,
											 ViInt32 ch5,   ViInt32 ch6,
											 ViInt32 ch7,   ViInt32 ch8,
											 ViInt32 allChannelsInGroup, ViInt32 attenuation);

/***************************************************************************
 * Function: tkvx4780_setAttenuation_grp3
 *
 * Purpose:
 *      This function sets the attenuation for the selected channel(s)
 *
 * Parameters:
 *	ViSession instrumentHandle: VISA instrument handle used to access
 *      instrument specific data. Must be initialized by
 *      tkvx4780_init() prior to use.
 *  ViStatus retError: error code to check.
 *  ViInt32 ch9,ch10,ch11,ch12,allChannelsInGroup: channels to enable/modify.
 *  ViInt32 attenuation: attenuation level.
 *
 * Returns:
 *	VI_SUCCESS if no errors occurred, error code otherwise
 ***************************************************************************/

ViStatus _VI_FUNC tkvx4780_setAttenuation_grp3 (ViSession instrumentHandle,
											 ViInt32 ch9,   ViInt32 ch10,
											 ViInt32 ch11,  ViInt32 ch12,
											 ViInt32 allChannelsInGroup, ViInt32 attenuation);

/***************************************************************************
 * Function: tkvx4780_setAttenuation_grp4
 *
 * Purpose:
 *      This function sets the attenuation for the selected channel(s)
 *
 * Parameters:
 *	ViSession instrumentHandle: VISA instrument handle used to access
 *      instrument specific data. Must be initialized by
 *      tkvx4780_init() prior to use.
 *  ViStatus retError: error code to check.
 *  ViInt32 ch13,ch14,ch15,ch16,allChannelsInGroup: channels to enable/modify.
 *  ViInt32 attenuation: attenuation level.
 *
 * Returns:
 *	VI_SUCCESS if no errors occurred, error code otherwise
 ***************************************************************************/

ViStatus _VI_FUNC tkvx4780_setAttenuation_grp4 (ViSession instrumentHandle,
											 ViInt32 ch13,  ViInt32 ch14,
											 ViInt32 ch15,  ViInt32 ch16,
											 ViInt32 allChannelsInGroup, ViInt32 attenuation);

/***************************************************************************
 * Function: tkvx4780_setInputState
 *
 * Purpose:
 *      This function sets the input state for the selected channel(s)
 *
 *  Note:  This is an EXCEPTION function (unable to create function panel
 *         with variable array length) retained for backward compatibility.
 *
 * Parameters:
 *	ViSession instrumentHandle: VISA instrument handle used to access
 *      instrument specific data. Must be initialized by
 *      tkvx4780_init() prior to use.
 *  ViStatus retError: error code to check.
 *  ViChar ch_list[]: channels to change.
 *  ViInt32 State: input state.
 *
 * Returns:
 *	VI_SUCCESS if no errors occurred, error code otherwise
 ***************************************************************************/

ViStatus _VI_FUNC tkvx4780_setInputState (ViSession instrumentHandle,
										ViChar ch_list[],
										ViInt32 state);

/***************************************************************************
 * Function: tkvx4780_setInputState_grp1
 *
 * Purpose:
 *      This function sets the input state for the selected channel(s)
 *
 * Parameters:
 *	ViSession instrumentHandle: VISA instrument handle used to access
 *      instrument specific data. Must be initialized by
 *      tkvx4780_init() prior to use.
 *  ViStatus retError: error code to check.
 *  ViInt32 ch1,ch2,ch3,ch4,allChannelsInGroup: channels to modify.
 *  ViInt32 State: input state.
 *
 * Returns:
 *	VI_SUCCESS if no errors occurred, error code otherwise
 ***************************************************************************/

ViStatus _VI_FUNC tkvx4780_setInputState_grp1 (ViSession instrumentHandle,
											 ViInt32 ch1,   ViInt32 ch2,
											 ViInt32 ch3,   ViInt32 ch4,
											 ViInt32 allChannelsInGroup, ViInt32 State);

/***************************************************************************
 * Function: tkvx4780_setInputState_grp2
 *
 * Purpose:
 *      This function sets the input state for the selected channel(s)
 *
 * Parameters:
 *	ViSession instrumentHandle: VISA instrument handle used to access
 *      instrument specific data. Must be initialized by
 *      tkvx4780_init() prior to use.
 *  ViStatus retError: error code to check.
 *  ViInt32 ch5,ch6,ch7,ch8,allChannelsInGroup: channels to modify.
 *  ViInt32 State: input state.
 *
 * Returns:
 *	VI_SUCCESS if no errors occurred, error code otherwise
 ***************************************************************************/

ViStatus _VI_FUNC tkvx4780_setInputState_grp2 (ViSession instrumentHandle,
											 ViInt32 ch5,   ViInt32 ch6,
											 ViInt32 ch7,   ViInt32 ch8,
											 ViInt32 allChannelsInGroup, ViInt32 State);

/***************************************************************************
 * Function: tkvx4780_setInputState_grp3
 *
 * Purpose:
 *      This function sets the input state for the selected channel(s)
 *
 * Parameters:
 *	ViSession instrumentHandle: VISA instrument handle used to access
 *      instrument specific data. Must be initialized by
 *      tkvx4780_init() prior to use.
 *  ViStatus retError: error code to check.
 *  ViInt32 ch9,ch10,ch11,ch12,allChannelsInGroup: channels to modify.
 *  ViInt32 State: input state.
 *
 * Returns:
 *	VI_SUCCESS if no errors occurred, error code otherwise
 ***************************************************************************/

ViStatus _VI_FUNC tkvx4780_setInputState_grp3 (ViSession instrumentHandle,
											 ViInt32 ch9,   ViInt32 ch10,
											 ViInt32 ch11,  ViInt32 ch12,
											 ViInt32 allChannelsInGroup, ViInt32 State);

/***************************************************************************
 * Function: tkvx4780_setInputState_grp4
 *
 * Purpose:
 *      This function sets the input state for the selected channel(s)
 *
 * Parameters:
 *	ViSession instrumentHandle: VISA instrument handle used to access
 *      instrument specific data. Must be initialized by
 *      tkvx4780_init() prior to use.
 *  ViStatus retError: error code to check.
 *  ViInt32 ch13,ch14,ch15,ch16,allChannelsInGroup: channels to modify.
 *  ViInt32 State: input state.
 *
 * Returns:
 *	VI_SUCCESS if no errors occurred, error code otherwise
 ***************************************************************************/

ViStatus _VI_FUNC tkvx4780_setInputState_grp4 (ViSession instrumentHandle,
											 ViInt32 ch13,  ViInt32 ch14,
											 ViInt32 ch15,  ViInt32 ch16,
											 ViInt32 allChannelsInGroup, ViInt32 State);

/***************************************************************************
 * Function: tkvx4780_setOutputState
 *
 * Purpose:
 *      This function sets the output state for the selected channel(s)
 *
 *  Note:  This is an EXCEPTION function (unable to create function panel
 *         with variable array length) retained for backward compatibility.
 *
 * Parameters:
 *	ViSession instrumentHandle: VISA instrument handle used to access
 *      instrument specific data. Must be initialized by
 *      tkvx4780_init() prior to use.
 *  ViStatus retError: error code to check.
 *  ViChar ch_list[]: channels to modify.
 *  ViInt32 State: output state.
 *
 * Returns:
 *	VI_SUCCESS if no errors occurred, error code otherwise
 ***************************************************************************/

ViStatus _VI_FUNC tkvx4780_setOutputState (ViSession instrumentHandle,
										 ViChar ch_list[],
										 ViInt32 state);

/***************************************************************************
 * Function: tkvx4780_setOutputState_grp1
 *
 * Purpose:
 *      This function sets the output state for the selected channel(s)
 *
 * Parameters:
 *	ViSession instrumentHandle: VISA instrument handle used to access
 *      instrument specific data. Must be initialized by
 *      tkvx4780_init() prior to use.
 *  ViStatus retError: error code to check.
 *  ViInt32 ch1,ch2,ch3,ch4,allChannelsInGroup: channels to modify.
 *  ViInt32 State: output state.
 *
 * Returns:
 *	VI_SUCCESS if no errors occurred, error code otherwise
 ***************************************************************************/

ViStatus _VI_FUNC tkvx4780_setOutputState_grp1 (ViSession instrumentHandle,
											  ViInt32 ch1,   ViInt32 ch2,
											  ViInt32 ch3,   ViInt32 ch4,
											  ViInt32 allChannelsInGroup, ViInt32 State);

/***************************************************************************
 * Function: tkvx4780_setOutputState_grp2
 *
 * Purpose:
 *      This function sets the output state for the selected channel(s)
 *
 * Parameters:
 *	ViSession instrumentHandle: VISA instrument handle used to access
 *      instrument specific data. Must be initialized by
 *      tkvx4780_init() prior to use.
 *  ViStatus retError: error code to check.
 *  ViInt32 ch5,ch6,ch7,ch8,allChannelsInGroup: channels to modify.
 *  ViInt32 State: output state.
 *
 * Returns:
 *	VI_SUCCESS if no errors occurred, error code otherwise
 ***************************************************************************/

ViStatus _VI_FUNC tkvx4780_setOutputState_grp2 (ViSession instrumentHandle,
											  ViInt32 ch5,   ViInt32 ch6,
											  ViInt32 ch7,   ViInt32 ch8,
											  ViInt32 allChannelsInGroup, ViInt32 State);

/***************************************************************************
 * Function: tkvx4780_setOutputState_grp3
 *
 * Purpose:
 *      This function sets the output state for the selected channel(s)
 *
 * Parameters:
 *	ViSession instrumentHandle: VISA instrument handle used to access
 *      instrument specific data. Must be initialized by
 *      tkvx4780_init() prior to use.
 *  ViStatus retError: error code to check.
 *  ViInt32 ch9,ch10,ch11,ch12,allChannelsInGroup: channels to modify.
 *  ViInt32 State: output state.
 *
 * Returns:
 *	VI_SUCCESS if no errors occurred, error code otherwise
 ***************************************************************************/

ViStatus _VI_FUNC tkvx4780_setOutputState_grp3 (ViSession instrumentHandle,
											  ViInt32 ch9,   ViInt32 ch10,
											  ViInt32 ch11,  ViInt32 ch12,
											  ViInt32 allChannelsInGroup, ViInt32 State);

/***************************************************************************
 * Function: tkvx4780_setOutputState_grp4
 *
 * Purpose:
 *      This function sets the output state for the selected channel(s)
 *
 * Parameters:
 *	ViSession instrumentHandle: VISA instrument handle used to access
 *      instrument specific data. Must be initialized by
 *      tkvx4780_init() prior to use.
 *  ViStatus retError: error code to check.
 *  ViInt32 ch13,ch14,ch15,ch16,allChannelsInGroup: channels to modify.
 *  ViInt32 State: output state.
 *
 * Returns:
 *	VI_SUCCESS if no errors occurred, error code otherwise
 ***************************************************************************/

ViStatus _VI_FUNC tkvx4780_setOutputState_grp4 (ViSession instrumentHandle,
											  ViInt32 ch13,  ViInt32 ch14,
											  ViInt32 ch15,  ViInt32 ch16,
											  ViInt32 allChannelsInGroup, ViInt32 State);

/***************************************************************************
 * Function: tkvx4780_setCoupling
 *
 * Purpose:
 *      This function sets the coupling for the selected channel(s)
 *
 *  Note:  This is an EXCEPTION function (unable to create function panel
 *         with variable array length) retained for backward compatibility.
 *
 * Parameters:
 *	ViSession instrumentHandle: VISA instrument handle used to access
 *      instrument specific data. Must be initialized by
 *      tkvx4780_init() prior to use.
 *  ViStatus retError: error code to check.
 *  ViChar ch_list[]: channels to enable.
 *  ViChar coupling[]: coupling type.
 *
 * Returns:
 *	VI_SUCCESS if no errors occurred, error code otherwise
 ***************************************************************************/

ViStatus _VI_FUNC tkvx4780_setCoupling (ViSession instrumentHandle,
									 ViChar ch_list[],
									 ViInt32 coupling);

/***************************************************************************
 * Function: tkvx4780_setCoupling_grp1
 *
 * Purpose:
 *      This function sets the coupling for the selected channel(s)
 *
 * Parameters:
 *	ViSession instrumentHandle: VISA instrument handle used to access
 *      instrument specific data. Must be initialized by
 *      tkvx4780_init() prior to use.
 *  ViStatus retError: error code to check.
 *  ViInt32 ch1,ch2,ch3,ch4,allChannelsInGroup: channels to eenable.
 *  ViChar coupling[]: coupling type.
 *
 * Returns:
 *	VI_SUCCESS if no errors occurred, error code otherwise
 ***************************************************************************/

ViStatus _VI_FUNC tkvx4780_setCoupling_grp1 (ViSession instrumentHandle,
										  ViInt32 ch1,   ViInt32 ch2,
										  ViInt32 ch3,   ViInt32 ch4,
										  ViInt32 allChannelsInGroup, ViChar coupling[]);

/***************************************************************************
 * Function: tkvx4780_setCoupling_grp2
 *
 * Purpose:
 *      This function sets the coupling for the selected channel(s)
 *
 * Parameters:
 *	ViSession instrumentHandle: VISA instrument handle used to access
 *      instrument specific data. Must be initialized by
 *      tkvx4780_init() prior to use.
 *  ViStatus retError: error code to check.
 *  ViInt32 ch5,ch6,ch7,ch8,allChannelsInGroup: channels to enable.
 *  ViChar coupling[]: coupling type.
 *
 * Returns:
 *	VI_SUCCESS if no errors occurred, error code otherwise
 ***************************************************************************/

ViStatus _VI_FUNC tkvx4780_setCoupling_grp2 (ViSession instrumentHandle,
										  ViInt32 ch5, ViInt32 ch6,
										  ViInt32 ch7, ViInt32 ch8,
										  ViInt32 allChannelsInGroup, ViChar coupling[]);

/***************************************************************************
 * Function: tkvx4780_setCoupling_grp3
 *
 * Purpose:
 *      This function sets the coupling for the selected channel(s)
 *
 * Parameters:
 *	ViSession instrumentHandle: VISA instrument handle used to access
 *      instrument specific data. Must be initialized by
 *      tkvx4780_init() prior to use.
 *  ViStatus retError: error code to check.
 *  ViInt32 ch9,ch10,ch11,ch12,allChannelsInGroup: channels to enable.
 *  ViChar coupling[]: coupling type.
 *
 * Returns:
 *	VI_SUCCESS if no errors occurred, error code otherwise
 ***************************************************************************/

ViStatus _VI_FUNC tkvx4780_setCoupling_grp3 (ViSession instrumentHandle,
										  ViInt32 ch9,   ViInt32 ch10,
										  ViInt32 ch11,  ViInt32 ch12,
										  ViInt32 allChannelsInGroup, ViChar coupling[]);

/***************************************************************************
 * Function: tkvx4780_setCoupling_grp4
 *
 * Purpose:
 *      This function sets the coupling for the selected channel(s)
 *
 * Parameters:
 *	ViSession instrumentHandle: VISA instrument handle used to access
 *      instrument specific data. Must be initialized by
 *      tkvx4780_init() prior to use.
 *  ViStatus retError: error code to check.
 *  ViInt32 ch13,ch14,ch15,ch16,allChannelsInGroup: channels to enable.
 *  ViChar coupling[]: coupling type.
 *
 * Returns:
 *	VI_SUCCESS if no errors occurred, error code otherwise
 ***************************************************************************/

ViStatus _VI_FUNC tkvx4780_setCoupling_grp4 (ViSession instrumentHandle,
										  ViInt32 ch13,  ViInt32 ch14,
										  ViInt32 ch15,  ViInt32 ch16,
										  ViInt32 allChannelsInGroup, ViChar coupling[]);

/***************************************************************************
 * Function: tkvx4780_setFilterFreq
 *
 * Purpose:
 *      This function sets the cutoff frequency setting of the
 *      lowpass filter for the selected channel(s).
 *
 *  Note:  This is an EXCEPTION function (unable to create function panel
 *         with variable array length) retained for backward compatibility.
 *
 * Parameters:
 *	ViSession instrumentHandle: VISA instrument handle used to access
 *      instrument specific data. Must be initialized by
 *      tkvx4780_init() prior to use.
 *  ViStatus retError: error code to check.
 *  ViChar ch_list[]: channels to enable.
 *  ViInt32 frequency: frequency setting.
 *
 * Returns:
 *	VI_SUCCESS if no errors occurred, error code otherwise
 ***************************************************************************/

ViStatus _VI_FUNC tkvx4780_setFilterFreq (ViSession instrumentHandle,
										ViChar ch_list[],
										ViInt32 frequency);

/***************************************************************************
 * Function: tkvx4780_setFilterFreq_grp1
 *
 * Purpose:
 *      This function sets the cutoff frequency setting of the
 *      lowpass filter for the selected channel(s).
 *
 * Parameters:
 *	ViSession instrumentHandle: VISA instrument handle used to access
 *      instrument specific data. Must be initialized by
 *      tkvx4780_init() prior to use.
 *  ViStatus retError: error code to check.
 *  ViInt32 ch1,ch2,ch3,ch4,allChannelsInGroup: channels to enable.
 *  ViInt32 frequency: frequency setting.
 *
 * Returns:
 *	VI_SUCCESS if no errors occurred, error code otherwise
 ***************************************************************************/

ViStatus _VI_FUNC tkvx4780_setFilterFreq_grp1 (ViSession instrumentHandle,
											 ViInt32 ch1,   ViInt32 ch2,
											 ViInt32 ch3,   ViInt32 ch4,
											 ViInt32 allChannelsInGroup, ViInt32 frequency);

/***************************************************************************
 * Function: tkvx4780_setFilterFreq_grp2
 *
 * Purpose:
 *      This function sets the cutoff frequency setting of the
 *      lowpass filter for the selected channel(s).
 *
 * Parameters:
 *	ViSession instrumentHandle: VISA instrument handle used to access
 *      instrument specific data. Must be initialized by
 *      tkvx4780_init() prior to use.
 *  ViStatus retError: error code to check.
 *  ViInt32 ch5,ch6,ch7,ch8,allChannelsInGroup: channels to enable.
 *  ViInt32 frequency: frequency setting.
 *
 * Returns:
 *	VI_SUCCESS if no errors occurred, error code otherwise
 ***************************************************************************/

ViStatus _VI_FUNC tkvx4780_setFilterFreq_grp2 (ViSession instrumentHandle,
											 ViInt32 ch5,   ViInt32 ch6,
											 ViInt32 ch7,   ViInt32 ch8,
											 ViInt32 allChannelsInGroup, ViInt32 frequency);

/***************************************************************************
 * Function: tkvx4780_setFilterFreq_grp3
 *
 * Purpose:
 *      This function sets the cutoff frequency setting of the
 *      lowpass filter for the selected channel(s).
 *
 * Parameters:
 *	ViSession instrumentHandle: VISA instrument handle used to access
 *      instrument specific data. Must be initialized by
 *      tkvx4780_init() prior to use.
 *  ViStatus retError: error code to check.
 *  ViInt32 ch9,ch10,ch11,ch12,allChannelsInGroup: channels to enable.
 *  ViInt32 frequency: frequency setting.
 *
 * Returns:
 *	VI_SUCCESS if no errors occurred, error code otherwise
 ***************************************************************************/

ViStatus _VI_FUNC tkvx4780_setFilterFreq_grp3 (ViSession instrumentHandle,
											 ViInt32 ch9,   ViInt32 ch10,
											 ViInt32 ch11,  ViInt32 ch12,
											 ViInt32 allChannelsInGroup, ViInt32 frequency);

/***************************************************************************
 * Function: tkvx4780_setFilterFreq_grp4
 *
 * Purpose:
 *      This function sets the cutoff frequency setting of the
 *      lowpass filter for the selected channel(s).
 *
 * Parameters:
 *	ViSession instrumentHandle: VISA instrument handle used to access
 *      instrument specific data. Must be initialized by
 *      tkvx4780_init() prior to use.
 *  ViStatus retError: error code to check.
 *  ViInt32 ch13,ch14,ch15,ch16,allChannelsInGroup: channels to enable.
 *  ViInt32 frequency: frequency setting.
 *
 * Returns:
 *	VI_SUCCESS if no errors occurred, error code otherwise
 ***************************************************************************/

ViStatus _VI_FUNC tkvx4780_setFilterFreq_grp4 (ViSession instrumentHandle,
											 ViInt32 ch13,  ViInt32 ch14,
											 ViInt32 ch15,  ViInt32 ch16,
											 ViInt32 allChannelsInGroup, ViInt32 frequency);

/***************************************************************************
 * Function: tkvx4780_setGainTrim
 *
 * Purpose:
 *   This function sets the input gain trim for the selected channel(s)
 *
 *  Note:  This is an EXCEPTION function (unable to create function panel
 *         with variable array length) retained for backward compatibility.
 *
 * Parameters:
 *	ViSession instrumentHandle: VISA instrument handle used to access
 *      instrument specific data. Must be initialized by
 *      tkvx4780_init() prior to use.
 *  ViStatus retError: error code to check.
 *  ViChar ch_list[]: channels to enable.
 *  ViInt32 gaintrim: gain trim data.
 *
 * Returns:
 *	VI_SUCCESS if no errors occurred, error code otherwise
 ***************************************************************************/

ViStatus _VI_FUNC tkvx4780_setGainTrim (ViSession instrumentHandle,
									  ViChar ch_list[],
									  ViInt32 gaintrim);

/***************************************************************************
 * Function: tkvx4780_setGainTrim_grp1
 *
 * Purpose:
 *   This function sets the input gain trim for the selected channel(s)
 *
 * Parameters:
 *	ViSession instrumentHandle: VISA instrument handle used to access
 *      instrument specific data. Must be initialized by
 *      tkvx4780_init() prior to use.
 *  ViStatus retError: error code to check.
 *  ViInt32 ch1,ch2,ch3,ch4,allChannelsInGroup: channels to enable.
 *  ViInt32 gaintrim: gain trim data.
 *
 * Returns:
 *	VI_SUCCESS if no errors occurred, error code otherwise
 ***************************************************************************/

ViStatus _VI_FUNC tkvx4780_setGainTrim_grp1 (ViSession instrumentHandle,
										   ViInt32 ch1,   ViInt32 ch2,
										   ViInt32 ch3,   ViInt32 ch4,
										   ViInt32 allChannelsInGroup, ViInt32 gaintrim);

/***************************************************************************
 * Function: tkvx4780_setGainTrim_grp2
 *
 * Purpose:
 *   This function sets the input gain trim for the selected channel(s)
 *
 * Parameters:
 *	ViSession instrumentHandle: VISA instrument handle used to access
 *      instrument specific data. Must be initialized by
 *      tkvx4780_init() prior to use.
 *  ViStatus retError: error code to check.
 *  ViInt32 ch5,ch6,ch7,ch8,allChannelsInGroup: channels to enable.
 *  ViInt32 gaintrim: gain trim data.
 *
 * Returns:
 *	VI_SUCCESS if no errors occurred, error code otherwise
 ***************************************************************************/

ViStatus _VI_FUNC tkvx4780_setGainTrim_grp2 (ViSession instrumentHandle,
										   ViInt32 ch5,   ViInt32 ch6,
										   ViInt32 ch7,   ViInt32 ch8,
										   ViInt32 allChannelsInGroup, ViInt32 gaintrim);

/***************************************************************************
 * Function: tkvx4780_setGainTrim_grp3
 *
 * Purpose:
 *   This function sets the input gain trim for the selected channel(s)
 *
 * Parameters:
 *	ViSession instrumentHandle: VISA instrument handle used to access
 *      instrument specific data. Must be initialized by
 *      tkvx4780_init() prior to use.
 *  ViStatus retError: error code to check.
 *  ViInt32 ch9,ch10,ch11,ch12,allChannelsInGroup: channels to enable.
 *  ViInt32 gaintrim: gain trim data.
 *
 * Returns:
 *	VI_SUCCESS if no errors occurred, error code otherwise
 ***************************************************************************/

ViStatus _VI_FUNC tkvx4780_setGainTrim_grp3 (ViSession instrumentHandle,
										   ViInt32 ch9,   ViInt32 ch10,
										   ViInt32 ch11,  ViInt32 ch12,
										   ViInt32 allChannelsInGroup, ViInt32 gaintrim);

/***************************************************************************
 * Function: tkvx4780_setGainTrim_grp4
 *
 * Purpose:
 *   This function sets the input gain trim for the selected channel(s)
 *
 * Parameters:
 *	ViSession instrumentHandle: VISA instrument handle used to access
 *      instrument specific data. Must be initialized by
 *      tkvx4780_init() prior to use.
 *  ViStatus retError: error code to check.
 *  ViInt32 ch13,ch14,ch15,ch16,allChannelsInGroup: channels to enable.
 *  ViInt32 gaintrim: gain trim data.
 *
 * Returns:
 *	VI_SUCCESS if no errors occurred, error code otherwise
 ***************************************************************************/

ViStatus _VI_FUNC tkvx4780_setGainTrim_grp4 (ViSession instrumentHandle,
										   ViInt32 ch13,  ViInt32 ch14,
										   ViInt32 ch15,  ViInt32 ch16,
										   ViInt32 allChannelsInGroup, ViInt32 gaintrim);

/***************************************************************************
 * Function: tkvx4780_setOffsetTrim
 *
 * Purpose:
 *   This function sets the output offset trim for the selected channel(s).
 *
 *  Note:  This is an EXCEPTION function (unable to create function panel
 *         with variable array length) retained for backward compatibility.
 *
 * Parameters:
 *	ViSession instrumentHandle: VISA instrument handle used to access
 *      instrument specific data. Must be initialized by
 *      tkvx4780_init() prior to use.
 *  ViStatus retError: error code to check.
 *  ViChar ch_list[]: channels to enable
 *  ViReal64 offsetTrim: offset trim data
 *
 * Returns:
 *	VI_SUCCESS if no errors occurred, error code otherwise
 ***************************************************************************/

ViStatus _VI_FUNC tkvx4780_setOffsetTrim (ViSession instrumentHandle,
										ViChar ch_list[],
										ViReal64 offsetTrim);

/***************************************************************************
 * Function: tkvx4780_setOffsetTrim_grp1
 *
 * Purpose:
 *   This function sets the output offset trim for the selected channel(s).
 *
 * Parameters:
 *	ViSession instrumentHandle: VISA instrument handle used to access
 *      instrument specific data. Must be initialized by
 *      tkvx4780_init() prior to use.
 *  ViStatus retError: error code to check.
 *  ViInt32 ch1,ch2,ch3,ch4,allChannelsInGroup: channels to enable.
 *  ViReal64 offsetTrim: offset trim data.
 *
 * Returns:
 *	VI_SUCCESS if no errors occurred, error code otherwise
 ***************************************************************************/

ViStatus _VI_FUNC tkvx4780_setOffsetTrim_grp1 (ViSession instrumentHandle,
											 ViInt32 ch1, ViInt32 ch2,
											 ViInt32 ch3, ViInt32 ch4,
											 ViInt32 allChannelsInGroup,
											 ViReal64 offsetTrim);

/***************************************************************************
 * Function: tkvx4780_setOffsetTrim_grp2
 *
 * Purpose:
 *   This function sets the output offset trim for the selected channel(s).
 *
 * Parameters:
 *	ViSession instrumentHandle: VISA instrument handle used to access
 *      instrument specific data. Must be initialized by
 *      tkvx4780_init() prior to use.
 *  ViStatus retError: error code to check.
 *  ViInt32 ch5,ch6,ch7,ch8,allChannelsInGroup: channels to enable.
 *  ViReal64 offsetTrim: offset trim data.
 *
 * Returns:
 *	VI_SUCCESS if no errors occurred, error code otherwise
 ***************************************************************************/

ViStatus _VI_FUNC tkvx4780_setOffsetTrim_grp2 (ViSession instrumentHandle,
											 ViInt32 ch5, ViInt32 ch6,
											 ViInt32 ch7, ViInt32 ch8,
											 ViInt32 allChannelsInGroup,
											 ViReal64 offsetTrim);

/***************************************************************************
 * Function: tkvx4780_setOffsetTrim_grp3
 *
 * Purpose:
 *   This function sets the output offset trim for the selected channel(s).
 *
 * Parameters:
 *	ViSession instrumentHandle: VISA instrument handle used to access
 *      instrument specific data. Must be initialized by
 *      tkvx4780_init() prior to use.
 *  ViStatus retError: error code to check.
 *  ViInt32 ch9,ch10,ch11,ch12,allChannelsInGroup: channels to enable.
 *  ViReal64 offsetTrim: offset trim data.
 *
 * Returns:
 *	VI_SUCCESS if no errors occurred, error code otherwise
 ***************************************************************************/

ViStatus _VI_FUNC tkvx4780_setOffsetTrim_grp3 (ViSession instrumentHandle,
											 ViInt32 ch9, ViInt32 ch10,
											 ViInt32 ch11, ViInt32 ch12,
											 ViInt32 allChannelsInGroup,
											 ViReal64 offsetTrim);

/***************************************************************************
 * Function: tkvx4780_setOffsetTrim_grp4
 *
 * Purpose:
 *   This function sets the output offset trim for the selected channel(s).
 *
 * Parameters:
 *	ViSession instrumentHandle: VISA instrument handle used to access
 *      instrument specific data. Must be initialized by
 *      tkvx4780_init() prior to use.
 *  ViStatus retError: error code to check.
 *  ViInt32 ch13,ch14,ch15,ch16,allChannelsInGroup: channels to enable.
 *  ViReal64 offsetTrim: offset trim data.
 *
 * Returns:
 *	VI_SUCCESS if no errors occurred, error code otherwise
 ***************************************************************************/

ViStatus _VI_FUNC tkvx4780_setOffsetTrim_grp4 (ViSession instrumentHandle,
											 ViInt32 ch13, ViInt32 ch14,
											 ViInt32 ch15, ViInt32 ch16,
											 ViInt32 allChannelsInGroup,
											 ViReal64 offsetTrim);

/***************************************************************************
 * Function: tkvx4780_queryInputState
 *
 * Purpose:
 *   This function returns the state of the input isolation
 *   relay for the selected channel(s).
 *
 *  Note:  This is an EXCEPTION function (unable to create function panel
 *         with variable array length) retained for backward compatibility.
 *
 * Parameters:
 *	ViSession instrumentHandle: VISA instrument handle used to access
 *      instrument specific data. Must be initialized by
 *      tkvx4780_init() prior to use.
 *  ViStatus retError: error code to check.
 *  ViChar ch_list[]: channels to read.
 *  ViChar message[]: readback data.
 *
 * Returns:
 *	VI_SUCCESS if no errors occurred, error code otherwise
 ***************************************************************************/

ViStatus _VI_FUNC tkvx4780_queryInputState (ViSession instrumentHandle,
											  ViChar ch_list[],
											  ViChar message[]);


/***************************************************************************
 * Function: tkvx4780_queryOutputState
 *
 * Purpose:
 *   This function returns the state of the output isolation
 *   relay for the selected channel(s)
 *
 *  Note:  This is an EXCEPTION function (unable to create function panel
 *         with variable array length) retained for backward compatibility.
 *
 * Parameters:
 *	ViSession instrumentHandle: VISA instrument handle used to access
 *      instrument specific data. Must be initialized by
 *      tkvx4780_init() prior to use.
 *  ViStatus retError: error code to check.
 *  ViChar ch_list[]: channels to read.
 *  ViChar message[]: readback data.
 *
 * Returns:
 *	VI_SUCCESS if no errors occurred, error code otherwise
 ***************************************************************************/

ViStatus _VI_FUNC tkvx4780_queryOutputState (ViSession instrumentHandle,
											   ViChar ch_list[],
											   ViChar message[]);

/***************************************************************************
 * Function: tkvx4780_queryCoupling
 *
 * Purpose:
 *   This function returns the state of the input coupling
 *   relay for the selected channel(s)
 *
 *  Note:  This is an EXCEPTION function (unable to create function panel
 *         with variable array length) retained for backward compatibility.
 *
 * Parameters:
 *	ViSession instrumentHandle: VISA instrument handle used to access
 *      instrument specific data. Must be initialized by
 *      tkvx4780_init() prior to use.
 *  ViStatus retError: error code to check.
 *  ViChar ch_list[]: channels to read.
 *  ViChar message[]: readback data.
 *
 * Returns:
 *	VI_SUCCESS if no errors occurred, error code otherwise
 ***************************************************************************/

ViStatus _VI_FUNC tkvx4780_queryCoupling (ViSession instrumentHandle,
										   ViChar ch_list[],
										   ViChar message[]);

/***************************************************************************
 * Function: tkvx4780_queryAttenuation
 *
 * Purpose:
 *   This function returns the attenuation setting in
 *    volt/volt for the selected channel(s)
 *
 *  Note:  This is an EXCEPTION function (unable to create function panel
 *         with variable array length) retained for backward compatibility.
 *
 * Parameters:
 *	ViSession instrumentHandle: VISA instrument handle used to access
 *      instrument specific data. Must be initialized by
 *      tkvx4780_init() prior to use.
 *  ViStatus retError: error code to check.
 *  ViChar ch_list[]: channels to read.
 *  ViChar message[]: readback data.
 *
 * Returns:
 *	VI_SUCCESS if no errors occurred, error code otherwise
 ***************************************************************************/

ViStatus _VI_FUNC tkvx4780_queryAttenuation (ViSession instrumentHandle,
											 ViChar ch_list[],
											 ViChar message[]);

/***************************************************************************
 * Function: tkvx4780_queryAttenuationState
 *
 * Purpose:
 *   This function returns the attenuation state
 *   for the selected channel(s)
 *
 *  Note:  This is an EXCEPTION function (unable to create function panel
 *         with variable array length) retained for backward compatibility.
 *
 * Parameters:
 *	ViSession instrumentHandle: VISA instrument handle used to access
 *      instrument specific data. Must be initialized by
 *      tkvx4780_init() prior to use.
 *  ViStatus retError: error code to check.
 *  ViChar ch_list[]: channels to read.
 *  ViChar message[]: readback data.
 *
 * Returns:
 *	VI_SUCCESS if no errors occurred, error code otherwise
 ***************************************************************************/

ViStatus _VI_FUNC tkvx4780_queryAttenuationState (ViSession instrumentHandle,
											  	  ViChar ch_list[],
											  	  ViChar message[]);

/***************************************************************************
 * Function: tkvx4780_queryGain
 *
 * Purpose:
 *   This function returns the gain setting in volt/volt
 *   for the selected channel(s)
 *
 *  Note:  This is an EXCEPTION function (unable to create function panel
 *         with variable array length) retained for backward compatibility.
 *
 * Parameters:
 *	ViSession instrumentHandle: VISA instrument handle used to access
 *      instrument specific data. Must be initialized by
 *      tkvx4780_init() prior to use.
 *  ViStatus retError: error code to check.
 *  ViChar ch_list[]: channels to read.
 *  ViChar message[]: readback data.
 *
 * Returns:
 *	VI_SUCCESS if no errors occurred, error code otherwise
 ***************************************************************************/

ViStatus _VI_FUNC tkvx4780_queryGain (ViSession instrumentHandle,
									  ViChar ch_list[],
									  ViChar message[]);

/***************************************************************************
 * Function: tkvx4780_queryGainTrim
 *
 * Purpose:
 *   This function returns the gain trim setting
 *   for the selected channel(s)
 *
 *  Note:  This is an EXCEPTION function (unable to create function panel
 *         with variable array length) retained for backward compatibility.
 *
 * Parameters:
 *	ViSession instrumentHandle: VISA instrument handle used to access
 *      instrument specific data. Must be initialized by
 *      tkvx4780_init() prior to use.
 *  ViStatus retError: error code to check.
 *  ViChar ch_list[]: channels to read.
 *  ViChar message[]: readback data.
 *
 * Returns:
 *	VI_SUCCESS if no errors occurred, error code otherwise
 ***************************************************************************/

ViStatus _VI_FUNC tkvx4780_queryGainTrim (ViSession instrumentHandle,
											ViChar ch_list[],
											ViChar message[]);

/***************************************************************************
 * Function: tkvx4780_queryOffsetTrim
 *
 * Purpose:
 *   This function returns the offset trim setting
 *   for the selected channel(s)
 *
 *  Note:  This is an EXCEPTION function (unable to create function panel
 *         with variable array length) retained for backward compatibility.
 *
 * Parameters:
 *	ViSession instrumentHandle: VISA instrument handle used to access
 *      instrument specific data. Must be initialized by
 *      tkvx4780_init() prior to use.
 *  ViStatus retError: error code to check.
 *  ViChar ch_list[]: channels to read.
 *  ViChar message[]: readback data.
 *
 * Returns:
 *	VI_SUCCESS if no errors occurred, error code otherwise
 ***************************************************************************/

ViStatus _VI_FUNC tkvx4780_queryOffsetTrim (ViSession instrumentHandle,
											ViChar ch_list[],
											ViChar message[]);

/***************************************************************************
 * Function: tkvx4780_queryFilterFreq
 *
 * Purpose:
 *   This function returns the cutoff frequency setting of the
 *   lowpass filter for the selected channel(s)
 *
 *  Note:  This is an EXCEPTION function (unable to create function panel
 *         with variable array length) retained for backward compatibility.
 *
 * Parameters:
 *	ViSession instrumentHandle: VISA instrument handle used to access
 *      instrument specific data. Must be initialized by
 *      tkvx4780_init() prior to use.
 *  ViStatus retError: error code to check.
 *  ViChar ch_list[]: channels to read.
 *  ViChar message[]: readback data.
 *
 * Returns:
 *	VI_SUCCESS if no errors occurred, error code otherwise
 ***************************************************************************/

ViStatus _VI_FUNC tkvx4780_queryFilterFreq (ViSession instrumentHandle,
											ViChar ch_list[],
											ViChar message[]);

/***************************************************************************
 * Function: tkvx4780_singleChannelState
 *
 * Purpose:
 *  This function performs an analysis of a single channel of the signal
 *  conditioner.  This function returns a string containing the current
 *  configuration of the selected channel.  The output string can be parsed
 *  to determine the settings.
 *
 *  The output string is a comma (,) delimited list of various queries about
 *  the channel.  The order of the query responses in the string follows.
 *  An example response string is "1,1,AC,1,10,5,5670,0.20000,468"
 *
 *		input state     	The state of the input isolation relay for
 *							the selected channel.  A 1 indicates the relay
 *							is closed and that the signal conditioning
 *							channel input is connected to the front panel
 *							input connector.  A 0 indicates the relay is
 *							open and the input is disconnected from the front
 *							panel input connector.
 *		output state		The state of the output isolation relay for the
 *							selected channel.  A 1 indicates the relay is
 *							closed and the output is connected to the front
 *							panel output connector.  A 0 indicates the relay
 *							is open and the output is disconnected from the
 *							front panel output connector.
 *		coupling			The state of the input coupling relay for the
 *							specified channel:  "DC", "AC", or "GRO" (ground).
 *		attenuation state   The state of the attenuator relay that indicates
 *							if the input attenuator is in the signal path or
 *							if it is bypassed.  A 1 indicates the attenuator
 *							is in the signal path; thus, the signal is modified
 *							by the attenuator setting.  A 0 value indicates
 *							that the attenuation circuits are taken out of the
 *							signal path; thus, the attenuator is bypassed and
 *							there is no distortion to the signal.
 *      attenuation value	The setting for the signal attenuation for the
 *							selected channel.  The input attenuator is set to
 *							divide the input signal by 10 or 100.
 *		gain				The gain setting, in volt/volt, for the specified
 * 							channel.  The gain setting is set to one of the
 *							following values:  1, 2, 5, 10, 20, 50, or 100.
 *		gain trim			The gain trim setting, in parts per million (PPM),
 *							for the selected channel.  This setting is used
 *							to correct the gain of a sensor.  The range of
 *							correction factors is +/- 10000 ppm = +/- 1%.
 *		offset trim			The output DC offset trim setting, in volts, for
 *							the specified channel.  This setting is used to
 *							null DC offset errors at the channel output.  The
 *							range of correction factors is +/- 0.2 V.
 *		filter frequency	The frequency setting of the lowpass filter cutoff,
 *							in Hz, for the specified channel.  The frequency
 *							range is 468 Hz to 7.02 kHz, in 468 Hz steps, and
 *							from 7.13 kHz to 107 kHz in 7.13 kHz steps.
 *
 * Parameters:
 *	ViSession instrumentHandle: VISA instrument handle used to access
 *      instrument specific data. Must be initialized by
 *      tkvx4780_init() prior to use.
 *  ViInt32 channel: 	  channel to query.
 *  ViChar  response[]:	  response array, output string.
 *  ViInt32 responseSize: size of response array.
 *
 * Returns:
 *	VI_SUCCESS if no errors occurred, error code otherwise.
 ***************************************************************************/

ViStatus _VI_FUNC tkvx4780_singleChannelState (ViSession instrumentHandle,
											   ViInt32	 channel,
											   ViChar	 response[],
											   ViInt32	 responseSize);

/***************************************************************************
 * Function: tkvx4780_allChannelState
 *
 * Purpose:
 *  This function performs an analysis of each channel of the signal
 *  conditioner.  This function returns a string containing the current
 *  configuration for each channel.  The output string can be parsed
 *  to determine the settings.
 *
 *  The output string is made of a semicolon (;) delimited list of lists.
 *  Each of the sublists is a comma (,) delimited list of various queries
 *  for a specific channel.  The output string uses the following pattern:
 *  "<channel 1 state>;<channel 2 state>; ... ;<channel 16 state>".  An
 *  example of each individual channel is "1,1,AC,1,10,5,5670,0.20000,468".
 *  The order of the query responses, grouped for each channel, follows.
 *
 *		input state     	The state of the input isolation relay for
 *							the selected channel.  A 1 indicates the relay
 *							is closed and that the signal conditioning
 *							channel input is connected to the front panel
 *							input connector.  A 0 indicates the relay is
 *							open and the input is disconnected from the front
 *							panel input connector.
 *		output state		The state of the output isolation relay for the
 *							selected channel.  A 1 indicates the relay is
 *							closed and the output is connected to the front
 *							panel output connector.  A 0 indicates the relay
 *							is open and the output is disconnected from the
 *							front panel output connector.
 *		coupling			The state of the input coupling relay for the
 *							specified channel:  "DC", "AC", or "GRO" (ground).
 *		attenuation state   The state of the attenuator relay that indicates
 *							if the input attenuator is in the signal path or
 *							if it is bypassed.  A 1 indicates the attenuator
 *							is in the signal path; thus, the signal is modified
 *							by the attenuator setting.  A 0 value indicates
 *							that the attenuation circuits are taken out of the
 *							signal path; thus, the attenuator is bypassed and
 *							there is no distortion to the signal.
 *      attenuation value	The setting for the signal attenuation for the
 *							selected channel.  The input attenuator is set to
 *							divide the input signal by 10 or 100.
 *		gain				The gain setting, in volt/volt, for the specified
 * 							channel.  The gain setting is set to one of the
 *							following values:  1, 2, 5, 10, 20, 50, or 100.
 *		gain trim			The gain trim setting, in parts per million (PPM),
 *							for the selected channel.  This setting is used
 *							to correct the gain of a sensor.  The range of
 *							correction factors is +/- 10000 ppm = +/- 1%.
 *		offset trim			The output DC offset trim setting, in volts, for
 *							the specified channel.  This setting is used to
 *							null DC offset errors at the channel output.  The
 *							range of correction factors is +/- 0.2 V.
 *		filter frequency	The frequency setting of the lowpass filter cutoff,
 *							in Hz, for the specified channel.  The frequency
 *							range is 468 Hz to 7.02 kHz, in 468 Hz steps, and
 *							from 7.13 kHz to 107 kHz in 7.13 kHz steps.
 *
 * Parameters:
 *	ViSession instrumentHandle: VISA instrument handle used to access
 *      instrument specific data. Must be initialized by
 *      tkvx4780_init() prior to use.
 *  ViChar  response[]:	  response array, output string.
 *  ViInt32 responseSize: size of response array.
 *
 * Returns:
 *	VI_SUCCESS if no errors occurred, error code otherwise.
 ***************************************************************************/

ViStatus _VI_FUNC tkvx4780_allChannelState (ViSession instrumentHandle,
											ViChar	 response[],
											ViInt32	 responseSize);

/***************************************************************************
 * Function: tkvx4780_queryAllChannels
 *
 * Purpose:
 *  This function performs a single query on all channels.  The same query
 *  is sent to each channel.  This function returns a string containing the
 *  query response for each channel.  The output string can be parsed
 *  to determine the settings.
 *
 *  The output string is made of a semicolon (;) delimited list.  The output
 *  string contains the responses in the order of channel number (i.e.,
 *  from 1 to 16).  For example, the output string for the input state
 *  query could would have the form "1;0;1;1;1;1;1;0;1;1;0;1;1;1;1;0"
 *
 * Parameters:
 *	ViSession instrumentHandle: VISA instrument handle used to access
 *      instrument specific data. Must be initialized by
 *      tkvx4780_init() prior to use.
 *  ViInt32 queryNumber:  type of query to perform.
 *  ViChar  response[]:	  response array, output string.
 *  ViInt32 responseSize: size of response array.
 *
 * Returns:
 *	VI_SUCCESS if no errors occurred, error code otherwise.
 ***************************************************************************/

ViStatus _VI_FUNC tkvx4780_queryAllChannels (ViSession	instrumentHandle,
											 ViInt32	queryNumber,
											 ViChar		response[],
											 ViInt32	responseSize);

/***************************************************************************
 * Function: tkvx4780_querySingleChannel
 *
 * Purpose:
 *  This function performs a single query on a single channel.  This
 *  function returns a string containing the query response.
 *
 * Parameters:
 *	ViSession instrumentHandle: VISA instrument handle used to access
 *      instrument specific data. Must be initialized by
 *      tkvx4780_init() prior to use.
 *	ViInt32	channelNumber:	channel to query.
 *  ViInt32 queryNumber:	type of query to perform.
 *  ViChar  response[]:		response array, output string.
 *  ViInt32 responseSize:	size of response array.
 *
 * Returns:
 *	VI_SUCCESS if no errors occurred, error code otherwise.
 ***************************************************************************/

ViStatus _VI_FUNC tkvx4780_querySingleChannel (ViSession	instrumentHandle,
											   ViInt32		channelNumber,
											   ViInt32		queryNumber,
											   ViChar		response[],
											   ViInt32		responseSize);

/***************************************************************************
 * Function: tkvx4780_diagOffsetTrim_grp1
 *
 * Purpose:
 *  This function loads data into the offset trim DACs for the selected
 *  channel(s).
 *
 *  NOTE:  Diagnostic commands are intended for calibration and for
 *         diagnosing problems on the module and should not be used
 *         for any other purpose.  Because they were included in a 
 *         previous driver release, it is included as an EXCEPTION function
 *         and is retained for backward compatibility.
 *
 * Parameters:
 *	ViSession instrumentHandle: VISA instrument handle used to access
 *      instrument specific data. Must be initialized by
 *      tkvx4780_init() prior to use.
 *  ViStatus retError: error code to check.
 *  ViInt32 ch1,ch2,ch3,ch4,allChannelsInGroup: channels to enable.
 *  ViInt32 msoffset: ms offset DAC data.
 *  ViInt32 lsoffset: ls offset DAC data.
 *
 * Returns:
 *	VI_SUCCESS if no errors occurred, error code otherwise
 ***************************************************************************/

ViStatus _VI_FUNC tkvx4780_diagOffsetTrim_grp1 (ViSession instrumentHandle,
												ViInt32 ch1, ViInt32 ch2,
												ViInt32 ch3, ViInt32 ch4,
												ViInt32 allChannelsInGroup,
												ViInt32 msoffset,
											 ViInt32 lsoffset);

/***************************************************************************
 * Function: tkvx4780_diagOffsetTrim_grp2
 *
 * Purpose:
 *  This function loads data into the offset trim DACs for the selected
 *  channel(s)
 *
 *  NOTE:  Diagnostic commands are intended for calibration and for
 *         diagnosing problems on the module and should not be used
 *         for any other purpose.  Because they were included in a 
 *         previous driver release, it is included as an EXCEPTION function
 *         and is retained for backward compatibility.
 *
 * Parameters:
 *	ViSession instrumentHandle: VISA instrument handle used to access
 *      instrument specific data. Must be initialized by
 *      tkvx4780_init() prior to use.
 *  ViStatus retError: error code to check.
 *  ViInt32 ch5,ch6,ch7,ch8,allChannelsInGroup: channels to enable.
 *  ViInt32 msoffset: ms offset DAC data.
 *  ViInt32 lsoffset: ls offset DAC data.
 *
 * Returns:
 *	VI_SUCCESS if no errors occurred, error code otherwise
 ***************************************************************************/

ViStatus _VI_FUNC tkvx4780_diagOffsetTrim_grp2 (ViSession instrumentHandle,
												ViInt32 ch5, ViInt32 ch6,
												ViInt32 ch7, ViInt32 ch8,
												ViInt32 allChannelsInGroup,
												ViInt32 msoffset,
												ViInt32 lsoffset);

/***************************************************************************
 * Function: tkvx4780_diagOffsetTrim_grp3
 *
 * Purpose:
 *  This function loads data into the offset trim DACs for the selected
 *  channel(s)
 *
 *  NOTE:  Diagnostic commands are intended for calibration and for
 *         diagnosing problems on the module and should not be used
 *         for any other purpose.  Because they were included in a 
 *         previous driver release, it is included as an EXCEPTION function
 *         and is retained for backward compatibility.
 *
 * Parameters:
 *	ViSession instrumentHandle: VISA instrument handle used to access
 *      instrument specific data. Must be initialized by
 *      tkvx4780_init() prior to use.
 *  ViStatus retError: error code to check.
 *  ViInt32 ch9,ch10,ch11,ch12,allChannelsInGroup: channels to enable.
 *  ViInt32 msoffset: ms offset DAC data.
 *  ViInt32 lsoffset: ls offset DAC data.
 *
 * Returns:
 *	VI_SUCCESS if no errors occurred, error code otherwise
 ***************************************************************************/

ViStatus _VI_FUNC tkvx4780_diagOffsetTrim_grp3 (ViSession instrumentHandle,
												ViInt32 ch9, ViInt32 ch10,
												ViInt32 ch11, ViInt32 ch12,
												ViInt32 allChannelsInGroup,
												ViInt32 msoffset,
												ViInt32 lsoffset);

/***************************************************************************
 * Function: tkvx4780_diagOffsetTrim_grp4
 *
 * Purpose:
 *  This function loads data into the offset trim DACs for the selected
 *  channel(s)
 *
 *  NOTE:  Diagnostic commands are intended for calibration and for
 *         diagnosing problems on the module and should not be used
 *         for any other purpose.  Because they were included in a 
 *         previous driver release, it is included as an EXCEPTION function
 *         and is retained for backward compatibility.
 *
 * Parameters:
 *	ViSession instrumentHandle: VISA instrument handle used to access
 *      instrument specific data. Must be initialized by
 *      tkvx4780_init() prior to use.
 *  ViStatus retError: error code to check.
 *  ViInt32 ch13,ch14,ch15,ch16,allChannelsInGroup: channels to enable.
 *  ViInt32 msoffset: ms offset DAC data.
 *  ViInt32 lsoffset: ls offset DAC data.
 *
 * Returns:
 *	VI_SUCCESS if no errors occurred, error code otherwise
 ***************************************************************************/

ViStatus _VI_FUNC tkvx4780_diagOffsetTrim_grp4 (ViSession instrumentHandle,
												ViInt32 ch13, ViInt32 ch14,
												ViInt32 ch15, ViInt32 ch16,
												ViInt32 allChannelsInGroup,
												ViInt32 msoffset,
												ViInt32 lsoffset);

/***************************************************************************
 * Function: tkvx4780_diagGainTrim_grp1
 *
 * Purpose:
 *  This function loads data into the gain
 *  trim DACs for the selected channel(s)
 *
 *  NOTE:  Diagnostic commands are intended for calibration and for
 *         diagnosing problems on the module and should not be used
 *         for any other purpose.  Because they were included in a 
 *         previous driver release, it is included as an EXCEPTION function
 *         and is retained for backward compatibility.
 *
 * Parameters:
 *	ViSession instrumentHandle: VISA instrument handle used to access
 *      instrument specific data. Must be initialized by
 *      tkvx4780_init() prior to use.
 *  ViStatus retError: error code to check.
 *  ViInt32 ch1,ch2,ch3,ch4,allChannelsInGroup: channels to enable.
 *  ViInt32 gaindac: gain data.
 *
 * Returns:
 *	VI_SUCCESS if no errors occurred, error code otherwise
 ***************************************************************************/

ViStatus _VI_FUNC tkvx4780_diagGainTrim_grp1 (ViSession instrumentHandle,
										 	  ViInt32 ch1, ViInt32 ch2,
											  ViInt32 ch3, ViInt32 ch4,
										 	  ViInt32 allChannelsInGroup, ViInt32 gaindac);

/***************************************************************************
 * Function: tkvx4780_diagGainTrim_grp2
 *
 * Purpose:
 *  This function loads data into the gain
 *  trim DACs for the selected channel(s)
 *
 *  NOTE:  Diagnostic commands are intended for calibration and for
 *         diagnosing problems on the module and should not be used
 *         for any other purpose.  Because they were included in a 
 *         previous driver release, it is included as an EXCEPTION function
 *         and is retained for backward compatibility.
 *
 * Parameters:
 *	ViSession instrumentHandle: VISA instrument handle used to access
 *      instrument specific data. Must be initialized by
 *      tkvx4780_init() prior to use.
 *  ViStatus retError: error code to check.
 *  ViInt32 ch5,ch6,ch7,ch8,allChannelsInGroup: channels to enable.
 *  ViInt32 gaindac: gain data.
 *
 * Returns:
 *	VI_SUCCESS if no errors occurred, error code otherwise
 ***************************************************************************/

ViStatus _VI_FUNC tkvx4780_diagGainTrim_grp2 (ViSession instrumentHandle,
										 	  ViInt32 ch5, ViInt32 ch6,
										 	  ViInt32 ch7, ViInt32 ch8,
										 	  ViInt32 allChannelsInGroup, ViInt32 gaindac);

/***************************************************************************
 * Function: tkvx4780_diagGainTrim_grp3
 *
 * Purpose:
 *  This function loads data into the gain
 *  trim DACs for the selected channel(s)
 *
 *  NOTE:  Diagnostic commands are intended for calibration and for
 *         diagnosing problems on the module and should not be used
 *         for any other purpose.  Because they were included in a 
 *         previous driver release, it is included as an EXCEPTION function
 *         and is retained for backward compatibility.
 *
 * Parameters:
 *	ViSession instrumentHandle: VISA instrument handle used to access
 *      instrument specific data. Must be initialized by
 *      tkvx4780_init() prior to use.
 *  ViStatus retError: error code to check.
 *  ViInt32 ch9,ch10,ch11,ch12,allChannelsInGroup: channels to enable.
 *  ViInt32 gaindac: gain data.
 *
 * Returns:
 *	VI_SUCCESS if no errors occurred, error code otherwise
 ***************************************************************************/

ViStatus _VI_FUNC tkvx4780_diagGainTrim_grp3 (ViSession instrumentHandle,
										 	  ViInt32 ch9, ViInt32 ch10,
										 	  ViInt32 ch11, ViInt32 ch12,
										 	  ViInt32 allChannelsInGroup, ViInt32 gaindac);

/***************************************************************************
 * Function: tkvx4780_diagGainTrim_grp4
 *
 * Purpose:
 *  This function loads data into the gain
 *  trim DACs for the selected channel(s)
 *
 *  NOTE:  Diagnostic commands are intended for calibration and for
 *         diagnosing problems on the module and should not be used
 *         for any other purpose.  Because they were included in a 
 *         previous driver release, it is included as an EXCEPTION function
 *         and is retained for backward compatibility.
 *
 * Parameters:
 *	ViSession instrumentHandle: VISA instrument handle used to access
 *      instrument specific data. Must be initialized by
 *      tkvx4780_init() prior to use.
 *  ViStatus retError: error code to check.
 *  ViInt32 ch13,ch14,ch15,ch16,allChannelsInGroup: channels to enable.
 *  ViInt32 gaindac: gain data.
 *
 * Returns:
 *	VI_SUCCESS if no errors occurred, error code otherwise
 ***************************************************************************/

ViStatus _VI_FUNC tkvx4780_diagGainTrim_grp4 (ViSession instrumentHandle,
										 	  ViInt32 ch13, ViInt32 ch14,
										 	  ViInt32 ch15, ViInt32 ch16,
										 	  ViInt32 allChannelsInGroup, ViInt32 gaindac);

/***************************************************************************
 * Function: tkvx4780_diagIAgain_grp1
 *
 * Purpose:
 *  This function sets the gain of the instrumentation amplifier
 *  for the selected channel(s)
 *
 *  NOTE:  Diagnostic commands are intended for calibration and for
 *         diagnosing problems on the module and should not be used
 *         for any other purpose.  Because they were included in a 
 *         previous driver release, it is included as an EXCEPTION function
 *         and is retained for backward compatibility.
 *
 * Parameters:
 *	ViSession instrumentHandle: VISA instrument handle used to access
 *      instrument specific data. Must be initialized by
 *      tkvx4780_init() prior to use.
 *  ViStatus retError: error code to check.
 *  ViInt32 ch1,ch2,ch3,ch4,allChannelsInGroup: channels to enable.
 *  ViInt32 iagain: gain data.
 *
 * Returns:
 *	VI_SUCCESS if no errors occurred, error code otherwise
 ***************************************************************************/

ViStatus _VI_FUNC tkvx4780_diagIAgain_grp1 (ViSession instrumentHandle,
											ViInt32 ch1, ViInt32 ch2,
											ViInt32 ch3, ViInt32 ch4,
											ViInt32 allChannelsInGroup, ViInt32 iagain);

/***************************************************************************
 * Function: tkvx4780_diagIAgain_grp2
 *
 * Purpose:
 *  This function sets the gain of the instrumentation amplifier
 *  for the selected channel(s)
 *
 *  NOTE:  Diagnostic commands are intended for calibration and for
 *         diagnosing problems on the module and should not be used
 *         for any other purpose.  Because they were included in a 
 *         previous driver release, it is included as an EXCEPTION function
 *         and is retained for backward compatibility.
 *
 * Parameters:
 *	ViSession instrumentHandle: VISA instrument handle used to access
 *      instrument specific data. Must be initialized by
 *      tkvx4780_init() prior to use.
 *  ViStatus retError: error code to check.
 *  ViInt32 ch5,ch6,ch7,ch8,allChannelsInGroup: channels to enable.
 *  ViInt32 iagain: gain data.
 *
 * Returns:
 *	VI_SUCCESS if no errors occurred, error code otherwise
 ***************************************************************************/

ViStatus _VI_FUNC tkvx4780_diagIAgain_grp2 (ViSession instrumentHandle,
											ViInt32 ch5, ViInt32 ch6,
											ViInt32 ch7, ViInt32 ch8,
											ViInt32 allChannelsInGroup, ViInt32 iagain);

/***************************************************************************
 * Function: tkvx4780_diagIAgain_grp3
 *
 * Purpose:
 *  This function sets the gain of the instrumentation amplifier
 *  for the selected channel(s)
 *
 *  NOTE:  Diagnostic commands are intended for calibration and for
 *         diagnosing problems on the module and should not be used
 *         for any other purpose.  Because they were included in a 
 *         previous driver release, it is included as an EXCEPTION function
 *         and is retained for backward compatibility.
 *
 * Parameters:
 *	ViSession instrumentHandle: VISA instrument handle used to access
 *      instrument specific data. Must be initialized by
 *      tkvx4780_init() prior to use.
 *  ViStatus retError: error code to check.
 *  ViInt32 ch9,ch10,ch11,ch12,allChannelsInGroup: channels to enable.
 *  ViInt32 iagain: gain data.
 *
 * Returns:
 *	VI_SUCCESS if no errors occurred, error code otherwise
 ***************************************************************************/

ViStatus _VI_FUNC tkvx4780_diagIAgain_grp3 (ViSession instrumentHandle,
											ViInt32 ch9, ViInt32 ch10,
											ViInt32 ch11, ViInt32 ch12,
											ViInt32 allChannelsInGroup, ViInt32 iagain);

/***************************************************************************
 * Function: tkvx4780_diagIAgain_grp4
 *
 * Purpose:
 *  This function sets the gain of the instrumentation amplifier
 *  for the selected channel(s)
 *
 *  NOTE:  Diagnostic commands are intended for calibration and for
 *         diagnosing problems on the module and should not be used
 *         for any other purpose.  Because they were included in a 
 *         previous driver release, it is included as an EXCEPTION function
 *         and is retained for backward compatibility.
 *
 * Parameters:
 *	ViSession instrumentHandle: VISA instrument handle used to access
 *      instrument specific data. Must be initialized by
 *      tkvx4780_init() prior to use.
 *  ViStatus retError: error code to check.
 *  ViInt32 ch13,ch14,ch15,ch16,allChannelsInGroup: channels to enable.
 *  ViInt32 iagain: gain data.
 *
 * Returns:
 *	VI_SUCCESS if no errors occurred, error code otherwise
 ***************************************************************************/

ViStatus _VI_FUNC tkvx4780_diagIAgain_grp4 (ViSession instrumentHandle,
											ViInt32 ch13, ViInt32 ch14,
											ViInt32 ch15, ViInt32 ch16,
											ViInt32 allChannelsInGroup, ViInt32 iagain);

/***************************************************************************
 * Function: tkvx4780_diagSTgain_grp1
 *
 * Purpose:
 *  This function sets the gain of the amplifier that follows
 *  the instrumentation amplifier for the selected channel(s)
 *
 *  NOTE:  Diagnostic commands are intended for calibration and for
 *         diagnosing problems on the module and should not be used
 *         for any other purpose.  Because they were included in a 
 *         previous driver release, it is included as an EXCEPTION function
 *         and is retained for backward compatibility.
 *
 * Parameters:
 *	ViSession instrumentHandle: VISA instrument handle used to access
 *      instrument specific data. Must be initialized by
 *      tkvx4780_init() prior to use.
 *  ViStatus retError: error code to check.
 *  ViInt32 ch1,ch2,ch3,ch4,allChannelsInGroup: channels to enable.
 *  ViInt32 stgain: gain data.
 *
 * Returns:
 *	VI_SUCCESS if no errors occurred, error code otherwise
 ***************************************************************************/

ViStatus _VI_FUNC tkvx4780_diagSTgain_grp1 (ViSession instrumentHandle,
											ViInt32 ch1, ViInt32 ch2,
											ViInt32 ch3, ViInt32 ch4,
											ViInt32 allChannelsInGroup, ViInt32 stgain);

/***************************************************************************
 * Function: tkvx4780_diagSTgain_grp2
 *
 * Purpose:
 *  This function sets the gain of the amplifier that follows
 *  the instrumentation amplifier for the selected channel(s)
 *
 *  NOTE:  Diagnostic commands are intended for calibration and for
 *         diagnosing problems on the module and should not be used
 *         for any other purpose.  Because they were included in a 
 *         previous driver release, it is included as an EXCEPTION function
 *         and is retained for backward compatibility.
 *
 * Parameters:
 *	ViSession instrumentHandle: VISA instrument handle used to access
 *      instrument specific data. Must be initialized by
 *      tkvx4780_init() prior to use.
 *  ViStatus retError: error code to check.
 *  ViInt32 ch5,ch6,ch7,ch8,allChannelsInGroup: channels to enable.
 *  ViInt32 stgain: gain data.
 *
 * Returns:
 *	VI_SUCCESS if no errors occurred, error code otherwise
 ***************************************************************************/

ViStatus _VI_FUNC tkvx4780_diagSTgain_grp2 (ViSession instrumentHandle,
											ViInt32 ch5, ViInt32 ch6,
											ViInt32 ch7, ViInt32 ch8,
											ViInt32 allChannelsInGroup, ViInt32 stgain);

/***************************************************************************
 * Function: tkvx4780_diagSTgain_grp3
 *
 * Purpose:
 *  This function sets the gain of the amplifier that follows
 *  the instrumentation amplifier for the selected channel(s)
 *
 *  NOTE:  Diagnostic commands are intended for calibration and for
 *         diagnosing problems on the module and should not be used
 *         for any other purpose.  Because they were included in a 
 *         previous driver release, it is included as an EXCEPTION function
 *         and is retained for backward compatibility.
 *
 * Parameters:
 *	ViSession instrumentHandle: VISA instrument handle used to access
 *      instrument specific data. Must be initialized by
 *      tkvx4780_init() prior to use.
 *  ViStatus retError: error code to check.
 *  ViInt32 ch9,ch10,ch11,ch12,allChannelsInGroup: channels to enable.
 *  ViInt32 stgain: gain data.
 *
 * Returns:
 *	VI_SUCCESS if no errors occurred, error code otherwise
 ***************************************************************************/

ViStatus _VI_FUNC tkvx4780_diagSTgain_grp3 (ViSession instrumentHandle,
											ViInt32 ch9, ViInt32 ch10,
											ViInt32 ch11, ViInt32 ch12,
											ViInt32 allChannelsInGroup, ViInt32 stgain);

/***************************************************************************
 * Function: tkvx4780_diagSTgain_grp4
 *
 * Purpose:
 *  This function sets the gain of the amplifier that follows
 *  the instrumentation amplifier for the selected channel(s)
 *
 *  NOTE:  Diagnostic commands are intended for calibration and for
 *         diagnosing problems on the module and should not be used
 *         for any other purpose.  Because they were included in a 
 *         previous driver release, it is included as an EXCEPTION function
 *         and is retained for backward compatibility.
 *
 * Parameters:
 *	ViSession instrumentHandle: VISA instrument handle used to access
 *      instrument specific data. Must be initialized by
 *      tkvx4780_init() prior to use.
 *  ViStatus retError: error code to check.
 *  ViInt32 ch13,ch14,ch15,ch16,allChannelsInGroup: channels to enable.
 *  ViInt32 stgain: gain data.
 *
 * Returns:
 *	VI_SUCCESS if no errors occurred, error code otherwise
 ***************************************************************************/

ViStatus _VI_FUNC tkvx4780_diagSTgain_grp4 (ViSession instrumentHandle,
											ViInt32 ch13, ViInt32 ch14,
											ViInt32 ch15, ViInt32 ch16,
											ViInt32 allChannelsInGroup, ViInt32 stgain);

/***************************************************************************
 * Function: tkvx4780_diagADCRead
 *
 * Purpose:
 *  This function takes a reading from the a/d converter
 *
 *  NOTE:  Diagnostic commands are intended for calibration and for
 *         diagnosing problems on the module and should not be used
 *         for any other purpose.  Because they were included in a 
 *         previous driver release, it is included as an EXCEPTION function
 *         and is retained for backward compatibility.
 *
 * Parameters:
 *	ViSession instrumentHandle: VISA instrument handle used to access
 *      instrument specific data. Must be initialized by
 *      tkvx4780_init() prior to use.
 *  ViStatus retError: error code to check.
 *  ViChar ch_list[]: channels to read.
 *  ViChar message[]: readback data.
 *
 * Returns:
 *	VI_SUCCESS if no errors occurred, error code otherwise
 ***************************************************************************/

ViStatus _VI_FUNC tkvx4780_diagADCRead (ViSession instrumentHandle,
										ViChar ch_list[],
										ViChar message[]);

/***************************************************************************
 * Function: tkvx4780_diagDCVolt
 *
 * Purpose:
 *  This function applies a dc voltage to the inputs of
 *  all channels that have their isolation relay open.
 *
 *  NOTE:  Diagnostic commands are intended for calibration and for
 *         diagnosing problems on the module and should not be used
 *         for any other purpose.
 *
 *  EXCEPTION:  The dc command is an exception for diagnostic
 *              commands.  It can be used to generate a known 
 *              test source.
 *
 * Parameters:
 *	ViSession instrumentHandle: VISA instrument handle used to access
 *      instrument specific data. Must be initialized by
 *      tkvx4780_init() prior to use.
 *	ViStatus retError: error code to check.
 *  ViReal64 DCInput: voltage setting.
 *
 * Returns:
 *	VI_SUCCESS if no errors occurred, error code otherwise
 ***************************************************************************/

ViStatus _VI_FUNC tkvx4780_diagDCVolt (ViSession instrumentHandle,
									   ViReal64 DCInput);

/***************************************************************************
 * Function: tkvx4780_diagInputSqrwave
 *
 * Purpose:
 *  This function applies a 5V p-p squarewave to the inputs of
 *  all channels that have their isolation relay open.
 *
 *  NOTE:  Diagnostic commands are intended for calibration and for
 *         diagnosing problems on the module and should not be used
 *         for any other purpose.
 *
 *  EXCEPTION:  The squarewave command is an exception for diagnostic
 *              commands.  It can be used to generate a test source
 *              waveform.
 *
 * Parameters:
 *	ViSession instrumentHandle: VISA instrument handle used to access
 *      instrument specific data. Must be initialized by
 *      tkvx4780_init() prior to use.
 *	ViStatus retError: error code to check.
 *  ViInt32 loopcount: frequency.
 *
 * Returns:
 *	VI_SUCCESS if no errors occurred, error code otherwise
 ***************************************************************************/

ViStatus _VI_FUNC tkvx4780_diagInputSqrwave (ViSession instrumentHandle,
											 ViInt32 loopcount);

/***************************************************************************
 * Function: tkvx4780_diagCAL
 *
 * Purpose:
 *  This function writes data to a specified NOVRAM location
 *
 *  NOTE:  Diagnostic commands are intended for calibration and for
 *         diagnosing problems on the module and should not be used
 *         for any other purpose.  Because they were included in a 
 *         previous driver release, it is included as an EXCEPTION function
 *         and is retained for backward compatibility.
 *
 * Parameters:
 *	ViSession instrumentHandle: VISA instrument handle used to access
 *      instrument specific data. Must be initialized by
 *      tkvx4780_init() prior to use.
 *	ViStatus retError: error code to check.
 *  ViInt32 caladr: value of NOVRAM address.
 *  ViInt32 caldata[]: data to write to NOVRAM address.
 *
 * Returns:
 *	VI_SUCCESS if no errors occurred, error code otherwise
 ***************************************************************************/

ViStatus _VI_FUNC tkvx4780_diagCAL (ViSession instrumentHandle,
									ViInt32 caladr,
									ViInt32 caldata);

/***************************************************************************
 * Function: tkvx4780_diagCALQuery
 *
 * Purpose:
 *  This function reads data at a specified NOVRAM location
 *
 *  NOTE:  Diagnostic commands are intended for calibration and for
 *         diagnosing problems on the module and should not be used
 *         for any other purpose.  Because they were included in a 
 *         previous driver release, it is included as an EXCEPTION function
 *         and is retained for backward compatibility.
 *
 * Parameters:
 *	ViSession instrumentHandle: VISA instrument handle used to access
 *      instrument specific data. Must be initialized by
 *      tkvx4780_init() prior to use.
 *  ViInt32 caladr: value of NOVRAM address.
 *  ViInt32 caldata[]: data to write to NOVRAM address.
 *
 * Returns:
 *	User defined error code if value out of range, else VI_SUCCESS.
 ***************************************************************************/

ViStatus _VI_FUNC tkvx4780_diagCALQuery (ViSession instrumentHandle,
										 ViInt32 caladr,
										 ViChar caldata[]);

/***************************************************************************
 * Function: tkvx4780_VXI_CLEAR
 *
 * Purpose:
 *  This function sends a VXI CLEAR command to the module.
 *
 *  NOTE:  Diagnostic commands are intended for calibration and for
 *         diagnosing problems on the module and should not be used
 *         for any other purpose.
 *
 * Parameters:
 *	ViStatus retError: error code to return if out of range.
 *
 * Returns:
 *	User defined error code if value out of range, else VI_SUCCESS.
 ***************************************************************************/

 ViStatus _VI_FUNC tkvx4780_VXI_CLEAR (ViSession instrumentHandle);

/***************************************************************************
 * Function: tkvx4780_quickSetup
 *
 * Purpose:
 *  This function places the instrument into a default configuration to  
 *  provide a quick setup routine.
 *
 *  The default configuration follows:
 *
 *  Ch  InState Coup Atten  Gain   Filter  GainTrim OffsetTrim OutState
 *
 *   1     on    DC   x10   1V/v   100kHz      0         0        on
 *   2     on    DC   x10   1V/v   100kHz      0         0        on
 *   3     on    DC   x10   1V/v   100kHz      0         0        on
 *   4     on    DC   x10   1V/v   100kHz      0         0        on
 *	        
 *   5     on    DC   byp  20V/v   100kHz      0         0        on
 *   6     on    DC   byp  20V/v   100kHz      0         0        on
 *   7     on    DC   byp  20V/v   100kHz      0         0        on
 *   8     on    DC   byp  20V/v   100kHz      0         0        on
 * 	   		 
 *   9     on    AC   byp  50V/v     5kHz      0         0        on
 *  10     on    AC   byp  50V/v     5kHz      0         0        on
 *  11     on    AC   byp  50V/v     5kHz      0         0        on
 *  12     on    AC   byp  50V/v     5kHz      0         0        on
 *			 
 *  13     on    DC   byp   1V/v   100kHz     0.5%    -0.002      on
 *  14     on    DC   byp   1V/v   100kHz     0.5%    -0.002      on
 *  15     on    DC   byp   1V/v   100kHz    -0.5%    -0.002      on
 *  16     on    DC   byp   1V/v   100kHz    -0.5%    -0.002      on
 *
 *
 * Parameters:
 *	ViSession instrumentHandle: VISA instrument handle used to access
 *      instrument specific data. Must be initialized by
 *      tkvx4780_init() prior to use.
 *
 * Returns:
 *	User defined error code if value out of range, else VI_SUCCESS.
 ***************************************************************************/

ViStatus _VI_FUNC tkvx4780_quickSetup (ViSession instrumentHandle);

#if defined(__cplusplus) || defined(__cplusplus__)
  }
#endif

#endif

