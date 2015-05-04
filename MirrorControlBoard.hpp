/*
 * Class which implements useful mid-level functionality for the mirror
 * control board.
 */

#ifndef MIRRORCONTROLBOARD_HPP
#define MIRRORCONTROLBOARD_HPP

#include <Layout.hpp>
#include <SpiInterface.hpp>
#include <mcspiInterface.hpp>
#include <TLC3548_ADC.hpp>
#include <GPIOInterface.hpp>
#include <vector>

class MirrorControlBoard
{
public:
    MirrorControlBoard();
    ~MirrorControlBoard();

    enum UStep { USTEP_1, USTEP_2, USTEP_4, USTEP_8 };
    enum Dir { DIR_EXTEND, DIR_RETRACT, DIR_NONE };
    enum GPIODir { DIR_OUTPUT, DIR_INPUT};

    //------------------------------------------------------------------------------
    // Power Control Function Prototypes
    //------------------------------------------------------------------------------

    // Powers down everything
    void powerDownAll();
    void powerUpAll();

    // Powers down everything except USB
    void powerDownBase();
    void powerUpBase();

    // Set the USB power enable_bar bit
    void powerDownUSB(unsigned iusb);
    void powerDownAllUSB();

    // Clear the USB power enable_bar bit
    void powerUpUSB(unsigned iusb);
    void powerUpAllUSB();

    // Check the USB power enable_bar bit
    bool isUSBPoweredUp(unsigned iusb);

    void powerDownDriveControllers();
    void powerUpDriveControllers();
    bool isDriveControllersPoweredUp();

    void powerDownEncoders();
    void powerUpEncoders();
    bool isEncodersPoweredUp();

    void powerUpADCs();
    void powerDownADCs();
    bool isADCsPoweredUp();

    //------------------------------------------------------------------------------
    // Motor Driver Function Prototypes
    //------------------------------------------------------------------------------

    // Steps motor a single step in a direction specified by dir, with some delay controlling the io speed
    void stepOneDrive(unsigned idrive, Dir dir, unsigned frequency = 1000);

    // Simultaneously Steps all drives in a configurable direction..
    void stepAllDrives(Dir dr1_dir, Dir dr2_dir, Dir dr3_dir,
            Dir dr4_dir, Dir dr5_dir, Dir dr6_dir,
            unsigned frequency = 1000);

    void setPhaseZeroOnAllDrives();

    void enableDriveSR(bool enable = true);
    void disableDriveSR();
    bool isDriveSREnabled();

    void setUStep(UStep ustep);
    UStep getUStep();

    // Enable or Disable Stepper Motors
    void enableDrive(unsigned idrive, bool enable = true);
    void enableAllDrives(bool enable = true);
    void disableDrive(unsigned idrive);
    void disableAllDrives();

    // Check Stepper Motor Enabled
    bool isDriveEnabled(unsigned idrive);

    void enableDriveHiCurrent(bool enable = true);
    void disableDriveHiCurrent();
    bool isDriveHiCurrentEnabled();

    //------------------------------------------------------------------------------
    // ADC Function Prototypes
    //------------------------------------------------------------------------------

    // Asserts Correct ADC Chip Select Line
    void selectADC(unsigned iadc);

    // Writes initialization codes to ADC
    void initializeADC(unsigned iadc);

    // Measures ADC and returns result as value
    uint32_t measureADC(unsigned iadc, unsigned ichan);

    // Makes some specified number measurements on ADC and keeps track of sum, sum of squares, min and max for statistics..
    void measureADCStat(unsigned iadc, unsigned ichan, unsigned nmeas, uint32_t& sum, uint64_t& sumsq, uint32_t& min, uint32_t& max, unsigned nburn = 0);

    // Measure encoder position
    int measureEncoder(unsigned ichan, unsigned calib_lo, unsigned calib_hi, unsigned ticks_per_rev = 400*8, const int* correction = 0);

    // --------------------------------------------------------------------------
    // Utility functions
    // --------------------------------------------------------------------------

    // Sleeps for a half-cycle of the frequency given in the argument...
    void waitHalfPeriod(unsigned frequency);

private:
    GPIOInterface gpio;
    Layout layout;
    mcspiInterface spi;
    TLC3548_ADC ADC;
    static const unsigned m_nusb=7;
    unsigned          m_ssp_clk_div;
};

#endif // defined MIRRORCONTROLBOARD_HPP
