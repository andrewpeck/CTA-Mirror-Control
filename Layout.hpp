//-*-mode:c++; mode:font-lock;-*-

/*! \file Layout.hpp

  Layout of signals of UCLA board

  \author     Stephen Fegan               \n
  UCLA                        \n
  sfegan@astro.ucla.edu       \n

  \version    3.0
  \date       09/11/2013
  */

#ifndef LAYOUT_HPP
#define LAYOUT_HPP

#include <TLCX5XX_ADC.hpp>
//#include <Overo.hpp>
#define N(x) (sizeof(x)/sizeof(*x))
#define GPIODIR_IN  0
#define GPIODIR_OUT 1

class OveroBase; 
typedef TLC3548                    ADC;

class OveroGPIOMap {
    public:

        //Configure GPIO Input/Output Direction
        //0 is OUTPUT
        //1 is INPUT
        //-1 not a gpio
        //DO NOT reorder this list
        static int gpioConfiguration(unsigned ipin) {
            static const int igpio[] =  {
                -1, //N_MANUAL_RESET      1 
                0,  //GPIO71_L_DD01       2     //EncoderEnable
                0,  //GPIO70_L_DD00       3     //DR5Dir
                0,  //GPIO73_L_DD03       4     //DR2EnableBar
                0,  //GPIO75_L_DD05       5     //DR6EnableBar
                1,  //GPIO72_L_DD02       6
                -1, //GPIO74_L_DD04       7     //USB Reset
                0,  //GPIO_10             8     //USB4EnableBar
                -1, //GPIO0_WAKEUP        9
                1,  //GPIO185_I2C3_SDA    10
                1,  //GPIO80_L_DD10       11    //USB3EnableBar
                0,  //GPIO81_L_DD11       12    //DR4EnableBar
                1,  //GPIO184_I2C3_SCL    13
                0,  //GPIO_186            14    //DR4Step
                0,  //GPIO92_L_DD22       15    //DR5EnableBar
                1,  //GPIO147_GPT8_PWM    16
                0,  //GPIO83_L_DD13       17    //ADCSel1
                0,  //GPIO144_GPT9_PWM    18    //DR4Dir
                1,  //GPIO84_L_DD14       19
                1,  //GPIO85_L_DD15       20
                1,  //GPIO146_GPT11_PWM   21
                1,  //GPIO163_IR_CTS3     22
                1,  //GPIO91_L_DD21       23
                0,  //GPIO87_L_DD17       24    //DR3Dir
                0,  //GPIO88_L_DD18       25    //DR3Step
                1,  //GPIO166_IR_TXD3     26
                0,  //GPIO89_L_DD19       27    //DR3EnableBar
                0,  //GPIO79_L_DD09       28    //DR2Dir
                0,  //GPIO77_L_DD07       29    //ADCSel2
                1,  //GPIO78_L_DD08       30    //USB5EnableBar
                1,  //GPIO165_IR_RXD3     31
                0,  //GPIO66_L_PCLK       32    //DR2Step
                0,  //GPIO76_L_DD06       33    //DR1Dir
                0,  //GPIO68_L_FCLK       34    //DR1EnableBar
                0,  //GPIO67_L_LCLK       35    //DR1Step
                -1, //USBOTG_DP           36  
                -1, //USBOTG_DM           37  
                -1, //AUXLF               38  
                -1, //MIC_SUB_MF          39  
                -1, //ADCIN4              40  
                -1, //AUXRF               41  
                -1, //PWM0                42  
                1,  //GPIO69_L_BIAS       43  
                1,  //GPIO86_L_DD16       44  
                1,  //GPIO90_L_DD20       45  
                -1, //USBOTG_ID           46  
                1,  //GPIO170_HDQ_1WIRE   47  
                -1, //ADCIN3              48  
                -1, //PWM1                49  
                -1, //AGND                50  
                -1, //ADCIN5              51  
                -1, //VBACKUP             52  
                -1, //ADCIN6              53  
                -1, //USBOTG_VBUS         54  
                0,  //GPIO145_GPT10_PWM   55  //DR5Step
                -1, //GND                 56  
                -1, //MIC_MAIN_MF         57  
                -1, //ADCIN2              58  
                -1, //SYSEN               59  
                1,  //GPIO82_L_DD12       60  
                1,  //GPIO93_L_DD23       61  
                -1, //TV_OUT2             62  
                -1, //TV_OUT1             63  
                -1, //ADCIN7              64  
                -1, //POWERON             65  
                -1, //VSYSTEM             66  
                -1, //VSYSTEM             67  
                -1, //HSOLF               68  
                -1, //HSORF               69  
                -1, //GND                 70  

                ////Connector J4 (70-pin): Extended Memory Bus & MMC Signals
                -1, //VSYSTEM             71 
                -1, //VSYSTEM             72 
                -1, //GND                 73 
                -1, //EM_NCS5_ETH0        74 
                -1, //EM_NCS4             75 
                -1, //EM_NWE              76 
                -1, //EM_NADV_ALE         77 
                -1, //EM_NOE              78 
                1,  //GPIO65_ETH1_IRQ1    79 
                1,  //GPIO64_ETH0_NRESET  80 
                -1, //EM_A2               81 
                -1, //EM_A8               82 
                -1, //EM_A5               83 
                -1, //EM_A7               84 
                -1, //EM_D2               85 
                -1, //EM_D10              86 
                -1, //EM_D3               87 
                -1, //EM_D11              88 
                -1, //EM_D4               89 
                -1, //EM_D12              90 
                -1, //EM_D5               91 
                -1, //EM_D15              92 
                1,  //GPIO13_MMC3_CMD     93 
                1,  //GPIO148_TXD1        94 
                1,  //GPIO176_ETH0_IRQ    95 
                0,  //GPIO18_MMC3_D0      96 
                1,  //GPIO174_SPI1_CS0    97 
                1,  //GPIO168_USBH_CPEN   98 
                1,  //GPIO14_MMC3_DAT4    99    //USB1Enablebar
                1,  //GPIO21_MMC3_DAT7    100
                0,  //GPIO17_MMC3_D3      101   //USB7EnableBar
                -1, //USBH_VBUS           102
                -1, //GND                 103
                -1, //USBH_DP             104
                -1, //USBH_DM             105
                1,  //GPIO19_MMC3_D1      106
                1,  //GPIO22_MMC3_DAT6    107
                0,  //GPIO23_MMC3_DAT5    108   //DR5Step
                0,  //GPIO20_MMC3_D2      109   //DR6Dir
                0,  //GPIO12_MMC3_CLK     110   //USB6EnableBar
                1,  //GPIO114_SPI1_NIRQ   111
                1,  //GPIO175_SPI1_CS1    112
                1,  //GPIO171_SPI1_CLK    113
                1,  //GPIO172_SPI1_MOSI   114
                1,  //GPIO173_SPI1_MISO   115
                -1, //4030GP2_N_MMC3_CD   116
                1,  //GPIO150_MMC3_WP     117
                1,  //GPIO151_RXD1        118
                -1, //EM_D7               119
                -1, //EM_D14              120
                -1, //EM_D6               121
                -1, //EM_D13              122
                -1, //EM_D1               123
                -1, //EM_D8               124
                -1, //EM_D9               125
                -1, //EM_D0               126
                -1, //EM_A6               127
                -1, //EM_A1               128
                -1, //EM_A3               129
                -1, //EM_A10              130
                -1, //EM_A4               131
                -1, //EM_A9               132
                -1, //EM_NWP              133
                -1, //EM_NCS1             134
                -1, //EM_NBE0             135
                -1, //EM_NCS0             136
                -1, //EM_NCS6             137
                -1, //EM_WAIT0            138
                -1, //EM_NBE1             139
                -1, //EM_CLK              140
            };
            if  ((ipin>0) && ((--ipin)< N(igpio)))
                return igpio[ipin];
            else 
                return -1;
        } 

