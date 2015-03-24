/*
 *  MirrorControlBoard.cpp - Class which implements useful mid-level
 *  functionality for the mirror control board.
 */

#include <cassert>
#include <MirrorControlBoard.hpp>
#include <GPIOInterface.hpp>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <cstdlib>
#include <pthread.h>
#include <stdio.h>
#include <iostream>
#include <chrono>

MirrorControlBoard::MirrorControlBoard(int calibrationConstant)
{
    m_calibrationConstant = calibrationConstant;
}

MirrorControlBoard::~MirrorControlBoard()
{
}

void MirrorControlBoard::enableIO ()
{
    gpio.WriteLevel(layout.igpioEN_IO, 1);
}

void MirrorControlBoard::disableIO ()
{
    gpio.WriteLevel(layout.igpioEN_IO, 0);
}

void MirrorControlBoard::adcSleep (int iadc)
{
    // Set on-board ADC into sleep mode
    selectADC(iadc);
    //spi.Configure();
    spi.WriteRead(ADC.codeSWPowerDown());

    selectADC(iadc);
    //spi.Configure();
    spi.WriteRead(ADC.codeSWPowerDown());
}

void MirrorControlBoard::powerDownUSB(unsigned iusb)
{
    gpio.WriteLevel(layout.igpioUSBOff(iusb),1);
}

void MirrorControlBoard::powerUpUSB(unsigned iusb)
{
    gpio.WriteLevel(layout.igpioUSBOff(iusb),0);
}

bool MirrorControlBoard::isUSBPoweredUp(unsigned iusb)
{
    return gpio.ReadLevel(layout.igpioUSBOff(iusb))?false:true;
}

void MirrorControlBoard::powerDownDriveControllers()
{
    gpio.WriteLevel(layout.igpioSleep,0);
}

void MirrorControlBoard::powerUpDriveControllers()
{
    gpio.WriteLevel(layout.igpioSleep,1);
}

bool MirrorControlBoard::isDriveControllersPoweredUp()
{
    return gpio.ReadLevel(layout.igpioSleep)?true:false;
}

void MirrorControlBoard::powerDownEncoders()
{
    gpio.WriteLevel(layout.igpioEncoderEnable,0);
}

void MirrorControlBoard::powerUpEncoders()
{
    gpio.WriteLevel(layout.igpioEncoderEnable,1);
}

bool MirrorControlBoard::isEncodersPoweredUp()
{
    return gpio.ReadLevel(layout.igpioEncoderEnable)?true:false;
}

void MirrorControlBoard::powerUpSensors()
{
    gpio.WriteLevel(layout.igpioPowerADC,1);
}

void MirrorControlBoard::powerDownSensors()
{
    gpio.WriteLevel(layout.igpioPowerADC,0);
}

bool MirrorControlBoard::isSensorsPoweredUp()
{
    return gpio.ReadLevel(layout.igpioPowerADC)?true:false;
}

void MirrorControlBoard::enableDriveSR(bool enable)
{
    gpio.WriteLevel(layout.igpioSR, enable?0:1);
}


void MirrorControlBoard::disableDriveSR()
{
    enableDriveSR(false);
}

bool MirrorControlBoard::isDriveSREnabled()
{
    return gpio.ReadLevel(layout.igpioSR)?false:true;
}

void MirrorControlBoard::setUStep(UStep ustep)
{
    unsigned mslog2 = 0;
    switch(ustep) {
        case USTEP_1:
            mslog2 = 0x0;
            break;
        case USTEP_2:
            mslog2 = 0x1;
            break;
        case USTEP_4:
            mslog2 = 0x2;
            break;
        case USTEP_8:
            mslog2 = 0x3;
            break;
    }
    gpio.WriteLevel(layout.igpioMS1, mslog2 & 0x1);
    gpio.WriteLevel(layout.igpioMS2, mslog2 & 0x2);
}

MirrorControlBoard::UStep MirrorControlBoard::getUStep()
{
    if(gpio.ReadLevel(layout.igpioMS2))
        return gpio.ReadLevel(layout.igpioMS1)?USTEP_8:USTEP_4;
    else
        return gpio.ReadLevel(layout.igpioMS1)?USTEP_2:USTEP_1;
}

void MirrorControlBoard:: stepOneDrive(unsigned idrive, Dir dir, unsigned frequency)
{
    /* Give this thread higher priority to improve timing stability */
    pthread_t this_thread = pthread_self();
    struct sched_param params;
    params.sched_priority = sched_get_priority_max(SCHED_FIFO);
    pthread_setschedparam(this_thread, SCHED_FIFO, &params);

    /* Write Direction to the DIR pin */
    gpio.WriteLevel(layout.igpioDir(idrive),(dir==DIR_RETRACT)?1:0);

    /* Writes one step to STEP pin */
    unsigned igpio = layout.igpioStep(idrive);
    gpio.WriteLevel(igpio,(dir==DIR_NONE)?0:1);

    /* a delay */
    waitHalfPeriod(frequency);

    /* Toggle pin back to low */
    gpio.WriteLevel(igpio,0);

    /* a delay */
    waitHalfPeriod(frequency);
    sched_yield();
}

void MirrorControlBoard::setPhaseZeroOnAllDrives()
{
    gpio.WriteLevel(layout.igpioReset,0);
    waitHalfPeriod(400);
    gpio.WriteLevel(layout.igpioReset,1);
}

void MirrorControlBoard::enableDrive(unsigned idrive, bool enable)
{
    gpio.WriteLevel(layout.igpioEnable(idrive), enable?0:1);
}

void MirrorControlBoard::disableDrive(unsigned idrive)
{
    enableDrive(idrive, false);
}

