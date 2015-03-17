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

#define DEBUG 0
#define debug_print(fmt, ...) \
            do { if (DEBUG) fprintf(stderr, fmt, __VA_ARGS__); } while (0)

#define MMAPFAIL ((void*)-1)

#define MUNMAP(VIRT) munmap(const_cast<void*>(VIRT), 4096)
#define MUNMAP(VIRT) \
    munmap(const_cast<void*>(VIRT), 4096)

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

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <fcntl.h>
#include <ctype.h>
#include <termios.h>
#include <sys/types.h>
#include <sys/mman.h>

#define FATAL do { fprintf(stderr, "Error at line %d, file %s (%d) [%s]\n", \
  __LINE__, __FILE__, errno, strerror(errno)); exit(1); } while(0)

#define MAP_SIZE 4096UL
#define MAP_MASK (MAP_SIZE - 1)

//------------------------------------------------------------------------------
// Public Members
//------------------------------------------------------------------------------

// constructor
mcspiInterface::mcspiInterface() :
    cm_fclken1_core   (NULL),
    cm_iclken1_core   (NULL),
    mcspi_sysconfig   (NULL),
    mcspi_sysstatus   (NULL),
    mcspi_wakeupenable(NULL),
    mcspi_modulctrl   (NULL),
    mcspi_chconf      (NULL),
    mcspi_chstat      (NULL),
    mcspi_chctrl      (NULL),
    mcspi_tx          (NULL),
    mcspi_rx          (NULL),
    mcspi_irqstatus   (NULL),
    mcspi_irqenable   (NULL),
    m_mmap_fd(-1),
    m_cm_core_base(NULL),
    m_mcspi1_base(NULL)
    //m_padconf(),
{
    debug_print("%s\n", "Start of MCSPI Constructor");


    // open /dev/mem and check for failure
    m_mmap_fd = open("/dev/mem", O_RDWR | O_SYNC);
    if(m_mmap_fd<0)
    {
        perror("open(\"/dev/mem\")");
        exit(EXIT_FAILURE);
    }

    //      virtual adr     physical adr
    m_mcspi1_base  = makeMap(ADR_MCSPI_BASE);
    m_cm_core_base = makeMap(ADR_CM_CORE_BASE);


    cm_fclken1_core    = mappedAddress(OFF_CM_FCLKEN1_CORE, m_cm_core_base);
    cm_iclken1_core    = mappedAddress(OFF_CM_ICLKEN1_CORE, m_cm_core_base);

    mcspi_sysconfig    = mappedAddress(OFF_MCSPI_SYSCONFIG , m_mcspi1_base);
    mcspi_sysstatus    = mappedAddress(OFF_MCSPI_SYSSTATUS , m_mcspi1_base);
    mcspi_wakeupenable = mappedAddress(OFF_MCSPI_WAKEUPENABLE , m_mcspi1_base);
    mcspi_modulctrl    = mappedAddress(OFF_MCSPI_MODULCTRL, m_mcspi1_base);
    mcspi_chconf       = mappedAddress(OFF_MCSPI_CHCONF, m_mcspi1_base);
    mcspi_chstat       = mappedAddress(OFF_MCSPI_CHSTAT, m_mcspi1_base);
    mcspi_chctrl       = mappedAddress(OFF_MCSPI_CHCTRL, m_mcspi1_base);
    mcspi_tx           = mappedAddress(OFF_MCSPI_TX, m_mcspi1_base);
    mcspi_rx           = mappedAddress(OFF_MCSPI_RX, m_mcspi1_base);
    mcspi_irqstatus    = mappedAddress(OFF_MCSPI_IRQSTATUS, m_mcspi1_base);
    mcspi_irqenable    = mappedAddress(OFF_MCSPI_IRQENABLE, m_mcspi1_base);

    //makeMap(m_padconf,      ADR_PADCONF_BASE);
    debug_print("%s\n", "End of MCSPI Constructor");

    Reset();
}

