/*
 * stm32f407xx_rcc_driver.h
 *
 *  Created on: 2 Aug 2026
 *      Author: joshb
 */

#ifndef INC_STM32F407XX_RCC_DRIVER_H_
#define INC_STM32F407XX_RCC_DRIVER_H_

#include "stm32f407xx.h"

#define RCC_CLOCK_HSE		8000000
#define RCC_CLOCK_HSI		16000000

/***************************************************************************
 * @brief	Returns the current clock speed of the APB1 bus.
 * @retval	Clock speed (in Hz) represented by 32 bit unsigned integer.
 */
uint32_t RCC_GetPeripheralClockAPB1Value(void);

/***************************************************************************
 * @brief	Returns the current clock speed of the APB2 bus.
 * @retval	Clock speed (in Hz) represented by 32 bit unsigned integer.
 */
uint32_t RCC_GetPeripheralClockAPB2Value(void);

#endif /* INC_STM32F407XX_RCC_DRIVER_H_ */
