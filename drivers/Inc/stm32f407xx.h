/*
 * stm32f407xx.h
 *
 *  Created on: 7 May 2026
 *      Author: joshb
 */

#ifndef INC_STM32F407XX_H_
#define INC_STM32F407XX_H_

#include <stdio.h>
#include <stdint.h>

#define __weak __attribute__((weak))

/*****************************START: Processor Specific details***************************/

//ARM Cortex Mx Processor NVIC ISERx register Addresses

#define NVIC_ISER0					((volatile uint32_t*)0xE000E100)
#define NVIC_ISER1					((volatile uint32_t*)0xE000E104)
#define NVIC_ISER2					((volatile uint32_t*)0xE000E108)
#define NVIC_ISER3					((volatile uint32_t*)0xE000E10C)


//ARM Cortex Mx Processor NVIC ICERx register Addresses

#define NVIC_ICER0					((volatile uint32_t*)0xE000E180)
#define NVIC_ICER1					((volatile uint32_t*)0xE000E184)
#define NVIC_ICER2					((volatile uint32_t*)0xE000E188)
#define NVIC_ICER3					((volatile uint32_t*)0xE000E18C)

//ARM Cortex Mx Processor Priority Register Address calculation

#define NVIC_PR_BASE_ADDR			((volatile uint32_t*)0xE000E400)

//ARM Cortex Mx Processor Number of Priority bits implemented in Priority register

#define NO_PR_BITS_IMPLEMENTED		4

/********************************START: MCU Specific details************************/

//Base addresses of Flash and SRAM memory
#define FLASH_BASE_ADDRESS			0x08000000U //start of flash memory
#define SRAM1_BASE_ADDRESS			0x20000000U
#define SRAM2_BASE_ADDRESS			0x2001C000U
#define ROM_BASE_ADDR				0x1FFF0000U //start of ROM
#define SRAM 						SRAM1_BASE_ADDRESS

#define PERIPH_BASE					0x40000000U
#define APB1_PERIPH_BASE			PERIPH_BASE
#define APB2_PERIPH_BASE			0x40010000U
#define AHB1_PERIPH_BASE			0x40020000U
#define AHB2_PERIPH_BASE			0x50000000U

//Base addresses of peripherals on AHB1 bus

#define GPIOA_BASE_ADDR				(AHB1_PERIPH_BASE + 0x0000)
#define GPIOB_BASE_ADDR				(AHB1_PERIPH_BASE + 0x0400)
#define GPIOC_BASE_ADDR				(AHB1_PERIPH_BASE + 0x0800)
#define GPIOD_BASE_ADDR				(AHB1_PERIPH_BASE + 0x0C00)
#define GPIOE_BASE_ADDR				(AHB1_PERIPH_BASE + 0x1000)
#define GPIOF_BASE_ADDR				(AHB1_PERIPH_BASE + 0x1400)
#define GPIOG_BASE_ADDR				(AHB1_PERIPH_BASE + 0x1800)
#define GPIOH_BASE_ADDR				(AHB1_PERIPH_BASE + 0x1C00)
#define GPIOI_BASE_ADDR				(AHB1_PERIPH_BASE + 0x2000)
#define RCC_BASE_ADDR				(AHB1_PERIPH_BASE + 0x3800)

//Base addresses of peripherals on APB1

#define I2C1_BASE_ADDR				(APB1_PERIPH_BASE + 0x5400)
#define I2C2_BASE_ADDR				(APB1_PERIPH_BASE + 0x5800)
#define I2C3_BASE_ADDR				(APB1_PERIPH_BASE + 0x5C00)

#define SPI2_BASE_ADDR				(APB1_PERIPH_BASE + 0x3800)
#define SPI3_BASE_ADDR 				(APB1_PERIPH_BASE + 0x3C00)

#define USART2_BASE_ADDR			(APB1_PERIPH_BASE + 0x4400)
#define USART3_BASE_ADDR			(APB1_PERIPH_BASE + 0x4800)
#define UART4_BASE_ADDR				(APB1_PERIPH_BASE + 0x4C00)
#define UART5_BASE_ADDR				(APB1_PERIPH_BASE + 0x5000)

//Base addresses of peripherals on APB2

