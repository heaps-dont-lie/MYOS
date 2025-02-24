#ifndef __MOUSEDRIVER_H
#define __MOUSEDRIVER_H

#include <hwcomms/interrupts.h>
#include <drivers/driver.h>

#define MOUSE_BUFFER_SIZE 3

const int INIT_X    = 40;
const int INIT_Y    = 12;

class MouseDriver : public HardwareInterruptHandler, public Driver {

    private:
        Port8Bit commandPort;
        Port8Bit dataPort;

        uint8_t buffer[MOUSE_BUFFER_SIZE];
        uint8_t offset;
        uint8_t button;

        static uint16_t* videoMemory;

    public:
        MouseDriver(InterruptManager* interruptManager);
        ~MouseDriver();
        virtual uint32_t handleInterrupt(uint32_t esp);
        virtual void initHardware();
};

/* 
    MouseEventHandler class hanldes events related to mouse
    Different kind of mouse usage (like clicking)
    can subscribe to this class and change the behavior of mouse.
*/
class MouseEventHandler {
    
    public:
        MouseEventHandler();
        ~MouseEventHandler();

        virtual void onClick(int8_t x, int8_t y);
        virtual void onRelease(int8_t x, int8_t y);
        virtual void onMove(int8_t x, int8_t y);
};

#endif