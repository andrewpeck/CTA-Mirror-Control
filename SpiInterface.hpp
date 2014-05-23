// SpiInterface.hpp

#ifndef SPIINTERFACE_HPP
#define SPIINTERFACE_HPP

#include <stdint.h>
#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))

class SpiInterface {
    public: 
        SpiInterface();                     //Constructor
        ~SpiInterface();                    //Destructor


        // Write a 32 bit integer to MOSI.
        // 32 bit MISO value is returned. 
        uint32_t WriteRead(uint32_t data); 

    private: 
        uint32_t transfer(int fd, uint32_t data); 
        static void pabort(const char *s); 

        static const char   *device; 
        const uint8_t       mode; 
        const uint8_t       bits; 
        const uint32_t      speed; 
        const uint16_t      delay; 
}; 

#endif
