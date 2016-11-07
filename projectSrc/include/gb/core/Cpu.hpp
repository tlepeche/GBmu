#ifndef _CPU_HPP
# define _CPU_HPP

#include "Timer.hpp"
#include "Memory.hpp"
#include "register.hpp"
#include "opcode.hpp"

#define REGISTER_TIMA	0xFF05
#define REGISTER_TMA	0xFF06
#define REGISTER_TAC	0xFF07
#define REGISTER_IF		0xFF0F //TODO timer overflow??? je peut le bypasser par le timer de base

#define NB_OPCODE 255
#define NOP_OPCODE 0

#define LENGTH_ADDR 0x01

class Cpu_z80
{
	/*
	** ################################################################
	** CREATE Singleton
	** ################################################################
	*/
	public:
		static Cpu_z80 &Instance();

	private:
		Cpu_z80(void);
		virtual ~Cpu_z80(void);
		static Cpu_z80					_instance;

	/*
	** ################################################################
	** METHOD opcode
	** ################################################################
	*/

	public:
		uint8_t							nbCycleNextOpCode(void);
		uint8_t							executeNextOpcode(void);

	private:
		t_opcode						_getOpcode(uint8_t opcode);
		uint8_t							_getCycleOpcode(void);
		uint16_t						_getDataOpcode(void);
		uint8_t							_getLengthDataOpcode(void);
		void							_setDataOpcode(void);

	/*
	** ################################################################
	** METHOD
	** ################################################################
	*/

	public:
		std::array<uint32_t, 4>			getArrayFrequency();
		void							interrupt(void);
		void							init(void);

	private:
		void							_resetPtrAddr(void);
		void 							_nextPtr(void);

	/*
	** ################################################################
	** METHOD setBits
	** ################################################################
	*/

	private:
		void							_setLowBit(uint16_t addr, uint8_t bit);
		void 							_setHightBit(uint16_t addr, uint8_t bit);

	/*
	** ################################################################
	** METHOD display -- see display.cpp
	** ################################################################
	*/
	public:
		void							setInterrupt(uint16_t addr, uint8_t bit);

	private:
		void							_getInterrupt(void);


	/*
	** ################################################################
	** ATTRIBUTE
	** ################################################################
	*/
	private:
		unsigned int					_ptrAddr;
		Memory							&_memory;
		std::array<uint32_t, 4>			_arrayFrequency {{static_cast<uint32_t>(4096), static_cast<uint32_t>(16385), static_cast<uint32_t>(65536), static_cast<uint32_t>(262144) }};
		t_opcode						_opcodeInProgress;

	public:
		unsigned int					_addrLength;
		t_register						_cpuRegister;
};

#endif
