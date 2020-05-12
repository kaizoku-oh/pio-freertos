#include "main.h"

#define USART_BAUDRATE 115200

static void usart_init(void);
static void led_thread_handler(void const * argument);
static void usart_thread_handler(void const * argument);

volatile uint8_t u08RxByte;
UART_HandleTypeDef stUsartHandle;

osThreadId stLedThreadHandle;
osThreadId stUsartThreadHandle;

int main(void)
{
  HAL_Init();

  osThreadDef(led, led_thread_handler, osPriorityNormal, 0, 128);
  osThreadDef(usart, usart_thread_handler, osPriorityNormal, 0, 128);

  stLedThreadHandle = osThreadCreate(osThread(led), NULL);
  stUsartThreadHandle = osThreadCreate(osThread(usart), NULL);

  osKernelStart();

  while(1) {}
}

static void led_thread_handler(void const * argument)
{
  const uint32_t u32BlinkDelayMs = 1000;

  BSP_LED_Init(LED_GREEN);
  BSP_LED_Init(LED_BLUE);
  BSP_LED_Init(LED_RED);
  for(;;)
  {
    BSP_LED_Toggle(LED_GREEN);
    BSP_LED_Toggle(LED_BLUE);
    BSP_LED_Toggle(LED_RED);
    osDelay(u32BlinkDelayMs);
  }
}

static void usart_thread_handler(void const * argument)
{
  const uint32_t u32MsgDelayMs = 1000;

  usart_init();
  HAL_UART_Receive_IT(&stUsartHandle, &u08RxByte, sizeof(u08RxByte));
  for(;;)
  {
    HAL_UART_Transmit_IT(&stUsartHandle, (uint8_t *)"Hello world\n", (sizeof("Hello world\n")-1));
    osDelay(u32MsgDelayMs);
  }
}

static void usart_init(void)
{
  GPIO_InitTypeDef stUsartGpio = {0};

  /* Initialize usart clock */
  USARTx_CLK_ENABLE();
  USARTx_GPIO_CLK_ENABLE();
  
  /* Initialize usart gpio */
  stUsartGpio.Pin = USARTx_TX_PIN|USARTx_RX_PIN;
  stUsartGpio.Mode = GPIO_MODE_AF_PP;
  stUsartGpio.Pull = GPIO_NOPULL;
  stUsartGpio.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  stUsartGpio.Alternate = GPIO_AF7_USART3;
  HAL_GPIO_Init(USARTx_GPIO_PORT, &stUsartGpio);

  /* Initialize usart intrrupt */
  HAL_NVIC_SetPriority(USART3_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(USART3_IRQn);

  /* Initialize usart configs */
  stUsartHandle.Instance = USART3;
  stUsartHandle.Init.BaudRate = USART_BAUDRATE;
  stUsartHandle.Init.WordLength = UART_WORDLENGTH_8B;
  stUsartHandle.Init.StopBits = UART_STOPBITS_1;
  stUsartHandle.Init.Parity = UART_PARITY_NONE;
  stUsartHandle.Init.Mode = UART_MODE_TX_RX;
  stUsartHandle.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  stUsartHandle.Init.OverSampling = UART_OVERSAMPLING_16;
  stUsartHandle.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  stUsartHandle.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if(HAL_UART_Init(&stUsartHandle) != HAL_OK)
  {
    while(1) {}
  }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *stHandle)
{
  if(USART3 == stHandle->Instance)
  {
    HAL_UART_Receive_IT(&stUsartHandle, &u08RxByte, sizeof(u08RxByte));
  }
}