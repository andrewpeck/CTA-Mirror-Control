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

#include <VerdexXM4.hpp>
#include <TLCX5XX_ADC.hpp>

#define PIN60_GROUND_1             1

#define PIN60_DR_SR_BAR            2
#define PIN60_DR_MS1               3
#define PIN60_DR_MS2               4
#define PIN60_DR_SLEEP_BAR         5
#define PIN60_DR_RESET_BAR         6

#define PIN60_GROUND_7             7

#define PIN60_ADC_SEL1             8

#define PIN60_UNUSED_9             9
#define PIN60_UNUSED_10            10

#define PIN60_USBH_P2              11

#define PIN60_UNUSED_12            12

#define PIN60_DR4_DIR              13

#define PIN60_DR5_ENABLE_BAR       14

#define PIN60_USB2_ENABLE_BAR      15 
#define PIN60_USB7_ENABLE_BAR      16 
#define PIN60_USB1_ENABLE_BAR      17 

#define PIN60_POWER_ADC            18

#define PIN60_I2C_SCL              19
#define PIN60_I2C_SDA              20

#define PIN60_SSP_CLK_PIN          21
#define PIN60_SSP_FRM_PIN          22
#define PIN60_SSP_TXD_PIN          23

#define PIN60_USB_OTG              24

#define PIN60_SYSEN_25             25

#define PIN60_CONSOLE_TXD          26

#define PIN60_SSP_RXD_PIN          27

#define PIN60_VBATT_28             28
#define PIN60_VBATT_29             29
#define PIN60_VBATT_30             30
#define PIN60_GROUND_31            31

#define PIN60_USB6_ENABLE_BAR      32

#define PIN60_DR_POWER_INC_BAR     33

#define PIN60_DR6_DIR              34
#define PIN60_DR6_STEP             35
#define PIN60_DR6_ENABLE_BAR       36

#define PIN60_NM_RESET             37

#define PIN60_DR2_ENABLE_BAR       38

#define PIN60_DR5_DIR              39

#define PIN60_ENCODER_ENABLE       40

#define PIN60_USB_RESET            41

#define PIN60_GROUND_42            42

#define PIN60_USB4_ENABLE_BAR      43
#define PIN60_USB3_ENABLE_BAR      44

#define PIN60_DR5_STEP             45

#define PIN60_DR4_ENABLE_BAR       46
#define PIN60_DR4_STEP             47

#define PIN60_DR3_DIR              48
#define PIN60_DR3_STEP             49
#define PIN60_DR3_ENABLE_BAR       50

#define PIN60_USB5_ENABLE_BAR      51

#define PIN60_DR2_DIR              52
#define PIN60_DR2_STEP             53

#define PIN60_CONSOLE_RXD          54

#define PIN60_ADC_SEL2             55

#define PIN60_DR1_DIR              56
#define PIN60_DR1_STEP             57
#define PIN60_DR1_ENABLE_BAR       58

#define PIN60_USBH_N               59

#define PIN60_GROUND_60            60

#define LED_PWM                    2
#define ADC_SSP                    1

typedef TLC3548                    ADC;

