/*
 * tank.c
 *
 *  Created on: Sep 21, 2016
 *      Author: superman
 */


#include <stdint.h>

// Packs each horizontal line of the figures into a single 32 bit word.
#define packword15(b14,b13,b12,b11,b10,b9,b8,b7,b6,b5,b4,b3,b2,b1,b0)  \
((b14 << 14) | (b13 << 13) | (b12 << 12) | (b11 << 11) | (b10 << 10) | \
 (b9  << 9 ) | (b8  << 8 ) | (b7  << 7 ) | (b6  << 6 ) | (b5  << 5 ) | \
 (b4  << 4 ) | (b3  << 3 ) | (b2  << 2 ) | (b1  << 1 ) | (b0  << 0 ) )

 // This seems like a *very bad* way to store the tank data, but this is what
 // we are doing for the moment.
static const int tank_15x8[] =
{
packword15(0,0,0,0,0,0,0,1,0,0,0,0,0,0,0),
packword15(0,0,0,0,0,0,1,1,1,0,0,0,0,0,0),
packword15(0,0,0,0,0,0,1,1,1,0,0,0,0,0,0),
packword15(0,1,1,1,1,1,1,1,1,1,1,1,1,1,0),
packword15(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1),
packword15(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1),
packword15(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1),
packword15(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1)
};

// This is where the tank is on the screen right now.
// in 320 x 240 syntax. We may change this later
// we might make these global variables too.
uint16_t x_pos = 150;	// Start in the middle of the screen
uint16_t y_pos = 220;	// The tank always stays near the bottom of the screen

// moves our tank left by a certain number of pixels
void move_left(uint16_t pixels){
	// TODO: TANK CANNOT MOVE OFF SCREEN
	x_pos -= pixels;
}

// moves our tank right by a certain number of pixels
void move_right(uint16_t pixels){
	// TODO: TANK CANNOT MOVE OFF SCREEN
	x_pos += pixels;
}


/*
 * Here are my thoughts.
 * If any move function (or explode funtion)
 * is called on the tank, it knows it has changed.
 * Then when the renderer goes to render the tank,
 * it understands that instead of not drawing anything
 * (or drawing the entire tank, as it does at the beginning)
 * it knows to draw "updated tank," which consists of a black rectangle
 * to delete old tank data, and color rectangles to make new tank data.
 */

