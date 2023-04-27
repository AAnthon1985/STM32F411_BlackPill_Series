/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Alexander Anthon
 * @brief          : Minimalistic program to run FreeRTOS
 ******************************************************************************
 * @attention
 *                  
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

#include "FreeRTOS.h"
#include "task.h"

void LEDInit();
void UARTInit();

// To use printf() without modifying the syscalls.c file
int __io_putchar(int ch)
{
    while (!LL_USART_IsActiveFlag_TXE(USART2)) {}
    LL_USART_TransmitData8(USART2, ch);
    return ch;
}

static void task1_handler(void* parameters);
static void task2_handler(void* parameters);

int main(void)
{
    LL_Init1msTick(SystemCoreClock);
    LEDInit();
    UARTInit();
    LL_GPIO_SetOutputPin(GPIOC, LL_GPIO_PIN_13);
    char flag = 1;
    TaskHandle_t task1_handle;
    TaskHandle_t task2_handle;
    BaseType_t status_task;
    
    status_task = xTaskCreate(task1_handler, "Task-1", 200, "Hello world from Task-1", 2, &task1_handle);

    configASSERT(status_task == pdPASS);

    status_task = xTaskCreate(task2_handler, "Task-2", 200, "Hello world from Task-2", 2, &task2_handle);

    configASSERT(status_task == pdPASS);

    // start the FreeRTOS scheduler
    vTaskStartScheduler();

    // If the control scheme comes here, then the launch of the scheduler has failed due to insufficient memory in the heap

    /* Loop forever */
    for (;;)
    {
        if (flag)
        {
            LL_GPIO_SetOutputPin(GPIOC, LL_GPIO_PIN_13);
            flag = 0;
        }
        else
        {
            LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_13);
            flag = 1;
        }
        LL_mDelay(1000);
    }

    return 0;
}

void LEDInit()
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

void UARTInit() {
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

static void task1_handler(void* parameters) {
    while(1) {
        printf("%s\n", (char*)parameters);
        taskYIELD();
    }

}

static void task2_handler(void* parameters) {
    while(1) {
        printf("%s\n", (char*)parameters);
        taskYIELD();
    }
}