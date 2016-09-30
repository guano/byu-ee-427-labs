/*
 * aliens.c
 *
 *  Created on: Sep 21, 2016
 *      Author: superman
 */

#include <stdio.h>
#include "platform.h"
#include "xparameters.h"
#include "xaxivdma.h"
#include "xio.h"
#include "time.h"
#include "unistd.h"
#include <stdbool.h>
#include <stdint.h>
#define ALIEN_HEIGHT 8
#define TOP_TOTAL 11
#define ALIEN_COLUMNS 11
#define LOC_ALIEN_ONE 50
#define MIDDLE_TOTAL 22
#define ALIEN_NUM_BULLETS 4

#define SCREEN_HEIGHT 320

#define WHITE 0xFFFFFFF
#define BLACK 0x0000000
#define BLUE  0xFFFFFFF
#define R  0xFFFF00F
// Packs each horizontal line of the figures into a single 32 bit word.
#define packword12(b11,b10,b9,b8,b7,b6,b5,b4,b3,b2,b1,b0) \
		((b11 << 11) | (b10 << 10) | (b9  << 9 ) | (b8  << 8 ) | (b7  << 7 ) | (b6  << 6 ) \
				| (b5  << 5 ) | (b4  << 4 ) | (b3  << 3 ) | (b2  << 2 ) | (b1  << 1 ) | (b0  << 0 ) )
void build_tops(unsigned int * framePointer, int alien_top[]);
void build_middle(unsigned int * framePointer, int alien_middle[]);
void build_bottom(unsigned int * framePointer, int alien_bottom[]);
int32_t fire_bottom(unsigned int * framePointer, int r);
int32_t fire_middle(unsigned int * framePointer, int r);
int32_t fire_top(unsigned int * framePointer, int r);
bool can_aliens_shoot();
void draw_bullet(unsigned int * framePointer, int32_t bullet, uint32_t color);


static const int alien_top_in_12x8[ALIEN_HEIGHT] =
{
		packword12(0,0,0,0,0,1,1,0,0,0,0,0),
		packword12(0,0,0,0,1,1,1,1,0,0,0,0),
		packword12(0,0,0,1,1,1,1,1,1,0,0,0),
		packword12(0,0,1,1,0,1,1,0,1,1,0,0),
		packword12(0,0,1,1,1,1,1,1,1,1,0,0),
		packword12(0,0,0,1,0,1,1,0,1,0,0,0),
		packword12(0,0,1,0,0,0,0,0,0,1,0,0),
		packword12(0,0,0,1,0,0,0,0,1,0,0,0)
};
#define WORD_WIDTH 12
static const int alien_top_out_12x8[ALIEN_HEIGHT] =
{
		packword12(0,0,0,0,0,1,1,0,0,0,0,0),
		packword12(0,0,0,0,1,1,1,1,0,0,0,0),
		packword12(0,0,0,1,1,1,1,1,1,0,0,0),
		packword12(0,0,1,1,0,1,1,0,1,1,0,0),
		packword12(0,0,1,1,1,1,1,1,1,1,0,0),
		packword12(0,0,0,0,1,0,0,1,0,0,0,0),
		packword12(0,0,0,1,0,1,1,0,1,0,0,0),
		packword12(0,0,1,0,1,0,0,1,0,1,0,0)
};

static const int alien_middle_in_12x8[ALIEN_HEIGHT] =
{
		packword12(0,0,0,1,0,0,0,0,0,1,0,0),
		packword12(0,0,0,0,1,0,0,0,1,0,0,0),
		packword12(0,0,0,1,1,1,1,1,1,1,0,0),
		packword12(0,0,1,1,0,1,1,1,0,1,1,0),
		packword12(0,1,1,1,1,1,1,1,1,1,1,1),
		packword12(0,1,1,1,1,1,1,1,1,1,1,1),
		packword12(0,1,0,1,0,0,0,0,0,1,0,1),
		packword12(0,0,0,0,1,1,0,1,1,0,0,0)
};
static const int alien_middle_out_12x8[] =
{
		packword12(0,0,0,1,0,0,0,0,0,1,0,0),
		packword12(0,1,0,0,1,0,0,0,1,0,0,1),
		packword12(0,1,0,1,1,1,1,1,1,1,0,1),
		packword12(0,1,1,1,0,1,1,1,0,1,1,1),
		packword12(0,1,1,1,1,1,1,1,1,1,1,1),
		packword12(0,0,1,1,1,1,1,1,1,1,1,0),
		packword12(0,0,0,1,0,0,0,0,0,1,0,0),
		packword12(0,0,1,0,0,0,0,0,0,0,1,0)
};
static const int alien_bottom_in_12x8[ALIEN_HEIGHT] =
{
		packword12(0,0,0,0,1,1,1,1,0,0,0,0),
		packword12(0,1,1,1,1,1,1,1,1,1,1,0),
		packword12(1,1,1,1,1,1,1,1,1,1,1,1),
		packword12(1,1,1,0,0,1,1,0,0,1,1,1),
		packword12(1,1,1,1,1,1,1,1,1,1,1,1),
		packword12(0,0,1,1,1,0,0,1,1,1,0,0),
		packword12(0,1,1,0,0,1,1,0,0,1,1,0),
		packword12(0,0,1,1,0,0,0,0,1,1,0,0)
};
static const int alien_bottom_out_12x8[] =
{
		packword12(0,0,0,0,1,1,1,1,0,0,0,0),
		packword12(0,1,1,1,1,1,1,1,1,1,1,0),
		packword12(1,1,1,1,1,1,1,1,1,1,1,1),
		packword12(1,1,1,0,0,1,1,0,0,1,1,1),
		packword12(1,1,1,1,1,1,1,1,1,1,1,1),
		packword12(0,0,0,1,1,0,0,1,1,0,0,0),
		packword12(0,0,1,1,0,1,1,0,1,1,0,0),
		packword12(1,1,0,0,0,0,0,0,0,0,1,1)
};

struct top{
	int row;
	int col;
	int alive;
}top[TOP_TOTAL];

struct middleAlien{
	int row;
	int col;
	int alive;
}middleAlien[MIDDLE_TOTAL];

struct bottomAlien{
	int row;
	int col;
	int alive;
}bottomAlien[MIDDLE_TOTAL];

// aliens can have two types of bullet: cross and lightning
// cross 0 and 3 are identical
typedef enum {cross0, cross1, cross2, cross3, lightning0, lightning1} bullet_type;

struct alien_bullet{
	int row;
	int col;
	bool alive;
	bullet_type bullet_type;

}alien_bullet[ALIEN_NUM_BULLETS];


int alien_count;

