/*
 * helloworld.c: simple test application
 * Currently used to test lab 3 for Space Invaders.
 * Taylor Cowley and Andrew Okazaki
 */

#include <stdio.h>
#include <stdint.h>
#include "platform.h"
#include "xparameters.h"
#include "xaxivdma.h"
#include "xio.h"
#include "time.h"
#include "unistd.h"
#include "tank.h"
#include "interface.h"
#include "aliens.h"
#include "bunkers.h"
#include "mother_ship.h"
#include "util.h"

#include "xgpio.h"
#include "mb_interface.h"
#include "xintc_l.h"

#define DEBUG

#define SCREEN_RES_X 640	// Our screen resolution is 640 * 480
#define SCREEN_RES_Y 480	// Our screen resolution is 640 * 480
#define BLACK 0x00000000	// Hex value for black
#define BLUE  0x2222FF

#define ONE_SECOND	100		// 100 ticks in a second
#define HALF_SECOND 50		// 50 ticks in half a second
#define QUARTER_SECOND 25
#define EIGHTH_SECOND 12
#define TENTH_SECOND 10
#define TWENTIETH_SECOND 5
#define SUPER_FAST 2

#define MOTHER_SHIP_SPEED TENTH_SECOND
#define MOTHER_SHIP_SPAWN_CONSTANT 1000
#define ALIEN_SHOT_SPAWN_CONSTANT 100
#define ALIEN_MOVE_SPEED HALF_SECOND



#define BUTTON_UP		0x10
#define BUTTON_DOWN		0x4
#define BUTTON_LEFT		0x8
#define BUTTON_RIGHT	0x2
#define BUTTON_CENTER	0x1

void print(char *str);



#define FRAME_BUFFER_0_ADDR 0xC1000000  // Starting location in DDR where we will store the images that we display.

//-----------------------------
void timer_interrupt_handler();
void pb_interrupt_handler();
void interttupt_handler_dispatcher();
//-----------------------------


XGpio gpLED;  // This is a handle for the LED GPIO block.
XGpio gpPB;   // This is a handle for the push-button GPIO block.
uint32_t* framePointer0 = (uint32_t*) FRAME_BUFFER_0_ADDR;
int32_t currentButtonState;		// Current button being pressed
int32_t mother_ship_points;


void timer_interrupt_handler(){
	static uint32_t timerCount;
	static uint32_t mother_ship_move_counter;
	tank_update_bullet(framePointer0);	// update all bullets
	aliens_update_bullets(framePointer0);	// update all bullets

	timerCount++;
	mother_ship_move_counter++;
	mother_ship_points++;

	int32_t r = rand();
	if(r%ALIEN_SHOT_SPAWN_CONSTANT == 0){
		alien_missle(framePointer0);	// Make the aliens fire
	}
	if(r%MOTHER_SHIP_SPAWN_CONSTANT == 0){
		mother_ship_spawn();
	}
	if(mother_ship_move_counter >= MOTHER_SHIP_SPEED){
		mother_ship_move_counter = 0;
		mother_ship_move();

	}
	if(mother_ship_points > TENTH_SECOND){
		mother_ship_points = 0;
		mother_ship_points_blink();
	}
	if(timerCount >= 5 /*ALIEN_MOVE_SPEED*/){
		timerCount = 0;
		aliens_move(framePointer0);	// move the aliens
	}




	// Now to check the buttons.
	if(currentButtonState & BUTTON_LEFT){
		tank_move_left(framePointer0);
	}
	if(currentButtonState & BUTTON_RIGHT){
		tank_move_right(framePointer0);
	}
	if(currentButtonState & BUTTON_CENTER){
		tank_fire(framePointer0);
	}
	if(currentButtonState & BUTTON_UP){
	}
}
void pb_interrupt_handler(){
	XGpio_InterruptGlobalDisable(&gpPB);	// Can't be interrupted by buttons
	xil_printf("Button Interrupt\n\r");
	currentButtonState = XGpio_DiscreteRead(&gpPB, 1);
	//xil_printf("Button state retrieved: %x\n\r", currentButtonState);
	/*switch(currentButtonState){
	case BUTTON_LEFT:
		tank_move_left(framePointer0);		// move the tank left
		break;
	case BUTTON_RIGHT:
		tank_move_right(framePointer0);		// move the tank right
		break;
	case BUTTON_UP:
		xil_printf("volume goes up\n\r");

		break;
	case BUTTON_DOWN:
		xil_printf("volume goes down\n\r");

		break;
	case BUTTON_CENTER:
		tank_fire(framePointer0);		// Make the tank fire
		break;
	default:
		xil_printf("Button pushed???\n\r");
		break;
	}*/



	// Time to clear the interrupt and reenable GPIO interrupts
	XGpio_InterruptClear(&gpPB, 0xFFFFFFFF);
	XGpio_InterruptGlobalEnable(&gpPB);
}

