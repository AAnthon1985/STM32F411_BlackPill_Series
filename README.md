# STM32F411_BlackPill_Series
Everything around the STM32F411 BlackPill from WeAct

## Install
- [arm-none-eabi-gcc](https://developer.arm.com/downloads/-/arm-gnu-toolchain-downloads)
- [GNU make](https://sourceforge.net/projects/gnuwin32/files/make/3.81/make-3.81.exe/download?use_mirror=kumisystems&download=)
- [STM32CubeProgrammer](https://www.st.com/en/development-tools/stm32cubeprog.html)
- [VSCode](https://code.visualstudio.com/download)
- [Python](https://www.python.org/downloads/)

## Set the paths in the environmental variables
- Add STM32_Programmer_CLI path as a user variable within the Environment Variables under Windows

## Init/Add ST's HAL and LL libraries into your project
- Type:
> git submodule update --init --recursive
