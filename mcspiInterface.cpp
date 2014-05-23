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

#include <mcspiInterface.hpp>

#define MMAPFAIL ((void*)-1)

#define MUNMAP(VIRT) \
    munmap(const_cast<void*>(VIRT), mapSize)


////////////////////////////////////////////////////////////////////////////////
// Public Members
////////////////////////////////////////////////////////////////////////////////

// constructor
mcspiInterface::mcspiInterface():
    m_mmap_fd(-1),
    m_mcspi1_base(),
    m_mcspi2_base(),
    m_mcspi3_base(),
    m_mcspi4_base()
{

    m_mmap_fd = open("/dev/mem", O_RDWR | O_SYNC);
    if(m_mmap_fd<0) {
        perror("open(\"/dev/mem\")");
        exit(EXIT_FAILURE);
    }

    //      virtual adr    physical adr
    makeMap(m_mcspi1_base, mapBaseMCSPI1);
    makeMap(m_mcspi2_base, mapBaseMCSPI2);
    makeMap(m_mcspi3_base, mapBaseMCSPI3);
    makeMap(m_mcspi4_base, mapBaseMCSPI4);
}

// destructor
mcspiInterface::~mcspiInterface()
{
    MUNMAP(m_mcspi1_base);
    MUNMAP(m_mcspi2_base);
    MUNMAP(m_mcspi3_base);
    MUNMAP(m_mcspi4_base);
}

