/*
 * stm32f407xx_i2c_driver.c
 *
 *  Created on: 19 Jul 2026
 *      Author: joshb
 */

#include "stm32f407xx.h"
#include "stm32f407xx_i2c_driver.h"

//Private helper functions
static void I2C_ExecuteAddressPhaseWrite(I2C_RegDef_t *pI2Cx, uint8_t Slave_Addr);
static void I2C_ExecuteAddressPhaseRead(I2C_RegDef_t *pI2Cx, uint8_t Slave_Addr);
static void I2C_ClearADDRFlag(I2C_Handle_t *pI2CHandle);
static void I2C_MasterHandleTXEInterrupt(I2C_Handle_t *pI2CHandle);
static void I2C_MasterHandleRXNEInterrupt(I2C_Handle_t *pI2CHandle);

void I2C_GenerateStartCondition(I2C_RegDef_t *pI2Cx){

	//generate start condition
	pI2Cx->CR1 |= (1 << I2C_CR1_START);

}

static void I2C_ExecuteAddressPhaseWrite(I2C_RegDef_t *pI2Cx, uint8_t Slave_Addr){

	Slave_Addr <<= 1;
	Slave_Addr &= ~(1); //set r/w bit to write
	pI2Cx->DR = Slave_Addr;
}

static void I2C_ExecuteAddressPhaseRead(I2C_RegDef_t *pI2Cx, uint8_t Slave_Addr){

	Slave_Addr <<= 1;
	Slave_Addr |= 1; //set r/w bit to read
	pI2Cx->DR = Slave_Addr;
}


static void I2C_ClearADDRFlag(I2C_Handle_t *pI2CHandle){

	uint32_t dummy_read;

	//Check for master or slave mode
	if(pI2CHandle->pI2Cx->SR2 & (1 << I2C_SR2_MSL))
	{

		//Device is in master mode
		if(pI2CHandle->TxRxState == I2C_BUSY_IN_RX)
		{

			if(pI2CHandle->RxSize == 1){

				I2C_ToggleAcking(pI2CHandle->pI2Cx, DISABLE);

				//Clear ADDR flag by reading SR1 and SR2
				dummy_read = pI2CHandle->pI2Cx->SR1;
				dummy_read = pI2CHandle->pI2Cx->SR2;
				(void)dummy_read;
			}
		}
		else
		{

			dummy_read = pI2CHandle->pI2Cx->SR1;
			dummy_read = pI2CHandle->pI2Cx->SR2;
			(void)dummy_read;
		}
	}
	else
	{

		//Device is in slave mode
		dummy_read = pI2CHandle->pI2Cx->SR1;
		dummy_read = pI2CHandle->pI2Cx->SR2;
		(void)dummy_read;
	}

}

void I2C_GenerateStopCondition(I2C_RegDef_t *pI2Cx)
{
	//generate stop condition
	pI2Cx->CR1 |= (1 << I2C_CR1_STOP);
}

//void I2C_SlaveEnableDisableCallbackEvents(I2C_RegDef_t *pI2Cx,uint8_t EnorDi)

void I2C_PeripheralControl(I2C_RegDef_t *pI2Cx, uint8_t EnorDi)
{
	if(EnorDi == ENABLE)
	{
		pI2Cx->CR1 |= (1 << I2C_CR1_PE);
	}
	else
	{
		pI2Cx->CR1 &= ~(1 << I2C_CR1_PE);
	}
}


//Peripheral clock setup
void I2C_PeripheralClockControl(I2C_RegDef_t *pI2Cx, uint8_t EnorDi)
{
	if(EnorDi == ENABLE)
	    {
	        if(pI2Cx == I2C1)
	        {
	            I2C1_PCLK_EN();
	        }
	        else if(pI2Cx == I2C2)
	        {
	            I2C2_PCLK_EN();
	        }
	        else if(pI2Cx == I2C3)
	        {
	            I2C3_PCLK_EN();
	        }
	    }
	    else
	    {
	        if(pI2Cx == I2C1)
	        {
	            I2C1_PCLK_DI();
	        }
	        else if(pI2Cx == I2C2)
	        {
	            I2C2_PCLK_DI();
	        }
	        else if(pI2Cx == I2C3)
	        {
	            I2C3_PCLK_DI();
	        }
	    }
}

