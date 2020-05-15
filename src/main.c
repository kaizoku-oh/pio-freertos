/* 
 **************************************************************************************************
 *
 * @file    : main.c
 * @author  : Bayrem GHARSELLAOUI
 * @version : 1.4.0
 * @date    : May 2020
 * @brief   : Application entry point source file
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
#define THREAD_STACK_SIZE                        128
#define QUEUE_ITEMS_COUNT                        8
#define QUEUE_MESSAGE                            (uint32_t)0xFF
#define USART_BAUDRATE                           115200
#define USART_TX_TIMEOUT_MS                      100
#define RX_BUFFER_SIZE                           32

/* Renaming ST functions and types to match coding style */
#define hal_init                                 HAL_Init
#define hal_gpio_init                            HAL_GPIO_Init
#define hal_nvic_set_priority                    HAL_NVIC_SetPriority
#define hal_nvic_enable_irq                      HAL_NVIC_EnableIRQ
#define hal_uart_init                            HAL_UART_Init
#define hal_usart_receive_it                     HAL_UART_Receive_IT
#define hal_usart_transmit                       HAL_UART_Transmit
#define bsp_button_init                          BSP_PB_Init
#define bsp_led_init                             BSP_LED_Init
#define bsp_led_toggle                           BSP_LED_Toggle
#define usart_on_byte_received_cb                HAL_UART_RxCpltCallback
#define gpio_on_button_pressed_cb                HAL_GPIO_EXTI_Callback
#define uart_handle_t                            UART_HandleTypeDef
#define gpio_init_t                              GPIO_InitTypeDef
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
  uart_handle_t stUsartHandle;                                     /**< USART handle             */
  uint8_t tu08RxBuffer[RX_BUFFER_SIZE];                            /**< USART rx buffer          */
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
  hal_init();

  osThreadDef(thread, thread_handler, osPriorityNormal, 0, THREAD_STACK_SIZE);
  osMessageQDef(queue, QUEUE_ITEMS_COUNT, uint32_t);

  stMainCtx.stThreadHandle = osThreadCreate(osThread(thread), NULL);
  stMainCtx.stQueue = osMessageCreate(osMessageQ(queue), stMainCtx.stThreadHandle);

  osKernelStart();

  while(1)
  {}
}

/**************************************************************************************************
  * @brief      Get usart handle
  * @param      stHandle usart handle
  * @return     Returns a boolean indicating if we get the handle successfully or not
  ********************************************************************************************** */
bool get_usart_handle(uart_handle_t *stHandle)
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
  * @brief      Thread handler
  * @param      argument argument pointer to be passed to thread handler
  * @return     Returns nothing
  ********************************************************************************************** */
static void thread_handler(void const * argument)
{
  osEvent stEvent;
  
  usart_init();
  hal_usart_receive_it(&stMainCtx.stUsartHandle, &stMainCtx.u08RxByte, sizeof(stMainCtx.u08RxByte));

  bsp_button_init(BUTTON_USER, BUTTON_MODE_EXTI);
  
  bsp_led_init(LED_GREEN);
  bsp_led_init(LED_BLUE);
  bsp_led_init(LED_RED);
  for(;;)
  {
    stEvent = osMessageGet(stMainCtx.stQueue, osWaitForever);
    if(osEventMessage == stEvent.status)
    {
      hal_usart_transmit(&stMainCtx.stUsartHandle,
                         (uint8_t *)"Button pressed!\n",
                         (sizeof("Button pressed!\n")-1),
                         USART_TX_TIMEOUT_MS);
      bsp_led_toggle(LED_GREEN);
      bsp_led_toggle(LED_BLUE);
      bsp_led_toggle(LED_RED);
    }
  }
}

/**************************************************************************************************
  * @brief      Initialize and configure uart
  * @return     Returns nothing
  ********************************************************************************************** */
static void usart_init(void)
{
  gpio_init_t stUsartGpio = {0};

  /* Initialize usart clock */
  USARTx_CLK_ENABLE();
  USARTx_GPIO_CLK_ENABLE();
  
  /* Initialize usart gpio */
  stUsartGpio.Pin = USARTx_TX_PIN | USARTx_RX_PIN;
  stUsartGpio.Mode = GPIO_MODE_AF_PP;
  stUsartGpio.Pull = GPIO_NOPULL;
  stUsartGpio.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  stUsartGpio.Alternate = USARTx_AF;
  hal_gpio_init(USARTx_GPIO_PORT, &stUsartGpio);

  /* Initialize usart interrupt */
  hal_nvic_set_priority(USART3_IRQn, 5, 0);
  hal_nvic_enable_irq(USART3_IRQn);

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
  if(HAL_OK != hal_uart_init(&stMainCtx.stUsartHandle))
  {
    while(1)
    {}
  }
}

/**************************************************************************************************
  * @brief      BUTTON callback
  * @param      u16Pin interrupt pin number
  * @return     Returns nothing
  ********************************************************************************************** */
void gpio_on_button_pressed_cb(uint16_t u16Pin)
{
  if(USER_BUTTON_PIN == u16Pin)
  {
    osMessagePut(stMainCtx.stQueue, QUEUE_MESSAGE, 0);
  }
}

/**************************************************************************************************
  * @brief      UART receive callback
  * @param      stHandle uart handle Structure
  * @return     Returns nothing
  ********************************************************************************************** */
void usart_on_byte_received_cb(uart_handle_t *stHandle)
{
  if(USART3 == stHandle->Instance)
  {
    if(RX_BUFFER_SIZE == stMainCtx.u08Index)
    {
      stMainCtx.u08Index = 0;
    }
    stMainCtx.tu08RxBuffer[stMainCtx.u08Index++] = stMainCtx.u08RxByte;
    hal_usart_receive_it(&stMainCtx.stUsartHandle,
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