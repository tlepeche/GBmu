/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Machine.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barbare <barbare@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/27 20:25:39 by barbare           #+#    #+#             */
/*   Updated: 2016/11/04 10:50:19 by barbare          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Machine.hpp"

#define DEBUG_LOOP 1
#define DEBUG_ROM 1
#define DEBUG_GPU 1

/*
** ############################################################################
** PUBLIC Function
** ############################################################################
*/
Machine::Machine(void) : _memory(Memory::Instance()), _clock(Timer::Instance()), _cpu(Cpu_z80::Instance())
{
	this->_cpu.initIOFlags();
	this->_clock.setCycleTotal(this->_getCycleOpcode());
}

void Machine::run(void)
{
	if ((this->_memory.read_byte(REGISTER_TAC) & 0x4 == 1))
	{
		std::cout << "REGISTER_TAC OK" << std::endl;
		if (this->_cpu.nbCycleNextOpCode() < this->_clock.getCycleAcc())
			this->_clock.setCycleAcc(this->_cpu.executeNextOpcode());
		else
		{
			//this->_gpu.render();
			this->_clock.sleep(16);
				//this->_clock.sleep(this->_gpu.getFrequencyRender());
			this->_clock.reset();
		}
		std::cout << "Interrupt launch OK" << std::endl;
		this->_cpu.interrupt();
		std::cout << "loop again OK" << std::endl;
		this->run();
	}
}

uint8_t Machine::_getCycleOpcode(void)
{
	double period;
	const unsigned int typeFrequency = this->_memory.read_byte(REGISTER_TAC) & 0x3;

	period = (double) (1. / (float)this->_clock.getArrayFrequency(typeFrequency));
	period *= 100;
	return (this->_getFrequencyFrameTimeGpu() / period);
}

uint8_t Machine::_getFrequencyFrameTimeGpu(void)
{
	return ((unsigned int)(1000 / 60)); //TODO: CHange 60 by this->_gpu.getFrequency
}
