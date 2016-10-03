################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/aliens.c \
../src/bunkers.c \
../src/helloworld.c \
../src/interface.c \
../src/platform.c \
../src/renderer.c \
../src/tank.c \
../src/tank_old.c 

LD_SRCS += \
../src/lscript.ld 

OBJS += \
./src/aliens.o \
./src/bunkers.o \
./src/helloworld.o \
./src/interface.o \
./src/platform.o \
./src/renderer.o \
./src/tank.o \
./src/tank_old.o 

C_DEPS += \
./src/aliens.d \
./src/bunkers.d \
./src/helloworld.d \
./src/interface.d \
./src/platform.d \
./src/renderer.d \
./src/tank.d \
./src/tank_old.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo Building file: $<
	@echo Invoking: MicroBlaze gcc compiler
	mb-gcc -Wall -O0 -g3 -c -fmessage-length=0 -I../../hello_world_bsp_0/microblaze_0/include -mlittle-endian -mxl-barrel-shift -mxl-pattern-compare -mcpu=v8.20.b -mno-xl-soft-mul -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo Finished building: $<
	@echo ' '