//Init and De-init
void I2C_Init(I2C_Handle_t *pI2CHandle)
{
	uint32_t tempreg = 0;

	//enable peripheral clock
	I2C_PeripheralClockControl(pI2CHandle->pI2Cx, ENABLE);

	//2. Configure speed of SCL
	tempreg = 0;
	tempreg |= RCC_GetPeripheralClockAPB1Value() / 1000000U;
	pI2CHandle->pI2Cx->CR2 = (tempreg & 0x3F);

	//3. Configure device address
	tempreg = 0;
	tempreg |= pI2CHandle->I2C_Config.I2C_DeviceAddress << 1;
	tempreg |= (1 << 14);
	pI2CHandle->pI2Cx->OAR1 = tempreg;

	//CCR calculations
	uint16_t ccr_value = 0;
	tempreg = 0;
	if(pI2CHandle->I2C_Config.I2C_SCLSpeed <= I2C_SCL_SPEED_SM)
	{
		//mode is standard mode
		ccr_value = RCC_GetPeripheralClockAPB1Value() / (2 * pI2CHandle->I2C_Config.I2C_SCLSpeed);
		tempreg |= ccr_value & 0xFFF;

	}
	else
	{
		//mode is fast mode
		tempreg |= (1 << 15);
		tempreg |= (pI2CHandle->I2C_Config.I2C_FMDutyCycle << 14);
		if(pI2CHandle->I2C_Config.I2C_FMDutyCycle == I2C_FM_DUTY_2)
		{
			ccr_value = (RCC_GetPeripheralClockAPB1Value() / (3 * pI2CHandle->I2C_Config.I2C_SCLSpeed));
		}
		else
		{
			ccr_value = (RCC_GetPeripheralClockAPB1Value() / (25 * pI2CHandle->I2C_Config.I2C_SCLSpeed));
		}
		tempreg |= (ccr_value & 0xFFF);
	}
	pI2CHandle->pI2Cx->CCR = tempreg;

	//5. Configure rise time
	if(pI2CHandle->I2C_Config.I2C_SCLSpeed <= I2C_SCL_SPEED_SM)
	{
		//mode is standard mode
		tempreg = (RCC_GetPeripheralClockAPB1Value() / 1000000U) + 1;
	}
	else
	{
		//mode is fast mode
		tempreg = ((RCC_GetPeripheralClockAPB1Value() * 300) / 1000000000U) + 1;
	}

	pI2CHandle->pI2Cx->TRISE = tempreg & 0x3F;

}


void I2C_DeInit(I2C_RegDef_t *pI2Cx)
{
	if(pI2Cx == I2C1)
	{
		I2C1_REG_RESET();
	}
	else if(pI2Cx == I2C2)
	{
		I2C2_REG_RESET();
	}
	else if(pI2Cx == I2C3)
	{
		I2C3_REG_RESET();
	}
}

uint8_t I2C_GetFlagStatus(I2C_RegDef_t *pI2Cx, uint32_t FlagName)
{

	if(pI2Cx->SR1 & FlagName)
	{
		return FLAG_SET;
	}
	return FLAG_RESET;
}

