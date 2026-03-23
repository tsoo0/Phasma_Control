#include <analysis.h>
#include <ansi_c.h>
#include <visa.h>
#include <formatio.h>
#include <stdlib.h>



#include <visa.h>
#include <ansi_c.h>
#include <utility.h>


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <visa.h>



#define MAX_SAMPLES 2000000

float Chan1_global[MAX_SAMPLES];
float Chan2_global[MAX_SAMPLES];
float Chan3_global[MAX_SAMPLES];
float Chan4_global[MAX_SAMPLES];


/* ================================
   Helper functions (BIG-ENDIAN FIX)
================================ */

float swapFloat(float val) {
    unsigned char *p = (unsigned char *)&val;
    float r;
    unsigned char *q = (unsigned char *)&r;
    q[0] = p[3];
    q[1] = p[2];
    q[2] = p[1];
    q[3] = p[0];
    return r;
}

int swapInt(int val) {
    unsigned char *p = (unsigned char *)&val;
    return (p[0] << 24) | (p[1] << 16) | (p[2] << 8) | p[3];
}

/* -----------------------------------------------
   Generic waveform reader
   Pass channel string e.g. "C1", "C2", "C3", "C4"
   vertical_gain and vertical_offset should be
   queried from scope before calling this function.
------------------------------------------------ */
float* ReadWaveformChannel(ViSession LC_VisaHandle, const char *channel, int *numSamples)
{
    ViUInt32 count, readCount;
    ViStatus result;
    char ch;
	char	send_string[32];
	char	read_string[32];
	char 	dummy_string[32];
	float 	dummy;	
	float 	vertical_gain;
	float 	vertical_offset;
	
	//Read the vertical gain
	sprintf (send_string,"VOLT_DIV?\r");
	result = viWrite (LC_VisaHandle, (ViConstBuf)send_string, (ViUInt32)strlen(send_string), &count);
	result = viRead(LC_VisaHandle, (ViConstBuf)read_string, 32, &count);

	//Strip out vertical gain from string
	sscanf(read_string,"%8s %f",dummy_string, &vertical_gain);

	//Read the vertical offset
	sprintf (send_string,"OFST?\r");
	result = viWrite (LC_VisaHandle, (ViConstBuf)send_string, (ViUInt32)strlen(send_string), &count);
	result = viRead(LC_VisaHandle, (ViConstBuf)read_string, 32, &count);

	//Strip out vertical offset from string
	sscanf(read_string,"%8s %f",dummy_string, &vertical_offset);

    if (!channel || !numSamples) return NULL;

    /* 1. Set format: 8-bit binary bytes */
    const char *fmtCmd = "COMM_FORMAT DEF9,BYTE,BIN\n";
    viWrite(LC_VisaHandle, (ViBuf)fmtCmd, (ViUInt32)strlen(fmtCmd), &count);
    viFlush(LC_VisaHandle, VI_READ_BUF_DISCARD);

    /* 2. Build and send waveform query */
    char wfCmd[32];
    snprintf(wfCmd, sizeof(wfCmd), "%s:WF? DAT1\n", channel);
    viWrite(LC_VisaHandle, (ViBuf)wfCmd, (ViUInt32)strlen(wfCmd), &count);

    /* 3. Flush read buffer BEFORE reading response
          to clear any stale bytes from previous transfers */
    viFlush(LC_VisaHandle, VI_READ_BUF_DISCARD);

    /* 4. Read bytes one at a time until we find the '#' header.
          Limit search to avoid infinite loop on bad response. */
    int foundHash = 0;
    int searchLimit = 256;
    while (!foundHash && searchLimit-- > 0) {
        result = viRead(LC_VisaHandle, (ViBuf)&ch, 1, &readCount);
        if (result < VI_SUCCESS || readCount != 1) {
            printf("Error or timeout waiting for '#' header\n");
            return NULL;
        }
        if (ch == '#') foundHash = 1;
    }
    if (!foundHash) {
        printf("Failed to find '#' header in response\n");
        return NULL;
    }

    /* 5. Read the single digit that says how many length digits follow */
    char nChar;
    result = viRead(LC_VisaHandle, (ViBuf)&nChar, 1, &readCount);
    if (result < VI_SUCCESS || readCount != 1) {
        printf("Failed to read numDigits byte\n");
        return NULL;
    }
    int numDigits = nChar - '0';
    if (numDigits < 1 || numDigits > 9) {
        printf("Invalid numDigits value: %d (raw byte: 0x%02X) — possible sync loss\n",
               numDigits, (unsigned char)nChar);
        return NULL;
    }

    /* 6. Read the length field */
    char lenStr[16] = {0};
    result = viRead(LC_VisaHandle, (ViBuf)lenStr, numDigits, &readCount);
    if (result < VI_SUCCESS || (int)readCount != numDigits) {
        printf("Failed to read length field\n");
        return NULL;
    }
    int dataLength = atoi(lenStr);
    if (dataLength <= 0) {
        printf("Invalid data length: %d\n", dataLength);
        return NULL;
    }

    /* 7. Allocate and read the raw waveform data */
    signed char *buffer = malloc(dataLength);
    if (!buffer) {
        printf("malloc failed for %d bytes\n", dataLength);
        return NULL;
    }

    int totalRead = 0;
    while (totalRead < dataLength) {
        result = viRead(LC_VisaHandle,
                        (ViBuf)(buffer + totalRead),
                        dataLength - totalRead,
                        &readCount);
        if (result < VI_SUCCESS || readCount == 0) {
            printf("viRead failed or returned 0 bytes (totalRead=%d, expected=%d)\n",
                   totalRead, dataLength);
            free(buffer);
            return NULL;
        }
        totalRead += (int)readCount;
    }

    /* 8. Each byte is one signed sample — convert to scaled float */
    *numSamples = dataLength;
    float *wave = malloc(*numSamples * sizeof(float));
    if (!wave) {
        free(buffer);
        return NULL;
    }

    for (int i = 0; i < *numSamples; i++) {
        /* buffer[i] is signed char: range -128 to +127 */
        wave[i] = (float)buffer[i] * vertical_gain*(10.0/256) + vertical_offset;
    }

    free(buffer);
    return wave;
}

