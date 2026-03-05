//Do a reset of VXI backplane. To be used before talking to VXI instruments as as way of trying to avoid lockups
void ResetVXI(void)
{
    ViSession vi;

    viOpen(Global_Visa_Session_Handle, "visa://192.168.1.207/VXI0::0::INSTR", VI_NULL, VI_NULL, &vi);

    viClear(vi);

    Delay(0.5);

    viClose(vi);
}

