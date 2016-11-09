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

	flag_cgb = (this->_rom[0x0143] & 0xFF);
	if (flag_cgb == 0x00 || flag_cgb == 0x80)
		this->_info.type = GB;
	else if (flag_cgb == 0xC0)
		this->_info.type = GBC;
	this->_info.cartridge = this->_rom[0x0147];
	this->_info.romSize = this->_rom[0x0148];
	this->_info.eramSize = this->_rom[0x0149];
	this->_nbanks = getBankRom(this->_info.romSize);
	this->_nrambanks = getBankEram(this->_info.eramSize);
	if (this->_nrambanks > 0)
		this->_eram = new uint8_t [this->_nrambanks * 8192];
	this->_bank = 0;
	this->_u_bank = 0;
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

uint8_t		Rom::read(uint16_t addr)
{
	if (this->_rom == NULL)
		return -1;
	if (addr < 0x4000)
		return this->_rom[addr];
	else if (addr < 0x8000)
		return this->_rom[addr + ((this->_bank + (this->_u_bank * 0x20)) * 0x4000)];
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
			if (this->_tbank)
			{
				// eram
				if (val == 0x01 || val <= 0x1f)
					this->_rambank = val;
			}
			else
			{
				// rom
				if (val == 0x01 || val <= 0x1f)
					this->_bank = val;
			}
			break;
		case 0x4000:
		case 0x5000:
			// Upper Rom bank code
			if (val <= 0x03)
			{
				if (this->_tbank == 0)
					this->_u_bank = val;
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

irom		Rom::getType(void)
{
	return this->_info;
}

uint8_t		Rom::getBankRom(uint8_t octet)
{
	if (octet == 1)
		return 4;
	else if (octet == 2)
		return 8;
	else if (octet == 3)
		return 16;
	else if (octet == 4)
		return 32;
	else if (octet == 5)
		return 64;
	else if (octet == 6)
		return 128;
	else if (octet == 52)
		return 72;
	else if (octet == 53)
		return 80;
	else if (octet == 54)
		return 96;
	return 2;
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
