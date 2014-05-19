////////////////////////////////////////////////////////////////////////////////
// Classes which have mapped registers for Gumstix Verdex XM4 architecture
////////////////////////////////////////////////////////////////////////////////

#ifndef REGISTERS_HPP
#define REGISTERS_HPP

#include <sys/mman.h>
#include <stdint.h>

#include <Bits.hpp>


// SPI1 supports up to four peripherals, SPI2 and SPI3 support up to two
// peripherals, and SPI4 supports only one peripheral.
// channel address = base address + 0x14 * x
// x= 0 to 3 for MCSPI1.
// x= 0 to 1 for MCSPI2 and MCSPI3.
// x= 0      for MCSPI4.
// in software let's enumerate these lines: 0,1,2,3,4,5,6,7,8

static const int channel[] =  {0,1,2,3,0,1,0,1,0}; 

class MappedRegisters
{
    public:
        MappedRegisters();
        ~MappedRegisters();

        // --------------------------------------------------------------------------
        // STATIC CLOCK manager register (PHYSICAL) address functions
        // --------------------------------------------------------------------------


        // Consider page 402 CM_CLKOUT_CTL (Clock Out Control Register)

        //static off_t physBaseClock()                    { return 0x48004D00; }

        //static off_t cm_clken_pll_offset()              { return 0x0000; }
        //static off_t cm_clken2_pll_offset()             { return 0x0004; }
        //static off_t cm_idlest_ckgen_offset()           { return 0x0020; }
        //static off_t cm_idlest2_ckgen_offset()          { return 0x0024; }
        //static off_t cm_autoidle_pll_offset()           { return 0x0030; }
        //static off_t cm_autoidle2_pll_offset()          { return 0x0034; }
        //static off_t cm_clksel1_pll_offset()            { return 0x0040; }
        //static off_t cm_clksel2_pll_offset()            { return 0x0044; }
        //static off_t cm_clksel3_pll_offset()            { return 0x0048; }
        //static off_t cm_clksel4_pll_offset()            { return 0x004C; }
        //static off_t cm_clksel5_pll_offset()            { return 0x0050; }
        //static off_t cm_clkout_ctrl_offset()            { return 0x0070; }


        // --------------------------------------------------------------------------
        // STATIC GPIO register (PHYSICAL) address functions
        // --------------------------------------------------------------------------

        static off_t physBaseGPIO1()                     { return 0x48310000; }
        static off_t physBaseGPIO2()                     { return 0x49050000; }
        static off_t physBaseGPIO3()                     { return 0x49052000; }
        static off_t physBaseGPIO4()                     { return 0x49054000; }
        static off_t physBaseGPIO5()                     { return 0x49056000; }
        static off_t physBaseGPIO6()                     { return 0x49058000; }

