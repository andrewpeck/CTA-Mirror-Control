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

    for (int i=0; i<192; i++)
    {
        int config = layout.gpioConfiguration(i);
        if (config==0)   // output
        {
            fprintf(configfile,"Configuring gpio %i as output\n", i);
            gpioSetDirection(i, 0);
        }
        if (config==1)    // input
        {
            fprintf(configfile,"Configuring gpio %i as input\n", i);
            gpioSetDirection(i, 1);
        }
    }
}
