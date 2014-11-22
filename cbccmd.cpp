#include <cbc.hpp>

#include <iostream>
#include <string>
#include <cstdlib>
#include <cstring>
#include <stdio.h>

void usage(); 

int main(int argc, const char** argv)
{
    argv++, argc--;

    CBC cbc;

    if(argc == 0)
        usage();

    std::string command = *argv;
    argv++, argc--;

    if (command == "power")
    {
        if(argc==0)
            usage();

        if (strcmp(*argv,"up")==0)
            cbc.powerUp();
        else if (strcmp(*argv,"down")==0)
            cbc.powerDown();
    }
    else if (command == "power_down")
        cbc.powerDown();
    else if (command == "power_up")
        cbc.powerUp();
    else if (command == "power_down_encoders")
        cbc.encoder.disable();
    else if (command == "power_up_encoders")
        cbc.encoder.enable();
    else if (command == "power_down_sensors")
        cbc.sensor.disable();
    else if (command == "power_up_sensors")
        cbc.sensor.enable();
    else if (command == "power_down_a3977")
        cbc.driver.sleep(); 
    else if (command == "power_up_a3977")
        cbc.driver.wakeup();
    else if (command == "reset")
        cbc.driver.reset();
    else if (command == "enable_sr")
        cbc.driver.enableSR();
    else if (command == "disable_sr")
        cbc.driver.disableSR();
    else if (command == "set_microstep")
    {
        if(argc==0)
            usage();
        unsigned usint = atoi(*argv);

        cbc.driver.setMicrosteps(usint);
    }
    else if (command == "enable_hi_current")
        cbc.driver.enableHighCurrent();
    else if (command == "disable_hi_current")
        cbc.driver.disableHighCurrent();
    else if (command == "enable")
    {
        if(argc==0)
            usage();

        unsigned idrive;
        if (strcmp(*argv,"all")==0)
            idrive=0;
        else
            idrive = atoi(*argv);

        cbc.driver.enable(idrive);
    }
    else if (command == "disable")
    {
        if(argc==0)
            usage();

        unsigned idrive;
        if (strcmp(*argv,"all")==0)
            idrive=0;
        else
            idrive = atoi(*argv);

        cbc.driver.disable(idrive);
    }
    else if ( (command == "enableusb") | (command == "enableUSB") )
    {
        if(argc==0)
            usage();

        unsigned iusb;
        if (strcmp(*argv,"all")==0)
            iusb=0;
        else
            iusb = atoi(*argv);

        cbc.usb.enable(iusb);
    }
    else if ((command == "disableusb") | (command == "disableUSB"))
    {
        if(argc==0)
            usage();

        unsigned iusb;
        if (strcmp(*argv,"all")==0)
            iusb=0; //iusb=0 for all channels
        else
            iusb = atoi(*argv);

        cbc.usb.disable(iusb);
    }
    
    else if (command == "step")
    {
        if(argc<2)
            usage();

        // actuator number
        unsigned idrive = atoi(*argv);
        argc--, argv++;

        // number of setps
        int nstep = atoi(*argv);
        argc--, argv++;

        cbc.driver.step(idrive, nstep);
    }

    else if (command == "slew")
    {
        // whine if no arugment given
        if(argc==0)
            usage();

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
                usage();
            argc--, argv++;
        }

        cbc.driver.slew(idrive, dir);
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

        cbc.driver.stepAll(nstep1, nstep2, nstep3, nstep4, nstep5, nstep6);
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
                usage();
            argc--, argv++;
        }

        cbc.driver.slewAll(dir);
    }
    else if (command == "status")
    {
        printf("Mirror Control Board status: \n");

        // Motor Driver Status
        printf("\nA3977 Drive Controllers:        ");
        if (cbc.driver.isAwake())
            printf("Enabled");
        else
            printf("Disabled");

        // SR Status
        printf("\nSynchonous Rectification (SR):  ");
        if (cbc.driver.isSREnabled())
            printf("Enabled");
        else
            printf("Disabled");

        // Microstep Status
        printf("\nDrive microstep set to:         ");
        switch(cbc.driver.getMicrosteps())
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
        if (cbc.driver.isHighCurrentEnabled())
            printf("Enabled");
        else
            printf("Disabled");

        // Encoder Status
        printf("\nEncoders:                       ");
        if (cbc.encoder.isEnabled())
            printf("Enabled");
        else
            printf("Disabled");

        // Sensor Power Status
        printf("\nSensor Enabled:             ");
        if (cbc.sensor.isEnabled())
            printf("Enabled");
        else
            printf("Disabled");

        // Print Drive Status
        printf("\n");
        for(int idrive=0; idrive<6; idrive++)
        {
            if (cbc.driver.isEnabled(idrive))
                printf("\nDrive %i                         Enabled",  idrive+1);
            else
                printf("\nDrive %i                         Disabled", idrive+1);
        }

        // Print USB Status
        printf("\n");
        for (int iusb=0; iusb<7; iusb++)
        {
            if (cbc.usb.isEnabled(iusb))
                printf("\nUSB %i                           Enabled",  iusb+1);
            else
                printf("\nUSB %i                           Disabled",  iusb+1);
        }


        printf("\n");
    }
    else if (command == "readTemperature")
    {
        adcData data; 

        data = cbc.adc.onboardTemp(); 
        printf("Onboard Voltage: %5.4f", data.voltage); 
        printf("Onboard Stddev:  %5.4f", data.stddev); 

        data = cbc.adc.externalTemp(); 
        printf("External Voltage: %5.4f", data.voltage); 
        printf("External Stddev:  %5.4f", data.stddev); 
    }
    else if (command == "readEncoder")
    {
        if(argc<1)
            usage();
        
        // channel
        if(argc==0)
            usage();
        int chan = atoi(*argv);
        argc--, argv++;

        adcData data = cbc.adc.readEncoder(chan); 
        printf("Voltage: %5.4f", data.voltage); 
        printf("Stddev:  %5.4f", data.stddev); 
    }
    else
        usage();
}

void usage () {
}
