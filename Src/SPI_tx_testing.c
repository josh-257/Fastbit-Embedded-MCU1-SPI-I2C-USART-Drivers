/*
 * SPI_tx_testing.c
 *
 *  Created on: 18 May 2026
 *      Author: joshb
 */

#include "stm32f407xx.h"
#include <string.h>

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
	SPIPins.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NOPUPD;
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

	//NSS
//	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_12;
//	GPIO_Init(&SPIPins);


}

void SPI2_Inits(void){

	SPI_Handle_t SPI2handle;

	SPI2handle.pSPIx = SPI2;
	SPI2handle.SPIConfig.SPI_BusConfig = SPI_BUS_CFG_FD;
	SPI2handle.SPIConfig.SPI_DeviceMode = SPI_MODE_MASTER;
	SPI2handle.SPIConfig.SPI_SclkSpeed = SPI_SCLK_SPEED_DIV2;
	SPI2handle.SPIConfig.SPI_DFF = SPI_DIFF_8;
	SPI2handle.SPIConfig.SPI_CPOL = SPI_CPOL_LOW;
	SPI2handle.SPIConfig.SPI_CPHA = SPI_CPHA_LOW;
	SPI2handle.SPIConfig.SPI_SSM = SPI_SSM_EN;

	SPI_Init(&SPI2handle);
}


int main (void){

	char user_data[] = "Hello World";

	//Initialise Pins
	SPI2_GPIO_Inits();
	SPI2_Inits();
	//This makes NSS signal internally high and avoids MODF error
	SPI_SSIConfig(SPI2, ENABLE);
	//enable the SPI2 peripheral
	SPI_PeripheralControl(SPI2, ENABLE);
	SPI_Send(SPI2, (uint8_t*)user_data, strlen(user_data));
	while(!SPI_Get_Flag_Status(SPI2, SPI_SR_TXE));
	while(SPI_Get_Flag_Status(SPI2, SPI_SR_BSY));
	//SPI_PeripheralControl(SPI2, DISABLE);
	while(1);
	return 0;
}

