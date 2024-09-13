/*
    Author: Aman Pandey
    E-mail: amanpandey1235@gmail.com
*/

#ifndef __INTERRUPTS_H
#define __INTERRUPTS_H

#include <common/stdio.h>
#include <hwcomms/ports.h>
#include <gdt.h>

const uint8_t TIMER_INT     = 0x20;
const uint8_t KEYB_INT      = 0x21;
const uint8_t MOUSE_INT     = 0x2C;

class HardwareInterruptHandler;

class InterruptManager {
    friend class HardwareInterruptHandler;

    protected:

        HardwareInterruptHandler* drivers[256]; // Storing drivers for respective interrupts/exceptions
        static InterruptManager* currentInterruptManager;

        struct GateDescriptor {
            uint16_t handlerAddressLowBits;
            uint16_t gdt_codeSegementSelector;
            uint8_t reserved;
            uint8_t access;
            uint16_t handlerAddressHighBits;
        }__attribute__((packed));

        static GateDescriptor interruptDescriptorTable[256];

        struct interruptDescriptorTablePointer {
            uint16_t size;
            uint32_t base;
        }__attribute__((packed));

        static uint32_t handleInterrupt(uint8_t interruptNumber, uint32_t esp);

        static void IgnoreInterruptRequest();

        static void HandleInterruptRequest0x00();
        static void HandleInterruptRequest0x01();
        static void HandleInterruptRequest0x02();
        static void HandleInterruptRequest0x03();
        static void HandleInterruptRequest0x04();
        static void HandleInterruptRequest0x05();
        static void HandleInterruptRequest0x06();
        static void HandleInterruptRequest0x07();
        static void HandleInterruptRequest0x08();
        static void HandleInterruptRequest0x09();
        static void HandleInterruptRequest0x0A();
        static void HandleInterruptRequest0x0B();
        static void HandleInterruptRequest0x0C();
        static void HandleInterruptRequest0x0D();
        static void HandleInterruptRequest0x0E();
        static void HandleInterruptRequest0x0F();
        static void HandleInterruptRequest0x31();

        static void HandleException0x00();
        static void HandleException0x01();
        static void HandleException0x02();
        static void HandleException0x03();
        static void HandleException0x04();
        static void HandleException0x05();
        static void HandleException0x06();
        static void HandleException0x07();
        static void HandleException0x08();
        static void HandleException0x09();
        static void HandleException0x0A();
        static void HandleException0x0B();
        static void HandleException0x0C();
        static void HandleException0x0D();
        static void HandleException0x0E();
        static void HandleException0x0F();
        static void HandleException0x10();
        static void HandleException0x11();
        static void HandleException0x12();
        static void HandleException0x13();

        static void setInterruptDescriptorTableEntry(uint8_t interruptNumber, uint16_t codeSegSelectorOffset, void (*handler)(), uint8_t descriptorPrivilegeLevel, uint8_t descriptorType);
        
        Port8BitSlow programmableInterruptControllerMasterCommandPort;
        Port8BitSlow programmableInterruptControllerMasterDataPort;
        Port8BitSlow programmableInterruptControllerSlaveCommandPort;
        Port8BitSlow programmableInterruptControllerSlaveDataPort;

    public:

        InterruptManager(GlobalDescriptorTable* gdt);
        ~InterruptManager();

        void activate();
        void deactivate();
        uint32_t doHandleInterrupt(uint8_t interruptNumber, uint32_t esp);
};


/*
* A HardwareInterruptHandler base class. Drivers (Keyboard driver, for example)
* can inherit from this base class so that its easier for Interrupt manager
* to handover the hardware interrupts to the appropriate hardware drivers.
*/
class HardwareInterruptHandler {

    protected:
        uint8_t interruptNumber;
        InterruptManager* interruptManager;

        HardwareInterruptHandler(uint8_t interruptNumber, InterruptManager* interruptManager);
        ~HardwareInterruptHandler();

    public:
        virtual uint32_t handleInterrupt(uint32_t esp);

};
#endif