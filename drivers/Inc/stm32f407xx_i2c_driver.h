/*
 * stm32f407xx_i2c_driver.h
 *
 *  Created on: 19 Jul 2026
 *      Author: joshb
 */

#ifndef INC_STM32F407XX_I2C_DRIVER_H_
#define INC_STM32F407XX_I2C_DRIVER_H_

#include "stm32f407xx.h"

//Configuration structure for I2C peripheral
typedef struct{
	uint32_t I2C_SCLSpeed;
	uint8_t I2C_DeviceAddress;
	uint8_t I2C_ACKControl;
	uint16_t I2C_FMDutyCycle;
}I2C_Config_t;

//Handle structure for I2C peripheral
typedef struct{
	I2C_RegDef_t *pI2Cx;
	I2C_Config_t I2C_Config;
	uint8_t *pTxBuffer;
	uint8_t *pRxBuffer;
	uint32_t TxLen;
	uint32_t RxLen;
	uint8_t TxRxState;
	uint8_t DevAddr;
	uint32_t RxSize;
	uint8_t Sr;
}I2C_Handle_t;

/*************************I2C configuration macros******************************/
#define I2C_SCL_SPEED_SM       100000
#define I2C_SCL_SPEED_FM4K     400000
#define I2C_SCL_SPEED_FM2K     200000

#define I2C_ACK_ENABLE		1
#define I2C_ACK_DISABLE		0

#define I2C_FM_DUTY_2		0
#define I2C_FM_DUTY_16_9	1

//Flags for I2C status
#define I2C_FLAG_TXE		(1 << I2C_SR1_TXE)
#define I2C_FLAG_RXNE		(1 << I2C_SR1_RXNE)
#define I2C_FLAG_SB			(1 << I2C_SR1_SB)
#define I2C_FLAG_ADDR		(1 << I2C_SR1_ADDR)
#define I2C_FLAG_BTF		(1 << I2C_SR1_BTF)
#define I2C_FLAG_STOPF		(1 << I2C_SR1_STOPF)
#define I2C_FLAG_BERR		(1 << I2C_SR1_BERR)
#define I2C_FLAG_ARLO		(1 << I2C_SR1_ARLO)
#define I2C_FLAG_AF			(1 << I2C_SR1_AF)
#define I2C_FLAG_OVR		(1 << I2C_SR1_OVR)
#define I2C_FLAG_TIMEOUT	(1 << I2C_SR1_TIMEOUT)

//I2C Application States
#define I2C_READY 			0
#define I2C_BUSY_IN_RX 		1
#define I2C_BUSY_IN_TX 		2

#define READ 1
#define WRITE 0
#define I2C_SR_DI 0
#define I2C_SR_EN 1

//I2C Application Events
#define I2C_EV_TX_CMPLT 	0
#define I2C_EV_RX_CMPLT		1
#define I2C_EV_STOP			2
#define I2C_ERROR_BERR  	3
#define I2C_ERROR_ARLO  	4
#define I2C_ERROR_AF    	5
#define I2C_ERROR_OVR   	6
#define I2C_ERROR_TIMEOUT 	7
#define I2C_EV_DATA_REQ		8
#define I2C_EV_DATA_RCV     9

/*******************************API's supported by this driver*******************************/

/***************************************************************************
 * @brief	Enables or disables peripheral clock for selected I2C peripheral.
 * @param 	pI2Cx: Pointer to register definition structure
 * 			containing memory address of chosen I2C peripheral.
 * @param	EnorDi: ENABLE(1) or DISABLE(0) macro.
 */
void I2C_PeripheralClockControl(I2C_RegDef_t *pI2Cx, uint8_t EnorDi);

/***************************************************************************
 * @brief	Configures I2C peripheral with user-defined I2C handle structure.
 * @param 	pI2CHandle: Pointer to handle structure containing user-defined
 * 			configuration.
 * @note 	This function enables the peripheral clock, therefore no
 * 			requirement to enable before calling function.
 */
void I2C_Init(I2C_Handle_t *pI2CHandle);

/***************************************************************************
 * @brief	Resets all registers in corresponding I2C peripheral.
 * @param 	pI2Cx: Pointer to register definition structure
 * 			containing memory address of chosen I2C peripheral.
 * @note 	This function resets the physical peripheral registers but not
 * 			the handle structure.
 */
void I2C_DeInit(I2C_RegDef_t *pI2Cx);

