////////////////////////////////////////////////////////////////////////////////
// cbc.cpp Console board control
////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <string>
#include <cassert>
#include <cstring>
#include <vector>
#include <unistd.h>
#include <MirrorControlBoard.hpp>
#include <cbc.hpp>
#define NCHANMAX 11

// fast integer square root
static unsigned julery_isqrt(unsigned long val)
{
    unsigned long temp, g=0, b = 0x8000, bshft = 15;
    do {
        if (val >= (temp = (((g << 1) + b)<<bshft--))) {
            g += b;
            val -= temp;
        }
    } while (b >>= 1);
    return g;
}

const char* usage_text =
    "initialize             CLK_DIV=4: Initialize the hardware. Should be done once after boot-up."
    "\n"
    "\npower_down             Go into power saving mode. Power down encoders, USB and A3977."
    "\npower_up               Power up all on-board and off-board electronics."
    "\n"
    "\npower_down_encoders    Power down encoders."
    "\npower_up_encoders      Power up encoders."
    "\n"
    "\npower_down_a3977       Power down A3977 motor controllers."
    "\npower_up_a3977         Power up A3977 motor controllers."
    "\n"
    "\nreset                  Send reset command to drive electronics."
    "\n"
    "\nenable_sr              Enable Synchronous Rectification (SR) mode."
    "\ndisable_sr             Disable Synchronous Rectification (SR) mode."
    "\n"
    "\nset_microstep          MS={1,2,4,8}."
    "\n                       Set number of micro steps to 1, 2, 4 or 8."
    "\n"
    "\nenable_hi_current      Enable high current mode on all drives."
    "\ndisable_hi_current     Disable high current mode on all drives."
    "\n"
    "\nenable                 DR={1-6} or DR=all"
    "\n                       Enable motor driver."
    "\ndisable                DR={1-6} or DR=all"
    "\n                       Disable motor driver"
    "\n"
    "\nenableusb              USB={1-7} or USB=all:"
    "\n                       Enable USB (USB)."
    "\ndisableusb             USB={1-7} or USB=all:"
    "\n                       Disable USB (USB)."
    "\n"
    "\nstep                   DR={1-6} NSTEP [DELAY]:"
    "\n                       Step drive some number of steps (positive to"
    "\n                       extend, negative to retract) with delay between"
    "\n                       steps given by DELAY (default 5000)."
    "\n"
    "\nslew                   DR={1-6} [DIR={extend,retract}] [DELAY]:"
    "\n                       Slew drive (DR) in given direction (DIR, default extend) "
    "\n                       with delay between steps given by DELAY (default 5000)."
    "\n"
    "\nstep_all               DR1_NSTEP DR2_NSTEP DR3_NSTEP DR4_NSTEP DR5_NSTEP DR6_NSTEP [DELAY]"
    "\n                       Step all drives some number of steps (positive to extend,"
    "\n                       negative to retract and zero to not move that drive.)"
    "\n"
    "\nslew_all               [DIR={extend,retract}] [DELAY]:"
    "\n                       Slew all (enabled) drives in given direction (DIR, default "
    "\n                       extend) with delay between steps given by DELAY (default 5000)."
    "\n"
    "\nstatus:                Print drive status information"
    "\n"
    "\nmeasure                ADC={1-8} CHAN={0-11} [MEAS=1 BURN=0 DELAY=100 SCALE=5.05]:"
    "\n                       Measure voltage of the specified ADC channel. Channel can be"
    "\n                       given as zero to specify all channels on one ADC. Channels 9, 10"
    "\n                       and 11 correspond to internal reference voltages on the ADC."
    "\n                       Prints out statistics."
    "\n"
    "\nmeasure_full           ADC={1-8} CHAN={0-11} [MEAS=1 DELAY=100 SCALE=5.05]:"
    "\n                       Measure voltage of the specified ADC channel. Channel can be"
    "\n                       given as zero to specify all channels on one ADC. Channels 9, 10"
    "\n                       and 11 correspond to internal reference voltages on the ADC."
    "\n                       Prints out raw data."
    "\n"
    "\ncalibrate              DR={1-6} [NSTEP=10000] [NCYCLE=0] [DELAY=10000] [ADC=7] [NMEAS=1]"
    "\n                       Step drive and read ADC after each step, printing its value to"
    "\n                       the terminal. If NCYCLE is 0 or 1 then it specifies the direction"
    "\n                       of the travel, otherwise it specifies the number of half cycles"
    "\n                       of expansion and contraction to perform."
    "\n";

