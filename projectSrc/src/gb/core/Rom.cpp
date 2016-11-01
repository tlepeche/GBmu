#include <fstream>
#include <iostream>
#include "Rom.hpp"

Rom			Rom::_instance = Rom();

Rom::Rom(void)
{
	this->_rom = NULL;
	this->_rom_size = 0;
	this->_pos = 0;
}

Rom::~Rom(void)
{
	if (this->_rom != NULL)
		delete[] this->_rom;
}

Rom			&Rom::Instance(void)
{
	return Rom::_instance;
}

int			Rom::load(char *file)
{
	std::ifstream	romFile;

	this->_pos = 0;
	this->_rom = NULL;
	romFile.open(file, std::ios::in | std::ios::ate | std::ios::binary);
	if (romFile.is_open())
	{
		this->_rom_size = romFile.tellg();
		this->_rom = new char [this->_rom_size];
		romFile.seekg(0, std::ios::beg);
		romFile.read(this->_rom, this->_rom_size);
		romFile.close();
		return 0;
	}
	return 1;
}

char		*Rom::read(uint32_t size)
{
	int		i;
	char	*data;

	if (this->_rom == NULL)
		return NULL;
	if (size > this->_rom_size)
		size = this->_rom_size;
	data = new char [size + 1];
	if (data == NULL)
		return NULL;
	for (i = 0; i < size; ++i)
		data[i] = this->_rom[this->_pos + i];
	this->_pos += i;
	data[i] = '\0';
	return data;
}

void		Rom::reset(void)
{
	this->_pos = 0;
}
