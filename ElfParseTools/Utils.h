
#include <cstringt.h>
#include<windows.h>
#include <ctime>
#include<iostream>
#include<stdio.h>
class Utils
{
public:
	Utils();
	~Utils();

public:
	char * g_mapH;
	HANDLE g_mapFileH;
	HANDLE g_fileH;
	unsigned long fileSize;
	char * readFile(char * filePath);
	LPCTSTR tranfer(char * file);
	void closeMapping();
	void printStr(char * str, int len);
	unsigned long HextoDec(char *hex, int length);
	int charToInt(char * res);
	short charToShort(char *res);
	long charToLong(char *res);//LongÎª8×Ö½Ú
	void printProgramInfo(char * str, int len);
	void printMathInfo(char * p_cName, int iLen);
	void printHexInfo(char * str, int len);
	void saveNewFile(char * p_cFilePath, char * p_cSrc, int iNewFileSize);
};

