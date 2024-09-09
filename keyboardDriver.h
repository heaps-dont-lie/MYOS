/*
    Author: Aman Pandey
    E-mail: amanpandey1235@gmail.com
*/

#ifndef __KEYBOARDDRIVER_H
#define __KEYBOARDDRIVER_H

#include "interrupts.h"
#include "driver.h"


class KeyboardDriver : public HardwareInterruptHandler, public Driver {

    Port8Bit commandPort;
    Port8Bit dataPort;
    uint8_t modKeys; 

    public:
        KeyboardDriver(InterruptManager* interruptManager);
        ~KeyboardDriver();
        virtual uint32_t handleInterrupt(uint32_t esp);
        virtual void initHardware();
};

#endif