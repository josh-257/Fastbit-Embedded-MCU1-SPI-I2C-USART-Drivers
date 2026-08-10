/*
 * lcd.h
 *
 *  Created on: 5 Aug 2026
 *      Author: joshb
 */

#ifndef LCD_H_
#define LCD_H_

#include "stm32f407xx.h"

//Application configurable items
#define LCD_GPIO_PORT 	GPIOD
#define LCD_GPIO_RS	  	GPIO_PIN_NO_0
#define LCD_GPIO_RW	  	GPIO_PIN_NO_1
#define LCD_GPIO_EN	  	GPIO_PIN_NO_2
#define LCD_GPIO_D4	  	GPIO_PIN_NO_3
#define LCD_GPIO_D5	  	GPIO_PIN_NO_4
#define LCD_GPIO_D6	  	GPIO_PIN_NO_5
#define LCD_GPIO_D7	  	GPIO_PIN_NO_6

//LCD commands
#define LCD_CMD_4DL_2N_5X8F 		0x28
#define LCD_CMD_DIS_ON_CUR_ON 		0x0E
#define LCD_CMD_INC_ADDR			0x06
#define LCD_CMD_DIS_CLEAR			0x01
#define LCD_CMD_DIS_RETURN_HOME		0x02

/***************************************************************************
 * @brief	Configures all GPIO pins connected to LCD, sets up LCD
 * 			display for 4bit data mode and clears display ready for input.
 */
void lcd_init(void);

/***************************************************************************
 * @brief	Takes an 8 bit command message and sends it to the LCD over two
 * 			4 bit transmissions (MSB first).
 * @param 	cmd: Command code macro to be sent.
 */
void lcd_send_command(uint8_t cmd);

/***************************************************************************
 * @brief	Sends 1 byte value to the LCD over two 4 bit transmissions
 * 			(MSB first).
 * @param 	data: Byte representing an ascii character value.
 */
void lcd_send_char(uint8_t data);

/***************************************************************************
 * @brief	Sends command to clear LCD display before waiting 2ms for the
 * 			LCD to clear.
 */
void lcd_display_clear(void);

/***************************************************************************
 * @brief	Sends a string of characters to the LCD (for bits at a time).
 * @param 	message:  Pointer to character array to be sent to LCD.
 */
void lcd_print_string(char *message);

/***************************************************************************
 * @brief	Sends command to make the LCD return to the start of displayed
 * 			message.
 */
void lcd_display_return_home(void);

void lcd_set_cursor(uint8_t row, uint8_t column);
void mdelay(uint32_t cnt);

#endif /* LCD_H_ */
