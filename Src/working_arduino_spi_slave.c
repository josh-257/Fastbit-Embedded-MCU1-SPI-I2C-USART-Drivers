/*
 * working_arduino_spi_slave.c
 *
 *  Created on: 29 May 2026
 *      Author: joshb
 */


/*
 * SPI_tx_testing.c
 *
 *  Created on: 18 May 2026
 *      Author: joshb
 */

#include "stm32f407xx.h"
#include <string.h>

void delay(void){
	for(uint32_t i = 0; i < 50000/2; i++);
}

//PB15 --> SPI2_MOSI
//pB14 --> SPI2_MISO
//pB13 --> SPI2_SCLK
//PB12 --> SPI2_NSS
//Alt func mode: 5



void SPI2_GPIO_Inits(void){

	GPIO_Handle_t SPIPins;
	SPIPins.pGPIOx = GPIOB;
	SPIPins.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
	SPIPins.GPIO_PinConfig.GPIO_AltFunMode = 5;
	SPIPins.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	SPIPins.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_NOPUPD;
	SPIPins.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_HIGH;

	//SCLK
	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_13;
	GPIO_Init(&SPIPins);

	//MOSI
	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_15;
	GPIO_Init(&SPIPins);

	//MISO
//	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_14;
//	GPIO_Init(&SPIPins);

	// 2. NSS (PB12) as Software Controlled GPIO Output
	SPIPins.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	SPIPins.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	SPIPins.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_PU; // Pull up to keep it High (idle)
	SPIPins.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_HIGH;

	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_12;
	GPIO_Init(&SPIPins);

}

void SPI2_Inits(void){

	SPI_Handle_t SPI2handle;

	SPI2handle.pSPIx = SPI2;
	SPI2handle.SPIConfig.SPI_BusConfig = SPI_BUS_CFG_FD;
	SPI2handle.SPIConfig.SPI_DeviceMode = SPI_MODE_MASTER;
	SPI2handle.SPIConfig.SPI_SclkSpeed = SPI_SCLK_SPEED_DIV256; //increasing speed breaks it
	SPI2handle.SPIConfig.SPI_DFF = SPI_DFF_8;
	SPI2handle.SPIConfig.SPI_CPOL = SPI_CPOL_HIGH;
	SPI2handle.SPIConfig.SPI_CPHA = SPI_CPHA_HIGH;
	SPI2handle.SPIConfig.SPI_SSM = SPI_SSM_EN;

	SPI_Init(&SPI2handle);
}


int main (void){

	char user_data[] = "HELLO WORLD";

	//button
	GPIO_Handle_t GPIO_btn;
	GPIO_btn.pGPIOx = GPIOA;
	GPIO_btn.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_0;
	GPIO_btn.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IN;
	GPIO_btn.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_NOPUPD;
	GPIO_Init(&GPIO_btn);

	//Initialise Pins
	SPI2_GPIO_Inits();
	SPI2_Inits();
	// Force the internal SSI bit HIGH to avoid MODF error
	SPI2->CR1 |= (1 << 8); // Bit 8 is the SSI bit in SPI_CR1
	GPIO_WriteToOutputPin(GPIOB, 12, GPIO_PIN_SET);

	//enable the SPI2 peripheral once
	SPI_PeripheralControl(SPI2, ENABLE);
	while(1){
		while(GPIO_ReadFromInputPin(GPIOA, GPIO_PIN_NO_0) == 0);
		delay();
		 // SAFETY GUARD: Clear any lingering error states before starting
		//uint8_t clearDR = SPI2->DR;
		//uint8_t clearSR = SPI2->SR;
		//(void)clearDR;
		//(void)clearSR;

		//Pull NSS low
		GPIO_WriteToOutputPin(GPIOB, 12, GPIO_PIN_RESET);
		//first send length information
		uint8_t dataLen = strlen(user_data);
		SPI_Send(SPI2, &dataLen, 1);
		//send data
		SPI_Send(SPI2, (uint8_t*)user_data, strlen(user_data));
		//check finished transmission before disabling
		//while(!SPI_GetFlagStatus(SPI2, SPI_SR_TXE));
		while(SPI_GetFlagStatus(SPI2, SPI_SR_BSY));
		//for(volatile uint32_t i = 0; i < 200; i++);
		while(GPIO_ReadFromInputPin(GPIOA, GPIO_PIN_NO_0));
		//stop transmission
		GPIO_WriteToOutputPin(GPIOB, 12, GPIO_PIN_SET);
		delay();
	}
	return 0;
}


