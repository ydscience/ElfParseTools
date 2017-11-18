#include "ParseHeaderInfo.h"
using namespace std;

string typeStr[6] = { "NONE(未知目标文件格式)", "REL(可重定位文件)", "EXEC(可执行文件)", "DYN(共享目标文件)", "CORE(CORE文件)", "OTHER(特定处理器文件)" };
string proggramHeaderType[7] = { "LOAD", "DYNAMIC", "INTERP", "NOTE", "SHLIB", "PHDR", "LOPORC" };
string sectionHeaderType[13] = { "NULL", "PROGBITS", "SYMTAB", "STRTAB", "RELA", "HASH", "DYNAMIC", "NOTE", "NOBITS", "REL", "SHLIB", "DYNSYM", "OPROC" };

ElFHeader_32  ParseHeaderInfo::parseElfHeader_32(char * p_cRes, bool bIsPrint)
{

		ElFHeader_32 elfHeader_32;
		memcpy_s(elfHeader_32.e_ident, 16, p_cRes, 16);
		memcpy_s(elfHeader_32.e_type, 2, p_cRes + 16, 2);
		memcpy_s(elfHeader_32.e_machine, 2, p_cRes + 18, 2);
		memcpy_s(elfHeader_32.e_version, 4, p_cRes + 20, 4);
		memcpy_s(elfHeader_32.e_entry, 4, p_cRes + 24, 4);
		memcpy_s(elfHeader_32.e_phoff, 4, p_cRes + 28, 4);
		memcpy_s(elfHeader_32.e_shoff, 4, p_cRes + 32, 4);
		memcpy_s(elfHeader_32.e_flags, 4, p_cRes + 36, 4);
		memcpy_s(elfHeader_32.e_ehsize, 2, p_cRes + 40, 2);
		memcpy_s(elfHeader_32.e_phentsize, 2, p_cRes + 42, 2);
		memcpy_s(elfHeader_32.e_phnum, 2, p_cRes + 44, 2);
		memcpy_s(elfHeader_32.e_shentsize, 2, p_cRes + 46, 2);
		memcpy_s(elfHeader_32.e_shnum, 2, p_cRes + 48, 2);
		memcpy_s(elfHeader_32.e_shstrndx, 2, p_cRes + 50, 2);
		if (bIsPrint)
		{
			printHeaderInfo_32(elfHeader_32);//1表示32位 2表示64位
		}
	
	return elfHeader_32;
}
ElFHeader_64  ParseHeaderInfo::parseElfHeader_64(char * p_cRes, bool bIsPrint)
{
	
	ElFHeader_64 elfHeader_64;
	memcpy_s(elfHeader_64.e_ident, 16, p_cRes, 16);
	memcpy_s(elfHeader_64.e_type, 2, p_cRes + 16, 2);
	memcpy_s(elfHeader_64.e_machine, 2, p_cRes + 18, 2);
	memcpy_s(elfHeader_64.e_version, 4, p_cRes + 20, 4);
	memcpy_s(elfHeader_64.e_entry, 8, p_cRes + 24, 8);
	memcpy_s(elfHeader_64.e_phoff, 8, p_cRes + 32, 8);
	memcpy_s(elfHeader_64.e_shoff, 8, p_cRes + 40, 8);
	memcpy_s(elfHeader_64.e_flags, 4, p_cRes + 48, 4);
	memcpy_s(elfHeader_64.e_ehsize, 2, p_cRes + 52, 2);
	memcpy_s(elfHeader_64.e_phentsize, 2, p_cRes + 54, 2);
	memcpy_s(elfHeader_64.e_phnum, 2, p_cRes + 56, 2);
	memcpy_s(elfHeader_64.e_shentsize, 2, p_cRes + 58, 2);
	memcpy_s(elfHeader_64.e_shnum, 2, p_cRes + 60, 2);
	memcpy_s(elfHeader_64.e_shstrndx, 2, p_cRes + 62, 2);
	if (bIsPrint)
	{
		printHeaderInfo_64(elfHeader_64);//1表示32位 2表示64位
	}
	return elfHeader_64;
}

