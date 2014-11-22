/*
 * cbc.cpp Console board control
 */

#include <iomanip>
#include <cstdlib>
#include <cassert>
#include <cstring>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include <cbc.hpp>


// DESTROYER
CBC::~CBC() {}; 

// Constructor..
CBC::CBC ( int  microsteps, int  steppingFrequency, bool highCurrentMode, bool driveSR, int adcReadDelay, int defaultADCSamples, int usbEnable, int driveEnable) 
{
    gpio.ConfigureAll();
    mcb.powerUpBase();

    /* Microsteps */
    driver.setMicrosteps(microsteps); 

    /* Set Stepping Frequency */
    driver.setSteppingFrequency(steppingFrequency); 

    /* High Current Mode */
    if (highCurrentMode)
        driver.enableHighCurrent();
    else
        driver.disableHighCurrent();

    /* Drive SR */
    if (driveSR)
        driver.enableSR();
    else 
        driver.disableSR(); 

    /* ADC Read Delay */
    adc.setReadDelay(adcReadDelay); 

    /* ADC Number of Samples */
    adc.setDefaultSamples(defaultADCSamples); 

    /* Turn on Ethernet Dongle */
    usb.enableEthernet(); 

    for (int i=0; i<6; i++) 
    {
        if ((usbEnable >> i) & 0x1)
            usb.enable(i+1);
        else
            usb.disable(i+1);
    }

    for (int i=0; i<6; i++) 
    {
        if ((driveEnable >> i) & 0x1)
            driver.enable(i);
        else
            driver.disable(i);
    }
}

void CBC::powerUp() {
    mcb.powerUpBase(); 
}
void CBC::powerDown() {
    mcb.powerDownBase(); 
}

//===========================================================================
//=USB Control===============================================================
//===========================================================================

void CBC::USB::enable(int iusb)
{
    if((iusb<1)||(iusb>6))
        return; 
    mcb.powerUpUSB(iusb);
}

void CBC::USB::disable(int iusb)
{
    if((iusb<1)||(iusb>6))
        return; 
    mcb.powerDownUSB(iusb);
}

bool CBC::USB::isEnabled (int iusb) {
    return (mcb.isUSBPoweredUp(iusb)); 
}

void CBC::USB::enableEthernet() {
    mcb.powerUpUSB(0); 
}

void CBC::USB::disableEthernet() {
    mcb.powerDownUSB(0); 
}

void CBC::USB::resetEthernet() { 
    disableEthernet(); 
    sleep(1); 
    enableEthernet(); 
}

//===========================================================================
//=driver Driver Control======================================================
//===========================================================================

void CBC::Driver::setMicrosteps(int usint)
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
            return; 
    }
    mcb.setUStep(us);
}

int CBC::Driver::getMicrosteps() {
    return (mcb.getUStep());
}

void CBC::Driver::enable(int idrive)
{
    // whine if drive is out of bounds
    if ((idrive<0)||(idrive>5))
        return; 

    //enable drive
    mcb.enableDrive(idrive);
}

void CBC::Driver::disable(int idrive)
{
    // whine if drive is out of bounds
    if ((idrive<0)||(idrive>5))
        return; 

    //disable drive
    mcb.disableDrive(idrive);
}

bool CBC::Driver::isEnabled(int idrive) {
    return (mcb.isDriveEnabled(idrive)); 
}

void CBC::Driver::sleep() {
    mcb.powerDownDriveControllers(); 
}

void CBC::Driver::wakeup() {
    mcb.powerUpDriveControllers(); 
}

bool CBC::Driver::isAwake() {
    MirrorControlBoard mcb; 
    return(mcb.isDriveControllersPoweredUp()); 
}

bool CBC::Driver::isHighCurrentEnabled() { 
    MirrorControlBoard mcb; 
    return(mcb.isDriveHiCurrentEnabled());
}

void CBC::Driver::enableHighCurrent () { 
    mcb.enableDriveHiCurrent(); 
}

void CBC::Driver::disableHighCurrent () { 
    mcb.disableDriveHiCurrent(); 
}

bool CBC::Driver::isSREnabled() {
    return(mcb.isDriveSREnabled()); 
}

