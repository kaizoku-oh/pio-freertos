#ifndef MAIN_H
#define MAIN_H

#include "stm32f7xx_hal.h"

#include "FreeRTOS.h"
// #include "task.h"
// #include "timers.h"
// #include "queue.h"
// #include "semphr.h"
// #include "event_groups.h"

// #include "cmsis_os.h"

#define GREEN_LED_PIN                          GPIO_PIN_0
#define BLUE_LED_PIN                           GPIO_PIN_7
#define RED_LED_PIN                            GPIO_PIN_14

#define LED_GPIO_PORT                          GPIOB

#define LED_GPIO_CLK_ENABLE()                  __HAL_RCC_GPIOB_CLK_ENABLE()

#endif // MAIN_H