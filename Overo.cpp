//  Class which maps to Gumstix Verdex XM4 architecture, which mostly
//  exposes the functionality of Marvell (formerly Intel) XScale PXA270
//  on which it is based.

#include <sys/mman.h>
#include <stdint.h>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <cctype>

#include <Layout.hpp>
#include <Bits.hpp>
#include <GPIOInterface.hpp>
#include <Overo.hpp>

#define GPIODIR_IN  0
#define GPIODIR_OUT 1



Overo::Overo() {}; 
Overo::~Overo() {}; 

// --------------------------------------------------------------------------
// GPIO utility functions
// --------------------------------------------------------------------------

// Read GPIO by ipin (0-192)
bool Overo::gpioReadLevel(const unsigned ipin) {
    bool level = *(gpio.ptrGPIOReadLevel(ipin)) & Bits::mask1Bit(ipin);
    printf("Read %i from pin %i",level,ipin);
    return level; 
}

// Write GPIO by ipin (0-192)
void Overo::gpioWriteLevel(const unsigned ipin, bool level) {
    //printf("\nWriting %i to pin %i", level, ipin); 
    if (level)
        gpio.gpioSetLevel(ipin);
    else 
        gpio.gpioClrLevel(ipin);
}

// Get GPIO Direction In/Out
bool Overo::gpioGetDirection(const unsigned ipin) {
    volatile uint32_t* reg = gpio.ptrGPIODirection(ipin);
    uint32_t val = *reg;
    bool dir = !!(val & Bits::mask1Bit(ipin));
    return dir; 
}

// Set GPIO Direction In/Out
void Overo::gpioSetDirection(const unsigned ipin, bool dir) {
    volatile uint32_t* reg = gpio.ptrGPIODirection(ipin);
    uint32_t val = *reg;
    if(dir==1)
        val |= (0x1<<ipin);
    else 
        val &= ~(0x1<<ipin);
    *reg = val;
    //printf("\ngpioSetDirection :: Writing %04X", val);
}

// Configures a given GPIO pin for a direction (1=OUT,0=in) and function
void Overo::gpioConfigure(const unsigned ipin, bool dir) {
    gpioSetDirection(ipin, dir);
}

//Configure Input/Output directions for GPIOs
void Overo::gpioConfigureAll() {
    for (int i=0; i<192; i++) {
        int config = layout.gpioConfiguration(i);
        if (config==0){ // output
            printf("\nConfiguring gpio %i as output", i); 
            gpioSetDirection(i, 0);
        }
        if (config==1) {  // input
            printf("\nConfiguring gpio %i as input", i); 
            gpioSetDirection(i, 1);
        }
    }
}

// --------------------------------------------------------------------------
// STATIC GPIO functions
// --------------------------------------------------------------------------

//Number of GPIOs Available
unsigned Overo::nGPIO() { 
    return 192; 
}

// check if ipin is greater than the number of GPIOs
bool Overo::hasGPIORegister(const unsigned ipin) { 
    return ipin<nGPIO(); 
}

// Curtain off unavailable GPIOs
bool Overo::hasGPIOPin(const unsigned ipin) {
    if((ipin==2) || ((ipin<=8) && (ipin>=5)) || (ipin>=nGPIO()))
        return false;
    return true;
}

uint64_t Overo::serialNumber() const {
    std::ifstream stream("/proc/cpuinfo");
    if(!stream.good())return 0;
    std::string line;
    while(getline(stream,line))
    {
        std::string key;
        std::string::size_type ichar=0;
        while(ichar<line.size())
        {
            char c = line[ichar];
            if(c==':')break;
            if(!isspace(c))key += tolower(c);
            ichar++;
        }
        if(ichar<line.size())ichar++;
        while((ichar<line.size())&&(isspace(line[ichar])))ichar++;
        if((key == "serial")&&(ichar<line.size()))
        {
            std::string val;
            while((ichar<line.size())&&(isxdigit(line[ichar])))
                val += line[ichar++];
            std::istringstream sstream(val);
            uint64_t serial = 0;
            sstream >> std::hex >> serial;
            return serial;
        }
    }
    return 0;
}

