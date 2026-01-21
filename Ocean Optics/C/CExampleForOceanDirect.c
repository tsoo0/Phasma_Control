

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "api/OceanDirectAPI.h"

/*
This code was tested with OceanDirect-3.1.1
*/
void readSpectra(long devId, long integrationTimeUs, int spectraCount) {
    int errorCode      = 0;
    int spectrumLength = odapi_get_formatted_spectrum_length(devId, &errorCode);
    double* spectrum   = (double*)malloc(spectrumLength * sizeof(double));

    char serialNumber[40];
    odapi_get_serial_number(devId, &errorCode, serialNumber, 40);
    odapi_set_integration_time_micros(devId, &errorCode, integrationTimeUs);

    printf("serial#/error_code   =  %s / %d \n", serialNumber, errorCode);
    printf("spectrum length      =  %d \n", spectrumLength);
    printf("integration time(us) =  %d \n", odapi_get_integration_time_micros(devId, &errorCode) );

    for (int i = 0; i < spectraCount; i++) {
        int spectraCount = odapi_get_formatted_spectrum(devId, &errorCode, spectrum, spectrumLength);
        printf("spectraCount/error_code/sample =  %d / %d / %.3f, %.3f, %.3f, %.3f, %.3f \n",
               spectraCount, errorCode, spectrum[100], spectrum[101],
               spectrum[102], spectrum[103], spectrum[104] );
    }

    free(spectrum);
    printf("\n\n");
}



int main(int argc, char* argv[]) {
    // Call initialize before using OceanDirect
    odapi_initialize();
    // Probe for Ocean devices
    int foundDevices = odapi_probe_devices();

    if (foundDevices > 0) {
        //Start by getting the devies identifiers we will use in most function calls
        long* ids   = (long *) malloc(foundDevices * sizeof(long));
        int idCount = odapi_get_device_ids(ids, (unsigned int)foundDevices);

        for (int i = 0; i < idCount; i++) {
            // Open the device detected.
            int errorCode = 0;
            odapi_open_device(ids[i], &errorCode);

            //10001 and up are warning codes. Not a show stopper but needs to be handled in the code
            //properly.
            if (errorCode > 0 && errorCode < 10001) {
                printf(" *** error opening device..closing device now!");
                odapi_close_device(ids[i], &errorCode);
                continue;
            }

            //Get the serial number
            unsigned char serialMaxLength = odapi_get_serial_number_maximum_length(ids[i], &errorCode);
            char* serial     = (char*)malloc(serialMaxLength);
            int serialLength = odapi_get_serial_number(ids[i], &errorCode, serial, serialMaxLength);
            printf("Found spectrometer %s\n", serial);

            //Set the integration time to 100ms (100000 microsecs) and acquire a spectrum
            int spectrumLength = odapi_get_formatted_spectrum_length(ids[i], &errorCode);
            double* spectrum   = (double*)malloc(spectrumLength * sizeof(double));
            unsigned long integrationTime = 100000;
            odapi_set_integration_time_micros(ids[i], &errorCode, integrationTime);
            int acquired = odapi_get_formatted_spectrum(ids[i], &errorCode, spectrum, spectrumLength);

            //For example, find the maximum pixel intensity in the acquired spectrum
            double maxPixelIntensity = 0.0;
            int maxPixel = 0;
            for (int pixel = 0; pixel < spectrumLength; ++pixel) {
                if (spectrum[pixel] > maxPixelIntensity) {
                    maxPixelIntensity = spectrum[pixel];
                    maxPixel = pixel;
                }
            }

            printf("Maximum pixel is %d, withintensity %f \n\n", maxPixel, maxPixelIntensity);
			
			//read more spectra
            readSpectra(ids[i], 30000, 10);

            //Clean up by closing the device and calling the shutdown function to free resources in OceanDirect
            odapi_close_device(ids[i], &errorCode);
            free(serial);
            free(spectrum);
        }

        odapi_shutdown();

        // Lastly release the allocated memory
        free(ids);
    }
    else {
        printf("No spectrometers detected\n");
    }

    printf("**** end of program ***** \n");
}
