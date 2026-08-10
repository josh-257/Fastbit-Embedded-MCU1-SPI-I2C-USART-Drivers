/*
 * led_button.c
 *
 *  Created on: 12 May 2026
 *      Author: joshb
 */


#include <stdint.h>
#include "stm32f407xx.h"

#define LOW  0
#define HIGH 1
#define BTN_PRESSED HIGH


void delay(void){
	for(volatile int i = 0; i < 500000/2; i++);
}

int main(void)
{
    /* Loop forever */
	//green led pd12
	GPIO_Handle_t GPIO_led, GPIO_btn;
	GPIO_led.pGPIOx = GPIOD;
	GPIO_led.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_12;
	GPIO_led.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	GPIO_led.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	GPIO_led.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_NOPUPD;
	GPIO_PeripheralClockControl(GPIOD, ENABLE);
	GPIO_Init(&GPIO_led);

	//button
	GPIO_btn.pGPIOx = GPIOB;
	GPIO_btn.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_5;
	GPIO_btn.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IN;
	GPIO_btn.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_PU;
	GPIO_PeripheralClockControl(GPIOB, ENABLE);
	GPIO_Init(&GPIO_btn);
	for(;;){
		if(GPIO_ReadFromInputPin(GPIOB, 5) == LOW){
			delay();
			GPIO_ToggleOutputPin(GPIOD, 12);
			while(GPIO_ReadFromInputPin(GPIOB, 5) == LOW);
		}
	}
}

void EXTI0_IRQHandler(void){
	GPIO_IRQHandling(0);
}
