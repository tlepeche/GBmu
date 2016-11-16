#include "Cpu.hpp"

/*
 ** ############################################
 ** GLOBAL METHODS
 ** ############################################
 */

void	Cpu_z80::rotateLeft(uint8_t *reg)
{
	_cpuRegister.c = *reg & 0x80;
	_cpuRegister.n = 0;
	_cpuRegister.h = 0;
	*reg <<= 1;
	if (_cpuRegister.c)
		*reg += 1;
	_cpuRegister.z = (*reg == 0) ? 1 : 0;
}

void	Cpu_z80::rotateRight(uint8_t *reg)
{
	_cpuRegister.c = *reg & 0x01;
	_cpuRegister.n = 0;
	_cpuRegister.h = 0;
	*reg <<= 1;
	if (_cpuRegister.c)
		*reg += 0x80;
	_cpuRegister.z = (*reg == 0) ? 1 : 0;
}

void	Cpu_z80::rotateLeftCarry(uint8_t *reg)
{
	int carry = ((_cpuRegister.c == 1) ? 1 : 0);
	_cpuRegister.c = *reg & 0x01;
	_cpuRegister.n = 0;
	_cpuRegister.h = 0;
	*reg <<= 1;
	*reg += _cpuRegister.c;
	_cpuRegister.z = (*reg == 0) ? 1 : 0;
}

void	Cpu_z80::rotateRightCarry(uint8_t *reg)
{
	int carry = ((_cpuRegister.c == 1) ? 1 : 0) << 7;
	_cpuRegister.c = *reg & 0x01;
	_cpuRegister.n = 0;
	_cpuRegister.h = 0;
	*reg >>= 1;
	*reg += _cpuRegister.c;
	_cpuRegister.z = (*reg == 0) ? 1 : 0;
}

void	Cpu_z80::shiftLeft(uint8_t *reg)
{
	_cpuRegister.c = *reg & 0x80;
	_cpuRegister.n = 0;
	_cpuRegister.h = 0;
	*reg <<= 1;
	*reg &= 0xfe;
	_cpuRegister.z = (*reg == 0) ? 1 : 0;
}

void	Cpu_z80::shiftRight(uint8_t *reg)
{
	uint8_t msb = *reg & 0x80;
	_cpuRegister.c = *reg & 0x01;
	_cpuRegister.n = 0;
	_cpuRegister.h = 0;
	*reg >>= 1;
	*reg += msb;
	_cpuRegister.z = (*reg == 0) ? 1 : 0;
}

void	Cpu_z80::swap(uint8_t *reg)
{
	uint8_t tmp = *reg && 0x0f;
	tmp <<= 4;
	*reg >>= 4;
	*reg += tmp;
	_cpuRegister.c = 0;
	_cpuRegister.n = 0;
	_cpuRegister.h = 0;
	_cpuRegister.z = (*reg == 0) ? 1 : 0;
}

void	Cpu_z80::shiftRightZero(uint8_t *reg)
{
	uint8_t msb = *reg & 0x80;
	_cpuRegister.c = *reg & 0x01;
	_cpuRegister.n = 0;
	_cpuRegister.h = 0;
	*reg >>= 1;
	*reg -= (*reg & 0x80) 0x80 ? 0x80 : 0;
	*reg += msb;
	_cpuRegister.z = (*reg == 0) ? 1 : 0;
}

void	Cpu_z80::bit(uint8_t mask, uint8_t reg)
{
	_cpuRegister.h = 1;
	_cpuRegister.n = 0;
	_cpuRegister.z = (reg & mask) ? 0 : 1;
}

/*
 ** ############################################
 ** CB OPCODE METHODS
 ** ############################################
 */

void	Cpu_z80::RLC_B() //0x00
{
	rotateLeft(&_cpuRegister.B);
}

void	Cpu_z80::RLC_C() //0x01
{
	rotateLeft(&_cpuRegister.C);
}

void	Cpu_z80::RLC_D() //0x02
{
	rotateLeft(&_cpuRegister.D);
}

void	Cpu_z80::RLC_E() //0x03
{
	rotateLeft(&_cpuRegister.E);
}

void	Cpu_z80::RLC_H() //0x04
{
	rotateLeft(&_cpuRegister.H);
}

void	Cpu_z80::RLC_L() //0x05
{
	rotateLeft(&_cpuRegister.L);
}

void	Cpu_z80::RLC_HL() //0x06
{
	uint8_t tmp;

	tmp = _memory->read_byte(_cpuRegister.HL);
	rotateLeft(&tmp);
	_memory->write_byte(_cpuRegister.HL, tmp);
}

void	Cpu_z80::RLC_A() //0x07
{
	rotateRight(&_cpuRegister.A);
}

void	Cpu_z80::RRC_B() //0x08
{
	rotateRight(&_cpuRegister.B);
}

void	Cpu_z80::RRC_C() //0x09
{
	rotateRight(&_cpuRegister.C);
}

void	Cpu_z80::RRC_D() //0x0a
{
	rotateRight(&_cpuRegister.D);
}

void	Cpu_z80::RRC_E() //0x0b
{
	rotateRight(&_cpuRegister.E);
}

void	Cpu_z80::RRC_H() //0x0c
{
	rotateRight(&_cpuRegister.H);
}

void	Cpu_z80::RRC_L() //0x0d
{
	rotateRight(&_cpuRegister.L);
}

void	Cpu_z80::RRC_HL() //0xe
{
	uint8_t tmp;

	tmp = _memory->read_byte(_cpuRegister.HL);
	rotateRight(&tmp);
	_memory->write_byte(_cpuRegister.HL, tmp);
}

void	Cpu_z80::RRC_A() //0x0f
{
	rotateRight(&_cpuRegister.A);
}

void	Cpu_z80::RL_B() //0x10
{
	rotateLeftCarry(&_cpuRegister.B);
}

void	Cpu_z80::RL_C() //0x11
{
	rotateLeftCarry(&_cpuRegister.C);
}

void	Cpu_z80::RL_D() //0x12
{
	rotateLeftCarry(&_cpuRegister.D);
}

void	Cpu_z80::RL_E() //0x13
{
	rotateLeftCarry(&_cpuRegister.E);
}

void	Cpu_z80::RL_H() //0x14
{
	rotateLeftCarry(&_cpuRegister.H);
}

void	Cpu_z80::RL_L() //0x15
{
	rotateLeftCarry(&_cpuRegister.L);
}

void	Cpu_z80::RL_HL() //0x16
{
	uint8_t tmp;

	tmp = _memory->read_byte(_cpuRegister.HL);
	rotateLeftCarry(&tmp);
	_memory->write_byte(_cpuRegister.HL, tmp);
}

void	Cpu_z80::RL_A() //0x17
{
	rotateLeftCarry(&_cpuRegister.A);
}

void	Cpu_z80::RR_B() //0x18
{
	rotateRightCarry(&_cpuRegister.B);
}

void	Cpu_z80::RR_C() //0x19
{
	rotateRightCarry(&_cpuRegister.C);
}

void	Cpu_z80::RR_D() //0x1a
{
	rotateRightCarry(&_cpuRegister.D);
}

void	Cpu_z80::RR_E() //0x1b
{
	rotateRightCarry(&_cpuRegister.E);
}

void	Cpu_z80::RR_H() //0x1c
{
	rotateRightCarry(&_cpuRegister.H);
}

void	Cpu_z80::RR_L() //0x1d
{
	rotateRightCarry(&_cpuRegister.L);
}

