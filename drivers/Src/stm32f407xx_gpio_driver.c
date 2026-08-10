/*
 * stm32f407xx_gpio_driver.c
 *
 *  Created on: 11 May 2026
 *      Author: joshb
 */

#include "stm32f407xx_gpio_driver.h"


void GPIO_Init(GPIO_Handle_t *pGPIOHandle)
{
	uint32_t tmp = 0;

	//Enable clock for GPIO pin
	GPIO_PeripheralClockControl(pGPIOHandle->pGPIOx, ENABLE);

	//Configure mode of GPIO pin (each pin's mode occupies 2 bits therefore multiply by 2)
	if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode <= GPIO_MODE_ANALOG)
	{
		tmp = (pGPIOHandle->GPIO_PinConfig.GPIO_PinMode << (2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
		pGPIOHandle->pGPIOx->MODER &= ~(0x3 << (2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
		pGPIOHandle->pGPIOx->MODER |= tmp;
	}
	else
	{
		//Interrupt mode
		if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_IT_FT)
		{
			//Configure the falling trigger selection register
			EXTI->FTSR |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
			//Clear the corresponding RTSR bit
			EXTI->RTSR &= ~(1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
		}
		else if (pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_IT_RT)
		{
			//Configure the rising trigger selection register
			EXTI->RTSR |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
			//Clear the corresponding FTSR bit
			EXTI->FTSR &= ~(1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
		}
		else if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_IT_RFT)
		{
			//Configure both
			EXTI->RTSR |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
			EXTI->FTSR |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
		}
		//Configure the GPIO port selection in SYSCFG_EXTICR
		uint8_t port_code = GPIO_BASE_ADDR_TO_CODE(pGPIOHandle->pGPIOx);
		uint8_t tmp1 = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber / 4;
		uint8_t tmp3 = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber % 4;
		SYSCFG_PCLK_EN();
		SYSCFG->EXTICR[tmp1] |= (port_code << (4 * tmp3));

		//Enable the exti interrupt delivery using IMR
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

void GPIO_DeInit(GPIO_RegDef_t *pGPIOx)
{
	if(pGPIOx == GPIOA)
		{
			GPIOA_REG_RESET();
		}
	else if (pGPIOx == GPIOB)
		{
			GPIOB_REG_RESET();
		}
	else if (pGPIOx == GPIOC)
		{
			GPIOC_REG_RESET();
		}
	else if (pGPIOx == GPIOD)
		{
			GPIOD_REG_RESET();
		}
	else if (pGPIOx == GPIOE)
		{
			GPIOE_REG_RESET();
		}
	else if (pGPIOx == GPIOF)
		{
			GPIOF_REG_RESET();
		}
	else if (pGPIOx == GPIOG)
		{
			GPIOG_REG_RESET();
		}
	else if (pGPIOx == GPIOH)
		{
			GPIOH_REG_RESET();
		}
	else if (pGPIOx == GPIOI)
		{
			GPIOI_REG_RESET();
		}
}


void GPIO_PeripheralClockControl(GPIO_RegDef_t *pGPIOx, uint8_t EnorDi)
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

uint8_t GPIO_ReadFromInputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber)
{

	uint8_t value;

	value = (uint8_t)((pGPIOx->IDR >> PinNumber) & 0x00000001);

	return value;

}

uint16_t GPIO_ReadFromInputPort(GPIO_RegDef_t *pGPIOx)
{

	uint16_t value;

	value = (uint16_t)pGPIOx->IDR;

	return value;
}

void GPIO_WriteToOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber, uint8_t value)
{

	if(value == GPIO_PIN_SET)
	{

		pGPIOx->ODR |= (1 << PinNumber);
	}
	else
	{
		pGPIOx->ODR &= ~(1 << PinNumber);
	}

}


void GPIO_WriteToOutputPort(GPIO_RegDef_t *pGPIOx, uint16_t value)
{
	pGPIOx->ODR = value;
}

void GPIO_ToggleOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber)
{
	pGPIOx->ODR ^= (1 << PinNumber);
}

void GPIO_IRQITConfig(uint8_t IRQ_Number, uint8_t EnorDi){
	if(EnorDi == ENABLE)
	{
		if(IRQ_Number <= 31)
		{
			*NVIC_ISER0 |= (1 << IRQ_Number);
		}
		else if(IRQ_Number > 31 && IRQ_Number < 64)
		{
			*NVIC_ISER1 |= (1 << IRQ_Number % 32);
		}
		else if(IRQ_Number > 64 && IRQ_Number < 96)
		{
			*NVIC_ISER2 |= (1 << IRQ_Number % 32);
		}
	}
	else
	{
		if(IRQ_Number <= 31)
		{
			*NVIC_ICER0 |= (1 << IRQ_Number);
		}
		else if(IRQ_Number > 31 && IRQ_Number < 64)
		{
			*NVIC_ICER1 |= (1 << IRQ_Number % 32);
		}
		else if(IRQ_Number > 64 && IRQ_Number < 96)
		{
			*NVIC_ICER2 |= (1 << IRQ_Number % 32);
		}
	}
}

void GPIO_IRQPriorityConfig(uint8_t IRQ_Number, uint32_t IRQPriority)
{
	//Select the correct register where the IRQ sits
	uint8_t iprx  = IRQ_Number / 4;
	//Select the correct field of this register
	uint8_t iprx_section  = IRQ_Number % 4;
	/*
	 *	Note: In Arm-Cortex M4 each field of IPR has 8 bits to configure
	 *	priority level. However STM32F407 only uses the highest 4 bits.
	 *	Therefore the priority number must be shifted up to the highest
	 *	4 bits as well as accounting for 8 bits per field.
	 */
	uint8_t shift_amount = (8 * iprx_section) + (8 - NO_PR_BITS_IMPLEMENTED);
	*(NVIC_PR_BASE_ADDR + iprx) |= (IRQPriority << shift_amount);
}

void GPIO_IRQHandling(uint8_t PinNumber)
{
	//clear the exti pr register corresponding to the pin number
	if(EXTI->PR & (1 << PinNumber))
	{
		//Clear by programming to 1
		EXTI->PR |= (1 << PinNumber);
	}
}
