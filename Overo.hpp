/*
 *  Class which maps to Gumstix Overo architecture
 */

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

class Overo
{
public:
    //Constuctor + Destructor
    Overo();
    ~Overo();

    // --------------------------------------------------------------------------
    // GPIO utility functions
    // --------------------------------------------------------------------------

    // Read GPIO by ipin (0-192)
    bool gpioReadLevel(const unsigned ipin);

    // Write GPIO by ipin (0-192)
    void gpioWriteLevel(const unsigned ipin, bool level);

    // Get GPIO Direction In/Out
    bool gpioGetDirection(const unsigned ipin);

    // Set GPIO Direction In/Out
    void gpioSetDirection(const unsigned ipin, bool dir);

    // Configures a given GPIO pin for a direction (1=OUT,0=in) and function
    void gpioConfigure(const unsigned ipin, bool dir);

    //Configure Input/Output directions for GPIOs
    void gpioConfigureAll();

    // --------------------------------------------------------------------------
    // Utility functions
    // --------------------------------------------------------------------------

    // Print some board Information from /proc/cpu
    uint64_t serialNumber() const;

private:
    // Instantiate objects
    GPIOInterface gpio;
    Layout layout;

    // Number of GPIOs Available
    static const int nGPIO = 192;

    // check if ipin is greater than the number of GPIOs
    bool hasGPIORegister(const unsigned ipin);

    // Curtain off unavailable GPIOs
    bool hasGPIOPin(const unsigned ipin);
};

#endif
