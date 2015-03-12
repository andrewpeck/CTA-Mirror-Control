//------------------------------------------------------------------------------
// Opens CPU Physical Memory at /dev/mem
// Maps physical memory into virtual address space (unix command mmap)
//------------------------------------------------------------------------------

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/mman.h>
#include <mcspiInterface.hpp>

#define DEBUG 1
#define debug_print(fmt, ...) \
            do { if (DEBUG) fprintf(stderr, fmt, __VA_ARGS__); } while (0)

#define MMAPFAIL ((void*)-1)

#define MUNMAP(VIRT) munmap(const_cast<void*>(VIRT), 4096)
#define MAP_SIZE     4096UL
#define MAP_MASK     (MAP_SIZE - 1)


#define BIT(nr)                  (1UL << (nr))

#define ENABLE_INTERFACE_CLOCK   BIT(18)
#define ENABLE_FUNCTIONAL_CLOCK  BIT(18)
#define RESETDONE                BIT(0)
#define CHANNEL_ENABLE           BIT(0)
#define MASTER_SLAVE             BIT(2)
#define PHASE                    BIT(0)
#define POLARITY                 BIT(1)
#define CLOCK_DIVIDER            (0xF << 2)
#define EPOL                     BIT(6)
#define CS_POLARITY              BIT(6)
#define SPI_WORD_LENGTH          (0x1F << 7)
#define SPI_TRANSFER_MODE        (0x7 << 12)
#define INPUT_SELECT             BIT(18)
#define TX_EN0                   BIT(16)
#define TX_EN1                   BIT(17)
#define FORCE                    BIT(20)

#define SYSCONFIG_SOFTRESET      BIT(1)
#define SYSCONFIG_AUTOIDLE       BIT(0)
#define SYSCONFIG_ENAWAKEUP      BIT(2)
#define SYSCONFIG_SMARTIDLE      BIT(4)

#define WAKEUPENABLE            BIT(0)


//------------------------------------------------------------------------------
// Public Members
//------------------------------------------------------------------------------

// constructor
mcspiInterface::mcspiInterface() :
    m_mmap_fd(-1),
    m_iclk(),
    m_fclk(),
    m_padconf(),
    m_mcspi1_base()
{
    debug_print("%s\n", "Start of MCSPI Constructor");
    // open /dev/mem and check for failure
    m_mmap_fd = open("/dev/mem", O_RDWR | O_SYNC);
    if(m_mmap_fd<0) {
        perror("open(\"/dev/mem\")");
        exit(EXIT_FAILURE);
    }

    //      virtual adr     physical adr
    makeMap(m_iclk,         physCM_FCLKEN1_CORE);
    makeMap(m_fclk,         physCM_ICLKEN1_CORE);
    makeMap(m_padconf,      physBasePadConf);
    makeMap(m_mcspi1_base,  physBaseMCSPI1);
    debug_print("%s\n", "End of MCSPI Constructor");
}

// destructor
mcspiInterface::~mcspiInterface()
{
    MUNMAP(m_iclk);
    MUNMAP(m_fclk);
    MUNMAP(m_padconf);
    MUNMAP(m_mcspi1_base);
}

/* Initializes MCSPI Controller: Enables clocks, and performs soft-reset of the
 * MCSPI system
*/

void mcspiInterface::EnableClocks()
{
    /*  Clock Enable
     *  Bit 18 = EN_MCSPI1
     *  Bit 19 = EN_MCSPI2
     *  Bit 20 = EN_MCSPI3
     *  Bit 21 = EN_MCSPI4
     *  0x1 = enable, 0x0 = disable
     */

    /* interface clock */
    *ptrMCSPI_ICLKEN() &= ~ENABLE_INTERFACE_CLOCK;

    /* functional clock */
    *ptrMCSPI_FCLKEN() &= ~ENABLE_FUNCTIONAL_CLOCK;
}

void mcspiInterface::DisableClocks()
{
    /*  Clock Enable
     *  Bit 18 = EN_MCSPI1
     *  Bit 19 = EN_MCSPI2
     *  Bit 20 = EN_MCSPI3
     *  Bit 21 = EN_MCSPI4
     *  0x1 = enable, 0x0 = disable
     */

    /* interface clock */
    *ptrMCSPI_ICLKEN() |= ENABLE_INTERFACE_CLOCK;

    /* functional clock */
    *ptrMCSPI_FCLKEN() |= ENABLE_FUNCTIONAL_CLOCK;
}



