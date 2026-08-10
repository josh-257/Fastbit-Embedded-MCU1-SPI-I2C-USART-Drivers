/*
 * stm32f407xx_SPI_driver.c
 *
 *  Created on: 15 May 2026
 *      Author: joshb
 */
#include <stm32f407xx_SPI_driver.h>

static void SPI_TxeInterruptHandle(SPI_Handle_t *pHandle);
static void SPI_RxneInterruptHandle(SPI_Handle_t *pHandle);
static void SPI_OvrErrInterruptHandle(SPI_Handle_t *pHandle);

void SPI_PeripheralClockControl(SPI_RegDef_t *pSPIx, uint8_t EnorDi)
{
    if(EnorDi == ENABLE)
    {
        if(pSPIx == SPI1)
        {
            SPI1_PCLK_EN();
        }
        else if(pSPIx == SPI2)
        {
            SPI2_PCLK_EN();
        }
        else if(pSPIx == SPI3)
        {
            SPI3_PCLK_EN();
        }
    }
    else
    {
        if(pSPIx == SPI1)
        {
            SPI1_PCLK_DI();
        }
        else if(pSPIx == SPI2)
        {
            SPI2_PCLK_DI();
        }
        else if(pSPIx == SPI3)
        {
            SPI3_PCLK_DI();
        }
    }
}

void SPI_Init(SPI_Handle_t *pSPIHandle){

	//enable peripheral clock
	SPI_PeripheralClockControl(pSPIHandle->pSPIx, ENABLE);

	//first configure the SPI_CR1 register
	uint32_t tempreg = 0;

	//1. configure the device mode
	tempreg |= pSPIHandle->SPIConfig.SPI_DeviceMode << SPI_CR1_MSTR;

	//2 configure bus mode
	if(pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CFG_FD){
		//bidi mode should be cleared
		tempreg &= ~(1 << SPI_CR1_BIDIMODE);
	}
	else if(pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CFG_HD){
		//bidi mode enabled
		tempreg |= (1 << SPI_CR1_BIDIMODE);
	}
	else if(pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CFG_SIMPLEX_RXONLY){
		//bidi mode should be cleared
		tempreg &= ~(1 << SPI_CR1_BIDIMODE);
		//RX only bit must be set
		tempreg |= (1 << SPI_CR1_RXONLY);
	}
	//3 configure clock speed
	tempreg |= pSPIHandle->SPIConfig.SPI_SclkSpeed << SPI_CR1_BAUDRATE;

	//4 configure DFF
	tempreg |= pSPIHandle->SPIConfig.SPI_DFF << SPI_CR1_DFF;

	//5 configure CPOL
	tempreg |= pSPIHandle->SPIConfig.SPI_CPOL << SPI_CR1_CPOL;

	//6 configure CPHA
	tempreg |= pSPIHandle->SPIConfig.SPI_CPHA << SPI_CR1_CPHA;

	//7 configure SSM
	tempreg |= pSPIHandle->SPIConfig.SPI_SSM << SPI_CR1_SSM;

	pSPIHandle->pSPIx->CR1 = tempreg;
}

void SPI_DeInit(SPI_RegDef_t *pSPIx){
	if(pSPIx == SPI1){
		SPI1_REG_RESET();
	}
	else if(pSPIx == SPI2){
		SPI2_REG_RESET();
	}
	else if(pSPIx == SPI3){
		SPI3_REG_RESET();
	}
}

uint8_t SPI_GetFlagStatus(SPI_RegDef_t *pSPIx, uint32_t FlagName)
{
	if(pSPIx->SR & FlagName){
		return FLAG_SET;
	}
	return FLAG_RESET;
}

void SPI_Send(SPI_RegDef_t *pSPIx, uint8_t *pTxBuffer, uint32_t len){

	while(len > 0){

		//wait until TXE is set
		while(SPI_GetFlagStatus(pSPIx, SPI_SR_TXE) == FLAG_RESET);

		//check the DFF bit in CR1
		if((pSPIx->CR1 & (1 << SPI_CR1_DFF))){

			//16 bit dff
			pSPIx->DR = *((uint16_t*)pTxBuffer);
			if(len >= 2) len -= 2;
			else len = 0;
			pTxBuffer += 2;
		}
		else{
			pSPIx->DR = *pTxBuffer;
			len--;
			pTxBuffer++;
		}

	}
}

