/*
 * tank.c
 *
 *  Created on: Sep 21, 2016
 *      Author: superman
 */


#include <stdint.h>
#include "platform.h"
#include "xparameters.h"
#include "xaxivdma.h"
#include "xio.h"
#include "time.h"
#include "unistd.h"

#include "tank.h"	// Do we normally have to include our own h function?
#define TANK_HEIGHT 8
#define GREEN 0x0000FF00
#define BLACK 0x00000000
#define WHITE 0xFFFFFFFF

// Packs each horizontal line of the figures into a single 32 bit word.
#define packword15(b14,b13,b12,b11,b10,b9,b8,b7,b6,b5,b4,b3,b2,b1,b0)  \
((b14 << 14) | (b13 << 13) | (b12 << 12) | (b11 << 11) | (b10 << 10) | \
 (b9  << 9 ) | (b8  << 8 ) | (b7  << 7 ) | (b6  << 6 ) | (b5  << 5 ) | \
 (b4  << 4 ) | (b3  << 3 ) | (b2  << 2 ) | (b1  << 1 ) | (b0  << 0 ) )

 // This seems like a *very bad* way to store the tank data, but this is what
 // we are doing for the moment.
static const int tank_15x8[TANK_HEIGHT] =
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

#define WORD_WIDTH 15
struct tank{
	int row;
	int col;
}tank;

struct tank_shell{
	int row;
	int col;
	bool alive;
}tank_shell;


void init_tank(){
	tank.row = 210;
	tank.col = 160;
}

// This draws (or erases, via the erase bool) an entire tank.
void draw_tank(unsigned int * framePointer, bool erase){
	// Color is either green or black depending on whether erase happens.
	int color = erase ? BLACK : GREEN ;
	int row, col;
	for(row=0;row<TANK_HEIGHT;row++){
		for(col=0;col<WORD_WIDTH;col++){
			if ((tank_15x8[row] & (1<<(WORD_WIDTH-col-1)))) {
				framePointer[(row+tank.row)*640*2 + (col + tank.col)*2] = color;
				framePointer[(row+tank.row)*640*2 + (col + tank.col)*2 + 1] = color;

				framePointer[(row+tank.row)*640*2 + 640 + (col + tank.col)*2] = color;
				framePointer[(row+tank.row)*640*2 + 640 + (col + tank.col)*2+1] = color;
			}
		}
	}
}


