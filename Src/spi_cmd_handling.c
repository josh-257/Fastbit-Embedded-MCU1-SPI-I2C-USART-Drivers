/*
 * 002SPislaveCMDHandling.c
 * Course: Mastering Microcontroller with Embedded Driver Development (MCU1)
 * Author: FastBit Embedded Brain Academy
 */

#include "stm32f407xx.h"
#include <stdio.h>
#include <string.h>

//extern void initialise_monitor_handles();


// Command Definitions matching the Arduino Slave Application
#define COMMAND_LED_CTRL          0x50
#define COMMAND_SENSOR_READ       0x51
#define COMMAND_LED_READ          0x52
#define COMMAND_PRINT             0x53
#define COMMAND_ID_READ           0x54

#define LED_ON                    1
#define LED_OFF                   0

// Analog pins used for sensor reading commands
#define ANALOG_PIN0               0

// Arduino peripheral pins
#define LED_PIN                   9

void delay(void){
    // Course standard software delay for button debouncing
    for(uint32_t i = 0; i < 500000/2; i++);
}

/*
 * Pin Configurations (SPI2):
 * PB13 --> SPI2_SCLK
 * PB14 --> SPI2_MISO
 * PB15 --> SPI2_MOSI
 * PB12 --> SPI2_NSS
 * Alternate Function Mode: 5
 */
void SPI2_GPIOInits(void){

    GPIO_Handle_t SPIPins;

    SPIPins.pGPIOx = GPIOB;
    SPIPins.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
    SPIPins.GPIO_PinConfig.GPIO_AltFunMode = 5;
    SPIPins.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
    SPIPins.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NOPUPD;
    SPIPins.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_HIGH;

    // SCLK
    SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_13;
    GPIO_Init(&SPIPins);

    // MOSI
    SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_15;
    GPIO_Init(&SPIPins);

    // MISO
    SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_14;
    GPIO_Init(&SPIPins);

    // Fix for PB12 (NSS) configuration
    SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_12;
    GPIO_Init(&SPIPins);
}

void SPI2_Inits(void){

    SPI_Handle_t SPI2handle;

    SPI2handle.pSPIx = SPI2;
    SPI2handle.SPIConfig.SPI_BusConfig = SPI_BUS_CFG_FD;
    SPI2handle.SPIConfig.SPI_DeviceMode = SPI_MODE_MASTER;
    SPI2handle.SPIConfig.SPI_SclkSpeed = SPI_SCLK_SPEED_DIV256; // Course default speed
    SPI2handle.SPIConfig.SPI_DFF = SPI_DFF_8;

    // Mode 0 Configuration used in Kiran's original command scripts
    SPI2handle.SPIConfig.SPI_CPOL = SPI_CPOL_LOW;
    SPI2handle.SPIConfig.SPI_CPHA = SPI_CPHA_LOW;

    // Hardware Slave Management: SSOE is enabled so the hardware manages NSS
    SPI2handle.SPIConfig.SPI_SSM = SPI_SSM_DI;

    SPI_Init(&SPI2handle);
}

void Button_Init(void){

    GPIO_Handle_t GPIO_btn, GpioLed;

    GPIO_btn.pGPIOx = GPIOA;
    GPIO_btn.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_0;
    GPIO_btn.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IN;
    GPIO_btn.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_HIGH;
    GPIO_btn.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NOPUPD;

    GPIO_Init(&GPIO_btn);

	GpioLed.pGPIOx = GPIOD;
	GpioLed.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_12;
	GpioLed.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	GpioLed.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_HIGH;
	GpioLed.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	GpioLed.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NOPUPD;

	GPIO_PeriClockControl(GPIOD,ENABLE);

	GPIO_Init(&GpioLed);
}

uint8_t SPI_VerifyResponse(uint8_t ackbyte){

    if(ackbyte == (uint8_t)0xF5){
        return 1; // ACK
    }
    return 0; // NACK
}

