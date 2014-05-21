////////////////////////////////////////////////////////////////////////////////
// Class which maps to Gumstix Overo architecture, which mostly
////////////////////////////////////////////////////////////////////////////////

#ifndef OVERO_HPP
#define OVERO_HPP

#include <sys/mman.h>
#include <stdint.h>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <cctype>

#include <Layout.hpp>
#include <Bits.hpp>
#include <Registers.hpp>

#define GPIODIR_IN  0
#define GPIODIR_OUT 1


class OveroBase
{
    public:

        // --------------------------------------------------------------------------
        // STATIC GPIO functions
        // --------------------------------------------------------------------------

        //Number of GPIOs Available
        static unsigned nGPIO() { 
            return 192; 
        }

        // check if ipin is greater than the number of GPIOs
        static bool hasGPIORegister(const unsigned ipin) { 
            return ipin<nGPIO(); 
        }

        // Curtain off unavailable GPIOs
        static bool hasGPIOPin(const unsigned ipin) {
            if((ipin==2) || ((ipin<=8) && (ipin>=5)) || (ipin>=nGPIO()))
                return false;
            return true;
        }

        // --------------------------------------------------------------------------
        // GPIO Definitions
        // --------------------------------------------------------------------------

        //enum GPIODir { 
        //    GPIODIR_IN = 0, 
        //    GPIODIR_OUT 
        //};

        //enum GPIOFn { 
        //    GPIOFN_IO = 0, 
        //    GPIOFN_ALT1, 
        //    GPIOFN_ALT2, 
        //    GPIOFN_ALT3 
        //};

        /*
        // --------------------------------------------------------------------------
        // SSP Definitions
        // --------------------------------------------------------------------------

        // Serial Port Data Format
        enum SSPFrameFormat {
            SSP_FF_SPI, 
            SSP_FF_TISSP, 
            SSP_FF_MICROWIRE, 
            SSP_FF_PSP 
        };

        //Serial Port Clock Source Select
        enum SSPClockSelect { 
            SSP_CS_INTERNAL, 
            SSP_CS_NETWORK, SSP_EXTERNAL, SSP_CS_AUDIO 
        };

        // Serial Port Clock Mode Select
        enum SSPPSPSerialClockMode { 
            SSP_PSP_SCMODE_FRL, 
            SSP_PSP_SCMODE_RFL, 
            SSP_PSP_SCMODE_RFH, 
            SSP_PSP_SCMODE_FRH 
        };
        */

//#define SSP_FLAG_NONE                                    0x00000000
//#define SSP_FLAG_NETWORK_MODE                            0x00000001
//#define SSP_FLAG_MASK_INT_TX_UNDERRUN                    0x00000002
//#define SSP_FLAG_MASK_INT_RX_OVERRUN                     0x00000004
//#define SSP_FLAG_SSP_ENABLE                              0x00000008
//#define SSP_FLAG_TXD_TRISTATE_ON_LAST_PHASE              0x00000010
//#define SSP_FLAG_TXD_TRISTATE                            0x00000020
//#define SSP_FLAG_ENABLE_INT_BIT_COUNT_ERROR              0x00000040
//#define SSP_FLAG_SLAVE_CLOCK_FREE_RUNNING                0x00000080
//#define SSP_FLAG_ENABLE_CLOCK_REQUEST_A                  0x00000100
//#define SSP_FLAG_ENABLE_CLOCK_REQUEST_B                  0x00000200
//#define SSP_FLAG_SLAVE_TO_SCLKDIR                        0x00000400
//#define SSP_FLAG_SLAVE_TO_SFRMDIR                        0x00000800
//#define SSP_FLAG_RX_WITHOUT_TX                           0x00001000
//#define SSP_FLAG_TRAILING_BYTES_HANDLED_BY_DMA           0x00002000
//#define SSP_FLAG_ENABLE_DMA_TX_SERVICE_REQ               0x00004000
//#define SSP_FLAG_ENABLE_DMA_RX_SERVICE_REQ               0x00008000
//#define SSP_FLAG_ENABLE_INT_RX_TIME_OUT                  0x00010000
//#define SSP_FLAG_ENABLE_INT_PERIPHERAL_TRAILING_BYTE     0x00020000
//#define SSP_FLAG_INVERT_FRAME_SIGNAL                     0x00040000
//#define SSP_FLAG_SELECT_RX_FIFO_FOR_TEST_MODE            0x00080000
//#define SSP_FLAG_ENABLE_FIFO_TEST_MODE                   0x00100000
//#define SSP_FLAG_MICROWIRE_16BIT_DATA_SIZE               0x00200000
//#define SSP_FLAG_SPH                                     0x00400000
//#define SSP_FLAG_SPO                                     0x00800000
//#define SSP_FLAG_LOOP_BACK_MODE                          0x01000000
//#define SSP_FLAG_ENABLE_INT_TX_FIFO                      0x02000000
//#define SSP_FLAG_ENABLE_INT_RX_FIFO                      0x04000000
//
//#define SSP_PSP_FLAG_NONE                                0x00000000
//#define SSP_PSP_FLAG_FSRT                                0x00000001
//#define SSP_PSP_FLAG_ETDS                                0x00000002
//#define SSP_PSP_FLAG_SFRMP                               0x00000004
};