// moves our tank left by a certain number of pixels
void move_left(unsigned int * framePointer){
	tank.col -= 1;
	int row;
	for(row = 0; row < TANK_HEIGHT; row++){
		switch (row){
		case 0:
			framePointer[(row+tank.row)*640*2 + 640 + (7 + tank.col)*2] = GREEN;
			framePointer[(row+tank.row)*640*2 + 640 + (8 + tank.col)*2] = BLACK;
			framePointer[(row+tank.row)*640*2 + 640 + (7 + tank.col)*2+1] = GREEN;
			framePointer[(row+tank.row)*640*2 + 640 + (8 + tank.col)*2+1] = BLACK;

			framePointer[(row+tank.row)*640*2 + (7 + tank.col)*2] = GREEN;
			framePointer[(row+tank.row)*640*2 + (8 + tank.col)*2] = BLACK;
			framePointer[(row+tank.row)*640*2 + (7 + tank.col)*2+1] = GREEN;
			framePointer[(row+tank.row)*640*2 + (8 + tank.col)*2+1] = BLACK;
			break;
		case 1:
			framePointer[(row+tank.row)*640*2 + 640 + (6 + tank.col)*2] = GREEN;
			framePointer[(row+tank.row)*640*2 + 640 + (9 + tank.col)*2] = BLACK;
			framePointer[(row+tank.row)*640*2 + 640 + (6 + tank.col)*2+1] = GREEN;
			framePointer[(row+tank.row)*640*2 + 640 + (9 + tank.col)*2+1] = BLACK;

			framePointer[(row+tank.row)*640*2 + (6 + tank.col)*2] = GREEN;
			framePointer[(row+tank.row)*640*2 + (9 + tank.col)*2] = BLACK;
			framePointer[(row+tank.row)*640*2 + (6 + tank.col)*2+1] = GREEN;
			framePointer[(row+tank.row)*640*2 + (9 + tank.col)*2+1] = BLACK;

			break;
		case 2:
			framePointer[(row+tank.row)*640*2 + 640 + (6 + tank.col)*2] = GREEN;
			framePointer[(row+tank.row)*640*2 + 640 + (9 + tank.col)*2] = BLACK;
			framePointer[(row+tank.row)*640*2 + 640 + (6 + tank.col)*2+1] = GREEN;
			framePointer[(row+tank.row)*640*2 + 640 + (9 + tank.col)*2+1] = BLACK;

			framePointer[(row+tank.row)*640*2 + (6 + tank.col)*2] = GREEN;
			framePointer[(row+tank.row)*640*2 + (9 + tank.col)*2] = BLACK;
			framePointer[(row+tank.row)*640*2 + (6 + tank.col)*2+1] = GREEN;
			framePointer[(row+tank.row)*640*2 + (9 + tank.col)*2+1] = BLACK;

			break;
		case 3:
			framePointer[(row+tank.row)*640*2 + 640 + (1 + tank.col)*2] = GREEN;
			framePointer[(row+tank.row)*640*2 + 640 + (14 + tank.col)*2] = BLACK;
			framePointer[(row+tank.row)*640*2 + 640 + (1 + tank.col)*2+1] = GREEN;
			framePointer[(row+tank.row)*640*2 + 640 + (14 + tank.col)*2+1] = BLACK;

			framePointer[(row+tank.row)*640*2 + (1 + tank.col)*2] = GREEN;
			framePointer[(row+tank.row)*640*2 + (14 + tank.col)*2] = BLACK;
			framePointer[(row+tank.row)*640*2 + (1 + tank.col)*2+1] = GREEN;
			framePointer[(row+tank.row)*640*2 + (14 + tank.col)*2+1] = BLACK;

			break;
		case 4:
			framePointer[(row+tank.row)*640*2 + 640 + (0 + tank.col)*2] = GREEN;
			framePointer[(row+tank.row)*640*2 + 640 + (15 + tank.col)*2] = BLACK;
			framePointer[(row+tank.row)*640*2 + 640 + (0 + tank.col)*2+1] = GREEN;
			framePointer[(row+tank.row)*640*2 + 640 + (15 + tank.col)*2+1] = BLACK;

			framePointer[(row+tank.row)*640*2 + (0 + tank.col)*2] = GREEN;
			framePointer[(row+tank.row)*640*2 + (15 + tank.col)*2] = BLACK;
			framePointer[(row+tank.row)*640*2 + (0 + tank.col)*2+1] = GREEN;
			framePointer[(row+tank.row)*640*2 + (15 + tank.col)*2+1] = BLACK;

			break;
		case 5:
			framePointer[(row+tank.row)*640*2 + 640 + (0 + tank.col)*2] = GREEN;
			framePointer[(row+tank.row)*640*2 + 640 + (15 + tank.col)*2] = BLACK;
			framePointer[(row+tank.row)*640*2 + 640 + (0 + tank.col)*2+1] = GREEN;
			framePointer[(row+tank.row)*640*2 + 640 + (15 + tank.col)*2+1] = BLACK;

			framePointer[(row+tank.row)*640*2 + (0 + tank.col)*2] = GREEN;
			framePointer[(row+tank.row)*640*2 + (15 + tank.col)*2] = BLACK;
			framePointer[(row+tank.row)*640*2 + (0 + tank.col)*2+1] = GREEN;
			framePointer[(row+tank.row)*640*2 + (15 + tank.col)*2+1] = BLACK;

			break;
		case 6:
			framePointer[(row+tank.row)*640*2 + 640 + (0 + tank.col)*2] = GREEN;
			framePointer[(row+tank.row)*640*2 + 640 + (15 + tank.col)*2] = BLACK;
			framePointer[(row+tank.row)*640*2 + 640 + (0 + tank.col)*2+1] = GREEN;
			framePointer[(row+tank.row)*640*2 + 640 + (15 + tank.col)*2+1] = BLACK;

			framePointer[(row+tank.row)*640*2 + (0 + tank.col)*2] = GREEN;
			framePointer[(row+tank.row)*640*2 + (15 + tank.col)*2] = BLACK;
			framePointer[(row+tank.row)*640*2 + (0 + tank.col)*2+1] = GREEN;
			framePointer[(row+tank.row)*640*2 + (15 + tank.col)*2+1] = BLACK;

			break;
		case 7:
			framePointer[(row+tank.row)*640*2 + 640 + (0 + tank.col)*2] = GREEN;
			framePointer[(row+tank.row)*640*2 + 640 + (15 + tank.col)*2] = BLACK;
			framePointer[(row+tank.row)*640*2 + 640 + (0 + tank.col)*2+1] = GREEN;
			framePointer[(row+tank.row)*640*2 + 640 + (15 + tank.col)*2+1] = BLACK;

			framePointer[(row+tank.row)*640*2 + (0 + tank.col)*2] = GREEN;
			framePointer[(row+tank.row)*640*2 + (15 + tank.col)*2] = BLACK;
			framePointer[(row+tank.row)*640*2 + (0 + tank.col)*2+1] = GREEN;
			framePointer[(row+tank.row)*640*2 + (15 + tank.col)*2+1] = BLACK;

			break;
		}
	}
}

