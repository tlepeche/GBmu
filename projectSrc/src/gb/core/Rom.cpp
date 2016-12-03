#include <fstream>
#include <iostream>
#include "Rom.hpp"
#include <string.h>

Rom::Rom(void) :
	_rom(nullptr),
	_eram(nullptr),
	_pathsave(nullptr)
{
	this->_mbcPtrRead = {
		std::bind(&Rom::_readRom, this, std::placeholders::_1),
		std::bind(&Rom::_readMbc1, this, std::placeholders::_1),
		std::bind(&Rom::_readMbc2, this, std::placeholders::_1),
		std::bind(&Rom::_readMbc3, this, std::placeholders::_1),
		std::bind(&Rom::_readMbc5, this, std::placeholders::_1)
		};
	this->_mbcPtrWrite = {
		std::bind(&Rom::_writeRom, this, std::placeholders::_1, std::placeholders::_2),
		std::bind(&Rom::_writeMbc1, this, std::placeholders::_1, std::placeholders::_2),
		std::bind(&Rom::_writeMbc2, this, std::placeholders::_1, std::placeholders::_2),
		std::bind(&Rom::_writeMbc3, this, std::placeholders::_1, std::placeholders::_2),
		std::bind(&Rom::_writeMbc5, this, std::placeholders::_1, std::placeholders::_2)
		};
}

Rom::~Rom(void)
{
	if (this->_rom != NULL)
		delete[] this->_rom;
	if (this->_eram != NULL)
		delete[] this->_eram;
	if (this->_pathsave != NULL)
		delete[] this->_pathsave;
}

void		Rom::save(void)
{
	if (this->_eram != NULL && this->_mbc != MBC2)
	{
		this->_save.open(this->_pathsave, std::fstream::out | std::fstream::binary);
		if (this->_save.is_open())
		{
			uint32_t	size = this->getBankEram(this->_rom[RAMSIZE]) * 8192;
			this->_save.write((const char *)this->_eram, size);
			this->_save.close();
		}
	}
}

char		*Rom::getNameSave(const char *nameFile)
{
	std::string		namesave;
	namesave.append(nameFile);
	namesave.replace(namesave.rfind(".gb"), 5, ".save");
	char			*str = new char[namesave.length() + 1];
	strncpy(str, namesave.c_str(), namesave.length() + 1);
	return str;
}

void		Rom::init(const char *file)
{
	uint8_t	flag_cgb;

	flag_cgb = (this->_rom[CGBFLAG] & 0xFF);
	if (flag_cgb == 0x00)
		this->_hardware = GB;
	else if (flag_cgb == 0xC0 || flag_cgb == 0x80)
		this->_hardware = GBC;
	this->_mbc = this->getMbc(this->_rom[CARTRIDGE]);
	if (this->getBankEram(this->_rom[RAMSIZE]) > 0)
	{
		uint32_t	size = this->getBankEram(this->_rom[RAMSIZE]) * 8192;
		this->_eram = new uint8_t [size];
		memset(this->_eram, 0x00, size);
		if (this->_pathsave != NULL)
			delete[] this->_pathsave;
		this->_pathsave = this->getNameSave(file);
		this->_save.open(this->_pathsave, std::fstream::in | std::fstream::binary);
		if (this->_save.is_open())
		{
			uint32_t	sizeSave = this->getBankEram(this->_rom[RAMSIZE]) * 8192;
			this->_save.read((char *)this->_eram, sizeSave);
			this->_save.close();
		}
	}
	else if (this->_mbc == MBC2)
	{
		this->_eram = new uint8_t [0x200];
		memset(this->_eram, 0x00, 0x200);
	}
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
		std::cout << "=== FILE OPENED ===" << std::endl;
		rom_size = romFile.tellg();
		this->_rom = new char [rom_size];
		romFile.seekg(0, std::ios::beg);
		romFile.read(this->_rom, rom_size);
		romFile.close();
		this->init(file);
		if (this->_mbc == 0xFF || !this->_checkHeader())
			return -2;
		return 0;
	}
	return -1;
}

htype		Rom::getHardware(void)
{
	return this->_hardware;
}

