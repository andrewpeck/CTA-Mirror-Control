//-*-mode:c++; mode:font-lock;-*-

/*! \file cbc.cpp

  Console board control

  \author     Stephen Fegan               \n
              UCLA                        \n
              sfegan@astro.ucla.edu       \n

  \version    1.0
  \date       06/04/2008
*/

#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <string>
#include <cassert>
#include <vector>

#include "MirrorControlBoard.hpp"

#include "cbc.h"

/* by Jim Ulery */
static unsigned julery_isqrt(unsigned long val) {
  unsigned long temp, g=0, b = 0x8000, bshft = 15;
  do {
    if (val >= (temp = (((g << 1) + b)<<bshft--))) {
      g += b;
      val -= temp;
    }
  } while (b >>= 1);
  return g;
}

const char* usage_text = "The following commands are available:\n\
\n\
initialize CLK_DIV=4: Initialize the hardware. Should be done once after boot-up.\n\
\n\
power_down: Go into power saving mode. Power down encoders, USB and A3977.\n\
\n\
power_up:   Power up all on-board and off-board electronics.\n\
\n\
power_down_encoders:\n\
            Power down encoders.\n\
\n\
power_up_encoders:\n\
            Power up encoders.\n\
\n\
power_down_a3977:\n\
            Power down A3977 motor controllers.\n\
\n\
power_up_a3977:\n\
            Power up A3977 motor controllers.\n\
\n\
reset:      Send reset command to drive electronics.\n\
\n\
enable_sr:  Enable Synchronous Rectification (SR) mode.\n\
\n\
disable_sr: Disable Synchronous Rectification (SR) mode.\n\
\n\
set_microstep MS={1,2,4,8}:\n\
            Set number of micro steps to 1, 2, 4 or 8.\n\
\n\
enable_hi_current:\n\
            Enable high current mode on all drives.\n\
\n\
disable_hi_current:\n\
            Disable high current mode on all drives.\n\
\n\
enable_all:\n\
            Enable all drives.\n\
\n\
disable_all:\n\
            Disable all drives.\n\
\n\
enable DR={1-6}:\n\
            Enable drive (DR).\n\
\n\
disable DR={1-6}:\n\
            Disable drive (DR).\n\
\n\
enableUSB_all:\n\
            Enable all USB.\n\
\n\
disableUSB_all:\n\
            Disable all USB.\n\
\n\
enableUSB USB={1-7}:\n\
            Enable USB (USB).\n\
\n\
disableUSB USB={1-7}:\n\
            Disable USB (USB).\n\
\n\
step DR={1-6} NSTEP [DELAY]:\n\
            Step drive some number of steps (positive to extend, negative to\n\
            retract) with delay between steps given by DELAY (default 5000).\n\
\n\
slew DR={1-6} [DIR={extend,retract}] [DELAY]:\n\
            Slew drive (DR) in given direction (DIR, default extend) with\n\
            delay between steps given by DELAY (default 5000).\n\
\n\
step_all DR1_NSTEP DR2_NSTEP DR3_NSTEP DR4_NSTEP DR5_NSTEP DR6_NSTEP [DELAY]\n\
            Step all drives some number of steps (positive to extend,\n\
            negative to retract and zero to not move that drive.\n\
\n\
slew_all [DIR={extend,retract}] [DELAY]:\n\
            Slew all (enabled) drives in given direction (DIR, default \n\
            extend) with delay between steps given by DELAY (default 5000).\n\
\n\
status:     Print drive status information\n\
\n\
measure ADC={1-8} CHAN={0-11} [MEAS=1 BURN=0 DELAY=100 SCALE=5.05]:\n\
            Measure voltage of the specified ADC channel. Channel can be\n\
            given as zero to specify all channels on one ADC. Channels 9, 10\n\
            and 11 correspond to internal reference voltages on the ADC.\n\
            Prints out statistics.\n\
\n\
measure_full ADC={1-8} CHAN={0-11} [MEAS=1 DELAY=100 SCALE=5.05]:\n\
            Measure voltage of the specified ADC channel. Channel can be\n\
            given as zero to specify all channels on one ADC. Channels 9, 10\n\
            and 11 correspond to internal reference voltages on the ADC.\n\
            Prints out raw data.\n\
\n\
calibrate DR={1-6} [NSTEP=10000] [NCYCLE=0] [DELAY=10000] [ADC=7] [NMEAS=1]\n\
            Step drive and read ADC after each step, printing its value to\n\
            the terminal. If NCYCLE is 0 or 1 then it specifies the direction\n\
            of the travel, otherwise it specifies the number of half cycles\n\
            of expansion and contraction to perform.\n\
";