#define EXTI_BASE_ADDR				(APB2_PERIPH_BASE + 0x3C00)
#define SPI1_BASE_ADDR				(APB2_PERIPH_BASE + 0x3000)
#define SPI4_BASE_ADDR				(APB2_PERIPH_BASE + 0x3400)
#define SPI5_BASE_ADDR				(APB2_PERIPH_BASE + 0x5000)
#define SPI6_BASE_ADDR				(APB2_PERIPH_BASE + 0x5400)
#define SYSCFG_BASE_ADDR			(APB2_PERIPH_BASE + 0x3800)
#define USART1_BASE_ADDR			(APB2_PERIPH_BASE + 0x1000)
#define USART6_BASE_ADDR			(APB2_PERIPH_BASE + 0x1400)

/**********************Peripheral register definition structures*****************************************************/

typedef struct {
	volatile uint32_t MODER; 		// GPIO mode register				                    Address offset:0x00
	volatile uint32_t OTYPER; 		// Output type register				                    Address offset:0x04
	volatile uint32_t OSPEEDR; 		// Output speed register			                    Address offset:0x08
	volatile uint32_t PUPDR; 		// Pull up, Pull down register		                    Address offset:0x0C
	volatile uint32_t IDR; 			// Input data register				                    Address offset:0x10
	volatile uint32_t ODR; 			// Output data register				                    Address offset:0x14
	volatile uint32_t BSRR; 		// Bit set/reset register			                    Address offset:0x18
	volatile uint32_t LCKR; 		// Configuration lock register		                    Address offset:0x1C
	volatile uint32_t AFR[2]; 		// Alternate function low (afr[0]) and high (afr[1]) register	Address offset:0x20-0x24
}GPIO_RegDef_t;

typedef struct {
    volatile uint32_t CR;           // RCC control register                                 Address offset: 0x00
    volatile uint32_t PLLCFGR;      // RCC PLL configuration register                       Address offset: 0x04
    volatile uint32_t CFGR;         // RCC clock configuration register                     Address offset: 0x08
    volatile uint32_t CIR;          // RCC clock interrupt register                         Address offset: 0x0C
    volatile uint32_t AHB1RSTR;     // RCC AHB1 peripheral reset register                   Address offset: 0x10
    volatile uint32_t AHB2RSTR;     // RCC AHB2 peripheral reset register                   Address offset: 0x14
    volatile uint32_t AHB3RSTR;     // RCC AHB3 peripheral reset register                   Address offset: 0x18
    uint32_t reserved0;             // Reserved                                             Address offset: 0x1C
    volatile uint32_t APB1RSTR;     // RCC APB1 peripheral reset register                   Address offset: 0x20
    volatile uint32_t APB2RSTR;     // RCC APB2 peripheral reset register                   Address offset: 0x24
    uint32_t reserved1[2];          // Reserved                                             Address offset: 0x28-0x2C
    volatile uint32_t AHB1ENR;      // RCC AHB1 peripheral clock enable register            Address offset: 0x30
    volatile uint32_t AHB2ENR;      // RCC AHB2 peripheral clock enable register            Address offset: 0x34
    volatile uint32_t AHB3ENR;      // RCC AHB3 peripheral clock enable register            Address offset: 0x38
    uint32_t reserved2;             // Reserved                                             Address offset: 0x3C
    volatile uint32_t APB1ENR;      // RCC APB1 peripheral clock enable register            Address offset: 0x40
    volatile uint32_t APB2ENR;      // RCC APB2 peripheral clock enable register            Address offset: 0x44
    uint32_t reserved3[2];          // Reserved                                             Address offset: 0x48-0x4C
    volatile uint32_t AHB1LPENR;    // RCC AHB1 peripheral clock enable in low power mode   Address offset: 0x50
    volatile uint32_t AHB2LPENR;    // RCC AHB2 peripheral clock enable in low power mode   Address offset: 0x54
    volatile uint32_t AHB3LPENR;    // RCC AHB3 peripheral clock enable in low power mode   Address offset: 0x58
    uint32_t reserved4;             // Reserved                                             Address offset: 0x5C
    volatile uint32_t APB1LPENR;    // RCC APB1 peripheral clock enable in low power mode   Address offset: 0x60
    volatile uint32_t APB2LPENR;    // RCC APB2 peripheral clock enable in low power mode   Address offset: 0x64
    uint32_t reserved5[2];          // Reserved                                             Address offset: 0x68-0x6C
    volatile uint32_t BDCR;         // RCC Backup domain control register                   Address offset: 0x70
    volatile uint32_t CSR;          // RCC clock control & status register                  Address offset: 0x74
    uint32_t reserved6[2];          // Reserved                                             Address offset: 0x78-0x7C
    volatile uint32_t SSCGR;        // RCC spread spectrum clock generation register        Address offset: 0x80
    volatile uint32_t PLLI2SCFGR;   // RCC PLLI2S configuration register                    Address offset: 0x84
} RCC_RegDef_t;

