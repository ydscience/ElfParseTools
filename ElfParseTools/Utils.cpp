#include "Utils.h"


Utils::Utils()
{
}


Utils::~Utils()
{
}

LPCTSTR Utils::tranfer(char * file)
{
	int num = MultiByteToWideChar(0, 0, file, -1, NULL, 0);
	wchar_t *wide = new wchar_t[num];
	MultiByteToWideChar(0, 0, file, -1, wide, num);
	return wide;
}


char* Utils::readFile(char * filePath)
{
	//------------------------------------------------------内存映射方法;
	//创建或打开文件内核对象;
	HANDLE fileH = CreateFile(tranfer(filePath),
		GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	g_fileH = fileH;
	if (fileH == INVALID_HANDLE_VALUE)
	{
		std::cout << "error in CreateFile" << std::endl;
		return 0;
	}
	DWORD dwFileSize = GetFileSize(fileH, NULL);
	fileSize = dwFileSize;
	//创建一个文件映射内核对象;
	HANDLE mapFileH = CreateFileMapping(fileH,
		NULL,
		PAGE_READWRITE,
		0,
		dwFileSize,
		NULL);
	g_mapFileH = mapFileH;
	if (mapFileH == NULL)
	{
		std::cout << "error in CreateFileMapping" <<std::endl;
		return 0;
	}

	//将文件数据映射到进程的地址空间;
	char * mapH = (char *)MapViewOfFile(mapFileH,
		FILE_MAP_ALL_ACCESS,
		0,
		0,
		0);
	g_mapH = mapH;
	if (mapH == NULL)
	{
		std::cout << "error in MapViewOfFile" << std::endl;
		return 0;
	}
	//读取数据;此时的数据已经存入到mapH中，此时读取和修改MpH指针，直接修改了文件，不需要再次读取和写入

	return mapH;
}

void Utils::closeMapping()
{
	UnmapViewOfFile(g_mapH);
	CloseHandle(g_mapFileH);
	CloseHandle(g_fileH);
}

void Utils::printStr(char * str, int len)
{
	if (str != NULL)
	{
		for (int i = 0; i < len; i++)
		{
			std::cout << std::hex << (unsigned int)(unsigned char)str[i] << "  ";
		}
	}
	std::cout << "\n";
}

void Utils::printProgramInfo(char * str, int len)
{
	if (str != NULL)
	{
		for (int i = 0; i < len; i++)
		{
			std::cout << std::hex << (unsigned int)(unsigned char)str[i];
		}
	
	}
}
int Utils::charToInt(char * res)
{
	int targets = (res[0] & 0xff)
		| ((res[1] << 8) & 0xff00)
		| ((res[2] << 24) >> 8)
		| (res[3] << 24);
	return targets;
}

short Utils::charToShort(char *res)
{
	short s = 0;
	short s0 = (short)(res[0] & 0xff);// 最低位 
	short s1 = (short)(res[1] & 0xff);
	s1 <<= 8;
	s = (short)(s0 | s1);
	return s;
}

long Utils::charToLong(char *res)
{
	long s = 0;
	long s0 = res[0] & 0xff;// 最低位
	long s1 = res[1] & 0xff;
	long s2 = res[2] & 0xff;
	long s3 = res[3] & 0xff;
	long s4 = res[4] & 0xff;// 最低位
	long s5 = res[5] & 0xff;
	long s6 = res[6] & 0xff;
	long s7 = res[7] & 0xff;
	// s0不变
	s1 <<= 8;
	s2 <<= 16;
	s3 <<= 24;
	s4 <<= 8 * 4;
	s5 <<= 8 * 5;
	s6 <<= 8 * 6;
	s7 <<= 8 * 7;
	s = s0 | s1 | s2 | s3 | s4 | s5 | s6 | s7;
	return s;
}

void Utils::printMathInfo(char * p_cName, int iLen)
{
	for (int i = 0; i < iLen; ++i)
	{
		printf("%c", p_cName[i]);
	}
}
void Utils::printHexInfo(char * str, int len)
{
	if (str != NULL)
	{
		for (int i = 0; i < len; i++)
		{
			std::cout << std::hex << (unsigned int)(unsigned char)str[i] << " " ;
		}
	}
}

void Utils::saveNewFile(char * p_cFilePath, char * p_cSrc, int iNewFileSize)
{
	char arrayNewName[100];
	strcpy_s(arrayNewName, p_cFilePath);
	if (strchr(arrayNewName, '.'))
	{
		strcpy(strchr(arrayNewName, '.'), "_new.so");
	}
	else{
		strcat_s(arrayNewName, "_new");
	}
	FILE * newFile = fopen(arrayNewName, "wb");
	if (newFile == NULL)
	{
		std::cout << "save new file failed" <<std::endl;
		return;
	}
	fseek(newFile, 0, SEEK_SET);//移到文件开头
	fwrite(p_cSrc, sizeof(char), iNewFileSize, newFile);
	fclose(newFile);
	delete[] p_cSrc;
	std::cout << "add section success" << std::endl;
}


