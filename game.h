#ifndef GAME_H
#define GAME_H

#include <stdint.h>

/* State shared by input handling and square movement. */
struct game_state {
	/* Index into the four background colors. */
	uint8_t color_index;
	/* Reserved; not used by the current game loop. */
	uint8_t flags;
	/* Keys held now and keys first pressed this frame. */
	uint8_t buttons;
	uint8_t newly_pressed;
	/* Center of the square in screen pixels. */
	uint8_t square_center_x;
	uint8_t square_center_y;
	/* Reserved; not used by the current game loop. */
	int8_t velocity_x;
	int8_t velocity_y;
};

/* Load the square sprites and place them at the given center. */
extern void init_square(uint8_t center_x,uint8_t center_y);
/* Move from held buttons; commit the new center only when it is valid. */
extern void update_square(struct game_state *state);

#endif