/* -----------------------------------------------
   Main LeCroy acquisition function
------------------------------------------------ */
int Lecroy(ViSession visa_handle, int Lecroy_RecordLength, float *timestep,
           float Chan1[], float Chan2[], float Chan3[], float Chan4[])
{
    int 	numSamples = 0;
	int 	result;
	int 	count;
	char	send_string[32];
	char	read_string[32];
	char 	dummy_string[32];
	float 	dummy;	
 	float 	*waveData;
 	int 	copyCount=0;
	int 	i;

	//Read the timebase
	sprintf (send_string,"TDIV?\n");
	result = viWrite (visa_handle, (ViConstBuf)send_string, (ViUInt32)strlen(send_string), &count);
	result = viRead(visa_handle, (ViConstBuf)read_string, 32, &count);

	//Strip out timebase from string
	sscanf(read_string,"%4s %f",dummy_string, &dummy);
	*timestep=dummy;		//put answer in microseconds

    /* ----------------------------------------                                       
	Read data from channel 1                  *
	-----------------------------------------*/
 	waveData = ReadWaveformChannel(visa_handle,"C1", &numSamples);
    if (!waveData) {
        printf("Failed to read waveform from channel 1\n");
    } else {

   		/* Copy into output array, up to the caller's buffer size */
	    copyCount = numSamples;
	    if (copyCount > Lecroy_RecordLength) copyCount = Lecroy_RecordLength;

	    for (i = 0; i < copyCount; i++)
	        Chan1[i] = waveData[i];
	}
	
   /* ----------------------------------------                                       
	Read data from channel 2                  *
	-----------------------------------------*/
 	waveData = ReadWaveformChannel(visa_handle,"C2", &numSamples);
    if (!waveData) {
        printf("Failed to read waveform from channel 2\n");
    } else {

	    /* Copy into output array, up to the caller's buffer size */
	    copyCount = numSamples;
	    if (copyCount > Lecroy_RecordLength) copyCount = Lecroy_RecordLength;

	    for (i = 0; i < copyCount; i++)
	        Chan2[i] = waveData[i];
	}
	
   /* ----------------------------------------                                       
	Read data from channel 3                  *
	-----------------------------------------*/
 	waveData = ReadWaveformChannel(visa_handle,"C3", &numSamples);
    if (!waveData) {
        printf("Failed to read waveform from channel 3\n");
    } else {

	    /* Copy into output array, up to the caller's buffer size */
	    copyCount = numSamples;
	    if (copyCount > Lecroy_RecordLength) copyCount = Lecroy_RecordLength;

	    for (i = 0; i < copyCount; i++)
	        Chan3[i] = waveData[i];
	}
	
   /* ----------------------------------------                                       
	Read data from channel 4                  *
	-----------------------------------------*/
 	waveData = ReadWaveformChannel(visa_handle,"C4", &numSamples);
    if (!waveData) {
        printf("Failed to read waveform from channel 4\n");
    } else {

	    /* Copy into output array, up to the caller's buffer size */
	    copyCount = numSamples;
	    if (copyCount > Lecroy_RecordLength) copyCount = Lecroy_RecordLength;

	    for (i = 0; i < copyCount; i++)
	        Chan4[i] = waveData[i];
	}	


    free(waveData);
    return copyCount;
}


