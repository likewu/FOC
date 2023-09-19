#!/bin/sh

F:\ST\STM32CubeIDE_1.13.1\STM32CubeIDE\plugins\com.st.stm32cube.ide.mcu.externaltools.gnu-tools-for-stm32.11.3.rel1.win32_1.1.0.202305231506\tools\bin\arm-none-eabi-objcopy -O binary Release/book4.elf Release/book4.bin

#wokwi-server --chip esp32 Release/book4.elf