template<typename REV = CurrentRev> class Layout
{
public:

  // --------------------------------------------------------------------------
  // Functions mapping from function to pin on 60pin connector
  // --------------------------------------------------------------------------

  static unsigned ipin60DRSRBar() { return PIN60_DR_SR_BAR; }
  static unsigned ipin60DRMS1() { return PIN60_DR_MS1; }
  static unsigned ipin60DRMS2() { return PIN60_DR_MS2; }
  static unsigned ipin60DRSleepBar() { return PIN60_DR_SLEEP_BAR; }
  static unsigned ipin60DRResetBar() { return PIN60_DR_RESET_BAR; }

  static unsigned ipin60ADCSel1() { return PIN60_ADC_SEL1; }
  static unsigned ipin60ADCSel2() { return PIN60_ADC_SEL2; }

  static unsigned ipin60I2CSCL() { return PIN60_I2C_SCL; }
  static unsigned ipin60I2CSDA() { return PIN60_I2C_SDA; }

  static unsigned ipin60SSPClk() { return PIN60_SSP_CLK_PIN; }
  static unsigned ipin60SSPFrm() { return PIN60_SSP_FRM_PIN; }
  static unsigned ipin60SSPTXD() { return PIN60_SSP_TXD_PIN; }
  static unsigned ipin60SSPRXD() { return PIN60_SSP_RXD_PIN; }

  static unsigned ipin60ConsoleRXD() { return PIN60_CONSOLE_RXD; }
  static unsigned ipin60ConsoleTXD() { return PIN60_CONSOLE_TXD; }

  static unsigned ipin60DRPowerIncreaseBar() { return PIN60_DR_POWER_INC_BAR; }
  static unsigned ipin60EncoderEnable() { return PIN60_ENCODER_ENABLE; }

  static unsigned ipin60DR6Dir() { return PIN60_DR6_DIR; }
  static unsigned ipin60DR6Step() { return PIN60_DR6_STEP; }
  static unsigned ipin60DR6EnableBar() { return PIN60_DR6_ENABLE_BAR; }

  static unsigned ipin60DR5Dir() { return PIN60_DR5_DIR; }
  static unsigned ipin60DR5Step() { return PIN60_DR5_STEP; }
  static unsigned ipin60DR5EnableBar() { return PIN60_DR5_ENABLE_BAR; }

  static unsigned ipin60DR4Dir() { return PIN60_DR4_DIR; }
  static unsigned ipin60DR4Step() { return PIN60_DR4_STEP; }
  static unsigned ipin60DR4EnableBar() { return PIN60_DR4_ENABLE_BAR; }

  static unsigned ipin60DR3Dir() { return PIN60_DR3_DIR; }
  static unsigned ipin60DR3Step() { return PIN60_DR3_STEP; }
  static unsigned ipin60DR3EnableBar() { return PIN60_DR3_ENABLE_BAR; }

  static unsigned ipin60DR2Dir() { return PIN60_DR2_DIR; }
  static unsigned ipin60DR2Step() { return PIN60_DR2_STEP; }
  static unsigned ipin60DR2EnableBar() { return PIN60_DR2_ENABLE_BAR; }

  static unsigned ipin60DR1Dir() { return PIN60_DR1_DIR; }
  static unsigned ipin60DR1Step() { return PIN60_DR1_STEP; }
  static unsigned ipin60DR1EnableBar() { return PIN60_DR1_ENABLE_BAR; }

  static unsigned ipin60USBOTG() { return PIN60_USB_OTG; }
  static unsigned ipin60USBReset() { return PIN60_USB_RESET; }
  static unsigned ipin60USB1EnableBar() { return PIN60_USB1_ENABLE_BAR; }
  static unsigned ipin60USB2EnableBar() { return PIN60_USB2_ENABLE_BAR; }
  static unsigned ipin60USB3EnableBar() { return PIN60_USB3_ENABLE_BAR; }
  static unsigned ipin60USB4EnableBar() { return PIN60_USB4_ENABLE_BAR; }
  static unsigned ipin60USB5EnableBar() { return PIN60_USB5_ENABLE_BAR; }
  static unsigned ipin60USB6EnableBar() { return PIN60_USB6_ENABLE_BAR; }
  static unsigned ipin60USB7EnableBar() { return PIN60_USB7_ENABLE_BAR; }

  static unsigned ipin60PowerADC() { return PIN60_POWER_ADC; }

  static unsigned ipin60Unused9()  { return PIN60_UNUSED_9; }
  static unsigned ipin60Unused10() { return PIN60_UNUSED_10; }
  static unsigned ipin60Unused12() { return PIN60_UNUSED_12; }

  static unsigned ipin60DRDir(const unsigned idrive)
  {
    static const unsigned ipin60[] = 
      { ipin60DR1Dir(), ipin60DR2Dir(), ipin60DR3Dir(),
	ipin60DR4Dir(), ipin60DR5Dir(), ipin60DR6Dir() };
    return ipin60[idrive];
  }

  static unsigned ipin60DRStep(const unsigned idrive)
  {
    static const unsigned ipin60[] = 
      { ipin60DR1Step(), ipin60DR2Step(), ipin60DR3Step(),
	ipin60DR4Step(), ipin60DR5Step(), ipin60DR6Step() };
    return ipin60[idrive];
  }

  static unsigned ipin60DREnableBar(const unsigned idrive)
  {
    static const unsigned ipin60[] = 
      { ipin60DR1EnableBar(), ipin60DR2EnableBar(), ipin60DR3EnableBar(),
	ipin60DR4EnableBar(), ipin60DR5EnableBar(), ipin60DR6EnableBar() };
    return ipin60[idrive];
  }

  static unsigned ipin60USBEnableBar(const unsigned iusb)
  {
    static const unsigned ipin60[] = 
      { ipin60USB1EnableBar(), ipin60USB2EnableBar(),
	ipin60USB3EnableBar(), ipin60USB4EnableBar(),
	ipin60USB5EnableBar(), ipin60USB6EnableBar(),
	ipin60USB7EnableBar() };
    return ipin60[iusb];
  }


  // --------------------------------------------------------------------------
  // Functions mapping from function to GPIO number
  // --------------------------------------------------------------------------

  static unsigned igpioDRSRBar() { return REV::pin60ToGPIO(PIN60_DR_SR_BAR); }
  static unsigned igpioDRMS1() { return REV::pin60ToGPIO(PIN60_DR_MS1); }
  static unsigned igpioDRMS2() { return REV::pin60ToGPIO(PIN60_DR_MS2); }
  static unsigned igpioDRSleepBar() { return REV::pin60ToGPIO(PIN60_DR_SLEEP_BAR); }
  static unsigned igpioDRResetBar() { return REV::pin60ToGPIO(PIN60_DR_RESET_BAR); }

  static unsigned igpioADCSel1() { return REV::pin60ToGPIO(PIN60_ADC_SEL1); }
  static unsigned igpioADCSel2() { return REV::pin60ToGPIO(PIN60_ADC_SEL2); }

  static unsigned igpioI2CSCL() { return REV::pin60ToGPIO(PIN60_I2C_SCL); }
  static unsigned igpioI2CSDA() { return REV::pin60ToGPIO(PIN60_I2C_SDA); }

  static unsigned igpioSSPClk() { return REV::pin60ToGPIO(PIN60_SSP_CLK_PIN); }
  static unsigned igpioSSPFrm() { return REV::pin60ToGPIO(PIN60_SSP_FRM_PIN); }
  static unsigned igpioSSPTXD() { return REV::pin60ToGPIO(PIN60_SSP_TXD_PIN); }
  static unsigned igpioSSPRXD() { return REV::pin60ToGPIO(PIN60_SSP_RXD_PIN); }

  static unsigned igpioConsoleRXD() { return REV::pin60ToGPIO(PIN60_CONSOLE_RXD); }
  static unsigned igpioConsoleTXD() { return REV::pin60ToGPIO(PIN60_CONSOLE_TXD); }

  static unsigned igpioDRPowerIncreaseBar() { return REV::pin60ToGPIO(PIN60_DR_POWER_INC_BAR); }
  static unsigned igpioEncoderEnable() { return REV::pin60ToGPIO(PIN60_ENCODER_ENABLE); }

  static unsigned igpioDR6Dir() { return REV::pin60ToGPIO(PIN60_DR6_DIR); }
  static unsigned igpioDR6Step() { return REV::pin60ToGPIO(PIN60_DR6_STEP); }
  static unsigned igpioDR6EnableBar() { return REV::pin60ToGPIO(PIN60_DR6_ENABLE_BAR); }

  static unsigned igpioDR5Dir() { return REV::pin60ToGPIO(PIN60_DR5_DIR); }
  static unsigned igpioDR5Step() { return REV::pin60ToGPIO(PIN60_DR5_STEP); }
  static unsigned igpioDR5EnableBar() { return REV::pin60ToGPIO(PIN60_DR5_ENABLE_BAR); }

  static unsigned igpioDR4Dir() { return REV::pin60ToGPIO(PIN60_DR4_DIR); }
  static unsigned igpioDR4Step() { return REV::pin60ToGPIO(PIN60_DR4_STEP); }
  static unsigned igpioDR4EnableBar() { return REV::pin60ToGPIO(PIN60_DR4_ENABLE_BAR); }

  static unsigned igpioDR3Dir() { return REV::pin60ToGPIO(PIN60_DR3_DIR); }
  static unsigned igpioDR3Step() { return REV::pin60ToGPIO(PIN60_DR3_STEP); }
  static unsigned igpioDR3EnableBar() { return REV::pin60ToGPIO(PIN60_DR3_ENABLE_BAR); }

  static unsigned igpioDR2Dir() { return REV::pin60ToGPIO(PIN60_DR2_DIR); }
  static unsigned igpioDR2Step() { return REV::pin60ToGPIO(PIN60_DR2_STEP); }
  static unsigned igpioDR2EnableBar() { return REV::pin60ToGPIO(PIN60_DR2_ENABLE_BAR); }

  static unsigned igpioDR1Dir() { return REV::pin60ToGPIO(PIN60_DR1_DIR); }
  static unsigned igpioDR1Step() { return REV::pin60ToGPIO(PIN60_DR1_STEP); }
  static unsigned igpioDR1EnableBar() { return REV::pin60ToGPIO(PIN60_DR1_ENABLE_BAR); }

  static unsigned igpioUSBOTG() { return REV::pin60ToGPIO(PIN60_USB_OTG); }
  static unsigned igpioUSBReset() { return REV::pin60ToGPIO(PIN60_USB_RESET); }
  static unsigned igpioUSB1EnableBar() { return REV::pin60ToGPIO(PIN60_USB1_ENABLE_BAR); }
  static unsigned igpioUSB2EnableBar() { return REV::pin60ToGPIO(PIN60_USB2_ENABLE_BAR); }
  static unsigned igpioUSB3EnableBar() { return REV::pin60ToGPIO(PIN60_USB3_ENABLE_BAR); }
  static unsigned igpioUSB4EnableBar() { return REV::pin60ToGPIO(PIN60_USB4_ENABLE_BAR); }
  static unsigned igpioUSB5EnableBar() { return REV::pin60ToGPIO(PIN60_USB5_ENABLE_BAR); }
  static unsigned igpioUSB6EnableBar() { return REV::pin60ToGPIO(PIN60_USB6_ENABLE_BAR); }
  static unsigned igpioUSB7EnableBar() { return REV::pin60ToGPIO(PIN60_USB7_ENABLE_BAR); }

  static unsigned igpioPowerADC() { return REV::pin60ToGPIO(PIN60_POWER_ADC); }

  static unsigned igpioUnused9() { return REV::pin60ToGPIO(PIN60_UNUSED_9); }
  static unsigned igpioUnused10() { return REV::pin60ToGPIO(PIN60_UNUSED_10); }
  static unsigned igpioUnused12() { return REV::pin60ToGPIO(PIN60_UNUSED_12); }

  static unsigned igpioDRDir(const unsigned idrive)
  {
    static const unsigned igpio[] = 
      { igpioDR1Dir(), igpioDR2Dir(), igpioDR3Dir(),
	igpioDR4Dir(), igpioDR5Dir(), igpioDR6Dir() };
    return igpio[idrive];
  }

  static unsigned igpioDRStep(const unsigned idrive)
  {
    static const unsigned igpio[] = 
      { igpioDR1Step(), igpioDR2Step(), igpioDR3Step(),
	igpioDR4Step(), igpioDR5Step(), igpioDR6Step() };
    return igpio[idrive];
  }

  static unsigned igpioDREnableBar(const unsigned idrive)
  {
    static const unsigned igpio[] = 
      { igpioDR1EnableBar(), igpioDR2EnableBar(), igpioDR3EnableBar(),
	igpioDR4EnableBar(), igpioDR5EnableBar(), igpioDR6EnableBar() };
    return igpio[idrive];
  }

  static unsigned igpioUSBEnableBar(const unsigned iusb)
  {
    static const unsigned igpio[] = 
      { igpioUSB1EnableBar(), igpioUSB2EnableBar(), igpioUSB3EnableBar(),
	igpioUSB4EnableBar(), igpioUSB5EnableBar(), igpioUSB6EnableBar(),
	igpioUSB7EnableBar() };
    return igpio[iusb];
  }

  // --------------------------------------------------------------------------
  // Pin configuration
  // --------------------------------------------------------------------------
  
  struct PinConf
  {
    unsigned               ipin;
    int                    igpio;
    VerdexXM4Base::GPIODir dir;
    VerdexXM4Base::GPIOFn  fn;
    bool                   val;
  };

#define DC60(IPIN,DIR,FN,VAL) { IPIN, REV::pin60ToGPIO(IPIN), VerdexXM4Base::DIR, VerdexXM4Base::FN, VAL }

  static unsigned pin60Conf(const PinConf*& C)
  {
    static const PinConf PC[] = 
      { 
	DC60(PIN60_DR_SR_BAR            , GPIODIR_OUT, GPIOFN_IO,   0),
	DC60(PIN60_DR_MS1               , GPIODIR_OUT, GPIOFN_IO,   1),
	DC60(PIN60_DR_MS2               , GPIODIR_OUT, GPIOFN_IO,   1),
	DC60(PIN60_DR_SLEEP_BAR         , GPIODIR_OUT, GPIOFN_IO,   0),
	DC60(PIN60_DR_RESET_BAR         , GPIODIR_OUT, GPIOFN_IO,   1),

	DC60(PIN60_ADC_SEL1             , GPIODIR_OUT, GPIOFN_IO,   1),
	DC60(PIN60_ADC_SEL2             , GPIODIR_OUT, GPIOFN_IO,   1),

	DC60(PIN60_USB_RESET            , GPIODIR_OUT, GPIOFN_IO,   0),
	DC60(PIN60_USB_OTG              , GPIODIR_OUT, GPIOFN_IO,   0),
	DC60(PIN60_USB1_ENABLE_BAR      , GPIODIR_OUT, GPIOFN_IO,   1),
	DC60(PIN60_USB2_ENABLE_BAR      , GPIODIR_OUT, GPIOFN_IO,   1),
	DC60(PIN60_USB3_ENABLE_BAR      , GPIODIR_OUT, GPIOFN_IO,   1),
	DC60(PIN60_USB4_ENABLE_BAR      , GPIODIR_OUT, GPIOFN_IO,   1),
	DC60(PIN60_USB5_ENABLE_BAR      , GPIODIR_OUT, GPIOFN_IO,   1),
	DC60(PIN60_USB6_ENABLE_BAR      , GPIODIR_OUT, GPIOFN_IO,   1),
	DC60(PIN60_USB7_ENABLE_BAR      , GPIODIR_OUT, GPIOFN_IO,   1),

//	DC60(PIN60_I2C_SCL              , GPIODIR_IN,  GPIOFN_ALT1, 0),
//	DC60(PIN60_I2C_SDA              , GPIODIR_IN,  GPIOFN_ALT1, 0),

	DC60(PIN60_SSP_CLK_PIN          , GPIODIR_OUT, GPIOFN_ALT1, 0),
	DC60(PIN60_SSP_FRM_PIN          , GPIODIR_OUT, GPIOFN_ALT3, 0),
	DC60(PIN60_SSP_TXD_PIN          , GPIODIR_OUT, GPIOFN_ALT1, 0),
	DC60(PIN60_SSP_RXD_PIN          , GPIODIR_IN,  GPIOFN_ALT1, 0),

//	DC60(PIN60_CONSOLE_RXD          , GPIODIR_IN,  GPIOFN_ALT1, 0),
//	DC60(PIN60_CONSOLE_TXD          , GPIODIR_OUT, GPIOFN_ALT2, 0),

	DC60(PIN60_DR_POWER_INC_BAR     , GPIODIR_OUT, GPIOFN_IO,   1),
	DC60(PIN60_ENCODER_ENABLE       , GPIODIR_OUT, GPIOFN_IO,   0),

	DC60(PIN60_DR6_DIR              , GPIODIR_OUT, GPIOFN_IO,   0),
	DC60(PIN60_DR6_STEP             , GPIODIR_OUT, GPIOFN_IO,   0),
	DC60(PIN60_DR6_ENABLE_BAR       , GPIODIR_OUT, GPIOFN_IO,   1),
	
	DC60(PIN60_DR5_DIR              , GPIODIR_OUT, GPIOFN_IO,   0),
	DC60(PIN60_DR5_STEP             , GPIODIR_OUT, GPIOFN_IO,   0),
	DC60(PIN60_DR5_ENABLE_BAR       , GPIODIR_OUT, GPIOFN_IO,   1),

	DC60(PIN60_DR4_DIR              , GPIODIR_OUT, GPIOFN_IO,   0),
	DC60(PIN60_DR4_STEP             , GPIODIR_OUT, GPIOFN_IO,   0),
	DC60(PIN60_DR4_ENABLE_BAR       , GPIODIR_OUT, GPIOFN_IO,   1),

	DC60(PIN60_DR3_DIR              , GPIODIR_OUT, GPIOFN_IO,   0),
	DC60(PIN60_DR3_STEP             , GPIODIR_OUT, GPIOFN_IO,   0),
	DC60(PIN60_DR3_ENABLE_BAR       , GPIODIR_OUT, GPIOFN_IO,   1),

	DC60(PIN60_DR2_DIR              , GPIODIR_OUT, GPIOFN_IO,   0),
	DC60(PIN60_DR2_STEP             , GPIODIR_OUT, GPIOFN_IO,   0),
	DC60(PIN60_DR2_ENABLE_BAR       , GPIODIR_OUT, GPIOFN_IO,   1),

	DC60(PIN60_DR1_DIR              , GPIODIR_OUT, GPIOFN_IO,   0),
	DC60(PIN60_DR1_STEP             , GPIODIR_OUT, GPIOFN_IO,   0),
	DC60(PIN60_DR1_ENABLE_BAR       , GPIODIR_OUT, GPIOFN_IO,   1),

	DC60(PIN60_POWER_ADC            , GPIODIR_OUT, GPIOFN_IO,   0),

	DC60(PIN60_UNUSED_9             , GPIODIR_IN,  GPIOFN_IO,   0),
	DC60(PIN60_UNUSED_10            , GPIODIR_IN,  GPIOFN_IO,   0),
	DC60(PIN60_UNUSED_12            , GPIODIR_IN,  GPIOFN_IO,   0),
      };

    C = PC;
    return N(PC);
  }

};

#endif // ndef LAYOUT_HPP
