#include <utility.h>
#include "GlobalVariables.h"


int WaitForPython()
{
	while(ExecutableHasTerminated(PyHandle)==PyScriptNotFinished) {
		 //no choice but to wait if one Python script tries to run while another is still in progress
	}
	return 0;
}


int TryPython()
{	
	switch (ExecutableHasTerminated(PyHandle) )
	{
		case PyScriptFinished:	
			return 0; // Ready to go
		case PyScriptNotFinished:
			return 1; // Python's still occupied
	}
	return -1; // Something weird happened
}
