#include 	<vpptype.h> 

double		group1_drate;
double		group2_drate;
double		group3_drate;
double		group4_drate;

void 		HousekeepingActivate(void); 
void		Write_HousekeepingData(void);
int			HousekeepingArm(void);
void 		OpenHousekeeping_Settings (void);
void 		setup_4244(int digitizer_hndl,int group1rlength, double group1drate,int group2rlength, double group2drate,int group3rlength, double group3drate,int group4rlength, double group4drate, ViInt16 group1vltrange, ViInt16  group2vltrange,
				ViInt16 group3vltrange,ViInt16  group4vltrange,ViInt16 channel_one_status,ViInt16 channel_two_status, ViInt16 channel_three_status, ViInt16 channel_four_status, ViInt16 channel_five_status, ViInt16 channel_six_status, ViInt16 channel_seven_status,
				ViInt16 channel_eight_status, ViInt16 channel_nine_status, ViInt16 channel_ten_status, ViInt16 channel_eleven_status, ViInt16 channel_twelve_status, ViInt16 channel_thirteen_status, ViInt16 channel_fourteen_status, ViInt16 channel_fifteen_status,
				ViInt16 channel_sixteen_status);
void 		acquire_data(ViSession handle, int channel,double voltage_scale, int record_length,
				double output_array[]);
void 		displayErr(ViSession gInstrHndl,ViStatus err);    
int			TransferHousekeepingData (double transfer_array1[], double transfer_array2[], double transfer_array3[]);