void  ParseHeaderInfo::printHeaderInfo_32(ElFHeader_32 elfHeader_32)
{
	Utils utils;
	cout << "Magic:";
	utils.printStr(elfHeader_32.e_ident, 16);
	cout << "type:";
	unsigned int nLen = utils.charToShort(elfHeader_32.e_type);
	if (nLen < 5 && nLen >= 0)
	{
		cout << typeStr[nLen].c_str() << endl;
	}
	else{
		cout << typeStr[5].c_str() << endl;
	}
	cout << "version:" << utils.charToInt(elfHeader_32.e_version) << endl;
	cout << "e_machine:";
	utils.printStr(elfHeader_32.e_machine, 2);
	int nEntry_len = utils.charToInt(elfHeader_32.e_entry);
	if (nEntry_len < 10 && nEntry_len >= 0)
	{
		cout << "程序入口地址:0x0" << nEntry_len << endl;;
	}
	else{
		cout << "程序入口地址:0x" << nEntry_len << endl;
	}

	cout << "ELF 头的大小：" << dec << utils.charToShort(elfHeader_32.e_ehsize) << endl;
	cout << "标志位：";
	utils.printStr(elfHeader_32.e_flags, 4);
	cout << "Program Header " << endl;
	cout << dec << "偏移量：" << utils.charToInt(elfHeader_32.e_phoff) << endl;
	cout << dec << "程序头大小：" << utils.charToShort(elfHeader_32.e_phentsize) << endl;
	cout << dec << "程序头数目：" << utils.charToShort(elfHeader_32.e_phnum) << endl;
	cout << "Section Header" << endl;
	cout << dec << "偏移量：" << utils.charToInt(elfHeader_32.e_shoff) << endl;
	cout << dec << "节区头大小：" << utils.charToShort(elfHeader_32.e_shentsize) << endl;
	cout << dec << "节区头数目：" << utils.charToShort(elfHeader_32.e_shnum) << endl;
	cout << "字符串表头索引：" << utils.charToShort(elfHeader_32.e_shstrndx) << endl;
}
void  ParseHeaderInfo::printHeaderInfo_64(ElFHeader_64 elfHeader_64)
{
	Utils utils;
	cout << "Magic:";
	utils.printStr(elfHeader_64.e_ident, 16);
	cout << "type:";
	unsigned int nLen = utils.charToShort(elfHeader_64.e_type);
	if (nLen < 5 && nLen >= 0)
	{
		cout << typeStr[nLen].c_str() << endl;
	}
	else{
		cout << typeStr[5].c_str() << endl;
	}
	cout << "version:" << utils.charToInt(elfHeader_64.e_version) << endl;
	cout << "e_machine:";
	utils.printStr(elfHeader_64.e_machine, 2);
	long nEntry_len = utils.charToLong(elfHeader_64.e_entry);
	if (nEntry_len < 10 && nEntry_len >= 0)
	{
		cout << "程序入口地址:0x0" << nEntry_len << endl;;
	}
	else{
		cout << "程序入口地址:0x" << nEntry_len << endl;
	}

	cout << "ELF 头的大小：" << dec << utils.charToShort(elfHeader_64.e_ehsize) << endl;
	cout << "标志位：";
	utils.printStr(elfHeader_64.e_flags, 4);
	cout << "Program Header " << endl;
	cout << dec << "偏移量：" << utils.charToLong(elfHeader_64.e_phoff) << endl;
	cout << dec << "程序头大小：" << utils.charToShort(elfHeader_64.e_phentsize) << endl;
	cout << dec << "程序头数目：" << utils.charToShort(elfHeader_64.e_phnum) << endl;
	cout << "Section Header" << endl;
	cout << dec << "偏移量：" << utils.charToLong(elfHeader_64.e_shoff) << endl;
	cout << dec << "节区头大小：" << utils.charToShort(elfHeader_64.e_shentsize) << endl;
	cout << dec << "节区头数目：" << utils.charToShort(elfHeader_64.e_shnum) << endl;
	cout << "字符串表头索引：" << utils.charToShort(elfHeader_64.e_shstrndx) << endl;
}

