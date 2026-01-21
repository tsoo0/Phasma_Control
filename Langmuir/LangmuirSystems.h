// This header file includes all the variable declarations necessary
// for acquiring data from the Langmuir probe.
void set_keithley(int gpib_num, double min_volts, double max_volts, double max_current);
void keithley_trigger(void);
int  Langmuir_Activate(void);			   
void keithley_aquire(int keithley_gpibS, int keithley_gpibP, double *volt, double *current, double *voltP, double *currentP, double *Keithley_time,double *Keithley_timeP);
void Langmuir_Analyze(void);
void Langmuir_plot(void);
void write_Langmuir_data(void);
void Langmuir_Acquire(void);  
void keithley_end(int gpib_num);
void OpenLang_Settings (void);