int usage(const char* program, std::ostream& stream, 
	   int status = EXIT_FAILURE)
{
  stream << "Usage: " << program << " command [command data]\n\n"
	 << usage_text;
  return status;
}

#if defined(__arm__)
typedef VerdexXM4<> Sys;
#else
typedef VerdexXM4<SimulatedRegisters> Sys;
#endif
typedef Layout<> LO;


int main(int argc, const char** argv)
{
  return SubMain(argc, argv);
}


int SubMain(int argc, const char** argv, std::ostream& oStr)
{
  const char* program = *argv;
  argv++, argc--;

  if(argc == 0)return usage(program, oStr, EXIT_SUCCESS);

  Sys sys;
  
  std::string command = *argv;
  argv++, argc--;

  if((command == "initialize")||(command == "init")||(command == "config"))
    {
      unsigned ssp_clk_div = 4;
      if(argc)
	{
	  ssp_clk_div = atoi(*argv);
	  argc--, argv++;
	}

      MirrorControlBoard mcb(&sys, false, 7, ssp_clk_div);
      mcb.powerUpBase();
      return EXIT_SUCCESS;
    }

  MirrorControlBoard mcb(&sys, true);

  if(command == "power_down")
    mcb.powerDownAll();
  else if(command == "power_up")
    mcb.powerUpAll();
  else if(command == "power_down_encoders")
    mcb.powerDownEncoders();
  else if(command == "power_up_encoders")
    mcb.powerUpEncoders();
  else if(command == "power_down_a3977")
    mcb.powerDownDriveControllers();
  else if(command == "power_up_a3977")
    mcb.powerUpDriveControllers();
  else if(command == "reset")
    mcb.setPhaseZeroOnAllDrives();
  else if(command == "enable_sr")
    mcb.enableDriveSR();
  else if(command == "disable_sr")
    mcb.disableDriveSR();
  else if(command == "set_microstep")
    {
      if(argc==0)usage(program, oStr);
      unsigned usint = atoi(*argv);
      MirrorControlBoard::UStep us;
      switch(usint)
	{
	case 1: us = MirrorControlBoard::USTEP_1; break;
	case 2: us = MirrorControlBoard::USTEP_2; break;
	case 4: us = MirrorControlBoard::USTEP_4; break;
	case 8: us = MirrorControlBoard::USTEP_8; break;
	default:
	  usage(program, oStr);
	}
      mcb.setUStep(us);
    }
  else if(command == "enable_hi_current")
    mcb.enableDriveHiCurrent();
  else if(command == "disable_hi_current")
    mcb.disableDriveHiCurrent();
  else if(command == "enable_all")
    mcb.enableAllDrives();
  else if(command == "disable_all")
    mcb.disableAllDrives();
  else if(command == "enable")
    {
      if(argc==0)usage(program, oStr);
      unsigned idrive = atoi(*argv);
      if((idrive<1)||(idrive>6))usage(program, oStr);
      idrive--;
      mcb.enableDrive(idrive);
    }
  else if(command == "disable")
    {
      if(argc==0)usage(program, oStr);
      unsigned idrive = atoi(*argv);
      if((idrive<1)||(idrive>6))usage(program, oStr);
      idrive--;
      mcb.disableDrive(idrive);
    }
  else if(command == "enableUSB_all")
    mcb.powerUpAllUSB();
  else if(command == "disableUSB_all")
    mcb.powerDownAllUSB();

  else if(command == "enableUSB")
    {
      if(argc==0)usage(program, oStr);
      unsigned iusb = atoi(*argv);
      if((iusb<1)||(iusb>7))usage(program, oStr);
      iusb--;
      mcb.powerUpUSB(iusb);
    }
  else if(command == "disableUSB")
    {
      if(argc==0)usage(program, oStr);
      unsigned iusb = atoi(*argv);
      if((iusb<1)||(iusb>7))usage(program, oStr);
      iusb--;
      mcb.powerDownUSB(iusb);
    }

  else if(command == "step")
    {
      if(argc==0)usage(program, oStr);
      unsigned idrive = atoi(*argv);
      argc--, argv++;
      if((idrive<1)||(idrive>6))usage(program, oStr);
      idrive--;

      if(argc==0)usage(program, oStr);
      int nstep = atoi(*argv);
      argc--, argv++;

      MirrorControlBoard::Dir dir = MirrorControlBoard::DIR_EXTEND;
      if(nstep<0)dir = MirrorControlBoard::DIR_RETRACT, nstep=-nstep;
      
      unsigned ndelay = 5000;
      if(argc)
	{
	  ndelay = atoi(*argv);
	  argc--, argv++;
	}
          
      for(unsigned istep=0;istep<unsigned(nstep);istep++)
	{
	  mcb.stepOneDrive(idrive, dir, ndelay);
	  mcb.loopDelay(ndelay);
	}
    }
  else if(command == "slew")
    {
      if(argc==0)usage(program, oStr);
      unsigned idrive = atoi(*argv);
      argc--, argv++;
      if((idrive<1)||(idrive>6))usage(program, oStr);
      idrive--;

      MirrorControlBoard::Dir dir = MirrorControlBoard::DIR_EXTEND;
      if(argc)
	{
	  std::string dirstr(*argv);
	  
	  if((dirstr.size()<=6)
	     &&(dirstr.compare(0,dirstr.size(),
			       std::string("extend"),0,dirstr.size())==0))
	    dir = MirrorControlBoard::DIR_EXTEND;
	  else if((dirstr.size()<=7)
		  &&(dirstr.compare(0,dirstr.size(),
				    std::string("retract"),0,dirstr.size())==0))
	    dir = MirrorControlBoard::DIR_RETRACT;
	  else
	    usage(program, oStr);
	  argc--, argv++;
	}

      unsigned ndelay = 5000;
      if(argc)
	{
	  ndelay = atoi(*argv);
	  argc--, argv++;
	}
          
      while(1)
	{
	  mcb.stepOneDrive(idrive, dir, ndelay);
	  mcb.loopDelay(ndelay);
	}
    }
  else if(command == "step_all")
    {
      int nstep1 = 0;
      int nstep2 = 0;
      int nstep3 = 0;
      int nstep4 = 0;
      int nstep5 = 0;
      int nstep6 = 0;

      if(argc)nstep1 = atoi(*argv), argc--, argv++;
      if(argc)nstep2 = atoi(*argv), argc--, argv++;
      if(argc)nstep3 = atoi(*argv), argc--, argv++;
      if(argc)nstep4 = atoi(*argv), argc--, argv++;
      if(argc)nstep5 = atoi(*argv), argc--, argv++;
      if(argc)nstep6 = atoi(*argv), argc--, argv++;

      unsigned ndelay = 5000;
      if(argc)
	{
	  ndelay = atoi(*argv);
	  argc--, argv++;
	}

      while(nstep1!=0 || nstep2!=0 || nstep3!=0 || 
	    nstep4!=0 || nstep5!=0 || nstep6!=0)
	{
	  MirrorControlBoard::Dir dir1 = MirrorControlBoard::DIR_NONE;
	  MirrorControlBoard::Dir dir2 = MirrorControlBoard::DIR_NONE;
	  MirrorControlBoard::Dir dir3 = MirrorControlBoard::DIR_NONE;
	  MirrorControlBoard::Dir dir4 = MirrorControlBoard::DIR_NONE;
	  MirrorControlBoard::Dir dir5 = MirrorControlBoard::DIR_NONE;
	  MirrorControlBoard::Dir dir6 = MirrorControlBoard::DIR_NONE;
	  
	  if(nstep1 > 0)dir1 = MirrorControlBoard::DIR_EXTEND, nstep1--;
	  else if(nstep1 < 0)dir1 = MirrorControlBoard::DIR_RETRACT, nstep1++;
	  if(nstep2 > 0)dir2 = MirrorControlBoard::DIR_EXTEND, nstep2--;
	  else if(nstep2 < 0)dir2 = MirrorControlBoard::DIR_RETRACT, nstep2++;
	  if(nstep3 > 0)dir3 = MirrorControlBoard::DIR_EXTEND, nstep3--;
	  else if(nstep3 < 0)dir3 = MirrorControlBoard::DIR_RETRACT, nstep3++;
	  if(nstep4 > 0)dir4 = MirrorControlBoard::DIR_EXTEND, nstep4--;
	  else if(nstep4 < 0)dir4 = MirrorControlBoard::DIR_RETRACT, nstep4++;
	  if(nstep5 > 0)dir5 = MirrorControlBoard::DIR_EXTEND, nstep5--;
	  else if(nstep5 < 0)dir5 = MirrorControlBoard::DIR_RETRACT, nstep5++;
	  if(nstep6 > 0)dir6 = MirrorControlBoard::DIR_EXTEND, nstep6--;
	  else if(nstep6 < 0)dir6 = MirrorControlBoard::DIR_RETRACT, nstep6++;

	  mcb.stepAllDrives(dir1, dir2, dir3, dir4, dir5, dir6, ndelay);
	  mcb.loopDelay(ndelay);
	}
    }
  else if(command == "slew_all")
    {
      MirrorControlBoard::Dir dir = MirrorControlBoard::DIR_EXTEND;
      if(argc)
	{
	  std::string dirstr(*argv);
	  
	  if((dirstr.size()<=6)
	     &&(dirstr.compare(0,dirstr.size(),
			       std::string("extend"),0,dirstr.size())==0))
	    dir = MirrorControlBoard::DIR_EXTEND;
	  else if((dirstr.size()<=7)
		  &&(dirstr.compare(0,dirstr.size(),
				  std::string("retract"),0,dirstr.size())==0))
	    dir = MirrorControlBoard::DIR_RETRACT;
	  else
	    usage(program, oStr);
	  argc--, argv++;
	}

      unsigned ndelay = 5000;
      if(argc)
	{
	  ndelay = atoi(*argv);
	  argc--, argv++;
	}
          
      while(1)
	{
	  mcb.stepAllDrives(dir, dir, dir, dir, dir, dir);
	  mcb.loopDelay(ndelay);
	}      
    }
  else if(command == "status")
    {
      oStr << "Drives:"
		<< (mcb.isDriveControllersPoweredUp()?
		    (char*)"":(char*)" A3977-OFF")
		<< (sys.gpioReadLevel(LO::igpioDRResetBar())?
		    (char*)"":(char*)" RESET")
		<< (mcb.isDriveSREnabled()?
		    (char*)" SR":(char*)"");
      switch(mcb.getUStep())
	{
	case MirrorControlBoard::USTEP_8: oStr << " 8-uSTEP"; break;
	case MirrorControlBoard::USTEP_4: oStr << " 4-uSTEP"; break;
	case MirrorControlBoard::USTEP_2: oStr << " 2-uSTEP"; break;
	case MirrorControlBoard::USTEP_1: oStr << " 1-uSTEP"; break;
	}
      oStr << (mcb.isDriveHiCurrentEnabled()?
		    (char*)" HI-CURRENT":(char*)"")
		<< (mcb.isEncodersPoweredUp()?
		    (char*)" ENCODERS-ON":(char*)"ENCODERS-OFF")
        << " LED: " << sys.pwmGetDutyCycle(LED_PWM) << "/" << sys.pwmGetPeriod(LED_PWM)
		<< '\n';
	
      for(unsigned idrive=0;idrive<6;idrive++)
	{
	  oStr << "  " << idrive+1 << ":"
		    << (mcb.isDriveEnabled(idrive)?
			" ENABLED ":" DISABLED")
		    << '\n';
	}

//       oStr << "\n\nPSDs:\n";
//       for(unsigned ipsd=0;ipsd<7;ipsd++)
// 	{
// 	  oStr << "  " << ipsd+1 << ":"
// 		    << (mcb.isPSDPoweredUp(ipsd)?
// 			" ENABLED ":" DISABLED")
// 		    << '\n';
// 	}
    }
  else if(command == "measure")
    {
      if(!sys.clockIsEnabledSSP(ADC_SSP))
	{
	  oStr << "SSP clock is not enabled. Power up the board!\n";
	  return EXIT_FAILURE;
	}

#define NCHANMAX 11
      if(argc==0)usage(program, oStr);
      unsigned iadc = atoi(*argv);
      argc--, argv++;
      if((iadc<1)||(iadc>8))usage(program, oStr);
      iadc--;

      if(argc==0)usage(program, oStr);
      unsigned zchan = atoi(*argv);
      argc--, argv++;
      if(zchan>NCHANMAX)usage(program, oStr);
      unsigned nchan = zchan;
      if(zchan==0)nchan=NCHANMAX;
      else zchan--;

      unsigned nmeas = 1;
      if(argc)
	{
	  nmeas = atoi(*argv);
	  argc--, argv++;
	}

      unsigned nburn = 0;
      if(argc)
	{
	  nburn = atoi(*argv);
	  argc--, argv++;
	}

      unsigned ndelay = 100;
      if(argc)
	{
	  ndelay = atoi(*argv);
	  argc--, argv++;
	}

      float volt_full = 5.05;
      if(argc)
	{
	  volt_full = atof(*argv);
	  argc--, argv++;
	}

      int hex_out = 0;
      if(argc)
	{
	  hex_out = atoi(*argv);
	  argc--, argv++;
	}

      oStr << "ADC:      " << iadc+1 << '\n'
		<< "Channels: " << zchan+1 << " to " << nchan << '\n'
		<< "NMeas:    " << nmeas << '\n'
		<< "NBurn:    " << nburn << '\n'
		<< "NDelay:   " << ndelay << '\n'
		<< "FullVolt: " << volt_full << '\n';

      uint32_t sum[NCHANMAX];
      uint64_t sum2[NCHANMAX];
      uint32_t max[NCHANMAX];
      uint32_t min[NCHANMAX];

      for(unsigned ichan=zchan; ichan<nchan; ichan++)
	mcb.measureADCStat(iadc, ichan, nmeas, 
			   sum[ichan], sum2[ichan], min[ichan], max[ichan],
			   nburn, ndelay);
	
      for(unsigned ichan=zchan; ichan<nchan; ichan++)
	{
	  uint32_t mean = sum[ichan]/nmeas;
	  uint64_t s = sum[ichan];
	  uint64_t var = (sum2[ichan] - s*s/nmeas)/nmeas;
	  //oStr << sum2[ichan] << ' ' << s << ' ' << s*s/nmeas <<'\n';
	  uint32_t rms = julery_isqrt(uint32_t(var));


	  if(hex_out != 0)
	    oStr << std::setw(2) << ichan+1 << ' '
		    << std::hex
		    << std::fixed << std::setw(6)
		    << mean << ' '
		    << std::setw(6)
		    << rms << ' '
		    << std::setw(6)
		    << max[ichan] << ' '
		    << std::setw(6)
		    << min[ichan] 
		    << std::dec
		    << '\n';

	  else
	    oStr << std::setw(2) << ichan+1 << ' '
		    << std::fixed << std::setw(6) << std::setprecision(4) 
		    << TLC3548::voltData(mean,volt_full) << ' '
		    << std::setw(6) << std::setprecision(4) 
		    << TLC3548::voltData(rms,volt_full) << ' '
		    << std::setw(6) << std::setprecision(4) 
		    << TLC3548::voltData(max[ichan],volt_full) << ' '
		    << std::setw(6) << std::setprecision(4) 
		    << TLC3548::voltData(min[ichan],volt_full) 
		    << '\n';
	}
      if(nchan != zchan + 1)
      {
          int v[4];
          v[0] = sum[0]/nmeas;
          v[1] = sum[1]/nmeas;
          v[2] = sum[2]/nmeas;
          v[3] = sum[3]/nmeas;

          std::cout << "POS " << std::setw(7) << std::setprecision(4)
                << double(v[0] + v[1] - v[2] - v[3])/(v[0] + v[1] + v[2] + v[3]) << " ";
          std::cout << std::setw(7) << std::setprecision(4) 
                << double(v[1] + v[2] - v[3] - v[0])/(v[0] + v[1] + v[2] + v[3]) << "\n";
          std::cout << "POS " << std::setw(7) << std::setprecision(4) 
                << double(v[0] + v[2] - v[1] - v[3])/(v[0] + v[1] + v[2] + v[3]) << " ";
          std::cout << std::setw(7) << std::setprecision(4) 
                << double(v[1] + v[2] - v[3] - v[0])/(v[0] + v[1] + v[2] + v[3]) << "\n";
      }
    }
  else if(command == "measure_full")
    {
      if(!sys.clockIsEnabledSSP(ADC_SSP))
	{
	  oStr << "SSP clock is not enabled. Power up the board!\n";
	  return EXIT_FAILURE;
	}

#define NCHANMAX 11
      if(argc==0)usage(program, oStr);
      unsigned iadc = atoi(*argv);
      argc--, argv++;
      if((iadc<1)||(iadc>7))usage(program, oStr);
      iadc--;

      if(argc==0)usage(program, oStr);
      unsigned zchan = atoi(*argv);
      argc--, argv++;
      if(zchan>NCHANMAX)usage(program, oStr);
      unsigned nchan = zchan;
      if(zchan==0)nchan=NCHANMAX;
      else zchan--;

      unsigned nmeas = 1;
      if(argc)
	{
	  nmeas = atoi(*argv);
	  argc--, argv++;
	}

      unsigned ndelay = 100;
      if(argc)
	{
	  ndelay = atoi(*argv);
	  argc--, argv++;
	}

      float volt_full = 5.05;
      if(argc)
	{
	  volt_full = atof(*argv);
	  argc--, argv++;
	}

      std::vector< std::vector<uint32_t> > vecChanMeas(NCHANMAX);

      for(unsigned ichan=zchan; ichan<nchan; ichan++)
        mcb.measureADC(iadc, ichan, nmeas, 
	               vecChanMeas[ichan],
	               ndelay);
	
      for(unsigned i = 0; i < nmeas; ++i)
      {
          for(unsigned ichan=zchan; ichan<nchan; ichan++)
          {
              oStr  << std::fixed
                    << std::setw(6) << std::setprecision(4) 
                    << TLC3548::voltData(vecChanMeas[ichan][i], volt_full) << " ";
          }
          oStr << "\n";
      }
    }
//   else if(command == "power_up_psd")
//     {
//       if(argc==0)usage(program, oStr);
//       unsigned ipsd = atoi(*argv);
//       if(ipsd>7)usage(program, oStr);
//       if((ipsd==0)||(ipsd==1))mcb.powerUpPSD(0);
//       if((ipsd==0)||(ipsd==2))mcb.powerUpPSD(1);
//       if((ipsd==0)||(ipsd==3))mcb.powerUpPSD(2);
//       if((ipsd==0)||(ipsd==4))mcb.powerUpPSD(3);
//       if((ipsd==0)||(ipsd==5))mcb.powerUpPSD(4);
//       if((ipsd==0)||(ipsd==6))mcb.powerUpPSD(5);
//       if((ipsd==0)||(ipsd==7))mcb.powerUpPSD(6);
//     }
//   else if(command == "power_down_psd")
//     {
//       if(argc==0)usage(program, oStr);
//       unsigned ipsd = atoi(*argv);
//       if(ipsd>7)usage(program, oStr);
//       if((ipsd==0)||(ipsd==1))mcb.powerDownPSD(0);
//       if((ipsd==0)||(ipsd==2))mcb.powerDownPSD(1);
//       if((ipsd==0)||(ipsd==3))mcb.powerDownPSD(2);
//       if((ipsd==0)||(ipsd==4))mcb.powerDownPSD(3);
//       if((ipsd==0)||(ipsd==5))mcb.powerDownPSD(4);
//       if((ipsd==0)||(ipsd==6))mcb.powerDownPSD(5);
//       if((ipsd==0)||(ipsd==7))mcb.powerDownPSD(6);
//     }
//   else if(command == "led_brightness")
//     {
//       if(argc==0)usage(program, oStr);
//       unsigned iduty = atoi(*argv);
//       if(iduty>10)usage(program, oStr);
//       mcb.setLEDBrightness(iduty);      
//     }
//   else if(command == "led_brightness_full")
//     {
//       if(argc != 2)usage(program, oStr);
//       unsigned iduty = atoi(*argv);
//       unsigned iperiod = atoi(*(++argv));
//       oStr << "Duty: " << iduty << "\nPeriod: " << iperiod << "\n";
//       mcb.setLEDBrightnessFull(iduty, iperiod);      
//     }
  else if(command == "calibrate")
    {
      if(argc==0)usage(program, oStr);

      unsigned idrive = atoi(*argv);
      argc--, argv++;
      if((idrive<1)||(idrive>6))usage(program, oStr);
      idrive--;

      unsigned nstep = 10000;
      if(argc) { nstep = atoi(*argv); argc--, argv++; }

      unsigned ncycle = 0;
      if(argc) { ncycle = atoi(*argv); argc--, argv++; }
      unsigned dir = 0;
      if(ncycle <= 1)dir = ncycle, ncycle = 1;

      unsigned ndelay = 10000;
      if(argc) { ndelay = atoi(*argv); argc--, argv++; }

      unsigned iadc = 7;
      if(argc) { iadc = atoi(*argv); argc--, argv++; }
      if((iadc<1)||(iadc>7))usage(program, oStr);
      iadc--;

      unsigned nmeas = 1;
      if(argc) { nmeas = atoi(*argv); argc--, argv++; }

      unsigned nburn = 1;
      if(argc) { nburn = atoi(*argv); argc--, argv++; }

      unsigned ichan = idrive+1;
      if(argc) { ichan = atoi(*argv); argc--, argv++; }
      if(ichan>8)usage(program, oStr);
      ichan--;

      unsigned ndelay_adc = 100;
      float volt_full = 1.0; // 5.05;

      uint32_t* sum = new uint32_t[nstep*ncycle];
      uint64_t* sum2 = new uint64_t[nstep*ncycle];
      uint32_t* max = new uint32_t[nstep*ncycle];
      uint32_t* min = new uint32_t[nstep*ncycle];

      assert(sum);
      assert(sum2);
      assert(max);
      assert(min);

      // Step and take data
      for(unsigned icycle = 0;icycle<ncycle;icycle++)
	{
	  MirrorControlBoard::Dir dir = MirrorControlBoard::DIR_RETRACT;
	  if((icycle + dir)%2 == 0)dir = MirrorControlBoard::DIR_EXTEND;

	  for(unsigned istep=0;istep<nstep;istep++)
	    {
	      mcb.stepOneDrive(idrive, dir);
	      mcb.loopDelay(ndelay);
	      mcb.measureADCStat(iadc, ichan, nmeas, 
			       sum[ichan], sum2[ichan], min[ichan], max[ichan],
				 nburn, ndelay_adc);
	    }
	}
      
      for(unsigned idatum=0;idatum<nstep*ncycle;idatum++)
	{
	  unsigned icycle = idatum/nstep;
	  unsigned istep = idatum%nstep;
	  if(icycle & 0x1)istep=nstep-istep-1;
	  else istep++;

	  oStr << istep << ' ';
	  if(nmeas>1)
	    {
	      uint32_t mean = sum[idatum]/nmeas;
	      uint64_t s = sum[idatum];
	      uint64_t var = (sum2[idatum] - s*s/nmeas)/nmeas;
	      uint32_t rms = julery_isqrt(uint32_t(var));

	      oStr << mean << ' '
			<< std::fixed << std::setw(6) << std::setprecision(4) 
			<< TLC3548::voltData(mean,volt_full) << ' '
			<< std::setw(6) << std::setprecision(4) 
			<< TLC3548::voltData(rms,volt_full) << ' '
			<< std::setw(6) << std::setprecision(4) 
			<< TLC3548::voltData(max[idatum],volt_full) << ' '
			<< std::setw(6) << std::setprecision(4) 
			<< TLC3548::voltData(min[idatum],volt_full) 
			<< '\n';
	    }
	  else
	    {
	      oStr << sum[idatum] << ' '
			<< std::fixed << std::setw(6) << std::setprecision(4) 
			<< TLC3548::voltData(sum[idatum],volt_full) << '\n';
	    }
	}

      delete[] sum;
      delete[] sum2;
      delete[] min;
      delete[] max;
    }
  else
    usage(program, oStr);
  return EXIT_SUCCESS;
}
