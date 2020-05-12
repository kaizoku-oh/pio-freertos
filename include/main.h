#ifndef MAIN_H
#define MAIN_H

#include "stm32f7xx_nucleo_144.h"
#include "cmsis_os.h"

/* Pins Config */
#define USARTx_TX_PIN                            GPIO_PIN_8
#define USARTx_RX_PIN                            GPIO_PIN_9

#define USARTx_TX_AF                             GPIO_AF7_USART3
#define USARTx_RX_AF                             GPIO_AF7_USART3

/* Ports Config */
#define USARTx_GPIO_PORT                         GPIOD

/* USART instance */
#define USARTx                                   USART3

/* Clocks Config */
#define USARTx_CLK_ENABLE()                      __HAL_RCC_USART3_CLK_ENABLE()
#define USARTx_CLK_DISABLE()                     __HAL_RCC_USART3_CLK_DISABLE()

#define USARTx_GPIO_CLK_ENABLE()                 __HAL_RCC_GPIOD_CLK_ENABLE()
#define USARTx_GPIO_CLK_DISABLE()                __HAL_RCC_GPIOD_CLK_DISABLE()

/* USART Resets */
#define USARTx_FORCE_RESET()                     __HAL_RCC_USART3_FORCE_RESET()
#define USARTx_RELEASE_RESET()                   __HAL_RCC_USART3_RELEASE_RESET()

#endif /* MAIN_H */