/*
 * ds1307.h
 *
 *  Created on: 5 Aug 2026
 *      Author: joshb
 */

#ifndef DS1307_H_
#define DS1307_H_

#include "stm32f407xx.h"

//Application configurable items
#define DS1307_I2C				I2C1
#define DS1307_I2C_GPIO_PORT	GPIOB
#define DS1307_I2C_SDA_PIN		GPIO_PIN_NO_7
#define DS1307_I2C_SCL_PIN		GPIO_PIN_NO_6
#define DS1307_I2C_SPEED		I2C_SCL_SPEED_SM
#define DS1307_I2C_PUPD			GPIO_PIN_PU

//Register addresses
#define DS_1307_ADDR_SEC	    0x00
#define DS_1307_ADDR_MIN	    0x01
#define DS_1307_ADDR_HRS	    0x02
#define DS_1307_ADDR_DAY	    0x03
#define DS_1307_ADDR_DATE	    0x04
#define DS_1307_ADDR_MONTH	    0x05
#define DS_1307_ADDR_YEAR	    0x06

#define TIME_FORMAT_12HRS_AM 	0
#define TIME_FORMAT_12HRS_PM 	1
#define TIME_FORMAT_24_HRS	 	2

#define DS1307_I2C_ADDRESS     	0x68

#define DS1307_SUNDAY			1
#define DS1307_MONDAY			2
#define DS1307_TUESDAY			3
#define DS1307_WEDNESDAY		4
#define DS1307_THURSDAY			5
#define DS1307_FRIDAY			6
#define DS1307_SATURDAY			7

//Struct to handle date information
typedef struct
{
	uint8_t date;
	uint8_t month;
	uint8_t year;
	uint8_t day;
}RTC_date_t;

//Struct to handle time information
typedef struct
{
	uint8_t seconds;
	uint8_t minutes;
	uint8_t hours;
	uint8_t time_format;
}RTC_time_t;

/***************************************************************************
 * @brief	Initialises I2C comms between MCU and RTC before setting halt
 * 			bit to 0 ensuring RTC is running.
 * @retval  Returns 0 if initialisation and communication succeeded, or
 *          a non-zero status if communication failed.
 * @note 	I2C SDA and SCL pins can be chosen by modifying the application
 * 			configurable macros in this header file.
 */
uint8_t ds1307_init(void);

/***************************************************************************
 * @brief	Configures RTC registers matching the time given by user.
 * @param 	*rtc_time: Pointer to struct containing time information.
 */
void ds1307_set_current_time(RTC_time_t *rtc_time);

/***************************************************************************
 * @brief	Retrieves time data from RTC and stores this data inside
 * 			RTC_time_t struct provided.
 * @param 	*rtc_time: Pointer to struct that will store the time data.
 */
void ds1307_get_current_time(RTC_time_t *rtc_time);

/***************************************************************************
 * @brief	Configures RTC registers matching the date given by user.
 * @param 	*rtc_date: Pointer to struct containing date information.
 */
void ds1307_set_current_date(RTC_date_t *rtc_date);

/***************************************************************************
 * @brief	Retrieves date information from RTC and stores this data inside
 * 			RTC_date_t struct provided.
 * @param 	*rtc_date: Pointer to struct that will store the date data.
 */
void ds1307_get_current_date(RTC_date_t *rtc_date);

#endif /* DS1307_H_ */
