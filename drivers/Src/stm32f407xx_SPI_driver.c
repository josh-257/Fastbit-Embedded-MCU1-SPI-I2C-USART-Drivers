/*
 * stm32f407xx_SPI_driver.c
 *
 *  Created on: 15 May 2026
 *      Author: joshb
 */
#include <stm32f407xx_SPI_driver.h>

static void spi_txe_interrupt_handle(SPI_Handle_t *pHandle);
static void spi_rxne_interrupt_handle(SPI_Handle_t *pHandle);
static void spi_ovr_err_interrupt_handle(SPI_Handle_t *pHandle);


/*******************************************************
 * @function name	 	- SPI_PeriClockControl
 *
 * @brief				- Function enables or disables the relevant
 * 						  peripheral bus for the SPI peripheral entered.
 *
 * @param[in]			- Pointer to the SPI register definition struct
 * @param[in]			- 1 byte integer set to either 1 (ENABLE) or 0
 * 						  (DISABLE).
 *
 * @return				- none
 *
 * @note				- none
 */
void SPI_PeriClockControl(SPI_RegDef_t *pSPIx, uint8_t EnorDi)
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


/*******************************************************
 * @function name	 	- SPI_Init
 *
 * @brief				- Function to initialise all registers of given SPI
 * 						  peripheral.
 *
 * @param[in]			- Pointer to SPI handle structure.
 *
 * @return				- none
 *
 * @note				- none
 */