typedef struct {
	volatile uint32_t IMR;   		// Interrupt mask register                       		Address offset: 0x00
	volatile uint32_t EMR;   		// Event mask register                           		Address offset: 0x04
	volatile uint32_t RTSR;  		// Rising trigger selection register             		Address offset: 0x08
	volatile uint32_t FTSR;  		// Falling trigger selection register            		Address offset: 0x0C
	volatile uint32_t SWIER; 		// Software interrupt event register             		Address offset: 0x10
	volatile uint32_t PR;    		// Pending register                              		Address offset: 0x14
} EXTI_RegDef_t;

typedef struct {
	volatile uint32_t MEMRMP;   	// Memory remap register                         		Address offset: 0x00
	volatile uint32_t PMC;      	// Peripheral mode configuration register        		Address offset: 0x04
	volatile uint32_t EXTICR[4];  	// External interrupt configuration register 1-4   		Address offset: 0x08-0x14
	volatile uint32_t CMPCR;    	// Compensation cell control register            		Address offset: 0x18
} SYSCFG_RegDef_t;

typedef struct{
	volatile uint32_t CR1;     		// Control register 1                                	Address offset: 0x00
	volatile uint32_t CR2;     		// Control register 2                                	Address offset: 0x04
	volatile uint32_t SR;      		// Status register                                   	Address offset: 0x08
	volatile uint32_t DR;      		// Data register                                     	Address offset: 0x0C
	volatile uint32_t CRCPR;   		// CRC polynomial register                           	Address offset: 0x10
	volatile uint32_t RXCRCR;  		// RX CRC register                                   	Address offset: 0x14
	volatile uint32_t TXCRCR;  		// TX CRC register                                   	Address offset: 0x18
	volatile uint32_t I2SCFGR; 		// I2S configuration register                        	Address offset: 0x1C
	volatile uint32_t I2SPR;   		// I2S prescaler register                            	Address offset: 0x20
}SPI_RegDef_t;

typedef struct {
	volatile uint32_t CR1;          // Control register 1                                   Address Offset: 0x00
	volatile uint32_t CR2;          // Control register 2                                   Address Offset: 0x04
	volatile uint32_t OAR1;         // Own address register 1                               Address Offset: 0x08
	volatile uint32_t OAR2;         // Own address register 2                               Address Offset: 0x0C
	volatile uint32_t DR;           // Data register                                        Address Offset: 0x10
	volatile uint32_t SR1;          // Status register 1                                    Address Offset: 0x14
	volatile uint32_t SR2;          // Status register 2                                    Address Offset: 0x18
	volatile uint32_t CCR;          // Clock control register                               Address Offset: 0x1C
	volatile uint32_t TRISE;        // TRISE register                                       Address Offset: 0x20
	volatile uint32_t FLTR;         // FLTR register                                        Address Offset: 0x24
}I2C_RegDef_t;

typedef struct {
	volatile uint32_t SR;			// Status Register                                      Address Offset: 0x00
	volatile uint32_t DR;			// Data Register                                        Address Offset: 0x04
	volatile uint32_t BRR;			// Baud Rate Register                                   Address Offset: 0x08
	volatile uint32_t CR1;			// Control Register 1                                   Address Offset: 0x0C
	volatile uint32_t CR2;			// Control Register 2                                   Address Offset: 0x10
	volatile uint32_t CR3;			// Control Register 3                                   Address Offset: 0x14
	volatile uint32_t GTPR;			// Guard Time and Prescaler Register                    Address Offset: 0x18
} USART_RegDef_t;

//Peripheral definitions
#define GPIOA 						((GPIO_RegDef_t*)GPIOA_BASE_ADDR)
#define GPIOB 						((GPIO_RegDef_t*)GPIOB_BASE_ADDR)
#define GPIOC 						((GPIO_RegDef_t*)GPIOC_BASE_ADDR)
#define GPIOD 						((GPIO_RegDef_t*)GPIOD_BASE_ADDR)
#define GPIOE 						((GPIO_RegDef_t*)GPIOE_BASE_ADDR)
#define GPIOF 						((GPIO_RegDef_t*)GPIOF_BASE_ADDR)
#define GPIOG 						((GPIO_RegDef_t*)GPIOG_BASE_ADDR)
#define GPIOH 						((GPIO_RegDef_t*)GPIOH_BASE_ADDR)
#define GPIOI 						((GPIO_RegDef_t*)GPIOI_BASE_ADDR)

