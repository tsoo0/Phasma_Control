#pragma once
#include "stdafx.h"

#ifdef MC2000COMMANDLIB_EXPORTS 
#define MC2000COMMANDLIB_API extern "C" __declspec( dllexport)
#else
#define MC2000COMMANDLIB_API extern "C"  __declspec( dllimport)
#endif 


/// <summary>
/// list all the possible port on this computer.
/// </summary>
/// <param name="nPort">port list returned string include serial number and device descriptor, seperated by comma</param>
MC2000COMMANDLIB_API int List(char* sn);

/// <summary>
///  open port function.
/// </summary>
/// <param name="serialNo">serial number of the device to be opened, use GetPorts function to get exist list first.</param>
/// <param name="nBaud">bit per second of port</param>
/// <param name="timeout">set timeout value in (s)</param>
/// <returns> non-negative number: hdl number returned successfully; negative number: failed.</returns>
MC2000COMMANDLIB_API int Open(char* serialNo, int nBaud, int timeout);

/// <summary>
/// check opened status of port
/// </summary>
/// <param name="serialNo">serial number of the device to be checked.</param>
/// <returns> 0: port is not opened; 1: port is opened.</returns>
MC2000COMMANDLIB_API int IsOpen(char* serialNo);

/// <summary>
/// close current opened port
/// </summary>
/// <param name="hdl">handle of port.</param>
/// <returns> 0: success; negative number: failed.</returns>
MC2000COMMANDLIB_API int Close(int hdl);



/// <summary>
/// <p>read string from device through opened port.</p>
/// <p>make sure the port was opened successful before call this function.</p>
/// </summary>
/// <param name="hdl">handle of port.</param>
/// <param name="b">returned string buffer</param>
/// <param name="limit">
/// <p>ABS(limit): max length value of b buffer. </p>
/// <p>SIGN(limit) == 1 : wait RX event until time out value expired;</p>
/// <p>SIGN(limit) == -1: INFINITE wait event untill RX has data;</p>
/// </param>
/// <returns>non-negative number: size of actual read data in byte; negative number: failed.</returns>
MC2000COMMANDLIB_API int Read(int hdl, char *b, int limit);


/// <summary>
/// <p>write string to device through opened port.</p>
/// <p>make sure the port was opened successful before call this function.</p>
/// </summary>
/// <param name="hdl">handle of port.</param>
/// <param name="b">input string</param>
/// <param name="size">size of string to be written.</param>
/// <returns>non-negative number: number of bytes written; negative number: failed.</returns>
MC2000COMMANDLIB_API int Write(int hdl, char *b, int size);

/// <summary>
/// Get the model number and firmware version.
/// </summary>
/// <param name="hdl">handle of port.</param>
/// <param name="id">pointer to id string buffer.</param>
/// <returns>
/// <p>0: success; negative number: failed.</p>
/// <p>-1: invalid string buffer;</p>
/// <p>-2: time out;</p>
/// <p>-3: time out;</p>
/// <p>-4: CMD_NOT_DEFINED;</p>
///</returns>
MC2000COMMANDLIB_API int GetId(int hdl, char* id);


/// <summary>
/// <p>set command to device according to protocol in manual.</p>
/// <p>make sure the port was opened successful before call this function.</p>
/// <p>make sure this is the correct device by checking the ID string before call this function.</p>
/// </summary>
/// <param name="hdl">handle of port.</param>
/// <param name="c">input command string</param>
/// <param name="var">lenth of input command string (<255)</param>
/// <returns>
/// <p>0: success;</p>
/// <p>0xEA: CMD_NOT_DEFINED;</p>
/// <p>0xEB: time out;</p>
/// <p>0xED: invalid string buffer;</p>
/// </returns>
MC2000COMMANDLIB_API int Set(int hdl, char *c, int var);

