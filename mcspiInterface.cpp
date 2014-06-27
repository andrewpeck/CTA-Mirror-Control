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
    munmap(const_cast<void*>(VIRT), 4096)
#define MAP_SIZE 4096UL
#define MAP_MASK (MAP_SIZE - 1)
#define ISPI 0


////////////////////////////////////////////////////////////////////////////////
// Public Members
////////////////////////////////////////////////////////////////////////////////

// constructor
mcspiInterface::mcspiInterface()
    //m_mmap_fd(-1),
    //m_iclk(), 
    //m_fclk(), 
    //m_padconf(), 
    //m_mcspi1_base(),
    //m_mcspi2_base(),
    //m_mcspi3_base(),
    //m_mcspi4_base()
{

    m_mmap_fd = open("/dev/mem", O_RDWR | O_SYNC);
    if(m_mmap_fd<0) {
        perror("open(\"/dev/mem\")");
        exit(EXIT_FAILURE);
    }

    //      virtual adr    physical adr
    makeMap(m_iclk,         0x48004A00);
    makeMap(m_fclk,         0x48004A10);
    makeMap(m_padconf,      0x480021C8);
    makeMap(m_mcspi1_base,  physBaseMCSPI1);
    makeMap(m_mcspi2_base,  physBaseMCSPI2);
    makeMap(m_mcspi3_base,  physBaseMCSPI3);
    makeMap(m_mcspi4_base,  physBaseMCSPI4);
}

// destructor
mcspiInterface::~mcspiInterface()
{
    MUNMAP(m_iclk);
    MUNMAP(m_fclk);
    MUNMAP(m_padconf);
    MUNMAP(m_mcspi1_base);
    MUNMAP(m_mcspi2_base);
    MUNMAP(m_mcspi3_base);
    MUNMAP(m_mcspi4_base);
}

void mcspiInterface::Initialize()
{
    // Clock Enable
    // Bit 18 = EN_MCSPI1
    // Bit 19 = EN_MCSPI2
    // Bit 20 = EN_MCSPI3
    // Bit 21 = EN_MCSPI4
    // 0x1 = enable, 0x0 = disable
    
    // interface clock
    //printf("\nEnabling Interface Clock"); 
    *ptrMCSPI_ICLKEN() = *ptrMCSPI_ICLKEN() | (0x1 << 18); 

    // functional clock
    //printf("\nEnabling Functional Clock"); 
    *ptrMCSPI_FCLKEN() = *ptrMCSPI_FCLKEN() | (0x1 << 18); 

    // 1) software reset 
    // mcspi_sysconfig[1] high activates software reset
    //printf("\nSoftware reset"); 
    *ptrMCSPI_sysconfig(ISPI) |= 0x1 << 1; 


    // SPIm.MCSPI_SYSSTATUS[0] will be set to 1 when the reset is finished
    // wait until it is 1..
    while (true)
    {
        if ( (*ptrMCSPI_sysstatus(ISPI) & 0x1)==1 ) 
            break; 
    }
}