// Main interrupt handler, queries interrupt controller to see what peripheral
// fired the interrupt and then dispatches the corresponding interrupt handler.
// This routine acks the interrupt at the controller level but the peripheral
// interrupt must be ack'd by the dispatched interrupt handler.
// Question: Why is timer_interrupt_handler() called after ack'ing controller
// but pb_interrupt_handler() is called before ack'ing the interrupt controller?
void interrupt_handler_dispatcher(void* ptr) {
	int intc_status = XIntc_GetIntrStatus(XPAR_INTC_0_BASEADDR);
	// Check the FIT interrupt first.
	if (intc_status & XPAR_FIT_TIMER_0_INTERRUPT_MASK){
		XIntc_AckIntr(XPAR_INTC_0_BASEADDR, XPAR_FIT_TIMER_0_INTERRUPT_MASK);
		timer_interrupt_handler();	// It was a timer interrupt! call that fn
	}
	// Check the push buttons.
	if (intc_status & XPAR_PUSH_BUTTONS_5BITS_IP2INTC_IRPT_MASK){
		pb_interrupt_handler();		// It was a button interrupt!
		XIntc_AckIntr(XPAR_INTC_0_BASEADDR, // Acknowledge the interrupt
				XPAR_PUSH_BUTTONS_5BITS_IP2INTC_IRPT_MASK);
	}
}


void init_interrupts(void){
	int32_t success;
	print("\n\rHello . Let's have a fun \e[31m\e[1mtime \e[21m\e[0m\n\r");
	success = XGpio_Initialize(&gpPB, XPAR_PUSH_BUTTONS_5BITS_DEVICE_ID);
	// Set the push button peripheral to be inputs.
	XGpio_SetDataDirection(&gpPB, 1, 0x0000001F);
	// Enable the global GPIO interrupt for push buttons.
	XGpio_InterruptGlobalEnable(&gpPB);
	// Enable all interrupts in the push button peripheral.
	XGpio_InterruptEnable(&gpPB, 0xFFFFFFFF);
	// Register the interrupt handler
	microblaze_register_handler(interrupt_handler_dispatcher, NULL);
	// And enable interrupts
	XIntc_EnableIntr(XPAR_INTC_0_BASEADDR,
			(XPAR_FIT_TIMER_0_INTERRUPT_MASK |
					XPAR_PUSH_BUTTONS_5BITS_IP2INTC_IRPT_MASK));
	// Master the enable
	XIntc_MasterEnable(XPAR_INTC_0_BASEADDR);
	// And enable again
	microblaze_enable_interrupts();
}



