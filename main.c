
#define F_CPU 8000000UL
#define SCL_CLOCK 100000L

#define LCD_START_LINE3  0x10
#define LCD_START_LINE4  0x50

#define LCD_DATA0_PIN   4
#define LCD_DATA1_PIN   5
#define LCD_DATA2_PIN   6
#define LCD_DATA3_PIN   7
#define LCD_RS_PIN      0
#define LCD_RW_PIN      1
#define LCD_E_PIN       2
#define LCD_LED_PIN     3

#define PCF8574_MAXDEVICES 1

#define PCF8574_I2CFLEURYPATH "../i2c/i2cmaster.h"

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "lcdpcf8574/lcdpcf8574.h"
#include "pcf8574/pcf8574.h"



int main(void)
{
	lcd_init(LCD_DISP_ON_BLINK);
	lcd_home();

    /* LCD test */
	lcd_led(0);
	lcd_gotoxy(0, 0);
	lcd_puts("Linia 1");

	lcd_gotoxy(0, 1);
	lcd_puts("Linia 2");

	lcd_gotoxy(0, 2);
	lcd_puts("Linia 3");

	lcd_gotoxy(0, 3);
	lcd_puts("Linia 4");

    for (;;) {

    }

    return 0;
}
