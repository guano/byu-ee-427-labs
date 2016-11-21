#  Simulation Model Generator
#  Xilinx EDK 13.4 EDK_O.87xd
#  Copyright (c) 1995-2011 Xilinx, Inc.  All rights reserved.
#
#  File     score_display_2_0_wave.tcl (Thu Nov 17 12:49:26 2016)
#
#  Module   score_display_2_0_wrapper
#  Instance score_display_2_0
if { [info exists PathSeparator] } { set ps $PathSeparator } else { set ps "/" }
if { ![info exists tbpath] } { set tbpath "${ps}system_tb${ps}dut" }

  wave add $tbpath${ps}score_display_2_0${ps}digit_0 -into $id
  wave add $tbpath${ps}score_display_2_0${ps}digit_1 -into $id
  wave add $tbpath${ps}score_display_2_0${ps}digit_2 -into $id
  wave add $tbpath${ps}score_display_2_0${ps}digit_3 -into $id
  wave add $tbpath${ps}score_display_2_0${ps}digit_4 -into $id
  wave add $tbpath${ps}score_display_2_0${ps}digit_5 -into $id
  wave add $tbpath${ps}score_display_2_0${ps}seg_a -into $id
  wave add $tbpath${ps}score_display_2_0${ps}seg_b -into $id
  wave add $tbpath${ps}score_display_2_0${ps}seg_c -into $id
  wave add $tbpath${ps}score_display_2_0${ps}seg_d -into $id
  wave add $tbpath${ps}score_display_2_0${ps}seg_e -into $id
  wave add $tbpath${ps}score_display_2_0${ps}seg_f -into $id
  wave add $tbpath${ps}score_display_2_0${ps}seg_g -into $id
# wave add $tbpath${ps}score_display_2_0${ps}S_AXI_ACLK -into $id
# wave add $tbpath${ps}score_display_2_0${ps}S_AXI_ARESETN -into $id
# wave add $tbpath${ps}score_display_2_0${ps}S_AXI_AWADDR -into $id
# wave add $tbpath${ps}score_display_2_0${ps}S_AXI_AWVALID -into $id
# wave add $tbpath${ps}score_display_2_0${ps}S_AXI_WDATA -into $id
# wave add $tbpath${ps}score_display_2_0${ps}S_AXI_WSTRB -into $id
# wave add $tbpath${ps}score_display_2_0${ps}S_AXI_WVALID -into $id
# wave add $tbpath${ps}score_display_2_0${ps}S_AXI_BREADY -into $id
# wave add $tbpath${ps}score_display_2_0${ps}S_AXI_ARADDR -into $id
# wave add $tbpath${ps}score_display_2_0${ps}S_AXI_ARVALID -into $id
# wave add $tbpath${ps}score_display_2_0${ps}S_AXI_RREADY -into $id
  wave add $tbpath${ps}score_display_2_0${ps}S_AXI_ARREADY -into $id
  wave add $tbpath${ps}score_display_2_0${ps}S_AXI_RDATA -into $id
  wave add $tbpath${ps}score_display_2_0${ps}S_AXI_RRESP -into $id
  wave add $tbpath${ps}score_display_2_0${ps}S_AXI_RVALID -into $id
  wave add $tbpath${ps}score_display_2_0${ps}S_AXI_WREADY -into $id
  wave add $tbpath${ps}score_display_2_0${ps}S_AXI_BRESP -into $id
  wave add $tbpath${ps}score_display_2_0${ps}S_AXI_BVALID -into $id
  wave add $tbpath${ps}score_display_2_0${ps}S_AXI_AWREADY -into $id

