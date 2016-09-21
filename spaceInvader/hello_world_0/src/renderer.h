/*
 * renderer.h
 *
 *  Created on: Sep 21, 2016
 *      Author: superman
 */

#ifndef RENDERER_H_
#define RENDERER_H_

#include "xaxivdma.h"


#define FRAME_BUFFER_0_ADDR 0xC0000000  // location in DDR where store images.

XAxiVdma vdmaController;					// Var for the video controller
XAxiVdma_DmaSetup frameBuffer;

// Inits everything to print to the screen.
int renderer_init();

#endif /* RENDERER_H_ */