//typedef OveroGPIOMap GPIOMap; 
template<typename REG = MappedRegisters, typename REV = OveroGPIOMap> 
class Overo: public OveroBase, public REV, public REG {
    public:
        Overo(): OveroBase(), REV(), REG() { }
        ~Overo() { }

        /*
        // --------------------------------------------------------------------------
        // Clock utility functions
        // --------------------------------------------------------------------------

        // Enable or Disable a Clock Bit
        void clockSetEnableBit(unsigned ibit, bool enable = true) {
            if (enable)
                *REG::ptrClockEnable() |= (0x1<<ibit); 
            else 
                *REG::ptrClockEnable() &= ~(0x1<<ibit);
        }

        // Disable a Clock Bit
        void clockClearEnableBit(unsigned ibit) { 
            clockSetEnableBit(ibit,false); 
        }

        // Check Clock bit Enable/Disable Status
        bool clockIsBitEnabled(unsigned ibit) {
            return *REG::ptrClockEnable()&(0x1<<ibit); 
        }
        */

        // Enable MCSPI Clocks
        //void clockEnableMCSPI1(bool enable = true) { clockSetEnableBit(/* something */,enable); }
        //void clockEnableMCSPI2(bool enable = true) { clockSetEnableBit(/* something */,enable); }
        //void clockEnableMCSPI3(bool enable = true) { clockSetEnableBit(/* something */,enable); }
        //void clockEnableMCSPI4(bool enable = true) { clockSetEnableBit(/* something */,enable); }

        //// Disable MCSPI Clocks
        //void clockDisableMCSPI1() { clockEnableSSP1(false); }
        //void clockDisableMCSPI2() { clockEnableSSP2(false); }
        //void clockDisableMCSPI3() { clockEnableSSP3(false); }
        //void clockDisableMCSPI4() { clockEnableSSP3(false); }

        //// Enable any MCSPI Clock by ispi number 
        //void clockEnableMCSPI(unsigned ispi, bool enable = true) {
        //    if(ispi<4)
        //        clockEnableMCSPI1(enable);
        //    else if(ispi<6)
        //        clockEnableMCSPI2(enable);
        //    else if(ispi<8)
        //        clockEnableMCSPI3(enable);
        //    else if(ispi==0)
        //        clockEnableMCSPI4(enable);
        //    else
        //        return; 
        //}

        //// Disable ALL MCSPI Clocks
        //void clockDisableMCSPI(unsigned ispi) { clockEnableSSP(issp,false); }

        //// Check Enable/Disable Status of MCSPI Clock
        //bool clockIsEnabledSSP1() { return clockIsBitEnabled(/* some number */); }
        //bool clockIsEnabledSSP2() { return clockIsBitEnabled(/* some number */); }
        //bool clockIsEnabledSSP3() { return clockIsBitEnabled(/* some number */); }
        //bool clockIsEnabledSSP4() { return clockIsBitEnabled(/* some number */); }

        //// Check Enable/Disable Status of any MCSPI Clock by ispi number
        //bool clockIsEnabledSSP(unsigned ispi) {
        //    if(ispi<4)
        //        return clockIsEnabledMCSPI1();
        //    else if(ispi<6)
        //        return clockIsEnabledMCSPI2();
        //    else if(ispi<8)
        //        return clockIsEnabledMCSPI3();
        //    else if(ispi==0)
        //        return clockIsEnabledMCSPI4();
        //    else
        //        return; 
        //}

        // --------------------------------------------------------------------------
        // GPIO utility functions
        // --------------------------------------------------------------------------

        // Read GPIO by ipin (0-192)
        bool gpioReadLevel(const unsigned ipin) {
            bool level = *(REG::ptrGPIOReadLevel(ipin)) & Bits::mask1Bit(ipin);
            printf("Read %i from pin %i",level,ipin);
            return level; 
        }

        // Write GPIO by ipin (0-192)
        void gpioWriteLevel(const unsigned ipin, bool level) {
            if (level)
                REG::gpioSetLevel(ipin);
            else 
                REG::gpioClrLevel(ipin);
            //printf("Writing %i to pin %i",level,ipin);
        }