//moves our tank right by a certain number of pixels
void move_right(unsigned int * framePointer){

		tank.col += 1;

		framePointer[(0+tank.row)*640*2 + 640 + (7 + tank.col)*2] = GREEN;
		framePointer[(0+tank.row)*640*2 + 640 + (6 + tank.col)*2] = BLACK;
		framePointer[(0+tank.row)*640*2 + 640 + (7 + tank.col)*2+1] = GREEN;
		framePointer[(0+tank.row)*640*2 + 640 + (6 + tank.col)*2+1] = BLACK;

		framePointer[(0+tank.row)*640*2 + (7 + tank.col)*2] = GREEN;
		framePointer[(0+tank.row)*640*2 + (6 + tank.col)*2] = BLACK;
		framePointer[(0+tank.row)*640*2 + (7 + tank.col)*2+1] = GREEN;
		framePointer[(0+tank.row)*640*2 + (6 + tank.col)*2+1] = BLACK;

		framePointer[(1+tank.row)*640*2 + 640 + (8 + tank.col)*2] = GREEN;
		framePointer[(1+tank.row)*640*2 + 640 + (5 + tank.col)*2] = BLACK;
		framePointer[(1+tank.row)*640*2 + 640 + (8 + tank.col)*2+1] = GREEN;
		framePointer[(1+tank.row)*640*2 + 640 + (5 + tank.col)*2+1] = BLACK;

		framePointer[(1+tank.row)*640*2 + (8 + tank.col)*2] = GREEN;
		framePointer[(1+tank.row)*640*2 + (5 + tank.col)*2] = BLACK;
		framePointer[(1+tank.row)*640*2 + (8 + tank.col)*2+1] = GREEN;
		framePointer[(1+tank.row)*640*2 + (5 + tank.col)*2+1] = BLACK;


		framePointer[(2+tank.row)*640*2 + 640 + (8 + tank.col)*2] = GREEN;
		framePointer[(2+tank.row)*640*2 + 640 + (5 + tank.col)*2] = BLACK;
		framePointer[(2+tank.row)*640*2 + 640 + (8 + tank.col)*2+1] = GREEN;
		framePointer[(2+tank.row)*640*2 + 640 + (5 + tank.col)*2+1] = BLACK;

		framePointer[(2+tank.row)*640*2 + (8 + tank.col)*2] = GREEN;
		framePointer[(2+tank.row)*640*2 + (5 + tank.col)*2] = BLACK;
		framePointer[(2+tank.row)*640*2 + (8 + tank.col)*2+1] = GREEN;
		framePointer[(2+tank.row)*640*2 + (5 + tank.col)*2+1] = BLACK;


		framePointer[(3+tank.row)*640*2 + 640 + (13 + tank.col)*2] = GREEN;
		framePointer[(3+tank.row)*640*2 + 640 + (0 + tank.col)*2] = BLACK;
		framePointer[(3+tank.row)*640*2 + 640 + (13 + tank.col)*2+1] = GREEN;
		framePointer[(3+tank.row)*640*2 + 640 + (0 + tank.col)*2+1] = BLACK;

		framePointer[(3+tank.row)*640*2 + (13 + tank.col)*2] = GREEN;
		framePointer[(3+tank.row)*640*2 + (0 + tank.col)*2] = BLACK;
		framePointer[(3+tank.row)*640*2 + (13 + tank.col)*2+1] = GREEN;
		framePointer[(3+tank.row)*640*2 + (0 + tank.col)*2+1] = BLACK;


		framePointer[(4+tank.row)*640*2 + 640 + (14 + tank.col)*2] = GREEN;
		framePointer[(4+tank.row)*640*2 + 640 + (-1 + tank.col)*2] = BLACK;
		framePointer[(4+tank.row)*640*2 + 640 + (14 + tank.col)*2+1] = GREEN;
		framePointer[(4+tank.row)*640*2 + 640 + (-1 + tank.col)*2+1] = BLACK;

		framePointer[(4+tank.row)*640*2 + (14 + tank.col)*2] = GREEN;
		framePointer[(4+tank.row)*640*2 + (-1 + tank.col)*2] = BLACK;
		framePointer[(4+tank.row)*640*2 + (14 + tank.col)*2+1] = GREEN;
		framePointer[(4+tank.row)*640*2 + (-1 + tank.col)*2+1] = BLACK;


		framePointer[(5+tank.row)*640*2 + 640 + (14 + tank.col)*2] = GREEN;
		framePointer[(5+tank.row)*640*2 + 640 + (-1 + tank.col)*2] = BLACK;
		framePointer[(5+tank.row)*640*2 + 640 + (14 + tank.col)*2+1] = GREEN;
		framePointer[(5+tank.row)*640*2 + 640 + (-1 + tank.col)*2+1] = BLACK;

		framePointer[(5+tank.row)*640*2 + (14 + tank.col)*2] = GREEN;
		framePointer[(5+tank.row)*640*2 + (-1 + tank.col)*2] = BLACK;
		framePointer[(5+tank.row)*640*2 + (14 + tank.col)*2+1] = GREEN;
		framePointer[(5+tank.row)*640*2 + (-1 + tank.col)*2+1] = BLACK;


		framePointer[(6+tank.row)*640*2 + 640 + (14 + tank.col)*2] = GREEN;
		framePointer[(6+tank.row)*640*2 + 640 + (-1 + tank.col)*2] = BLACK;
		framePointer[(6+tank.row)*640*2 + 640 + (14 + tank.col)*2+1] = GREEN;
		framePointer[(6+tank.row)*640*2 + 640 + (-1 + tank.col)*2+1] = BLACK;

		framePointer[(6+tank.row)*640*2 + (14 + tank.col)*2] = GREEN;
		framePointer[(6+tank.row)*640*2 + (-1 + tank.col)*2] = BLACK;
		framePointer[(6+tank.row)*640*2 + (14 + tank.col)*2+1] = GREEN;
		framePointer[(6+tank.row)*640*2 + (-1 + tank.col)*2+1] = BLACK;


		framePointer[(7+tank.row)*640*2 + 640 + (14 + tank.col)*2] = GREEN;
		framePointer[(7+tank.row)*640*2 + 640 + (-1 + tank.col)*2] = BLACK;
		framePointer[(7+tank.row)*640*2 + 640 + (14 + tank.col)*2+1] = GREEN;
		framePointer[(7+tank.row)*640*2 + 640 + (-1 + tank.col)*2+1] = BLACK;

		framePointer[(7+tank.row)*640*2 + (14 + tank.col)*2] = GREEN;
		framePointer[(7+tank.row)*640*2 + (-1 + tank.col)*2] = BLACK;
		framePointer[(7+tank.row)*640*2 + (14 + tank.col)*2+1] = GREEN;
		framePointer[(7+tank.row)*640*2 + (-1 + tank.col)*2+1] = BLACK;
}

