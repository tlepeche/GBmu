#include "Cpu.hpp"

void	Cpu_z80::NOP() //0x00
{
	std::cout << "NOP" << std::endl;
}

void	Cpu_z80::LD_BC_n() //0x01
{
	uint16_t	nn;

	nn = _memory.read_word(_cpuRegister.PC + 1); 
	_memory.write_word(_cpuRegister.BC, nn);
}

void	Cpu_z80::LD_BC_A() //0x02
{
	_memory.write_byte(_cpuRegister.BC, _cpuRegister.A);
}

void	Cpu_z80::INC_BC() //0x03
{
	_cpuRegister.BC++;
}

void	Cpu_z80::INC_B() //0x04
{
	_cpuRegister.n = 0;
	_cpuRegister.h = static_cast<int>(testAdd(_cpuRegister.B, 1, 0xf0));
	_cpuRegister.B++;
	_cpuRegister.z = (_cpuRegister.B == 0) ? 1 : 0;
}

void	Cpu_z80::DEC_B() //0x05
{
	_cpuRegister.n = 1;
	_cpuRegister.h = static_cast<int>(testSub(_cpuRegister.B, 1, 0x0f));
	_cpuRegister.B--;
	_cpuRegister.z = (_cpuRegister.B == 0) ? 1 : 0;
}

void	Cpu_z80::LD_B_n() //0x06
{
	_memory.write_byte(_cpuRegister.PC + 1, _cpuRegister.B);
}

void	Cpu_z80::RLCA() //0x07
{
	_cpuRegister.c = _cpuRegister.A & 0x80;
	_cpuRegister.A <<= 1;
	if (_cpuRegister.c)
		_cpuRegister.A += 1;
	_cpuRegister.h = 0;
	_cpuRegister.n = 0;
	_cpuRegister.z = (_cpuRegister.A == 0) ? 1 : 0;
}

void	Cpu_z80::LD_n_SP() //0x08
{
	_memory.write_word(_memory.read_word(_cpuRegister.PC + 1), _cpuRegister.SP);
}

void	Cpu_z80::ADD_HL_BC() //0x09
{
	_cpuRegister.h = static_cast<int>(testAdd(_cpuRegister.HL, _cpuRegister.BC, 0xf000));
	_cpuRegister.n = 0;
	_cpuRegister.c = static_cast<int>(testAdd(_cpuRegister.HL, _cpuRegister.BC, 0xffff0000));
	_cpuRegister.HL += _cpuRegister.BC;
}

void	Cpu_z80::LD_A_BC() //0x0a
{
	_cpuRegister.A = _memory.read_byte(_cpuRegister.BC);
}

void	Cpu_z80::DEC_BC() //0x0b
{
	_cpuRegister.BC--;
}

void	Cpu_z80::INC_C() //0x0c
{
	_cpuRegister.h = static_cast<int>(testAdd(_cpuRegister.C, 1, 0xf0));
	_cpuRegister.C++;
	_cpuRegister.z = (_cpuRegister.C == 0) ? 1 : 0;
}

void	Cpu_z80::DEC_C() //0x0d
{
	_cpuRegister.n = 1;
	_cpuRegister.h = static_cast<int>(testSub(_cpuRegister.C, 1, 0x0f));
	_cpuRegister.C--;
	_cpuRegister.z = (_cpuRegister.C == 0) ? 1 : 0;
}

void	Cpu_z80::LD_C_n() //0x0e
{
	_memory.write_byte(_cpuRegister.PC + 1, _cpuRegister.C);
}

void	Cpu_z80::RRCA() //0x0f
{
	_cpuRegister.c = _cpuRegister.A & 0x80;
	_cpuRegister.A >>= 1;
	if (_cpuRegister.c)
		_cpuRegister.A += 0x80;
	_cpuRegister.h = 0;
	_cpuRegister.n = 0;
	_cpuRegister.z = (_cpuRegister.A == 0) ? 1 : 0;
}

void	Cpu_z80::LD_DE_n() //0x11
{
	uint16_t	nn;

	nn = _memory.read_word(_cpuRegister.PC + 1); 
	_memory.write_word(_cpuRegister.DE, nn);
}

void	Cpu_z80::LD_DE_A() //0x12
{
	_memory.write_word(_cpuRegister.DE, _cpuRegister.A);
}

void	Cpu_z80::INC_DE() //0x13
{
	_cpuRegister.DE++;
}

void	Cpu_z80::INC_D() //0x14
{
	_cpuRegister.h = static_cast<int>(testAdd(_cpuRegister.D, 1, 0xf0));
	_cpuRegister.D++;
	_cpuRegister.z = (_cpuRegister.D == 0) ? 1 : 0;
}

void	Cpu_z80::DEC_D() //0x15
{
	_cpuRegister.n = 1;
	_cpuRegister.h = static_cast<int>(testSub(_cpuRegister.D, 1, 0x0f));
	_cpuRegister.D--;
	_cpuRegister.z = (_cpuRegister.D == 0) ? 1 : 0;
}

void	Cpu_z80::LD_D_n() //0x16
{
	_memory.write_byte(_cpuRegister.PC + 1, _cpuRegister.D);
}

void	Cpu_z80::RLA() //0x17
{
	int	carry;

	carry = (_cpuRegister.c == 1) ? 1 : 0;
	_cpuRegister.c = _cpuRegister.A & 0x01;
	_cpuRegister.A <<= 1;
	_cpuRegister.A += carry;
	_cpuRegister.n = 0;
	_cpuRegister.h = 0;
	_cpuRegister.z = (_cpuRegister.A == 0) ? 1 : 0;
}

void	Cpu_z80::JR_n() //0x18
{
	int8_t	n;
	uint8_t a = _memory.read_byte(_cpuRegister.PC + 1);
	a = 0x00;
	n = 0;
	n |= a;
	_cpuRegister.PC = _memory.read_byte(_cpuRegister.PC) + n;
}

void	Cpu_z80::ADD_HL_DE() //0x19
{
	_cpuRegister.h = static_cast<int>(testAdd(_cpuRegister.HL, _cpuRegister.DE, 0xf000));
	_cpuRegister.n = 0;
	_cpuRegister.c = static_cast<int>(testAdd(_cpuRegister.HL, _cpuRegister.DE, 0xffff0000));
	_cpuRegister.HL += _cpuRegister.DE;
}

void	Cpu_z80::LD_A_DE() //0x1a
{
	_cpuRegister.A = _memory.read_byte(_cpuRegister.DE);
}

void	Cpu_z80::DEC_DE() //0x1b
{
	_cpuRegister.DE--;
}

void	Cpu_z80::INC_E() //0x1c
{
	_cpuRegister.h = static_cast<int>(testAdd(_cpuRegister.E, 1, 0xf0));
	_cpuRegister.E++;
	_cpuRegister.z = (_cpuRegister.E == 0) ? 1 : 0;
}

void	Cpu_z80::DEC_E() //0x1d
{
	_cpuRegister.n = 1;
	_cpuRegister.h = static_cast<int>(testSub(_cpuRegister.E, 1, 0x0f));
	_cpuRegister.E--;
	_cpuRegister.z = (_cpuRegister.E == 0) ? 1 : 0;
}

void	Cpu_z80::LD_E_n() //0x1e
{
	_memory.write_byte(_cpuRegister.PC + 1, _cpuRegister.E);
}

void	Cpu_z80::RRA() //0x1f
{
	int	carry;

	carry = ((_cpuRegister.c == 1) ? 1 : 0) << 7;
	_cpuRegister.c = _cpuRegister.A & 0x01;
	_cpuRegister.A >>= 1;
	_cpuRegister.A += carry;
	_cpuRegister.n = 0;
	_cpuRegister.h = 0;
	_cpuRegister.z = (_cpuRegister.A == 0) ? 1 : 0;
}

void	Cpu_z80::JR_NZ_n() //0x20
{
	if (_cpuRegister.z == 0)
		JR_n();
}

void	Cpu_z80::LD_HL_nn() //0x21
{
	uint16_t	nn;

	nn = _memory.read_word(_cpuRegister.PC + 1); 
	_memory.write_word(_cpuRegister.HL, nn);
}

