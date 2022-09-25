/**
 ******************************************************************************
 * @file           : HD44780.c
 * @author         : Alexander Anthon
 * @brief          : Functions to drive the HD44780
 ******************************************************************************
 * @attention
 *                  
 ******************************************************************************
 */

#include "stm32f4xx.h"
#include "stm32f4xx_ll_i2c.h"
#include "stm32f4xx_ll_utils.h"
#include "HD44780.h"

void HD44780Init(void)
{
	LL_mDelay(15); // 15ms bootup time

	LCD_cmd(LCD_SOFT_RESET, INSTRUCTION);
	LL_mDelay(5); // 5ms
	LCD_cmd(LCD_SOFT_RESET, INSTRUCTION);
	LL_mDelay(1); // 1ms
	LCD_cmd(LCD_SOFT_RESET, INSTRUCTION);
	LL_mDelay(5); // 5ms
	LL_mDelay(5); // 5ms
	LCD_cmd(LCD_SET_FUNCTION | LCD_FUNCTION_4BIT, INSTRUCTION);
	LL_mDelay(10); // 10ms

	LCD_cmd(LCD_SET_FUNCTION | LCD_FUNCTION_4BIT | LCD_FUNCTION_2LINE | LCD_FUNCTION_5X7 , INSTRUCTION); // Set to two lines
	LL_mDelay(1);	// 1ms
	LCD_cmd(LCD_SET_DISPLAY, INSTRUCTION);
	LL_mDelay(1);	// 1ms
	LCD_cmd(LCD_BLINKING_ON, INSTRUCTION);
	LL_mDelay(1);	// 1ms
	LL_mDelay(1); // 1ms
	LCD_cmd(LCD_DISPLAY_ON | LCD_CURSOR_ON, INSTRUCTION);
	LL_mDelay(1); // 1ms
	LCD_cmd(LCD_SET_DISPLAY | LCD_DISPLAY_ON | LCD_CURSOR_ON | LCD_BLINKING_ON, INSTRUCTION);
	LL_mDelay(1); // 1ms

	LCD_setcursor(0, 1);
	LCD_send("Init done");
}

void LCD_cmd(uint8_t data, uint8_t command)
{
	LL_I2C_GenerateStartCondition(I2C1);
	while (!LL_I2C_IsActiveFlag_SB(I2C1));
	uint8_t slaveAddress = HD44780_ADDRESS;		   // 0x27 = 39dec = 0b0010 0111
	slaveAddress = (slaveAddress << 1); // Make space on the 0th bit for R/W
	slaveAddress &= ~(1 << 0);		   // Clear 0th bit for write
	LL_I2C_TransmitData8(I2C1, slaveAddress);
	while (!LL_I2C_IsActiveFlag_ADDR(I2C1));
	LL_I2C_ClearFlag_ADDR(I2C1);
	while (!LL_I2C_IsActiveFlag_TXE(I2C1));

	uint8_t instruction = 0x00;
	if (command == 0)
	{
		instruction = 0x08; // 0b00001000. Sets RS=0, RW=0, EN=0, Backlight=On
	}
	else if (command == 1)
	{
		instruction = 0x09; // 0b00001001. Sets RS=1, RW=0, EN=0, Backlight=On
	}
	else
	{
		// Error - Call asseert fail in the long run
		#ifdef  USE_FULL_ASSERT
		// Call assert function
		#endif
		while(1);
	}
	uint8_t Nibble_H = 0;
	uint8_t Nibble_L = 0;
	Nibble_H = data & 0xF0;
	Nibble_L = data & 0x0F;

	LL_I2C_TransmitData8(I2C1, (Nibble_H | instruction));
	while (!LL_I2C_IsActiveFlag_TXE(I2C1));
	while (!LL_I2C_IsActiveFlag_BTF(I2C1));

	LCD_enable((Nibble_H | instruction));
	LL_I2C_TransmitData8(I2C1, ((Nibble_L << 4) | instruction));
	while (!LL_I2C_IsActiveFlag_TXE(I2C1))
		;
	while (!LL_I2C_IsActiveFlag_BTF(I2C1))
		;
	LCD_enable(((Nibble_L << 4) | instruction));
	while (!LL_I2C_IsActiveFlag_TXE(I2C1))
		;
	while (!LL_I2C_IsActiveFlag_BTF(I2C1))
		;
	LL_I2C_GenerateStopCondition(I2C1);
}

void LCD_enable(uint8_t data)
{	
	data |= (1<<LCD_E);
	LL_I2C_TransmitData8(I2C1, data);	// Enables pulse to secure command

	while (!LL_I2C_IsActiveFlag_TXE(I2C1));
	while (!LL_I2C_IsActiveFlag_BTF(I2C1));

	data &= ~(1 << LCD_E);

	LL_I2C_TransmitData8(I2C1, data);	// Disables pulse again

	while (!LL_I2C_IsActiveFlag_TXE(I2C1));
	while (!LL_I2C_IsActiveFlag_BTF(I2C1));
}

void LCD_send(const char *data)
{
	while (*data != '\0')
	{
		LCD_cmd(*data++, DATA);
	}
}

void LCD_setcursor(uint8_t col, uint8_t row)
{
	uint8_t data = 0;

	if (row == 1)
	{
		data = 0x80 + 0x00 + col;
	}
	else if (row == 2)
	{
		data = 0x80 + 0x40 + col;
	}
	else
	{
		// Error - Call asseert fail in the long run
		#ifdef  USE_FULL_ASSERT
		// Call assert function
		#endif
		while(1);
	}
	LCD_cmd(data, INSTRUCTION);
}
