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
blink_test_project.elf.size \

ELFCHECK += \
blink_test_project.elf.elfcheck \


# All Target
all: blink_test_project.elf secondary-outputs

# Tool invocations
blink_test_project.elf: $(OBJS) ../src/lscript.ld $(USER_OBJS)
	@echo Building target: $@
	@echo Invoking: MicroBlaze gcc linker
	mb-gcc -Wl,-T -Wl,../src/lscript.ld -L../../blink_test_bsp_0/microblaze_0/lib -mlittle-endian -mxl-barrel-shift -mxl-pattern-compare -mcpu=v8.20.b -mno-xl-soft-mul -o"blink_test_project.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo Finished building target: $@
	@echo ' '

blink_test_project.elf.size: blink_test_project.elf
	@echo Invoking: MicroBlaze Print Size
	mb-size blink_test_project.elf  |tee "blink_test_project.elf.size"
	@echo Finished building: $@
	@echo ' '

blink_test_project.elf.elfcheck: blink_test_project.elf
	@echo Invoking: Xilinx ELF Check
	elfcheck blink_test_project.elf -hw ../../HelloWorldHW_hw_platform/system.xml -pe microblaze_0  |tee "blink_test_project.elf.elfcheck"
	@echo Finished building: $@
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(EXECUTABLES)$(ELFSIZE)$(ELFCHECK)$(S_UPPER_DEPS) blink_test_project.elf
	-@echo ' '

secondary-outputs: $(ELFSIZE) $(ELFCHECK)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