void	Cpu_z80::LD_HLI_A() //0x22
{
	LD_HL_A();
	INC_HLF();
}

void	Cpu_z80::INC_HL() //0x23
{
	_cpuRegister.HL++;
}

void	Cpu_z80::INC_H() //0x24
{
	_cpuRegister.h = static_cast<int>(testAdd(_cpuRegister.H, 1, 0xf0));
	_cpuRegister.H++;
	_cpuRegister.z = (_cpuRegister.H == 0) ? 1 : 0;
}

void	Cpu_z80::DEC_H() //0x25
{
	_cpuRegister.n = 1;
	_cpuRegister.h = static_cast<int>(testSub(_cpuRegister.H, 1, 0x0f));
	_cpuRegister.H--;
	_cpuRegister.z = (_cpuRegister.H == 0) ? 1 : 0;
}

void	Cpu_z80::LD_H_n() //0x26
{

	_memory.write_byte(_cpuRegister.PC + 1, _cpuRegister.H);
}

void	Cpu_z80::DAA() //0x27
{
	if (_cpuRegister.n == 0)
	{
		if (_cpuRegister.h || (_cpuRegister.A & 0x0f) > 9)
			_cpuRegister.A += 0x06;

		if (_cpuRegister.c || _cpuRegister.A > 0x9f)
			_cpuRegister.A += 0x60;
	}
	else
	{
		if (_cpuRegister.h)
			_cpuRegister.A = (_cpuRegister.A - 6) & 0xFF;
		if (_cpuRegister.c)
			_cpuRegister.A -= 0x60;
	}
	_cpuRegister.c = (_cpuRegister.A & 0xff00) ? 1 : 0;
	_cpuRegister.z = _cpuRegister.A == 0 ? 1 : 0;
}

void	Cpu_z80::JR_Z_n() //0x28
{
	if (_cpuRegister.z == 1)
		JR_n();
}

void	Cpu_z80::ADD_HL_HL() //0x29
{
	_cpuRegister.h = static_cast<int>(testAdd(_cpuRegister.HL, _cpuRegister.HL, 0xf000));
	_cpuRegister.n = 0;
	_cpuRegister.c = static_cast<int>(testAdd(_cpuRegister.HL, _cpuRegister.HL, 0xffff0000));
	_cpuRegister.HL += _cpuRegister.HL;
}

void	Cpu_z80::LD_A_HLI() //0x2a
{
	LD_A_HL();
	INC_HLF();
}

void	Cpu_z80::DEC_HL() //0x2b
{
	_cpuRegister.HL--;
}

void	Cpu_z80::INC_L() //0x2c
{
	_cpuRegister.h = static_cast<int>(testAdd(_cpuRegister.L, 1, 0xf0));
	_cpuRegister.L++;
	_cpuRegister.z = (_cpuRegister.L == 0) ? 1 : 0;
}

void	Cpu_z80::DEC_L() //0x2d
{
	_cpuRegister.n = 1;
	_cpuRegister.h = static_cast<int>(testSub(_cpuRegister.L, 1, 0x0f));
	_cpuRegister.L--;
	_cpuRegister.z = (_cpuRegister.L == 0) ? 1 : 0;
}

void	Cpu_z80::LD_L_n() //0x2e
{
	_memory.write_byte(_cpuRegister.PC + 1, _cpuRegister.L);
}

void	Cpu_z80::CPL() //0x2f
{
	_cpuRegister.A = ~_cpuRegister.A;
	_cpuRegister.n = 1;
	_cpuRegister.h = 1;
}

void	Cpu_z80::JR_NC_n() //0x30
{
	if (_cpuRegister.c == 0)
		JR_n();
}

void	Cpu_z80::LD_SP_n() //0x31
{
	uint16_t	nn;

	nn = _memory.read_word(_cpuRegister.PC + 1); 
	_memory.write_word(_cpuRegister.SP, nn);
}

void	Cpu_z80::LD_HLD_A() //0x32
{
	LD_HL_A();
	DEC_HLF();
}

void	Cpu_z80::INC_SP() //0x33
{
	_cpuRegister.SP++;
}

void	Cpu_z80::INC_HLF() //0x34
{
	uint16_t tmp;

	tmp = _memory.read_word(_cpuRegister.HL);
	_cpuRegister.h = static_cast<int>(testAdd(tmp, 1, 0xf0));
	_cpuRegister.n = 0;
	_memory.write_word(_cpuRegister.HL, (tmp + 1));
	_cpuRegister.z = (_memory.read_word(_cpuRegister.HL) == 0) ? 1 : 0;
}

void	Cpu_z80::DEC_HLF() //0x35
{
	uint16_t tmp;

	tmp = _memory.read_word(_cpuRegister.HL);
	_cpuRegister.n = 1;
	_cpuRegister.h = static_cast<int>(testSub(tmp, 1, 0x0f));
	_memory.write_word(_cpuRegister.HL, (tmp - 1));
	_cpuRegister.z = (_memory.read_word(_cpuRegister.HL) == 0) ? 1 : 0;
}
void	Cpu_z80::LD_HL_n() //0x36
{
	uint8_t n;

	n = _memory.read_byte(_cpuRegister.HL);
	_memory.write_byte(_cpuRegister.PC + 1, n);
}

void	Cpu_z80::SCF() //0x37
{
	_cpuRegister.c = 1;
	_cpuRegister.h = 0;
	_cpuRegister.n = 0;
}

void	Cpu_z80::JR_C_n() //0x38
{
	if (_cpuRegister.c == 1)
		JR_n();
}

void	Cpu_z80::ADD_HL_SP() //0x39
{
	_cpuRegister.h = static_cast<int>(testAdd(_cpuRegister.HL, _cpuRegister.SP, 0xf000));
	_cpuRegister.n = 0;
	_cpuRegister.c = static_cast<int>(testAdd(_cpuRegister.HL, _cpuRegister.SP, 0xffff0000));
	_cpuRegister.HL += _cpuRegister.SP;
}

void	Cpu_z80::LD_A_HLD() //0x3a
{
	LD_A_HL();
	DEC_HLF();
}

void	Cpu_z80::DEC_SP() //0x3b
{
	_cpuRegister.SP--;
}

void	Cpu_z80::INC_A() //0x3c
{
	_cpuRegister.h = static_cast<int>(testAdd(_cpuRegister.A, 1, 0xf0));
	_cpuRegister.n = 0;
	_cpuRegister.A++;
	_cpuRegister.z = (_cpuRegister.A == 0) ? 1 : 0;
}

void	Cpu_z80::DEC_A() //0x3d
{
	_cpuRegister.n = 1;
	_cpuRegister.h = static_cast<int>(testSub(_cpuRegister.A, 1, 0x0f));
	_cpuRegister.A--;
	_cpuRegister.z = (_cpuRegister.A == 0) ? 1 : 0;
}


void	Cpu_z80::LD_A_n() //0x3e
{
	_memory.write_byte(_cpuRegister.PC + 1, _cpuRegister.A);
}

void	Cpu_z80::CCF() //0x3f
{
	_cpuRegister.c = (_cpuRegister.c == 1) ? 0 : 1;
	_cpuRegister.n = 0;
	_cpuRegister.h = 0;
}

void	Cpu_z80::LD_B_B() //0x40
{
	_cpuRegister.B = _cpuRegister.B;
}

void	Cpu_z80::LD_B_C() //0x41
{
	_cpuRegister.B = _cpuRegister.C;
}

void	Cpu_z80::LD_B_D() //0x42
{
	_cpuRegister.B = _cpuRegister.D;
}

void	Cpu_z80::LD_B_E() //0x43
{
	_cpuRegister.B = _cpuRegister.E;
}

void	Cpu_z80::LD_B_H() //0x44
{
	_cpuRegister.B = _cpuRegister.H;
}

void	Cpu_z80::LD_B_L() //0x45
{
	_cpuRegister.B = _cpuRegister.L;
}

