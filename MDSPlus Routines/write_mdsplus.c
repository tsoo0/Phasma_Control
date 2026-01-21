#include <mdslib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// Helper function to check status
int statusOk(int status) {
    return (status & 1);
}

// Need to declare this as it's not in headers
char *MdsGetMsg(int status);

int main() {
    // MDSplus connection parameters
    char server_address[128] = "localhost:8000";
    char tree_name[128] = "MyTree";
    char signal_name[128] = "\\MySignal";  // Node to write to
    int shot = 1250416081;
    
    // Connection variables
    int socket, status;
    int null = 0;
    int dtype_long = DTYPE_LONG;
    int dtype_float = DTYPE_FLOAT;
    
    // Data to write
    int num_samples = 100;
    float* y_data = NULL;
    float* x_data = NULL;
    int y_desc, x_desc;
    int i;
    
    printf("[DEBUG] Program started\n");
    
    // 1. Connect to server
    printf("[DEBUG] Connecting to MDSplus server at %s...\n", server_address);
    socket = MdsConnect(server_address);
    if (socket == -1) {
        fprintf(stderr, "[ERROR] Failed to connect to mdsip server\n");
        return -1;
    }
    printf("[DEBUG] Connected with socket = %d\n", socket);
    
    // 2. Open tree
    printf("[DEBUG] Opening tree '%s' (shot: %d)...\n", tree_name, shot);
    status = MdsOpen(tree_name, &shot);
    if (!statusOk(status)) {
        fprintf(stderr, "[ERROR] Failed to open tree: %s\n", MdsGetMsg(status));
        return -1;
    }
    printf("[DEBUG] Tree opened successfully with status = %d\n", status);
    
    // 3. Allocate and populate data arrays
    printf("[DEBUG] Creating sample data...\n");
    y_data = (float*)malloc(num_samples * sizeof(float));
    x_data = (float*)malloc(num_samples * sizeof(float));
    
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
    
    // 4. Create descriptors for the data
    y_desc = descr(&dtype_float, y_data, &num_samples, &null);
    x_desc = descr(&dtype_float, x_data, &num_samples, &null);
    
    // 5. Write the signal to the node
    printf("[DEBUG] Writing signal to node '%s'...\n", signal_name);
    
    // Write using BUILD_SIGNAL TDI function
    // First arg ($1) is Y values, third arg ($2) is X values
    status = MdsPut(signal_name, "BUILD_SIGNAL($1,,$2)", &y_desc, &x_desc, &null);
    
    if (!statusOk(status)) {
        fprintf(stderr, "[ERROR] Failed to write signal: %s\n", MdsGetMsg(status));
        free(y_data);
        free(x_data);
        MdsClose(tree_name, &shot);
        return -1;
    }
    printf("[DEBUG] Signal successfully written to node '%s'\n", signal_name);
    
    // 6. Close tree
    printf("[DEBUG] Closing tree...\n");
    status = MdsClose(tree_name, &shot);
    if (!statusOk(status)) {
        fprintf(stderr, "[ERROR] Failed to close tree: %s\n", MdsGetMsg(status));
    }
    
    // 7. Clean up
    free(y_data);
    free(x_data);
    
    printf("[DEBUG] Program completed successfully\n");
    return 0;
}