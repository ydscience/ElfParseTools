#include<iostream>
#include"ElfGotAnalyse.h"
#include "string.h"
#include"ParseHeaderInfo.h"
#include"AddSection.h"
#define ELF32_R_SYM(x) ((x) >> 8)
#define ELF64_R_SYM(i) ((i) >> 32)

using namespace std;
 
vector<ProgramHeader_32> decorateProgramHeader_32(char *p_cRes, bool bIsPrint, ElFHeader_32 elfHeader_32);
vector<ProgramHeader_64> decorateProgramHeader_64(char *p_cRes, bool bIsPrint, ElFHeader_64 elfHeader_64);
void printProgramHeader();
vector<SectionHeader_32> decorateSectionHeader_32(char *p_cRes, bool bIsPrint, ElFHeader_32 elfHeader_32);
vector<SectionHeader_64> decorateSectionHeader_64(char *p_cRes, bool bIsPrint, ElFHeader_64 elfHeader_64);
void printSectionHeader();
void  getShstrtabOffset_32(char *p_cRes, char * p_cOptator,ElFHeader_32 elfHeader_32, vector<SectionHeader_32> elfHeaderVector_32
	,char *p_cMathName, char *p_cNewOffset);
void  getShstrtabOffset_64(char *p_cRes, char * p_cOptator,ElFHeader_64 elfHeader_64, vector<SectionHeader_64> elfHeaderVector_64
	,char *p_cMathName, char *p_cNewOffset);
ElfSym_32 parseSym_32(char * p_cRes, int offset);
ElfSym_64 parseSym_64(char * p_cRes, int offset);
ElfRel_32 parseRelplt_32(char * p_cRes, int offset);
ElfRel_64 parseRelplt_64(char * p_cRes, int offset);
void parseElfInfo(int argc, char **argv);
void parseMathFromDynstr_32(char *p_cDynStrOffset, int iLen, vector<ElfSym_32> elfSymVector_32);
void parseParameterLenThree(char * p_cparmeterone, char * p_cparametertwo);
void parseMathFromDynstr_64(char *p_cDynStrOffset, int iLen, vector<ElfSym_64> elfSymVector_64);
void printMathName(char *p_cDynStrOffset, int start);
void addSectionFunction(char *p_cParameter, char *p_cSectionName, int iSectionSize, char *p_cFilePath);
void hookMathOderbyOffset_32(char *p_cMathName, char *p_cNewOffset, char *p_cReadMathPath, char *p_cFilePath);
int  getSpecMathOffset(char *p_cMathName, char * p_cRes, int iDynStrIndex, int iDynSymIndex, vector<ElfSym_32> elfSymVector_32);
void hookSpeciMath(char *p_cRes, int iOffset, char * p_cNewOffset, int iType);
void copyHookProgam(char * p_cNewOffset, int iType, char*p_cRes, char *p_cNewFilePath);
int  iSymCount_32 = 0, iRelCount_32 = 0, iSymCount_64 = 0, iRelCount_64 = 0;
char * p_cGotName = ".got";
char * p_crelpltName = ".rel.plt";
char * p_cDynStrName = ".dynstr";
char * p_cDymSymName = ".dynsym";
char * P_cAtexit = "xa_atexit";
char * p_cElfHeaderPara = "-h";
char * p_cElfProgramHeaderPara = "-p";
char * p_cElfSectionHeaderPara = "-s";
char * p_cElfMathListPara = "-m";
char * p_cAddEldSectionPara = "-a";
char * p_cHookSpecifyMath = "-f";
int iMathIndex = 0, iMathSize = 0;
int main(int argc, char **argv)
{
	//从控制台获取参数
	parseElfInfo(argc, argv);
	return 0;
	
}