void	Cpu_z80::LD_B_HL() //0x46
{
	_cpuRegister.B = _memory.read_byte(_cpuRegister.HL);
}

void	Cpu_z80::LD_B_A() //0x47
{
	_cpuRegister.B = _cpuRegister.A;
}

void	Cpu_z80::LD_C_B() //0x48
{
	_cpuRegister.C = _cpuRegister.B;
}

void	Cpu_z80::LD_C_C() //0x49
{
	_cpuRegister.C = _cpuRegister.C;
}

void	Cpu_z80::LD_C_D() //0x4a
{
	_cpuRegister.C = _cpuRegister.D;
}

void	Cpu_z80::LD_C_E() //0x4b
{
	_cpuRegister.C = _cpuRegister.E;
}

void	Cpu_z80::LD_C_H() //0x4c
{
	_cpuRegister.C = _cpuRegister.H;
}

void	Cpu_z80::LD_C_L() //0x4d
{
	_cpuRegister.C = _cpuRegister.L;
}

void	Cpu_z80::LD_C_HL() //0x4e
{
	_cpuRegister.C = _memory.read_byte(_cpuRegister.HL);
}

void	Cpu_z80::LD_C_A() //0x4f
{
	_cpuRegister.C = _cpuRegister.A;
}

void	Cpu_z80::LD_D_B() //0x50
{
	_cpuRegister.D = _cpuRegister.B;
}

void	Cpu_z80::LD_D_C() //0x51
{
	_cpuRegister.D = _cpuRegister.C;
}

void	Cpu_z80::LD_D_D() //0x52
{
	_cpuRegister.D = _cpuRegister.D;
}

void	Cpu_z80::LD_D_E() //0x53
{
	_cpuRegister.D = _cpuRegister.E;
}

void	Cpu_z80::LD_D_H() //0x54
{
	_cpuRegister.D = _cpuRegister.H;
}

void	Cpu_z80::LD_D_L() //0x55
{
	_cpuRegister.D = _cpuRegister.L;
}

void	Cpu_z80::LD_D_HL() //0x56
{
	_cpuRegister.D = _memory.read_byte(_cpuRegister.HL);
}

void	Cpu_z80::LD_D_A() //0x57
{
	_cpuRegister.D = _cpuRegister.A;
}

void	Cpu_z80::LD_E_B() //0x58
{
	_cpuRegister.E = _cpuRegister.B;
}

void	Cpu_z80::LD_E_C() //0x59
{
	_cpuRegister.E = _cpuRegister.C;
}

void	Cpu_z80::LD_E_D() //0x5a
{
	_cpuRegister.E = _cpuRegister.D;
}

void	Cpu_z80::LD_E_E() //0x5b
{
	_cpuRegister.E = _cpuRegister.E;
}

void	Cpu_z80::LD_E_H() //0x5c
{
	_cpuRegister.E = _cpuRegister.H;
}

void	Cpu_z80::LD_E_L() //0x5d
{
	_cpuRegister.E = _cpuRegister.L;

}
void	Cpu_z80::LD_E_HL() //0x5e
{
	_cpuRegister.E = _memory.read_byte(_cpuRegister.HL);
}

void	Cpu_z80::LD_E_A() //0x5f
{
	_cpuRegister.E = _cpuRegister.A;
}

void	Cpu_z80::LD_H_B() //0x60
{
	_cpuRegister.H = _cpuRegister.B;
}

void	Cpu_z80::LD_H_C() //0x61
{
	_cpuRegister.H = _cpuRegister.C;
}

void	Cpu_z80::LD_H_D() //0x62
{
	_cpuRegister.H = _cpuRegister.D;
}

void	Cpu_z80::LD_H_E() //0x63
{
	_cpuRegister.H = _cpuRegister.E;
}

void	Cpu_z80::LD_H_H() //0x64
{
	_cpuRegister.H = _cpuRegister.H;
}

void	Cpu_z80::LD_H_L() //0x65
{
	_cpuRegister.H = _cpuRegister.L;
}

void	Cpu_z80::LD_H_HL() //0x66
{
	_cpuRegister.H = _memory.read_byte(_cpuRegister.HL);
}

void	Cpu_z80::LD_H_A() //0x67
{
	_cpuRegister.H = _cpuRegister.A;
}

void	Cpu_z80::LD_L_B() //0x68
{
	_cpuRegister.L = _cpuRegister.B;
}

void	Cpu_z80::LD_L_C() //0x69
{
	_cpuRegister.L = _cpuRegister.C;
}

void	Cpu_z80::LD_L_D() //0x6a
{
	_cpuRegister.L = _cpuRegister.D;
}

void	Cpu_z80::LD_L_E() //0x6b
{
	_cpuRegister.L = _cpuRegister.E;
}

void	Cpu_z80::LD_L_H() //0x6c
{
	_cpuRegister.L = _cpuRegister.H;
}

void	Cpu_z80::LD_L_L() //0x6d
{
	_cpuRegister.L = _cpuRegister.L;
}

void	Cpu_z80::LD_L_HL() //0x6e
{
	_cpuRegister.L = _memory.read_byte(_cpuRegister.HL);
}

void	Cpu_z80::LD_L_A() //0x6f
{
	_cpuRegister.L = _cpuRegister.A;
}

void	Cpu_z80::LD_HL_B() //0x70
{
	_memory.write_word(_cpuRegister.HL, _cpuRegister.B);
}

void	Cpu_z80::LD_HL_C() //0x71
{
	_memory.write_word(_cpuRegister.HL, _cpuRegister.C);
}

void	Cpu_z80::LD_HL_D() //0x72
{
	_memory.write_word(_cpuRegister.HL, _cpuRegister.D);
}

void	Cpu_z80::LD_HL_E() //0x73
{
	_memory.write_word(_cpuRegister.HL, _cpuRegister.E);
}

void	Cpu_z80::LD_HL_H() //0x74
{
	_memory.write_word(_cpuRegister.HL, _cpuRegister.H);
}

void	Cpu_z80::LD_HL_L() //0x75
{
	_memory.write_word(_cpuRegister.HL, _cpuRegister.L);
}

void	Cpu_z80::HALT()	//0x76
{}

void	Cpu_z80::LD_HL_A() //0x77
{
	_memory.write_word(_cpuRegister.HL, _cpuRegister.A);
}

void	Cpu_z80::LD_A_B() //0x78
{
	_cpuRegister.A = _cpuRegister.B;
}

void	Cpu_z80::LD_A_C() //0x79
{
	_cpuRegister.A = _cpuRegister.C;
}

void	Cpu_z80::LD_A_D() //0x7a
{
	_cpuRegister.A = _cpuRegister.D;
}

void	Cpu_z80::LD_A_E() //0x7b
{
	_cpuRegister.A = _cpuRegister.E;
}

void	Cpu_z80::LD_A_H() //0x7c
{
	_cpuRegister.A = _cpuRegister.H;
}

void	Cpu_z80::LD_A_L() //0x7d
{
	_cpuRegister.A = _cpuRegister.L;
}

void	Cpu_z80::LD_A_HL() //0x7e
{
	_cpuRegister.A = _memory.read_byte(_cpuRegister.HL);
}

void	Cpu_z80::LD_A_A() //0x7f
{
	_cpuRegister.A = _cpuRegister.A;
}

void	Cpu_z80::ADD_A_B() //0x80
{
	_cpuRegister.n = 0;
	_cpuRegister.h = static_cast<int>(testAdd(_cpuRegister.A, _cpuRegister.B, 0xf0));
	_cpuRegister.c = static_cast<int>(testAdd(_cpuRegister.A, _cpuRegister.B, 0xff00));
	_cpuRegister.A += _cpuRegister.B;
	_cpuRegister.z = (_cpuRegister.A == 0) ? 1 : 0;
}

void	Cpu_z80::ADD_A_C() //0x81
{
	_cpuRegister.n = 0;
	_cpuRegister.h = static_cast<int>(testAdd(_cpuRegister.A, _cpuRegister.C, 0xf0));
	_cpuRegister.c = static_cast<int>(testAdd(_cpuRegister.A, _cpuRegister.C, 0xff00));
	_cpuRegister.A += _cpuRegister.C;
	_cpuRegister.z = (_cpuRegister.A == 0) ? 1 : 0;
}