//initialize all of the aliens by setting values contained in struct's and printing aliens to the screen
void alien_init(unsigned int * framePointer){

	//local variables, loc is the starting location of alien one on the screen
	int i, loc = LOC_ALIEN_ONE;

	//loops through one row of aliens
	for( i=0; i < ALIEN_COLUMNS ; i++){
		top[i].row = 30; //set the row of alien tops to 30
		top[i].col = loc;//sets the column of alien tops
		top[i].alive = 1;//sets the alien is alive flag

		middleAlien[i].row = 45; //sets the first row of the middle aliens
		middleAlien[i].col = loc;//sets the column of first row of middle aliens
		middleAlien[i].alive = 1;//sets the first row of middle aliens to alive state
		middleAlien[i+11].row = 60;//sets the second row of the middle aliens
		middleAlien[i+11].col = loc;//sets the column of second row of middle aliens
		middleAlien[i+11].alive = 1;//sets the second row of middle aliens to alive state

		bottomAlien[i].row = 75;//sets the first row of the bottom aliens
		bottomAlien[i].col = loc;//sets the column of first row of bottom aliens
		bottomAlien[i].alive = 1;//sets the first row of bottom aliens to alive state
		bottomAlien[i+11].row = 90;//sets the second row of the bottom aliens
		bottomAlien[i+11].col = loc;//sets the column of second row of bottom aliens
		bottomAlien[i+11].alive = 1;//sets the second row of bottom aliens to alive state
		loc += 15; //controls the column spacing in-between aliens
	}
	//now that the struct's have been built draw the top aliens to screen
	build_tops(framePointer,alien_top_in_12x8);

	//now that the struct's have been built draw the middle aliens to screen
	build_middle(framePointer,alien_middle_in_12x8 );

	//now that the struct's have been built draw the bottom aliens to screen
	build_bottom(framePointer, alien_bottom_in_12x8);
}

//draw top aliens to the screen
void build_tops(unsigned int * framePointer, int alien_top[]){

	int row, col, i; // initialize variables

	for(i = 0; i < TOP_TOTAL; i++){//loop through top column of alien's
		for(row = 0;row < ALIEN_HEIGHT; row++){ //loop through top alien sprite's row
			for(col = 0;col < WORD_WIDTH; col++){//loop through top alien sprite's column
				if ((alien_top[row] & (1<<(WORD_WIDTH-col-1)))&& top[i].alive == 1) {
					framePointer[(row+top[i].row)*1280 + (col + top[i].col)*2] = WHITE;
					framePointer[(row+top[i].row)*1280 + (col + top[i].col)*2+1] = WHITE;
					framePointer[(row+top[i].row)*1280 + 640 + (col + top[i].col)*2] = WHITE;
					framePointer[(row+top[i].row)*1280 + 640 + (col + top[i].col)*2 + 1] = WHITE;
				}
				else{
					framePointer[(row+top[i].row)*1280 + (col + top[i].col)*2] = BLACK;
					framePointer[(row+top[i].row)*1280 + (col + top[i].col)*2+1] = BLACK;
					framePointer[(row+top[i].row)*1280 + 640 + (col + top[i].col)*2] = BLACK;
					framePointer[(row+top[i].row)*1280 + 640 + (col + top[i].col)*2 + 1] = BLACK;
				}
			}
		}
	}
}

void build_middle(unsigned int * framePointer, int alien_middle[]){
	int row, col, i;
	for(row=0;row<ALIEN_HEIGHT;row++){
		for(col=0;col<WORD_WIDTH;col++){
			for(i = 0; i < MIDDLE_TOTAL;i++){
				if ((alien_middle[row] & (1<<(WORD_WIDTH-col-1))) && middleAlien[i].alive == 1) {
					framePointer[(row+middleAlien[i].row)*1280 + (col + middleAlien[i].col)*2] = WHITE;
					framePointer[(row+middleAlien[i].row)*1280 + (col + middleAlien[i].col)*2+1] = WHITE;
					framePointer[(row+middleAlien[i].row)*1280 + 640 + (col + middleAlien[i].col)*2] = WHITE;
					framePointer[(row+middleAlien[i].row)*1280 + 640 + (col + middleAlien[i].col)*2+1] = WHITE;
				}else{
					framePointer[(row+middleAlien[i].row)*1280 + (col + middleAlien[i].col)*2] = BLACK;
					framePointer[(row+middleAlien[i].row)*1280 + (col + middleAlien[i].col)*2+1] = BLACK;
					framePointer[(row+middleAlien[i].row)*1280 + 640 + (col + middleAlien[i].col)*2] = BLACK;
					framePointer[(row+middleAlien[i].row)*1280 + 640 + (col + middleAlien[i].col)*2+1] = BLACK;
				}
			}
		}
	}
}

void build_bottom(unsigned int * framePointer, int alien_bottom[]){
	int row, col, i;
	for(row=0;row<ALIEN_HEIGHT;row++){
		for(col=0;col<WORD_WIDTH;col++){
			for(i = 0; i < MIDDLE_TOTAL;i++){
				if ((alien_bottom[row] & (1<<(WORD_WIDTH-col-1))) && bottomAlien[i].alive == 1) {
					framePointer[(row+bottomAlien[i].row)*1280 + (col + bottomAlien[i].col)*2] = WHITE;
					framePointer[(row+bottomAlien[i].row)*1280 + (col + bottomAlien[i].col)*2+1] = WHITE;
					framePointer[(row+bottomAlien[i].row)*1280 + 640 + (col + bottomAlien[i].col)*2] = WHITE;
					framePointer[(row+bottomAlien[i].row)*1280 + 640 + (col + bottomAlien[i].col)*2+1] = WHITE;
				}else{
					framePointer[(row+bottomAlien[i].row)*1280 + (col + bottomAlien[i].col)*2] = BLACK;
					framePointer[(row+bottomAlien[i].row)*1280 + (col + bottomAlien[i].col)*2+1] = BLACK;
					framePointer[(row+bottomAlien[i].row)*1280 + 640 + (col + bottomAlien[i].col)*2] = BLACK;
					framePointer[(row+bottomAlien[i].row)*1280 + 640 + (col + bottomAlien[i].col)*2+1] = BLACK;
				}
			}
		}
	}
}

void aliens_left(unsigned int * framePointer){
	int i, row;

	for(i = 0; i < MIDDLE_TOTAL; i++){
		if(i < 11 ){top[i].col -= 1;}
		middleAlien[i].col -= 1;
		bottomAlien[i].col -= 1;
	}
	if(alien_count == 0){
		alien_count = 1;
		build_tops(framePointer,alien_top_in_12x8);
		build_middle(framePointer,alien_middle_in_12x8 );
		build_bottom(framePointer, alien_bottom_in_12x8);
	}else{
		alien_count = 0;
		build_tops(framePointer,alien_top_out_12x8);
		build_middle(framePointer,alien_middle_out_12x8 );
		build_bottom(framePointer, alien_bottom_out_12x8);
	}

	for(row = 0;row < ALIEN_HEIGHT; row++){
		for(i = 0; i < MIDDLE_TOTAL;i++){
			framePointer[(row+bottomAlien[i].row)*1280 + (WORD_WIDTH + bottomAlien[i].col)*2] = BLACK;
			framePointer[(row+bottomAlien[i].row)*1280 + (WORD_WIDTH + bottomAlien[i].col)*2+1] = BLACK;
			framePointer[(row+bottomAlien[i].row)*1280 + 640 + (WORD_WIDTH + bottomAlien[i].col)*2] = BLACK;
			framePointer[(row+bottomAlien[i].row)*1280 + 640 + (WORD_WIDTH + bottomAlien[i].col)*2+1] = BLACK;
			framePointer[(row+middleAlien[i].row)*1280 + (WORD_WIDTH + middleAlien[i].col)*2] = BLACK;
			framePointer[(row+middleAlien[i].row)*1280 + (WORD_WIDTH + middleAlien[i].col)*2+1] = BLACK;
			framePointer[(row+middleAlien[i].row)*1280 + 640 + (WORD_WIDTH + middleAlien[i].col)*2] = BLACK;
			framePointer[(row+middleAlien[i].row)*1280 + 640 + (WORD_WIDTH + middleAlien[i].col)*2+1] = BLACK;
		}

	}
}