int main(void){

    uint8_t dummy_write = 0xFF;
    uint8_t dummy_read;

    //initialise_monitor_handles();

    printf("Application is running\n");

    // Initialise Button and SPI configuration
    Button_Init();
    SPI2_GPIOInits();
    SPI2_Inits();

    SPI_SSOEConfig(SPI2, ENABLE);

    while(1){
        // Wait until User Button (PA0) is pressed (goes HIGH)
        while(GPIO_ReadFromInputPin(GPIOA, GPIO_PIN_NO_0) == 0);


        delay();

        // Enable the SPI2 peripheral
        SPI_PeripheralControl(SPI2, ENABLE);

        //Cmd 1
        // 1. Send the custom Command Code
        uint8_t commandcode = COMMAND_LED_CTRL;
        uint8_t ackbyte;
        uint8_t args[2];

        //send command
        SPI_Send(SPI2, &commandcode, 1);

        //dummy read to clear off RXNE
        SPI_Receive(SPI2, &dummy_read, 1);

        // 2. Send 1 dummy byte to fetch the ACK/NACK response from the slave
        SPI_Send(SPI2, &dummy_write, 1);

        //read the ackbyte received
        SPI_Receive(SPI2, &ackbyte, 1);

        // 3. Verify the handshake response before pushing payload arguments
        if(SPI_VerifyResponse(ackbyte)){

            args[0] = LED_PIN;
            args[1] = LED_ON;

            //send arguments
			SPI_Send(SPI2, args, 2);

			//dummy read
			SPI_Receive(SPI2, args, 2);
        }//End of cmd 1

        SPI_PeripheralControl(SPI2, DISABLE);

        //Cmd 2
        // Wait until User Button (PA0) is pressed (goes HIGH)
		while(GPIO_ReadFromInputPin(GPIOA, GPIO_PIN_NO_0) == 0);

		SPI_PeripheralControl(SPI2, ENABLE);

		delay();

		commandcode = COMMAND_SENSOR_READ;

		//send command
		SPI_Send(SPI2, &commandcode, 1);

		//dummy read to clear off RXNE
		SPI_Receive(SPI2, &dummy_read, 1);

		// 2. Send 1 dummy byte to fetch the ACK/NACK response from the slave
		SPI_Send(SPI2, &dummy_write, 1);

		//read the ackbyte received
		SPI_Receive(SPI2, &ackbyte, 1);

		// 3. Verify the handshake response before pushing payload arguments
		if(SPI_VerifyResponse(ackbyte)){

			args[0] = ANALOG_PIN0;

			//send arguments
			SPI_Send(SPI2, args, 1);

			//dummy read
			SPI_Receive(SPI2, &dummy_read, 1);

			//insert some delay so that slave can get ready with the data
			delay();

			//Send some dummy bits (1byte) fetch the response from the slave
			SPI_Send(SPI2, &dummy_write, 1);

			uint8_t analog_read;
			SPI_Receive(SPI2, &analog_read, 1);
		}
		SPI_PeripheralControl(SPI2, DISABLE);

		//cmd3
		// Wait until User Button (PA0) is pressed (goes HIGH)
		while(GPIO_ReadFromInputPin(GPIOA, GPIO_PIN_NO_0) == 0);

		SPI_PeripheralControl(SPI2, ENABLE);

		delay();

		commandcode = COMMAND_LED_READ;

		//send command
		SPI_Send(SPI2, &commandcode, 1);

		//dummy read to clear off RXNE
		SPI_Receive(SPI2, &dummy_read, 1);

		// 2. Send 1 dummy byte to fetch the ACK/NACK response from the slave
		SPI_Send(SPI2, &dummy_write, 1);

		//read the ackbyte received
		SPI_Receive(SPI2, &ackbyte, 1);

		// 3. Verify the handshake response before pushing payload arguments
		if(SPI_VerifyResponse(ackbyte)){

			args[0] = LED_PIN;

			//send arguments
			SPI_Send(SPI2, args, 1);

			//dummy read
			SPI_Receive(SPI2, &dummy_read, 1);

			//insert some delay so that slave can get ready with the data
			delay();

			//Send some dummy bits (1byte) fetch the response from the slave
			SPI_Send(SPI2, &dummy_write, 1);

			uint8_t analog_read;
			SPI_Receive(SPI2, &analog_read, 1);
			printf("SENSOR READ: %d\n", analog_read);
		}
		SPI_PeripheralControl(SPI2, DISABLE);

		//cmd4
		// Wait until User Button (PA0) is pressed (goes HIGH)
		while(GPIO_ReadFromInputPin(GPIOA, GPIO_PIN_NO_0) == 0);

		SPI_PeripheralControl(SPI2, ENABLE);

		delay();

		commandcode = COMMAND_PRINT;

		//send command
		SPI_Send(SPI2, &commandcode, 1);

		//dummy read to clear off RXNE
		SPI_Receive(SPI2, &dummy_read, 1);

		// 2. Send 1 dummy byte to fetch the ACK/NACK response from the slave
		SPI_Send(SPI2, &dummy_write, 1);

		//read the ackbyte received
		SPI_Receive(SPI2, &ackbyte, 1);

		// 3. Verify the handshake response before pushing payload arguments
		if(SPI_VerifyResponse(ackbyte)){

			args[0] = 3;
			char *str = "hi";

			//send arguments
			SPI_Send(SPI2, args, 1);

			//dummy read
			SPI_Receive(SPI2, &dummy_read, 1);

			//insert some delay so that slave can get ready with the data
			delay();

			//send arguments
			SPI_Send(SPI2, str, 3);

			delay();

			//dummy read
			SPI_Receive(SPI2, &dummy_read, 3);

		}
		SPI_PeripheralControl(SPI2, DISABLE);

		//cmd5
		// Wait until User Button (PA0) is pressed (goes HIGH)
		while(GPIO_ReadFromInputPin(GPIOA, GPIO_PIN_NO_0) == 0);

		SPI_PeripheralControl(SPI2, ENABLE);

		delay();

		commandcode = COMMAND_ID_READ;

		//send command
		SPI_Send(SPI2, &commandcode, 1);

		//dummy read to clear off RXNE
		SPI_Receive(SPI2, &dummy_read, 1);

		// 2. Send 1 dummy byte to fetch the ACK/NACK response from the slave
		SPI_Send(SPI2, &dummy_write, 1);

		//read the ackbyte received
		SPI_Receive(SPI2, &ackbyte, 1);

		uint8_t id[11];
		uint32_t i = 0;
		if( SPI_VerifyResponse(ackbyte))
		{
			// satisfy the Arduino's shared "len = SPI_SlaveReceive()" step
			SPI_Send(SPI2, &dummy_write, 1);

			//read 10 bytes id from the slave
			for(i = 0; i < 10 ; i++)
			{
				//send dummy byte to fetch data from slave
				SPI_Receive(SPI2,&id[i],1);
				SPI_Send(SPI2,&dummy_write,1);

				delay();
			}

			id[10] = '\0';

			printf("COMMAND_ID : %s \n",id);
		}

		//lets confirm SPI is not busy
		while(SPI_Get_Flag_Status(SPI2, SPI_SR_BSY) == FLAG_SET);

        // Disable the SPI2 peripheral (Hardware pulls NSS line HIGH instantly)
        SPI_PeripheralControl(SPI2, DISABLE);

        delay();

    }

    return 0;
}