void	Cpu_z80::ADD_A_D() //0x82
{
	_cpuRegister.n = 0;
	_cpuRegister.h = static_cast<int>(testAdd(_cpuRegister.A, _cpuRegister.D, 0xf0));
	_cpuRegister.c = static_cast<int>(testAdd(_cpuRegister.A, _cpuRegister.D, 0xff00));
	_cpuRegister.A += _cpuRegister.D;
	_cpuRegister.z = (_cpuRegister.A == 0) ? 1 : 0;
}

void	Cpu_z80::ADD_A_E() //0x83
{
	_cpuRegister.n = 0;
	_cpuRegister.h = static_cast<int>(testAdd(_cpuRegister.A, _cpuRegister.E, 0xf0));
	_cpuRegister.c = static_cast<int>(testAdd(_cpuRegister.A, _cpuRegister.E, 0xff00));
	_cpuRegister.A += _cpuRegister.E;
	_cpuRegister.z = (_cpuRegister.A == 0) ? 1 : 0;
}

void	Cpu_z80::ADD_A_H() //0x84
{
	_cpuRegister.n = 0;
	_cpuRegister.h = static_cast<int>(testAdd(_cpuRegister.A, _cpuRegister.H, 0xf0));
	_cpuRegister.c = static_cast<int>(testAdd(_cpuRegister.A, _cpuRegister.H, 0xff00));
	_cpuRegister.A += _cpuRegister.H;
	_cpuRegister.z = (_cpuRegister.A == 0) ? 1 : 0;
}

void	Cpu_z80::ADD_A_L() //0x85
{
	_cpuRegister.n = 0;
	_cpuRegister.h = static_cast<int>(testAdd(_cpuRegister.A, _cpuRegister.L, 0xf0));
	_cpuRegister.c = static_cast<int>(testAdd(_cpuRegister.A, _cpuRegister.L, 0xff00));
	_cpuRegister.A += _cpuRegister.L;
	_cpuRegister.z = (_cpuRegister.A == 0) ? 1 : 0;
}

void	Cpu_z80::ADD_A_HL() //0x86
{
	uint8_t tmp;

	tmp = _memory.read_byte(_cpuRegister.HL);
	_cpuRegister.n = 0;
	_cpuRegister.h = static_cast<int>(testAdd(_cpuRegister.A, tmp, 0xf0));
	_cpuRegister.c = static_cast<int>(testAdd(_cpuRegister.A, tmp, 0xff00));
	_cpuRegister.A += tmp;
	_cpuRegister.z = (_cpuRegister.A == 0) ? 1 : 0;
}

void	Cpu_z80::ADD_A_A() //0x87
{
	_cpuRegister.n = 0;
	_cpuRegister.h = static_cast<int>(testAdd(_cpuRegister.A, _cpuRegister.A, 0xf0));
	_cpuRegister.c = static_cast<int>(testAdd(_cpuRegister.A, _cpuRegister.A, 0xff00));
	_cpuRegister.A += _cpuRegister.A;
	_cpuRegister.z = (_cpuRegister.A == 0) ? 1 : 0;
}

void	Cpu_z80::ADC_A_B()	//0x88
{
	uint8_t tmp;

	tmp = _cpuRegister.B + _cpuRegister.c;
	_cpuRegister.n = 0;
	_cpuRegister.h = static_cast<int>(testAdd(_cpuRegister.A, tmp, 0xf0));
	_cpuRegister.c = static_cast<int>(testAdd(_cpuRegister.A, tmp, 0xff00));
	_cpuRegister.A += tmp;
	_cpuRegister.z = (_cpuRegister.A == 0) ? 1 : 0;
}

void	Cpu_z80::ADC_A_C()	//0x89
{
	uint8_t tmp;

	tmp = _cpuRegister.C + _cpuRegister.c;
	_cpuRegister.n = 0;
	_cpuRegister.h = static_cast<int>(testAdd(_cpuRegister.A, tmp, 0xf0));
	_cpuRegister.c = static_cast<int>(testAdd(_cpuRegister.A, tmp, 0xff00));
	_cpuRegister.A += tmp;
	_cpuRegister.z = (_cpuRegister.A == 0) ? 1 : 0;
}

void	Cpu_z80::ADC_A_D()	//0x8a
{
	uint8_t tmp;

	tmp = _cpuRegister.D + _cpuRegister.c;
	_cpuRegister.n = 0;
	_cpuRegister.h = static_cast<int>(testAdd(_cpuRegister.A, tmp, 0xf0));
	_cpuRegister.c = static_cast<int>(testAdd(_cpuRegister.A, tmp, 0xff00));
	_cpuRegister.A += tmp;
	_cpuRegister.z = (_cpuRegister.A == 0) ? 1 : 0;
}

void	Cpu_z80::ADC_A_E()	//0x8b
{
	uint8_t tmp;

	tmp = _cpuRegister.E + _cpuRegister.c;
	_cpuRegister.n = 0;
	_cpuRegister.h = static_cast<int>(testAdd(_cpuRegister.A, tmp, 0xf0));
	_cpuRegister.c = static_cast<int>(testAdd(_cpuRegister.A, tmp, 0xff00));
	_cpuRegister.A += tmp;
	_cpuRegister.z = (_cpuRegister.A == 0) ? 1 : 0;
}

void	Cpu_z80::ADC_A_H()	//0x8c
{
	uint8_t tmp;

	tmp = _cpuRegister.H + _cpuRegister.c;
	_cpuRegister.n = 0;
	_cpuRegister.h = static_cast<int>(testAdd(_cpuRegister.A, tmp, 0xf0));
	_cpuRegister.c = static_cast<int>(testAdd(_cpuRegister.A, tmp, 0xff00));
	_cpuRegister.A += tmp;
	_cpuRegister.z = (_cpuRegister.A == 0) ? 1 : 0;
}

void	Cpu_z80::ADC_A_L()	//0x8d
{
	uint8_t tmp;

	tmp = _cpuRegister.L + _cpuRegister.c;
	_cpuRegister.n = 0;
	_cpuRegister.h = static_cast<int>(testAdd(_cpuRegister.A, tmp, 0xf0));
	_cpuRegister.c = static_cast<int>(testAdd(_cpuRegister.A, tmp, 0xff00));
	_cpuRegister.A += tmp;
	_cpuRegister.z = (_cpuRegister.A == 0) ? 1 : 0;
}

void	Cpu_z80::ADC_A_HL() //0x8e
{
	uint8_t tmp;

	tmp = _memory.read_byte(_cpuRegister.HL) + _cpuRegister.c;
	_cpuRegister.n = 0;
	_cpuRegister.h = static_cast<int>(testAdd(_cpuRegister.A, tmp, 0xf0));
	_cpuRegister.c = static_cast<int>(testAdd(_cpuRegister.A, tmp, 0xff00));
	_cpuRegister.A += tmp;
	_cpuRegister.z = (_cpuRegister.A == 0) ? 1 : 0;
}

void	Cpu_z80::ADC_A_A()	//0x8f
{
	uint8_t tmp;

	tmp = _cpuRegister.A + _cpuRegister.c;
	_cpuRegister.n = 0;
	_cpuRegister.h = static_cast<int>(testAdd(_cpuRegister.A, tmp, 0xf0));
	_cpuRegister.c = static_cast<int>(testAdd(_cpuRegister.A, tmp, 0xff00));
	_cpuRegister.A += tmp;
	_cpuRegister.z = (_cpuRegister.A == 0) ? 1 : 0;
}

void	Cpu_z80::SUB_B() //0x90
{
	_cpuRegister.n = 1;
	_cpuRegister.h = static_cast<int>(testSub(_cpuRegister.A, _cpuRegister.B, 0x0f));
	_cpuRegister.c = static_cast<int>(testSub(_cpuRegister.A, _cpuRegister.B, 0xff00));
	_cpuRegister.A -= _cpuRegister.B;
	_cpuRegister.z = (_cpuRegister.A == 0) ? 1 : 0;
}