// destructor
mcspiInterface::~mcspiInterface()
{
    munmap(m_mcspi1_base, 4096);
    munmap(m_cm_core_base, 4096);
    //MUNMAP(m_padconf);
}

/* Initializes MCSPI Controller: Enables clocks, and performs soft-reset of the
 * MCSPI system
 */

void* mcspiInterface::makeMap(off_t target)
{
    void *map_base = mmap(0, MAP_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, m_mmap_fd, target & ~MAP_MASK);
    if (map_base == (void*) -1)
        exit(EXIT_FAILURE);

    void *virt_address = map_base + (target & MAP_MASK);

    return virt_address;
}

volatile uint32_t* mcspiInterface::mappedAddress(off_t offset, void* mappedBaseAddress)
{
    void* mappedAddress = static_cast<char *>(mappedBaseAddress) + offset;
    return reinterpret_cast<volatile uint32_t*>(mappedAddress);
}


void mcspiInterface::EnableClocks()
{
    /*  Clock Enable
     *  Bit 18 0x1 = enable, 0x0 = disable
     */

    /* interface clock */
    *cm_iclken1_core |= ENABLE_INTERFACE_CLOCK;

    /* functional clock */
    *cm_fclken1_core |= ENABLE_FUNCTIONAL_CLOCK;
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
    *cm_iclken1_core &= ~ENABLE_INTERFACE_CLOCK;

    /* functional clock */
    *cm_fclken1_core &= ~ENABLE_FUNCTIONAL_CLOCK;
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
    debug_print("%s\n", "Start Enable Clocks");
    EnableClocks();
    debug_print("%s\n", "Finished Enabling Clocks");

    debug_print("Reset:mcspi_sysconfig%s\n", "");
    *mcspi_sysconfig |= SYSCONFIG_SOFTRESET;
    debug_print("%s\n", "before while loop");

    // SPIm.MCSPI_SYSSTATUS[0] will be set to 1 when the reset is finished
    // wait until it is 1..
    while (true)
    {
        debug_print("%s\n", "waiting while true");
        if ( (*mcspi_sysstatus & RESETDONE)==1 )
            break;
    }

    debug_print("%s\n", "sysconfig");
    *mcspi_sysconfig |= (SYSCONFIG_AUTOIDLE | SYSCONFIG_ENAWAKEUP | SYSCONFIG_SMARTIDLE);

    debug_print("%s\n", "wakeup");
    *mcspi_wakeupenable |= WAKEUPENABLE;

    debug_print("%s\n", "Set Master Mode");
    SetMasterMode();

    debug_print("%s\n", "Disable Clocks");
    //DisableClocks();
}

void mcspiInterface::EnableChannel()
{
    // Start Channel
    *mcspi_chctrl |= CHANNEL_ENABLE;
}

void mcspiInterface::DisableChannel()
{
    //Stop Channel
    *mcspi_chctrl &= ~CHANNEL_ENABLE;
}