void aliens_right(unsigned int * framePointer){
	int i, row;
	//increment column to create a move to the right
	for(i = 0; i < MIDDLE_TOTAL; i++){
		if(i < 11 ){top[i].col += 1;}
		middleAlien[i].col += 1;
		bottomAlien[i].col += 1;
	}
	//build alien's
	if(alien_count == 0){ //if flag is 0 draw the "in" version of alien
		alien_count = 1;  // set flag to 1 so that it cycles in-between "in" and "out" versions of the aliens
		build_tops(framePointer,alien_top_in_12x8);
		build_middle(framePointer,alien_middle_in_12x8 );
		build_bottom(framePointer, alien_bottom_in_12x8);
	}else{             // if flag is 1 draw the "out" version of alien
		alien_count = 0;// set flag to 0 so that it cycles in-between "in" and "out" versions of the aliens
		build_tops(framePointer,alien_top_out_12x8);
		build_middle(framePointer,alien_middle_out_12x8 );
		build_bottom(framePointer, alien_bottom_out_12x8);
	}

	for(row=0;row<ALIEN_HEIGHT;row++){
		for(i = 0; i < MIDDLE_TOTAL;i++){
			framePointer[(row+middleAlien[i].row)*1280 + (middleAlien[i].col)*2] = BLACK;
			framePointer[(row+middleAlien[i].row)*1280 + (middleAlien[i].col)*2+1] = BLACK;
			framePointer[(row+middleAlien[i].row)*1280 + 640 + (middleAlien[i].col)*2] = BLACK;
			framePointer[(row+middleAlien[i].row)*1280 + 640 + (middleAlien[i].col)*2+1] = BLACK;

			framePointer[(row+bottomAlien[i].row)*1280 + (bottomAlien[i].col-1)*2] = BLACK;
			framePointer[(row+bottomAlien[i].row)*1280 + (bottomAlien[i].col-1)*2+1] = BLACK;
			framePointer[(row+bottomAlien[i].row)*1280 + 640 + (bottomAlien[i].col-1)*2] = BLACK;
			framePointer[(row+bottomAlien[i].row)*1280 + 640 + (bottomAlien[i].col-1)*2+1] = BLACK;
		}

	}
}

void hit_left_rail(unsigned int * framePointer){
	int col,row, i;
	for(row=0;row<ALIEN_HEIGHT;row++){
		for(col=0;col<WORD_WIDTH;col++){
			if ((alien_top_out_12x8[row] & (1<<(WORD_WIDTH-col-1)))) {
				for(i = 0; i < TOP_TOTAL;i++){
					framePointer[(row+top[i].row)*1280 + (col + top[i].col)*2] = BLACK;
					framePointer[(row+top[i].row)*1280 + (col + top[i].col)*2+1] = BLACK;
					framePointer[(row+top[i].row)*1280 + 640 + (col + top[i].col)*2] = BLACK;
					framePointer[(row+top[i].row)*1280 + 640 + (col + top[i].col)*2 + 1] = BLACK;
				}
			}
		}
	}
	for(i = 0; i < MIDDLE_TOTAL; i++){
		if(i < 11 ){
			top[i].row += 15;
		}
		middleAlien[i].row += 15;
		bottomAlien[i].row += 15;
	}
	for(row=0;row<ALIEN_HEIGHT;row++){
		for(i = 0; i < MIDDLE_TOTAL;i++){
			framePointer[(row+middleAlien[i].row)*1280 + (middleAlien[i].col)*2] = BLACK;
			framePointer[(row+middleAlien[i].row)*1280 + (middleAlien[i].col)*2+1] = BLACK;
			framePointer[(row+middleAlien[i].row)*1280 + 640 + (middleAlien[i].col)*2] = BLACK;
			framePointer[(row+middleAlien[i].row)*1280 + 640 + (middleAlien[i].col)*2+1] = BLACK;
		}
	}
}

void hit_right_rail(unsigned int * framePointer){
	int col,row, i;
	for(row=0;row<ALIEN_HEIGHT;row++){
		for(col=0;col<WORD_WIDTH;col++){
			if ((alien_top_out_12x8[row] & (1<<(WORD_WIDTH-col-1)))) {
				for(i = 0; i < TOP_TOTAL;i++){
					framePointer[(row+top[i].row)*1280 + (col + top[i].col)*2] = BLACK;
					framePointer[(row+top[i].row)*1280 + (col + top[i].col)*2+1] = BLACK;
					framePointer[(row+top[i].row)*1280 + 640 + (col + top[i].col)*2] = BLACK;
					framePointer[(row+top[i].row)*1280 + 640 + (col + top[i].col)*2 + 1] = BLACK;
				}
			}
			if ((alien_top_in_12x8[row] & (1<<(WORD_WIDTH-col-1)))) {
				for(i = 0; i < TOP_TOTAL;i++){
					framePointer[(row+top[i].row)*1280 + (col + top[i].col)*2] = BLACK;
					framePointer[(row+top[i].row)*1280 + (col + top[i].col)*2+1] = BLACK;
					framePointer[(row+top[i].row)*1280 + 640 + (col + top[i].col)*2] = BLACK;
					framePointer[(row+top[i].row)*1280 + 640 + (col + top[i].col)*2 + 1] = BLACK;
				}
			}

		}
	}
	for(i = 0; i < MIDDLE_TOTAL; i++){
		if(i < 11 ){
			top[i].row += 15;
		}
		middleAlien[i].row += 15;
		bottomAlien[i].row += 15;
	}
	for(row=0;row<ALIEN_HEIGHT;row++){
		for(i = 0; i < TOP_TOTAL;i++){
			framePointer[(row+top[i].row)*1280 + (WORD_WIDTH-1 + top[i].col)*2] = BLACK;
			framePointer[(row+top[i].row)*1280 + (WORD_WIDTH-1 + top[i].col)*2+1] = BLACK;
			framePointer[(row+top[i].row)*1280 + 640 + (WORD_WIDTH-1 + top[i].col)*2] = BLACK;
			framePointer[(row+top[i].row)*1280 + 640 + (WORD_WIDTH-1 + top[i].col)*2+1] = BLACK;
		}
	}
}

