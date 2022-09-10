# STM32F411_BlackPill_00
Basic setup of a STM32F411 BlackPill project in VSCode including makefile and ST's low layer (LL) libraries

## Install
- [arm-none-eabi-gcc](https://developer.arm.com/downloads/-/arm-gnu-toolchain-downloads)
- [GNU make](https://sourceforge.net/projects/gnuwin32/files/make/3.81/make-3.81.exe/download?use_mirror=kumisystems&download=)
- [STM32CubeProgrammer](https://www.st.com/en/development-tools/stm32cubeprog.html)
- [Python](https://www.python.org/downloads/)

## Set the paths in the environmental variables

## Init/Add ST's HAL and LL libraries into your project
- Type:
> git submodule update --init --recursive
