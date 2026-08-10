/*
 * RTC_LCD.c
 *
 *  Created on: 5 Aug 2026
 *      Author: joshb
 */

#include <stdio.h>
#include "ds1307.h"
#include "lcd.h"

#define SYSTICK_TIM_CLK 16000000UL

#define PRINT_LCD
//#define DEBUG_WITH_SWV


void init_systick_timer(uint32_t tick_hz)
{
	uint32_t *pSRVR = (uint32_t*)0xE000E014;
	uint32_t *pSCSR = (uint32_t*)0xE000E010;

    //calculation of reload value
    uint32_t count_value = (SYSTICK_TIM_CLK/tick_hz)-1;

    //Clear the value of SVR
    *pSRVR &= ~(0x00FFFFFFFF);

    //load the value in to SVR
    *pSRVR |= count_value;

    //do some settings
    *pSCSR |= ( 1 << 1); //Enables SysTick exception request:
    *pSCSR |= ( 1 << 2);  //Indicates the clock source, processor clock source

    //enable the systick
    *pSCSR |= ( 1 << 0); //enables the counter

}

#ifdef DEBUG_WITH_SWV
/*********************************Needed for using SWV printf**********************************/
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
#endif

char *get_day_of_week(uint8_t i);
char *time_to_string(RTC_time_t *rtc_time);
char *date_to_string(RTC_date_t *rtc_date);
void number_to_string(uint8_t num, char* buf);

int main(void)
{
	RTC_time_t current_time;
	RTC_date_t current_date;

#ifndef PRINT_LCD
	printf("RTC test\n");
#else
	lcd_init();

	lcd_print_string("RTC test...");

	mdelay(2000);

	lcd_display_clear();
	lcd_display_return_home();
	#endif
	if(ds1307_init())
	{
		printf("RTC init has failed\n");
		while(1);
	}

	init_systick_timer(1);

	current_date.day = DS1307_THURSDAY;
	current_date.date = 6;
	current_date.month = 8;
	current_date.year = 26;

	current_time.hours = 11;
	current_time.minutes = 59;
	current_time.seconds = 45;
	current_time.time_format = TIME_FORMAT_12HRS_PM;

	ds1307_set_current_date(&current_date);
	ds1307_set_current_time(&current_time);

	ds1307_get_current_time(&current_time);
	ds1307_get_current_date(&current_date);

	char *am_pm;
	if(current_time.time_format != TIME_FORMAT_24_HRS)
	{
		am_pm = (current_time.time_format) ? "PM" : "AM";
#ifndef PRINT_LCD
		printf("Current time = %s %s\n", time_to_string(&current_time), am_pm);
#else
		lcd_print_string(time_to_string(&current_time));
		lcd_print_string(am_pm);
#endif
	}
	else
	{
#ifndef PRINT_LCD
		printf("Current time = %s\n", time_to_string(&current_time));
#else
		lcd_print_string(time_to_string(&current_time));
#endif
	}
#ifndef PRINT_LCD
	printf("Current date = %s <%s>\n", date_to_string(&current_date), get_day_of_week(current_date.day));
#else
	lcd_set_cursor(2, 1);
	lcd_print_string(date_to_string(&current_date));
#endif
	while(1);
	return 0;
}

char *get_day_of_week(uint8_t i)
{
	const char* days[] = {
	    "Sun",
	    "Mon",
	    "Tue",
	    "Wed",
	    "Thur",
	    "Fri",
	    "Sat"
	};

	return days[i-1];
}

void number_to_string(uint8_t num, char* buf)
{
	if(num < 10)
	{
		buf[0] = '0';
		buf[1] = num  + 48; //adding 48 to number gives ascii code
	}
	else if(num >= 10 && num <= 99)
	{
		buf[0] = (num / 10) + 48;
		buf[1] = (num % 10) + 48;
	}
}

//hh:mm:ss
char *time_to_string(RTC_time_t *rtc_time)
{
	static char buf[9];

	buf[2] = ':';
	buf[5] = ':';

	number_to_string(rtc_time->hours, buf);
	number_to_string(rtc_time->minutes, &buf[3]);
	number_to_string(rtc_time->seconds, &buf[6]);

	buf[8] = '\0';

	return buf;
}

//dd/mm/yy
char *date_to_string(RTC_date_t *rtc_date)
{
	static char buf[9];

	buf[2] = '/';
	buf[5] = '/';

	number_to_string(rtc_date->date, buf);
	number_to_string(rtc_date->month, &buf[3]);
	number_to_string(rtc_date->year, &buf[6]);

	buf[8] = '\0';

	return buf;
}

void SysTick_Handler(void)
{
	RTC_time_t current_time;
	RTC_date_t current_date;

	ds1307_get_current_time(&current_time);
	ds1307_get_current_date(&current_date);

	lcd_set_cursor(1, 1);

	char *am_pm;
	if(current_time.time_format != TIME_FORMAT_24_HRS)
	{
		am_pm = (current_time.time_format) ? "PM" : "AM";
		//printf("Current time = %s %s\n", time_to_string(&current_time), am_pm);
		lcd_print_string(time_to_string(&current_time));
		lcd_print_string(am_pm);
	}
	else
	{
		//printf("Current time = %s\n", time_to_string(&current_time));
		lcd_print_string(time_to_string(&current_time));
	}

	//printf("Current date = %s <%s>\n", date_to_string(&current_date), get_day_of_week(current_date.day));
	lcd_set_cursor(2, 1);
	lcd_print_string(date_to_string(&current_date));
	lcd_send_char('<');
	lcd_print_string(get_day_of_week(current_date.day));
	lcd_send_char('>');
}
