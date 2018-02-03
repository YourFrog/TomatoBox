
#define F_CPU 8000000UL
#define SCL_CLOCK 100000L

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

#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "lcdpcf8574/lcdpcf8574.h"
#include "pcf8574/pcf8574.h"
#include <avr/interrupt.h>

// Menu nie może zostac zaznaczone
#define MENU_NOT_SELECTABLE 0

// Menu moze zostac zaznaczone
#define MENU_SELECTABLE 1

typedef struct
{
	char *name;
	uint8_t selectable;
	struct Node *parent;
	struct Node *children[4];
	void (*callback)();
} Node;

Node *menuRoot;
Node *actualMenu;

// Pozycja kursora na wyświetlaczu
int cursor = 0;

/**
 * 	Definicja callback przy kliknięciu w menu
 */
void (*callback)();

volatile unsigned long delayCounter; // Pomocnicza zmienna do liczenia 1 sekundy z 8ms bo 8ms * 125 = 1 sec.
volatile unsigned long time; // Wlasciwy czas w sekundach

/**
 * 	Wejście w głąb menu
 */
void menuCallback_goToChildren()
{
	lcd_clrscr();

	actualMenu = actualMenu->children[cursor];
	resetCursor(actualMenu);
}

/**
 * 	Wyjście w górę menu
 */
void menuCallback_goToParent()
{
	lcd_clrscr();

	actualMenu = actualMenu->parent;
	resetCursor(actualMenu);
}

void menuCallback_undefinedOperation()
{
	lcd_clrscr();

	lcd_gotoxy(0, 0);
	lcd_puts("W krotce");

	_delay_ms(1000);
	lcd_clrscr();
}

/**
 *  Dodanie nowego elementu do menu
 */
Node* appendMenuItem(Node *root, int index, char *name, void (*callback)())
{
	Node *children = malloc(sizeof(Node));

	root->children[index] = children;

	children->selectable = MENU_SELECTABLE;
	children->name = name;
	children->parent = root;
	children->callback = callback;

	return children;
}

/**
 * 	Zresetowanie pozycji kursora do pierwszego elementu
 */
void resetCursor(Node *root)
{
	for(int index = 0; index < 4; index++) {

		Node *children = root->children[index];

		if( children != NULL && children->selectable == MENU_SELECTABLE ) {
			cursor = index;
			return;
		}
	}

	// Jeżeli nic nie odnalezliźmy to ustaw pierwszy wiersz
	for(int index = 0; index < 4; index++) {

		Node *children = root->children[index];

		if( children != NULL && children->selectable == MENU_SELECTABLE ) {
			cursor = index;
			return;
		}
	}
}

/**
 * 	Przesunięcie kursora na następną pozycje w menu
 */
void nextCursor(Node *root)
{
	for(int index = cursor + 1; index < 4; index++) {

		Node *children = root->children[index];

		if( children != NULL && children->selectable == MENU_SELECTABLE ) {
			// Możemy ustawic cursor na pozycje wiec zatrzymujemy pętle
			cursor = index;
			return;
		}
	}

	// Jeżeli nic nie odnalezliźmy to ustaw pierwszy wiersz
	for(int index = 0; index < 4; index++) {

		Node *children = root->children[index];

		if( children != NULL && children->selectable == MENU_SELECTABLE ) {
			cursor = index;
			return;
		}
	}
}

/**
 * 	Przesunięcie kursora na poprzednią pozycję
 */
void prevCursor(Node *root)
{
	for(int index = cursor - 1; index >= 0; index--) {

		Node *children = root->children[index];

		if( children != NULL && children->selectable == MENU_SELECTABLE ) {
			// Możemy ustawic cursor na pozycje wiec zatrzymujemy pętle
			cursor = index;
			return;
		}
	}

	// Jeżeli nic nie odnalezliźmy to ustaw ostatni wiersz
	for(int index = 3; index >= 0; index--) {

		Node *children = root->children[index];

		if( children != NULL && children->selectable == MENU_SELECTABLE ) {
			cursor = index;
			return;
		}
	}
}

void drawCursor()
{
	lcd_gotoxy(0, cursor);
	lcd_puts(">");
}

void clearCursor()
{
	lcd_gotoxy(0, cursor);
	lcd_puts(" ");
}

char* toString(int i)
{
	char *result = "";

	sprintf(result, "%d", i);

	return result;
}

/**
 * 	Zainicjowanie przycisku
 */
uint8_t button_init(uint8_t portType, uint8_t pin)
{
	portType |= (1 << pin);

	return portType;
}

/**
 * 	Sprawdzenie czy przycisk jest wciśnięty
 */
uint8_t button_isPress(uint8_t pinType, uint8_t pinButton)
{
	return !(pinType & (1 << pinButton));
}