int flag;
void aliens_move(unsigned int * framePointer){

	int i,j;
	for( i=0; i < TOP_TOTAL ; i++){
		if(top[i].alive == 1 || middleAlien[i].alive == 1 || middleAlien[i+11].alive == 1 || bottomAlien[i].alive == 1 || bottomAlien[i+11].alive == 1){
			if(top[i].col == 10){
				flag = 1;
				hit_left_rail(framePointer);
			}
		}
	}

	for(j = 10; j >= 0 ; j--){
		if(top[j].alive == 1 || middleAlien[j].alive == 1 || middleAlien[j+11].alive == 1 || bottomAlien[j].alive == 1 || bottomAlien[j+11].alive == 1){
			if(top[j].col == 307){
				flag = 0;
				hit_right_rail(framePointer);
			}
		}
	}

	if(flag == 1){
		aliens_right(framePointer);
	}else{
		aliens_left(framePointer);
	}
}


void aliens_kill(unsigned int * framePointer){
	int r;
	r = rand()%55;

	if(r < 11){
		if(top[r].alive == 0){
			aliens_kill(framePointer);
		}else{
			top[r].alive = 0;
			build_tops(framePointer,  alien_top_in_12x8);
		}
	}else if(r < 33 ){
		if(middleAlien[r-11].alive == 0){
			aliens_kill(framePointer);
		}else{
			middleAlien[r-11].alive = 0;
			build_middle(framePointer,  alien_middle_in_12x8);
		}
	}else{
		if(bottomAlien[r-33].alive == 0){
			aliens_kill(framePointer);
		}else{
			bottomAlien[r-33].alive = 0;
			build_bottom(framePointer,  alien_bottom_in_12x8);
		}
	}
}


// Returns true if aliens can shoot- that is, if there exists a top alive alien
bool can_aliens_shoot(){

	// This code checks to see if a top alien is alive
	int i;
	for(i = 0; i<TOP_TOTAL; i++){
		if(top[i].alive){	// If there exists a single alive top alien
			return true;
		}
	}

	return false;			// All the top aliens are dead; we cannot shoot
}

void alien_missle(unsigned int * framePointer){
	if(!can_aliens_shoot()){	// The aliens can't even shoot! Don't even try.
		xil_printf("FAIL TO SHOOT\n\r");
		return;
	}

	int r = rand()%ALIEN_COLUMNS;
	// Keep trying to fire a bullet until we get a real address for one!
	int bullet_address = -1;
	do{
		bullet_address = fire_bottom(framePointer, r);
	} while(bullet_address == -1);


	// We have a bullet address! now to make it alive and draw it.
	int i;
	for(i=0;i<ALIEN_NUM_BULLETS; i++){
		if(alien_bullet[i].alive){	// If we already have a living bullet
			continue;	// Go on to the next one
		} else{	// We have a dead bullet spot- let's alive a bullet here!
			alien_bullet[i].alive = true;

			// Randomly choose a bullet type
			alien_bullet[i].bullet_type = rand()%2 ? cross0 : lightning0;

			// # might be 640 instead??
			alien_bullet[i].col = bullet_address % 640;
			alien_bullet[i].row = bullet_address / 640;
			draw_bullet(framePointer, i, WHITE);
			return;
		}
	}
	return;

	/*
	//int r;
	r = rand()%55;
	if(r < 11){
		fire_top( framePointer, r);
	}else if(r < 33 ){
		fire_middle( framePointer,  r);
	}else{
		fire_bottom( framePointer,  r);
	}
	 */
}