/// <summary>
/// <p>set command to device according to protocol in manual and get the return string.</p>
/// <p>make sure the port was opened successful before call this function.</p>
/// <p>make sure this is the correct device by checking the ID string before call this function.</p>
/// </summary>
/// <param name="hdl">handle of port.</param>
/// <param name="c">input command string (<255)</param>
/// <param name="d">output string (<255)</param>
/// <returns>
/// <p>0: success;</p>
/// <p>0xEA: CMD_NOT_DEFINED;</p>
/// <p>0xEB: time out;</p>
/// <p>0xED: invalid string buffer;</p>
/// </returns>
MC2000COMMANDLIB_API int Get(int hdl, char *c, char *d);


/// <summary>
/// set time out value for read or write process.
/// </summary>
/// <param name="hdl">handle of port.</param>
/// <param name="time">time out value</param>
MC2000COMMANDLIB_API int SetTimeout(int hdl, int time);

/// <summary>
/// Purge the RX and TX buffer on port.
/// </summary>
/// <param name="hdl">handle of port.</param>
/// <param name="flag">
/// <p>FT_PURGE_RX: 0x01</p>
/// <p>FT_PURGE_TX: 0x02</p>
/// </param>
/// <returns> 0: success; negative number: failed.</returns>
MC2000COMMANDLIB_API int Purge(int hdl, int flag);


/// <summary>
/// Set the desired internal reference frequency.
/// </summary>
/// <param name="hdl">handle of port.</param>
/// <param name="id">desired internal reference frequency.</param>
/// <returns> negative number: failed.
/// <p>0: success; negative number: failed.</p>
/// <p>-1: invalid string buffer;</p>
/// <p>-2: time out;</p>
/// <p>-3: time out;</p>
/// <p>-4: CMD_NOT_DEFINED;</p>
/// </returns>
MC2000COMMANDLIB_API int SetFrequency(int hdl, int frequency);

/// <summary>
/// Get the internal reference frequency
/// </summary>
/// <param name="hdl">handle of port.</param>
/// <returns> negative number: failed.
/// <p>0: success; negative number: failed.</p>
/// <p>-1: invalid string buffer;</p>
/// <p>-2: time out;</p>
/// <p>-3: time out;</p>
/// <p>-4: CMD_NOT_DEFINED;</p>
/// </returns>
MC2000COMMANDLIB_API int GetFrequency(int hdl, int* frequency);


/// <summary>
/// Set the blade type
/// </summary>
/// <param name="hdl">handle of port.</param>
/// <param name="type">desired blade type.</param>
/// <returns> negative number: failed.
/// <p>0: success; negative number: failed.</p>
/// <p>-1: invalid string buffer;</p>
/// <p>-2: time out;</p>
/// <p>-3: time out;</p>
/// <p>-4: CMD_NOT_DEFINED;</p>
/// </returns>
MC2000COMMANDLIB_API int SetBladeType(int hdl, int type);

/// <summary>
/// Get the blade type
/// </summary>
/// <param name="hdl">handle of port.</param>
/// <param name="type">pointer to the blade type.</param>
/// <returns> negative number: failed.
/// <p>0: success; negative number: failed.</p>
/// <p>-1: invalid string buffer;</p>
/// <p>-2: time out;</p>
/// <p>-3: time out;</p>
/// <p>-4: CMD_NOT_DEFINED;</p>
/// </returns>
MC2000COMMANDLIB_API int GetBladeType(int hdl, int* type);


/// <summary>
/// Set Harmonic Multiplier applied to external reference frequency
/// </summary>
/// <param name="hdl">handle of port.</param>
/// <param name="nharmonic">desired harmonic multiplier.</param>
/// <returns> negative number: failed.
/// <p>0: success; negative number: failed.</p>
/// <p>-1: invalid string buffer;</p>
/// <p>-2: time out;</p>
/// <p>-3: time out;</p>
/// <p>-4: CMD_NOT_DEFINED;</p>
/// </returns>
MC2000COMMANDLIB_API int SetHarmonicMultiplier(int hdl, int nharmonic);


