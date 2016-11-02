/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Machine.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barbare <barbare@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/27 20:25:39 by barbare           #+#    #+#             */
/*   Updated: 2016/10/27 20:32:27 by barbare          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define DEBUG_LOOP 1
#define DEBUG_ROM 1
#define DEBUG_GPU 1

/*
** ############################################################################
** PUBLIC Function
** ############################################################################
*/

void Machine::run(void)
{
	if (!_cpu && !_clock)
		return ;
	if (!DEBUG_ROM && (!_rom || !_rom.isRomAssociate()))
		return ;
	if (DEBUG_LOOP || (_IO && _IO.register.read(REGISTER_TAC) & 0x4 == 1))
	{
		if (this->_cpu.nbCycleNextOpCode() < this->_clock.getCycleAcc())
			this->_clock.setCycleAcc(this->_cpu.executeNextOpcode());
		else
		{
			//this->_gpu.render();
			#if DEBUG_GPU //TODO: pour tester sans le gpu
				this->_clock.sleep(16);
			#else if
				this->_clock.sleep(this->_gpu.getFrequencyRender());
			#endif
			this->_clock.reset();
		}
		this->_cpu.interrupt();
		this->run();
	}
}

unsigned int Machine::_getCycleOpcode(void)
{
	unsigned double period;
	period = (unsigned double) (1. / (float)this->_clock.getArrayFrequency[_IO.register.read(REGISTER_TAC) & 0x2]);

	period *= 100;
	return (this->_getFrequencyFrameTimeGpu() / period);
}

unsigned int Machine::_getFrequencyFrameTimeGpu(void)
{
	return ((unsigned int)(1000 / this->_gpu.getFrequency));
}
