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
#include <cbc.hpp>
#define NCHANMAX 11

int main(int argc, const char** argv)
{
    argv++, argc--;

    cbc cbc;
    MirrorControlBoard mcb;


    if(argc == 0)
        cbc.usage();

    std::string command = *argv;
    argv++, argc--;

    if (command ==  "init")
        cbc.initialize();
    else if (command == "power_down")
        mcb.powerDownAll();
    else if (command == "power_up")
        mcb.powerUpAll();
    else if (command == "power_down_encoders")
        mcb.powerDownEncoders();
    else if (command == "power_up_encoders")
        mcb.powerUpEncoders();
    else if (command == "power_down_a3977")
        mcb.powerDownDriveControllers();
    else if (command == "power_up_a3977")
        mcb.powerUpDriveControllers();
    else if (command == "reset")
        mcb.setPhaseZeroOnAllDrives();
    else if (command == "enable_sr")
        mcb.enableDriveSR();
    else if (command == "disable_sr")
        mcb.disableDriveSR();
    else if (command == "set_microstep")
    {
        if(argc==0)
            cbc.usage();
        unsigned usint = atoi(*argv);

        cbc.set_microstep(usint);
    }
    else if (command == "enable_hi_current")
        mcb.enableDriveHiCurrent();
    else if (command == "disable_hi_current")
        mcb.disableDriveHiCurrent();
    else if (command == "enable")
    {
        if(argc==0)
            cbc.usage();

        unsigned idrive;
        if (strcmp(*argv,"all")==0)
            idrive=0;
        else
            idrive = atoi(*argv);

        cbc.enable(idrive);
    }
    else if (command == "disable")
    {
        if(argc==0)
            cbc.usage();

        unsigned idrive;
        if (strcmp(*argv,"all")==0)
            idrive=0;
        else
            idrive = atoi(*argv);

        cbc.disable(idrive);
    }
    else if (command == "enableusb")
    {
        if(argc==0)
            cbc.usage();

        unsigned iusb;
        if (strcmp(*argv,"all")==0)
            iusb=0;
        else
            iusb = atoi(*argv);

        cbc.enableusb(iusb);
    }
    else if (command == "disableusb")
    {
        if(argc==0)
            cbc.usage();

        unsigned iusb;
        if (strcmp(*argv,"all")==0)
            iusb=0;
        else
            iusb = atoi(*argv);

        cbc.disableusb(iusb);
    }
    else if (command == "testusb")
        cbc.testusb();

    else if (command == "frequsb")
    {
        if (argc==0)
            cbc.usage();
        unsigned frequency = atoi(*argv);
        cbc.frequsb(frequency);
    }
    else if (command == "step")
    {
        if(argc<2)
            cbc.usage();

        // actuator number
        unsigned idrive = atoi(*argv);
        argc--, argv++;

        // number of setps
        int nstep = atoi(*argv);
        argc--, argv++;

        // delay
        unsigned ndelay = 5000;
        if(argc)
        {
            ndelay = atoi(*argv);
            argc--, argv++;
        }
        cbc.step(idrive, nstep, ndelay);
    }
    else if (command == "slew")
    {
        // whine if no arugment given
        if(argc==0)
            cbc.usage();

        // drive number
        unsigned idrive = atoi(*argv);
        argc--, argv++;

        // Default direction = extend
        MirrorControlBoard::Dir dir = MirrorControlBoard::DIR_EXTEND;

        // Check the string length to see if somebody typed extend or retract....
        if(argc)
        {
            std::string dirstr(*argv);

            if((dirstr.size()<=6) &&(dirstr.compare(0,dirstr.size(), std::string("extend"),0,dirstr.size())==0))
                dir = MirrorControlBoard::DIR_EXTEND;
            else if((dirstr.size()<=7) &&(dirstr.compare(0,dirstr.size(), std::string("retract"),0,dirstr.size())==0))
                dir = MirrorControlBoard::DIR_RETRACT;
            else
                cbc.usage();
            argc--, argv++;
        }

        // default delay = 5000 steps
        unsigned ndelay = 5000;

        // take optional argument for delay
        if(argc)
        {
            ndelay = atoi(*argv);
            argc--, argv++;
        }

        cbc.slew(idrive, dir, ndelay);
    }
    else if (command == "step_all")
    {
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
        if(argc)
        {
            ndelay = atoi(*argv);
            argc--, argv++;
        }
        cbc.step_all(nstep1, nstep2, nstep3, nstep4, nstep5, nstep6, ndelay);
    }
    else if (command == "slew_all")
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
                cbc.usage();
            argc--, argv++;
        }

        unsigned ndelay = 5000;
        if(argc)
        {
            ndelay = atoi(*argv);
            argc--, argv++;
        }

        cbc.slew_all(dir, ndelay);
    }
    else if (command == "status")
        cbc.status();
    else if (command == "measure")
    {
        if(argc<2)
            cbc.usage();
        unsigned iadc = atoi(*argv);
        argc--, argv++;

        if(argc==0)
            cbc.usage();
        unsigned zchan = atoi(*argv);
        argc--, argv++;

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

        cbc.measure(iadc, zchan, nmeas, nburn, ndelay, volt_full, hex_out);
    }
    else if (command == "measure_full")
    {
        if(argc==0)
            cbc.usage();
        unsigned iadc = atoi(*argv);
        argc--, argv++;


        if(argc==0)
            cbc.usage();
        unsigned zchan = atoi(*argv);
        argc--, argv++;

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
        cbc.measure_full(iadc, zchan, nmeas, ndelay, volt_full);
    }
    else if (command == "calibrate")
    {
        if(argc==0)
            cbc.usage();

        // choose drive
        unsigned idrive = atoi(*argv);
        argc--, argv++;

        // default number of steps = 0
        unsigned nstep = 10000;
        // (possibly) not default number of steps
        if(argc)
        {
            nstep = atoi(*argv);
            argc--, argv++;
        }

        // default number of cycles = 0
        unsigned ncycle = 0;
        // otherwise... take argument
        if(argc)
        {
            ncycle = atoi(*argv);
            argc--, argv++;
        }

        //unsigned dir = 0;
        //if(ncycle <= 1)
        //    dir = ncycle, ncycle = 1;

        // default delay = 10000
        unsigned ndelay = 10000;
        // otherwise... take argument
        if(argc)
        {
            ndelay = atoi(*argv);
            argc--, argv++;
        }

        // default ADC=7
        unsigned iadc = 7;
        // otherwise... take argument
        if(argc)
        {
            iadc = atoi(*argv);
            argc--, argv++;
        }

        // default nmeas=1
        unsigned nmeas = 1;
        // argument
        if(argc)
        {
            nmeas = atoi(*argv);
            argc--, argv++;
        }

        // default nburn=1
        unsigned nburn = 1;
        // otherwise.. take argument
        if(argc)
        {
            nburn = atoi(*argv);
            argc--, argv++;
        }

        // get channel number
        unsigned ichan = idrive+1;
        if(argc)
        {
            ichan = atoi(*argv);
            argc--, argv++;
        }
        cbc.calibrate(idrive, nstep, ncycle, ndelay, iadc, nmeas, nburn, ichan);
    }
    else
        cbc.usage();
}

