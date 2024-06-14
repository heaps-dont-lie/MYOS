#include "types.h"
#include "gdt.h"

void printf(char* str) {
	static uint16_t* videoMemBaseAddr = (uint16_t*)0xb8000;

	for(int i = 0; str[i] != '\0'; ++i) {
		videoMemBaseAddr[i] = (videoMemBaseAddr[i] & 0xFF00) | str[i];
	}
}

extern "C" void kernelMain(void* multibootStruct, uint32_t magicNumber) {
	printf("Hello World! This is Aman, your creator.");

	/*
	*	Instantiate the Global Descriptor table
	*/
	GlobalDescriptorTable gdt;

	/*
	* We dont want the kernel to ever stop.
	* So, let's dive into an infinite loop.
	*/
	while(1);
}