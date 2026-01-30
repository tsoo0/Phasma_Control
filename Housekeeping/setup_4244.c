#include <ansi_c.h>
#include "Tkvx4244.h"
 

void setup_4244(int digitizer_hndl,int group1_rlength, double group1_drate,
				int group2_rlength, double group2_drate,
				int group3_rlength, double group3_drate,
				int group4_rlength, double group4_drate,
				ViInt16 group1_vltrange,ViInt16  group2_vltrange,
				ViInt16 group3_vltrange,ViInt16  group4_vltrange,
				ViInt16 channel_one_status,
				ViInt16 channel_two_status, ViInt16 channel_three_status,
				ViInt16 channel_four_status, ViInt16 channel_five_status,
				ViInt16 channel_six_status, ViInt16 channel_seven_status,
				ViInt16 channel_eight_status, ViInt16 channel_nine_status,
				ViInt16 channel_ten_status, ViInt16 channel_eleven_status,
				ViInt16 channel_twelve_status, ViInt16 channel_thirteen_status,
				ViInt16 channel_fourteen_status, ViInt16 channel_fifteen_status,
				ViInt16 channel_sixteen_status)
{
    ViChar 		Slope[16];
    static		ViStatus   	result;                 			/* function return status */
	static 		ViChar display_buff[36];
	void 		displayErr(ViSession InstrHndl,ViStatus err); 	/* For Displaying Error Messages */ 
	ViChar 		dummy1[128];
	ViChar 		dummy2[128];
	ViInt16		group_one_status=0;
	ViInt16		group_two_status=0;
	ViInt16		group_three_status=0;
	ViInt16		group_four_status=0;
	extern int	Mwave_setup_panel;		//needed to set up group4 trigger level
	
//	Up the Reference Oscillator Frequency to its maximum so that the digitizer
//	can run as fast as 200 kHz
	if (result = tkvx4244_confMasterClock (digitizer_hndl, 2.0E6,
										   tkvx4244_CLOCK_INTERNAL, dummy1,
										   dummy2)){
		displayErr(digitizer_hndl,result);
	}
   
	//Set the inpuot trigger slope to postive
	if (result = tkvx4244_confTriggerSlope (digitizer_hndl, tkvx4244_SLOPE_POS, Slope)){
        displayErr(digitizer_hndl,result);
    }
	 
    switch (group1_vltrange) {
    	  case 0: group1_vltrange = tkvx4244_VOLT_RANGE_500MV;
    	  		  break;
    	  case 1: group1_vltrange = tkvx4244_VOLT_RANGE_1VOLT;
    	  		  break;
    	  case 2: group1_vltrange = tkvx4244_VOLT_RANGE_2VOLT;
    	  		  break;
    	  case 5: group1_vltrange = tkvx4244_VOLT_RANGE_5VOLT;
    	  		  break;
    	  case 10: group1_vltrange = tkvx4244_VOLT_RANGE_10VOLT;
    	  		  break;
    	  case 20: group1_vltrange = tkvx4244_VOLT_RANGE_20VOLT;
    	  		  break;
    	  default: printf("Invalid voltage range selected for group1! \n");
    }	  
    switch (group2_vltrange) {
    	  case 0: group2_vltrange = tkvx4244_VOLT_RANGE_500MV;
    	  		  break;
    	  case 1: group2_vltrange = tkvx4244_VOLT_RANGE_1VOLT;
    	  		  break;
    	  case 2: group2_vltrange = tkvx4244_VOLT_RANGE_2VOLT;
    	  		  break;
    	  case 5: group2_vltrange = tkvx4244_VOLT_RANGE_5VOLT;
    	  		  break;
    	  case 10: group2_vltrange = tkvx4244_VOLT_RANGE_10VOLT;
    	  		  break;
    	  case 20: group2_vltrange = tkvx4244_VOLT_RANGE_20VOLT;
    	  		  break;
    	  default: printf("Invalid voltage range selected for group2! \n");
    }	  
    switch (group3_vltrange) {
    	  case 0: group3_vltrange = tkvx4244_VOLT_RANGE_500MV;
    	  		  break;
    	  case 1: group3_vltrange = tkvx4244_VOLT_RANGE_1VOLT;
    	  		  break;
    	  case 2: group3_vltrange = tkvx4244_VOLT_RANGE_2VOLT;
    	  		  break;
    	  case 5: group3_vltrange = tkvx4244_VOLT_RANGE_5VOLT;
    	  		  break;
    	  case 10: group3_vltrange = tkvx4244_VOLT_RANGE_10VOLT;
    	  		  break;
    	  case 20: group3_vltrange = tkvx4244_VOLT_RANGE_20VOLT;
    	  		  break;
    	  default: printf("Invalid voltage range selected for group3! \n");
    }	  
    switch (group4_vltrange) {
    	  case 0: group4_vltrange = tkvx4244_VOLT_RANGE_500MV;
    	  		  break;
    	  case 1: group4_vltrange = tkvx4244_VOLT_RANGE_1VOLT;
    	  		  break;
    	  case 2: group4_vltrange = tkvx4244_VOLT_RANGE_2VOLT;
    	  		  break;
    	  case 5: group4_vltrange = tkvx4244_VOLT_RANGE_5VOLT;
    	  		  break;
    	  case 10: group4_vltrange = tkvx4244_VOLT_RANGE_10VOLT;
    	  		  break;
    	  case 20: group4_vltrange = tkvx4244_VOLT_RANGE_20VOLT;
    	  		  break;
    	  default: printf("Invalid voltage range selected for group4! \n");
    }	  
    

//	Set the Active Channels and Group for Configuration
	if (channel_one_status == 1 || channel_two_status == 1 ||
		channel_three_status == 1 || channel_four_status == 1)
    	group_one_status=1;

	if (channel_five_status == 1 || channel_six_status == 1 ||
		channel_seven_status == 1 || channel_eight_status == 1)
		group_two_status=1;

	if (channel_nine_status == 1 || channel_ten_status == 1 ||
		channel_eleven_status == 1 || channel_twelve_status == 1)
		group_three_status=1;

	if (channel_thirteen_status == 1 || channel_fourteen_status == 1 ||
		channel_fifteen_status == 1 || channel_sixteen_status == 1)
		group_four_status = 1;

//	Configure the Requested Groups and Channels
	if (group_one_status) {
		if (result = tkvx4244_confGroupOneChannels (digitizer_hndl,
												channel_one_status,
												channel_two_status,
												channel_three_status,
												channel_four_status,
												tkvx4244_SELECT_ENABLE)){
        	displayErr(digitizer_hndl,result);
    	}
		if (result = tkvx4244_confGpOneVoltageRange (digitizer_hndl,
													 channel_one_status,
													 channel_two_status,
													 channel_three_status,
													 channel_four_status,
													 group1_vltrange)){
			displayErr(digitizer_hndl,result);
		}
		if (result = tkvx4244_confSamplingInterval (digitizer_hndl,
													tkvx4244_CLOCK_INTERNAL,
													tkvx4244_FREQUENCY,
													group1_drate,
													tkvx4244_GROUP_ONE_ENABLE,
													tkvx4244_GROUP_TWO_DISABLE,
													tkvx4244_GROUP_THREE_DISABLE,
													tkvx4244_GROUP_FOUR_DISABLE,
													tkvx4244_ALL_GROUPS_DISABLE)){
		   displayErr(digitizer_hndl,result);
		}
		if (result = tkvx4244_confGpOneTrigSrcLogic (digitizer_hndl, tkvx4244_TRIGGER_LOGIC_OR, tkvx4244_TRIGGER_EXT_ENABLE, tkvx4244_TRIGGER_IMM_DISABLE, tkvx4244_TRIGGER_THRESH_DISABLE, tkvx4244_TRIGGER_VXI_DISABLE, tkvx4244_TRIGGER_TTL0_ENABLE,
													 tkvx4244_TRIGGER_TTL1_ENABLE, tkvx4244_TRIGGER_TTL2_DISABLE, tkvx4244_TRIGGER_TTL3_DISABLE, tkvx4244_TRIGGER_TTL4_DISABLE, tkvx4244_TRIGGER_TTL5_DISABLE, tkvx4244_TRIGGER_TTL6_DISABLE,
													 tkvx4244_TRIGGER_TTL7_DISABLE)){
			displayErr(digitizer_hndl,result);
		}
		if (result = tkvx4244_confFrontPanelDisplay (digitizer_hndl, tkvx4244_GROUP_ONE,
													 display_buff)){
			displayErr(digitizer_hndl,result);
		}
	}
	
	if(group_two_status) {
		if (result = tkvx4244_confGroupTwoChannels (digitizer_hndl,
												channel_five_status,
												channel_six_status,
												channel_seven_status,
												channel_eight_status,
												tkvx4244_SELECT_ENABLE)){
        	displayErr(digitizer_hndl,result);
    	}
		if (result = tkvx4244_confGpTwoVoltageRange (digitizer_hndl,
													 channel_five_status,
													 channel_six_status,
													 channel_seven_status,
													 channel_eight_status,
													 group2_vltrange)){
			displayErr(digitizer_hndl,result);
		}
		if (result = tkvx4244_confSamplingInterval (digitizer_hndl,
													tkvx4244_CLOCK_INTERNAL,
													tkvx4244_FREQUENCY,
													group2_drate,
													tkvx4244_GROUP_ONE_DISABLE,
													tkvx4244_GROUP_TWO_ENABLE,
													tkvx4244_GROUP_THREE_DISABLE,
													tkvx4244_GROUP_FOUR_DISABLE,
													tkvx4244_ALL_GROUPS_DISABLE)){
		   displayErr(digitizer_hndl,result);
		}
		if (result = tkvx4244_confGpTwoTrigSrcLogic (digitizer_hndl, tkvx4244_TRIGGER_LOGIC_OR, tkvx4244_TRIGGER_EXT_ENABLE, tkvx4244_TRIGGER_IMM_DISABLE, tkvx4244_TRIGGER_THRESH_DISABLE, tkvx4244_TRIGGER_VXI_DISABLE, tkvx4244_TRIGGER_TTL0_ENABLE,
													 tkvx4244_TRIGGER_TTL1_ENABLE, tkvx4244_TRIGGER_TTL2_DISABLE, tkvx4244_TRIGGER_TTL3_DISABLE, tkvx4244_TRIGGER_TTL4_DISABLE, tkvx4244_TRIGGER_TTL5_DISABLE, tkvx4244_TRIGGER_TTL6_DISABLE,
													 tkvx4244_TRIGGER_TTL7_DISABLE)){
			displayErr(digitizer_hndl,result);
		}
		if (result = tkvx4244_confFrontPanelDisplay (digitizer_hndl, tkvx4244_GROUP_TWO,
													 display_buff)){
			displayErr(digitizer_hndl,result);
		}
	}
	
	if(group_three_status) {
		if (result = tkvx4244_confGroupThreeChannels (digitizer_hndl,
												channel_nine_status,
												channel_ten_status,
												channel_eleven_status,
												channel_twelve_status,
												tkvx4244_SELECT_ENABLE)){
        	displayErr(digitizer_hndl,result);
    	}
		if (result =  tkvx4244_confGpThreeVoltageRange (digitizer_hndl,
														channel_nine_status,
														channel_ten_status,
														channel_eleven_status,
														channel_twelve_status,
														group3_vltrange)){
			displayErr(digitizer_hndl,result);
		}
		if (result = tkvx4244_confSamplingInterval (digitizer_hndl,
													tkvx4244_CLOCK_INTERNAL,
													tkvx4244_FREQUENCY,
													group3_drate,
													tkvx4244_GROUP_ONE_DISABLE,
													tkvx4244_GROUP_TWO_DISABLE,
													tkvx4244_GROUP_THREE_ENABLE,
													tkvx4244_GROUP_FOUR_DISABLE,
													tkvx4244_ALL_GROUPS_DISABLE)){
		   displayErr(digitizer_hndl,result);
		}
		if (result = tkvx4244_confGpThreeTrigSrcLogic (digitizer_hndl, tkvx4244_TRIGGER_LOGIC_OR, tkvx4244_TRIGGER_EXT_ENABLE, tkvx4244_TRIGGER_IMM_DISABLE, tkvx4244_TRIGGER_THRESH_DISABLE, tkvx4244_TRIGGER_VXI_DISABLE, tkvx4244_TRIGGER_TTL0_ENABLE,
													   tkvx4244_TRIGGER_TTL1_ENABLE, tkvx4244_TRIGGER_TTL2_DISABLE, tkvx4244_TRIGGER_TTL3_DISABLE, tkvx4244_TRIGGER_TTL4_DISABLE, tkvx4244_TRIGGER_TTL5_DISABLE, tkvx4244_TRIGGER_TTL6_DISABLE, tkvx4244_TRIGGER_TTL7_DISABLE)){
			displayErr(digitizer_hndl,result);
		}
		if (result = tkvx4244_confFrontPanelDisplay (digitizer_hndl, tkvx4244_GROUP_THREE,
													 display_buff)){
			displayErr(digitizer_hndl,result);
		}
	}
	
	if (group_four_status) {
		if (result = tkvx4244_confGroupFourChannels (digitizer_hndl,
												channel_thirteen_status,
												channel_fourteen_status,
												channel_fifteen_status,
												channel_sixteen_status,
												tkvx4244_SELECT_ENABLE)){
        	displayErr(digitizer_hndl,result);
    	}
		if (result = tkvx4244_confGpFourVoltageRange (digitizer_hndl,
													   channel_thirteen_status,
													   channel_fourteen_status,
													   channel_fifteen_status,
													   channel_sixteen_status,
													   group4_vltrange)){
			displayErr(digitizer_hndl,result);
		}
		if (result = tkvx4244_confSamplingInterval (digitizer_hndl,
													tkvx4244_CLOCK_INTERNAL,
													tkvx4244_FREQUENCY,
													group4_drate,
													tkvx4244_GROUP_ONE_DISABLE,
													tkvx4244_GROUP_TWO_DISABLE,
													tkvx4244_GROUP_THREE_DISABLE,
													tkvx4244_GROUP_FOUR_ENABLE,
													tkvx4244_ALL_GROUPS_DISABLE)){
		   displayErr(digitizer_hndl,result);
		}
		
		//set up trigger level for group4
		if (result = tkvx4244_confGpFourTrigSrcLogic (digitizer_hndl, tkvx4244_TRIGGER_LOGIC_OR, tkvx4244_TRIGGER_EXT_ENABLE, tkvx4244_TRIGGER_IMM_DISABLE, tkvx4244_TRIGGER_THRESH_DISABLE, tkvx4244_TRIGGER_VXI_DISABLE, tkvx4244_TRIGGER_TTL0_ENABLE,
													  tkvx4244_TRIGGER_TTL1_ENABLE, tkvx4244_TRIGGER_TTL2_DISABLE, tkvx4244_TRIGGER_TTL3_DISABLE, tkvx4244_TRIGGER_TTL4_DISABLE, tkvx4244_TRIGGER_TTL5_DISABLE, tkvx4244_TRIGGER_TTL6_DISABLE, tkvx4244_TRIGGER_TTL7_DISABLE)){
			displayErr(digitizer_hndl,result);
		}
		if (result = tkvx4244_confFrontPanelDisplay (digitizer_hndl, tkvx4244_GROUP_FOUR,
													 display_buff)){
			displayErr(digitizer_hndl,result);
		}
	}


//	Configure the Requested Group Record Lengths
	if (group_one_status) {
		if (result = tkvx4244_confDataCollectCount (digitizer_hndl,
													group1_rlength,
													tkvx4244_GROUP_ONE_ENABLE,
													tkvx4244_GROUP_TWO_DISABLE,
													tkvx4244_GROUP_THREE_DISABLE,
													tkvx4244_GROUP_FOUR_DISABLE,
													tkvx4244_ALL_GROUPS_DISABLE)){
	 		displayErr(digitizer_hndl,result);
		} 
	}
	
	if(group_two_status) {
		if (result = tkvx4244_confDataCollectCount (digitizer_hndl,
													group2_rlength,
													tkvx4244_GROUP_ONE_DISABLE,
													tkvx4244_GROUP_TWO_ENABLE,
													tkvx4244_GROUP_THREE_DISABLE,
													tkvx4244_GROUP_FOUR_DISABLE,
													tkvx4244_ALL_GROUPS_DISABLE)){
	 		displayErr(digitizer_hndl,result);
		} 
	}
	
	if(group_three_status) {
		if (result = tkvx4244_confDataCollectCount (digitizer_hndl,
													group3_rlength,
													tkvx4244_GROUP_ONE_DISABLE,
													tkvx4244_GROUP_TWO_DISABLE,
													tkvx4244_GROUP_THREE_ENABLE,
													tkvx4244_GROUP_FOUR_DISABLE,
													tkvx4244_ALL_GROUPS_DISABLE)){
	 		displayErr(digitizer_hndl,result);
		} 
	}
	
	if (group_four_status) {
		if (result = tkvx4244_confDataCollectCount (digitizer_hndl,
													group4_rlength,
													tkvx4244_GROUP_ONE_DISABLE,
													tkvx4244_GROUP_TWO_DISABLE,
													tkvx4244_GROUP_THREE_DISABLE,
													tkvx4244_GROUP_FOUR_ENABLE,
													tkvx4244_ALL_GROUPS_DISABLE)){
	 		displayErr(digitizer_hndl,result);
		} 
	}

	if (result = tkvx4244_confArmSrcMemZero (digitizer_hndl,
												tkvx4244_IMMEDIATE_ARM,
												group_one_status,
												group_two_status,
												group_three_status,
												group_four_status,
												tkvx4244_ALL_GROUPS_DISABLE,
												tkvx4244_MEMORY_ZERO_OFF)){
        displayErr(digitizer_hndl,result);
    }
}

