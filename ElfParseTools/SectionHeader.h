#pragma once
class SectionHeader_32
{
public:
	SectionHeader_32();
	~SectionHeader_32();
public:
	char sh_name[4];
	char sh_type[4];
	char sh_flags[4];
	char sh_addr[4];
	char sh_offset[4];
	char sh_size[4];
	char sh_link[4];
	char sh_info[4];
	char sh_addralign[4];
	char sh_entsize[4];
};


class SectionHeader_64
{
public:
	SectionHeader_64();
	~SectionHeader_64();
public:
	char sh_name[4];
	char sh_type[4];
	char sh_flags[8];
	char sh_addr[8];
	char sh_offset[8];
	char sh_size[8];
	char sh_link[4];
	char sh_info[4];
	char sh_addralign[8];
	char sh_entsize[8];
};