void SPI_Receive(SPI_RegDef_t *pSPIx, uint8_t *pRxBuffer, uint32_t len)
{

	while(len > 0)
	{
		while(SPI_GetFlagStatus(SPI2, SPI_SR_BSY) == FLAG_SET);

		//check the DFF bit in CR1
		if(pSPIx->CR1 & (1 << SPI_CR1_DFF))
		{
			//16 bit dff
			//load buffer from dr to rxbuffer address
			*((uint16_t*)pRxBuffer) = pSPIx->DR;
			if(len >= 2) len -= 2;
			else len = 0;
			pRxBuffer += 2;
		}
		else
		{
			*pRxBuffer = pSPIx->DR;
			len--;
			pRxBuffer++;
		}
	}
}

void SPI_IRQITConfig(uint8_t IRQ_Number, uint8_t EnorDi)
{
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

void SPI_IRQPriorityConfig(uint8_t IRQNumber,uint32_t IRQPriority)
{
	//1. first lets find out the ipr register
	uint8_t iprx = IRQNumber / 4;
	uint8_t iprx_section  = IRQNumber %4 ;

	uint8_t shift_amount = ( 8 * iprx_section) + ( 8 - NO_PR_BITS_IMPLEMENTED) ;

	*(  NVIC_PR_BASE_ADDR + iprx ) |=  ( IRQPriority << shift_amount );

}

void SPI_PeripheralControl(SPI_RegDef_t *pSPIx, uint8_t EnOrDi)
{
	if(EnOrDi == ENABLE)
	{
		pSPIx->CR1 |= (1 << SPI_CR1_SPE);
	}
	else
	{
		pSPIx->CR1 &= ~(1 << SPI_CR1_SPE);
	}
}

void SPI_SSIConfig(SPI_RegDef_t *pSPIx, uint8_t EnOrDi)
{
	if(EnOrDi == ENABLE)
	{
		pSPIx->CR1 |= (1 << SPI_CR1_SSI);
	}
	else
	{
		pSPIx->CR1 &= ~(1 << SPI_CR1_SSI);
	}
}

void SPI_SSOEConfig(SPI_RegDef_t *pSPIx, uint8_t EnOrDi)
{
	if(EnOrDi == ENABLE){
			pSPIx->CR2 |= (1 << SPI_CR2_SSOE);
		}
		else{
			pSPIx->CR2 &= ~(1 << SPI_CR2_SSOE);
		}
}

uint8_t SPI_SendIT(SPI_Handle_t *pSPIHandle, uint8_t *pTxBuffer, uint32_t len)
{
	uint8_t state = pSPIHandle->TxState;

	if(state != SPI_BUSY_IN_TX){

		//1. Save the Tx buffer address and Len information to handle structure
		pSPIHandle->pTxBuffer = pTxBuffer;
		pSPIHandle->Txlen = len;

		//2. Mark the SPI state as busy in transmission so that no other code
		//   can take over same SPI peripheral until transmission is over
		pSPIHandle->TxState = SPI_BUSY_IN_TX;

		//3. Enable the TXEIE control bit to get interrupt whenever TXE flag
		//	 is set in SR
		pSPIHandle->pSPIx->CR2 |= (1 << SPI_CR2_TXEIE);
	}

	return state;
}








uint8_t SPI_ReceiveIT(SPI_Handle_t *pSPIHandle, uint8_t *pRxBuffer, uint32_t len)
{
	uint8_t state = pSPIHandle->RxState;

		if(state != SPI_BUSY_IN_RX){

			//1. Save the Rx buffer address and Len information in some global variables
			pSPIHandle->pRxBuffer = pRxBuffer;
			pSPIHandle->Rxlen = len;

			//2. Mark the SPI state as busy in transmission so that no other code
			//   can take over same SPI peripheral until transmission is over
			pSPIHandle->RxState = SPI_BUSY_IN_RX;

			//3. Enable the RXNEIE control bit to get interrupt whenever TXE flag
			//	 is set in SR
			pSPIHandle->pSPIx->CR2 |= (1 << SPI_CR2_RXNEIE);
		}

		return state;
}

void SPI_IRQHandling(SPI_Handle_t *pHandle)
{
	uint8_t temp1, temp2;

	//1. Lets check for TXE
	temp1 = pHandle->pSPIx->SR & (1 << SPI_SR_TXE);
	temp2 = pHandle->pSPIx->CR2 & (1 << SPI_CR2_TXEIE);

	if(temp1 && temp2){

		//Handle TXE
		SPI_TxeInterruptHandle(pHandle);
	}

	//2. Check for RXNE
	temp1 = pHandle->pSPIx->SR & (1 << SPI_SR_RXNE);
	temp2 = pHandle->pSPIx->CR2 & (1 << SPI_CR2_RXNEIE);

	if(temp1 && temp2){

			//Handle RXNE
			SPI_RxneInterruptHandle(pHandle);
	}

	//3. Check for ovr flag
	temp1 = pHandle->pSPIx->SR & (1 << SPI_SR_OVR);
	temp2 = pHandle->pSPIx->CR2 & (1 << SPI_CR2_ERRIE);

	if(temp1 && temp2){

			//Handle RXNE
			SPI_OvrErrInterruptHandle(pHandle);
	}
}


//Some helper function implementations
static void SPI_TxeInterruptHandle(SPI_Handle_t *pSPIHandle){

	//Check the DFF bit in CR1
	if((pSPIHandle->pSPIx->CR1 & (1 << SPI_CR1_DFF)))
	{
		//16 bit dff
		pSPIHandle->pSPIx->DR = *((uint16_t*)pSPIHandle->pTxBuffer);
		pSPIHandle->Txlen--;
		pSPIHandle->Txlen--;
		(uint16_t*)pSPIHandle->pTxBuffer++;
	}
	else
	{
		pSPIHandle->pSPIx->DR = *pSPIHandle->pTxBuffer;
		pSPIHandle->Txlen--;
		pSPIHandle->pTxBuffer++;
	}
	if(!pSPIHandle->Txlen)
	{
		//TxLen zero, so close the spi transmission and inform the application
		//that TX is over, this prevents interrupts from setting up of TXE flag
		SPI_CloseTransmission(pSPIHandle);
		SPI_ApplicationEventCallback(pSPIHandle, SPI_EVENT_TX_CMPLT);
	}

}


static void SPI_RxneInterruptHandle(SPI_Handle_t *pSPIHandle)
{
		//check the DFF bit in CR1
		if(pSPIHandle->pSPIx->CR1 & (1 << SPI_CR1_DFF))
		{
			//16 bit dff
			*((uint16_t*)pSPIHandle->pRxBuffer) = (uint16_t)pSPIHandle->pSPIx->DR;
			pSPIHandle->Rxlen -= 2;
			pSPIHandle->pRxBuffer++;
			pSPIHandle->pRxBuffer++;
		}
		else
		{
			*(pSPIHandle->pRxBuffer) = (uint8_t)pSPIHandle->pSPIx->DR;
			pSPIHandle->Rxlen--;
			pSPIHandle->pRxBuffer++;
		}
		if(!pSPIHandle->Rxlen)
		{
			//RxLen zero, so close the spi transmission and inform the application
			//that RX is over, this prevents interrupts from setting up of RXNE flag
			SPI_CloseReception(pSPIHandle);
			SPI_ApplicationEventCallback(pSPIHandle, SPI_EVENT_RX_CMPLT);
		}
}


static void SPI_OvrErrInterruptHandle(SPI_Handle_t *pSPIHandle)
{
	uint8_t temp;
	//clear the ovr flag
	if(pSPIHandle->TxState != SPI_BUSY_IN_TX)
	{
		temp = pSPIHandle->pSPIx->DR;
		temp = pSPIHandle->pSPIx->SR;
	}
	(void)temp;

	//inform the application
	SPI_ApplicationEventCallback(pSPIHandle, SPI_EVENT_OVR_ERR);

}

void SPI_ClearOVRFlag(SPI_RegDef_t *pSPIx)
{
	uint8_t temp;
	temp = pSPIx->DR;
	temp = pSPIx->SR;
	(void)temp;
}



void SPI_CloseTransmission(SPI_Handle_t *pSPIHandle)
{
	pSPIHandle->pSPIx->CR2 &= ~(1 << SPI_CR2_TXEIE);
	pSPIHandle->pTxBuffer = NULL;
	pSPIHandle->Txlen = 0;
	pSPIHandle->TxState = SPI_READY;
}

void SPI_CloseReception(SPI_Handle_t *pSPIHandle)
{
	pSPIHandle->pSPIx->CR2 &= ~(1 << SPI_CR2_RXNEIE);
	pSPIHandle->pRxBuffer = NULL;
	pSPIHandle->Rxlen = 0;
	pSPIHandle->RxState = SPI_READY;
}


__weak void SPI_ApplicationEventCallback(SPI_Handle_t *pSPIHandle, uint8_t AppEv)
{
	//This is a weak implementation. The application may override this function
}
