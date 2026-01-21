						 /*********************************************************************************************************
*	Here are all the functions that connect, open, and interact with the MDSPlus database
*
*
*********************************************************************************************************/
// Helper function to get node type
// Helper function to check status

//Include the MDS libraries
#include 	<ansi_c.h>
#include 	<mdslib.h>				
					 

#include	"PHASMA-MDS-Routines.h"
#include 	"GlobalVariables.h"

extern int TreeSetCurrentShotId(char *tree, int shot);
char PHASMA_server_address[128] = "157.182.26.6:57800";		//This is the server running on the main lab computer. You must be on the same switch to access it
char tree_name[128] = "PHASMA2025";							//This is the new tree created in the MDS system on the lab computer
															//Part of the setup of the new tree is where the MDS data records get stored. This is set up by creating
															//the folders you want on the main lab computer or one of the data storage systems and then setting the 
															//the two paths in the lab computer environment variables to point to that location. See the MDS setup instructions
															//for more details.
char * MdsGetMsg(int status);     
int shot = -1; //default to current shot
   
// MDSplus connection parameters
//char signal_name[128] = "RAW_DATA.JOERGER_01:JOERGER_01_1";  // Node to read for PHASMA tree. This was for testing
//char signal_name2[128] = "RAW_DATA.TESTAREA:TESTSIGNAL";  	// Node to write to

	
//Open the MDS connection and the tree
int OpenPHASMA_MDS() {

	#define statusOk(status) ((status) & 1)  
	#define	M_PI 3.14	


    // Connection variables
    int 	null = 0;
    int 	nodeExists = 0; 
	int 	dtype_long = DTYPE_LONG;
    int 	dtype_float = DTYPE_FLOAT;
    int 	nodeSizeDesc, nodeExistsDesc;
	int		socket, status;

    // Data to write
    int num_samples = 100;
    double* y_data = NULL;
    double* x_data = NULL;
    int 	i;
 	int 	def_current_shot=-1;
		
    printf("[DEBUG] Program started\n");
    
    // 1. Connect to server
	printf("[DEBUG] Connecting to MDSplus server at %s...\n", PHASMA_server_address);
	socket = MdsConnect(PHASMA_server_address);
 	if ( socket == -1 )
 		{
     	fprintf(stderr,"Error connecting to mdsip server.\n");
     	return -1;
 	}
    printf("[DEBUG] Connected with socket = %d\n", socket);
       
	
   /*// For checking exists, we need a properly formatted expression
    // Note: The guide shows that in C strings, backslashes need to be escaped
    // So we shouldn't need to escape the backslash in signal_name as it's already escaped
    status = MdsValue(signal_name, &nodeExistsDesc, &null, &null);

    // If this succeeds, the node exists
    if (statusOk(status)) {
        printf("[DEBUG] Node '%s' exists\n", signal_name);
        } else {
            printf("[DEBUG] Could not determine node type: %s\n", MdsGetMsg(status));
        }
  	*/
	
	/*
    // 3. Allocate and populate data arrays
    printf("[DEBUG] Creating sample data...\n");
    y_data = (double*)malloc(num_samples * sizeof(float));
    x_data = (double*)malloc(num_samples * sizeof(float));
    
    if (!y_data || !x_data) {
        fprintf(stderr, "[ERROR] Memory allocation failed\n");
        if (y_data) free(y_data);
        if (x_data) free(x_data);
        MdsClose(tree_name, &shot);
        return -1;
    }
    
    // Create some sample data - a sine wave
    for (i = 0; i < num_samples; i++) {
        x_data[i] = 0.001 * i;  // Time base in seconds
        y_data[i] = sin(2.0 * M_PI * 10.0 * x_data[i]);  // 10 Hz sine wave
    }
    
     // 5. Write the signal to the node
    printf("[DEBUG] Writing signal to node '%s'...\n", signal_name2);
	Write_to_PHASMA_MDS(x_data,y_data,"RAW_DATA");

    // 6. Clean up
    free(y_data);
    free(x_data);
    */
	
	
    //printf("[DEBUG] Program completed successfully\n");	

    return 0;
}


int ClosePHASMA_MDS() 
{
	int	status;

    // Close tree
    printf("[DEBUG] Closing tree...\n");
    status = MdsClose(tree_name, &shot);
    if (!statusOk(status)) {
        fprintf(stderr, "[ERROR] Failed to close tree: %s\n", MdsGetMsg(status));
		return -1;
    }
    
    printf("[DEBUG] Tree closed successfully\n");	

    return 0;
}

