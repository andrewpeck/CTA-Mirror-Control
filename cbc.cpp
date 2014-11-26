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
#include <math.h>


// DESTROYER
CBC::~CBC() {};

// Constructor..
CBC::CBC ( int  microsteps, int  steppingFrequency, bool highCurrentMode, bool driveSR, int adcReadDelay, int defaultADCSamples, int usbEnable, int driveEnable)
{
    gpio.ConfigureAll();

    powerUp();

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

    for (int i=1; i<7; i++)
    {
        if ((usbEnable >> i) & 0x1)
            usb.enable(i);
        else
            usb.disable(i);
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
    // turn on level shifters
    mcb.enableIO();

    driver.wakeup();
    driver.reset();
    encoder.enable();
    auxSensor.enable();

    //gpio.WriteLevel(layout.igpioADCSel1, 1);
    //gpio.WriteLevel(layout.igpioADCSel2, 1);
    //spi.Configure();
    //spi.WriteRead(0x0000);

    mcb.initializeADC(0);
    mcb.initializeADC(1);
}

void CBC::powerDown() {
    driver.disableAll();
    driver.sleep();
    encoder.disable();
    auxSensor.disable();
    usb.disableAll();
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

void CBC::USB::enableAll()
{
    for (int i=1; i<7; i++)
        enable(i);
}

void CBC::USB::disable(int iusb)
{
    if((iusb<1)||(iusb>6))
        return;
    mcb.powerDownUSB(iusb);
}

void CBC::USB::disableAll()
{
    for (int i=1; i<7; i++)
        enable(i);
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
//=Motor Driver Control======================================================
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

void CBC::Driver::enable(int drive)
{
    // whine if drive is out of bounds
    if ((drive<1)||(drive>6))
        return;

    //enable drive
    mcb.enableDrive(drive-1); //MCB counts from zero
}

void CBC::Driver::disable(int drive)
{
    // whine if drive is out of bounds
    if ((drive<1)||(drive>6))
        return;

    //disable drive
    mcb.disableDrive(drive-1); //MCB counts from zero
}

void CBC::Driver::disableAll()
{
    for (int i=1; i<7; i++)
        disable(i-1);
}

bool CBC::Driver::isEnabled(int drive) {
    return (mcb.isDriveEnabled(drive));
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

int CBC::Driver::calibrateSteppingFrequency (int nsteps) {
    timespec ts;

    //begin time
    clock_gettime(CLOCK_MONOTONIC, &ts);
    long starttime = ts.tv_nsec;

    step(1, nsteps, steppingFrequency, true);

    //finish time
    clock_gettime(CLOCK_MONOTONIC, &ts);
    long endtime = ts.tv_nsec;

    int measured_time = endtime - starttime;
    int expected_time = (1000000000 * nsteps ) / steppingFrequency; //nanoseconds

    float correction = (1.0 * measured_time) / expected_time;

    int constant_new = static_cast <int> (mcb.getCalibrationConstant() * correction);
    mcb.setCalibrationConstant(constant_new);

    return(constant_new);
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

int CBC::Driver::step(int drive, int nsteps)
{
    return(step(drive, nsteps, steppingFrequency));
}

int CBC::Driver::step(int drive, int nsteps, int frequency)
{
    int calibrationFrequency = calibrateSteppingFrequency();
    step(drive, nsteps, frequency, false);
    return (calibrationFrequency);
}

void CBC::Driver::step(int drive, int nsteps, int frequency, bool dummy)
{
    /* Give this thread higher priority to improve timing stability */
    pthread_t this_thread = pthread_self();
    struct sched_param params;
    params.sched_priority = sched_get_priority_max(SCHED_FIFO);
    pthread_setschedparam(this_thread, SCHED_FIFO, &params);

    /* whine if invalid actuator number is used */
    if ((drive<1)||(drive>6))
        return;
    drive = drive - 1;

    // if nstep > 0, extend. If nstep < 0, retract
    MirrorControlBoard::Dir dir = MirrorControlBoard::DIR_EXTEND;
    if (nsteps<0)
        dir = MirrorControlBoard::DIR_RETRACT, nsteps=-nsteps;

    if (dummy==true)
        dir = MirrorControlBoard::DIR_NONE;

    int microsteps = nsteps * getMicrosteps();

    // loop over number of micro Steps
    for (unsigned istep=0; istep<unsigned(microsteps); istep++)
    {
        mcb.stepOneDrive(drive, dir, frequency);
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

CBC::ADC::adcData CBC::ADC::readEncoder ( int encoder) {
    return(readEncoder(encoder,defaultSamples));
}

CBC::ADC::adcData CBC::ADC::readEncoder ( int encoder, int nsamples) {
    return(measure(0,encoder,nsamples));
}

CBC::ADC::adcData CBC::ADC::readOnboardTemp () {
    return(readOnboardTemp(defaultSamples));
}

CBC::ADC::adcData CBC::ADC::readOnboardTemp (int nsamples) {
    return(measure(0,6,nsamples));
}

CBC::ADC::adcData CBC::ADC::readExternalTemp () {
    return(readExternalTemp(defaultSamples));
}

CBC::ADC::adcData CBC::ADC::readExternalTemp (int nsamples) {
    return(measure(0,7,nsamples));
}

int  CBC::ADC::getReadDelay() {
    return (readDelay);
}

void CBC::ADC::setReadDelay(int delay) {
    readDelay = delay;
}

CBC::ADC::adcData CBC::ADC::measure(int adc, int channel, int nsamples) {
    uint32_t sum;
    uint64_t sumsq;
    uint32_t min;
    uint32_t max;

    mcb.measureADCStat(adc, channel, nsamples, sum, sumsq, min, max);

    uint32_t mean   = sum/nsamples;
    uint32_t var    = (sumsq - sum*sum / nsamples) / nsamples;
    uint32_t stddev = sqrt(var);

    adcData data;
    data.voltage      = tlcadc.voltData(mean,                  volt_full);
    data.stddev       = tlcadc.voltData(stddev,                volt_full);
    data.voltageMin   = tlcadc.voltData(min,                   volt_full);
    data.voltageMax   = tlcadc.voltData(max,                   volt_full);
    data.voltageError = tlcadc.voltData(stddev/sqrt(nsamples), volt_full);

    return (data);
}

void CBC::ADC::setDefaultSamples(int nsamples) {
    defaultSamples = nsamples;
}

//===========================================================================
//=Sensor Control============================================================
//===========================================================================

void CBC::auxSensor::enable() {
    mcb.powerUpSensors();
}

void CBC::auxSensor::disable() {
    mcb.powerDownSensors();
}

bool CBC::auxSensor::isEnabled() {
    return(mcb.isSensorsPoweredUp());
}
