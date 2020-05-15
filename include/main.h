/* 
 **************************************************************************************************
 *
 * @file    : main.h
 * @author  : Bayrem GHARSELLAOUI
 * @version : 1.4.0
 * @date    : May 2020
 * @brief   : Application entry point header file
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

/**
  @mainpage Project overview
  @brief Hi! Welcome to pio-freertos project documentation.<br>
  This project is designed to be an rtos implementation on PlatformIO.<br>
  The Git project path is https://github.com/kaizoku-619/pio-freertos.git<br>
  For further details, please refer to documentation available in doc/ directory.<br>
*/

/** @defgroup main MAIN
  * @brief Project entry point.
  * @{
  */

#ifndef MAIN_H
#define MAIN_H

/*-----------------------------------------------------------------------------------------------*/
/* Includes                                                                                      */
/*-----------------------------------------------------------------------------------------------*/
/** @defgroup main_includes Includes
 * @{
 */
#include "stm32f7xx_nucleo_144.h"
/**
  * @}
  */

/*-----------------------------------------------------------------------------------------------*/
/* Defines                                                                                       */
/*-----------------------------------------------------------------------------------------------*/
/** @defgroup main_defines Defines
 * @{
 */
/* Boolean defines */
#define TRUE                                     (bool)(1==1)
#define FALSE                                    (bool)(0==1)

/* Pins Config */
#define USARTx_TX_PIN                            GPIO_PIN_8
#define USARTx_RX_PIN                            GPIO_PIN_9

#define USARTx_AF                                GPIO_AF7_USART3

/* Ports Config */
#define USARTx_GPIO_PORT                         GPIOD

/* USART instance */
#define USARTx                                   USART3
/**
  * @}
  */

/*-----------------------------------------------------------------------------------------------*/
/* Macros                                                                                        */
/*-----------------------------------------------------------------------------------------------*/
/** @defgroup main_alice_macros Macros
 * @{
 */
/* Clocks Config */
#define USARTx_CLK_ENABLE()                      __HAL_RCC_USART3_CLK_ENABLE()
#define USARTx_CLK_DISABLE()                     __HAL_RCC_USART3_CLK_DISABLE()

#define USARTx_GPIO_CLK_ENABLE()                 __HAL_RCC_GPIOD_CLK_ENABLE()
#define USARTx_GPIO_CLK_DISABLE()                __HAL_RCC_GPIOD_CLK_DISABLE()

/* USART Resets */
#define USARTx_FORCE_RESET()                     __HAL_RCC_USART3_FORCE_RESET()
#define USARTx_RELEASE_RESET()                   __HAL_RCC_USART3_RELEASE_RESET()
/**
  * @}
  */

/*-----------------------------------------------------------------------------------------------*/
/* Exported types                                                                                 */
/*-----------------------------------------------------------------------------------------------*/
/** @defgroup main_types Exported types
  * @{
  */
typedef unsigned char                            bool;
/**
  * @}
  */
/*-----------------------------------------------------------------------------------------------*/
/* Exported function prototypes ------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------*/
/** @defgroup main_exported_function_prototypes Exported function prototypes
  * @{
  */
bool get_usart_handle(UART_HandleTypeDef *stHandle);
/**
  * @}
  */
/**
  * @}
  */

#endif /* MAIN_H */