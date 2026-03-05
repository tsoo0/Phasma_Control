
/***
#ifndef Py_PACK_FULL_VERSION
#define Py_PACK_FULL_VERSION(X, Y, Z, LEVEL, SERIAL) ( \
   (((X) & 0xff) << 24) |                              \
   (((Y) & 0xff) << 16) |                              \
   (((Z) & 0xff) << 8) |                               \
   (((LEVEL) & 0xf) << 4) |                            \
   (((SERIAL) & 0xf) << 0))
#endif

#define Py_LIMITED_API Py_PACK_FULL_VERSION(3, 9, 0, 0, 0)
****/



#include <windows.h>
#include <stdio.h>
#pragma warning(disable : 4996) // disable string safety; Microsoft can pry strcat from my cold dead hands
// Export macro for Windows DLLs
#define EXPORT __declspec(dllexport)
#include <string.h>

#define BUFFSIZE 1024
const char WavelengthArgFormat[] = "%.1f";
const char CommandArgFormat[] = "%s";

// Tom's version: no reason to do evironment management ourselves. Let Conda do the hard work. Move to ShellExecute for non-blocking function calls

EXPORT int SendToMcPhersonPy(char * argstr) {

    char command[BUFFSIZE] = "powershell.exe conda activate McPhersonPython; cd 'C:/PHASMA 2025 DAQ/PHASMA Python Routines/PHASMA_C_to_Python_convertor_Project/McPhersonControl'; python McPherson_main.py ";
    char arg[BUFFSIZE];

    strncat_s(command, BUFFSIZE, argstr, BUFFSIZE);

    system(command);

    return 0;

    //system("powershell.exe conda activate McPhersonPython; cd 'C:/PHASMA 2025 DAQ/PHASMA Python Routines/PHASMA_C_to_Python_convertor_Project/McPhersonControl'; python McPherson_main.py '410'");
}

EXPORT int send_command(char* command, float wavelength) {

    char arg1[BUFFSIZE];
    char arg2[BUFFSIZE];
    snprintf(arg1, sizeof(arg1), CommandArgFormat, command);

    snprintf(arg2, sizeof(arg2), WavelengthArgFormat, wavelength);
    strcat(arg1, " ");
    strcat(arg1, arg2);

    return SendToMcPhersonPy(arg1);
}

EXPORT int overwriteLocation_207(float wavelength) {

    char arg1[BUFFSIZE];
    char arg2[] = "overwrite207";
    
    snprintf(arg1, sizeof(arg1), WavelengthArgFormat, wavelength);
    strcat(arg1, " ");
    strcat(arg1, arg2);

    return SendToMcPhersonPy(arg1);
}

EXPORT int overwriteLocation_209(float wavelength) {

    char arg1[BUFFSIZE];
    char arg2[] = "overwrite209";

    snprintf(arg1, sizeof(arg1), WavelengthArgFormat, wavelength);
    strcat(arg1, " ");
    strcat(arg1, arg2);

    return SendToMcPhersonPy(arg1);
}

EXPORT int move_207(float wavelength) {

    char arg1[BUFFSIZE];
    char arg2[] = "move207";

    snprintf(arg1, sizeof(arg1), WavelengthArgFormat, wavelength);
    strcat(arg1, " ");
    strcat(arg1, arg2);

    return SendToMcPhersonPy(arg1);
}

EXPORT int move207_nonblocking(float wavelength) {

    char arg1[BUFFSIZE];
    char arg2[] = "move207";

    snprintf(arg1, sizeof(arg1), WavelengthArgFormat, wavelength);
    strcat(arg1, " ");
    strcat(arg1, arg2);

    char command[BUFFSIZE] = "powershell.exe conda activate McPhersonPython; cd 'C:/PHASMA 2025 DAQ/PHASMA Python Routines/PHASMA_C_to_Python_convertor_Project/McPhersonControl'; python McPherson_main.py ";
    char arg[BUFFSIZE];

    strncat_s(command, BUFFSIZE, arg1, BUFFSIZE);

    system(command);

    return 0;
}

EXPORT int move_209(float wavelength) {

    char arg1[BUFFSIZE];
    char arg2[] = "move209";

    snprintf(arg1, sizeof(arg1), WavelengthArgFormat, wavelength);
    strcat(arg1, " ");
    strcat(arg1, arg2);

    return SendToMcPhersonPy(arg1);
}

EXPORT int command207(char * argstr) {

    return SendToMcPhersonPy(argstr);
}

EXPORT int command209(char * argstr) {

    return SendToMcPhersonPy(argstr);
}


EXPORT int move_probe(char* IP, float arg1, float arg2, float arg3)
{
    return 0;
}

EXPORT int home_probe(char* IP) {
    printf("'asdas'");
    return 0;
}

EXPORT int acq400_Arm(char* IP) {
    return 0;
}

EXPORT int acq400_Read(char* IP, char* path) {
    return 0;
}

EXPORT int acq400_config(char* IP, char* settings) {
    return 0;
}
//void main(void) {}

/*
This code snippet is here to be able to test the primary routines during a compile.
It is commented out because when this program is being used to create a library of
routines to be called elsewhere, it is not needed
*/
/*
void main(void)
{
    //overwriteLocation_207(435.0);
    overwriteLocation207_nonblocking(500);

    printf("beep");
}



*/