bool MirrorControlBoard::isDriveEnabled(unsigned idrive)
{
    return gpio.ReadLevel(layout.igpioEnable(idrive))?false:true;
}

void MirrorControlBoard::enableDriveHiCurrent(bool enable)
{
    gpio.WriteLevel(layout.igpioPwrIncBar, enable?0:1);
}

void MirrorControlBoard::disableDriveHiCurrent()
{
    enableDriveHiCurrent(false);
}


bool MirrorControlBoard::isDriveHiCurrentEnabled()
{
    return gpio.ReadLevel(layout.igpioPwrIncBar)?false:true;
}

//------------------------------------------------------------------------------
// ADCs
//------------------------------------------------------------------------------

void MirrorControlBoard::initializeADC(unsigned iadc)
{
    selectADC(iadc);                                        // Assert Chip Select for ADC in question
    spi.WriteRead(ADC.codeInitialize());
    spi.WriteRead(ADC.codeConfig());
}

void MirrorControlBoard::selectADC(unsigned iadc)
{
    gpio.WriteLevel(layout.igpioADCSel1, iadc==0?1:0);
    gpio.WriteLevel(layout.igpioADCSel2, iadc==1?1:0);
}

uint32_t MirrorControlBoard::measureADC(unsigned iadc, unsigned ichan)
{

    initializeADC(iadc);

    // Assert Chip Select
    selectADC(iadc);

    // ADC Channel Select
    uint32_t code = ADC.codeSelect(ichan);
    spi.WriteRead(code);

    // Read ADC
    uint32_t datum = spi.WriteRead(ADC.codeReadFIFO());

    return ADC.decodeUSB(datum);
}

void MirrorControlBoard::measureADCStat(unsigned iadc, unsigned ichan, unsigned nmeas, uint32_t& sum, uint64_t& sumsq, uint32_t& min, uint32_t& max, unsigned ndelay)
{
    //spi.Configure();
    initializeADC(iadc);
    selectADC(iadc);
    uint32_t code   = ADC.codeSelect(ichan);
    unsigned nburn = 1;
    unsigned nloop  = nburn + nmeas;
    sum             = 0;
    sumsq           = 0;
    max             = 0;
    min             = ~max;

    // Loop over number of measurements
    for(unsigned iloop=0; iloop<nloop; iloop++) {
        // Read data
        uint32_t datum = spi.WriteRead(code);

        if (iloop>nburn) {
            // Decode data
            datum = ADC.decodeUSB(datum);
            // Typecast to 64 bit integer..
            uint64_t datum64 = datum;

            // Accumulate statistics
            sum+=datum;
            sumsq+=datum64*datum64;
            if(datum>max)
                max=datum;
            if(datum<min)
                min=datum;
        }

        for (volatile unsigned i=0; i<ndelay; i++);
    }

    // Read last FIFO
    uint32_t datum = spi.WriteRead(ADC.codeReadFIFO());
    //decode data
    datum = ADC.decodeUSB(datum);

    //typecast and accumulate statistics
    uint64_t datum64 = datum;
    sum+=datum;
    sumsq+=datum64*datum64;

    if(datum>max)
        max=datum;
    if(datum<min)
        min=datum;
}

//------------------------------------------------------------------------------
// General Purpose Utilities
//------------------------------------------------------------------------------

void MirrorControlBoard::waitHalfPeriod(unsigned frequency)
{

#define NANOS 1000000000LL
    /*
     * Method 1: Sleep Method
     */
    //long halfperiod = NANOS/ ( 2*frequency);
    //struct timespec delay;
    //delay.tv_sec = 0;
    //delay.tv_nsec = halfperiod/2;
    //clock_nanosleep(CLOCK_MONOTONIC, 0, &delay, NULL);



    /*
     * Method 2: Waiting for a clock to pass
     */

    //long long halfperiod = NANOS /(1000*2*frequency);
    //long long start, elapsed, microseconds;
    //struct timespec begin, current;

    ///* set up start time data */
    //if (clock_gettime(CLOCK_MONOTONIC, &begin)) {
    //    return;
    //}

    ///* Start time in nanoseconds */
    //start = begin.tv_sec*NANOS + begin.tv_nsec;

    //while (true) {
    //    /* Elapsed time in nanoseconds */
    //    if (clock_gettime(CLOCK_MONOTONIC, &current)) {
    //        return;
    //    }

    //    elapsed = current.tv_sec*NANOS + current.tv_nsec - start;
    //    microseconds = elapsed / 1000; // + (elapsed % 1000 >= 500); // round up halves

    //    if (microseconds >= halfperiod)
    //        break;
    //}


    /*
     * Method 3: For loop
     */

    /*
     * This is a MACHINE DEPENDANT calibration constant
     * its units are [for-loops/second]
     */
    //unsigned nloops = m_calibrationConstant/(2*frequency);
    //for(volatile unsigned iloop=0;iloop<nloops;iloop++);


    /*
     * Method 4: std::chrono (note! requires C++11)
     */
    using std::chrono::nanoseconds;
    using std::chrono::duration_cast;
    long int halfperiod = (NANOS / ( 2*frequency));
    auto start = duration_cast<nanoseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
    while (true) {
        /* Give this thread higher priority to improve timing stability */
        pthread_t this_thread = pthread_self();
        struct sched_param params;
        params.sched_priority = sched_get_priority_max(SCHED_FIFO);
        pthread_setschedparam(this_thread, SCHED_FIFO, &params);

        auto now = duration_cast<nanoseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
        auto diff = now-start;
        if (diff > halfperiod)
            break;
    }
    sched_yield();
}

void MirrorControlBoard::setCalibrationConstant(int constant)
{
    m_calibrationConstant = constant;
}

int MirrorControlBoard::getCalibrationConstant()
{
    return (m_calibrationConstant);
}
