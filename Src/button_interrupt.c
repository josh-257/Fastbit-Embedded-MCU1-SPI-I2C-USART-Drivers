/*
 * led_button.c
 *
 *  Created on: 12 May 2026
 *      Author: joshb
 */

#include <string.h>
#include <stdint.h>
#include "stm32f407xx.h"

#define LOW  0
#define HIGH 1
#define BTN_PRESSED HIGH


void delay(void){
	for(int i = 0; i < 500000/2; i++);
}

int main(void)
{
    /* Loop forever */
	//green led pd12
	GPIO_Handle_t GPIO_led, GPIO_btn;
	memset(&GPIO_led, 0, sizeof(GPIO_led));
	memset(&GPIO_btn, 0, sizeof(GPIO_btn));
	GPIO_led.pGPIOx = GPIOD;
	GPIO_led.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_12;
	GPIO_led.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	GPIO_led.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	GPIO_led.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_NOPUPD;
	GPIO_PeripheralClockControl(GPIOD, ENABLE);
	GPIO_Init(&GPIO_led);

	//button
	GPIO_btn.pGPIOx = GPIOD;
	GPIO_btn.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_5;
	GPIO_btn.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IT_FT;
	GPIO_btn.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_PU;
	GPIO_PeripheralClockControl(GPIOD, ENABLE);
	GPIO_Init(&GPIO_btn);
	GPIO_WriteToOutputPin(GPIOD, 12, GPIO_PIN_RESET);
	//IRQ configurations
	GPIO_IRQPriorityConfig(IRQ_NO_EXTI9_5, NVIC_IRQ_PRIO15);
	GPIO_IRQITConfig(IRQ_NO_EXTI9_5, ENABLE);


	return 0;
}

void EXTI9_5_IRQHandler(void){
	delay();
	GPIO_IRQHandling(GPIO_PIN_NO_5);
	GPIO_ToggleOutputPin(GPIOD, 12);
}