/***************************************************************************
 * @brief	Sends a complete transmission over I2C.
 * @param 	pI2CHandle: Pointer to handle structure containing user-defined
 * 			configuration.
 * @param	pTxBuffer: Pointer to the user data buffer
 * 			holding the data to be transmitted.
 * @param	len: Specifies the length (in bytes) of the transmission.
 * @param	slave_address: 7 bit address of the chosen slave to send the
 * 			transmission to.
 * @param	Sr: Option for repeated start condition, I2C_SR_EN to enable and
 * 			I2C_SR_DI to disable.
 * @note	This is a blocking function.
 */
void I2C_MasterSendData(I2C_Handle_t *pI2CHandle, uint8_t *pTxBuffer, uint32_t len, uint8_t slave_address, uint8_t Sr);

/***************************************************************************
 * @brief	Receives data from slave device over I2C peripheral.
 * @param  	pI2CHandle: Pointer to handle structure containing user-defined
 * 			configuration.
 * @param 	pRxBuffer: Pointer to the user receive buffer where the received
 * 			data will land.
 * @param	len: Specifies the length (in bytes) of the data to be received.
 * @param	slave_address: 7 bit address of the chosen slave to read data from.
 * @param	Sr: Option for repeated start condition, I2C_SR_EN to enable and
 * 			I2C_SR_DI to disable.
 * @note	This is a blocking function.
 */
void I2C_MasterReceiveData(I2C_Handle_t *pI2CHandle, uint8_t *pRxBuffer, uint32_t len, uint8_t slave_address, uint8_t Sr);

/***************************************************************************
 * @brief	Initiates interrupt driven data transmission over I2C.
 * @param  	pI2CHandle: Pointer to handle structure containing user-defined
 * 			configuration.
 * @param 	pTxBuffer: Pointer to the user data buffer that will be assigned
 *  		to the handle structure.
 * @param	len: Specifies the length (in bytes) of the data to be sent.
 * @param	slave_address: 7 bit address of the chosen slave to send data to.
 * @param	Sr: Option for repeated start condition, I2C_SR_EN to enable and
 * 			I2C_SR_DI to disable.
 * @note 	This is a non-blocking function.
 */
uint8_t I2C_MasterSendDataIT(I2C_Handle_t *pI2CHandle, uint8_t *pTxBuffer, uint32_t len, uint8_t slave_address, uint8_t Sr);

/***************************************************************************
 * @brief	Initiates interrupt driven reception of data from slave over I2C.
 * @param  	pI2CHandle: Pointer to handle structure containing user-defined
 * 			configuration.
 * @param 	pRxBuffer: Pointer to the user data buffer that will be assigned
 *  		to the handle structure.
 * @param	len: Specifies the length (in bytes) of the data to be received.
 * @param	slave_address: 7 bit address of the chosen slave to receive data
 * 			from.
 * @param	Sr: Option for repeated start condition, I2C_SR_EN to enable and
 * 			I2C_SR_DI to disable.
 * @note 	This is a non-blocking function.
 */
uint8_t I2C_MasterReceiveDataIT(I2C_Handle_t *pI2CHandle, uint8_t *pRxBuffer, uint32_t len, uint8_t slave_address, uint8_t Sr);

/***************************************************************************
 * @brief	Disables I2C EV and BUF interrupts, resets all fields of handle
 * 			structure configured by I2C_MasterSendDataIT.
 * @param	pI2CHandle: Pointer to handle structure containing user-defined
 * 			configuration.
 */
void I2C_CloseSendData(I2C_Handle_t *pI2CHandle);

/***************************************************************************
 * @brief	Disables I2C EV and BUF interrupts, resets all fields of handle
 * 			structure configured by I2C_MasterSendDataIT.
 * @param	pI2CHandle: Pointer to handle structure containing user-defined
 * 			configuration.
 */
void I2C_CloseReceiveData(I2C_Handle_t *pI2CHandle);

void I2C_IRQITConfig(uint8_t IRQ_Number, uint8_t EnorDi);
void I2C_IRQPriorityConfig(uint8_t IRQ_Number, uint32_t IRQPriority);

/***************************************************************************
 * @brief	Handles all I2C event interrupts through checking the status
 * 			flags in the SR registers, calls relevant functions for each
 * 			interrupt and notifies application layer.
 * @param	pI2CHandle: Pointer to handle structure containing user-defined
 * 			configuration and hardware configuration register data.
 */
