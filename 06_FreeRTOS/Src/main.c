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

#include "FreeRTOS.h"
#include "task.h"

void LEDInit();

static void task1_handler(void* parameters);
static void task2_handler(void* parameters);

int main(void)
{
    LL_Init1msTick(SystemCoreClock);
    LEDInit();
    LL_GPIO_SetOutputPin(GPIOC, LL_GPIO_PIN_13);
    char flag = 1;
    TaskHandle_t task1_handle;
    TaskHandle_t task2_handle;
    BaseType_t status_task;
    
    status_task = xTaskCreate(task1_handler, "Task-1", 200, "Hello world from Task-1", 2, &task1_handle);

    configASSERT(status_task == pdPASS);

    status_task = xTaskCreate(task2_handler, "Task-2", 200, "Hello world from Task-2", 2, &task2_handle);

    configASSERT(status_task == pdPASS);

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

static void task1_handler(void* parameters) {

}

static void task2_handler(void* parameters) {

}