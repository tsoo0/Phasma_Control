#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include "api/OceanDirectAPI.h"

/*
This code was tested with OceanDirect-3.1.1
*/
using oceandirect::api::OceanDirectAPI;
using oceandirect::SpectrumWithMetadata;

int main() {
    // It all starts with the instance of the OceanDirect API
    OceanDirectAPI* ocean = OceanDirectAPI::getInstance();

    // Find any discoverable devices e.g. USB connected then get their device IDs
    int found = ocean->probeDevices();
    if (!found) {
        std::cout << "No spectrometers detected" << std::endl;
        return 0;
    }

    // Assume we are expecting only one device for this example
    long id[1] = { 0 };
    int idCount = ocean->getDeviceIDs(id, 1);
    int errorCode = 0;
    ocean->openDevice(id[0], &errorCode);

    //10001 and up are warning codes. Not a show stopper but needs to be handled in the code
    //properly.
    if (errorCode > 0 && errorCode < 10001) {
        std::cout << "Error " << errorCode << " in openDevice" << std::endl;
        return 0;
    }

    // Confirm the device type. Assume the type name is no longer than 32 characters in this example
    const unsigned int deviceTypeLength = 32;
    char deviceType[deviceTypeLength] = { 0 };
    int actualDeviceTypeLength = ocean->getDeviceType(id[0], &errorCode, deviceType, deviceTypeLength);
    if (errorCode > 0 && errorCode < 10001) {
        std::cout << "Error " << errorCode << " in getDeviceType" << std::endl;
    }
    else {
        std::cout << "Device type found is " << deviceType << std::endl;
    }

    // Get the maximum buffer size and the current buffer size
    unsigned long bufferCapacityMaximum = ocean->AdvancedControl()->DataBufferControl()->getBufferCapacityMaximum(id[0], &errorCode);
    if (errorCode > 0 && errorCode < 10001) {
        std::cout << "Error " << errorCode << " in getBufferCapacityMaximum" << std::endl;
    }
    else {
        std::cout << "Device maximum buffer capacity is " << bufferCapacityMaximum << std::endl;
    }
    unsigned long currentBufferCapacity = ocean->AdvancedControl()->DataBufferControl()->getBufferCapacity(id[0], &errorCode);
    if (errorCode > 0 && errorCode < 10001) {
        std::cout << "Error " << errorCode << " in getBufferCapacity" << std::endl;
    }
    else {
        std::cout << "Device current buffer capacity is " << currentBufferCapacity << std::endl;
    }

    // Use an integration time of 200ms
    unsigned long integrationTimeMicros = 200000;
    ocean->setIntegrationTimeMicros(id[0], &errorCode, integrationTimeMicros);
    if (errorCode > 0 && errorCode < 10001) {
        std::cout << "Error " << errorCode << " in setIntegrationTimeMicros" << std::endl;
    }

    // Determine the size of a spectrum to enable us to allocate sufficient buffer space for acquisitions
    int spectrumLength = ocean->getFormattedSpectrumLength(id[0], &errorCode);
    if (errorCode > 0 && errorCode < 10001) {
        std::cout << "Error " << errorCode << " in getFormattedSpectrumLength" << std::endl;
    }

    // Set up the buffer to receive spectra. This is a vector of 15 (in this example) vectors each big enough to hold one spectrum.
    // Check your spectrometer documentation for the maximum value this can be. For the HDX this is 15.
    const int bufferLength = 15;
    std::vector<std::vector<double>> spectra(bufferLength, std::vector<double>(spectrumLength, 0.0));
    std::vector<SpectrumWithMetadata> buffer(bufferLength);
    // pointers to the individual spectra are associated with each element of buffer
    for (size_t b = 0; b < bufferLength; ++b) {
        buffer[b].setSpectrum(spectra[b].data());
        buffer[b].setSpectrumSize(spectrumLength);
    }

    // For "software triggering" set buffer enabled before setting the number of back-to-back scans.
    ocean->AdvancedControl()->DataBufferControl()->setBufferEnabled(id[0], &errorCode, true);
    if (errorCode > 0 && errorCode < 10001) {
        std::cout << "Error " << errorCode << " in setBufferEnabled" << std::endl;
    }

    // add in a throwaway
    std::vector<double> b(spectrumLength);
    SpectrumWithMetadata m;
    m.setSpectrum(b.data());
    m.setSpectrumSize(spectrumLength);
    ocean->getRawSpectrumWithMetadata(id[0], &errorCode, &m, 1);

    // Set the number of back-to-back scans to be acquired
    const unsigned long numScans = 100;
    ocean->AdvancedControl()->BackToBackControl()->setNumberOfBackToBackScans(id[0], &errorCode, numScans);
    if (errorCode > 0 && errorCode < 10001) {
        std::cout << "Error " << errorCode << " in setNumberOfBackToBackScans" << std::endl;
    }

    // Let the buffer get some spectra
    unsigned long gotElements = 0;
    while ((gotElements = ocean->AdvancedControl()->DataBufferControl()->getNumberOfElements(id[0], &errorCode)) < 1) {
        if (errorCode > 0 && errorCode < 10001) {
            std::cout << "Error " << errorCode << " in getNumberOfElements" << std::endl;
        }
    }

    // Then do some acquisition. Each call to getRawSpectrumWithMetadata will fill the buffer with spectra
    // up to the maximum size of the buffer.
    for (int count = 0; count < 5; ++count) {
        int acquired = ocean->getRawSpectrumWithMetadata(id[0], &errorCode, buffer.data(), 15);

        std::cout << std::endl;
        std::cout << "Received " << acquired << " buffered spectra." << std::endl;
        std::cout << "Error code =  " << errorCode << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    // Disable the buffering then close the device and shut down
    ocean->AdvancedControl()->DataBufferControl()->setBufferEnabled(id[0], &errorCode, false);
    if (errorCode > 0 && errorCode < 10001) {
        std::cout << "Error " << errorCode << " in setBufferEnabled" << std::endl;
    }

    ocean->closeDevice(id[0], &errorCode);
    if (errorCode > 0 && errorCode < 10001) {
        std::cout << "Error " << errorCode << " in closeDevice" << std::endl;
    }
    OceanDirectAPI::shutdown();
}