        static off_t gpio_offset_revision()              { return 0x000; }
        static off_t gpio_offset_sysconfig()             { return 0x010; }
        static off_t gpio_offset_sysstatus()             { return 0x014; }
        static off_t gpio_offset_irqstatus1()            { return 0x018; }
        static off_t gpio_offset_irqenable1()            { return 0x01C; }
        static off_t gpio_offset_wakeupenable()          { return 0x020; }
        static off_t gpio_offset_irqstatus2()            { return 0x028; }
        static off_t gpio_offset_irqenable2()            { return 0x02C; }
        static off_t gpio_offset_ctrl()                  { return 0x030; } //
        static off_t gpio_offset_oe()                    { return 0x034; } //enable the pins output capabilities. Its only function is to carry the pads configuration.
        static off_t gpio_offset_datain()                { return 0x038; } //register the data that is read from the GPIO pins
        static off_t gpio_offset_dataout()               { return 0x03C; } //setting the value of the GPIO output pins
        static off_t gpio_offset_leveldetect0()          { return 0x040; } //enable/disable for each input lines the low-level (0) detection to be used for the interrupt request generation.
        static off_t gpio_offset_leveldetect1()          { return 0x044; } //enable/disable for each input lines the high-level (1) detection to be used for the interrupt request generation.
        static off_t gpio_offset_risingdetect()          { return 0x048; } //enable/disable for each input lines the rising-edge (transition 0=>1) detection to be used for the interrupt request and the wake-up generation.
        static off_t gpio_offset_fallingdetect()         { return 0x04C; } //enable/disable for each input lines the falling-edge (transition 1=>0) detection to be used for the interrupt request and the wake-up generation.
        static off_t gpio_offset_debouncenable()         { return 0x050; }
        static off_t gpio_offset_debouncingtime()        { return 0x054; }
        static off_t gpio_offset_clearirqenable1()       { return 0x060; }
        static off_t gpio_offset_setirqenable1()         { return 0x064; }
        static off_t gpio_offset_clearirqenable2()       { return 0x070; }
        static off_t gpio_offset_setirqenable2()         { return 0x074; }
        static off_t gpio_offset_clearwkuena()           { return 0x080; }
        static off_t gpio_offset_setwkuena()             { return 0x084; }
        static off_t gpio_offset_cleardataout()          { return 0x090; }
        static off_t gpio_offset_setdataout()            { return 0x094; }

#define __OFFSET2ADRGPIO(IPIN,OFFSET)       \
        if (ipin<0)                         \
        return (0);                         \
        else                                \
        if (ipin<32)                        \
        return(physBaseGPIO1()+OFFSET);      \
        else                                \
        if (ipin<64)                        \
        return(physBaseGPIO2()+OFFSET);     \
        else                                \
        if (ipin<96)                        \
        return(physBaseGPIO3()+OFFSET);     \
        else                                \
        if (ipin<128)                       \
        return(physBaseGPIO4()+OFFSET);     \
        else                                \
        if (ipin<160)                       \
        return(physBaseGPIO5()+OFFSET);     \
        else                                \
        if (ipin<192)                       \
        return(physBaseGPIO6()+OFFSET);     \
        else                                \
        return(0) 

        // Returns physical address of GPIO Read Register for a given GPIO pin
        static off_t physGPIOReadLevel(const unsigned ipin) {
            __OFFSET2ADRGPIO(ipin,gpio_offset_datain()); 
        }

        // Returns physical address of Output Enable Register for a given GPIO pin
        static off_t physGPIODirection(const unsigned ipin) {
            __OFFSET2ADRGPIO(ipin,gpio_offset_oe()); 
        }

        // Returns physical address of Output Write Register for a given GPIO pin
        static off_t physGPIOSetLevel(const unsigned ipin) { 
            __OFFSET2ADRGPIO(ipin,gpio_offset_dataout());
        }

        static off_t physGPIORiseEdge(const unsigned ipin) {
            __OFFSET2ADRGPIO(ipin,gpio_offset_risingdetect());
        }

        static off_t physGPIOFallEdge(const unsigned ipin) {
            __OFFSET2ADRGPIO(ipin,gpio_offset_fallingdetect());
        }

        // need to figure out what this does....
        //static off_t physGPIOAltFunc(const unsigned ipin) { 
            //return physGPIOAltFuncBase() + ((ipin>>2)&(~Bits::mask2Bit()));
        //        }

        // --------------------------------------------------------------------------
        // STATIC SSP register (PHYSICAL) address functions
        // --------------------------------------------------------------------------

#define __OFFSET2ADRMCSPI(ISPI,OFFSET)             \
        if (ISPI<0)                           \
        return (0);                       \
        else                                  \
        if (ISPI<4)                           \
        return(physBaseMCSPI1()+OFFSET);  \
        else                                  \
        if (ISPI<6)                           \
        return(physBaseMCSPI2()+OFFSET);  \
        else                                  \
        if (ISPI<8)                           \
        return(physBaseMCSPI3()+OFFSET);  \
        else                                  \
        if (ISPI==8)                           \
        return(physBaseMCSPI4()+OFFSET);  \
        else                                  \
        return(0) 


        static off_t physBaseMCSPI1()             { return 0x48098000; }
        static off_t physBaseMCSPI2()             { return 0x4809A000; }
        static off_t physBaseMCSPI3()             { return 0x480B8000; }
        static off_t physBaseMCSPI4()             { return 0x480BA000; }


