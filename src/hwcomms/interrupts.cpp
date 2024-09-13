/*
    Author: Aman Pandey
    E-mail: amanpandey1235@gmail.com
*/

#include <hwcomms/interrupts.h>


void printf(char* str);

InterruptManager* InterruptManager::currentInterruptManager = NULL;
InterruptManager::GateDescriptor InterruptManager::interruptDescriptorTable[256];

void InterruptManager::setInterruptDescriptorTableEntry(uint8_t interruptNumber, uint16_t codeSegSelectorOffset, void (*handler)(), uint8_t descriptorPrivilegeLevel, uint8_t descriptorType) {
    const uint8_t IDT_DESC_PRESENT = 0x80;

    interruptDescriptorTable[interruptNumber].handlerAddressLowBits = ((uint32_t)handler) & 0xFFFF;
    interruptDescriptorTable[interruptNumber].handlerAddressHighBits = ((uint32_t)handler >> 16) & 0XFFFF;
    interruptDescriptorTable[interruptNumber].gdt_codeSegementSelector = codeSegSelectorOffset;
    interruptDescriptorTable[interruptNumber].reserved = 0;
    interruptDescriptorTable[interruptNumber].access = IDT_DESC_PRESENT | descriptorType | ((descriptorPrivilegeLevel&3) << 5);
}

InterruptManager::InterruptManager(GlobalDescriptorTable* gdt) :
    programmableInterruptControllerMasterCommandPort(0x20),
    programmableInterruptControllerMasterDataPort(0x21),
    programmableInterruptControllerSlaveCommandPort(0xA0),
    programmableInterruptControllerSlaveDataPort(0xA1)
 {
    uint16_t CodeSegment = gdt->codeSegmentOffset();
    const uint8_t IDT_INTERRUPT_GATE = 0xE;

    for(uint8_t i = 255; i > 0; --i) { //Need to initialize unmanaged interrupts with some template handler to avoid crash.
        drivers[i] = NULL;
        setInterruptDescriptorTableEntry(i, CodeSegment, &IgnoreInterruptRequest, 0, IDT_INTERRUPT_GATE);
    }
    drivers[0] = NULL;
    setInterruptDescriptorTableEntry(0, CodeSegment, &IgnoreInterruptRequest, 0, IDT_INTERRUPT_GATE);

    setInterruptDescriptorTableEntry(0x00, CodeSegment, &HandleException0x00, 0, IDT_INTERRUPT_GATE);
    setInterruptDescriptorTableEntry(0x01, CodeSegment, &HandleException0x01, 0, IDT_INTERRUPT_GATE);
    setInterruptDescriptorTableEntry(0x02, CodeSegment, &HandleException0x02, 0, IDT_INTERRUPT_GATE);
    setInterruptDescriptorTableEntry(0x03, CodeSegment, &HandleException0x03, 0, IDT_INTERRUPT_GATE);
    setInterruptDescriptorTableEntry(0x04, CodeSegment, &HandleException0x04, 0, IDT_INTERRUPT_GATE);
    setInterruptDescriptorTableEntry(0x05, CodeSegment, &HandleException0x05, 0, IDT_INTERRUPT_GATE);
    setInterruptDescriptorTableEntry(0x06, CodeSegment, &HandleException0x06, 0, IDT_INTERRUPT_GATE);
    setInterruptDescriptorTableEntry(0x07, CodeSegment, &HandleException0x07, 0, IDT_INTERRUPT_GATE);
    setInterruptDescriptorTableEntry(0x08, CodeSegment, &HandleException0x08, 0, IDT_INTERRUPT_GATE);
    setInterruptDescriptorTableEntry(0x09, CodeSegment, &HandleException0x09, 0, IDT_INTERRUPT_GATE);
    setInterruptDescriptorTableEntry(0x0A, CodeSegment, &HandleException0x0A, 0, IDT_INTERRUPT_GATE);
    setInterruptDescriptorTableEntry(0x0B, CodeSegment, &HandleException0x0B, 0, IDT_INTERRUPT_GATE);
    setInterruptDescriptorTableEntry(0x0C, CodeSegment, &HandleException0x0C, 0, IDT_INTERRUPT_GATE);
    setInterruptDescriptorTableEntry(0x0D, CodeSegment, &HandleException0x0D, 0, IDT_INTERRUPT_GATE);
    setInterruptDescriptorTableEntry(0x0E, CodeSegment, &HandleException0x0E, 0, IDT_INTERRUPT_GATE);
    setInterruptDescriptorTableEntry(0x0F, CodeSegment, &HandleException0x0F, 0, IDT_INTERRUPT_GATE);
    setInterruptDescriptorTableEntry(0x10, CodeSegment, &HandleException0x10, 0, IDT_INTERRUPT_GATE);
    setInterruptDescriptorTableEntry(0x11, CodeSegment, &HandleException0x11, 0, IDT_INTERRUPT_GATE);
    setInterruptDescriptorTableEntry(0x12, CodeSegment, &HandleException0x12, 0, IDT_INTERRUPT_GATE);
    setInterruptDescriptorTableEntry(0x13, CodeSegment, &HandleException0x13, 0, IDT_INTERRUPT_GATE);

    setInterruptDescriptorTableEntry(0x20, CodeSegment, &HandleInterruptRequest0x00, 0, IDT_INTERRUPT_GATE);
    setInterruptDescriptorTableEntry(0x21, CodeSegment, &HandleInterruptRequest0x01, 0, IDT_INTERRUPT_GATE);
    setInterruptDescriptorTableEntry(0x22, CodeSegment, &HandleInterruptRequest0x02, 0, IDT_INTERRUPT_GATE);
    setInterruptDescriptorTableEntry(0x23, CodeSegment, &HandleInterruptRequest0x03, 0, IDT_INTERRUPT_GATE);
    setInterruptDescriptorTableEntry(0x24, CodeSegment, &HandleInterruptRequest0x04, 0, IDT_INTERRUPT_GATE);
    setInterruptDescriptorTableEntry(0x25, CodeSegment, &HandleInterruptRequest0x05, 0, IDT_INTERRUPT_GATE);
    setInterruptDescriptorTableEntry(0x26, CodeSegment, &HandleInterruptRequest0x06, 0, IDT_INTERRUPT_GATE);
    setInterruptDescriptorTableEntry(0x27, CodeSegment, &HandleInterruptRequest0x07, 0, IDT_INTERRUPT_GATE);
    setInterruptDescriptorTableEntry(0x28, CodeSegment, &HandleInterruptRequest0x08, 0, IDT_INTERRUPT_GATE);
    setInterruptDescriptorTableEntry(0x29, CodeSegment, &HandleInterruptRequest0x09, 0, IDT_INTERRUPT_GATE);
    setInterruptDescriptorTableEntry(0x2A, CodeSegment, &HandleInterruptRequest0x0A, 0, IDT_INTERRUPT_GATE);
    setInterruptDescriptorTableEntry(0x2B, CodeSegment, &HandleInterruptRequest0x0B, 0, IDT_INTERRUPT_GATE);
    setInterruptDescriptorTableEntry(0x2C, CodeSegment, &HandleInterruptRequest0x0C, 0, IDT_INTERRUPT_GATE);
    setInterruptDescriptorTableEntry(0x2D, CodeSegment, &HandleInterruptRequest0x0D, 0, IDT_INTERRUPT_GATE);
    setInterruptDescriptorTableEntry(0x2E, CodeSegment, &HandleInterruptRequest0x0E, 0, IDT_INTERRUPT_GATE);
    setInterruptDescriptorTableEntry(0x2F, CodeSegment, &HandleInterruptRequest0x0F, 0, IDT_INTERRUPT_GATE);

    programmableInterruptControllerMasterCommandPort.Write(0x11);
    programmableInterruptControllerSlaveCommandPort.Write(0x11);

    // remap
    programmableInterruptControllerMasterDataPort.Write(0x20);
    programmableInterruptControllerSlaveDataPort.Write(0x28);

    programmableInterruptControllerMasterDataPort.Write(0x04);
    programmableInterruptControllerSlaveDataPort.Write(0x02);

    programmableInterruptControllerMasterDataPort.Write(0x01);
    programmableInterruptControllerSlaveDataPort.Write(0x01);

    programmableInterruptControllerMasterDataPort.Write(0x00);
    programmableInterruptControllerSlaveDataPort.Write(0x00);

    interruptDescriptorTablePointer idt_pointer;
    idt_pointer.size = 256*(sizeof(GateDescriptor)) - 1;
    idt_pointer.base = (uint32_t)interruptDescriptorTable;

    //Load the IDT in the CPU register.
    asm volatile("lidt %0" : : "m" (idt_pointer));
}