int cbc::initialize()
{
    printf("Configuring GPIOs...\n");
    sys.gpioConfigureAll();

    printf("Powering Up Base...\n");
    mcb.powerUpBase();

    return EXIT_SUCCESS;
}

void cbc::set_microstep(unsigned usint)
{
    MirrorControlBoard::UStep us=MirrorControlBoard::USTEP_1;
    switch(usint)
    {
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
            usage();
    }
    mcb.setUStep(us);
}

void cbc::enable(int idrive)
{
    // whine if drive is out of bounds
    if ((idrive<0)||(idrive>6))
        usage();

    if (idrive==0)
        mcb.enableAllDrives();

    else
    {
        //count from zero
        idrive--;

        //enable drive
        mcb.enableDrive(idrive);
    }
}

void cbc::disable(int idrive)
{
    // whine if drive is out of bounds
    if ((idrive<0)||(idrive>6))
        usage();

    if (idrive==0)
        mcb.disableAllDrives();

    else
    {
        //count from zero
        idrive--;

        //disable drive
        mcb.disableDrive(idrive);
    }
}

void cbc::enableusb(int iusb)
{
    // whine if usb is out of bounds
    if((iusb<0)||(iusb>7))
        usage();

    if(iusb==0)
        mcb.powerUpAllUSB();
    else
    {
        //count from zero
        iusb--;

        mcb.powerUpUSB(iusb);
    }
}

