/*
    Author: Aman Pandey
    E-mail: amanpandey1235@gmail.com
*/

#ifndef __KEYBOARDDRIVER_H
#define __KEYBOARDDRIVER_H

#include <hwcomms/interrupts.h>
#include <drivers/driver.h>

/* 
    KeyBoardEventHandler class hanldes events related to keyboard
    Different kind of keyboard usage (like printing to the screen)
    can subscribe to this class and change the behavior of keyboard.
*/
class KeyBoardEventHandler {
    
    public:
        KeyBoardEventHandler();
        ~KeyBoardEventHandler();

        virtual void keyPress(char*);
        virtual void keyRelease(char*);
};

class PrintKeyToScreenEvent : public KeyBoardEventHandler {

    public:
        PrintKeyToScreenEvent();
        ~PrintKeyToScreenEvent();

        virtual void keyPress(char*);
        virtual void keyRelease(char*);

};

class KeyboardDriver : public HardwareInterruptHandler, public Driver {

    Port8Bit commandPort;
    Port8Bit dataPort;
    uint8_t modKeys; 
    KeyBoardEventHandler* eventHandler;

    public:
        KeyboardDriver(InterruptManager* interruptManager, KeyBoardEventHandler* eventHandler);
        ~KeyboardDriver();
        virtual uint32_t handleInterrupt(uint32_t esp);
        virtual void initHardware();
};

#endif