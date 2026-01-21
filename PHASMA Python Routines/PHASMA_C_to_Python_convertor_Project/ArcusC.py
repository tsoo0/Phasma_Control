import math
from pymodbus.client import ModbusTcpClient as Client
#from pymodbus.client import AsyncModbusTcpClient as AClient
#import asyncio
import time


### BEGIN HELPER FUNCTIONS

def checkRegisters(client: Client, *registers):
    decodedValues = []
    for i in registers:
        encodedValue = client.read_holding_registers(address=i, count=2)
        
        decoded = client.convert_from_registers(encodedValue.registers,client.DATATYPE.INT32,"big")
        decodedValues.append(decoded)
    return decodedValues

def writeRegisters(client: Client, register, value):
    for i in range(len(register)):

        payload = client.convert_to_registers(value[i],client.DATATYPE.INT32,"big")
        client.write_registers(address=register[i], values=payload)

def initiate(client:Client, speed=20, accel=4000, jerk=10000, movePattern=1, homingMode=0):
    '''Initiates parameters on the motor, if you do not give parameters the default ones shown above will be used'''
 
    
    payloadunbuilt = [speed, accel, jerk, movePattern, homingMode]
    addresses = [2, 4, 6, 8, 10]

    #in order, these addresses corrispond to:
    #target speed, target acceleration, target jerk, move pattern, homing mode
    writeRegisters(client, addresses, payloadunbuilt)

    client.write_coils(7, [False]) 
    client.write_coils(7, [True]) #turn servo on


### END HELPER FUNCTIONS


### BEGIN USER FUNCTIONS

def motorStatus(client:Client):
    '''returns the status of the motor (used as a helper function as well)
       in order, the returned array is [enabled, in position, moving, in fault, homed]
       because it is also a helper function, you need to give it a client, not an ip   
    '''
    statusDecimal = checkRegisters(client, 10)
    statusBin = f'{statusDecimal[0]:0{5}b}'
    #the :0b specifies in decimal, while the {5} ensures there are 5 total digits, even if they are 0
    #if I used bin(statusDecimal[0]) it would trim the leading 0's and the info with it
    
    enabled = int(statusBin[4])
    inPos = int(statusBin[3])
    moving = int(statusBin[2])
    inFault = int(statusBin[1])
    homed = int(statusBin[0])
    return [enabled,inPos,moving,inFault,homed]

def move(ip: str, distance, CPMM, insertRadius, speed=20, accel=4000, jerk=10000, movePattern=1, homingMode=0):
    '''moves a motor given the IP, distance to move (in mm), how many encoder counts per mm, the maximum allowed move distance, speed, acceleration, jerk, moving pattern, and homing mode. The final 5 parameters are optional and have default values if not provided.'''
    client = Client(ip, port=5000)
    client.connect()                # connect to device, reconnect automatically
        
    client.write_coils(13, [False]) #clears errors
    client.write_coils(13, [True])

    initiate(client, speed, accel, jerk, movePattern, homingMode) #initiates the motor
    
    isMoving = motorStatus(client)
    if isMoving[2] == 1:
        print("already moving, ignoring command!")
        return
    elif isMoving[4] != True:
        print("please home before moving. Ignoring command!")
        return

    MovDistance = -1*(distance * float(CPMM))
    print(math.floor(MovDistance))
    SoftLimit = -1*(insertRadius * CPMM)
    if MovDistance <= SoftLimit:
        raise Exception(f"You cannot move more than what this probe allows ({insertRadius})")

    writeRegisters([0], [int(math.floor(MovDistance))])
    client.write_coils(9, [False])
    client.write_coils(9, [True]) #begin movement

    client.close() #closes the connection to the client (does not stop movement)

def home(ip: str, speed=20, accel=4000, jerk=10000, movePattern=1, homingMode=0):
    '''Homes the motor given ip, speed, accel, jerk, move pattern, and homing mode. The last 5 parameters are optional and have default vaules if not provided'''
    client = Client(ip, port=5000)
    client.connect()                # connect to device, reconnect automatically
        
    client.write_coils(13, [False]) #clears errors
    client.write_coils(13, [True])
    
    initiate(client, speed, accel, jerk, movePattern, homingMode) #initiates the motor

    isMoving = motorStatus(client)
    if isMoving[2] == 1:
        print("already moving, ignoring command!")
        return
    
    client.write_coils(14, [False])
    client.write_coils(14, [True]) #stop all motion

    #client.write_coils(12, [False])
    #client.write_coils(12, [True]) #homes the motor until limit switch is hit (which is won't)


    #isHomed = motorStatus(client) #blocking while loop.
    #while isHomed[3] != 1: #check for a fault
    #    isHomed = motorStatus(client)
    #    time.sleep(1)

    #once faulted, reenable then set encoder position to 0
    #client.write_coils(7, [False]) 
    #client.write_coils(7, [True]) #turn servo on

    #client.write_coils(13, [False]) #clears errors
    #client.write_coils(13, [True])

    #writeRegisters(client, [14], [0]) #set encoder position to 0
    
    client.write_coil(27, [True]) #Runs A-script programs (this is shown below)
    '''
    SVON ;***Turn on the servo
    WAITX ;***Wait for servo to be turned on
    HSPD=50 ;***Set High Speed to 50 RPM
    ACC=10000 ;***Set Acceleration to 10000 RPM/sec
    INPOS=2000 ;***Set In Position Range to 2000
    JOGXP ;***Jog in Plus Direction
    WHILE CUR < 500 ;***Wait for current to above 500 mA
    ENDWHILE
    STOPX ;***Stop jogging
    EX=1000 ;***Set the current hard stop position to 1000
    X0 ;***Move out slight from hard stop to zero
    HMODE=9 ;***Negative index homing
    HOMEX ;***Start homing
    WAITH ;***Wait for homing to complete

    '''

    client.close() #closes the connection to the client (does not stop movement)

