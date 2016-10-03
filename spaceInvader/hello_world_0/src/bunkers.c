/*
 * bunkers.c
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

#include "bunkers.h"

#define BUNKER_HEIGHT 18		// Bunkers are 18 pixels high
#define BUNKER_DAMAGE_HEIGHT 6	// Each bunnker square is size 6
#define BUNKER_ROW 175			// All bunkers are at row 175
#define GREEN 0x0000FF00
#define BLACK 0x00000000

#define packword6(b5,b4,b3,b2,b1,b0) \
	((b5  << 5 ) | (b4  << 4 ) | (b3  << 3 ) | (b2  << 2 ) | (b1  << 1 ) | (b0  << 0 ) )


// Necessary for storing the bunker data
#define packword24(b23,b22,b21,b20,b19,b18,b17,b16,b15,b14,b13,b12,b11,b10,b9,b8,b7,b6,b5,b4,b3,b2,b1,b0) \
		((b23 << 23) | (b22 << 22) | (b21 << 21) | (b20 << 20) | (b19 << 19) | (b18 << 18) | (b17 << 17) | (b16 << 16) |						  \
				(b15 << 15) | (b14 << 14) | (b13 << 13) | (b12 << 12) | (b11 << 11) | (b10 << 10) | (b9  << 9 ) | (b8  << 8 ) |						  \
				(b7  << 7 ) | (b6  << 6 ) | (b5  << 5 ) | (b4  << 4 ) | (b3  << 3 ) | (b2  << 2 ) | (b1  << 1 ) | (b0  << 0 ) )
// Shape of the entire bunker.
static const int bunker_24x18[BUNKER_HEIGHT] = {
		packword24(0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0),
		packword24(0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0),
		packword24(0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0),
		packword24(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1),
		packword24(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1),
		packword24(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1),
		packword24(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1),
		packword24(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1),
		packword24(1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1),
		packword24(1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1),
		packword24(1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1),
		packword24(1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1),
		packword24(1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1),
		packword24(1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1),
		packword24(1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1),
		packword24(1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1),
		packword24(1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1),
		packword24(1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1)};

// First time a bunker is hit, the first damage that happens
static const int bunkerDamage0_6x6[BUNKER_DAMAGE_HEIGHT] = {
	packword6(0,1,1,0,0,0),	packword6(0,0,0,0,0,1),	packword6(1,1,0,1,0,0),
	packword6(1,0,0,0,0,0),	packword6(0,0,1,1,0,0),	packword6(0,0,0,0,1,0)};

// Second time a bunker is hit, this is its damage
static const int bunkerDamage1_6x6[BUNKER_DAMAGE_HEIGHT] = {
	packword6(1,1,1,0,1,0),	packword6(1,0,1,0,0,1),	packword6(1,1,0,1,1,1),
	packword6(1,0,0,0,0,0),	packword6(0,1,1,1,0,1),	packword6(0,1,1,0,1,0)};

// Third time a bunker is hit, this is its damage
static const int bunkerDamage2_6x6[BUNKER_DAMAGE_HEIGHT] = {
	packword6(1,1,1,1,1,1),	packword6(1,0,1,1,0,1),	packword6(1,1,0,1,1,1),
	packword6(1,1,0,1,1,0),	packword6(0,1,1,1,0,1),	packword6(1,1,1,1,1,1)};

// Fourth time a bunker is hit, this is its damage
static const int bunkerDamage3_6x6[BUNKER_DAMAGE_HEIGHT] = {
	packword6(1,1,1,1,1,1),	packword6(1,1,1,1,1,1),	packword6(1,1,1,1,1,1),
	packword6(1,1,1,1,1,1),	packword6(1,1,1,1,1,1),	packword6(1,1,1,1,1,1)};

#define DAMAGE_WORD_WIDTH 6
#define WORD_WIDTH 24
#define NUM_OF_BUNKERS 4
#define LOC_BUNKER_ONE 60	// Divided this by 2 because screen is half

struct bunker{		// Holds the data for each bunker
	int row;		// Where it is
	int col;		// on the screen
	int damage;		// What damage level the bunker is at
	int pixel[];	// A bunker is made out of squares- whether it's alive/dead
}bunker[3];

// Initializes the bunkers
void init_bunkers(unsigned int * framePointer){
	int i, loc = LOC_BUNKER_ONE;	//
	for(i = 0; i < NUM_OF_BUNKERS ; i++){
		bunker[i].row = BUNKER_ROW;	// Divided by 2 because screen is half
		bunker[i].col = loc;		// which column it is at
		bunker[i].damage = 0;		// Start undamaged
		loc += LOC_BUNKER_ONE;		// Add by the offset
	}
	build_bunkers(framePointer);	// Draw the bunkers on the screen
}


void build_bunkers(unsigned int * framePointer){
	int row, col;
	for(row=0;row<18;row++){
		for(col=0;col<24;col++){
			if ((bunker_24x18[row] & (1<<(WORD_WIDTH-col-1)))) {
				framePointer[(row + bunker[0].row)*640*2 + (col + bunker[0].col)*2] = GREEN;
				framePointer[(row + bunker[0].row)*640*2 + (col + bunker[0].col)*2 + 1] = GREEN;
				framePointer[(row + bunker[0].row)*640*2 + 640 + (col + bunker[0].col)*2] = GREEN;
				framePointer[(row + bunker[0].row)*640*2 + 640 + (col + bunker[0].col)*2 + 1] = GREEN;

				framePointer[(row + bunker[1].row)*640*2 + (col + bunker[1].col)*2] = GREEN;
				framePointer[(row + bunker[1].row)*640*2 + (col + bunker[1].col)*2 + 1] = GREEN;
				framePointer[(row + bunker[1].row)*640*2 + 640 + (col + bunker[1].col)*2] = GREEN;
				framePointer[(row + bunker[1].row)*640*2 + 640 + (col + bunker[1].col)*2 + 1] = GREEN;

				framePointer[(row + bunker[2].row)*640*2 + (col + bunker[2].col)*2] = GREEN;
				framePointer[(row + bunker[2].row)*640*2 + (col + bunker[2].col)*2 + 1] = GREEN;
				framePointer[(row + bunker[2].row)*640*2 + 640 + (col + bunker[2].col)*2] = GREEN;
				framePointer[(row + bunker[2].row)*640*2 + 640 + (col + bunker[2].col)*2 + 1] = GREEN;


				framePointer[(row + bunker[3].row)*640*2 + (col + bunker[3].col)*2] = GREEN;
				framePointer[(row + bunker[3].row)*640*2 + (col + bunker[3].col)*2 + 1] = GREEN;
				framePointer[(row + bunker[3].row)*640*2 + 640 + (col + bunker[3].col)*2] = GREEN;
				framePointer[(row + bunker[3].row)*640*2 + 640 + (col + bunker[3].col)*2 + 1] = GREEN;
			}
		}
	}
}

void rand_bunker(unsigned int * framePointer){

	int r = rand()%4;
	switch (r){
	case 0:
		bunker0(rand()%10, framePointer);
		break;
	case 1:
		bunker1(rand()%10, framePointer);
		break;
	case 2:
		bunker2(rand()%10, framePointer);
		break;
	case 3:
		bunker3(rand()%10, framePointer);
		break;
	}
}
int bunker_zero[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int bunker_one[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int bunker_two[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int bunker_three[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

void bunker1(int r, unsigned int * framePointer){
	switch(r){
	case 0:
		if(bunker_one[0]== 4){
			rand_bunker(framePointer);
		}else {
			degrigation_patern(0, 0, 1, bunker_one[0], framePointer);
			bunker_one[0]+=1;
		}
		break;
	case 1:
		if(bunker_one[1]== 4){
			rand_bunker(framePointer);
		}else {
			degrigation_patern(0, 6, 1, bunker_one[1], framePointer);
			bunker_one[1]+=1;
		}
		break;
	case 2:
		if(bunker_one[2]== 4){
			rand_bunker(framePointer);
		}else {
			degrigation_patern(0, 12, 1, bunker_one[2], framePointer);
			bunker_one[2]+=1;
		}
		break;
	case 3:
		if(bunker_one[3]== 4){
			rand_bunker(framePointer);
		}else {
			degrigation_patern(0, 18, 1, bunker_one[3], framePointer);
			bunker_one[3]+=1;
		}
		break;
	case 4:
		if(bunker_one[4]== 4){
			rand_bunker(framePointer);
		}else {
			degrigation_patern(6, 0, 1, bunker_one[4], framePointer);
			bunker_one[4]+=1;
		}
		break;
	case 5:
		if(bunker_one[5]== 4){
			rand_bunker(framePointer);
		}else {
			degrigation_patern(6, 6, 1, bunker_one[5], framePointer);
			bunker_one[5]+=1;
		}
		break;
	case 6:
		if(bunker_one[6]== 4){
			rand_bunker(framePointer);
		}else {
			degrigation_patern(6, 12, 1, bunker_one[6], framePointer);
			bunker_one[6]+=1;
		}
		break;
	case 7:
		if(bunker_one[7]== 4){
			rand_bunker(framePointer);
		}else {
			degrigation_patern(6, 18, 1, bunker_one[7], framePointer);
			bunker_one[7]+=1;
		}
		break;
	case 8:
		if(bunker_one[8]== 4){
			rand_bunker(framePointer);
		}else {
			degrigation_patern(12, 0, 1, bunker_one[8], framePointer);
			bunker_one[8]+=1;
		}
		break;
	case 9:
		if(bunker_one[9]== 4){
			rand_bunker(framePointer);
		}else {
			degrigation_patern(12, 18, 1, bunker_one[9], framePointer);
			bunker_one[9]+=1;
		}
		break;
	}
}


void bunker0(int r, unsigned int * framePointer){
	switch(r){
	case 0:
		if(bunker_zero[0]== 4){
			rand_bunker(framePointer);
		}else {
			degrigation_patern(0, 0, 0, bunker_zero[0], framePointer);
			bunker_zero[0]+=1;
		}
		break;
	case 1:
		if(bunker_zero[1]== 4){
			rand_bunker(framePointer);
		}else {
			degrigation_patern(0, 6, 0, bunker_zero[1], framePointer);
			bunker_zero[1]+=1;
		}
		break;
	case 2:
		if(bunker_zero[2]== 4){
			rand_bunker(framePointer);
		}else {
			degrigation_patern(0, 12, 0, bunker_zero[2], framePointer);
			bunker_zero[2]+=1;
		}
		break;
	case 3:
		if(bunker_zero[3]== 4){
			rand_bunker(framePointer);
		}else {
			degrigation_patern(0, 18, 0, bunker_zero[3], framePointer);
			bunker_zero[3]+=1;
		}
		break;
	case 4:
		if(bunker_zero[4]== 4){
			rand_bunker(framePointer);
		}else {
			degrigation_patern(6, 0, 0, bunker_zero[4], framePointer);
			bunker_zero[4]+=1;
		}
		break;
	case 5:
		if(bunker_zero[5]== 4){
			rand_bunker(framePointer);
		}else {
			degrigation_patern(6, 6, 0, bunker_zero[5], framePointer);
			bunker_zero[5]+=1;
		}
		break;
	case 6:
		if(bunker_zero[6]== 4){
			rand_bunker(framePointer);
		}else {
			degrigation_patern(6, 12, 0, bunker_zero[6], framePointer);
			bunker_zero[6]+=1;
		}
		break;
	case 7:
		if(bunker_zero[7]== 4){
			rand_bunker(framePointer);
		}else {
			degrigation_patern(6, 18, 0, bunker_zero[7], framePointer);
			bunker_zero[7]+=1;
		}
		break;
	case 8:
		if(bunker_zero[8]== 4){
			rand_bunker(framePointer);
		}else {
			degrigation_patern(12, 0, 0, bunker_zero[8], framePointer);
			bunker_zero[8]+=1;
		}
		break;
	case 9:
		if(bunker_zero[9]== 4){
			rand_bunker(framePointer);
		}else {
			degrigation_patern(12, 18, 0, bunker_zero[9], framePointer);
			bunker_zero[9]+=1;
		}
		break;
	}
}



void bunker2(int r, unsigned int * framePointer){
	switch(r){
	case 0:
		if(bunker_two[0]== 4){
			rand_bunker(framePointer);
		}else {
			degrigation_patern(0, 0, 2, bunker_two[0], framePointer);
			bunker_two[0]+=1;
		}
		break;
	case 1:
		if(bunker_two[1]== 4){
			rand_bunker(framePointer);
		}else {
			degrigation_patern(0, 6, 2, bunker_two[1], framePointer);
			bunker_two[1]+=1;
		}
		break;
	case 2:
		if(bunker_two[2]== 4){
			rand_bunker(framePointer);
		}else {
			degrigation_patern(0, 12, 2, bunker_two[2], framePointer);
			bunker_two[2]+=1;
		}
		break;
	case 3:
		if(bunker_two[3]== 4){
			rand_bunker(framePointer);
		}else {
			degrigation_patern(0, 18, 2, bunker_two[3], framePointer);
			bunker_two[3]+=1;
		}
		break;
	case 4:
		if(bunker_two[4]== 4){
			rand_bunker(framePointer);
		}else {
			degrigation_patern(6, 0, 2, bunker_two[4], framePointer);
			bunker_two[4]+=1;
		}
		break;
	case 5:
		if(bunker_two[5]== 4){
			rand_bunker(framePointer);
		}else {
			degrigation_patern(6, 6, 2, bunker_two[5], framePointer);
			bunker_two[5]+=1;
		}
		break;
	case 6:
		if(bunker_two[6]== 4){
			rand_bunker(framePointer);
		}else {
			degrigation_patern(6, 12, 2, bunker_two[6], framePointer);
			bunker_two[6]+=1;
		}
		break;
	case 7:
		if(bunker_two[7]== 4){
			rand_bunker(framePointer);
		}else {
			degrigation_patern(6, 18, 2, bunker_two[7], framePointer);
			bunker_two[7]+=1;
		}
		break;
	case 8:
		if(bunker_two[8]== 4){
			rand_bunker(framePointer);
		}else {
			degrigation_patern(12, 0, 2, bunker_two[8], framePointer);
			bunker_two[8]+=1;
		}
		break;
	case 9:
		if(bunker_two[9]== 4){
			rand_bunker(framePointer);
		}else {
			degrigation_patern(12, 18, 2, bunker_two[9], framePointer);
			bunker_two[9]+=1;
		}
		break;
	}
}


void bunker3(int r, unsigned int * framePointer){
	switch(r){
	case 0:
		if(bunker_three[0]== 4){
			rand_bunker(framePointer);
		}else {
			degrigation_patern(0, 0, 3, bunker_three[0], framePointer);
			bunker_three[0]+=1;
		}
		break;
	case 1:
		if(bunker_three[1]== 4){
			rand_bunker(framePointer);
		}else {
			degrigation_patern(0, 6, 3, bunker_three[1], framePointer);
			bunker_three[1]+=1;
		}
		break;
	case 2:
		if(bunker_three[2]== 4){
			rand_bunker(framePointer);
		}else {
			degrigation_patern(0, 12, 3, bunker_three[2], framePointer);
			bunker_three[2]+=1;
		}
		break;
	case 3:
		if(bunker_three[3]== 4){
			rand_bunker(framePointer);
		}else {
			degrigation_patern(0, 18, 3, bunker_three[3], framePointer);
			bunker_three[3]+=1;
		}
		break;
	case 4:
		if(bunker_three[4]== 4){
			rand_bunker(framePointer);
		}else {
			degrigation_patern(6, 0, 3, bunker_three[4], framePointer);
			bunker_three[4]+=1;
		}
		break;
	case 5:
		if(bunker_three[5]== 4){
			rand_bunker(framePointer);
		}else {
			degrigation_patern(6, 6, 3, bunker_three[5], framePointer);
			bunker_three[5]+=1;
		}
		break;
	case 6:
		if(bunker_three[6]== 4){
			rand_bunker(framePointer);
		}else {
			degrigation_patern(6, 12, 3, bunker_three[6], framePointer);
			bunker_three[6]+=1;
		}
		break;
	case 7:
		if(bunker_three[7]== 4){
			rand_bunker(framePointer);
		}else {
			degrigation_patern(6, 18, 3, bunker_three[7], framePointer);
			bunker_three[7]+=1;
		}
		break;
	case 8:
		if(bunker_three[8]== 4){
			rand_bunker(framePointer);
		}else {
			degrigation_patern(12, 0, 3, bunker_three[8], framePointer);
			bunker_three[8]+=1;
		}
		break;
	case 9:
		if(bunker_three[9]== 4){
			rand_bunker(framePointer);
		}else {
			degrigation_patern(12, 18, 3, bunker_three[9], framePointer);
			bunker_three[9]+=1;
		}
		break;
	}
}


void degrigation_patern(int row, int col, int bunker_number, int damage, unsigned int * framePointer){
	int r,c;
	for(r=0;r<BUNKER_DAMAGE_HEIGHT;r++){
		for(c=0;c<DAMAGE_WORD_WIDTH;c++){
			if (damage == 0){
				if ((bunkerDamage0_6x6[r] & (1<<(DAMAGE_WORD_WIDTH-c-1)))) {
					framePointer[(r + row + bunker[bunker_number].row)*640*2 + (c + col + bunker[bunker_number].col)*2] = BLACK;
					framePointer[(r + row + bunker[bunker_number].row)*640*2 + (c + col + bunker[bunker_number].col)*2 + 1] = BLACK;
					framePointer[(r + row + bunker[bunker_number].row)*640*2 + 640 + (c + col + bunker[bunker_number].col)*2] = BLACK;
					framePointer[(r + row + bunker[bunker_number].row)*640*2 + 640 + (c + col + bunker[bunker_number].col)*2 + 1] = BLACK;
				}
			}else if(damage == 1){
				if ((bunkerDamage1_6x6[r] & (1<<(DAMAGE_WORD_WIDTH-c-1)))) {
					framePointer[(row + r + bunker[bunker_number].row)*640*2 + (c + col + bunker[bunker_number].col)*2] = BLACK;
					framePointer[(row + r + bunker[bunker_number].row)*640*2 + (c + col + bunker[bunker_number].col)*2 + 1] = BLACK;
					framePointer[(row + r + bunker[bunker_number].row)*640*2 + 640 + (c + col + bunker[bunker_number].col)*2] = BLACK;
					framePointer[(row + r + bunker[bunker_number].row)*640*2 + 640 + (c + col + bunker[bunker_number].col)*2 + 1] = BLACK;
				}

			}else if(damage == 2){
				if ((bunkerDamage2_6x6[r] & (1<<(DAMAGE_WORD_WIDTH-c-1)))) {
					framePointer[(row + r + bunker[bunker_number].row)*640*2 + (col + c + bunker[bunker_number].col)*2] = BLACK;
					framePointer[(row + r + bunker[bunker_number].row)*640*2 + (col + c + bunker[bunker_number].col)*2 + 1] = BLACK;
					framePointer[(row + r + bunker[bunker_number].row)*640*2 + 640 + (col + c + bunker[bunker_number].col)*2] = BLACK;
					framePointer[(row + r + bunker[bunker_number].row)*640*2 + 640 + (col + c + bunker[bunker_number].col)*2 + 1] = BLACK;
				}

			}else if(damage == 3){
				if ((bunkerDamage3_6x6[r] & (1<<(DAMAGE_WORD_WIDTH-c-1)))) {
					framePointer[(row + r + bunker[bunker_number].row)*640*2 + (col + c + bunker[bunker_number].col)*2] = BLACK;
					framePointer[(row + r + bunker[bunker_number].row)*640*2 + (col + c + bunker[bunker_number].col)*2 + 1] = BLACK;
					framePointer[(row + r + bunker[bunker_number].row)*640*2 + 640 + (col + c + bunker[bunker_number].col)*2] = BLACK;
					framePointer[(row + r + bunker[bunker_number].row)*640*2 + 640 + (col + c + bunker[bunker_number].col)*2 + 1] = BLACK;
				}
			}
		}
	}
}


