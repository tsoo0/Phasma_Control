/*******************************************************************************
These sets of functions use a new hybrid implementation of C written in LabWindows,
C written in Microsoft Visual C (MSVC) and Python. The reason for this process is that
the C compiler in LabWindows is incompatible with the native C Python API - which assumes
that MVSC is the version of C that is being used. Therefore, to be able to call Python
commands from LabWindows, we first write C code in MSVC that calls the Python routines and
then export those C commands to LabWindows C by creating a header file, a .dll file, and
a library file. Then from LabWindows we can call these C functions that call the Python.
One of the challenges was setting up up the paths in the MVSC C code to properly call the
Python libraries and headers which are set up in Anaconda. Right now the implementation has 
very fixed paths that must be modified to match the paths on the machine being used. Eventually
it would be good to make these relative paths so that the code is more self-contained and portable.
To this end, we will assume for now that the primary Python installation is on the C drive in 
an anaconda folder and the DAQ code is on the C drive in a PHASMA_DAQ folder. Note, you cannot
run the debug version of Visual Studio code development because the python.d dubugging library
will not load. Note also that if the Visual Studio environment does not show a option for compiling
and running, go to the right hand side window, right click on the Project and "reload" it. The rest 
of the process is as follows:

Step 1: Create the Python code with a set of modules that will be called by the MVSC code. Here there
will be two sets of codes references by this C code. The first will be t_DAQ codes written by Tom Rood
for accessing the t_DAQ instruments. The second will be Titan motor codes written by Daniel Curtis. 
Both sets of codes will be accessed by this single C code.

Step 2: Create the MVSC C code (this code here) and make sure it calls the Python functions properly - use the template of
the existing code to get the initialization and Python paths correct. Very Important, you must add to this MSVC Project
in the "Linker-General-Additional Library Directories" path, the path to the Python312.lib (here "C:\ProgramData\anaconda3\libs")
Without this path, this C code will not be able to compile.

Step 3: Open the x64 Native Tools Command Prompt for Visual Studio and run the following commands
	C:
    cd C:\PHASMA 2025 DAQ\PHASMA Python Routines\PHASMA_C_to_Python_convertor_Project  (path to MVSC code to change the directory to this path)
	cl /LD PHASMA_C_to_Python_convertor.c /I"C:\ProgramData\anaconda3\include" /link /LIBPATH:"C:\ProgramData\anaconda3\libs" python312.lib /OUT:PHASMA_C_to_Python_convertor.dll  (to compile and build the MVSC code)

Step 4: Move a copy of the PHASMA_C_to_Python_convertor.dll to same location as the LabWindows executable 

Step 5: Create a PHASMA_C_to_Python_convertor.h file with all the C function calls that were mapped to the Python modules

Step 5: Add the PHASMA_C_to_Python_convertor.lib and PHASMA_C_to_Python_convertor.h files to the LabWindows code

Step 6: Call the MVSC code from the LabWindows code using the MVSC C functions that have been created.


*******************************************************************************/

#include "C:\ProgramData\anaconda3\include\Python.h"
#include <windows.h>
#include <stdio.h>

// Export macro for Windows DLLs
#define EXPORT __declspec(dllexport)


