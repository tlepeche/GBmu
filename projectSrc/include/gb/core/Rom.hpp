#ifndef ROM_HPP
#define ROM_HPP

#include "gameboy.hpp"

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
		int				load(char *file);
		char			*read(uint32_t size);
		void			reset(void);
		irom			getType(void);

	private:
		static Rom		_instance;
		char			*_rom;
		uint32_t		_rom_size;
		uint32_t		_pos;

		Rom(void);
		~Rom(void);
};

#endif
