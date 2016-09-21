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



#define BUNKER_HEIGHT 18


#define packword24(b23,b22,b21,b20,b19,b18,b17,b16,b15,b14,b13,b12,b11,b10,b9,b8,b7,b6,b5,b4,b3,b2,b1,b0) \
((b23 << 23) | (b22 << 22) | (b21 << 21) | (b20 << 20) | (b19 << 19) | (b18 << 18) | (b17 << 17) | (b16 << 16) |						  \
 (b15 << 15) | (b14 << 14) | (b13 << 13) | (b12 << 12) | (b11 << 11) | (b10 << 10) | (b9  << 9 ) | (b8  << 8 ) |						  \
 (b7  << 7 ) | (b6  << 6 ) | (b5  << 5 ) | (b4  << 4 ) | (b3  << 3 ) | (b2  << 2 ) | (b1  << 1 ) | (b0  << 0 ) )
// Shape of the entire bunker.
static const int bunker_24x18[BUNKER_HEIGHT] =
{
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
packword24(1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1)
};

#define WORD_WIDTH 24
struct bunker1{
	int row;
	int col;
};
struct bunker2{
	int row;
	int col;
};
struct bunker3{
	int row;
	int col;
};

void init_bunker(unsigned int * framePointer){
	//build_bunker_one(framePointer);
	build_bunker_two(framePointer);
	//build_bunker_three(framePointer);
}

void build_bunker_one(unsigned int * framePointer){
	int row, col;
	for( row = 0; row < 18; row++){
		 for( col = 0; col < 24; col++){
			if ((bunker_24x18[row] & (1<<(WORD_WIDTH - col - 1)))) {
				framePointer[row*640 + col ] = 0xFF000000;
			}else{
				framePointer[row*640 + col] = 0x00000000;  // frame 0 is green here.
			}
		 }
	 }
}
void build_bunker_two(unsigned int * framePointer){
	 int row, col;
	 for(row=0;row<18;row++){
		 for(col=0;col<24;col++){
			if ((bunker_24x18[row] & (1<<(WORD_WIDTH-1-col)))) {
				framePointer[(row+ 400)*640 + col + 150] = 0x0000FF00;
			}else{
				framePointer[(row + 400)*640 + col + 150] = 0x00000000;  // frame 0 is green here.
			}
		 }
	 }
}
void build_bunker_three(unsigned int * framePointer){
	int row, col;
		 for(row = 300; row < 318; row++){
			 for(col = 292; col < 324; col++){
				if ((bunker_24x18[row-300] & (1<<(WORD_WIDTH-1-col-292)))) {
					framePointer[row*640 + col] = 0x0000FF00;
				}else{
					framePointer[row*640 + col] = 0x00000000;  // frame 0 is green here.
				}
			 }
		 }
}
void hit_bunker(int bunker){

}
void draw_data(){

}