/*This first function is a simple test function to demonstrate the writing of prime numbers*/
EXPORT int WritePrimesToFile() {
    PyStatus result;
    PyStatus status;
    PyConfig config;

    //This command fixes the problem of problems loading numpy and other Python libraries. 
    //Without it, nothing will work
    SetDllDirectoryW(L"C:\\ProgramData\\anaconda3\\Library\\bin");
    
    PyConfig_InitIsolatedConfig(&config);  // No site module, safe for embedders

    config.module_search_paths_set = 1;  // Explicitly set sys.path
    PyWideStringList_Append(&config.module_search_paths, L"C:\\ProgramData\\anaconda3\\Lib");
    PyWideStringList_Append(&config.module_search_paths, L"C:\\ProgramData\\anaconda3\\DLLs");

    result = PyConfig_SetString(&config, &config.home, L"C:\\ProgramData\\anaconda3");
    //result = PyConfig_SetString(&config, &config.home, L"C:\\ProgramData\\anaconda3\\Lib");
    PyWideStringList_Append(&config.argv, L"");
 
    status = Py_InitializeFromConfig(&config);
    if (PyStatus_Exception(status)) {
        Py_ExitStatusException(status);
    }
 
    PyRun_SimpleString("import sys");
    PyRun_SimpleString("sys.path.append('C:/ProgramData/anaconda3/Lib/site-packages')");
    
    // Add current directory to Python path
    PyRun_SimpleString("import sys\nsys.path.append(\".\")");
    
    //Import the system path
    PyRun_SimpleString(
        "import sys\n"
        "sys.path.append(r'C:\\PHASMA 2025 DAQ\\PHASMA Python Routines\\PHASMA_C_to_Python_convertor_Project')\n"
    );
    
    // Import the Python module
    PyObject *pName = PyUnicode_DecodeFSDefault("t_DAQ_Python_Functions");
    
    if (!pName) {
        PyErr_Print();
        fprintf(stderr, "Failed to convert module name\n");
        return 1;
    }
    
    PyObject *pModule = PyImport_Import(pName);
    Py_DECREF(pName);

    if (pModule != NULL) {
        // Get the function from the module
        PyObject *pFunc = PyObject_GetAttrString(pModule, "write_primes");

        if (pFunc && PyCallable_Check(pFunc)) {
            PyObject *pResult = PyObject_CallObject(pFunc, NULL);
            if (pResult != NULL) {
                Py_DECREF(pResult);
                printf("Python function executed successfully.\n");
            } else {
                PyErr_Print();
                fprintf(stderr, "Call to write_primes() failed.\n");
            }
        } else {
            if (PyErr_Occurred()) PyErr_Print();
            fprintf(stderr, "Function write_primes() not found or not callable.\n");
        }

        Py_XDECREF(pFunc);
        Py_DECREF(pModule);
    } else {
        PyErr_Print();
        fprintf(stderr, "Failed to load \"t_DAQ_Python_Functions\"\n");
    }

    // Finalize the Python Interpreter
    Py_Finalize();
   
}

/* t-DAQ functions here  *************************************************************************/

EXPORT int t_DAQ_Config(char *ip_address, float sample_freq, int num_samples) {
    PyStatus result;
    PyStatus status;
    PyConfig config;

    //This command fixes the problem of problems loading numpy and other Python libraries. 
    //Without it, nothing will work
    SetDllDirectoryW(L"C:\\ProgramData\\anaconda3\\Library\\bin");

    PyConfig_InitIsolatedConfig(&config);  // No site module, safe for embedders

    config.module_search_paths_set = 1;  // Explicitly set sys.path
    PyWideStringList_Append(&config.module_search_paths, L"C:\\ProgramData\\anaconda3\\Lib");
    PyWideStringList_Append(&config.module_search_paths, L"C:\\ProgramData\\anaconda3\\DLLs");

    result = PyConfig_SetString(&config, &config.home, L"C:\\ProgramData\\anaconda3");
    //result = PyConfig_SetString(&config, &config.home, L"C:\\ProgramData\\anaconda3\\Lib");

    PyWideStringList_Append(&config.argv, L"");

    status = Py_InitializeFromConfig(&config);
    if (PyStatus_Exception(status)) {
        Py_ExitStatusException(status);
    }

    PyRun_SimpleString("import sys");
    PyRun_SimpleString("sys.path.append('C:/ProgramData/anaconda3/Lib/site-packages')");

    // Add current directory to Python path
    PyRun_SimpleString("import sys\nsys.path.append(\".\")");

    //Import the system path
    PyRun_SimpleString(
        "import sys\n"
        "sys.path.append(r'C:\\PHASMA 2025 DAQ\\PHASMA Python Routines\\PHASMA_C_to_Python_convertor_Project')\n"
    );

    // Import the Python module
    PyObject* pName = PyUnicode_DecodeFSDefault("t_DAQ_Python_Functions");

    if (!pName) {
        PyErr_Print();
        fprintf(stderr, "Failed to convert module name\n");
        return 1;
    }

    PyObject* pModule = PyImport_Import(pName);
    Py_DECREF(pName);

    if (pModule != NULL) {
        // Get the function from the module
 		PyObject *pFunc = PyObject_GetAttrString(pModule, "t_DAQ_Config_Master");
		
        PyObject* pArgs = PyTuple_New(3);
        PyTuple_SetItem(pArgs, 0, PyUnicode_FromString(ip_address));
        PyTuple_SetItem(pArgs, 1, PyFloat_FromDouble(sample_freq));
        PyTuple_SetItem(pArgs, 2, PyLong_FromLong(num_samples));
        
		//Call the function with the arguments		
		PyObject *pValue = PyObject_CallObject(pFunc, pArgs);
		Py_DECREF(pArgs);

		//Handle the return value and clean up:
		if (pValue != NULL) {
    		// Process the result
    		Py_DECREF(pValue);
            printf("Python function executed successfully.\n");
 		} else {
    		PyErr_Print();
            fprintf(stderr, "Call to t_DAQ_Config failed.\n");
    		// Handle the error
		}

        Py_XDECREF(pFunc);
        Py_DECREF(pModule);
    } else {
        PyErr_Print();
        fprintf(stderr, "Failed to load \"t_DAQ_Python_Functions.py\"\n");
    }

    // Finalize the Python Interpreter
    Py_Finalize();
   
}

