/*
 * timer.c
 *  Created on: Sep 13, 2016
 *  By Andrew Okazaki and Taylor Cowley
 */
#include "xgpio.h"          // Provides access to PB GPIO driver.
#include <stdio.h>          // xil_printf and so forth.
#include <stdint.h>			// so we don't just use int
#include "platform.h"       // Enables caching and other system stuff.
#include "mb_interface.h"   // provides the microblaze interrupt enables, etc.
#include "xintc_l.h"        // Provides  macros for the interrupt controller.
#include <stdbool.h>
#include "pit.h"

#define BUTTON_UP 0x10			// The mask values for each button
#define BUTTON_DOWN 0x4
#define BUTTON_HOUR 0x8
#define BUTTON_MIN 0x1
#define BUTTON_SEC 0x2
#define NO_BUTTONS 0			// Mask for no buttons
#define BUTTON_DEBOUNCE_TIME 1	// Only need one clock tick to debounce buttons
#define ONE_SECOND			 100// 100 ticks in a second
#define ONE_AND_HALF_SECONDS 150// One and a half seconds, in ticks.
#define HALF_SECOND			 50	// Half a second, in ticks
#define RESET 0					// When we reset a timer, it goes to zero
#define PRETTY_NUMBER 10		// For clocks, use 10 to make pretty
#define MAX_SEC 59				// Maximum possible for seconds, mins, and hours
#define MAX_MIN 59
#define MAX_HOUR 23

XGpio gpLED;  // This is a handle for the LED GPIO block.
XGpio gpPB;   // This is a handle for the push-button GPIO block.
int32_t currentButtonState = RESET;	// Start with no buttons being pushed
int32_t timerCount = RESET;			// Seconds timer not running
int32_t debounce_timer_up = RESET;	// Timers for debouncing the up and
int32_t debounce_timer_down = RESET;// down buttons

int32_t hours = PRETTY_NUMBER;		// Variables to store the time
int32_t minutes = PRETTY_NUMBER;	// Start the time at
int32_t seconds = PRETTY_NUMBER;	// nice friendly numbers

// We call this once a second to update the time
void evaluate();