void parseElfInfo(int argc, char **argv)
{
	if (argc == 3)
	{
		parseParameterLenThree(argv[1],argv[2]);
	}
	else if (argc == 5)
	{
		if (argv[3] != NULL)
		{
			int iLen = atoi(argv[3]);
			addSectionFunction(argv[1], argv[2], iLen, argv[4]);
		}
		else
		{
			cout << "invaild parameter" << endl;
		}
	}
	else if (argc == 8)
	{
		if (strcmp(argv[1], p_cHookSpecifyMath) == 0 )
		{
			if (argv[2] != NULL && argv[3] != NULL && argv[4] != NULL && argv[5] != NULL
				&& argv[6] != NULL && argv[7] != NULL)
			{
				Utils utils;
				iMathIndex = strtoul(argv[4], NULL, 16);
				iMathSize =atoi(argv[5]);
				hookMathOderbyOffset_32(argv[2], argv[3], argv[6], argv[7]);
			}
			
		}
		else{
			cout << "invaild parameter" << endl;
		}
	}
	else{
		cout << "invaild parameter" << endl;
	}
	
}


void parseParameterLenThree(char * p_cparmeterone, char * p_cparametertwo)
{
	Utils utils;
	ParseHeaderInfo parseHeaderInfo;
	if (p_cparmeterone != NULL && p_cparametertwo != NULL)
	{
		char * p_cMessage = utils.readFile(p_cparametertwo);//使用内存映射读取文件
		int type = *(p_cMessage + 4);
		if (strcmp(p_cparmeterone, p_cElfHeaderPara) == 0)
		{
			//解析elf头文件
			if (type == 1){
				parseHeaderInfo.parseElfHeader_32(p_cMessage, true);
			}
			if (type == 2)
			{
				parseHeaderInfo.parseElfHeader_64(p_cMessage, true);
			}
			utils.closeMapping();
		}
		else if (strcmp(p_cparmeterone, p_cElfProgramHeaderPara) == 0)
		{
			// 解析elf程序头
			if (type == 1){
				ElFHeader_32 elfHeader_32 = parseHeaderInfo.parseElfHeader_32(p_cMessage, false);
				decorateProgramHeader_32(p_cMessage, true, elfHeader_32);
			}
			if (type == 2)
			{
				ElFHeader_64 elfHeader_64 = parseHeaderInfo.parseElfHeader_64(p_cMessage, false);
				decorateProgramHeader_64(p_cMessage, true, elfHeader_64);
			}
			utils.closeMapping();
		}
		else if (strcmp(p_cparmeterone, p_cElfSectionHeaderPara) == 0)
		{
			//解析elf节区头
			if (type == 1){
				ElFHeader_32 elfHeader_32 = parseHeaderInfo.parseElfHeader_32(p_cMessage, false);
				decorateProgramHeader_32(p_cMessage, false, elfHeader_32);
				decorateSectionHeader_32(p_cMessage, true, elfHeader_32);
			}
			if (type == 2)
			{
				ElFHeader_64 elfHeader_64 = parseHeaderInfo.parseElfHeader_64(p_cMessage, false);
				decorateProgramHeader_64(p_cMessage, false, elfHeader_64);
				decorateSectionHeader_64(p_cMessage, true, elfHeader_64);
			}
			utils.closeMapping();
		}
		else if (strcmp(p_cparmeterone, p_cElfMathListPara) == 0)
		{
			//解析函数列表
			if (type == 1){
				ElFHeader_32 elfHeader_32 = parseHeaderInfo.parseElfHeader_32(p_cMessage, false);
				decorateProgramHeader_32(p_cMessage, false, elfHeader_32);
				vector<SectionHeader_32> sectionHeaderVector_32 = decorateSectionHeader_32(p_cMessage, false, elfHeader_32);
				getShstrtabOffset_32(p_cMessage, p_cparmeterone, elfHeader_32, sectionHeaderVector_32,NULL,NULL);
			}
			if (type == 2)
			{
				ElFHeader_64 elfHeader_64 = parseHeaderInfo.parseElfHeader_64(p_cMessage, false);
				decorateProgramHeader_64(p_cMessage, false, elfHeader_64);
				vector<SectionHeader_64> sectionHeaderVector_64 = decorateSectionHeader_64(p_cMessage, false, elfHeader_64);
				getShstrtabOffset_64(p_cMessage, p_cparmeterone, elfHeader_64, sectionHeaderVector_64,NULL,NULL);
			}
			utils.closeMapping();
		}
		else{
			cout << "invaild parameter" << endl;
		}
	}
	else{
		cout << "invaild parameter" << endl;
	}
}

