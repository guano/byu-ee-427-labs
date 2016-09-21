/*
 * Copyright (c) 2009 Xilinx, Inc.  All rights reserved.
 *
 * Xilinx, Inc.
 * XILINX IS PROVIDING THIS DESIGN, CODE, OR INFORMATION "AS IS" AS A
 * COURTESY TO YOU.  BY PROVIDING THIS DESIGN, CODE, OR INFORMATION AS
 * ONE POSSIBLE   IMPLEMENTATION OF THIS FEATURE, APPLICATION OR
 * STANDARD, XILINX IS MAKING NO REPRESENTATION THAT THIS IMPLEMENTATION
 * IS FREE FROM ANY CLAIMS OF INFRINGEMENT, AND YOU ARE RESPONSIBLE
 * FOR OBTAINING ANY RIGHTS YOU MAY REQUIRE FOR YOUR IMPLEMENTATION.
 * XILINX EXPRESSLY DISCLAIMS ANY WARRANTY WHATSOEVER WITH RESPECT TO
 * THE ADEQUACY OF THE IMPLEMENTATION, INCLUDING BUT NOT LIMITED TO
 * ANY WARRANTIES OR REPRESENTATIONS THAT THIS IMPLEMENTATION IS FREE
 * FROM CLAIMS OF INFRINGEMENT, IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 */

/*
 * helloworld.c: simple test application
 */

#include <stdio.h>
#include "platform.h"
#include "xparameters.h"
#include "xaxivdma.h"
#include "xio.h"
#include "time.h"
#include "unistd.h"
#include "renderer.h"
#define DEBUG
void print(char *str);

#define FRAME_BUFFER_0_ADDR 0xC0000000  // Starting location in DDR where we will store the images that we display.
#define MAX_SILLY_TIMER 1;

int main()
{
	init_platform();                   // Necessary for all programs.
	renderer_init();


	unsigned int * framePointer0 = (unsigned int *) FRAME_BUFFER_0_ADDR;
	unsigned int * framePointer1 = ((unsigned int *) FRAME_BUFFER_0_ADDR) + 640*480;

	int frameIndex = 0;
	// Oscillate between frame 0 and frame 1.
	int sillyTimer = MAX_SILLY_TIMER;  // Just a cheap delay between frames.
	while (1) {
		while (sillyTimer) sillyTimer--;    // Decrement the timer.
		sillyTimer = MAX_SILLY_TIMER;       // Reset the timer.
		frameIndex = (frameIndex + 1) % 2;  // Alternate between frame 0 and frame 1.
		if(frameIndex == 0){
			int row=0, col=0;
			for( row=0; row<480; row++) {
				for(col=0; col<640; col++) {
					framePointer0[row*640 + col] += 100;
				}
			}
		}
		if (XST_FAILURE == (&vdmaController, frameIndex,  XAXIVDMA_READ)) {
			xil_printf("vdma parking failed\n\r");
		}
	}
	cleanup_platform();

	return 0;
}
