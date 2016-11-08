#include "Cpu.hpp"
#include <stdio.h>

/*
 ** ################################################################
 ** CREATE Singleton
 ** ################################################################
 */
std::array<t_opcode, 256>	_opcodeMap;
std::array<t_opcode, 256>	_CBopcodeMap;

Cpu_z80		Cpu_z80::_instance = Cpu_z80();

Cpu_z80::Cpu_z80(void) :
	_memory(Memory::Instance()),
	_rom(Rom::Instance())
{
	_setOpcodeMap();
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
	if (opcode == 0xCB)
	{
		this->_cpuRegister.PC += LENGTH_ADDR;
		uint8_t cbopcode = this->_memory.read_byte(this->_cpuRegister.PC);
		return _CBopcodeMap[cbopcode];
	}
	return _opcodeMap[opcode];
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

void Cpu_z80::_nextPtr(void) {
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
	if (this->_opcodeInProgress.functionOpcode == NULL)
		printf("Function not yet implemented\n");
	else
		this->_opcodeInProgress.functionOpcode();
	uint8_t cycle = this->_getCycleOpcode();
	this->_nextPtr();
	return cycle;
}

void Cpu_z80::init(void)
{
	printf("INITIALIZING\n");
	htype typeRom;
	this->_memory.reset();
	this->_rom.reset();

	//init register cpu
	//if (this->_rom.isLoaded()) //TODO: when david PR his code
	typeRom = GB; //this->_rom.getType().type;


	//TODO: A modifier pour rajouter le forcage de type GB GBC
	this->_cpuRegister.A = typeRom == GB ? 0x01 : 0x11;
	this->_cpuRegister.F = 0xB0;
	this->_cpuRegister.BC = 0x0013;
	this->_cpuRegister.DE = 0x00D8;
	this->_cpuRegister.HL = 0x014D;

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

	//TODO: Setup loop, i think this one is up when rom memory is plugged
	this->_setHightBit(REGISTER_TAC, 2);
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

void Cpu_z80::_setOpcodeMap()
{
	std::cout << "salut" << std::endl;
	_opcodeMap = {
		(t_opcode){0x00, 0x00, 4 , 4 , 1, std::bind(&Cpu_z80::NOP, this), 0x0000},
		(t_opcode){0x01, 0x00, 12, 12, 3, std::bind(&Cpu_z80::LD_BC_n, this), 0x0000},
		(t_opcode){0x02, 0x00, 8 , 8 , 1, std::bind(&Cpu_z80::LD_BC_A, this), 0x0000},
		(t_opcode){0x03, 0x00, 8 , 8 , 1, std::bind(&Cpu_z80::INC_BC, this), 0x0000},
		(t_opcode){0x04, 0x00, 4 , 4 , 1, std::bind(&Cpu_z80::INC_B, this), 0x0000},
		(t_opcode){0x05, 0x00, 4 , 4 , 1, std::bind(&Cpu_z80::INC_B, this), 0x0000},
		(t_opcode){0x06, 0x00, 8 , 8 , 2, std::bind(&Cpu_z80::INC_B_n, this), 0x0000},
		(t_opcode){0x07, 0x00, 4 , 4 , 1, std::bind(&Cpu_z80::RLCA, this), 0x0000},
		(t_opcode){0x08, 0x00, 20, 20, 3, std::bind(&Cpu_z80::LD_n_SP, this),  0x0000},
		(t_opcode){0x09, 0x00, 8 , 8 , 1, std::bind(&Cpu_z80::ADD_HL_BC, this), 0x0000},
		(t_opcode){0x0a, 0x00, 8 , 8 , 1, std::bind(&Cpu_z80::LD_A_BC, this), 0x0000},
		(t_opcode){0x0b, 0x00, 8 , 8 , 1, std::bind(&Cpu_z80::DEC_BC, this), 0x0000},
		(t_opcode){0x0c, 0x00, 4 , 4 , 1, std::bind(&Cpu_z80::INC_C, this), 0x0000},
		(t_opcode){0x0d, 0x00, 4 , 4 , 1, std::bind(&Cpu_z80::DEC_C, this), 0x0000},
		(t_opcode){0x0e, 0x00, 8 , 8 , 2, std::bind(&Cpu_z80::LD_C_n, this), 0x0000},
		(t_opcode){0x0f, 0x00, 4 , 4 , 1, std::bind(&Cpu_z80::RRCA, this), 0x0000},
		(t_opcode){0x10, 0x00, 4 , 4 , 2, std::bind(&Cpu_z80::STOP, this), 0x0000},
		(t_opcode){0x11, 0x00, 12, 12, 3, std::bind(&Cpu_z80::LD_DE_n, this),  0x0000},
		(t_opcode){0x12, 0x00, 8 , 8 , 1, std::bind(&Cpu_z80::LD_DE_A, this), 0x0000},
		(t_opcode){0x13, 0x00, 8 , 8 , 1, std::bind(&Cpu_z80::INC_DE, this), 0x0000},
		(t_opcode){0x14, 0x00, 4 , 4 , 1, std::bind(&Cpu_z80::INC_D, this), 0x0000},
		(t_opcode){0x15, 0x00, 4 , 4 , 1, std::bind(&Cpu_z80::DEC_D, this), 0x0000},
		(t_opcode){0x16, 0x00, 8 , 8 , 2, std::bind(&Cpu_z80::LD_D_n, this), 0x0000},
		(t_opcode){0x17, 0x00, 4 , 4 , 1, std::bind(&Cpu_z80::RLA, this), 0x0000},
		(t_opcode){0x18, 0x00, 12, 12, 2, std::bind(&Cpu_z80::JR_n, this),  0x0000},
		(t_opcode){0x19, 0x00, 8 , 8 , 1, std::bind(&Cpu_z80::ADD_HL_DE, this), 0x0000},
		(t_opcode){0x1a, 0x00, 8 , 8 , 1, std::bind(&Cpu_z80::LD_A_DE, this), 0x0000},
		(t_opcode){0x1b, 0x00, 8 , 8 , 1, std::bind(&Cpu_z80::DEC_DE, this), 0x0000},
		(t_opcode){0x1c, 0x00, 4 , 4 , 1, std::bind(&Cpu_z80::INC_E, this), 0x0000},
		(t_opcode){0x1d, 0x00, 4 , 4 , 1, std::bind(&Cpu_z80::DEC_E, this), 0x0000},
		(t_opcode){0x1e, 0x00, 8 , 8 , 2, std::bind(&Cpu_z80::LD_E_n, this), 0x0000},
		(t_opcode){0x1f, 0x00, 4 , 4 , 1, std::bind(&Cpu_z80::RRA, this), 0x0000},
		(t_opcode){0x20, 0xc0, 12, 8 , 2, std::bind(&Cpu_z80::JR_NZ_n, this),  0x0000},
		(t_opcode){0x21, 0x00, 12, 12, 3, std::bind(&Cpu_z80::LD_HL_nn, this),  0x0000},
		(t_opcode){0x22, 0x00, 8 , 8 , 1, std::bind(&Cpu_z80::LD_HLI_A, this), 0x0000},
		(t_opcode){0x23, 0x00, 8 , 8 , 1, std::bind(&Cpu_z80::INC_HL, this), 0x0000},
		(t_opcode){0x24, 0x00, 4 , 4 , 1, std::bind(&Cpu_z80::INC_H, this), 0x0000},
		(t_opcode){0x25, 0x00, 4 , 4 , 1, std::bind(&Cpu_z80::DEC_H, this), 0x0000},
		(t_opcode){0x26, 0x00, 12, 12, 2, std::bind(&Cpu_z80::LD_H_n, this),  0x0000},
		(t_opcode){0x27, 0x00, 4 , 4 , 1, std::bind(&Cpu_z80::DAA, this), 0x0000},
		(t_opcode){0x28, 0x80, 12, 8 , 2, std::bind(&Cpu_z80::JR_Z_n, this),  0x0000},
		(t_opcode){0x29, 0x00, 8 , 8 , 1, std::bind(&Cpu_z80::ADD_HL_HL, this), 0x0000},
		(t_opcode){0x2a, 0x00, 8 , 8 , 1, std::bind(&Cpu_z80::LD_A_HLI, this), 0x0000},
		(t_opcode){0x2b, 0x00, 8 , 8 , 1, std::bind(&Cpu_z80::DEC_HL, this), 0x0000},
		(t_opcode){0x2c, 0x00, 4 , 4 , 1, std::bind(&Cpu_z80::INC_L, this), 0x0000},
		(t_opcode){0x2d, 0x00, 4 , 4 , 1, std::bind(&Cpu_z80::DEC_L, this), 0x0000},
		(t_opcode){0x2e, 0x00, 8 , 8 , 2, std::bind(&Cpu_z80::LD_L_n, this), 0x0000},
		(t_opcode){0x2f, 0x00, 4 , 4 , 3, std::bind(&Cpu_z80::CPL, this), 0x0000},
		(t_opcode){0x30, 0x70, 12, 8 , 2, std::bind(&Cpu_z80::JR_NC_n, this),  0x0000},
		(t_opcode){0x31, 0x00, 12, 12, 3, std::bind(&Cpu_z80::LD_SP_n, this),  0x0000},
		(t_opcode){0x32, 0x00, 12, 12, 3, std::bind(&Cpu_z80::LD_HLD_A, this),  0x0000},
		(t_opcode){0x33, 0x00, 8 , 8 , 1, std::bind(&Cpu_z80::INC_SP, this), 0x0000},
		(t_opcode){0x34, 0x00, 12, 12, 1, std::bind(&Cpu_z80::INC_HLF, this),  0x0000},
		(t_opcode){0x35, 0x00, 12, 12, 1, std::bind(&Cpu_z80::DEC_HLF, this),  0x0000},
		(t_opcode){0x36, 0x00, 12, 12, 2, std::bind(&Cpu_z80::LD_HL_n, this),  0x0000},
		(t_opcode){0x37, 0x00, 4 , 4 , 1, std::bind(&Cpu_z80::SCF, this), 0x0000},
		(t_opcode){0x38, 0x10, 12, 8 , 2, std::bind(&Cpu_z80::JR_C_n, this),  0x0000},
		(t_opcode){0x39, 0x00, 8 , 8 , 1, std::bind(&Cpu_z80::ADD_HL_SP, this), 0x0000},
		(t_opcode){0x3a, 0x00, 8 , 8 , 1, std::bind(&Cpu_z80::LD_A_HLD, this), 0x0000},
		(t_opcode){0x3b, 0x00, 8 , 8 , 1, std::bind(&Cpu_z80::DEC_SP, this), 0x0000},
		(t_opcode){0x3c, 0x00, 4 , 4 , 1, std::bind(&Cpu_z80::INC_A, this), 0x0000},
		(t_opcode){0x3d, 0x00, 4 , 4 , 1, std::bind(&Cpu_z80::DEC_A, this), 0x0000},
		(t_opcode){0x3e, 0x00, 8 , 8 , 2, std::bind(&Cpu_z80::LD_A_n, this), 0x0000},
		(t_opcode){0x3f, 0x00, 4 , 4 , 1, std::bind(&Cpu_z80::CCF, this), 0x0000},
		(t_opcode){0x40, 0x00, 4 , 4 , 1, std::bind(&Cpu_z80::LD_B_B, this), 0x0000},
		(t_opcode){0x41, 0x00, 4 , 4 , 1, std::bind(&Cpu_z80::LD_B_C, this), 0x0000},
		(t_opcode){0x42, 0x00, 4 , 4 , 1, std::bind(&Cpu_z80::LD_B_D, this), 0x0000},
		(t_opcode){0x43, 0x00, 4 , 4 , 1, std::bind(&Cpu_z80::LD_B_E, this), 0x0000},
		(t_opcode){0x44, 0x00, 4 , 4 , 1, std::bind(&Cpu_z80::LD_B_H, this), 0x0000},
		(t_opcode){0x45, 0x00, 4 , 4 , 1, std::bind(&Cpu_z80::LD_B_L, this), 0x0000},
		(t_opcode){0x46, 0x00, 8 , 8 , 1, std::bind(&Cpu_z80::LD_B_HL, this), 0x0000},
		(t_opcode){0x47, 0x00, 4 , 4 , 1, std::bind(&Cpu_z80::LD_B_A, this), 0x0000},
		(t_opcode){0x48, 0x00, 4 , 4 , 1, std::bind(&Cpu_z80::LD_C_B, this), 0x0000},
		(t_opcode){0x49, 0x00, 4 , 4 , 1, std::bind(&Cpu_z80::LD_C_C, this), 0x0000},
		(t_opcode){0x4a, 0x00, 4 , 4 , 1, std::bind(&Cpu_z80::LD_C_D, this), 0x0000},
		(t_opcode){0x4b, 0x00, 4 , 4 , 1, std::bind(&Cpu_z80::LD_C_E, this), 0x0000},
		(t_opcode){0x4c, 0x00, 4 , 4 , 1, std::bind(&Cpu_z80::LD_C_H, this), 0x0000},
		(t_opcode){0x4d, 0x00, 4 , 4 , 1, std::bind(&Cpu_z80::LD_C_L, this), 0x0000},
		(t_opcode){0x4e, 0x00, 8 , 8 , 1, std::bind(&Cpu_z80::LD_C_HL, this), 0x0000},
		(t_opcode){0x4f, 0x00, 4 , 4 , 1, std::bind(&Cpu_z80::LD_C_A, this), 0x0000},
		(t_opcode){0x50, 0x00, 4 , 4 , 1, std::bind(&Cpu_z80::LD_D_B, this), 0x0000},
		(t_opcode){0x51, 0x00, 4 , 4 , 1, std::bind(&Cpu_z80::LD_D_C, this), 0x0000},
		(t_opcode){0x52, 0x00, 4 , 4 , 1, std::bind(&Cpu_z80::LD_D_D, this), 0x0000},
		(t_opcode){0x53, 0x00, 4 , 4 , 1, std::bind(&Cpu_z80::LD_D_E, this), 0x0000},
		(t_opcode){0x54, 0x00, 4 , 4 , 1, std::bind(&Cpu_z80::LD_D_H, this), 0x0000},
		(t_opcode){0x55, 0x00, 4 , 4 , 1, std::bind(&Cpu_z80::LD_D_L, this), 0x0000},
		(t_opcode){0x56, 0x00, 8 , 8 , 1, std::bind(&Cpu_z80::LD_D_HL, this), 0x0000},
		(t_opcode){0x57, 0x00, 4 , 4 , 1, std::bind(&Cpu_z80::LD_D_A, this), 0x0000},
		(t_opcode){0x58, 0x00, 4 , 4 , 1, std::bind(&Cpu_z80::LD_E_B, this), 0x0000},
		(t_opcode){0x59, 0x00, 4 , 4 , 1, std::bind(&Cpu_z80::LD_E_C, this), 0x0000},
		(t_opcode){0x5a, 0x00, 4 , 4 , 1, std::bind(&Cpu_z80::LD_E_D, this), 0x0000},
		(t_opcode){0x5b, 0x00, 4 , 4 , 1, std::bind(&Cpu_z80::LD_E_E, this), 0x0000},
		(t_opcode){0x5c, 0x00, 4 , 4 , 1, std::bind(&Cpu_z80::LD_E_H, this), 0x0000},
		(t_opcode){0x5d, 0x00, 4 , 4 , 1, std::bind(&Cpu_z80::LD_E_L, this), 0x0000},
		(t_opcode){0x5e, 0x00, 8 , 8 , 1, std::bind(&Cpu_z80::LD_E_HL, this), 0x0000},
		(t_opcode){0x5f, 0x00, 4 , 4 , 1, std::bind(&Cpu_z80::LD_E_A, this), 0x0000},
		(t_opcode){0x60, 0x00, 4 , 4 , 1, std::bind(&Cpu_z80::LD_H_B, this), 0x0000},
		(t_opcode){0x61, 0x00, 4 , 4 , 1, std::bind(&Cpu_z80::LD_H_C, this), 0x0000},
		(t_opcode){0x62, 0x00, 4 , 4 , 1, std::bind(&Cpu_z80::LD_H_D, this), 0x0000},
		(t_opcode){0x63, 0x00, 4 , 4 , 1, std::bind(&Cpu_z80::LD_H_E, this), 0x0000},
		(t_opcode){0x64, 0x00, 4 , 4 , 1, std::bind(&Cpu_z80::LD_H_H, this), 0x0000},
		(t_opcode){0x65, 0x00, 4 , 4 , 1, std::bind(&Cpu_z80::LD_H_L, this), 0x0000},
		(t_opcode){0x66, 0x00, 8 , 8 , 1, std::bind(&Cpu_z80::LD_H_HL, this), 0x0000},
		(t_opcode){0x67, 0x00, 4 , 4 , 1, std::bind(&Cpu_z80::LD_H_A, this), 0x0000},
		(t_opcode){0x68, 0x00, 4 , 4 , 1, std::bind(&Cpu_z80::LD_L_B, this), 0x0000},
		(t_opcode){0x69, 0x00, 4 , 4 , 1, std::bind(&Cpu_z80::LD_L_C, this), 0x0000},
		(t_opcode){0x6a, 0x00, 4 , 4 , 1, std::bind(&Cpu_z80::LD_L_D, this), 0x0000},
		(t_opcode){0x6b, 0x00, 4 , 4 , 1, std::bind(&Cpu_z80::LD_L_E, this), 0x0000},
		(t_opcode){0x6c, 0x00, 4 , 4 , 1, std::bind(&Cpu_z80::LD_L_H, this), 0x0000},
		(t_opcode){0x6d, 0x00, 4 , 4 , 1, std::bind(&Cpu_z80::LD_L_L, this), 0x0000},
		(t_opcode){0x6e, 0x00, 8 , 8 , 1, std::bind(&Cpu_z80::LD_L_HL, this), 0x0000},
		(t_opcode){0x6f, 0x00, 4 , 4 , 1, std::bind(&Cpu_z80::LD_L_A, this), 0x0000},
		(t_opcode){0x70, 0x00, 8 , 8 , 1, std::bind(&Cpu_z80::LD_HL_B, this), 0x0000},
		(t_opcode){0x71, 0x00, 8 , 8 , 1, std::bind(&Cpu_z80::LD_HL_C, this), 0x0000},
		(t_opcode){0x72, 0x00, 8 , 8 , 1, std::bind(&Cpu_z80::LD_HL_D, this), 0x0000},
		(t_opcode){0x73, 0x00, 8 , 8 , 1, std::bind(&Cpu_z80::LD_HL_E, this), 0x0000},
		(t_opcode){0x74, 0x00, 8 , 8 , 1, std::bind(&Cpu_z80::LD_HL_H, this), 0x0000},
		(t_opcode){0x75, 0x00, 8 , 8 , 1, std::bind(&Cpu_z80::LD_HL_L, this), 0x0000},
		(t_opcode){0x76, 0x00, 4 , 4 , 1, std::bind(&Cpu_z80::HALT, this), 0x0000},
		(t_opcode){0x77, 0x00, 8 , 8 , 1, std::bind(&Cpu_z80::LD_HL_A, this), 0x0000},
		(t_opcode){0x78, 0x00, 4 , 4 , 1, std::bind(&Cpu_z80::LD_A_B, this), 0x0000},
		(t_opcode){0x79, 0x00, 4 , 4 , 1, std::bind(&Cpu_z80::LD_A_C, this), 0x0000},
		(t_opcode){0x7a, 0x00, 4 , 4 , 1, std::bind(&Cpu_z80::LD_A_D, this), 0x0000},
		(t_opcode){0x7b, 0x00, 4 , 4 , 1, std::bind(&Cpu_z80::LD_A_E, this), 0x0000},
		(t_opcode){0x7c, 0x00, 4 , 4 , 1, std::bind(&Cpu_z80::LD_A_H, this), 0x0000},
		(t_opcode){0x7d, 0x00, 4 , 4 , 1, std::bind(&Cpu_z80::LD_A_L, this), 0x0000},
		(t_opcode){0x7e, 0x00, 8 , 8 , 1, std::bind(&Cpu_z80::LD_A_HL, this), 0x0000},
		(t_opcode){0x7f, 0x00, 4 , 4 , 1, std::bind(&Cpu_z80::LD_A_A, this), 0x0000},
		(t_opcode){0x80, 0x00, 4 , 4 , 1, std::bind(&Cpu_z80::ADD_A_B, this), 0x0000},
		(t_opcode){0x81, 0x00, 4 , 4 , 1, std::bind(&Cpu_z80::ADD_A_C, this), 0x0000},
		(t_opcode){0x82, 0x00, 4 , 4 , 1, std::bind(&Cpu_z80::ADD_A_D, this), 0x0000},
		(t_opcode){0x83, 0x00, 4 , 4 , 1, std::bind(&Cpu_z80::ADD_A_E, this), 0x0000},
		(t_opcode){0x84, 0x00, 4 , 4 , 1, std::bind(&Cpu_z80::ADD_A_H, this), 0x0000},
		(t_opcode){0x85, 0x00, 4 , 4 , 1, std::bind(&Cpu_z80::ADD_A_L, this), 0x0000},
		(t_opcode){0x86, 0x00, 8 , 8 , 1, std::bind(&Cpu_z80::ADD_A_HL, this), 0x0000},
		(t_opcode){0x87, 0x00, 4 , 4 , 1, std::bind(&Cpu_z80::ADD_A_A, this), 0x0000},
		(t_opcode){0x88, 0x00, 4 , 4 , 1, std::bind(&Cpu_z80::ADC_A_B, this), 0x0000},
		(t_opcode){0x89, 0x00, 4 , 4 , 1, std::bind(&Cpu_z80::ADC_A_C, this), 0x0000},
		(t_opcode){0x8a, 0x00, 4 , 4 , 1, std::bind(&Cpu_z80::ADC_A_D, this), 0x0000},
		(t_opcode){0x8b, 0x00, 4 , 4 , 1, std::bind(&Cpu_z80::ADC_A_E, this), 0x0000},
		(t_opcode){0x8c, 0x00, 4 , 4 , 1, std::bind(&Cpu_z80::ADC_A_H, this), 0x0000},
		(t_opcode){0x8d, 0x00, 4 , 4 , 1, std::bind(&Cpu_z80::ADC_A_L, this), 0x0000},
		(t_opcode){0x8e, 0x00, 8 , 8 , 1, std::bind(&Cpu_z80::ADC_A_HL, this), 0x0000},
		(t_opcode){0x8f, 0x00, 4 , 4 , 1, std::bind(&Cpu_z80::ADC_A_A, this), 0x0000},
		(t_opcode){0x90, 0x00, 4 , 4 , 1, std::bind(&Cpu_z80::SUB_B, this), 0x0000},
		(t_opcode){0x91, 0x00, 4 , 4 , 1, std::bind(&Cpu_z80::SUB_C, this), 0x0000},
		(t_opcode){0x92, 0x00, 4 , 4 , 1, std::bind(&Cpu_z80::SUB_D, this), 0x0000},
		(t_opcode){0x93, 0x00, 4 , 4 , 1, std::bind(&Cpu_z80::SUB_E, this), 0x0000},
		(t_opcode){0x94, 0x00, 4 , 4 , 1, std::bind(&Cpu_z80::SUB_H, this), 0x0000},
		(t_opcode){0x95, 0x00, 4 , 4 , 1, std::bind(&Cpu_z80::SUB_L, this), 0x0000},
		(t_opcode){0x96, 0x00, 8 , 8 , 1, std::bind(&Cpu_z80::SUB_HL, this), 0x0000},
		(t_opcode){0x97, 0x00, 4 , 4 , 1, std::bind(&Cpu_z80::SUB_A, this), 0x0000},
		(t_opcode){0x98, 0x00, 4 , 4 , 1, std::bind(&Cpu_z80::SBC_A_B, this), 0x0000},
		(t_opcode){0x99, 0x00, 4 , 4 , 1, std::bind(&Cpu_z80::SBC_A_C, this), 0x0000},
		(t_opcode){0x9a, 0x00, 4 , 4 , 1, std::bind(&Cpu_z80::SBC_A_D, this), 0x0000},
		(t_opcode){0x9b, 0x00, 4 , 4 , 1, std::bind(&Cpu_z80::SBC_A_E, this), 0x0000},
		(t_opcode){0x9c, 0x00, 4 , 4 , 1, std::bind(&Cpu_z80::SBC_A_H, this), 0x0000},
		(t_opcode){0x9d, 0x00, 4 , 4 , 1, std::bind(&Cpu_z80::SBC_A_L, this), 0x0000},
		(t_opcode){0x9e, 0x00, 8 , 8 , 1, std::bind(&Cpu_z80::SBC_A_HL, this), 0x0000},
		(t_opcode){0x9f, 0x00, 4 , 4 , 1, std::bind(&Cpu_z80::SBC_A_A, this), 0x0000},
		(t_opcode){0xa0, 0x00, 4 , 4 , 1, std::bind(&Cpu_z80::AND_B, this), 0x0000},
		(t_opcode){0xa1, 0x00, 4 , 4 , 1, std::bind(&Cpu_z80::AND_C, this), 0x0000},
		(t_opcode){0xa2, 0x00, 4 , 4 , 1, std::bind(&Cpu_z80::AND_D, this), 0x0000},
		(t_opcode){0xa3, 0x00, 4 , 4 , 1, std::bind(&Cpu_z80::AND_E, this), 0x0000},
		(t_opcode){0xa4, 0x00, 4 , 4 , 1, std::bind(&Cpu_z80::AND_H, this), 0x0000},
		(t_opcode){0xa5, 0x00, 4 , 4 , 1, std::bind(&Cpu_z80::AND_L, this), 0x0000},
		(t_opcode){0xa6, 0x00, 8 , 8 , 1, std::bind(&Cpu_z80::AND_HL, this), 0x0000},
		(t_opcode){0xa7, 0x00, 4 , 4 , 1, std::bind(&Cpu_z80::AND_A, this), 0x0000},
		(t_opcode){0xa8, 0x00, 4 , 4 , 1, std::bind(&Cpu_z80::XOR_B, this), 0x0000},
		(t_opcode){0xa9, 0x00, 4 , 4 , 1, std::bind(&Cpu_z80::XOR_C, this), 0x0000},
		(t_opcode){0xaa, 0x00, 4 , 4 , 1, std::bind(&Cpu_z80::XOR_D, this), 0x0000},
		(t_opcode){0xab, 0x00, 4 , 4 , 1, std::bind(&Cpu_z80::XOR_E, this), 0x0000},
		(t_opcode){0xac, 0x00, 4 , 4 , 1, std::bind(&Cpu_z80::XOR_H, this), 0x0000},
		(t_opcode){0xad, 0x00, 4 , 4 , 1, std::bind(&Cpu_z80::XOR_L, this), 0x0000},
		(t_opcode){0xae, 0x00, 8 , 8 , 1, std::bind(&Cpu_z80::XOR_HL, this), 0x0000},
		(t_opcode){0xaf, 0x00, 4 , 4 , 1, std::bind(&Cpu_z80::XOR_A, this), 0x0000},
		(t_opcode){0xb0, 0x00, 4 , 4 , 1, std::bind(&Cpu_z80::OR_B, this), 0x0000},
		(t_opcode){0xb1, 0x00, 4 , 4 , 1, std::bind(&Cpu_z80::OR_C, this), 0x0000},
		(t_opcode){0xb2, 0x00, 4 , 4 , 1, std::bind(&Cpu_z80::OR_D, this), 0x0000},
		(t_opcode){0xb3, 0x00, 4 , 4 , 1, std::bind(&Cpu_z80::OR_E, this), 0x0000},
		(t_opcode){0xb4, 0x00, 4 , 4 , 1, std::bind(&Cpu_z80::OR_H, this), 0x0000},
		(t_opcode){0xb5, 0x00, 4 , 4 , 1, std::bind(&Cpu_z80::OR_L, this), 0x0000},
		(t_opcode){0xb6, 0x00, 8 , 8 , 1, std::bind(&Cpu_z80::OR_HL, this), 0x0000},
		(t_opcode){0xb7, 0x00, 4 , 4 , 1, std::bind(&Cpu_z80::OR_A, this), 0x0000},
		(t_opcode){0xb8, 0x00, 4 , 4 , 1, std::bind(&Cpu_z80::CP_B, this), 0x0000},
		(t_opcode){0xb9, 0x00, 4 , 4 , 1, std::bind(&Cpu_z80::CP_C, this), 0x0000},
		(t_opcode){0xba, 0x00, 4 , 4 , 1, std::bind(&Cpu_z80::CP_D, this), 0x0000},
		(t_opcode){0xbb, 0x00, 4 , 4 , 1, std::bind(&Cpu_z80::CP_E, this), 0x0000},
		(t_opcode){0xbc, 0x00, 4 , 4 , 1, std::bind(&Cpu_z80::CP_H, this), 0x0000},
		(t_opcode){0xbd, 0x00, 4 , 4 , 1, std::bind(&Cpu_z80::CP_L, this), 0x0000},
		(t_opcode){0xbe, 0x00, 8 , 8 , 1, std::bind(&Cpu_z80::CP_HL, this), 0x0000},
		(t_opcode){0xbf, 0x00, 4 , 4 , 1, std::bind(&Cpu_z80::CP_A, this), 0x0000},
		(t_opcode){0xc0, 0xc0, 20, 8 , 1, std::bind(&Cpu_z80::RET_NZ, this), 0x0000},
		(t_opcode){0xc1, 0x00, 12, 12, 1, std::bind(&Cpu_z80::POP_BC, this), 0x0000},
		(t_opcode){0xc2, 0xc0, 16, 16, 3, std::bind(&Cpu_z80::JP_NZ_n, this), 0x0000},
		(t_opcode){0xc3, 0x00, 16, 16, 3, std::bind(&Cpu_z80::JP_n, this),  0x0000},
		(t_opcode){0xc4, 0xc0, 24, 12, 3, std::bind(&Cpu_z80::CALL_NZ_n, this), 0x0000},
		(t_opcode){0xc5, 0x00, 16, 16, 1, std::bind(&Cpu_z80::PUSH_BC, this), 0x0000},
		(t_opcode){0xc6, 0x00, 8 , 8 , 2, std::bind(&Cpu_z80::ADD_A_n, this), 0x0000},
		(t_opcode){0xc7, 0x00, 16, 16, 1, std::bind(&Cpu_z80::RST_00H, this), 0x0000},
		(t_opcode){0xc8, 0x80, 20, 8 , 1, std::bind(&Cpu_z80::RET_Z, this), 0x0000},
		(t_opcode){0xc9, 0x00, 16, 16, 1, std::bind(&Cpu_z80::RET, this), 0x0000},
		(t_opcode){0xca, 0x80, 16, 12, 3, std::bind(&Cpu_z80::JP_Z_n, this), 0x0000},
		(t_opcode){0xcb, 0x00, 4 , 4 , 1, std::bind(&Cpu_z80::PREFIX_CB, this), 0x0000},
		(t_opcode){0xcc, 0x80, 24, 12, 3, std::bind(&Cpu_z80::CALL_Z_n, this), 0x0000},
		(t_opcode){0xcd, 0x00, 24, 24, 3, std::bind(&Cpu_z80::CALL_n, this), 0x0000},
		(t_opcode){0xce, 0x00, 8 , 8 , 2, std::bind(&Cpu_z80::ADC_A_n, this), 0x0000},
		(t_opcode){0xcf, 0x00, 16, 16, 1, std::bind(&Cpu_z80::RST_08H, this), 0x0000},
		(t_opcode){0xd0, 0x50, 20, 8 , 1, std::bind(&Cpu_z80::RET_NC, this), 0x0000},
		(t_opcode){0xd1, 0x00, 12, 12, 1, std::bind(&Cpu_z80::POP_DE, this), 0x0000},
		(t_opcode){0xd2, 0x50, 16, 12, 3, std::bind(&Cpu_z80::JP_NC_n, this), 0x0000},
		(t_opcode){0xd3, 0x00, 0 , 0 , 0, NULL, 0x0000},
		(t_opcode){0xd4, 0x50, 24, 12, 3, std::bind(&Cpu_z80::CALL_NC_n, this), 0x0000},
		(t_opcode){0xd5, 0x00, 16, 16, 1, std::bind(&Cpu_z80::PUSH_DE, this), 0x0000},
		(t_opcode){0xd6, 0x00, 8 , 8 , 2, std::bind(&Cpu_z80::SUB_n, this), 0x0000},
		(t_opcode){0xd7, 0x00, 16, 16, 1, std::bind(&Cpu_z80::RST_10H, this), 0x0000},
		(t_opcode){0xd8, 0x10, 20, 8 , 1, std::bind(&Cpu_z80::RET_C, this), 0x0000},
		(t_opcode){0xd9, 0x00, 16, 16, 1, std::bind(&Cpu_z80::RETI, this), 0x0000},
		(t_opcode){0xda, 0x10, 16, 12, 3, std::bind(&Cpu_z80::JP_C_n, this), 0x0000},
		(t_opcode){0xdb, 0x00, 0 , 0 , 0, NULL, 0x0000},
		(t_opcode){0xdc, 0x10, 24, 12, 3, std::bind(&Cpu_z80::CALL_C_n, this), 0x0000},
		(t_opcode){0xdd, 0x00, 0 , 0 , 0, NULL, 0x0000},
		(t_opcode){0xde, 0x00, 8 , 8 , 2, std::bind(&Cpu_z80::SBC_A_n, this), 0x0000},
		(t_opcode){0xdf, 0x00, 16, 16, 1, std::bind(&Cpu_z80::RST_18H, this), 0x0000},
		(t_opcode){0xe0, 0x00, 12, 12, 2, std::bind(&Cpu_z80::LDH_n_A, this), 0x0000},
		(t_opcode){0xe1, 0x00, 12, 12, 1, std::bind(&Cpu_z80::POP_HL, this), 0x0000},
		(t_opcode){0xe2, 0x00, 8 , 8 , 2, std::bind(&Cpu_z80::LD_CC_A, this), 0x0000},
		(t_opcode){0xe3, 0x00, 0 , 0 , 0, NULL, 0x0000},
		(t_opcode){0xe4, 0x00, 0 , 0 , 0, NULL, 0x0000},
		(t_opcode){0xe5, 0x00, 16, 16, 1, std::bind(&Cpu_z80::PUSH_HL, this), 0x0000},
		(t_opcode){0xe6, 0x00, 8 , 8 , 2, std::bind(&Cpu_z80::AND_n, this), 0x0000},
		(t_opcode){0xe7, 0x00, 16, 16, 1, std::bind(&Cpu_z80::RST_20H, this), 0x0000},
		(t_opcode){0xe8, 0x00, 16, 16, 2, std::bind(&Cpu_z80::ADD_SP_n, this), 0x0000},
		(t_opcode){0xe9, 0x00, 4 , 4 , 1, std::bind(&Cpu_z80::JP_HL, this), 0x0000},
		(t_opcode){0xea, 0x00, 16, 16, 3, std::bind(&Cpu_z80::LD_n_A, this), 0x0000},
		(t_opcode){0xeb, 0x00, 0 , 0 , 0, NULL, 0x0000},
		(t_opcode){0xec, 0x00, 0 , 0 , 0, NULL, 0x0000},
		(t_opcode){0xed, 0x00, 0 , 0 , 0, NULL, 0x0000},
		(t_opcode){0xee, 0x00, 8 , 8 , 2, std::bind(&Cpu_z80::XOR_n, this), 0x0000},
		(t_opcode){0xef, 0x00, 16, 16, 1, std::bind(&Cpu_z80::RST_28H, this), 0x0000},
		(t_opcode){0xf0, 0x00, 12, 12, 2, std::bind(&Cpu_z80::LDH_A_n, this), 0x0000},
		(t_opcode){0xf1, 0x00, 12, 12, 1, std::bind(&Cpu_z80::POP_AF, this), 0x0000},
		(t_opcode){0xf2, 0x00, 8 , 8 , 2, std::bind(&Cpu_z80::LD_A_CC, this), 0x0000},
		(t_opcode){0xf3, 0x00, 4 , 4 , 1, std::bind(&Cpu_z80::DI, this), 0x0000},
		(t_opcode){0xf4, 0x00, 0 , 0 , 0, NULL, 0x0000},
		(t_opcode){0xf5, 0x00, 16, 16, 1, std::bind(&Cpu_z80::PUSH_AF, this), 0x0000},
		(t_opcode){0xf6, 0x00, 8 , 8 , 2, std::bind(&Cpu_z80::OR_n, this), 0x0000},
		(t_opcode){0xf7, 0x00, 16, 16, 1, std::bind(&Cpu_z80::RST_30H, this), 0x0000},
		(t_opcode){0xf8, 0x00, 12, 12, 2, std::bind(&Cpu_z80::LD_HL_SP_n, this), 0x0000},
		(t_opcode){0xf9, 0x00, 8 , 8 , 1, std::bind(&Cpu_z80::LD_SP_HL, this), 0x0000},
		(t_opcode){0xfa, 0x00, 16, 16, 3, std::bind(&Cpu_z80::LD_A_n, this), 0x0000},
		(t_opcode){0xfb, 0x00, 4 , 4 , 1, std::bind(&Cpu_z80::EI, this), 0x0000},
		(t_opcode){0xfc, 0x00, 0 , 0 , 0, NULL, 0x0000},
		(t_opcode){0xfd, 0x00, 0 , 0 , 0, NULL, 0x0000},
		(t_opcode){0xfe, 0x00, 8 , 8 , 2, std::bind(&Cpu_z80::CP_n, this), 0x0000},
		(t_opcode){0xff, 0x00, 16, 16, 1, std::bind(&Cpu_z80::RST_38H, this), 0x0000}
	};
}
