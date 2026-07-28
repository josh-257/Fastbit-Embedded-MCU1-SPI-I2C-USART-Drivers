/*
 * stm32f407xx_driver_SPI.h
 *
 *  Created on: 15 May 2026
 *      Author: joshb
 */

#ifndef INC_STM32F407XX_SPI_DRIVER_H_
#define INC_STM32F407XX_SPI_DRIVER_H_

#include "stm32f407xx.h"

//Configuration structure for SPI peripheral
typedef struct {
	uint8_t SPI_DeviceMode;
	uint8_t SPI_BusConfig;
	uint8_t SPI_SclkSpeed;
	uint8_t SPI_DFF;
	uint8_t SPI_CPOL;
	uint8_t SPI_CPHA;
	uint8_t SPI_SSM;
}SPI_Config_t;

//Handle structure for SPI peripheral
typedef struct{
	SPI_RegDef_t *pSPIx;
	SPI_Config_t SPIConfig;
	uint8_t		 *pTxbuffer;
	uint8_t		 *pRxbuffer;
	uint32_t	 Txlen;
	uint32_t	 Rxlen;
	uint8_t 	 TxState;
	uint8_t 	 RxState;
}SPI_Handle_t;

/** @SPI_DEVICE_MODES
 *
 */
#define SPI_MODE_SLAVE					0
#define SPI_MODE_MASTER					1

/** @SPI_BUS_CONFIG
 *
 */
#define SPI_BUS_CFG_FD					1
#define SPI_BUS_CFG_HD					2
#define SPI_BUS_CFG_SIMPLEX_RXONLY		3

/** @SPI_SCLK_SPEED
 *
 */
#define SPI_SCLK_SPEED_DIV2				0
#define SPI_SCLK_SPEED_DIV4				1
#define SPI_SCLK_SPEED_DIV8				2
#define SPI_SCLK_SPEED_DIV16			3
#define SPI_SCLK_SPEED_DIV32			4
#define SPI_SCLK_SPEED_DIV64			5
#define SPI_SCLK_SPEED_DIV128			6
#define SPI_SCLK_SPEED_DIV256			7

/** @SPI_DFF
 *
 */
#define SPI_DFF_8						0
#define SPI_DFF_16						1

/** @SPI_CPOL
 *
 */
#define SPI_CPOL_HIGH					1
#define SPI_CPOL_LOW					0

/** @SPI_CPHA
 *
 */
#define SPI_CPHA_HIGH					1
#define SPI_CPHA_LOW					0

/** @SPI_SSM
 *
 */
#define SPI_SSM_EN						1
#define SPI_SSM_DI						0

/** SPI application states
 *
 */
#define SPI_READY						0
#define SPI_BUSY_IN_RX					1
#define SPI_BUSY_IN_TX					2

/** Possible SPI Application events
 *
 */
#define SPI_EVENT_TX_CMPLT 				1
#define SPI_EVENT_RX_CMPLT 				2
#define SPI_EVENT_OVR_ERR 				3
#define SPI_EVENT_CRC_ERR 				4



// Peripheral clock setup
void SPI_PeriClockControl(SPI_RegDef_t *pSPIx, uint8_t EnorDi);

//Init and De-init
void SPI_Init(SPI_Handle_t *pSPIHandle);
void SPI_DeInit(SPI_RegDef_t *pSPIx);

//Data send and receive
void SPI_Send(SPI_RegDef_t *pSPIx, uint8_t *pTxBuffer, uint32_t len);
void SPI_Receive(SPI_RegDef_t *pSPIx, uint8_t *pRxBuffer, uint32_t len);

uint8_t SPI_SendIT(SPI_Handle_t *pSPIHandle, uint8_t *pTxBuffer, uint32_t len);
uint8_t SPI_ReceiveIT(SPI_Handle_t *pSPIHandle, uint8_t *pRxBuffer, uint32_t len);
//IRQ configuration and ISR handling
void SPI_IRQITConfig(uint8_t IRQ_Number, uint8_t EnorDi);
void SPI_IRQPriorityConfig(uint8_t IRQ_Number, uint32_t IRQPriority);
void SPI_IRQHandling(SPI_Handle_t *pHandle);

//enable SPI peripheral
void SPI_PeripheralControl(SPI_RegDef_t *pSPIx, uint8_t EnOrDi);
void SPI_SSIConfig(SPI_RegDef_t *pSPIx, uint8_t EnOrDi);
void SPI_SSOEConfig(SPI_RegDef_t *pSPIx, uint8_t EnOrDi);
uint8_t SPI_Get_Flag_Status(SPI_RegDef_t *pSPIx, uint32_t FlagName);
void SPI_Clear_OVR_Flag(SPI_RegDef_t *pSPIx);
void SPI_Close_Transmission(SPI_Handle_t *pSPIHandle);
void SPI_Close_Reception(SPI_Handle_t *pSPIHandle);

//application callback
__weak void SPI_Application_Event_Callback(SPI_Handle_t *pSPIHandle, uint8_t AppEv);



#endif /* INC_STM32F407XX_SPI_DRIVER_H_ */