ProgramHeader_32  ParseHeaderInfo::parseProgramHeader_32(char *p_cRes, int start)
{
	ProgramHeader_32   programHeader_32;
	memcpy_s(programHeader_32.p_type, 4, p_cRes + start, 4);
	memcpy_s(programHeader_32.p_offset, 4, p_cRes + start + 4, 4);
	memcpy_s(programHeader_32.p_vaddr, 4, p_cRes + start + 8, 4);
	memcpy_s(programHeader_32.p_paddr, 4, p_cRes + start + 12, 4);
	memcpy_s(programHeader_32.p_filesz, 4, p_cRes + start + 16, 4);
	memcpy_s(programHeader_32.p_memsz, 4, p_cRes + start + 20, 4);
	memcpy_s(programHeader_32.p_flags, 4, p_cRes + start + 24, 4);
	memcpy_s(programHeader_32.p_align, 4, p_cRes + start + 28, 4);
	return programHeader_32;
}
ProgramHeader_64  ParseHeaderInfo::parseProgramHeader_64(char *p_cRes, int start)
{
	ProgramHeader_64   programHeader_64;
	memcpy_s(programHeader_64.p_type, 4, p_cRes + start, 4);
	memcpy_s(programHeader_64.p_flags, 4, p_cRes + start + 4, 4);
	memcpy_s(programHeader_64.p_offset, 8, p_cRes + start +8, 8);
	memcpy_s(programHeader_64.p_vaddr, 8, p_cRes + start + 16, 8);
	memcpy_s(programHeader_64.p_paddr, 8, p_cRes + start + 24, 8);
	memcpy_s(programHeader_64.p_filesz,8, p_cRes + start + 32, 8);
	memcpy_s(programHeader_64.p_memsz, 8, p_cRes + start + 40, 8);
	memcpy_s(programHeader_64.p_align, 8, p_cRes + start + 48, 8);
	return programHeader_64;
}
void  ParseHeaderInfo::printProgramHeader_32(vector<ProgramHeader_32> programHeaderVector, ElFHeader_32 elfHeader_32)
{
	Utils utils;
	cout << "\n\n";
	cout << "Program  Header Info" << endl;
	int nSize = utils.charToShort(elfHeader_32.e_phnum);
	cout << "共有" << nSize << "程序头，开始偏移量为" << utils.charToInt(elfHeader_32.e_phoff) << endl;
	cout << "type	" << "offset	" << "virAddre   " << "phyAdd	"
		<< "fileSize    " << "memSize   " << "flag	" << "align" << endl;
	for (int i = 0; i < nSize; i++)
	{
		int nType = utils.charToInt(programHeaderVector[i].p_type);
		if (nType < 6 && nType >= 0)
		{
			cout << proggramHeaderType[i].c_str();
		}
		else{
			cout << proggramHeaderType[6].c_str();
		}
		cout << "  ";
		utils.printProgramInfo(programHeaderVector[i].p_offset, 4);
		cout << "    ";
		utils.printProgramInfo(programHeaderVector[i].p_vaddr, 4);
		cout << "    ";
		utils.printProgramInfo(programHeaderVector[i].p_paddr, 4);
		cout << "       ";
		cout << dec << utils.charToInt(programHeaderVector[i].p_filesz);
		cout << "         ";
		cout << dec << utils.charToInt(programHeaderVector[i].p_memsz);
		cout << "        ";
		cout << dec << utils.charToInt(programHeaderVector[i].p_flags);
		cout << "        ";
		cout << dec << utils.charToInt(programHeaderVector[i].p_align);
		cout << "\n";
	}
}
void  ParseHeaderInfo::printProgramHeader_64(vector<ProgramHeader_64> programHeaderVector, ElFHeader_64 elfHeader_64)
{
	Utils utils;
	cout << "\n\n";
	cout << "Program  Header Info" << endl;
	int nSize = utils.charToShort(elfHeader_64.e_phnum);
	cout << "共有" << nSize << "程序头，开始偏移量为" << utils.charToLong(elfHeader_64.e_phoff) << endl;
	cout << "type	" << "flag	" << "offset	" << "virAddre   " << "phyAdd	"
		<< "fileSize    " << "memSize   " << "align" << endl;
	for (int i = 0; i < nSize; i++)
	{
		int nType = utils.charToInt(programHeaderVector[i].p_type);
		if (nType < 6 && nType >= 0)
		{
			cout << proggramHeaderType[i].c_str();
		}
		else{
			cout << proggramHeaderType[6].c_str();
		}
		cout << "  ";
		cout << dec << utils.charToInt(programHeaderVector[i].p_flags);
		cout << "  ";
		utils.printProgramInfo(programHeaderVector[i].p_offset, 8);
		cout << "  ";
		utils.printProgramInfo(programHeaderVector[i].p_vaddr, 8);
		cout << "  ";
		utils.printProgramInfo(programHeaderVector[i].p_paddr, 8);
		cout << "    ";
		cout << dec << utils.charToLong(programHeaderVector[i].p_filesz);
		cout << "     ";
		cout << dec << utils.charToLong(programHeaderVector[i].p_memsz);
		cout << "     ";
		cout << dec << utils.charToLong(programHeaderVector[i].p_align);
		cout << "\n";
	}
}

