# -*- coding: utf-8 -*-
"""
Created 9/18/2024

The purpose of this progrtam is to observe intensities at user-defined wavelengths. 
The user will set the integration time, wavelengths of interest, and the program will give the intensities at those points.

@original author: Ocean Optics
@editor: Adam B
"""

from oceandirect.OceanDirectAPI import OceanDirectAPI, OceanDirectError

def get_device_info(od):
    """
    Retrieves and returns device count and IDs.
    
    Parameters:
    od (OceanDirectAPI): Initialized OceanDirectAPI object.
    
    Returns:
    tuple: A tuple containing the device count and a list of device IDs.
    """
    device_count = od.find_usb_devices()  # Find all connected USB devices
    device_ids = od.get_device_ids()  # Get the IDs of the connected devices
    return device_count, device_ids

def prompt_user_for_integration_time():
    """
    Prompts the user to enter the desired integration time in microseconds.
    
    Returns:
    int: The user-provided integration time.
    """
    while True:
        try:
            # Ask the user for an integration time in microseconds
            integration_time = int(input("Enter the desired integration time in microseconds (e.g., 50000 for 50ms): "))
            if integration_time > 0:
                return integration_time
            else:
                print("Please enter a positive integer value for the integration time.")
        except ValueError:
            print("Invalid input. Please enter a valid integer.")

def prompt_user_for_pixel_selection():
    """
    Prompts the user to select up to 4 pixel (wavelength) positions for spectral focus.
    
    Returns:
    list: A list of selected pixel positions.
    """
    while True:
        try:
            # Ask the user for 4 pixel positions, separated by commas
            pixels_input = input("Enter up to 4 pixel (wavelength) positions to focus on, separated by commas (e.g., 100,200,300,400): ")
            # Convert input string into a list of integers
            pixel_positions = [int(x.strip()) for x in pixels_input.split(",")]
            if len(pixel_positions) > 4:
                print("Please enter no more than 4 pixel positions.")
            elif all(p >= 0 for p in pixel_positions):
                return pixel_positions
            else:
                print("Please enter only positive integers for pixel positions.")
        except ValueError:
            print("Invalid input. Please enter valid integers separated by commas.")

def main():
    """
    Main function to execute the script, retrieve device info, and process devices.
    NOTE:
    This code was only tested in Windows Command Shell.
    """
    od = OceanDirectAPI()  # Initialize the OceanDirect API object
    device_count, device_ids = get_device_info(od)  # Retrieve device count and IDs
    
    print(f"Total Device: {device_count}")  # Print the total number of connected devices
    
    # Get the integration time and pixel positions from the user
    integration_time = prompt_user_for_integration_time()
    selected_pixels  = prompt_user_for_pixel_selection()
    
    if device_ids:
        device_id = device_ids[0]  # Get the first device ID from the list
        sn = ""  # Variable to store the device's serial number
        try:
            print(f"Opening Device ID = {device_id}")  # Print the ID of the device being opened
            device = od.open_device(device_id)  # Open the device with the given ID
            sn = device.get_serial_number()  # Retrieve the serial number of the device

            try:
                device.set_scans_to_average(1)
                device.set_integration_time(integration_time)  # Set user-defined integration time
                print(f"Integration time set to {integration_time} microseconds.")  # Confirm the integration time

                print(f"[START] Reading spectra for dev S/N = {sn}", flush=True)  # Indicate the start of spectra reading
                for _ in range(10):  # Repeat the process 10 times
                    spectra = device.get_formatted_spectrum()  # Retrieve the formatted spectrum data
                    # Print intensity values at user-selected pixels
                    pixel_intensities = [spectra[pixel] for pixel in selected_pixels if pixel < len(spectra)]
                    print(f"Spectra @ selected pixels {selected_pixels}: {pixel_intensities}")

            except OceanDirectError as e:
                print(f"Error during device operation: {e.get_error_details()}")  # Print any errors encountered during the device operation

        except OceanDirectError as e:
            print(f"Error opening device: {e.get_error_details()}")  # Print any errors encountered during device opening
        
        # Delete the device instance & shut down OceanDirect only after the user presses Enter
        print("**** Complete ****")  # Print exit message
        input("Press Enter to close...")  # Wait for user to press Enter
        
        try:
            del od
            print(f"Device {device_id} closed and OceanDirect was shutdown.")
        except Exception as e:
            print(f"Error during shutdown: {e}")

    else:
        print("No device found.")  # Print message if no devices are found

if __name__ == '__main__':
    main()  # Start the main function

