/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cpu.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barbare <barbare@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/27 14:09:39 by barbare           #+#    #+#             */
/*   Updated: 2016/10/27 19:28:57 by barbare          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _CPU_HPP
# define _CPU_HPP

# include "Timer.hpp"

#define REGISTER_TIMA	0xFF05
#define REGISTER_TMA	0xFF06
#define REGISTER_TAC	0xFF07
#define REGISTER_IF		0xFF0F //TODO timer overflow??? je peut le bypasser par le timer de base

#define 8_BITS = 8
#define 16_BITS = 16

struct s_opcode
{
	bool execute;
	struct opcode
	{
		unsigned int opcode;
		unsigned int cycleOpcode;
		unsigned int lenghtData;
		void *functionOpcode;
		struct data
		{
		}
	}
}

class Cpu_z80 : Cpu, RegisterZ80
{
	public:
		Cpu_z80(void) : Cpu(8_BITS, _arrayFrequency, RegisterZ80::register);
		~Cpu_z80(void);

	private:
		std::array<int, 4> _arrayFrequency{ 4096, 262144, 65536, 16384 };
};

class Cpu : Register
{
	protected:
		Cpu(unsigned int addressLength, std::array<int, 4> arrFrequency, Register register);
		~Cpu(void);
		std::array<unsigned int, 4> _arrayFrequency;
		unsigned int ptrAddr;
		unsigned int addrLenght;
		Register register;
		s_reg regInProgress;
}

#endif