/// <summary>
/// Get Harmonic Multiplier applied to external reference frequency
/// </summary>
/// <param name="hdl">handle of port.</param>
/// <param name="nharmonic">pointer to the harmonic multiplier.</param>
/// <returns> negative number: failed.
/// <p>0: success; negative number: failed.</p>
/// <p>-1: invalid string buffer;</p>
/// <p>-2: time out;</p>
/// <p>-3: time out;</p>
/// <p>-4: CMD_NOT_DEFINED;</p>
/// </returns>
MC2000COMMANDLIB_API int GetHarmonicMultiplier(int hdl, int* nharmonic);

/// <summary>
/// Set the Harmonic Divider applied to external reference frequency
/// </summary>
/// <param name="hdl">handle of port.</param>
/// <param name="dharmonic">desired harmonic divider applied to external reference frequency.</param>
/// <returns> negative number: failed.
/// <p>0: success; negative number: failed.</p>
/// <p>-1: invalid string buffer;</p>
/// <p>-2: time out;</p>
/// <p>-3: time out;</p>
/// <p>-4: CMD_NOT_DEFINED;</p>
/// </returns>
MC2000COMMANDLIB_API int SetHarmonicDivider(int hdl, int dharmonic);

/// <summary>
/// Get the Harmonic Divider applied to external reference frequency
/// </summary>
/// <param name="hdl">handle of port.</param>
/// <param name="dharmonic">pointer to the harmonic divider applied to external reference frequency.</param>
/// <returns> negative number: failed.
/// <p>0: success; negative number: failed.</p>
/// <p>-1: invalid string buffer;</p>
/// <p>-2: time out;</p>
/// <p>-3: time out;</p>
/// <p>-4: CMD_NOT_DEFINED;</p>
/// </returns>
MC2000COMMANDLIB_API int GetHarmonicDivider(int hdl, int* dharmonic);


/// <summary>
/// Set the Phase adjust
/// </summary>
/// <param name="hdl">handle of port.</param>
/// <param name="phase">desired phase adjust.</param>
/// <returns> negative number: failed.
/// <p>0: success; negative number: failed.</p>
/// <p>-1: invalid string buffer;</p>
/// <p>-2: time out;</p>
/// <p>-3: time out;</p>
/// <p>-4: CMD_NOT_DEFINED;</p>
/// </returns>
MC2000COMMANDLIB_API int SetPhase(int hdl, int phase);


/// <summary>
/// Get the Phase adjust
/// </summary>
/// <param name="hdl">handle of port.</param>
/// <param name="phase">oointer to the phase adjust.</param>
/// <returns> negative number: failed.
/// <p>0: success; negative number: failed.</p>
/// <p>-1: invalid string buffer;</p>
/// <p>-2: time out;</p>
/// <p>-3: time out;</p>
/// <p>-4: CMD_NOT_DEFINED;</p>
/// </returns>
MC2000COMMANDLIB_API int GetPhase(int hdl, int* phase);

/// <summary>
/// Set enable or disable state of device
/// </summary>
/// <param name="hdl">handle of port.</param>
/// <param name="enable">enable state 1:enable, 0:disable.</param>
/// <returns> negative number: failed.
/// <p>0: success; negative number: failed.</p>
/// <p>-1: invalid string buffer;</p>
/// <p>-2: time out;</p>
/// <p>-3: time out;</p>
/// <p>-4: CMD_NOT_DEFINED;</p>
/// </returns>
MC2000COMMANDLIB_API int SetEnable(int hdl, int enable);


/// <summary>
/// Get enable or disable state of device
/// </summary>
/// <param name="hdl">handle of port.</param>
/// <param name="enable">pointer to enable state 1:enable, 0:disable.</param>
/// <returns> negative number: failed.
/// <p>0: success; negative number: failed.</p>
/// <p>-1: invalid string buffer;</p>
/// <p>-2: time out;</p>
/// <p>-3: time out;</p>
/// <p>-4: CMD_NOT_DEFINED;</p>
/// </returns>
MC2000COMMANDLIB_API int GetEnable(int hdl, int* enable);

