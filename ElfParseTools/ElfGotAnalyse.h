#pragma once
class ElfSym_32
{
	public:
		ElfSym_32();
		~ElfSym_32();

	public:
		char st_name[4];
		char 	st_value[4];
		char 	st_size[4];
		char	st_info[1];
		char	st_other[1];
		char	st_shndx[2];
	};

class ElfSym_64
{
	public:
		ElfSym_64();
		~ElfSym_64();

	public:
		char st_name[4];
		char	st_info[1];
		char	st_other[1];
		char	st_shndx[2];
		char 	st_value[8];
		char 	st_size[8];
};

class ElfRel_32
{
	public:
		ElfRel_32();
		~ElfRel_32();
	public:
		char r_offset[4];
		char r_info[4];
};

class ElfRel_64
{
public:
	ElfRel_64();
	~ElfRel_64();
public:
	char r_offset[8];
	char r_info[8];
};
