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
#include <stdint.h>
#include <sys/mman.h>

#include <GPIOInterface.hpp>

#define MMAPFAIL ((void*)-1)

#define MUNMAP(VIRT) \
    munmap(const_cast<void*>(VIRT), mapSize)


////////////////////////////////////////////////////////////////////////////////
// Public Members
////////////////////////////////////////////////////////////////////////////////

GPIOInterface::GPIOInterface(): 
    m_mmap_fd(-1), 
    m_gpio1_base(), 
    m_gpio2_base(), 
    m_gpio3_base(), 
    m_gpio4_base(), 
    m_gpio5_base(), 
    m_gpio6_base() {

        m_mmap_fd = open("/dev/mem", O_RDWR | O_SYNC);
        if(m_mmap_fd<0) {
            perror("open(\"/dev/mem\")");
            exit(EXIT_FAILURE);
        }

        //      virtual adr    physical adr
        makeMap(m_gpio1_base, mapBaseGPIO1);
        makeMap(m_gpio2_base, mapBaseGPIO2);
        makeMap(m_gpio3_base, mapBaseGPIO3);
        makeMap(m_gpio4_base, mapBaseGPIO4);
        makeMap(m_gpio5_base, mapBaseGPIO5);
        makeMap(m_gpio6_base, mapBaseGPIO6);
    }

GPIOInterface::~GPIOInterface() {
    MUNMAP(m_gpio1_base);
    MUNMAP(m_gpio2_base);
    MUNMAP(m_gpio3_base);
    MUNMAP(m_gpio4_base);
    MUNMAP(m_gpio5_base);
    MUNMAP(m_gpio6_base);
}


volatile uint32_t* GPIOInterface::ptrGPIODirection(const unsigned ipin) {
    return phys2VirtGPIO32(physGPIODirection(ipin),ipin); 
}

volatile uint32_t* GPIOInterface::ptrGPIOSetLevel(const unsigned ipin) { 
    return phys2VirtGPIO32(physGPIOSetLevel(ipin),ipin); 
}

volatile uint32_t* GPIOInterface::ptrGPIOReadLevel(const unsigned ipin) {
    return phys2VirtGPIO32(physGPIOReadLevel(ipin),ipin); 
}

void GPIOInterface::gpioSetLevel(const unsigned ipin) {
    *(ptrGPIOSetLevel(ipin)) = *(ptrGPIOReadLevel(ipin)) | 0x1 << ipin; 
}

void GPIOInterface::gpioClrLevel(const unsigned ipin) {
    *(ptrGPIOSetLevel(ipin)) = *(ptrGPIOReadLevel(ipin)) & ~(0x1 << ipin); 
}

////////////////////////////////////////////////////////////////////////////////
// Private Members
////////////////////////////////////////////////////////////////////////////////

// SPI1 supports up to four peripherals, SPI2 and SPI3 support up to two
// peripherals, and SPI4 supports only one peripheral.
// channel address = base address + 0x14 * x
// x= 0 to 3 for MCSPI1.
// x= 0 to 1 for MCSPI2 and MCSPI3.
// x= 0      for MCSPI4.
// in software let's enumerate these lines: 0,1,2,3,4,5,6,7,8
const int GPIOInterface::channel[] =  {0,1,2,3,0,1,0,1,0}; 

// Returns Virtual (memory mapped) address for a given GPIO pin 
volatile uint32_t* GPIOInterface::phys2VirtGPIO32(off_t phys, const unsigned ipin) {
    if (ipin<32)  {
        printf("\nphys: %04X",phys); 
        printf("\nm_gpio1_base: %04X", m_gpio1_base); 
        printf("\nmapBaseGPIO1: %04X", mapBaseGPIO1); 
        return phys2Virt32(phys,m_gpio1_base,mapBaseGPIO1); 
    }
    else if (ipin<64)
        return phys2Virt32(phys,m_gpio2_base,mapBaseGPIO2);
    else if (ipin<96)
        return phys2Virt32(phys,m_gpio3_base,mapBaseGPIO3);
    else if (ipin<160)
        return phys2Virt32(phys,m_gpio4_base,mapBaseGPIO4);
    else if (ipin<160)
        return phys2Virt32(phys,m_gpio5_base,mapBaseGPIO5);
    else if (ipin<192)
        return phys2Virt32(phys,m_gpio6_base,mapBaseGPIO6);
    else
        return 0; 
}

// Takes Physical Address and Returns pointer to memory mapped virtual address
volatile uint32_t* GPIOInterface::phys2Virt32(off_t phys, volatile void* map_base_virt, off_t map_base_phys) {
    static off_t map_offset; 
    static volatile uint32_t* adr_virtual; 

    map_offset = phys - map_base_phys; 
    printf("\nmap_offset: %02X", map_offset); 
    adr_virtual = reinterpret_cast<volatile uint32_t*>(static_cast<volatile uint8_t*>(map_base_virt) + map_offset);
    printf("\nadr_virtual: %04X", adr_virtual); 

    return adr_virtual; 
}

// --------------------------------------------------------------------------
// STATIC GPIO register (PHYSICAL) address functions
// --------------------------------------------------------------------------

off_t GPIOInterface::offset2adrGPIO(unsigned ipin, off_t offset) {
    if (ipin<0)                              
        return (0);                                  
    else if (ipin<32)                                 
        return(physBaseGPIO1+offset);                
    else if (ipin<64)                                 
        return(physBaseGPIO2+offset);                
    else if (ipin<96)                                 
        return(physBaseGPIO3+offset);                
    else if (ipin<128)                                
        return(physBaseGPIO4+offset);                
    else if (ipin<160)                                
        return(physBaseGPIO5+offset);                
    else if (ipin<192)                                
        return(physBaseGPIO6+offset);                
    else
        return(0);
}

// Returns physical address of GPIO Read Register for a given GPIO pin
off_t GPIOInterface::physGPIOReadLevel(const unsigned ipin) {
    return offset2adrGPIO(ipin,gpio_offset_datain); 
}

// Returns physical address of Output Enable Register for a given GPIO pin
off_t GPIOInterface::physGPIODirection(const unsigned ipin) {
    return offset2adrGPIO(ipin,gpio_offset_oe); 
}

// Returns physical address of Output Write Register for a given GPIO pin
off_t GPIOInterface::physGPIOSetLevel(const unsigned ipin) { 
    return offset2adrGPIO(ipin,gpio_offset_dataout);
}

// --------------------------------------------------------------------------
// static functions defining (physical) start of memory mapped regions
// --------------------------------------------------------------------------


volatile void* GPIOInterface::makeMap(volatile void*& virtual_addr, off_t physical_addr, size_t length) {
    virtual_addr = mmap(0, length, PROT_READ|PROT_WRITE, MAP_SHARED, m_mmap_fd, physical_addr);
    if (&virtual_addr==MMAPFAIL)
        exit(EXIT_FAILURE); 
    else
        return &virtual_addr;
}
