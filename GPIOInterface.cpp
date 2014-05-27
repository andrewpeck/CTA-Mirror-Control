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
    munmap(const_cast<void*>(VIRT), 4096)

GPIOInterface::GPIOInterface():
    m_mmap_fd(-1),
    m_gpio1_base(),
    m_gpio2_base(),
    m_gpio3_base(),
    m_gpio4_base(),
    m_gpio5_base(),
    m_gpio6_base()
{

    m_mmap_fd = open("/dev/mem", O_RDWR | O_SYNC);
    if(m_mmap_fd<0)
    {
        perror("open(\"/dev/mem\")");
        exit(EXIT_FAILURE);
    }

    //      virtual adr   physical adr
    makeMap(m_gpio1_base, physBaseGPIO1);
    makeMap(m_gpio2_base, physBaseGPIO2);
    makeMap(m_gpio3_base, physBaseGPIO3);
    makeMap(m_gpio4_base, physBaseGPIO4);
    makeMap(m_gpio5_base, physBaseGPIO5);
    makeMap(m_gpio6_base, physBaseGPIO6);
}

GPIOInterface::~GPIOInterface()
{
    // Unmap everything 
    MUNMAP(m_gpio1_base);
    MUNMAP(m_gpio2_base);
    MUNMAP(m_gpio3_base);
    MUNMAP(m_gpio4_base);
    MUNMAP(m_gpio5_base);
    MUNMAP(m_gpio6_base);
}

volatile uint32_t* GPIOInterface::ptrGPIODirection(const unsigned ipin)
{
    return phys2VirtGPIO32(physGPIODirection(ipin),ipin);
}

volatile uint32_t* GPIOInterface::ptrGPIOSetLevel(const unsigned ipin)
{
    return phys2VirtGPIO32(physGPIOSetLevel(ipin),ipin);
}

volatile uint32_t* GPIOInterface::ptrGPIOReadLevel(const unsigned ipin)
{
    return phys2VirtGPIO32(physGPIOReadLevel(ipin),ipin);
}

void GPIOInterface::gpioSetLevel(const unsigned ipin)
{
    // Write a One to the bit specified by ipin
    *(ptrGPIOSetLevel(ipin)) = *(ptrGPIOReadLevel(ipin)) | 0x1 << ipin;
}

void GPIOInterface::gpioClrLevel(const unsigned ipin)
{
    // Write a zero to the bit specified by ipin
    *(ptrGPIOSetLevel(ipin)) = *(ptrGPIOReadLevel(ipin)) & ~(0x1 << ipin);
}

volatile uint32_t* GPIOInterface::phys2VirtGPIO32(off_t phys, const unsigned ipin)
{
    if (ipin<32)
        return phys2Virt32(phys,m_gpio1_base,physBaseGPIO1);
    else if (ipin<64)
        return phys2Virt32(phys,m_gpio2_base,physBaseGPIO2);
    else if (ipin<96)
        return phys2Virt32(phys,m_gpio3_base,physBaseGPIO3);
    else if (ipin<128)
        return phys2Virt32(phys,m_gpio4_base,physBaseGPIO4);
    else if (ipin<160)
        return phys2Virt32(phys,m_gpio5_base,physBaseGPIO5);
    else if (ipin<192)
        return phys2Virt32(phys,m_gpio6_base,physBaseGPIO6);
    else
        return 0;
}

volatile uint32_t* GPIOInterface::phys2Virt32(off_t phys, volatile void* map_base_virt, off_t map_base_phys)
{
    static off_t map_offset;
    static volatile uint32_t* adr_virtual;

    // map offset is the difference between the physical address and the physical base address
    map_offset = phys - map_base_phys;

    // virtual address the base of the virtual address + the map_offset with some fancy typecasting done for poor reasons
    adr_virtual = reinterpret_cast<volatile uint32_t*>(static_cast<volatile uint8_t*>(map_base_virt) + map_offset);

    return adr_virtual;
}

off_t GPIOInterface::offset2adrGPIO(unsigned ipin, off_t offset)
{
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

off_t GPIOInterface::physGPIOReadLevel(const unsigned ipin)
{
    return offset2adrGPIO(ipin,gpio_offset_datain);
}

off_t GPIOInterface::physGPIODirection(const unsigned ipin)
{
    return offset2adrGPIO(ipin,gpio_offset_oe);
}

off_t GPIOInterface::physGPIOSetLevel(const unsigned ipin)
{
    return offset2adrGPIO(ipin,gpio_offset_dataout);
}

volatile void* GPIOInterface::makeMap(volatile void*& virtual_addr, off_t physical_addr, size_t length)
{
    // Create a virtual addressing space for a given physical address
    virtual_addr = mmap(0, length, PROT_READ|PROT_WRITE, MAP_SHARED, m_mmap_fd, physical_addr);

    // Error Handling
    if (&virtual_addr==MMAPFAIL)
        exit(EXIT_FAILURE);
    else
        return &virtual_addr;
}
