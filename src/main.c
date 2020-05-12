#include "main.h"

void vTask1(void *pvParameters);
void vTask2(void *pvParameters);
void vTask3(void *pvParameters);

int main(void)
{
  HAL_Init();

  xTaskCreate(vTask1, "Task 1", 1000, NULL, 1, NULL);
  xTaskCreate(vTask2, "Task 2", 1000, NULL, 1, NULL);
  xTaskCreate(vTask3, "Task 3", 1000, NULL, 1, NULL);

  vTaskStartScheduler();

  while (1) {}
}

void vTask1(void *pvParameters)
{
  const TickType_t xDelayInMs = pdMS_TO_TICKS(1000);

  BSP_LED_Init(LED_GREEN);
  for(;;)
  {
    BSP_LED_Toggle(LED_GREEN);
    vTaskDelay(xDelayInMs);
  }
}

void vTask2(void *pvParameters)
{
  const TickType_t xDelayInMs = pdMS_TO_TICKS(1000);

  BSP_LED_Init(LED_BLUE);
  for(;;)
  {
    BSP_LED_Toggle(LED_BLUE);
    vTaskDelay(xDelayInMs);
  }
}

void vTask3(void *pvParameters)
{
  const TickType_t xDelayInMs = pdMS_TO_TICKS(1000);

  BSP_LED_Init(LED_RED);
  for(;;)
  {
    BSP_LED_Toggle(LED_RED);
    vTaskDelay(xDelayInMs);
  }
}