void I2C_EVIRQHandling(I2C_Handle_t *pI2CHandle);

/***************************************************************************
  * @brief	Handles all I2C error interrupts by checking the status flags
  * 		in the SR registers and clearing the relevant flag before
  * 		notifying the application layer.
 * @param	pI2CHandle: Pointer to handle structure containing user-defined
 * 			configuration and hardware configuration register data.
 */
void I2C_ERIRQHandling(I2C_Handle_t *pI2CHandle);

/***************************************************************************
 * @brief	Loads given data into data register.
 * @param 	pI2Cx: Pointer to register definition structure
 * 			containing memory address of chosen I2C peripheral.
 * @param	data: Value containing the data to be loaded into the data
 * 			register.
 */
void I2C_SlaveSendData(I2C_RegDef_t *pI2Cx, uint8_t data);

/***************************************************************************
 * @brief	Returns data inside data register.
 * @param 	pI2Cx: Pointer to register definition structure
 * 			containing memory address of chosen I2C peripheral.
 * @retval	8 bit integer containing the byte of data fetched from the data
 * 			register.
 */
uint8_t I2C_SlaveReceiveData(I2C_RegDef_t *pI2Cx);

/***************************************************************************
 * @brief	Enables or Disables I2C peripheral.
 * @param 	pI2Cx: Pointer to register definition structure
 * 			containing memory address of chosen I2C peripheral.
 * @param	EnorDi: ENABLE(1) or DISABLE(0) macro.
 */
void I2C_PeripheralControl(I2C_RegDef_t *pI2Cx, uint8_t EnorDi);

/***************************************************************************
 * @brief	Returns the status of specified flag inside the SR1 register.
 * @param 	pI2Cx: Pointer to register definition structure
 * 			containing memory address of chosen I2C peripheral.
 * @param	FlagName: Macro representing chosen flag of SR1 register to
 * 			be checked. (eg. I2C_FLAG_TXE).
 * @note 	Must use pre shifted flag macros provided in this header file.
 */
uint8_t I2C_GetFlagStatus(I2C_RegDef_t *pI2Cx, uint32_t FlagName);

/***************************************************************************
 * @brief	Enables or disables the ack bit inside the CR1 register.
 * @param 	pI2Cx: Pointer to register definition structure
 * 			containing memory address of chosen I2C peripheral.
 * @param	EnorDi: ENABLE(1) or DISABLE(0) macro.
 */
void I2C_ToggleAcking(I2C_RegDef_t *pI2Cx, uint8_t EnorDi);

/***************************************************************************
 * @brief	Either enables or disables all 3 I2C interrupts.
 * @param 	pI2Cx: Pointer to register definition structure
 * 			containing memory address of chosen I2C peripheral.
 * @param	EnorDi: ENABLE(1) or DISABLE(0) macro.
 */
void I2C_SlaveEnableDisableCallbackEvents(I2C_RegDef_t *pI2Cx, uint8_t EnorDi);

/***************************************************************************
 * @brief	Generates stop condition (SDA low to high while SCL high) by
 * 			setting stop bit in CR1 to high.
 * @param 	pI2Cx: Pointer to register definition structure
 * 			containing memory address of chosen I2C peripheral.
 * @note 	Stop condition will only be generated if device is in master mode.
 */
void I2C_GenerateStopCondition(I2C_RegDef_t *pI2Cx);

/***************************************************************************
 * @brief	Generates start condition (SDA high to low while SCL high) by
 * 			setting start bit in CR1 to high.
 * @param 	pI2Cx: Pointer to register definition structure
 * 			containing memory address of chosen I2C peripheral.
 * @note 	If device is in master mode and is already controlling the I2C
 * 			bus then a repeated start condition is generated.
 */
void I2C_GenerateStartCondition(I2C_RegDef_t *pI2Cx);

/***************************************************************************
 * @brief	This function allow the ISR to inform the application. This
 * 			function will be implemented by the application.
 * @param	pI2CHandle: Pointer to handle structure containing user-defined
 * 			configuration.
 * @param	AppEv: Application event macro (eg. I2C_EV_TX_CMPLT).
 */
__weak void I2C_ApplicationEventCallback(I2C_Handle_t *pI2CHandle, uint8_t AppEv);



#endif /* INC_STM32F407XX_I2C_DRIVER_H_ */
