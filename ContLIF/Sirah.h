// This header file includes all the variable declarations necessary
// for sending commands to the Sirah Matisse Dye Laser


void Sirah_Laser_Scan(char[]);
void Sirah_Laser_Stop(char[]);
void Sirah_Laser_ScanReset(int, char[],double, double, double, double, int, int);
void Sirah_Laser_SetScan(char[],double, double, double);
void Sirah_Laser_SetScan_Down(char[],double, double, double);
void Sirah_Laser_Slew(char[],double, double, double, double, double);
