/*
 * stm32f407xx_gpio_driver.h
 *
 *  Created on: 11 May 2026
 *      Author: joshb
 */

#ifndef INC_STM32F407XX_GPIO_DRIVER_H_
#define INC_STM32F407XX_GPIO_DRIVER_H_

#include "stm32f407xx.h"

//Configuration structure for a GPIO pin
typedef struct{
	uint8_t GPIO_PinNumber; /*!< possible values from @GPIO_PIN_NUMBERS>*/
	uint8_t GPIO_PinMode;  /*!< possible values from @GPIO_PIN_MODES >*/
	uint8_t GPIO_PinSpeed; /*!< possible values from @GPIO_PIN_SPEED >*/
	uint8_t GPIO_PinPuPdControl; /*!< possible values from @GPIO_PIN_PUPD >*/
	uint8_t GPIO_PinOPType; /*!< possible values from @GPIO_PIN_OP_TYPE >*/
	uint8_t GPIO_AltFunMode; /*!< possible values from @GPIO_PIN_ALFUNMODE >*/
}GPIO_PinConfig_t;

//Handle structure for a GPIO pin
typedef struct{
	GPIO_RegDef_t *pGPIOx; //Holds base address of port to which the pin belongs to
	GPIO_PinConfig_t GPIO_PinConfig; // Holds GPIO pin config settings
}GPIO_Handle_t;

/** @GPIO_PIN_NUMBERS
 * GPIO pin numbers
 */
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

/** @GPIO_PIN_MODES
 * GPIO pin possible modes
 */
#define GPIO_MODE_IN				0 //Input mode
#define GPIO_MODE_OUT				1 //Output mode
#define GPIO_MODE_ALTFN				2 //Alternate function mode
#define GPIO_MODE_ANALOG			3 //Analog mode
#define GPIO_MODE_IT_FT				4 //Interrupt falling edge
#define GPIO_MODE_IT_RT				5 //Interrupt rising edge
#define GPIO_MODE_IT_RFT			6 //Interrupt rising falling edge

/** @GPIO_PIN_OP_TYPE
 * GPIO pin possible output types
 */
#define GPIO_OP_TYPE_PP				0 //Push-pull
#define GPIO_OP_TYPE_OD				1 //Open drain

/** @GPIO_PIN_SPEED
 * GPIO pin possible output speeds
 */
#define GPIO_SPEED_LOW					0
#define GPIO_SPEED_MED					1
#define GPIO_SPEED_HIGH					2
#define GPIO_SPEED_VHIGH				3

/** @GPIO_PIN_PUPD
 * GPIO pin pull-up and pull-down configuration macros
 */
#define GPIO_NOPUPD			    	0 //No pull-up/pull-down
#define GPIO_PIN_PU					1 //Pull up
#define GPIO_PIN_PD					2 //Pull down

/** @GPIO_PIN_ALFUNMODE
 * GPIO pin possible alt function modes
 */
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

/********************************************************************************
 * 							APIs Supported by this Driver
 * 				For More info about the APIs ctrl + click function definitions
 ********************************************************************************/

// Init and De-init
void GPIO_Init(GPIO_Handle_t *pGPIOHandle);
void GPIO_DeInit(GPIO_RegDef_t *pGPIOx);

//peripheral clock setup
void GPIO_PeriClockControl(GPIO_RegDef_t *pGPIOx, uint8_t EnorDi);

//Read and write and toggle
uint8_t GPIO_ReadFromInputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber);
uint16_t GPIO_ReadFromInputPort(GPIO_RegDef_t *pGPIOx);
void GPIO_WriteToOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber, uint8_t value);
void GPIO_WriteToOutputPort(GPIO_RegDef_t *pGPIOx, uint16_t Value);
void GPIO_ToggleOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber);

//Interrupt handling and configuration
void GPIO_IRQITConfig(uint8_t IRQ_Number, uint8_t EnorDi);
void GPIO_IRQPriorityConfig(uint8_t IRQ_Number, uint32_t IRQPriority);
void GPIO_IRQHandling(uint8_t PinNumber);

//macro for alfun low max pin number
#define HIGHEST_PIN_ALFUN_LOW_REG 	7

#endif /* INC_STM32F407XX_GPIO_DRIVER_H_ */
