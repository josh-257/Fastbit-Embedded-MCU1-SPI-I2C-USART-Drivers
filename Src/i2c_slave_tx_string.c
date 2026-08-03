#include "stm32f407xx.h"
#include <stdio.h>
#include <string.h>

#define SLAVE_ADDR 0x68
#define MY_ADDR	SLAVE_ADDR

void delay(void){
    // Course standard software delay for button debouncing
    for(uint32_t i = 0; i < 500000/2; i++);
}



/*********************************This is needed for using SWV printf**************************/
// Manual Hardware mapping for ARM Cortex-M4 ITM Trace Registers
#define DEMCR                   *((volatile uint32_t*) 0xE000EDFCU )
#define ITM_STIMULUS_PORT0      *((volatile uint32_t*) 0xE0000000U )
#define ITM_TRACE_EN            *((volatile uint32_t*) 0xE0000E00U )

void ITM_SendChar(uint8_t ch)
{
    // 1. Enable Core Trace Hardware (TRCENA)
    DEMCR |= (1 << 24);

    // 2. Enable ITM Stimulus Port 0
    ITM_TRACE_EN |= (1 << 0);

    // 3. Wait until the ITM transmit FIFO buffer is ready/not full
    while(!(ITM_STIMULUS_PORT0 & 1));

    // 4. Write data byte to port 0
    ITM_STIMULUS_PORT0 = ch;
}


int _write(int file, char *ptr, int len) {
    for (int i = 0; i < len; i++) {
        ITM_SendChar((*ptr++));
    }
    return len;
}
/**********************************************************************************************/


/*
 * pb6 --> SCL
 * pb7 --> SDA
 */

I2C_Handle_t I2C1Handle;

uint8_t Tx_buf[] = "STM32 Slave mode testing..";

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

    GPIO_Handle_t GPIO_btn;

    GPIO_btn.pGPIOx = GPIOA;
    GPIO_btn.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_0;
    GPIO_btn.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IN;
    GPIO_btn.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_HIGH;
    GPIO_btn.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NOPUPD;

    GPIO_Init(&GPIO_btn);

}

int main(void){


	Button_Init();

	I2C1_GPIOInits();

	I2C1_Inits();

	//I2C IRQ configuration
	I2C_IRQITConfig(IRQ_NO_I2C1_EV, ENABLE);
	I2C_IRQITConfig(IRQ_NO_I2C1_ER, ENABLE);

	I2C_SlaveEnableDisableCallbackEvents(I2C1, ENABLE);

	I2C_PeripheralControl(I2C1, ENABLE);

	I2C_Toggle_Acking(I2C1, I2C_ACK_ENABLE);

	while(1);
	return 0;
}

void I2C1_EV_IRQHandler(void){

	I2C_EV_IRQHandling(&I2C1Handle);
}

void I2C1_ER_IRQHandler(void){

	I2C_ER_IRQHandling(&I2C1Handle);
}

void I2C_Application_Event_Callback(I2C_Handle_t *pI2CHandle, uint8_t AppEv){

	static uint8_t cnt = 0;
	static uint8_t commandCode = 0;

	if(AppEv == I2C_EV_DATA_REQ){

		//Slave must send data
		if(commandCode == 0x51){

			//Send the length information to the master
			I2C_SlaveSendData(pI2CHandle->pI2Cx, strlen((char*)Tx_buf));
		}
		else if(commandCode == 0x52){

			//Send the contents of Tx_buf
			I2C_SlaveSendData(pI2CHandle->pI2Cx, Tx_buf[cnt++]);
		}
	}
	else if(AppEv == I2C_EV_DATA_RCV){

		//Slave must receive data
		commandCode = I2C_SlaveReceiveData(pI2CHandle->pI2Cx);

	}
	else if(AppEv == I2C_ERROR_AF){

		//Master sends nack so slave should stop transmitting
		commandCode = 0xff;
		cnt = 0;
	}
	else if(AppEv == I2C_EV_STOP){

		//Masters has ended communication
	}

}