void draw_bullet(unsigned int * framePointer, int32_t bullet, uint32_t color){

	switch(alien_bullet[bullet].bullet_type){
	case cross0:
	case cross3:
		// 5 pixels down in a line
		framePointer[(alien_bullet[bullet].row)*1280 + (alien_bullet[bullet].col)*2] = color;
		framePointer[(alien_bullet[bullet].row+1)*1280 + (alien_bullet[bullet].col)*2] = color;
		framePointer[(alien_bullet[bullet].row+2)*1280 + (alien_bullet[bullet].col)*2] = color;
		framePointer[(alien_bullet[bullet].row+3)*1280 + (alien_bullet[bullet].col)*2] = color;
		framePointer[(alien_bullet[bullet].row+4)*1280 + (alien_bullet[bullet].col)*2] = color;

		framePointer[(alien_bullet[bullet].row)*1280 + (alien_bullet[bullet].col)*2+1] = color;
		framePointer[(alien_bullet[bullet].row+1)*1280 + (alien_bullet[bullet].col)*2+1] = color;
		framePointer[(alien_bullet[bullet].row+2)*1280 + (alien_bullet[bullet].col)*2+1] = color;
		framePointer[(alien_bullet[bullet].row+3)*1280 + (alien_bullet[bullet].col)*2+1] = color;
		framePointer[(alien_bullet[bullet].row+4)*1280 + (alien_bullet[bullet].col)*2+1] = color;

		framePointer[(alien_bullet[bullet].row)*1280 + 640 + (alien_bullet[bullet].col)*2+1] = color;
		framePointer[(alien_bullet[bullet].row+1)*1280 + 640 + (alien_bullet[bullet].col)*2+1] = color;
		framePointer[(alien_bullet[bullet].row+2)*1280 + 640 + (alien_bullet[bullet].col)*2+1] = color;
		framePointer[(alien_bullet[bullet].row+3)*1280 + 640 + (alien_bullet[bullet].col)*2+1] = color;
		framePointer[(alien_bullet[bullet].row+4)*1280 + 640 + (alien_bullet[bullet].col)*2+1] = color;

		framePointer[(alien_bullet[bullet].row)*1280 + 640 + (alien_bullet[bullet].col)*2] = color;
		framePointer[(alien_bullet[bullet].row+1)*1280 + 640 + (alien_bullet[bullet].col)*2] = color;
		framePointer[(alien_bullet[bullet].row+2)*1280 + 640 + (alien_bullet[bullet].col)*2] = color;
		framePointer[(alien_bullet[bullet].row+3)*1280 + 640 + (alien_bullet[bullet].col)*2] = color;
		framePointer[(alien_bullet[bullet].row+4)*1280 + 640 + (alien_bullet[bullet].col)*2] = color;


		// Side bars on cross
		framePointer[(alien_bullet[bullet].row+2)*1280 + (alien_bullet[bullet].col+1)*2] = color;
		framePointer[(alien_bullet[bullet].row+2)*1280 + (alien_bullet[bullet].col+1)*2+1] = color;
		framePointer[(alien_bullet[bullet].row+2)*1280 + 640 + (alien_bullet[bullet].col+1)*2] = color;
		framePointer[(alien_bullet[bullet].row+2)*1280 + 640 + (alien_bullet[bullet].col+1)*2+1] = color;

		framePointer[(alien_bullet[bullet].row+2)*1280 + (alien_bullet[bullet].col-1)*2+1] = color;
		framePointer[(alien_bullet[bullet].row+2)*1280 + (alien_bullet[bullet].col-1)*2] = color;
		framePointer[(alien_bullet[bullet].row+2)*1280 + 640 + (alien_bullet[bullet].col-1)*2+1] = color;
		framePointer[(alien_bullet[bullet].row+2)*1280 + 640 + (alien_bullet[bullet].col-1)*2] = color;
		break;
	case cross1:
		// 5 pixels down in a line
		framePointer[(alien_bullet[bullet].row)*1280 + (alien_bullet[bullet].col)*2] = color;
		framePointer[(alien_bullet[bullet].row+1)*1280 + (alien_bullet[bullet].col)*2] = color;
		framePointer[(alien_bullet[bullet].row+2)*1280 + (alien_bullet[bullet].col)*2] = color;
		framePointer[(alien_bullet[bullet].row+3)*1280 + (alien_bullet[bullet].col)*2] = color;
		framePointer[(alien_bullet[bullet].row+4)*1280 + (alien_bullet[bullet].col)*2] = color;

		framePointer[(alien_bullet[bullet].row)*1280 + (alien_bullet[bullet].col)*2+1] = color;
		framePointer[(alien_bullet[bullet].row+1)*1280 + (alien_bullet[bullet].col)*2+1] = color;
		framePointer[(alien_bullet[bullet].row+2)*1280 + (alien_bullet[bullet].col)*2+1] = color;
		framePointer[(alien_bullet[bullet].row+3)*1280 + (alien_bullet[bullet].col)*2+1] = color;
		framePointer[(alien_bullet[bullet].row+4)*1280 + (alien_bullet[bullet].col)*2+1] = color;

		framePointer[(alien_bullet[bullet].row)*1280 + 640 + (alien_bullet[bullet].col)*2+1] = color;
		framePointer[(alien_bullet[bullet].row+1)*1280 + 640 + (alien_bullet[bullet].col)*2+1] = color;
		framePointer[(alien_bullet[bullet].row+2)*1280 + 640 + (alien_bullet[bullet].col)*2+1] = color;
		framePointer[(alien_bullet[bullet].row+3)*1280 + 640 + (alien_bullet[bullet].col)*2+1] = color;
		framePointer[(alien_bullet[bullet].row+4)*1280 + 640 + (alien_bullet[bullet].col)*2+1] = color;

		framePointer[(alien_bullet[bullet].row)*1280 + 640 + (alien_bullet[bullet].col)*2] = color;
		framePointer[(alien_bullet[bullet].row+1)*1280 + 640 + (alien_bullet[bullet].col)*2] = color;
		framePointer[(alien_bullet[bullet].row+2)*1280 + 640 + (alien_bullet[bullet].col)*2] = color;
		framePointer[(alien_bullet[bullet].row+3)*1280 + 640 + (alien_bullet[bullet].col)*2] = color;
		framePointer[(alien_bullet[bullet].row+4)*1280 + 640 + (alien_bullet[bullet].col)*2] = color;


		// Side bars on cross
		framePointer[(alien_bullet[bullet].row+3)*1280 + (alien_bullet[bullet].col+1)*2] = color;
		framePointer[(alien_bullet[bullet].row+3)*1280 + (alien_bullet[bullet].col+1)*2+1] = color;
		framePointer[(alien_bullet[bullet].row+3)*1280 + 640 + (alien_bullet[bullet].col+1)*2] = color;
		framePointer[(alien_bullet[bullet].row+3)*1280 + 640 + (alien_bullet[bullet].col+1)*2+1] = color;

		framePointer[(alien_bullet[bullet].row+3)*1280 + (alien_bullet[bullet].col-1)*2+1] = color;
		framePointer[(alien_bullet[bullet].row+3)*1280 + (alien_bullet[bullet].col-1)*2] = color;
		framePointer[(alien_bullet[bullet].row+3)*1280 + 640 + (alien_bullet[bullet].col-1)*2+1] = color;
		framePointer[(alien_bullet[bullet].row+3)*1280 + 640 + (alien_bullet[bullet].col-1)*2] = color;
		break;
	case cross2:
		// 5 pixels down in a line
		framePointer[(alien_bullet[bullet].row)*1280 + (alien_bullet[bullet].col)*2] = color;
		framePointer[(alien_bullet[bullet].row+1)*1280 + (alien_bullet[bullet].col)*2] = color;
		framePointer[(alien_bullet[bullet].row+2)*1280 + (alien_bullet[bullet].col)*2] = color;
		framePointer[(alien_bullet[bullet].row+3)*1280 + (alien_bullet[bullet].col)*2] = color;
		framePointer[(alien_bullet[bullet].row+4)*1280 + (alien_bullet[bullet].col)*2] = color;

		framePointer[(alien_bullet[bullet].row)*1280 + (alien_bullet[bullet].col)*2+1] = color;
		framePointer[(alien_bullet[bullet].row+1)*1280 + (alien_bullet[bullet].col)*2+1] = color;
		framePointer[(alien_bullet[bullet].row+2)*1280 + (alien_bullet[bullet].col)*2+1] = color;
		framePointer[(alien_bullet[bullet].row+3)*1280 + (alien_bullet[bullet].col)*2+1] = color;
		framePointer[(alien_bullet[bullet].row+4)*1280 + (alien_bullet[bullet].col)*2+1] = color;

		framePointer[(alien_bullet[bullet].row)*1280 + 640 + (alien_bullet[bullet].col)*2+1] = color;
		framePointer[(alien_bullet[bullet].row+1)*1280 + 640 + (alien_bullet[bullet].col)*2+1] = color;
		framePointer[(alien_bullet[bullet].row+2)*1280 + 640 + (alien_bullet[bullet].col)*2+1] = color;
		framePointer[(alien_bullet[bullet].row+3)*1280 + 640 + (alien_bullet[bullet].col)*2+1] = color;
		framePointer[(alien_bullet[bullet].row+4)*1280 + 640 + (alien_bullet[bullet].col)*2+1] = color;

		framePointer[(alien_bullet[bullet].row)*1280 + 640 + (alien_bullet[bullet].col)*2] = color;
		framePointer[(alien_bullet[bullet].row+1)*1280 + 640 + (alien_bullet[bullet].col)*2] = color;
		framePointer[(alien_bullet[bullet].row+2)*1280 + 640 + (alien_bullet[bullet].col)*2] = color;
		framePointer[(alien_bullet[bullet].row+3)*1280 + 640 + (alien_bullet[bullet].col)*2] = color;
		framePointer[(alien_bullet[bullet].row+4)*1280 + 640 + (alien_bullet[bullet].col)*2] = color;


		// Side bars on cross
		framePointer[(alien_bullet[bullet].row+1)*1280 + (alien_bullet[bullet].col+1)*2] = color;
		framePointer[(alien_bullet[bullet].row+1)*1280 + (alien_bullet[bullet].col+1)*2+1] = color;
		framePointer[(alien_bullet[bullet].row+1)*1280 + 640 + (alien_bullet[bullet].col+1)*2] = color;
		framePointer[(alien_bullet[bullet].row+1)*1280 + 640 + (alien_bullet[bullet].col+1)*2+1] = color;

		framePointer[(alien_bullet[bullet].row+1)*1280 + (alien_bullet[bullet].col-1)*2+1] = color;
		framePointer[(alien_bullet[bullet].row+1)*1280 + (alien_bullet[bullet].col-1)*2] = color;
		framePointer[(alien_bullet[bullet].row+1)*1280 + 640 + (alien_bullet[bullet].col-1)*2+1] = color;
		framePointer[(alien_bullet[bullet].row+1)*1280 + 640 + (alien_bullet[bullet].col-1)*2] = color;
		break;
	case lightning0:
		// 5 pixels down in a NOT line
		framePointer[(alien_bullet[bullet].row)*1280 + (alien_bullet[bullet].col-1)*2] = color;
		framePointer[(alien_bullet[bullet].row+1)*1280 + (alien_bullet[bullet].col)*2] = color;
		framePointer[(alien_bullet[bullet].row+2)*1280 + (alien_bullet[bullet].col+1)*2] = color;
		framePointer[(alien_bullet[bullet].row+3)*1280 + (alien_bullet[bullet].col)*2] = color;
		framePointer[(alien_bullet[bullet].row+4)*1280 + (alien_bullet[bullet].col-1)*2] = color;

		framePointer[(alien_bullet[bullet].row)*1280 + (alien_bullet[bullet].col-1)*2+1] = color;
		framePointer[(alien_bullet[bullet].row+1)*1280 + (alien_bullet[bullet].col)*2+1] = color;
		framePointer[(alien_bullet[bullet].row+2)*1280 + (alien_bullet[bullet].col+1)*2+1] = color;
		framePointer[(alien_bullet[bullet].row+3)*1280 + (alien_bullet[bullet].col)*2+1] = color;
		framePointer[(alien_bullet[bullet].row+4)*1280 + (alien_bullet[bullet].col-1)*2+1] = color;

		framePointer[(alien_bullet[bullet].row)*1280 + 640 + (alien_bullet[bullet].col-1)*2+1] = color;
		framePointer[(alien_bullet[bullet].row+1)*1280 + 640 + (alien_bullet[bullet].col)*2+1] = color;
		framePointer[(alien_bullet[bullet].row+2)*1280 + 640 + (alien_bullet[bullet].col+1)*2+1] = color;
		framePointer[(alien_bullet[bullet].row+3)*1280 + 640 + (alien_bullet[bullet].col)*2+1] = color;
		framePointer[(alien_bullet[bullet].row+4)*1280 + 640 + (alien_bullet[bullet].col-1)*2+1] = color;

		framePointer[(alien_bullet[bullet].row)*1280 + 640 + (alien_bullet[bullet].col-1)*2] = color;
		framePointer[(alien_bullet[bullet].row+1)*1280 + 640 + (alien_bullet[bullet].col)*2] = color;
		framePointer[(alien_bullet[bullet].row+2)*1280 + 640 + (alien_bullet[bullet].col+1)*2] = color;
		framePointer[(alien_bullet[bullet].row+3)*1280 + 640 + (alien_bullet[bullet].col)*2] = color;
		framePointer[(alien_bullet[bullet].row+4)*1280 + 640 + (alien_bullet[bullet].col-1)*2] = color;
		break;
	case lightning1:
		// 5 pixels down in a line
		framePointer[(alien_bullet[bullet].row)*1280 + (alien_bullet[bullet].col+1)*2] = color;
		framePointer[(alien_bullet[bullet].row+1)*1280 + (alien_bullet[bullet].col)*2] = color;
		framePointer[(alien_bullet[bullet].row+2)*1280 + (alien_bullet[bullet].col-1)*2] = color;
		framePointer[(alien_bullet[bullet].row+3)*1280 + (alien_bullet[bullet].col)*2] = color;
		framePointer[(alien_bullet[bullet].row+4)*1280 + (alien_bullet[bullet].col+1)*2] = color;

		framePointer[(alien_bullet[bullet].row)*1280 + (alien_bullet[bullet].col+1)*2+1] = color;
		framePointer[(alien_bullet[bullet].row+1)*1280 + (alien_bullet[bullet].col)*2+1] = color;
		framePointer[(alien_bullet[bullet].row+2)*1280 + (alien_bullet[bullet].col-1)*2+1] = color;
		framePointer[(alien_bullet[bullet].row+3)*1280 + (alien_bullet[bullet].col)*2+1] = color;
		framePointer[(alien_bullet[bullet].row+4)*1280 + (alien_bullet[bullet].col+1)*2+1] = color;

		framePointer[(alien_bullet[bullet].row)*1280 + 640 + (alien_bullet[bullet].col+1)*2+1] = color;
		framePointer[(alien_bullet[bullet].row+1)*1280 + 640 + (alien_bullet[bullet].col)*2+1] = color;
		framePointer[(alien_bullet[bullet].row+2)*1280 + 640 + (alien_bullet[bullet].col-1)*2+1] = color;
		framePointer[(alien_bullet[bullet].row+3)*1280 + 640 + (alien_bullet[bullet].col)*2+1] = color;
		framePointer[(alien_bullet[bullet].row+4)*1280 + 640 + (alien_bullet[bullet].col+1)*2+1] = color;

		framePointer[(alien_bullet[bullet].row)*1280 + 640 + (alien_bullet[bullet].col+1)*2] = color;
		framePointer[(alien_bullet[bullet].row+1)*1280 + 640 + (alien_bullet[bullet].col)*2] = color;
		framePointer[(alien_bullet[bullet].row+2)*1280 + 640 + (alien_bullet[bullet].col-1)*2] = color;
		framePointer[(alien_bullet[bullet].row+3)*1280 + 640 + (alien_bullet[bullet].col)*2] = color;
		framePointer[(alien_bullet[bullet].row+4)*1280 + 640 + (alien_bullet[bullet].col+1)*2] = color;
		break;
	}
	/*
	framePointer[(alien_bullet[bullet].row)*1280 + (alien_bullet[bullet].col)*2] = color;
	framePointer[(alien_bullet[bullet].row+1)*1280 + (alien_bullet[bullet].col)*2] = color;
	framePointer[(alien_bullet[bullet].row+2)*1280 + (alien_bullet[bullet].col)*2] = color;

	framePointer[(alien_bullet[bullet].row)*1280 + (alien_bullet[bullet].col)*2+1] = color;
	framePointer[(alien_bullet[bullet].row+1)*1280 + (alien_bullet[bullet].col)*2+1] = color;
	framePointer[(alien_bullet[bullet].row+2)*1280 + (alien_bullet[bullet].col)*2+1] = color;

	framePointer[(alien_bullet[bullet].row)*1280 + 640 + (alien_bullet[bullet].col)*2+1] = color;
	framePointer[(alien_bullet[bullet].row+1)*1280 + 640 + (alien_bullet[bullet].col)*2+1] = color;
	framePointer[(alien_bullet[bullet].row+2)*1280 + 640 + (alien_bullet[bullet].col)*2+1] = color;

	framePointer[(alien_bullet[bullet].row)*1280 + 640 + (alien_bullet[bullet].col)*2] = color;
	framePointer[(alien_bullet[bullet].row+1)*1280 + 640 + (alien_bullet[bullet].col)*2] = color;
	framePointer[(alien_bullet[bullet].row+2)*1280 + 640 + (alien_bullet[bullet].col)*2] = color;
	 */
}

