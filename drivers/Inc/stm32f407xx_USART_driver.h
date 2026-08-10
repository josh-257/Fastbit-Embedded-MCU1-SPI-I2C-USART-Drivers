/*
 * stm32f407xx_USART_driver.h
 *
 *  Created on: 31 Jul 2026
 *      Author: joshb
 */

#ifndef INC_STM32F407XX_USART_DRIVER_H_
#define INC_STM32F407XX_USART_DRIVER_H_


#include "stm32f407xx.h"

//Configuration structure for USARTx peripheral
typedef struct
{
	uint8_t USART_Mode;
	uint32_t USART_Baud;
	uint8_t USART_NoOfStopBits;
	uint8_t USART_WordLength;
	uint8_t USART_ParityControl;
	uint8_t USART_HWFlowControl;
}USART_Config_t;


//Handle structure for USARTx peripheral
typedef struct
{
	USART_RegDef_t *pUSARTx;
	USART_Config_t  USART_Config;
	uint8_t *pTxBuffer;
	uint8_t *pRxBuffer;
	uint32_t TxLen;
	uint32_t RxLen;
	uint8_t TxBusyState;
	uint8_t RxBusyState;
	uint32_t RxSize;
}USART_Handle_t;

//Possible options for USART_Mode
#define USART_MODE_ONLY_TX 0
#define USART_MODE_ONLY_RX 1
#define USART_MODE_TXRX  2

//Possible options for USART_Baud
#define USART_STD_BAUD_1200					1200
#define USART_STD_BAUD_2400					2400
#define USART_STD_BAUD_9600					9600
#define USART_STD_BAUD_19200 				19200
#define USART_STD_BAUD_38400 				38400
#define USART_STD_BAUD_57600 				57600
#define USART_STD_BAUD_115200 				115200
#define USART_STD_BAUD_230400 				230400
#define USART_STD_BAUD_460800 				460800
#define USART_STD_BAUD_921600 				921600
#define USART_STD_BAUD_2M 					2000000
#define SUART_STD_BAUD_3M 					3000000


//Possible options for USART_ParityControl
#define USART_PARITY_EN_ODD   2
#define USART_PARITY_EN_EVEN  1
#define USART_PARITY_DISABLE   0


//Possible options for USART_WordLength
#define USART_WORDLEN_8BITS  0
#define USART_WORDLEN_9BITS  1


//Possible options for USART_NoOfStopBits
#define USART_STOPBITS_1     0
#define USART_STOPBITS_0_5   1
#define USART_STOPBITS_2     2
#define USART_STOPBITS_1_5   3

//Possible options for USART_HWFlowControl
#define USART_HW_FLOW_CTRL_NONE    	0
#define USART_HW_FLOW_CTRL_CTS    	1
#define USART_HW_FLOW_CTRL_RTS    	2
#define USART_HW_FLOW_CTRL_CTS_RTS	3

//Application states
#define USART_READY			0
#define USART_BUSY_IN_TX 	1
#define USART_BUSY_IN_RX	2

//Application event states
#define USART_EVENT_TX_CMPLT 0
#define USART_EVENT_RX_CMPLT 1
#define USART_EVENT_IDLE     2
#define USART_EVENT_CTS      3
#define USART_EVENT_PE       4
#define USART_ERR_FE     	 5
#define USART_ERR_NE    	 6
#define USART_ERR_ORE    	 7

//Flags for SR register
#define USART_FLAG_PE      (1 << 0)
#define USART_FLAG_FE      (1 << 1)
#define USART_FLAG_NE      (1 << 2)
#define USART_FLAG_ORE     (1 << 3)
#define USART_FLAG_IDLE    (1 << 4)
#define USART_FLAG_RXNE    (1 << 5)
#define USART_FLAG_TC      (1 << 6)
#define USART_FLAG_TXE     (1 << 7)
#define USART_FLAG_LBD     (1 << 8)
#define USART_FLAG_CTS     (1 << 9)

/*******************************API's supported by this driver*******************************/

/***************************************************************************
 * @brief	Enables or disables peripheral clock for selected USART peripheral.
 * @param 	pUSARTx: Pointer to register definition structure
 * 			containing memory address of chosen USART peripheral.
 * @param	EnorDi: ENABLE(1) or DISABLE(0) macro.
 */
void USART_PeripheralClockControl(USART_RegDef_t *pUSARTx, uint8_t EnorDi);

/***************************************************************************
 * @brief	Configures USART peripheral with user-defined USART handle structure.
 * @param 	pUSARTHandle: Pointer to handle structure containing user-defined
 * 			configuration.
 * @note 	This function enables the peripheral clock, therefore no
 * 			requirement to enable before calling function.
 */
void USART_Init(USART_Handle_t *pUSARTHandle);

/***************************************************************************
 * @brief	Resets all registers in corresponding USART peripheral.
 * @param 	pUSARTx: Pointer to register definition structure
 * 			containing memory address of chosen USART peripheral.
 * @note 	This function resets the physical peripheral registers but not
 * 			the handle structure.
 */
void USART_DeInit(USART_RegDef_t *pUSARTx);

/***************************************************************************
 * @brief	Transmits data (len number of bytes) over USART.
 * @param 	pUSARTx: Pointer to register definition structure
 * 			containing memory address of chosen USART peripheral.
 * @param	pTxBuffer: Pointer to the user data buffer
 * 			holding the data to be transmitted.
 * @param	len: Specifies the length (in bytes) of the transmission.
 * @note	This function will send in either 9 bit or 8 bit lengths (with
 * 			or without parity). This is a blocking function.
 */
