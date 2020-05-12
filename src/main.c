#include "main.h"

void thread_1_handler(void const * argument);
void thread_2_handler(void const * argument);
void thread_3_handler(void const * argument);

osThreadId stThread1Handle;
osThreadId stThread2Handle;
osThreadId stThread3Handle;

int main(void)
{
  HAL_Init();

  osThreadDef(thread1, thread_1_handler, osPriorityNormal, 0, 128);
  osThreadDef(thread2, thread_2_handler, osPriorityNormal, 0, 128);
  osThreadDef(thread3, thread_3_handler, osPriorityNormal, 0, 128);

  stThread1Handle = osThreadCreate(osThread(thread1), NULL);
  stThread2Handle = osThreadCreate(osThread(thread2), NULL);
  stThread3Handle = osThreadCreate(osThread(thread3), NULL);

  osKernelStart();

  while (1) {}
}

void thread_1_handler(void const * argument)
{
  const uint32_t u32BlinkDelayMs = 1000;

  BSP_LED_Init(LED_GREEN);
  for(;;)
  {
    BSP_LED_Toggle(LED_GREEN);
    osDelay(u32BlinkDelayMs);
  }
}

void thread_2_handler(void const * argument)
{
  const uint32_t u32BlinkDelayMs = 500;

  BSP_LED_Init(LED_BLUE);
  for(;;)
  {
    BSP_LED_Toggle(LED_BLUE);
    osDelay(u32BlinkDelayMs);
  }
}

void thread_3_handler(void const * argument)
{
  const uint32_t u32BlinkDelayMs = 250;

  BSP_LED_Init(LED_RED);
  for(;;)
  {
    BSP_LED_Toggle(LED_RED);
    osDelay(u32BlinkDelayMs);
  }
}