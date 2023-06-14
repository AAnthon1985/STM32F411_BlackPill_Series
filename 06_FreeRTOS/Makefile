# CC and MACH are variables
CC=arm-none-eabi-gcc
MACH=cortex-m4
OUTPUT=build
BIN = main
OBJS = $(OUTPUT)/main.o
OBJS += $(OUTPUT)/system_stm32f4xx.o
OBJS += $(OUTPUT)/stm32f4xx_ll_gpio.o
OBJS += $(OUTPUT)/stm32f4xx_ll_utils.o
OBJS += $(OUTPUT)/stm32f4xx_ll_usart.o
OBJS += $(OUTPUT)/stm32f4xx_ll_rcc.o
OBJS += $(OUTPUT)/syscalls.o
OBJS += $(OUTPUT)/tasks.o
OBJS += $(OUTPUT)/list.o
OBJS += $(OUTPUT)/port.o
OBJS += $(OUTPUT)/heap_4.o
OBJS += $(OUTPUT)/SEGGER_SYSVIEW.o
OBJS += $(OUTPUT)/SEGGER_RTT.o
OBJS += $(OUTPUT)/SEGGER_SYSVIEW_FreeRTOS.o
OBJS += $(OUTPUT)/SEGGER_SYSVIEW_Config_FreeRTOS.o
OBJS += $(OUTPUT)/segger_uart.o
OBJS += $(OUTPUT)/SEGGER_RTT_ASM_ARMv7M.o
OBJS += $(OUTPUT)/startup_stm32f411xe.o

# Includes
STM32CUBE_DIR = ../STM32CubeF4/Drivers
FREERTOS_DIR = ./ThirdParty/FreeRTOS
SEGGER_DIR = ./ThirdParty/SEGGER
C_INCLUDES = -I./Inc
C_INCLUDES += -I$(STM32CUBE_DIR)/STM32F4xx_HAL_Driver/Inc
C_INCLUDES += -I$(STM32CUBE_DIR)/CMSIS/Include 
C_INCLUDES += -I$(STM32CUBE_DIR)/CMSIS/Device/ST/STM32F4xx/Include
C_INCLUDES += -I$(FREERTOS_DIR)/include
C_INCLUDES += -I$(FREERTOS_DIR)/portable/GCC/ARM_CM4F
C_INCLUDES += -I$(SEGGER_DIR)/OS
C_INCLUDES += -I$(SEGGER_DIR)/SEGGER
C_INCLUDES += -I$(SEGGER_DIR)/Config
C_INCLUDES += -I$(SEGGER_DIR)/Rec

# Defines
C_DEFS = -DSTM32F411xE
C_DEFS += -DUSE_FULL_LL_DRIVER


# CFLAGS will be passed to the compiler
CFLAGS= -c -mcpu=$(MACH) -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard $(C_INCLUDES) -std=gnu11 -O0 -g --specs=nano.specs -Wall -Werror -ffunction-sections -fdata-sections $(C_DEFS)
SFLAGS= -c -x assembler-with-cpp -mcpu=$(MACH) -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard $(C_INCLUDES) -std=gnu11 -O0 -g --specs=nano.specs -Wall -Werror -ffunction-sections -fdata-sections $(C_DEFS)
LDFLAGS= -mcpu=$(MACH) -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard --specs=nano.specs --specs=nosys.specs -TSTM32F411CEUX_FLASH.ld -Wl,-Map=$(OUTPUT)/$(BIN).map -Wl,--gc-sections -static

# Target: Dependencies
all: $(OUTPUT)/$(BIN).elf

$(OUTPUT)/%.o: $(STM32CUBE_DIR)/STM32F4xx_HAL_Driver/Src/%.c
	$(CC) $(CFLAGS) $< -o $@

$(OUTPUT)/%.o: ./Src/%.c
	$(CC) $(CFLAGS) $< -o $@

$(OUTPUT)/%.o: $(STM32CUBE_DIR)/CMSIS/Device/ST/STM32F4xx/Source/Templates/%.c
	$(CC) $(CFLAGS) $< -o $@

$(OUTPUT)/%.o: ./ThirdParty/FreeRTOS/%.c
	$(CC) $(CFLAGS) $< -o $@

$(OUTPUT)/%.o: ./ThirdParty/FreeRTOS/portable/GCC/ARM_CM4F/%.c
	$(CC) $(CFLAGS) $< -o $@

$(OUTPUT)/%.o: ./ThirdParty/FreeRTOS/portable/MemMang/%.c
	$(CC) $(CFLAGS) $< -o $@

$(OUTPUT)/%.o: ./ThirdParty/SEGGER/Config/%.c
	$(CC) $(CFLAGS) $< -o $@

$(OUTPUT)/%.o: ./ThirdParty/SEGGER/OS/%.c
	$(CC) $(CFLAGS) $< -o $@

$(OUTPUT)/%.o: ./ThirdParty/SEGGER/Config/%.c
	$(CC) $(CFLAGS) $< -o $@

$(OUTPUT)/%.o: ./ThirdParty/SEGGER/SEGGER/%.c
	$(CC) $(CFLAGS) $< -o $@

$(OUTPUT)/%.o: ./ThirdParty/SEGGER/Rec/%.c
	$(CC) $(CFLAGS) $< -o $@

$(OUTPUT)/%.o: ./ThirdParty/SEGGER/SEGGER/%.S
	$(CC) $(CFLAGS) $< -o $@

$(OUTPUT)/%.o: $(STM32CUBE_DIR)/CMSIS/Device/ST/STM32F4xx/Source/Templates/gcc/%.s
	$(CC) $(SFLAGS) $< -o $@

$(OUTPUT)/$(BIN).elf: $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

.PHONY: clean

clean:
	@echo "Clean project"
	-rm -rf $(OUTPUT)/*.o $(OUTPUT)/*.map $(OUTPUT)/*.elf
	@echo "Clean completed"
	
STM32CUBEPROG ?= STM32_Programmer_CLI

flash: $(OUTPUT)/$(BIN).elf
	@printf "  FLASH\t$<\n"
	$(STM32CUBEPROG) -c port=SWD -w $(realpath $(OUTPUT)/$(BIN).elf) -v -rst