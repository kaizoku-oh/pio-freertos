#include "main.h"

#define USART_BAUDRATE 115200
#define RX_BUFFER_SIZE 32
#define QUEUE_ITEMS_COUNT 1

uint8_t u08RxByte;
volatile uint8_t u08Index;
UART_HandleTypeDef stUsartHandle;
uint8_t u08RxBuffer[RX_BUFFER_SIZE];

osThreadId stLedThreadHandle;
osThreadId stUsartThreadHandle;
osMessageQId stButtonQueue;

static void usart_init(void);
static void led_thread_handler(void const * argument);
static void usart_thread_handler(void const * argument);

int main(void)
{
  HAL_Init();

  osThreadDef(led, led_thread_handler, osPriorityNormal, 0, 128);
  osThreadDef(usart, usart_thread_handler, osPriorityNormal, 0, 128);
  osMessageQDef(queue, QUEUE_ITEMS_COUNT, uint32_t);

  stLedThreadHandle = osThreadCreate(osThread(led), NULL);
  stUsartThreadHandle = osThreadCreate(osThread(usart), NULL);
  stButtonQueue = osMessageCreate(osMessageQ(queue), stLedThreadHandle);

  osKernelStart();

  while(1) {}
}

static void led_thread_handler(void const * argument)
{
  osEvent stEvent;
  const uint32_t u32BlinkDelayMs = 1000;
  
  BSP_PB_Init(BUTTON_USER, BUTTON_MODE_EXTI);
  BSP_LED_Init(LED_GREEN);
  BSP_LED_Init(LED_BLUE);
  BSP_LED_Init(LED_RED);
  for(;;)
  {
    stEvent = osMessageGet(stButtonQueue, osWaitForever);
    if(osEventMessage == stEvent.status)
    {
      BSP_LED_Toggle(LED_GREEN);
      BSP_LED_Toggle(LED_BLUE);
      BSP_LED_Toggle(LED_RED);
    }
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
    HAL_UART_Transmit(&stUsartHandle, (uint8_t *)"Hello world\n", (sizeof("Hello world\n")-1), 100);
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
  stUsartHandle.Instance = USARTx;
  stUsartHandle.Init.BaudRate = USART_BAUDRATE;
  stUsartHandle.Init.WordLength = UART_WORDLENGTH_8B;
  stUsartHandle.Init.StopBits = UART_STOPBITS_1;
  stUsartHandle.Init.Parity = UART_PARITY_NONE;
  stUsartHandle.Init.Mode = UART_MODE_TX_RX;
  stUsartHandle.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  stUsartHandle.Init.OverSampling = UART_OVERSAMPLING_16;
  stUsartHandle.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  stUsartHandle.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if(HAL_OK != HAL_UART_Init(&stUsartHandle))
  {
    while(1) {}
  }
}

void HAL_GPIO_EXTI_Callback(uint16_t u16GpioPin)
{
  if(USER_BUTTON_PIN == u16GpioPin)
  {
    osMessagePut(stButtonQueue, (uint32_t)0xFF, 0);
  }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *stHandle)
{
  if(USART3 == stHandle->Instance)
  {
    if(RX_BUFFER_SIZE == u08Index)
    {
      u08Index = 0;
    }
    u08RxBuffer[u08Index++] = u08RxByte;
    HAL_UART_Receive_IT(&stUsartHandle, &u08RxByte, sizeof(u08RxByte));
  }
}