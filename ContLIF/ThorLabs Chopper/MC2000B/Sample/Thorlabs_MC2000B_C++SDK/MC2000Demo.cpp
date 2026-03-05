#include "MC2000CommandLib.h"

#define BUFFER_SIZE 255

char* TestList();
int TestGetCommand();
int TestGetCommand();

int main()
{	
	char* port = TestList();
	Open("COM4", 115200, 3);
	free(port);
	char id[100] = { 0 };
	GetId(0, id);
	printf("%s\r\n", id);
	int ret = TestSetCommand();
	printf("set : %d\n", ret);
	return 0;
}

char* TestList()
{
	char sn[BUFFER_SIZE] = {0};
	fnUART_LIBRARY_list(sn, BUFFER_SIZE);
	const int n = 9;
	char* port = (char*)calloc(n+1, sizeof(char));
	int index = 0;
	while (index < n&&sn[index] != ',')
	{
		port[index] = sn[index];
		index++;
	}
	port[n] = 0;
	return port;
}

int TestGetCommand()
{
	const int hdl = 0;
	char id[100] = { 0 };
	int ret = GetId(hdl, id);
	return ret;
}

int TestSetCommand()
{
	const int hdl = 0;
	const int value = 40;
	int ret = SetHarmonicMultiplier(hdl, value);
	return ret;
}


