#include "main.h"

static void Clock_Config(void);
static void GPIO_Init(void);

/* The tasks functions. */
void vTask1(void *pvParameters);
void vTask2(void *pvParameters);

int main(void)
{
  HAL_Init();
  Clock_Config();
  GPIO_Init();

  /* Create one of the two tasks. */
  xTaskCreate(vTask1                ,	/* Pointer to the function that implements the task. */
              "Task 1"              ,	/* Text name for the task.  This is to facilitate debugging only. */
              1000                  ,	/* Stack depth - most small microcontrollers will use much less stack than this. */
              NULL                  ,	/* We are not using the task parameter. */
              1                     ,	/* This task will run at priority 1. */
              NULL);                  /* We are not using the task handle. */

  /* Create the other task in exactly the same way. */
  xTaskCreate(vTask2, "Task 2", 1000, NULL, 1, NULL);

  /* Start the scheduler to start executing the tasks . */
  vTaskStartScheduler();

  while (1) {}
}

static void Clock_Config(void)
{
  LED_GPIO_CLK_ENABLE();
}

static void GPIO_Init(void)
{
  GPIO_InitTypeDef LED_GPIO_InitStruct;
  LED_GPIO_InitStruct.Pin = BLUE_LED_PIN;
  LED_GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  LED_GPIO_InitStruct.Pull = GPIO_PULLUP;
  LED_GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
  HAL_GPIO_Init(LED_GPIO_PORT, &LED_GPIO_InitStruct);

  LED_GPIO_InitStruct.Pin = RED_LED_PIN;
  LED_GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  LED_GPIO_InitStruct.Pull = GPIO_PULLUP;
  LED_GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
  HAL_GPIO_Init(LED_GPIO_PORT, &LED_GPIO_InitStruct);
}

void vTask1(void *pvParameters)
{
  const TickType_t xDelayInMs = pdMS_TO_TICKS(1000);

  for(;;)
  {
    HAL_GPIO_TogglePin(LED_GPIO_PORT, RED_LED_PIN);
    vTaskDelay(xDelayInMs);
  }
}

void vTask2(void *pvParameters)
{
  const TickType_t xDelayInMs = pdMS_TO_TICKS(100);

  for(;;)
  {
    HAL_GPIO_TogglePin(LED_GPIO_PORT, BLUE_LED_PIN);
    vTaskDelay(xDelayInMs);
  }
}

uint32_t HAL_GetTick(void)
{
  return xTaskGetTickCount(); 
}

/* Dummy function to avoid the standard initialization code */
HAL_StatusTypeDef HAL_InitTick(uint32_t TickPriority)
{
  return HAL_OK;
}

void Error_Handler()
{
  while (1);
}

/* Fault Handlers */
void HardFault_Handler(void)
{
  Error_Handler();
}

void MemManage_Handler(void)
{
  Error_Handler();
}

void BusFault_Handler(void)
{
  Error_Handler();
}

void UsageFault_Handler(void)
{
  Error_Handler();
}