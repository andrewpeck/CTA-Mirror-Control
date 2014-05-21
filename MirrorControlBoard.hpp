//-*-mode:c++; mode:font-lock;-*-

/*! \file MirrorControlBoard.hpp

  Class which implements useful mid-level functionality for the mirror
  control board.

  \author     Stephen Fegan               \n
              UCLA                        \n
              sfegan@astro.ucla.edu       \n

  \version    1.0
  \date       08/12/2008
*/

#ifndef MIRRORCONTROLBOARD_HPP
#define MIRRORCONTROLBOARD_HPP

#include <Layout.hpp>
#include <Overo.hpp>
#include <TLCX5XX_ADC.hpp>
#include <vector>

//#if defined(__arm__)
//typedef Overo<> Sys;
//Overo Sys;
//#else
//typedef Overo<SimulatedRegisters> Sys;
//#endif

typedef Layout<> LO;

class MirrorControlBoard
{
public:

  MirrorControlBoard(bool no_initialize = false, unsigned nusb = 7, unsigned ssp_clk_div = 2);
  ~MirrorControlBoard(); 

  enum UStep { USTEP_1, USTEP_2, USTEP_4, USTEP_8 };
  enum Dir { DIR_EXTEND, DIR_RETRACT, DIR_NONE };
  enum GPIODir { DIR_OUTPUT, DIR_INPUT};
  
  void initialize(const unsigned ssp_clk_div);
  void initializeSPI();

  ////////////////////////////////////////////////////////////////////////////////
  // Power Control Function Prototypes
  ////////////////////////////////////////////////////////////////////////////////
  void powerDownAll();
  void powerUpAll();

  void powerDownBase();
  void powerUpBase();

  void powerDownAllUSB();
  void powerUpAllUSB();
  void powerDownUSB(unsigned iusb);
  void powerUpUSB(unsigned iusb);
  bool isUSBPoweredUp(unsigned iusb) const;

  void powerDownDriveControllers();
  void powerUpDriveControllers();
  bool isDriveControllersPoweredUp() const;

  void powerDownEncoders();
  void powerUpEncoders();
  bool isEncodersPoweredUp() const;

  void powerUpADCs();
  void powerDownADCs();
  bool isADCsPoweredUp() const;

  ////////////////////////////////////////////////////////////////////////////////
  // Motor Driver Function Prototypes
  ////////////////////////////////////////////////////////////////////////////////
  void stepOneDrive(unsigned idrive, Dir dir, unsigned ndelayloop = 3000);
  void stepAllDrives(Dir dr1_dir, Dir dr2_dir, Dir dr3_dir,
		     Dir dr4_dir, Dir dr5_dir, Dir dr6_dir,
		     unsigned ndelayloop = 3000);
  void setPhaseZeroOnAllDrives();

  void enableDriveSR(bool enable = true);
  void disableDriveSR() { enableDriveSR(false); }
  bool isDriveSREnabled() const;
  
  void setUStep(UStep ustep);
  UStep getUStep() const;

  void enableAllDrives(bool enable = true);
  void disableAllDrives() { enableAllDrives(false); }

  void enableDrive(unsigned idrive, bool enable = true);
  void disableDrive(unsigned idrive) { enableDrive(idrive, false); }
  bool isDriveEnabled(unsigned idrive) const;

  void enableDriveHiCurrent(bool enable = true);
  void disableDriveHiCurrent() { enableDriveHiCurrent(false); }
  bool isDriveHiCurrentEnabled() const;
  
  ////////////////////////////////////////////////////////////////////////////////
  // ADC Function Prototypes
  ////////////////////////////////////////////////////////////////////////////////
  void initializeADC(unsigned iadc);
  void selectADC(unsigned iadc);
  uint32_t measureADC(unsigned iadc, unsigned ichan, unsigned ndelayloop = 100);
  void measureManyADC(uint32_t* data, unsigned iadc, unsigned zchan, unsigned nchan, unsigned ndelayloop);
  uint32_t measureADCWithBurn(unsigned iadc, unsigned ichan, unsigned ndelayloop = 100);
  void measureManyADCWithBurn(uint32_t* data, unsigned iadc, unsigned zchan, unsigned nchan, unsigned ndelayloop);
  void measureADCStat(unsigned iadc, unsigned ichan, unsigned nmeas, uint32_t& sum, uint64_t& sumsq, uint32_t& min, uint32_t& max, unsigned nburn = 0, unsigned ndelayloop = 100);
  void measureADC(unsigned iadc, unsigned ichan, unsigned nmeas, std::vector<uint32_t>& vMeas, unsigned ndelayloop = 100);

  int measureEncoder(unsigned ichan, unsigned calib_lo, unsigned calib_hi, unsigned ticks_per_rev = 400*8, const int* correction = 0) {
    int meas = int(measureADCWithBurn(7, ichan));
    int calib_range = calib_lo - calib_hi;
    meas -= calib_hi;
    meas *= ticks_per_rev;
    meas /= calib_range;
    if (correction && meas>=0 && meas<int(ticks_per_rev))
        meas+=correction[meas];
    return meas;
  }

  // --------------------------------------------------------------------------
  // Utility functions
  // --------------------------------------------------------------------------

  void loopDelay(unsigned nloop); 
  void usecDelay(unsigned nusec); 
  uint64_t serialNumber(); 

private:
  //Overo             m_sys;
  unsigned          m_nusb;
  unsigned          m_ssp_clk_div;
};

#endif // defined MIRRORCONTROLBOARD_HPP
