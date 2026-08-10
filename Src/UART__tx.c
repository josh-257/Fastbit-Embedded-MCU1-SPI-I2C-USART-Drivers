#include "stm32f407xx.h"
#include <stdio.h>
#include <string.h>

char msg[] = "UART Tx testing...\n\r";

void delay(void){

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


USART_Handle_t usart2_handle;

void USART2_GPIOInits(void){

    GPIO_Handle_t usart_gpios;

    usart_gpios.pGPIOx = GPIOA;
    usart_gpios.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
    usart_gpios.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
    usart_gpios.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_PU;
    usart_gpios.GPIO_PinConfig.GPIO_AltFunMode = 7;
    usart_gpios.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_HIGH;

    //USART2 TX
    usart_gpios.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_2;
    GPIO_Init(&usart_gpios);

    //USART2 RX
    usart_gpios.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_3;
    GPIO_Init(&usart_gpios);
}

void USART2_Inits(void){

	usart2_handle.pUSARTx = USART2;
	usart2_handle.USART_Config.USART_Baud = USART_STD_BAUD_115200;
	usart2_handle.USART_Config.USART_HWFlowControl = USART_HW_FLOW_CTRL_NONE;
	usart2_handle.USART_Config.USART_Mode = USART_MODE_ONLY_TX;
	usart2_handle.USART_Config.USART_NoOfStopBits = USART_STOPBITS_1;
	usart2_handle.USART_Config.USART_WordLength = USART_WORDLEN_8BITS;
	usart2_handle.USART_Config.USART_ParityControl = USART_PARITY_DISABLE;

	USART_Init(&usart2_handle);
}

void Button_Init(void){

    GPIO_Handle_t GPIO_btn;

    GPIO_btn.pGPIOx = GPIOA;
    GPIO_btn.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_0;
    GPIO_btn.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IN;
    GPIO_btn.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_HIGH;
    GPIO_btn.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_NOPUPD;

    GPIO_Init(&GPIO_btn);

}

int main(void){

	Button_Init();

	USART2_GPIOInits();

	USART2_Inits();

	USART_PeripheralControl(USART2, ENABLE);

	while(1){

		while(GPIO_ReadFromInputPin(GPIOA, GPIO_PIN_NO_0) == 0);

		delay();

		USART_SendData(&usart2_handle, (uint8_t*)msg, strlen(msg));
	}

	return 0;
}
