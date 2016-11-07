#include "Cpu.hpp"
#include "stdio.h"

/*
** ################################################################
** CREATE Singleton
** ################################################################
*/
Cpu_z80		Cpu_z80::_instance = Cpu_z80();

Cpu_z80::Cpu_z80(void) :
						_memory(Memory::Instance())
{
}

Cpu_z80::~Cpu_z80(void)
{
}

Cpu_z80 &Cpu_z80::Instance(void)
{
	return Cpu_z80::_instance;
}

	/*
	** ################################################################
	** METHOD opcode
	** ################################################################
	*/

t_opcode Cpu_z80::_getOpcode(uint8_t opcode)
{
	uint8_t i = 0;

	if (opcode == 0xCB)
	{
		this->_cpuRegister.PC += LENGTH_ADDR;
		uint8_t cbopcode = this->_memory.read_byte(this->_cpuRegister.PC);
		while (i < NB_OPCODE)
		{
			if (_CBopcodeMapp[i] == cbopcode)
				return _CBopcodeMapp[i];
			++i;
		}
	}
	else
		while (i < NB_OPCODE)
		{
			if (_opcodeMap[i].opcode == opcode)
				return _opcodeMap[i];
			++i;
		}
	return _opcodeMap[0]; //Return NOP instruction
}

uint8_t	Cpu_z80::_getCycleOpcode(void)
{
	return this->_opcodeInProgress.cycleOpcodeNoFlag > this->_opcodeInProgress.cycleOpcodeFlag ?
				this->_opcodeInProgress.cycleOpcodeNoFlag : this->_opcodeInProgress.cycleOpcodeFlag;
}


void Cpu_z80::_setDataOpcode(void)
{
	if (this->_opcodeInProgress.lengthData > 1)
	{
		if (this->_opcodeInProgress.lengthData > 2)
			this->_opcodeInProgress.data = this->_memory.read_byte(this->_cpuRegister.PC + LENGTH_ADDR);
		else
			this->_opcodeInProgress.data = this->_memory.read_word(this->_cpuRegister.PC + LENGTH_ADDR);
	}
}

uint16_t Cpu_z80::_getDataOpcode(void)
{
	return this->_opcodeInProgress.data;
}

uint8_t Cpu_z80::_getLengthDataOpcode(void)
{
	return this->_opcodeInProgress.lengthData;
}

void Cpu_z80::_nextPtr(void)
{
	this->_cpuRegister.PC = this->_cpuRegister.PC + this->_opcodeInProgress.lengthData;
	this->_opcodeInProgress = this->_getOpcode(this->_memory.read_byte(this->_cpuRegister.PC));
	this->_setDataOpcode();
}

uint8_t Cpu_z80::nbCycleNextOpCode(void)
{
	return this->_getCycleOpcode();
}

uint8_t Cpu_z80::executeNextOpcode(void)
{
	if (this->_opcodeInProgress.functionOpcode() == NULL)
		printf("Function not yet implemented\n");
	else
		this->_opcodeInProgress.functionOpcode();
	uint8_t cycle = this->_getCycleOpcode();
	this->_nextPtr();
	return cycle;
}

void Cpu_z80::init(void)
{
	this->_memory.reset();

	//init register cpu
	htype typeRom = this->_rom.getType().type;

	//TODO: A modifier pour rajouter le forcage de type GB GBC
	this->_register.A = typeRom == htype.GB ? 0x01 : 0x11;
	this->_register.F = 0xB0;
	this->_register.BC = 0x0013;
	this->_register.DE = 0x00D8;
	this->_register.HL = 0x014D;

	this->_cpuRegister.PC = 0x100;
	this->_cpuRegister.SP = 0xFFFE;

	//init register memory
	this->_memory.write_byte(0xFF05, 0x00); // ; TIMA
	this->_memory.write_byte(0xFF06, 0x00); // ; TMA
	this->_memory.write_byte(0xFF07, 0x00); // ; TAC
	this->_memory.write_byte(0xFF10, 0x80); // ; NR10
	this->_memory.write_byte(0xFF11, 0xBF); // ; NR11
	this->_memory.write_byte(0xFF12, 0xF3); // ; NR12
	this->_memory.write_byte(0xFF14, 0xBF); // ; NR14
	this->_memory.write_byte(0xFF16, 0x3F); // ; NR21
	this->_memory.write_byte(0xFF17, 0x00); // ; NR22
	this->_memory.write_byte(0xFF19, 0xBF); // ; NR24
	this->_memory.write_byte(0xFF1A, 0x7F); // ; NR30
	this->_memory.write_byte(0xFF1B, 0xFF); // ; NR31
	this->_memory.write_byte(0xFF1C, 0x9F); // ; NR32
	this->_memory.write_byte(0xFF1E, 0xBF); // ; NR33
	this->_memory.write_byte(0xFF20, 0xFF); // ; NR41
	this->_memory.write_byte(0xFF21, 0x00); // ; NR42
	this->_memory.write_byte(0xFF22, 0x00); // ; NR43
	this->_memory.write_byte(0xFF23, 0xBF); // ; NR30
	this->_memory.write_byte(0xFF24, 0x77); // ; NR50
	this->_memory.write_byte(0xFF25, 0xF3); // ; NR51
	this->_memory.write_byte(0xFF26, 0xF1); // ; NR52
	this->_memory.write_byte(0xFF40, 0x91); // ; LCDC
	this->_memory.write_byte(0xFF42, 0x00); // ; SCY
	this->_memory.write_byte(0xFF43, 0x00); // ; SCX
	this->_memory.write_byte(0xFF45, 0x00); // ; LYC
	this->_memory.write_byte(0xFF47, 0xFC); // ; BGP
	this->_memory.write_byte(0xFF48, 0xFF); // ; OBP0
	this->_memory.write_byte(0xFF49, 0xFF); // ; OBP1
	this->_memory.write_byte(0xFF4A, 0x00); // ; WY
	this->_memory.write_byte(0xFF4B, 0x00); // ; WX
	this->_memory.write_byte(0xFFFF, 0x00); // ; IE

	this->_opcodeInProgress = this->_getOpcode(this->_memory.read_byte(this->_cpuRegister.PC));
	this->_setDataOpcode();
}

/*
** ################################################################
** METHOD Math
** ################################################################
*/

void Cpu_z80::_setLowBit(uint16_t addr, uint8_t bit)
{
	this->_memory.write_byte(addr, (uint8_t)((0x01 << bit) ^ this->_memory.read_byte(addr)));
}

void Cpu_z80::_setHightBit(uint16_t addr, uint8_t bit)
{
	this->_memory.write_byte(addr, (uint8_t)((0x01 << bit) | this->_memory.read_byte(addr)));
}

void Cpu_z80::interrupt(void)
{
	std::cout << "interrupt cpu" << std::endl;
}

std::array<uint32_t, 4> Cpu_z80::getArrayFrequency()
{
	return this->_arrayFrequency;
}

void Cpu_z80::_resetPtrAddr(void)
{
	this->_cpuRegister.PC = 0x100;
}