uint8_t		Rom::read(uint16_t addr)
{
	if (this->_rom == NULL || this->_mbc == 0xFF)
		return -1;
	if (this->_bank == 0)
		this->_bank = 1;
	return this->_mbcPtrRead[this->_mbc](addr);
}

void		Rom::write(uint16_t addr, uint8_t val)
{
	if (this->_rom == NULL || this->_mbc == 0xFF)
		return ;
	this->_mbcPtrWrite[this->_mbc](addr, val);
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
	else if (octet == 5)
		return 8;
	return 0;
}

bool		Rom::isLoaded(void)
{
	if (this->_rom == NULL)
		return false;
	return true;
}

bool		Rom::_mbcRamAccess(void)
{
	if (this->_write_protect == 0x0A)
		return true;
	return false;
}

bool		Rom::_mbcTimerAccess(void)
{
	return this->_timer.lock;
}

bool		Rom::_checkHeader(void)
{
	int			i;
	uint8_t		x;
	uint8_t		nitendoLogo[0x30] = {0xCE,0xED,0x66,0x66,0xCC,0x0D,0x00,0x0B,0x03,0x73,0x00,0x83,0x00,0x0C,0x00,0x0D,0x00,0x08,0x11,0x1F,0x88,0x89,0x00,0x0E,0xDC,0xCC,0x6E,0xE6,0xDD,0xDD,0xD9,0x99,0xBB,0xBB,0x67,0x63,0x6E,0x0E,0xEC,0xCC,0xDD,0xDC,0x99,0x9F,0xBB,0xB9,0x33,0x3E};

	x = 0;
	for (i = 0; i < 0x30 ;++i)
	{
		if (nitendoLogo[i] != (this->_rom[LOGO + i] & 0xFF))
			return false;
	}
	for (i = 0x0134; i <= 0x014C; ++i)
		x = x - this->_rom[i]-1;
	if (x == (this->_rom[HEADERCHECKSUM] & 0xFF))
		return true;
	return false;
}

uint8_t		Rom::getMbc(uint8_t octet)
{
	switch (octet)
	{
		case 0x00:
		case 0x08:
		case 0x09:
			return ROM;
			break;
		case 0x01:
		case 0x02:
		case 0x03:
			return MBC1;
			break;
		case 0x05:
		case 0x06:
			return MBC2;
			break ;
		case 0x0F:
		case 0x10:
		case 0x11:
		case 0x12:
		case 0x13:
			return MBC3;
			break ;
		case 0x19:
		case 0x1A:
		case 0x1B:
		case 0x1C:
		case 0x1D:
		case 0x1E:
			return MBC5;
			break;
		default:
			return -1;
			break;
	}
	return -1;
}

uint8_t		Rom::_readRom(uint16_t addr)
{
	if (addr < 0x8000)
		return this->_rom[addr];
	return 0;
}

uint8_t		Rom::_readMbc1(uint16_t addr)
{
	if (addr < 0x4000)
		return this->_rom[addr];
	else if (addr < 0x8000)
		return this->_rom[(addr - 0x4000) + (this->_bank * 0x4000)];
	else if (addr >= 0xA000 && addr < 0xC000 && this->_mbcRamAccess())
		return this->_eram[(addr & 0x1FFF) + (this->_rambank * 0x2000)];
	return 0;
}

uint8_t		Rom::_readMbc2(uint16_t addr)
{
	if (addr < 0x4000)
		return this->_rom[addr];
	else if (addr < 0x8000)
		return this->_rom[(addr - 0x4000) + (this->_bank * 0x4000)];
	else if (addr >= 0xA000 && addr < 0xA200 && this->_mbcRamAccess())
		return this->_eram[(addr & 0x01FF)];
	return 0;
}

uint8_t		Rom::_readMbc3(uint16_t addr)
{
	if (addr < 0x4000)
		return this->_rom[addr];
	else if (addr < 0x8000)
		return this->_rom[(addr - 0x4000) + (this->_bank * 0x4000)];
	else if (addr >= 0xA000 && addr < 0xC000 && this->_mbcRamAccess())
	{
		if (this->_rambank <= 0x03 && this->_mbcRamAccess())
			return this->_eram[(addr & 0x1FFF) + (this->_rambank * 0x2000)];
		else if (this->_mbcTimerAccess())
			return *(&this->_timer.reg.rtc_s + (this->_rambank - 0x08));
	}
	return 0;
}