void CBC::Driver::enableSR() {
    mcb.enableDriveSR(); 
}

void CBC::Driver::disableSR() {
    mcb.disableDriveSR(); 
}

int CBC::Driver::getSteppingFrequency () {
    return(steppingFrequency); 
}

void CBC::Driver::setSteppingFrequency (int frequency) {
    if (frequency > maximumSteppingFrequency)
        steppingFrequency = maximumSteppingFrequency; 
    else if (frequency < minimumSteppingFrequency)
        steppingFrequency = minimumSteppingFrequency; 
    else 
        steppingFrequency = frequency; 
}

void CBC::Driver::reset() {
    mcb.setPhaseZeroOnAllDrives();
}

void CBC::Driver::step(int idrive, int nsteps) {
    step(idrive, nsteps, steppingFrequency); 
}
void CBC::Driver::step(int idrive, int nsteps, int frequency)
{
    /* Give this thread higher priority to improve timing stability */
    pthread_t this_thread = pthread_self(); 
    struct sched_param params; 
    params.sched_priority = sched_get_priority_max(SCHED_FIFO); 
    pthread_setschedparam(this_thread, SCHED_FIFO, &params);

    /* whine if invalid actuator number is used */
    if ((idrive<0)||(idrive>5))
        return; 

    // if nstep > 0, extend. If nstep < 0, retract
    MirrorControlBoard::Dir dir = MirrorControlBoard::DIR_EXTEND;
    if (nsteps<0)
        dir = MirrorControlBoard::DIR_RETRACT, nsteps=-nsteps;

    int microsteps = nsteps * getMicrosteps(); 

    // loop over number of micro Steps
    for (unsigned istep=0; istep<unsigned(microsteps); istep++)
    {
        mcb.stepOneDrive(idrive, dir, frequency);
        mcb.waitHalfPeriod(frequency);
    }
}

void CBC::Driver::stepAll(int nstep1, int nstep2, int nstep3, int nstep4, int nstep5, int nstep6)
{
    stepAll(nstep1, nstep2, nstep3, nstep4, nstep5, nstep6, steppingFrequency); 
}
void CBC::Driver::stepAll(int nstep1, int nstep2, int nstep3, int nstep4, int nstep5, int nstep6, int frequency)
{
    // loop while theres still some stepping to do..
    int microsteps = getMicrosteps(); 

    int microsteps1 = nstep1 * microsteps; 
    int microsteps2 = nstep2 * microsteps; 
    int microsteps3 = nstep3 * microsteps; 
    int microsteps4 = nstep4 * microsteps; 
    int microsteps5 = nstep5 * microsteps; 
    int microsteps6 = nstep6 * microsteps; 

    while(microsteps1!=0 || microsteps2!=0 || microsteps3!=0 || microsteps4!=0 || microsteps5!=0 || microsteps6!=0)
    {
        //default dir is none
        MirrorControlBoard::Dir dir1 = MirrorControlBoard::DIR_NONE;
        MirrorControlBoard::Dir dir2 = MirrorControlBoard::DIR_NONE;
        MirrorControlBoard::Dir dir3 = MirrorControlBoard::DIR_NONE;
        MirrorControlBoard::Dir dir4 = MirrorControlBoard::DIR_NONE;
        MirrorControlBoard::Dir dir5 = MirrorControlBoard::DIR_NONE;
        MirrorControlBoard::Dir dir6 = MirrorControlBoard::DIR_NONE;

        // driver 1 direction
        if (nstep1 > 0)
            dir1 = MirrorControlBoard::DIR_EXTEND, microsteps1--;
        else if (microsteps1 < 0)
            dir1 = MirrorControlBoard::DIR_RETRACT, microsteps1++;

        // driver 2 direction
        if (microsteps2 > 0)
            dir2 = MirrorControlBoard::DIR_EXTEND, microsteps2--;
        else if (microsteps2 < 0)
            dir2 = MirrorControlBoard::DIR_RETRACT, microsteps2++;

        // driver 3 direction
        if (microsteps3 > 0)
            dir3 = MirrorControlBoard::DIR_EXTEND, microsteps3--;
        else if (microsteps3 < 0)
            dir3 = MirrorControlBoard::DIR_RETRACT, microsteps3++;

        // driver 4 direction
        if (microsteps4 > 0)
            dir4 = MirrorControlBoard::DIR_EXTEND, microsteps4--;
        else if (microsteps4 < 0)
            dir4 = MirrorControlBoard::DIR_RETRACT, microsteps4++;

        // driver 5 direction
        if (microsteps5 > 0)
            dir5 = MirrorControlBoard::DIR_EXTEND, microsteps5--;
        else if (microsteps5 < 0)
            dir5 = MirrorControlBoard::DIR_RETRACT, microsteps5++;

        // driver 6 direction
        if (microsteps6 > 0)
            dir6 = MirrorControlBoard::DIR_EXTEND, microsteps6--;
        else if (microsteps6 < 0)
            dir6 = MirrorControlBoard::DIR_RETRACT, microsteps6++;

        // step all drives once in specified directions
        mcb.stepAllDrives(dir1, dir2, dir3, dir4, dir5, dir6, frequency);
        // some delay
        mcb.waitHalfPeriod(frequency);
    }
}

