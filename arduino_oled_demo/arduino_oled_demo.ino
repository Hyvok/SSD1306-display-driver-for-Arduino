#include "lib_ssd1306.h"
#include <SPI.h>

void setup() {

	SPI.begin();
	Serial.begin(9600);

	SSD1306_Display display;

	display.display_buffer();


	for(uint8_t i = 0x20; i < 0x7E; ++i) {

		display.printChar(i);

	}

	display.display_buffer();
	delay(10000);

	int x = 0;
	int y = 0;

	while(1) {
		while(1) {
			display.set_pixel(x, y);
			display.display_buffer();
			++x;
			if(x == 128) { x = 0; break; }
		}
		++y;
		if(y == 32) { break; }
	}

	display.clear_display();
	display.invert_display(1);
	display.display_buffer();

	x = 0;
	y = 0;

	while(1) {
		while(1) {
			display.set_pixel(x, y);
			display.display_buffer();
			++x;
			if(x == 128) { x = 0; break; }
		}
		++y;
		if(y == 32) { break; }
	}
}

void loop() {

  
}