uint8_t		Rom::_readMbc5(uint16_t addr)
{
	if (addr < 0x4000)
		return this->_rom[addr];
	else if (addr < 0x8000)
		return this->_rom[(addr - 0x4000) + (this->_bank * 0x4000)];
	else if (addr >= 0xA000 && addr < 0xC000 && this->_mbcRamAccess())
		return this->_eram[(addr & 0x1FFF) + (this->_rambank * 0x2000)];
	return 0;
}

void		Rom::_writeRom(uint16_t addr, uint8_t val)
{
	(void)addr;
	(void)val;
	return ;
}

void		Rom::_writeMbc1(uint16_t addr, uint8_t val)
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
			val &= 0x1F;
			this->_bank &= 0xE0;
			this->_bank |= val;
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
			if (val <= 0x01)
				this->_tbank = val;
			break;
		case 0xA000:
		case 0xB000:
			// ERAM
			if (this->_mbcRamAccess())
				this->_eram[(addr & 0x1FFF) + (this->_rambank * 0x2000)] = val;
			break;
		default:
			break;
	}
}

void		Rom::_writeMbc2(uint16_t addr, uint8_t val)
{
	switch (addr & 0xF000){
		case 0x0000:
			// RAMCS
			this->_write_protect = val;
			break;
		case 0x2000:
			// Rom bank code
			if ((addr & 0x0F00) == 0x0100)
				this->_bank = val & 0x0F;
			break;
		case 0xA000:
			// ERAM
			if ((addr & 0x0F00) <= 0x0100)
			{
				if (this->_mbcRamAccess())
					this->_eram[(addr & 0x1FF)] = val;
			}
			break;
		default:
			break;
	}
}

void		Rom::_writeMbc3(uint16_t addr, uint8_t val)
{
	switch (addr & 0xF000){
		case 0x0000:
		case 0x1000:
			// RAMCS
			this->_write_protect = val;
			break;
		case 0x2000:
		case 0x3000:
			// ROM BANK CODE
			val &= 0x7F;
			this->_bank &= 0x80;
			this->_bank |= val;
			break;
		case 0x4000:
		case 0x5000:
			// RAM / CLOCK BANK CODE
			if (val <= 0x03 || (val >= 0x08 && val <= 0x0C))
				this->_rambank = val;
			break;
		case 0x6000:
		case 0x7000:
			// LOCK CLOCK
			if (val <= 0x01)
			{
				if (this->_timer.last == false)
					this->_timer.lock = !this->_timer.lock;
				this->_timer.last = !this->_timer.last;
			}
			break;
		case 0xA000:
		case 0xB000:
		case 0xC000:
			if (this->_mbcRamAccess())
			{
				if (this->_rambank <= 0x03)
					this->_eram[(addr & 0x1FFF) + (this->_rambank * 0x2000)] = val;
				else
					*(&this->_timer.reg.rtc_s + (this->_rambank - 0x08)) = val;
			}
			break;
		default:
			break;
	}
}

void		Rom::_writeMbc5(uint16_t addr, uint8_t val)
{
	switch (addr & 0xF000){
		case 0x0000:
		case 0x1000:
			// RAMG
			this->_write_protect = val;
			break;
		case 0x2000:
			// ROMB 0
			val &= 0xFF;
			this->_bank &= 0x0100;
			this->_bank |= val;
			break;
		case 0x3000:
			// ROMB 1
			if (val <= 0x01)
			{
				val <<= 5;
				this->_bank &= 0xFF;
				val &= 0x0100;
				this->_bank |= val;
			}
			break;
		case 0x4000:
		case 0x5000:
			// RAMB
			if (val <= 0x0F)
				this->_rambank = val;
			break;
		case 0xA000:
		case 0xB000:
			// ERAM
			if (this->_mbcRamAccess())
				this->_eram[(addr & 0x1FFF) + (this->_rambank * 0x2000)] = val;
			break;
		default:
			break;
	}
}
