#include <iostream>

#ifndef _OPCODE_HPP
# define _OPCODE_HPP

void nop(void);

typedef struct	s_opcode
{
	uint8_t		opcode;
	uint8_t		mask;
	uint8_t		cycleOpcodeFlag;
	uint8_t		cycleOpcodeNoFlag;
	uint8_t		lengthData;
	void		(*functionOpcode)();
	uint16_t	data;
}				t_opcode;

extern t_opcode _opcodeMap[255];

#endif
