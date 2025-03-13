cmake_minimum_required(VERSION 3.22)

project(stm32cubemx)
add_library(stm32cubemx INTERFACE)

# Enable CMake support for ASM and C languages
enable_language(C ASM)

target_compile_definitions(stm32cubemx INTERFACE 
	USE_FULL_LL_DRIVER 
	HSE_VALUE=8000000 
	HSE_STARTUP_TIMEOUT=100 
	LSE_STARTUP_TIMEOUT=5000 
	LSE_VALUE=32768 
	HSI_VALUE=8000000 
	LSI_VALUE=40000 
	VDD_VALUE=3300 
	PREFETCH_ENABLE=1 
	INSTRUCTION_CACHE_ENABLE=0 
	DATA_CACHE_ENABLE=0 
	STM32F030xC
    $<$<CONFIG:Debug>:DEBUG>
)

target_include_directories(stm32cubemx INTERFACE
    ../../Core/Inc
    ../../Drivers/STM32F0xx_HAL_Driver/Inc
    ../../Drivers/CMSIS/Device/ST/STM32F0xx/Include
    ../../Drivers/CMSIS/Include
    ../../Utils
    ../../App
)

target_sources(stm32cubemx INTERFACE
    ../../Core/Src/main.c
    ../../Core/Src/stm32f0xx_it.c
    ../../Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_ll_gpio.c
    ../../Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_ll_adc.c
    ../../Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_ll_dma.c
    ../../Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_ll_rcc.c
    ../../Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_ll_utils.c
    ../../Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_ll_exti.c
    ../../Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_ll_pwr.c
    ../../Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_ll_usart.c
    ../../Core/Src/system_stm32f0xx.c
    ../../Core/Src/sysmem.c
    ../../Core/Src/syscalls.c
    ../../startup_stm32f030xc.s
    ../../App/app.c
    ../../App/command.c
    ../../App/cmd_line_task.c
    ../../App/adc_task.c
    ../../App/charge_task.c
    ../../Utils/delay.c
    ../../Utils/scheduler.c
    ../../Utils/uart.c
    ../../Utils/cmd_line.c
    ../../Utils/adc.c
    ../../Utils/pwm.c
)

target_link_directories(stm32cubemx INTERFACE
)

target_link_libraries(stm32cubemx INTERFACE
)

# Validate that STM32CubeMX code is compatible with C standard
if(CMAKE_C_STANDARD LESS 11)
    message(ERROR "Generated code requires C11 or higher")
endif()