int main() {
	init_platform();                   // Necessary for all programs.
	init_interrupts();
	int Status;                        // Keep track of success/failure of system function calls.
	XAxiVdma videoDMAController;
	// There are 3 steps to initializing the vdma driver and IP.
	// Step 1: lookup the memory structure that is used to access the vdma driver.
	XAxiVdma_Config * VideoDMAConfig = XAxiVdma_LookupConfig(XPAR_AXI_VDMA_0_DEVICE_ID);
	// Step 2: Initialize the memory structure and the hardware.
	if(XST_FAILURE == XAxiVdma_CfgInitialize(&videoDMAController, VideoDMAConfig,	XPAR_AXI_VDMA_0_BASEADDR)) {
		xil_printf("VideoDMA Did not initialize.\r\n");
	}
	// Step 3: (optional) set the frame store number.
	if(XST_FAILURE ==  XAxiVdma_SetFrmStore(&videoDMAController, 2, XAXIVDMA_READ)) {
		xil_printf("Set Frame Store Failed.");
	}
	// Initialization is complete at this point.

	// Setup the frame counter. We want two read frames. We don't need any write frames but the
	// function generates an error if you set the write frame count to 0. We set it to 2
	// but ignore it because we don't need a write channel at all.
	XAxiVdma_FrameCounter myFrameConfig;
	myFrameConfig.ReadFrameCount = 2;
	myFrameConfig.ReadDelayTimerCount = 10;
	myFrameConfig.WriteFrameCount =2;
	myFrameConfig.WriteDelayTimerCount = 10;
	Status = XAxiVdma_SetFrameCounter(&videoDMAController, &myFrameConfig);
	if (Status != XST_SUCCESS) {
		xil_printf("Set frame counter failed %d\r\n", Status);
		if(Status == XST_VDMA_MISMATCH_ERROR)
			xil_printf("DMA Mismatch Error\r\n");
	}
	// Now we tell the driver about the geometry of our frame buffer and a few other things.
	// Our image is 480 x 640.
	XAxiVdma_DmaSetup myFrameBuffer;
	myFrameBuffer.VertSizeInput = 480;    // 480 vertical pixels.
	myFrameBuffer.HoriSizeInput = 640*4;  // 640 horizontal (32-bit pixels).
	myFrameBuffer.Stride = 640*4;         // Dont' worry about the rest of the values.
	myFrameBuffer.FrameDelay = 0;
	myFrameBuffer.EnableCircularBuf=1;
	myFrameBuffer.EnableSync = 0;
	myFrameBuffer.PointNum = 0;
	myFrameBuffer.EnableFrameCounter = 0;
	myFrameBuffer.FixedFrameStoreAddr = 0;
	if(XST_FAILURE == XAxiVdma_DmaConfig(&videoDMAController, XAXIVDMA_READ, &myFrameBuffer)) {
		xil_printf("DMA Config Failed\r\n");
	}
	// We need to give the frame buffer pointers to the memory that it will use. This memory
	// is where you will write your video data. The vdma IP/driver then streams it to the HDMI
	// IP.
	myFrameBuffer.FrameStoreStartAddr[0] = FRAME_BUFFER_0_ADDR;
	myFrameBuffer.FrameStoreStartAddr[1] = FRAME_BUFFER_0_ADDR + 4*640*480;

	if(XST_FAILURE == XAxiVdma_DmaSetBufferAddr(&videoDMAController, XAXIVDMA_READ,
			myFrameBuffer.FrameStoreStartAddr)) {
		xil_printf("DMA Set Address Failed Failed\r\n");
	}
	// Print a sanity message if you get this far.
	xil_printf("Woohoo! I made it through initialization.\n\r");
	// Now, let's get ready to start displaying some stuff on the screen.
	// The variables framePointer and framePointer1 are just pointers to the base address
	// of frame 0 and frame 1.
	uint32_t* framePointer0 = (uint32_t*) FRAME_BUFFER_0_ADDR;
	// Just paint some large red, green, blue, and white squares in different
	// positions of the image for each frame in the buffer (framePointer0 and framePointer1).
	int row=0, col=0;
	for( row=0; row<SCREEN_RES_Y; row++) {
		for(col=0; col<SCREEN_RES_X; col++) {
			framePointer0[row*SCREEN_RES_X + col] = BLACK;
		}
	}

	bunkers_init(framePointer0);



	tank_init();							// initialize the tank
	tank_draw(framePointer0, false);		// draw the tank
	interface_init_board(framePointer0);	// draw the tanks at the top
	aliens_init(framePointer0);				// initialize aliens
	mother_ship_init(framePointer0);




	// This tells the HDMI controller the resolution of your display (there must be a better way to do this).
	XIo_Out32(XPAR_AXI_HDMI_0_BASEADDR, 640*480);

	// Start the DMA for the read channel only.
	if(XST_FAILURE == XAxiVdma_DmaStart(&videoDMAController, XAXIVDMA_READ)){
		xil_printf("DMA START FAILED\r\n");
	}
	int frameIndex = 0;
	// We have two frames, let's park on frame 0. Use frameIndex to index them.
	// Note that you have to start the DMA process before parking on a frame.

	if (XST_FAILURE == XAxiVdma_StartParking(&videoDMAController, frameIndex,  XAXIVDMA_READ)) {
		xil_printf("vdma parking failed\n\r");
	}
	char input;
	srand((unsigned)time( NULL ));


	xil_printf("Are we getting here?\n\r");
	while(1){
		//aliens_move(framePointer0);	// move the aliens
		tank_update_bullet(framePointer0);	// update all bullets
		aliens_update_bullets(framePointer0);	// update all bullets
		//interface_increment_score(framePointer0,0);
		input = getchar();
		switch(input){
		case '4':
			tank_move_left(framePointer0);		// move the tank left
			break;
		case '6':
			tank_move_right(framePointer0);		// move the tank right
			break;
		case '8':
			mother_ship_spawn();
			break;
		case '2':
			interface_kill_tank();
			interface_increment_score(1);
			//aliens_kill(framePointer0);	// Kill an alien
			break;
		case '5':
			tank_fire(framePointer0);		// Make the tank fire
			break;
		case '3':
			alien_missle(framePointer0);	// Make the aliens fire
			break;
		case'9':
			mother_ship_move();
			break;
		case '7':
			break;
		}
	}


	cleanup_platform();

	return 0;
}