void	Cpu_z80::SUB_C() //0x91
{
	_cpuRegister.n = 1;
	_cpuRegister.h = static_cast<int>(testSub(_cpuRegister.A, _cpuRegister.C, 0x0f));
	_cpuRegister.c = static_cast<int>(testSub(_cpuRegister.A, _cpuRegister.C, 0xff00));
	_cpuRegister.A -= _cpuRegister.C;
	_cpuRegister.z = (_cpuRegister.A == 0) ? 1 : 0;
}

void	Cpu_z80::SUB_D() //0x92
{
	_cpuRegister.n = 1;
	_cpuRegister.h = static_cast<int>(testSub(_cpuRegister.A, _cpuRegister.D, 0x0f));
	_cpuRegister.c = static_cast<int>(testSub(_cpuRegister.A, _cpuRegister.D, 0xff00));
	_cpuRegister.A -= _cpuRegister.D;
	_cpuRegister.z = (_cpuRegister.A == 0) ? 1 : 0;
}

void	Cpu_z80::SUB_E() //0x93
{
	_cpuRegister.n = 1;
	_cpuRegister.h = static_cast<int>(testSub(_cpuRegister.A, _cpuRegister.E, 0x0f));
	_cpuRegister.c = static_cast<int>(testSub(_cpuRegister.A, _cpuRegister.E, 0xff00));
	_cpuRegister.A -= _cpuRegister.E;
	_cpuRegister.z = (_cpuRegister.A == 0) ? 1 : 0;
}

void	Cpu_z80::SUB_H() //0x94
{
	_cpuRegister.n = 1;
	_cpuRegister.h = static_cast<int>(testSub(_cpuRegister.A, _cpuRegister.H, 0x0f));
	_cpuRegister.c = static_cast<int>(testSub(_cpuRegister.A, _cpuRegister.H, 0xff00));
	_cpuRegister.A -= _cpuRegister.H;
	_cpuRegister.z = (_cpuRegister.A == 0) ? 1 : 0;
}

void	Cpu_z80::SUB_L() //0x95
{
	_cpuRegister.n = 1;
	_cpuRegister.h = static_cast<int>(testSub(_cpuRegister.A, _cpuRegister.L, 0x0f));
	_cpuRegister.c = static_cast<int>(testSub(_cpuRegister.A, _cpuRegister.L, 0xff00));
	_cpuRegister.A -= _cpuRegister.L;
	_cpuRegister.z = (_cpuRegister.A == 0) ? 1 : 0;
}

void	Cpu_z80::SUB_HL() //0x96
{
	uint8_t tmp;

	tmp = _memory.read_byte(_cpuRegister.HL);
	_cpuRegister.n = 1;
	_cpuRegister.h = static_cast<int>(testSub(_cpuRegister.A, tmp, 0x0f));
	_cpuRegister.c = static_cast<int>(testSub(_cpuRegister.A, tmp, 0xff00));
	_cpuRegister.A += tmp;
	_cpuRegister.z = (_cpuRegister.A == 0) ? 1 : 0;
}

void	Cpu_z80::SUB_A() //0x97
{
	_cpuRegister.n = 1;
	_cpuRegister.h = static_cast<int>(testSub(_cpuRegister.A, _cpuRegister.A, 0x0f));
	_cpuRegister.c = static_cast<int>(testSub(_cpuRegister.A, _cpuRegister.A, 0xff00));
	_cpuRegister.A -= _cpuRegister.A;
	_cpuRegister.z = (_cpuRegister.A == 0) ? 1 : 0;
}

void	Cpu_z80::SBC_A_B() //0x98
{
	uint8_t	tmp;

	tmp = _cpuRegister.B + _cpuRegister.c;
	_cpuRegister.n = 1;
	_cpuRegister.h = static_cast<int>(testSub(_cpuRegister.A, tmp, 0x0f));
	_cpuRegister.c = static_cast<int>(testSub(_cpuRegister.A, tmp, 0xff00));
	_cpuRegister.A -= tmp;
	_cpuRegister.z = (_cpuRegister.A == 0) ? 1 : 0;
}

void	Cpu_z80::SBC_A_C() //0x99
{
	uint8_t	tmp;

	tmp = _cpuRegister.C + _cpuRegister.c;

	_cpuRegister.n = 1;
	_cpuRegister.h = static_cast<int>(testSub(_cpuRegister.A, tmp, 0x0f));
	_cpuRegister.c = static_cast<int>(testSub(_cpuRegister.A, tmp, 0xff00));
	_cpuRegister.A -= tmp;
	_cpuRegister.z = (_cpuRegister.A == 0) ? 1 : 0;
}

void	Cpu_z80::SBC_A_D() //0x9a
{
	uint8_t	tmp;

	tmp = _cpuRegister.D + _cpuRegister.c;
	_cpuRegister.n = 1;
	_cpuRegister.h = static_cast<int>(testSub(_cpuRegister.A, tmp, 0x0f));
	_cpuRegister.c = static_cast<int>(testSub(_cpuRegister.A, tmp, 0xff00));
	_cpuRegister.A -= tmp;
	_cpuRegister.z = (_cpuRegister.A == 0) ? 1 : 0;
}

void	Cpu_z80::SBC_A_E() //0x9b
{
	uint8_t	tmp;

	tmp = _cpuRegister.E + _cpuRegister.c;
	_cpuRegister.n = 1;
	_cpuRegister.h = static_cast<int>(testSub(_cpuRegister.A, tmp, 0x0f));
	_cpuRegister.c = static_cast<int>(testSub(_cpuRegister.A, tmp, 0xff00));
	_cpuRegister.A -= tmp;
	_cpuRegister.z = (_cpuRegister.A == 0) ? 1 : 0;
}

void	Cpu_z80::SBC_A_H() //0x9c
{
	uint8_t	tmp;

	tmp = _cpuRegister.H + _cpuRegister.c;
	_cpuRegister.n = 1;
	_cpuRegister.h = static_cast<int>(testSub(_cpuRegister.A, tmp, 0x0f));
	_cpuRegister.c = static_cast<int>(testSub(_cpuRegister.A, tmp, 0xff00));
	_cpuRegister.A -= tmp;
	_cpuRegister.z = (_cpuRegister.A == 0) ? 1 : 0;
}

void	Cpu_z80::SBC_A_L() //0x9d
{
	uint8_t	tmp;

	tmp = _cpuRegister.L + _cpuRegister.c;
	_cpuRegister.n = 1;
	_cpuRegister.h = static_cast<int>(testSub(_cpuRegister.A, tmp, 0x0f));
	_cpuRegister.c = static_cast<int>(testSub(_cpuRegister.A, tmp, 0xff00));
	_cpuRegister.A -= tmp;
	_cpuRegister.z = (_cpuRegister.A == 0) ? 1 : 0;
}

void	Cpu_z80::SBC_A_HL() //0x9e
{
	uint8_t tmp;

	tmp = _memory.read_byte(_cpuRegister.HL) + _cpuRegister.c;
	_cpuRegister.n = 0;
	_cpuRegister.h = static_cast<int>(testAdd(_cpuRegister.A, tmp, 0xf0));
	_cpuRegister.c = static_cast<int>(testAdd(_cpuRegister.A, tmp, 0xff00));
	_cpuRegister.A += tmp;
	_cpuRegister.z = (_cpuRegister.A == 0) ? 1 : 0;
}

void	Cpu_z80::SBC_A_A() //0x9f
{
	uint8_t	tmp;

	tmp = _cpuRegister.A + _cpuRegister.c;
	_cpuRegister.n = 1;
	_cpuRegister.h = static_cast<int>(testSub(_cpuRegister.A, tmp, 0x0f));
	_cpuRegister.c = static_cast<int>(testSub(_cpuRegister.A, tmp, 0xff00));
	_cpuRegister.A -= tmp;
	_cpuRegister.z = (_cpuRegister.A == 0) ? 1 : 0;
}

