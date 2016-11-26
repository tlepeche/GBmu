
#pragma once

typedef struct s_sprite
{
	uint8_t		y_pos;		//y pos of top-left corner
							// spriteScreenPosY = y_pos - 16;
	uint8_t		x_pos;		//x pos if top-left corner
	uint8_t		tile_nbr;	//data tile number (0 - 255)
	union
	{
		uint8_t		options;
		struct
		{
			uint8_t		B0:1;
			uint8_t		B1:1;
			uint8_t		B2:1;
			uint8_t		B3:1;
			uint8_t		pal:1;			//0 -> obj from palette #0, 1 -> pal #1
			uint8_t		x_flip:1;		//0 -> normal, 1 -> hori flip
			uint8_t		y_flip:1;		//0 -> normal, 1 -> vertical flip
			uint8_t		bckgrd_prio:1;	//0 -> above bckgrd, 1 -> below
		};
	};
}			t_sprite;
