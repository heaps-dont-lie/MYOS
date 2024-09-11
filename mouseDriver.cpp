#include "mouseDriver.h"

uint16_t* MouseDriver::videoMemory;

MouseDriver::MouseDriver(InterruptManager* interruptManager) :
HardwareInterruptHandler(MOUSE_INT, interruptManager), commandPort(0x64), dataPort(0x60) {
    videoMemory = (uint16_t*)0xB8000;
    initHardware();
}

MouseDriver::~MouseDriver() {

}

void MouseDriver::initHardware() {

    printf("MouseDriver::initHardware : Initializing Mouse Hardware...\n");

    offset = 0;
    button = 0;

    // Swapping the foreground (first 4 bits of MSByte) with background color (last 4 bits of MSByte)
    // LSByte remains the same (its the data)
    videoMemory[80*INIT_Y + INIT_X] = ((videoMemory[80*INIT_Y + INIT_X] & 0xF000) >> 4) 
                            | ((videoMemory[80*INIT_Y + INIT_X] & 0x0F00) << 4)
                            | (videoMemory[80*INIT_Y + INIT_X] & 0x00FF);
    
    //Enable the mouse
    commandPort.Write(0xA8);

    //Read the PS/2 controller command byte
    commandPort.Write(0x20);
    uint8_t status = dataPort.Read();
    
    //Set the bit 1 (enable mouse IRQ 12) and write back
    status |= 0x02;
    commandPort.Write(0x60);
    dataPort.Write(status);

    commandPort.Write(0xD4);
    dataPort.Write(0xF4);
    dataPort.Read();    //TODO: should assert ACK (0xFA) for safety. 

    if (dataPort.Read() != 0xFA) {
        printf("Mouse initialization failed.\n");
        return;  // Handle failure case
    }

    printf("MouseDriver::initHardware : Done initializing Mouse Hardware.\n");
}

uint32_t MouseDriver::handleInterrupt(uint32_t esp) {

    //Check if there is any data from the mouse
    uint8_t status = commandPort.Read();
    if (!(status & 0x20))
        return esp;  
    
    // The cursor should begin from the middle of the screen.
    static int8_t x = INIT_X, y = INIT_Y;

    buffer[offset] = dataPort.Read();
    offset = (offset + 1) % 3; 

    // If offset is 0 then all 3 bytes is ready to determine the cursor position
    if (offset == 0) {

        // Before we swap the new position, swap back the old one (restore it)
        videoMemory[80*y + x] = ((videoMemory[80*y + x] & 0xF000) >> 4) 
                                | ((videoMemory[80*y + x] & 0x0F00) << 4)
                                | (videoMemory[80*y + x] & 0x00FF);

        x += buffer[1];
        if (x < 0) x = 0;
        if (x >= 80) x = 79;

        y -= buffer[2];
        if (y < 0) y = 0;
        if (y >= 25) y = 24; 

        // Swapping the foreground (first 4 bits of MSByte) with background color (last 4 bits of MSByte)
        // LSByte remains the same (its the data)
        videoMemory[80*y + x] = ((videoMemory[80*y + x] & 0xF000) >> 4)
                                | ((videoMemory[80*y + x] & 0x0F00) << 4)
                                | (videoMemory[80*y + x] & 0x00FF);

        // Handle buttons
        for(uint8_t i = 0; i < 3; i++) {
            if ((buffer[0] & (0x01 << i)) != (button & (0x01 << i))) {
                // TODO: Button is pressed
            }
        }
        button = buffer[0];
    }

    return esp;
}