/*void Lecroy(ViSession Lecroy_visa_handle, int Lecroy_RecordLength, float *timestep, double Chan1[], double Chan2[], double Chan3[], double Chan4[])
{
	int				i;
	int 			result;
	int 			count;
	int 			headerDigits;
	int 			dataLength;
	int 			totalRead = 0;
	unsigned int 	bytesRead;

	char 			dummy_string[32];
	char 			header[16];
	char 			*buf = malloc(Lecroy_RecordLength*sizeof(double));
	float 			dummy;
	float 			Vertical_Scale;
	ViUInt32 writeCount, readCount;

	
	//Do some Visa settings
	viSetAttribute(Lecroy_visa_handle, VI_ATTR_TMO_VALUE, 20000);
	viSetAttribute(Lecroy_visa_handle, VI_ATTR_TERMCHAR_EN, VI_FALSE);

	if (!buf) { perror("malloc"); exit(1); }
	
	//Set up the parameters of the timeseries call to the Lecroy, send all data points
	sprintf (send_string,"WFSU SP,0,NP,0,FP,0\n");
	result = viWrite (Lecroy_visa_handle, (ViConstBuf)send_string, (ViUInt32)strlen(send_string), &count);

	//Read the timebase
	sprintf (send_string,"TDIV?\n");
	result = viWrite (Lecroy_visa_handle, (ViConstBuf)send_string, (ViUInt32)strlen(send_string), &count);
	result = viRead(Lecroy_visa_handle, (ViConstBuf)read_string, 32, &count);

	//Strip out timebase from string
	sscanf(read_string,"%4s %f",dummy_string, &dummy);
	*timestep=dummy;		//put answer in microseconds
		
	
	/* -------------------------------------------------- */
	/* Transfer data for channel 1    				      */
	/* -------------------------------------------------- */

// -----------------------------
    // Configure scope output format
    // -----------------------------
   // result=viWrite(Lecroy_visa_handle,(ViBuf)"COMM_FORMAT DEF9,WORD,BIN\n",27, &count);

 	/* OPTIONAL but recommended */
	//viFlush(Lecroy_visa_handle, VI_READ_BUF_DISCARD);
	
// -----------------------------
    // Request Channel 1 waveform
    // -----------------------------
    //result=viWrite(Lecroy_visa_handle, (ViBuf)"C3:WF? DAT1\n", 13, &count);


/* ================================
   Read initial chunk
================================ */

//char temp[4096];
//viRead(Lecroy_visa_handle, (ViBuf)temp, sizeof(temp), &readCount);

/* Find '#' */
//char *ptr = memchr(temp, '#', readCount);

/*if (!ptr)
{
    printf("ERROR: No # found\n");
    fwrite(temp, 1, readCount, stdout);
    return -1;
}

/* ================================
   Parse SCPI block header
================================ */

/*int digits = *(ptr + 1) - '0';

char lenStr[16] = {0};
memcpy(lenStr, ptr + 2, digits);

dataLength = atoi(lenStr);

printf("Data length: %d\n", dataLength);

/* Calculate header offset */
//int headerOffset = (ptr - temp) + 2 + digits;

/* ================================
   Allocate buffer
================================ */

/*char *buffer = malloc(dataLength);
if (!buffer)
{
    printf("Memory allocation failed\n");
    return -1;
}

/* Copy already received data safely */
/*int alreadyRead = readCount - headerOffset;
if (alreadyRead < 0) alreadyRead = 0;

memcpy(buffer, temp + headerOffset, alreadyRead);

totalRead = alreadyRead;

/* ================================
   Read remaining data
================================ */

/*while (totalRead < dataLength)
{
    viRead(Lecroy_visa_handle,
           (ViBuf)(buffer + totalRead),
           dataLength - totalRead,
           &readCount);

    totalRead += readCount;
}

/* ================================
   Parse WAVEDESC (BIG-ENDIAN FIX)
================================ */

