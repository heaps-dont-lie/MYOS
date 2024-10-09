#include <hwcomms/pci.h>
#include <common/stdio.h>

PeripheralComponentInterconnectController::PeripheralComponentInterconnectController()
: pciCommandPort(PCI_CMD_PORT_NUM), pciDataPort(PCI_DATA_PORT_NUM)
{}

PeripheralComponentInterconnectController::~PeripheralComponentInterconnectController() {}

uint32_t PeripheralComponentInterconnectController::Read(uint8_t busNumber, uint8_t deviceNumber, uint8_t funcNumber, uint8_t offset) {
    uint32_t data;
    uint32_t address = static_cast<uint32_t>(1) << 31
                | static_cast<uint32_t>(busNumber) << 16
                | static_cast<uint32_t>(deviceNumber) << 11
                | static_cast<uint32_t>(funcNumber) << 8
                | (static_cast<uint32_t>(offset) & 0xFC); // & with 0xFC to make it 4 byte aligned.
    pciCommandPort.Write(address);
    data = pciDataPort.Read();

    return data >> (8 * (offset % 4));
}

void PeripheralComponentInterconnectController::Write(uint8_t busNumber, uint8_t deviceNumber, uint8_t funcNumber, uint8_t offset, uint32_t data) {
    uint32_t address = static_cast<uint32_t>(1) << 31
                | static_cast<uint32_t>(busNumber) << 16
                | static_cast<uint32_t>(deviceNumber) << 11
                | static_cast<uint32_t>(funcNumber) << 8
                | (static_cast<uint32_t>(offset) & 0xFC);
    pciCommandPort.Write(address);
    pciDataPort.Write(data);  
}

/*
* Multifunction presence is at the 0x0E offset address 
* and depicted by the 8th bit. 1 is true else false.
*/
bool PeripheralComponentInterconnectController::isFunctionAvailable(uint8_t busNumber, uint8_t deviceNumber) {
    uint32_t data = Read(busNumber, deviceNumber, 0x0, 0x0E);
    return data << (0x1 << 7);
}

void PeripheralComponentInterconnectController::enumerateDevices(DriverManager* driverManager, InterruptManager* interruptManager) {
    for (uint16_t bus = 0; bus < 8; bus++) {
        for (uint16_t device = 0; device < 32; device++) {
            uint16_t func_count = isFunctionAvailable(bus, device) ? 1 : 8;
            for (uint16_t func = 0; func < func_count; func++) {
                PeripheralComponentInterconnectDescriptor devDescriptor = getDeviceDescriptor(bus, device, func);

                // No more functions
                if (devDescriptor.vendorId == 0x0000 || devDescriptor.vendorId == 0xFFFF)
                    continue;

                // Iterate over base address registers
                for(int barNum = 0; barNum < 6; barNum++) {
                    BaseAddressRegister bar = getBaseAddressRegister(bus, device, func, barNum);
                    /*
                    * In the getBaseAddressRegister func, the "address" will be
                    * set to the higher bits of the BAR, which in case of PMIO
                    * contain the port number.
                    */
                    if (bar.address && (bar.type == BaseAddressRegisterType::PMIO))
                        devDescriptor.portBase = (uint32_t) bar.address;

                    Driver* driver = getDriverForDevice(devDescriptor, interruptManager);
                    if (driver != NULL)
                        driverManager->addDriver(driver);
                }

                printf("PCI BUS ");
                printfHex(bus & 0xFF);

                printf(", DEVICE ");
                printfHex(device & 0xFF);

                printf(", FUNCTION = ");
                printfHex(func & 0xFF);

                printf(", VENDOR ID ");
                printfHex((devDescriptor.vendorId & 0xFF00) >> 8);
                printfHex(devDescriptor.vendorId & 0xFF);

                printf(", DEVICE ID ");
                printfHex((devDescriptor.deviceId & 0xFF00) >> 8);
                printfHex(devDescriptor.deviceId & 0xFF);

                printf("\n");
            }
        }
    }
}


PeripheralComponentInterconnectDescriptor::PeripheralComponentInterconnectDescriptor() {}

PeripheralComponentInterconnectDescriptor::~PeripheralComponentInterconnectDescriptor() {}

PeripheralComponentInterconnectDescriptor PeripheralComponentInterconnectController::getDeviceDescriptor(uint8_t busNumber, uint8_t deviceNumber, uint8_t funcNumber) {
    PeripheralComponentInterconnectDescriptor ret;

    ret.busNumber = busNumber;
    ret.deviceNumber = deviceNumber;
    ret.funcNumber = funcNumber;

    ret.vendorId = Read(busNumber, deviceNumber, funcNumber, 0x00);
    ret.deviceId = Read(busNumber, deviceNumber, funcNumber, 0x02);

    ret.classId = Read(busNumber, deviceNumber, funcNumber, 0x0B);
    ret.subClassId = Read(busNumber, deviceNumber, funcNumber, 0x0A);
    ret.interfaceId = Read(busNumber, deviceNumber, funcNumber, 0x09);

    ret.revision = Read(busNumber, deviceNumber, funcNumber, 0x08);
    ret.interrupt = Read(busNumber, deviceNumber, funcNumber, 0x3C);

    return ret;
}

BaseAddressRegister PeripheralComponentInterconnectController::getBaseAddressRegister(uint8_t busNumber, uint8_t deviceNumber, uint8_t funcNumber, uint16_t barNum) {
    BaseAddressRegister bar;
    uint8_t headerType = Read(busNumber, deviceNumber, funcNumber, 0x0E) & 0x7F;
    uint8_t maxBarCnt = 6 - 4*headerType;

    if (barNum >= maxBarCnt)
        return bar;

    uint32_t barData = Read(busNumber, deviceNumber, funcNumber, 0x10 + 4*(barNum));
    bar.type = (barData & 0x01) ? BaseAddressRegisterType::PMIO : BaseAddressRegisterType::MMIO;

    if (bar.type == BaseAddressRegisterType::PMIO) {
        // Port number that we will use for comms.
        bar.address = (uint8_t*)(barData & ~0x03);
        bar.prefetchableBit = false;
    } else {
        //TODO: Refer low-level.edu
    }

    return bar;
}

Driver* PeripheralComponentInterconnectController::getDriverForDevice(PeripheralComponentInterconnectDescriptor devDescriptor, InterruptManager* interruptManager) {
    return NULL;
}