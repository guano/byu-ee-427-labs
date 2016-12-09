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
#include <stdint.h>
#include <xgpio.h>          	// Provides access to PB GPIO driver.
#include <stdlib.h>
#include <xaxivdma.h>
#include <xio.h>
#include <unistd.h>
#include "dma.h"
#include "platform.h"
#include "xparameters.h"
#include "mb_interface.h"   	// provides the microblaze interrupt enables, etc.
#include "xintc_l.h"        	// Provides handy macros for the interrupt controller.
#include "time.h"


void timer_interrupt_handler() {
//	xil_printf("timer interrupt\r\n");
}

void dma_interrupt_handler() {
	xil_printf("DMA interrupt\r\n");
}

void interrupt_handler_dispatcher(void* ptr) {
	int intc_status = XIntc_GetIntrStatus(XPAR_INTC_0_BASEADDR);
	// Check the pit interrupt first.
	if (intc_status & XPAR_FIT_TIMER_0_INTERRUPT_MASK){
		XIntc_AckIntr(XPAR_INTC_0_BASEADDR, XPAR_FIT_TIMER_0_INTERRUPT_MASK);
//		xil_printf("pit\n\r");
		timer_interrupt_handler();
	}
	if (intc_status & XPAR_DMA_0_DMA_INTERRUPT_MASK){
		XIntc_AckIntr(XPAR_INTC_0_BASEADDR, XPAR_DMA_0_DMA_COMPLETE_INTERRUPT_MASK);
//		xil_printf("pit\n\r");
		dma_interrupt_handler();
	}

}

void print(char *str);

int main()
{
	//Set the source array of random values
	int source_arr[] = {0x12345678, 0xDEADBEEF, 0x09876543, 0xBEEFB00B, 0xBEEF1234};
	// set the destination array to be empty
	int dest_arr[] = {0x0, 0x0, 0x0, 0x0, 0x0};
	//init the platform
	init_platform();

	microblaze_register_handler(interrupt_handler_dispatcher, NULL);
	// Enable interrupts for pit, Push Buttons, and FIFO on the AC97 XPAR_FIT_TIMER_0_INTERRUPT_MASK |
	XIntc_EnableIntr(XPAR_INTC_0_BASEADDR,(XPAR_DMA_0_DMA_COMPLETE_INTERRUPT_MASK));
	XIntc_MasterEnable(XPAR_INTC_0_BASEADDR);
	microblaze_enable_interrupts();

	cleanup_platform();
	xil_printf("Printing value before DMA transfer.\n\r");
	int i;
	int length = 6; // in words
	for(i = 0; i < length; i++){
		xil_printf("%x\r\n", dest_arr[i]);
	}

	DMA_CONTROLLER_TranseferInitialize(XPAR_DMA_0_BASEADDR, (Xuint32) source_arr, (Xuint32) dest_arr, (Xuint32) (length-1)*4);
	DMA_CONTROLLER_TransferGo(XPAR_DMA_0_BASEADDR);

	//print the destination array after transfer
	xil_printf("Printing value after DMA burst transfer.\n\r");
	for(i = 0; i < length; i++){
		xil_printf("%x\r\n", dest_arr[i]);
	}
	cleanup_platform();
    return 0;
}