void CBC::Driver::slew(int idrive, MirrorControlBoard::Dir dir)
{
    slew (idrive, dir, steppingFrequency); 
}
void CBC::Driver::slew(int idrive, MirrorControlBoard::Dir dir, int frequency)
{
    // do nothing
    if ((idrive<0)||(idrive>5))
        return; 

    // Loop forever while stepping the driver
    while(1)
    {
        mcb.stepOneDrive(idrive, dir, frequency);
        mcb.waitHalfPeriod(frequency);
    }
}


void CBC::Driver::slewAll(MirrorControlBoard::Dir dir)
{
    slewAll(dir, steppingFrequency); 
}
void CBC::Driver::slewAll(MirrorControlBoard::Dir dir, int frequency)
{
    while(1)
    {
        mcb.stepAllDrives(dir, dir, dir, dir, dir, dir);
        mcb.waitHalfPeriod(frequency);
    }
}

//===========================================================================
//=Encoder Control===========================================================
//===========================================================================

void CBC::Encoder::enable() {
    mcb.powerUpEncoders(); 
}

void CBC::Encoder::disable() {
    mcb.powerDownEncoders(); 
}

bool CBC::Encoder::isEnabled() {
    return (mcb.isEncodersPoweredUp()); 
}

//===========================================================================
//=ADC Control/Readout=======================================================
//===========================================================================

adcData CBC::ADC::readEncoder ( int encoder) {
    return(readEncoder(encoder,defaultSamples));
}

adcData CBC::ADC::readEncoder ( int encoder, int nsamples) {
    return(measure(0,encoder,nsamples));
}

adcData CBC::ADC::onboardTemp () {
    return(onboardTemp(defaultSamples));
}

adcData CBC::ADC::onboardTemp (int nsamples) {
    return(measure(0,6,nsamples));
}

adcData CBC::ADC::externalTemp () {
    return(externalTemp(defaultSamples)); 
}

adcData CBC::ADC::externalTemp (int nsamples) {
    return(measure(0,7,nsamples));
}

int  CBC::ADC::getReadDelay() {
    return (readDelay); 
}

void CBC::ADC::setReadDelay(int delay) {
    readDelay = delay; 
}

adcData CBC::ADC::measure(int adc, int channel, int nsamples) {
    uint32_t mean;
    uint32_t stddev;

    mcb.measureADCStat(adc, channel, nsamples, mean, stddev); 

    adcData data; 
    data.voltage = tlcadc.voltData(mean,   volt_full);
    data.stddev  = tlcadc.voltData(stddev, volt_full);

    return (data); 
}

void CBC::ADC::setDefaultSamples(int nsamples) {
    defaultSamples = nsamples; 
}

//===========================================================================
//=Sensor Control============================================================
//===========================================================================

void CBC::Sensor::enable() {
    mcb.powerUpSensors(); 
}

void CBC::Sensor::disable() {
    mcb.powerDownSensors(); 
}

bool CBC::Sensor::isEnabled() {
    return(mcb.isSensorsPoweredUp());
}
