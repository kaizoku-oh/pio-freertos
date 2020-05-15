/* 
 **************************************************************************************************
 *
 * @file    : main.c
 * @author  : Bayrem GHARSELLAOUI
 * @version : 1.4.0
 * @date    : May 2020
 * @brief   : Application entry point file
 * 
 **************************************************************************************************
 * 
 * @project   : {pio-freertos}
 * @board     : {Nucleo 144 STM32F767Zi}
 * @compiler  : {PlatformIO, version 4.3.3}
 * @framework : {stm32cube}
 * 
 **************************************************************************************************
 */

/** @addtogroup main
  * @{
  */

/*-----------------------------------------------------------------------------------------------*/
/* Includes                                                                                      */
/*-----------------------------------------------------------------------------------------------*/
/** @defgroup main_includes Includes
 * @{
 */
#include "main.h"
#include "cmsis_os.h"
/**
  * @}
  */

/*-----------------------------------------------------------------------------------------------*/
/* Defines                                                                                       */
/*-----------------------------------------------------------------------------------------------*/
/** @defgroup main_defines Defines
 * @{
 */
#define USART_BAUDRATE                           115200
#define USART_TX_TIMEOUT_MS                      100
#define RX_BUFFER_SIZE                           32
#define QUEUE_ITEMS_COUNT                        8
#define QUEUE_MESSAGE                            (uint32_t)0xFF
/**
  * @}
  */
/*-----------------------------------------------------------------------------------------------*/
/* Private types                                                                                 */
/*-----------------------------------------------------------------------------------------------*/
/** @defgroup main_types Private types
  * @{
  */
/** MAIN data context structure */ 
typedef struct
{
  osThreadId stThreadHandle;                                       /**< Thread handler           */
  osMessageQId stQueue;                                            /**< Queue handler            */
  UART_HandleTypeDef stUsartHandle;                                /**< USART handle             */
  uint8_t u08RxBuffer[RX_BUFFER_SIZE];                             /**< USART rx buffer          */
  uint8_t u08RxByte;                                               /**< USART rx byte            */
  volatile uint8_t u08Index;                                       /**< USART rx byte index      */
}main_ctx_t;
/**
  * @}
  */

/*-----------------------------------------------------------------------------------------------*/
/* Private variables                                                                             */
/*-----------------------------------------------------------------------------------------------*/
/** @defgroup main_private_variables Private variables
  * @{
  */
static main_ctx_t stMainCtx;
/**
  * @}
  */

/*-----------------------------------------------------------------------------------------------*/
/* Private function prototypes ------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------*/
/** @defgroup main_private_function_prototypes Private function prototypes
  * @{
  */
static void usart_init(void);
static void thread_handler(void const * argument);
/**
  * @}
  */

/*-----------------------------------------------------------------------------------------------*/
/* Exported functions                                                                            */
/*-----------------------------------------------------------------------------------------------*/
/** @defgroup main_exported_functions Main function
  * @{
  */
/** ***********************************************************************************************
  * @brief      Main entry point
  * @return     Returns nothing
  ********************************************************************************************** */
int main(void)
{
  HAL_Init();

  osThreadDef(thread, thread_handler, osPriorityNormal, 0, 128);
  osMessageQDef(queue, QUEUE_ITEMS_COUNT, uint32_t);

  stMainCtx.stThreadHandle = osThreadCreate(osThread(thread), NULL);
  stMainCtx.stQueue = osMessageCreate(osMessageQ(queue), stMainCtx.stThreadHandle);

  osKernelStart();

  while(1) {}
}

/**************************************************************************************************
  * @brief      Get usart handle
  * @param      stHandle usart handle
  * @return     Returns a boolean indicating if we get the handle successfully or not
  ********************************************************************************************** */
bool usart_get_handle(UART_HandleTypeDef *stHandle)
{
  bool bRet;

  bRet = FALSE;
  if(stHandle)
  {
    *stHandle = stMainCtx.stUsartHandle;
    bRet = TRUE;
  }
  return bRet;
}
/**
  * @}
  */

