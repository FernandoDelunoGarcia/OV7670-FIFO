################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Sources/Application.c \
../Sources/Fifo.c \
../Sources/HardwareProxy.c \
../Sources/OV7670.c \
../Sources/SCCB.c \
../Sources/main.c 

OBJS += \
./Sources/Application.o \
./Sources/Fifo.o \
./Sources/HardwareProxy.o \
./Sources/OV7670.o \
./Sources/SCCB.o \
./Sources/main.o 

C_DEPS += \
./Sources/Application.d \
./Sources/Fifo.d \
./Sources/HardwareProxy.d \
./Sources/OV7670.d \
./Sources/SCCB.d \
./Sources/main.d 


# Each subdirectory must supply rules for building sources it contributes
Sources/%.o: ../Sources/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g3 -I"../Sources" -I"../Includes" -std=c99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