EXPORT int  t_DAQ_Arm(char* ip_address) {
    PyStatus result;
    PyStatus status;
    PyConfig config;

    //This command fixes the problem of problems loading numpy and other Python libraries. 
    //Without it, nothing will work
    SetDllDirectoryW(L"C:\\ProgramData\\anaconda3\\Library\\bin");

    PyConfig_InitIsolatedConfig(&config);  // No site module, safe for embedders

    config.module_search_paths_set = 1;  // Explicitly set sys.path
    PyWideStringList_Append(&config.module_search_paths, L"C:\\ProgramData\\anaconda3\\Lib");
    PyWideStringList_Append(&config.module_search_paths, L"C:\\ProgramData\\anaconda3\\DLLs");

    result = PyConfig_SetString(&config, &config.home, L"C:\\ProgramData\\anaconda3");
    //result = PyConfig_SetString(&config, &config.home, L"C:\\ProgramData\\anaconda3\\Lib");

    PyWideStringList_Append(&config.argv, L"");

    status = Py_InitializeFromConfig(&config);
    if (PyStatus_Exception(status)) {
        Py_ExitStatusException(status);
    }

    PyRun_SimpleString("import sys");
    PyRun_SimpleString("sys.path.append('C:/ProgramData/anaconda3/Lib/site-packages')");

    // Add current directory to Python path
    PyRun_SimpleString("import sys\nsys.path.append(\".\")");

    //Import the system path
    PyRun_SimpleString(
        "import sys\n"
        "sys.path.append(r'D:\\TANSTAAFL\\To Do\\PHASMA DAQ V2025\\PHASMA 2025 DAQ\\PHASMA Python Routines\\PHASMA_C_to_Python_convertor_Project')\n"
    );

    // Import the Python module
    PyObject* pName = PyUnicode_DecodeFSDefault("t_DAQ_Python_Functions");

    if (!pName) {
        PyErr_Print();
        fprintf(stderr, "Failed to convert module name\n");
        return 1;
    }

    PyObject* pModule = PyImport_Import(pName);
    Py_DECREF(pName);

    if (pModule != NULL) {
        // Get the function from the module
        PyObject* pFunc = PyObject_GetAttrString(pModule, "t_DAQ_ARM");

        PyObject* pArgs = PyTuple_New(1);
        PyTuple_SetItem(pArgs, 0, PyUnicode_FromString(ip_address));

        //Call the function with the arguments		
        PyObject* pValue = PyObject_CallObject(pFunc, pArgs);
        Py_DECREF(pArgs);

        //Handle the return value and clean up:
        if (pValue != NULL) {
            // Process the result
            Py_DECREF(pValue);
            printf("Python function executed successfully.\n");
        }
        else {
            PyErr_Print();
            fprintf(stderr, "Call to Arm failed.\n");
            // Handle the error
        }

        Py_XDECREF(pFunc);
        Py_DECREF(pModule);
    }
    else {
        PyErr_Print();
        fprintf(stderr, "Failed to load \"t_DAQ_Python_Functions.py\"\n");
    }

    // Finalize the Python Interpreter
    Py_Finalize();

}



