////////////////////////////////////////////////////////////////////////////////
// Class which maps to Gumstix Overo architecture, which mostly
////////////////////////////////////////////////////////////////////////////////

#ifndef OVERO_HPP
#define OVERO_HPP

#include <sys/mman.h>
#include <stdint.h>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <cctype>

#include <Layout.hpp>
#include <GPIOInterface.hpp>

#define GPIODIR_IN  0
#define GPIODIR_OUT 1


class OveroBase {
    public:
        // --------------------------------------------------------------------------
        // STATIC GPIO functions
        // --------------------------------------------------------------------------
        unsigned nGPIO(); 
        bool hasGPIORegister(const unsigned ipin); 
        bool hasGPIOPin(const unsigned ipin); 
};


class Overo {
    public:
        Overo(); 
        ~Overo(); 

        bool gpioReadLevel(const unsigned ipin); 
        void gpioWriteLevel(const unsigned ipin, bool level); 
        bool gpioGetDirection(const unsigned ipin); 
        void gpioSetDirection(const unsigned ipin, bool dir); 
        void gpioConfigure(const unsigned ipin, bool dir); 
        void gpioConfigureAll(); 

        // --------------------------------------------------------------------------
        // Utility functions
        // --------------------------------------------------------------------------

        void loopDelay(unsigned nloop) const; 
        void usecDelay(unsigned nusec) const; 
        uint64_t serialNumber() const; 
    private: 
        GPIOInterface gpio; 
        Layout layout; 
};

#endif