void SPI_Init(SPI_Handle_t *pSPIHandle){

	//enable peripheral clock
	SPI_PeriClockControl(pSPIHandle->pSPIx, ENABLE);

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

/*******************************************************
 * @function name	 	- SPI_DeInit
 *
 * @brief				- This function resets values of selected
 * 						  peripheral register to 0;
 *
 * @param[in]			- Pointer to the SPI register definition structure
 *
 * @return				- none
 *
 * @note				- none
 */
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

/*******************************************************
 * @function name	 	- SPI_Get_Flag_Status
 *
 * @brief				- This function returns 1(SET) or 0(RESET) if the
 * 						  specified flag inside the SPI status register
 * 						  is set or not set.
 *
 * @param[in]			- Pointer to the SPI register definition structure
 * @param[in]			- Integer value corresponding to flag to check
 * 						  inside status register.
 *
 * @return				- 8 bit integer.
 *
 * @note				- none
 */
uint8_t SPI_Get_Flag_Status(SPI_RegDef_t *pSPIx, uint32_t FlagName){
	if(pSPIx->SR & (1 << FlagName)){
		return FLAG_SET;
	}
	return FLAG_RESET;
}

/*******************************************************
 * @function name	 	- SPI_Send
 *
 * @brief				- This function sends data over the SPI peripheral
 * 						  from the data inside the txbuffer up to length len.
 *
 * @param[in]			- Pointer to the SPI register definition structure
 * @param[in]			- Pointer to the transfer buffer containing the data
 * 						  to send.
 * @param[in]			- 32 bit integer specifying the number of bytes to be
 * 						  transmitted.
 *
 * @return				- none
 *
 * @note				- none
 */
void SPI_Send(SPI_RegDef_t *pSPIx, uint8_t *pTxBuffer, uint32_t len){

	while(len > 0){

		//wait until TXE is set
		while(SPI_Get_Flag_Status(pSPIx, SPI_SR_TXE) == FLAG_RESET);

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


/*******************************************************
 * @function name	 	- SPI_Receive
 *
 * @brief				- This function receives data sent from another device,
 * 						  the specified number of bytes is then read from the
 * 						  rx buffer.
 *
 * @param[in]			- Pointer to the SPI register definition structure
 * @param[in]			- Pointer to the rx buffer containing the data
 * 						  to be read.
 * @param[in]			- 32 bit integer specifying the number of bytes to
 * 						  read.
 *
 * @return				- none
 *
 * @note				- none
 */
void SPI_Receive(SPI_RegDef_t *pSPIx, uint8_t *pRxBuffer, uint32_t len){

	while(len > 0){


		while(SPI_Get_Flag_Status(SPI2, SPI_SR_BSY) == FLAG_SET);

		//check the DFF bit in CR1
		if(pSPIx->CR1 & (1 << SPI_CR1_DFF)){

			//16 bit dff
			//load buffer from dr to rxbuffer address
			*((uint16_t*)pRxBuffer) = pSPIx->DR;
			if(len >= 2) len -= 2;
			else len = 0;
			pRxBuffer += 2;
		}
		else{
			*pRxBuffer = pSPIx->DR;
			len--;
			pRxBuffer++;
		}
	}
}


//IRQ configuration and ISR handling
/*******************************************************
 * @function name	 	- This function sets or clears the
 * 						  given IRQ number for the specific
 *
 *
 * @brief				-
 *
 * @param[in]			-
 * @param[in]			-
 * @param[in]			-
 *
 * @return				- none
 *
 * @note				- none
 */
void SPI_IRQITConfig(uint8_t IRQ_Number, uint8_t EnorDi){
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

/*******************************************************
 * @function name	 	-
 *
 * @brief				-
 *
 * @param[in]			-
 * @param[in]			-
 * @param[in]			-
 *
 * @return				- none
 *
 * @note				- none
 */
void SPI_IRQPriorityConfig(uint8_t IRQNumber,uint32_t IRQPriority)
{
	//1. first lets find out the ipr register
	uint8_t iprx = IRQNumber / 4;
	uint8_t iprx_section  = IRQNumber %4 ;

	uint8_t shift_amount = ( 8 * iprx_section) + ( 8 - NO_PR_BITS_IMPLEMENTED) ;

	*(  NVIC_PR_BASE_ADDR + iprx ) |=  ( IRQPriority << shift_amount );

}

/*******************************************************
 * @function name	 	-
 *
 * @brief				-
 *
 * @param[in]			-
 * @param[in]			-
 * @param[in]			-
 *
 * @return				- none
 *
 * @note				- none
 */


/*******************************************************
 * @function name	 	- SPI_PeripheralControl
 *
 * @brief				- Sets SPE bit of CR1 register to 0(disable
 * 						  peripheral) or 1(enable peripheral).
 *
 * @param[in]			- Pointer to the SPI register definition structure
 * @param[in]			- 8 bit integer set to 1(enable) or 0(disable)
 *
 * @return				- none
 *
 * @note				- none
 */
void SPI_PeripheralControl(SPI_RegDef_t *pSPIx, uint8_t EnOrDi){
	if(EnOrDi == ENABLE){
		pSPIx->CR1 |= (1 << SPI_CR1_SPE);
	}
	else{
		pSPIx->CR1 &= ~(1 << SPI_CR1_SPE);
	}
}


/*******************************************************
 * @function name	 	- SPI_SSIConfig
 *
 * @brief				- Enables or disables internal slave sele
 *
 * @param[in]			- Pointer to the SPI register definition structure
 * @param[in]			- 8 bit integer set to 1(enable) or 0(disable)
 *
 * @return				- none
 *
 * @note				- none
 */
void SPI_SSIConfig(SPI_RegDef_t *pSPIx, uint8_t EnOrDi){
	if(EnOrDi == ENABLE){
			pSPIx->CR1 |= (1 << SPI_CR1_SSI);
		}
		else{
			pSPIx->CR1 &= ~(1 << SPI_CR1_SSI);
		}
}

/*******************************************************
 * @function name	 	- SPI_SSOEConfig
 *
 * @brief				- Enables or disables the slave select output enable (SSOE)
 * 						  allowing for NSS pin to automatically be pulled low when
 * 						  the SPI peripheral is enabled and released when the SPI
 * 						  peripheral is disabled.
 *
 * @param[in]			- Pointer to the SPI register definition structure
 * @param[in]			- 8 bit integer set to 1(enable) or 0(disable)
 *
 * @return				- none
 *
 * @note				- This must be set when using SPI in Master mode with
 *                        Hardware Slave Management (SSM = 0) to allow the
 *                        hardware to automatically drive the physical NSS pin.
 */
void SPI_SSOEConfig(SPI_RegDef_t *pSPIx, uint8_t EnOrDi){

	if(EnOrDi == ENABLE){
			pSPIx->CR2 |= (1 << SPI_CR2_SSOE);
		}
		else{
			pSPIx->CR2 &= ~(1 << SPI_CR2_SSOE);
		}
}

uint8_t SPI_SendIT(SPI_Handle_t *pSPIHandle, uint8_t *pTxBuffer, uint32_t len){

	uint8_t state = pSPIHandle->TxState;

	if(state != SPI_BUSY_IN_TX){

		//1. Save the Tx buffer address and Len information in some global variables
		pSPIHandle->pTxbuffer = pTxBuffer;
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








uint8_t SPI_ReceiveIT(SPI_Handle_t *pSPIHandle, uint8_t *pRxBuffer, uint32_t len){

	uint8_t state = pSPIHandle->RxState;

		if(state != SPI_BUSY_IN_RX){

			//1. Save the Rx buffer address and Len information in some global variables
			pSPIHandle->pRxbuffer = pRxBuffer;
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

void SPI_IRQHandling(SPI_Handle_t *pHandle){

	uint8_t temp1, temp2;

	//first lets check for TXE
	temp1 = pHandle->pSPIx->SR & (1 << SPI_SR_TXE);
	temp2 = pHandle->pSPIx->CR2 & (1 << SPI_CR2_TXEIE);

	if(temp1 && temp2){

		//handle TXE
		spi_txe_interrupt_handle(pHandle);
	}

	//2 check for RXNE
	temp1 = pHandle->pSPIx->SR & (1 << SPI_SR_RXNE);
	temp2 = pHandle->pSPIx->CR2 & (1 << SPI_CR2_RXNEIE);

	if(temp1 && temp2){

			//handle RXNE
			spi_rxne_interrupt_handle(pHandle);
	}

	//3 check for ovr flag
	temp1 = pHandle->pSPIx->SR & (1 << SPI_SR_OVR);
	temp2 = pHandle->pSPIx->CR2 & (1 << SPI_CR2_ERRIE);

	if(temp1 && temp2){

			//handle RXNE
			spi_ovr_err_interrupt_handle(pHandle);
	}
}


//some helper function implementations
static void spi_txe_interrupt_handle(SPI_Handle_t *pSPIHandle){

	//check the DFF bit in CR1
	if((pSPIHandle->pSPIx->CR1 & (1 << SPI_CR1_DFF))){

		//16 bit dff
		pSPIHandle->pSPIx->DR = *((uint16_t*)pSPIHandle->pTxbuffer);
		pSPIHandle->Txlen--;
		pSPIHandle->Txlen--;
		(uint16_t*)pSPIHandle->pTxbuffer++;
	}
	else{
		pSPIHandle->pSPIx->DR = *pSPIHandle->pTxbuffer;
		pSPIHandle->Txlen--;
		pSPIHandle->pTxbuffer++;
	}
	if(!pSPIHandle->Txlen){

		//TxLen zero, so close the spi transmission and inform the application
		//that TX is over

		//this prevents interrupts from setting up of TXE flag
		SPI_Close_Transmission(pSPIHandle);
		SPI_Application_Event_Callback(pSPIHandle, SPI_EVENT_TX_CMPLT);
	}

}


static void spi_rxne_interrupt_handle(SPI_Handle_t *pSPIHandle){


		//check the DFF bit in CR1
		if(pSPIHandle->pSPIx->CR1 & (1 << SPI_CR1_DFF)){

			//16 bit dff
			*((uint16_t*)pSPIHandle->pRxbuffer) = (uint16_t)pSPIHandle->pSPIx->DR;
			pSPIHandle->Rxlen -= 2;
			pSPIHandle->pRxbuffer++;
			pSPIHandle->pRxbuffer++;
		}
		else{
			*(pSPIHandle->pRxbuffer) = (uint8_t)pSPIHandle->pSPIx->DR;
			pSPIHandle->Rxlen--;
			pSPIHandle->pRxbuffer++;
		}
		if(!pSPIHandle->Rxlen){

			//RxLen zero, so close the spi transmission and inform the application
			//that RX is over

			//this prevents interrupts from setting up of RXNE flag
			SPI_Close_Reception(pSPIHandle);
			SPI_Application_Event_Callback(pSPIHandle, SPI_EVENT_RX_CMPLT);
		}
}


static void spi_ovr_err_interrupt_handle(SPI_Handle_t *pSPIHandle){

	uint8_t temp;
	//clear the ovr flag
	if(pSPIHandle->TxState != SPI_BUSY_IN_TX){
		temp = pSPIHandle->pSPIx->DR;
		temp = pSPIHandle->pSPIx->SR;
	}
	(void)temp;

	//inform the application
	SPI_Application_Event_Callback(pSPIHandle, SPI_EVENT_OVR_ERR);

}

void SPI_Clear_OVR_Flag(SPI_RegDef_t *pSPIx){

	uint8_t temp;
	temp = pSPIx->DR;
	temp = pSPIx->SR;
	(void)temp;
}



void SPI_Close_Transmission(SPI_Handle_t *pSPIHandle){

	pSPIHandle->pSPIx->CR2 &= ~(1 << SPI_CR2_TXEIE);
	pSPIHandle->pTxbuffer = NULL;
	pSPIHandle->Txlen = 0;
	pSPIHandle->TxState = SPI_READY;
}

void SPI_Close_Reception(SPI_Handle_t *pSPIHandle){

	pSPIHandle->pSPIx->CR2 &= ~(1 << SPI_CR2_RXNEIE);
	pSPIHandle->pRxbuffer = NULL;
	pSPIHandle->Rxlen = 0;
	pSPIHandle->RxState = SPI_READY;
}


__weak void SPI_Application_Event_Callback(SPI_Handle_t *pSPIHandle, uint8_t AppEv){

	//This is a weak implementation. The application may override this function

}
