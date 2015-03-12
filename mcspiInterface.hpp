#ifndef MCSPIINTERFACE_H
#define MCSPIINTERFACE_H

#include <sys/types.h>
#include <stdint.h>
#include <sys/mman.h>
#include <Layout.hpp>

class mcspiInterface
{
public:
    mcspiInterface();
    ~mcspiInterface();

    uint32_t WriteRead(uint32_t data);
    void Configure();



private:

    void EnableChannel  ();
    void DisableChannel ();
    void EnableClocks   ();
    void DisableClocks  ();
    void SetMasterMode  ();
    void Reset();

    // SPI1 supports up to four peripherals, SPI2 and SPI3 support up to two
    // peripherals, and SPI4 supports only one peripheral.
    // channel address = base address + 0x14 * x
    // x= 0 to 3 for MCSPI1.
    // x= 0 to 1 for MCSPI2 and MCSPI3.
    // x= 0      for MCSPI4.
    // in software let's enumerate these lines: 0,1,2,3,4,5,6,7,8
    static const int channel[9];

    // --------------------------------------------------------------------------
    // GPIO register (PHYSICAL) Address Definitions
    // --------------------------------------------------------------------------

    const off_t physBaseMCSPI1             = 0x48098000;

    const off_t mcspi_offset_revision      =  0x00;
    const off_t mcspi_offset_sysconfig     =  0x10;
    const off_t mcspi_offset_sysstatus     =  0x14;
    const off_t mcspi_offset_irqstatus     =  0x18;
    const off_t mcspi_offset_irqenable     =  0x1C;
    const off_t mcspi_offset_wakeupenable  =  0x20;
    const off_t mcspi_offset_syst          =  0x24;
    const off_t mcspi_offset_modulctrl     =  0x28;

    const off_t mcspi_offset_chconf=  0x2C;
    const off_t mcspi_offset_chstat=  0x30;
    const off_t mcspi_offset_chctrl=  0x34;
    const off_t mcspi_offset_tx=  0x38;
    const off_t mcspi_offset_rx=  0x3C;

    const off_t mcspi_offset_xferlevel     =  0x7C;
    //
    // Clock control
    // Bit 18 = EN_MCSPI1
    // Bit 19 = EN_MCSPI2
    // Bit 20 = EN_MCSPI3
    // Bit 21 = EN_MCSPI4
    // 0x1 = enable, 0x0 = disable

    const off_t physBaseCLKCTRL     = 0x48004A00;
    // Module interface clock control
    const off_t physCM_ICLKEN1_CORE = 0x48004A10;
    // Module functional clock control
    const off_t physCM_FCLKEN1_CORE = 0x48004A00;

    const off_t physBasePadConf     = 0x48002030;
    const off_t physMCSPIPadConf    = 0x48004A00;

    // --------------------------------------------------------------------------
    // Functions to return pointers to mapped SSP registers
    // --------------------------------------------------------------------------

    volatile uint32_t* phys2VirtMCSPI32      ( off_t adr_physical);
    volatile uint32_t* phys2Virt32           ( off_t phys, volatile void* map_base_virt, off_t map_base_phys);


    volatile uint32_t* ptrMCSPI_sysconfig    ();
    volatile uint32_t* ptrMCSPI_sysstatus    ();
    volatile uint32_t* ptrMCSPI_wakeupenable ();
    volatile uint32_t* ptrMCSPI_modulctrl    ();
    volatile uint32_t* ptrMCSPI_chconf       ();
    volatile uint32_t* ptrMCSPI_chstat       ();
    volatile uint32_t* ptrMCSPI_chctrl       ();
    volatile uint32_t* ptrMCSPI_tx           ();
    volatile uint32_t* ptrMCSPI_rx           ();
    volatile uint32_t* ptrMCSPI_irqstatus    ();
    volatile uint32_t* ptrMCSPI_irqenable    ();
    volatile uint32_t* ptrMCSPI_ICLKEN       ();
    volatile uint32_t* ptrMCSPI_FCLKEN       ();
    volatile uint32_t* ptrMCSPIPadConf       ();

    // --------------------------------------------------------------------------

    // Create a virtual addressing space for a given physical address
    volatile void*  makeMap(volatile void*& virtual_addr, off_t physical_addr, size_t length=4096);

    // Memory mapped file descriptor
    int             m_mmap_fd;

    // Pointer to base of memory mapped address space
    volatile void*  m_iclk;
    volatile void*  m_fclk;
    volatile void*  m_padconf;
    volatile void*  m_mcspi1_base;
    volatile void*  m_mcspi2_base;
    volatile void*  m_mcspi3_base;
    volatile void*  m_mcspi4_base;

    // utilities
    bool txFifoFull ();
};
#endif
