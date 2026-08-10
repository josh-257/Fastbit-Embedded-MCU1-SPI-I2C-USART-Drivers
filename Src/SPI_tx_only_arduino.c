/*
 * SPI_tx_testing.c
 *
 *  Created on: 18 May 2026
 *      Author: joshb
 */

#include "stm32f407xx.h"
#include <string.h>

void delay(void){
	for(uint32_t i = 0; i < 500000/2; i++);
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

	// 2. NSS (PB12)
//	SPIPins.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
//	SPIPins.GPIO_PinConfig.GPIO_AltFunMode = GPIO_AF_AF5;
//	SPIPins.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
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
	SPI2handle.SPIConfig.SPI_SclkSpeed = SPI_SCLK_SPEED_DIV256;
	SPI2handle.SPIConfig.SPI_DFF = SPI_DFF_8;
	SPI2handle.SPIConfig.SPI_CPOL = SPI_CPOL_LOW;
	SPI2handle.SPIConfig.SPI_CPHA = SPI_CPHA_LOW;
	SPI2handle.SPIConfig.SPI_SSM = SPI_SSM_DI;

	SPI_Init(&SPI2handle);
}


int main (void){

	char user_data[] = "HELLO WORLD I am following Kiran's fastbit embedded course called MCU 1 MCU stands for microcontroller!";

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


	SPI_SSOEConfig(SPI2, ENABLE);

	//enable the SPI2 peripheral once

	while(1){
		while(GPIO_ReadFromInputPin(GPIOA, GPIO_PIN_NO_0) == 0);

		delay();

		SPI_PeripheralControl(SPI2, ENABLE);

		//first send length information
		uint8_t dataLen = strlen(user_data);
		SPI_Send(SPI2, &dataLen, 1);
		//send data
		SPI_Send(SPI2, (uint8_t*)user_data, strlen(user_data));
		//check finished transmission before disabling
		while(!SPI_GetFlagStatus(SPI2, SPI_SR_TXE));
		while(SPI_GetFlagStatus(SPI2, SPI_SR_BSY));
		//Needed for working with arduino (hardware latency)
		for(volatile uint32_t i = 0; i < 500; i++);
		//stop transmission
		SPI_PeripheralControl(SPI2, DISABLE);
		while(GPIO_ReadFromInputPin(GPIOA, GPIO_PIN_NO_0) == 1);
		delay();
	}
	return 0;
}