#define RCC							((RCC_RegDef_t*)RCC_BASE_ADDR)

#define EXTI						((EXTI_RegDef_t*)EXTI_BASE_ADDR)

#define SYSCFG						((SYSCFG_RegDef_t*)SYSCFG_BASE_ADDR)

#define SPI1						((SPI_RegDef_t*)SPI1_BASE_ADDR)
#define SPI2						((SPI_RegDef_t*)SPI2_BASE_ADDR)
#define SPI3						((SPI_RegDef_t*)SPI3_BASE_ADDR)

#define I2C1						((I2C_RegDef_t*)I2C1_BASE_ADDR)
#define I2C2						((I2C_RegDef_t*)I2C2_BASE_ADDR)
#define I2C3						((I2C_RegDef_t*)I2C3_BASE_ADDR)

#define USART1						((USART_RegDef_t*)USART1_BASE_ADDR)
#define USART2						((USART_RegDef_t*)USART2_BASE_ADDR)
#define USART3						((USART_RegDef_t*)USART3_BASE_ADDR)
#define UART4						((USART_RegDef_t*)UART4_BASE_ADDR)
#define UART5						((USART_RegDef_t*)UART5_BASE_ADDR)
#define USART6						((USART_RegDef_t*)USART6_BASE_ADDR)

//Clock enable macros for GPIOx peripherals
#define GPIOA_PCLK_EN()				(RCC->AHB1ENR |= (1 << 0))
#define GPIOB_PCLK_EN()				(RCC->AHB1ENR |= (1 << 1))
#define GPIOC_PCLK_EN()				(RCC->AHB1ENR |= (1 << 2))
#define GPIOD_PCLK_EN()				(RCC->AHB1ENR |= (1 << 3))
#define GPIOE_PCLK_EN()				(RCC->AHB1ENR |= (1 << 4))
#define GPIOF_PCLK_EN()				(RCC->AHB1ENR |= (1 << 5))
#define GPIOG_PCLK_EN()				(RCC->AHB1ENR |= (1 << 6))
#define GPIOH_PCLK_EN()				(RCC->AHB1ENR |= (1 << 7))
#define GPIOI_PCLK_EN()				(RCC->AHB1ENR |= (1 << 8))

//Clock enable macros for I2Cx peripherals
#define I2C1_PCLK_EN()				(RCC->APB1ENR |= (1 << 21))
#define I2C2_PCLK_EN()				(RCC->APB1ENR |= (1 << 22))
#define I2C3_PCLK_EN()				(RCC->APB1ENR |= (1 << 23))

//Clock enable macros for SPIx peripherals
#define SPI1_PCLK_EN()				(RCC->APB2ENR |= (1 << 12))
#define SPI2_PCLK_EN()				(RCC->APB1ENR |= (1 << 14))
#define SPI3_PCLK_EN()				(RCC->APB1ENR |= (1 << 15))


//Clock enable macros for USARTx peripherals
#define USART1_PCLK_EN()			(RCC->APB2ENR |= (1 << 4))
#define USART2_PCLK_EN()			(RCC->APB1ENR |= (1 << 17))
#define USART3_PCLK_EN()			(RCC->APB1ENR |= (1 << 18))
#define UART4_PCLK_EN()				(RCC->APB1ENR |= (1 << 19))
#define UART5_PCLK_EN()				(RCC->APB1ENR |= (1 << 20))
#define USART6_PCLK_EN()			(RCC->APB2ENR |= (1 << 5))

//Clock enable macros for Syscfg peripherals
#define SYSCFG_PCLK_EN()			(RCC->APB2ENR |= (1 << 14))

//Clock disable macros for GPIOx peripherals
#define GPIOA_PCLK_DI()				(RCC->AHB1ENR &= ~(1 << 0))
#define GPIOB_PCLK_DI()				(RCC->AHB1ENR &= ~(1 << 1))
#define GPIOC_PCLK_DI()				(RCC->AHB1ENR &= ~(1 << 2))
#define GPIOD_PCLK_DI()				(RCC->AHB1ENR &= ~(1 << 3))
#define GPIOE_PCLK_DI()				(RCC->AHB1ENR &= ~(1 << 4))
#define GPIOF_PCLK_DI()				(RCC->AHB1ENR &= ~(1 << 5))
#define GPIOG_PCLK_DI()				(RCC->AHB1ENR &= ~(1 << 6))
#define GPIOH_PCLK_DI()				(RCC->AHB1ENR &= ~(1 << 7))
#define GPIOI_PCLK_DI()				(RCC->AHB1ENR &= ~(1 << 8))

