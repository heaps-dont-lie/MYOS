#include <drivers/driver.h>

Driver::Driver() {}

Driver::~Driver() {}

void Driver::initHardware() {}

int Driver::resetHardware() {
    return 0;
}

void Driver::deinitHardware() {}

DriverManager::DriverManager() {
    _driver_count = 0;
}

DriverManager::~DriverManager() {}

void DriverManager::addDriver(Driver* driver) {
    _drivers[_driver_count] = driver;
    _driver_count++;
}