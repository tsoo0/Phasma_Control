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
 
    print("Hello World\n")   
 
    with open("D:\TANSTAAFL\To Do\PHASMA DAQ V2025\PHASMA 2025 DAQ\d-TAC Instruments\PHASMA Python Routines\primeoutput.txt", "w") as f:
        for p in primes:
            f.write(f"{p}\n")
    f.close()


def write_primes2():
    primes = []
    num = 2
    while len(primes) < 10:
        for p in primes:
            if num % p == 0:
                break
        else:
             primes.append(num)
        num += 1
 
    print("Hello World\n")   
 
    with open("D:\TANSTAAFL\To Do\PHASMA DAQ V2025\PHASMA 2025 DAQ\d-TAC Instruments\PHASMA Python Routines\primeoutput2.txt", "w") as f:
        for p in primes:
            f.write(f"{p}\n")
    f.close()