void cbc::disableusb(int iusb)
{
    // whine if usb is out of bounds
    if((iusb<0)||(iusb>7))
        usage();

    if(iusb==0)
        mcb.powerDownAllUSB();
    else
    {
        //count from zero
        iusb--;

        mcb.powerDownUSB(iusb);
    }
}

void cbc::testusb()
{
    for (int i=0; i<1000000; i++)
    {
        if (i%2==0)
            sys.gpioWriteLevel(layout.igpioUSBOff4,1);
        if (i%2==1)
            sys.gpioWriteLevel(layout.igpioUSBOff4,0);
    }
}

void cbc::frequsb(unsigned frequency)
{
    float period = (1000000/(frequency));

    for (int i=0; i<1000000; i++)
    {
        sys.gpioWriteLevel(layout.igpioUSBOff4,1);
        usleep(period/2);
        sys.gpioWriteLevel(layout.igpioUSBOff4,0);
        usleep(period/2);
    }
}

void cbc::step(unsigned idrive, int nstep, unsigned ndelay)
{
    // whine if invalid actuator number
    if ((idrive<1)||(idrive>6))
        usage();

    //count from 0
    idrive--;

    // if nstep > 0, extend. If nstep < 0, retract
    MirrorControlBoard::Dir dir = MirrorControlBoard::DIR_EXTEND;
    if (nstep<0)
        dir = MirrorControlBoard::DIR_RETRACT, nstep=-nstep;


    // loop over number of steps
    for (unsigned istep=0; istep<unsigned(nstep); istep++)
    {
        mcb.stepOneDrive(idrive, dir, ndelay);
        mcb.loopDelay(ndelay);
    }
}

void cbc::slew(unsigned idrive, MirrorControlBoard::Dir dir, unsigned ndelay)
{
    // whine if invalid drivenumber
    if ((idrive<1)||(idrive>6))
        usage();

    // count from zero
    idrive--;

    // Loop forever while stepping the motor
    while(1)
    {
        mcb.stepOneDrive(idrive, dir, ndelay);
        mcb.loopDelay(ndelay);
    }
}

