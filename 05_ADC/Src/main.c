/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Alexander Anthon
 * @brief          : Minimalistic program to configure the ADC, and send the data to the HD44780
 ******************************************************************************
 * @attention
 *                  TIM2 triggers the ADC and displays the data to the HD44780
 ******************************************************************************
 */

#include <stdint.h>
#include <stdio.h>
#include "stm32f4xx.h"
#include "stm32f4xx_ll_bus.h"
#include "stm32f4xx_ll_gpio.h"
#include "stm32f4xx_ll_utils.h"
#include "stm32f4xx_ll_rcc.h"
#include "stm32f4xx_ll_usart.h"
#include "stm32f4xx_ll_i2c.h"
#include "stm32f4xx_ll_adc.h"
#include "stm32f4xx_ll_tim.h"
#include "HD44780.h"

void LEDInit(void);
void UARTInit(void);
void I2CInit(void);
void TIM2Init(void);
void ADCInit(void);

// To use printf() without modifying the syscalls.c file
int __io_putchar(int ch)
{
    while (!LL_USART_IsActiveFlag_TXE(USART2)) {}
    LL_USART_TransmitData8(USART2, ch);
    return ch;
}

char flag = 1;
char timer_triggered = 0;
uint32_t adc_value = 0;

int main(void)
{
    LL_Init1msTick(SystemCoreClock);
    LEDInit();
    UARTInit();
    I2CInit();
    HD44780Init();
    TIM2Init();
    ADCInit();
    LL_GPIO_SetOutputPin(GPIOC, LL_GPIO_PIN_13);
    
    /* Loop forever */
    for (;;)
    {
        if(timer_triggered) {
            if (flag)
            {
                LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_13);
                printf("ADC reading: %ld\r\n", adc_value);
                LCD_cmd(LCD_CLEAR_DISPLAY, INSTRUCTION);
                LL_mDelay(5);
                LCD_setcursor(0, 1);
                LCD_send("Hello");
            }
            else
            {
                LL_GPIO_SetOutputPin(GPIOC, LL_GPIO_PIN_13);
                printf("ADC reading: %ld\r\n", adc_value);
                LCD_cmd(LCD_CLEAR_DISPLAY, INSTRUCTION);
                LL_mDelay(5);
                LCD_setcursor(0, 1);
                LCD_send("World");
            }
            timer_triggered = 0;
        }
    }

    return 0;
}

void LEDInit(void)
{
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOC);
    LL_GPIO_InitTypeDef LEDInit;
    LEDInit.Mode = LL_GPIO_MODE_OUTPUT;
    LEDInit.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    LEDInit.Speed = LL_GPIO_SPEED_FREQ_HIGH;
    LEDInit.Pin = LL_GPIO_PIN_13;
    LEDInit.Pull = LL_GPIO_PULL_NO;
    LL_GPIO_Init(GPIOC, &LEDInit);
}

void UARTInit(void) {
    // Enable APB1 Bus and set PA2 and PA3 to Alternate Function (AF7)
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
    LL_GPIO_InitTypeDef PA_UART2_Init = {0};
    PA_UART2_Init.Mode = LL_GPIO_MODE_ALTERNATE;
    PA_UART2_Init.Alternate = LL_GPIO_AF_7;
    PA_UART2_Init.Pull = LL_GPIO_PULL_UP;
    PA_UART2_Init.Speed = LL_GPIO_SPEED_FREQ_LOW;
    PA_UART2_Init.Pin = LL_GPIO_PIN_2 | LL_GPIO_PIN_3;
    PA_UART2_Init.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    LL_GPIO_Init(GPIOA, &PA_UART2_Init);

    // Initialize UART2
    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART2);
    LL_USART_InitTypeDef UART2Init = {0};
    UART2Init.BaudRate = 115200;
    UART2Init.DataWidth = LL_USART_DATAWIDTH_8B;
    UART2Init.StopBits = LL_USART_STOPBITS_1;
    UART2Init.Parity = LL_USART_PARITY_NONE;
    UART2Init.TransferDirection = LL_USART_DIRECTION_TX_RX;
    UART2Init.HardwareFlowControl = LL_USART_HWCONTROL_NONE;
    UART2Init.OverSampling = LL_USART_OVERSAMPLING_16;
    LL_USART_Init(USART2, &UART2Init);
    LL_USART_Enable(USART2);
}

