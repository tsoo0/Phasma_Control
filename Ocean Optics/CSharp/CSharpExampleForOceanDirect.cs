using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using NetOceanDirect;
using System.Threading;
using System.Runtime;
using System.IO;


/*
This code was tested with OceanDirect-3.1.1
*/
namespace OceanDirectTest {

    partial class Test {

        static void Main(string[] args) {
            //define this here
            int errorCode = 0; // errorCode returns as zero 

            // create object from dll
            var ocean = OceanDirect.getInstance(); // Create an object to call functions on
            Devices[] devices = ocean.findDevices(); // Find all attached Ocean Spectrometers

            if (devices.Length == 0) {
                Console.WriteLine("Did not find any spectrometers");
            }
            else {
                for(int i=0; i < devices.Length; i++) {
                    // We need a deviceID to use functions
                    int deviceID = devices[0].Id; // For this example, assume only 1 device index = 0;
                    Console.WriteLine("Device ID =  {0}", deviceID);

                    // Devices must be open to call other functions
                    ocean.openDevice(deviceID, ref errorCode);

                    //10001 and up are warning codes. Not a show stopper but needs to be handled in the code
                    //properly.
                    if (errorCode > 0 && errorCode < 10001) {
                        Console.WriteLine("Failed to open the spectrometer. The error code is: {0}", errorCode);
                        ocean.closeDevice(deviceID, ref errorCode);
                        continue;
                    }
                    else {
                        // This is a property of the spectrometer
                        string deviceName = devices[0].Name;
                        Console.WriteLine("Device name =  {0}", deviceName);

                        // This is how a function of the ocean object is called
                        string serialNumber = ocean.getSerialNumber(deviceID, ref errorCode);
                        if (errorCode > 0 && errorCode < 10001) {
                            Console.WriteLine("Could not read the device serial number with error code {0}.", errorCode);
                        }
                        else {
                            Console.WriteLine("Device serial number =  {0}", serialNumber);
                        }

                        //read total pixels
                        int numberOfPixels = ocean.getNumberOfPixels(deviceID, ref errorCode);
                        if (errorCode == 0) {
                            Console.WriteLine("Number of pixels =  " + numberOfPixels.ToString());
                        }
                        else {
                            Console.WriteLine("Could not read the number of pixels with error code {0}.", errorCode);
                        }

                        //read wavelengths
                        double[] wavelengths = ocean.getWavelengths(deviceID, ref errorCode);
                        if (errorCode > 0 && errorCode < 10001)  {
                            Console.WriteLine("Could not read the wavelengths with error code {0}.", errorCode);
                        }
                        else {
                            Console.WriteLine("1st wavelength value =  {0:F3} nm \n", wavelengths[0]);
                        }

                        //read counts
                        for (int x = 0; x < 4; x++) {
                            double[] spectrum = ocean.getSpectrum(deviceID, ref errorCode);
                            if (errorCode != 0) {
                                Console.WriteLine("Could not read the spectrum with error code {0}.", errorCode);
                            }
                            else {
                                //Console.WriteLine("The 1st spectra value =  " + currentSpectrum[0].ToString() + " counts");
                                Console.WriteLine("Pixel 100,120,150 wavelength =  {0:F3},\t{1:F3},\t{2:F3}", wavelengths[100], wavelengths[120], wavelengths[150]);
                                Console.WriteLine("Pixel 100,120,150 intensity  =  {0:F3},\t{1:F3},\t{2:F3} \n", spectrum[100], spectrum[120], spectrum[150]);
                            }
                        }
                    }

                    ocean.closeDevice(deviceID, ref errorCode);
                }

                ocean.shutDown();
                Console.WriteLine("**** end of program **** \n");
            }
        }

    } 
}