void I2C_MasterSendData(I2C_Handle_t *pI2CHandle, uint8_t *pTxBuffer, uint32_t len, uint8_t slave_address, uint8_t Sr)
{
	I2C_GenerateStartCondition(pI2CHandle->pI2Cx);

	//Confirm that start generation is completed by checking the SB flag in the SR1
	//Note: until SB is cleared SCL will be stretched (pulled LOW)
	while(!I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_FLAG_SB));

	//Send the address of the slave with r/w bit set to w(0) (total 8 bits)
	I2C_ExecuteAddressPhaseWrite(pI2CHandle->pI2Cx, slave_address);

	//Confirm that the address phase is completed by checking the ADDR flag in SR1
	while(!I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_FLAG_ADDR));

	//Clear the ADDR flag according to its software sequence
	//Note: Until ADDR is cleared SCL will be stretched (pulled to LOW)
	I2C_ClearADDRFlag(pI2CHandle);

	//Send data until len is 0
	while(len > 0)
	{
		while(! I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_FLAG_TXE)); //Wait until TXE bit set
		pI2CHandle->pI2Cx->DR = *pTxBuffer;
		pTxBuffer++;
		len--;
	}

	//When len becomes zero wait for TXE = 1 and BTF = 1 before generating the STOP condition
	//Note: TXE = 1, BTF = 1, means that both SR and DR are empty and enxt transmission should begin
	//when BTF = 1SCL will be stretched (pulled to LOW)
	while(! I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_FLAG_TXE));
	while(! I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_FLAG_BTF));

	//Generate stop condition
	if(Sr == I2C_SR_DI)
	{
		I2C_GenerateStopCondition(pI2CHandle->pI2Cx);
	}
}

void I2C_MasterReceiveData(I2C_Handle_t *pI2CHandle, uint8_t *pRxBuffer, uint32_t len, uint8_t slave_address, uint8_t Sr)
{
	//Generate start condition
	I2C_GenerateStartCondition(pI2CHandle->pI2Cx);

	//Confirm that start generation is compeleted by checking the SB flag in the SR1
	while(!I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_FLAG_SB));

	//Send the address of the slave with r/nw bit set to R(1) (total 8 bits)
	I2C_ExecuteAddressPhaseRead(pI2CHandle->pI2Cx, slave_address);

	//Wait until address phase is completed by checking the ADDR flag in SR1
	while(!I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_FLAG_ADDR));

	//Procedure to read only 1 byte from slave
	if(len == 1)
	{
		//Disable acking
		I2C_ToggleAcking(pI2CHandle->pI2Cx, I2C_ACK_DISABLE);

		//Clear the ADDR flag
		I2C_ClearADDRFlag(pI2CHandle);

		//Wait until RXNE becomes 1
		while(!I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_FLAG_RXNE));

		//Generate STOP condition
		if(Sr == I2C_SR_DI)
		{
				I2C_GenerateStopCondition(pI2CHandle->pI2Cx);
		}

		//Read data into buffer
		*pRxBuffer = pI2CHandle->pI2Cx->DR;
	}

	if(len > 1){

		//Clear the ADDR flag
		I2C_ClearADDRFlag(pI2CHandle);

		//Read the data until len becomes zero
		for(uint32_t i = len; i >0; i--){

			//Wait until RXNE becomes 1
			while(!I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_FLAG_RXNE));

			if(i == 2){

				//Disable acking
				I2C_ToggleAcking(pI2CHandle->pI2Cx, I2C_ACK_DISABLE);

				//Generate STOP condition
				if(Sr == I2C_SR_DI){
						I2C_GenerateStopCondition(pI2CHandle->pI2Cx);
					}
			}

			//Read the data from the data register into the buffer
			*pRxBuffer = pI2CHandle->pI2Cx->DR;

			//Increment the buffer address
			pRxBuffer++;
		}
	}

	//Re-enable acking
	if(pI2CHandle->I2C_Config.I2C_ACKControl == I2C_ACK_ENABLE){

		I2C_ToggleAcking(pI2CHandle->pI2Cx, I2C_ACK_ENABLE);
	}


}

void I2C_ToggleAcking(I2C_RegDef_t *pI2Cx, uint8_t EnorDi){

	if(EnorDi == I2C_ACK_ENABLE){

		pI2Cx->CR1 |= (1 << I2C_CR1_ACK);
	}
	else{

		pI2Cx->CR1 &= ~(1 << I2C_CR1_ACK);
	}
}

