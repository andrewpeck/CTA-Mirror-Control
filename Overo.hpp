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

#include <Bits.hpp>
#include <Registers.hpp>

#define N(x) (sizeof(x)/sizeof(*x))
class Rev1511
{
    public:

        //Maps Overo output pins (J1 1-70, J 71-140) to GPIO Pins
        static int pin140ToGPIO(unsigned ipin140 /* STARTS FROM ONE */) {
            static const int igpio[] =  {
                -1,  //N_MANUAL_RESET      1 
                71,  //GPIO71_L_DD01       2
                70,  //GPIO70_L_DD00       3
                73,  //GPIO73_L_DD03       4
                75,  //GPIO75_L_DD05       5
                72,  //GPIO72_L_DD02       6
                74,  //GPIO74_L_DD04       7
                10,  //GPIO_10             8
                -1,  //GPIO0_WAKEUP        9
                185, //GPIO185_I2C3_SDA    10
                80,  //GPIO80_L_DD10       11
                81,  //GPIO81_L_DD11       12
                184, //GPIO184_I2C3_SCL    13
                186, //GPIO_186            14
                92,  //GPIO92_L_DD22       15
                147, //GPIO147_GPT8_PWM    16
                83,  //GPIO83_L_DD13       17
                144, //GPIO144_GPT9_PWM    18
                84,  //GPIO84_L_DD14       19
                85,  //GPIO85_L_DD15       20
                146, //GPIO146_GPT11_PWM   21
                163, //GPIO163_IR_CTS3     22
                91,  //GPIO91_L_DD21       23
                87,  //GPIO87_L_DD17       24
                88,  //GPIO88_L_DD18       25
                166, //GPIO166_IR_TXD3     26
                89,  //GPIO89_L_DD19       27
                79,  //GPIO79_L_DD09       28
                77,  //GPIO77_L_DD07       29
                78,  //GPIO78_L_DD08       30
                165, //GPIO165_IR_RXD3     31
                66,  //GPIO66_L_PCLK       32
                76,  //GPIO76_L_DD06       33
                68,  //GPIO68_L_FCLK       34
                67,  //GPIO67_L_LCLK       35
                -1,  //USBOTG_DP           36  
                -1,  //USBOTG_DM           37  
                -1,  //AUXLF               38  
                -1,  //MIC_SUB_MF          39  
                -1,  //ADCIN4              40  
                -1,  //AUXRF               41  
                -1,  //PWM0                42  
                69,  //GPIO69_L_BIAS       43  
                86,  //GPIO86_L_DD16       44  
                90,  //GPIO90_L_DD20       45  
                -1,  //USBOTG_ID           46  
                170, //GPIO170_HDQ_1WIRE   47  
                -1,  //ADCIN3              48  
                -1,  //PWM1                49  
                -1,  //AGND                50  
                -1,  //ADCIN5              51  
                -1,  //VBACKUP             52  
                -1,  //ADCIN6              53  
                -1,  //USBOTG_VBUS         54  
                145, //GPIO145_GPT10_PWM   55  
                -1,  //GND                 56  
                -1,  //MIC_MAIN_MF         57  
                -1,  //ADCIN2              58  
                -1,  //SYSEN               59  
                82,  //GPIO82_L_DD12       60  
                93,  //GPIO93_L_DD23       61  
                -1,  //TV_OUT2             62  
                -1,  //TV_OUT1             63  
                -1,  //ADCIN7              64  
                -1,  //POWERON             65  
                -1,  //VSYSTEM             66  
                -1,  //VSYSTEM             67  
                -1,  //HSOLF               68  
                -1,  //HSORF               69  
                -1,  //GND                 70  

                ////Connector J4 (70-pin): Extended Memory Bus & MMC Signals
                -1,  //VSYSTEM             71 
                -1,  //VSYSTEM             72 
                -1,  //GND                 73 
                -1,  //EM_NCS5_ETH0        74 
                -1,  //EM_NCS4             75 
                -1,  //EM_NWE              76 
                -1,  //EM_NADV_ALE         77 
                -1,  //EM_NOE              78 
                65,  //GPIO65_ETH1_IRQ1    79 
                64,  //GPIO64_ETH0_NRESET  80 
                -1,  //EM_A2               81 
                -1,  //EM_A8               82 
                -1,  //EM_A5               83 
                -1,  //EM_A7               84 
                -1,  //EM_D2               85 
                -1,  //EM_D10              86 
                -1,  //EM_D3               87 
                -1,  //EM_D11              88 
                -1,  //EM_D4               89 
                -1,  //EM_D12              90 
                -1,  //EM_D5               91 
                -1,  //EM_D15              92 
                13,  //GPIO13_MMC3_CMD     93 
                148, //GPIO148_TXD1        94 
                176, //GPIO176_ETH0_IRQ    95 
                18,  //GPIO18_MMC3_D0      96 
                174, //GPIO174_SPI1_CS0    97 
                168, //GPIO168_USBH_CPEN   98 
                14,  //GPIO14_MMC3_DAT4    99 
                21,  //GPIO21_MMC3_DAT7    100
                17,  //GPIO17_MMC3_D3      101
                -1,  //USBH_VBUS           102
                -1,  //GND                 103
                -1,  //USBH_DP             104
                -1,  //USBH_DM             105
                19,  //GPIO19_MMC3_D1      106
                22,  //GPIO22_MMC3_DAT6    107
                23,  //GPIO23_MMC3_DAT5    108
                20,  //GPIO20_MMC3_D2      109
                12,  //GPIO12_MMC3_CLK     110
                114, //GPIO114_SPI1_NIRQ   111
                175, //GPIO175_SPI1_CS1    112
                171, //GPIO171_SPI1_CLK    113
                172, //GPIO172_SPI1_MOSI   114
                173, //GPIO173_SPI1_MISO   115
                -1,  //4030GP2_N_MMC3_CD   116
                150, //GPIO150_MMC3_WP     117
                151, //GPIO151_RXD1        118
                -1,  //EM_D7               119
                -1,  //EM_D14              120
                -1,  //EM_D6               121
                -1,  //EM_D13              122
                -1,  //EM_D1               123
                -1,  //EM_D8               124
                -1,  //EM_D9               125
                -1,  //EM_D0               126
                -1,  //EM_A6               127
                -1,  //EM_A1               128
                -1,  //EM_A3               129
                -1,  //EM_A10              130
                -1,  //EM_A4               131
                -1,  //EM_A9               132
                -1,  //EM_NWP              133
                -1,  //EM_NCS1             134
                -1,  //EM_NBE0             135
                -1,  //EM_NCS0             136
                -1,  //EM_NCS6             137
                -1,  //EM_WAIT0            138
                -1,  //EM_NBE1             139
                -1,  //EM_CLK              140
            }

            if((ipin140>0)&&((--ipin140)<N(igpio)))
                return igpio[ipin140];
            else return -1;
        }
};

