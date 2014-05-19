////////////////////////////////////////////////////////////////////////////////
// Opens CPU Physical Memory at /dev/mem
// Maps physical memory into virtual address space (unix command mmap)
////////////////////////////////////////////////////////////////////////////////

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>

#include "Registers.hpp"

#define MMAPFAIL ((void*)-1)

#define MAKEMAP(PHYS,VIRT,TEXT) \
    if(makeMap(PHYS,VIRT) == MMAPFAIL)			\
{							\
    perror("mmap(" TEXT ")");				\
    exit(EXIT_FAILURE);				\
}

MappedRegisters::MappedRegisters():
    m_mmap_fd(-1), 
    m_gpio1_base(), 
    m_gpio2_base(), 
    m_gpio3_base(), 
    m_gpio4_base(), 
    m_gpio5_base(), 
    m_gpio6_base() 
    //m_mcspi1_base(), 
    //m_mcspi2_base(), 
    //m_mcspi3_base(),
    //m_mcspi4_base(),
    //m_clock_base()
{
    m_mmap_fd = open("/dev/mem", O_RDWR | O_SYNC);
    if(m_mmap_fd<0) 
    {
        perror("open(\"/dev/mem\")");
        exit(EXIT_FAILURE);
    }

    MAKEMAP(m_gpio1_base,  mapBaseGPIO1(),  "GPI01");
    MAKEMAP(m_gpio2_base,  mapBaseGPIO2(),  "GPI02");
    MAKEMAP(m_gpio3_base,  mapBaseGPIO3(),  "GPI03");
    MAKEMAP(m_gpio4_base,  mapBaseGPIO4(),  "GPI04");
    MAKEMAP(m_gpio5_base,  mapBaseGPIO5(),  "GPI04");
    MAKEMAP(m_gpio6_base,  mapBaseGPIO6(),  "GPI06");
    //MAKEMAP(m_mcspi1_base, mapBaseMCSPI1(),   "SSP1" );
    //MAKEMAP(m_mcspi2_base, mapBaseMCSPI2(),   "SSP2" );
    //MAKEMAP(m_mcspi3_base, mapBaseMCSPI3(),   "SSP3" );
    //MAKEMAP(m_mcspi4_base, mapBaseMCSPI4(),   "SSP4" );
    //MAKEMAP(m_clock_base,  mapBaseClock(),  "CLOCK");
}

#define MUNMAP(VIRT) \
    munmap(const_cast<void*>(VIRT), mapSize())

MappedRegisters::~MappedRegisters()
{
    MUNMAP(m_gpio1_base );
    MUNMAP(m_gpio2_base );
    MUNMAP(m_gpio3_base );
    MUNMAP(m_gpio4_base );
    MUNMAP(m_gpio5_base );
    MUNMAP(m_gpio6_base );
    //MUNMAP(m_mcspi1_base);
    //MUNMAP(m_mcspi2_base);
    //MUNMAP(m_mcspi3_base);
    //MUNMAP(m_mcspi4_base);
    //MUNMAP(m_clock_base );
}

volatile void* MappedRegisters::
makeMap(volatile void*& virtual_addr, off_t physical_addr, size_t length)
{
    virtual_addr = mmap(0, length, PROT_READ|PROT_WRITE, MAP_SHARED, m_mmap_fd, 
            physical_addr);
    return &virtual_addr;
}
