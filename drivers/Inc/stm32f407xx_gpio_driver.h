/*
 * stm32f407xx_gpio_driver.h
 *
 *  Created on: 11 May 2026
 *      Author: joshb
 */

#ifndef INC_STM32F407XX_GPIO_DRIVER_H_
#define INC_STM32F407XX_GPIO_DRIVER_H_

#include "stm32f407xx.h"

//Configuration structure for GPIO pin
typedef struct{
	uint8_t GPIO_PinNumber;
	uint8_t GPIO_PinMode;
	uint8_t GPIO_PinSpeed;
	uint8_t GPIO_PinPuPdControl;
	uint8_t GPIO_PinOPType;
	uint8_t GPIO_AltFunMode;
}GPIO_PinConfig_t;

//Handle structure for GPIO pin
typedef struct{
	GPIO_RegDef_t *pGPIOx; //Holds base address of port to which the pin belongs to
	GPIO_PinConfig_t GPIO_PinConfig; // Holds GPIO pin config settings
}GPIO_Handle_t;

/*************************GPIO Pin configuration macros******************************/

#define GPIO_PIN_NO_0  				0
#define GPIO_PIN_NO_1  				1
#define GPIO_PIN_NO_2  				2
#define GPIO_PIN_NO_3  				3
#define GPIO_PIN_NO_4  				4
#define GPIO_PIN_NO_5  				5
#define GPIO_PIN_NO_6  				6
#define GPIO_PIN_NO_7  				7
#define GPIO_PIN_NO_8  				8
#define GPIO_PIN_NO_9  				9
#define GPIO_PIN_NO_10 				10
#define GPIO_PIN_NO_11 				11
#define GPIO_PIN_NO_12 				12
#define GPIO_PIN_NO_13 				13
#define GPIO_PIN_NO_14 				14
#define GPIO_PIN_NO_15 				15

#define GPIO_MODE_IN				0
#define GPIO_MODE_OUT				1
#define GPIO_MODE_ALTFN				2
#define GPIO_MODE_ANALOG			3
#define GPIO_MODE_IT_FT				4
#define GPIO_MODE_IT_RT				5
#define GPIO_MODE_IT_RFT			6

#define GPIO_OP_TYPE_PP				0
#define GPIO_OP_TYPE_OD				1

#define GPIO_SPEED_LOW				0
#define GPIO_SPEED_MED				1
#define GPIO_SPEED_HIGH				2
#define GPIO_SPEED_VHIGH			3


#define GPIO_PIN_NOPUPD			    0
#define GPIO_PIN_PU					1
#define GPIO_PIN_PD					2

#define GPIO_AF_AF0					0
#define GPIO_AF_AF1					1
#define GPIO_AF_AF2					2
#define GPIO_AF_AF3					3
#define GPIO_AF_AF4					4
#define GPIO_AF_AF5					5
#define GPIO_AF_AF6					6
#define GPIO_AF_AF7					7
#define GPIO_AF_AF8					8
#define GPIO_AF_AF9					9
#define GPIO_AF_AF10				10
#define GPIO_AF_AF11				11
#define GPIO_AF_AF12				12
#define GPIO_AF_AF13				13
#define GPIO_AF_AF14				14
#define GPIO_AF_AF15				15

/*******************************API's supported by this driver*******************************/

/***************************************************************************
 * @brief	Configures GPIO pin with user-defined GPIO handle structure.
 * @param 	pGPIOHandle: Pointer to handle structure containing user-defined
 * 			configuration.
 * @note 	This function enables the peripheral clock, therefore no
 * 			requirement to enable before calling function.
 */
void GPIO_Init(GPIO_Handle_t *pGPIOHandle);

/***************************************************************************
 * @brief	Resets all registers in corresponding GPIO port.
 * @param 	pGPIOHandle: Pointer to handle structure containing user-defined
 * 			configuration.
 * @note 	This function resets the physical port but not the handle
 * 			structure.
 */
void GPIO_DeInit(GPIO_RegDef_t *pGPIOx);

/***************************************************************************
 * @brief	Enables or disables peripheral clock for selected GPIO port.
 * @param 	pGPIOx: Pointer to register definition structure
 * 			containing memory address of chosen GPIO port.
 * @param	EnorDi: ENABLE(1) or DISABLE(0) macro.
 */
void GPIO_PeripheralClockControl(GPIO_RegDef_t *pGPIOx, uint8_t EnorDi);

/***************************************************************************
 * @brief	Returns the current value held in the input data register of
 * 			the specified GPIO pin.
 * @param 	pGPIOx: Pointer to register definition structure
 * 			containing memory address of chosen GPIO port.
 * @param	PinNumber: GPIO_PIN_NO_0 - GPIO_PIN_NO_15.
 * @retval	The value of the input data register.
 */
uint8_t GPIO_ReadFromInputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber);

/***************************************************************************
 * @brief	Returns the current values held in the input data register of
 * 			the specified GPIO port.
 * @param 	pGPIOx: Pointer to register definition structure
 * 			containing memory address of chosen GPIO port.
 * @retval	The value of the input data register across
 * 			all pins for specific port.
 */
uint16_t GPIO_ReadFromInputPort(GPIO_RegDef_t *pGPIOx);

/***************************************************************************
 * @brief 	Writes specified value (either SET or RESET) to chosen GPIO pin.
 * @param	pGPIOx: Pointer to register definition structure
 * 			containing memory address of chosen GPIO port.
 * @param	PinNumber: GPIO_PIN_NO_0 - GPIO_PIN_NO_15.
 * @param	value: Either SET(1) or RESET(0).
 */
void GPIO_WriteToOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber, uint8_t value);

/***************************************************************************
 * @brief 	Writes specified 16 bit value to set and reset
 * 			register of chosen GPIO port.
 * @param 	pGPIOx: Pointer to register definition structure
 * 			containing memory address of chosen GPIO port.
 * @param   value: Holding set values (0-8 bits) and
 * 			reset values (bits 8-15) for each pin of the GPIO port.
 */
void GPIO_WriteToOutputPort(GPIO_RegDef_t *pGPIOx, uint16_t value);

/***************************************************************************
 * @brief	Toggles state of specified GPIO pin.
 * @param 	pGPIOx: Pointer to register definition structure
 * 			containing memory address of chosen GPIO port.
 * @param	PinNumber: GPIO_PIN_NO_0 - GPIO_PIN_NO_15.
 */
void GPIO_ToggleOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber);

/***************************************************************************
 * @brief	Either enables or disables an interrupt in the NVIC.
 * @param 	IRQ_Number: Containing the interrupt number
 * 			(found in vector table). From 0 - 81.
 * @param	EnorDi: ENABLE(1) or DISABLE(0) macro.
 */
void GPIO_IRQITConfig(uint8_t IRQ_Number, uint8_t EnorDi);

/***************************************************************************
 * @brief	Sets the provided IRQ's priority number (0-15).
 * @param 	IRQ_Number: Containing the interrupt
 * 			number (found in vector table).
 * @param 	IRQPriority: Number to configure priority (0-15).
 */
void GPIO_IRQPriorityConfig(uint8_t IRQ_Number, uint32_t IRQPriority);

/***************************************************************************
 * @brief	Clears pending bit for specific pin number in PR register if
 * 			it is set.
 * @param	PinNumber: GPIO_PIN_NO_0 - GPIO_PIN_NO_15.
 */
void GPIO_IRQHandling(uint8_t PinNumber);

//Macro for alfun low max pin number
#define HIGHEST_PIN_ALFUN_LOW_REG 	7

#endif /* INC_STM32F407XX_GPIO_DRIVER_H_ */