EXPORT int  t_DAQ_Data_Read(char *ip_address, char *Raw_Data_Path) {
    PyStatus result;
    PyStatus status;
    PyConfig config;

    //This command fixes the problem of problems loading numpy and other Python libraries. 
    //Without it, nothing will work
    SetDllDirectoryW(L"C:\\ProgramData\\anaconda3\\Library\\bin");

    PyConfig_InitIsolatedConfig(&config);  // No site module, safe for embedders

    config.module_search_paths_set = 1;  // Explicitly set sys.path
    PyWideStringList_Append(&config.module_search_paths, L"C:\\ProgramData\\anaconda3\\Lib");
    PyWideStringList_Append(&config.module_search_paths, L"C:\\ProgramData\\anaconda3\\DLLs");

    result = PyConfig_SetString(&config, &config.home, L"C:\\ProgramData\\anaconda3");
    //result = PyConfig_SetString(&config, &config.home, L"C:\\ProgramData\\anaconda3\\Lib");

    PyWideStringList_Append(&config.argv, L"");

    status = Py_InitializeFromConfig(&config);
    if (PyStatus_Exception(status)) {
        Py_ExitStatusException(status);
    }

    PyRun_SimpleString("import sys");
    PyRun_SimpleString("sys.path.append('C:/ProgramData/anaconda3/Lib/site-packages')");

    // Add current directory to Python path
    PyRun_SimpleString("import sys\nsys.path.append(\".\")");

    //Import the system path
    PyRun_SimpleString(
        "import sys\n"
        "sys.path.append(r'C:\\PHASMA 2025 DAQ\\PHASMA Python Routines\\PHASMA_C_to_Python_convertor_Project')\n"
    );

    // Import the Python module
    PyObject* pName = PyUnicode_DecodeFSDefault("t_DAQ_Python_Functions");

    if (!pName) {
        PyErr_Print();
        fprintf(stderr, "Failed to convert module name\n");
        return 1;
    }

    PyObject* pModule = PyImport_Import(pName);
    Py_DECREF(pName);

    if (pModule != NULL) {
        // Get the function from the module
        PyObject* pFunc = PyObject_GetAttrString(pModule, "t_DAQ_Config_Master");

        PyObject* pArgs = PyTuple_New(2);
        PyTuple_SetItem(pArgs, 0, PyUnicode_FromString(ip_address));
        PyTuple_SetItem(pArgs, 0, PyUnicode_FromString(Raw_Data_Path));

        //Call the function with the arguments		
        PyObject* pValue = PyObject_CallObject(pFunc, pArgs);
        Py_DECREF(pArgs);

        //Handle the return value and clean up:
        if (pValue != NULL) {
            // Process the result
            Py_DECREF(pValue);
            printf("Python function executed successfully.\n");
        }
        else {
            PyErr_Print();
            fprintf(stderr, "Call to Config_Arm failed.\n");
            // Handle the error
        }

        Py_XDECREF(pFunc);
        Py_DECREF(pModule);
    }
    else {
        PyErr_Print();
        fprintf(stderr, "Failed to load \"d_TAC_Python_Functions.py\"\n");
    }

    // Finalize the Python Interpreter
    Py_Finalize();

}

/* Probe moving functions here  *************************************************************************/

