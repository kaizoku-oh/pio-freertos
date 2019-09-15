#include "main.h"

#define BLINK_DELAY_MS 100

static void LED_Init(void);

/* The task functions. */
void vTask1(void *pvParameters);
void vTask2(void *pvParameters);

int main(void)
{
  HAL_Init();
  LED_Init();

  /* Create one of the two tasks. */
  xTaskCreate(vTask1                ,	/* Pointer to the function that implements the task. */
              "Task 1"              ,	/* Text name for the task.  This is to facilitate debugging only. */
              1000                  ,	/* Stack depth - most small microcontrollers will use much less stack than this. */
              NULL                  ,	/* We are not using the task parameter. */
              1 ,	                    /* This task will run at priority 1. */
              NULL);		              /* We are not using the task handle. */

  /* Create the other task in exactly the same way. */
  xTaskCreate(vTask2, "Task 2", 1000, NULL, 1, NULL);

  /* Start the scheduler to start the tasks executing. */
  vTaskStartScheduler();

  while (1)
  {
    HAL_GPIO_TogglePin(LED_GPIO_PORT, RED_LED_PIN);
    HAL_Delay(BLINK_DELAY_MS);

    HAL_GPIO_TogglePin(LED_GPIO_PORT, BLUE_LED_PIN);
    HAL_Delay(BLINK_DELAY_MS);
    
    HAL_GPIO_TogglePin(LED_GPIO_PORT, GREEN_LED_PIN);
    HAL_Delay(BLINK_DELAY_MS);
  }
}

void LED_Init()
{
  LED_GPIO_CLK_ENABLE();

  GPIO_InitTypeDef BLUE_LED_GPIO_InitStruct;
  BLUE_LED_GPIO_InitStruct.Pin = BLUE_LED_PIN;
  BLUE_LED_GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  BLUE_LED_GPIO_InitStruct.Pull = GPIO_PULLUP;
  BLUE_LED_GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
  HAL_GPIO_Init(LED_GPIO_PORT, &BLUE_LED_GPIO_InitStruct);

  GPIO_InitTypeDef RED_LED_GPIO_InitStruct;
  RED_LED_GPIO_InitStruct.Pin = RED_LED_PIN;
  RED_LED_GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  RED_LED_GPIO_InitStruct.Pull = GPIO_PULLUP;
  RED_LED_GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
  HAL_GPIO_Init(LED_GPIO_PORT, &RED_LED_GPIO_InitStruct);

  GPIO_InitTypeDef GREEN_LED_GPIO_InitStruct;
  GREEN_LED_GPIO_InitStruct.Pin = GREEN_LED_PIN;
  GREEN_LED_GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GREEN_LED_GPIO_InitStruct.Pull = GPIO_PULLUP;
  GREEN_LED_GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
  HAL_GPIO_Init(LED_GPIO_PORT, &GREEN_LED_GPIO_InitStruct);
}

void vTask1(void *pvParameters)
{
  const char *pcTaskName = "Task 1 is running\r\n";
  for(;;)
  {
    vTaskDelay(500);
  }
}

void vTask2(void *pvParameters)
{
  const char *pcTaskName = "Task 2 is running\r\n";
  for(;;)
  {
    vTaskDelay(1000);
  }
}

void SysTick_Handler(void)
{
  HAL_IncTick();
}