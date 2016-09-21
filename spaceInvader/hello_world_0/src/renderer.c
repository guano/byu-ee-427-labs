/*
 * renderer.c
 *
 *  Created on: Sep 21, 2016
 *      Author: taylor
 *
 *
 *      This is the render file.
 *      This is the only file that accesses display memory.
 *
 *      DO NOTE:
 *      Our game thinks it is in 320 x 240.
 *
 *      But our screen is double that.
 *      Everything in renderer.c that writes to memory will need to double
 *      everything to make it work.
 */

#include "renderer.h"
#include "xparameters.h"
#include "xaxivdma.h"
#include "xio.h"

#define VDMA_ID 		XPAR_AXI_VDMA_0_DEVICE_ID	// Need a shorter nickname
#define VDMA_BASEADDR 	XPAR_AXI_VDMA_0_BASEADDR	// Need a shorter nickname
#define FAILURE 1
#define SUCCESS 0


int renderer_init(){
	// There are 3 steps to initializing the vdma driver and IP.
	// Step 1: lookup the memory structure used to access the vdma driver.
	XAxiVdma_Config * VideoDMAConfig = XAxiVdma_LookupConfig(VDMA_ID);

	// Step 2: Initialize the memory structure and the hardware.
	if(XST_FAILURE == XAxiVdma_CfgInitialize(
			&vdmaController, VideoDMAConfig,VDMA_BASEADDR)) {
		xil_printf("VideoDMA Did not initialize.\r\n");
		return FAILURE;
	}

	// Step 3: (optional) set the frame store number.
	if(XST_FAILURE == XAxiVdma_SetFrmStore(&vdmaController, 2, XAXIVDMA_READ)){
		xil_printf("Set Frame Store Failed.");
		return FAILURE;
	}
	// Initialization is complete at this point.


	// WE WILL PROBABLY ONLY BE USING ONE FRAME.
	// Setup the frame counter. We want two read frames.
	// We don't need any write frames but the
	// function generates an error if you set the write frame count to 0.
	// We set it to 2 but ignore it because we don't need a write channel.
	XAxiVdma_FrameCounter myFrameConfig;
	myFrameConfig.ReadFrameCount = 2;
	myFrameConfig.ReadDelayTimerCount = 10;
	myFrameConfig.WriteFrameCount =2;
	myFrameConfig.WriteDelayTimerCount = 10;
	int Status = XAxiVdma_SetFrameCounter(&vdmaController, &myFrameConfig);
	if (Status != XST_SUCCESS) {
		xil_printf("Set frame counter failed %d\r\n", Status);
		return FAILURE;
		if(Status == XST_VDMA_MISMATCH_ERROR) {
			xil_printf("DMA Mismatch Error\r\n");
			return FAILURE;
		}
	}




	// Now we tell the driver about the geometry of our frame buffer.
	// Our image is 480 x 640.
	frameBuffer.VertSizeInput = 480;    // 480 vertical pixels.
	frameBuffer.HoriSizeInput = 640*4;  // 640 horizontal (32-bit pixels).
	frameBuffer.Stride = 640*4;         // Dont' worry about the rest.
	frameBuffer.FrameDelay = 0;
	frameBuffer.EnableCircularBuf=1;
	frameBuffer.EnableSync = 0;
	frameBuffer.PointNum = 0;
	frameBuffer.EnableFrameCounter = 0;
	frameBuffer.FixedFrameStoreAddr = 0;
	if(XST_FAILURE ==
			XAxiVdma_DmaConfig(&vdmaController, XAXIVDMA_READ, &frameBuffer)) {
		xil_printf("DMA Config Failed\r\n");
		return FAILURE;
	}

	// We need to give the frame buffer pointers to the memory that it will use.
	// This memory is where you will write your video data.
	// The vdma IP/driver then streams it to the HDMI IP.
	frameBuffer.FrameStoreStartAddr[0] = FRAME_BUFFER_0_ADDR;
	frameBuffer.FrameStoreStartAddr[1] = FRAME_BUFFER_0_ADDR + 4*640*480;

	if(XST_FAILURE == XAxiVdma_DmaSetBufferAddr(&vdmaController, XAXIVDMA_READ,
			frameBuffer.FrameStoreStartAddr)) {
		xil_printf("DMA Set Address Failed Failed\r\n");
	}
	// Print a sanity message if you get this far.
	xil_printf("Woohoo! I made it through initialization.\n\r");



	// This tells the HDMI controller the resolution of your display
	// (there must be a better way to do this).
	XIo_Out32(XPAR_AXI_HDMI_0_BASEADDR, 640*480);



	// Now, let's get ready to start displaying some stuff on the screen.
	// The variables framePointer and framePointer1 are just pointers to the base address
	// of frame 0 and frame 1.
	unsigned int * framePointer0 = (unsigned int *) FRAME_BUFFER_0_ADDR;
	unsigned int * framePointer1 = ((unsigned int *) FRAME_BUFFER_0_ADDR) + 640*480;
	// Just paint some large red, green, blue, and white squares in different
	// positions of the image for each frame in the buffer (framePointer0 and framePointer1).
	int row=0, col=0;
	for( row=0; row<480; row++) {
		for(col=0; col<640; col++) {
			if(row < 240) {
				if(col<320) {
					// upper left corner.
					framePointer0[row*640 + col] = 0x00000000	;  // frame 0 is red here.
					framePointer1[row*640 + col] = 0x00000000	;  // frame 1 is green here.
				} else {
					// upper right corner.
					framePointer0[row*640 + col] = 0x00000000	;  // frame 0 is blue here.
					framePointer1[row*640 + col] = 0x00000000	;  // frame 1 is red here.
				}
			} else {
				if(col<320) {
					// lower left corner.
					framePointer0[row*640 + col] = 0x00000000	;  // frame 0 is green here.
					framePointer1[row*640 + col] = 0x00000000	;  // frame 1 is white here.
				} else {
					// lower right corner.
					framePointer0[row*640 + col] = 0x00000000	;  // frame 0 is white here.
					framePointer1[row*640 + col] = 0x000000FF;  // frame 1 is blue here.
				}
			}
		}
	}




	// Start the DMA for the read channel only.
	if(XST_FAILURE == XAxiVdma_DmaStart(&vdmaController, XAXIVDMA_READ)){
		xil_printf("DMA START FAILED\r\n");
		return FAILURE;
	}

	// This is in the init function to just display the init frame on the screen
	// We have two frames, let's park on frame 0.
	int frameIndex = 0;
	// Note that you have to start the DMA process before parking on a frame.
	if (XST_FAILURE == XAxiVdma_StartParking(&vdmaController, frameIndex, XAXIVDMA_READ)) {
		xil_printf("vdma parking failed\n\r");
		return FAILURE;
	}


	return SUCCESS;
}