typedef Rev1511 CurrentRev;

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

        enum GPIODir { 
            GPIODIR_IN = 0, 
            GPIODIR_OUT 
        };

        enum GPIOFn { 
            GPIOFN_IO = 0, 
            GPIOFN_ALT1, 
            GPIOFN_ALT2, 
            GPIOFN_ALT3 
        };

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

template<typename REG = MappedRegisters, typename REV = CurrentRev> 
class Overo: public OveroBase, public REV, public REG
{
    public:
        Overo(): OveroBase(), REV(), REG() { }
        ~Overo() { }

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

        // Enable MCSPI Clocks
        void clockEnableMCSPI1(bool enable = true) { clockSetEnableBit(/* something */,enable); }
        void clockEnableMCSPI2(bool enable = true) { clockSetEnableBit(/* something */,enable); }
        void clockEnableMCSPI3(bool enable = true) { clockSetEnableBit(/* something */,enable); }
        void clockEnableMCSPI4(bool enable = true) { clockSetEnableBit(/* something */,enable); }

        // Disable MCSPI Clocks
        void clockDisableMCSPI1() { clockEnableSSP1(false); }
        void clockDisableMCSPI2() { clockEnableSSP2(false); }
        void clockDisableMCSPI3() { clockEnableSSP3(false); }
        void clockDisableMCSPI4() { clockEnableSSP3(false); }

