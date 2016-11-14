
#pragma once

typedef struct s_gpuControl
{
	union
	{
		uint8_t		LCD;
		struct
		{
			uint8_t		background:1; // 0x01
			uint8_t		sprite:1;
			uint8_t		sprite_size:1;
			uint8_t		tile_map:1; // 0x08
			uint8_t		tile_set:1; // 0x10
			uint8_t		window:1;
			uint8_t		wtile_map:1;
			uint8_t		display:1; // 0x80


		};
	};
}			t_gpuControl;
