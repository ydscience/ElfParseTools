#include "AddSection.h"
#include"Utils.h"

AddSection::AddSection()
{
}


AddSection::~AddSection()
{
}

long AddSection::alignOffset(long addr, long align)//对其算法
{
	if (align > addr){
		return addr;
	}
	int offset = addr % align;
	return addr + (align - offset);
}

void  AddSection::initParameter_32(ElFHeader_32 elfHeader_32, vector<ProgramHeader_32> programHeaderVector_32, vector<SectionHeader_32> sectionHeaderVector_32)
{
	Utils utils;
	iSectionHeaderOffset = utils.charToInt(elfHeader_32.e_shoff);
	sStringSectionTableIndex = utils.charToShort(elfHeader_32.e_shstrndx);
	iStringSectionOffset = utils.charToInt(sectionHeaderVector_32[sStringSectionTableIndex].sh_offset);
	boolean bFlag = true;
	for (unsigned int i = 0; i < programHeaderVector_32.size(); i++)
	{
		if (utils.charToInt(programHeaderVector_32[i].p_type) == 1)
		{
			if (bFlag){
				iFristLoadIndex = i;
				bFlag = false;
			}
			else
			{
				iLastLoadIndex = i;
			}
		}
	}
	int iLastLoadVaddr = utils.charToInt(programHeaderVector_32[iLastLoadIndex].p_vaddr);
	int iLastMem = utils.charToInt(programHeaderVector_32[iLastLoadIndex].p_memsz);
	int iLastAlign = utils.charToInt(programHeaderVector_32[iLastLoadIndex].p_align);
	iAddSectionStartAddr = alignOffset(iLastLoadVaddr + iLastMem, iLastAlign);
}
void  AddSection::initParameter_64(ElFHeader_64 elfHeader_64, vector<ProgramHeader_64> programHeaderVector_64, vector<SectionHeader_64> sectionHeaderVector_64)
{
	Utils utils;
	iSectionHeaderOffset = utils.charToLong(elfHeader_64.e_shoff);
	sStringSectionTableIndex = utils.charToShort(elfHeader_64.e_shstrndx);
	iStringSectionOffset = utils.charToInt(sectionHeaderVector_64[sStringSectionTableIndex].sh_offset);
	boolean bFlag = true;
	for (unsigned int i = 0; i < programHeaderVector_64.size(); i++)
	{
		if (utils.charToInt(programHeaderVector_64[i].p_type) == 1)
		{
			if (bFlag){
				iFristLoadIndex = i;
				bFlag = false;
			}
			else
			{
				iLastLoadIndex = i;
			}
		}
	}
	
	long iLastLoadVaddr = utils.charToLong(programHeaderVector_64[iLastLoadIndex].p_vaddr);
	long iLastMem = utils.charToLong(programHeaderVector_64[iLastLoadIndex].p_memsz);
	long iLastAlign = utils.charToLong(programHeaderVector_64[iLastLoadIndex].p_align);
	iAddSectionStartAddr = alignOffset(iLastLoadVaddr + iLastMem, iLastAlign);
}
char* AddSection::addSectionHeader(char *p_cSrc,int type)//1 32位 2 64位
{
	char *p_cNewHeader = NULL;
	char *p_cNewSrc = NULL;
	Utils utils;
	int iSrcLen = ireadSrcFileSize;
	if (type == 1)
	{
		p_cNewHeader = new char[40];
		*(int *)p_cNewHeader = iAddSectionStartAddr - iStringSectionOffset;
		*((int *)p_cNewHeader + 1) = 1;//name
		*((int *)p_cNewHeader + 2) = 0x2;//flags
		*((int *)p_cNewHeader + 3) = iAddSectionStartAddr + iNewSectionNameLen;//addr
		*((int *)p_cNewHeader + 4) = iAddSectionStartAddr + iNewSectionNameLen;//offset
		*((int *)p_cNewHeader + 5) = iNewSectionSize;//size
		*((int *)p_cNewHeader + 6) = 0;//link
		*((int *)p_cNewHeader + 7) = 0;//info
		*((int *)p_cNewHeader + 8) = 4;//align
		*((int *)p_cNewHeader + 9) = 0;//entsize
		int iNewSrcLen = iSrcLen + 40;
		p_cNewSrc = new char[iNewSrcLen];
		memcpy_s(p_cNewSrc, iSrcLen, p_cSrc, iSrcLen);//拷贝原有的节区
		memcpy_s(p_cNewSrc + iSrcLen,40, p_cNewHeader, 40);//在原有的基础上附加上新增加的节区
		iAddsectionAfterFileSize = iNewSrcLen;
		delete[]p_cNewHeader;
	}
	if (type == 2)
	{
		p_cNewHeader = new char[64];

	}
	return p_cNewSrc;

}
char* AddSection::adSectionForFileEnd(char *p_cSrc)//增加新的节区于文件末尾
{
	int iNewSectionLen = iNewSectionSize + iNewSectionNameLen;
	char * p_cNewSection = new char[iNewSectionLen];//增加的节区（包含节区名称和节区）
	memcpy_s(p_cNewSection, iNewSectionNameLen, p_cNewSectionName, iNewSectionNameLen);//将节区名称拷贝进去	
	char * p_cNewSrc = new char[iAddSectionStartAddr + iNewSectionLen];
	memcpy_s(p_cNewSrc, iAddsectionAfterFileSize, p_cSrc, iAddsectionAfterFileSize);//拷贝原有的文件
	memcpy_s(p_cNewSrc + iAddSectionStartAddr, iNewSectionLen, p_cNewSection, iNewSectionLen);//在文件的真实末尾（云星于内存中）添加新的节区
	delete[] p_cNewSection;
	delete[] p_cSrc;
	iFinalFileSize = iAddSectionStartAddr + iNewSectionLen;
	return p_cNewSrc;
}
char* AddSection::chageStrTabLen(char *p_cSrc,int type)//修改str
{
	int iStrIndexPos = 0;
	if (type == 1)
	{
		iStrIndexPos = iSectionHeaderOffset + (sStringSectionTableIndex)* 40 + 20;//（20为size字段在section中的位置，32位 20 64位32）
		
	}
	if (type == 2)
	{
		iStrIndexPos = iSectionHeaderOffset + (sStringSectionTableIndex)* 40 + 32;//（20为size字段在section中的位置，32位 20 64位32）
	}
	int iSizelen = iAddSectionStartAddr - iStringSectionOffset + iNewSectionNameLen;
	*((int *)(p_cSrc + iStrIndexPos)) = iSizelen;
	return p_cSrc;
}
char* AddSection::changeElfHeaderSectionCount(char *p_Src,int type)
{
	int iSectionCountIndex = 0;
	if (type == 1)
	{
		iSectionCountIndex = 48;
	}
	if (type == 2)
	{
		iSectionCountIndex = 60;
	}
	short sCount = *((short *)(p_Src + iSectionCountIndex));
	sCount++;
	*((short *)(p_Src + iSectionCountIndex)) = sCount;
	return p_Src;
}
void AddSection::changeProgramHeaderLoadInfo(char *p_cSrc, int type)
{
	Utils utils;
	int iElfHeaderSize = 0;
	int iProgramHeaderSize = 0;
	int iOffset = 0;
	int	iFileSizePos = 0;
	if (type == 1)
	{
		iElfHeaderSize = 52;
		iProgramHeaderSize = 32;
		iFileSizePos = 16;
		iOffset = iElfHeaderSize + iProgramHeaderSize *iFristLoadIndex + iFileSizePos;
		*((int *)(p_cSrc + iOffset)) = iFinalFileSize;
		*((int *)(p_cSrc + iOffset + 4)) =iFinalFileSize;
		*((int *)(p_cSrc + iOffset + 8)) = 7;
	}
	if (type == 2)
	{
		iElfHeaderSize = 64;
		iProgramHeaderSize = 56;
		iFileSizePos = 32;
	}
	//修改完成后保存为新的文件
	utils.saveNewFile(p_cSrcFilePath, p_cSrc, iFinalFileSize);
}