void	Cpu_z80::RR_HL() //0x1e
{
	uint8_t tmp;

	tmp = _memory->read_byte(_cpuRegister.HL);
	rotateRightCarry(&tmp);
	_memory->write_byte(_cpuRegister.HL, tmp);
}

void	Cpu_z80::RR_A() //0x1f
{
	rotateRightCarry(&_cpuRegister.A);
}

void	Cpu_z80::SLA_B() //0x20
{
	shiftLeft(&_cpuRegister.B);
}

void	Cpu_z80::SLA_C() //0x21
{
	shiftLeft(&_cpuRegister.C);
}

void	Cpu_z80::SLA_D() //0x22
{
	shiftLeft(&_cpuRegister.D);
}

void	Cpu_z80::SLA_E() //0x23
{
	shiftLeft(&_cpuRegister.E);
}

void	Cpu_z80::SLA_H() //0x24
{
	shiftLeft(&_cpuRegister.H);
}

void	Cpu_z80::SLA_L() //0x25
{
	shiftLeft(&_cpuRegister.L);
}

void	Cpu_z80::SLA_HL() //0x26
{
	uint8_t tmp;

	tmp = _memory->read_byte(_cpuRegister.HL);
	shiftLeft(&tmp);
	_memory->write_byte(_cpuRegister.HL, tmp);
}

void	Cpu_z80::SLA_A() //0x27
{
	shiftLeft(&_cpuRegister.A);
}

void	Cpu_z80::SRA_B() //0x28
{
	shiftRight(&_cpuRegister.B);
}

void	Cpu_z80::SRA_C() //0x29
{
	shiftRight(&_cpuRegister.C);
}

void	Cpu_z80::SRA_D() //0x2a
{
	shiftRight(&_cpuRegister.D);
}

void	Cpu_z80::SRA_E() //0x2b
{
	shiftRight(&_cpuRegister.E);
}

void	Cpu_z80::SRA_H() //0x2c
{
	shiftRight(&_cpuRegister.H);
}

void	Cpu_z80::SRA_L() //0x2d
{
	shiftRight(&_cpuRegister.L);
}

void	Cpu_z80::SRA_HL() //0x2e
{
	uint8_t tmp;

	tmp = _memory->read_byte(_cpuRegister.HL);
	shiftRight(&tmp);
	_memory->write_byte(_cpuRegister.HL, tmp);
}

void	Cpu_z80::SRA_A() //0x2f
{
	shiftRight(&_cpuRegister.A);
}

void	Cpu_z80::SWAP_B() //0x30
{
	swap(&_cpuRegister.B);
}

void	Cpu_z80::SWAP_C() //0x31
{
	swap(&_cpuRegister.C);
}

void	Cpu_z80::SWAP_D() //0x32
{
	swap(&_cpuRegister.D);
}

void	Cpu_z80::SWAP_E() //0x33
{
	swap(&_cpuRegister.E);
}

void	Cpu_z80::SWAP_H() //0x34
{
	swap(&_cpuRegister.H);
}

void	Cpu_z80::SWAP_L() //0x35
{
	swap(&_cpuRegister.L);
}

void	Cpu_z80::SWAP_HL() //0x36
{
	uint8_t tmp;

	tmp = _memory->read_byte(_cpuRegister.HL);
	swap(&tmp);
	_memory->write_byte(_cpuRegister.HL, tmp);
}

void	Cpu_z80::SWAP_A() //0x37
{
	swap(&_cpuRegister.A);
}

void	Cpu_z80::SRL_B() //0x38
{
	shiftRightZero(&_cpuRegister.B);
}

void	Cpu_z80::SRL_C() //0x39
{
	shiftRightZero(&_cpuRegister.C);
}

void	Cpu_z80::SRL_D() //0x3a
{
	shiftRightZero(&_cpuRegister.D);
}

void	Cpu_z80::SRL_E() //0x3b
{
	shiftRightZero(&_cpuRegister.E);
}

void	Cpu_z80::SRL_H() //0x3c
{
	shiftRightZero(&_cpuRegister.H);
}

void	Cpu_z80::SRL_L() //0x3d
{
	shiftRightZero(&_cpuRegister.L);
}

void	Cpu_z80::SRL_HL() //0x3e
{
	uint8_t tmp;

	tmp = _memory->read_byte(_cpuRegister.HL);
	shiftRightZero(&tmp);
	_memory->write_byte(_cpuRegister.HL, tmp);
}

void	Cpu_z80::SRL_A() //0x3f
{
	shiftRightZero(&_cpuRegister.A);
}

void	Cpu_z80::bit0_B() //0x40
{
	bit(0x01, _cpuRegister.B);
}

void	Cpu_z80::bit0_C() //0x41
{
	bit(0x01, _cpuRegister.C);
}

void	Cpu_z80::bit0_D() //0x42
{
	bit(0x01, _cpuRegister.D);
}

void	Cpu_z80::bit0_E() //0x43
{
	bit(0x01, _cpuRegister.E);
}

void	Cpu_z80::bit0_H() //0x44
{
	bit(0x01, _cpuRegister.H);
}

void	Cpu_z80::bit0_L() //0x45
{
	bit(0x01, _cpuRegister.L);
}

void	Cpu_z80::bit0_HL() //0x46
{
	bit(0x01, _memory->read_byte(_cpuRegister.HL));
}

void	Cpu_z80::bit0_A() //0x47
{
	bit(0x01, _cpuRegister.A);
}

void	Cpu_z80::bit1_B() //0x48
{
	bit(0x02, _cpuRegister.B);
}

void	Cpu_z80::bit1_C() //0x49
{
	bit(0x02, _cpuRegister.C);
}

void	Cpu_z80::bit1_D() //0x4a
{
	bit(0x02, _cpuRegister.D);
}

void	Cpu_z80::bit1_E() //0x4b
{
	bit(0x02, _cpuRegister.E);
}

void	Cpu_z80::bit1_H() //0x4c
{
	bit(0x02, _cpuRegister.H);
}

void	Cpu_z80::bit1_L() //0x4d
{
	bit(0x02, _cpuRegister.:);
}

void	Cpu_z80::bit1_HL() //0x4e
{
	bit(0x02, _memory->read_byte(_cpuRegister.HL));
}

void	Cpu_z80::bit1_A() //0x4f
{
	bit(0x02, _cpuRegister.A);
}

void	Cpu_z80::bit2_B() //0x50
{
	bit(0x04, _cpuRegister.B);
}

void	Cpu_z80::bit2_C() //0x51
{
	bit(0x04, _cpuRegister.C);
}

void	Cpu_z80::bit2_D() //0x52
{
	bit(0x04, _cpuRegister.D);
}

void	Cpu_z80::bit2_E() //0x53
{
	bit(0x04, _cpuRegister.E);
}

void	Cpu_z80::bit2_H() //0x54
{
	bit(0x04, _cpuRegister.H);
}

void	Cpu_z80::bit2_L() //0x55
{
	bit(0x04, _cpuRegister.L);
}

void	Cpu_z80::bit2_HL() //0x56
{
	bit(0x04, _memory->read_byte(_cpuRegister.HL));
}

void	Cpu_z80::bit2_A() //0x57
{
	bit(0x04, _cpuRegister.A);
}

void	Cpu_z80::bit3_B() //0x58
{
	bit(0x08, _cpuRegister.B);
}

void	Cpu_z80::bit3_C() //0x59
{
	bit(0x08, _cpuRegister.V);
}

void	Cpu_z80::bit3_D() //0x5a
{
	bit(0x08, _cpuRegister.F);
}

void	Cpu_z80::bit3_E() //0x5b
{
	bit(0x08, _cpuRegister.R);
}

