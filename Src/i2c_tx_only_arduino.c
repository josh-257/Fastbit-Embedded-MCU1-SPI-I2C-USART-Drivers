/*
 * i2c_tx_only_arduino.c
 *
 *  Created on: 23 Jul 2026
 *      Author: joshb
 */

#include "stm32f407xx.h"
#include <stdio.h>
#include <string.h>

#define MY_ADDR 0x61
#define SLAVE_ADDR 0x68

//some data
uint8_t some_data[] = "We are testing I2C master Tx\n";

void delay(void){
    // Course standard software delay for button debouncing
    for(uint32_t i = 0; i < 500000/2; i++);
}


/*
 * pb6 --> SCL
 * pb7 --> SDA
 */

I2C_Handle_t I2C1Handle;

void I2C1_GPIOInits(void){

    GPIO_Handle_t I2CPins;

    I2CPins.pGPIOx = GPIOB;
    I2CPins.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
    I2CPins.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_OD;
    I2CPins.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_PU;
    I2CPins.GPIO_PinConfig.GPIO_AltFunMode = 4;
    I2CPins.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_HIGH;

    //scl
    I2CPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_6;
    GPIO_Init(&I2CPins);

    //sda
    I2CPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_7;
    GPIO_Init(&I2CPins);
}

void I2C1_Inits(void){



	I2C1Handle.pI2Cx = I2C1;
	I2C1Handle.I2C_Config.I2C_ACKControl = I2C_ACK_ENABLE;
	I2C1Handle.I2C_Config.I2C_DeviceAddress = MY_ADDR;
	I2C1Handle.I2C_Config.I2C_FMDutyCycle = I2C_FM_DUTY_2;
	I2C1Handle.I2C_Config.I2C_SCLSpeed = I2C_SCL_SPEED_SM;

	I2C_Init(&I2C1Handle);
}

void Button_Init(void){

    GPIO_Handle_t GPIO_btn, GpioLed;

    GPIO_btn.pGPIOx = GPIOA;
    GPIO_btn.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_0;
    GPIO_btn.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IN;
    GPIO_btn.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_HIGH;
    GPIO_btn.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_NOPUPD;

    GPIO_Init(&GPIO_btn);

	GpioLed.pGPIOx = GPIOD;
	GpioLed.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_12;
	GpioLed.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	GpioLed.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_HIGH;
	GpioLed.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	GpioLed.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_NOPUPD;

	GPIO_PeripheralClockControl(GPIOD,ENABLE);

	GPIO_Init(&GpioLed);
}

int main(void){


	Button_Init();

		I2C1_GPIOInits();

		I2C1_Inits();

		I2C_PeripheralControl(I2C1, ENABLE);

	while(1){

		//wait for button press
		while(GPIO_ReadFromInputPin(GPIOA, GPIO_PIN_NO_0) == 0);

		delay();

		//send some data to the slave
		I2C_MasterSendData(&I2C1Handle, some_data, strlen((char*)some_data), SLAVE_ADDR);
	}



}