//Clock disable macros for I2Cx peripherals
#define I2C1_PCLK_DI()				(RCC->APB1ENR &= ~(1 << 21))
#define I2C2_PCLK_DI()				(RCC->APB1ENR &= ~(1 << 22))
#define I2C3_PCLK_DI()				(RCC->APB1ENR &= ~(1 << 23))

//Clock disable macros for SPIx peripherals
#define SPI1_PCLK_DI()				(RCC->APB2ENR &= ~(1 << 12))
#define SPI2_PCLK_DI()				(RCC->APB1ENR &= ~(1 << 14))
#define SPI3_PCLK_DI()				(RCC->APB1ENR &= ~(1 << 15))

//Clock disable macros for USARTx peripherals
#define USART1_PCLK_DI()			(RCC->APB2ENR &= ~(1 << 4))
#define USART2_PCLK_DI()			(RCC->APB1ENR &= ~(1 << 17))
#define USART3_PCLK_DI()			(RCC->APB1ENR &= ~(1 << 18))
#define UART4_PCLK_DI()				(RCC->APB1ENR &= ~(1 << 19))
#define UART5_PCLK_DI()				(RCC->APB1ENR &= ~(1 << 20))
#define USART6_PCLK_DI()			(RCC->APB2ENR &= ~(1 << 5))

//Clock disable macros for Syscfg peripherals
#define SYSCFG_PCLK_DI()			(RCC->APB2ENR &= ~(1 << 14))

//Macros to reset GPIOx peripherals
#define GPIOA_REG_RESET()			do{(RCC->AHB1RSTR |= (1 << 0)); (RCC->AHB1RSTR &= ~(1 << 0));} while(0)
#define GPIOB_REG_RESET()			do{(RCC->AHB1RSTR |= (1 << 1)); (RCC->AHB1RSTR &= ~(1 << 1));} while(0)
#define GPIOC_REG_RESET()			do{(RCC->AHB1RSTR |= (1 << 2)); (RCC->AHB1RSTR &= ~(1 << 2));} while(0)
#define GPIOD_REG_RESET()			do{(RCC->AHB1RSTR |= (1 << 3)); (RCC->AHB1RSTR &= ~(1 << 3));} while(0)
#define GPIOE_REG_RESET()			do{(RCC->AHB1RSTR |= (1 << 4)); (RCC->AHB1RSTR &= ~(1 << 4));} while(0)
#define GPIOF_REG_RESET()			do{(RCC->AHB1RSTR |= (1 << 5)); (RCC->AHB1RSTR &= ~(1 << 5));} while(0)
#define GPIOG_REG_RESET()			do{(RCC->AHB1RSTR |= (1 << 6)); (RCC->AHB1RSTR &= ~(1 << 6));} while(0)
#define GPIOH_REG_RESET()			do{(RCC->AHB1RSTR |= (1 << 7)); (RCC->AHB1RSTR &= ~(1 << 7));} while(0)
#define GPIOI_REG_RESET()			do{(RCC->AHB1RSTR |= (1 << 8)); (RCC->AHB1RSTR &= ~(1 << 8));} while(0)

//Macros to reset SPIx peripherals
#define SPI1_REG_RESET()			do{(RCC->APB2RSTR |= (1 << 12)); (RCC->APB2RSTR &= ~(1 << 12));} while(0)
#define SPI2_REG_RESET()			do{(RCC->APB1RSTR |= (1 << 14)); (RCC->APB1RSTR &= ~(1 << 14));} while(0)
#define SPI3_REG_RESET()			do{(RCC->APB1RSTR |= (1 << 15)); (RCC->APB1RSTR &= ~(1 << 15));} while(0)

//Macros to reset I2Cx peripherals
#define I2C1_REG_RESET()			do{(RCC->APB1RSTR |= (1 << 21)); (RCC->APB1RSTR &= ~(1 << 21));} while(0)
#define I2C2_REG_RESET()			do{(RCC->APB1RSTR |= (1 << 22)); (RCC->APB1RSTR &= ~(1 << 22));} while(0)
#define I2C3_REG_RESET()			do{(RCC->APB1RSTR |= (1 << 23)); (RCC->APB1RSTR &= ~(1 << 23));} while(0)

