################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/IIC.c \
../Core/Src/MAX30102.c \
../Core/Src/control.c \
../Core/Src/display.c \
../Core/Src/gps.c \
../Core/Src/gyro.c \
../Core/Src/heart.c \
../Core/Src/lcd.c \
../Core/Src/main.c \
../Core/Src/stm32f4xx_hal_msp.c \
../Core/Src/stm32f4xx_it.c \
../Core/Src/syscalls.c \
../Core/Src/sysmem.c \
../Core/Src/system_stm32f4xx.c \
../Core/Src/temperature.c 

OBJS += \
./Core/Src/IIC.o \
./Core/Src/MAX30102.o \
./Core/Src/control.o \
./Core/Src/display.o \
./Core/Src/gps.o \
./Core/Src/gyro.o \
./Core/Src/heart.o \
./Core/Src/lcd.o \
./Core/Src/main.o \
./Core/Src/stm32f4xx_hal_msp.o \
./Core/Src/stm32f4xx_it.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o \
./Core/Src/system_stm32f4xx.o \
./Core/Src/temperature.o 

C_DEPS += \
./Core/Src/IIC.d \
./Core/Src/MAX30102.d \
./Core/Src/control.d \
./Core/Src/display.d \
./Core/Src/gps.d \
./Core/Src/gyro.d \
./Core/Src/heart.d \
./Core/Src/lcd.d \
./Core/Src/main.d \
./Core/Src/stm32f4xx_hal_msp.d \
./Core/Src/stm32f4xx_it.d \
./Core/Src/syscalls.d \
./Core/Src/sysmem.d \
./Core/Src/system_stm32f4xx.d \
./Core/Src/temperature.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F411xE -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/IIC.d ./Core/Src/IIC.o ./Core/Src/MAX30102.d ./Core/Src/MAX30102.o ./Core/Src/control.d ./Core/Src/control.o ./Core/Src/display.d ./Core/Src/display.o ./Core/Src/gps.d ./Core/Src/gps.o ./Core/Src/gyro.d ./Core/Src/gyro.o ./Core/Src/heart.d ./Core/Src/heart.o ./Core/Src/lcd.d ./Core/Src/lcd.o ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/stm32f4xx_hal_msp.d ./Core/Src/stm32f4xx_hal_msp.o ./Core/Src/stm32f4xx_it.d ./Core/Src/stm32f4xx_it.o ./Core/Src/syscalls.d ./Core/Src/syscalls.o ./Core/Src/sysmem.d ./Core/Src/sysmem.o ./Core/Src/system_stm32f4xx.d ./Core/Src/system_stm32f4xx.o ./Core/Src/temperature.d ./Core/Src/temperature.o

.PHONY: clean-Core-2f-Src

