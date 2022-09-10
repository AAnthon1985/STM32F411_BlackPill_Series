/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Alexander Anthon
 * @brief          : Minimalistic program to toggle an LED
 ******************************************************************************
 * @attention
 *                  Program toggles D2
 *                  D2 sits on GPIOC pin 13
 ******************************************************************************
 */

#include <stdint.h>
#include "stm32f4xx.h"
#include "stm32f4xx_ll_bus.h"
#include "stm32f4xx_ll_gpio.h"
#include "stm32f4xx_ll_utils.h"

void LEDInit();

int main(void)
{
    LL_Init1msTick(SystemCoreClock);
    LEDInit();
    LL_GPIO_SetOutputPin(GPIOC, LL_GPIO_PIN_13);
    char flag = 1;
    
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
        LL_mDelay(500);
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