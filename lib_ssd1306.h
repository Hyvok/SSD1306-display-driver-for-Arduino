#ifndef lib_ssd1306_h
#define lib_ssd1306_h

/* Display size in pixels */
#define WIDTH 128
#define HEIGHT 32

/* MCU connections, SPI in regular SPI pins */
#define DATA_COMMAND 9
#define RESET 8

/* Commands for the display */
#define DISPLAY_OFF 0xAE
#define SET_DISPLAY_CLOCK_DIV 0xD5
#define SET_MULTIPLEX 0xA8
#define SET_DISPLAY_OFFSET 0xD3
#define SET_START_LINE 0x40
#define CHARGE_PUMP 0x8D
#define MEMORY_MODE 0x20
#define SEGMENT_RE_MAP 0xA1
#define COM_SCAN_DIRECTION 0xC8
#define SET_COM_PINS 0xDA
#define SET_CONTRAST 0x81
#define SET_PRECHARGE 0xD9
#define SET_VCOM_DETECT 0xDB
#define DISPLAY_ON_RESUME 0xA4
#define SET_NORMAL_DISPLAY 0xA6
#define SET_INVERTED_DISPLAY 0xA7
#define DISPLAY_ON 0xAF
#define SET_LOW_COLUMN 0x00
#define SET_HIGH_COLUMN 0x10

#include "Arduino.h"

//#include "lib_font_handler.h"

/* Driver inherits functions from Font_Handler to be able to use fonts */
class SSD1306_Display {
	public:
		/* Constructor, this also initializes the screen */
		SSD1306_Display();

		/* Clear the display buffer */
		void clear_display();

		/* Send buffer contents to the display */
		void display_buffer();

		/* Set one pixel on */
		void set_pixel(char x, char y);

		/* Invert the display, 0 for normal  */
		void invert_display(int invert);

		/* Return display width */
		uint16_t get_width();

		/* Return display height */
		uint16_t get_height();

		/* Print a char */
		void printChar(char ch);

		/* Set cursor position in pixels */
		void setCursor(uint16_t x, uint16_t y);

	private:
		/* Size of the display, never changes */
		uint16_t width_, height_;
		uint16_t cursor_x_, cursor_y_;

		/* Write a byte to the display */
		void write(char data);

		/* Send a command to the display */
		void write_command(char data);

		/* Send data to the display */
		void write_data(char data);

};
#endif