uint32_t mcspiInterface::WriteRead(uint32_t data)
{
    printf("Not implemented yet..");
    return 0;
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
const int mcspiInterface::channel[] =  {0,1,2,3,0,1,0,1,0};

off_t mcspiInterface::offset2adrMCSPI(unsigned ispi, off_t offset)
{
    if (ispi<0)
        return (0);
    else if (ispi<4)
        return(physBaseMCSPI1+offset);
    else if (ispi<6)
        return(physBaseMCSPI2+offset);
    else if (ispi<8)
        return(physBaseMCSPI3+offset);
    else if (ispi==9)
        return(physBaseMCSPI4+offset);
    else
        return(0);
}

off_t mcspiInterface::physMCSPI_revision(const unsigned ispi)
{
    return offset2adrMCSPI(ispi,mcspi_offset_revision);
}

off_t mcspiInterface::physMCSPI_sysconfig(const unsigned ispi)
{
    return offset2adrMCSPI(ispi,mcspi_offset_sysconfig);
}

off_t mcspiInterface::physMCSPI_sysstatus(const unsigned ispi)
{
    return offset2adrMCSPI(ispi,mcspi_offset_sysstatus);
}

off_t mcspiInterface::physMCSPI_irqstatus(const unsigned ispi)
{
    return offset2adrMCSPI(ispi,mcspi_offset_irqstatus);
}

off_t mcspiInterface::physMCSPI_irqenable(const unsigned ispi)
{
    return offset2adrMCSPI(ispi,mcspi_offset_irqenable);
}

off_t mcspiInterface::physMCSPI_wakeupenable(const unsigned ispi)
{
    return offset2adrMCSPI(ispi,mcspi_offset_wakeupenable);
}

off_t mcspiInterface::physMCSPI_syst(const unsigned ispi)
{
    return offset2adrMCSPI(ispi,mcspi_offset_syst);
}

off_t mcspiInterface::physMCSPI_tx(const unsigned ispi)
{
    return offset2adrMCSPI(ispi,mcspi_offset_syst);
}

off_t mcspiInterface::physMCSPI_rx(const unsigned ispi)
{
    return offset2adrMCSPI(ispi,mcspi_offset_syst);
}

off_t mcspiInterface::physMCSPI_modulctrl(const unsigned ispi)
{
    return offset2adrMCSPI(ispi,mcspi_offset_modulctrl);
}

off_t mcspiInterface::physMCSPI_chconf(const unsigned ispi)
{
    return offset2adrMCSPI(ispi,mcspi_offset_chconf_base+0x14*channel[ispi]);
}

off_t mcspiInterface::physMCSPI_chstat(const unsigned ispi)
{
    return offset2adrMCSPI(ispi,mcspi_offset_chstat_base+0x14*channel[ispi]);
}

off_t mcspiInterface::physMCSPI_chctrl(const unsigned ispi)
{
    return offset2adrMCSPI(ispi,mcspi_offset_chctrl_base+0x14*channel[ispi]);
}

off_t mcspiInterface::physMCSPI_read(const unsigned ispi)
{
    return offset2adrMCSPI(ispi,mcspi_offset_rx_base+0x14*channel[ispi]);
}

off_t mcspiInterface::physMCSPI_write(const unsigned ispi)
{
    return offset2adrMCSPI(ispi,mcspi_offset_tx_base+0x14*channel[ispi]);
}

// static functions defining (physical) start of memory mapped regions
// --------------------------------------------------------------------------
// Functions to return pointers to mapped SSP registers
// --------------------------------------------------------------------------

volatile uint32_t* mcspiInterface::phys2VirtMCSPI32(off_t adr_physical, const unsigned ispi)
{
    if (ispi<4)                     // Module 1 (Channels 0-3)
        return phys2Virt32(adr_physical,m_mcspi1_base,mapBaseMCSPI1);
    else if (ispi<6)                // Module 2 (Channels 0,1,2)
        return phys2Virt32(adr_physical,m_mcspi2_base,mapBaseMCSPI2);
    else if (ispi<8)                // Module 3 (Channel 0,1)
        return phys2Virt32(adr_physical,m_mcspi3_base,mapBaseMCSPI3);
    else if (ispi==8)               // Module 4 (Channel 0)
        return phys2Virt32(adr_physical,m_mcspi4_base,mapBaseMCSPI4);
    else
        return 0;
}

// Takes Physical Address and Returns pointer to memory mapped virtual address
volatile uint32_t* mcspiInterface::phys2Virt32(off_t phys, volatile void* map_base_virt, off_t map_base_phys)
{
    static off_t map_offset;
    static volatile uint32_t* adr_virtual;

    map_offset = phys - map_base_phys;
    //printf("\nmap_offset: %02X", map_offset);
    adr_virtual = reinterpret_cast<volatile uint32_t*>(static_cast<volatile uint8_t*>(map_base_virt) + map_offset);
    //printf("\nadr_virtual: %04X", adr_virtual);

    return adr_virtual;
}

volatile uint32_t* mcspiInterface::ptrMCSPI_sysconfig(const unsigned ispi)
{
    return phys2VirtMCSPI32(physMCSPI_sysconfig(ispi),ispi);
}

volatile uint32_t* mcspiInterface::ptrMCSPI_wakeupenable(const unsigned ispi)
{
    return phys2VirtMCSPI32(physMCSPI_wakeupenable(ispi),ispi);
}

volatile uint32_t* mcspiInterface::ptrMCSPI_modulctrl(const unsigned ispi)
{
    return phys2VirtMCSPI32(physMCSPI_modulctrl(ispi),ispi);
}

volatile uint32_t* mcspiInterface::ptrMCSPI_chconf(const unsigned ispi)
{
    uint32_t  adr_physical = physMCSPI_chconf(ispi);
    volatile uint32_t* adr_virtual  = phys2VirtMCSPI32(adr_physical,ispi);
    return adr_virtual;
}

volatile uint32_t* mcspiInterface::ptrMCSPI_chstat(const unsigned ispi)
{
    return phys2VirtMCSPI32(physMCSPI_chstat(ispi),ispi);
}

volatile uint32_t* mcspiInterface::ptrMCSPI_chctrl(const unsigned ispi)
{
    return phys2VirtMCSPI32(physMCSPI_chctrl(ispi),ispi);
}

volatile uint32_t* mcspiInterface::ptrMCSPI_tx(const unsigned ispi)
{
    return phys2VirtMCSPI32(physMCSPI_tx(ispi),ispi);
}

volatile uint32_t* mcspiInterface::ptrMCSPI_rx(const unsigned ispi)
{
    return phys2VirtMCSPI32(physMCSPI_rx(ispi),ispi);
}

volatile uint32_t* mcspiInterface::ptrMCSPI_irqstatus(const unsigned ispi)
{
    return phys2VirtMCSPI32(physMCSPI_irqstatus(ispi),ispi);
}


volatile void* mcspiInterface::makeMap(volatile void*& virtual_addr, off_t physical_addr, size_t length)
{
    virtual_addr = mmap(0, length, PROT_READ|PROT_WRITE, MAP_SHARED, m_mmap_fd, physical_addr);
    if (&virtual_addr==MMAPFAIL)
        exit(EXIT_FAILURE);
    else
        return &virtual_addr;
}