        // Get GPIO Direction In/Out
        bool gpioGetDirection(const unsigned ipin) {
            volatile uint32_t* reg = REG::ptrGPIODirection(ipin);
            uint32_t val = *reg;
            bool dir = !!(val & Bits::mask1Bit(ipin));
            return dir; 
        }

        // Set GPIO Direction In/Out
        void gpioSetDirection(const unsigned ipin, bool dir) {
            volatile uint32_t* reg = REG::ptrGPIODirection(ipin);
            uint32_t val = *reg;
            if(dir==1)
                val |= Bits::mask1Bit(ipin);
            else 
                val &= ~Bits::mask1Bit(ipin);
            *reg = val;
            printf("gpioSetDirection :: Writing %zu", val);
        }

        // figure out what this is used by
        //GPIOFn gpioGetAltFunc(const unsigned ipin) {
        //    volatile uint32_t* reg = REG::ptrGPIOAltFunc(ipin);
        //    uint32_t val = ((*reg)>>Bits::shift2Bit(ipin))&Bits::mask2Bit();
        //    return static_cast<GPIOFn>(val);
        //}

        //
        //void gpioSetAltFunc(const unsigned ipin, const GPIOFn fn)
        //{ 
        //    volatile uint32_t* reg = REG::ptrGPIOAltFunc(ipin);
        //    uint32_t val = *reg & ~Bits::mask2Bit(ipin);
        //    val |= (static_cast<uint32_t>(fn) << Bits::shift2Bit(ipin));
        //    *reg = val;
        //}

        // Configures a given GPIO pin for a direction (1=OUT,0=in) and function
        void gpioConfigure(const unsigned ipin, bool dir) {
            gpioSetDirection(ipin, dir);
            //gpioSetAltFunc(ipin, fn);
        }
        
        void gpioConfigureAll() {
        //Configure Input/Output directions for GPIOs
            for (int i=0; i<192; i++) {
                int config = REV::gpioConfiguration(i);
                if (config==-1)      // don't use
                    return; 
                else if (config==0)  // input
                    gpioSetDirection(i, 0);
                else if (config==1)  //output
                    gpioSetDirection(i, 1);
                else 
                    return;
            }
        }

        // --------------------------------------------------------------------------
        // SSP utility functions
        // --------------------------------------------------------------------------

        void spiConfigure (int ispi, unsigned clk_phase, bool clk_polarity, bool clk_div, bool epol, int word_length) {
            uint32_t data = 0x0000; 

            // Set Clock Phase
            data |= (clk_phase<<0);

            // Set Clock Polarity
            data |= (clk_polarity << 1);

            // Set Clock Rate (1 = 48 Mhz)
            data |= (clk_div<<2);
            
            // Set CS Polarity
            // 0x0: spim_csx is held high during the active state.
            // 0x1: spim_csx is held low during the active state.
            data |= (epol<<6);

            // Set SPI Word Length
            // 0x1F = 32 bit length
            data |= (word_length<<7);

            // Set SPI Transmit/Receive Mode
            // 0x0: Transmit and receive mode
            // 0x1: Receive-only mode
            // 0x2: Transmit-only mode
            // 0x3: Reserved
            data |= (0x00 << 12); 

            // Set DMA Write Request
            data |= (0x00 << 14); 
            // Set DMA Read Request
            data |= (0x00 << 15); 

            // Transmission enable for data line 0 (spim_somi)
            // 0x0: Data Line 0 (spim_somi) selected for transmission
            // 0x1: No transmission on data Line 0 (spim_somi)
            data |= (0x1 << 16); 

            //Transmission enable for data line 1 (spim_simo)
            // 0x0: Data Line 1 (spim_simo) selected for transmission
            // 0x1: No transmission on data Line 1 (spim_simo)
            data |= (0x0 << 17); 

            //Input Select
            //0x0: Data Line 0 (spim_somi) selected for reception
            //0x1: Data Line 1 (spim_simo) selected for reception
            data |= (0x1 << 18); 

            //Turbo Mode
            //0x0: Turbo is deactivated (recommended for single SPI word transfer)
            //0x1: Turbo is activated to maximize the throughput for multi-SPI word transfers.
            data |= (0x0 << 19); 

            //Force
            //Manual spim_csx assertion to keep spim_csx for channel x active 
            //between SPI words (single channel master mode only). The
            //MCSPI_MODULCTRL[0] SINGLE bit must bit set to 1.
            //0x0: Writing 0 into this bit drives the spin_csx line low for
            //     channel x when MCSPI_CHxCONF [6] EPOL = 0, and
            //     drives it high when MCSPI_CHxCONF[6] EPOL = 1.
            //0x1: Writing 1 into this bit drives the spim_csx line high for
            //     channel x when MCSPI_CHxCONF[E6] EPOL = 0, and
            //     drives it low when MCSPI_CHxCONF[6] EPOL = 1.
            data |= (0x0 << 20); 

            //Reserved Bits
            data |= (0x0 << 21); 
            data |= (0x0 << 22); 

            //Start bit enable for SPI transfer
            //0x0: Default SPI transfer length as specified by WL bit field
            //0x1: Start bit D/CX added before SPI transfer. Polarity is
            //     defined by MCSPI_CHxCONF[SBPOL].
            data |= (0x0 << 23); 

            //Start bit polarity RW 0x0
            //0x0: Start bit polarity is held to 0 during SPI transfer.
            //0x1: Start bit polarity is held to 1 during SPI transfer.
            data |= (0x0 << 24); 

            //Chip select time control
            //Defines the number of interface clock cycles between CS toggling
            //and first (or last) edge of SPI clock.
            //0x0: 0.5 clock cycle
            //0x1: 1.5 clock cycles
            //0x2: 2.5 clock cycles
            //0x3: 3.5 clock cycles
            data |= (0x00 << 25); 

            //FIFO enabled for Transmit. Only one channel can have this bit field set.
            //0x0: The buffer is not used to Transmit data
            //0x1: The buffer is used to Transmit data
            data |= (0x00 << 27); 

            //FIFO enabled for Receive. Only one channel can have this bit RW 0x0
            //field set.
            //0x0: The buffer is not used to Receive data
            //0x1: The buffer is used to Receive data
            data |= (0x00 << 28); 

            //Clock divider granularity. This register defines the granularity of RW 0x0
            //channel clock divider: power of two or one clock cycle granularity.
            //When this bit is set, the MCSPI_CHxCTRL[15:8] EXTCLK bit field
            //must be configured to reach a maximum of 4096 clock divider
            //ratio. Then the clock divider ratio is a concatenation of
            //MCSPI_CHxCONF[5:2] CLKD and EXTCLK values.
            //0x0: Clock granularity of power of two
            //0x1: One clock cycle ganularity
            data |= (0x0 << 29); 

            //Reserved Bits
            data |= (0x00 << 30); 

            //printf("\nData to Write: 0x%04X", data); 
            
            // Write Data To Register
            //*REG::ptrMCSPI_chconf(ispi); 
            //= data; 
            printf("\nData Written to Bus: 0x%04X", data); 

            data = *REG::ptrMCSPI_chconf(ispi); 
            //printf("\nData on Bus: 0x%04X", data); 
        }

