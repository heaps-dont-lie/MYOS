/*
    Author: Aman Pandey
    E-mail: amanpandey1235@gmail.com
*/

#include "stdio.h"

void printf(char* str) {
	static uint16_t* videoMemBaseAddr = (uint16_t*)0xb8000;
	static uint8_t x = 0, y = 0;

	/*
	* Screen is 80 characters wide and 25 characters long
	*/
	for(int i = 0; str[i] != '\0'; ++i) {

		switch(str[i]) {
			case '\n':
				y++;
				x = 0;
				break;

			default:
				videoMemBaseAddr[80*y + x] = (videoMemBaseAddr[80*y + x] & 0xFF00) | str[i];
				x++;
				break;
		}

		if (x >= 80) {
			y++;
			x = 0;
		}

		if (y >= 25) {
			/*
			* Clear the screen first and
			  then begin from x=0 and y=0
			*/
			for(int y = 0; y <= 25; y++) {
				for(int x = 0; x <= 80; x++) {
					videoMemBaseAddr[80*y + x] = (videoMemBaseAddr[80*y + x] & 0xFF00) | ' ';
				}
			}
			x = 0;
			y = 0;
		}
	}
}

void clearScreen() {
	uint16_t* videoMemBaseAddr = (uint16_t*)0xb8000;

	for (int i = 0; i < 25; i++) {
		for (int j = 0; j < 80; j++) {
			videoMemBaseAddr[80*i + j] = (videoMemBaseAddr[80*i + j] & 0xFF00) | ' ';
		}
	}
}