/*-----------------------------------------------------------------------------------------------*/
/* Private functions                                                                             */
/*-----------------------------------------------------------------------------------------------*/
/** @defgroup main_private_functions
  * @{
  */
/**************************************************************************************************
  * @brief      LED thread handler
  * @param      argument argument pointer to be passed to thread handler
  * @return     Returns nothing
  ********************************************************************************************** */
static void thread_handler(void const * argument)
{
  osEvent stEvent;
  
  usart_init();
  HAL_UART_Receive_IT(&stMainCtx.stUsartHandle, &stMainCtx.u08RxByte, sizeof(stMainCtx.u08RxByte));

  BSP_PB_Init(BUTTON_USER, BUTTON_MODE_EXTI);
  
  BSP_LED_Init(LED_GREEN);
  BSP_LED_Init(LED_BLUE);
  BSP_LED_Init(LED_RED);
  for(;;)
  {
    stEvent = osMessageGet(stMainCtx.stQueue, osWaitForever);
    if(osEventMessage == stEvent.status)
    {
      HAL_UART_Transmit(&stMainCtx.stUsartHandle,
                        (uint8_t *)"Button pressed!\n",
                        (sizeof("Button pressed!\n")-1),
                        USART_TX_TIMEOUT_MS);
      BSP_LED_Toggle(LED_GREEN);
      BSP_LED_Toggle(LED_BLUE);
      BSP_LED_Toggle(LED_RED);
    }
  }
}

/**************************************************************************************************
  * @brief      Initialize and configure uart
  * @return     Returns nothing
  ********************************************************************************************** */
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

  /* Initialize usart interrupt */
  HAL_NVIC_SetPriority(USART3_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(USART3_IRQn);

  /* Initialize usart configs */
  stMainCtx.stUsartHandle.Instance = USARTx;
  stMainCtx.stUsartHandle.Init.BaudRate = USART_BAUDRATE;
  stMainCtx.stUsartHandle.Init.WordLength = UART_WORDLENGTH_8B;
  stMainCtx.stUsartHandle.Init.StopBits = UART_STOPBITS_1;
  stMainCtx.stUsartHandle.Init.Parity = UART_PARITY_NONE;
  stMainCtx.stUsartHandle.Init.Mode = UART_MODE_TX_RX;
  stMainCtx.stUsartHandle.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  stMainCtx.stUsartHandle.Init.OverSampling = UART_OVERSAMPLING_16;
  stMainCtx.stUsartHandle.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  stMainCtx.stUsartHandle.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if(HAL_OK != HAL_UART_Init(&stMainCtx.stUsartHandle))
  {
    while(1) {}
  }
}

/**************************************************************************************************
  * @brief      BUTTON callback
  * @param      u16GpioPin interrupt pin number
  * @return     Returns nothing
  ********************************************************************************************** */
void HAL_GPIO_EXTI_Callback(uint16_t u16GpioPin)
{
  if(USER_BUTTON_PIN == u16GpioPin)
  {
    osMessagePut(stMainCtx.stQueue, QUEUE_MESSAGE, 0);
  }
}

/**************************************************************************************************
  * @brief      UART receive callback
  * @param      stHandle uart handle Structure
  * @return     Returns nothing
  ********************************************************************************************** */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *stHandle)
{
  if(USART3 == stHandle->Instance)
  {
    if(RX_BUFFER_SIZE == stMainCtx.u08Index)
    {
      stMainCtx.u08Index = 0;
    }
    stMainCtx.u08RxBuffer[stMainCtx.u08Index++] = stMainCtx.u08RxByte;
    HAL_UART_Receive_IT(&stMainCtx.stUsartHandle,
                        &stMainCtx.u08RxByte,
                        sizeof(stMainCtx.u08RxByte));
  }
}
/**
 * @}
 */

/**
  * @}
  */