//Macros to reset USARTx peripherals
#define USART1_REG_RESET()			do{(RCC->APB2RSTR |= (1 << 4)); (RCC->APB2RSTR &= ~(1 << 4));} while(0)
#define USART2_REG_RESET()			do{(RCC->APB1RSTR |= (1 << 17)); (RCC->APB1RSTR &= ~(1 << 17));} while(0)
#define USART3_REG_RESET()			do{(RCC->APB1RSTR |= (1 << 18)); (RCC->APB1RSTR &= ~(1 << 18));} while(0)
#define UART4_REG_RESET()			do{(RCC->APB1RSTR |= (1 << 18)); (RCC->APB1RSTR &= ~(1 << 19));} while(0)
#define UART5_REG_RESET()			do{(RCC->APB1RSTR |= (1 << 18)); (RCC->APB1RSTR &= ~(1 << 20));} while(0)
#define USART6_REG_RESET()			do{(RCC->APB2RSTR |= (1 << 5)); (RCC->APB2RSTR &= ~(1 << 5));} while(0)


//Returns port code for given GPIOx base address
#define GPIO_BASE_ADDR_TO_CODE(x)	(x == GPIOA) ? 0 :\
									(x == GPIOB) ? 1 :\
									(x == GPIOC) ? 2 :\
									(x == GPIOD) ? 3 :\
									(x == GPIOE) ? 4 :\
									(x == GPIOF) ? 5 :\
									(x == GPIOG) ? 6 :\
									(x == GPIOH) ? 7 :\
									(x == GPIOI) ? 8 : -1

//IRQ numbers for stm32f407x MCU
#define IRQ_NO_EXTI0				6
#define IRQ_NO_EXTI1				7
#define IRQ_NO_EXTI2				8
#define IRQ_NO_EXTI3				9
#define IRQ_NO_EXTI4				10
#define IRQ_NO_EXTI9_5				23
#define IRQ_NO_SPI1					35
#define IRQ_NO_SPI2					36
#define IRQ_NO_EXTI5_10				40
#define IRQ_NO_SPI3					51
#define IRQ_NO_I2C1_EV				31
#define IRQ_NO_I2C1_ER				32
#define IRQ_NO_I2C2_EV				33
#define IRQ_NO_I2C2_ER				34

//IRQ priority
#define NVIC_IRQ_PRIO0				0
#define NVIC_IRQ_PRIO1				1
#define NVIC_IRQ_PRIO2				2
#define NVIC_IRQ_PRIO3				3
#define NVIC_IRQ_PRIO4				4
#define NVIC_IRQ_PRIO5				5
#define NVIC_IRQ_PRIO6				6
#define NVIC_IRQ_PRIO7				7
#define NVIC_IRQ_PRIO8				8
#define NVIC_IRQ_PRIO9				9
#define NVIC_IRQ_PRIO10				10
#define NVIC_IRQ_PRIO11				11
#define NVIC_IRQ_PRIO12				12
#define NVIC_IRQ_PRIO13				13
#define NVIC_IRQ_PRIO14				14
#define NVIC_IRQ_PRIO15				15

//Some generic Macros
#define ENABLE						1
#define DISABLE						0
#define SET							ENABLE
#define RESET						DISABLE
#define GPIO_PIN_SET				SET
#define GPIO_PIN_RESET				RESET
#define FLAG_RESET					RESET
#define FLAG_SET					SET

//Bit position definitions of SPI peripheral
#define SPI_CR1_CPHA                0   // Clock Phase
#define SPI_CR1_CPOL                1   // Clock Polarity
#define SPI_CR1_MSTR                2   // Master Selection
#define SPI_CR1_BAUDRATE            3   // Baud Rate Control bit-field start
#define SPI_CR1_SPE                 6   // SPI Peripheral Enable
#define SPI_CR1_LSBFIRST            7   // Frame Format Selection
#define SPI_CR1_SSI                 8   // Internal Slave Select
#define SPI_CR1_SSM                 9   // Software Slave Management Enable
#define SPI_CR1_RXONLY              10  // Receive Only Enable
#define SPI_CR1_DFF                 11  // Data Frame Format Selection
#define SPI_CR1_CRCNEXT             12  // CRC Next Transfer Selection
#define SPI_CR1_CRCEN               13  // Hardware CRC Calculation Enable
#define SPI_CR1_BIDIOE              14  // Output Enable in Bidirectional Mode
#define SPI_CR1_BIDIMODE            15  // Bidirectional Data Mode Enable