void mcspiInterface::SetMasterMode()
{
    *mcspi_modulctrl &= ~MASTER_SLAVE;

    // Manage CS with force
    //*mcspi_modulctrl |= 0x1;
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
    //uint32_t config = *mcspi_chconf;
    //printf("\nInitial Config = 0x%08X", config);

    //set polarity and phase
    //mcspi_chxconf[0] phase
    //mcspi_chxconf[1] polarity
    //mode 1 --> polarity=0 phase=1
    int phase    = 1;
    int polarity = 0;

    //7. Set the SPI1.MCSPI_CHxCONF[0] PHA bit to 0 for data latched on odd-numbered edges of the SPI
    *mcspi_chconf &= ~(PHASE);
    if (phase)
    {
        *mcspi_chconf |=  (PHASE);        //mcspi_chxconf[0]
    }

    //6. Set the SPI1.MCSPI_CHxCONF[1] POL bit to 1
    *mcspi_chconf &= ~(POLARITY);
    if (polarity)
    {
        *mcspi_chconf |=  (POLARITY); //mcspi_chxconf[1]
    }

    /*      set clock rate in MCSPI_CHXCONF[5..2]
     *      Divider Clock Rate
     *      0x0  1       48      MHz
     *      0x1  2       24      MHz
     *      0x2  4       12      MHz
     *      0x3  8       6       MHz
     *      0x4  16      3       MHz
     *      0x5  32      1.5     MHz
     *      0x6  64      750     kHz
     *      0x7  128     375     kHz
     *      0x8  256     ~187    kHz
     *      0x9  512     ~93.7   kHz
     *      0xA  1024    ~46.8   kHz
     *      0xB  2048    ~23.4   kHz
     *      0xC  4096    ~11.7   kHz
     *      0xD  8192    ~5.8    kHz
     *      0xE  16384   ~2.9    kHz
     *      0xF  32768   ~1.5    kHz
     */
    int clock_divider = 0x1 << 2;
    *mcspi_chconf &= ~(CLOCK_DIVIDER);         //mcspi_chxconf[5..2]
    *mcspi_chconf |= clock_divider;            //mcspi_chxconf[5..2]

    /*     5. Set the SPI1.MCSPI_CHxCONF[6] EPOL bit to 1 for spi1_cs0 activated low
     *     during active state.  clock.
     */
    *mcspi_chconf |= EPOL;

    //spim_csx polarity
    //0x0 => spim_csx high during active state
    //0x1 => spim_csx low  during active state
    int spim_polarity = 0x1 << 6;
    *mcspi_chconf &= ~(CS_POLARITY);
    *mcspi_chconf |=  (spim_polarity); //mcspi_chxconf[6]

    //set spi word length
    int spi_word_length = 0xF << 7;  //16 bit word length
    *mcspi_chconf &= ~(SPI_WORD_LENGTH);
    *mcspi_chconf |=  (spi_word_length); //mcspi_chxconf[11..7]

    /*
     *     3. Set the SPI1.MCSPI_CHxCONF[13:12] TRM field to 0x0 for transmit
     *     and receive mode.  Transmit/receive modes RW 0x0
     *         0x0: Transmit and receive mode
     *         0x1: Receive-only mode
     *         0x2: Transmit-only mode
     *         0x3: Reserved
     */

    int transfer_mode = 0x0 << 12;
    *mcspi_chconf &= ~(SPI_TRANSFER_MODE);
    *mcspi_chconf |=  (transfer_mode); //mcspi_chxconf[13..12]

    /*     1. Set the SPI1.MCSPI_CHxCONF[18] IS bit to 0 for the spi1_somi pin in receive mode.
    */

    *mcspi_chconf &= ~(INPUT_SELECT);

    /*     2. Set the SPI1.MCSPI_CHxCONF[17] DPE1 bit to 0 and the
     *     SPI1.MCSPI_CHxCONF[16] DPE0 bit to 1 for the spi1.simo pin in
     *     transmit mode.
     */
    *mcspi_chconf &= ~(TX_EN1);
    *mcspi_chconf |=  (TX_EN0);

    // Chip Select Time Control
    *mcspi_chconf &= ~(0x3 << 25);
    *mcspi_chconf |=  (0x3 << 25);

    int tx_fifo_enable = 0x1 << 27;
    *mcspi_chconf &= ~(tx_fifo_enable);

    int rx_fifo_enable = 0x1 << 28;
    *mcspi_chconf &= ~(rx_fifo_enable);

    //int clock_divider_granularity = 0x0 << 29;
    //*mcspi_chconf &= (~0x1 << 29);
    //*mcspi_chconf |= clock_divider_granularity;
    //printf("\nWrite Config = 0x%08X", config);
    //config = *mcspi_chconf;
    //printf("\nFinal Config  = 0x%08X", config);
    //printf("\n");


    /*  Manual spim_csx assertion to keep spim_csx for channel x active between
     *  SPI words (single channel master mode only). The MCSPI_MODULCTRL[0]
     *  SINGLE bit must bit set to 1.
     */
    //*mcspi_chconf |= FORCE;
}

uint32_t mcspiInterface::WriteRead(uint32_t data)
{
    return(WriteReadInterruptMode(data));
}