int usage(const char* program, std::ostream& stream)
{
    stream << "Usage: " << program << " command [command data]\n\n" << usage_text;
    exit(EXIT_SUCCESS);
}

int main(int argc, const char** argv)
{
    return SubMain(argc, argv);
}

int SubMain(int argc, const char** argv, std::ostream& oStr)
{
    const char* program = *argv;
    argv++, argc--;

    if(argc == 0)
        return usage(program, oStr);

    Overo sys;
    Layout layout;
    MirrorControlBoard mcb(true, 7);

    std::string command = *argv;
    argv++, argc--;

    if((command == "initialize") || (command == "init") || (command == "config")) {

        printf("\nConfiguring GPIOs...");
        sys.gpioConfigureAll();

        printf("\nPowering Up Base...");
        mcb.powerUpBase();

        return EXIT_SUCCESS;
    }

    if (command == "power_down")
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
    else if(command == "set_microstep") {
        if(argc==0)
            usage(program, oStr);
        unsigned usint = atoi(*argv);
        MirrorControlBoard::UStep us=MirrorControlBoard::USTEP_1;
        switch(usint) {
        case 1:
            us = MirrorControlBoard::USTEP_1;
            break;
        case 2:
            us = MirrorControlBoard::USTEP_2;
            break;
        case 4:
            us = MirrorControlBoard::USTEP_4;
            break;
        case 8:
            us = MirrorControlBoard::USTEP_8;
            break;
        default:
            usage(program, oStr);
        }
        mcb.setUStep(us);
    } else if(command == "enable_hi_current")
        mcb.enableDriveHiCurrent();
    else if(command == "disable_hi_current")
        mcb.disableDriveHiCurrent();
    else if(command == "enable") {
        // whine if no argument given
        if(argc==0)
            usage(program, oStr);

        if(strcmp(*argv,"all")==0)
            mcb.enableAllDrives();
        else {
            // drive number
            unsigned idrive = atoi(*argv);

            // whine if drive is out of bounds
            if ((idrive<1)||(idrive>6))
                usage(program, oStr);

            //count from zero
            idrive--;

            //enable drive
            mcb.enableDrive(idrive);
        }
    } // close enable (idrive)
    else if (command == "disable") {
        // whine if no argument given
        if(argc==0)
            usage(program, oStr);

        if(strcmp(*argv,"all")==0)
            mcb.enableAllDrives();
        else {
            // drive number
            unsigned idrive = atoi(*argv);

            // whine if drive is out of bounds
            if ((idrive<1)||(idrive>6))
                usage(program, oStr);

            //count from zero
            idrive--;

            //enable drive
            mcb.disableDrive(idrive);
        }
    } // close disable (idrive)
    else if(command == "enableUSB" || command == "enableusb") {
        // whine if no argument given
        if(argc==0)
            usage(program, oStr);

        if (strcmp(*argv,"all")==0)
            mcb.powerUpAllUSB();
        else {
            // usb number
            unsigned iusb = atoi(*argv);

            // whine if usb is out of bounds
            if((iusb<1)||(iusb>7))
                usage(program, oStr);

            //count from zero
            iusb--;

            mcb.powerUpUSB(iusb);
        }
    } // close enableUSB
    else if(command == "disableUSB" || command == "disableusb") {
        // whine if no argument given
        if(argc==0)
            usage(program, oStr);

        if(strcmp(*argv,"all")==0)
            mcb.powerDownAllUSB();
        else {
            // usb number
            unsigned iusb = atoi(*argv);

            // whine if usb is out of bounds
            if((iusb<1)||(iusb>7))
                usage(program, oStr);

            //count from zero
            iusb--;

            mcb.powerDownUSB(iusb);
        }
    } // close disableUSB
    else if(command == "testusb") {
        for (int i=0; i<1000000; i++) {
            if (i%2==0)
                sys.gpioWriteLevel(layout.igpioUSBOff4(),1);
            if (i%2==1)
                sys.gpioWriteLevel(layout.igpioUSBOff4(),0);
        }
    } // close testusb
    else if(command == "frequsb") {
        if (argc==0)
            usage(program,oStr);

        //50 is an empirically derived offset used to allow actual (Hz) frequency inputs which accomodates for the tmie to process through the inherent inaccuracy of the clock
        unsigned frequency = atoi(*argv);
        float period = (1000000/(frequency));

        for (int i=0; i<1000000; i++) {
            sys.gpioWriteLevel(layout.igpioUSBOff4(),1);
            usleep(period/2);
            sys.gpioWriteLevel(layout.igpioUSBOff4(),0);
            usleep(period/2);
        }
    } // close frequsb
    else if(command == "step") {
        // whine if no argument given
        if(argc==0)
            usage(program, oStr);
        // actuator number
        unsigned idrive = atoi(*argv);

        // remove the 1st argument from arglist
        argc--, argv++;

        // whine if invalid actuator number
        if ((idrive<1)||(idrive>6))
            usage(program, oStr);

        //count from 0
        idrive--;

        // whine if no second argument
        if(argc==0)
            usage(program, oStr);

        // number of setps
        int nstep = atoi(*argv);

        // remove the 2nd argument from arglist
        argc--, argv++;

        // if nstep > 0, extend. If nstep < 0, retract
        MirrorControlBoard::Dir dir = MirrorControlBoard::DIR_EXTEND;
        if(nstep<0)
            dir = MirrorControlBoard::DIR_RETRACT, nstep=-nstep;

        // default delay=5000
        unsigned ndelay = 5000;

        // optional argument #3 for delay value
        if(argc) {
            ndelay = atoi(*argv);
            argc--, argv++;
        }

        // loop over number of steps
        for(unsigned istep=0; istep<unsigned(nstep); istep++) {
            mcb.stepOneDrive(idrive, dir, ndelay);
            mcb.loopDelay(ndelay);
        }
    } // close step
    else if(command == "slew") {
        // whine if no arugment given
        if(argc==0)
            usage(program, oStr);

        // drive number
        unsigned idrive = atoi(*argv);

        // remove first argument
        argc--, argv++;

        // whine if invalid drivenumber
        if ((idrive<1)||(idrive>6))
            usage(program, oStr);

        // count from zero
        idrive--;

        // Default direction = extend
        MirrorControlBoard::Dir dir = MirrorControlBoard::DIR_EXTEND;

        // Check the string length to see if somebody typed extend or retract....

        if(argc) {
            std::string dirstr(*argv);

            if((dirstr.size()<=6) &&(dirstr.compare(0,dirstr.size(), std::string("extend"),0,dirstr.size())==0))
                dir = MirrorControlBoard::DIR_EXTEND;
            else if((dirstr.size()<=7) &&(dirstr.compare(0,dirstr.size(), std::string("retract"),0,dirstr.size())==0))
                dir = MirrorControlBoard::DIR_RETRACT;
            else
                usage(program, oStr);
            argc--, argv++;
        }

        // default delay = 5000 steps
        unsigned ndelay = 5000;

        // take optional argument for delay
        if(argc) {
            ndelay = atoi(*argv);
            argc--, argv++;
        }

        // Loop forever while stepping the motor
        while(1) {
            mcb.stepOneDrive(idrive, dir, ndelay);
            mcb.loopDelay(ndelay);
        }
    } // close slew

    else if (command == "step_all") {
        int nstep1 = 0;
        int nstep2 = 0;
        int nstep3 = 0;
        int nstep4 = 0;
        int nstep5 = 0;
        int nstep6 = 0;

        // take in (optional) number of steps as arugment
        if(argc)
            nstep1 = atoi(*argv), argc--, argv++;
        if(argc)
            nstep2 = atoi(*argv), argc--, argv++;
        if(argc)
            nstep3 = atoi(*argv), argc--, argv++;
        if(argc)
            nstep4 = atoi(*argv), argc--, argv++;
        if(argc)
            nstep5 = atoi(*argv), argc--, argv++;
        if(argc)
            nstep6 = atoi(*argv), argc--, argv++;

        // default delay = 5000
        unsigned ndelay = 5000;
        // otherwise choose a delay via argument
        if(argc) {
            ndelay = atoi(*argv);
            argc--, argv++;
        }

        // loop while theres still some stepping to do..
        while(nstep1!=0 || nstep2!=0 || nstep3!=0 || nstep4!=0 || nstep5!=0 || nstep6!=0) {
            //default dir is none
            MirrorControlBoard::Dir dir1 = MirrorControlBoard::DIR_NONE;
            MirrorControlBoard::Dir dir2 = MirrorControlBoard::DIR_NONE;
            MirrorControlBoard::Dir dir3 = MirrorControlBoard::DIR_NONE;
            MirrorControlBoard::Dir dir4 = MirrorControlBoard::DIR_NONE;
            MirrorControlBoard::Dir dir5 = MirrorControlBoard::DIR_NONE;
            MirrorControlBoard::Dir dir6 = MirrorControlBoard::DIR_NONE;

            // motor 1 direction
            if (nstep1 > 0)
                dir1 = MirrorControlBoard::DIR_EXTEND, nstep1--;
            else if (nstep1 < 0)
                dir1 = MirrorControlBoard::DIR_RETRACT, nstep1++;

            // motor 2 direction
            if (nstep2 > 0)
                dir2 = MirrorControlBoard::DIR_EXTEND, nstep2--;
            else if (nstep2 < 0)
                dir2 = MirrorControlBoard::DIR_RETRACT, nstep2++;

            // motor 3 direction
            if (nstep3 > 0)
                dir3 = MirrorControlBoard::DIR_EXTEND, nstep3--;
            else if (nstep3 < 0)
                dir3 = MirrorControlBoard::DIR_RETRACT, nstep3++;

            // motor 4 direction
            if (nstep4 > 0)
                dir4 = MirrorControlBoard::DIR_EXTEND, nstep4--;
            else if (nstep4 < 0)
                dir4 = MirrorControlBoard::DIR_RETRACT, nstep4++;

            // motor 5 direction
            if (nstep5 > 0)
                dir5 = MirrorControlBoard::DIR_EXTEND, nstep5--;
            else if (nstep5 < 0)
                dir5 = MirrorControlBoard::DIR_RETRACT, nstep5++;

            // motor 6 direction
            if (nstep6 > 0)
                dir6 = MirrorControlBoard::DIR_EXTEND, nstep6--;
            else if (nstep6 < 0)
                dir6 = MirrorControlBoard::DIR_RETRACT, nstep6++;

            // step all drives once in specified directions
            mcb.stepAllDrives(dir1, dir2, dir3, dir4, dir5, dir6, ndelay);
            // some delay
            mcb.loopDelay(ndelay);
        }
    } // close step_all
    else if(command == "slew_all") {
        MirrorControlBoard::Dir dir = MirrorControlBoard::DIR_EXTEND;
        if(argc) {
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
        if(argc) {
            ndelay = atoi(*argv);
            argc--, argv++;
        }

        while(1) {
            mcb.stepAllDrives(dir, dir, dir, dir, dir, dir);
            mcb.loopDelay(ndelay);
        }
    } // close slew_all
    else if(command == "status") {
        oStr << "Drives:"
             << (mcb.isDriveControllersPoweredUp()?
                 (char*)"":(char*)" A3977-OFF")
             << (sys.gpioReadLevel(layout.igpioReset())?
                 (char*)"":(char*)" RESET")
             << (mcb.isDriveSREnabled()?
                 (char*)" SR":(char*)"");
        switch(mcb.getUStep()) {
        case MirrorControlBoard::USTEP_8:
            oStr << " 8-uSTEP";
            break;
        case MirrorControlBoard::USTEP_4:
            oStr << " 4-uSTEP";
            break;
        case MirrorControlBoard::USTEP_2:
            oStr << " 2-uSTEP";
            break;
        case MirrorControlBoard::USTEP_1:
            oStr << " 1-uSTEP";
            break;
        }
        oStr << (mcb.isDriveHiCurrentEnabled()?  (char*)" HI-CURRENT":(char*)"") << (mcb.isEncodersPoweredUp()?  (char*)" ENCODERS-ON":(char*)"ENCODERS-OFF") << '\n';

        for(unsigned idrive=0; idrive<6; idrive++) {
            oStr << "  " << idrive+1 << ":" << (mcb.isDriveEnabled(idrive)?  " ENABLED ":" DISABLED") << '\n';
        }
    } // close status
    else if(command == "measure") {

        if(argc<2)
            usage(program, oStr);
        unsigned iadc = atoi(*argv);
        argc--, argv++;

        if ((iadc<1)||(iadc>8))
            usage(program, oStr);
        iadc--;

        if(argc==0)
            usage(program, oStr);
        unsigned zchan = atoi(*argv);
        argc--, argv++;

        if (zchan>NCHANMAX)
            usage(program, oStr);
        unsigned nchan = zchan;

        if (zchan==0)
            nchan=NCHANMAX;
        else
            zchan--;

        unsigned nmeas = 1;
        if(argc) {
            nmeas = atoi(*argv);
            argc--, argv++;
        }

        unsigned nburn = 0;
        if(argc) {
            nburn = atoi(*argv);
            argc--, argv++;
        }

        unsigned ndelay = 100;
        if(argc) {
            ndelay = atoi(*argv);
            argc--, argv++;
        }

        float volt_full = 5.05;
        if(argc) {
            volt_full = atof(*argv);
            argc--, argv++;
        }

        int hex_out = 0;
        if(argc) {
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

        for (unsigned ichan=zchan; ichan<nchan; ichan++)
            mcb.measureADCStat(iadc, ichan, nmeas, sum[ichan], sum2[ichan], min[ichan], max[ichan], nburn, ndelay);

        for(unsigned ichan=zchan; ichan<nchan; ichan++) {
            uint32_t mean = sum[ichan]/nmeas;
            uint64_t s = sum[ichan];
            uint64_t var = (sum2[ichan] - s*s/nmeas)/nmeas;
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
        } // close for ichan

        if(nchan != zchan + 1) {
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
        } // close if nchan!=zchan+1
    } // close measure
    else if(command == "measure_full") {
        //if(!sys.clockIsEnabledSSP(ADC_SSP))
        //{
        //    oStr << "SSP clock is not enabled. Power up the board!\n";
        //    return EXIT_FAILURE;
        //}

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
        if(argc) {
            nmeas = atoi(*argv);
            argc--, argv++;
        }

        unsigned ndelay = 100;
        if(argc) {
            ndelay = atoi(*argv);
            argc--, argv++;
        }

        float volt_full = 5.05;
        if(argc) {
            volt_full = atof(*argv);
            argc--, argv++;
        }

        std::vector< std::vector<uint32_t> > vecChanMeas(NCHANMAX);

        for(unsigned ichan=zchan; ichan<nchan; ichan++)
            mcb.measureADC(iadc, ichan, nmeas,
                           vecChanMeas[ichan],
                           ndelay);

        for(unsigned i = 0; i < nmeas; ++i) {
            for(unsigned ichan=zchan; ichan<nchan; ichan++) {
                oStr  << std::fixed
                      << std::setw(6) << std::setprecision(4)
                      << TLC3548::voltData(vecChanMeas[ichan][i], volt_full) << " ";
            }
            oStr << "\n";
        }
    } // close measure_full
    else if (command == "calibrate") {
        // whine if no argument
        if(argc==0)
            usage(program, oStr);

        // choose drive
        unsigned idrive = atoi(*argv);
        argc--, argv++;

        // whine if drive is invalid
        if((idrive<1)||(idrive>6))
            usage(program, oStr);
        idrive--;

        // default number of steps = 0
        unsigned nstep = 10000;
        // (possibly) not default number of steps
        if(argc) {
            nstep = atoi(*argv);
            argc--, argv++;
        }

        // default number of cycles = 0
        unsigned ncycle = 0;
        // otherwise... take argument
        if(argc) {
            ncycle = atoi(*argv);
            argc--, argv++;
        }

        //unsigned dir = 0;
        //if(ncycle <= 1)
        //    dir = ncycle, ncycle = 1;

        // default delay = 10000
        unsigned ndelay = 10000;
        // otherwise... take argument
        if(argc) {
            ndelay = atoi(*argv);
            argc--, argv++;
        }

        // default ADC=7
        unsigned iadc = 7;
        // otherwise... take argument
        if(argc) {
            iadc = atoi(*argv);
            argc--, argv++;
        }

        // whine if adc is invalid
        if ((iadc<1)||(iadc>7))
            usage(program, oStr);
        iadc--;

        // default nmeas=1
        unsigned nmeas = 1;
        // argument
        if(argc) {
            nmeas = atoi(*argv);
            argc--, argv++;
        }

        // default nburn=1
        unsigned nburn = 1;
        // otherwise.. take argument
        if(argc) {
            nburn = atoi(*argv);
            argc--, argv++;
        }

        // get channel number
        unsigned ichan = idrive+1;
        if(argc) {
            ichan = atoi(*argv);
            argc--, argv++;
        }

        // whine if invalid channel chosen
        if(ichan>8)
            usage(program, oStr);

        // count from 0
        ichan--;

        unsigned ndelay_adc = 100;
        float volt_full = 1.0; // 5.05;

        uint32_t* sum   = new uint32_t[nstep*ncycle];
        uint64_t* sum2  = new uint64_t[nstep*ncycle];
        uint32_t* max   = new uint32_t[nstep*ncycle];
        uint32_t* min   = new uint32_t[nstep*ncycle];

        assert(sum);
        assert(sum2);
        assert(max);
        assert(min);

        // Step and take data
        for (unsigned icycle = 0; icycle<ncycle; icycle++) {
            // Default Direction is Retract
            MirrorControlBoard::Dir dir  = MirrorControlBoard::DIR_RETRACT;

            // Extend on even icycles
            if ((icycle + dir)%2 == 0)
                dir = MirrorControlBoard::DIR_EXTEND;

            for (unsigned istep=0; istep<nstep; istep++) {
                // Move motor one step
                mcb.stepOneDrive(idrive, dir);
                // Wait a moment
                mcb.loopDelay(ndelay);
                // Measure on ADC
                mcb.measureADCStat(iadc, ichan, nmeas, sum[ichan], sum2[ichan], min[ichan], max[ichan], nburn, ndelay_adc);
            } // close (for istep)
        } // close (for icycle)

        for (unsigned idatum=0; idatum<nstep*ncycle; idatum++) {
            unsigned icycle = idatum / nstep;
            unsigned istep = idatum%nstep;
            if (icycle & 0x1)
                istep=nstep-istep-1;
            else
                istep++;

            oStr << istep << ' ';

            if(nmeas>1) {
                uint32_t mean   = sum[idatum]/nmeas;
                uint64_t s      = sum[idatum];
                uint64_t var    = (sum2[idatum] - s*s/nmeas)/nmeas;
                uint32_t rms    = julery_isqrt(uint32_t(var));

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
            } // close if(nmeas>1)
            else {
                oStr << sum[idatum] << ' '
                     << std::fixed << std::setw(6) << std::setprecision(4)
                     << TLC3548::voltData(sum[idatum],volt_full) << '\n';
            }
        } // close (for idatum)

        delete[] sum;   //delete pointers
        delete[] sum2;  //delete pointers
        delete[] min;   //delete pointers
        delete[] max;   //delete pointers
    } // close calibrate
    else
        usage(program, oStr); //print program usage

    return EXIT_SUCCESS;
}