// We are trying to fire a bullet from a bottom-tier alien.
// Returns the coordinate of the bullet (if any)
int32_t fire_bottom(unsigned int * framePointer, int r){
	if(!bottomAlien[r+11].alive){ // If the very bottom alien is dead
		if(!bottomAlien[r].alive){// AND the second row alien is also dead
			return fire_middle(framePointer, r);	// Try to make a higher alien shoot it
		} else{	// the bottom alien is dead, but the second-row one is alive

			// This is the starting coordinate of the bullet.
			return (bottomAlien[r].row+7)*640 + (6 + bottomAlien[r].col);

			/*
			framePointer[(bottomAlien[r].row+7)*1280 + (6 + bottomAlien[r].col)*2] = BLUE;
			framePointer[(bottomAlien[r].row+8)*1280 + (6 + bottomAlien[r].col)*2] = BLUE;
			framePointer[(bottomAlien[r].row+9)*1280 + (6 + bottomAlien[r].col)*2] = BLUE;

			framePointer[(bottomAlien[r].row+7)*1280 + (6 + bottomAlien[r].col)*2+1] = BLUE;
			framePointer[(bottomAlien[r].row+8)*1280 + (6 + bottomAlien[r].col)*2+1] = BLUE;
			framePointer[(bottomAlien[r].row+9)*1280 + (6 + bottomAlien[r].col)*2+1] = BLUE;

			framePointer[(bottomAlien[r].row+7)*1280 + 640 + (6 + bottomAlien[r].col)*2+1] = BLUE;
			framePointer[(bottomAlien[r].row+8)*1280 + 640 + (6 + bottomAlien[r].col)*2+1] = BLUE;
			framePointer[(bottomAlien[r].row+9)*1280 + 640 + (6 + bottomAlien[r].col)*2+1] = BLUE;

			framePointer[(bottomAlien[r].row+7)*1280 + 640 + (6 + bottomAlien[r].col)*2] = BLUE;
			framePointer[(bottomAlien[r].row+8)*1280 + 640 + (6 + bottomAlien[r].col)*2] = BLUE;
			framePointer[(bottomAlien[r].row+9)*1280 + 640 + (6 + bottomAlien[r].col)*2] = BLUE;
			 */
		}
	} else {	// The very bottom alien is alive and needs to shoot

		// Time to return the starting position of the bullet!
		return (bottomAlien[r+11].row+7)*640 + (6 + bottomAlien[r+11].col);

		/*
		framePointer[(bottomAlien[r+11].row+7)*1280 + (6 + bottomAlien[r+11].col)*2] = BLUE;
		framePointer[(bottomAlien[r+11].row+8)*1280 + (6 + bottomAlien[r+11].col)*2] = BLUE;
		framePointer[(bottomAlien[r+11].row+9)*1280 + (6 + bottomAlien[r+11].col)*2] = BLUE;

		framePointer[(bottomAlien[r+11].row+7)*1280 + (6 + bottomAlien[r+11].col)*2+1] = BLUE;
		framePointer[(bottomAlien[r+11].row+8)*1280 + (6 + bottomAlien[r+11].col)*2+1] = BLUE;
		framePointer[(bottomAlien[r+11].row+9)*1280 + (6 + bottomAlien[r+11].col)*2+1] = BLUE;

		framePointer[(bottomAlien[r+11].row+7)*1280 + 640 + (6 + bottomAlien[r+11].col)*2+1] = BLUE;
		framePointer[(bottomAlien[r+11].row+8)*1280 + 640 + (6 + bottomAlien[r+11].col)*2+1] = BLUE;
		framePointer[(bottomAlien[r+11].row+9)*1280 + 640 + (6 + bottomAlien[r+11].col)*2+1] = BLUE;

		framePointer[(bottomAlien[r+11].row+7)*1280 + 640 + (6 + bottomAlien[r+11].col)*2] = BLUE;
		framePointer[(bottomAlien[r+11].row+8)*1280 + 640 + (6 + bottomAlien[r+11].col)*2] = BLUE;
		framePointer[(bottomAlien[r+11].row+9)*1280 + 640 + (6 + bottomAlien[r+11].col)*2] = BLUE;
		 */
	}
	return 0;

	/*
	int b = r-33;
		if(bottomAlien[r-33].alive == 0){
			alien_missle(framePointer);
		}else{
			framePointer[(bottomAlien[b].row+7)*1280 + (6 + bottomAlien[b].col)*2] = BLUE;
			framePointer[(bottomAlien[b].row+8)*1280 + (6 + bottomAlien[b].col)*2] = BLUE;
			framePointer[(bottomAlien[b].row+9)*1280 + (6 + bottomAlien[b].col)*2] = BLUE;

			framePointer[(bottomAlien[b].row+7)*1280 + (6 + bottomAlien[b].col)*2+1] = BLUE;
			framePointer[(bottomAlien[b].row+8)*1280 + (6 + bottomAlien[b].col)*2+1] = BLUE;
			framePointer[(bottomAlien[b].row+9)*1280 + (6 + bottomAlien[b].col)*2+1] = BLUE;

			framePointer[(bottomAlien[b].row+7)*1280 + 640 + (6 + bottomAlien[b].col)*2+1] = BLUE;
			framePointer[(bottomAlien[b].row+8)*1280 + 640 + (6 + bottomAlien[b].col)*2+1] = BLUE;
			framePointer[(bottomAlien[b].row+9)*1280 + 640 + (6 + bottomAlien[b].col)*2+1] = BLUE;

			framePointer[(bottomAlien[b].row+7)*1280 + 640 + (6 + bottomAlien[b].col)*2] = BLUE;
			framePointer[(bottomAlien[b].row+8)*1280 + 640 + (6 + bottomAlien[b].col)*2] = BLUE;
			framePointer[(bottomAlien[b].row+9)*1280 + 640 + (6 + bottomAlien[b].col)*2] = BLUE;
		}
	 */
}


