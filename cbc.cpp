/*
 * cbc.cpp Console board control
 */

#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <string>
#include <cassert>
#include <cstring>
#include <vector>
#include <unistd.h>
#include <time.h>
#include <cbc.hpp>
#include <stdio.h>
#include <pthread.h>
#define NCHANMAX 11

int main(int argc, const char** argv)
{
    int stepping_frequency = 400;
    argv++, argc--;

    cbc cbc;
    MirrorControlBoard mcb;


    if(argc == 0)
        cbc.usage();

    std::string command = *argv;
    argv++, argc--;

    if ( (command ==  "init") | (command == "initialize") | (command=="config") )
        cbc.initialize();
    else if (command == "power")
    {
        if(argc==0)
            cbc.usage();

        if (strcmp(*argv,"up")==0)
            mcb.powerUpAll();
        else if (strcmp(*argv,"down")==0)
            mcb.powerDownAll();
    }
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
    else if ( (command == "enableusb") | (command == "enableUSB") )
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
    else if ((command == "disableusb") | (command == "disableUSB"))
    {
        if(argc==0)
            cbc.usage();

        unsigned iusb;
        if (strcmp(*argv,"all")==0)
            iusb=0; //iusb=0 for all channels
        else
            iusb = atoi(*argv);

        cbc.disableusb(iusb);
    }
    //------------------------------------------------------------------------------
    // Some option handlers for backwards compatibility
    else if (command == "enable_all")       { cbc.enable(0); }
    else if (command == "disable_all")      { cbc.disable(0); }
    else if (command == "enableUSB_all")    { cbc.disableusb(0); }
    else if (command == "disableUSB_all")   { cbc.disableusb(0); }
    // end of legacy functions..
    //------------------------------------------------------------------------------
    else if (command == "testgpio")
    {
        if (argc==0)
            cbc.usage();
        unsigned igpio = atoi(*argv);
        cbc.testgpio(igpio);
    }
    else if (command == "writespi")
    {
        if (argc==0)
            cbc.usage();
        uint32_t data = strtol(*argv, NULL, 16);
        SpiInterface spi;
        spi.Configure();
        printf("write: %04X", data);
        data = spi.WriteRead(data);
        printf("read: %04X", data);
    }
    else if (command == "testdrive")
    {
        if (argc==0)
            cbc.usage();
        int idrive = atoi(*argv);
        cbc.testdrive (idrive);

    }
    else if (command == "freqloop")
    {
        if (argc==0)
            cbc.usage();
        unsigned frequency = atoi(*argv);
        cbc.freqloop(frequency);
    }
    else if (command == "frequ")
    {
        if (argc==0)
            cbc.usage();
        unsigned frequency = atoi(*argv);
        cbc.frequ(frequency);
    }
    else if (command == "freqn")
    {
        if (argc==0)
            cbc.usage();
        unsigned frequency = atoi(*argv);
        cbc.freqn(frequency);
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
        unsigned frequency = stepping_frequency;
        if(argc)
        {
            frequency = atoi(*argv);
            argc--, argv++;
        }
        cbc.step(idrive, nstep, frequency);
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

        unsigned frequency = stepping_frequency; // Hz
        if(argc)
        {
            frequency = atoi(*argv);
            argc--, argv++;
        }

        cbc.slew(idrive, dir, frequency);
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

        unsigned frequency = stepping_frequency; // Hz
        if(argc)
        {
            frequency = atoi(*argv);
            argc--, argv++;
        }
        cbc.step_all(nstep1, nstep2, nstep3, nstep4, nstep5, nstep6, frequency);
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

        unsigned frequency = stepping_frequency;
        if(argc)
        {
            frequency = atoi(*argv);
            argc--, argv++;
        }

        cbc.slew_all(dir, frequency);
    }
    else if (command == "status")
        cbc.status();
    else if (command == "measure")
    {
        if(argc<2)
            cbc.usage();
        unsigned iadc = atoi(*argv);
        argc--, argv++;

        // channel
        if(argc==0)
            cbc.usage();
        unsigned zchan = atoi(*argv);
        argc--, argv++;

        // number of measurements
        unsigned nmeas = 1;
        if(argc)
        {
            nmeas = atoi(*argv);
            argc--, argv++;
        }

        // number to burn
        unsigned nburn = 0;
        if(argc)
        {
            nburn = atoi(*argv);
            argc--, argv++;
        }

        // delay (burn an argument for legacy purposes). 
        if(argc) {
            argc--, argv++;
        }
        
        // voltage reference
        float volt_full = 5.00;
        if(argc)
        {
            volt_full = atof(*argv);
            argc--, argv++;
        }

        // hex out yes or no ? 
        int hex_out = 0;
        if(argc)
        {
            hex_out = atoi(*argv);
            argc--, argv++;
        }

        cbc.measure(iadc, zchan, nmeas, nburn, volt_full, hex_out);
    }
    else if (command == "measure_full")
    {
        // adc number 
        if(argc==0)
            cbc.usage();
        unsigned iadc = atoi(*argv);
        argc--, argv++;

        // adc channel
        if(argc==0)
            cbc.usage();
        unsigned zchan = atoi(*argv);
        argc--, argv++;

        // number of measurements
        unsigned nmeas = 1;
        if(argc)
        {
            nmeas = atoi(*argv);
            argc--, argv++;
        }

        // delay (burn a value for legacy reasons)
        argc--, argv++;

        // voltage reference
        float volt_full = 5.00;
        if(argc)
        {
            volt_full = atof(*argv);
            argc--, argv++;
        }
        cbc.measure_full(iadc, zchan, nmeas, volt_full);
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

        // delay
        unsigned frequency = stepping_frequency;
        if(argc)
        {
            frequency = atoi(*argv);
            argc--, argv++;
        }

        cbc.step(idrive, nstep, frequency);
        //unsigned dir = 0;
        //if(ncycle <= 1)
        //    dir = ncycle, ncycle = 1;

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
        cbc.calibrate(idrive, nstep, ncycle, frequency, iadc, nmeas, nburn, ichan);
    }
    else
        cbc.usage();
}

