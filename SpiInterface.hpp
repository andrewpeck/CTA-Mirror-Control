////////////////////////////////////////////////////////////////////////////////
#ifndef SPIINTERFACE_HPP
#define SPIINTERFACE_HPP
////////////////////////////////////////////////////////////////////////////////
// SpiInterface.hpp
////////////////////////////////////////////////////////////////////////////////

#include <stdint.h>
#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))

class SpiInterface {
    public: 
        SpiInterface();     //Constructor
        ~SpiInterface();    //Destructor
        uint32_t WriteRead(uint32_t data); 

    private: 
        uint32_t transfer(int fd, uint32_t data); 
        static void pabort(const char *s); 

        static const char     *device; 
        static uint8_t  mode; 
        static uint8_t  bits; 
        static uint32_t speed; 
        static uint16_t delay; 
}; 

#endif