        //In multichannel, only one channel can use the FIFO. Before enabling the FIFO for a channel (FFExW and
        //FFExR bits in the MCSPI_CHx_CONF register), the host must ensure that the FIFO is not enabled for
        //another channel, even if these channels are not used.
        void disableAllFIFOs() {
            uint32_t data = 0x0000; 
            data |= ~(0x1 << 27); 
            data |= ~(0x1 << 28); 
            data = ~data; 

            for (int ispi=0; ispi<=8; ispi++) {
                *REG::ptrMCSPI_chconf(ispi) &= data; 
            }
        }

        //TransmitReceiveNoWordCount
        uint32_t spiWriteRead (unsigned ispi, uint32_t wr_data) {
            uint32_t rd_data=0; 

            //Start Channel
            *REG::ptrMCSPI_chctrl(ispi) |= 0x1;

            //Read MCSPI_CHxStat
            while (!spiEndofTransfer(ispi) && !spiTxFIFOisEmpty(ispi)) {
                if (spiTxFIFOisEmpty(ispi)) {
                    //write write_request_size words to MCSPI_TXx
                    *REG::ptrMCSPI_tx(ispi)=wr_data; 
                }
                else {
                    if (spiRxFIFOisFull(ispi)) 
                        rd_data = *REG::ptrMCSPI_rx(ispi); 
                }
            }

            //Stop Channel
            *REG::ptrMCSPI_chctrl(ispi) &= 0x0; 

            return rd_data; 
        }

        //void WriteData(unsigned ispi) {
        //    uint32_t data = 0x0000; 
        //    //Interrupt Initialization: 
        //    //  1. Reset Status Bits in MCSPI_IRQSTATUS
        //    *REG::ptrMCSPI_chconf(ispi) = 0x0000; 
        //    //  2. Enable Interrupts in MCSPI_IRQENABLE
        //    data = 0x0000; 
        //    data |= 
        //    *REG::ptrMCSPI_irqenable(ispi) = 0x0000; 



        //    //Channel Configuration
        //    //  1. Write MCSPI_CHXConf
        //    //  2. Write MCSPI_XFERLEVEL

        //    //Start Channel
        //    //  1. Write 1 in MCSPI_CHXCtrl: ENx  
        //    *REG::ptrMCSPI_chctrl(ispi) |= 0x1
        //    //page 3036
        //    
        //    
        //    data = *REG::ptrMCSPI_chstat(ispi); 
        //    if (data 
        //    //If Receive Only: 
        //    //  1. Write Request (tx empty or dma write)
        //    //  2. Transmit word: Write MCSPI_TXx