int cbc::initialize()
{
    // configure gpio directions
    printf("Configuring GPIOs...\n");
    gpio.ConfigureAll();

    printf("Powering up and configuring base...\n");
    mcb.powerUpBase();

    //disable all usbs except 1
    printf("Turn off USBs...\n");
    for (int i=2; i<=7; i++) {
        disableusb(i);
    }

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

void cbc::testdrive(int idrive)
{
    srand (time(NULL));
    int nsamples = 1000;
    //enable drive
    enable(idrive);
    for (int npasses=0; npasses < 10; npasses++)
    {
        int nsteps = rand() % 100;  //random number from 0 to 99
        uint32_t datum = 0;

        // take some number of samples from adc
        for (int i=0; i<nsamples; i++)
            datum += mcb.measureADC(0,idrive-1);
        datum = datum / nsamples;
        float initial_position = adc.voltData(datum,5.00);

        usleep(100000);
        // step motor
        step (idrive, nsteps, 1000);


        // take some number of samples from adc
        datum = 0;
        for (int i=0; i<nsamples; i++)
            datum += mcb.measureADC(0,idrive-1);
        datum = datum / nsamples;
        float mid_position = adc.voltData(datum,5.00);

        // step motor back
        step (idrive, -nsteps, 1000);

        // take some number of samples from adc
        datum = 0;
        for (int i=0; i<nsamples; i++)
            datum += mcb.measureADC(0,idrive-1);
        datum = datum / nsamples;
        float final_position = adc.voltData(datum,5.00);

        float diff = initial_position - final_position;
        float nsteps_measured = 8.0*(mid_position-initial_position)/0.006267;
        printf ("drive %02i: steps=%02i step_measured=%02.02f delta position=%.04f\n", idrive, nsteps, nsteps_measured, diff);
    }
    // disable drive once again
    disable(idrive);
}

void cbc::testgpio(unsigned igpio)
{
    for (int i=0; i<10000000; i++)
    {
        if (i%2==0)
            gpio.WriteLevel(igpio,1);
        if (i%2==1)
            gpio.WriteLevel(igpio,0);
        usleep(1000);
    }
}

void cbc::frequ(unsigned frequency)
{
    int period = (1000000/(frequency));
    period=period/2;

    for (int i=0; i<1000; i++)
    {
        gpio.WriteLevel(layout.igpioUSBOff4,1);
        usleep(period);
        gpio.WriteLevel(layout.igpioUSBOff4,0);
        usleep(period);
    }
}
void cbc::freqn(unsigned frequency)
{
    // Increase Scheduleing Priority for this Loop
    int policy;
    struct sched_param param;
    pthread_getschedparam(pthread_self(), &policy, &param);
    param.sched_priority = sched_get_priority_max(policy);
    policy = SCHED_FIFO;
    pthread_setschedparam(pthread_self(), policy, &param);

    for (int i=0; i<1000; i++)
    {
        gpio.WriteLevel(layout.igpioUSBOff4,1);
        mcb.waitHalfPeriod(frequency);
        gpio.WriteLevel(layout.igpioUSBOff4,0);
        mcb.waitHalfPeriod(frequency);
    }
}

void cbc::freqloop(unsigned nloop)
{
    for (int i=0; i<1000; i++)
    {
        gpio.WriteLevel(layout.igpioUSBOff4,1);
        for(volatile unsigned iloop=0; iloop<nloop; iloop++);
        gpio.WriteLevel(layout.igpioUSBOff4,0);
        for(volatile unsigned iloop=0; iloop<nloop; iloop++);
    }
}

void cbc::step(unsigned idrive, int nstep, unsigned frequency)
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
        mcb.stepOneDrive(idrive, dir, frequency);
        mcb.waitHalfPeriod(frequency);
    }
}