/// <summary>
/// Set the reference mode
/// </summary>
/// <param name="hdl">handle of port.</param>
/// <param name="ref">reference mode 1:external, 0:internal.</param>
/// <returns> negative number: failed.
/// <p>0: success; negative number: failed.</p>
/// <p>-1: invalid string buffer;</p>
/// <p>-2: time out;</p>
/// <p>-3: time out;</p>
/// <p>-4: CMD_NOT_DEFINED;</p>
/// </returns>
MC2000COMMANDLIB_API int SetReference(int hdl, int ref);

/// <summary>
/// Get the reference mode
/// </summary>
/// <param name="hdl">handle of port.</param>
/// <param name="ref">pointer to reference mode 1:external, 0:internal.</param>
/// <returns> negative number: failed.
/// <p>0: success; negative number: failed.</p>
/// <p>-1: invalid string buffer;</p>
/// <p>-2: time out;</p>
/// <p>-3: time out;</p>
/// <p>-4: CMD_NOT_DEFINED;</p>
/// </returns>
MC2000COMMANDLIB_API int GetReference(int hdl, int* ref);


/// <summary>
/// Set the output reference mode
/// </summary>
/// <param name="hdl">handle of port.</param>
/// <param name="output">output mode 1:actual, 0:target.</param>
/// <returns> negative number: failed.
/// <p>0: success; negative number: failed.</p>
/// <p>-1: invalid string buffer;</p>
/// <p>-2: time out;</p>
/// <p>-3: time out;</p>
/// <p>-4: CMD_NOT_DEFINED;</p>
/// </returns>
MC2000COMMANDLIB_API int SetReferenceOutput(int hdl, int output);

/// <summary>
/// Get the output reference mode
/// </summary>
/// <param name="hdl">handle of port.</param>
/// <param name="output">pointer to the output mode 1:actual, 0:target.</param>
/// <returns> negative number: failed.
/// <p>0: success; negative number: failed.</p>
/// <p>-1: invalid string buffer;</p>
/// <p>-2: time out;</p>
/// <p>-3: time out;</p>
/// <p>-4: CMD_NOT_DEFINED;</p>
/// </returns>
MC2000COMMANDLIB_API int GetReferenceOutput(int hdl, int* output);


/// <summary>
/// Set the display intensity (1-10).
/// </summary>
/// <param name="hdl">handle of port.</param>
/// <param name="intensity">display intensity (1-10).</param>
/// <returns> negative number: failed.
/// <p>0: success; negative number: failed.</p>
/// <p>-1: invalid string buffer;</p>
/// <p>-2: time out;</p>
/// <p>-3: time out;</p>
/// <p>-4: CMD_NOT_DEFINED;</p>
/// </returns>
MC2000COMMANDLIB_API int SetDisplayIntensity(int hdl, int intensity);

/// <summary>
/// Get the display intensity (1-10).
/// </summary>
/// <param name="hdl">handle of port.</param>
/// <param name="intensity">pointer to display intensity (1-10).</param>
/// <returns> negative number: failed.
/// <p>0: success; negative number: failed.</p>
/// <p>-1: invalid string buffer;</p>
/// <p>-2: time out;</p>
/// <p>-3: time out;</p>
/// <p>-4: CMD_NOT_DEFINED;</p>
/// </returns>
MC2000COMMANDLIB_API int GetDisplayIntensity(int hdl, int* intensity);

/// <summary>
/// Get the current supplied external reference frequency.
/// </summary>
/// <param name="hdl">handle of port.</param>
/// <param name="frequence">pointer to the current supplied external reference frequency.</param>
/// <returns> negative number: failed.
/// <p>0: success; negative number: failed.</p>
/// <p>-1: invalid string buffer;</p>
/// <p>-2: time out;</p>
/// <p>-3: time out;</p>
/// <p>-4: CMD_NOT_DEFINED;</p>
/// </returns>
MC2000COMMANDLIB_API int GetReferenceFrequency(int hdl, int* frequence);

/// <summary>
/// Restore the factory default parameters.
/// </summary>
/// <param name="hdl">handle of port.</param>
/// <returns> negative number: failed.
/// <p>0: success; negative number: failed.</p>
/// <p>-1: invalid string buffer;</p>
/// <p>-2: time out;</p>
/// <p>-3: time out;</p>
/// <p>-4: CMD_NOT_DEFINED;</p>
/// </returns>
MC2000COMMANDLIB_API int Restore(int hdl);