void	Cpu_z80::AND_B() //0xa0
{
	_cpuRegister.n = 0;
	_cpuRegister.h = 1;
	_cpuRegister.c = 0;
	_cpuRegister.A = (_cpuRegister.A & _cpuRegister.B);
	_cpuRegister.z = (_cpuRegister.A == 0) ? 1 : 0;
}

void	Cpu_z80::AND_C() //0xa1
{
	_cpuRegister.n = 0;
	_cpuRegister.h = 1;
	_cpuRegister.c = 0;
	_cpuRegister.A = (_cpuRegister.A & _cpuRegister.C);
	_cpuRegister.z = (_cpuRegister.A == 0) ? 1 : 0;
}

void	Cpu_z80::AND_D() //0xa2
{
	_cpuRegister.n = 0;
	_cpuRegister.h = 1;
	_cpuRegister.c = 0;
	_cpuRegister.A = (_cpuRegister.A & _cpuRegister.D);
	_cpuRegister.z = (_cpuRegister.A == 0) ? 1 : 0;
}

void	Cpu_z80::AND_E() //0xa3
{
	_cpuRegister.n = 0;
	_cpuRegister.h = 1;
	_cpuRegister.c = 0;
	_cpuRegister.A = (_cpuRegister.A & _cpuRegister.E);
	_cpuRegister.z = (_cpuRegister.A == 0) ? 1 : 0;
}

void	Cpu_z80::AND_H() //0xa4
{
	_cpuRegister.n = 0;
	_cpuRegister.h = 1;
	_cpuRegister.c = 0;
	_cpuRegister.A = (_cpuRegister.A & _cpuRegister.H);
	_cpuRegister.z = (_cpuRegister.A == 0) ? 1 : 0;
}

void	Cpu_z80::AND_L() //0xa5
{
	_cpuRegister.n = 0;
	_cpuRegister.h = 1;
	_cpuRegister.c = 0;
	_cpuRegister.A = (_cpuRegister.A & _cpuRegister.L);
	_cpuRegister.z = (_cpuRegister.A == 0) ? 1 : 0;
}

void	Cpu_z80::AND_HL() //0xa6
{
	_cpuRegister.n = 0;
	_cpuRegister.h = 1;
	_cpuRegister.c = 0;
	_cpuRegister.A = (_cpuRegister.A & _memory.read_byte(_cpuRegister.HL));
	_cpuRegister.z = (_cpuRegister.A == 0) ? 1 : 0;
}

void	Cpu_z80::AND_A() //0xa7
{
	_cpuRegister.n = 0;
	_cpuRegister.h = 1;
	_cpuRegister.c = 0;
	_cpuRegister.A = (_cpuRegister.A & _cpuRegister.A);
	_cpuRegister.z = (_cpuRegister.A == 0) ? 1 : 0;
}

void	Cpu_z80::XOR_B() //0xa8
{
	_cpuRegister.n = 0;
	_cpuRegister.h = 0;
	_cpuRegister.c = 0;
	_cpuRegister.A = (_cpuRegister.A ^ _cpuRegister.B);
	_cpuRegister.z = (_cpuRegister.A == 0) ? 1 : 0;
}

void	Cpu_z80::XOR_C() //0xa9
{
	_cpuRegister.n = 0;
	_cpuRegister.h = 0;
	_cpuRegister.c = 0;
	_cpuRegister.A = (_cpuRegister.A ^ _cpuRegister.C);
	_cpuRegister.z = (_cpuRegister.A == 0) ? 1 : 0;
}

void	Cpu_z80::XOR_D() //0xaa
{
	_cpuRegister.n = 0;
	_cpuRegister.h = 0;
	_cpuRegister.c = 0;
	_cpuRegister.A = (_cpuRegister.A ^ _cpuRegister.D);
	_cpuRegister.z = (_cpuRegister.A == 0) ? 1 : 0;
}

void	Cpu_z80::XOR_E() //0xab
{
	_cpuRegister.n = 0;
	_cpuRegister.h = 0;
	_cpuRegister.c = 0;
	_cpuRegister.A = (_cpuRegister.A ^ _cpuRegister.E);
	_cpuRegister.z = (_cpuRegister.A == 0) ? 1 : 0;
}

void	Cpu_z80::XOR_H() //0xac
{
	_cpuRegister.n = 0;
	_cpuRegister.h = 0;
	_cpuRegister.c = 0;
	_cpuRegister.A = (_cpuRegister.A ^ _cpuRegister.H);
	_cpuRegister.z = (_cpuRegister.A == 0) ? 1 : 0;
}

void	Cpu_z80::XOR_L() //0xad
{
	_cpuRegister.n = 0;
	_cpuRegister.h = 0;
	_cpuRegister.c = 0;
	_cpuRegister.A = (_cpuRegister.A ^ _cpuRegister.L);
	_cpuRegister.z = (_cpuRegister.A == 0) ? 1 : 0;
}

void	Cpu_z80::XOR_HL() //0xae
{
	_cpuRegister.n = 0;
	_cpuRegister.h = 0;
	_cpuRegister.c = 0;
	_cpuRegister.A = (_cpuRegister.A ^ _memory.read_byte(_cpuRegister.HL));
	_cpuRegister.z = (_cpuRegister.A == 0) ? 1 : 0;
}

void	Cpu_z80::XOR_A() //0xaf
{
	_cpuRegister.n = 0;
	_cpuRegister.h = 0;
	_cpuRegister.c = 0;
	_cpuRegister.A = (_cpuRegister.A ^ _cpuRegister.A);
	_cpuRegister.z = (_cpuRegister.A == 0) ? 1 : 0;
}

void	Cpu_z80::OR_B() //0xb0
{
	_cpuRegister.n = 0;
	_cpuRegister.h = 0;
	_cpuRegister.c = 0;
	_cpuRegister.A = (_cpuRegister.A | _cpuRegister.B);
	_cpuRegister.z = (_cpuRegister.A == 0) ? 1 : 0;
}

void	Cpu_z80::OR_C() //0xb1
{
	_cpuRegister.n = 0;
	_cpuRegister.h = 0;
	_cpuRegister.c = 0;
	_cpuRegister.A = (_cpuRegister.A | _cpuRegister.C);
	_cpuRegister.z = (_cpuRegister.A == 0) ? 1 : 0;
}

void	Cpu_z80::OR_D() //0xb2
{
	_cpuRegister.n = 0;
	_cpuRegister.h = 1;
	_cpuRegister.c = 0;
	_cpuRegister.A = (_cpuRegister.A | _cpuRegister.D);
	_cpuRegister.z = (_cpuRegister.A == 0) ? 1 : 0;
}

void	Cpu_z80::OR_E() //0xb3
{
	_cpuRegister.n = 0;
	_cpuRegister.h = 0;
	_cpuRegister.c = 0;
	_cpuRegister.A = (_cpuRegister.A | _cpuRegister.E);
	_cpuRegister.z = (_cpuRegister.A == 0) ? 1 : 0;
}

void	Cpu_z80::OR_H() //0xb4
{
	_cpuRegister.n = 0;
	_cpuRegister.h = 0;
	_cpuRegister.c = 0;
	_cpuRegister.A = (_cpuRegister.A | _cpuRegister.H);
	_cpuRegister.z = (_cpuRegister.A == 0) ? 1 : 0;
}

void	Cpu_z80::OR_L() //0xb5
{
	_cpuRegister.n = 0;
	_cpuRegister.h = 0;
	_cpuRegister.c = 0;
	_cpuRegister.A = (_cpuRegister.A | _cpuRegister.L);
	_cpuRegister.z = (_cpuRegister.A == 0) ? 1 : 0;
}

void	Cpu_z80::OR_HL() //0xb6
{
	_cpuRegister.n = 0;
	_cpuRegister.h = 0;
	_cpuRegister.c = 0;
	_cpuRegister.A = (_cpuRegister.A | _memory.read_byte(_cpuRegister.HL));
	_cpuRegister.z = (_cpuRegister.A == 0) ? 1 : 0;
}