int32_t fire_middle(unsigned int * framePointer, int r){
	if(!middleAlien[r+11].alive){ // If the very bottom (middle) alien is dead
		if(!middleAlien[r].alive){// AND the second row (middle) alien is dead
			return fire_top(framePointer, r);	// Top row alien has to fire
		} else{	// the bottom alien is dead, but the second-row one is alive
			// This is the starting coordinate of the bullet
			return (middleAlien[r].row+6)*640 + (6 + middleAlien[r].col);

			/*
			framePointer[(middleAlien[r].row+6)*1280 + (6 + middleAlien[r].col)*2] = BLUE;
			framePointer[(middleAlien[r].row+7)*1280 + (6 + middleAlien[r].col)*2] = BLUE;
			framePointer[(middleAlien[r].row+8)*1280 + (6 + middleAlien[r].col)*2] = BLUE;

			framePointer[(middleAlien[r].row+6)*1280 + (6 + middleAlien[r].col)*2+1] = BLUE;
			framePointer[(middleAlien[r].row+7)*1280 + (6 + middleAlien[r].col)*2+1] = BLUE;
			framePointer[(middleAlien[r].row+8)*1280 + (6 + middleAlien[r].col)*2+1] = BLUE;

			framePointer[(middleAlien[r].row+6)*1280 + 640 + (6 + middleAlien[r].col)*2+1] = BLUE;
			framePointer[(middleAlien[r].row+7)*1280 + 640 + (6 + middleAlien[r].col)*2+1] = BLUE;
			framePointer[(middleAlien[r].row+8)*1280 + 640 + (6 + middleAlien[r].col)*2+1] = BLUE;

			framePointer[(middleAlien[r].row+6)*1280 + 640 + (6 + middleAlien[r].col)*2] = BLUE;
			framePointer[(middleAlien[r].row+7)*1280 + 640 + (6 + middleAlien[r].col)*2] = BLUE;
			framePointer[(middleAlien[r].row+8)*1280 + 640 + (6 + middleAlien[r].col)*2] = BLUE;
			 */
		}
	} else{	// The bottom alien is alive and needs to fire
		// This is the starting coordinate of the bullet
		return (middleAlien[r+11].row+6)*640 + (6 + middleAlien[r+11].col);
		/*
		framePointer[(middleAlien[r+11].row+6)*1280 + (6 + middleAlien[r+11].col)*2] = BLUE;
		framePointer[(middleAlien[r+11].row+7)*1280 + (6 + middleAlien[r+11].col)*2] = BLUE;
		framePointer[(middleAlien[r+11].row+8)*1280 + (6 + middleAlien[r+11].col)*2] = BLUE;

		framePointer[(middleAlien[r+11].row+6)*1280 + (6 + middleAlien[r+11].col)*2+1] = BLUE;
		framePointer[(middleAlien[r+11].row+7)*1280 + (6 + middleAlien[r+11].col)*2+1] = BLUE;
		framePointer[(middleAlien[r+11].row+8)*1280 + (6 + middleAlien[r+11].col)*2+1] = BLUE;

		framePointer[(middleAlien[r+11].row+6)*1280 + 640 + (6 + middleAlien[r+11].col)*2+1] = BLUE;
		framePointer[(middleAlien[r+11].row+7)*1280 + 640 + (6 + middleAlien[r+11].col)*2+1] = BLUE;
		framePointer[(middleAlien[r+11].row+8)*1280 + 640 + (6 + middleAlien[r+11].col)*2+1] = BLUE;

		framePointer[(middleAlien[r+11].row+6)*1280 + 640 + (6 + middleAlien[r+11].col)*2] = BLUE;
		framePointer[(middleAlien[r+11].row+7)*1280 + 640 + (6 + middleAlien[r+11].col)*2] = BLUE;
		framePointer[(middleAlien[r+11].row+8)*1280 + 640 + (6 + middleAlien[r+11].col)*2] = BLUE;
		 */
	}
	return 0;


	/*
	int m = r-11;
		if(middleAlien[m].alive == 0){
			alien_missle(framePointer);
		}else{
			framePointer[(middleAlien[m].row+6)*1280 + (6 + middleAlien[m].col)*2] = BLUE;
			framePointer[(middleAlien[m].row+7)*1280 + (6 + middleAlien[m].col)*2] = BLUE;
			framePointer[(middleAlien[m].row+8)*1280 + (6 + middleAlien[m].col)*2] = BLUE;

			framePointer[(middleAlien[m].row+6)*1280 + (6 + middleAlien[m].col)*2+1] = BLUE;
			framePointer[(middleAlien[m].row+7)*1280 + (6 + middleAlien[m].col)*2+1] = BLUE;
			framePointer[(middleAlien[m].row+8)*1280 + (6 + middleAlien[m].col)*2+1] = BLUE;

			framePointer[(middleAlien[m].row+6)*1280 + 640 + (6 + middleAlien[m].col)*2+1] = BLUE;
			framePointer[(middleAlien[m].row+7)*1280 + 640 + (6 + middleAlien[m].col)*2+1] = BLUE;
			framePointer[(middleAlien[m].row+8)*1280 + 640 + (6 + middleAlien[m].col)*2+1] = BLUE;

			framePointer[(middleAlien[m].row+6)*1280 + 640 + (6 + middleAlien[m].col)*2] = BLUE;
			framePointer[(middleAlien[m].row+7)*1280 + 640 + (6 + middleAlien[m].col)*2] = BLUE;
			framePointer[(middleAlien[m].row+8)*1280 + 640 + (6 + middleAlien[m].col)*2] = BLUE;
		}
	 */
}