#define SPI_CR2_RXDMAEN             0   // Receive DMA Enable
#define SPI_CR2_TXDMAEN             1   // Transmit DMA Enable
#define SPI_CR2_SSOE                2   // Slave Select Output Enable
#define SPI_CR2_FRF                 4   // Frame Format Protocol Standard
#define SPI_CR2_ERRIE               5   // Error Interrupt Enable
#define SPI_CR2_RXNEIE              6   // RX Buffer Not Empty Interrupt Enable
#define SPI_CR2_TXEIE               7   // TX Buffer Empty Interrupt Enable

#define SPI_SR_RXNE                 0   // Receive Buffer Not Empty Flag
#define SPI_SR_TXE                  1   // Transmit Buffer Empty Flag
#define SPI_SR_CHSIDE               2   // Channel Side Flag
#define SPI_SR_UDR                  3   // Underrun Error Flag
#define SPI_SR_CRCERR               4   // CRC Error Flag
#define SPI_SR_MODF                 5   // Mode Fault Flag
#define SPI_SR_OVR                  6   // Overrun Error Flag
#define SPI_SR_BSY                  7   // SPI Busy Flag
#define SPI_SR_FRE                  8   // Frame Format Error Flag

// Bit position definitions of I2C peripheral
// I2C_CR1 bits
#define I2C_CR1_PE                  0   // Peripheral enable
#define I2C_CR1_SMBUS               1   // SMBus mode
#define I2C_CR1_SMBTYPE             3   // SMBus type
#define I2C_CR1_ENARP               4   // ARP enable
#define I2C_CR1_ENPEC               5   // PEC enable
#define I2C_CR1_ENGC                6   // General call enable
#define I2C_CR1_NOSTRETCH           7   // Clock stretching disable
#define I2C_CR1_START               8   // Start generation
#define I2C_CR1_STOP                9   // Stop generation
#define I2C_CR1_ACK                 10  // Acknowledge enable
#define I2C_CR1_POS                 11  // Acknowledge/PEC position
#define I2C_CR1_PEC                 12  // Packet error checking
#define I2C_CR1_SWRST               15  // Software reset

// I2C_CR2 bits
#define I2C_CR2_FREQ                0   // Peripheral clock frequency bit-field start (Bits 0-5)
#define I2C_CR2_ITERREN             8   // Error interrupt enable
#define I2C_CR2_ITEVTEN             9   // Event interrupt enable
#define I2C_CR2_ITBUFEN             10  // Buffer interrupt enable
#define I2C_CR2_DMAEN               11  // DMA requests enable
#define I2C_CR2_LAST                12  // DMA last transfer

// I2C_SR1 bits
#define I2C_SR1_SB                  0   // Start bit (Master mode)
#define I2C_SR1_ADDR                1   // Address sent (Master) / Address matched (Slave)
#define I2C_SR1_BTF                 2   // Byte transfer finished
#define I2C_SR1_ADD10               3   // 10-bit header sent (Master mode)
#define I2C_SR1_STOPF               4   // Stop detection (Slave mode)
#define I2C_SR1_RXNE                6   // Data register not empty (receivers)
#define I2C_SR1_TXE                 7   // Data register empty (transmitters)
#define I2C_SR1_BERR                8   // Bus error
#define I2C_SR1_ARLO                9   // Arbitration lost (Master mode)
#define I2C_SR1_AF                  10  // Acknowledge failure
#define I2C_SR1_OVR                 11  // Overrun/Underrun
#define I2C_SR1_PECERR              12  // PEC error in reception
#define I2C_SR1_TIMEOUT             14  // Timeout or Tlow error
#define I2C_SR1_SMBALERT            15  // SMBus alert

// I2C_SR2 bits
#define I2C_SR2_MSL                 0   // Master/Slave mode selection
#define I2C_SR2_BUSY                1   // Bus busy flag
#define I2C_SR2_TRA                 2   // Transmitter/Receiver mode selection
#define I2C_SR2_GENCALL             4   // General call address received (Slave mode)
#define I2C_SR2_SMBDEFAULT          5   // SMBus device default address received
#define I2C_SR2_SMBHOST             6   // SMBus host header received
#define I2C_SR2_DUALF               7   // Dual flag (Slave mode)
#define I2C_SR2_PEC                 8   // Packet error checking register bit-field start (Bits 8-15)

