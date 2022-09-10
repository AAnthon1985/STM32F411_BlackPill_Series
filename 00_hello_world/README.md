# STM32F411_BlackPill_00
Basic setup of a STM32F411 BlackPill project in VSCode including makefile and ST's low layer (LL) libraries

## Set the paths in the environmental variables
- Add STM32_Programmer_CLI path as a user variable within the Environment Variables under Windows

## Init/Add ST's HAL and LL libraries into your project
- Type:
> git submodule update --init --recursive