/*  Reset Sequence:
 *  enable Clocks
 *  Write softreset
 *  wait for reset to finish
 *  set autoidle+enawakeup+smartidle
 *
 *  write wakeupenable
 *
 *  set mode to master
 *  disable clocks */
void mcspiInterface::Reset()
{
    debug_print("%s\n", "Before Enable Clocks");
    EnableClocks();
    debug_print("%s\n", "After Enable Clocks");

    debug_print("Reset:ptrMCSPI_sysconfig%s\n", "");
    *ptrMCSPI_sysconfig() |= SYSCONFIG_SOFTRESET;
    debug_print("%s\n", "before while loop");

    // SPIm.MCSPI_SYSSTATUS[0] will be set to 1 when the reset is finished
    // wait until it is 1..
    while (true)
    {
        debug_print("%s\n", "waiting while true");
        if ( (*ptrMCSPI_sysstatus() & RESETDONE)==1 )
            break;
    }

    debug_print("%s\n", "sysconfig");
    *ptrMCSPI_sysconfig() |= (SYSCONFIG_AUTOIDLE | SYSCONFIG_ENAWAKEUP | SYSCONFIG_SMARTIDLE);

    debug_print("%s\n", "wakeup");
    *ptrMCSPI_wakeupenable() |= WAKEUPENABLE;

    debug_print("%s\n", "Set Master Mode");
    SetMasterMode();

    debug_print("%s\n", "Disable Clocks");
    DisableClocks();
}

void mcspiInterface::EnableChannel()
{
    // Start Channel
    *ptrMCSPI_chctrl() |= CHANNEL_ENABLE;
}

void mcspiInterface::DisableChannel()
{
    //Stop Channel
    *ptrMCSPI_chctrl() &= ~CHANNEL_ENABLE;
}

void mcspiInterface::SetMasterMode()
{
    *ptrMCSPI_modulctrl() &= ~MASTER_SLAVE;

    // Manage CS with force
    //*ptrMCSPI_modulctrl() |= 0x1;
}

//void mcspiInterface::RestoreContext()
//{
//
//    /* Write to MCSPI_MODULCTRL
//     * Write to MCSPI_SYSCONFIG
//     * Write to MCSPI_WAKUPENABLE
//     * Some weird loop
//     *
//     * Seems to just be rewriting a saved configuration
//     */
//}

