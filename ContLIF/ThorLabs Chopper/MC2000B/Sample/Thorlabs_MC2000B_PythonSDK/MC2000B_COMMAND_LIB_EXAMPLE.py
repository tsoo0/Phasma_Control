try:
    from MC2000B_COMMAND_LIB import *
except OSError as ex:
    print("Warning:",ex)


def CommonFunc(serialNumber):
   hdl = MC2000BOpen(serialNumber,115200,3)

   if(hdl < 0):
       print("Connect ",serialNumber, "fail" )
       return -1;
   else:
       print("Connect ",serialNumber, "successful")
   
   result = MC2000BIsOpen(serialNumber)
   if(result<0):
       print("Open failed ")
   else:
       print("MC2000B Is Open ")
      
   return hdl

def CheckFunctions(hdl):


    result=MC2000BSetBladeType(hdl,9) # Blade type 0:MC1F2, 1:MC1F10, 2:MC1F15, 3:MC1F30, 4:MC1F60, 5:MC1F100, 6:MC1F10HP, 7:MC1F2P10, 8:MC1F6P10, 9:MC1F10A, 10:MC2F330, 11:MC2F47, 12:MC2F57B, 13:MC2F860, 14:MC2F5360 
    if(result<0):
       print("Set Blade Type fail",result)
    else:
       print("Set Blade Type :", "MC1F10A")

    result=MC2000BSetFrequency(hdl,20)# Frequency range dependent on blade type
    if(result<0):
       print("Set Frequency fail",result)
    else:
       print("Set Frequency :", "20Hz")


    result=MC2000BSetReference(hdl,1)# Dependent on blade type. Reference mode 0:internal,1:external / Reference high precision mode 0:InternalOuter 1:InternalInner 2:ExternalOuter 3:ExternalInner    
    if(result<0):
       print("Set Reference Mode fail",result)
    else:
       print("Set Reference Mode :", "1")

    result=MC2000BSetHarmonicMultiplier(hdl,6)
    if(result<0):
       print("Set Harmonic Multiplier fail",result)
    else:
       print("Set Harmonic Multiplier :", "6")

    result=MC2000BSetHarmonicDivider(hdl,5)
    if(result<0):
       print("Set Harmonic Divider fail",result)
    else:
       print("Set Harmonic Divider :", "5")

    result=MC2000BSetPhase(hdl,2)
    if(result<0):
       print("Set phase fail",result)
    else:
       print("Set phase :", "2")

    result=MC2000BSetEnable(hdl,0)# enable state 1:enable, 0:disable
    if(result<0):
       print("Set Enable State fail",result)
    else:
       print("Set Enable State:", "disable") 

    result=MC2000BSetReferenceOutput(hdl,1) # Output mode dependent on blade type
    if(result<0):
       print("Set Output mode",result)
    else:
       print("Set Output mode :", "1")

    result=MC2000BSetDisplayIntensity(hdl,5) # display intensity (1-10)
    if(result<0):
       print("Set Display Intensity fail",result)
    else:
       print("Set Display Intensity :", "5")

    result=MC2000BSetVerbose(hdl,0) # When verbose mode is set to 1, status messages are output on the USB
    if(result<0):
       print("Set Verbose Mode fail",result)
    else:
       print("Set Verbose Mode :", "0")
    
    result=MC2000BSetCycleAdjust(hdl,3)# Used for specific blade type
    if(result<0):
       print("Set Cycle fail",result)
    else:
       print("Set Cycle :", "3")

    print("---------------------------Paras set finnished-------------------------")

     
    bladetype=[0]
    result= MC2000BGetBladeType(hdl,bladetype)
    BladeTypeList={0:"MC1F2",1:"MC1F10",2:"MC1F15",3:"MC1F30",4:"MC1F60",5:"MC1F100",6:"MC1F10HP",7:"MC1F2P10",8:"MC1F6P10",9:"MC1F10A",10:"MC2F330",11:"MC2F47",12:"MC2F57B",13:"MC2F860",14:"MC2F5360"}
    if(result<0):
       print("Get Blade Type fail",result)
    else:
       print("Get Blade Type :",BladeTypeList.get(bladetype[0]))

    frequency=[0]
    result=MC2000BGetFrequency(hdl,frequency)
    if(result<0):
       print("Get Frequency fail",result)
    else:
       print("Get Frequency :",frequency)

    reference=[0]
    result=MC2000BGetReference(hdl,reference)
    if(result<0):
       print("Get Reference Mode fail",result)
    else:
       print("Get Reference Mode :",reference)

    harmonicMultiplier=[0]
    result=MC2000BGetHarmonicMultiplier(hdl,harmonicMultiplier)
    if(result<0):
       print("Get Harmonic Multiplier fail",result)
    else:
       print("Get Harmonic Multiplier :",harmonicMultiplier)

    harmonicDivider=[0]
    result=MC2000BGetHarmonicDivider(hdl,harmonicDivider)
    if(result<0):
       print("Get Harmonic Divider fail",result)
    else:
       print("Get Harmonic Divider :",harmonicDivider)

    phase=[0]
    result=MC2000BGetPhase(hdl,phase)
    if(result<0):
       print("Get Phase fail",result)
    else:
       print("Get Phase :",phase)

    enable=[0]
    enableList={0:"disable",1:"enable"}
    result=MC2000BGetEnable(hdl,enable)
    if(result<0):
       print("Get Enable State",result)
    else:
       print("Get Enable State :",enableList.get(enable[0]))



    referenceOutput=[0]
    result=MC2000BGetReferenceOutput(hdl,referenceOutput)
    if(result<0):
       print("Get Output Mode fail",result)
    else:
       print("Get Output Mode :",referenceOutput)

    displayIntensity=[0]
    result=MC2000BGetDisplayIntensity(hdl,displayIntensity)
    if(result<0):
       print("Get Display Intensity fail",result)
    else:
       print("Get Display Intensity :",displayIntensity)

    verbose=[0]
    result=MC2000BGetVerbose(hdl,verbose)
    if(result<0):
       print("Get Verbose Mode fail",result)
    else:
       print("Get Verbose Mode :",verbose)

    cycle=[0]
    result=MC2000BGetCycleAdjust(hdl,cycle)
    if(result<0):
       print("Get Cycle fail",result)
    else:
       print("Get Cycle :",cycle)

    print("---------------------------Paras get finnished-------------------------")


def main():
    print("*** MC2000B device python example ***")
    try:
        devs = MC2000BListDevices()
        print(devs)
        if(len(devs)<=0):
           print('There is no devices connected')
           exit()

        MC2000B= devs[0]

        hdl=CommonFunc(MC2000B[0])
        print("---------------------------Device initialize finished-------------------------")
        CheckFunctions(hdl)
        MC2000BClose(hdl)
        
    except Exception as ex:
        print("Warning:", ex)
    print("*** End ***")
    input()
main()