void addSectionFunction(char *p_cParameter, char *p_cSectionName, int iSectionSize, char *p_cFilePath)
{
	if (p_cParameter != NULL && p_cSectionName != NULL && iSectionSize > 0 && p_cFilePath != NULL)
	{
		Utils utils;
		char * p_cMessage = utils.readFile(p_cFilePath);//使用内存映射读取文件
		int type = *(p_cMessage + 4);
		
		if (strcmp(p_cParameter, p_cAddEldSectionPara) == 0)//增加Section节区
		{
			AddSection addSection;
			ParseHeaderInfo parseHeaderInfo;
			if (type == 1)
			{
				char * p_cAddSectionFile;
				ElFHeader_32 elfHeader_32 = parseHeaderInfo.parseElfHeader_32(p_cMessage, false);
				vector<ProgramHeader_32> programHeaderVector_32 = decorateProgramHeader_32(p_cMessage, false, elfHeader_32);
				vector<SectionHeader_32> sectionHeaderVector_32 = decorateSectionHeader_32(p_cMessage, false, elfHeader_32);
				addSection.iNewSectionSize = iSectionSize;
				addSection.p_cNewSectionName = p_cSectionName;
				addSection.iNewSectionNameLen = strlen(p_cSectionName);
				addSection.ireadSrcFileSize = utils.fileSize;
				addSection.p_cSrcFilePath = p_cFilePath;
				addSection.initParameter_32(elfHeader_32, programHeaderVector_32, sectionHeaderVector_32);
				p_cAddSectionFile = addSection.addSectionHeader(p_cMessage, type);//增加一个SectionHeader
				p_cAddSectionFile = addSection.adSectionForFileEnd(p_cAddSectionFile);//在文件末尾追加一个Section
				p_cAddSectionFile = addSection.chageStrTabLen(p_cAddSectionFile, type);//修改strtab的长度
				p_cAddSectionFile = addSection.changeElfHeaderSectionCount(p_cAddSectionFile, type);//修改ElfHeader中的program 长度
				addSection.changeProgramHeaderLoadInfo(p_cAddSectionFile, type);//修改Load的filesize和memsize
			}
			if (type == 2)
			{
				cout << "待添加";
			}
			utils.closeMapping();
		}
		else{
			cout << "invaild parameter" << endl;
		}
	}
}

void hookMathOderbyOffset_32(char *p_cMathName, char *p_cNewOffset, char *p_cReadMathPath, char *p_cFilePath)
{
	Utils utils; 
	ParseHeaderInfo parseHeaderInfo;
	char *p_cMessage = utils.readFile(p_cFilePath);
	if (p_cMessage != NULL){
		ElFHeader_32 elfHeader_32 = parseHeaderInfo.parseElfHeader_32(p_cMessage, false);
		decorateProgramHeader_32(p_cMessage, false, elfHeader_32);
		vector<SectionHeader_32> sectionHeaderVector_32 = decorateSectionHeader_32(p_cMessage, false, elfHeader_32);
		getShstrtabOffset_32(p_cMessage, p_cHookSpecifyMath, elfHeader_32, sectionHeaderVector_32, p_cMathName, p_cNewOffset);//获取dynsym、dynstr表的偏移地址
		copyHookProgam(p_cNewOffset, 1,p_cMessage,p_cReadMathPath);
	}
	else
	{
		cout << "解析错误" << endl;
	}
	utils.closeMapping();
}
int  getSpecMathOffset(char *p_cMathName,char * p_cRes,int iDynStrIndex, int iDynSymIndex, vector<ElfSym_32> elfSymVector_32)
{
	int iMathIndex = 0;
	Utils utils;
	for (unsigned int i = 0; i < elfSymVector_32.size(); i++)
	{
		int index = utils.charToInt(elfSymVector_32[i].st_name);
		if (strcmp(p_cRes+iDynStrIndex + index, p_cMathName) == 0)
		{
			iMathIndex = iDynSymIndex + i * 16 + 4;
			utils.printMathInfo(elfSymVector_32[i].st_value,4);
			return iMathIndex;
		}
	}
}

void hookSpeciMath(char *p_cRes, int iOffset, char * p_cNewOffset,int iType)
{
	if (iType == 1)
	{
		*(int *)(p_cRes + iOffset) = strtoul(p_cNewOffset, NULL, 16);
	}
	if (iType == 2)
	{
		*(INT64 *)(p_cRes + iOffset) = _strtoui64(p_cNewOffset, NULL, 16);
	}
	cout << "modify success" << endl;
}