/// <summary>
/// Get the verbose mode.
/// </summary>
/// <param name="hdl">handle of port.</param>
/// <param name="verbose">verbose mode</param>
/// <returns> negative number: failed.
/// <p>0: success; negative number: failed.</p>
/// <p>-1: invalid string buffer;</p>
/// <p>-2: time out;</p>
/// <p>-3: time out;</p>
/// <p>-4: CMD_NOT_DEFINED;</p>
/// </returns>
MC2000COMMANDLIB_API int GetVerbose(int hdl, int* verbose);


/// <summary>
/// Set the verbose mode.
/// </summary>
/// <param name="hdl">handle of port.</param>
/// <param name="verbose">verbose mode. 
///                       When verbose mode is set to 1, status messages are output on the USB</param>
/// <returns> negative number: failed.
/// <p>0: success; negative number: failed.</p>
/// <p>-1: invalid string buffer;</p>
/// <p>-2: time out;</p>
/// <p>-3: time out;</p>
/// <p>-4: CMD_NOT_DEFINED;</p>
/// </returns>
MC2000COMMANDLIB_API int SetVerbose(int hdl, int verbose);


/// <summary>
/// Set display language in the device.
/// </summary>
/// <param name="hdl">handle of port.</param>
/// <param name="lang">language mode. 1: English, 0: Chinese</param>
/// <returns> negative number: failed.
/// <p>0: success; negative number: failed.</p>
/// <p>-1: invalid string buffer;</p>
/// <p>-2: time out;</p>
/// <p>-3: time out;</p>
/// <p>-4: CMD_NOT_DEFINED;</p>
/// </returns>
MC2000COMMANDLIB_API int SetLanguage(int hdl, int lang);

/// <summary>
/// Get the lock state.
/// </summary>
/// <param name="hdl">handle of port.</param>
/// <param name="verbose">pointer to the lock state</param>
/// <returns> negative number: failed.
/// <p>0: success; negative number: failed.</p>
/// <p>-1: invalid string buffer;</p>
/// <p>-2: time out;</p>
/// <p>-3: time out;</p>
/// <p>-4: CMD_NOT_DEFINED;</p>
/// </returns>
MC2000COMMANDLIB_API int GetLocked(int hdl, int * lock);


/// <summary>
/// Get the cycle adjustment.
/// </summary>
/// <param name="hdl">handle of port.</param>
/// <param name="verbose">pointer to the cycle adjustment</param>
/// <returns> negative number: failed.
/// <p>0: success; negative number: failed.</p>
/// <p>-1: invalid string buffer;</p>
/// <p>-2: time out;</p>
/// <p>-3: time out;</p>
/// <p>-4: CMD_NOT_DEFINED;</p>
/// </returns>
MC2000COMMANDLIB_API int GetCycleAdjust(int hdl, int* cycle);

/// <summary>
/// Set the cycle adjustment.
/// </summary>
/// <param name="hdl">handle of port.</param>
/// <param name="verbose"> the desired cycle adjustment</param>
/// <returns> negative number: failed.
/// <p>0: success; negative number: failed.</p>
/// <p>-1: invalid string buffer;</p>
/// <p>-2: time out;</p>
/// <p>-3: time out;</p>
/// <p>-4: CMD_NOT_DEFINED;</p>
/// </returns>
MC2000COMMANDLIB_API int SetCycleAdjust(int hdl, int cycle);


/// <summary>
/// Get actual frequency of blade spinning
/// </summary>
/// <param name="hdl">handle of port.</param>
/// <param name="verbose">pointer to the actual frequency</param>
/// <returns> negative number: failed.
/// <p>0: success; negative number: failed.</p>
/// <p>-1: invalid string buffer;</p>
/// <p>-2: time out;</p>
/// <p>-3: time out;</p>
/// <p>-4: CMD_NOT_DEFINED;</p>
/// </returns>
MC2000COMMANDLIB_API int GetReferenceOutFrequency(int hdl, int* freq);