//char *wavedesc = buffer;
/*
int descriptor_length = ReadInt32BE(wavedesc + 36);
int wave_array_bytes  = ReadInt32BE(wavedesc + 60);
float vertical_gain   = 5;
float vertical_offset = 0;

/* Safety checks */
/*if (descriptor_length <= 0 || descriptor_length > dataLength)
{
    printf("Invalid descriptor_length: %d\n", descriptor_length);
    free(buffer);
    return -1;
}

if (wave_array_bytes <= 0 || wave_array_bytes > dataLength)
{
    printf("Invalid wave_array_bytes: %d\n", wave_array_bytes);
    free(buffer);
    return -1;
}

printf("Descriptor length: %d\n", descriptor_length);
printf("Wave bytes: %d\n", wave_array_bytes);
printf("Gain: %e Offset: %e\n", vertical_gain, vertical_offset);

/* ================================
   Extract waveform data
================================ */

/*char *dataStart = buffer + descriptor_length;

int numSamples = wave_array_bytes / 2;

/* Safer cast */
/*short *waveRaw = (short*)(void*)dataStart;

/* Allocate output */
/*float *wave = malloc(numSamples * sizeof(float));
if (!wave)
{
    printf("Wave allocation failed\n");
    free(buffer);
    return -1;
}

/* ================================
   Convert to voltages
================================ */

/*for (i = 0; i < numSamples; i++)
{
    /* swap 16-bit (data is big-endian) */
 /*   short val = (waveRaw[i] >> 8) | (waveRaw[i] << 8);

    wave[i] = (val * vertical_gain) - vertical_offset;

    /* your array */
    //Chan1[i] = wave[i];
//}

/* ================================
   Debug output
================================ */

/*printf("First 10 samples:\n");
for (i = 0; i < 10; i++)
{
    printf("%f\n", wave[i]);
}

/* ================================
   Cleanup
================================ */

/*free(buffer);
free(wave);

	/*
	
	// Configure for ASCII transfer for Channel 2:
	sprintf (send_string,"C2:WAVEFORM?\r");
	result = viWrite (Lecroy_visa_handle, (ViConstBuf)send_string, (ViUInt32)strlen(send_string), &count);
	result = viRead(Lecroy_visa_handle, (ViBuf)read_string, 32, &count);

	// Ensure data is null terminated and ead the data
	viRead(Lecroy_visa_handle, buf, (unsigned int)(bufSize - 1), &bytesRead);
	buf[bytesRead] = '\0';


	for (i = 0; i < numPts && tok; ++i) {
	    realBuf[i] = atof(tok);
	    tok = strtok(NULL, ", ");
		Chan2[i]=realBuf[i];
	}
	

	// Configure for ASCII transfer for Channel 3:
	sprintf (send_string,"C3:WAVEFORM?\r");
	result = viWrite (Lecroy_visa_handle, (ViConstBuf)send_string, (ViUInt32)strlen(send_string), &count);
	result = viRead(Lecroy_visa_handle, (ViConstBuf)read_string, 32, &count);

	// Ensure data is null terminated and ead the data
	viRead(Lecroy_visa_handle, buf, (unsigned int)(bufSize - 1), &bytesRead);
	buf[bytesRead] = '\0';

	for (i = 0; i < numPts && tok; ++i) {
	    realBuf[i] = atof(tok);
	    tok = strtok(NULL, ", ");
		Chan3[i]=realBuf[i];
	}

	// Configure for ASCII transfer for Channel 4:
	sprintf (send_string,"C4:WAVEFORM?\r");
	result = viWrite (Lecroy_visa_handle, (ViConstBuf)send_string, (ViUInt32)strlen(send_string), &count);
	result = viRead(Lecroy_visa_handle, (ViConstBuf)read_string, 32, &count);

	// Ensure data is null terminated and ead the data
	viRead(Lecroy_visa_handle, buf, (unsigned int)(bufSize - 1), &bytesRead);
	buf[bytesRead] = '\0';

	for (i = 0; i < numPts && tok; ++i) {
	    realBuf[i] = atof(tok);
	    tok = strtok(NULL, ", ");
		Chan4[i]=realBuf[i];
	}

	*/
	
	//Clear out the extra memory usage
	//free(buf);
	//free(wave);
	//free(waveRaw);
							 
//}