void mcspiInterface::Configure()
{
    /*  Transfer Setup Sequence from Kernel
     *  Set MCSPI_MAX_FREQ
     *  Set Chip Select Polarity, Manage with Force
     *  Set Clock Divisor
     *  Set SPI Mode
     *  */

    //printf("\nReset finished..");

    // get current config
    //uint32_t config = *ptrMCSPI_chconf();
    //printf("\nInitial Config = 0x%08X", config);

    //set polarity and phase
    //mcspi_chxconf[0] phase
    //mcspi_chxconf[1] polarity
    //mode 1 --> polarity=0 phase=1
    int phase    = 1;
    int polarity = 0;

    //7. Set the SPI1.MCSPI_CHxCONF[0] PHA bit to 0 for data latched on odd-numbered edges of the SPI
    *ptrMCSPI_chconf() &= ~(PHASE);
    if (phase)
    {
        *ptrMCSPI_chconf() |=  (PHASE);        //mcspi_chxconf[0]
    }

    //6. Set the SPI1.MCSPI_CHxCONF[1] POL bit to 1
    *ptrMCSPI_chconf() &= ~(POLARITY);
    if (polarity)
    {
        *ptrMCSPI_chconf() |=  (POLARITY); //mcspi_chxconf[1]
    }

    /*      set clock rate in MCSPI_CHXCONF[5..2]
     *      Divider Clock Rate
     *        1       48      MHz
     *        2       24      MHz
     *        4       12      MHz
     *        8       6       MHz
     *        16      3       MHz
     *        32      1.5     MHz
     *        64      750     kHz
     *        128     375     kHz
     *        256     ~187    kHz
     *        512     ~93.7   kHz
     *        1024    ~46.8   kHz
     *        2048    ~23.4   kHz
     *        4096    ~11.7   kHz
     *        8192    ~5.8    kHz
     *        16384   ~2.9    kHz
     *        32768   ~1.5    kHz
     */
    int clock_divider = 0x4 << 2;      //16
    *ptrMCSPI_chconf() &= ~(CLOCK_DIVIDER);         //mcspi_chxconf[5..2]
    *ptrMCSPI_chconf() |= clock_divider;            //mcspi_chxconf[5..2]

    /*     5. Set the SPI1.MCSPI_CHxCONF[6] EPOL bit to 1 for spi1_cs0 activated low
     *     during active state.  clock.
     */
    *ptrMCSPI_chconf() |= EPOL;

    //spim_csx polarity
    //0x0 => spim_csx high during active state
    //0x1 => spim_csx low  during active state
    int spim_polarity = 0x1 << 6;
    *ptrMCSPI_chconf() &= ~(CS_POLARITY);
    *ptrMCSPI_chconf() |=  (spim_polarity); //mcspi_chxconf[6]

    //set spi word length
    int spi_word_length = 0xF << 7;  //16 bit word length
    *ptrMCSPI_chconf() &= ~(SPI_WORD_LENGTH);
    *ptrMCSPI_chconf() |=  (spi_word_length); //mcspi_chxconf[11..7]

    /*
     *     3. Set the SPI1.MCSPI_CHxCONF[13:12] TRM field to 0x0 for transmit
     *     and receive mode.  Transmit/receive modes RW 0x0
     *         0x0: Transmit and receive mode
     *         0x1: Receive-only mode
     *         0x2: Transmit-only mode
     *         0x3: Reserved
     */

    int transfer_mode = 0x0 << 12;
    *ptrMCSPI_chconf() &= ~(SPI_TRANSFER_MODE);
    *ptrMCSPI_chconf() |=  (transfer_mode); //mcspi_chxconf[13..12]

    /*     1. Set the SPI1.MCSPI_CHxCONF[18] IS bit to 0 for the spi1_somi pin in receive mode.
    */

    *ptrMCSPI_chconf() &= ~(INPUT_SELECT);

    /*     2. Set the SPI1.MCSPI_CHxCONF[17] DPE1 bit to 0 and the
     *     SPI1.MCSPI_CHxCONF[16] DPE0 bit to 1 for the spi1.simo pin in
     *     transmit mode.
     */

    *ptrMCSPI_chconf() &= ~(TX_EN1);
    *ptrMCSPI_chconf() |=  (TX_EN0);


    //int tx_fifo_enable = 0x1;
    //*ptrMCSPI_chconf() |= ( << 27);

    //printf("\nWrite Config = 0x%08X", config);
    //config = *ptrMCSPI_chconf(0);
    //printf("\nFinal Config  = 0x%08X", config);
    //printf("\n");


    /*  Manual spim_csx assertion to keep spim_csx for channel x active between
     *  SPI words (single channel master mode only). The MCSPI_MODULCTRL[0]
     *  SINGLE bit must bit set to 1.
     */
    //*ptrMCSPI_chconf(0) |= FORCE;
}