        //    // see page 3033
        //    // Host Event for the end of transfer
        //    // Stop the channel: Write 0 in MCSPI_CHxCTRL: ENx
        //    *REG::ptrMCSPI_chctrl(const unsigned issp) &= 0x0
        //}

        void spiEnable(unsigned ispi, bool enable = true) { 
            if (enable) {
                //clockEnableSSP(issp);
                *REG::ptrMCSPI_chctrl(ispi) |= (0x1); 
            }
            else {
                *REG::ptrMCSPI_chctrl(ispi) &= (0x0);
                //clockDisableSSP(issp);
            }
        }

        void spiDisable(unsigned ispi) { 
            spiEnable(ispi,false);
        }


        //bool sspIsBitCountError(unsigned issp) { return sspTestStatusBit(issp,23); }
        //bool sspIsClockSynchBad(unsigned issp) { return sspTestStatusBit(issp,22); }
        //bool sspIsTXUnderrun(unsigned issp) { return sspTestStatusBit(issp,21); }
        //bool sspIsEndOfDMAChain(unsigned issp) { return sspTestStatusBit(issp,20); }
        //bool sspIsTimeOutInt(unsigned issp) { return sspTestStatusBit(issp,19); }
        //bool sspIsTrailingByteInt(unsigned issp) { return sspTestStatusBit(issp,18); }
        //bool sspIsRXOverrun(unsigned issp) { return sspTestStatusBit(issp,7); }
        //bool sspIsRXServiceReqd(unsigned issp) { return sspTestStatusBit(issp,6); }
        //bool sspIsTXServiceReqd(unsigned issp) { return sspTestStatusBit(issp,5); }
        //bool sspIsBusy(unsigned issp) { return sspTestStatusBit(issp,4); }
        
        bool spiTestStatusBit(unsigned ispi, unsigned ibit) {
            return *REG::ptrMCSPI_chstat(ispi) & (0x1<<ibit);
        }

        bool spiTxFIFOisFull  (unsigned ispi) { return spiTestStatusBit(ispi,4); }
        bool spiTxFIFOisEmpty (unsigned ispi) { return spiTestStatusBit(ispi,3); }
        bool spiRxFIFOisFull  (unsigned ispi) { return spiTestStatusBit(ispi,6); }
        bool spiRxFIFOisEmpty (unsigned ispi) { return spiTestStatusBit(ispi,5); }
        bool spiEndofTransfer (unsigned ispi) { return spiTestStatusBit(ispi,2); }
        bool spiTxIsFull      (unsigned ispi) { return spiTestStatusBit(ispi,1); }
        bool spiRxIsFull      (unsigned ispi) { return spiTestStatusBit(ispi,0); }

        //void sspGetTXRXLevels(unsigned issp, unsigned& ntx, unsigned& nrx)
        //{
        //  unsigned val = *REG::ptrSSPStatus(issp);
        //  if(val & (0x1<<3))
        //      nrx = ((val>>12)&0xF)+1;
        //  else 
        //      nrx = 16;
        //  if(val & (0x1<<2))
        //      ntx = ((val>>8)&0xF);
        //  else 
        //      ntx = 16;
        //}

        //unsigned sspGetRXLevel(unsigned issp)
        //{
        //    unsigned val = *REG::ptrSSPStatus(issp);
        //    if(val & (0x1<<3))return ((val>>12)&0xF)+1;
        //    else return 16;
        //}

        //unsigned sspGetTXLevel(unsigned issp)
        //{
        //    unsigned val = *REG::ptrSSPStatus(issp);
        //    if(val & (0x1<<2))return ((val>>8)&0xF);
        //    else return 16;
        //}

        //bool sspTestAndWrite(unsigned issp, uint32_t data) {
        //    if(sspIsTXNotFull(issp)) {
        //        *REG::ptrSSPDataRW(issp) = data;
        //        return true;
        //    }
        //    else 
        //        return false;
        //}

        //void sspPollAndWrite(unsigned issp, uint32_t data)
        //{
        //    while(!sspIsTXNotFull(issp));
        //    *REG::ptrSSPDataRW(issp) = data;
        //}

        //bool sspTestAndRead(unsigned issp, uint32_t& data)
        //{
        //    if(sspIsRXNotEmpty(issp))
        //    {
        //        data = *REG::ptrSSPDataRW(issp);
        //        return true;
        //    }
        //    else return false;
        //}

        //uint32_t sspPollAndRead(unsigned issp)
        //{
        //    while(!sspIsRXNotEmpty(issp));
        //    return *REG::ptrSSPDataRW(issp);
        //}

