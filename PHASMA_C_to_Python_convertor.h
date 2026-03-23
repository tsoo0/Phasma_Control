// PHASMA_C_to_Python_convertor.h
#ifndef PHASMA_C_to_Python_convertor_H
#define PHASMA_C_to_Python_convertor_H

int acq400_Arm(char *);
int acq400_Read(char * , char *);
int move_probe(char *, float, float, float);
//int acq400_config(char * , char *);
int home_probe(char *);

int WritePrimesToFile(void);

int command209(char *);
int command207(char *);
int move_207(float );
int overwriteLocation_207(float );
int move_209(float );
int overwriteLocation_209(float );

#endif