//IRQ configuration and ISR handling
void I2C_IRQITConfig(uint8_t IRQ_Number, uint8_t EnorDi){

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

void I2C_IRQPriorityConfig(uint8_t IRQ_Number, uint32_t IRQPriority){

	//1. first lets find out the ipr register
	uint8_t iprx = IRQ_Number / 4;
	uint8_t iprx_section  = IRQ_Number % 4;

	uint8_t shift_amount = (8 * iprx_section) + (8 - NO_PR_BITS_IMPLEMENTED) ;

	*(NVIC_PR_BASE_ADDR + iprx) |=  (IRQPriority << shift_amount);

}

uint8_t  I2C_MasterSendDataIT(I2C_Handle_t *pI2CHandle, uint8_t *pTxBuffer, uint32_t len, uint8_t slaveAddr, uint8_t Sr)
{

	uint8_t busystate = pI2CHandle->TxRxState;

	if((busystate != I2C_BUSY_IN_TX) && (busystate != I2C_BUSY_IN_RX))
	{
		pI2CHandle->pTxBuffer = pTxBuffer;
		pI2CHandle->TxLen = len;
		pI2CHandle->TxRxState = I2C_BUSY_IN_TX;
		pI2CHandle->DevAddr = slaveAddr;
		pI2CHandle->Sr = Sr;

		//Implement code to Generate START Condition
		I2C_GenerateStartCondition(pI2CHandle->pI2Cx);

		//Implement the code to enable ITBUFEN Control Bit
		pI2CHandle->pI2Cx->CR2 |= (1 << I2C_CR2_ITBUFEN);

		//Implement the code to enable ITEVTEN Control Bit
		pI2CHandle->pI2Cx->CR2 |= (1 << I2C_CR2_ITEVTEN);

		//Implement the code to enable ITERREN Control Bit
		pI2CHandle->pI2Cx->CR2 |= (1 << I2C_CR2_ITERREN);
	}

	return busystate;
}

uint8_t I2C_MasterReceiveDataIT(I2C_Handle_t *pI2CHandle,uint8_t *pRxBuffer, uint32_t len, uint8_t slaveAddr,uint8_t Sr)
{

	uint8_t busystate = pI2CHandle->TxRxState;

	if((busystate != I2C_BUSY_IN_TX) && (busystate != I2C_BUSY_IN_RX))
	{
		pI2CHandle->pRxBuffer = pRxBuffer;
		pI2CHandle->RxLen = len;
		pI2CHandle->TxRxState = I2C_BUSY_IN_RX;
		pI2CHandle->RxSize = len; //Rxsize is used in the ISR code to manage the data reception
		pI2CHandle->DevAddr = slaveAddr;
		pI2CHandle->Sr = Sr;


		//Implement code to Generate START Condition
		I2C_GenerateStartCondition(pI2CHandle->pI2Cx);

		//Implement the code to enable ITBUFEN Control Bit
		pI2CHandle->pI2Cx->CR2 |= (1 << I2C_CR2_ITBUFEN);

		//Implement the code to enable ITEVTEN Control Bit
		pI2CHandle->pI2Cx->CR2 |= (1 << I2C_CR2_ITEVTEN);

		//Implement the code to enable ITERREN Control Bit
		pI2CHandle->pI2Cx->CR2 |= (1 << I2C_CR2_ITERREN);

	}

	return busystate;
}

static void I2C_MasterHandleTXEInterrupt(I2C_Handle_t *pI2CHandle)
{


	if(pI2CHandle->TxLen > 0){

		//1. Load the data in to DR
		pI2CHandle->pI2Cx->DR = *(pI2CHandle->pTxBuffer);

		//2. Decrement the TxLen
		pI2CHandle->TxLen--;

		//3. Increment buffer address
		pI2CHandle->pTxBuffer++;

	}
}

static void I2C_MasterHandleRXNEInterrupt(I2C_Handle_t *pI2CHandle)
{

	if(pI2CHandle->RxSize == 1)
	{

		*pI2CHandle->pRxBuffer = pI2CHandle->pI2Cx->DR;
		pI2CHandle->RxLen--;

	}

	if(pI2CHandle->RxSize > 1){

		if(pI2CHandle->RxLen == 2)
		{

			//clear ack bit
			I2C_ToggleAcking(pI2CHandle->pI2Cx, DISABLE);
		}

		//Read DR
		*pI2CHandle->pRxBuffer = pI2CHandle->pI2Cx->DR;
		pI2CHandle->pRxBuffer++;
		pI2CHandle->RxLen--;
	}

	if(pI2CHandle->RxLen == 0){

		//Close the I2C data reception and notify the application

		//1. Generate the stop condition
		if(pI2CHandle->Sr == I2C_SR_DI)
		{

			I2C_GenerateStartCondition(pI2CHandle->pI2Cx);
		}

		//2. Close the I2C rx
		I2C_CloseReceiveData(pI2CHandle);

		//3. Notify the application
		I2C_ApplicationEventCallback(pI2CHandle, I2C_EV_RX_CMPLT);
	}
}

void I2C_CloseReceiveData(I2C_Handle_t *pI2CHandle){

	//Implement the code to disable ITBUFEN control bit
	pI2CHandle->pI2Cx->CR2 &= ~(1 << I2C_CR2_ITBUFEN);

	//Implement the code to disable ITEVTEN control bit
	pI2CHandle->pI2Cx->CR2 &= ~(1 << I2C_CR2_ITEVTEN);

	pI2CHandle->TxRxState = I2C_READY;
	pI2CHandle->pRxBuffer = NULL;
	pI2CHandle->RxLen = 0;
	pI2CHandle->RxSize = 0;

	if(pI2CHandle->I2C_Config.I2C_ACKControl == I2C_ACK_ENABLE)
	{

		I2C_ToggleAcking(pI2CHandle->pI2Cx, ENABLE);
	}
}

void I2C_CloseSendData(I2C_Handle_t *pI2CHandle){

	//Implement the code to disable ITBUFEN control bit
	pI2CHandle->pI2Cx->CR2 &= ~(1 << I2C_CR2_ITBUFEN);

	//Implement the code to disable ITEVTEN control bit
	pI2CHandle->pI2Cx->CR2 &= ~(1 << I2C_CR2_ITEVTEN);

	pI2CHandle->TxRxState = I2C_READY;
	pI2CHandle->pTxBuffer = NULL;
	pI2CHandle->TxLen = 0;
}


void I2C_SlaveSendData(I2C_RegDef_t *pI2Cx, uint8_t data){

	pI2Cx->DR = data;
}

uint8_t I2C_SlaveReceiveData(I2C_RegDef_t *pI2Cx){

	return (uint8_t)pI2Cx->DR;
}

void I2C_EVIRQHandling(I2C_Handle_t *pI2CHandle)
{
	//Interrupt handling for both master and slave mode of a device
	uint32_t temp1, temp2, temp3;

	temp1 = pI2CHandle->pI2Cx->CR2 & (1 << I2C_CR2_ITEVTEN);
	temp2 = pI2CHandle->pI2Cx->CR2 & (1 << I2C_CR2_ITBUFEN);

	temp3 = pI2CHandle->pI2Cx->SR1 & (1 << I2C_SR1_SB);

	//1. Handle for interrupt generated by SB event
	//Note: SB flag is only applicable in Master mode
	if(temp1 && temp3){

		//SB flag is set
		//1. Execute address phase
		if(pI2CHandle->TxRxState == I2C_BUSY_IN_TX)
		{

			I2C_ExecuteAddressPhaseWrite(pI2CHandle->pI2Cx, pI2CHandle->DevAddr);
		}
		else if(pI2CHandle->TxRxState == I2C_BUSY_IN_RX)
		{

			I2C_ExecuteAddressPhaseRead(pI2CHandle->pI2Cx, pI2CHandle->DevAddr);
		}


	}

	temp3 = pI2CHandle->pI2Cx->SR1 & (1 << I2C_SR1_ADDR);
	//2. Handle for interrupt generated by ADDR event
	//Note: when in master mode : Address is sent
	//		when in slave mode : Address is matched with own address
	if(temp1 && temp3)
	{

		//ADDR flag is set
		I2C_ClearADDRFlag(pI2CHandle);
	}

	temp3 = pI2CHandle->pI2Cx->SR1 & (1 << I2C_SR1_BTF);

	//3. Handle for interrupt generated by BTF event
	if(temp1 && temp3)
	{

		//BTF flag is set
		if(pI2CHandle->TxRxState == I2C_BUSY_IN_TX)
		{

			if(pI2CHandle->pI2Cx->SR1 & (1 << I2C_SR1_TXE))
			{

				//BTF, TXE = 1
				if(pI2CHandle->TxLen == 0)
				{

					//1. Generate the STOP condition
					if(pI2CHandle->Sr == I2C_SR_DI)
					{

						I2C_GenerateStopCondition(pI2CHandle->pI2Cx);
					}


					//2. Reset all the member elements of the handle structure
					I2C_CloseSendData(pI2CHandle);

					//3. Notify the application
					I2C_ApplicationEventCallback(pI2CHandle, I2C_EV_TX_CMPLT);
				}
			}
		}
		else if(pI2CHandle->TxRxState == I2C_BUSY_IN_RX)
		{
			;
		}

	}

	temp3 = pI2CHandle->pI2Cx->SR1 & (1 << I2C_SR1_STOPF);
	//4. Handle for interrupt generated by STOPF event
	//Note: Stop detection flag is applicable only in slave mode
	if(temp1 && temp3)
	{

		//STOPF flag is set
		//Clear the flag (read SR1, write to CR1)
		pI2CHandle->pI2Cx->CR1 |= 0x0000;

		//Notify the application that STOP is detected
		I2C_ApplicationEventCallback(pI2CHandle, I2C_EV_STOP);

	}

	temp3 = pI2CHandle->pI2Cx->SR1 & (1 << I2C_SR1_TXE);
	//5. Handle for interrupt generated by TXE event
	if(temp1 && temp2 && temp3)
	{

		if(pI2CHandle->pI2Cx->SR2 & (1 << I2C_SR2_MSL))
		{

			//Master
			//We have to do the data transmission
			if(pI2CHandle->TxRxState == I2C_BUSY_IN_TX)
			{

				I2C_MasterHandleTXEInterrupt(pI2CHandle);
			}
		}
		else
		{

			//slave
			//make sure that the slave is really in transmitter mode
			if(pI2CHandle->pI2Cx->SR2 & ( 1 << I2C_SR2_TRA))
			{

				I2C_ApplicationEventCallback(pI2CHandle, I2C_EV_DATA_REQ);
			}

		}


	}

	temp3 = pI2CHandle->pI2Cx->SR1 & (1 << I2C_SR1_RXNE);
	//6. Handle for interrupt generated by RXNE event
	if(temp1 && temp2 && temp3)
	{

		if(pI2CHandle->pI2Cx->SR2 & (1 << I2C_SR2_MSL))
		{

			//Device is in master mode

			//RXNE flag is set
			if(pI2CHandle->TxRxState == I2C_BUSY_IN_RX)
			{

				I2C_MasterHandleRXNEInterrupt(pI2CHandle);
			}
		}
		else
		{
			//slave
			//make sure that the slave is really in receiver mode
			if(!(pI2CHandle->pI2Cx->SR2 & ( 1 << I2C_SR2_TRA)))
			{

				I2C_ApplicationEventCallback(pI2CHandle, I2C_EV_DATA_RCV);
			}
		}
	}
}

void I2C_ERIRQHandling(I2C_Handle_t *pI2CHandle)
{

	uint32_t temp1,temp2;

    //Know the status of  ITERREN control bit in the CR2
	temp2 = (pI2CHandle->pI2Cx->CR2) & ( 1 << I2C_CR2_ITERREN);


/***********************Check for Bus error************************************/
	temp1 = (pI2CHandle->pI2Cx->SR1) & ( 1 << I2C_SR1_BERR);
	if(temp1  && temp2 )
	{
		//This is Bus error

		//Implement the code to clear the buss error flag
		pI2CHandle->pI2Cx->SR1 &= ~( 1 << I2C_SR1_BERR);

		//Implement the code to notify the application about the error
		I2C_ApplicationEventCallback(pI2CHandle, I2C_ERROR_BERR);
	}

/***********************Check for arbitration lost error************************************/
	temp1 = (pI2CHandle->pI2Cx->SR1) & ( 1 << I2C_SR1_ARLO );
	if(temp1  && temp2)
	{
		//This is arbitration lost error

		//Implement the code to clear the arbitration lost error flag
		pI2CHandle->pI2Cx->SR1 &= ~( 1 << I2C_SR1_ARLO);

		//Implement the code to notify the application about the error
		I2C_ApplicationEventCallback(pI2CHandle, I2C_ERROR_ARLO);
	}

/***********************Check for ACK failure  error************************************/

	temp1 = (pI2CHandle->pI2Cx->SR1) & ( 1 << I2C_SR1_AF);
	if(temp1  && temp2)
	{
		//This is ACK failure error

	    //Implement the code to clear the ACK failure error flag
		pI2CHandle->pI2Cx->SR1 &= ~( 1 << I2C_SR1_AF);

		//Implement the code to notify the application about the error
		I2C_ApplicationEventCallback(pI2CHandle, I2C_ERROR_AF);
	}

/***********************Check for Overrun/underrun error************************************/
	temp1 = (pI2CHandle->pI2Cx->SR1) & ( 1 << I2C_SR1_OVR);
	if(temp1  && temp2)
	{
		//This is Overrun/underrun

	    //Implement the code to clear the Overrun/underrun error flag
		pI2CHandle->pI2Cx->SR1 &= ~( 1 << I2C_SR1_OVR);

		//Implement the code to notify the application about the error
		I2C_ApplicationEventCallback(pI2CHandle, I2C_ERROR_OVR);
	}

/***********************Check for Time out error************************************/
	temp1 = (pI2CHandle->pI2Cx->SR1) & ( 1 << I2C_SR1_TIMEOUT);
	if(temp1  && temp2)
	{
		//This is Time out error

	    //Implement the code to clear the Time out error flag
		pI2CHandle->pI2Cx->SR1 &= ~( 1 << I2C_SR1_TIMEOUT);

		//Implement the code to notify the application about the error
		I2C_ApplicationEventCallback(pI2CHandle, I2C_ERROR_TIMEOUT);
	}

}

void I2C_SlaveEnableDisableCallbackEvents(I2C_RegDef_t *pI2Cx, uint8_t EnorDi){

	if(EnorDi == ENABLE){
		pI2Cx->CR2 |= (1 << I2C_CR2_ITEVTEN);
		pI2Cx->CR2 |= (1 << I2C_CR2_ITBUFEN);
		pI2Cx->CR2 |= (1 << I2C_CR2_ITERREN);
	}
	else{

		pI2Cx->CR2 &= ~(1 << I2C_CR2_ITEVTEN);
		pI2Cx->CR2 &= ~(1 << I2C_CR2_ITBUFEN);
		pI2Cx->CR2 &= ~(1 << I2C_CR2_ITERREN);
	}
}


//application callback
__weak void I2C_ApplicationEventCallback(I2C_Handle_t *pI2CHandle, uint8_t AppEv);
