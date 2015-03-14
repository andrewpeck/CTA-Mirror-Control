#ifndef MCSPIINTERFACE_H
#define MCSPIINTERFACE_H

#include <sys/types.h>
#include <stdint.h>
#include <sys/mman.h>
#include "Layout.hpp"

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

    const off_t ADR_MCSPI_BASE          = 0x48098000; // this is right
    //const off_t ADR_MCSPI_BASE           = 0x4809A000; // this is NOT right!

    const off_t OFF_MCSPI_REVISION      = 0x000;
    const off_t OFF_MCSPI_SYSCONFIG     = 0x010;
    const off_t OFF_MCSPI_SYSSTATUS     = 0x014;
    const off_t OFF_MCSPI_IRQSTATUS     = 0x018; // this IS right
    //const off_t OFF_MCSPI_IRQSTATUS     = 0x000; // this is NOT right
    //const off_t OFF_MCSPI_IRQSTATUS     = 0x030; // this is NOT right
    const off_t OFF_MCSPI_IRQENABLE     = 0x01C;
    const off_t OFF_MCSPI_WAKEUPENABLE  = 0x020;
    const off_t OFF_MCSPI_SYST          = 0x024;
    const off_t OFF_MCSPI_MODULCTRL     = 0x028;

    const off_t OFF_MCSPI_CHCONF        = 0x02C;
    const off_t OFF_MCSPI_CHSTAT        = 0x030;
    const off_t OFF_MCSPI_CHCTRL        = 0x034;
    const off_t OFF_MCSPI_TX            = 0x038;
    const off_t OFF_MCSPI_RX            = 0x03C;
    const off_t OFF_MCSPI_XFERLEVEL     = 0x07C;

    // Clock control
    // Bit 18 = EN_MCSPI1
    // Bit 19 = EN_MCSPI2
    // Bit 20 = EN_MCSPI3
    // Bit 21 = EN_MCSPI4
    // 0x1 = enable, 0x0 = disable

    const off_t ADR_CM_CORE_BASE    = 0x48004A00; //size = 8192 bytes // REAL
    const off_t OFF_CM_FCLKEN1_CORE = 0x00; // Module functional clock control
    const off_t OFF_CM_ICLKEN1_CORE = 0x10; // Module interface clock control

    const off_t ADR_PADCONF_BASE     = 0x48002030;
    //const off_t physMCSPIPadConf    = 0x48004A00;

    // --------------------------------------------------------------------------
    // Pointers to mapped registers
    // --------------------------------------------------------------------------

    volatile uint32_t* cm_fclken1_core    ;
    volatile uint32_t* cm_iclken1_core    ;

    volatile uint32_t* mcspi_sysconfig    ;
    volatile uint32_t* mcspi_sysstatus    ;
    volatile uint32_t* mcspi_wakeupenable ;
    volatile uint32_t* mcspi_modulctrl    ;
    volatile uint32_t* mcspi_chconf       ;
    volatile uint32_t* mcspi_chstat       ;
    volatile uint32_t* mcspi_chctrl       ;
    volatile uint32_t* mcspi_tx           ;
    volatile uint32_t* mcspi_rx           ;
    volatile uint32_t* mcspi_irqstatus    ;
    volatile uint32_t* mcspi_irqenable    ;

    //volatile uint32_t* ptrMCSPIPadConf       ();

    // --------------------------------------------------------------------------

    // Create a virtual addressing space for a given physical address
    void* makeMap(off_t target);
    volatile uint32_t* mappedAddress(off_t offset, void* mappedBaseAddress);

    // Memory mapped file descriptor
    int             m_mmap_fd;

    // Pointer to base of memory mapped address space
    void*  m_cm_core_base;
    //volatile void*  m_padconf;
    void*  m_mcspi1_base;

    // utilities
    //bool txFifoFull ();
};
#endif
