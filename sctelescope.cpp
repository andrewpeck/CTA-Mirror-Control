//-*-mode:c++; mode:font-lock;-*-

/*! \file sctelescope.cpp

  Main function for SC telescope mirror system code

  \author     Stephen Fegan               \n
  UCLA                        \n
  sfegan@astro.ucla.edu       \n

  \version    1.0
  \date       05/22/2008 (World Indiana Jones 4 Day)
  */

#include <iostream>
#include <iomanip>
#include "Overo.hpp"
#include "Layout.hpp"

#define TESTSSP 2

int main(int argc, char** argv)
{
    typedef Overo<SimulatedRegisters> Sys;
    Sys sys;
//#if 0
//    uint32_t M32[] = { 0xFFFFFE1B, 0xFFFFFFFF, 0xFFFFFFFF, 0x007FFFFF };
//    uint32_t M16[] = { 0xFFFC03CF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
//        0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00003FFF };
//
//    int32_t C[][4] = { -1,  -1, -1, -1,    // 1
//        67,   1,  0,  0,    // 2    COMMON SR_BAR
//        77,   1,  0,  1,    // 3    COMMON MS1
//        61,   1,  0,  1,    // 4    COMMON MS2
//        30,   1,  0,  1,    // 5    COMMON SLEEP_BAR
//        66,   1,  0,  1,    // 6    COMMON RESET_BAR
//        -1,  -1, -1, -1,    // 7
//        71,   1,  0,  1,    // 8    ADC1 CS_BAR
//        70,   1,  0,  1,    // 9    ADC2 CS_BAR
//        69,   1,  0,  1,    // 10   ADC3 CS_BAR
//        -1,  -1, -1, -1,    // 11
//        46,   1,  2,  0,    // 12   PWM BRIGHTNESS
//        60,   1,  0,  0,    // 13   LED1 ENABLE
//        100,  1,  0,  0,    // 14   LED2 ENABLE
//        9,    1,  0,  0,    // 15   LED3 ENABLE
//        75,   1,  0,  0,    // 16   LED4 ENABLE
//        17,   0,  0,  0,    // 17   unused
//        58,   0,  0,  0,    // 18   unused
//        118,  0,  1,  0,    // 19   SCL
//        117,  0,  1,  0,    // 20   SDA
//        19,   1,  1,  0,    // 21   SSPSCLK2
//        87,   1,  3,  0,    // 22   SSPSFRM2
//        13,   1,  1,  0,    // 23   SSPTXD2
//        41,   1,  0,  1,    // 24   temporary DR2 ENABLE_BAR
//        //		     41,   0,  1,  0,    // 24   FFRXD
//        -1,  -1, -1, -1,    // 25
//        39,   1,  2,  0,    // 26   FFTXD
//        86,   0,  1,  0,    // 27   SSPRXD2
//        -1,  -1, -1, -1,    // 28
//        -1,  -1, -1, -1,    // 29
//        -1,  -1, -1, -1,    // 30
//        -1,  -1, -1, -1,    // 31
//        44,   0,  0,  0,    // 32   unused
//        42,   0,  0,  0,    // 33   DR6 HOME
//        45,   1,  0,  0,    // 34   DR6 DIR
//        43,   1,  0,  0,    // 35   DR6 STEP
//        16,   1,  0,  1,    // 36   DR6 ENABLE_BAR
//        -1,  -1, -1, -1,    // 37
//        101,  0,  0,  0,    // 38   DR5 HOME
//        27,   1,  0,  0,    // 39   DR5 DIR
//        64,   1,  0,  0,    // 40   DR5 STEP
//        74,   1,  0,  1,    // 41   DR5 ENABLE_BAR
//        -1,  -1, -1, -1,    // 42
//        29,   0,  0,  0,    // 43   DR4 HOME
//        14,   1,  0,  0,    // 44   DR4 DIR
//        11,   1,  0,  0,    // 45   DR4 STEP
//        31,   1,  0,  1,    // 46   DR4 ENABLE_BAR
//        62,   0,  0,  0,    // 47   DR3 HOME
//        47,   1,  0,  0,    // 48   DR3 DIR
//        113,  1,  0,  0,    // 49   DR3 STEP
//        59,   1,  0,  1,    // 50   DR3 ENABLE_BAR
//        68,   0,  0,  0,    // 51   DR2 HOME
//        63,   1,  0,  0,    // 52   DR2 DIR
//        65,   1,  0,  0,    // 53   DR2 STEP
//        34,   0,  1,  0,    // 54   temporary FFRXD
//        //		     34,   1,  0,  1,    // 54   DR2 ENABLE_BAR
//        72,   0,  0,  0,    // 55   DR1 HOME
//        73,   1,  0,  0,    // 56   DR1 DIR
//        76,   1,  0,  0,    // 57   DR1 STEP
//        28,   1,  0,  1,    // 58   DR1 ENABLE_BAR
//        -1,  -1, -1, -1,    // 59
//        -1,  -1, -1, -1 };  // 60
//
// #if 1
//     const Layout<>::PinConf* PC = 0;
//     unsigned npinconf = Layout<>::pin60Conf(PC);
//     for(unsigned ipin=0;ipin<npinconf;ipin++)
//     {
//         std::cout << ipin << ' ' << PC[ipin].ipin << ' ' << PC[ipin].igpio << '\n';
//         sys.gpioConfigure(PC[ipin].igpio, PC[ipin].dir, PC[ipin].fn);
//         sys.gpioWriteLevel(PC[ipin].igpio, PC[ipin].val);
//     }
// #endif
    Sys sy2;
//
//    std::cout << std::setfill('0') << std::uppercase;
//    for(unsigned ireg=0;ireg<4;ireg++)
//        std::cout
//            << "#define CFG_GPSR" << std::dec << ireg << "_VAL           0x"
//            << std::hex << std::setw(8) << (sys.simregGPIOLevel(ireg)&M32[ireg])
//            << " /* 0x" << std::setw(8) << (sy2.simregGPIOLevel(ireg)&M32[ireg])
//            << " */\n";
//    std::cout << '\n';
//    for(unsigned ireg=0;ireg<4;ireg++)
//        std::cout
//            << "#define CFG_GPCR" << std::dec << ireg << "_VAL           0x"
//            << std::hex << std::setw(8) << ((~sys.simregGPIOLevel(ireg))&M32[ireg])
//            << " /* 0x" << std::setw(8) << ((~sy2.simregGPIOLevel(ireg))&M32[ireg])
//            << " */\n";
//    std::cout << '\n';
//    for(unsigned ireg=0;ireg<4;ireg++)
//        std::cout
//            << "#define CFG_GPDR" << std::dec << ireg << "_VAL           0x"
//            << std::hex << std::setw(8) << (sys.simregGPIODirection(ireg)&M32[ireg])
//            << " /* 0x" << std::setw(8) << (sy2.simregGPIODirection(ireg)&M32[ireg])
//            << " */\n";
//    std::cout << '\n';
//    for(unsigned ireg=0;ireg<8;ireg++)
//        std::cout
//            << "#define CFG_GAFR" << std::dec << (ireg/2) << '_' << (ireg%2?'U':'L')
//            << "_VAL         0x"
//            << std::hex << std::setw(8) << (sys.simregGPIOFn(ireg)&M16[ireg])
//            << " /* 0x" << std::setw(8) << (sy2.simregGPIOFn(ireg)&M32[ireg])
//            << " */\n";
//
//
//#endif

    //  Overo<MappedRegisters> sys;
    std::cout << std::dec;
    unsigned npin = sys.nGPIO();
    for(unsigned ipin=0; ipin<npin; ipin++)
        if(sys.hasGPIOPin(ipin)) {
            std::cout << ipin << ' ' << int(sys.gpioGetDirection(ipin)) << ' '
                      //<< int(sys.gpioGetAltFunc(ipin)) << ' '
                      << int(sys.gpioReadLevel(ipin)) << '\n';
        }

//#if 0
//    sys.sspConfigure(TESTSSP,1,32,sys.SSP_FF_SPI,
//            SSP_FLAG_NONE
//            //|SSP_FLAG_ENABLE_FIFO_TEST_MODE
//            //|SSP_FLAG_LOOP_BACK_MODE
//            //|SSP_FLAG_SELECT_RX_FIFO_FOR_TEST_MODE
//            |SSP_FLAG_MASK_INT_TX_UNDERRUN
//            |SSP_FLAG_MASK_INT_RX_OVERRUN);
//    sys.gpioConfigure(35, sys.GPIODIR_IN,  sys.GPIOFN_ALT3);
//    sys.gpioConfigure(38, sys.GPIODIR_OUT, sys.GPIOFN_ALT1);
//    sys.gpioConfigure(40, sys.GPIODIR_OUT, sys.GPIOFN_ALT3);
//    sys.gpioConfigure(89, sys.GPIODIR_IN,  sys.GPIOFN_ALT1);
//    sys.sspEnable(TESTSSP);
//
//    std::cout << std::setfill('0');
//
//    for(unsigned issp=0;issp<3;issp++)
//    {
//        std::cout
//            << std::dec << std::setw(3) << issp << ' '
//            << std::hex << std::setw(8) << *sys.ptrSSPControl0(issp) << ' '
//            << std::hex << std::setw(8) << *sys.ptrSSPControl1(issp) << ' '
//            << std::hex << std::setw(8) << *sys.ptrSSPPSPConfig(issp) << ' '
//            << std::hex << std::setw(8) << *sys.ptrSSPTimeOut(issp) << ' '
//            << std::hex << std::setw(8) << *sys.ptrSSPTXTimeslotActive(issp) << ' '
//            << std::hex << std::setw(8) << *sys.ptrSSPRXTimeslotActive(issp) << ' '
//            << std::hex << std::setw(8) << *sys.ptrSSPTimeslotStatus(issp) << ' '
//            << std::hex << std::setw(8) << *sys.ptrSSPAudioClockDiv(issp) << ' '
//            << std::hex << std::setw(8) << *sys.ptrSSPStatus(issp) << '\n';
//    }
//
//    std::cout << "CLOCK: "
//        << std::hex << std::setw(8) << *sys.ptrClockEnable() << '\n';
//    std::cout << std::hex << std::setw(8) << *sys.ptrSSPStatus(TESTSSP) << '\n';
//
//    std::cout << std::dec;
//    for(unsigned ibyte=0;ibyte<16;ibyte++)
//    {
//#if 0
//        std::cout
//            << sys.sspGetTXLevel(TESTSSP) << ' '
//            << sys.sspGetRXLevel(TESTSSP) << '\n';
//#endif
//        sys.sspTestAndWrite(TESTSSP, 0xdeadbeef + ibyte);
//    }
//
//    unsigned ntx;
//    unsigned nrx;
//    sys.sspGetTXRXLevels(TESTSSP,ntx,nrx);
//
//    std::cout << ntx << ' ' << nrx << '\n';
//
//    usleep(100000);
//
//    std::cout
//        << sys.sspGetTXLevel(TESTSSP) << ' '
//        << sys.sspGetRXLevel(TESTSSP) << '\n';
//
//    for(unsigned ibyte=0;ibyte<16;ibyte++)
//    {
//        std::cout << std::hex << std::setw(8)
//            << sys.sspPollAndRead(TESTSSP) << '\n';
//    }
//
//#endif
//
//
//#if 0
//    unsigned npin = Overo::nGPIO();
//    for(unsigned ipin=0;ipin<npin;ipin++)
//    {
//        std::cout
//            << std::setfill('0')
//            << std::dec << std::setw(3) << ipin << ' '
//            << std::hex << std::setw(8) << Overo::physGPIODirection(ipin) << ' '
//            << std::hex << std::setw(8) << Overo::mask1Bit(ipin) << ' '
//            << std::dec << std::setw(2) << Overo::shift1Bit(ipin) << ' '
//            //<< std::hex << std::setw(8) << Overo::physGPIOAltFunc(ipin) << ' '
//            << std::hex << std::setw(8) << Overo::mask2Bit(ipin) << ' '
//            << std::dec << std::setw(2) << Overo::shift2Bit(ipin) << '\n';
//
//    }
//
//#endif
}
