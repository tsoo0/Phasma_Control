#include <analysis.h>
#include <ansi_c.h>
#include <visa.h>
#include <formatio.h>
#include <stdlib.h>

void Rigol(int Rigol_visa_handle, int Rigol_RecordLength, float *timestep, double Chan1[], double Chan2[], double Chan3[], double Chan4[])
{
	int			i;
	unsigned int bytesRead;
	char		read_string[32];
	float 		dummy;
	
	// Estimated total size:
	size_t numPts       = Rigol_RecordLength;
	size_t avgDigits    = 15;           // max len per value
	size_t sepPerPt     = 1;            // comma separator
	size_t termOverhead = 2;            // "\r\n"
	size_t nullByte     = 1;

	size_t bufSize = numPts * (avgDigits + sepPerPt)
	               + termOverhead
	               + nullByte;

	char *buf = malloc(bufSize);
	if (!buf) { perror("malloc"); exit(1); }
	
	//Obtain the timestep for the Rigol
	viPrintf(Rigol_visa_handle, ":WAV:XINC?\n");
	viRead(Rigol_visa_handle, read_string, 32, &bytesRead);
	sscanf(read_string,"%f",&dummy);
	*timestep=dummy;
	
	// Configure for ASCII transfer for Channel 1:
	viPrintf(Rigol_visa_handle, ":WAV:SOUR CHAN1\n");
	viPrintf(Rigol_visa_handle, ":WAV:MODE RAW\n");     // or NORMAL
	viPrintf(Rigol_visa_handle, ":WAV:FORM ASC\n");
	viPrintf(Rigol_visa_handle, ":WAV:POIN %zu\n", Rigol_RecordLength);
	viPrintf(Rigol_visa_handle, ":WAV:DATA?\n");

	// Ensure data is null terminated and ead the data

	viRead(Rigol_visa_handle, buf, (unsigned int)(bufSize - 1), &bytesRead);
	buf[bytesRead] = '\0';

	float *realBuf = malloc(numPts * sizeof *realBuf);
	char *tok = strtok(buf, ", ");
	for (i = 0; i < numPts && tok; ++i) {
	    realBuf[i] = atof(tok);
	    tok = strtok(NULL, ", ");
		Chan1[i]=realBuf[i];
	}

	// Configure for ASCII transfer for Channel 2:
	viPrintf(Rigol_visa_handle, ":WAV:SOUR CHAN2\n");
	viPrintf(Rigol_visa_handle, ":WAV:MODE RAW\n");     // or NORMAL
	viPrintf(Rigol_visa_handle, ":WAV:FORM ASC\n");
	viPrintf(Rigol_visa_handle, ":WAV:POIN %zu\n", Rigol_RecordLength);
	viPrintf(Rigol_visa_handle, ":WAV:DATA?\n");

	// Ensure data is null terminated and ead the data
	viRead(Rigol_visa_handle, buf, (unsigned int)(bufSize - 1), &bytesRead);
	buf[bytesRead] = '\0';
	tok = strtok(buf, ", ");
	for (i = 0; i < numPts && tok; ++i) {
	    realBuf[i] = atof(tok);
	    tok = strtok(NULL, ", ");
		Chan2[i]=realBuf[i];
	}

	// Configure for ASCII transfer for Channel 3:
	viPrintf(Rigol_visa_handle, ":WAV:SOUR CHAN3\n");
	viPrintf(Rigol_visa_handle, ":WAV:MODE RAW\n");     // or NORMAL
	viPrintf(Rigol_visa_handle, ":WAV:FORM ASC\n");
	viPrintf(Rigol_visa_handle, ":WAV:POIN %zu\n", Rigol_RecordLength);
	viPrintf(Rigol_visa_handle, ":WAV:DATA?\n");

	// Ensure data is null terminated and ead the data
	viRead(Rigol_visa_handle, buf, (unsigned int)(bufSize - 1), &bytesRead);
	buf[bytesRead] = '\0';
	tok = strtok(buf, ", ");
	for (i = 0; i < numPts && tok; ++i) {
	    realBuf[i] = atof(tok);
	    tok = strtok(NULL, ", ");
		Chan3[i]=realBuf[i];
	}

	// Configure for ASCII transfer for Channel 4:
	viPrintf(Rigol_visa_handle, ":WAV:SOUR CHAN4\n");
	viPrintf(Rigol_visa_handle, ":WAV:MODE RAW\n");     // or NORMAL
	viPrintf(Rigol_visa_handle, ":WAV:FORM ASC\n");
	viPrintf(Rigol_visa_handle, ":WAV:POIN %zu\n", Rigol_RecordLength);
	viPrintf(Rigol_visa_handle, ":WAV:DATA?\n");

	// Ensure data is null terminated and ead the data
	viRead(Rigol_visa_handle, buf, (unsigned int)(bufSize - 1), &bytesRead);
	buf[bytesRead] = '\0';
	tok = strtok(buf, ", ");
	for (i = 0; i < numPts && tok; ++i) {
	    realBuf[i] = atof(tok);
	    tok = strtok(NULL, ", ");
		Chan4[i]=realBuf[i];
	}

	
	//Clear out the extra memory usage
	free(buf);
	free(realBuf);
							 
}
