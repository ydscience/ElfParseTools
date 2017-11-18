#include"ElfHeader.h"
#include"ProgramHeader.h"
#include"SectionHeader.h"
#include<vector>
#include"Windows.h"
using namespace std;
class AddSection
{
public:
	AddSection();
	~AddSection();
public:
	long iSectionHeaderOffset;
	short sStringSectionTableIndex;
	long iStringSectionOffset;
	int iFristLoadIndex;
	int iLastLoadIndex;
	long iAddSectionStartAddr = 0;//添加段节区开始的地址
	int iNewSectionNameLen = 0;//添加section的名字的长度
	int iNewSectionSize = 0;//新增加的Section的大小
	int ireadSrcFileSize = 0;//读取的源文件大小
	int iAddsectionAfterFileSize = 0;//增加节区头之后的文件大小
	int iFinalFileSize = 0;//最终的文件大小
	char * p_cNewSectionName;
	char * p_cSrcFilePath;//源文件路径
public:
	void initParameter_32(ElFHeader_32 elfHeader_32, vector<ProgramHeader_32> programHeaderVector_32,vector<SectionHeader_32> sectionHeaderVector_32);
	void initParameter_64(ElFHeader_64 elfHeader_64, vector<ProgramHeader_64> programHeaderVector_64, vector<SectionHeader_64> sectionHeaderVector_64);
	char* addSectionHeader(char *p_cSrc,int type);
	char* adSectionForFileEnd(char *p_cSrc);
	char* chageStrTabLen(char *p_cSrc,int type);
	char* changeElfHeaderSectionCount(char *p_Src,int type);
	void changeProgramHeaderLoadInfo(char *p_cSrc,int type);
	long alignOffset(long addr, long align);
};