// This is invoked in response to a timer interrupt.
// It does 2 things: 1) debounce switches, and 2) advances the time.
void timer_interrupt_handler() {
	if(currentButtonState == NO_BUTTONS){	// Only tick if no pushed buttons
		timerCount++;
		if(timerCount >= ONE_SECOND){		// Wait a second
			timerCount = RESET;				// Reset the timer
			seconds++;						// Tick seconds
			evaluate();						// Fix the time
		}
	}else{
		// The hour button is being pushed
		if(currentButtonState & BUTTON_HOUR){
			// The up button is being pushed
			if(currentButtonState & BUTTON_UP){
				debounce_timer_up++;		// increase time held
				if(debounce_timer_up == BUTTON_DEBOUNCE_TIME){		// READY
					hours++;				// increase hours
					evaluate();				// and fix time
				// For every half second over one and a half seconds
				}else if(debounce_timer_up % HALF_SECOND == 0
						&& debounce_timer_up > ONE_AND_HALF_SECONDS){
					hours++;				// we also increase
					evaluate();				// and fix the time
				}
			}else{debounce_timer_up =RESET;}// Up is not pushed, reset debounce
			//The down button is being pushed
			if(currentButtonState & BUTTON_DOWN){
				debounce_timer_down++;		// Increase the time held
				if(debounce_timer_down == BUTTON_DEBOUNCE_TIME){	// READY
					hours--;				// decrease hours
					evaluate();				// And fix time
				// For every half second over one and a half seconds
				}else if(debounce_timer_down % HALF_SECOND == 0
						&& debounce_timer_down > ONE_AND_HALF_SECONDS){
					hours--;				// decrease and fix time
					evaluate();
				}
			}else{debounce_timer_down=RESET;}// Not being held, reset held timer
		}
		// The minute button is being pushed
		if(currentButtonState & BUTTON_MIN){
			// The up button is being pushed
			if(currentButtonState & BUTTON_UP){
				debounce_timer_up++;		// Increase held time
				if(debounce_timer_up == BUTTON_DEBOUNCE_TIME){		// READY
					minutes++;				// increase minutes and fix
					evaluate();
				// For every half second over one and a half seconds
				}else if(debounce_timer_up % HALF_SECOND == 0
						&& debounce_timer_up > ONE_AND_HALF_SECONDS){
					minutes++;				// Also tick and fix
					evaluate();
				}
			}else{debounce_timer_up =RESET;}// Not being pushed; reset timer
			// The down button is being pushed
			if(currentButtonState & BUTTON_DOWN){
				debounce_timer_down++;		// Increase time pressed
				if(debounce_timer_down == BUTTON_DEBOUNCE_TIME){	// READY
					minutes--;				// decrease minutes
					evaluate();
				// For every half second over one and a half seconds
				}else if(debounce_timer_down % HALF_SECOND == 0
						&& debounce_timer_down > ONE_AND_HALF_SECONDS){
					minutes--;				// Tick and fix
					evaluate();
				}
			}else{debounce_timer_down=RESET;}// Not being held, reset timer
		}
		// The second button is being pushed
		if(currentButtonState & BUTTON_SEC){
			// The up button is being pushed
			if(currentButtonState & BUTTON_UP){
				debounce_timer_up++;		// Increase held timer
				if(debounce_timer_up == BUTTON_DEBOUNCE_TIME){		// READY
					seconds++;				// Tick and fix
					evaluate();
				// For every half second over one and a half seconds
				}else if(debounce_timer_up % HALF_SECOND == 0
						&& debounce_timer_up > ONE_AND_HALF_SECONDS){
					seconds++;				// Tick and fix
					evaluate();
				}
			}else{debounce_timer_up =RESET;}// Not being held, reset timer
			// The down button is being pushed
			if(currentButtonState & BUTTON_DOWN){
				debounce_timer_down++;		// Increase held timer
				if(debounce_timer_down == BUTTON_DEBOUNCE_TIME){	// READY
					seconds--;				// tick and fix
					evaluate();
				// For every half second over one and a half seconds
				}else if(debounce_timer_down % HALF_SECOND == 0
						&& debounce_timer_down > ONE_AND_HALF_SECONDS){
					seconds--;				// tick and fix
					evaluate();
				}
			}else{debounce_timer_down=RESET;}// Not being held, reset held timer
		}
	}
}

// This updates our time variables to make time sense
// This is also what displays the time
void evaluate(){
	// These if statements make the time go up
	if(seconds > MAX_SEC){	// Seconds are between 0 and 59
		seconds = RESET;
		minutes++;	// new minute!
	}
	if(minutes > MAX_MIN){		// Minutes are between 0 and 59
		minutes = RESET;
		hours++;	// new hour!
	}
	if(hours > MAX_HOUR){		// Hours are between 0 and 23
		hours = RESET;
	}

	// These if statements make the time go down
	if(seconds < 0){		// Can't have negative seconds
		seconds = MAX_SEC;
		minutes--;	// Subtract a minute
	}
	if(minutes < 0){		// Can't have negative minutes
		minutes = MAX_MIN;
		hours--;	// Subtract an hour
	}
	if(hours < 0){		// Can't have negative hours
		hours = MAX_HOUR;
	}
	// Prints the time. We only use a carriage return so we can overwrite it
	xil_printf("\e[104m %02d:%02d:%02d \e[49m\r", hours, minutes, seconds);
}