        // Enable any MCSPI Clock by ispi number 
        void clockEnableMCSPI(unsigned ispi, bool enable = true) {
            if(ispi<4)
                clockEnableMCSPI1(enable);
            else if(ispi<6)
                clockEnableMCSPI2(enable);
            else if(ispi<8)
                clockEnableMCSPI3(enable);
            else if(ispi==0)
                clockEnableMCSPI4(enable);
            else
                return; 
        }

        // Disable ALL MCSPI Clocks
        void clockDisableMCSPI(unsigned ispi) { clockEnableSSP(issp,false); }

        // Check Enable/Disable Status of MCSPI Clock
        bool clockIsEnabledSSP1() { return clockIsBitEnabled(/* some number */); }
        bool clockIsEnabledSSP2() { return clockIsBitEnabled(/* some number */); }
        bool clockIsEnabledSSP3() { return clockIsBitEnabled(/* some number */); }
        bool clockIsEnabledSSP4() { return clockIsBitEnabled(/* some number */); }

        // Check Enable/Disable Status of any MCSPI Clock by ispi number
        bool clockIsEnabledSSP(unsigned ispi) {
            if(ispi<4)
                return clockIsEnabledMCSPI1();
            else if(ispi<6)
                return clockIsEnabledMCSPI2();
            else if(ispi<8)
                return clockIsEnabledMCSPI3();
            else if(ispi==0)
                return clockIsEnabledMCSPI4();
            else
                return; 
        }

        // --------------------------------------------------------------------------
        // GPIO utility functions
        // --------------------------------------------------------------------------

        // Read GPIO by ipin (0-192)
        bool gpioReadLevel(const unsigned ipin) {
            return *(REG::ptrGPIOReadLevel(ipin)) & Bits::mask1Bit(ipin);
        }

        // Write GPIO by ipin (0-192)
        void gpioWriteLevel(const unsigned ipin, bool level) {
            if (level)
                REG::gpioSetLevel(ipin);
            else 
                REG::gpioClrLevel(ipin);
        }

        // Get GPIO Direction In/Out
        GPIODir gpioGetDirection(const unsigned ipin) {
            volatile uint32_t* reg = REG::ptrGPIODirection(ipin);
            uint32_t val = *reg;
            if(val & Bits::mask1Bit(ipin))
                return GPIODIR_OUT;
            else 
                return GPIODIR_IN;
        }

