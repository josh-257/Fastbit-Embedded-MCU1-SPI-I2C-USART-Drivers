/*
 * stm32f407xx_gpio_driver.c
 *
 *  Created on: 11 May 2026
 *      Author: joshb
 */

#include "stm32f407xx_gpio_driver.h"

/*******************************************************
 * @function name					- GPIO_Init
 *
 * @brief				- This function configures GPIO registers for specific
 * 						port selected.
 *
 * @param[in]			- pointer of type struct GPIO_Handle_t
 * @param[in]			- ENABLE or DISABLE macros
 * @param[in]			-
 *
 * @return				- none
 *
 * @note				- none
 */
void GPIO_Init(GPIO_Handle_t *pGPIOHandle){

	uint32_t tmp = 0;

	//enable clock for GPIO pin
	GPIO_PeriClockControl(pGPIOHandle->pGPIOx, ENABLE);

	//Configure mode of GPIO pin
	if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode <= GPIO_MODE_ANALOG){
		tmp = (pGPIOHandle->GPIO_PinConfig.GPIO_PinMode << (2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
		pGPIOHandle->pGPIOx->MODER &= ~(0x3 << (2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
		pGPIOHandle->pGPIOx->MODER |= tmp;

	}
	else{
		//interrupt mode
		if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_IT_FT){
			//configure the falling trigger selection register
			EXTI->FTSR |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
			// clear the corresponding RTSR bit
			EXTI->RTSR &= ~(1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
		}
		else if (pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_IT_RT){
			//configure the rising trigger selection register
			EXTI->RTSR |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
			// clear the corresponding FTSR bit
			EXTI->FTSR &= ~(1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
		}
		else if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_IT_RFT){
			//configure both
			EXTI->RTSR |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
			EXTI->FTSR |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
		}
		// configure the GPIO port selection in SYSCFG_EXTICR
		uint8_t port_code = GPIO_BASE_ADDR_TO_CODE(pGPIOHandle->pGPIOx);
		uint8_t tmp1 = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber / 4;
		uint8_t tmp3 = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber % 4;
		SYSCFG_PCLK_EN();
		SYSCFG->EXTICR[tmp1] |= (port_code << (4 * tmp3));

		// enable the exti interrupt delivery using IMR
		EXTI->IMR |= 1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber;
	}
	//Configure speed
	tmp = (pGPIOHandle->GPIO_PinConfig.GPIO_PinSpeed << (2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
	pGPIOHandle->pGPIOx->OSPEEDR &= ~(0x3 << (2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
	pGPIOHandle->pGPIOx->OSPEEDR |= tmp;

	//Configure pupd settings
	tmp = (pGPIOHandle->GPIO_PinConfig.GPIO_PinPuPdControl << (2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
	pGPIOHandle->pGPIOx->PUPDR &= ~(0x3 << (2 *pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
	pGPIOHandle->pGPIOx->PUPDR |= tmp;

	//Configure output type
	tmp = (pGPIOHandle->GPIO_PinConfig.GPIO_PinOPType << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
	pGPIOHandle->pGPIOx->OTYPER &= ~(0x1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
	pGPIOHandle->pGPIOx->OTYPER |= tmp;

	//Configure alt functionality
	if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_ALTFN)
		{
			//configure the alt function registers.
			uint8_t temp1, temp2;

			temp1 = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber / 8;
			temp2 = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber  % 8;
			pGPIOHandle->pGPIOx->AFR[temp1] &= ~(0xF << ( 4 * temp2 ) ); //clearing
			pGPIOHandle->pGPIOx->AFR[temp1] |= (pGPIOHandle->GPIO_PinConfig.GPIO_AltFunMode << ( 4 * temp2 ) );
		}
}



/*********************************************************************
 * @function name     - GPIO_DeInit
 *
 * @brief             - This function resets the RCC for the chosen port.
 *
 * @param[in]         - Pointer to GPIO register definition structure.
 *
 * @return            - none
 *
 * @Note              - none

 */
void GPIO_DeInit(GPIO_RegDef_t *pGPIOx){
	if(pGPIOx == GPIOA)
		{
			GPIOA_REG_RESET();
		}else if (pGPIOx == GPIOB)
		{
			GPIOB_REG_RESET();
		}else if (pGPIOx == GPIOC)
		{
			GPIOC_REG_RESET();
		}else if (pGPIOx == GPIOD)
		{
			GPIOD_REG_RESET();
		}else if (pGPIOx == GPIOE)
		{
			GPIOE_REG_RESET();
		}else if (pGPIOx == GPIOF)
		{
			GPIOF_REG_RESET();
		}else if (pGPIOx == GPIOG)
		{
			GPIOG_REG_RESET();
		}else if (pGPIOx == GPIOH)
		{
			GPIOH_REG_RESET();
		}else if (pGPIOx == GPIOI)
		{
			GPIOI_REG_RESET();
		}
}


/*******************************************************
 * @function name					- GPIO_PeriClockControl
 *
 * @brief				- this function enables or disables the
 * 						peripheral clock for the given GPIO port
 *
 * @param[in]			- base address of the gpio peripheral
 * @param[in]			- ENABLE or DISABLE macros
 * @param[in]			-
 *
 * @return				- none
 *
 * @note				- none
 */
void GPIO_PeriClockControl(GPIO_RegDef_t *pGPIOx, uint8_t EnorDi)
{
    if (EnorDi == ENABLE)
    {
        if (pGPIOx == GPIOA)
        {
            GPIOA_PCLK_EN();
        }
        else if (pGPIOx == GPIOB)
        {
            GPIOB_PCLK_EN();
        }
        else if (pGPIOx == GPIOC)
        {
            GPIOC_PCLK_EN();
        }
        else if (pGPIOx == GPIOD)
        {
            GPIOD_PCLK_EN();
        }
        else if (pGPIOx == GPIOE)
        {
            GPIOE_PCLK_EN();
        }
        else if (pGPIOx == GPIOF)
        {
            GPIOF_PCLK_EN();
        }
        else if (pGPIOx == GPIOG)
        {
            GPIOG_PCLK_EN();
        }
        else if (pGPIOx == GPIOH)
        {
            GPIOH_PCLK_EN();
        }
        else if (pGPIOx == GPIOI)
        {
            GPIOI_PCLK_EN();
        }
    }
    else
    {
        if (pGPIOx == GPIOA)
        {
            GPIOA_PCLK_DI();
        }
        else if (pGPIOx == GPIOB)
        {
            GPIOB_PCLK_DI();
        }
        else if (pGPIOx == GPIOC)
        {
            GPIOC_PCLK_DI();
        }
        else if (pGPIOx == GPIOD)
        {
            GPIOD_PCLK_DI();
        }
        else if (pGPIOx == GPIOE)
        {
            GPIOE_PCLK_DI();
        }
        else if (pGPIOx == GPIOF)
        {
            GPIOF_PCLK_DI();
        }
        else if (pGPIOx == GPIOG)
        {
            GPIOG_PCLK_DI();
        }
        else if (pGPIOx == GPIOH)
        {
            GPIOH_PCLK_DI();
        }
        else if (pGPIOx == GPIOI)
        {
            GPIOI_PCLK_DI();
        }
    }
}



/*******************************************************
 * @function name		- GPIO_ReadFromInputPin
 *
 * @brief				- This function returns the input from
 * 						  the selected GPIO port and pin.
 *
 * @param[in]			- Pointer to GPIO register definition structure.
 * @param[in]			- 8 bit integer identifying the pin number to be read.
 * @param[in]			-
 *
 * @return				- none
 *
 * @note				- none
 */
uint8_t GPIO_ReadFromInputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber){

	uint8_t value;

	value = (uint8_t)((pGPIOx->IDR >> PinNumber) & 0x00000001);

	return value;

}


/*******************************************************
 * @function name		- GPIO_ReadFromInputPort
 *
 * @brief				- This function reads the entire input
 * 						  data register for the specified port.
 *
 * @param[in]			- Pointer to GPIO register definition structure.
 *
 * @return				- 16 bit integer representing the state of all bits
 * 						  in the input data register.
 *
 * @note				- none
 */
uint16_t GPIO_ReadFromInputPort(GPIO_RegDef_t *pGPIOx){

	uint16_t value;

	value = (uint16_t)pGPIOx->IDR;

	return value;
}


/*******************************************************
 * @function name	 	- GPIO_WriteToOutputPin
 *
 * @brief				- Writes the specified value to the
 * 						  chosen output pin of the output port in the
 * 						  output data register.
 *
 * @param[in]			- Pointer to GPIO register definition structure.
 * @param[in]			- 8 bit integer specifying the pin number.
 * @param[in]			- 8 bit integer specifying whether to set(1) or reset(0)
 *
 * @return				- none
 *
 * @note				- none
 */

void GPIO_WriteToOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber, uint8_t value){

	if(value == GPIO_PIN_SET){

		//write 1 to the output data register at the bit field corresponding to the pin
		pGPIOx->ODR |= (1 << PinNumber);
	}
	else{
		pGPIOx->ODR &= ~(1 << PinNumber);
	}

}

/*******************************************************
 * @function name		- GPIO_WriteToOutputPort
 *
 * @brief				- This function writes the specified 16 bit value to
 * 						  the chosen GPIO port's output data register.
 *
 * @param[in]			- Base address of the GPIO port
 * @param[in]			- 16 bit integer specifying the value to be written.
 *
 * @return				- none
 *
 * @note				- none
 */
void GPIO_WriteToOutputPort(GPIO_RegDef_t *pGPIOx, uint16_t Value){
	pGPIOx->ODR = Value;
}

/*******************************************************
 * @function name		- GPIO_WriteToOutputPort
 *
 * @brief				- This function toggles the specified pin of the
 * 						  chosen GPIO port.
 *
 * @param[in]			- Base address of the GPIO port
 * @param[in]			- 8 bit integer specifying the pin number.
 *
 * @return				- none
 *
 * @note				- none
 */
void GPIO_ToggleOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber){
	pGPIOx->ODR ^= (1 << PinNumber);
}

/*******************************************************
 * @function name					- GPIO_PeriClockControl
 *
 * @brief				- this function enables or disables the
 * 						peripheral clock for the given GPIO port
 *
 * @param[in]			- base address of the gpio peripheral
 * @param[in]			- ENABLE or DISABLE macros
 * @param[in]			-
 *
 * @return				- none
 *
 * @note				- none
 */
void GPIO_IRQITConfig(uint8_t IRQ_Number, uint8_t EnorDi){
	if(EnorDi == ENABLE){
		if(IRQ_Number <= 31){
			//program ISER0 register
			*NVIC_ISER0 |= (1 << IRQ_Number);
		}
		else if(IRQ_Number > 31 && IRQ_Number < 64){
			//program ISER1 register
			*NVIC_ISER1 |= (1 << IRQ_Number % 32);
		}
		else if(IRQ_Number > 64 && IRQ_Number < 96){
			//program ISER2 register
			*NVIC_ISER2 |= (1 << IRQ_Number % 32);
		}
	}
	else{
		if(IRQ_Number <= 31){
			//program ICER0 register
			*NVIC_ICER0 |= (1 << IRQ_Number);
		}
		else if(IRQ_Number > 31 && IRQ_Number < 64){
			//program ICER1 register
			*NVIC_ICER1 |= (1 << IRQ_Number % 32);
		}
		else if(IRQ_Number > 64 && IRQ_Number < 96){
			//program ICER2 register
			*NVIC_ICER2 |= (1 << IRQ_Number % 32);
		}
	}
}

void GPIO_IRQPriorityConfig(uint8_t IRQ_Number, uint32_t IRQPriority){
	//first find out the interrupt priority register
	uint8_t iprx  = IRQ_Number / 4;
	uint8_t iprx_section  = IRQ_Number % 4;
	uint8_t shift_amount = (8 * iprx_section) + (8 - NO_PR_BITS_IMPLEMENTED);
	*(NVIC_PR_BASE_ADDR + iprx) |= (IRQPriority << shift_amount);
}

/*******************************************************
 * @function name		- GPIO_PeriClockControl
 *
 * @brief				- this function enables or disables the
 * 						peripheral clock for the given GPIO port
 *
 * @param[in]			- base address of the gpio peripheral
 * @param[in]			- ENABLE or DISABLE macros
 * @param[in]			-
 *
 * @return				- none
 *
 * @note				- none
 */
void GPIO_IRQHandling(uint8_t PinNumber){
	//clear the exti pr register corresponding to the pin number
	if(EXTI->PR & (1 << PinNumber)){
		//clear
		EXTI->PR |= (1 << PinNumber);
	}
}