EXPORT int move_probe(char *ip_address, float move, float COUNTSPERMM, float insertRadius) {
    PyStatus result;
    PyStatus status;
    PyConfig config;

    //This command fixes the problem of problems loading numpy and other Python libraries. 
    //Without it, nothing will work
    SetDllDirectoryW(L"C:\\ProgramData\\anaconda3\\Library\\bin");

    PyConfig_InitIsolatedConfig(&config);  // No site module, safe for embedders

    config.module_search_paths_set = 1;  // Explicitly set sys.path
    PyWideStringList_Append(&config.module_search_paths, L"C:\\ProgramData\\anaconda3\\Lib");
    PyWideStringList_Append(&config.module_search_paths, L"C:\\ProgramData\\anaconda3\\DLLs");

    result = PyConfig_SetString(&config, &config.home, L"C:\\ProgramData\\anaconda3");
    //result = PyConfig_SetString(&config, &config.home, L"C:\\ProgramData\\anaconda3\\Lib");

    PyWideStringList_Append(&config.argv, L"");

    status = Py_InitializeFromConfig(&config);
    if (PyStatus_Exception(status)) {
        Py_ExitStatusException(status);
    }

    PyRun_SimpleString("import sys");
    PyRun_SimpleString("sys.path.append('C:/ProgramData/anaconda3/Lib/site-packages')");

    // Add current directory to Python path
    PyRun_SimpleString("import sys\nsys.path.append(\".\")");

    //Import the system path
    PyRun_SimpleString(
        "import sys\n"
        "sys.path.append(r'C:\\PHASMA 2025 DAQ\\PHASMA Python Routines\\PHASMA_C_to_Python_convertor_Project')\n"
    );

    // Import the Python module
    PyObject* pName = PyUnicode_DecodeFSDefault("ArcusC");

    if (!pName) {
        PyErr_Print();
        fprintf(stderr, "Failed to convert module name\n");
        return 1;
    }
    
	
    PyObject *pModule = PyImport_Import(pName);
    Py_DECREF(pName);

    if (pModule != NULL) {
        // Get the function from the module
        PyObject* pFunc = PyObject_GetAttrString(pModule, "move");

        //Build the arguments tuple
        PyObject* pArgs = PyTuple_New(4);
        PyTuple_SetItem(pArgs, 0, PyUnicode_FromString(ip_address));
        PyTuple_SetItem(pArgs, 1, PyFloat_FromDouble(move));
        PyTuple_SetItem(pArgs, 2, PyFloat_FromDouble(COUNTSPERMM));
       	PyTuple_SetItem(pArgs, 3, PyFloat_FromDouble(insertRadius));

        //Call the function with the arguments		
        PyObject* pValue = PyObject_CallObject(pFunc, pArgs);
        Py_DECREF(pArgs);

        //Handle the return value and clean up:
        if (pValue != NULL) {
            // Process the result
            Py_DECREF(pValue);
            printf("Python function executed successfully.\n");
        }
        else {
            PyErr_Print();
            fprintf(stderr, "Call to move failed.\n");
            // Handle the error
        }

        Py_XDECREF(pFunc);
        Py_DECREF(pModule);
    }
    else {
        PyErr_Print();
        fprintf(stderr, "Failed to load \"Arcus.py\"\n");
    }

    // Finalize the Python Interpreter
    Py_Finalize();

}


    EXPORT int home_probe(char *ip_address) {
    PyStatus result;
    PyStatus status;
    PyConfig config;

    //This command fixes the problem of problems loading numpy and other Python libraries. 
    //Without it, nothing will work
    SetDllDirectoryW(L"C:\\ProgramData\\anaconda3\\Library\\bin");

    PyConfig_InitIsolatedConfig(&config);  // No site module, safe for embedders

    config.module_search_paths_set = 1;  // Explicitly set sys.path
    PyWideStringList_Append(&config.module_search_paths, L"C:\\ProgramData\\anaconda3\\Lib");
    PyWideStringList_Append(&config.module_search_paths, L"C:\\ProgramData\\anaconda3\\DLLs");

    result = PyConfig_SetString(&config, &config.home, L"C:\\ProgramData\\anaconda3");
    //result = PyConfig_SetString(&config, &config.home, L"C:\\ProgramData\\anaconda3\\Lib");

    PyWideStringList_Append(&config.argv, L"");

    status = Py_InitializeFromConfig(&config);
    if (PyStatus_Exception(status)) {
        Py_ExitStatusException(status);
    }

    PyRun_SimpleString("import sys");
    PyRun_SimpleString("sys.path.append('C:/ProgramData/anaconda3/Lib/site-packages')");

    // Add current directory to Python path
    PyRun_SimpleString("import sys\nsys.path.append(\".\")");

    //Import the system path
    PyRun_SimpleString(
        "import sys\n"
        "sys.path.append(r'C:\\PHASMA 2025 DAQ\\PHASMA Python Routines\\PHASMA_C_to_Python_convertor_Project')\n"
    );

    // Import the Python module
    PyObject* pName = PyUnicode_DecodeFSDefault("ArcusC");

    if (!pName) {
        PyErr_Print();
        fprintf(stderr, "Failed to convert module name\n");
        return 1;
    }


    PyObject* pModule = PyImport_Import(pName);
    Py_DECREF(pName);

    if (pModule != NULL) {
        // Get the function from the module
        PyObject* pFunc = PyObject_GetAttrString(pModule, "home");

        //Build the arguments tuple
        PyObject* pArgs = PyTuple_New(1);
        PyTuple_SetItem(pArgs, 0, PyUnicode_FromString(ip_address));


        //Call the function with the arguments		
        PyObject* pValue = PyObject_CallObject(pFunc, pArgs);
        Py_DECREF(pArgs);

        //Handle the return value and clean up:
        if (pValue != NULL) {
            // Process the result
            Py_DECREF(pValue);
            printf("Python function executed successfully.\n");
        }
        else {
            PyErr_Print();
            fprintf(stderr, "Home failed.\n");
            // Handle the error
        }

        Py_XDECREF(pFunc);
        Py_DECREF(pModule);
    }
    else {
        PyErr_Print();
        fprintf(stderr, "Failed to load \"ArcusC.py\"\n");
    }

    // Finalize the Python Interpreter
    Py_Finalize();

}

    /* McPherson 207 control functions here  *************************************************************************/
    EXPORT int move_207(float wavelength) {
        PyStatus result;
        PyStatus status;
        PyConfig config;

        //This command fixes the problem of problems loading numpy and other Python libraries. 
        //Without it, nothing will work
        SetDllDirectoryW(L"C:\\ProgramData\\anaconda3\\Library\\bin");

        PyConfig_InitIsolatedConfig(&config);  // No site module, safe for embedders

        config.module_search_paths_set = 1;  // Explicitly set sys.path
        PyWideStringList_Append(&config.module_search_paths, L"C:\\ProgramData\\anaconda3\\Lib");
        PyWideStringList_Append(&config.module_search_paths, L"C:\\ProgramData\\anaconda3\\DLLs");

        result = PyConfig_SetString(&config, &config.home, L"C:\\ProgramData\\anaconda3");
        //result = PyConfig_SetString(&config, &config.home, L"C:\\ProgramData\\anaconda3\\Lib");

        PyWideStringList_Append(&config.argv, L"");

        status = Py_InitializeFromConfig(&config);
        if (PyStatus_Exception(status)) {
            Py_ExitStatusException(status);
        }

        PyRun_SimpleString("import sys");
        PyRun_SimpleString("sys.path.append('C:/ProgramData/anaconda3/Lib/site-packages')");

        // Add current directory to Python path
        PyRun_SimpleString("import sys\nsys.path.append(\".\")");

        //Import the system path
        PyRun_SimpleString(
            "import sys\n"
            "sys.path.append(r'C:\\PHASMA 2025 DAQ\\PHASMA Python Routines\\PHASMA_C_to_Python_convertor_Project')\n"
        );

        // Import the Python module
        PyObject* pName = PyUnicode_DecodeFSDefault("McPherson207controller");

        if (!pName) {
            PyErr_Print();
            fprintf(stderr, "Failed to convert module name\n");
            return 1;
        }


        PyObject* pModule = PyImport_Import(pName);
        Py_DECREF(pName);

        if (pModule != NULL) {
            // Get the function from the module
            PyObject* pFunc = PyObject_GetAttrString(pModule, "move");

            //Build the arguments tuple
            PyObject* pArgs = PyTuple_New(1);
            PyTuple_SetItem(pArgs, 0, PyFloat_FromDouble(wavelength));


            //Call the function with the arguments		
            PyObject* pValue = PyObject_CallObject(pFunc, pArgs);
            Py_DECREF(pArgs);

            //Handle the return value and clean up:
            if (pValue != NULL) {
                // Process the result
                Py_DECREF(pValue);
                printf("Python function executed successfully.\n");
            }
            else {
                PyErr_Print();
                fprintf(stderr, "Move\n");
                // Handle the error
            }

            Py_XDECREF(pFunc);
            Py_DECREF(pModule);
        }
        else {
            PyErr_Print();
            fprintf(stderr, "Failed to load \"McPherson207controller.py\"\n");
        }

        // Finalize the Python Interpreter
        Py_Finalize();

    }

    EXPORT int overwriteLocation_207(float wavelength) {
        PyStatus result;
        PyStatus status;
        PyConfig config;

        //This command fixes the problem of problems loading numpy and other Python libraries. 
        //Without it, nothing will work
        SetDllDirectoryW(L"C:\\ProgramData\\anaconda3\\Library\\bin");

        PyConfig_InitIsolatedConfig(&config);  // No site module, safe for embedders

        config.module_search_paths_set = 1;  // Explicitly set sys.path
        PyWideStringList_Append(&config.module_search_paths, L"C:\\ProgramData\\anaconda3\\Lib");
        PyWideStringList_Append(&config.module_search_paths, L"C:\\ProgramData\\anaconda3\\DLLs");

        result = PyConfig_SetString(&config, &config.home, L"C:\\ProgramData\\anaconda3");
        //result = PyConfig_SetString(&config, &config.home, L"C:\\ProgramData\\anaconda3\\Lib");

        PyWideStringList_Append(&config.argv, L"");

        status = Py_InitializeFromConfig(&config);
        if (PyStatus_Exception(status)) {
            Py_ExitStatusException(status);
        }

        PyRun_SimpleString("import sys");
        PyRun_SimpleString("sys.path.append('C:/ProgramData/anaconda3/Lib/site-packages')");

        // Add current directory to Python path
        PyRun_SimpleString("import sys\nsys.path.append(\".\")");
        
        //Import the system path
        PyRun_SimpleString(
            "import sys\n"
            "sys.path.append(r'C:\\PHASMA 2025 DAQ\\PHASMA Python Routines\\PHASMA_C_to_Python_convertor_Project')\n"
        );

        // Import the Python module
        PyObject* pName = PyUnicode_DecodeFSDefault("McPherson207controller");

        if (!pName) {
            PyErr_Print();
            fprintf(stderr, "Failed to convert module name\n");
            return 1;
        }


        PyObject* pModule = PyImport_Import(pName);
        Py_DECREF(pName);

        if (pModule != NULL) {
            // Get the function from the module
            PyObject* pFunc = PyObject_GetAttrString(pModule, "overwriteLocation");

            //Build the arguments tuple
            PyObject* pArgs = PyTuple_New(1);
            PyTuple_SetItem(pArgs, 0, PyFloat_FromDouble(wavelength));


            //Call the function with the arguments		
            PyObject* pValue = PyObject_CallObject(pFunc, pArgs);
            Py_DECREF(pArgs);

            //Handle the return value and clean up:
            if (pValue != NULL) {
                // Process the result
                Py_DECREF(pValue);
                printf("Python function executed successfully.\n");
            }
            else {
                PyErr_Print();
                fprintf(stderr, "overwriteLocation\n");
                // Handle the error
            }

            Py_XDECREF(pFunc);
            Py_DECREF(pModule);
        }
        else {
            PyErr_Print();
            fprintf(stderr, "Failed to load \"McPherson207controller.py\"\n");
        }

        // Finalize the Python Interpreter
        Py_Finalize();

    }

    /* McPherson 209 control functions here  *************************************************************************/
    EXPORT int move_209(float wavelength) {
        PyStatus result;
        PyStatus status;
        PyConfig config;

        //This command fixes the problem of problems loading numpy and other Python libraries. 
        //Without it, nothing will work
        SetDllDirectoryW(L"C:\\ProgramData\\anaconda3\\Library\\bin");

        PyConfig_InitIsolatedConfig(&config);  // No site module, safe for embedders

        config.module_search_paths_set = 1;  // Explicitly set sys.path
        PyWideStringList_Append(&config.module_search_paths, L"C:\\ProgramData\\anaconda3\\Lib");
        PyWideStringList_Append(&config.module_search_paths, L"C:\\ProgramData\\anaconda3\\DLLs");

        result = PyConfig_SetString(&config, &config.home, L"C:\\ProgramData\\anaconda3");
        //result = PyConfig_SetString(&config, &config.home, L"C:\\ProgramData\\anaconda3\\Lib");

        PyWideStringList_Append(&config.argv, L"");

        status = Py_InitializeFromConfig(&config);
        if (PyStatus_Exception(status)) {
            Py_ExitStatusException(status);
        }

        PyRun_SimpleString("import sys");
        PyRun_SimpleString("sys.path.append('C:/ProgramData/anaconda3/Lib/site-packages')");

        // Add current directory to Python path
        PyRun_SimpleString("import sys\nsys.path.append(\".\")");

        //Import the system path
        PyRun_SimpleString(
            "import sys\n"
            "sys.path.append(r'C:\\PHASMA 2025 DAQ\\PHASMA Python Routines\\PHASMA_C_to_Python_convertor_Project')\n"
        );

        // Import the Python module
        PyObject* pName = PyUnicode_DecodeFSDefault("McPherson209controller");

        if (!pName) {
            PyErr_Print();
            fprintf(stderr, "Failed to convert module name\n");
            return 1;
        }


        PyObject* pModule = PyImport_Import(pName);
        Py_DECREF(pName);

        if (pModule != NULL) {
            // Get the function from the module
            PyObject* pFunc = PyObject_GetAttrString(pModule, "move209");

            //Build the arguments tuple
            PyObject* pArgs = PyTuple_New(1);
            PyTuple_SetItem(pArgs, 0, PyFloat_FromDouble(wavelength));


            //Call the function with the arguments		
            PyObject* pValue = PyObject_CallObject(pFunc, pArgs);
            Py_DECREF(pArgs);

            //Handle the return value and clean up:
            if (pValue != NULL) {
                // Process the result
                Py_DECREF(pValue);
                printf("Python function executed successfully.\n");
            }
            else {
                PyErr_Print();
                fprintf(stderr, "Move209\n");
                // Handle the error
            }

            Py_XDECREF(pFunc);
            Py_DECREF(pModule);
        }
        else {
            PyErr_Print();
            fprintf(stderr, "Failed to load \"McPherson209controller\"\n");
        }

        // Finalize the Python Interpreter
        Py_Finalize();

    }

    EXPORT int overwriteLocation_209(float wavelength) {
        PyStatus result;
        PyStatus status;
        PyConfig config;

        //This command fixes the problem of problems loading numpy and other Python libraries. 
        //Without it, nothing will work
        SetDllDirectoryW(L"C:\\ProgramData\\anaconda3\\Library\\bin");

        PyConfig_InitIsolatedConfig(&config);  // No site module, safe for embedders

        config.module_search_paths_set = 1;  // Explicitly set sys.path
        PyWideStringList_Append(&config.module_search_paths, L"C:\\ProgramData\\anaconda3\\Lib");
        PyWideStringList_Append(&config.module_search_paths, L"C:\\ProgramData\\anaconda3\\DLLs");

        result = PyConfig_SetString(&config, &config.home, L"C:\\ProgramData\\anaconda3");
        //result = PyConfig_SetString(&config, &config.home, L"C:\\ProgramData\\anaconda3\\Lib");

        PyWideStringList_Append(&config.argv, L"");

        status = Py_InitializeFromConfig(&config);
        if (PyStatus_Exception(status)) {
            Py_ExitStatusException(status);
        }

        PyRun_SimpleString("import sys");
        PyRun_SimpleString("sys.path.append('C:/ProgramData/anaconda3/Lib/site-packages')");

        // Add current directory to Python path
        PyRun_SimpleString("import sys\nsys.path.append(\".\")");

        //Import the system path
        PyRun_SimpleString(
            "import sys\n"
            "sys.path.append(r'C:\\PHASMA 2025 DAQ\\PHASMA Python Routines\\PHASMA_C_to_Python_convertor_Project')\n"
        );

        // Import the Python module
        PyObject* pName = PyUnicode_DecodeFSDefault("McPherson209controller");

        if (!pName) {
            PyErr_Print();
            fprintf(stderr, "Failed to convert module name\n");
            return 1;
        }


        PyObject* pModule = PyImport_Import(pName);
        Py_DECREF(pName);

        if (pModule != NULL) {
            // Get the function from the module
            PyObject* pFunc = PyObject_GetAttrString(pModule, "overwriteLocation209");

            //Build the arguments tuple
            PyObject* pArgs = PyTuple_New(1);
            PyTuple_SetItem(pArgs, 0, PyFloat_FromDouble(wavelength));


            //Call the function with the arguments		
            PyObject* pValue = PyObject_CallObject(pFunc, pArgs);
            Py_DECREF(pArgs);

            //Handle the return value and clean up:
            if (pValue != NULL) {
                // Process the result
                Py_DECREF(pValue);
                printf("Python function executed successfully.\n");
            }
            else {
                PyErr_Print();
                fprintf(stderr, "overwriteLocation209\n");
                // Handle the error
            }

            Py_XDECREF(pFunc);
            Py_DECREF(pModule);
        }
        else {
            PyErr_Print();
            fprintf(stderr, "Failed to load \"McPherson209controller.py\"\n");
        }

        // Finalize the Python Interpreter
        Py_Finalize();

    }
/*
This code snippet is here to be able to test the primary routines during a compile.
It is commented out because when this program is being used to create a library of
routines to be called elsewhere, it is not needed
*/
void main (void)
{
    int a = 99;
    int b = 1;

    // Printing Variables and 
    // value of expressions
    printf("Sum of %d and %d is %d\n", a, b, a + b);

   // move_probe("162.108.1.1", 1000, 30000);
    WritePrimesToFile();
    move_209(465);
  
}