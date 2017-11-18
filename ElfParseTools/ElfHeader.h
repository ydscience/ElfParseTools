#pragma once
class ElFHeader_64
{
public:
	ElFHeader_64();
	~ElFHeader_64();
public:
	char e_ident[16];
	char e_type[2];
	char e_machine[2];
	char e_version[4];
	char e_entry[8];
	char e_phoff[8];
	char e_shoff[8];
	char e_flags[4];
	char e_ehsize[2];
	char e_phentsize[2];
	char e_phnum[2];
	char e_shentsize[2];
	char e_shnum[2];
	char e_shstrndx[2];
};

class ElFHeader_32
{
public:
	ElFHeader_32();
	~ElFHeader_32();
public:
	char e_ident[16];
	char e_type[2];
	char e_machine[2];
	char e_version[4];
	char e_entry[4];
	char e_phoff[4];
	char e_shoff[4];
	char e_flags[4];
	char e_ehsize[2];
	char e_phentsize[2];
	char e_phnum[2];
	char e_shentsize[2];
	char e_shnum[2];
	char e_shstrndx[2];
};