        static off_t mcspi_offset_revision()     { return 0x00;}
        static off_t mcspi_offset_sysconfig()    { return 0x10;}
        static off_t mcspi_offset_sysstatus()    { return 0x14;}
        static off_t mcspi_offset_irqstatus()    { return 0x18;}
        static off_t mcspi_offset_irqenable()    { return 0x1C;}
        static off_t mcspi_offset_wakeupenable() { return 0x20;}
        static off_t mcspi_offset_syst()         { return 0x24;}
        static off_t mcspi_offset_modulctrl()    { return 0x28;}

        static off_t mcspi_offset_chconf_base()  { return 0x2C;}
        static off_t mcspi_offset_chstat_base()  { return 0x30;}
        static off_t mcspi_offset_chctrl_base()  { return 0x34;}
        static off_t mcspi_offset_tx_base()      { return 0x38;}
        static off_t mcspi_offset_rx_base()      { return 0x3C;}

        static off_t mcspi_offset_xferlevel()    { return 0x7C;}

        static off_t physMCSPI_revision(const unsigned ispi) {
            __OFFSET2ADRMCSPI(ispi,mcspi_offset_revision()); 
        }

        static off_t physMCSPI_sysconfig(const unsigned ispi) {
            __OFFSET2ADRMCSPI(ispi,mcspi_offset_sysconfig()); 
        }

        static off_t physMCSPI_sysstatus(const unsigned ispi) {
            __OFFSET2ADRMCSPI(ispi,mcspi_offset_sysstatus()); 
        }

        static off_t physMCSPI_irqstatus(const unsigned ispi) {
            __OFFSET2ADRMCSPI(ispi,mcspi_offset_irqstatus()); 
        }

        static off_t physMCSPI_irqenable(const unsigned ispi) {
            __OFFSET2ADRMCSPI(ispi,mcspi_offset_irqenable()); 
        }

        static off_t physMCSPI_wakeupenable(const unsigned ispi) {
            __OFFSET2ADRMCSPI(ispi,mcspi_offset_wakeupenable()); 
        }

        static off_t physMCSPI_syst(const unsigned ispi) {
            __OFFSET2ADRMCSPI(ispi,mcspi_offset_syst()); 
        }

        static off_t physMCSPI_modulctrl(const unsigned ispi) {
            __OFFSET2ADRMCSPI(ispi,mcspi_offset_modulctrl()); 
        }

        static off_t physMCSPI_chconf(const unsigned ispi) {
            __OFFSET2ADRMCSPI(ispi,mcspi_offset_chconf_base()+0x14*channel[ispi]);
        }

        static off_t physMCSPI_chstat(const unsigned ispi) {
            __OFFSET2ADRMCSPI(ispi,mcspi_offset_chstat_base()+0x14*channel[ispi]);
        }

        static off_t physMCSPI_chctrl(const unsigned ispi) {
            __OFFSET2ADRMCSPI(ispi,mcspi_offset_chctrl_base()+0x14*channel[ispi]);
        }

        static off_t physMCSPI_read(const unsigned ispi) {
            __OFFSET2ADRMCSPI(ispi,mcspi_offset_rx_base()+0x14*channel[ispi]);
        }

        static off_t physMCSPI_write(const unsigned ispi) {
            __OFFSET2ADRMCSPI(ispi,mcspi_offset_tx_base()+0x14*channel[ispi]);
        }


        // --------------------------------------------------------------------------
        // STATIC functions defining (PHYSICAL) start of memory mapped regions
        // --------------------------------------------------------------------------

        static off_t mapSize()          { return 4096; }
        static off_t mapMask()          { return ~(mapSize()-1); }

        static off_t mapBaseGPIO1()     { return physBaseGPIO1() & mapMask(); }
        static off_t mapBaseGPIO2()     { return physBaseGPIO2() & mapMask(); }
        static off_t mapBaseGPIO3()     { return physBaseGPIO3() & mapMask(); }
        static off_t mapBaseGPIO4()     { return physBaseGPIO4() & mapMask(); }
        static off_t mapBaseGPIO5()     { return physBaseGPIO5() & mapMask(); }
        static off_t mapBaseGPIO6()     { return physBaseGPIO6() & mapMask(); }

        //static off_t mapBaseClock()     { return physBaseClock() & mapMask(); }