uint32_t mcspiInterface::WriteRead(uint32_t data)
{
    /* 20.6.2.6.3 Programming in Interrupt Mode
     * This section follows the flow of Figure 20-26.
     * 1. Initialize software variables: WRITE_COUNT = 0 and READ_COUNT = 0.
     */
    int         write_count = 0;
    int         read_count  = 0;
    int         nwrite      = 1;
    int         nread       = 1;

    uint32_t    read        = 0;

    debug_print("%s\n", "Before first Reset");
    Reset();
    debug_print("%s\n", "After first Reset");

    /* 2. Initialize interrupts: Write 0x7 in the SPI1.MCSPI_IRQSTATUS[3:0]
    *     field and set the SPI1.MCSPI_IRQENABLE[3:0] field to 0x7.
    */
    *ptrMCSPI_irqenable() &= ~0x7;
    *ptrMCSPI_irqenable() |=  0x7;

    *ptrMCSPI_irqstatus() &= ~0x7;
    *ptrMCSPI_irqstatus() |=  0x7;

    /* 3. Follow the steps described in Section 20.6.2.6.2.1.1, Mode Selection.  */
    Configure();

    /* Clock Initialization and spi1_cs0 Enable
     * In master mode, the SPI must provide the clock and enable the channel:
     * Set the SPI1.MCSPI_MODULCTRL[2] MS bit to 0 to provide the clock.
     */
    SetMasterMode();

    /*     Set the INPUTENABLE bit of the corresponding CONTROL_PADCONF_x
     *     register to achieve the synchronous clock by using loopback through
     *     the output and input buffers of the pad.
     *
     *     CONTROL_PADCONF_MCSPI1_CLK[15:0]
     *     0x4800 21C8
     *     Section 13.4.4
     *     Bit 8 == Input Enable
     */

    *ptrMCSPIPadConf() |= 0x1 << 8;


    /* Set the SPI1.MCSPI_CHxCTRL[0] EN bit to 1 (where x = 0) to enable channel 0. */
    EnableChannel();

    // assert !CS
    //*ptrMCSPI_chconf(0) |= (FORCE);

    /* 4. If WRITE_COUNT = w and READ_COUNT = w, write SPI1.MCSPI_CHxCTRL[0] = 0x0 (x = 0) to stop
     *    the channel.  This interrupt routine follows the flow of Table
     *    20-16 and Figure 20-28.
     *      1. Read the SPI1.MCSPI_IRQSTATUS[3:0] field.
     *      2. If the SPI1.MCSPI_IRQSTATUS[0] TX0_EMPTY bit is set to 1:
     */

    while (write_count < nwrite && read_count < nread)
    {
        /*  2. If the SPI1.MCSPI_IRQSTATUS[0] TX0_EMPTY bit is set to 1: */
        if ((*ptrMCSPI_irqstatus() & 0x1) == 1)
        {
            printf("\n I am here");
            /* (a) Write the command/address or data value in SPI1.MCSPI_TXx (where x = 0). */
            *ptrMCSPI_tx() = data;
            /* (b) WRITE_COUNT + = 1 */
            write_count++;
            /* (c) Write SPI1.MCSPI_IRQSTATUS[0] = 0x1. */
            *ptrMCSPI_irqstatus() |= 0x1;
        }

        /* 3. If the SPI1.MCSPI_IRQSTATUS[2] RX0_FULL bit is set to 1: */
        if (((*ptrMCSPI_irqstatus() >> 2) & 0x1) == 1)
        {
            printf("\n I am there");
            /* a) Read SPI1.MCSPI_RXx (where x = 0) */
            read = *ptrMCSPI_rx();
            /* b) READ_COUNT += 1 */
            read_count++;
            /* c) Write SPI1.MCSPI_IRQSTATUS[2] = 0x1 */
            *ptrMCSPI_irqstatus() |= 0x1 << 2;
        }
    }
    DisableChannel();

    // deassert !CS
    // *ptrMCSPI_chconf(0) &= ~(FORCE);

    return read;
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

volatile uint32_t* mcspiInterface::phys2VirtMCSPI32(off_t adr_physical)
{
    debug_print("ADR_PHYSICAL  %08X\n", adr_physical);
    debug_print("m_mcspi1_base %08X\n", &m_mcspi1_base);

    return phys2Virt32(adr_physical,m_mcspi1_base,physBaseMCSPI1);
}

// Takes Physical Address and Returns pointer to memory mapped virtual address
volatile uint32_t* mcspiInterface::phys2Virt32(off_t phys, volatile void* map_base_virt, off_t map_base_phys)
{
    off_t map_offset;

    // map offset is the difference between the physical address and the
    // physical base address
    map_offset = phys - map_base_phys;

    volatile uint8_t *virtual_base_pointer = static_cast<volatile uint8_t*>(map_base_virt);

    printf("phys2Virt32:map_offset:         %08X\n", map_offset);
    printf("phys2Virt32:map_base_virt:      %08X\n", &map_base_virt);
    printf("phys2Virt32:map_base_virt_cast: %08X\n", virtual_base_pointer);

    // virtual address the base of the virtual address + the map_offset with
    // some fancy typecasting we static cast the void pointer map_base_virt
    // into a pointer to an 8 bit integer...  then offset that pointer by some
    // number of bytes, then reinterpret the data at that memory as a pointer
    // to a 32bit unsigned integer, and return that pointer!
    volatile uint32_t* adr_virtual = reinterpret_cast<volatile uint32_t*>
        (static_cast<volatile uint8_t*>(map_base_virt) + map_offset);

    printf("phys2Virt32:%s\n", "return adr_virtual");
    return adr_virtual;
}

// --------------------------------------------------------------------------
// functions defining (physical) start of memory mapped regions
// --------------------------------------------------------------------------
// Functions to return pointers to mapped SSP registers
// --------------------------------------------------------------------------

volatile uint32_t* mcspiInterface::ptrMCSPI_sysconfig()
{
    debug_print("%s\n", "Before phys2Virt");
    return phys2VirtMCSPI32(mcspi_offset_sysconfig);
}

volatile uint32_t* mcspiInterface::ptrMCSPI_sysstatus()
{
    return phys2VirtMCSPI32(mcspi_offset_sysconfig);
}

volatile uint32_t* mcspiInterface::ptrMCSPI_wakeupenable()
{
    return phys2VirtMCSPI32(mcspi_offset_wakeupenable);
}

volatile uint32_t* mcspiInterface::ptrMCSPI_modulctrl()
{
    return phys2VirtMCSPI32(mcspi_offset_modulctrl);
}

volatile uint32_t* mcspiInterface::ptrMCSPI_chconf()
{
    uint32_t  adr_physical = mcspi_offset_chconf;
    volatile uint32_t* adr_virtual  = phys2VirtMCSPI32(adr_physical);
    return adr_virtual;
}

volatile uint32_t* mcspiInterface::ptrMCSPI_chstat()
{
    return phys2VirtMCSPI32(mcspi_offset_chstat);
}

volatile uint32_t* mcspiInterface::ptrMCSPI_chctrl()
{
    uint32_t adr_physical = mcspi_offset_chctrl;
    volatile uint32_t* adr_virtual  = phys2VirtMCSPI32(adr_physical);
    return adr_virtual;
}

volatile uint32_t* mcspiInterface::ptrMCSPI_tx()
{
    return phys2VirtMCSPI32(mcspi_offset_tx);
}

volatile uint32_t* mcspiInterface::ptrMCSPI_rx()
{
    return phys2VirtMCSPI32(mcspi_offset_rx);
}

volatile uint32_t* mcspiInterface::ptrMCSPI_irqstatus()
{
    return phys2VirtMCSPI32(mcspi_offset_irqstatus);
}

volatile uint32_t* mcspiInterface::ptrMCSPI_irqenable()
{
    return phys2VirtMCSPI32(mcspi_offset_irqenable);
}

volatile uint32_t* mcspiInterface::ptrMCSPI_ICLKEN()
{
    return phys2Virt32(physCM_ICLKEN1_CORE,m_iclk,physCM_ICLKEN1_CORE);
}

volatile uint32_t* mcspiInterface::ptrMCSPI_FCLKEN()
{
    return phys2Virt32(physCM_FCLKEN1_CORE,m_fclk,physCM_FCLKEN1_CORE);
}

volatile uint32_t* mcspiInterface::ptrMCSPIPadConf()
{
    return phys2Virt32(physMCSPIPadConf,m_padconf,physBasePadConf);
}

volatile void* mcspiInterface::makeMap(volatile void*& virtual_addr, off_t physical_addr, size_t length)
{
    virtual_addr = mmap(0, length, PROT_READ | PROT_WRITE, MAP_SHARED, m_mmap_fd, physical_addr);
    debug_print("\nmap_address: %08X", &virtual_addr);

    // Error handling
    if (&virtual_addr==MMAPFAIL)
    {
        fprintf(stderr, "ERROR: Failed to map memory!\n");
        exit(EXIT_FAILURE);
    }
    else
        return &virtual_addr;
}


bool mcspiInterface::txFifoFull ()
{
    //Read 0x0: FIFO Transmit Buffer is not full
    //Read 0x1: FIFO Transmit Buffer is full
    return (0x1 & (*ptrMCSPI_chstat() >> 4));
}