void mcspiInterface::Configure()
{
    //printf("\nReset finished..");

    // reset status bits
    // MCSPI_IRQSTATUS
    // enable interrupts
    // MCSPI_IRQENABLE
    
    // 3) Write MCSPI_MODULCTRL
    //    Write MCSPI_SYSCONFIG
    
    //enable channel 1
    //printf("\nEnable Channel 1"); 
    //*ptrMCSPI_chctrl(0) |= 0x1; 
    
    // get current config
    uint32_t config = *ptrMCSPI_chconf(ISPI); 
    //printf("\nInitial Config = 0x%08X", config); 

    //set polarity and phase
    //mcspi_chxconf[0] phase
    //mcspi_chxconf[1] polarity
    //mode 1 --> polarity=0 phase=1
    int phase    = 1; 
    int polarity = 0; 

    config &= ~(0x1 << 0); 
    config |=  (phase << 0);        //mcspi_chxconf[0]

    config &= ~(0x1 << 1); 
    config |=  (polarity << 1);     //mcspi_chxconf[1]
    
    // set clock rate in MCSPI_CHXCONF[5..2]
    // Divider Clock Rate
    //   1       48      MHz
    //   2       24      MHz
    //   4       12      MHz
    //   8       6       MHz
    //   16      3       MHz
    //   32      1.5     MHz
    //   64      750     kHz
    //   128     375     kHz
    //   256     ~187    kHz
    //   512     ~93.7   kHz
    //   1024    ~46.8   kHz
    //   2048    ~23.4   kHz
    //   4096    ~11.7   kHz
    //   8192    ~5.8    kHz
    //   16384   ~2.9    kHz
    //   32768   ~1.5    kHz
    int clock_divider = 0x4;      //16
    config &= ~(0xF<<2);          //mcspi_chxconf[5..2]
    config |= clock_divider<<2;   //mcspi_chxconf[5..2]

    //spim_csx polarity
    //0x0 => spim_csx high during active state
    //0x1 => spim_csx low  during active state
    int spim_polarity = 0x1; 
    config &= ~(0x1 << 6); 
    config |=  (spim_polarity << 6); //mcspi_chxconf[6]

    //set spi word length
    int spi_word_length = 0xF;  //16 bit word length
    config &= ~(0x1F << 7); 
    config |=  (spi_word_length << 7); //mcspi_chxconf[11..7]

    //Transmit/receive modes RW 0x0
    //    0x0: Transmit and receive mode
    //    0x1: Receive-only mode
    //    0x2: Transmit-only mode
    //    0x3: Reserved
    int transfer_mode = 0x0;
    config &= ~(0x7 << 12); 
    config |=  (transfer_mode << 12); //mcspi_chxconf[13..12]
    
    // Set MCSPI_CHXCONF[18] to 0 for spi1_somi pin in receive mode
    config &= ~(0x1 << 18); 

    config &= ~(0x1 << 17); 
    config |=  (0x1 << 16); 

    //Set the SPI1.MCSPI_MODULCTRL[2] MS bit to 0 to provide the clock.
    *ptrMCSPI_modulctrl(ISPI) &= ~(0x1 << 2) ; 

    *ptrMCSPI_modulctrl(ISPI) |= 0x1; 

    //int tx_fifo_enable = 0x1; 
    //config |= ( << 27); 

    *ptrMCSPI_chconf(ISPI) = config; 

    //printf("\nWrite Config = 0x%08X", config); 
    config = *ptrMCSPI_chconf(0); 
    //printf("\nFinal Config  = 0x%08X", config); 
    //printf("\n"); 

    //disable channel 1
    *ptrMCSPI_chctrl(ISPI) &= ~0x1; 
}