int32_t fire_top(unsigned int * framePointer, int r){
	if(top[r].alive == 0){// Our top alien is dead. Check to see if all top aliens are dead
		// We failed to fire a missle! return -1
		return -1;
	}else{
		return (top[r].row+6)*640 + (6 + top[r].col);
		/*
		framePointer[(top[r].row+6)*1280 + (6 + top[r].col)*2] = BLUE;
		framePointer[(top[r].row+7)*1280 + (6 + top[r].col)*2] = BLUE;
		framePointer[(top[r].row+8)*1280 + (6 + top[r].col)*2] = BLUE;

		framePointer[(top[r].row+6)*1280 + (6 + top[r].col)*2+1] = BLUE;
		framePointer[(top[r].row+7)*1280 + (6 + top[r].col)*2+1] = BLUE;
		framePointer[(top[r].row+8)*1280 + (6 + top[r].col)*2+1] = BLUE;

		framePointer[(top[r].row+6)*1280 + 640 + (6 + top[r].col)*2+1] = BLUE;
		framePointer[(top[r].row+7)*1280 + 640 + (6 + top[r].col)*2+1] = BLUE;
		framePointer[(top[r].row+8)*1280 + 640 + (6 + top[r].col)*2+1] = BLUE;

		framePointer[(top[r].row+6)*1280 + 640 + (6 + top[r].col)*2] = BLUE;
		framePointer[(top[r].row+7)*1280 + 640 + (6 + top[r].col)*2] = BLUE;
		framePointer[(top[r].row+8)*1280 + 640 + (6 + top[r].col)*2] = BLUE;
		 */
	}
}

/*
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
 */

void update_shells(unsigned int * framePointer){
	int i;
	for(i=0;i<ALIEN_NUM_BULLETS;i++){

		if(alien_bullet[i].row > 240){		// If alien bullet is off the screen
			alien_bullet[i].alive = false;
		} else if(alien_bullet[i].alive){
			//	erase bullet
			draw_bullet(framePointer, i, BLACK);
			if(alien_bullet[i].bullet_type == cross0){
				alien_bullet[i].bullet_type = cross1;
			} else if (alien_bullet[i].bullet_type == cross1){
				alien_bullet[i].bullet_type = cross3;
			} else if (alien_bullet[i].bullet_type == cross3){
				alien_bullet[i].bullet_type = cross2;
			} else if (alien_bullet[i].bullet_type == cross2){
				alien_bullet[i].bullet_type = cross0;
			}else if (alien_bullet[i].bullet_type == lightning0){
				alien_bullet[i].bullet_type = lightning1;
			}else if (alien_bullet[i].bullet_type == lightning1){
				alien_bullet[i].bullet_type = lightning0;
			}

			alien_bullet[i].row++;
			draw_bullet(framePointer, i, BLUE);
			// redraw the alien bullet now.
		}
	}

	/*
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
	 */
}






