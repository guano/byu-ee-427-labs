# Lab 4 - Taylor Cowley and Andrew Okazaki
---
## Chapter 2: Game Console and Engine  
* Section 2.1: Game Console  
  * 2.1.a: Diligent ATLYS Board
  * 2.1.b: Xilinx Spartan-6 and Microblaze
  * 2.1.c: System Organization
* Section 2.2: Game Engine
  * 2.2.a: Game Engine (Main Game Loop)
  * 2.2.b: Meeting the Game Specifications
* Section 2.3: Application Programming Interfaces
---
#### Section 2.1: Game Console
##### 2.1.a: Diligent ATLYS Board
Xilinx's _ATLYS_ board "is a complete, ready-to-use digital circuit development platform based on a Xilinx® Spartan®-6 LX45 FPGA." [store.digilentinc.com] it has a large number of peripherals built-in, including switches, buttons, LEDs, several HDMI ports, Ethernet, sound, USB, and a VHDC connector for GPIO. It is a "legacy product"; replaced by the _Nexys Video_ product line.

##### 2.1.b: Xilinx Spartan-6 and Microblaze  
Xilinx's _Spartan-6_ FPGAs are marketed as a low-cost FPGA, "Spartan®-6 devices are the most cost-optimized FPGAs, offering industry leading connectivity features such as high logic-to-pin ratios, small form-factor packaging, and a diverse number of supported I/O protocols." [www.xilinx.com] They are therefore good for applications with low-power necessities and high-volume. The _Spartan-6_ product line is currently being replaced by Xilinx's _Artix-7_ line.

Xilinx developed a soft microprocessor core called the _MicroBlaze_ for their FPGAs. Being a soft microprocessor, it is implemented entirely in the general logic of the FPGA. Hence it can be very customizable for the specific application. It is a RISC-based architecture, and with few exceptions can issue a new instruction every cycle, maintaining single-cycle throughput most of the time.

##### 2.1.c: System Organization  
Discuss how the hardware was setup for Space Invaders.

---
#### Section 2.2: Game Engine
##### 2.2.a: Game Engine (Main Game Loop)  
Discuss how you organized the game at the top level.
##### 2.2.b: Meeting the Game Specifications  
Discuss how you met the various game specifications.

---
#### Section 2.3: Application Programming Interfaces
Discuss any existing programming interfaces that you used or created.

---
##### Timing and Memory Report
As mentioned in the Lab 4 instructions:
* You must measure how long it takes to execute one loop of your program. You can use the fixed timer on the FIT do this. Report on the shortest time it takes to execute, the longest time it takes to execute, and the average. What percentage of the CPU's capacity does your implementation of Space Invaders require? Don't count debounce delays in this time. Add this information to your report.
* Tell us how much memory your program consumes.

---
##### Bug Report
Report on any difficult bugs (in your code) or problems with the hardware and how you fixed it. You may find that as you describe the problem in prose the solution becomes obvious. We suggest that as you encounter bugs that you begin a bug report and describe the problem. Then after the problem is fixed describe how you fixed it.

