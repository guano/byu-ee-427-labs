##############################################################################
## Filename:          C:\Users\superman\Desktop\byu-ee-427-labs\SCORE_DISPLAY_2\MyProcessorIPLib/drivers/score_display_2_v1_00_a/data/score_display_2_v2_1_0.tcl
## Description:       Microprocess Driver Command (tcl)
## Date:              Tue Nov 15 17:51:08 2016 (by Create and Import Peripheral Wizard)
##############################################################################

#uses "xillib.tcl"

proc generate {drv_handle} {
  xdefine_include_file $drv_handle "xparameters.h" "score_display_2" "NUM_INSTANCES" "DEVICE_ID" "C_BASEADDR" "C_HIGHADDR" 
}
