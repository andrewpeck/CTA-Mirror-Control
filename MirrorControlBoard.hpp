/*
 * Class which implements useful mid-level functionality for the mirror
 * control board.
 */

#ifndef MIRRORCONTROLBOARD_HPP
#define MIRRORCONTROLBOARD_HPP

#include <Layout.hpp>
#include <SpiInterface.hpp>
#include <TLC3548_ADC.hpp>
#include <GPIOInterface.hpp>
#include <vector>

class MirrorControlBoard
{
public:
    MirrorControlBoard(bool no_initialize = true, unsigned nusb = 7);
    ~MirrorControlBoard();

    enum UStep { USTEP_1, USTEP_2, USTEP_4, USTEP_8 };
    enum Dir { DIR_EXTEND, DIR_RETRACT, DIR_NONE };
    enum GPIODir { DIR_OUTPUT, DIR_INPUT};

    // sets microstep, enables drive SR (synchronous rectification), disables hi current, resets drive phase
    void initialize(const unsigned ssp_clk_div);
    // used to be useful. using spidev driver it is no longer needed. 
    // void initializeSPI();

    //------------------------------------------------------------------------------
    // Power Control Function Prototypes
    //------------------------------------------------------------------------------
    void powerDownAll();
    void powerUpAll();

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
    void stepOneDrive(unsigned idrive, Dir dir, unsigned frequency = 400);

    // Simultaneously Steps all drives in a configurable direction..
    void stepAllDrives(Dir dr1_dir, Dir dr2_dir, Dir dr3_dir,
            Dir dr4_dir, Dir dr5_dir, Dir dr6_dir,
            unsigned frequency = 400);

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
    uint32_t measureADC(unsigned iadc, unsigned ichan, unsigned ndelayloop = 100);
    // Measures ADC for some number of loops and fills the results into a vector
    void measureADC(unsigned iadc, unsigned ichan, unsigned nmeas, std::vector<uint32_t>& vMeas, unsigned ndelayloop = 100);
    // Measures ADC over several \Channels and Fulls data into an array (data)
    void measureManyADC(uint32_t* data, unsigned iadc, unsigned zchan, unsigned nchan, unsigned ndelayloop);

    // Measures by having the ADC sample a few times before reading out from the FIFO (don't know the purpose)
    uint32_t measureADCWithBurn(unsigned iadc, unsigned ichan, unsigned ndelayloop = 100);
    // Loop over ADC Channels taking measurements and filling data array. Also has the ADC sample a few times before reading data
    void measureManyADCWithBurn(uint32_t* data, unsigned iadc, unsigned zchan, unsigned nchan, unsigned ndelayloop);

    // Makes some specified number measurements on ADC and keeps track of sum, sum of squares, min and max for statistics.. 
    void measureADCStat(unsigned iadc, unsigned ichan, unsigned nmeas, uint32_t& sum, uint64_t& sumsq, uint32_t& min, uint32_t& max, unsigned nburn = 0, unsigned ndelayloop = 100);

    // Measure encoder position
    int measureEncoder(unsigned ichan, unsigned calib_lo, unsigned calib_hi, unsigned ticks_per_rev = 400*8, const int* correction = 0); 

    // --------------------------------------------------------------------------
    // Utility functions
    // --------------------------------------------------------------------------

    // Sleeps for a half-cycle of the frequency given in the argument...
    // calibrated over the range from 0-1000 Hz.. should work elsewhere 
    void waitHalfPeriod(unsigned frequency);

private:
    GPIOInterface gpio;
    Layout layout;
    SpiInterface spi;
    TLC3548_ADC ADC; 
    unsigned          m_nusb;
    unsigned          m_ssp_clk_div;
};

#endif // defined MIRRORCONTROLBOARD_HPP