uint32_t mcspiInterface::WriteReadInterruptMode(uint32_t data)
{

    ///* 20.6.2.6.3 Programming in Interrupt Mode
    // * This section follows the flow of Figure 20-26.
    // * 1. Initialize software variables: WRITE_COUNT = 0 and READ_COUNT = 0.
    // */
    int         write_count = 0;
    int         read_count  = 0;
    int         nwrite      = 1;
    int         nread       = 1;

    uint32_t    readdata    = 0x0;

start:
    debug_print("%s\n", "Start Reset");
    //Reset();
    debug_print("%s\n", "End Reset");

    ///* 2. Initialize interrupts: Write 0x7 in the SPI1.MCSPI_IRQSTATUS[3:0]
    //*     field and set the SPI1.MCSPI_IRQENABLE[3:0] field to 0x7.
    //*/
    *mcspi_irqenable &= ~0x7;
    *mcspi_irqenable |=  0x7;

    debug_print("%s\n", "irqenabled");

    *mcspi_irqstatus &= ~0x7;
    *mcspi_irqstatus |=  0x7;

    debug_print("%s %08x\n", "irqstatus updated", *mcspi_irqstatus);

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

    //this should be uncommented
    //*mcspiPadConf |= 0x1 << 8;


    /* Set the SPI1.MCSPI_CHxCTRL[0] EN bit to 1 (where x = 0) to enable channel 0. */
    EnableChannel();

    // assert !CS
    //*mcspi_chconf &= ~(FORCE);

    /* 4. If WRITE_COUNT = w and READ_COUNT = w, write SPI1.MCSPI_CHxCTRL[0] = 0x0 (x = 0) to stop
     *    the channel.  This interrupt routine follows the flow of Table
     *    20-16 and Figure 20-28.
     *      1. Read the SPI1.MCSPI_IRQSTATUS[3:0] field.
     *      2. If the SPI1.MCSPI_IRQSTATUS[0] TX0_EMPTY bit is set to 1:
     */

    int read_attempts = 0;
    while (write_count < nwrite)
    {
        /*  2. If the SPI1.MCSPI_IRQSTATUS[0] TX0_EMPTY bit is set to 1: */
        if ((*mcspi_irqstatus & 0x1)==1)
        {
            //printf("Writing Data: %04x\n", data);
            /* (a) Write the command/address or data value in SPI1.MCSPI_TXx (where x = 0). */
            *mcspi_tx = data;
            /* (b) WRITE_COUNT + = 1 */
            write_count++;
            /* (c) Write SPI1.MCSPI_IRQSTATUS[0] = 0x1. */
            *mcspi_irqstatus |= 0x1;
        }
    }

    while (read_count < nread)
    {
        //usleep(1);
        //printf("Waiting for read\n");
        /* 3. If the SPI1.MCSPI_IRQSTATUS[2] RX0_FULL bit is set to 1: */
        if (((*mcspi_irqstatus >> 2) & 0x1)==1)
        {
            /* a) Read SPI1.MCSPI_RXx (where x = 0) */
            //readdata = (*mcspi_rx >> 16) & 0xFFFF;
            readdata = (*mcspi_rx & 0xFFFF);
            //printf("Read Data: %04x\n",readdata);
            /* b) READ_COUNT += 1 */
            read_count++;
            /* c) Write SPI1.MCSPI_IRQSTATUS[2] = 0x1 */
            *mcspi_irqstatus |= (0x1 << 2);
            DisableChannel();
            return readdata;
        }
        else {
            read_attempts += 1;
            if (read_attempts > 5) {
                printf("Stalled... reset\n");
                write_count = 0;
                DisableChannel();
                goto start;
            }
        }
    }

    // deassert !CS
    //*mcspi_chconf |= (FORCE);

}


//bool mcspiInterface::txFifoFull ()
//{
//    //Read 0x0: FIFO Transmit Buffer is not full
//    //Read 0x1: FIFO Transmit Buffer is full
//    return (0x1 & (*ptrMCSPI_chstat() >> 4));
//}