        static off_t mapBaseMCSPI1()   { return physBaseMCSPI1() & mapMask(); }
        static off_t mapBaseMCSPI2()   { return physBaseMCSPI2() & mapMask(); }
        static off_t mapBaseMCSPI3()   { return physBaseMCSPI3() & mapMask(); }
        static off_t mapBaseMCSPI4()   { return physBaseMCSPI4() & mapMask(); }

        // --------------------------------------------------------------------------
        // Generic Functions for Mapped Addresses
        // --------------------------------------------------------------------------

        static off_t phys2Offset(off_t phys, off_t map_base_phys) {
            return phys - map_base_phys; }

        template<typename T> static volatile T* offset2Virt(off_t map_offset, volatile void* map_base_virt) {
            return reinterpret_cast<volatile T*>(static_cast<volatile uint8_t*>(map_base_virt) + map_offset); }

        static volatile uint32_t* offset2Virt32(off_t map_offset, volatile void* map_base_virt) {
            return offset2Virt<uint32_t>(map_offset, map_base_virt); }

        template<typename T> static volatile T* phys2Virt(off_t phys, volatile void* map_base_virt, off_t map_base_phys) {
            return offset2Virt<T>(phys2Offset(phys, map_base_phys), map_base_virt); }

        static volatile uint32_t* phys2Virt32(off_t phys, volatile void* map_base_virt, off_t map_base_phys) {
            return phys2Virt<uint32_t>(phys, map_base_virt, map_base_phys); }

        // --------------------------------------------------------------------------
        // Functions to return pointers to mapped Clock registers
        // --------------------------------------------------------------------------

        //volatile uint32_t* offset2VirtClock32(off_t map_offset) {
        //    return offset2Virt32(map_offset, m_clock_base); }

        //volatile uint32_t* phys2VirtClock32(off_t phys) {
        //    return phys2Virt32(phys, m_clock_base, mapBaseClock()); }

        //volatile uint32_t* ptrClockConf() {
        //    return phys2VirtClock32(physClockConf()); }

        //volatile uint32_t* ptrClockEnable() {
        //    return phys2VirtClock32(physClockEnable()); }

        //volatile uint32_t* ptrClockOscConf() {
        //    return phys2VirtClock32(physClockOscConf()); }

        //volatile uint32_t* ptrClockStatus() {
        //    return phys2VirtClock32(physClockStatus()); }

        // --------------------------------------------------------------------------
        // Functions to return pointers to mapped GPIO registers
        // --------------------------------------------------------------------------

        volatile uint32_t* phys2VirtGPIO32(off_t phys, const unsigned ipin) {
            if (ipin<32)
                return phys2Virt32(phys,m_gpio1_base,mapBaseGPIO1());
            else if (ipin<64)
                return phys2Virt32(phys,m_gpio2_base,mapBaseGPIO2());
            else if (ipin<96)
                return phys2Virt32(phys,m_gpio3_base,mapBaseGPIO3());
            else if (ipin<160)
                return phys2Virt32(phys,m_gpio4_base,mapBaseGPIO4());
            else if (ipin<160)
                return phys2Virt32(phys,m_gpio5_base,mapBaseGPIO5());
            else if (ipin<192)
                return phys2Virt32(phys,m_gpio6_base,mapBaseGPIO6());
        }

        volatile uint32_t* ptrGPIOReadLevel(const unsigned ipin) {
            return phys2VirtGPIO32(physGPIOReadLevel(ipin),ipin); }

        volatile uint32_t* ptrGPIODirection(const unsigned ipin) {
            return phys2VirtGPIO32(physGPIODirection(ipin),ipin); }

        volatile uint32_t* ptrGPIOSetLevel(const unsigned ipin) { 
            return phys2VirtGPIO32(physGPIOSetLevel(ipin),ipin); }

        volatile uint32_t* ptrGPIORiseEdge(const unsigned ipin) {
            return phys2VirtGPIO32(physGPIORiseEdge(ipin),ipin); }

        volatile uint32_t* ptrGPIOFallEdge(const unsigned ipin) {
            return phys2VirtGPIO32(physGPIOFallEdge(ipin),ipin); }