void InterruptManager::activate() {
    /*  If there is some other InterruptManager instance running
    *   then deactivate that and initialize the current InterruptManager
    *   with the current instance.
    */
    if (currentInterruptManager != NULL)
        currentInterruptManager->deactivate();
    currentInterruptManager = this;
    asm("sti");
}

void InterruptManager::deactivate() {
    if (currentInterruptManager == this) {
        currentInterruptManager = NULL;
        asm("cli");
    }
}

uint32_t InterruptManager::doHandleInterrupt(uint8_t interruptNumber, uint32_t esp) {

    // If a driver exists for this interrupt then call the respective handler
    if (drivers[interruptNumber] != NULL)
        esp = drivers[interruptNumber]->handleInterrupt(esp);

    else if (interruptNumber != TIMER_INT) {
        char* message = "INTERRUPT 0x00";
        char* hex = "0123456789ABCDEF";

        message[12] = hex[(interruptNumber >> 4) & 0xF];
        message[13] = hex[interruptNumber & 0xF];
        printf(message);
    }

    /*
    *   Once the interrupt is handled we need
    *   to notify the PIC that last sent hardware interrupt
    *   signal was handled and now send the next
    *   interrupt. We notify by writting 0x20 at the 
    *   appropriate command port.
    *
    *   Note: [0x20, 0x30) are all possible Hardware
    *   interrupts.
    *   We only write to the slave command port if the hardware
    *   interrupt was mapped to the slave PIC.
    */
    if (interruptNumber >= 0x20 and interruptNumber < 0x30) {
        programmableInterruptControllerMasterCommandPort.Write(0x20);
        if (interruptNumber >= 0x28)
            programmableInterruptControllerSlaveCommandPort.Write(0x20);
    }
    return esp;
}

uint32_t InterruptManager::handleInterrupt(uint8_t interruptNumber, uint32_t esp) {
    if (currentInterruptManager != NULL)
        currentInterruptManager->doHandleInterrupt(interruptNumber, esp);

    return esp;
}

HardwareInterruptHandler::HardwareInterruptHandler(uint8_t interruptNumber, InterruptManager* interruptManager) {
    this->interruptNumber = interruptNumber;
    this->interruptManager = interruptManager;

    // Register this driver into the list of drivers for this interrupt manager.
    interruptManager->drivers[interruptNumber] = this;
}

HardwareInterruptHandler::~HardwareInterruptHandler() {
    // Unregister the driver from the list of drivers for this interrupt manager
    if (interruptManager->drivers[interruptNumber] == this)
        interruptManager->drivers[interruptNumber] = NULL;
}

uint32_t HardwareInterruptHandler::handleInterrupt(uint32_t esp) {
    return esp;
}