uint32_t mcspiInterface::WriteRead(uint32_t data)
{

    ////9. Set the INPUTENABLE bit of the corresponding CONTROL_PADCONF_x register to achieve the
    ////synchronous clock by using loopback through the output and input buffers of the pad.
    //page2455

    //

    ////Write Operation
    ////1. Write 1 to the SPI1.MCSPI_IRQSTATUS[0] TX0_EMPTY bit to reset the status.
    //*ptrMCSPI_irqstatus(ISPI) |= 0x00000001; 
    //while (true)
    //{
    //    //2. Write the command/address or data value in the SPI1.MCSPI_TX0 register to transmit the value.
    //    //3. If the SPI1.MCSPI_IRQSTATUS[0] TX0_EMPTY bit is set to 1, write 1 to it and return to Step 2
    //    else
    //        break; 
    //}

    ////(polling method).
    ////20.6.2.6.2.1.3 Read Operation
    ////1. Read the SPI1.MCSPI_IRQSTATUS[2] RX0_FULL bit and if it is set to 1, go to Step 2.
    ////2. If the SPI1.MCSPI_IRQSTATUS[2] RX0_FULL bit is set to 1, write 1 to it and return to Step 1 (polling
    ////method).
    ////NOTE:
    ////see page 3020



    //20.6.2.6.3 Programming in Interrupt Mode
    //This section follows the flow of Figure 20-26.
    //1. Initialize software variables: WRITE_COUNT = 0 and READ_COUNT = 0.
    int write_count=0; 
    int read_count=0; 
    uint32_t read = 0; 

    Initialize(); 

    //2. Initialize interrupts: Write 0x7 in the SPI1.MCSPI_IRQSTATUS[3:0]
    //field and set the SPI1.MCSPI_IRQENABLE[3:0] field to 0x7.
    *ptrMCSPI_irqenable(ISPI) &= ~0x7; 
    *ptrMCSPI_irqenable(ISPI) |=  0x7; 

    *ptrMCSPI_irqstatus(ISPI) &= ~0x7; 
    *ptrMCSPI_irqstatus(ISPI) |=  0x7; 

    //3. Follow the steps described in Section 20.6.2.6.2.1.1, Mode Selection.
        //The SPI1.MCSPI_CHxCONF register (with x = 0) allows configuration of the operating mode:
        //1. Set the SPI1.MCSPI_CHxCONF[18] IS bit to 0 for the spi1_somi pin in receive mode.
        //2. Set the SPI1.MCSPI_CHxCONF[17] DPE1 bit to 0 and the SPI1.MCSPI_CHxCONF[16] DPE0 bit to 1
        //for the spi1.simo pin in transmit mode.
        //3. Set the SPI1.MCSPI_CHxCONF[13:12] TRM field to 0x0 for transmit and receive mode.
        //4. Write 0x8 in the SPI1.MCSPI_CHxCONF[11:7] WL field for 9-bit word length.
        //5. Set the SPI1.MCSPI_CHxCONF[6] EPOL bit to 1 for spi1_cs0 activated low during active state.
        //6. Set the SPI1.MCSPI_CHxCONF[1] POL bit to 0 for spi1_clk held high during active state.
        //7. Set the SPI1.MCSPI_CHxCONF[0] PHA bit to 0 for data latched on odd-numbered edges of the SPI
        //clock.
        //
        //Clock Initialization and spi1_cs0 Enable
        //In master mode, the SPI must provide the clock and enable the channel:
        //8. Set the SPI1.MCSPI_MODULCTRL[2] MS bit to 0 to provide the clock.

        //10. Set the SPI1.MCSPI_CHxCTRL[0] EN bit to 1 (where x = 0) to enable channel 0.
    Configure(); 


        //9. Set the INPUTENABLE bit of the corresponding CONTROL_PADCONF_x register to achieve the
        //synchronous clock by using loopback through the output and input buffers of the pad.
        //CONTROL_PADCONF_MCSPI1_CLK[15:0]
        //0x4800 21C8
        //Section 13.4.4
        //Bit 8 == Input Enable
    //*ptrMCSPIPadConf() |= 0x1 << 8; 

    // Start Channel
    *ptrMCSPI_chctrl(ISPI) |= 0x1; 


    //4. If WRITE_COUNT = w and READ_COUNT = w, write SPI1.MCSPI_CHxCTRL[0] = 0x0 (x = 0) to stop
    //the channel.
    //This interrupt routine follows the flow of Table 20-16 and Figure 20-28.
    //1. Read the SPI1.MCSPI_IRQSTATUS[3:0] field.
    //2. If the SPI1.MCSPI_IRQSTATUS[0] TX0_EMPTY bit is set to 1:
    if ((*ptrMCSPI_irqstatus(ISPI) & 0x1) == 1)
    {
        //(a) Write the command/address or data value in SPI1.MCSPI_TXx (where x = 0).
        *ptrMCSPI_tx(ISPI) = data; 
        //(b) WRITE_COUNT + = 1
        write_count++; 
        //(c) Write SPI1.MCSPI_IRQSTATUS[0] = 0x1.
        *ptrMCSPI_irqstatus(ISPI) |= 0x1; 
    }

    //3. If the SPI1.MCSPI_IRQSTATUS[2] RX0_FULL bit is set to 1:
    if (((*ptrMCSPI_irqstatus(ISPI) >> 2) & 0x1) == 1)
    {
        //(a) Read SPI1.MCSPI_RXx (where x = 0)
        read = *ptrMCSPI_rx(ISPI); 
        //(b) READ_COUNT += 1
        read_count++; 
        //(c) Write SPI1.MCSPI_IRQSTATUS[2] = 0x1
        *ptrMCSPI_irqstatus(ISPI) |= 0x1 << 2; 
    }
    return read; 

    // Stop Channel
    //*ptrMCSPI_chctrl(ISPI) &= ~0x1; 

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

volatile uint32_t* mcspiInterface::phys2VirtMCSPI32(off_t adr_physical, const unsigned ispi)
{
    if (ispi<4)                     // Module 1 (Channels 0-3)
        return phys2Virt32(adr_physical,m_mcspi1_base,physBaseMCSPI1);
    else if (ispi<6)                // Module 2 (Channels 0,1,2)
        return phys2Virt32(adr_physical,m_mcspi2_base,physBaseMCSPI2);
    else if (ispi<8)                // Module 3 (Channel 0,1)
        return phys2Virt32(adr_physical,m_mcspi3_base,physBaseMCSPI3);
    else if (ispi==8)               // Module 4 (Channel 0)
        return phys2Virt32(adr_physical,m_mcspi4_base,physBaseMCSPI4);
    else
        return 0;
}

// Takes Physical Address and Returns pointer to memory mapped virtual address
volatile uint32_t* mcspiInterface::phys2Virt32(off_t phys, volatile void* map_base_virt, off_t map_base_phys)
{
    static off_t map_offset;
    static volatile uint32_t* adr_virtual;

    // map offset is the difference between the physical address and the physical base address
    map_offset = phys - map_base_phys;

    // virtual address the base of the virtual address + the map_offset with some fancy typecasting done for possibly poor reasons
    adr_virtual = reinterpret_cast<volatile uint32_t*>(static_cast<volatile uint8_t*>(map_base_virt) + map_offset);


    //printf("\nmap_offset:  %08X", map_offset); 
    //printf("\nadr_virtual: %08X", adr_virtual); 

    return adr_virtual;
}


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

volatile uint32_t* mcspiInterface::ptrMCSPI_sysconfig(const unsigned ispi)
{
    return phys2VirtMCSPI32(physMCSPI_sysconfig(ispi),ispi);
}

volatile uint32_t* mcspiInterface::ptrMCSPI_sysstatus(const unsigned ispi)
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
    uint32_t adr_physical = physMCSPI_chctrl(ispi); 
    volatile uint32_t* adr_virtual  = phys2VirtMCSPI32(adr_physical,ispi);
    return adr_virtual;
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

volatile uint32_t* mcspiInterface::ptrMCSPI_irqenable(const unsigned ispi)
{
    return phys2VirtMCSPI32(physMCSPI_irqenable(ispi),ispi);
}

volatile uint32_t* mcspiInterface::ptrMCSPI_ICLKEN()
{
    return phys2Virt32(physCM_ICLKEN1_CORE,m_iclk,physCM_ICLKEN1_CORE);
}

volatile uint32_t* mcspiInterface::ptrMCSPI_FCLKEN()
{
    return phys2Virt32(physCM_FCLKEN1_CORE,m_fclk,physCM_ICLKEN1_CORE);
}

volatile uint32_t* mcspiInterface::ptrMCSPIPadConf()
{
    return phys2Virt32(physMCSPIPadConf,m_padconf,physMCSPIPadConf);
}

volatile void* mcspiInterface::makeMap(volatile void*& virtual_addr, off_t physical_addr, size_t length)
{
    virtual_addr = mmap(0, MAP_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, m_mmap_fd, physical_addr & ~MAP_MASK);
    //printf("\nmap_address: %08X", virtual_addr); 

    // Error handling
    if (&virtual_addr==MMAPFAIL)
        exit(EXIT_FAILURE);
    else
        return &virtual_addr;
}

bool mcspiInterface::txFifoFull (const unsigned ispi)
{
    //Read 0x0: FIFO Transmit Buffer is not full
    //Read 0x1: FIFO Transmit Buffer is full
    return (0x1 & (*ptrMCSPI_chstat(ispi) >> 4)); 
}
