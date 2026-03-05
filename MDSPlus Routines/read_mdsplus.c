#include <mdslib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



int main() {
    // MDSplus connection parameters - use non-const strings!
    char server_address[128] = "localhost:8000";
    char tree_name[128] = "MyTree";
    char signal_name[128] = "\\MySignal";
    int shot = 1250416081;
    
    // Connection variables
    int socket, status;
    int null = 0;
    int dtype_long = DTYPE_LONG;
    int dtype_float = DTYPE_FLOAT;
    
    // For checking node info
    int nodeExists = 0;
    int dataSize = 0;
    int nodeSizeDesc, nodeExistsDesc;
    char nodeType[32];
    
    // For reading data
    float* y_data = NULL;
    float* x_data = NULL;
    int y_desc, x_desc;
    int len_read = 0;
    int max_size = 0;
    int i;
    char dim_expression[256];
    
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
    
    // 3. Check if node exists
    printf("[DEBUG] Checking if node '%s' exists...\n", signal_name);
    nodeExistsDesc = descr(&dtype_long, &nodeExists, &null);
    
    // For checking exists, we need a properly formatted expression
    // Note: The guide shows that in C strings, backslashes need to be escaped
    // So we shouldn't need to escape the backslash in signal_name as it's already escaped
    status = MdsValue(signal_name, &nodeExistsDesc, &null, &null);
    
    // If this succeeds, the node exists
    if (statusOk(status)) {
        printf("[DEBUG] Node '%s' exists\n", signal_name);
        
      } else {
            printf("[DEBUG] Could not determine node type: %s\n", MdsGetMsg(status));
        }
        
        // 4. Get data size
        printf("[DEBUG] Getting size of node data...\n");
        nodeSizeDesc = descr(&dtype_long, &dataSize, &null);
        
        char sizeExpr[256];
        sprintf(sizeExpr, "SIZE(%s)", signal_name);
        
        status = MdsValue(sizeExpr, &nodeSizeDesc, &null, &null);
        if (!statusOk(status)) {
            fprintf(stderr, "[ERROR] Failed to get data size: %s\n", MdsGetMsg(status));
            MdsClose(tree_name, &shot);
            return -1;
        }
        printf("[DEBUG] Node '%s' contains %d elements\n", signal_name, dataSize);
        
        if (dataSize <= 0) {
            printf("[ERROR] Node has no data (size = %d)\n", dataSize);
            MdsClose(tree_name, &shot);
            return -1;
        }
        
        // Continue with data reading if needed...
        
    } else {
        fprintf(stderr, "[ERROR] Node '%s' does not exist or check failed: %s\n", 
                signal_name, MdsGetMsg(status));
        MdsClose(tree_name, &shot);
        return -1;
    }
    
    // 9. Close tree
    printf("[DEBUG] Closing tree...\n");
    status = MdsClose(tree_name, &shot);
    if (!statusOk(status)) {
        fprintf(stderr, "[ERROR] Failed to close tree: %s\n", MdsGetMsg(status));
    }
    
    printf("[DEBUG] Program completed successfully\n");
    return 0;
}
