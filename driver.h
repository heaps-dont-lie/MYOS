#ifndef __DRIVER_H
#define __DRIVER_H

#define MAX_DRIVERS 256


/*
* A driver base class. Drivers (Keyboard driver, for example)
* can inherit from this base class
*/
class Driver {
    public:
        Driver();
        ~Driver();

        virtual void initHardware();
        virtual int resetHardware();
        virtual void deinitHardware();
};

/*
* A manager class that manages all the drivers
*/
class DriverManager {
    
    private:
        Driver* _drivers[MAX_DRIVERS];
        int _driver_count;

    public:
        DriverManager();
        ~DriverManager();
        void addDriver(Driver*);
};

#endif