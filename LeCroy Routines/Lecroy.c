#include <analysis.h>
#include <ansi_c.h>
#include <visa.h>
#include <formatio.h>
#include <stdlib.h>

void Lecroy(int Lecroy_visa_handle, int Lecroy_RecordLength, float *timestep, double Chan1[], double Chan2[], double Chan3[], double Chan4[])
{
	int				i;
	unsigned int 	bytesRead;
	char			read_string[32];
	float 			dummy;
	
	// Estimated total size:
	size_t numPts       = Lecroy_RecordLength;
	size_t avgDigits    = 15;           // max len per value
	size_t sepPerPt     = 1;            // comma separator
	size_t termOverhead = 2;            // "\r\n"
	size_t nullByte     = 1;

	size_t bufSize = numPts * (avgDigits + sepPerPt)
	               + termOverhead
	               + nullByte;

	char *buf = malloc(bufSize);
	if (!buf) { perror("malloc"); exit(1); }
	
	//Set up the parameters of the timeseries call to the Lecroy, send all data points
	viPrintf(Lecroy_visa_handle, "WFSU SP,0,NP,0,FP,0\n");

		 
	//Obtain the timestep for the Lecroy
	viPrintf(Lecroy_visa_handle, "C1:INSP? 'TIMEBASE'\n");
	viRead(Lecroy_visa_handle, read_string, 32, &bytesRead);
	sscanf(read_string,"%f",&dummy);
	*timestep=dummy;
	
	// Configure for ASCII transfer for Channel 1:
	viPrintf(Lecroy_visa_handle, "C1:WAVEFORM?\n");

	// Ensure data is null terminated and ead the data
	viRead(Lecroy_visa_handle, buf, (unsigned int)(bufSize - 1), &bytesRead);
	buf[bytesRead] = '\0';

	float *realBuf = malloc(numPts * sizeof *realBuf);
	char *tok = strtok(buf, ", ");
	for (i = 0; i < numPts && tok; ++i) {
	    realBuf[i] = atof(tok);
	    tok = strtok(NULL, ", ");
		Chan1[i]=realBuf[i];
	}

	// Configure for ASCII transfer for Channel 2:
	viPrintf(Lecroy_visa_handle, "C2:WAVEFORM?\n");

	// Ensure data is null terminated and ead the data
	viRead(Lecroy_visa_handle, buf, (unsigned int)(bufSize - 1), &bytesRead);
	buf[bytesRead] = '\0';


	for (i = 0; i < numPts && tok; ++i) {
	    realBuf[i] = atof(tok);
	    tok = strtok(NULL, ", ");
		Chan2[i]=realBuf[i];
	}

	// Configure for ASCII transfer for Channel 3:
	viPrintf(Lecroy_visa_handle, "C3:WAVEFORM?\n");

	// Ensure data is null terminated and ead the data
	viRead(Lecroy_visa_handle, buf, (unsigned int)(bufSize - 1), &bytesRead);
	buf[bytesRead] = '\0';

	for (i = 0; i < numPts && tok; ++i) {
	    realBuf[i] = atof(tok);
	    tok = strtok(NULL, ", ");
		Chan3[i]=realBuf[i];
	}

	// Configure for ASCII transfer for Channel 4:
	viPrintf(Lecroy_visa_handle, "C4:WAVEFORM?\n");

	// Ensure data is null terminated and ead the data
	viRead(Lecroy_visa_handle, buf, (unsigned int)(bufSize - 1), &bytesRead);
	buf[bytesRead] = '\0';

	for (i = 0; i < numPts && tok; ++i) {
	    realBuf[i] = atof(tok);
	    tok = strtok(NULL, ", ");
		Chan4[i]=realBuf[i];
	}

	
	//Clear out the extra memory usage
	free(buf);
	free(realBuf);
							 
}
