##############################################################################
## Filename:          C:\Users\superman\Desktop\byu-ee-427-labs\PIT\MyProcessorIPLib/drivers/pit_v1_00_a/data/pit_v2_1_0.tcl
## Description:       Microprocess Driver Command (tcl)
## Date:              Wed Nov 02 16:29:39 2016 (by Create and Import Peripheral Wizard)
##############################################################################

#uses "xillib.tcl"

proc generate {drv_handle} {
  xdefine_include_file $drv_handle "xparameters.h" "pit" "NUM_INSTANCES" "DEVICE_ID" "C_BASEADDR" "C_HIGHADDR" 
}