        //uint32_t sspTestWritePollRead(unsigned issp, uint32_t data, bool& good)
        //{
        //    good = sspTestAndWrite(issp, data);
        //    if(!good)return 0;
        //    return sspPollAndRead(issp);
        //}

        //uint32_t sspTestWritePollRead(unsigned issp, uint32_t data)
        //{
        //    bool good;
        //    return sspTestWritePollRead(issp, data, good);
        //}

        //uint32_t sspPollWritePollRead(unsigned issp, uint32_t data)
        //{
        //    sspPollAndWrite(issp, data);
        //    return sspPollAndRead(issp);
        //}

        unsigned spiFlush(unsigned ispi) {
            unsigned nflush = 0;
            volatile uint32_t dummy;

            //Start Channel
            *REG::ptrMCSPI_chctrl(ispi) |= 0x1; 

            // flush rx fifo
            while(!spiRxFIFOisEmpty(ispi)) {
                dummy = *REG::ptrMCSPI_rx(ispi); 
                nflush++;
            }

            // flush tx fifo
            while(!spiTxFIFOisEmpty(ispi)) {
                dummy = *REG::ptrMCSPI_tx(ispi); 
                nflush++;
            }

            //Stop Channel
            *REG::ptrMCSPI_chctrl(ispi) &= 0x0; 

            return nflush; 
        }

        // --------------------------------------------------------------------------
        // Utility functions
        // --------------------------------------------------------------------------

        void loopDelay(unsigned nloop) const {
            for(volatile unsigned iloop=0;iloop<nloop;iloop++);
        }

        void usecDelay(unsigned nusec) const {
            loopDelay(nusec);
        }

        uint64_t serialNumber() const {
            std::ifstream stream("/proc/cpuinfo");
            if(!stream.good())return 0;
            std::string line;
            while(getline(stream,line))
            {
                std::string key;
                std::string::size_type ichar=0;
                while(ichar<line.size())
                {
                    char c = line[ichar];
                    if(c==':')break;
                    if(!isspace(c))key += tolower(c);
                    ichar++;
                }
                if(ichar<line.size())ichar++;
                while((ichar<line.size())&&(isspace(line[ichar])))ichar++;
                if((key == "serial")&&(ichar<line.size()))
                {
                    std::string val;
                    while((ichar<line.size())&&(isxdigit(line[ichar])))
                        val += line[ichar++];
                    std::istringstream sstream(val);
                    uint64_t serial = 0;
                    sstream >> std::hex >> serial;
                    return serial;
                }
            }
            return 0;
        }

};

