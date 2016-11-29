
#pragma once

typedef struct	s_color15
{
	union {
		uint16_t	color;
		struct {
			uint16_t	useless:1;
			uint16_t	b:5;
			uint16_t	v:5;
			uint16_t	r:5;
		};
	};
}				t_color15;
