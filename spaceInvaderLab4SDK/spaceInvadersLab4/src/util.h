/*
 * utilities.h
 *
 *	Taylor Cowley and Andrew Okazaki
 *
 *	This is a collection of functions used by several things.
 */

#ifndef UTIL_H_
#define UTIL_H_

#include <stdint.h>


#define UTIL_SCREEN_WIDTH	320	// Our game screen is 320
#define UTIL_SCREEN_HEIGHT	240	// by 240

// Draws a pixel on the screen.
void util_draw_pixel(uint32_t *framePointer,uint32_t row,uint32_t col,uint32_t color);

#endif /* UTILITIES_H_ */
