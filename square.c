#include <gb/gb.h>
#include "game.h"

#define SQUARE_SIZE 16U
#define SQUARE_X_VALID(x) \
	((x)>=SQUARE_SIZE / 2 && (x)<=SCREENWIDTH - SQUARE_SIZE / 2)
#define SQUARE_Y_VALID(y) \
	((y)>=SQUARE_SIZE / 2 && (y)<=SCREENHEIGHT - SQUARE_SIZE / 2)

/* Every pixel uses sprite palette entry 3, making four sprites one solid
 * square. */
static const uint8_t solid_tile[16]={
	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff
};

static uint8_t move_square(uint8_t center_x,uint8_t center_y)
{
	uint8_t sprite_x;
	uint8_t sprite_y;

	/* Reject an invalid center without moving any sprites. */
	if(!SQUARE_X_VALID(center_x) || !SQUARE_Y_VALID(center_y))
		return 1;

	sprite_x=center_x - SQUARE_SIZE / 2 + DEVICE_SPRITE_PX_OFFSET_X;
	sprite_y=center_y - SQUARE_SIZE / 2 + DEVICE_SPRITE_PX_OFFSET_Y;

	/* Place four 8x8 sprites around the square's center. */
	move_sprite(0,sprite_x,sprite_y);
	move_sprite(1,sprite_x + SQUARE_SIZE / 2,sprite_y);
	move_sprite(2,sprite_x,sprite_y + SQUARE_SIZE / 2);
	move_sprite(3,sprite_x + SQUARE_SIZE / 2,sprite_y + SQUARE_SIZE / 2);
	return 0;
}

void init_square(uint8_t center_x,uint8_t center_y)
{
	uint8_t sprite;

	set_sprite_data(0,1,solid_tile);
	for(sprite=0;sprite<4;sprite++){
		set_sprite_tile(sprite,0);
		set_sprite_prop(sprite,0);
	}
	move_square(center_x,center_y);
	SPRITES_8x8;
}

void update_square(struct game_state *state)
{
	/* The coordinates are normally stored in a uint8_t, but using int16_t
	 * here prevents mistakenly interpreting an invalid coordinate as
	 * valid in motion mode
	 *
	 * Example: If the x coordinate is updated to 277, it becomes 21 when
	 * stored as uint8_t, which would pass the validation checks if
	 * center_x is a uint8_t.
	 *
	 * P.S. This only matters when you are bored enough to hit the arrow
	 * keys so many times. */
	int16_t center_x=state->square_center_x;
	int16_t center_y=state->square_center_y;

	if(state->flags & FLG_VELOCITY){
		/* Change velocity
		 *
		 * The velocity components are stored as int8_t, so it will
		 * wrap around if you click way too many times. */
		if(state->newly_pressed & J_LEFT) state->velocity_x--;
		if(state->newly_pressed & J_RIGHT) state->velocity_x++;
		if(state->newly_pressed & J_UP) state->velocity_y--;
		if(state->newly_pressed & J_DOWN) state->velocity_y++;
	}else if(!(state->flags & FLG_MOTION)){
		if(state->buttons & J_LEFT) center_x--;
		if(state->buttons & J_RIGHT) center_x++;
		if(state->buttons & J_UP) center_y--;
		if(state->buttons & J_DOWN) center_y++;

		/* If any of the new coordinates are invalid, roll back
		 *
		 * We don't call move_square() directly because it allows the
		 * square to continue moving along a side when two arrow
		 * buttons are pressed at the same time */
		if(!SQUARE_X_VALID(center_x))
			center_x=state->square_center_x;
		if(!SQUARE_Y_VALID(center_y))
			center_y=state->square_center_y;
	}else{
		/* Update position using the velocity */
		center_x+=state->velocity_x;
		center_y+=state->velocity_y;
	}

	if(SQUARE_X_VALID(center_x) && SQUARE_Y_VALID(center_y)
		&& !move_square((uint8_t)center_x,(uint8_t)center_y)){
		state->square_center_x=center_x;
		state->square_center_y=center_y;
	}else{
		/* Bounce it if hitting a wall
		 *
		 * We only reflect the velocity here. The position update will
		 * be done in the next call. */
		if(!SQUARE_X_VALID(center_x))
			state->velocity_x=-state->velocity_x;
		if(!SQUARE_Y_VALID(center_y))
			state->velocity_y=-state->velocity_y;
	}
}