void cbc::step_all(int nstep1, int nstep2, int nstep3, int nstep4, int nstep5, int nstep6, unsigned ndelay)
{
    // loop while theres still some stepping to do..
    while(nstep1!=0 || nstep2!=0 || nstep3!=0 || nstep4!=0 || nstep5!=0 || nstep6!=0)
    {
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
}

void cbc::slew_all(MirrorControlBoard::Dir dir, unsigned ndelay)
{
    while(1)
    {
        mcb.stepAllDrives(dir, dir, dir, dir, dir, dir);
        mcb.loopDelay(ndelay);
    }
}

void cbc::status()
{
    std::cout << "Drives:"
        << (mcb.isDriveControllersPoweredUp()?
                (char*)"":(char*)" A3977-OFF")
        << (sys.gpioReadLevel(layout.igpioReset)?
                (char*)"":(char*)" RESET")
        << (mcb.isDriveSREnabled()?
                (char*)" SR":(char*)"");

    switch(mcb.getUStep())
    {
        case MirrorControlBoard::USTEP_8:
            std::cout << " 8-uSTEP";
            break;
        case MirrorControlBoard::USTEP_4:
            std::cout << " 4-uSTEP";
            break;
        case MirrorControlBoard::USTEP_2:
            std::cout << " 2-uSTEP";
            break;
        case MirrorControlBoard::USTEP_1:
            std::cout << " 1-uSTEP";
            break;
    }
    std::cout << (mcb.isDriveHiCurrentEnabled()?  (char*)" HI-CURRENT":(char*)"") << (mcb.isEncodersPoweredUp()?  (char*)" ENCODERS-ON":(char*)"ENCODERS-OFF") << '\n';

    for(unsigned idrive=0; idrive<6; idrive++)
    {
        std::cout << "  " << idrive+1 << ":" << (mcb.isDriveEnabled(idrive)?  " ENABLED ":" DISABLED") << '\n';
    }
}

void cbc::measure(unsigned iadc, unsigned zchan, unsigned nmeas, unsigned nburn, unsigned ndelay, float volt_full, int hex_out)
{
    if ((iadc<1)||(iadc>8))
        usage();
    iadc--;

    if (zchan>NCHANMAX)
        usage();
    unsigned nchan = zchan;

    if (zchan==0)
        nchan=NCHANMAX;
    else
        zchan--;

    printf("ADC:        %i\n",          iadc+1);
    printf("Channels:   %i to %i\n",    zchan+1,nchan);
    printf("Nburn:      %i\n",          nburn); 
    printf("NDelay:     %i\n",          ndelay);
    printf("FullVolt    %f\n\n",        volt_full);

    uint32_t sum [NCHANMAX];
    uint64_t sum2[NCHANMAX];
    uint32_t max [NCHANMAX];
    uint32_t min [NCHANMAX];

    for (unsigned ichan=zchan; ichan<nchan; ichan++)
        mcb.measureADCStat(iadc, ichan, nmeas, sum[ichan], sum2[ichan], min[ichan], max[ichan], nburn, ndelay);

    for(unsigned ichan=zchan; ichan<nchan; ichan++)
    {
        uint32_t mean = sum[ichan]/nmeas;
        uint64_t s = sum[ichan];
        uint64_t var = (sum2[ichan] - s*s/nmeas)/nmeas;
        uint32_t rms = julery_isqrt(uint32_t(var));

        if(hex_out != 0)
        {
            printf("i  mean     rms      max      min     \n");
            printf("%02i 0x%06X 0x%06X 0x%06X 0x%06X\n", ichan+1, mean, rms, max[ichan], min[ichan]);
        }
        else
        {
            float adcmean = adc.voltData(mean,      volt_full);
            float adcrms  = adc.voltData(mean,      volt_full);
            float adcmax  = adc.voltData(max[ichan],volt_full);
            float adcmin  = adc.voltData(min[ichan],volt_full);

            printf("i mean   rms    max    min     \n");
            printf("%i %06.04f %06.04f %06.04f %06.04f\n",ichan+1,adcmean,adcrms,adcmax,adcmin);
        }
    } // close for ichan

    if(nchan != zchan + 1)
    {
        int v[4];
        v[0] = sum[0]/nmeas;
        v[1] = sum[1]/nmeas;
        v[2] = sum[2]/nmeas;
        v[3] = sum[3]/nmeas;

        printf("POS %07.04f ", double(v[0] + v[1] - v[2] - v[3])/(v[0] + v[1] + v[2] + v[3]));
        printf("%07.04f\n",    double(v[1] + v[2] - v[3] - v[0])/(v[0] + v[1] + v[2] + v[3]));
        printf("POS %07.04f ", double(v[0] + v[2] - v[1] - v[3])/(v[0] + v[1] + v[2] + v[3]));
        printf("%07.04f\n",    double(v[1] + v[2] - v[3] - v[0])/(v[0] + v[1] + v[2] + v[3]));
    }
}

void cbc::measure_full(unsigned iadc, unsigned zchan, unsigned nmeas, unsigned ndelay, unsigned volt_full)
{
    if((iadc<1)||(iadc>7))
        usage();
    iadc--;

    if(zchan>NCHANMAX)
        usage();
    unsigned nchan = zchan;
    if(zchan==0)nchan=NCHANMAX;
    else zchan--;

    std::vector< std::vector<uint32_t> > vecChanMeas(NCHANMAX);

    for(unsigned ichan=zchan; ichan<nchan; ichan++)
        mcb.measureADC(iadc, ichan, nmeas, vecChanMeas[ichan], ndelay);

    for(unsigned i = 0; i < nmeas; ++i)
    {
        for(unsigned ichan=zchan; ichan<nchan; ichan++)
        {
            printf("%06.04f ", adc.voltData(vecChanMeas[ichan][i]));
        }
        printf("\n");
    }
}

void cbc::calibrate(unsigned idrive, unsigned nstep, unsigned ncycle, unsigned ndelay, unsigned iadc, unsigned nmeas, unsigned nburn, unsigned ichan)
{
    // invalid drive
    if((idrive<1)||(idrive>6))
        usage();
    else
        idrive--;

    // invalid adc
    if ((iadc<1)||(iadc>7))
        usage();
    else
        iadc--;

    // invalid channel
    if(ichan>8)
        usage();
    else
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
    for (unsigned icycle = 0; icycle<ncycle; icycle++)
    {
        // Default Direction is Retract
        MirrorControlBoard::Dir dir  = MirrorControlBoard::DIR_RETRACT;

        // Extend on even icycles
        if ((icycle + dir)%2 == 0)
            dir = MirrorControlBoard::DIR_EXTEND;

        for (unsigned istep=0; istep<nstep; istep++)
        {
            // Move motor one step
            mcb.stepOneDrive(idrive, dir);
            // Wait a moment
            mcb.loopDelay(ndelay);
            // Measure on ADC
            mcb.measureADCStat(iadc, ichan, nmeas, sum[ichan], sum2[ichan], min[ichan], max[ichan], nburn, ndelay_adc);
        } // close (for istep)
    } // close (for icycle)

    for (unsigned idatum=0; idatum<nstep*ncycle; idatum++)
    {
        unsigned icycle = idatum / nstep;
        unsigned istep = idatum%nstep;
        if (icycle & 0x1)
            istep=nstep-istep-1;
        else
            istep++;

        std::cout << istep << ' ';

        if(nmeas>1)
        {
            uint32_t mean   = sum[idatum]/nmeas;
            uint64_t s      = sum[idatum];
            uint64_t var    = (sum2[idatum] - s*s/nmeas)/nmeas;
            uint32_t rms    = julery_isqrt(uint32_t(var));

            float adcmean = adc.voltData(mean,        volt_full);
            float adcrms  = adc.voltData(rms,         volt_full);
            float adcmax  = adc.voltData(max[idatum], volt_full);
            float adcmin  = adc.voltData(min[idatum], volt_full);

            printf("i mean   rms    max    min     \n");
            printf("%i %06.04f %06.04f %06.04f %06.04f\n",idatum,adcmean,adcrms,adcmax,adcmin);
        }
        else
        {
            float adcsum = adc.voltData(sum[idatum],volt_full);
            printf("%u %06.04f\n", sum[idatum], adcsum);
        }
    } // close (for idatum)

    delete[] sum;   //delete pointers
    delete[] sum2;  //delete pointers
    delete[] min;   //delete pointers
    delete[] max;   //delete pointers
}

unsigned cbc::julery_isqrt(unsigned long val)
{
    unsigned long temp, g=0, b = 0x8000, bshft = 15;
    do
    {
        if (val >= (temp = (((g << 1) + b)<<bshft--)))
        {
            g += b;
            val -= temp;
        }
    }
    while (b >>= 1);
    return g;
}

int cbc::usage()
{
    std::cout <<usage_text;
    exit(EXIT_SUCCESS);
}

std::string cbc::usage_text =
"CBC Usage: command [command data]\n"
"\ninitialize             CLK_DIV=4: Initialize the hardware. Should be done once after boot-up."
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