void	Cpu_z80::bit3_H() //0x5c
{
	bit(0x08, _cpuRegister.H);
}

void	Cpu_z80::bit3_L() //0x5d
{
	bit(0x08, _cpuRegister.L);
}

void	Cpu_z80::bit3_HL() //0x5e
{
	bit(0x08, _memory->read_byte(_cpuRegister.HL));
}

void	Cpu_z80::bit3_A() //0x5f
{
	bit(0x08, _cpuRegister.A);
}

void	Cpu_z80::bit4_B() //0x60
{
	bit(0x10, _cpuRegister.B);
}

void	Cpu_z80::bit4_C() //0x61
{
	bit(0x10, _cpuRegister.C);
}

void	Cpu_z80::bit4_D() //0x62
{
	bit(0x10, _cpuRegister.D);
}

void	Cpu_z80::bit4_E() //0x65
{
	bit(0x10, _cpuRegister.E);
}

void	Cpu_z80::bit4_H() //0x64
{
	bit(0x10, _cpuRegister.H);
}

void	Cpu_z80::bit4_L() //0x65
{
	bit(0x10, _cpuRegister.L);
}

void	Cpu_z80::bit4_HL() //0x66
{
	bit(0x10, _memory->read_byte(_cpuRegister.HL));
}

void	Cpu_z80::bit4_A() //0x67
{
	bit(0x10, _cpuRegister.A);
}

void	Cpu_z80::bit5_B() //0x68
{
	bit(0x20, _cpuRegister.B);
}

void	Cpu_z80::bit5_C() //0x69
{
	bit(0x20, _cpuRegister.V);
}

void	Cpu_z80::bit5_D() //0x6a
{
	bit(0x20, _cpuRegister.F);
}

void	Cpu_z80::bit5_E() //0x6b
{
	bit(0x20, _cpuRegister.R);
}

void	Cpu_z80::bit5_H() //0x6c
{
	bit(0x20, _cpuRegister.H);
}

void	Cpu_z80::bit5_L() //0x6d
	bit(0x20, _cpuRegister.L);
}

void	Cpu_z80::bit5_HL() //0x6e
{
	bit(0x20, _memory->read_byte(_cpuRegister.HL));
}

void	Cpu_z80::bit5_A() //0x6f
{
	bit(0x20, _cpuRegister.A);
}

void	Cpu_z80::bit6_B() //0x70
{
	bit(0x40, _cpuRegister.B);
}

void	Cpu_z80::bit6_C() //0x71
{
	bit(0x40, _cpuRegister.C);
}

void	Cpu_z80::bit6_D() //0x72
{
	bit(0x40, _cpuRegister.D);
}

void	Cpu_z80::bit6_E() //0x73
{
	bit(0x40, _cpuRegister.E);
}

void	Cpu_z80::bit6_H() //0x74
{
	bit(0x40, _cpuRegister.H);
}

void	Cpu_z80::bit6_L() //0x75
{
	bit(0x40, _cpuRegister.L);
}

void	Cpu_z80::bit6_HL() //0x76
{
	bit(0x40, _memory->read_byte(_cpuRegister.HL));
}

void	Cpu_z80::bit6_A() //0x77
{
	bit(0x40, _cpuRegister.A);
}

void	Cpu_z80::bit7_B() //0x78
{
	bit(0x80, _cpuRegister.B);
}

void	Cpu_z80::bit7_C() //0x79
{
	bit(0x80, _cpuRegister.V);
}

void	Cpu_z80::bit7_D() //0x7a
{
	bit(0x80, _cpuRegister.F);
}

void	Cpu_z80::bit7_E() //0x7b
{
	bit(0x80, _cpuRegister.R);
}

void	Cpu_z80::bit7_H() //0x7c
{
	bit(0x80, _cpuRegister.H);
}

void	Cpu_z80::bit7_L() //0x7d
{
	bit(0x80, _cpuRegister.L);
}

void	Cpu_z80::bit7_HL() //0x7e
{
	bit(0x80, _memory->read_byte(_cpuRegister.HL));
}

void	Cpu_z80::bit7_A() //0x7f
{
	bit(0x80, _cpuRegister.A);
}










