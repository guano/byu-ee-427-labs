################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/aliens.c \
../src/bunkers.c \
../src/interface.c \
../src/mother_ship.c \
../src/pit.c \
../src/platform.c \
../src/spaceInvadersRUN.c \
../src/tank.c \
../src/util.c 

LD_SRCS += \
../src/lscript.ld 

OBJS += \
./src/aliens.o \
./src/bunkers.o \
./src/interface.o \
./src/mother_ship.o \
./src/pit.o \
./src/platform.o \
./src/spaceInvadersRUN.o \
./src/tank.o \
./src/util.o 

C_DEPS += \
./src/aliens.d \
./src/bunkers.d \
./src/interface.d \
./src/mother_ship.d \
./src/pit.d \
./src/platform.d \
./src/spaceInvadersRUN.d \
./src/tank.d \
./src/util.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo Building file: $<
	@echo Invoking: MicroBlaze gcc compiler
	mb-gcc -Wall -O0 -g3 -c -fmessage-length=0 -I../../spaceInvadersLab4SDK_BSP/microblaze_0/include -mlittle-endian -mxl-barrel-shift -mxl-pattern-compare -mcpu=v8.20.b -mno-xl-soft-mul -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo Finished building: $<
	@echo ' '


