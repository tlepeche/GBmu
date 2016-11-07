#ifndef REGISTER_H_
# define REGISTER_H_

#include <cstdint>

typedef struct			s_register
{
	union
	{
		struct
		{
			union
			{
				struct
				{
					uint8_t	B0:1;
					uint8_t	B1:1;
					uint8_t	B2:1;
					uint8_t	B3:1;
					uint8_t	c:1;
					uint8_t	h:1;
					uint8_t	n:1;
					uint8_t	z:1;
				};
				uint8_t	F;
			};
			uint8_t		A;
		};
		uint16_t		AF;
	};
	union
	{
		struct
		{
			uint8_t		C;
			uint8_t		B;
		};
		uint16_t		BC;
	};
	union
	{
		struct
		{
			uint8_t		E;
			uint8_t		D;
		};
		uint16_t		DE;
	};
	union
	{
		struct
		{
			uint8_t		L;
			uint8_t		H;
		};
		uint16_t		HL;
	};
	uint16_t			PC;
	uint16_t			SP;

}						t_register;

#endif
