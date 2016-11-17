#ifndef _TIMER_HPP
# define _TIMER_HPP

#include <iostream>
#include <array>
#include "Memory.hpp"

class Machine;

class Timer
{
	friend	Machine;
	/*
	** ################################################################
	** CREATE Singleton
	** ################################################################
	*/
	private:
		Timer(Memory *mem);
		virtual ~Timer(void) {};

	/*
	** ############################################################
	** SETTEUR
	** ############################################################
	*/
	public:
		void						reset();
		void 						setCycleAcc(uint8_t cycle);
		void 						setFrequency(std::array<uint32_t, 4> arrFrequency);
		void 						setCycleTotal(uint32_t cycle);

	/*
	** ########################################################
	** GETTEUR
	** ########################################################
	*/
	public:
		uint32_t						getArrayFrequency(const uint8_t idFrequency);
		uint32_t						getCycleAcc(void);
		bool							isCycleAcc(uint32_t cycle);

	/*
	** ########################################################
	** METHOD
	** ########################################################
	*/
	public:
		void						sleep(unsigned char ms);

	private:
		uint32_t					_getCycleOpcodeTotal(void);
		void						_doDividerRegister(uint8_t cycles);
		void						_doClockRegister(uint8_t cycles);

	/*
	** ########################################################
	** ATTRIBUTE
	** ########################################################
	*/
	private:
		std::array<uint32_t, 4>		_arrayFrequency;
		uint32_t					_cycles = 0;
		uint32_t					_cyclesTotal;
		uint16_t					_divider = 0;
		Memory						*_memory;
};

#endif
