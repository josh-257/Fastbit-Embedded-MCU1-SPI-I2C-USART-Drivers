/*
 * spi_arduino_tes.c
 *
 *  Created on: 28 May 2026
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



int main(void)
{
	// 1. Set up your PB5 Button exactly as before
	GPIO_Handle_t GPIO_btn;
	GPIO_btn.pGPIOx = GPIOB;
	GPIO_btn.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_5;
	GPIO_btn.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IN;
	GPIO_btn.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_PU;
	GPIO_PeripheralClockControl(GPIOB, ENABLE);
	GPIO_Init(&GPIO_btn);

	// 2. Configure PB13 and PB15 as simple OUTPUTS instead of SPI
	GPIO_Handle_t TestPins;
	TestPins.pGPIOx = GPIOB;
	TestPins.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;        // Simple Output Mode
	TestPins.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	TestPins.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_NOPUPD;

	TestPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_13;     // SCLK line
	GPIO_Init(&TestPins);

	TestPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_15;     // MOSI line
	GPIO_Init(&TestPins);

	while(1)
	{
		// If button is pressed (GND / LOW)
		if(GPIO_ReadFromInputPin(GPIOB, GPIO_PIN_NO_5) == 0)
		{
			GPIO_WriteToOutputPin(GPIOB, GPIO_PIN_NO_13, GPIO_PIN_SET);  // Drive 3.3V out
			GPIO_WriteToOutputPin(GPIOB, GPIO_PIN_NO_15, GPIO_PIN_SET);  // Drive 3.3V out
		}
		else
		{
			GPIO_WriteToOutputPin(GPIOB, GPIO_PIN_NO_13, GPIO_PIN_RESET);   // Drive 0V out
			GPIO_WriteToOutputPin(GPIOB, GPIO_PIN_NO_15, GPIO_PIN_RESET);   // Drive 0V out
		}
	}
}



