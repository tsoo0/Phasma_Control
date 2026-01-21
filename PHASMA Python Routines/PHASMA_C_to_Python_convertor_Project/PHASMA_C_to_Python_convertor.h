// PHASMA_C_to_Python_convertor.h
#ifndef PHASMA_C_to_Python_convertor_H
#define PHASMA_C_to_Python_convertor_H

int WritePrimesToFile();
int t_DAQ_Config(char*, float, int);
int t_DAQ_Arm(char* );
int t_DAQ_Data_Read(char *, char *);
int move_probe(char *, float , float , float );
int home_probe(char *);
int move_207(float );
int overwriteLocation_207(float );
int move_209(float );
int overwriteLocation_209(float );

#endif
