#include "interrupts.h"

extern void printf(char* str);

InterruptManager::GateDescriptor InterruptManager::interruptDescriptorTable[256];

void InterruptManager::setInterruptDescriptorTableEntry(uint8_t interruptNumber, uint16_t codeSegSelectorOffset, void (*handler)(), uint8_t descriptorPrivilegeLevel, uint8_t descriptorType) {
    const uint8_t IDT_DESC_PRESENT = 0x80;

    interruptDescriptorTable[interruptNumber].handlerAddressLowBits = ((uint32_t)handler) & 0xFFFF;
    interruptDescriptorTable[interruptNumber].handlerAddressHighBits = ((uint32_t)handler >> 16) & 0XFFFF;
    interruptDescriptorTable[interruptNumber].gdt_codeSegementSelector = codeSegSelectorOffset;
    interruptDescriptorTable[interruptNumber].reserved = 0;
    interruptDescriptorTable[interruptNumber].access = IDT_DESC_PRESENT | descriptorType | ((descriptorPrivilegeLevel&3) << 5);
}

InterruptManager::InterruptManager(GlobalDescriptorTable* gdt) {
    uint16_t codeSegment = gdt->codeSegmentOffset();
    const uint8_t IDT_INTERRUPT_GATE = 0xE;

    for(int i = 0; i < 256; i++) { //Need to initialize unmanaged interrupts with some template handler to avoid crash.
        setInterruptDescriptorTableEntry(i, codeSegment, &IgnoreInterruptRequest, 0, IDT_INTERRUPT_GATE);
    }
    setInterruptDescriptorTableEntry(0x20, codeSegment, &HandleInterruptRequest0x00, 0, IDT_INTERRUPT_GATE);
    setInterruptDescriptorTableEntry(0x21, codeSegment, &HandleInterruptRequest0x01, 0, IDT_INTERRUPT_GATE);

    interruptDescriptorTablePointer idt;
    idt.size = 256*(sizeof(GateDescriptor)) - 1;
    idt.base = (uint32_t)interruptDescriptorTable;

    //Load the IDT in the CPU register.
    asm volatile("lidt %0" : : "m" (idt));
}

void InterruptManager::activate() {
    asm("sti");
}

uint32_t InterruptManager::handleInterrupt(uint8_t interruptNumber, uint32_t esp) {
    printf("INTERRUPT");

    return esp;
}