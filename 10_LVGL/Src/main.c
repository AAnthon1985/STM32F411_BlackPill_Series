/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Alexander Anthon
 * @brief          : Minimalistic program to configure the MCU to use ILI9341 driver for touchscreen display
 *                    PA2/PA3: UART2
 *                    PA4/PA5/PA6/PA7: SPI1
 *                    @TODO: Set up SPI DMA
 *                    @TODO: Set up Interrupt
 ******************************************************************************
 * @attention
 *                  SysTick set to trigger interrupt every 1ms
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
#include "stm32f4xx_ll_spi.h"
#include "stm32f4xx_ll_system.h"

void LEDInit(void);
void UARTInit(void);
void SPIInit(void);

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
    /* Set up for HSE */
    LL_RCC_HSE_Enable();
    while(LL_RCC_HSE_IsReady() != 1) {};
    LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);

      /* Set FLASH latency */
    LL_FLASH_SetLatency(LL_FLASH_LATENCY_3);

    /* Set up PLL */
    LL_RCC_PLL_Disable();
    while(LL_RCC_PLL_IsReady() != 0) {};
    /* f_VCO = HSE*80/10=200MHz */
    /* f_PLL = f_VCO/2 = 100MHz */
    LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_10, 80, LL_RCC_PLLP_DIV_2);

    LL_RCC_PLL_Enable();
    while(LL_RCC_PLL_IsReady() != 1) {};


    /* Update Clocks */
    SystemCoreClockUpdate();
    SysTick_Config(SystemCoreClock / 1000);
    LL_RCC_GetSystemClocksFreq(&rcc_clocks);
    LEDInit();
    UARTInit();
    SPIInit();
    printf("HCLK frequency: %lu\r\n", rcc_clocks.HCLK_Frequency);
    printf("SYSCLK frequency: %lu\r\n", rcc_clocks.SYSCLK_Frequency);
    printf("PCLK1 frequency: %lu\r\n", rcc_clocks.PCLK1_Frequency);
    printf("PCLK2 frequency: %lu\r\n", rcc_clocks.PCLK2_Frequency);
    
    /* Loop forever */
    for (;;) {
      if (time_elapsed) {
        LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_13);
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

void SPIInit(void) {

  // Enable APB2 Bus and set PA4/PA5/PA6/Pa7 to Alternative Function (AF5)
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
  LL_GPIO_InitTypeDef PA_SPI1_Init = {0};
  PA_SPI1_Init.Mode = LL_GPIO_MODE_ALTERNATE;
  PA_SPI1_Init.Alternate = LL_GPIO_AF_5;
  PA_SPI1_Init.Pull = LL_GPIO_PULL_UP;
  PA_SPI1_Init.Speed = LL_GPIO_SPEED_FREQ_LOW;
  PA_SPI1_Init.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  LL_GPIO_Init(GPIOA, &PA_SPI1_Init);

  // Setup the LCD reset pin

  // Initialize SPI1
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SPI1);
  LL_SPI_InitTypeDef SPI1Init = {0};
  SPI1Init.BaudRate = LL_SPI_BAUDRATEPRESCALER_DIV2;
  SPI1Init.BitOrder = LL_SPI_MSB_FIRST;
  SPI1Init.DataWidth = LL_SPI_DATAWIDTH_8BIT;
  SPI1Init.TransferDirection = LL_SPI_FULL_DUPLEX;
  SPI1Init.NSS = LL_SPI_NSS_HARD_INPUT;
  SPI1Init.Mode = LL_SPI_MODE_MASTER;
  LL_SPI_Init(SPI1, &SPI1Init);
  LL_SPI_Enable(SPI1);

}

void SysTick_Handler(void) {
  LL_GPIO_SetOutputPin(GPIOC, LL_GPIO_PIN_13);
  time_elapsed = 1;
}