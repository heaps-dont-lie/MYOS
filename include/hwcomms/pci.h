/*
    Author: Aman Pandey
    E-mail: amanpandey1235@gmail.com
*/

#ifndef __PCI_H
#define __PCI_H

#include <hwcomms/ports.h>
#include <drivers/driver.h>
#include <hwcomms/interrupts.h>

#define PCI_CMD_PORT_NUM 0xCF8
#define PCI_DATA_PORT_NUM 0xCFC

enum BaseAddressRegisterType {
    MMIO = 0, // Memory Mapping I/O
    PMIO = 1  // Port Mapping I/O
};

class BaseAddressRegister {
    public:
        bool prefetchableBit;
        uint8_t* address;
        uint32_t size;
        BaseAddressRegisterType type;
};

class PeripheralComponentInterconnectDescriptor {
    public:
        uint32_t portBase;
        uint32_t interrupt;

        uint16_t busNumber;
        uint16_t deviceNumber;
        uint16_t funcNumber;

        uint16_t vendorId;
        uint16_t deviceId;

        uint8_t classId;
        uint8_t subClassId;
        uint8_t interfaceId;

        uint8_t revision;

        PeripheralComponentInterconnectDescriptor();
        ~PeripheralComponentInterconnectDescriptor();
};

class PeripheralComponentInterconnectController {
    private:
        Port32Bit pciCommandPort;
        Port32Bit pciDataPort;

    public:
        PeripheralComponentInterconnectController();
        ~PeripheralComponentInterconnectController();

        uint32_t Read(uint8_t busNumber, uint8_t deviceNumber, uint8_t funcNumber, uint8_t offset);
        void Write(uint8_t busNumber, uint8_t deviceNumber, uint8_t funcNumber, uint8_t offset, uint32_t data);
        bool isFunctionAvailable(uint8_t busNumber, uint8_t deviceNumber);
        void enumerateDevices(DriverManager* driverManager, InterruptManager* interruptManager);
        PeripheralComponentInterconnectDescriptor getDeviceDescriptor(uint8_t busNumber, uint8_t deviceNumber, uint8_t funcNumber);
        BaseAddressRegister getBaseAddressRegister(uint8_t busNumber, uint8_t deviceNumber, uint8_t funcNumber, uint16_t barNum);
        Driver* getDriverForDevice(PeripheralComponentInterconnectDescriptor devDescriptor, InterruptManager* interruptManager);
};

#endif