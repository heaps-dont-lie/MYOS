/*
    Author: Aman Pandey
    E-mail: amanpandey1235@gmail.com
*/

#include <drivers/keyboardDriver.h>
#include <drivers/keyboardMap.h>

KeyboardDriver::KeyboardDriver(InterruptManager* interruptManager, KeyBoardEventHandler* eventHandler) 
: HardwareInterruptHandler(KEYB_INT, interruptManager), commandPort(0x64), dataPort(0x60) {

    // Default value of modifier key is normal
    modKeys = MOD_NORMAL;

    // Initialise the event handler for the keboard
    this->eventHandler = eventHandler;

    // Initialise the keyboard hardware.
    initHardware();
}

KeyboardDriver::~KeyboardDriver() {}

void KeyboardDriver::initHardware() {
    // Disable the keyboard interrupts
    commandPort.Write(0xAD);

    //Flush the output buffer
    while (commandPort.Read() & 0x1)
        dataPort.Read();

    //Enable keyboard interrupts
    commandPort.Write(0xAE);

    // command 0x20 = read controller command byte
    commandPort.Write(0x20);
    uint8_t status = (dataPort.Read() | 1) & ~0x10;
    // command 0x60 = set controller command byte
    commandPort.Write(0x60);
    dataPort.Write(status);

    //Enable Scanning
    dataPort.Write(0xF4);

    if (dataPort.Read() != 0xFA) {
        printf("Keyboard initialization failed.\n");
        return;  // Handle failure case
    }
}

uint32_t KeyboardDriver::handleInterrupt(uint32_t esp) {
    uint8_t scanCode = dataPort.Read();
    unsigned char key = 0;

    if (eventHandler == NULL)
        return esp;

    /*
	 * Check for modifier keycodes. If present, toggle their state (if necessary).
	 */
	switch (scanCode) {
		case 0x2a: /* shift down */
		case 0x36: /* right shift down */
			modKeys |= MOD_SHIFT;
			return esp;
			break;
		case 0xaa: /* shift up */
		case 0xb6: /* right shift up */
			modKeys &= ~MOD_SHIFT;
			return esp;
			break;

		case 0x1d: /* ctrl down */
			modKeys |= MOD_CTRL;
			return esp;
			break;
		case 0x9d: /* ctrl up */
			modKeys &= ~MOD_CTRL;
			return esp;
			break;

		case 0x38: /* alt down */
			modKeys |= MOD_ALT;
			return esp;
			break;
		case 0xb8: /* alt up */
			modKeys &= ~MOD_ALT;
			return esp;
			break;

		default:
			break;
	}

    // Ignoring the break code (release key). 
    // MSB (Most Significant Bit) is 1 for break code.
    if (scanCode >= 0x80)
        return esp;

    // Get the keys from the appropriate map depending on modifier state.
    if (modKeys == MOD_SHIFT)
        key = scancode_to_key_shift[scanCode];
    else if (modKeys == MOD_NORMAL)
        key = scancode_to_key[scanCode];

    if (key != 0) {
        char c[2] = {key, '\0'};    //appending null terminator at the end.
        eventHandler->keyPress(c);
    } else {
        char* message = "KEYBOARD 0x00 ";
        char* hex = "0123456789ABCDEF";

        message[11] = hex[(scanCode >> 4) & 0xF];
        message[12] = hex[scanCode & 0xF];
        eventHandler->keyPress(message);
    }

    return esp;
}

KeyBoardEventHandler::KeyBoardEventHandler() {}

KeyBoardEventHandler::~KeyBoardEventHandler() {}

void KeyBoardEventHandler::keyPress(char* key) {}

void KeyBoardEventHandler::keyRelease(char* key) {}


PrintKeyToScreenEvent::PrintKeyToScreenEvent() {}

PrintKeyToScreenEvent::~PrintKeyToScreenEvent() {}

void PrintKeyToScreenEvent::keyPress(char* key) {
    printf(key);
}

void PrintKeyToScreenEvent::keyRelease(char* key) {}