void copyHookProgam(char * p_cNewOffset,int iType,char*p_cRes,char *p_cNewFilePath)
{
	Utils utils;
	char * p_cNewMessage = utils.readFile(p_cNewFilePath);
	int iNewMeaageLen = utils.fileSize;
	if (iType = 1)
	{
		int  iOffset = strtoul(p_cNewOffset, NULL, 16);
		memcpy_s(p_cRes + iOffset, iMathSize, p_cNewMessage + iMathIndex, iMathSize);
		FlushViewOfFile(p_cRes + iOffset, iMathSize);
	}
	if (iType == 2)
	{
		INT64 iOffset = _strtoui64(p_cNewOffset, NULL, 16);
		memcpy_s(p_cRes + iOffset, iMathSize, p_cNewMessage + iMathIndex, iMathSize);
		FlushViewOfFile(p_cRes + iOffset, iMathSize);
	}
	utils.closeMapping();
}

vector<ProgramHeader_32> decorateProgramHeader_32(char *p_cRes, bool bIsPrint, ElFHeader_32 elfHeader_32)
{
	Utils utils;
	vector<ProgramHeader_32> programHeaderVector_32;
	ParseHeaderInfo parseHeaderInfo;
	int nHeadersize = 32;
	int nHeaderCount = utils.charToShort(elfHeader_32.e_phnum);
	int offset = utils.charToInt(elfHeader_32.e_phoff);
	for (int i = 0; i < nHeaderCount; i++)
	{
		programHeaderVector_32.push_back(parseHeaderInfo.parseProgramHeader_32(p_cRes, i*nHeadersize + offset));
	}
	if (bIsPrint)
	{
		parseHeaderInfo.printProgramHeader_32(programHeaderVector_32,elfHeader_32);
	}
	return programHeaderVector_32;
}

vector<ProgramHeader_64> decorateProgramHeader_64(char *p_cRes, bool bIsPrint, ElFHeader_64 elfHeader_64)
{
	Utils utils;
	vector<ProgramHeader_64> programHeaderVector_64;
	ParseHeaderInfo parseHeaderInfo;
	int nHeadersize = 56;
	int nHeaderCount = utils.charToShort(elfHeader_64.e_phnum);
	int offset = utils.charToLong(elfHeader_64.e_phoff);
	for (int i = 0; i < nHeaderCount; i++)
	{
		programHeaderVector_64.push_back(parseHeaderInfo.parseProgramHeader_64(p_cRes, i*nHeadersize + offset));
	}
	if (bIsPrint)
	{
		parseHeaderInfo.printProgramHeader_64(programHeaderVector_64,elfHeader_64);
	}
	return programHeaderVector_64;
}

vector<SectionHeader_32> decorateSectionHeader_32(char *p_cRes, bool bIsPrint, ElFHeader_32 elfHeader_32)
{
	vector<SectionHeader_32> sectionHeaderVectcor_32;
	ParseHeaderInfo parseHeaderInfo;
	Utils utils;
	int nHeaderSize = 40;
	int nHeaerCount = utils.charToShort(elfHeader_32.e_shnum);
	int offset = utils.charToInt(elfHeader_32.e_shoff);
	for (int i = 0; i < nHeaerCount; i++)
	{
		sectionHeaderVectcor_32.push_back(parseHeaderInfo.parseSectionHeader_32(p_cRes, i*nHeaderSize + offset));
	}
	if (bIsPrint)
	{
		parseHeaderInfo.printSectionHeader_32(sectionHeaderVectcor_32, elfHeader_32);
	}
	return sectionHeaderVectcor_32;
}

vector<SectionHeader_64> decorateSectionHeader_64(char *p_cRes, bool bIsPrint, ElFHeader_64 elfHeader_64)
{
	vector<SectionHeader_64> sectionHeaderVectcor_64;
	ParseHeaderInfo parseHeaderInfo;
	Utils utils;
	int nHeaderSize = 64;
	int nHeaerCount = utils.charToShort(elfHeader_64.e_shnum);
	long offset = utils.charToLong(elfHeader_64.e_shoff);
	for (int i = 0; i < nHeaerCount; i++)
	{
		sectionHeaderVectcor_64.push_back(parseHeaderInfo.parseSectionHeader_64(p_cRes, i*nHeaderSize + offset));
	}
	if (bIsPrint)
	{
		parseHeaderInfo.printSectionHeader_64(sectionHeaderVectcor_64, elfHeader_64);
	}
	return sectionHeaderVectcor_64;
}