        // unused ? 
        //volatile uint32_t* ptrGPIOEdgeStat(const unsigned ipin) {
        //return phys2VirtGPIO32(physGPIOEdgeStat(ipin)); }

        //find out what this does
        //volatile uint32_t* ptrGPIOAltFunc(const unsigned ipin) { 
        //return phys2VirtGPIO32(physGPIOAltFunc(ipin)); }

        void gpioSetLevel(const unsigned ipin) {
            *(ptrGPIOSetLevel(ipin)) = Bits::mask1Bit(ipin); }

        void gpioClrLevel(const unsigned ipin) {
            *(ptrGPIOSetLevel(ipin)) &= ~Bits::mask1Bit(ipin); }


        // --------------------------------------------------------------------------
        // Functions to return pointers to mapped SSP registers
        // --------------------------------------------------------------------------

        //volatile uint32_t* phys2VirtMCSPI32(off_t phys, const unsigned ispi) {
        //    if (ispi<4)
        //        return phys2Virt32(phys,m_mcspi1_base,mapBaseSPI1());
        //    else if (ispi<6)       
        //        return phys2Virt32(phys,m_mcspi2_base,mapBaseSPI2());
        //    else if (ispi<8)       
        //        return phys2Virt32(phys,m_mcspi3_base,mapBaseSPI3());
        //    else if (ispi=8)       
        //        return phys2Virt32(phys,m_mcspi4_base,mapBaseSPI4());
        //}

        //volatile uint32_t* ptrMCSPI_sysconfig(const unsigned ispi) {
        //    return phys2VirtMCSPI32(physMCSPI_sysconfig(ispi),ispi); }

        //volatile uint32_t* ptrMCSPI_wakeupenable(const unsigned ispi) {
        //    return phys2VirtMCSPI32(physMCSPI_wakeupenable(ispi),ispi); }

        //volatile uint32_t* ptrMCSPI_modulctrl(const unsigned ispi) {
        //    return phys2VirtMCSPI32(physMCSPI_modulctrl(ispi),ispi); }

        //volatile uint32_t* ptrMCSPI_chconf(const unsigned issp) {
        //    return phys2VirtMCSPI32(physMCSPI_chconf(ispi),ispi); }

        //volatile uint32_t* ptrMCSPI_chstat(const unsigned issp) {
        //    return phys2VirtMCSPI32(physMCSPI_chstat(ispi),ispi); }

        //volatile uint32_t* ptrMCSPI_chctrl(const unsigned issp) {
        //    return phys2VirtMCSPI32(physMCSPI_chctrl(ispi),ispi); }

        //volatile uint32_t* ptrMCSPI_tx(const unsigned issp) {
        //    return phys2VirtMCSPI32(physMCSPI_tx(ispi),ispi); }

        //volatile uint32_t* ptrMCSPI_rx(const unsigned issp) {
        //    return phys2VirtMCSPI32(physMCSPI_rx(ispi),ispi); }

    private:
        volatile void* makeMap(volatile void*& virtual_addr, off_t physical_addr, 
                size_t length = mapSize());

        int             m_mmap_fd;

        volatile void*  m_gpio1_base;
        volatile void*  m_gpio2_base;
        volatile void*  m_gpio3_base;
        volatile void*  m_gpio4_base;
        volatile void*  m_gpio5_base;
        volatile void*  m_gpio6_base;

        volatile void*  m_mcspi1_base;
        volatile void*  m_mcspi2_base;
        volatile void*  m_mcspi3_base;
        volatile void*  m_mcspi4_base;

        volatile void*  m_clock_base;
};