void Cpu_z80::_setCbOpcodeMap()
{
	_CBopcodeMap = {
		(t_opcode){0x00, 0x00, 8, 8, 2, std::bind(&Cpu_z80::RLC_B, this),	"RLC B",	0x0000},
		(t_opcode){0x01, 0x00, 8, 8, 2, std::bind(&Cpu_z80::RLC_C, this),	"RLC C",	0x0000},
		(t_opcode){0x02, 0x00, 8, 8, 2, std::bind(&Cpu_z80::RLC_D, this),	"RLC D",	0x0000},
		(t_opcode){0x03, 0x00, 8, 8, 2, std::bind(&Cpu_z80::RLC_E, this),	"RLC E",	0x0000},
		(t_opcode){0x04, 0x00, 8, 8, 2, std::bind(&Cpu_z80::RLC_H, this),	"RLC H",	0x0000},
		(t_opcode){0x05, 0x00, 8, 8, 2, std::bind(&Cpu_z80::RLC_L, this),	"RLC L",	0x0000},
		(t_opcode){0x06, 0x00, 8, 8, 2, std::bind(&Cpu_z80::RLC_HL, this),  "RLC HL",	0x0000},
		(t_opcode){0x07, 0x00, 8, 8, 2, std::bind(&Cpu_z80::RLC_A, this),	"RLC A,		0x0000},
		(t_opcode){0x08, 0x00, 8, 8, 2, std::bind(&Cpu_z80::RRC_B, this),   "RRC B",	0x0000},
		(t_opcode){0x09, 0x00, 8, 8, 2, std::bind(&Cpu_z80::RRC_C, this),	"RRC C",	0x0000},
		(t_opcode){0x0a, 0x00, 8, 8, 2, std::bind(&Cpu_z80::RRC_D, this),   "RRC D",	0x0000},
		(t_opcode){0x0b, 0x00, 8, 8, 2, std::bind(&Cpu_z80::RRC_E, this),   "RRC E",	0x0000},
		(t_opcode){0x0c, 0x00, 8, 8, 2, std::bind(&Cpu_z80::RRC_H, this),   "RRC H",	0x0000},
		(t_opcode){0x0d, 0x00, 8, 8, 2, std::bind(&Cpu_z80::RRC_L, this),   "RRC L",	0x0000},
		(t_opcode){0x0e, 0x00, 8, 8, 2, std::bind(&Cpu_z80::RRC_HL, this),  "RRC HL",	0x0000},
		(t_opcode){0x0f, 0x00, 8, 8, 2, std::bind(&Cpu_z80::RRC_A, this),   "RRC A",	0x0000},
		(t_opcode){0x10, 0x00, 8, 8, 2, std::bind(&Cpu_z80::RL_B, this),	"RL B",		0x0000},
		(t_opcode){0x11, 0x00, 8, 8, 2, std::bind(&Cpu_z80::RL_C, this),	"RL C",		0x0000},
		(t_opcode){0x12, 0x00, 8, 8, 2, std::bind(&Cpu_z80::RL_D, this),  	"RL D",		0x0000},
		(t_opcode){0x13, 0x00, 8, 8, 2, std::bind(&Cpu_z80::RL_E, this),  	"RL E",		0x0000},
		(t_opcode){0x14, 0x00, 8, 8, 2, std::bind(&Cpu_z80::RL_H, this),  	"RL H",		0x0000},
		(t_opcode){0x15, 0x00, 8, 8, 2, std::bind(&Cpu_z80::RL_L, this),  	"RL L",		0x0000},
		(t_opcode){0x16, 0x00, 8, 8, 2, std::bind(&Cpu_z80::RL_HL, this),  	"RL HL",	0x0000},
		(t_opcode){0x17, 0x00, 8, 8, 2, std::bind(&Cpu_z80::RL_A, this),  	"RL A",		0x0000},
		(t_opcode){0x18, 0x00, 8, 8, 2, std::bind(&Cpu_z80::RR_B, this),  	"RR B",		0x0000},
		(t_opcode){0x19, 0x00, 8, 8, 2, std::bind(&Cpu_z80::RR_C, this),	"RR C",		0x0000},
		(t_opcode){0x1a, 0x00, 8, 8, 2, std::bind(&Cpu_z80::RR_D, this), 	"RR D",		0x0000},
		(t_opcode){0x1b, 0x00, 8, 8, 2, std::bind(&Cpu_z80::RR_E, this),   	"RR E",		0x0000},
		(t_opcode){0x1c, 0x00, 8, 8, 2, std::bind(&Cpu_z80::RR_H, this),   	"RR H",		0x0000},
		(t_opcode){0x1d, 0x00, 8, 8, 2, std::bind(&Cpu_z80::RR_L, this),  	"RR L",		0x0000},
		(t_opcode){0x1e, 0x00, 8, 8, 2, std::bind(&Cpu_z80::RR_HL, this),   "RR HL",	0x0000},
		(t_opcode){0x1f, 0x00, 8, 8, 2, std::bind(&Cpu_z80::RR_A, this),   	"RR A",		0x0000},
		(t_opcode){0x20, 0x00, 8, 8, 2, std::bind(&Cpu_z80::SLA_B, this),   "SLA B",	0x0000},
		(t_opcode){0x21, 0x00, 8, 8, 2, std::bind(&Cpu_z80::SLA_C, this), 	"SLA C",	0x0000},
		(t_opcode){0x22, 0x00, 8, 8, 2, std::bind(&Cpu_z80::SLA_D, this), 	"SLA D",	0x0000},
		(t_opcode){0x23, 0x00, 8, 8, 2, std::bind(&Cpu_z80::SLA_E, this),   "SLA E",	0x0000},
		(t_opcode){0x24, 0x00, 8, 8, 2, std::bind(&Cpu_z80::SLA_H, this),   "SLA H",	0x0000},
		(t_opcode){0x25, 0x00, 8, 8, 2, std::bind(&Cpu_z80::SLA_L, this),   "SLA L",	0x0000},
		(t_opcode){0x26, 0x00, 8, 8, 2, std::bind(&Cpu_z80::SLA_HL, this),  "SLA HL",	0x0000},
		(t_opcode){0x27, 0x00, 8, 8, 2, std::bind(&Cpu_z80::SLA_A, this),   "SLA A",	0x0000},
		(t_opcode){0x28, 0x00, 8, 8, 2, std::bind(&Cpu_z80::SRA_B, this),   "SRA B",	0x0000},
		(t_opcode){0x29, 0x00, 8, 8, 2, std::bind(&Cpu_z80::SRA_C, this),	"SRA C",	0x0000},
		(t_opcode){0x2a, 0x00, 8, 8, 2, std::bind(&Cpu_z80::SRA_D, this), 	"SRA D",	0x0000},
		(t_opcode){0x2b, 0x00, 8, 8, 2, std::bind(&Cpu_z80::SRA_E, this),   "SRA E",	0x0000},
		(t_opcode){0x2c, 0x00, 8, 8, 2, std::bind(&Cpu_z80::SRA_H, this),   "SRA H",	0x0000},
		(t_opcode){0x2d, 0x00, 8, 8, 2, std::bind(&Cpu_z80::SRA_L, this),   "SRA L",	0x0000},
		(t_opcode){0x2e, 0x00, 8, 8, 2, std::bind(&Cpu_z80::SRA_HL, this),  "SRA Hl",	0x0000},
		(t_opcode){0x2f, 0x00, 8, 8, 2, std::bind(&Cpu_z80::SRA_Q, this),   "SRA A",	0x0000},
		(t_opcode){0x30, 0x00, 8, 8, 2, std::bind(&Cpu_z80::SWAP_B, this),  "SWAP B",	0x0000},
		(t_opcode){0x31, 0x00, 8, 8, 2, std::bind(&Cpu_z80::SWAP_C, this),  "SWAP C",	0x0000},
		(t_opcode){0x32, 0x00, 8, 8, 2, std::bind(&Cpu_z80::SWAP_D, this),	"SWAP D",	0x0000},
		(t_opcode){0x33, 0x00, 8, 8, 2, std::bind(&Cpu_z80::SWAP_E, this),  "SWAP E",	0x0000},
		(t_opcode){0x34, 0x00, 8, 8, 2, std::bind(&Cpu_z80::SWAP_H, this),  "SWAP H",	0x0000},
		(t_opcode){0x35, 0x00, 8, 8, 2, std::bind(&Cpu_z80::SWAP_L, this),  "SWAP L",	0x0000},
		(t_opcode){0x36, 0x00, 8, 8, 2, std::bind(&Cpu_z80::SWAP_HL, this), "SWAP HL",	0x0000},
		(t_opcode){0x37, 0x00, 8, 8, 2, std::bind(&Cpu_z80::SWAP_A, this),  "SWAP A",	0x0000},
		(t_opcode){0x38, 0x00, 8, 8, 2, std::bind(&Cpu_z80::SRL_B, this),   "SRL B",	0x0000},
		(t_opcode){0x39, 0x00, 8, 8, 2, std::bind(&Cpu_z80::SRL_C, this),	"SRL C",	0x0000},
		(t_opcode){0x3a, 0x00, 8, 8, 2, std::bind(&Cpu_z80::SRL_D, this), 	"SRL D",	0x0000},
		(t_opcode){0x3b, 0x00, 8, 8, 2, std::bind(&Cpu_z80::SRL_E, this),   "SRL E",	0x0000},
		(t_opcode){0x3c, 0x00, 8, 8, 2, std::bind(&Cpu_z80::SRL_H, this),   "SRL H",	0x0000},
		(t_opcode){0x3d, 0x00, 8, 8, 2, std::bind(&Cpu_z80::SRL_L, this),   "SRL L",	0x0000},
		(t_opcode){0x3e, 0x00, 8, 8, 2, std::bind(&Cpu_z80::SRL_HL, this),  "SRL HL",	0x0000},
		(t_opcode){0x3f, 0x00, 8, 8, 2, std::bind(&Cpu_z80::SRL_A, this),   "SRL A",	0x0000},
		(t_opcode){0x40, 0x00, 8, 8, 2, std::bind(&Cpu_z80::BIT0_B, this),  "BIT 0 B",	0x0000},
		(t_opcode){0x41, 0x00, 8, 8, 2, std::bind(&Cpu_z80::BIT0_C, this),  "BIT 0 C",	0x0000},
		(t_opcode){0x42, 0x00, 8, 8, 2, std::bind(&Cpu_z80::BIT0_D, this),  "BIT 0 D",	0x0000},
		(t_opcode){0x43, 0x00, 8, 8, 2, std::bind(&Cpu_z80::BIT0_E, this),  "BIT 0 E",	0x0000},
		(t_opcode){0x44, 0x00, 8, 8, 2, std::bind(&Cpu_z80::BIT0_H, this),  "BIT 0 H",	0x0000},
		(t_opcode){0x45, 0x00, 8, 8, 2, std::bind(&Cpu_z80::BIT0_L, this),  "BIT 0 L",	0x0000},
		(t_opcode){0x46, 0x00, 8, 8, 2, std::bind(&Cpu_z80::BIT0_HL, this), "BIT 0 HL",	0x0000},
		(t_opcode){0x47, 0x00, 8, 8, 2, std::bind(&Cpu_z80::BIT0_A, this),  "BIT 0 A",	0x0000},
		(t_opcode){0x48, 0x00, 8, 8, 2, std::bind(&Cpu_z80::BIT3_B, this),  "BIT 1 B",	0x0000},
		(t_opcode){0x49, 0x00, 8, 8, 2, std::bind(&Cpu_z80::BIT3_C, this),  "BIT 1 C",	0x0000},
		(t_opcode){0x4a, 0x00, 8, 8, 2, std::bind(&Cpu_z80::BIT3_D, this),  "BIT 1 D",	0x0000},
		(t_opcode){0x4b, 0x00, 8, 8, 2, std::bind(&Cpu_z80::BIT3_E, this),  "BIT 1 E",	0x0000},
		(t_opcode){0x4c, 0x00, 8, 8, 2, std::bind(&Cpu_z80::BIT3_H, this),  "BIT 1 H",	0x0000},
		(t_opcode){0x4d, 0x00, 8, 8, 2, std::bind(&Cpu_z80::BIT3_L, this),  "BIT 1 L",	0x0000},
		(t_opcode){0x4e, 0x00, 8, 8, 2, std::bind(&Cpu_z80::BIT3_HL, this), "BIT 1 HL",	0x0000},
		(t_opcode){0x4f, 0x00, 8, 8, 2, std::bind(&Cpu_z80::BIT3_A, this),  "BIT 1 A",	0x0000},
		(t_opcode){0x50, 0x00, 8, 8, 2, std::bind(&Cpu_z80::BIT2_B, this),  "BIT 2 B",	0x0000},
		(t_opcode){0x51, 0x00, 8, 8, 2, std::bind(&Cpu_z80::BIT2_C, this),  "BIT 2 C",	0x0000},
		(t_opcode){0x52, 0x00, 8, 8, 2, std::bind(&Cpu_z80::BIT2_D, this),  "BIT 2 D",	0x0000},
		(t_opcode){0x53, 0x00, 8, 8, 2, std::bind(&Cpu_z80::BIT2_E, this),  "BIT 2 E",	0x0000},
		(t_opcode){0x54, 0x00, 8, 8, 2, std::bind(&Cpu_z80::BIT2_H, this),  "BIT 2 H",	0x0000},
		(t_opcode){0x55, 0x00, 8, 8, 2, std::bind(&Cpu_z80::BIT2_L, this),  "BIT 2 L",	0x0000},
		(t_opcode){0x56, 0x00, 8, 8, 2, std::bind(&Cpu_z80::BIT2_HL, this), "BIT 2 HL",	0x0000},
		(t_opcode){0x57, 0x00, 8, 8, 2, std::bind(&Cpu_z80::BIT2_A, this),  "BIT 2 A",	0x0000},
		(t_opcode){0x58, 0x00, 8, 8, 2, std::bind(&Cpu_z80::BIT3_B, this),  "BIT 3 B",	0x0000},
		(t_opcode){0x59, 0x00, 8, 8, 2, std::bind(&Cpu_z80::BIT3_C, this),  "BIT 3 C",	0x0000},
		(t_opcode){0x5a, 0x00, 8, 8, 2, std::bind(&Cpu_z80::BIT3_D, this),  "BIT 3 D",	0x0000},
		(t_opcode){0x5b, 0x00, 8, 8, 2, std::bind(&Cpu_z80::BIT3_E, this),  "BIT 3 E",	0x0000},
		(t_opcode){0x5c, 0x00, 8, 8, 2, std::bind(&Cpu_z80::BIT3_H, this),  "BIT 3 H",	0x0000},
		(t_opcode){0x5d, 0x00, 8, 8, 2, std::bind(&Cpu_z80::BIT3_L, this),  "BIT 3 L",	0x0000},
		(t_opcode){0x5e, 0x00, 8, 8, 2, std::bind(&Cpu_z80::BIT3_HL, this), "BIT 3 HL",	0x0000},
		(t_opcode){0x5f, 0x00, 8, 8, 2, std::bind(&Cpu_z80::BIT3_A, this),  "BIT 3 A",	0x0000},
		(t_opcode){0x60, 0x00, 8, 8, 2, std::bind(&Cpu_z80::BIT4_B, this),  "BIT 4 B",	0x0000},
		(t_opcode){0x61, 0x00, 8, 8, 2, std::bind(&Cpu_z80::BIT4_C, this),  "BIT 4 C",	0x0000},
		(t_opcode){0x62, 0x00, 8, 8, 2, std::bind(&Cpu_z80::BIT4_D, this),  "BIT 4 D",	0x0000},
		(t_opcode){0x63, 0x00, 8, 8, 2, std::bind(&Cpu_z80::BIT4_E, this),  "BIT 4 E",	0x0000},
		(t_opcode){0x64, 0x00, 8, 8, 2, std::bind(&Cpu_z80::BIT4_H, this),  "BIT 4 H",	0x0000},
		(t_opcode){0x65, 0x00, 8, 8, 2, std::bind(&Cpu_z80::BIT4_L, this),  "BIT 4 L",	0x0000},
		(t_opcode){0x66, 0x00, 8, 8, 2, std::bind(&Cpu_z80::BIT4_HL, this), "BIT 4 HL",	0x0000},
		(t_opcode){0x67, 0x00, 8, 8, 2, std::bind(&Cpu_z80::BIT4_A, this),  "BIT 4 A",	0x0000},
		(t_opcode){0x68, 0x00, 8, 8, 2, std::bind(&Cpu_z80::BIT5_B, this),  "BIT 5 B",	0x0000},
		(t_opcode){0x69, 0x00, 8, 8, 2, std::bind(&Cpu_z80::BIT5_C, this),  "BIT 5 C",	0x0000},
		(t_opcode){0x6a, 0x00, 8, 8, 2, std::bind(&Cpu_z80::BIT5_D, this),  "BIT 5 D",	0x0000},
		(t_opcode){0x6b, 0x00, 8, 8, 2, std::bind(&Cpu_z80::BIT5_E, this),  "BIT 5 E",	0x0000},
		(t_opcode){0x6c, 0x00, 8, 8, 2, std::bind(&Cpu_z80::BIT5_H, this),  "BIT 5 H",	0x0000},
		(t_opcode){0x6d, 0x00, 8, 8, 2, std::bind(&Cpu_z80::BIT5_L, this),  "BIT 5 L",	0x0000},
		(t_opcode){0x6e, 0x00, 8, 8, 2, std::bind(&Cpu_z80::BIT5_HL, this), "BIT 5 HL",	0x0000},
		(t_opcode){0x6f, 0x00, 8, 8, 2, std::bind(&Cpu_z80::BIT5_A, this),  "BIT 5 A",	0x0000},
		(t_opcode){0x70, 0x00, 8, 8, 2, std::bind(&Cpu_z80::BIT6_B, this),  "BIT 6 B",	0x0000},
		(t_opcode){0x71, 0x00, 8, 8, 2, std::bind(&Cpu_z80::BIT6_C, this),  "BIT 6 C",	0x0000},
		(t_opcode){0x72, 0x00, 8, 8, 2, std::bind(&Cpu_z80::BIT6_D, this),  "BIT 6 D",	0x0000},
		(t_opcode){0x73, 0x00, 8, 8, 2, std::bind(&Cpu_z80::BIT6_E, this),  "BIT 6 E",	0x0000},
		(t_opcode){0x74, 0x00, 8, 8, 2, std::bind(&Cpu_z80::BIT6_H, this),  "BIT 6 H",	0x0000},
		(t_opcode){0x75, 0x00, 8, 8, 2, std::bind(&Cpu_z80::BIT6_L, this),  "BIT 6 L",	0x0000},
		(t_opcode){0x76, 0x00, 8, 8, 2, std::bind(&Cpu_z80::BIT6_HL, this), "BIT 6 HL",	0x0000},
		(t_opcode){0x77, 0x00, 8, 8, 2, std::bind(&Cpu_z80::BIT6_A, this),  "BIT 6 A",	0x0000},
		(t_opcode){0x78, 0x00, 8, 8, 2, std::bind(&Cpu_z80::BIT7_B, this),  "BIT 7 B",	0x0000},
		(t_opcode){0x79, 0x00, 8, 8, 2, std::bind(&Cpu_z80::BIT7_C, this),  "BIT 7 C",	0x0000},
		(t_opcode){0x7a, 0x00, 8, 8, 2, std::bind(&Cpu_z80::BIT7_D, this),  "BIT 7 D",	0x0000},
		(t_opcode){0x7b, 0x00, 8, 8, 2, std::bind(&Cpu_z80::BIT7_E, this),  "BIT 7 E",	0x0000},
		(t_opcode){0x7c, 0x00, 8, 8, 2, std::bind(&Cpu_z80::BIT7_H, this),  "BIT 7 H",	0x0000},
		(t_opcode){0x7d, 0x00, 8, 8, 2, std::bind(&Cpu_z80::BIT7_L, this),  "BIT 7 L",	0x0000},
		(t_opcode){0x7e, 0x00, 8, 8, 2, std::bind(&Cpu_z80::BIT7_HL, this), "BIT 7 HL",	0x0000},
		(t_opcode){0x7f, 0x00, 8, 8, 2, std::bind(&Cpu_z80::BIT7_A, this),  "BIT 7 A",	0x0000},
		(t_opcode){0x80, 0x00, 8, 8, 2, std::bind(&Cpu_z80::RES0_B, this),  "RES 0 B",	0x0000},
		(t_opcode){0x81, 0x00, 8, 8, 2, std::bind(&Cpu_z80::RES0_C, this),  "RES 0 C",	0x0000},
		(t_opcode){0x82, 0x00, 8, 8, 2, std::bind(&Cpu_z80::RES0_D, this),  "RES 0 D",	0x0000},
		(t_opcode){0x83, 0x00, 8, 8, 2, std::bind(&Cpu_z80::RES0_E, this),  "RES 0 E",	0x0000},
		(t_opcode){0x84, 0x00, 8, 8, 2, std::bind(&Cpu_z80::RES0_H, this),  "RES 0 H",	0x0000},
		(t_opcode){0x85, 0x00, 8, 8, 2, std::bind(&Cpu_z80::RES0_L, this),  "RES 0 L",	0x0000},
		(t_opcode){0x86, 0x00, 8, 8, 2, std::bind(&Cpu_z80::RES0_HL, this), "RES 0 HL",	0x0000},
		(t_opcode){0x87, 0x00, 8, 8, 2, std::bind(&Cpu_z80::RES0_A, this),  "RES 0 A",	0x0000},
		(t_opcode){0x88, 0x00, 8, 8, 2, std::bind(&Cpu_z80::RES1_B, this),  "RES 1 B",	0x0000},
		(t_opcode){0x89, 0x00, 8, 8, 2, std::bind(&Cpu_z80::RES1_C, this),  "RES 1 C",	0x0000},
		(t_opcode){0x8a, 0x00, 8, 8, 2, std::bind(&Cpu_z80::RES1_D, this),  "RES 1 D",	0x0000},
		(t_opcode){0x8b, 0x00, 8, 8, 2, std::bind(&Cpu_z80::RES1_E, this),  "RES 1 E",	0x0000},
		(t_opcode){0x8c, 0x00, 8, 8, 2, std::bind(&Cpu_z80::RES1_H, this),  "RES 1 H",	0x0000},
		(t_opcode){0x8d, 0x00, 8, 8, 2, std::bind(&Cpu_z80::RES1_L, this),  "RES 1 L",	0x0000},
		(t_opcode){0x8e, 0x00, 8, 8, 2, std::bind(&Cpu_z80::RES1_HL, this), "RES 1 HL",	0x0000},
		(t_opcode){0x8f, 0x00, 8, 8, 2, std::bind(&Cpu_z80::RES1_A, this),  "RES 1 A",	0x0000},
		(t_opcode){0x90, 0x00, 8, 8, 2, std::bind(&Cpu_z80::RES2_B, this),  "RES 2 B",	0x0000},
		(t_opcode){0x91, 0x00, 8, 8, 2, std::bind(&Cpu_z80::RES2_C, this),  "RES 2 C",	0x0000},
		(t_opcode){0x92, 0x00, 8, 8, 2, std::bind(&Cpu_z80::RES2_D, this),  "RES 2 D",	0x0000},
		(t_opcode){0x93, 0x00, 8, 8, 2, std::bind(&Cpu_z80::RES2_E, this),  "RES 2 E",	0x0000},
		(t_opcode){0x94, 0x00, 8, 8, 2, std::bind(&Cpu_z80::RES2_H, this),  "RES 2 H",	0x0000},
		(t_opcode){0x95, 0x00, 8, 8, 2, std::bind(&Cpu_z80::RES2_L, this),  "RES 2 L",	0x0000},
		(t_opcode){0x96, 0x00, 8, 8, 2, std::bind(&Cpu_z80::RES2_HL, this), "RES 2 HL",	0x0000},
		(t_opcode){0x97, 0x00, 8, 8, 2, std::bind(&Cpu_z80::RES2_A, this),  "RES 2 A",	0x0000},
		(t_opcode){0x98, 0x00, 8, 8, 2, std::bind(&Cpu_z80::RES3_B, this),  "RES 3 B",	0x0000},
		(t_opcode){0x99, 0x00, 8, 8, 2, std::bind(&Cpu_z80::RES3_C, this),  "RES 3 C",	0x0000},
		(t_opcode){0x9a, 0x00, 8, 8, 2, std::bind(&Cpu_z80::RES3_D, this),  "RES 3 D",	0x0000},
		(t_opcode){0x9b, 0x00, 8, 8, 2, std::bind(&Cpu_z80::RES3_E, this),  "RES 3 E",	0x0000},
		(t_opcode){0x9c, 0x00, 8, 8, 2, std::bind(&Cpu_z80::RES3_H, this),  "RES 3 H",	0x0000},
		(t_opcode){0x9d, 0x00, 8, 8, 2, std::bind(&Cpu_z80::RES3_L, this),  "RES 3 L",	0x0000},
		(t_opcode){0x9e, 0x00, 8, 8, 2, std::bind(&Cpu_z80::RES3_HL, this), "RES 3 HL,	0x0000},
		(t_opcode){0x9f, 0x00, 8, 8, 2, std::bind(&Cpu_z80::RES3_A, this),  "RES 3 A",	0x0000},
		(t_opcode){0xa0, 0x00, 8, 8, 2, std::bind(&Cpu_z80::RES4_B, this),  "RES 4 B",	0x0000},
		(t_opcode){0xa1, 0x00, 8, 8, 2, std::bind(&Cpu_z80::RES4_C, this),  "RES 4 C",	0x0000},
		(t_opcode){0xa2, 0x00, 8, 8, 2, std::bind(&Cpu_z80::RES4_D, this),  "RES 4 D",	0x0000},
		(t_opcode){0xa3, 0x00, 8, 8, 2, std::bind(&Cpu_z80::RES4_E, this),  "RES 4 E",	0x0000},
		(t_opcode){0xa4, 0x00, 8, 8, 2, std::bind(&Cpu_z80::RES4_H, this),  "RES 4 H",	0x0000},
		(t_opcode){0xa5, 0x00, 8, 8, 2, std::bind(&Cpu_z80::RES4_L, this),  "RES 4 L",	0x0000},
		(t_opcode){0xa6, 0x00, 8, 8, 2, std::bind(&Cpu_z80::RES4_HL, this), "RES 4 HL",	0x0000},
		(t_opcode){0xa7, 0x00, 8, 8, 2, std::bind(&Cpu_z80::RES4_A, this),  "RES 4 A",	0x0000},
		(t_opcode){0xa8, 0x00, 8, 8, 2, std::bind(&Cpu_z80::RES5_B, this),  "RES 5 B",	0x0000},
		(t_opcode){0xa9, 0x00, 8, 8, 2, std::bind(&Cpu_z80::RES5_C, this),  "RES 5 C",	0x0000},
		(t_opcode){0xaa, 0x00, 8, 8, 2, std::bind(&Cpu_z80::RES5_D, this),  "RES 5 D",	0x0000},
		(t_opcode){0xab, 0x00, 8, 8, 2, std::bind(&Cpu_z80::RES5_E, this),  "RES 5 E",	0x0000},
		(t_opcode){0xac, 0x00, 8, 8, 2, std::bind(&Cpu_z80::RES5_H, this),  "RES 5 H",	0x0000},
		(t_opcode){0xad, 0x00, 8, 8, 2, std::bind(&Cpu_z80::RES5_L, this),  "RES 5 L",	0x0000},
		(t_opcode){0xae, 0x00, 8, 8, 2, std::bind(&Cpu_z80::RES5_HL, this), "RES 5 HL",	0x0000},
		(t_opcode){0xaf, 0x00, 8, 8, 2, std::bind(&Cpu_z80::RES5_A, this),  "RES 5 A",	0x0000},
		(t_opcode){0xb0, 0x00, 8, 8, 2, std::bind(&Cpu_z80::RES6_B, this),  "RES 6 B",	0x0000},
		(t_opcode){0xb1, 0x00, 8, 8, 2, std::bind(&Cpu_z80::RES6_C, this),  "RES 6 C",	0x0000},
		(t_opcode){0xb2, 0x00, 8, 8, 2, std::bind(&Cpu_z80::RES6_D, this),  "RES 6 D",	0x0000},
		(t_opcode){0xb3, 0x00, 8, 8, 2, std::bind(&Cpu_z80::RES6_E, this),  "RES 6 E",	0x0000},
		(t_opcode){0xb4, 0x00, 8, 8, 2, std::bind(&Cpu_z80::RES6_H, this),  "RES 6 H",	0x0000},
		(t_opcode){0xb5, 0x00, 8, 8, 2, std::bind(&Cpu_z80::RES6_L, this),  "RES 6 L",	0x0000},
		(t_opcode){0xb6, 0x00, 8, 8, 2, std::bind(&Cpu_z80::RES6_HL, this), "RES 6 HL",	0x0000},
		(t_opcode){0xb7, 0x00, 8, 8, 2, std::bind(&Cpu_z80::RES6_A, this), 	"RES 6 A",	0x0000},
		(t_opcode){0xb8, 0x00, 8, 8, 2, std::bind(&Cpu_z80::RES7_B, this),  "RES 7 B",	0x0000},
		(t_opcode){0xb9, 0x00, 8, 8, 2, std::bind(&Cpu_z80::RES7_C, this),  "RES 7 C",	0x0000},
		(t_opcode){0xba, 0x00, 8, 8, 2, std::bind(&Cpu_z80::RES7_D, this),  "RES 7 D",	0x0000},
		(t_opcode){0xbb, 0x00, 8, 8, 2, std::bind(&Cpu_z80::RES7_E, this),  "RES 7 E",	0x0000},
		(t_opcode){0xbc, 0x00, 8, 8, 2, std::bind(&Cpu_z80::RES7_H, this),  "RES 7 H",	0x0000},
		(t_opcode){0xbd, 0x00, 8, 8, 2, std::bind(&Cpu_z80::RES7_L, this),  "RES 7 L",	0x0000},
		(t_opcode){0xbe, 0x00, 8, 8, 2, std::bind(&Cpu_z80::RES7_HL, this), "RES 7 HL",	0x0000},
		(t_opcode){0xbf, 0x00, 8, 8, 2, std::bind(&Cpu_z80::RES7_A, this), 	"RES 7 A",	0x0000},
		(t_opcode){0xc0, 0x00, 8, 8, 2, std::bind(&Cpu_z80::SET0_B, this),  "SET 0 B",	0x0000},
		(t_opcode){0xc1, 0x00, 8, 8, 2, std::bind(&Cpu_z80::SET0_C, this),  "SET 0 C",	0x0000},
		(t_opcode){0xc2, 0x00, 8, 8, 2, std::bind(&Cpu_z80::SET0_D, this),  "SET 0 D",	0x0000},
		(t_opcode){0xc3, 0x00, 8, 8, 2, std::bind(&Cpu_z80::SET0_E, this),  "SET 0 E",	0x0000},
		(t_opcode){0xc4, 0x00, 8, 8, 2, std::bind(&Cpu_z80::SET0_H, this),	"SET 0 H",	0x0000},
		(t_opcode){0xc5, 0x00, 8, 8, 2, std::bind(&Cpu_z80::SET0_L, this),  "SET 0 L",	0x0000},
		(t_opcode){0xc6, 0x00, 8, 8, 2, std::bind(&Cpu_z80::SET0_HL, this), "SET 0 HL",	0x0000},
		(t_opcode){0xc7, 0x00, 8, 8, 2, std::bind(&Cpu_z80::SET0_A, this),  "SET 0 A",	0x0000},
		(t_opcode){0xc8, 0x00, 8, 8, 2, std::bind(&Cpu_z80::SET1_B, this),  "SET 1 B",	0x0000},
		(t_opcode){0xc9, 0x00, 8, 8, 2, std::bind(&Cpu_z80::SET1_C, this),  "SET 1 C",	0x0000},
		(t_opcode){0xca, 0x00, 8, 8, 2, std::bind(&Cpu_z80::SET1_D, this),  "SET 1 D",	0x0000},
		(t_opcode){0xcb, 0x00, 8, 8, 2, std::bind(&Cpu_z80::SET1_E, this),	"SET 1 E",	0x0000},
		(t_opcode){0xcc, 0x00, 8, 8, 2, std::bind(&Cpu_z80::SET1_H, this), 	"SET 1 H",	0x0000},
		(t_opcode){0xcd, 0x00, 8, 8, 2, std::bind(&Cpu_z80::SET1_L, this),  "SET 1 L",	0x0000},
		(t_opcode){0xce, 0x00, 8, 8, 2, std::bind(&Cpu_z80::SET1_HL, this), "SET 1 HL",	0x0000},
		(t_opcode){0xcf, 0x00, 8, 8, 2, std::bind(&Cpu_z80::SET1_A, this),  "SET 1 A",	0x0000},
		(t_opcode){0xd0, 0x00, 8, 8, 2, std::bind(&Cpu_z80::SET2_B, this),  "SET 2 B",	0x0000},
		(t_opcode){0xd1, 0x00, 8, 8, 2, std::bind(&Cpu_z80::SET2_C, this),  "SET 2 C",	0x0000},
		(t_opcode){0xd2, 0x00, 8, 8, 2, std::bind(&Cpu_z80::SET2_D, this),  "SET 2 D",	0x0000},
		(t_opcode){0xd3, 0x00, 8, 8, 2, std::bind(&Cpu_z80::SET2_E, this),  "SET 2 E",	0x0000},
		(t_opcode){0xd4, 0x00, 8, 8, 2, std::bind(&Cpu_z80::SET2_H, this),	"SET 2 H",	0x0000},
		(t_opcode){0xd5, 0x00, 8, 8, 2, std::bind(&Cpu_z80::SET2_L, this),  "SET 2 L",	0x0000},
		(t_opcode){0xd6, 0x00, 8, 8, 2, std::bind(&Cpu_z80::SET2_HL, this), "SET 2 HL",	0x0000},
		(t_opcode){0xd7, 0x00, 8, 8, 2, std::bind(&Cpu_z80::SET2_A, this),  "SET 2 A",	0x0000},
		(t_opcode){0xd8, 0x00, 8, 8, 2, std::bind(&Cpu_z80::SET3_B, this),  "SET 3 B",	0x0000},
		(t_opcode){0xd9, 0x00, 8, 8, 2, std::bind(&Cpu_z80::SET3_C, this),  "SET 3 C",	0x0000},
		(t_opcode){0xda, 0x00, 8, 8, 2, std::bind(&Cpu_z80::SET3_D, this),  "SET 3 D",	0x0000},
		(t_opcode){0xdb, 0x00, 8, 8, 2, std::bind(&Cpu_z80::SET3_E, this),  "SET 3 E",	0x0000},
		(t_opcode){0xdc, 0x00, 8, 8, 2, std::bind(&Cpu_z80::SET3_H, this), 	"SET 3 H",	0x0000},
		(t_opcode){0xdd, 0x00, 8, 8, 2, std::bind(&Cpu_z80::SET3_L, this), 	"SET 3 L",	0x0000},
		(t_opcode){0xde, 0x00, 8, 8, 2, std::bind(&Cpu_z80::SET3_HL, this), "SET 3 HL",	0x0000},
		(t_opcode){0xdf, 0x00, 8, 8, 2, std::bind(&Cpu_z80::SET3_A, this),  "SET 3 A",	0x0000},
		(t_opcode){0xe0, 0x00, 8, 8, 2, std::bind(&Cpu_z80::SET4_B, this),  "SET 4 B",	0x0000},
		(t_opcode){0xe1, 0x00, 8, 8, 2, std::bind(&Cpu_z80::SET4_C, this),  "SET 4 C",	0x0000},
		(t_opcode){0xe2, 0x00, 8, 8, 2, std::bind(&Cpu_z80::SET4_D, this),  "SET 4 D",	0x0000},
		(t_opcode){0xe3, 0x00, 8, 8, 2, std::bind(&Cpu_z80::SET4_E, this),  "SET 4 E",	0x0000},
		(t_opcode){0xe4, 0x00, 8, 8, 2, std::bind(&Cpu_z80::SET4_H, this),  "SET 4 H",	0x0000},
		(t_opcode){0xe5, 0x00, 8, 8, 2, std::bind(&Cpu_z80::SET4_L, this),  "SET 4 L",	0x0000},
		(t_opcode){0xe6, 0x00, 8, 8, 2, std::bind(&Cpu_z80::SET4_HL, this), "SET 4 HL",	0x0000},
		(t_opcode){0xe7, 0x00, 8, 8, 2, std::bind(&Cpu_z80::SET4_A, this),  "SET 4 A",	0x0000},
		(t_opcode){0xe8, 0x00, 8, 8, 2, std::bind(&Cpu_z80::SET5_B, this), 	"SET 5 B",	0x0000},
		(t_opcode){0xe9, 0x00, 8, 8, 2, std::bind(&Cpu_z80::SET5_C, this),  "SET 5 C",	0x0000},
		(t_opcode){0xea, 0x00, 8, 8, 2, std::bind(&Cpu_z80::SET5_D, this),  "SET 5 D",	0x0000},
		(t_opcode){0xeb, 0x00, 8, 8, 2, std::bind(&Cpu_z80::SET5_E, this),  "SET 5 E",	0x0000},
		(t_opcode){0xec, 0x00, 8, 8, 2, std::bind(&Cpu_z80::SET5_H, this),  "SET 5 H",	0x0000},
		(t_opcode){0xed, 0x00, 8, 8, 2, std::bind(&Cpu_z80::SET5_L, this),  "SET 5 L",	0x0000},
		(t_opcode){0xee, 0x00, 8, 8, 2, std::bind(&Cpu_z80::SET5_HL, this), "SET 5 HL",	0x0000},
		(t_opcode){0xef, 0x00, 8, 8, 2, std::bind(&Cpu_z80::SET5_A, this),  "SET 5 A",	0x0000},
		(t_opcode){0xf0, 0x00, 8, 8, 2, std::bind(&Cpu_z80::SET6_B, this),  "SET 6 B",	0x0000},
		(t_opcode){0xf1, 0x00, 8, 8, 2, std::bind(&Cpu_z80::SET6_C, this),  "SET 6 C",	0x0000},
		(t_opcode){0xf2, 0x00, 8, 8, 2, std::bind(&Cpu_z80::SET6_D, this),  "SET 6 D",	0x0000},
		(t_opcode){0xf3, 0x00, 8, 8, 2, std::bind(&Cpu_z80::SET6_E, this),  "SET 6 E",	0x0000},
		(t_opcode){0xf4, 0x00, 8, 8, 2, std::bind(&Cpu_z80::SET6_H, this),  "SET 6 H",	0x0000},
		(t_opcode){0xf5, 0x00, 8, 8, 2, std::bind(&Cpu_z80::SET6_L, this),  "SET 6 L",	0x0000},
		(t_opcode){0xf6, 0x00, 8, 8, 2, std::bind(&Cpu_z80::SET6_HL, this), "SET 6 HL",	0x0000},
		(t_opcode){0xf7, 0x00, 8, 8, 2, std::bind(&Cpu_z80::SET6_A, this),  "SET 6 A",	0x0000},
		(t_opcode){0xf8, 0x00, 8, 8, 2, std::bind(&Cpu_z80::SET7_B, this),	"SET 7 B",	0x0000},
		(t_opcode){0xf9, 0x00, 8, 8, 2, std::bind(&Cpu_z80::SET7_C, this), 	"SET 7 C",	0x0000},
		(t_opcode){0xfa, 0x00, 8, 8, 2, std::bind(&Cpu_z80::SET7_D, this),  "SET 7 D",	0x0000},
		(t_opcode){0xfb, 0x00, 8, 8, 2, std::bind(&Cpu_z80::SET7_E, this),  "SET 7 E",	0x0000},
		(t_opcode){0xfc, 0x00, 8, 8, 2, std::bind(&Cpu_z80::SET7_H, this),  "SET 7 H",	0x0000},
		(t_opcode){0xfd, 0x00, 8, 8, 2, std::bind(&Cpu_z80::SET7_L, this),  "SET 7 L",	0x0000},
		(t_opcode){0xfe, 0x00, 8, 8, 2, std::bind(&Cpu_z80::SET7_HL, this), "SET 7 HL",	0x0000},
		(t_opcode){0xff, 0x00, 8, 8, 2, std::bind(&Cpu_z80::SET7_A, this),  "SET 7 A",	0x0000}
	};
}

