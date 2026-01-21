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
an anaconda folder and the DAQ code is on the C drive in a PHASMA_DAQ folder. The process is as 
follows:

Step 1: Create the Python code with a set of modules that will be called by the MVSC code.

Step 2: Create the MVSC C code (this code here) and make sure it calls the Python functions properly - use the template of
the existing code to get the initialization and Python paths correct.

Step 3: Open the x64 Native Tools Command Prompt for Visual Studio and run the following commands
	cd C:\\path to MVSC code (to change the directory to this path)
	cl /LD python_bridge.c /I"C:\ProgramData\anaconda3\include" /link /LIBPATH:"C:\ProgramData\anaconda3\libs" python312.lib /OUT:python_bridge.dll  (to compile and build the MVSC code)

Step 4: Move a copy of the python_bridge.dll to same location as the LabWindows executable 

Step 5: Create a python_bridge.h file with all the C function calls that were mapped to the Python modules

Step 5: Add the python_bridge.lib and python_bridge.h files to the LabWindows code

Step 6: Call the MVSC code from the LabWindows code using the MVSC C functions that have been created.


*******************************************************************************/

#include "C:\ProgramData\anaconda3\include\Python.h"
#include <windows.h>

// Export macro for Windows DLLs
#define EXPORT __declspec(dllexport)

EXPORT int WritePrimesToFile() {
    PyStatus result;
    
    PyStatus status;
    PyConfig config;
    PyConfig_InitIsolatedConfig(&config);  // No site module, safe for embedders

    config.module_search_paths_set = 1;  // Explicitly set sys.path
    PyWideStringList_Append(&config.module_search_paths, L"C:\\ProgramData\\anaconda3\\Lib");
    PyWideStringList_Append(&config.module_search_paths, L"C:\\ProgramData\\anaconda3\\DLLs");

    result = PyConfig_SetString(&config, &config.home, L"C:\\ProgramData\\anaconda3");
   
    status = Py_InitializeFromConfig(&config);
    if (PyStatus_Exception(status)) {
        Py_ExitStatusException(status);
    }

    // Add current directory to Python path
     PyRun_SimpleString("import sys\nsys.path.append(\".\")");
    
    //Import the system path
    PyRun_SimpleString(
        "import sys\n"
        "sys.path.append(r'D:\\TANSTAAFL\\To Do\\PHASMA DAQ V2025\\PHASMA 2025 DAQ\\d-TAC Instruments\\PHASMA Python Routines')\n"
    );

    
    // Import the Python module
    PyObject *pName = PyUnicode_DecodeFSDefault("PHASMA_Python_Routines");
    
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
        fprintf(stderr, "Failed to load \"PHASMA_Python_Routines\"\n");
    }

    // Finalize the Python Interpreter
    Py_Finalize();
   
}

EXPORT int Config_Arm(int num_samples,int shot_number) {
    PyStatus result;
    
    PyStatus status;
    PyConfig config;
    PyConfig_InitIsolatedConfig(&config);  // No site module, safe for embedders

    config.module_search_paths_set = 1;  // Explicitly set sys.path
    PyWideStringList_Append(&config.module_search_paths, L"C:\\ProgramData\\anaconda3\\Lib");
    PyWideStringList_Append(&config.module_search_paths, L"C:\\ProgramData\\anaconda3\\DLLs");

    result = PyConfig_SetString(&config, &config.home, L"C:\\ProgramData\\anaconda3");
   
    status = Py_InitializeFromConfig(&config);
    if (PyStatus_Exception(status)) {
        Py_ExitStatusException(status);
    }

    // Add current directory to Python path
     PyRun_SimpleString("import sys\nsys.path.append(\".\")");
    
    //Import the system path
    PyRun_SimpleString(
        "import sys\n"
        "sys.path.append(r'D:\\TANSTAAFL\\To Do\\PHASMA DAQ V2025\\PHASMA 2025 DAQ\\d-TAC Instruments\\PHASMA Python Routines')\n"
    );

    
    // Import the Python module
    PyObject *pName = PyUnicode_DecodeFSDefault("PHASMA_Python_Routines");
    
    if (!pName) {
        PyErr_Print();
        fprintf(stderr, "Failed to convert module name\n");
        return 1;
    }
    
    PyObject *pModule = PyImport_Import(pName);
    Py_DECREF(pName);

    if (pModule != NULL) {
        // Get the function from the module
 		PyObject *pFunc = PyObject_GetAttrString(pModule, "Config_Arm");
		
		//Build the arguments tuple
		PyObject *pArgs = PyTuple_New(2);
		PyTuple_SetItem(pArgs, 0, PyLong_FromLong(num_samples));
		PyTuple_SetItem(pArgs, 1, PyLong_FromLong(shot_number));

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
            fprintf(stderr, "Call to Config_Arm failed.\n");
    		// Handle the error
		}

        Py_XDECREF(pFunc);
        Py_DECREF(pModule);
    } else {
        PyErr_Print();
        fprintf(stderr, "Failed to load \"PHASMA_Python_Routines\"\n");
    }

    // Finalize the Python Interpreter
    Py_Finalize();
   
}

