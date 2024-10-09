/*
    Author: Aman Pandey
    E-mail: amanpandey1235@gmail.com
*/

#include <gdt.h>
#include <common/stdio.h>
#include <drivers/driver.h>
#include <drivers/keyboardDriver.h>
#include <drivers/mouseDriver.h>
#include <drivers/vgaDriver.h>
#include <hwcomms/interrupts.h>
#include <hwcomms/pci.h>

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
	clearScreen();

	printf("Initializing Operating System...\n"); 

	/*
	*	Instantiate the Global Descriptor table
	*	Instantiate the Interrupt Descriptor Table
	*	Activate the Hardware
	*	Activate the interrupts - that is PIC will from now on notify the CPU in case of interrupts
	*/
	GlobalDescriptorTable gdt;
	InterruptManager interruptManager(&gdt);
	DriverManager driverManager;
		printf("Initializing Keyboard...\n");
		PrintKeyToScreenEvent eventHandler;
		KeyboardDriver keyboard(&interruptManager, &eventHandler);
		driverManager.addDriver(&keyboard);

		printf("Initializing Mouse...\n");
		MouseDriver mouse(&interruptManager);	//TODO: Make event handler for the mouse later.
		driverManager.addDriver(&mouse);

		printf("Initializing PCI...\n");
		PeripheralComponentInterconnectController pci;
		pci.enumerateDevices(&driverManager, &interruptManager);

		VideoGraphicsArrayDriver vga;
		
	printf("Activating Interrupts...\n");
	interruptManager.activate();

	vga.setMode(320, 200, 8);
	for(uint16_t y = 0; y < 200; y++) {
		for(uint16_t x = 0; x < 320; x++) {
			vga.putPixel(x, y, 0x00, 0x00, 0xA8);
		}
	}
	/*
	* We dont want the kernel to ever stop.
	* So, let's dive into an infinite loop.
	*/
	while(1);
}