void	Cpu_z80::OR_A() //0xb7
{
	_cpuRegister.n = 0;
	_cpuRegister.h = 0;
	_cpuRegister.c = 0;
	_cpuRegister.A = (_cpuRegister.A | _cpuRegister.A);
	_cpuRegister.z = (_cpuRegister.A == 0) ? 1 : 0;
}

void	Cpu_z80::CP_B() //0xb8
{
	_cpuRegister.n = 1;
	_cpuRegister.h = (testSub(_cpuRegister.A, _cpuRegister.B, 0xf0)) ? 1 : 0;
	_cpuRegister.c = (_cpuRegister.A > _cpuRegister.B) ? 1 : 0;
	_cpuRegister.z = (_cpuRegister.A == _cpuRegister.B) ? 1 : 0;
}

void	Cpu_z80::CP_C() //0xb9
{
	_cpuRegister.n = 1;
	_cpuRegister.h = (testSub(_cpuRegister.A, _cpuRegister.C, 0xf0)) ? 1 : 0;
	_cpuRegister.c = (_cpuRegister.A > _cpuRegister.C) ? 1 : 0;
	_cpuRegister.z = (_cpuRegister.A == _cpuRegister.C) ? 1 : 0;
}

void	Cpu_z80::CP_D() //0xba
{
	_cpuRegister.n = 1;
	_cpuRegister.h = (testSub(_cpuRegister.A, _cpuRegister.D, 0xf0)) ? 1 : 0;
	_cpuRegister.c = (_cpuRegister.A > _cpuRegister.D) ? 1 : 0;
	_cpuRegister.z = (_cpuRegister.A == _cpuRegister.D) ? 1 : 0;
}

void	Cpu_z80::CP_E() //0xbb
{
	_cpuRegister.n = 1;
	_cpuRegister.h = (testSub(_cpuRegister.A, _cpuRegister.E, 0xf0)) ? 1 : 0;
	_cpuRegister.c = (_cpuRegister.A > _cpuRegister.E) ? 1 : 0;
	_cpuRegister.z = (_cpuRegister.A == _cpuRegister.E) ? 1 : 0;
}

void	Cpu_z80::CP_H() //0xbc
{
	_cpuRegister.n = 1;
	_cpuRegister.h = (testSub(_cpuRegister.A, _cpuRegister.H, 0xf0)) ? 1 : 0;
	_cpuRegister.c = (_cpuRegister.A > _cpuRegister.H) ? 1 : 0;
	_cpuRegister.z = (_cpuRegister.A == _cpuRegister.H) ? 1 : 0;
}

void	Cpu_z80::CP_L() //0xbd
{
	_cpuRegister.n = 1;
	_cpuRegister.h = (testSub(_cpuRegister.A, _cpuRegister.L, 0xf0)) ? 1 : 0;
	_cpuRegister.c = (_cpuRegister.A > _cpuRegister.L) ? 1 : 0;
	_cpuRegister.z = (_cpuRegister.A == _cpuRegister.L) ? 1 : 0;
}

void	Cpu_z80::CP_HL() //0xbe
{
	uint8_t tmp;

	tmp = _memory.read_byte(_cpuRegister.HL);
	_cpuRegister.n = 1;
	_cpuRegister.h = (testSub(_cpuRegister.A, tmp, 0xf0)) ? 1 : 0;
	_cpuRegister.c = (_cpuRegister.A > tmp) ? 1 : 0;
	_cpuRegister.z = (_cpuRegister.A == tmp) ? 1 : 0;
}

void	Cpu_z80::CP_A() //0xbf
{
	_cpuRegister.n = 1;
	_cpuRegister.h = (testSub(_cpuRegister.A, _cpuRegister.A, 0xf0)) ? 1 : 0;
	_cpuRegister.c = 0;
	_cpuRegister.z = 1;
}

void	Cpu_z80::RET_NZ() //0xc0
{
	if (_cpuRegister.z == 0)
		RET();
}

void	Cpu_z80::POP_BC() //0xc1 
{
	_cpuRegister.BC = _memory.read_word(_cpuRegister.SP);
	_cpuRegister.SP += 2;
}

void	Cpu_z80::JP_NZ_n() //0xc2
{
	if (_cpuRegister.z == 0)
		JP_n();
}

void	Cpu_z80::JP_n()		//0xc3
{
	_cpuRegister.PC = _memory.read_word(_cpuRegister.PC + 1);
}

void	Cpu_z80::CALL_NZ_n() //0xc4
{
	if (_cpuRegister.z == 0)
		CALL_n();
}

void	Cpu_z80::PUSH_BC() //0xc5
{
	_cpuRegister.SP -= 2;
	_memory.write_word(_cpuRegister.SP, _cpuRegister.BC);
}

void	Cpu_z80::ADD_A_n() //0xc6
{
	uint8_t n;

	n = _memory.read_byte(_cpuRegister.PC + 1);
	_cpuRegister.n = 0;
	_cpuRegister.h = static_cast<int>(testAdd(_cpuRegister.A, n, 0xf0));
	_cpuRegister.c = static_cast<int>(testAdd(_cpuRegister.A, n, 0xff00));
	_cpuRegister.A += n;
	_cpuRegister.z = (_cpuRegister.A == 0) ? 1 : 0;

}

void	Cpu_z80::RST_00H()		//0xc7
{
	_cpuRegister.SP -= 2;
	_memory.write_word(_cpuRegister.SP, _memory.read_word(_cpuRegister.PC));
	_cpuRegister.PC = 0x0000;
}

void	Cpu_z80::RET_Z() //0xc9
{
	if (_cpuRegister.z == 1)
		RET();
}

void	Cpu_z80::RET() //0xc9
{
	_cpuRegister.PC = _memory.read_word(_cpuRegister.SP);
	_cpuRegister.SP += 2;
}

void	Cpu_z80::JP_Z_n() //0xca
{
	if (_cpuRegister.z == 1)
		JP_n();
}

void	Cpu_z80::CALL_Z_n() //0xcc
{
	if (_cpuRegister.z == 1)
		CALL_n();
}

void	Cpu_z80::CALL_n() //0xcd
{
	  uint16_t nn;

	 nn = _memory.read_word(_cpuRegister.PC + 1);
	 _memory.write_word(_cpuRegister.SP, nn);
	 JP_n();
}

void	Cpu_z80::ADC_A_n() //0xce
{
	uint8_t tmp;

	tmp = _memory.read_byte(_cpuRegister.PC + 1) + _cpuRegister.c;
	_cpuRegister.n = 0;
	_cpuRegister.h = static_cast<int>(testAdd(_cpuRegister.A, tmp, 0xf0));
	_cpuRegister.c = static_cast<int>(testAdd(_cpuRegister.A, tmp, 0xff00));
	_cpuRegister.A += tmp;
	_cpuRegister.z = (_cpuRegister.A == 0) ? 1 : 0;

}

void	Cpu_z80::RST_08H() //0xcf
{
	_cpuRegister.SP -= 2;
	_memory.write_word(_cpuRegister.SP, _memory.read_word(_cpuRegister.PC));
	_cpuRegister.PC = 0x0008;
}

void	Cpu_z80::RET_NC() //0xd0
{
	if (_cpuRegister.c == 0)
		RET();
}

void	Cpu_z80::POP_DE() //0xd1 
{
	_cpuRegister.DE = _memory.read_word(_cpuRegister.SP);
	_cpuRegister.SP += 2;
}

void	Cpu_z80::JP_NC_n() //0xd2
{
	if (_cpuRegister.c == 0)
		JP_n();
}

void	Cpu_z80::CALL_NC_n() //0xd4
{
	if (_cpuRegister.c == 0)
		CALL_n();
}

void	Cpu_z80::PUSH_DE() //0xd5
{
	_cpuRegister.SP -= 2;
	_memory.write_word(_cpuRegister.SP, _cpuRegister.DE);
}

void	Cpu_z80::SUB_n() //0xd6
{
	uint8_t n;

	n =  _memory.read_byte(_cpuRegister.PC + 1);
	_cpuRegister.n = 1;
	_cpuRegister.h = static_cast<int>(testSub(_cpuRegister.A, n, 0x0f));
	_cpuRegister.c = static_cast<int>(testSub(_cpuRegister.A, n, 0xff00));
	_cpuRegister.A -= _cpuRegister.B;
	_cpuRegister.z = (_cpuRegister.A == 0) ? 1 : 0;
}

