#include "stm32f7xx_it.h"
#include "main.h"

extern UART_HandleTypeDef stUsartHandle;

void USART3_IRQHandler(void)
{
  HAL_UART_IRQHandler(&stUsartHandle);
}

void NMI_Handler(void)
{
}

void HardFault_Handler(void)
{
  while (1)
  {}
}

void MemManage_Handler(void)
{
  while (1)
  {}
}

void BusFault_Handler(void)
{
  while (1)
  {}
}

void UsageFault_Handler(void)
{
  while (1)
  {}
}