class SimulatedRegisters: private Bits
{
    public:
        SimulatedRegisters()
        {
            m_gpio_level[0] = 0xFFFDEF1B;
            m_gpio_level[1] = 0xFFEFFFFF;
            m_gpio_level[2] = 0xFFFFFDFF;
            m_gpio_level[3] = 0x00000000;

            m_gpio_level[0] &= ~0x08023000;
            m_gpio_level[1] &= ~0x00100200;
            m_gpio_level[2] &= ~0x00010000;
            m_gpio_level[3] &= ~0x00000800;

            m_gpio_direction[0] = 0xC182BA18;
            m_gpio_direction[1] = 0x0012AAA1;
            m_gpio_direction[2] = 0x00010000;
            m_gpio_direction[3] = 0x00014800;

            m_gpio_fn[0] = 0x80040000;
            m_gpio_fn[1] = 0xA5254010;
            m_gpio_fn[2] = 0x69908010;
            m_gpio_fn[3] = 0xAAA5A8AA;
            m_gpio_fn[4] = 0xAAAAAAAA;
            m_gpio_fn[5] = 0x00010000;
            m_gpio_fn[6] = 0x10000000;
            m_gpio_fn[7] = 0x00000001;
        }

        ~SimulatedRegisters()
        {

        }

        // --------------------------------------------------------------------------
        // Functions to return pointers to simulated Clock registers
        // --------------------------------------------------------------------------

        volatile uint32_t* ptrClockEnable()
        {
            return &m_clock_enable;
        }

        // --------------------------------------------------------------------------
        // Functions to return pointers to simulated GPIO registers
        // --------------------------------------------------------------------------

        volatile uint32_t* ptrGPIOReadLevel(const unsigned ipin)
        {
            return &m_gpio_level[(ipin>>5)&0x3];
        }

        volatile uint32_t* ptrGPIODirection(const unsigned ipin)
        {
            return &m_gpio_direction[(ipin>>5)&0x3];
        }

        volatile uint32_t* ptrGPIOAltFunc(const unsigned ipin)
        { 
            return &m_gpio_fn[(ipin>>4)&0x7];
        }

        void gpioSetLevel(const unsigned ipin)
        {
            *(ptrGPIOReadLevel(ipin)) |= Bits::mask1Bit(ipin);
        }

        void gpioClrLevel(const unsigned ipin)
        {
            *(ptrGPIOReadLevel(ipin)) &= ~Bits::mask1Bit(ipin);
        }

        uint32_t simregGPIOLevel(const unsigned ireg) const
        {
            return m_gpio_level[ireg&0x03];
        }

        uint32_t simregGPIODirection(const unsigned ireg) const
        {
            return m_gpio_direction[ireg&0x03];
        }

        uint32_t simregGPIOFn(const unsigned ireg) const
        {
            return m_gpio_fn[ireg&0x07];
        }

        // --------------------------------------------------------------------------
        // Functions to return pointers to simulated SSP registers
        // --------------------------------------------------------------------------

        volatile uint32_t* ptrPWMControl(const unsigned ipwm)
        {
            return m_pwm_c + ipwm;
        }

        volatile uint32_t* ptrPWMDutyCycle(const unsigned ipwm)
        {
            return m_pwm_d + ipwm;
        }

        volatile uint32_t* ptrPWMPeriod(const unsigned ipwm)
        {
            return m_pwm_p + ipwm;
        }

        // --------------------------------------------------------------------------
        // Functions to return pointers to simulated SSP registers
        // --------------------------------------------------------------------------

        volatile uint32_t* ptrSSPControl0(const unsigned issp)
        {
            return m_ssp_c0 + issp;
        }

        volatile uint32_t* ptrSSPControl1(const unsigned issp)
        {
            return m_ssp_c1 + issp;
        }

        volatile uint32_t* ptrSSPStatus(const unsigned issp)
        {
            return m_ssp_status + issp;
        }

        volatile uint32_t* ptrSSPDataRW(const unsigned issp)
        {
            return m_ssp_data + issp;
        }

        volatile uint32_t* ptrSSPPSPConfig(const unsigned issp)
        {
            return m_ssp_cpsp + issp;
        }

    private:
        uint32_t m_clock_enable;

        uint32_t m_gpio_level[4];
        uint32_t m_gpio_direction[4];
        uint32_t m_gpio_fn[8];

        uint32_t m_pwm_c[4];
        uint32_t m_pwm_d[4];
        uint32_t m_pwm_p[4];

        uint32_t m_ssp_c0[3];
        uint32_t m_ssp_c1[3];
        uint32_t m_ssp_cpsp[3];
        uint32_t m_ssp_status[3];
        uint32_t m_ssp_data[3];
};


#endif // ndef REGISTERS_HPP