// I2C_CCR bits
#define I2C_CCR_CCR                 0   // Clock control register bit-field start (Bits 0-11)
#define I2C_CCR_DUTY                14  // Fast mode duty cycle
#define I2C_CCR_FS                  15  // I2C master mode selection (Standard / Fast)

// Bit position definitions of USART peripheral
//USART_SR bits
#define USART_SR_PE					0   // Parity Error
#define USART_SR_FE					1   // Framing Error
#define USART_SR_NE					2   // Noise Error Flag
#define USART_SR_ORE				3   // Overrun Error
#define USART_SR_IDLE				4   // IDLE Line Detected
#define USART_SR_RXNE				5   // Read Data Register Not Empty
#define USART_SR_TC					6   // Transmission Complete
#define USART_SR_TXE				7   // Transmit Data Register Empty
#define USART_SR_LBD				8   // LIN Break Detection Flag
#define USART_SR_CTS				9   // CTS Change Flag

// 3. Baud Rate Register (USART_BRR) bits
#define USART_BRR_DIV_FRACTION		0   // Fraction of USARTDIV offset (Bits 0:3)
#define USART_BRR_DIV_MANTISSA		4   // Mantissa of USARTDIV offset (Bits 4:15)

// 4. Control Register 1 (USART_CR1) bits
#define USART_CR1_SBK				0   // Send Break
#define USART_CR1_RWU				1   // Receiver Wakeup
#define USART_CR1_RE				2   // Receiver Enable
#define USART_CR1_TE				3   // Transmitter Enable
#define USART_CR1_IDLEIE			4   // IDLE Interrupt Enable
#define USART_CR1_RXNEIE			5   // RXNE Interrupt Enable
#define USART_CR1_TCIE				6   // Transmission Complete Interrupt Enable
#define USART_CR1_TXEIE				7   // TXE Interrupt Enable
#define USART_CR1_PEIE				8   // PE Interrupt Enable
#define USART_CR1_PS				9   // Parity Selection
#define USART_CR1_PCE				10  // Parity Control Enable
#define USART_CR1_WAKE				11  // Wakeup Method
#define USART_CR1_M					12  // Word Length
#define USART_CR1_UE				13  // USART Enable
#define USART_CR1_OVER8				15  // Oversampling Mode

// 5. Control Register 2 (USART_CR2) bits
#define USART_CR2_ADD				0   // Address of the USART node offset (Bits 0:3)
#define USART_CR2_LBDL				5   // LIN Break Detection Length
#define USART_CR2_LBDIE				6   // LIN Break Detection Interrupt Enable
#define USART_CR2_LBCL				8   // Last Bit Clock Pulse
#define USART_CR2_CPHA				9   // Clock Phase
#define USART_CR2_CPOL				10  // Clock Polarity
#define USART_CR2_CLKEN				11  // Clock Enable
#define USART_CR2_STOP				12  // STOP bits offset (Bits 12:13)
#define USART_CR2_LINEN				14  // LIN Mode Enable

// 6. Control Register 3 (USART_CR3) bits
#define USART_CR3_EIE				0   // Error Interrupt Enable
#define USART_CR3_IREN				1   // IrDA Mode Enable
#define USART_CR3_IRLP				2   // IrDA Low-Power
#define USART_CR3_HDSEL				3   // Half-Duplex Selection
#define USART_CR3_NACK				4   // Smartcard NACK Enable
#define USART_CR3_SCEN				5   // Smartcard Mode Enable
#define USART_CR3_DMAR				6   // DMA Enable Receiver
#define USART_CR3_DMAT				7   // DMA Enable Transmitter
#define USART_CR3_RTSE				8   // Request to Send Enable
#define USART_CR3_CTSE				9   // Clear to Send Enable
#define USART_CR3_CTSIE				10  // CTS Interrupt Enable
#define USART_CR3_ONEBIT			11  // One-Bit Sampling Method

// 7. Guard Time and Prescaler Register (USART_GTPR) bits
#define USART_GTPR_PSC				0   // Prescaler Value offset (Bits 0:7)
#define USART_GTPR_GT				8   // Guard Time Value offset (Bits 8:15)


#include "stm32f407xx_gpio_driver.h"
#include "stm32f407xx_SPI_driver.h"
#include "stm32f407xx_i2c_driver.h"
#include "stm32f407xx_USART_driver.h"
#include "stm32f407xx_rcc_driver.h"

#endif /* INC_STM32F407XX_H_ */
