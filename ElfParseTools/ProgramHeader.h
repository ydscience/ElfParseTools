#pragma once

class ProgramHeader_32
{
public:
	ProgramHeader_32();
	~ProgramHeader_32();
public:
	char p_type[4];
	char p_offset[4];
	char p_vaddr[4];
	char p_paddr[4];
	char p_filesz[4];
	char p_memsz[4];
	char p_flags[4];
	char p_align[4];
};
class ProgramHeader_64
{
public:
	ProgramHeader_64();
	~ProgramHeader_64();
public:
	char p_type[4];
	char p_flags[4];
	char p_offset[8];
	char p_vaddr[8];
	char p_paddr[8];
	char p_filesz[8];
	char p_memsz[8];
	char p_align[8];
};