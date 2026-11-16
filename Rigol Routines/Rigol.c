#include <analysis.h>
#include <ansi_c.h>
#include <visa.h>
#include <formatio.h>
#include <stdlib.h>




/*
void Rigol(ViSession Rigol_visa_handle, int Rigol_RecordLength, double *timestep, double Chan1[], double Chan2[], double Chan3[], double Chan4[])
{
	int			i;
	ViUInt32 	bytesRead;
	char		short_read_string[13];
	double 		dummy=0.0;
	
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
	viRead(Rigol_visa_handle, (ViPBuf)short_read_string, (ViUInt32)(sizeof(short_read_string) - 1), &bytesRead);
	sscanf(short_read_string,"%lf",&dummy);
	*timestep=dummy;						//Typecast double into a float
	
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

	// Ensure data is null terminated and read the data
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
*/

// Max points the scope returns per :WAV:DATA? in RAW mode with 16-bit (WORD) samples.
// (250000 bytes per read = 125000 points; check your model's programming guide.)
// Longer records are fetched in several chunks using :WAV:STAR / :WAV:STOP.
#define RIGOL_CHUNK_POINTS   125000
#define RIGOL_BYTES_PER_PT   2
#define RIGOL_TMO_MS         10000

// Read exactly n bytes (viRead may return fewer than requested).
static int ReadExact(ViSession h, unsigned char *dst, ViUInt32 n)
{
	ViUInt32 got = 0, r = 0;
	while (got < n) {
		ViStatus s = viRead(h, dst + got, n - got, &r);
		if (s < VI_SUCCESS || r == 0) return -1;
		got += r;
	}
	return 0;
}

// Parse an IEEE-488.2 definite-length block: "#" <N> <N digits of length> <data> <LF>
// Returns number of data bytes in *nData, or -1 on error.
static int ReadBlock(ViSession h, unsigned char *dst, ViUInt32 maxBytes, ViUInt32 *nData)
{
	unsigned char hdr[2];
	char          lenStr[16];
	int           nDigits;
	ViUInt32      len;
	unsigned char term;

	if (ReadExact(h, hdr, 2) || hdr[0] != '#') return -1;
	nDigits = hdr[1] - '0';
	if (nDigits < 1 || nDigits > 9) return -1;

	if (ReadExact(h, (unsigned char *)lenStr, nDigits)) return -1;
	lenStr[nDigits] = '\0';
	len = (ViUInt32)strtoul(lenStr, NULL, 10);
	if (len > maxBytes) return -1;

	if (ReadExact(h, dst, len)) return -1;
	ReadExact(h, &term, 1);              // trailing newline after the block
	*nData = len;
	return 0;
}

// Fetch one channel in 16-bit word format and convert to volts on the PC.
// Returns 0 on success.

// This could theoretically be sped up by a factor of two by switching from word format to byte format 
// since it's an 8-bit scope and the leading 8 bits of word are 0 anyway
static int FetchChannel(ViSession h, int chan, int numPts, double *volts, double *xinc)
{
	char           pre[512];
	double         xorig, xref, yinc, yorig, yref;
	int            fmt, type, pts, count;
	int            i, start, stop, n;
	ViUInt32       nData;
	unsigned char *raw;

	viPrintf(h, ":WAV:SOUR CHAN%d\n", chan);
	viPrintf(h, ":WAV:MODE RAW\n");
	viPrintf(h, ":WAV:FORM WORD\n");
	viPrintf(h, ":WAV:POIN %d\n", numPts);

	// Preamble: format,type,points,count,xinc,xorigin,xref,yinc,yorigin,yref
	// (read after selecting source and format - the scaling is per channel and per format)
	if (viQueryf(h, ":WAV:PRE?\n", "%t", pre) < VI_SUCCESS) return -1;
	if (sscanf(pre, "%d,%d,%d,%d,%lf,%lf,%lf,%lf,%lf,%lf",
	           &fmt, &type, &pts, &count, xinc, &xorig, &xref, &yinc, &yorig, &yref) != 10)
		return -2;
	if (fmt != 1) return -5;             // 0 = BYTE, 1 = WORD, 2 = ASC: scope did not accept WORD

	raw = malloc((size_t)RIGOL_CHUNK_POINTS * RIGOL_BYTES_PER_PT);
	if (!raw) return -3;

	// Binary data may contain 0x0A, so don't let a newline end the read.
	viSetAttribute(h, VI_ATTR_TERMCHAR_EN, VI_FALSE);

	for (start = 1; start <= numPts; start += RIGOL_CHUNK_POINTS) {      // STAR/STOP are 1-based
		stop = start + RIGOL_CHUNK_POINTS - 1;
		if (stop > numPts) stop = numPts;
		n = stop - start + 1;

		viPrintf(h, ":WAV:STAR %d\n", start);
		viPrintf(h, ":WAV:STOP %d\n", stop);
		viPrintf(h, ":WAV:DATA?\n");

		if (ReadBlock(h, raw, RIGOL_CHUNK_POINTS * RIGOL_BYTES_PER_PT, &nData)
		    || (int)nData < n * RIGOL_BYTES_PER_PT) {
			viSetAttribute(h, VI_ATTR_TERMCHAR_EN, VI_TRUE);
			free(raw);
			return -4;
		}

		// 16-bit little-endian ADC code (low byte first) -> volts
		for (i = 0; i < n; ++i) {
			unsigned int code = (unsigned int)raw[2 * i] | ((unsigned int)raw[2 * i + 1] << 8);
			volts[start - 1 + i] = ((double)code - yorig - yref) * yinc;
		}
	}

	viSetAttribute(h, VI_ATTR_TERMCHAR_EN, VI_TRUE);
	free(raw);
	return 0;
}

int Rigol(ViSession Rigol_visa_handle, int Rigol_RecordLength, double *timestep,
          double Chan1[], double Chan2[], double Chan3[], double Chan4[])
{
	ViUInt32 oldTmo;
	double   dt = 0.0;
	int      err = 0, opc = 0;

	viGetAttribute(Rigol_visa_handle, VI_ATTR_TMO_VALUE, &oldTmo);
	viSetAttribute(Rigol_visa_handle, VI_ATTR_TMO_VALUE, RIGOL_TMO_MS);

	// Stop acquisition so the full RAW memory is readable and stays consistent across channels.
	viPrintf(Rigol_visa_handle, ":STOP\n");
	viQueryf(Rigol_visa_handle, "*OPC?\n", "%d", &opc);     // wait until the scope has stopped

	if (!err) err = FetchChannel(Rigol_visa_handle, 1, Rigol_RecordLength, Chan1, &dt);
	if (!err) err = FetchChannel(Rigol_visa_handle, 2, Rigol_RecordLength, Chan2, &dt);
	if (!err) err = FetchChannel(Rigol_visa_handle, 3, Rigol_RecordLength, Chan3, &dt);
	if (!err) err = FetchChannel(Rigol_visa_handle, 4, Rigol_RecordLength, Chan4, &dt);

	*timestep = dt;      // seconds per sample; time of sample i = i * (*timestep)

	viSetAttribute(Rigol_visa_handle, VI_ATTR_TMO_VALUE, oldTmo);
	return err;          // 0 = OK
}
