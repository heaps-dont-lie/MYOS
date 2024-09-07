/*
    Author: Aman Pandey
    E-mail: amanpandey1235@gmail.com
*/

#include "stdio.h"
#include "gdt.h"
#include "interrupts.h"
#include "keyboardDriver.h"

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

	KeyboardDriver keyboard(&interruptManager);
	interruptManager.activate();

	/*
	* We dont want the kernel to ever stop.
	* So, let's dive into an infinite loop.
	*/
	while(1);
}