// We are trying to delete this function. Slowly move everything out of it
int render(){



	// Now, let's get ready to start displaying some stuff on the screen.
	// The variables framePointer and framePointer1 are just pointers to the base address
	// of frame 0 and frame 1.
	unsigned int * framePointer0 = (unsigned int *) FRAME_BUFFER_0_ADDR;
	unsigned int * framePointer1 = ((unsigned int *) FRAME_BUFFER_0_ADDR) + 640*480;
	// Just paint some large red, green, blue, and white squares in different
	// positions of the image for each frame in the buffer (framePointer0 and framePointer1).
	int row=0, col=0;
	for( row=0; row<480; row++) {
		for(col=0; col<640; col++) {
			framePointer0[row*640 + col] = row * col;

			if(row < 240) {
				if(col<320) {
					// upper left corner.
					//framePointer0[row*640 + col] = 0xFFF00000	;  // frame 0 is red here.
					framePointer1[row*640 + col] = 0x000FFF00	;  // frame 1 is green here.
				} else {
					// upper right corner.
					//    			 framePointer0[row*640 + col] = 0x000000FF	;  // frame 0 is blue here.
					framePointer1[row*640 + col] = 0xFFF00000	;  // frame 1 is red here.
				}
			} else {
				if(col<320) {
					// lower left corner.
					//    			 framePointer0[row*640 + col] = 0x000FFF00	;  // frame 0 is green here.
					framePointer1[row*640 + col] = 0xFFFFFFFF	;  // frame 1 is white here.
				} else {
					// lower right corner.
					//    			 framePointer0[row*640 + col] = 0xFFFFFFFF	;  // frame 0 is white here.
					framePointer1[row*640 + col] = 0x000000FF;  // frame 1 is blue here.
				}
			}
		}
	}

	return 0;
}
