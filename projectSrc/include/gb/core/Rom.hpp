#ifndef ROM_HPP
#define ROM_HPP

#include "htype.hpp"

typedef struct		info_rom
{
	htype			type;
	char			cartridge;
	char			romSize;
	char			eramSize;
}					irom;

class Rom {

	public:
		static Rom		&Instance(void);
		int				load(const char *file);
		bool			isLoaded(void);
		uint8_t			read(uint16_t addr);
		void			write(uint16_t addr, uint8_t val);
		void			reset(void);
		irom			getType(void);

	private:
		static Rom		_instance;
		char			*_rom;
		uint8_t			*_eram;
		irom			_info;
		uint8_t			_nbanks;
		uint8_t			_nrambanks;
		uint8_t			_bank;
		uint8_t			_rambank;
		uint8_t			_tbank;
		uint8_t			_write_protect;

		Rom(void);
		~Rom(void);
		void			init(void);
		uint8_t			getBankRom(uint8_t octet);
		uint8_t			getBankEram(uint8_t octet);
};

#endif