//template<typename REG, typename REV> bool Overo<REG,REV>::
//sspConfigure(unsigned issp, 
//        unsigned serial_clock_rate,
//        unsigned data_size,
//        SSPFrameFormat frame_format,
//        uint32_t flags,
//        SSPClockSelect clock_source,
//        uint32_t rx_fifo_threshold,
//        uint32_t tx_fifo_threshold,
//        uint32_t network_mode_frame_rate_divider,
//        uint32_t psp_dmystop,
//        uint32_t psp_sfrmwdth,
//        uint32_t psp_sfrmdly,
//        uint32_t psp_dmystrt,
//        uint32_t psp_strtdly,
//        SSPPSPSerialClockMode psp_clock_mode,
//        uint32_t psp_flags)
//{
//    // Clear all the control register flags, disabling the port
//    *REG::ptrSSPControl0(issp) = 0;
//    *REG::ptrSSPControl1(issp) = 0;
//    *REG::ptrSSPPSPConfig(issp) = 0;
//    clockDisableSSP(issp);
//    sspClearStatusBits(issp);
//
//    uint32_t sscr0 = 0;
//    uint32_t sscr1 = 0;
//
//    // SSCR0 : BIT 31, 26:24
//    if((flags & SSP_FLAG_NETWORK_MODE) &&((frame_format == SSP_FF_TISSP)||(frame_format == SSP_FF_PSP)))
//    {
//        sscr0 |= (0x1<<31);
//        sscr0 |= (network_mode_frame_rate_divider&0x07)<<24;
//        if(frame_format == SSP_FF_PSP)
//        {
//            psp_sfrmdly  = 0;
//            psp_strtdly  = 0;
//            psp_dmystop  = 0;
//            psp_dmystrt  = 0;
//        }
//    }
//
//    // SSCR0 : BIT 30, 21, 06
//    if(flags & SSP_FLAG_SLAVE_TO_SCLKDIR)clock_source = SSP_CS_INTERNAL;
//    switch(clock_source)
//    {
//        case SSP_CS_INTERNAL:
//            break;
//        case SSP_CS_NETWORK:
//            sscr0 |= (0x1<<21);
//            break;
//        case SSP_EXTERNAL:
//            sscr0 |= (0x1<<21);
//            break;
//        case SSP_CS_AUDIO:
//            sscr0 |= (0x1<<30);
//            break;
//    }
//
//    // SSCR0 : BIT 29:27 - RESERVED
//    // SSCR0 : BIT 26:24 - ABOVE
//
//    // SSCR0 : BIT 23, 22
//    if(flags & SSP_FLAG_MASK_INT_TX_UNDERRUN)sscr0 |= (0x1<<23);
//    if(flags & SSP_FLAG_MASK_INT_RX_OVERRUN)sscr0 |= (0x1<<22);
//
//    // SSCR0 : BIT 21 - ABOVE
//
//    // SSCR0 : BIT 20, 03:00
//    if((data_size>32)||(data_size<4))return false;
//    data_size--;
//    if(data_size & 0x10)sscr0 |= (0x1<<20);
//    sscr0 |= (data_size&0x0F);
//
//    // SSCR0 : BIT 19:08
//    if((serial_clock_rate>4096)||(serial_clock_rate<1))return false;
//    serial_clock_rate--;
//    sscr0 |= (serial_clock_rate&0x0FFF)<<8;
//
//    // SSCR0 : BIT 07
//    if(flags & SSP_FLAG_SSP_ENABLE)sscr0 |= (0x1<<7);
//
//    // SSCR0 : BIT 06 - ABOVE
//
//    // SSCR0 : BIT 05:04
//    switch(frame_format)
//    {
//        case SSP_FF_SPI:
//            sscr0 |= (0x00<<4);
//            break;
//        case SSP_FF_TISSP:
//            sscr0 |= (0x01<<4);
//            break;
//        case SSP_FF_MICROWIRE:
//            sscr0 |= (0x02<<4);
//            break;
//        case SSP_FF_PSP:
//            sscr0 |= (0x03<<4);
//            break;
//    }
//
//    // SSCR0 : BIT 03:00 - ABOVE
//
//    // SSCR1 : BIT 31, 30
//    if(flags & SSP_FLAG_TXD_TRISTATE)
//    {
//        sscr1 |= (0x1<<30);
//        if((flags & SSP_FLAG_TXD_TRISTATE_ON_LAST_PHASE)&&
//                (frame_format != SSP_FF_SPI)&&(frame_format != SSP_FF_MICROWIRE))
//            sscr1 |= (0x1<<31);
//    }
//
//    // SSCR1 : BIT 29
//    if(flags & SSP_FLAG_ENABLE_INT_BIT_COUNT_ERROR)sscr1 |= (0x1<<29);
//
//    // SSCR1 : BIT 28
//    if((flags & SSP_FLAG_SLAVE_CLOCK_FREE_RUNNING)
//            &&(flags & SSP_FLAG_SLAVE_TO_SCLKDIR))sscr1 |= (0x1<<28);
//
//    // SSCR1 : BIT 27, 26
//    if(flags & SSP_FLAG_ENABLE_CLOCK_REQUEST_A)sscr1 |= (0x1<<27);
//    if(flags & SSP_FLAG_ENABLE_CLOCK_REQUEST_B)sscr1 |= (0x1<<26);
//
//    // SSCR1 : BIT 25, 24
//    if(flags & SSP_FLAG_SLAVE_TO_SCLKDIR)sscr1 |= (0x1<<25);
//    if(flags & SSP_FLAG_SLAVE_TO_SFRMDIR)sscr1 |= (0x1<<24);
//
//    // SSCR1 : BIT 23
//    if((flags & SSP_FLAG_RX_WITHOUT_TX)&&((sscr0&(0x1<<31))==0))
//        sscr1 |= (0x1<<23);
//
//    // SSCR1 : BIT 22
//    if(flags & SSP_FLAG_TRAILING_BYTES_HANDLED_BY_DMA)sscr1 |= (0x1<<22);
//
//    // SSCR1 : BIT 21, 20
//    if(flags & SSP_FLAG_ENABLE_DMA_TX_SERVICE_REQ)sscr1 |= (0x1<<21);
//    if(flags & SSP_FLAG_ENABLE_DMA_RX_SERVICE_REQ)sscr1 |= (0x1<<20);
//
//    // SSCR1 : BIT 19, 18
//    if(flags & SSP_FLAG_ENABLE_INT_RX_TIME_OUT)sscr1 |= (0x1<<19);
//    if(flags & SSP_FLAG_ENABLE_INT_PERIPHERAL_TRAILING_BYTE)sscr1 |= (0x1<<18);
//
//    // SSCR1 : BIT 17 - RESERVED
//
//    // SSCR1 : BIT 16
//    if(flags & SSP_FLAG_INVERT_FRAME_SIGNAL)sscr1 |= (0x1<<17);
//
//    // SSCR1 : BIT 15, 14
//    if(flags & SSP_FLAG_ENABLE_FIFO_TEST_MODE)
//    {
//        sscr1 |= (0x1<<14);
//        if(flags & SSP_FLAG_SELECT_RX_FIFO_FOR_TEST_MODE)sscr1 |= (0x1<<15);
//    }
//
//    // SSCR1 : BIT 13:10, 09:06
//    if((rx_fifo_threshold<1)||(rx_fifo_threshold>16)||
//            (tx_fifo_threshold<1)||(tx_fifo_threshold>16))return false;
//    rx_fifo_threshold--;
//    tx_fifo_threshold--;
//    sscr1 |= (rx_fifo_threshold&0xF)<<10;
//    sscr1 |= (tx_fifo_threshold&0xF)<<6;
//
//    // SSCR1 : BIT 05
//    if((frame_format == SSP_FF_MICROWIRE)
//            &&(flags & SSP_FLAG_MICROWIRE_16BIT_DATA_SIZE))sscr1 |= (0x1<<5);
//
//    // SSCR1 : BIT 04, 03
//    if(frame_format == SSP_FF_SPI)
//    {
//        if(flags & SSP_FLAG_SPH)
//            sscr1 |= (0x1<<4);
//        if(flags & SSP_FLAG_SPO)
//            sscr1 |= (0x1<<3);
//    }
//
//    // SSCR1 : BIT 02
//    if((frame_format != SSP_FF_MICROWIRE)
//            &&(flags & SSP_FLAG_LOOP_BACK_MODE))sscr1 |= (0x1<<2);
//
//    // SSCR1 : BIT 01, 00
//    if(flags & SSP_FLAG_ENABLE_INT_TX_FIFO)sscr1 |= (0x1<<1);
//    if(flags & SSP_FLAG_ENABLE_INT_RX_FIFO)sscr1 |= (0x1<<0);
//
//    if(frame_format == SSP_FF_PSP)
//    {
//        uint32_t sspsp = 0;
//
//        // SSPSP : BIT 31:26 - RESERVED
//
//        // SSPSP : BIT 25
//        if(psp_flags & SSP_PSP_FLAG_FSRT)
//        {
//            sspsp |= (0x1<<25);
//            psp_dmystop = 0;
//        }
//
//        // SSPSP : BIT 24:23
//        if(psp_dmystop>3)return false;
//        sspsp |= psp_dmystop<<23;
//
//        // SSPSP : BIT 22 - RESERVED
//
//        // SSPSP : BIT 21:16
//        if((psp_sfrmwdth<1)||(psp_sfrmwdth>44))return false;
//        sspsp |= psp_sfrmwdth<<16;
//
//        // SSPSP : BIT 15:09
//        if(psp_sfrmdly>88)return false;
//        sspsp |= psp_sfrmdly<<9;
//
//        // SSPSP : BIT 08:07
//        if(psp_dmystrt>3)return false;
//        sspsp |= psp_dmystrt<<7;
//
//        // SSPSP : BIT 06:04
//        if((flags & SSP_FLAG_SLAVE_TO_SCLKDIR)
//                ||(flags & SSP_FLAG_SLAVE_TO_SFRMDIR)
//                ||(SSP_FLAG_ENABLE_CLOCK_REQUEST_A)
//                ||(SSP_FLAG_ENABLE_CLOCK_REQUEST_B))psp_strtdly=0;
//        if(psp_strtdly>7)return false;
//        sspsp |= psp_strtdly<<4;
//
//        // SSPSP : BIT 03
//        if((psp_flags & SSP_PSP_FLAG_ETDS)
//                &&((flags&SSP_FLAG_TXD_TRISTATE)==0))sspsp |= (0x1<<3);
//
//        // SSPSP : BIT 02
//        if(psp_flags & SSP_PSP_FLAG_SFRMP)sspsp |= (0x1<<2);
//
//        // SSPSP : BIT 01:00
//        switch(psp_clock_mode)
//        {
//            case SSP_PSP_SCMODE_FRL: sspsp |= 0x00; break;
//            case SSP_PSP_SCMODE_RFL: sspsp |= 0x01; break;
//            case SSP_PSP_SCMODE_RFH: sspsp |= 0x02; break;
//            case SSP_PSP_SCMODE_FRH: sspsp |= 0x03; break;
//        }
//
//        *REG::ptrSSPPSPConfig(issp) = sspsp;
//    }
//
//    *REG::ptrSSPControl1(issp) = sscr1;
//    if(flags & SSP_FLAG_SSP_ENABLE)clockEnableSSP(issp);
//    *REG::ptrSSPControl0(issp) = sscr0;
//
//    return true;
//}

#endif // ndef VERDEXXM4PHYS_HPP
