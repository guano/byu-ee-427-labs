##############################################################################
## Filename:          C:\Users\superman\Desktop\lab8\SpaceInvadersHW_Pit_fit/drivers/dma_v1_00_a/data/dma_v2_1_0.tcl
## Description:       Microprocess Driver Command (tcl)
## Date:              Wed Dec 07 14:57:16 2016 (by Create and Import Peripheral Wizard)
##############################################################################

#uses "xillib.tcl"

proc generate {drv_handle} {
  xdefine_include_file $drv_handle "xparameters.h" "dma" "NUM_INSTANCES" "DEVICE_ID" "C_BASEADDR" "C_HIGHADDR" 
}