        // Set GPIO Direction In/Out
        void gpioSetDirection(const unsigned ipin, const GPIODir dir) {
            volatile uint32_t* reg = REG::ptrGPIODirection(ipin);
            uint32_t val = *reg;
            if(dir==GPIODIR_OUT)
                val |= Bits::mask1Bit(ipin);
            else 
                val &= ~Bits::mask1Bit(ipin);
            *reg = val;
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
        void gpioConfigure(const unsigned ipin, const GPIODir dir, const GPIOFn fn) {
            gpioSetDirection(ipin, dir);
            gpioSetAltFunc(ipin, fn);
        }

        // --------------------------------------------------------------------------
        // SSP utility functions
        // --------------------------------------------------------------------------

        bool sspConfigure(unsigned issp, 
                unsigned serial_clock_rate = 1,
                unsigned data_size = 32,
                SSPFrameFormat frame_format = SSP_FF_SPI,
                uint32_t flags = SSP_FLAG_NONE,
                SSPClockSelect clock_source = SSP_CS_INTERNAL,
                uint32_t rx_fifo_threshold = 1,
                uint32_t tx_fifo_threshold = 1,
                uint32_t network_mode_frame_rate_divider = 0,
                uint32_t psp_dmystop = 0,
                uint32_t psp_sfrmwdth = 1,
                uint32_t psp_sfrmdly = 0,
                uint32_t psp_dmystrt = 0,
                uint32_t psp_strtdly = 0,
                SSPPSPSerialClockMode psp_clock_mode = SSP_PSP_SCMODE_FRL,
                uint32_t psp_flags = SSP_PSP_FLAG_NONE);

        void sspEnable(unsigned issp, bool enable = true) {
            if(enable) {
                clockEnableSSP(issp);
                *REG::ptrSSPControl0(issp) |= (0x1<<7);
            }
            else {
                *REG::ptrSSPControl0(issp) &= ~(0x1<<7);
                clockDisableSSP(issp);
            }
        }

        void sspDisable(unsigned issp) { 
            sspEnable(issp,false);
        }

        void sspClearStatusBits(unsigned issp) {
            *REG::ptrSSPStatus(issp) = (0x1<<23)|(0x1<<21)|(0x1<<20)|(0x1<<19)|(0x1<<18)|(0x1<<7);
        }

        bool sspTestStatusBit(unsigned issp, unsigned ibit) {
            return *REG::ptrSSPStatus(issp) & (0x1<<ibit);
        }

        bool sspIsBitCountError(unsigned issp) { return sspTestStatusBit(issp,23); }
        bool sspIsClockSynchBad(unsigned issp) { return sspTestStatusBit(issp,22); }
        bool sspIsTXUnderrun(unsigned issp) { return sspTestStatusBit(issp,21); }
        bool sspIsEndOfDMAChain(unsigned issp) { return sspTestStatusBit(issp,20); }
        bool sspIsTimeOutInt(unsigned issp) { return sspTestStatusBit(issp,19); }
        bool sspIsTrailingByteInt(unsigned issp) { return sspTestStatusBit(issp,18); }
        bool sspIsRXOverrun(unsigned issp) { return sspTestStatusBit(issp,7); }
        bool sspIsRXServiceReqd(unsigned issp) { return sspTestStatusBit(issp,6); }
        bool sspIsTXServiceReqd(unsigned issp) { return sspTestStatusBit(issp,5); }
        bool sspIsBusy(unsigned issp) { return sspTestStatusBit(issp,4); }
        bool sspIsRXNotEmpty(unsigned issp) { return sspTestStatusBit(issp,3); }
        bool sspIsTXNotFull(unsigned issp) { return sspTestStatusBit(issp,2); }

        void sspGetTXRXLevels(unsigned issp, unsigned& ntx, unsigned& nrx)
        {
            unsigned val = *REG::ptrSSPStatus(issp);
            if(val & (0x1<<3))nrx = ((val>>12)&0xF)+1;
            else nrx = 16;
            if(val & (0x1<<2))ntx = ((val>>8)&0xF);
            else ntx = 16;
        }

        unsigned sspGetRXLevel(unsigned issp)
        {
            unsigned val = *REG::ptrSSPStatus(issp);
            if(val & (0x1<<3))return ((val>>12)&0xF)+1;
            else return 16;
        }

        unsigned sspGetTXLevel(unsigned issp)
        {
            unsigned val = *REG::ptrSSPStatus(issp);
            if(val & (0x1<<2))return ((val>>8)&0xF);
            else return 16;
        }

        bool sspTestAndWrite(unsigned issp, uint32_t data) {
            if(sspIsTXNotFull(issp)) {
                *REG::ptrSSPDataRW(issp) = data;
                return true;
            }
            else 
                return false;
        }

        void sspPollAndWrite(unsigned issp, uint32_t data)
        {
            while(!sspIsTXNotFull(issp));
            *REG::ptrSSPDataRW(issp) = data;
        }

        bool sspTestAndRead(unsigned issp, uint32_t& data)
        {
            if(sspIsRXNotEmpty(issp))
            {
                data = *REG::ptrSSPDataRW(issp);
                return true;
            }
            else return false;
        }

        uint32_t sspPollAndRead(unsigned issp)
        {
            while(!sspIsRXNotEmpty(issp));
            return *REG::ptrSSPDataRW(issp);
        }

        uint32_t sspTestWritePollRead(unsigned issp, uint32_t data, bool& good)
        {
            good = sspTestAndWrite(issp, data);
            if(!good)return 0;
            return sspPollAndRead(issp);
        }

        uint32_t sspTestWritePollRead(unsigned issp, uint32_t data)
        {
            bool good;
            return sspTestWritePollRead(issp, data, good);
        }

        uint32_t sspPollWritePollRead(unsigned issp, uint32_t data)
        {
            sspPollAndWrite(issp, data);
            return sspPollAndRead(issp);
        }

        unsigned sspFlush(unsigned issp)
        {
            unsigned nflush = 0;
            do
            {
                volatile uint32_t dummy;
                while(sspIsRXNotEmpty(issp))dummy = *REG::ptrSSPDataRW(issp), nflush++;
            }while(sspIsBusy(issp));
            return nflush;
        }

        // --------------------------------------------------------------------------
        // Utility functions
        // --------------------------------------------------------------------------

        void loopDelay(unsigned nloop) const
        {
            for(volatile unsigned iloop=0;iloop<nloop;iloop++);
        }

        void usecDelay(unsigned nusec) const
        {
            loopDelay(nusec);
        }

        uint64_t serialNumber() const
        {
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

template<typename REG, typename REV> bool Overo<REG,REV>::
sspConfigure(unsigned issp, 
        unsigned serial_clock_rate,
        unsigned data_size,
        SSPFrameFormat frame_format,
        uint32_t flags,
        SSPClockSelect clock_source,
        uint32_t rx_fifo_threshold,
        uint32_t tx_fifo_threshold,
        uint32_t network_mode_frame_rate_divider,
        uint32_t psp_dmystop,
        uint32_t psp_sfrmwdth,
        uint32_t psp_sfrmdly,
        uint32_t psp_dmystrt,
        uint32_t psp_strtdly,
        SSPPSPSerialClockMode psp_clock_mode,
        uint32_t psp_flags)
{
    // Clear all the control register flags, disabling the port
    *REG::ptrSSPControl0(issp) = 0;
    *REG::ptrSSPControl1(issp) = 0;
    *REG::ptrSSPPSPConfig(issp) = 0;
    clockDisableSSP(issp);
    sspClearStatusBits(issp);

    uint32_t sscr0 = 0;
    uint32_t sscr1 = 0;

    // SSCR0 : BIT 31, 26:24
    if((flags & SSP_FLAG_NETWORK_MODE) &&((frame_format == SSP_FF_TISSP)||(frame_format == SSP_FF_PSP)))
    {
        sscr0 |= (0x1<<31);
        sscr0 |= (network_mode_frame_rate_divider&0x07)<<24;
        if(frame_format == SSP_FF_PSP)
        {
            psp_sfrmdly  = 0;
            psp_strtdly  = 0;
            psp_dmystop  = 0;
            psp_dmystrt  = 0;
        }
    }

    // SSCR0 : BIT 30, 21, 06
    if(flags & SSP_FLAG_SLAVE_TO_SCLKDIR)clock_source = SSP_CS_INTERNAL;
    switch(clock_source)
    {
        case SSP_CS_INTERNAL:
            break;
        case SSP_CS_NETWORK:
            sscr0 |= (0x1<<21);
            break;
        case SSP_EXTERNAL:
            sscr0 |= (0x1<<21);
            break;
        case SSP_CS_AUDIO:
            sscr0 |= (0x1<<30);
            break;
    }

    // SSCR0 : BIT 29:27 - RESERVED
    // SSCR0 : BIT 26:24 - ABOVE

    // SSCR0 : BIT 23, 22
    if(flags & SSP_FLAG_MASK_INT_TX_UNDERRUN)sscr0 |= (0x1<<23);
    if(flags & SSP_FLAG_MASK_INT_RX_OVERRUN)sscr0 |= (0x1<<22);

    // SSCR0 : BIT 21 - ABOVE

    // SSCR0 : BIT 20, 03:00
    if((data_size>32)||(data_size<4))return false;
    data_size--;
    if(data_size & 0x10)sscr0 |= (0x1<<20);
    sscr0 |= (data_size&0x0F);

    // SSCR0 : BIT 19:08
    if((serial_clock_rate>4096)||(serial_clock_rate<1))return false;
    serial_clock_rate--;
    sscr0 |= (serial_clock_rate&0x0FFF)<<8;

    // SSCR0 : BIT 07
    if(flags & SSP_FLAG_SSP_ENABLE)sscr0 |= (0x1<<7);

    // SSCR0 : BIT 06 - ABOVE

    // SSCR0 : BIT 05:04
    switch(frame_format)
    {
        case SSP_FF_SPI:
            sscr0 |= (0x00<<4);
            break;
        case SSP_FF_TISSP:
            sscr0 |= (0x01<<4);
            break;
        case SSP_FF_MICROWIRE:
            sscr0 |= (0x02<<4);
            break;
        case SSP_FF_PSP:
            sscr0 |= (0x03<<4);
            break;
    }

    // SSCR0 : BIT 03:00 - ABOVE

    // SSCR1 : BIT 31, 30
    if(flags & SSP_FLAG_TXD_TRISTATE)
    {
        sscr1 |= (0x1<<30);
        if((flags & SSP_FLAG_TXD_TRISTATE_ON_LAST_PHASE)&&
                (frame_format != SSP_FF_SPI)&&(frame_format != SSP_FF_MICROWIRE))
            sscr1 |= (0x1<<31);
    }

    // SSCR1 : BIT 29
    if(flags & SSP_FLAG_ENABLE_INT_BIT_COUNT_ERROR)sscr1 |= (0x1<<29);

    // SSCR1 : BIT 28
    if((flags & SSP_FLAG_SLAVE_CLOCK_FREE_RUNNING)
            &&(flags & SSP_FLAG_SLAVE_TO_SCLKDIR))sscr1 |= (0x1<<28);

    // SSCR1 : BIT 27, 26
    if(flags & SSP_FLAG_ENABLE_CLOCK_REQUEST_A)sscr1 |= (0x1<<27);
    if(flags & SSP_FLAG_ENABLE_CLOCK_REQUEST_B)sscr1 |= (0x1<<26);

    // SSCR1 : BIT 25, 24
    if(flags & SSP_FLAG_SLAVE_TO_SCLKDIR)sscr1 |= (0x1<<25);
    if(flags & SSP_FLAG_SLAVE_TO_SFRMDIR)sscr1 |= (0x1<<24);

    // SSCR1 : BIT 23
    if((flags & SSP_FLAG_RX_WITHOUT_TX)&&((sscr0&(0x1<<31))==0))
        sscr1 |= (0x1<<23);

    // SSCR1 : BIT 22
    if(flags & SSP_FLAG_TRAILING_BYTES_HANDLED_BY_DMA)sscr1 |= (0x1<<22);

    // SSCR1 : BIT 21, 20
    if(flags & SSP_FLAG_ENABLE_DMA_TX_SERVICE_REQ)sscr1 |= (0x1<<21);
    if(flags & SSP_FLAG_ENABLE_DMA_RX_SERVICE_REQ)sscr1 |= (0x1<<20);

    // SSCR1 : BIT 19, 18
    if(flags & SSP_FLAG_ENABLE_INT_RX_TIME_OUT)sscr1 |= (0x1<<19);
    if(flags & SSP_FLAG_ENABLE_INT_PERIPHERAL_TRAILING_BYTE)sscr1 |= (0x1<<18);

    // SSCR1 : BIT 17 - RESERVED

    // SSCR1 : BIT 16
    if(flags & SSP_FLAG_INVERT_FRAME_SIGNAL)sscr1 |= (0x1<<17);

    // SSCR1 : BIT 15, 14
    if(flags & SSP_FLAG_ENABLE_FIFO_TEST_MODE)
    {
        sscr1 |= (0x1<<14);
        if(flags & SSP_FLAG_SELECT_RX_FIFO_FOR_TEST_MODE)sscr1 |= (0x1<<15);
    }

    // SSCR1 : BIT 13:10, 09:06
    if((rx_fifo_threshold<1)||(rx_fifo_threshold>16)||
            (tx_fifo_threshold<1)||(tx_fifo_threshold>16))return false;
    rx_fifo_threshold--;
    tx_fifo_threshold--;
    sscr1 |= (rx_fifo_threshold&0xF)<<10;
    sscr1 |= (tx_fifo_threshold&0xF)<<6;

    // SSCR1 : BIT 05
    if((frame_format == SSP_FF_MICROWIRE)
            &&(flags & SSP_FLAG_MICROWIRE_16BIT_DATA_SIZE))sscr1 |= (0x1<<5);

    // SSCR1 : BIT 04, 03
    if(frame_format == SSP_FF_SPI)
    {
        if(flags & SSP_FLAG_SPH)
            sscr1 |= (0x1<<4);
        if(flags & SSP_FLAG_SPO)
            sscr1 |= (0x1<<3);
    }

    // SSCR1 : BIT 02
    if((frame_format != SSP_FF_MICROWIRE)
            &&(flags & SSP_FLAG_LOOP_BACK_MODE))sscr1 |= (0x1<<2);

    // SSCR1 : BIT 01, 00
    if(flags & SSP_FLAG_ENABLE_INT_TX_FIFO)sscr1 |= (0x1<<1);
    if(flags & SSP_FLAG_ENABLE_INT_RX_FIFO)sscr1 |= (0x1<<0);

    if(frame_format == SSP_FF_PSP)
    {
        uint32_t sspsp = 0;

        // SSPSP : BIT 31:26 - RESERVED

        // SSPSP : BIT 25
        if(psp_flags & SSP_PSP_FLAG_FSRT)
        {
            sspsp |= (0x1<<25);
            psp_dmystop = 0;
        }

        // SSPSP : BIT 24:23
        if(psp_dmystop>3)return false;
        sspsp |= psp_dmystop<<23;

        // SSPSP : BIT 22 - RESERVED

        // SSPSP : BIT 21:16
        if((psp_sfrmwdth<1)||(psp_sfrmwdth>44))return false;
        sspsp |= psp_sfrmwdth<<16;

        // SSPSP : BIT 15:09
        if(psp_sfrmdly>88)return false;
        sspsp |= psp_sfrmdly<<9;

        // SSPSP : BIT 08:07
        if(psp_dmystrt>3)return false;
        sspsp |= psp_dmystrt<<7;

        // SSPSP : BIT 06:04
        if((flags & SSP_FLAG_SLAVE_TO_SCLKDIR)
                ||(flags & SSP_FLAG_SLAVE_TO_SFRMDIR)
                ||(SSP_FLAG_ENABLE_CLOCK_REQUEST_A)
                ||(SSP_FLAG_ENABLE_CLOCK_REQUEST_B))psp_strtdly=0;
        if(psp_strtdly>7)return false;
        sspsp |= psp_strtdly<<4;

        // SSPSP : BIT 03
        if((psp_flags & SSP_PSP_FLAG_ETDS)
                &&((flags&SSP_FLAG_TXD_TRISTATE)==0))sspsp |= (0x1<<3);

        // SSPSP : BIT 02
        if(psp_flags & SSP_PSP_FLAG_SFRMP)sspsp |= (0x1<<2);

        // SSPSP : BIT 01:00
        switch(psp_clock_mode)
        {
            case SSP_PSP_SCMODE_FRL: sspsp |= 0x00; break;
            case SSP_PSP_SCMODE_RFL: sspsp |= 0x01; break;
            case SSP_PSP_SCMODE_RFH: sspsp |= 0x02; break;
            case SSP_PSP_SCMODE_FRH: sspsp |= 0x03; break;
        }

        *REG::ptrSSPPSPConfig(issp) = sspsp;
    }

    *REG::ptrSSPControl1(issp) = sscr1;
    if(flags & SSP_FLAG_SSP_ENABLE)clockEnableSSP(issp);
    *REG::ptrSSPControl0(issp) = sscr0;

    return true;
}

#endif // ndef VERDEXXM4PHYS_HPP
