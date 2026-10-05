#ifndef GAME_H
#define GAME_H

#include <stdint.h>

/* State shared by input handling and square movement */
struct game_state {
	/* Index into the four background colors */
	uint8_t color_index;
	/* Other configurations */
	uint8_t flags;
	/* Keys held now and keys first pressed this frame */
	uint8_t buttons;
	uint8_t newly_pressed;
	/* Center of the square in screen pixels */
	uint8_t square_center_x;
	uint8_t square_center_y;
	/* Velocity; will be updated when the square hits a wall */
	int8_t velocity_x;
	int8_t velocity_y;
};

#define FLG_VELOCITY	0x1U	/* Whether D-pad adjusts velocity */
#define FLG_MOTION	0x2U	/* Whether the square is moving on its own */

/* Load the square sprites and place them at the given center. */
extern void init_square(uint8_t center_x,uint8_t center_y);
/* Updates square location based on game state */
extern void update_square(struct game_state *state);

#endif
