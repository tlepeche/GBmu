#ifndef ROM_HPP
#define ROM_HPP

#include "htype.hpp"

#define LOGO			0x0104
#define TITLE			0x0134
#define CGBFLAG			0x0143
#define SGBFLAG			0x0146
#define CARTRIDGE		0x0147
#define ROMSIZE			0x0148
#define RAMSIZE			0x0149
#define HEADERCHECKSUM	0x014D
#define GLOBALCHECKSUM	0x014D

class Rom {

	public:
		static Rom		&Instance(void);
		int				load(const char *file);
		htype			getHardware(void);
		bool			isLoaded(void);
		uint8_t			read(uint16_t addr);
		void			write(uint16_t addr, uint8_t val);
		void			reset(void);

	private:
		static Rom		_instance;
		char			*_rom;
		uint8_t			*_eram;
		htype			_hardware;
		uint8_t			_bank;
		uint8_t			_rambank;
		uint8_t			_tbank;
		uint8_t			_write_protect;

		Rom(void);
		~Rom(void);
		void			init(void);
		uint8_t			getBankEram(uint8_t octet);
};

#endif