/*
int Write_to_PHASMA_MDS(double t[], double x[], int num_samples, char tree_name[], char tree_node_name[]) {
    #define statusOk(status) ((status) & 1)

    int status;
    int dtype_double = DTYPE_DOUBLE;
    int n_dim = 1;

    struct descriptor t_desc, x_desc;
    int null = 0;

    // 1?? Create descriptors
    t_desc = descr(&dtype_double, t, &num_samples, &n_dim);
    x_desc = descr(&dtype_double, x, &num_samples, &n_dim);

    // 2?? Ensure tree is open
    int shot = 0;  // Replace with your current shot
    status = MdsOpen(tree_name, &shot);
    if (!statusOk(status)) {
        fprintf(stderr, "[ERROR] Failed to open tree '%s': %s\n", tree_name, MdsGetMsg(status));
        return -1;
    }

    // 3?? Write the signal
    status = MdsPut(tree_node_name, "BUILD_SIGNAL($1,,$2)", &x_desc, &t_desc, &null);
    if (!statusOk(status)) {
        fprintf(stderr, "[ERROR] Failed to write signal: %s\n", MdsGetMsg(status));
        MdsClose(tree_name, &shot);
        return -1;
    }

    printf("[DEBUG] Signal successfully written to node '%s'\n", tree_node_name);

    MdsClose(tree_name, &shot);
    return 0;
}
*/


int Write_to_PHASMA_MDS(double t[], double x[], char tree_node_name[]) {

	#define statusOk(status) ((status) & 1)  

	int	socket, status;
	 
    // Connection variables
    int null = 0;
    int nodeExists = 0; 
	int dtype_long = DTYPE_LONG;
    int dtype_float = DTYPE_FLOAT;
    int nodeSizeDesc, nodeExistsDesc;
    int num_samples = 100;
    //int num_samples;

    // Data to write
    int t_desc, x_desc;
 
	// 1. Create descriptors for the data
    x_desc = descr(&dtype_float, x, &num_samples, &null);
    t_desc = descr(&dtype_float, t, &num_samples, &null);

    // Write using BUILD_SIGNAL TDI function
    // First arg ($1) is Y values, third arg ($2) is X values
    //status = MdsPut(signal_name, "BUILD_SIGNAL($1,,$2)", &t_desc, &x_desc, &null);
    status = MdsPut(tree_node_name, "BUILD_SIGNAL($1,,$2)", &x_desc, &t_desc, &null);
    //status = MdsPut(signal_name2, "$1", &y_desc, &null);
    
    if (!statusOk(status)) {
        fprintf(stderr, "[ERROR] Failed to write signal: %s\n", MdsGetMsg(status));
        //MdsClose(tree_name, &shot);
        return -1;
    }
    printf("[DEBUG] Signal successfully written to node '%s'\n", tree_node_name);
    

    return 0;
}



// Function to get the current shot number
int getMDSCurrentShot(void) {
    int status;
    int current_shot = -1;  // -1 typically means the latest shot
    int null = 0;
    int dtype_long = DTYPE_LONG;
    int shotDesc;
    char tdi_expr[256];
    
    printf("[DEBUG] Getting current shot for tree '%s'...\n", tree_name);
    
    // Create descriptor for result
    shotDesc = descr(&dtype_long, &current_shot, &null);
    
    // Use TDI to get the latest shot
    sprintf(tdi_expr, "current_shot('%s')", tree_name);
    
    status = MdsValue(tdi_expr, &shotDesc, &null, &null);
    if (!statusOk(status)) {
        fprintf(stderr, "[ERROR] Failed to get current shot: %s\n", MdsGetMsg(status));
        return 0;  // Default to 0 if we can't get the current shot
    }
    
    printf("[DEBUG] Current shot for tree '%s' is %d\n", tree_name, current_shot);
	
	//Store current shot in global string constant for file name use
	sprintf(ShotNumberString,"%d",current_shot);
	
    return current_shot;
}

int CloseMDSTree(void) {
    int status;
    int current_shot = -1;  // -1 typically means the latest shot
    int null = 0;
    int dtype_long = DTYPE_LONG;
    int shotDesc;
    char tdi_expr[256];
      
    // Create descriptor for result
    shotDesc = descr(&dtype_long, &current_shot, &null);
    
    // Use TDI command to close the tree for the current shot
    sprintf(tdi_expr, "TreeClose()");
    status = MdsValue(tdi_expr, &shotDesc, &null, &null);
	
    if (!statusOk(status)) {
        fprintf(stderr, "[ERROR] Failed to close tree for current shot: %s\n", MdsGetMsg(status));
        return 0;  // Default to 0 if the tree didn't close
    }
    
    printf("[DEBUG] Tree closed '%s' for current shot\n", tree_name);
 
    return status;
}

