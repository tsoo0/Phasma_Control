import numpy

def write_primes():
    primes = []
    num = 2
    while len(primes) < 10:
        for p in primes:
            if num % p == 0:
                break
        else:
             primes.append(num)
        num += 1
 
    print("Hello World from inside write_primes\n")   
 
    with open("D:\\TANSTAAFL\\To Do\\PHASMA DAQ V2025\\PHASMA 2025 DAQ\\d-TAC Instruments\\PHASMA Python Routines\\primeoutput.txt", "w") as f:
        for p in primes:
            f.write(f"{p}\n")
    f.close()



def Config_Arm(num_samples,shot_number):

    samples = int(num_samples)

    shot = int(shot_number)
      
	
    print("Hello World from inside of Config_Arm %d\n" % shot)   
    
    primes = []
    num = 2
    while len(primes) < shot:
        for p in primes:
            if num % p == 0:
                break
        else:
             primes.append(num)
        num += 1
 
    print("Hello World from inside write_primes\n")   
 
    with open("D:\\TANSTAAFL\\To Do\\PHASMA DAQ V2025\\PHASMA 2025 DAQ\\d-TAC Instruments\\PHASMA Python Routines\\Confi_output.txt", "w") as f:
        for p in primes:
            f.write(f"{p}\n")
    f.close()
