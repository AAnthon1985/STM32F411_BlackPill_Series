/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Alexander Anthon
 * @brief          : Minimalistic program to configure the MCU to use HSE (25MHz mounted on the PCB)
 ******************************************************************************
 * @attention
 *                  SysTick set to trigger interrupt every 1s
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

void LEDInit(void);
void UARTInit(void);

// To use printf() without modifying the syscalls.c file
int __io_putchar(int ch)
{
    while (!LL_USART_IsActiveFlag_TXE(USART2)) {}
    LL_USART_TransmitData8(USART2, ch);
    return ch;
}

char flag = 1;
char time_elapsed = 0;

int main(void)
{
    LL_RCC_ClocksTypeDef rcc_clocks;
    /* Set up for HSE without PLL */
    LL_RCC_HSE_Enable();
    while(LL_RCC_HSE_IsReady() != 1) {};
    LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_HSE);
    SystemCoreClockUpdate();
    SysTick_Config(SystemCoreClock / 8);
    LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_8 - 1);
    LL_RCC_GetSystemClocksFreq(&rcc_clocks);
    LEDInit();
    UARTInit();
    printf("Frequency: %lu\r\n", rcc_clocks.SYSCLK_Frequency);
    
    /* Loop forever */
    for (;;) {
      if (time_elapsed) {
        printf("Frequency: %lu\r\n", rcc_clocks.SYSCLK_Frequency);
        time_elapsed = 0;
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
    LL_GPIO_SetOutputPin(GPIOC, LL_GPIO_PIN_13);
}

void UARTInit(void) {
    // Enable AHB1 Bus and set PA2 and PA3 to Alternate Function (AF7)
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

void SysTick_Handler(void) {
    time_elapsed = 1;
}