// Function to increment to the next shot number
// Function writes to a TDI command which it then runs through the MdsValue function.
// The MdsValue function does not appear to return a proper, newly updated shot number.
int IncrementMDSCurrentShot(void) {

	
    int 	status;
    int		next_shot = 0;  		// 0 typically means the latest shot
   	int 	shotDesc;
    int 	null = 0;
	int 	dummy;
   	int 	dtype_long = DTYPE_LONG;
    int		current_shot = 0;  		// 0 typically means the latest shot
    int		def_current_shot = 0;  	// 0 typically means the latest shot
 	char 	tdi_cmd[256];
	char 	src[128];
	char 	dst[128];
	
  
	//Start the incrementing process by first creating a new pulse and then opening it up
    printf("[DEBUG] Incrementing shot for tree '%s'...\n", tree_name);
  	
	//Calculate new shot number
	next_shot=getMDSCurrentShot()+1;
	
  	//Brute force the creation of the next sho .tree file by manually copying the model tree over to the next shot number
	sprintf(src, "%s\\%s_model.tree", MDSDATAPATH, tree_name);
    sprintf(dst, "%s\\%s_%d.tree", MDSDATAPATH, tree_name, next_shot);

    if (!CopyFile(src, dst, FALSE)) {
        fprintf(stderr, "Failed to copy model tree file: %s -> %s\n", src, dst);
        return 0;
    }
	
 	//Brute force the creation of the next shot .characteristics file by manually copying the model tree over to the next shot number
	sprintf(src, "%s\\%s_model.characteristics", MDSDATAPATH, tree_name);
    sprintf(dst, "%s\\%s_%d.characteristics", MDSDATAPATH, tree_name, next_shot);

    if (!CopyFile(src, dst, FALSE)) {
        fprintf(stderr, "Failed to copy model tree file: %s -> %s\n", src, dst);
        return 0;
    }

    printf("[DEBUG] Incrementing shot for tree '%s'...\n", tree_name);
  
	//Create the command to increment to the next shot
    //snprintf(tdi_cmd, sizeof(tdi_cmd),"TreeSetCurrentShot(\"%s\", TreeGetCurrentShot(\"%s\") + 1)", tree_name, tree_name);
   	sprintf(tdi_cmd,"TreeSetCurrentShot('%s', %d)", tree_name, (getMDSCurrentShot()+1));

    // Run the TDI command
    status = MdsValue(tdi_cmd, &dtype_long, &current_shot, &null);

	//Check for errors
    if (status & 1) {
        printf("[DEBUG] Shot number incremented\n");
		//Store current shot in global string constant for file name use
		sprintf(ShotNumberString,"%d",current_shot);        
    } else {
        printf("[DEBUG] Error incrementing shot number: 0x%X\n", status);
        return -1;
    }
	
    // Use TDI command to open the current shot in edit mode	
	sprintf(tdi_cmd, "TreeOpenEdit('%s', %d)", tree_name, current_shot);
   	status = MdsValue(tdi_cmd, &dtype_long, &current_shot, &null);
	if (!statusOk(status)) {
	    fprintf(stderr, "[ERROR] Failed to open tree in edit mode via TDI: %s\n", MdsGetMsg(status));
	    return -1;
	}
	
	//Make sure shot is open
	status = MdsOpen(tree_name, &next_shot);

	//Print status and return
    printf("[DEBUG] Tree opened to current shot successfully with status = %d\n", status);
	
	return next_shot;
}




int Write_to_PHASMA_2D_MDS(double image[], double x[], double y[], char tree_node_name[]) {

	#define statusOk(status) ((status) & 1)  

	int	socket, status;
	 
 
    // Connection variables
    int null = 0;
    int nodeExists = 0; 
	int dtype_long = DTYPE_LONG;
    int dtype_float = DTYPE_FLOAT;
    int nodeSizeDesc, nodeExistsDesc;
    int num_samples;

    // Data to write
    int x_desc, y_desc, image_desc;
  
	//Create descriptors for the data
    image_desc = descr(&dtype_float, image, &num_samples, &null);
    x_desc = descr(&dtype_float, x, &num_samples, &null);
    y_desc = descr(&dtype_float, y, &num_samples, &null);

    // Write using BUILD_SIGNAL TDI function
    // First arg ($1) is image values, third arg ($2) is X values, fourth arg ($3) is Y values
    status = MdsPut(tree_node_name, "BUILD_SIGNAL($1,,$2,$3)", &image_desc, &x_desc, &y_desc,&null);
     
    if (!statusOk(status)) {
        fprintf(stderr, "[ERROR] Failed to write signal: %s\n", MdsGetMsg(status));
        //MdsClose(tree_name, &shot);
        return -1;
    }
    printf("[DEBUG] Signal successfully written to node '%s'\n", tree_node_name);
    

    return 0;
}


