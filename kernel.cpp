#include "types.h"
#include "gdt.h"
#include "interrupts.h"

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

// Need to understand this Lecture 1B
typedef void (*constructor)();
extern "C" constructor start_ctors;
extern "C" constructor end_ctors;
extern "C" void callConstructors()
{
    for(constructor* i = &start_ctors; i != &end_ctors; i++)
        (*i)();
}

extern "C" void kernelMain(void* multibootStruct, uint32_t magicNumber) {
	printf("Hello World! This is Aman, your creator.\n"); 

	/*
	*	Instantiate the Global Descriptor table
	*	Instantiate the Interrupt Descriptor Table
	*	Activate the Hardware
	*	Activate the interrupts - that is PIC will from now on notify the CPU in case of interrupts
	*/
	GlobalDescriptorTable gdt;
	InterruptManager interrupts(&gdt);

	interrupts.activate();

	/*
	* We dont want the kernel to ever stop.
	* So, let's dive into an infinite loop.
	*/
	while(1);
}