void cbc::slew(unsigned idrive, MirrorControlBoard::Dir dir, unsigned frequency)
{
    // whine if invalid drivenumber
    if ((idrive<1)||(idrive>6))
        usage();

    // count from zero
    idrive--;

    // Loop forever while stepping the motor
    while(1)
    {
        mcb.stepOneDrive(idrive, dir, frequency);
        mcb.waitHalfPeriod(frequency);
    }
}

void cbc::step_all(int nstep1, int nstep2, int nstep3, int nstep4, int nstep5, int nstep6, unsigned frequency)
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
        mcb.stepAllDrives(dir1, dir2, dir3, dir4, dir5, dir6, frequency);
        // some delay
        mcb.waitHalfPeriod(frequency);
    }
}

void cbc::slew_all(MirrorControlBoard::Dir dir, unsigned frequency)
{
    while(1)
    {
        mcb.stepAllDrives(dir, dir, dir, dir, dir, dir);
        mcb.waitHalfPeriod(frequency);
    }
}

void cbc::status()
{
    printf("Mirror Control Board status: \n");

    // Motor Driver Status
    printf("\nA3977 Drive Controllers:        ");
    if (mcb.isDriveControllersPoweredUp())
        printf("Enabled");
    else
        printf("Disabled");

    // Reset Status
    printf("\nDrive Reset:                    ");
    if (gpio.ReadLevel(layout.igpioReset))
        printf("Disabled");
    else
        printf("Enabled");

    // SR Status
    printf("\nSynchonous Rectification (SR):  ");
    if (mcb.isDriveSREnabled())
        printf("Enabled");
    else
        printf("Disabled");

    // Microstep Status
    printf("\nDrive microstep set to:         ");
    switch(mcb.getUStep())
    {
        case MirrorControlBoard::USTEP_8:
            printf("8-uSTEP");
            break;
        case MirrorControlBoard::USTEP_4:
            printf("4-uSTEP");
            break;
        case MirrorControlBoard::USTEP_2:
            printf("2-uSTEP");
            break;
        case MirrorControlBoard::USTEP_1:
            printf("1-uSTEP");
            break;
    }

    // Hi Current Status
    printf("\nHi-Current Mode:                ");
    if (mcb.isDriveHiCurrentEnabled())
        printf("Enabled");
    else
        printf("Disabled");

    // Encoder Status
    printf("\nEncoders:                       ");
    if (mcb.isEncodersPoweredUp())
        printf("Enabled");
    else
        printf("Disabled");

    // Print Drive Status
    printf("\n");
    for(unsigned idrive=0; idrive<6; idrive++)
    {
        if (mcb.isDriveEnabled(idrive))
            printf("\nDrive %i                         Enabled",  idrive+1);
        else
            printf("\nDrive %i                         Disabled", idrive+1);
    }

    // Print USB Status
    printf("\n");
    for (unsigned iusb=0; iusb<7; iusb++)
    {
        if (mcb.isUSBPoweredUp(iusb))
            printf("\nUSB %i                           Enabled",  iusb+1);
        else
            printf("\nUSB %i                           Disabled",  iusb+1);
    }


    printf("\n");
}

