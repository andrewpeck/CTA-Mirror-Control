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

    // Functions to return pointers to mapped GPIO registers
    uint32_t WriteRead(uint32_t data);
    void Configure(); 

private:
    void Initialize(); 

    static const int channel[];

    // --------------------------------------------------------------------------
    // GPIO register (PHYSICAL) Address Definitions
    // --------------------------------------------------------------------------

    static const off_t physBaseMCSPI1             = 0x48098000;
    static const off_t physBaseMCSPI2             = 0x4809A000;
    static const off_t physBaseMCSPI3             = 0x480B8000;
    static const off_t physBaseMCSPI4             = 0x480BA000;


    static const off_t mcspi_offset_revision      =  0x00;
    static const off_t mcspi_offset_sysconfig     =  0x10;
    static const off_t mcspi_offset_sysstatus     =  0x14;
    static const off_t mcspi_offset_irqstatus     =  0x18;
    static const off_t mcspi_offset_irqenable     =  0x1C;
    static const off_t mcspi_offset_wakeupenable  =  0x20;
    static const off_t mcspi_offset_syst          =  0x24;
    static const off_t mcspi_offset_modulctrl     =  0x28;

    static const off_t mcspi_offset_chconf_base   =  0x2C;
    static const off_t mcspi_offset_chstat_base   =  0x30;
    static const off_t mcspi_offset_chctrl_base   =  0x34;
    static const off_t mcspi_offset_tx_base       =  0x38;
    static const off_t mcspi_offset_rx_base       =  0x3C;

    static const off_t mcspi_offset_xferlevel     =  0x7C;

    off_t physMCSPI_revision     ( const unsigned ispi ) ;
    off_t physMCSPI_sysconfig    ( const unsigned ispi ) ;
    off_t physMCSPI_sysstatus    ( const unsigned ispi ) ;
    off_t physMCSPI_irqstatus    ( const unsigned ispi ) ;
    off_t physMCSPI_irqenable    ( const unsigned ispi ) ;
    off_t physMCSPI_wakeupenable ( const unsigned ispi ) ;
    off_t physMCSPI_syst         ( const unsigned ispi ) ;
    off_t physMCSPI_tx           ( const unsigned ispi ) ;
    off_t physMCSPI_rx           ( const unsigned ispi ) ;
    off_t physMCSPI_modulctrl    ( const unsigned ispi ) ;
    off_t physMCSPI_chconf       ( const unsigned ispi ) ;
    off_t physMCSPI_chstat       ( const unsigned ispi ) ;
    off_t physMCSPI_chctrl       ( const unsigned ispi ) ;
    off_t physMCSPI_read         ( const unsigned ispi ) ;
    off_t physMCSPI_write        ( const unsigned ispi ) ;

    // Clock control 
    // Bit 18 = EN_MCSPI1
    // Bit 19 = EN_MCSPI2
    // Bit 20 = EN_MCSPI3
    // Bit 21 = EN_MCSPI4
    // 0x1 = enable, 0x0 = disable
    
    static const off_t physBaseCLKCTRL     = 0x48004A00; 
    // Module interface clock control
    static const off_t physCM_ICLKEN1_CORE = 0x48004A10; 
    // Module functional clock control
    static const off_t physCM_FCLKEN1_CORE = 0x48004A00; 

    static const off_t physBasePadConf     = 0x48002030; 
    static const off_t physMCSPIPadConf    = 0x48004A00; 

    // --------------------------------------------------------------------------
    // Functions to return pointers to mapped SSP registers
    // --------------------------------------------------------------------------

    off_t offset2adrMCSPI(unsigned ispi, off_t offset);
    volatile uint32_t* phys2VirtMCSPI32      ( off_t adr_physical, const unsigned ispi);
    volatile uint32_t* phys2Virt32           ( off_t phys, volatile void* map_base_virt, off_t map_base_phys);
    volatile uint32_t* ptrMCSPI_sysconfig    ( const unsigned ispi);
    volatile uint32_t* ptrMCSPI_sysstatus    ( const unsigned ispi);
    volatile uint32_t* ptrMCSPI_wakeupenable ( const unsigned ispi);
    volatile uint32_t* ptrMCSPI_modulctrl    ( const unsigned ispi);
    volatile uint32_t* ptrMCSPI_chconf       ( const unsigned ispi);
    volatile uint32_t* ptrMCSPI_chstat       ( const unsigned ispi);
    volatile uint32_t* ptrMCSPI_chctrl       ( const unsigned ispi);
    volatile uint32_t* ptrMCSPI_tx           ( const unsigned ispi);
    volatile uint32_t* ptrMCSPI_rx           ( const unsigned ispi);
    volatile uint32_t* ptrMCSPI_irqstatus    ( const unsigned ispi);
    volatile uint32_t* ptrMCSPI_irqenable    ( const unsigned ispi);
    volatile uint32_t* ptrMCSPI_ICLKEN       (); 
    volatile uint32_t* ptrMCSPI_FCLKEN       (); 
    volatile uint32_t* ptrMCSPIPadConf       (); 

    // --------------------------------------------------------------------------
    // 
    // --------------------------------------------------------------------------

    // Create a virtual addressing space for a given physical address
    volatile void*  makeMap(volatile void*& virtual_addr, off_t physical_addr, size_t length=4096);

    // Memory mapped file descriptor
    int             m_mmap_fd;

    // Base addresses of memory mapped address space
    volatile void*  m_mcspi1_base;
    volatile void*  m_mcspi2_base;
    volatile void*  m_mcspi3_base;
    volatile void*  m_mcspi4_base;
    volatile void*  m_iclk; 
    volatile void*  m_fclk; 
    volatile void*  m_padconf; 

    // utilities 
    bool txFifoFull (const unsigned ispi); 

    Layout layout; 
};
#endif
