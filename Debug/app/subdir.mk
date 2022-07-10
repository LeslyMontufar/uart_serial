################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../app/app.c \
../app/cbf.c \
../app/hw.c \
../app/shell.c 

OBJS += \
./app/app.o \
./app/cbf.o \
./app/hw.o \
./app/shell.o 

C_DEPS += \
./app/app.d \
./app/cbf.d \
./app/hw.d \
./app/shell.d 


# Each subdirectory must supply rules for building sources it contributes
app/%.o app/%.su: ../app/%.c app/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../app -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-app

clean-app:
	-$(RM) ./app/app.d ./app/app.o ./app/app.su ./app/cbf.d ./app/cbf.o ./app/cbf.su ./app/hw.d ./app/hw.o ./app/hw.su ./app/shell.d ./app/shell.o ./app/shell.su

.PHONY: clean-app

