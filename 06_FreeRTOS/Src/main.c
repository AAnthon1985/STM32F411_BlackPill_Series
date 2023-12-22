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
#include "stm32f4xx_ll_system.h"

#include "FreeRTOS.h"
#include "task.h"

extern void SEGGER_UART_init(uint32_t);

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
static void task3_handler(void* parameters);

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
    LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_25, 200, LL_RCC_PLLP_DIV_2);

    LL_RCC_PLL_Enable();
    while(LL_RCC_PLL_IsReady() != 1) {};

    /* Update Clocks and set up SysTick Timer */
    LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_2);
    SystemCoreClockUpdate();

    LL_RCC_GetSystemClocksFreq(&rcc_clocks);

    LEDInit();
    LL_GPIO_SetOutputPin(GPIOC, LL_GPIO_PIN_13);


    TaskHandle_t task1_handle;
    TaskHandle_t task2_handle;
    TaskHandle_t task3_handle;
    BaseType_t status_task;

    vInitPrioGroupValue();

    // Enable Debug Exception and Monitor Control Register
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    // Enable cycle counter
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;

    SEGGER_UART_init(115200);

    SEGGER_SYSVIEW_Conf();
    
    status_task = xTaskCreate(task1_handler, "Task-1", 200, "Hello world from Task-1", 2, &task1_handle);

    configASSERT(status_task == pdPASS);

    status_task = xTaskCreate(task2_handler, "Task-2", 200, "Hello world from Task-2", 2, &task2_handle);

    configASSERT(status_task == pdPASS);

    status_task = xTaskCreate(task3_handler, "Task-3", 200, "Hello world from Task-2", 2, &task3_handle);

    configASSERT(status_task == pdPASS);

    // start the FreeRTOS scheduler
    vTaskStartScheduler();

    // If the control scheme comes here, then the launch of the scheduler has failed due to insufficient memory in the heap

    /* Loop forever */
    for (;;);

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
    char msg[100];
    TickType_t last_wakeup_time;
    last_wakeup_time = xTaskGetTickCount();
    while(1) {
        snprintf(msg,100,"%s\n", (char*)parameters);
        SEGGER_SYSVIEW_PrintfTarget(msg);
        vTaskDelayUntil(&last_wakeup_time, pdMS_TO_TICKS(1000));
    }
}

static void task2_handler(void* parameters) {
    char msg[100];
    TickType_t last_wakeup_time;
    last_wakeup_time = xTaskGetTickCount();
    while(1) {
        snprintf(msg,100,"%s\n", (char*)parameters);
        SEGGER_SYSVIEW_PrintfTarget(msg);
        vTaskDelayUntil(&last_wakeup_time, pdMS_TO_TICKS(800));
    }
}

static void task3_handler(void* parameters) {
    char msg[100];
    TickType_t last_wakeup_time;
    last_wakeup_time = xTaskGetTickCount();
    while(1) {
        snprintf(msg,100,"%s\n", (char*)parameters);
        SEGGER_SYSVIEW_PrintfTarget(msg);
        vTaskDelayUntil(&last_wakeup_time, pdMS_TO_TICKS(400));
    }
}