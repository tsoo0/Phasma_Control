"""
@author: Ocean Insight Inc. - Modified by Adam B to better demonstrate 9/20/2024

The purpose of this program is to demonstrate the use of HSAM mode on devices that are capable of using HSAM. 
It will perform a set number of single scans, average them, and then calculate the time taken. 
Then it will perform the same number of scans, while performing on-device averaging, and calculate the time taken, 
and then calculate the speed difers between High-speed and Standard Averaging. HSAM does not need to be toggled, 
and may simple be used by setting Scans to Average; if the device has it, it will use it.
"""

import time
from oceandirect.od_logger import od_logger
from oceandirect.OceanDirectAPI import OceanDirectAPI, OceanDirectError

# Create stream logger
logger = od_logger()

def get_spectra_accelerate(device, num_spectra):
    """
    Retrieves spectra using accelerated mode (with averaging over multiple scans).
    
    Parameters:
    device: Spectrometer device object.
    num_spectra (int): Number of scans to average.
    """
    try:
        # Set the number of scans to average (accelerated mode)
        device.set_scans_to_average(num_spectra)

        # Get the number of pixels in the spectrum
        numb_pixel = len(device.get_formatted_spectrum())

        # Retrieve the formatted spectrum from the device
        print("Starting accelerated spectra collection & simultaneous averaging...")
        spectra_m = device.get_formatted_spectrum()
        print("Accelerated spectra collection & averaging finished.")

    except OceanDirectError as e:
        # Log any errors that occur during the spectra collection
        logger.error(e.get_error_details())

def get_spectra_single(device, numb_spectra):
    """
    Retrieves spectra one by one without averaging until all spectra have been collected and stored in a list (single scan mode).
    
    Parameters:
    device: Spectrometer device object.
    numb_spectra (int): Number of spectra to capture.
    """
    try:
        # Set the device to single scan mode (1 scan)
        device.set_scans_to_average(1)

        # Get the number of pixels in the spectrum
        numb_pixel = len(device.get_formatted_spectrum())

        # Initialize a list to store the summed spectra
        summed_spectrum = [0.0 for _ in range(numb_pixel)]

        print("Starting single spectra collection...")
        # Retrieve the spectra one by one
        for i in range(numb_spectra):
            current_spectrum = device.get_formatted_spectrum()
            # Sum the current spectrum with the summed spectrum
            for j in range(numb_pixel):
                summed_spectrum[j] += current_spectrum[j]
        
        # Notify that averaging is about to begin
        print(f"Averaging {numb_spectra} single scans...")
        # Average the summed spectrum
        averaged_spectrum = [value / numb_spectra for value in summed_spectrum]
        print("Single spectra collection finished.")

        return averaged_spectrum  # Return the averaged spectrum for further use

    except OceanDirectError as e:
        # Log any errors that occur during the spectra collection
        logger.error(e.get_error_details())

def main():
    """
    NOTE:
    This code was only tested in Windows Command Shell.
    """   
    # Initialize the OceanDirect API
    od = OceanDirectAPI()

    # Find all connected USB devices and retrieve their IDs
    device_count = od.find_usb_devices()
    device_ids = od.get_device_ids()
    device_count = len(device_ids)

    if device_count:  # Check if any devices were found
        # Loop through each connected device
        for id in device_ids:
            # Open a connection to the spectrometer device using its ID
            device = od.open_device(id)

            # Get and print the serial number of the device for identification
            serialNumber = device.get_serial_number()
            print("Serial Number: %s" % serialNumber)

            # Set the integration time in microseconds (the duration for which the spectrometer collects light)
            int_time_us  = 5000 #Not all devices can handle this low of an integration time, this example is using an SR2 & HR2
            numb_spectra = 200  #Define the number of spectra to capture
            
            print("Integration time: %d us\n" % int_time_us)

            # Disable electric dark correction (prevents noise in measurements)
            device.set_electric_dark_correction_usage(False)

            # Enable nonlinearity correction to improve accuracy in light intensity measurements
            device.set_nonlinearity_correction_usage(True)

            # Set the integration time for the device to define how long it collects data
            device.set_integration_time(int_time_us)

            # Time and perform single spectra collection
            start_single = time.time()  # Start timer for single capture
            print("Single Capture: %d scans" % numb_spectra)
            averaged_spectrum = get_spectra_single(device, numb_spectra)  # Call function to capture single spectra
            single_time = time.time() - start_single  # Calculate elapsed time for single capture
            print(f"Single spectra collection time: {single_time:.4f} seconds")
            # Note: This time includes capturing and averaging the scans.

            # Time and perform accelerated spectra collection
            start_accelerated = time.time()  # Start timer for accelerated capture
            print("Accelerated Capture: %d scans" % numb_spectra)
            get_spectra_accelerate(device, numb_spectra)  # Call function to capture accelerated spectra
            accelerated_time = time.time() - start_accelerated  # Calculate elapsed time for accelerated capture
            print(f"Accelerated spectra collection time: {accelerated_time:.4f} seconds")
            # Note: This time reflects the device averaging multiple scans in a shorter time frame.

            # Calculate and print the time difference between single and accelerated collection
            time_difference = single_time - accelerated_time
            print(f"Time difference (Single - Accelerated): {time_difference:.4f} seconds")

            # Calculate the difference factor
            if accelerated_time > 0:
                difference_factor = single_time / accelerated_time
                print(f"Accelerated is {difference_factor:.4f} times faster than single scan.") 
            else:
                print("Error: Accelerated collection time is zero, cannot calculate difference factor.")

            # Prompt user to press Enter before shutting down
            input("Press Enter to shut down the device...")

            # Delete the device instance & shut down OceanDirect
            try:
                del od  
                print(f"Device {serialNumber} closed and OceanDirect was shut down.")
            except Exception as e:
                print(f"Error during shutdown: {e}")

    print("**** exiting program ****")  # Indicate program termination

if __name__ == '__main__':
    main()