### END USER FUNCTIONS






### Ignore the rest it's just the original code commented out




#class Probes:
#    def __init__(self, ip: str, insertRadius: int, COUNTSPERMM: int):
#        self.ip = ip
#        self.r = insertRadius
#        self.CPMM = COUNTSPERMM
#        self.client = 0
#        self.speed = 0
#        self.accel = 0
#        self.homed = False
#
#    async def connect(self):
#        self.client = AClient(self.ip, port=5000)             # Create client object
#        await self.client.connect()                # connect to device, reconnect automatically
#        
#        await self.client.write_coils(13, [False]) #clears errors
#        await self.client.write_coils(13, [True])
#
#    
#    async def disconnect(self):
#        await self.client.write_coils(8, [False])
#        await self.client.write_coils(8, [True]) #turn off motor
#
#        self.client.close() #close the connection
#    
#    async def initiate(self, speed: int, accel: int):
#        payloadunbuilt = [speed, accel, 10000, 1, 0]
#        addresses = [2, 4, 6, 8, 10]
#
#        #in order, these addresses corrispond to:
#        #target speed, target acceleration, target jerk, move pattern, homing mode
#        await self.writeRegisters(addresses, payloadunbuilt)
#
#        await self.client.write_coils(7, [False]) 
#        await self.client.write_coils(7, [True]) #turn servo on
#
#    async def checkRegisters(self, *registers):
#        decodedValues = []
#        for i in registers:
#            encodedValue = await self.client.read_holding_registers(address=i, count=2)
#            #decoded = BinaryPayloadDecoder.fromRegisters(encodedValue.registers, byteorder=Endian.BIG, wordorder=Endian.BIG).decode_32bit_int()
#            decoded = self.client.convert_from_registers(encodedValue.registers,self.client.DATATYPE.INT32,"big")
#            decodedValues.append(decoded)
#        return decodedValues
#    
#    async def writeRegisters(self, register, value):
#        #builder = BinaryPayloadBuilder(byteorder=Endian.BIG, wordorder=Endian.BIG)
#        for i in range(len(register)):
#            #builder.add_32bit_int(value[i])
#            #payload = builder.to_registers()
#            payload = self.client.convert_to_registers(value[i],self.client.DATATYPE.INT32,"big")
#            await self.client.write_registers(address=register[i], values=payload)
#            #builder.reset()
#
#    async def motorStatus(self):
#        statusDecimal = await self.checkRegisters(10)
#        statusBin = f'{statusDecimal[0]:0{5}b}'
#        #the :0b specifies in decimal, while the {5} ensures there are 5 total digits, even if they are 0
#        #if I used bin(statusDecimal[0]) it would trim the leading 0's and the info with it
#    
#        enabled = int(statusBin[4])
#        inPos = int(statusBin[3])
#        moving = int(statusBin[2])
#        inFault = int(statusBin[1])
#        homed = int(statusBin[0])
#        return [enabled,inPos,moving,inFault,homed]
#    
#    async def move(self, distance: float): #give distance in mm
#        
#        isMoving = await self.motorStatus()
#        if isMoving[2] == 1:
#            print("already moving, ignoring command!")
#            return
#        elif self.homed != True:
#            print("please home before moving. Ignoring command!")
#            return
#
#        MovDistance = -1*(distance * float(self.CPMM))
#        print(math.floor(MovDistance))
#        SoftLimit = -1*(self.r * self.CPMM)
#        if MovDistance <= SoftLimit:
#            raise Exception(f"You cannot move more than what this probe allows ({self.r})")
#
#
#        #print(payload)
#        await self.writeRegisters([0], [int(math.floor(MovDistance))])
#
#        await self.client.write_coils(9, [False])
#        await self.client.write_coils(9, [True]) #begin movement
#
#        isMoving = await self.motorStatus()
#        while isMoving[2] == 1:
#            isMoving = await self.motorStatus()
#            await asyncio.sleep(1)
#
#    
#    async def home(self):
#
#        isMoving = await self.motorStatus()
#        if isMoving[2] == 1:
#            print("already moving, ignoring command!")
#            return
#
#        await self.client.write_coils(14, [False])
#        await self.client.write_coils(14, [True]) #stop all motion
#
#        await self.client.write_coils(12, [False])
#        await self.client.write_coils(12, [True]) #homes the motor until limit switch is hit
#
#
#        #TODO: somehow stop movement without interupting the flow of the program
#        isHomed = await self.motorStatus()
#        while isHomed[3] != 1: #check for a fault
#            isHomed = await self.motorStatus()
#            await asyncio.sleep(1)
#
#        #once faulted, reenable then set encoder position to 0
#
#        await self.client.write_coils(7, [False]) 
#        await self.client.write_coils(7, [True]) #turn servo on
#
#        await self.client.write_coils(13, [False]) #clears errors
#        await self.client.write_coils(13, [True])
#
#        await self.writeRegisters([14], [0]) #set encoder position to 0
#
#        self.homed = True
#
#        return True
#    
#    async def ESTOP(self):
#        print("STOPPING MOTION")
#        await self.client.write_coils(14, [False])
#        await self.client.write_coils(14, [True]) #stop all motion
#        print("MOTION STOPPED, SHUTTING DOWN")
#        status = await self.motorStatus()
#        faultCodes = await self.checkRegisters(12)
#        print(f"Motor Status: {status} :: Fault Codes: {faultCodes}")
#        await self.disconnect()