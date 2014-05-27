/*
 * Hexapod motion demo
 */

#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <string>
#include <cassert>
#include <cmath>

#include <GPIOInterface.hpp>
#include <Layout.hpp>
#include <TLC3548_ADC.hpp>

//fast integer square root by Jim Ulery
static unsigned julery_isqrt(unsigned long val)
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

const char* usage_text = "\n    amplitude [delay=5000] [period_factor=1.0]\n"
                         "\n    where \"amplitude\" is the amplitude of the motion in steps"
                         "\n    (half the total peak-to-peak motion), \"delay\" is the length"
                         "\n    of the delay loop and \"period_factor\" is the period of the"
                         "\n    motion in units of amplitude*2*PI (this factor must be"
                         "\n    greater than or equal to 1.0).";

void usage(const char* program, std::ostream& stream, int status = EXIT_FAILURE)
{
    stream << "Usage: " << program << usage_text;
    exit(status);
}

int main(int argc, char** argv)
{
    GPIOInterface gpio;
    Layout layout;

    char* program = *argv;
    argv++, argc--;

    if(argc == 0)usage(program, std::cout, EXIT_SUCCESS);
    int32_t iamp = atoi(*argv);
    argv++, argc--;

    unsigned ndelay = 5000;
    if(argc)
    {
        ndelay = atoi(*argv);
        argv++, argc--;
    }

    float pfactor = 1.0;
    if(argc)
    {
        pfactor = atof(*argv);
        argv++, argc--;
    }

    if(pfactor<1.0)
        pfactor=1.0;

    unsigned nperiod = lroundf(pfactor*float(iamp)*M_PI*2.0);

    unsigned nloopA = julery_isqrt(nperiod);
    unsigned phase_offset = nloopA/3;
    nloopA = phase_offset*3;

    unsigned nloopB = nperiod/nloopA;
    nperiod = nloopA*nloopB;

    float amp = float(nperiod)/M_PI*0.5/pfactor;
    //float sqrtamp = sqrtf(amp);
    iamp = lroundf(amp);

    int64_t* sinA = new int64_t[nloopA];
    int64_t* cosA = new int64_t[nloopA];
    int64_t* sinB = new int64_t[nloopB];
    int64_t* cosB = new int64_t[nloopB];

    for(unsigned iloopA=0; iloopA<nloopA; iloopA++)
    {
        float phi = float(iloopA)/float(nloopA)*M_PI*2.0;
        sinA[iloopA] = llroundf(amp*sin(phi));
        cosA[iloopA] = llroundf(amp*cos(phi));
    }

    for(unsigned iloopB=0; iloopB<nloopB; iloopB++)
    {
        float phi = float(iloopB)/float(nperiod)*M_PI*2.0;
        sinB[iloopB] = llroundf(amp*sin(phi));
        cosB[iloopB] = llroundf(amp*cos(phi));
    }

    int64_t ix1a = -iamp;
    int64_t ix2a = -iamp;
    int64_t ix3a = -iamp;

    while(1)
    {
        for (unsigned iloopA=0; iloopA<nloopA; iloopA++)
        {
            const unsigned p1 = iloopA;
            const unsigned p2 = (p1+phase_offset)%nloopA;
            const unsigned p3 = (p2+phase_offset)%nloopA;
            for(unsigned iloopB=0; iloopB<nloopB; iloopB++)
            {
                const int64_t ix1t = (sinA[p1]*cosB[iloopB]+cosA[p1]*sinB[iloopB])/iamp;
                const int64_t ix2t = (sinA[p2]*cosB[iloopB]+cosA[p2]*sinB[iloopB])/iamp;
                const int64_t ix3t = (sinA[p3]*cosB[iloopB]+cosA[p3]*sinB[iloopB])/iamp;

                if (ix1t != ix1a)
                {
                    int dir = 0;
                    if (ix1t > ix1a)
                        ix1a++;
                    else
                    {
                        ix1a--;
                        dir=1;
                    }
                    gpio.WriteLevel(layout.igpioDir1,dir);
                    gpio.WriteLevel(layout.igpioDir4,dir);
                    gpio.WriteLevel(layout.igpioStep1,1);
                    gpio.WriteLevel(layout.igpioStep4,1);
                }

                if(ix2t != ix2a)
                {
                    int dir = 0;
                    if (ix2t > ix2a)
                        ix2a++;
                    else
                    {
                        ix2a--;
                        dir=1;
                    }
                    gpio.WriteLevel(layout.igpioDir2,dir);
                    gpio.WriteLevel(layout.igpioDir5,dir);
                    gpio.WriteLevel(layout.igpioStep2,1);
                    gpio.WriteLevel(layout.igpioStep5,1);
                }

                if(ix3t != ix3a)
                {
                    int dir = 0;
                    if(ix3t > ix3a)
                        ix3a++;
                    else
                    {
                        ix3a--;
                        dir=1;
                    }
                    gpio.WriteLevel(layout.igpioDir3,dir);
                    gpio.WriteLevel(layout.igpioDir6,dir);
                    gpio.WriteLevel(layout.igpioStep3,1);
                    gpio.WriteLevel(layout.igpioStep6,1);
                }

                //delay
                for(volatile unsigned idelay=0; idelay<ndelay; idelay++);

                gpio.WriteLevel(layout.igpioStep1,0);
                gpio.WriteLevel(layout.igpioStep2,0);
                gpio.WriteLevel(layout.igpioStep3,0);
                gpio.WriteLevel(layout.igpioStep4,0);
                gpio.WriteLevel(layout.igpioStep5,0);
                gpio.WriteLevel(layout.igpioStep6,0);

                //delay
                for(volatile unsigned idelay=0; idelay<ndelay; idelay++);

                std::cout << ix1a << ' ' << ix2a << ' ' << ix3a << '\n';
            } // close iloopB
        } // close iloopA
    } // close while(1)
} // close main