void USART_SendData(USART_Handle_t *pUSARTHandle, uint8_t *pTxBuffer, uint32_t Len);

/***************************************************************************
 * @brief	Receives data (len number of bytes) from slave device over USART.
 * @param 	pUSARTx: Pointer to register definition structure
 * 			containing memory address of chosen USART peripheral.
 * @param 	pRxBuffer: Pointer to the user receive buffer where the received
 * 			data will land.
 * @param	len: Specifies the length (in bytes) of the data to be received.
 * @note	This is a blocking function.
 */
void USART_ReceiveData(USART_Handle_t *pUSARTHandle, uint8_t *pRxBuffer, uint32_t Len);

/***************************************************************************
 * @brief	Initiates interrupt driven data transmission over USART by
 * 			enabling TXEIE and TCIE bit.
 * @param  	pUSARTHandle: Pointer to handle structure containing user-defined
 * 			configuration.
 * @param 	pTxBuffer: Pointer to the user data buffer that will be assigned
 *  		to the handle structure.
 * @param	len: Specifies the length (in bytes) of the data to be sent.
 * @retval	TxBusyState that will be assigned to the handle structure indicating
 * 			that the transmission is in progress.
 * @note 	This is a non-blocking function.
 */
uint8_t USART_SendDataIT(USART_Handle_t *pUSARTHandle,uint8_t *pTxBuffer, uint32_t Len);

/***************************************************************************
 * @brief	Initiates interrupt driven data reception over USART by
 * 			enabling RXNEIE bit.
 * @param  	pUSARTHandle: Pointer to handle structure containing user-defined
 * 			configuration.
 * @param 	pRxBuffer: Pointer to the user data buffer where the data will
 * 			land.
 * @retval	RxBusyState that will be assigned to the handle structure
 * 			indicating that the reception is in progress.
 * @param	len: Specifies the length (in bytes) of the data to be sent.
 * @note 	This is a non-blocking function.
 */
uint8_t USART_ReceiveDataIT(USART_Handle_t *pUSARTHandle, uint8_t *pRxBuffer, uint32_t Len);


void USART_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi);
void USART_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority);

/***************************************************************************
 * @brief	Handles USART interrupts for transmission, receive and over run
 * 			errors through checking the status flags in the SR registers,
 * 			calls relevant functions for each interrupt and notifies
 * 			application layer.
 * @param	pUSARTHandle: Pointer to handle structure containing user-defined
 * 			configuration and hardware configuration register data.
 * @note	Not currently configured to handle PE, CTS and LBD interrupts.
 */
void USART_IRQ_Handling(USART_Handle_t *pHandle);

/***************************************************************************
 * @brief	Enables or Disables USART peripheral.
 * @param 	pUSARTx: Pointer to register definition structure
 * 			containing memory address of chosen USART peripheral.
 * @param	EnorDi: ENABLE(1) or DISABLE(0) macro.
 */
void USART_PeripheralControl(USART_RegDef_t *pUSARTx, uint8_t EnOrDi);

/***************************************************************************
 * @brief	Returns the status of specified flag inside the SR1 register.
 * @param 	pUSARTx: Pointer to register definition structure
 * 			containing memory address of chosen USART peripheral.
 * @param	FlagName: Macro representing chosen flag of SR1 register to
 * 			be checked. (eg. USART_FLAG_TXE).
 * @note 	Must use pre shifted flag macros provided in this header file.
 */
uint8_t USART_GetFlagStatus(USART_RegDef_t *pUSARTx, uint8_t StatusFlagName);

/***************************************************************************
 * @brief	Clears specified flag by writing 0 to flag bit in SR.
 * @param 	pUSARTx: Pointer to register definition structure
 * 			containing memory address of chosen USART peripheral.
 * @param	FlagName: Macro representing chosen flag of SR1 register to
 * 			be cleared. (eg. USART_FLAG_TXE).
 * @note 	Must use pre shifted flag macros provided in this header file.
 */
void USART_ClearFlag(USART_RegDef_t *pUSARTx, uint16_t StatusFlagName);

/***************************************************************************
 * @brief	Programs given baud rate into the baud rate register.
 * @param 	pUSARTx: Pointer to register definition structure
 * 			containing memory address of chosen USART peripheral.
 * @param	BaudRate: The desired baud rate to be programmed.
 */
void USART_SetBaudRate(USART_RegDef_t *pUSARTx, uint32_t BaudRate);

/***************************************************************************
 * @brief	Clears over run error flag by reading both DR and SR.
 * @param 	pUSARTx: Pointer to register definition structure
 * 			containing memory address of chosen USART peripheral.
 */
void USART_ClearOVRRunFlag(USART_RegDef_t *pUSART);

/***************************************************************************
 * @brief	This function allow the ISR to inform the application. This
 * 			function will be implemented by the application.
 * @param	pUSARTHandle: Pointer to handle structure containing user-defined
 * 			configuration.
 * @param	AppEv: Application event macro (eg. USART_EVENT_TX_CMPLT).
 */
void USART_ApplicationEventCallback(USART_Handle_t *pUSARTHandle,uint8_t AppEv);

#endif /* INC_STM32F407XX_USART_DRIVER_H_ */