float cbc::measure(unsigned iadc, unsigned zchan, unsigned nmeas, unsigned nburn, float volt_full, int hex_out)
{
    if ((iadc<1)||(iadc>8))
        usage();
    // count from 0
    iadc--;

    if (zchan>NCHANMAX)
        usage();
    unsigned nchan = zchan;

    if (zchan==0)
        nchan=NCHANMAX;
    else
        zchan--;

    //------------------------------------------------------------------------------
    // good code: 
    // printf("ADC:        %i\n",          iadc+1);
    // printf("Channels:   %i to %i\n",    zchan+1,nchan);
    // printf("Nburn:      %i\n",          nburn);
    // printf("FullVolt    %f\n\n",        volt_full);
    //------------------------------------------------------------------------------
    // bad code
    std::cout 
        << "ADC:      " << iadc+1       << '\n'
		<< "Channels: " << zchan+1      << " to " << nchan << '\n'
		<< "NMeas:    " << nmeas        << '\n'
		<< "NBurn:    " << nburn        << '\n'
		<< "NDelay:   " << "0"          << '\n'
		<< "FullVolt: " << volt_full    << '\n';
    //------------------------------------------------------------------------------

    uint32_t sum [NCHANMAX];
    uint64_t sum2[NCHANMAX];
    uint32_t max [NCHANMAX];
    uint32_t min [NCHANMAX];

    for (unsigned ichan=zchan; ichan<nchan; ichan++)
        mcb.measureADCStat(iadc, ichan, nmeas, sum[ichan], sum2[ichan], min[ichan], max[ichan], nburn);

    //------------------------------------------------------------------------------
    // good code: 
    //if (hex_out == 0) 
    //{
    //    printf("i  mean   rms    max    min     \n");
    //}
    //else 
    //{
    //    good code: // printf("i  mean     rms      max      min     \n");
    //}
    //------------------------------------------------------------------------------

    for(unsigned ichan=zchan; ichan<nchan; ichan++)
    {
        uint32_t mean = sum[ichan]/nmeas;
        uint64_t s = sum[ichan];
        uint64_t var = (sum2[ichan] - s*s/nmeas)/nmeas;
        uint32_t rms = julery_isqrt(uint32_t(var));

        if(hex_out == 0)
        {
            float adcmean = adc.voltData(mean,      volt_full);
            float adcrms  = adc.voltData(rms,       volt_full);
            float adcmax  = adc.voltData(max[ichan],volt_full);
            float adcmin  = adc.voltData(min[ichan],volt_full);

            
            //------------------------------------------------------------------------------
            //bad code
            std::cout 
                << std::setw(2) << ichan+1 << ' '
                << std::fixed   
                << std::setw(6) << std::setprecision(4) << adcmean      << ' '
                << std::setw(6) << std::setprecision(4) << adcrms       << ' '
                << std::setw(6) << std::setprecision(4) << adcmax       << ' '
                << std::setw(6) << std::setprecision(4) << adcmin       << '\n';
            //------------------------------------------------------------------------------
            // good code: 
            // printf("%02i %06.04f %06.04f %06.04f %06.04f\n",ichan+1,adcmean,adcrms,adcmax,adcmin);
            //------------------------------------------------------------------------------
        }
        else
        {
            //------------------------------------------------------------------------------
            //bad code
            std::cout 
                << std::setw(2) << ichan+1 << ' '
                << std::hex << std::fixed 
                << std::setw(6) << mean << ' '
                << std::setw(6) << rms << ' '
                << std::setw(6) << max[ichan] << ' '
                << std::setw(6) << min[ichan] 
                << std::dec << '\n';
            //------------------------------------------------------------------------------
            // good code 
            // printf("%02i 0x%06X 0x%06X 0x%06X 0x%06X\n", ichan+1, mean, rms, max[ichan], min[ichan]);
            //------------------------------------------------------------------------------
        }
    } // close for ichan

    if(nchan != zchan + 1)
    {
        int v[4];
        v[0] = sum[0]/nmeas;
        v[1] = sum[1]/nmeas;
        v[2] = sum[2]/nmeas;
        v[3] = sum[3]/nmeas;

        //------------------------------------------------------------------------------
        //bad code
        std::cout 
            << "POS " << std::setw(7) << std::setprecision(4)
            << double(v[0] + v[1] - v[2] - v[3])/(v[0] + v[1] + v[2] + v[3]) << " ";
        std::cout 
            << std::setw(7) << std::setprecision(4) 
            << double(v[1] + v[2] - v[3] - v[0])/(v[0] + v[1] + v[2] + v[3]) << "\n";
        std::cout 
            << "POS " << std::setw(7) << std::setprecision(4) 
            << double(v[0] + v[2] - v[1] - v[3])/(v[0] + v[1] + v[2] + v[3]) << " ";
        std::cout 
            << std::setw(7) << std::setprecision(4) 
            << double(v[1] + v[2] - v[3] - v[0])/(v[0] + v[1] + v[2] + v[3]) << "\n";
        //------------------------------------------------------------------------------
        // good code 
        //printf("POS %07.04f ", double(v[0] + v[1] - v[2] - v[3])/(v[0] + v[1] + v[2] + v[3]));
        //printf("%07.04f\n",    double(v[1] + v[2] - v[3] - v[0])/(v[0] + v[1] + v[2] + v[3]));
        //printf("POS %07.04f ", double(v[0] + v[2] - v[1] - v[3])/(v[0] + v[1] + v[2] + v[3]));
        //printf("%07.04f\n",    double(v[1] + v[2] - v[3] - v[0])/(v[0] + v[1] + v[2] + v[3]));
        //------------------------------------------------------------------------------
    }

    if (nchan == NCHANMAX)
        return 0;
    else
        return (adc.voltData(sum[zchan]/nmeas, volt_full));
}

