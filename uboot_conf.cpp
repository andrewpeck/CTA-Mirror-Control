//-*-mode:c++; mode:font-lock;-*-

/*! \file uboot_conf.cpp

  Write GPIO configuration constants for uBoot

  \author     Stephen Fegan               \n
  UCLA                        \n
  sfegan@astro.ucla.edu       \n

  \version    1.0
  \date       08/30/2008
  */

#include <iostream>
#include <iomanip>
#include "Overo.hpp"
#include "Layout.hpp"

int main(int argc, char** argv)
{
    typedef Overo<SimulatedRegisters> Sys;
    Sys sys;  

    uint32_t M32[] = { 0xFFFFFE1B, 0xFFFFFFFF, 0xFFFFFFFF, 0x007FFFFF };
    uint32_t M16[] = { 0xFFFC03CF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
        0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00003FFF };

    const Layout<>::PinConf* PC = 0;
    unsigned npinconf = Layout<>::pin140Conf(PC);

    for(unsigned ipin=0;ipin<npinconf;ipin++)
    {
        sys.gpioConfigure(PC[ipin].igpio, PC[ipin].dir);
        sys.gpioWriteLevel(PC[ipin].igpio, PC[ipin].val);
    }

    Sys sy2;  

    std::cout << std::setfill('0') << std::uppercase;
    for(unsigned ireg=0;ireg<4;ireg++)
        std::cout 
            << "#define CFG_GPSR" << std::dec << ireg << "_VAL           0x"
            << std::hex << std::setw(8) << (sys.simregGPIOLevel(ireg)&M32[ireg])
            << " /* 0x" << std::setw(8) << (sy2.simregGPIOLevel(ireg)&M32[ireg])
            << " */\n";
    std::cout << '\n';
    for(unsigned ireg=0;ireg<4;ireg++)
        std::cout 
            << "#define CFG_GPCR" << std::dec << ireg << "_VAL           0x"
            << std::hex << std::setw(8) << ((~sys.simregGPIOLevel(ireg))&M32[ireg])
            << " /* 0x" << std::setw(8) << ((~sy2.simregGPIOLevel(ireg))&M32[ireg])
            << " */\n";
    std::cout << '\n';
    for(unsigned ireg=0;ireg<4;ireg++)
        std::cout 
            << "#define CFG_GPDR" << std::dec << ireg << "_VAL           0x"
            << std::hex << std::setw(8) << (sys.simregGPIODirection(ireg)&M32[ireg])
            << " /* 0x" << std::setw(8) << (sy2.simregGPIODirection(ireg)&M32[ireg])
            << " */\n";
    std::cout << '\n';
    for(unsigned ireg=0;ireg<8;ireg++)
        std::cout 
            << "#define CFG_GAFR" << std::dec << (ireg/2) << '_' << (ireg%2?'U':'L') 
            << "_VAL         0x"
            << std::hex << std::setw(8) << (sys.simregGPIOFn(ireg)&M16[ireg])
            << " /* 0x" << std::setw(8) << (sy2.simregGPIOFn(ireg)&M32[ireg])
            << " */\n";

#if 0
    std::cout << std::dec;
    unsigned npin = sys.nGPIO();
    for(unsigned ipin=0;ipin<npin;ipin++)
        if(sys.hasGPIOPin(ipin))
        {
            std::cout << ipin << ' ' << int(sys.gpioGetDirection(ipin)) << ' '
                << int(sys.gpioGetAltFunc(ipin)) << ' '
                << int(sys.gpioReadLevel(ipin)) << '\n';
        }
#endif
}