SectionHeader_32  ParseHeaderInfo::parseSectionHeader_32(char *p_cRes, int start)
{
	SectionHeader_32 sectionHeader_32;
	memcpy_s(sectionHeader_32.sh_name, 4, p_cRes + start, 4);
	memcpy_s(sectionHeader_32.sh_type, 4, p_cRes + start + 4, 4);
	memcpy_s(sectionHeader_32.sh_flags, 4, p_cRes + start + 8, 4);
	memcpy_s(sectionHeader_32.sh_addr, 4, p_cRes + start + 12, 4);
	memcpy_s(sectionHeader_32.sh_offset, 4, p_cRes + start + 16, 4);
	memcpy_s(sectionHeader_32.sh_size, 4, p_cRes + start + 20, 4);
	memcpy_s(sectionHeader_32.sh_link, 4, p_cRes + start + 24, 4);
	memcpy_s(sectionHeader_32.sh_info, 4, p_cRes + start + 28, 4);
	memcpy_s(sectionHeader_32.sh_addralign, 4, p_cRes + start + 32, 4);
	memcpy_s(sectionHeader_32.sh_entsize, 4, p_cRes + start + 36, 4);
	return sectionHeader_32;
}
SectionHeader_64 ParseHeaderInfo::parseSectionHeader_64(char *p_cRes, int start)
{
	SectionHeader_64 sectionHeader_64;
	memcpy_s(sectionHeader_64.sh_name, 4, p_cRes + start, 4);
	memcpy_s(sectionHeader_64.sh_type, 4, p_cRes + start + 4, 4);
	memcpy_s(sectionHeader_64.sh_flags, 8, p_cRes + start + 8, 8);
	memcpy_s(sectionHeader_64.sh_addr, 8, p_cRes + start + 16, 8);
	memcpy_s(sectionHeader_64.sh_offset, 8, p_cRes + start + 24, 8);
	memcpy_s(sectionHeader_64.sh_size, 8, p_cRes + start + 32, 8);
	memcpy_s(sectionHeader_64.sh_link, 4, p_cRes + start + 40, 4);
	memcpy_s(sectionHeader_64.sh_info, 4, p_cRes + start + 44, 4);
	memcpy_s(sectionHeader_64.sh_addralign, 8, p_cRes + start + 48, 8);
	memcpy_s(sectionHeader_64.sh_entsize, 8, p_cRes + start + 56, 8);
	return sectionHeader_64;
}

