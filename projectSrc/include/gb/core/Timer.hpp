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
	/*
	** ########################################################
	** GETTEUR
	** ########################################################
	*/


	/*
	** ########################################################
	** ATTRIBUTE
	** ########################################################
	*/
	public:
		void						incTima();
		void						reset();
		void						step(unsigned int cycles);
	private:
		std::array<uint32_t, 4>		_arrayFrequency {
				{
					static_cast<uint32_t>(4096), static_cast<uint32_t>(262144),
					static_cast<uint32_t>(65536), static_cast<uint32_t>(16384)
				}};

		unsigned int				_cycles;
		unsigned int				_cyclesAcc;
		Memory						*_memory;
};

#endif