        //Maps Overo output pins (J1 1-70, J 71-140) to GPIO Pins
        //-1 is returned for a pin140 that is not a GPIO
        static int pin140ToGPIO(unsigned ipin140) {
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
            };
            if  ((ipin140>0) && ((--ipin140) < N(igpio)))
                return igpio[ipin140];
            else 
                return -1;
        } 
};

template<typename MAP = OveroGPIOMap> class Layout {
    public:

        ////////////////////////////////////////////////////////////////////////////////
        // Layout Specific Pin Assignments for Overo
        ////////////////////////////////////////////////////////////////////////////////

        //Connector J1 (70-pin): LCD, PWM & Analog Signals
        /////// SIGNAL                  PIN
#define         N_MANUAL_RESET       1
#define         GPIO71_L_DD01        2
#define         GPIO70_L_DD00        3
#define         GPIO73_L_DD03        4
#define         GPIO75_L_DD05        5
#define         GPIO72_L_DD02        6
#define         GPIO74_L_DD04        7
#define         GPIO_10              8
#define         GPIO0_WAKEUP         9
#define         GPIO185_I2C3_SDA     10
#define         GPIO80_L_DD10        11
#define         GPIO81_L_DD11        12
#define         GPIO184_I2C3_SCL     13
#define         GPIO_186             14
#define         GPIO92_L_DD22        15
#define         GPIO147_GPT8_PWM     16
#define         GPIO83_L_DD13        17
#define         GPIO144_GPT9_PWM     18
#define         GPIO84_L_DD14        19
#define         GPIO85_L_DD15        20
#define         GPIO146_GPT11_PWM    21
#define         GPIO163_IR_CTS3      22
#define         GPIO91_L_DD21        23
#define         GPIO87_L_DD17        24
#define         GPIO88_L_DD18        25
#define         GPIO166_IR_TXD3      26
#define         GPIO89_L_DD19        27
#define         GPIO79_L_DD09        28
#define         GPIO77_L_DD07        29
#define         GPIO78_L_DD08        30
#define         GPIO165_IR_RXD3      31
#define         GPIO66_L_PCLK        32
#define         GPIO76_L_DD06        33
#define         GPIO68_L_FCLK        34
#define         GPIO67_L_LCLK        35
#define         USBOTG_DP            36
#define         USBOTG_DM            37
#define         AUXLF                38
#define         MIC_SUB_MF           39
#define         ADCIN4               40
#define         AUXRF                41
#define         PWM0                 42
#define         GPIO69_L_BIAS        43
#define         GPIO86_L_DD16        44
#define         GPIO90_L_DD20        45
#define         USBOTG_ID            46
#define         GPIO170_HDQ_1WIRE    47
#define         ADCIN3               48
#define         PWM1                 49
#define         AGND                 50
#define         ADCIN5               51
#define         VBACKUP              52
#define         ADCIN6               53
#define         USBOTG_VBUS          54
#define         GPIO145_GPT10_PWM    55
#define         GND                  56
#define         MIC_MAIN_MF          57
#define         ADCIN2               58
#define         SYSEN                59
#define         GPIO82_L_DD12        60
#define         GPIO93_L_DD23        61
#define         TV_OUT2              62
#define         TV_OUT1              63
#define         ADCIN7               64
#define         POWERON              65
#define         VSYSTEM              66
#define         VSYSTEM              67
#define         HSOLF                68
#define         HSORF                69
#define         GND                  70

        //Connector J4 (70-pin): Extended Memory Bus & MMC Signals
        ///////  SIGNAL                 PIN
#define VSYSTEM               71
#define VSYSTEM               72
#define GND                   73
#define EM_NCS5_ETH0          74
#define EM_NCS4               75
#define EM_NWE                76
#define EM_NADV_ALE           77
#define EM_NOE                78
#define GPIO65_ETH1_IRQ1      79
#define GPIO64_ETH0_NRESET    80
#define EM_A2                 81
#define EM_A8                 82
#define EM_A5                 83
#define EM_A7                 84
#define EM_D2                 85
#define EM_D10                86
#define EM_D3                 87
#define EM_D11                88
#define EM_D4                 89
#define EM_D12                90
#define EM_D5                 91
#define EM_D15                92
#define GPIO13_MMC3_CMD       93
#define GPIO148_TXD1          94
#define GPIO176_ETH0_IRQ      95
#define GPIO18_MMC3_D0        96
#define GPIO174_SPI1_CS0      97
#define GPIO168_USBH_CPEN     98
#define GPIO14_MMC3_DAT4      99
#define GPIO21_MMC3_DAT7      100
#define GPIO17_MMC3_D3        101
#define USBH_VBUS             102
#define GND                   103
#define USBH_DP               104
#define USBH_DM               105
#define GPIO19_MMC3_D1        106
#define GPIO22_MMC3_DAT6      107
#define GPIO23_MMC3_DAT5      108
#define GPIO20_MMC3_D2        109
#define GPIO12_MMC3_CLK       110
#define GPIO114_SPI1_NIRQ     111
#define GPIO175_SPI1_CS1      112
#define GPIO171_SPI1_CLK      113
#define GPIO172_SPI1_MOSI     114
#define GPIO173_SPI1_MISO     115
#define _4030GP2_N_MMC3_CD    116
#define GPIO150_MMC3_WP       117
#define GPIO151_RXD1          118
#define EM_D7                 119
#define EM_D14                120
#define EM_D6                 121
#define EM_D13                122
#define EM_D1                 123
#define EM_D8                 124
#define EM_D9                 125
#define EM_D0                 126
#define EM_A6                 127
#define EM_A1                 128
#define EM_A3                 129
#define EM_A10                130
#define EM_A4                 131
#define EM_A9                 132
#define EM_NWP                133
#define EM_NCS1               134
#define EM_NBE0               135
#define EM_NCS0               136
#define EM_NCS6               137
#define EM_WAIT0              138
#define EM_NBE1               139
#define EM_CLK                140

        // --------------------------------------------------------------------------
        // Returns LAYOUT independent GPIO number 0-192 for Each Signal
        // --------------------------------------------------------------------------

        static unsigned igpioN_M_RESET()     { return MAP::pin140ToGPIO(N_MANUAL_RESET)     ;} 
        static unsigned igpioEN_IO()         { return MAP::pin140ToGPIO(GPIO72_L_DD02)      ;}

        static unsigned igpioPowerADC()      { return MAP::pin140ToGPIO(GPIO150_MMC3_WP)    ;} //PowerADC
        static unsigned igpioADCSel1()       { return MAP::pin140ToGPIO(GPIO83_L_DD13)      ;} //ADCSel1
        static unsigned igpioADCSel2()       { return MAP::pin140ToGPIO(GPIO77_L_DD07)      ;} //ADCSel2

        static unsigned igpioEncoderEnable() { return MAP::pin140ToGPIO(GPIO71_L_DD01)      ;} //EncoderEnable

        static unsigned igpioConsoleRXD()    { return MAP::pin140ToGPIO(GPIO165_IR_RXD3)    ;} //ConsoleRXD
        static unsigned igpioConsoleTXD()    { return MAP::pin140ToGPIO(GPIO166_IR_TXD3)    ;} //ConsoleTXD

        static unsigned igpioTP1()           { return MAP::pin140ToGPIO(GPIO69_L_BIAS)      ;}
        static unsigned igpioTP2()           { return MAP::pin140ToGPIO(GPIO86_L_DD16)      ;}
        static unsigned igpioTP3()           { return MAP::pin140ToGPIO(GPIO90_L_DD20)      ;}
        static unsigned igpioTP4()           { return MAP::pin140ToGPIO(GPIO170_HDQ_1WIRE)  ;}
        static unsigned igpioTP5()           { return MAP::pin140ToGPIO(GPIO145_GPT10_PWM)  ;}
        static unsigned igpioTP6()           { return MAP::pin140ToGPIO(GPIO82_L_DD12)      ;}
        static unsigned igpioTP7()           { return MAP::pin140ToGPIO(GPIO93_L_DD23)      ;}

        // usb otg ? 
        static unsigned igpioUSBReset()      { return MAP::pin140ToGPIO(GPIO74_L_DD04)      ;} //USBReset
        static unsigned igpioUSBOff1()       { return MAP::pin140ToGPIO(GPIO14_MMC3_DAT4)   ;} //USB1EnableBar
        static unsigned igpioUSBOff2()       { return MAP::pin140ToGPIO(GPIO18_MMC3_D0)     ;} //USB2EnableBar
        static unsigned igpioUSBOff3()       { return MAP::pin140ToGPIO(GPIO80_L_DD10)      ;} //USB3EnableBar
        static unsigned igpioUSBOff4()       { return MAP::pin140ToGPIO(GPIO_10)            ;} //USB4EnableBar
        static unsigned igpioUSBOff5()       { return MAP::pin140ToGPIO(GPIO78_L_DD08)      ;} //USB5EnableBar
        static unsigned igpioUSBOff6()       { return MAP::pin140ToGPIO(GPIO12_MMC3_CLK)    ;} //USB6EnableBar
        static unsigned igpioUSBOff7()       { return MAP::pin140ToGPIO(GPIO17_MMC3_D3)     ;} //USB7EnableBar

        static unsigned igpioMS1()           { return MAP::pin140ToGPIO(GPIO146_GPT11_PWM)  ;} //DRMS1
        static unsigned igpioMS2()           { return MAP::pin140ToGPIO(GPIO85_L_DD15)      ;} //DRMS2
        static unsigned igpioPwrIncBar()     { return MAP::pin140ToGPIO(GPIO175_SPI1_CS1)   ;} //DRPowerIncreaseBar
        static unsigned igpioSR()            { return MAP::pin140ToGPIO(GPIO91_L_DD21)      ;} //DRSRBar
        static unsigned igpioReset()         { return MAP::pin140ToGPIO(GPIO147_GPT8_PWM)   ;} //DRResetBar
        static unsigned igpioSleep()         { return MAP::pin140ToGPIO(GPIO84_L_DD14)      ;} //DRSleepBar

        static unsigned igpioStep1()         { return MAP::pin140ToGPIO(GPIO67_L_LCLK)      ;} //DR1Step
        static unsigned igpioStep2()         { return MAP::pin140ToGPIO(GPIO66_L_PCLK)      ;} //DR2Step
        static unsigned igpioStep3()         { return MAP::pin140ToGPIO(GPIO88_L_DD18)      ;} //DR3Step
        static unsigned igpioStep4()         { return MAP::pin140ToGPIO(GPIO_186)           ;} //DR4Step
        static unsigned igpioStep5()         { return MAP::pin140ToGPIO(GPIO145_GPT10_PWM)  ;} //DR5Step
        static unsigned igpioStep6()         { return MAP::pin140ToGPIO(GPIO23_MMC3_DAT5)   ;} //DR6Step

        static unsigned igpioDir1()          { return MAP::pin140ToGPIO(GPIO76_L_DD06)      ;} //DR1Dir
        static unsigned igpioDir2()          { return MAP::pin140ToGPIO(GPIO79_L_DD09)      ;} //DR2Dir
        static unsigned igpioDir3()          { return MAP::pin140ToGPIO(GPIO87_L_DD17)      ;} //DR3Dir
        static unsigned igpioDir4()          { return MAP::pin140ToGPIO(GPIO144_GPT9_PWM)   ;} //DR4Dir
        static unsigned igpioDir5()          { return MAP::pin140ToGPIO(GPIO70_L_DD00)      ;} //DR5Dir
        static unsigned igpioDir6()          { return MAP::pin140ToGPIO(GPIO20_MMC3_D2)     ;} //DR6Dir

        static unsigned igpioEnable1()       { return MAP::pin140ToGPIO(GPIO68_L_FCLK)      ;} //DR1EnableBar
        static unsigned igpioEnable2()       { return MAP::pin140ToGPIO(GPIO73_L_DD03)      ;} //DR2EnableBar
        static unsigned igpioEnable3()       { return MAP::pin140ToGPIO(GPIO89_L_DD19)      ;} //DR3EnableBar
        static unsigned igpioEnable4()       { return MAP::pin140ToGPIO(GPIO81_L_DD11)      ;} //DR4EnableBar
        static unsigned igpioEnable5()       { return MAP::pin140ToGPIO(GPIO92_L_DD22)      ;} //DR5EnableBar
        static unsigned igpioEnable6()       { return MAP::pin140ToGPIO(GPIO75_L_DD05)      ;} //DR6EnableBar

        static unsigned igpioSPI_Tx()        { return MAP::pin140ToGPIO(GPIO172_SPI1_MOSI)  ;} 
        static unsigned igpioSPI_Rx()        { return MAP::pin140ToGPIO(GPIO173_SPI1_MISO)  ;}
        static unsigned igpioSPI_Sclk()      { return MAP::pin140ToGPIO(GPIO171_SPI1_CLK)   ;}
        static unsigned igpioSPI_SFRM_bar()  { return MAP::pin140ToGPIO(GPIO174_SPI1_CS0)   ;}

        // Returns Motor Direction Control Pin GPIO Number for a given idrive
        static unsigned igpioDir(const unsigned idrive) {
            static const unsigned igpio[] = { 
                igpioDir1(),
                igpioDir2(),
                igpioDir3(),
                igpioDir4(),
                igpioDir5(),
                igpioDir6()
            };
            return igpio[idrive];
        }

        // Returns Motor Step Pin GPIO Number for a given idrive
        static unsigned igpioStep(const unsigned idrive) {
            static const unsigned igpio[] = { 
                igpioStep1(),
                igpioStep2(),
                igpioStep3(),
                igpioStep4(),
                igpioStep5(),
                igpioStep6()
            };
            return igpio[idrive];
        }


        // Returns Motor Enable Pin GPIO Number for a given idrive
        static unsigned igpioEnable(const unsigned idrive) {
            static const unsigned igpio[] = { 
                igpioEnable1(),
                igpioEnable2(),
                igpioEnable3(),
                igpioEnable4(),
                igpioEnable5(),
                igpioEnable6()
            };
            return igpio[idrive];
        }

        // Returns USB Power Enable Pin GPIO Number for a given iusb
        static unsigned igpioUSBOff(const unsigned iusb) {
            static const unsigned igpio[] = { 
                igpioUSBOff1(),
                igpioUSBOff2(),
                igpioUSBOff3(),
                igpioUSBOff4(),
                igpioUSBOff5(),
                igpioUSBOff6(),
                igpioUSBOff7()
            };
            return igpio[iusb];
        }

        // --------------------------------------------------------------------------
        // Pin configuration
        // --------------------------------------------------------------------------

        struct PinConf
        {
            unsigned                ipin;
            int                     igpio;
            int                     dir;
            bool                    val;
        };
};

#endif // ndef LAYOUT_HPP
