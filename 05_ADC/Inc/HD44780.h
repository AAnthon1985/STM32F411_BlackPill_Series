/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Alexander Anthon
 * @brief          : LCD defines
 ******************************************************************************
 * @attention
 *                  LCD HD44780 with PCF8574
 *                  LCD runs in 4bit mode. Following connection: 
 *                  PCF8574 |   HD44780 |   Description
 *                  P0          Pin4        RS (Instruction: 0; Data: 1)
 *                  P1          Pin5        RW (Write: 0; Read: 1)  -> Should always be write
 *                  P2          Pin6        E (Enable to confirm that the data is correct and can be read)
 *                  P3                      Not connected
 *                              Pin7        D0 (Not connected)
 *                              Pin8        D1 (Not connected)
 *                              Pin9        D2 (Not connected)
 *                              Pin10       D3 (Not connected)
 *                  P4          Pin11       D4
 *                  P5          Pin12       D5
 *                  P6          Pin13       D6
 *                  P7          Pin14       D7
 ******************************************************************************
 */

#ifndef LCD_H_
#define LCD_H_

#include <stdint.h>

#define INSTRUCTION				0
#define DATA					1

#define LCD_E                   2

#define LCD_BOOTUP_MS           15
#define LCD_ENABLE_US           20
#define LCD_WRITEDATA_US        46
#define LCD_COMMAND_US          42

#define LCD_SOFT_RESET_MS1      5
#define LCD_SOFT_RESET_MS2      1
#define LCD_SOFT_RESET_MS3      1
#define LCD_SET_4BITMODE_MS     5

#define LCD_CLEAR_DISPLAY_MS    2
#define LCD_CURSOR_HOME_MS      2

#define LCD_DDADR_LINE1         0x00
#define LCD_DDADR_LINE2         0x40
#define LCD_DDADR_LINE3         0x10
#define LCD_DDADR_LINE4         0x50

// Clear Display -------------- 0b00000001
#define LCD_CLEAR_DISPLAY       0x01

// Cursor Home ---------------- 0b0000001x
#define LCD_CURSOR_HOME         0x02

// Set Entry Mode ------------- 0b000001xx
#define LCD_SET_ENTRY           0x04

#define LCD_ENTRY_DECREASE      0x00
#define LCD_ENTRY_INCREASE      0x02
#define LCD_ENTRY_NOSHIFT       0x00
#define LCD_ENTRY_SHIFT         0x01

// Set Display ---------------- 0b00001xxx
#define LCD_SET_DISPLAY         0x08

#define LCD_DISPLAY_OFF         0x00
#define LCD_DISPLAY_ON          0x04
#define LCD_CURSOR_OFF          0x00
#define LCD_CURSOR_ON           0x02
#define LCD_BLINKING_OFF        0x00
#define LCD_BLINKING_ON         0x01

// Set Shift ------------------ 0b0001xxxx
#define LCD_SET_SHIFT           0x10

#define LCD_CURSOR_MOVE         0x00
#define LCD_DISPLAY_SHIFT       0x08
#define LCD_SHIFT_LEFT          0x00
#define LCD_SHIFT_RIGHT         0x04

// Set Function --------------- 0b001xxxxx
#define LCD_SET_FUNCTION        0x20

#define LCD_FUNCTION_4BIT       0x00
#define LCD_FUNCTION_8BIT       0x10
#define LCD_FUNCTION_1LINE      0x00
#define LCD_FUNCTION_2LINE      0x08
#define LCD_FUNCTION_5X7        0x00
#define LCD_FUNCTION_5X10       0x04

#define LCD_SOFT_RESET          0x30

#define HD44780_ADDRESS         0x27

void HD44780Init(void);
void LCD_cmd(uint8_t data, uint8_t command);
void LCD_enable(uint8_t data);
void LCD_send(const char *data);
void LCD_setcursor(uint8_t col, uint8_t row);

#endif /* LCD_H_ */
