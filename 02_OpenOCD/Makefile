# CC and MACH are variables
CC=arm-none-eabi-gcc
MACH=cortex-m4
OUTPUT=build
BIN = main
OBJS = $(OUTPUT)/main.o
OBJS += $(OUTPUT)/system_stm32f4xx.o
OBJS += $(OUTPUT)/stm32f4xx_ll_gpio.o
OBJS += $(OUTPUT)/stm32f4xx_ll_utils.o
OBJS += $(OUTPUT)/startup_stm32f411xe.o
# CFLAGS will be passed to the compiler
CFLAGS= -c -mcpu=$(MACH) -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard -I./Inc -I../STM32CubeF4/Drivers/STM32F4xx_HAL_Driver/Inc -I../STM32CubeF4/Drivers/CMSIS/Include -I../STM32CubeF4/Drivers/CMSIS/Device/ST/STM32F4xx/Include -std=gnu11 -O0 -g --specs=nano.specs -Wall -Werror -ffunction-sections -fdata-sections -DSTM32F411xE -DUSE_FULL_LL_DRIVER
SFLAGS= -c -x assembler-with-cpp -mcpu=$(MACH) -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard -I./Inc -I../STM32CubeF4/Drivers/STM32F4xx_HAL_Driver/Inc -I../STM32CubeF4/Drivers/CMSIS/Include -I../STM32CubeF4/Drivers/CMSIS/Device/ST/STM32F4xx/Include -std=gnu11 -O0 -g --specs=nano.specs -Wall -Werror -ffunction-sections -fdata-sections -DSTM32F411xE -DUSE_FULL_LL_DRIVER
LDFLAGS= -mcpu=$(MACH) -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard --specs=nano.specs --specs=nosys.specs -TSTM32F411CEUX_FLASH.ld -Wl,-Map=$(OUTPUT)/$(BIN).map -Wl,--gc-sections -static
# Target: Dependencies
all: $(OUTPUT)/$(BIN).elf

$(OUTPUT)/%.o: ../STM32CubeF4/Drivers/STM32F4xx_HAL_Driver/Src/%.c
	$(CC) $(CFLAGS) $< -o $@

$(OUTPUT)/%.o: ./Src/%.c
	$(CC) $(CFLAGS) $< -o $@

$(OUTPUT)/%.o: ../STM32CubeF4/Drivers/CMSIS/Device/ST/STM32F4xx/Source/Templates/%.c
	$(CC) $(CFLAGS) $< -o $@

$(OUTPUT)/%.o: ../STM32CubeF4/Drivers/CMSIS/Device/ST/STM32F4xx/Source/Templates/gcc/%.s
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