/**
 * 	Zainicjowanie potrzebnych danych
 */
void initialize()
{
	///////////////////////////////
	//
	// 	Inicjalizacja MENU
	//
	///////////////////////////////
	menuRoot = malloc(sizeof(Node));
	actualMenu = menuRoot;

	menuRoot->name = "Root";

	Node *temperature = appendMenuItem(menuRoot, 0, "Temperatura", &menuCallback_undefinedOperation);
	Node *groundMenu = appendMenuItem(menuRoot, 1, "Gleba", &menuCallback_goToChildren);
	appendMenuItem(menuRoot, 2, "Cisnienie", &menuCallback_undefinedOperation);

	Node *sensor_1 = appendMenuItem(groundMenu, 0, "Czujnik (0): ", &menuCallback_undefinedOperation);
	Node *sensor_2 = appendMenuItem(groundMenu, 1, "Czujnik (1): ", &menuCallback_undefinedOperation);
	Node *returnMenu = appendMenuItem(groundMenu, 3, "Wstecz", &menuCallback_goToParent);

	sensor_1->selectable = MENU_NOT_SELECTABLE;
	sensor_2->selectable = MENU_NOT_SELECTABLE;

	///////////////////////////////
	//
	// 	Inicjalizacja LCD
	//
	///////////////////////////////
	lcd_init(LCD_DISP_ON);
	lcd_home();
	lcd_led(0);
	lcd_clrscr();

	///////////////////////////////
	//
	// 	Inicjalizacja portów
	//
	///////////////////////////////
	DDRB = 0xFF;
	DDRD = 0xFB;
	DDRB = 0xFB;

	PORTD = button_init(PORTD, PORTD7);
	PORTD = button_init(PORTD, PORTD6);

	PORTB = button_init(PORTB, PORTB0);

	///////////////////////////////
	//
	// 	Inicjalizacja Timera https://elektronika327.blogspot.com/2016/09/3-atmega328-timer-2timer-1.html
	//
	///////////////////////////////
	delayCounter = 0;
	time = 0;
    // set up timer with prescaler = 256
	TIMSK1 |= (1<<OCIE1A); // Enable Interrupt Timer/Counter1, Output Compare A (TIMER1_COMPA_vect)
//	TCCR1B |= ((1<<CS12) | (1<<CS10) | (1<<WGM12));    // Clock/1024, 0.001024 seconds per tick, Mode=CTC
//	OCR1A = 0x1E84; //8000; //15625-1;                       // 15625 means 1 sec and 62500 means 4 sec, We will make use of 1 sec


	TCCR1B |= ((1<<CS12) | (1<<WGM12));    // Clock/256, 0.001024 seconds per tick, Mode=CTC
	OCR1A = 250 - 1; //8000; //15625-1;                       // 15625 means 1 sec and 62500 means 4 sec, We will make use of 1 sec

	sei();
}

/**
 *	Obsuluga przerwania co 1 sekunde
 */
ISR (TIMER1_COMPA_vect)
{
	sei();
	delayCounter++;

	if( delayCounter >= 125 ) {
		delayCounter = 0;

		time++;

		if( time >= 86400 ) {
			time = 0;
		}
	}
}


char* padLeft(char *str, int length, char *ch)
{
	int len = strlen(str);

	if( length <= len ) {
		return str;
	}

	char *res = (char *) malloc(1 + length);


	for(int i = 0; i < length - len; i++) {
		strcat(res, ch);
	}

	strcat(res, str);

	return res;
}

void showTimer()
{
	int seconds = time % 60;
	int minutes = (time - seconds) / 60 % 60;
	int hours = ((time - seconds) / 60) / 60;

	char str[20];

	sprintf(str, "Godzina: %02d:%02d:%02d", hours, minutes, seconds);

	lcd_gotoxy(0, 3);
	lcd_puts(str);
}

/**
 * 	Pojedyńczy obieg pętli
 */
void loop()
{
	showTimer();
	drawCursor();

	for(int index = 0; index < 4; index++) {
		Node *children = actualMenu->children[index];

		if( children == NULL ) {
			continue;
		}

		lcd_gotoxy(2, index);
		lcd_puts(children->name);
	}


	if( button_isPress(PIND, PD7) ) {
		_delay_ms(50);

		clearCursor();
		nextCursor(actualMenu);
	}

	if( button_isPress(PIND, PD6) ) {
		_delay_ms(50);

		clearCursor();
		prevCursor(actualMenu);
	}

	if( button_isPress(PINB, PB0) ) {
		_delay_ms(50);

		Node *node = actualMenu->children[cursor];

		if( node->callback != NULL ) {
			node->callback();
		}
	}
}

/**
 * 	Podstawowa funkcja programu
 */
int main(void)
{
	initialize();

	while(1) {
		loop();
	}

    return 0;
}
