/*
 * utilities.c
 *
 *  Taylor Cowley and Andrew Okazaki
 */


#include "util.h"
#define ROW_MULTIPLIER 1280	// 640 * 2 for screen doubling
#define ROW 640				// one row offset
#define COL_MULTIPLIER 2	// Offset of the row



/*
 * Draws a pixel on the screen. To compensate for our double-resolution screen,
 * it must draw 4 real pixels for every in-came pixel.
 */
void util_draw_pixel(uint32_t *frame, uint32_t row, uint32_t col,uint32_t color) {
	// We draw 4 pixels for every 1 small-screen pixel
	frame[row * ROW_MULTIPLIER + col * COL_MULTIPLIER] 				= color;
	frame[row * ROW_MULTIPLIER + col * COL_MULTIPLIER + 1] 			= color;
	frame[row * ROW_MULTIPLIER + ROW + col * COL_MULTIPLIER] 		= color;
	frame[row * ROW_MULTIPLIER + ROW + col * COL_MULTIPLIER + 1] 	= color;
}
