/*****************************************************************************
* Filename:          C:\Users\superman\Desktop\byu-ee-427-labs\SCORE_DISPLAY_2\MyProcessorIPLib/drivers/score_display_2_v1_00_a/src/score_display_2_selftest.c
* Version:           1.00.a
* Description:       Contains a diagnostic self-test function for the score_display_2 driver
* Date:              Tue Nov 15 17:51:08 2016 (by Create and Import Peripheral Wizard)
*****************************************************************************/


/***************************** Include Files *******************************/

#include "score_display_2.h"
#include "stdio.h"
#include "xio.h"
#include "xparameters.h"


/************************** Constant Definitions ***************************/

#define READ_WRITE_MUL_FACTOR 0x10

/************************** Variable Definitions ****************************/


/************************** Function Definitions ***************************/

/**
 *
 * Run a self-test on the driver/device. Note this may be a destructive test if
 * resets of the device are performed.
 *
 * If the hardware system is not built correctly, this function may never
 * return to the caller.
 *
 * @param   baseaddr_p is the base address of the SCORE_DISPLAY_2 instance to be worked on.
 *
 * @return
 *
 *    - XST_SUCCESS   if all self-test code passed
 *    - XST_FAILURE   if any self-test code failed
 *
 * @note    Caching must be turned off for this function to work.
 * @note    Self test may fail if data memory and device are not on the same bus.
 *
 */
XStatus SCORE_DISPLAY_2_SelfTest(void * baseaddr_p)
{
  Xuint32 baseaddr;
  int write_loop_index;
  int read_loop_index;
  int Index;
  
  /*
   * Check and get the device address
   */
  /*
   * Base Address maybe 0. Up to developer to uncomment line below.
  XASSERT_NONVOID(baseaddr_p != XNULL);
   */
  baseaddr = (Xuint32) baseaddr_p;

  xil_printf("******************************\n\r");
  xil_printf("* User Peripheral Self Test\n\r");
  xil_printf("******************************\n\n\r");

  /*
   * Write to user logic slave module register(s) and read back
   */
  xil_printf("User logic slave module test...\n\r");

  for (write_loop_index = 0 ; write_loop_index < SCORE_DISPLAY_2_USER_NUM_REG; write_loop_index++)
    SCORE_DISPLAY_2_mWriteSlaveReg0 (baseaddr, write_loop_index*4, (write_loop_index+1)*READ_WRITE_MUL_FACTOR);
  for (read_loop_index = 0 ; read_loop_index < SCORE_DISPLAY_2_USER_NUM_REG; read_loop_index++)
    if ( SCORE_DISPLAY_2_mReadSlaveReg0 (baseaddr, read_loop_index*4) != (read_loop_index+1)*READ_WRITE_MUL_FACTOR){
      xil_printf ("Error reading register value at address %x", (int)baseaddr + read_loop_index*4);
      return XST_FAILURE;
    }

  xil_printf("   - slave register write/read passed\n\n\r");

  /* Read the registers at the base address to ensure that this is indeed working */
  if ( (SCORE_DISPLAY_2_mReadSlaveReg0 (baseaddr, 0)) != 0x0){
    xil_printf("   - soft reset failed\n\n\r");
    return XST_FAILURE;
  }

  xil_printf("   - soft reset passed\n\n\r");

  return XST_SUCCESS;
}


int main(){
	xil_printf("Attempting a test of our hardware\n\r");
	SCORE_DISPLAY_2_SelfTest(XPAR_SCORE_DISPLAY_2_0_BASEADDR);

	xil_printf("now doing other tests >:)\n\r");
	SCORE_DISPLAY_2_mWriteSlaveReg0(XPAR_SCORE_DISPLAY_2_0_BASEADDR, 0, 9);
	SCORE_DISPLAY_2_mWriteSlaveReg1(XPAR_SCORE_DISPLAY_2_0_BASEADDR, 0, 9);
	SCORE_DISPLAY_2_mWriteSlaveReg2(XPAR_SCORE_DISPLAY_2_0_BASEADDR, 0, 9);
	SCORE_DISPLAY_2_mWriteSlaveReg3(XPAR_SCORE_DISPLAY_2_0_BASEADDR, 0, 9);
	SCORE_DISPLAY_2_mWriteSlaveReg4(XPAR_SCORE_DISPLAY_2_0_BASEADDR, 0, 9);
	SCORE_DISPLAY_2_mWriteSlaveReg5(XPAR_SCORE_DISPLAY_2_0_BASEADDR, 0, 9);

	int i = 0;
	while(1){
		char input = getchar();
		if (input == '0')
			i--;
		else if(input == '1')
			i++;
		else if(input == '2')
			i = i<<1;
		else if(input == '3')
			i = i>>1;
		else if(input == '7')
			SCORE_DISPLAY_2_mWriteSlaveReg5(XPAR_SCORE_DISPLAY_2_0_BASEADDR, 0, i);
		else if(input == '8')
			SCORE_DISPLAY_2_mWriteSlaveReg4(XPAR_SCORE_DISPLAY_2_0_BASEADDR, 0, i);
		else if(input == '9')
			SCORE_DISPLAY_2_mWriteSlaveReg3(XPAR_SCORE_DISPLAY_2_0_BASEADDR, 0, i);
		else if(input == '4')
			SCORE_DISPLAY_2_mWriteSlaveReg2(XPAR_SCORE_DISPLAY_2_0_BASEADDR, 0, i);
		else if(input == '5')
			SCORE_DISPLAY_2_mWriteSlaveReg1(XPAR_SCORE_DISPLAY_2_0_BASEADDR, 0, i);
		else if(input == '6')
			SCORE_DISPLAY_2_mWriteSlaveReg0(XPAR_SCORE_DISPLAY_2_0_BASEADDR, 0, i);
		else
			xil_printf("only supports input of 1 and o\n\r");


//		long y;
//		for(y = 0;y<4000000;y++);	// long delay
		xil_printf("changing the thing: %x\n\r", i);
//		i = (i>=32)? 0 : i+1 ;
	}

	return 0;
}

