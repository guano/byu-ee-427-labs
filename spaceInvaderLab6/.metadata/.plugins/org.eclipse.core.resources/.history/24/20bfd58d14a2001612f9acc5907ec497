################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include src/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
ELFSIZE += \
pit_timer.elf.size \

ELFCHECK += \
pit_timer.elf.elfcheck \


# All Target
all: pit_timer.elf secondary-outputs

# Tool invocations
pit_timer.elf: $(OBJS) ../src/lscript.ld $(USER_OBJS)
	@echo Building target: $@
	@echo Invoking: MicroBlaze gcc linker
	mb-gcc -Wl,-T -Wl,../src/lscript.ld -L../../pit_timer_bsp/microblaze_0/lib -mlittle-endian -mxl-barrel-shift -mxl-pattern-compare -mcpu=v8.20.b -mno-xl-soft-mul -o"pit_timer.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo Finished building target: $@
	@echo ' '

pit_timer.elf.size: pit_timer.elf
	@echo Invoking: MicroBlaze Print Size
	mb-size pit_timer.elf  |tee "pit_timer.elf.size"
	@echo Finished building: $@
	@echo ' '

pit_timer.elf.elfcheck: pit_timer.elf
	@echo Invoking: Xilinx ELF Check
	elfcheck pit_timer.elf -hw ../../SpaceInvadersHW_hw_platform/system.xml -pe microblaze_0  |tee "pit_timer.elf.elfcheck"
	@echo Finished building: $@
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(EXECUTABLES)$(ELFSIZE)$(ELFCHECK)$(S_UPPER_DEPS) pit_timer.elf
	-@echo ' '

secondary-outputs: $(ELFSIZE) $(ELFCHECK)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
