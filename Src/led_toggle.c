#include <stdint.h>
#include "stm32f407xx.h"


void delay(void){
	for(int i = 0; i < 500000; i+=2);
}

int main(void)
{
    /* Loop forever */
	//green led pd12
	GPIO_Handle_t GPIO_led;
	GPIO_led.pGPIOx = GPIOD;
	GPIO_led.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_12;
	GPIO_led.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	GPIO_led.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_OD;
	GPIO_led.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_PU;
	GPIO_PeriClockControl(GPIOD, ENABLE);
	GPIO_Init(&GPIO_led);
	for(;;){
		GPIO_ToggleOutputPin(GPIOD, 12);
		delay();
	}
}