void	Cpu_z80::RST_10H() //0xd7
{
	_cpuRegister.SP -= 2;
	_memory.write_word(_cpuRegister.SP, _memory.read_word(_cpuRegister.PC));
	_cpuRegister.PC = 0x0010;
}

void	Cpu_z80::RET_C() //0xd8
{
	if (_cpuRegister.c == 1)
		RET();
}

void	Cpu_z80::RETI() //0xd9
{
	RET();
	//TODO: ADD BOOL TO ALLOW INTERUPTS
}

void	Cpu_z80::JP_C_n() //0xda
{
	if (_cpuRegister.c == 1)
		JP_n();
}

void	Cpu_z80::CALL_C_n() //0xdc
{
	if (_cpuRegister.c == 1)
		CALL_n();
}

void	Cpu_z80::SBC_A_n()	//0xde
{
	uint8_t	tmp;

	tmp = _memory.read_byte(_cpuRegister.PC + 1) + _cpuRegister.c;
	_cpuRegister.n = 1;
	_cpuRegister.h = static_cast<int>(testSub(_cpuRegister.A, tmp, 0x0f));
	_cpuRegister.c = static_cast<int>(testSub(_cpuRegister.A, tmp, 0xff00));
	_cpuRegister.A -= tmp;
	_cpuRegister.z = (_cpuRegister.A == 0) ? 1 : 0;

}

void	Cpu_z80::RST_18H() //0xdf
{
	_cpuRegister.SP -= 2;
	_memory.write_word(_cpuRegister.SP, _memory.read_word(_cpuRegister.PC));
	_cpuRegister.PC = 0x0018;
}

void	Cpu_z80::LDH_n_A() //0xe0
{
	uint8_t n;

	n = _memory.read_byte(_cpuRegister.PC + 1);
	_memory.write_byte(0xff00 + n, _cpuRegister.A);
}

void	Cpu_z80::POP_HL() //0xe1 
{
	_cpuRegister.HL = _memory.read_word(_cpuRegister.SP);
	_cpuRegister.SP += 2;
}

void	Cpu_z80::LD_CC_A()	//0xe2
{
	_memory.write_word((0x0000 + _cpuRegister.c), _cpuRegister.A);
}

void	Cpu_z80::PUSH_HL() //0xe5
{
	_cpuRegister.SP -= 2;
	_memory.write_word(_cpuRegister.SP, _cpuRegister.HL);
}

void	Cpu_z80::AND_n()	//0xe6
{
	_cpuRegister.n = 0;
	_cpuRegister.h = 1;
	_cpuRegister.c = 0;
	_cpuRegister.A = (_cpuRegister.A & _memory.read_byte(_cpuRegister.PC + 1));
	_cpuRegister.z = (_cpuRegister.A == 0) ? 1 : 0;
}

void	Cpu_z80::RST_20H() //0xe7
{
	_cpuRegister.SP -= 2;
	_memory.write_word(_cpuRegister.SP, _memory.read_word(_cpuRegister.PC));
	_cpuRegister.PC = 0x0020;
}

void	Cpu_z80::ADD_SP_n()	//0xe8
{
	int8_t	n;
	uint8_t a = _memory.read_byte(_cpuRegister.PC + 1);
	a = 0x00;
	n = 0;
	n |= a;
	_cpuRegister.h = static_cast<int>(testAdd(_cpuRegister.SP, n, 0xf000));
	_cpuRegister.c = static_cast<int>(testAdd(_cpuRegister.SP, n, 0xffff0000));
	_cpuRegister.SP += n;
}

void	Cpu_z80::JP_HL() //0xe9
{
	_cpuRegister.PC = _memory.read_word(_cpuRegister.HL);
}

void	Cpu_z80::LD_n_A() //0xea
{
	uint8_t nn;

	nn = _memory.read_word(_cpuRegister.PC + 1);
	_memory.write_byte(nn, _cpuRegister.A);
}

void	Cpu_z80::XOR_n() //0xee
{
	uint8_t n;

	n = _memory.read_byte(_cpuRegister.PC + 1);
	_cpuRegister.n = 0;
	_cpuRegister.h = 0;
	_cpuRegister.c = 0;
	_cpuRegister.A = (_cpuRegister.A ^ n);
	_cpuRegister.z = (_cpuRegister.A == 0) ? 1 : 0;
}

void	Cpu_z80::RST_28H() //0xef
{
	_cpuRegister.SP -= 2;
	_memory.write_word(_cpuRegister.SP, _memory.read_word(_cpuRegister.PC));
	_cpuRegister.PC = 0x0028;
}

void	Cpu_z80::LDH_A_n() //0xf0
{
	uint8_t n;

	n = _memory.read_byte(_cpuRegister.PC + 1);
	_cpuRegister.A = n;
}

void	Cpu_z80::POP_AF() //0xf1 
{
	_cpuRegister.AF = _memory.read_word(_cpuRegister.SP);
	_cpuRegister.SP += 2;
}

void	Cpu_z80::LD_A_CC()	//0xf2
{
	_cpuRegister.A = _memory.read_byte(0x0000 + _cpuRegister.c);
}

void	Cpu_z80::PUSH_AF() //0xf5
{
	_cpuRegister.SP -= 2;
	_memory.write_word(_cpuRegister.SP, _cpuRegister.AF);
}

void	Cpu_z80::OR_n() //0xf6
{
	uint8_t n;

	n = _memory.read_byte(_cpuRegister.PC + 1);
	_cpuRegister.n = 0;
	_cpuRegister.h = 0;
	_cpuRegister.c = 0;
	_cpuRegister.A = (_cpuRegister.A | n);
	_cpuRegister.z = (_cpuRegister.A == 0) ? 1 : 0;
}

void	Cpu_z80::RST_30H() //0xf7
{
	_cpuRegister.SP -= 2;
	_memory.write_word(_cpuRegister.SP, _memory.read_word(_cpuRegister.PC));
	_cpuRegister.PC = 0x0030;
}

void	Cpu_z80::LD_HL_SP_n()	//0xf8
{
	int8_t	n;
	uint8_t a = _memory.read_byte(_cpuRegister.PC + 1);
	a = 0x00;
	n = 0;
	n |= a;
	_cpuRegister.h = static_cast<int>(testAdd(_cpuRegister.SP, n, 0xf000));
	_cpuRegister.c = static_cast<int>(testAdd(_cpuRegister.SP, n, 0xffff0000));
	_cpuRegister.HL = _cpuRegister.SP + n;
}

void	Cpu_z80::LD_SP_HL()	//0xf9
{
	_memory.write_word(_cpuRegister.SP, _cpuRegister.HL);
}

void	Cpu_z80::LD_A_b() //0xfa
{
	uint8_t nn;

	nn = _memory.read_word(_cpuRegister.PC + 1);
	_cpuRegister.A = nn;
}

void	Cpu_z80::CP_n()	//0xfe
{
	uint8_t n;

	n = _memory.read_byte(_cpuRegister.PC + 1);
	_cpuRegister.n = 1;
	_cpuRegister.h = (testSub(_cpuRegister.A, n, 0xf0)) ? 1 : 0;
	_cpuRegister.c = (_cpuRegister.A > n) ? 1 : 0;
	_cpuRegister.z = (_cpuRegister.A == n) ? 1 : 0;
}

void	Cpu_z80::RST_38H() //0xff
{
	_cpuRegister.SP -= 2;
	_memory.write_word(_cpuRegister.SP, _memory.read_word(_cpuRegister.PC));
	_cpuRegister.PC = 0x0038;
}


void	Cpu_z80::STOP()			//0x10     Not done yet
{}
void	Cpu_z80::PREFIX_CB()	//0xcb     Not done yet
{}
void	Cpu_z80::DI()	//0xf3     Not done yet
{}
void	Cpu_z80::EI()	//0xfb
{}