void fire_tank(unsigned int * framePointer){
	if(!tank_shell.alive){
		tank_shell.col = tank.col;
		tank_shell.row = tank.row;
		tank_shell.alive = true;
		framePointer[(tank_shell.row-1)*1280 + (7 + tank_shell.col)*2] = WHITE;
		framePointer[(tank_shell.row-2)*1280 + (7 + tank_shell.col)*2] = WHITE;
		framePointer[(tank_shell.row-3)*1280 + (7 + tank_shell.col)*2] = WHITE;

		framePointer[(tank_shell.row-1)*1280 + (7 + tank_shell.col)*2+1] = WHITE;
		framePointer[(tank_shell.row-2)*1280 + (7 + tank_shell.col)*2+1] = WHITE;
		framePointer[(tank_shell.row-3)*1280 + (7 + tank_shell.col)*2+1] = WHITE;

		framePointer[(tank_shell.row-1)*1280 + 640 + (7 + tank_shell.col)*2+1] = WHITE;
		framePointer[(tank_shell.row-2)*1280 + 640 + (7 + tank_shell.col)*2+1] = WHITE;
		framePointer[(tank_shell.row-3)*1280 + 640 + (7 + tank_shell.col)*2+1] = WHITE;

		framePointer[(tank_shell.row-1)*1280 + 640 + (7 + tank_shell.col)*2] = WHITE;
		framePointer[(tank_shell.row-2)*1280 + 640 + (7 + tank_shell.col)*2] = WHITE;
		framePointer[(tank_shell.row-3)*1280 + 640 + (7 + tank_shell.col)*2] = WHITE;
	}
}

void update_shell(unsigned int * framePointer){
	if(tank_shell.row<0){
		tank_shell.alive = false;
	}
	else if(tank_shell.alive){
		tank_shell.row -= 1;

		framePointer[(tank_shell.row-3)*1280 + (7 + tank_shell.col)*2] = WHITE;
		framePointer[(tank_shell.row)*1280 + (7 + tank_shell.col)*2] = BLACK;

		framePointer[(tank_shell.row-3)*1280 + (7 + tank_shell.col)*2+1] = WHITE;
		framePointer[(tank_shell.row)*1280 + (7 + tank_shell.col)*2+1] = BLACK;

		framePointer[(tank_shell.row-3)*1280 + 640 + (7 + tank_shell.col)*2+1] = WHITE;
		framePointer[(tank_shell.row)*1280 + 640 + (7 + tank_shell.col)*2+1] = BLACK;

		framePointer[(tank_shell.row-3)*1280 + 640 + (7 + tank_shell.col)*2] = WHITE;
		framePointer[(tank_shell.row)*1280 + 640 + (7 + tank_shell.col)*2] = BLACK;
	}
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