// This is invoked each time there is a change in the button state
// (result of a push or a bounce).
void pb_interrupt_handler() {
	// Clear the GPIO interrupt.
	XGpio_InterruptGlobalDisable(&gpPB);                // Off PB interrupts now
	currentButtonState = XGpio_DiscreteRead(&gpPB, 1);  // Get state of buttons.

	// This was all that was necessary. Just update the button state
	XGpio_InterruptClear(&gpPB, 0xFFFFFFFF);            // Ack the PB interrupt.
	XGpio_InterruptGlobalEnable(&gpPB);                 // Enable PB interrupts.
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
	if (intc_status & XPAR_PIT_0_MYINTERRUPT_MASK){
		XIntc_AckIntr(XPAR_INTC_0_BASEADDR, XPAR_PIT_0_MYINTERRUPT_MASK);
		timer_interrupt_handler();	// It was a timer interrupt! call that fn
	}
	// Check the push buttons.
	if (intc_status & XPAR_PUSH_BUTTONS_5BITS_IP2INTC_IRPT_MASK){
		pb_interrupt_handler();		// It was a button interrupt!
		XIntc_AckIntr(XPAR_INTC_0_BASEADDR, // Acknowledge the interrupt
				XPAR_PUSH_BUTTONS_5BITS_IP2INTC_IRPT_MASK);
	}
}


void init_pit(){
	PIT_mWriteSlaveReg0 (XPAR_PIT_0_BASEADDR, 0, 100000000);
	PIT_mWriteSlaveReg0 (XPAR_PIT_0_BASEADDR, 4*1, 1000000);	// 100x a second
    PIT_mWriteSlaveReg0 (XPAR_PIT_0_BASEADDR, 2*4, 0x00000007);	// writing stuff to the control register - allowing decrement

}

int main (void) {
	init_platform();
	init_pit();
	// Initialize the GPIO peripherals.
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
			(XPAR_PIT_0_MYINTERRUPT_MASK |
					XPAR_PUSH_BUTTONS_5BITS_IP2INTC_IRPT_MASK));
	// Master the enable
	XIntc_MasterEnable(XPAR_INTC_0_BASEADDR);
	// And enable again
	microblaze_enable_interrupts();

	while(1){


		// Now we wait for input. You can input 0-9, with varying speeds for the
		// clock depending on the number
		char input = getchar();
		switch (input){
		case '1':
			PIT_mWriteSlaveReg0 (XPAR_PIT_0_BASEADDR, 4*1, 10000000);	// 10x a second
			break;
		case '2':
			PIT_mWriteSlaveReg0 (XPAR_PIT_0_BASEADDR, 4*1, 5000000);	// 50x a second
			break;
		case '3':
			PIT_mWriteSlaveReg0 (XPAR_PIT_0_BASEADDR, 4*1, 7500000);	// 75x a second
			break;
		case '4':
			PIT_mWriteSlaveReg0 (XPAR_PIT_0_BASEADDR, 4*1, 1000000); // 1x speed
			break;
		case '5':
			PIT_mWriteSlaveReg0 (XPAR_PIT_0_BASEADDR, 4*1, 750000);	// 1.5 speed
			break;
		case '6':
			PIT_mWriteSlaveReg0 (XPAR_PIT_0_BASEADDR, 4*1, 500000);	// 2x speed
			break;
		case '7':
			PIT_mWriteSlaveReg0 (XPAR_PIT_0_BASEADDR, 4*1, 250000);	// 4x speed
			break;
		case '8':
			PIT_mWriteSlaveReg0 (XPAR_PIT_0_BASEADDR, 4*1, 100000);	// 10x speed
			break;
		case '9':
			PIT_mWriteSlaveReg0 (XPAR_PIT_0_BASEADDR, 4*1, 10000);	// LUDICROUS SPEED
			break;
		case '0':
			PIT_mWriteSlaveReg0 (XPAR_PIT_0_BASEADDR, 4*1, 1000000);	// 100x a second NORMAL SPEED
			break;
		}


	}  // Program never ends.



	cleanup_platform();
	return 0;
}
