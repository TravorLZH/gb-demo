#include <gb/gb.h>
#include <gb/cgb.h>
#include "game.h"

/* Tile 0 has no set pixels, so every pixel uses background palette color 0. */
const uint8_t blank_tile[16]={0};

/* The four colors visited when A is pressed. RGB components range from
 * 0x00 to 0x1f. */
const palette_color_t colors[4]={
	RGB(0x00,0x00,0x00), /* black */
	RGB(0x1f,0x00,0x00), /* red */
	RGB(0x1f,0x1f,0x00), /* yellow */
	RGB(0x00,0x00,0x1f), /* blue */
};

static void handle_buttons(struct game_state *state)
{
	uint8_t buttons=joypad();
	uint8_t old_color_index=state->color_index;

	/* A and B act once per press; held directions move every frame. */
	state->newly_pressed=buttons & ~state->buttons;
	state->buttons=buttons;

	if(state->newly_pressed & J_B){
		state->color_index=0;
	}else if(state->newly_pressed & J_A){
		state->color_index=(state->color_index + 1) & 3;
	}
	if(state->color_index!=old_color_index){
		set_bkg_palette_entry(0,0,colors[state->color_index]);
		set_sprite_palette_entry(0,3,
			colors[state->color_index] ^ 0x7fffU);
	}

	/* The START button recenters the square. */
	if(state->newly_pressed & J_START){
		state->square_center_x=SCREENWIDTH / 2;
		state->square_center_y=SCREENHEIGHT / 2;
	}

	update_square(state);
}

void main(void)
{
	struct game_state state={
		0,0,0,0,SCREENWIDTH / 2,SCREENHEIGHT / 2,0,0
	};
	/* Fill the screen with one background color and make a 16x16
	 * sprite square. */
	set_bkg_data(0,1,blank_tile);
	fill_bkg_rect(0,0,20,18,0);
	set_bkg_palette_entry(0,0,colors[state.color_index]);
	/* XOR flips each of the 15 RGB bits, so the square inverts the
	 * background. */
	set_sprite_palette_entry(0,3,colors[state.color_index] ^ 0x7fffU);
	init_square(state.square_center_x,state.square_center_y);
	SHOW_BKG;
	SHOW_SPRITES;
	DISPLAY_ON;

	while(1){
		vsync();
		handle_buttons(&state);
	}
}