void cbc::measure_full(unsigned iadc, unsigned zchan, unsigned nmeas, unsigned volt_full)
{
    if((iadc<1)||(iadc>7))
        usage();
    iadc--;

    if(zchan>NCHANMAX)
        usage();
    unsigned nchan = zchan;
    if(zchan==0)
        nchan=NCHANMAX;
    else zchan--;

    // Loop over the number of measurements requested
    for(unsigned i = 0; i < nmeas; ++i)
    {
        // Loop over the number of channels
        for(unsigned ichan=zchan; ichan<nchan; ichan++)
        {
            // Print voltage
            uint32_t datum = mcb.measureADC(iadc,ichan);
            printf("%06.04f ", adc.voltData(datum));
        }
        printf("\n"); 
    }
}

void cbc::calibrate(unsigned idrive, unsigned nstep, unsigned ncycle, unsigned frequency, unsigned iadc, unsigned nmeas, unsigned nburn, unsigned ichan)
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
            mcb.waitHalfPeriod(frequency);
            // Measure on ADC
            mcb.measureADCStat(iadc, ichan, nmeas, sum[ichan], sum2[ichan], min[ichan], max[ichan], nburn);
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

            //------------------------------------------------------------------------------
            // good code
            //printf("i mean   rms    max    min     \n");
            //printf("%02i %06.04f %06.04f %06.04f %06.04f\n",idatum,adcmean,adcrms,adcmax,adcmin);
            //------------------------------------------------------------------------------
            // bad code
            std::cout 
            << mean << ' '
			<< std::fixed 
            << std::setw(6) << std::setprecision(4) << adcmean << ' ' 
            << std::setw(6) << std::setprecision(4) << adcrms  << ' '
			<< std::setw(6) << std::setprecision(4) << adcmax  << ' '
			<< std::setw(6) << std::setprecision(4) << adcmin  << '\n';
            //------------------------------------------------------------------------------
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
"CBC version 2.1.7 Usage:"
"\n    command                {required arguments} [optional arguments]\n"
"\n    initialize             Initialize the hardware. Should be done once"
"                             after boot-up. Configures GPIOs, turns on all"
"                             hardware except USBs." 
"\n"
"\n    power down             Go into power saving mode. Power down encoders, USB and A3977."
"\n    power up               Power up all on-board and off-board electronics."
"\n    "
"\n    power_down_encoders    Power down encoders."
"\n    power_up_encoders      Power up encoders."
"\n    "
"\n    power_down_a3977       Power down A3977 motor controllers."
"\n    power_up_a3977         Power up A3977 motor controllers."
"\n    "
"\n    reset                  Resets phase to zero on all drives."
"\n    "
"\n    enable_sr              Enable Synchronous Rectification (SR) mode."
"\n    disable_sr             Disable Synchronous Rectification (SR) mode."
"\n    "
"\n    set_microstep          {MS 1,2,4,8}"
"\n                           Set number of micro steps to 1, 2, 4 or 8."
"\n    "
"\n    enable_hi_current      Enable high current mode on all drives."
"\n    disable_hi_current     Disable high current mode on all drives."
"\n    "
"\n    enable                 {DR 1-6, or all)"
"\n                           Enable motor driver."
"\n    disable                {DR 1-6, or all}"
"\n                           Disable motor driver"
"\n    "
"\n    enableusb              {USB 1-7, or all}"
"\n                           Enable USB (USB)."
"\n    disableusb             {USB 1-7, or all"
"\n                           Disable USB (USB)."
"\n    "
"\n    step                   {DR 1-6} {NSTEPS} [Frequency=400]"
"\n                           Step drive some number of steps (positive to"
"\n                           extend, negative to retract) with frequency in Hz"
"\n    "
"\n    slew                   {DR 1-6} [DIR=(extend/retract)] [Frequency=400]"
"\n                           Slew drive (DR) in given direction (DIR, default extend) "
"\n                           with frequency in Hz."
"\n    "
"\n    step_all               {DR1_NSTEP DR2_NSTEP DR3_NSTEP DR4_NSTEP DR5_NSTEP DR6_NSTEP} [Frequency=400]"
"\n                           Step all drives some number of steps (positive to extend,"
"\n                           negative to retract and zero to not move that drive.)"
"\n    "
"\n    slew_all               [DIR=(extend/retract)] [Frequency=400]"
"\n                           Slew all (enabled) drives in given direction (DIR, default "
"\n                           extend) with frequency of steps in Hz."
"\n    "
"\n    status                 Print drive status information"
"\n    "
"\n    measure                {ADC 1-8} {CHAN 0-11} [MEAS=1 BURN=0 DELAY=0 SCALE=5.00]"
"\n                           Measure voltage of the specified ADC channel. Channel can be"
"\n                           given as zero to specify all channels on one ADC. Channels 9, 10"
"\n                           and 11 correspond to internal reference voltages on the ADC."
"\n                           Prints out statistics."
"\n                           Note that delay does nothing but you have to put"
"\n                           it anyway if you want to chance the scale."
"\n"
"\n    measure_full           {ADC 1-8} {CHAN 0-11} [MEAS=1 DELAY=0 SCALE=5.00]"
"\n                           Measure voltage of the specified ADC channel. Channel can be"
"\n                           given as zero to specify all channels on one ADC. Channels 9, 10"
"\n                           and 11 correspond to internal reference voltages on the ADC."
"\n                           Prints out raw data."
"\n                           Note that delay does nothing but you have to put" 
"\n                           it anyway if you want to chance the scale."
"\n"
"\n    calibrate              {DR 1-6} [NSTEP=10000 NCYCLE=0 FREQUENCY=400 ADC=7 MEAS=1]"
"\n                           Step drive and read ADC after each step, printing its value to"
"\n                           the terminal. If NCYCLE is 0 or 1 then it specifies the direction"
"\n                           of the travel, otherwise it specifies the number of half cycles"
"\n                           of expansion and contraction to perform."
"\ncbc version 2.1.7"
"\n";