void  getShstrtabOffset_32(char *p_cRes, char * p_cOptator,ElFHeader_32 elfHeader_32,
	vector<SectionHeader_32> elfHeaderVector_32,char *p_cMathName,char *p_cNewOffset)
{
	vector<ElfSym_32> elfSymVector_32;
	vector<ElfRel_32> elfRelVector_32;
	Utils utils;
	int iLen = 0;
	int iGotOffset = 0, iGotSize = 0;
	char * p_cDynstr = NULL;
	char * p_cDynSym = NULL;
	char * p_cRelplt = NULL;
	int iDynSymOffset = 0;
	int iRelPltOffset = 0;
	int iDynStrLen = 0;
	int iDynStrOffset = 0;
	int iStrtabOffset = utils.charToInt(elfHeader_32.e_shoff) + utils.charToShort(elfHeader_32.e_shstrndx) *
		utils.charToShort(elfHeader_32.e_shentsize);//获取strtabSetcion的位置
	int index = utils.charToShort(elfHeader_32.e_shstrndx);
	iLen = utils.charToInt(elfHeaderVector_32[index].sh_size);//str的大小
	int iStrOffset = utils.charToInt(elfHeaderVector_32[index].sh_offset);
	char  *p_cStrArray = new char[iLen];
	memcpy_s(p_cStrArray, iLen, p_cRes + iStrOffset, iLen);//将字符串复制
	
	int count = utils.charToShort(elfHeader_32.e_shnum);
	for (int i = 0; i < count; i++)
	{
		char * p_cSubmitStr = p_cStrArray + utils.charToInt(elfHeaderVector_32[i].sh_name);
		if (strcmp(p_cGotName, p_cSubmitStr) == 0)//got表的位置
		{
			int n = i;
			iGotOffset = utils.charToInt(elfHeaderVector_32[i].sh_offset);
			iGotSize = utils.charToInt(elfHeaderVector_32[i].sh_size);
		}
		if (strcmp(p_cDynStrName, p_cSubmitStr) == 0)//synstr字符串的位置
		{
			iDynStrLen = utils.charToInt(elfHeaderVector_32[i].sh_size);
			iDynStrOffset = utils.charToInt(elfHeaderVector_32[i].sh_offset);
			p_cDynstr = new char[iDynStrLen];
			memcpy_s(p_cDynstr, iDynStrLen, p_cRes + iDynStrOffset, iDynStrLen);
		}
		if (strcmp(P_cAtexit, p_cSubmitStr) == 0)//特殊情况下的synstr字符串的位置
		{
			iDynStrLen = utils.charToInt(elfHeaderVector_32[i].sh_size);
			iDynStrOffset = utils.charToInt(elfHeaderVector_32[i].sh_offset);
			p_cDynstr = new char[iDynStrLen];
			memcpy_s(p_cDynstr, iDynStrLen, p_cRes + iDynStrOffset, iDynStrLen);
		}
		if (strcmp(p_cDymSymName, p_cSubmitStr) == 0)//符号表位置
		{
			int iDynSymLen = utils.charToInt(elfHeaderVector_32[i].sh_size);
			iDynSymOffset = utils.charToInt(elfHeaderVector_32[i].sh_offset);
			iSymCount_32 = iDynSymLen / sizeof(ElfSym_32);
			for (int i = 0; i < iSymCount_32; i++)
			{
				elfSymVector_32.push_back(parseSym_32(p_cRes, i * 16 + iDynSymOffset));
			}
		}
		if (strcmp(p_crelpltName, p_cSubmitStr) == 0)//重定位表位置
		{
			int iRelPltLen = utils.charToInt(elfHeaderVector_32[i].sh_size);
			iRelPltOffset = utils.charToInt(elfHeaderVector_32[i].sh_offset);
			iRelCount_32 = iRelPltLen / sizeof(ElfRel_32);
			for (int i = 0; i < iRelCount_32; i++)
			{
				elfRelVector_32.push_back(parseRelplt_32(p_cRes, i * 8 + iRelPltOffset));
			}
		}
	}
	if (strcmp(p_cOptator, "-m") == 0)
	{
		parseMathFromDynstr_32(p_cDynstr, iDynStrLen, elfSymVector_32);
	}
	if (strcmp(p_cOptator, p_cHookSpecifyMath) == 0)
	{
		int iOffset = getSpecMathOffset(p_cMathName, p_cRes ,iDynStrOffset, iDynSymOffset, elfSymVector_32);
		if (iOffset > 0)
		{
			hookSpeciMath(p_cRes, iOffset, p_cNewOffset, 1);
		}
		else
		{
			cout << "iOffset is null" << endl;
		}
	}
	
}

