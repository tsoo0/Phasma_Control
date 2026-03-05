#include "api/OceanDirectAPI.h"
#include <iostream>
#include <vector>
#include <memory>

/*
This code was tested with OceanDirect-3.1.1
*/
int main() {
    std::cout << "OceanDirect SDK C++ Sample Program" << std::endl;

    // Start by initializing the SDK
    odapi_initialize();
    // Find any connected spectrometers
    int deviceCount = odapi_probe_devices();
    if (deviceCount < 1) {
        std::cout << "No connected spectrometers found" << std::endl;
    }
    else {
        // Retrieve the number of device IDs from the connected spectrometers.
        // The device ID will be used to address a specific spectrometer in later function calls
        int deviceIdCount = odapi_get_number_of_device_ids();
        // Declare a vector to hold the device IDs
        std::vector<long> deviceIds(deviceIdCount);
        // then get the device IDs
        int retrievedIdCount = odapi_get_device_ids(deviceIds.data(), deviceIdCount);
        // Errors in the following functions will be reported in an integer argument, zero indicates success
        int errorCode = 0;
        // Open the first device found to enable it to be used
        odapi_open_device(deviceIds[0], &errorCode);

        //10001 and up are warning codes. Not a show stopper but needs to be handled in the code
        //properly.
        if (errorCode > 0 && errorCode < 10001) {
            std::cout << "Failed to open the spectrometer. The error code is: " << errorCode << std::endl;
        }
        else {
            // Get the device name
            const int nameLength = 32;
            char deviceName[nameLength] = { 0 };
            odapi_get_device_name(deviceIds[0], &errorCode, deviceName, nameLength);
            if (errorCode > 0 && errorCode < 10001) {
                std::cout << "Failed to retrieve the spectrometer type. The error code is: " << errorCode << std::endl;
            }
            // and serial number
            int serialNumberLength = odapi_get_serial_number_maximum_length(deviceIds[0], &errorCode);
            std::unique_ptr<char> serialNumber(new char[serialNumberLength]);
            odapi_get_serial_number(deviceIds[0], &errorCode, serialNumber.get(), serialNumberLength);
            if (errorCode > 0 && errorCode < 10001) {
                std::cout << "Failed to retrieve the spectrometer serial number. The error code is: " << errorCode << std::endl;
            }
            else {
                std::cout << "The device is " << deviceName << ", serial number " << serialNumber.get() << std::endl;
            }

            // Find the number of pixels returned by the first spectrometer
            int pixelCount = odapi_get_formatted_spectrum_length(deviceIds[0], &errorCode);
            if (errorCode > 0 && errorCode < 10001) {
                std::cout << "Failed to get the spectrum length. The error code is: " << errorCode << std::endl;
            }
            else {
                // and then declare vectors to be used to retrieve the wavelengths and intensities
                std::vector<double> wavelengths(pixelCount);
                std::vector<double> spectrum(pixelCount);
                // get the wavelengths
                int wavelengthCount = odapi_get_wavelengths(deviceIds[0], &errorCode, wavelengths.data(), pixelCount);
                if (errorCode > 0 && errorCode < 10001) {
                    std::cout << "Failed to retrieve the wavelengths. Error code is: " << errorCode << std::endl;
                }

                // Now set the integration time to 200ms. The time must be specified in microseconds.
                unsigned long integrationTimeMicroseconds = 200000;
                odapi_set_integration_time_micros(deviceIds[0], &errorCode, integrationTimeMicroseconds);
                if (errorCode != 0) {
                    std::cout << "Failed to set the integration time. Error code is: " << errorCode << std::endl;
                }

                for (int i = 0; i < 4; i++) {
                    int intensityCount = odapi_get_formatted_spectrum(deviceIds[0], &errorCode, spectrum.data(), pixelCount);
                    if (errorCode != 0) {
                        std::cout << "Failed to retrieve the spectrum. Error code is: " << errorCode << std::endl;
                    }
                    std::cout << "Pixel 100,120,150 wavelength =  " << wavelengths[100] << "\t" << wavelengths[120] << "\t" << wavelengths[150] << std::endl;
                    std::cout << "Pixel 100,120,150 intensity  =  " << spectrum[100] << "\t" << spectrum[120] << "\t" << spectrum[150] << std::endl << std::endl;
                }
            }
            odapi_close_device(deviceIds[0], &errorCode);
        }
    }
    // Finally clean up resources claimed by the SDK
    odapi_shutdown();

    std::cout << "**** end of program ****" << std::endl;
}

