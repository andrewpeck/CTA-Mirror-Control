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
#include <Overo.hpp>

#define GPIODIR_IN  0
#define GPIODIR_OUT 1

Overo::Overo() {};
Overo::~Overo() {};

bool Overo::gpioReadLevel(const unsigned ipin)
{
    bool level = *(gpio.ptrGPIOReadLevel(ipin)) & (0x1 << ipin);
    printf("Read %i from pin %i",level,ipin);
    return level;
}

void Overo::gpioWriteLevel(const unsigned ipin, bool level)
{
    //printf("\nWriting %i to pin %i", level, ipin);
    if (level)
        gpio.gpioSetLevel(ipin);
    else
        gpio.gpioClrLevel(ipin);
}

bool Overo::gpioGetDirection(const unsigned ipin)
{
    volatile uint32_t* reg = gpio.ptrGPIODirection(ipin);
    uint32_t val = *reg;
    bool dir = !!(val & (0x1<<ipin));
    return dir;
}

void Overo::gpioSetDirection(const unsigned ipin, bool dir)
{
    volatile uint32_t* reg = gpio.ptrGPIODirection(ipin);
    uint32_t val = *reg;
    if(dir==1)
        val |= (0x1<<ipin);
    else
        val &= ~(0x1<<ipin);
    *reg = val;
    //printf("\ngpioSetDirection :: Writing %04X", val);
}

void Overo::gpioConfigure(const unsigned ipin, bool dir)
{
    gpioSetDirection(ipin, dir);
}

void Overo::gpioConfigureAll()
{
    FILE * configfile;
    configfile = fopen ("gpioconf","w");

    for (int i=0; i<192; i++) {
        int config = layout.gpioConfiguration(i);
        if (config==0) { // output
            fprintf(configfile,"Configuring gpio %i as output\n", i);
            gpioSetDirection(i, 0);
        }
        if (config==1) {  // input
            fprintf(configfile,"Configuring gpio %i as input\n", i);
            gpioSetDirection(i, 1);
        }
    }
}

bool Overo::hasGPIORegister(const unsigned ipin)
{
    return ipin<nGPIO;
}

bool Overo::hasGPIOPin(const unsigned ipin)
{
    if (ipin<0 || ipin>=nGPIO)
        return false;
    else
        return true;
}

uint64_t Overo::serialNumber() const
{
    std::ifstream stream("/proc/cpuinfo");
    if(!stream.good())return 0;
    std::string line;
    while(getline(stream,line)) {
        std::string key;
        std::string::size_type ichar=0;
        while(ichar<line.size()) {
            char c = line[ichar];
            if(c==':')break;
            if(!isspace(c))key += tolower(c);
            ichar++;
        }
        if(ichar<line.size())ichar++;
        while((ichar<line.size())&&(isspace(line[ichar])))ichar++;
        if((key == "serial")&&(ichar<line.size())) {
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