void I2CInit(void) {
    // PB8 SCL and PB9 SDA
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);
    LL_GPIO_InitTypeDef PB_I2C1_Init = {0};
    PB_I2C1_Init.Mode = LL_GPIO_MODE_ALTERNATE;
    PB_I2C1_Init.Alternate = LL_GPIO_AF_4;
    PB_I2C1_Init.Pull = LL_GPIO_PULL_UP;
    PB_I2C1_Init.Speed = LL_GPIO_SPEED_FREQ_LOW;
    PB_I2C1_Init.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
    PB_I2C1_Init.Pin = LL_GPIO_PIN_8 | LL_GPIO_PIN_9;
    LL_GPIO_Init(GPIOB, &PB_I2C1_Init);

    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_I2C1);

    LL_I2C_Disable(I2C1);
    LL_I2C_EnableClockStretching(I2C1);

    LL_I2C_InitTypeDef I2C1Init = {0};
    I2C1Init.PeripheralMode = LL_I2C_MODE_I2C;
    I2C1Init.DutyCycle = LL_I2C_DUTYCYCLE_2;
    I2C1Init.AnalogFilter = LL_I2C_ANALOGFILTER_ENABLE;
    I2C1Init.DigitalFilter = 0;
    I2C1Init.OwnAddress1 = 0;
    I2C1Init.OwnAddrSize = LL_I2C_OWNADDRESS1_7BIT;
    I2C1Init.TypeAcknowledge = LL_I2C_ACK;
    I2C1Init.ClockSpeed = 400000;
    LL_I2C_Init(I2C1, &I2C1Init);
}

void TIM2Init(void) {
    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM2);
    LL_TIM_InitTypeDef TIM2_InitStruct = {0};
    TIM2_InitStruct.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;
    TIM2_InitStruct.CounterMode = LL_TIM_COUNTERMODE_UP;
    TIM2_InitStruct.Prescaler = 0x3E80; // 16MHz/0x3E80=1kHz
    TIM2_InitStruct.Autoreload = 0x03E8;    // 0x03E8 = 1000: 1kHz=1ms: 1ms*1000=1s
    TIM2_InitStruct.RepetitionCounter = 0;
    LL_TIM_Init(TIM2, &TIM2_InitStruct);

    LL_TIM_EnableARRPreload(TIM2);

    LL_TIM_SetTriggerOutput(TIM2, LL_TIM_TRGO_UPDATE);

    LL_TIM_EnableCounter(TIM2);


}

void ADCInit(void) {
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
    LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = LL_GPIO_PIN_1;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
    LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_ADC1);
    LL_ADC_InitTypeDef ADC1_InitStruct = {0};
    ADC1_InitStruct.SequencersScanMode = LL_ADC_SEQ_SCAN_DISABLE;
    ADC1_InitStruct.Resolution = LL_ADC_RESOLUTION_12B;
    ADC1_InitStruct.DataAlignment = LL_ADC_DATA_ALIGN_RIGHT;
    LL_ADC_Init(ADC1, &ADC1_InitStruct);

    LL_ADC_REG_InitTypeDef ADC_REG_InitStruct = {0};
    ADC_REG_InitStruct.ContinuousMode = LL_ADC_REG_CONV_SINGLE;
    ADC_REG_InitStruct.DMATransfer = LL_ADC_REG_DMA_TRANSFER_NONE;
    ADC_REG_InitStruct.SequencerDiscont = LL_ADC_REG_SEQ_DISCONT_DISABLE;
    ADC_REG_InitStruct.SequencerLength = LL_ADC_REG_SEQ_SCAN_DISABLE;
    ADC_REG_InitStruct.TriggerSource = LL_ADC_REG_TRIG_EXT_TIM2_TRGO;
    LL_ADC_REG_Init(ADC1, &ADC_REG_InitStruct);
    LL_ADC_REG_StartConversionExtTrig(ADC1,LL_ADC_REG_TRIG_EXT_RISING);
    LL_ADC_REG_SetSequencerRanks(ADC1, LL_ADC_REG_RANK_1, LL_ADC_CHANNEL_1);
    LL_ADC_SetChannelSamplingTime(ADC1, LL_ADC_CHANNEL_1, LL_ADC_SAMPLINGTIME_56CYCLES);
    LL_ADC_EnableIT_EOCS(ADC1);
    
    __NVIC_EnableIRQ(ADC_IRQn);

    LL_ADC_Enable(ADC1);


}

void ADC_IRQHandler(void) {
    adc_value = ADC1->DR;
    timer_triggered = 1;
    if (flag)
        {
            flag = 0;
        }
        else
        {
            flag = 1;
        }
}