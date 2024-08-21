#include "ports.h"

Port::Port(uint16_t portNumber) {
    this.portNumber = portNumber;
}

Port::~Port() {}

Port8Bit::Port8Bit(uint16_t portNumber) : Port(portNumber) {}

Port8Bit::~Port8Bit() {}

Port8Bit::Write(uint8_t data) {
    __asm__ volatile("outb %0, %1" : : "a" (data), "Nd" (portNumber));
}

Port8Bit::Read() {
    uint8_t result;
    __asm__ volatile("inb %1, %0" : "=a" (result) : "Nd" (portNumber));
    return result;
}

Port16Bit::Write(uint16_t data) {
    __asm__ volatile("outw %0, %1" : : "a" (data), "Nd" (portNumber));
}

Port16Bit::Read() {
    uint16_t result;
    __asm__ volatile("inw %1, %0" : "=a" (result) : "Nd" (portNumber));
    return result;
}

Port32Bit::Write(uint32_t data) {
    __asm__ volatile("outl %0, %1" : : "a" (data), "Nd" (portNumber));
}

Port32Bit::Read() {
    uint32_t result;
    __asm__ volatile("inl %1, %0" : "=a" (result) : "Nd" (portNumber));
    return result;
}