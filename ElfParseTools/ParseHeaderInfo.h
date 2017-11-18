#pragma once
#include<iostream>
#include <vector>
#include"ProgramHeader.h"
#include"SectionHeader.h"
#include"ElfHeader.h"
#include"Utils.h"
#include<vector>
using namespace std;


class ParseHeaderInfo
{
public:
	ParseHeaderInfo();
	~ParseHeaderInfo();
public:
	ElFHeader_32 parseElfHeader_32(char * p_cRes, bool bIsPrint);
	ElFHeader_64 parseElfHeader_64(char * p_cRes, bool bIsPrint);
	void printHeaderInfo_32(ElFHeader_32 elfHeader_32);
	void printHeaderInfo_64(ElFHeader_64 elfHeader_32);

	ProgramHeader_32 parseProgramHeader_32(char *p_cRes, int start);
	ProgramHeader_64 parseProgramHeader_64(char *p_cRes, int start);
	void printProgramHeader_32(vector<ProgramHeader_32> programHeaderVector, ElFHeader_32 elfHeader_32);
	void printProgramHeader_64(vector<ProgramHeader_64> programHeaderVector, ElFHeader_64 elfHeader_64);

	SectionHeader_32 parseSectionHeader_32(char *p_cRes, int start);
	SectionHeader_64 parseSectionHeader_64(char *p_cRes, int start);
	void printSectionHeader_32(vector<SectionHeader_32> sectionHeaderVector, ElFHeader_32 elfHeader_32);
	void printSectionHeader_64(vector<SectionHeader_64> sectionHeaderVector, ElFHeader_64 elfHeader_64);
};

