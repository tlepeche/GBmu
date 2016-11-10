#ifndef ROM_HPP
#define ROM_HPP

#include "htype.hpp"
#include <array>
#include <functional>

#define LOGO			0x0104
#define TITLE			0x0134
#define CGBFLAG			0x0143
#define SGBFLAG			0x0146
#define CARTRIDGE		0x0147
#define ROMSIZE			0x0148
#define RAMSIZE			0x0149
#define HEADERCHECKSUM	0x014D
#define GLOBALCHECKSUM	0x014D

#define ROM		0x00
#define MBC1	0x01
#define MBC2	0x02
#define MBC3	0x03
#define MBC5	0x04

typedef std::function<uint8_t(uint16_t)> FnReadRom;
typedef std::function<void(uint16_t,uint8_t)> FnWriteRom;

class Rom {

	public:
		static Rom						&Instance(void);
		int								load(const char *file);
		htype							getHardware(void);
		bool							isLoaded(void);
		uint8_t							read(uint16_t addr);
		void							write(uint16_t addr, uint8_t val);
		void							reset(void);

	private:
		static Rom						_instance;
		char							*_rom;
		uint8_t							*_eram;
		htype							_hardware;
		uint8_t							_bank;
		uint8_t							_rambank;
		uint8_t							_tbank;
		uint8_t							_write_protect;
		uint8_t							_mbc;
		std::array<FnReadRom, 5>		_mbcPtrRead;
		std::array<FnWriteRom, 5>		_mbcPtrWrite;

		Rom(void);
		~Rom(void);
		void							init(void);
		uint8_t							getMbc(uint8_t octet);
		uint8_t							getBankEram(uint8_t octet);

		uint8_t							_readRom(uint16_t addr);
		uint8_t							_readMbc1(uint16_t addr);
		uint8_t							_readMbc2(uint16_t addr);
		uint8_t							_readMbc3(uint16_t addr);
		uint8_t							_readMbc5(uint16_t addr);

		void							_writeRom(uint16_t addr, uint8_t val);
		void							_writeMbc1(uint16_t addr, uint8_t val);
		void							_writeMbc2(uint16_t addr, uint8_t val);
		void							_writeMbc3(uint16_t addr, uint8_t val);
		void							_writeMbc5(uint16_t addr, uint8_t val);
};

#endif