void  getShstrtabOffset_64(char *p_cRes, char * p_cOptator,ElFHeader_64 elfHeader_64,
	vector<SectionHeader_64> elfHeaderVector_64, char *p_cMathName, char *p_cNewOffset)
{
	vector<ElfSym_64> elfSymVector_64;
	vector<ElfRel_64> elfRelVector_64;
	Utils utils;
	int iLen = 0;
	int iGotOffset = 0, iGotSize = 0;
	char * p_cDynstr = NULL;
	char * p_cDynSym = NULL;
	char * p_cRelplt = NULL;
	int iDynSymOffset = 0;
	int iRelPltOffset = 0;
	int iDynStrLen = 0;
	int iStrtabOffset = utils.charToLong(elfHeader_64.e_shoff) + utils.charToShort(elfHeader_64.e_shstrndx) *
		utils.charToShort(elfHeader_64.e_shentsize);//获取strtabSetcion的位置
	int index = utils.charToShort(elfHeader_64.e_shstrndx);
	iLen = utils.charToLong(elfHeaderVector_64[index].sh_size);//str的大小
	int iStrOffset = utils.charToLong(elfHeaderVector_64[index].sh_offset);
	char  *p_cStrArray = new char[iLen];
	memcpy_s(p_cStrArray, iLen, p_cRes + iStrOffset, iLen);//将字符串复制

	int count = utils.charToShort(elfHeader_64.e_shnum);
	for (int i = 0; i < count; i++)
	{
		char * p_cSubmitStr = p_cStrArray + utils.charToInt(elfHeaderVector_64[i].sh_name);
		if (strcmp(p_cGotName, p_cSubmitStr) == 0)//got表的位置
		{
			int n = i;
			iGotOffset = utils.charToLong(elfHeaderVector_64[i].sh_offset);
			iGotSize = utils.charToLong(elfHeaderVector_64[i].sh_size);
		}
		if (strcmp(p_cDynStrName, p_cSubmitStr) == 0)//synstr字符串的位置
		{
			iDynStrLen = utils.charToLong(elfHeaderVector_64[i].sh_size);
			int iDynStrOffset = utils.charToLong(elfHeaderVector_64[i].sh_offset);
			p_cDynstr = new char[iDynStrLen];
			memcpy_s(p_cDynstr, iDynStrLen, p_cRes + iDynStrOffset, iDynStrLen);
		}
		if (strcmp(P_cAtexit, p_cSubmitStr) == 0)//特殊情况下的synstr字符串的位置
		{
			iDynStrLen = utils.charToInt(elfHeaderVector_64[i].sh_size);
			int iDynStrOffset = utils.charToInt(elfHeaderVector_64[i].sh_offset);
			p_cDynstr = new char[iDynStrLen];
			memcpy_s(p_cDynstr, iDynStrLen, p_cRes + iDynStrOffset, iDynStrLen);
		}
		if (strcmp(p_cDymSymName, p_cSubmitStr) == 0)//符号表位置
		{
			int iDynSymLen = utils.charToLong(elfHeaderVector_64[i].sh_size);
			iDynSymOffset = utils.charToLong(elfHeaderVector_64[i].sh_offset);
			iSymCount_64 = iDynSymLen / sizeof(ElfSym_64);
			for (int i = 0; i < iSymCount_64; i++)
			{
				elfSymVector_64.push_back(parseSym_64(p_cRes, i * 24 + iDynSymOffset));
			}
		}
		if (strcmp(p_crelpltName, p_cSubmitStr) == 0)//重定位表位置
		{
			int iRelPltLen = utils.charToLong(elfHeaderVector_64[i].sh_size);
			iRelPltOffset = utils.charToLong(elfHeaderVector_64[i].sh_offset);
			iRelCount_64 = iRelPltLen / sizeof(ElfRel_64);
			for (int i = 0; i < iRelCount_64; i++)
			{
				elfRelVector_64.push_back(parseRelplt_64(p_cRes, i * 16 + iRelPltOffset));
			}
		}
	}
	if (strcmp(p_cOptator, "-m") == 0)
	{
		parseMathFromDynstr_64(p_cDynstr, iDynStrLen, elfSymVector_64);
	}
	
}
ElfSym_32 parseSym_32(char * p_cRes, int offset)
{
	ElfSym_32 elf32Sym;
	memcpy_s(elf32Sym.st_name, 4, p_cRes + offset, 4);
	memcpy_s(elf32Sym.st_value, 4, p_cRes + offset + 4, 4);
	memcpy_s(elf32Sym.st_size, 4, p_cRes + offset + 8, 4);
	memcpy_s(elf32Sym.st_info, 1, p_cRes + offset + 12, 1);
	memcpy_s(elf32Sym.st_other, 1, p_cRes + offset + 13, 1);
	memcpy_s(elf32Sym.st_shndx, 2, p_cRes + offset + 14, 2);
	return elf32Sym;
}
ElfSym_64 parseSym_64(char * p_cRes, int offset)
{
	ElfSym_64 elf64Sym;
	memcpy_s(elf64Sym.st_name, 4, p_cRes + offset, 4);
	memcpy_s(elf64Sym.st_info, 1, p_cRes + offset + 4, 1);
	memcpy_s(elf64Sym.st_other, 1, p_cRes + offset + 5, 1);
	memcpy_s(elf64Sym.st_shndx, 2, p_cRes + offset + 6, 2);
	memcpy_s(elf64Sym.st_value, 8, p_cRes + offset + 8, 8);
	memcpy_s(elf64Sym.st_size, 8, p_cRes + offset + 16, 8);
	return elf64Sym;
}

