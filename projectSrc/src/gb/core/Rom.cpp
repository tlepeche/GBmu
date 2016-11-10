#include <fstream>
#include <iostream>
#include "Rom.hpp"

Rom			Rom::_instance = Rom();

Rom::Rom(void)
{
	this->_eram = NULL;
	this->_rom = NULL;
}

Rom::~Rom(void)
{
	if (this->_rom != NULL)
		delete[] this->_rom;
	if (this->_eram != NULL)
		delete[] this->_eram;
}

Rom			&Rom::Instance(void)
{
	return Rom::_instance;
}

void		Rom::init(void)
{
	uint8_t	flag_cgb;

	flag_cgb = (this->_rom[CGBFLAG] & 0xFF);
	if (flag_cgb == 0x00 || flag_cgb == 0x80)
		this->_hardware = GB;
	else if (flag_cgb == 0xC0)
		this->_hardware = GBC;
	if (this->getBankEram(this->_rom[RAMSIZE]) > 0)
		this->_eram = new uint8_t [this->getBankEram(this->_rom[RAMSIZE]) * 8192];
	this->_bank = 0;
	this->_rambank = 0;
	this->_write_protect = 0;
}

int			Rom::load(const char *file)
{
	std::ifstream	romFile;
	uint32_t		rom_size;

	if (this->_rom != NULL)
		delete[] this->_rom;
	if (this->_eram != NULL)
		delete[] this->_eram;
	this->_rom = NULL;
	this->_eram = NULL;
	romFile.open(file, std::ios::in | std::ios::ate | std::ios::binary);
	if (romFile.is_open())
	{
		rom_size = romFile.tellg();
		this->_rom = new char [rom_size];
		romFile.seekg(0, std::ios::beg);
		romFile.read(this->_rom, rom_size);
		romFile.close();
		this->init();
		return 0;
	}
	return 1;
}

htype		Rom::getHardware(void)
{
	return this->_hardware;
}

uint8_t		Rom::read(uint16_t addr)
{
	if (this->_rom == NULL)
		return -1;
	if (addr < 0x4000)
		return this->_rom[addr];
	else if (addr < 0x8000)
		return this->_rom[(addr - 0x4000) + (this->_bank * 0x4000)];
	else if (addr >= 0xA000 && addr < 0xC000)
		return this->_eram[addr + (this->_rambank * 0x2000)];
	return 0;
}

void		Rom::write(uint16_t addr, uint8_t val)
{
	switch (addr & 0xF000){
		case 0x0000:
		case 0x1000:
			// RAMCS
			if (val == 0x00 || val == 0x0A)
				this->_write_protect = val;
			break;
		case 0x2000:
		case 0x3000:
			// Rom bank code
			if (val >= 0x01 && val <= 0x1F)
			{
				val &= 0x1F;
				this->_bank &= 0xE0;
				this->_bank |= val;
			}
			break;
		case 0x4000:
		case 0x5000:
			// Upper Rom bank code
			if (val <= 0x03)
			{
				if (this->_tbank == 0)
				{
					// ROM
					val <<= 5;
					this->_bank &= 0x1F;
					val &= 0xE0;
					this->_bank |= val;
				}
				else
				{
					// ERAM
					this->_rambank = val;
				}
			}
			break;
		case 0x6000:
		case 0x7000:
			// Rom/Ram change
			if (val == 0x00 || val == 0x01)
				this->_tbank = val;
			break;
		case 0xA000:
		case 0xB000:
			// ERAM
			if (this->_write_protect == 0x0A)
				this->_eram[addr + (this->_rambank * 0x2000)] = val;
			break;
	}
}

void		Rom::reset(void)
{
}

uint8_t		Rom::getBankEram(uint8_t octet)
{
	if (octet == 1 || octet == 2)
		return 1;
	else if (octet == 3)
		return 4;
	else if (octet == 4)
		return 16;
	return 0;
}

bool		Rom::isLoaded(void)
{
	if (this->_rom == NULL)
		return false;
	return true;
}
