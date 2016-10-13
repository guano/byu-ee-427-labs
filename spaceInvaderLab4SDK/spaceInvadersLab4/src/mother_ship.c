/*
 * mother_ship.c
 *
 *  Created on: Oct 12, 2016
 *      Author: superman
 */

#include "mother_ship.h"
#include "interface.h" // enables update score
#include "util.h"

#define packword16(b15,b14,b13,b12,b11,b10,b9,b8,b7,b6,b5,b4,b3,b2,b1,b0)  \
		((b15<<15)|(b14<<14)|(b13 << 13) | (b12 << 12) | (b11 << 11) | (b10 << 10) | \
				(b9  << 9 ) | (b8  << 8 ) | (b7  << 7 ) | (b6  << 6 ) | (b5  << 5 ) | \
				(b4  << 4 ) | (b3  << 3 ) | (b2  << 2 ) | (b1  << 1 ) | (b0  << 0 ) )
static const uint32_t MOTHER_SHIP_16x7[] ={
		packword16(0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0),
		packword16(0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0),
		packword16(0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0),
		packword16(0,1,1,0,1,1,0,1,1,0,1,1,0,1,1,0),
		packword16(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1),
		packword16(0,0,1,1,1,0,0,1,1,0,0,1,1,1,0,0),
		packword16(0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0)};

#define MOTHER_SHIP_ROW 22	// Where the mother ship spawns at
#define MOTHER_SHIP_HEIGHT 7
#define MOTHER_SHIP_WIDTH 16
#define MOTHER_SHIP_MOVE_SPEED 2
#define SCREEN_WIDTH 320
#define SHOCKING_PINK 0xFF6FFF
#define MOTHER_SHIP_COLOR SHOCKING_PINK
#define BLACK 0x0
#define BLINKING_TIMES 10


struct{
	uint32_t row;
	uint32_t col;
	bool alive;
}mother_ship;

uint32_t * frame;

bool blinking;

// ----------------------------------

// ----------------------------------




// Initializes the mother ship
void mother_ship_init(uint32_t * framePointer){
	blinking = false;
	frame = framePointer;
	mother_ship.row = MOTHER_SHIP_ROW;
	mother_ship.alive = false;
	mother_ship.col = 0;
}


uint32_t mother_ship_points = 123;
// Blinks the points
void mother_ship_points_blink(){
	if(!blinking){
		return;
	}

	static uint32_t times_blink = 0;
	times_blink++;
	bool blink_on = times_blink %2 == 0 ? true : false ;

	//interface_alien_ship_points(mother_ship_points, mother_ship.col, blink_on);
	if(times_blink > BLINKING_TIMES){
		times_blink = 0;
		interface_alien_ship_points(mother_ship_points, mother_ship.col, true);
		blinking = false;
	}

}

// Spawns a mother ship
void mother_ship_spawn(){
	if(mother_ship.alive){
		return;
	}
	interface_alien_ship_points(mother_ship_points, mother_ship.col, true);

	mother_ship.col = 0;
	mother_ship.alive = true;
	mother_ship_draw(MOTHER_SHIP_COLOR);
	blinking = false;
}

// Moves the mother ship right
void mother_ship_move(){
	if(!mother_ship.alive){
		return;
	}
	mother_ship_draw(BLACK);
	mother_ship.col += MOTHER_SHIP_MOVE_SPEED;
	if(mother_ship.col > SCREEN_WIDTH-MOTHER_SHIP_WIDTH){
		mother_ship.alive = false;
		mother_ship.col = SCREEN_WIDTH;
		return;
	}
	mother_ship_draw(MOTHER_SHIP_COLOR);
}

// Detects a bullet collision on the mother ship
bool mother_ship_detect_collision(uint32_t row, uint32_t col){
	if(row == mother_ship.row+MOTHER_SHIP_HEIGHT && col > mother_ship.col && col < mother_ship.col+MOTHER_SHIP_WIDTH){
		mother_ship_points = rand()%500 + 316;
		interface_increment_score(mother_ship_points);
		mother_ship.alive = false;
		mother_ship_draw(BLACK);
		interface_alien_ship_points(mother_ship_points, mother_ship.col, false);
		blinking = true;
		return true;
	}
	return false;
}

// Draws the mother ship
void mother_ship_draw(uint32_t color){
	int r, c;
	for(r=0;r<MOTHER_SHIP_HEIGHT;r++){				// Go through width
		for(c=0;c<MOTHER_SHIP_WIDTH;c++){			// and height
			if((MOTHER_SHIP_16x7[r] & (1<<(MOTHER_SHIP_WIDTH-c-1)))){// and draw ship
				util_draw_pixel(frame, r+mother_ship.row, c+mother_ship.col, color);
			}
		}
	}
}












