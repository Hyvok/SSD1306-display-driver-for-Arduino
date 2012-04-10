#include "lib_ssd1306.h"
#include "Arduino.h"
#include <SPI.h>
#include "LCD_font_5x7.h"

static uint8_t buffer[512] = {0x00};
extern uint8_t PROGMEM font_5x7[];

SSD1306_Display::SSD1306_Display() : width_(WIDTH), height_(HEIGHT), 
										cursor_x_(0), cursor_y_(0) {

	pinMode(RESET, OUTPUT);
	pinMode(DATA_COMMAND, OUTPUT);

	SPI.setDataMode(SPI_MODE3);
	SPI.setBitOrder(MSBFIRST);
	SPI.setClockDivider(SPI_CLOCK_DIV16);

	/* reset the display */
	digitalWrite(RESET, HIGH);
	delay(1);
	digitalWrite(RESET, LOW);
	delay(1);
	digitalWrite(RESET, HIGH);

	/* Start initializing the display */
	write_command(DISPLAY_OFF);
	write_command(SET_DISPLAY_CLOCK_DIV);
	write_command(0x80);
	write_command(SET_MULTIPLEX);
	write_command(0x1F);
	write_command(SET_DISPLAY_OFFSET);
	write_command(0x0);
	write_command(SET_START_LINE);
	write_command(CHARGE_PUMP);
	/* We are using the internal DC/DC converter */
	write_command(0x14);
	write_command(MEMORY_MODE);
	write_command(0x00);
	write_command(SEGMENT_RE_MAP);
	write_command(COM_SCAN_DIRECTION);
	write_command(SET_COM_PINS);
	write_command(0x02);
	write_command(SET_CONTRAST);
	write_command(0x8F);
	write_command(SET_PRECHARGE);
	/* We are using the internal DC/DC converter */
	write_command(0xF1);
	write_command(SET_VCOM_DETECT);
	write_command(0x40);
	write_command(DISPLAY_ON_RESUME);
	write_command(SET_NORMAL_DISPLAY);

	/* We are done configuring the display, we can turn the display on */
	write_command(DISPLAY_ON);

	/* Construct the font handler */
	//FontHandler(get_width(), get_height());

}

void SSD1306_Display::clear_display() {

	memset(buffer, 0, 512);

}
void SSD1306_Display::set_pixel(char x, char y) {

	if(x > 128 || y > 32) { return; }

	uint16_t byte = 0;
	if(y/8 == 0) { byte = x; }
	else { byte = x+128*(y/8); }

	buffer[byte] |= (0x01<<(y-8*(y/8)));

}
void SSD1306_Display::display_buffer() {

	/* Go to the start of the screen */
	write_command(SET_LOW_COLUMN);
	write_command(SET_HIGH_COLUMN);
	write_command(SET_START_LINE);

	for (uint16_t i = 0; i < 512; ++i) {
		write_data(buffer[i]);
	}
	/* Fill the rest of the controller buffer with 0 since screen is 
	only 32 rows high */
	for (uint16_t i = 0; i < 512; ++i) {
		write_data(0);
	}
}
uint16_t SSD1306_Display::get_width() {

	return WIDTH;

}
uint16_t SSD1306_Display::get_height() {

	return HEIGHT;

}
void SSD1306_Display::invert_display(int invert) {

	if(invert) {
		write_command(SET_INVERTED_DISPLAY);
	}
	else {
		write_command(SET_NORMAL_DISPLAY);
	}
}
void SSD1306_Display::printChar(char ch) {

	uint16_t byte = 0;
	if(cursor_y_/8 == 0) { byte = cursor_x_; }
	else { byte = cursor_x_+128*(cursor_y_/8); }

	for(uint8_t i = 0; i < 5; ++i) {
		/* For reading a byte from the font table we need to read it from flash */
		buffer[byte+i] = pgm_read_byte(&font_5x7[(ch-0x20)*5+i]);
	}

	cursor_x_ += 6;
	if(cursor_x_ > width_-5 && cursor_y_ >= height_-8) {
		cursor_x_ = 128;
		cursor_y_ = height_-8;
	}
		else {
		if(cursor_x_ > width_-5) { 
			cursor_x_ = 0; 
			cursor_y_ += 8;
		}
		if(cursor_y_ > height_-8) { 
			cursor_y_ = height_ - 8; 
		}
	}
}
void SSD1306_Display::setCursor(uint16_t x, uint16_t y) {

	if(x > width_) { cursor_x_ = width_; }
	else { 	cursor_x_ = x; }
	if( y > height_) { cursor_y_ = height_; }
	else { cursor_y_ = y; }

}
inline void SSD1306_Display::write(char data) {

    PORTB &= ~_BV(2);
	SPI.transfer(data);
    PORTB |= _BV(2);

}
inline void SSD1306_Display::write_command(char data) {

	digitalWrite(DATA_COMMAND, LOW);
	write(data);

}
inline void SSD1306_Display::write_data(char data) {

	digitalWrite(DATA_COMMAND, HIGH);
	write(data);

}