void  ParseHeaderInfo::printSectionHeader_32(vector<SectionHeader_32> sectionHeaderVector, ElFHeader_32 elfHeader_32)
{
	Utils utils;
	int nCount = utils.charToShort(elfHeader_32.e_shnum);
	cout << "\n\n";
	cout << "section  Header Info" << endl;
	cout << "共有" << dec << nCount << "节区头";
	cout << "开始偏移量为" << hex << utils.charToInt(elfHeader_32.e_shoff) << endl;
	cout << "name	 " << "type	  " << "flg      " << "addr    "
		<< "off     " << "size     " << "lk	  " << "inf     " << "al" << endl;
	for (int i = 0; i < nCount; i++)
	{
		utils.printProgramInfo(sectionHeaderVector[i].sh_name, 4);
		cout << "   ";
		int nType = utils.charToInt(sectionHeaderVector[i].sh_type);
		if (nType < 12 && nType >=0)
		{
			cout << sectionHeaderType[nType].c_str();
		}
		else{
			cout << sectionHeaderType[12].c_str();
		}
		cout << "       " << utils.charToInt(sectionHeaderVector[i].sh_flags);
		cout << "     ";
		utils.printProgramInfo(sectionHeaderVector[i].sh_addr, 4);
		cout << "     ";
		utils.printProgramInfo(sectionHeaderVector[i].sh_offset, 4);
		cout << "     ";
		utils.printProgramInfo(sectionHeaderVector[i].sh_size, 4);
		cout << "    " << utils.charToInt(sectionHeaderVector[i].sh_link);
		cout << "    " << utils.charToInt(sectionHeaderVector[i].sh_info);
		cout << "    " << utils.charToInt(sectionHeaderVector[i].sh_addralign);
		cout << "\n";
	}
}
void  ParseHeaderInfo::printSectionHeader_64(vector<SectionHeader_64> sectionHeaderVector, ElFHeader_64 elfHeader_64)
{
	Utils utils;
	int nCount = utils.charToShort(elfHeader_64.e_shnum);
	cout << "\n\n";
	cout << "section  Header Info" << endl;
	cout << "共有" << dec << nCount << "节区头";
	cout << "开始偏移量为" << hex << utils.charToLong(elfHeader_64.e_shoff) << endl;
	cout << "name	 " << "type	  " << "flg      " << "addr    "
		<< "off     " << "size     " << "lk	  " << "inf     " << "al" << endl;
	for (int i = 0; i < nCount; i++)
	{
		utils.printProgramInfo(sectionHeaderVector[i].sh_name, 4);
		cout << "   ";
		int nType = utils.charToInt(sectionHeaderVector[i].sh_type);
		if (nType < 12 && nType >=0)
		{
			cout << sectionHeaderType[nType].c_str();
		}
		else{
			cout << sectionHeaderType[12].c_str();
		}
		cout << "       " << utils.charToLong(sectionHeaderVector[i].sh_flags);
		cout << "     ";
		utils.printProgramInfo(sectionHeaderVector[i].sh_addr, 8);
		cout << "     ";
		utils.printProgramInfo(sectionHeaderVector[i].sh_offset, 8);
		cout << "     ";
		utils.printProgramInfo(sectionHeaderVector[i].sh_size, 8);
		cout << "    " << utils.charToInt(sectionHeaderVector[i].sh_link);
		cout << "    " << utils.charToInt(sectionHeaderVector[i].sh_info);
		cout << "    " << utils.charToLong(sectionHeaderVector[i].sh_addralign);
		cout << "\n";
	}

}

ParseHeaderInfo::~ParseHeaderInfo()
{

}

ParseHeaderInfo::ParseHeaderInfo()
{

}