ElfRel_32 parseRelplt_32(char * p_cRes, int offset)
{
	ElfRel_32 elf32Rel;
	memcpy_s(elf32Rel.r_offset, 4, p_cRes + offset, 4);
	memcpy_s(elf32Rel.r_info, 4, p_cRes + offset + 4, 4);
	return elf32Rel;
}

ElfRel_64 parseRelplt_64(char * p_cRes, int offset)
{
	ElfRel_64 elf64Rel;
	memcpy_s(elf64Rel.r_offset, 8, p_cRes + offset, 8);
	memcpy_s(elf64Rel.r_info, 8, p_cRes + offset + 8, 8);
	return elf64Rel;
}

void parseMathFromDynstr_32(char *p_cDynStrOffset, int iLen,vector<ElfSym_32> elfSymVector_32)
{
	Utils utils;
	for (unsigned int i = 0; i < elfSymVector_32.size(); i++)
	{
		int index = utils.charToInt(elfSymVector_32[i].st_name);
		printMathName(p_cDynStrOffset, index);
		if (index > 0){
			cout << "  偏移地址";
			utils.printHexInfo(elfSymVector_32[i].st_value, 4);
			cout << "\n";
		}	
	}
}

void parseMathFromDynstr_64(char *p_cDynStrOffset, int iLen, vector<ElfSym_64> elfSymVector_64)
{
	Utils utils;
	for (unsigned int i = 0; i < elfSymVector_64.size(); i++)
	{
		int index = utils.charToInt(elfSymVector_64[i].st_name);
		printMathName(p_cDynStrOffset, index);
		if (index > 0){
			cout << "  偏移地址";
			utils.printHexInfo(elfSymVector_64[i].st_value, 8);
			cout << "\n";
		}
	}
}


void printMathName(char *p_cDynStrOffset, int start)
{
	Utils utils;
	int iCount = 0;
	int iInitPos = start;
	while (*(p_cDynStrOffset + start) != '\0')
	{
		start++;
		iCount++;
	}
	utils.printMathInfo(p_cDynStrOffset + iInitPos, iCount);
}
