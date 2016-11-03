#ifndef _CPU_HPP
# define _CPU_HPP

#include "Timer.hpp"
#include "Memory.hpp"

#define REGISTER_TIMA	0xFF05
#define REGISTER_TMA	0xFF06
#define REGISTER_TAC	0xFF07
#define REGISTER_IF		0xFF0F //TODO timer overflow??? je peut le bypasser par le timer de base

#define NB_OPCODE 255
#define NOP_OPCODE 0

typedef struct	s_opcode
{
	uint8_t	opcode;
	int8_t	mask;
	uint8_t	cycleOpcodeFlag;
	uint8_t cycleOpcodeNoFlag;
	uint8_t lengthData;
	void	(*functionOpcode)();
}				t_opcode;

class Cpu_z80
{
	/*
	** ################################################################
	** CREATE Singleton
	** ################################################################
	*/
	public:
		Cpu_z80 &Instance();

	private:
		Cpu_z80(void);
		~Cpu_z80(void);
		static Cpu_z80	_instance;

	/*
	** ################################################################
	** METHOD opcode
	** ################################################################
	*/

	public:
		unsigned int nbCycleNextOpCode(void);
		unsigned int executeNextOpcode(void);

	private:
		uint8_t _getNextOpcode();
		t_opcode _getOpcode(unsigned int opcode);
		unsigned int _getCycleOpcode(void);
		void *_getFunctionOpcode(void);
		unsigned int _getDataOpcode(void); //TODO: can create type 8b or 16b data, moment use uint
		unsigned int _getLenghtDataOpcode(void);

	/*
	** ################################################################
	** METHOD opcode
	** ################################################################
	*/

	public:
		std::array<unsigned int, 4> getArrayFrequency();
		void interrupt(void);

	private:
		void _resetPtrAddr(void);
		void _nextPtr(void);

	/*
	** ################################################################
	** ATTRIBUTE
	** ################################################################
	*/
	private:
		unsigned int				_ptrAddr;
		Memory						&_memory;
		std::array<unsigned int, 4>	_arrayFrequency;
		t_opcode					_opcodeMap[NB_OPCODE];
		t_opcode					opcodeInProgress;
		uint16_t					_pc; //Program counter
		uint16_t					sp;

	public:
		unsigned int _addrLenght;

};

#endif
