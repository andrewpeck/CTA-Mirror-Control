////////////////////////////////////////////////////////////////////////////////
// MirrorControlBoard.cpp - Class which implements useful mid-level 
//                          functionality for the mirror control board.
////////////////////////////////////////////////////////////////////////////////

#include <cassert>
#include <SpiInterface.hpp>
#include <MirrorControlBoard.hpp>
#include <TLCX5XX_ADC.hpp>

typedef TLC3548 ADC; 

//Overo MirrorControlBoard::m_sys; 
//Layout MirrorControlBoard::layout; 

MirrorControlBoard::MirrorControlBoard(bool no_initialize, unsigned nusb): m_nusb(nusb>7?7:nusb) {
    printf("Initializing Board..."); 
    if(!no_initialize) {
        printf("Powering down all chips..."); 
        powerDownAll();
    }
    else {
        return; 
        //if(m_sys.clockIsEnabledSSP(ADC_SSP))
        //m_sys.sspFlush(ADC_SSP);
    }
}

MirrorControlBoard::~MirrorControlBoard() { 
}

void MirrorControlBoard::initialize(const unsigned ssp_clk_div) {
    setUStep(USTEP_8);
    enableDriveSR();
    disableDriveHiCurrent();
    m_sys.gpioWriteLevel(layout.igpioReset(),1);
    initializeSPI();
}

void MirrorControlBoard::initializeSPI() {
    //void spiConfigure (int issp, unsigned clk_phase, bool clk_polarity, bool clk_div, bool epol, int word_length) {
    //for (int issp=0; issp<=8; issp++) { m_sys.spiConfigure (issp, 0, 0, 4, 1, 0xF); }
    //printf("Enabling SPI");
    //m_sys.spiEnable(ADC_SPI);                      
    //printf("Configuring SPI");
    //m_sys.spiConfigure (ADC_SPI, 0, 0, 4, 1, 0xF);
    //m_sys.spiFlush (ADC_SPI);    
}

void MirrorControlBoard::powerDownAll() {
    powerDownAllUSB();
    powerDownBase();
}

void MirrorControlBoard::powerUpAll() {
    powerUpBase();
    powerUpAllUSB();
}

void MirrorControlBoard::powerUpBase() {

    // Power up the A3977 chips
    powerUpDriveControllers();

    // Power up encoders
    powerUpEncoders();

    // Power up ADCs
    powerUpADCs();

    // Power up SSP clock and initialize port
    //initializeSPI();

    // Initialize on-board ADC
    initializeADC(0);
    initializeADC(1);
}

void MirrorControlBoard::powerDownBase() {
    // Set on-board ADC into sleep mode
    selectADC(7);
    m_sys.spiWriteRead(ADC::codeSWPowerDown());

    // Set PWM state
    //m_sys.pwmSetDutyCycle(0, 0, false); 
    //m_sys.pwmSetDutyCycle(1, 0, false); 
    //m_sys.pwmSetDutyCycle(2, 0, false); 
    //m_sys.pwmSetDutyCycle(3, 0, false); 
    //m_sys.clockDisablePWM();

    // Power down ADCs
    powerDownADCs();

    // Power down encoders
    powerDownEncoders();

    // Disable all drives, clear high current bit and power down A3977
    disableAllDrives();
    disableDriveHiCurrent();
    powerDownDriveControllers();
}

void MirrorControlBoard::powerDownAllUSB() {
    for(unsigned iusb=0;iusb<m_nusb;iusb++)
        powerDownUSB(iusb);
}

void MirrorControlBoard::powerUpAllUSB() {
    for(unsigned iusb=0;iusb<m_nusb;iusb++)
        powerUpUSB(iusb);
}


// Set the USB power enable_bar bit
void MirrorControlBoard::powerDownUSB(unsigned iusb) {
    m_sys.gpioWriteLevel(layout.igpioUSBOff(iusb),1);
}

// Clear the USB power enable_bar bit
void MirrorControlBoard::powerUpUSB(unsigned iusb) {
    m_sys.gpioWriteLevel(layout.igpioUSBOff(iusb),0);
}

bool MirrorControlBoard::isUSBPoweredUp(unsigned iusb) {
    return m_sys.gpioReadLevel(layout.igpioUSBOff(iusb))?false:true;
}

void MirrorControlBoard::powerDownDriveControllers() {
    m_sys.gpioWriteLevel(layout.igpioSleep(),0);
}

void MirrorControlBoard::powerUpDriveControllers() {
    m_sys.gpioWriteLevel(layout.igpioSleep(),1);
}

bool MirrorControlBoard::isDriveControllersPoweredUp() {
    return m_sys.gpioReadLevel(layout.igpioSleep())?true:false;
}

void MirrorControlBoard::powerDownEncoders() {
    m_sys.gpioWriteLevel(layout.igpioEncoderEnable(),0);
}

void MirrorControlBoard::powerUpEncoders() {
    m_sys.gpioWriteLevel(layout.igpioEncoderEnable(),1);
}

bool MirrorControlBoard::isEncodersPoweredUp() {
    return m_sys.gpioReadLevel(layout.igpioEncoderEnable())?true:false;
}

void MirrorControlBoard::powerUpADCs() {
    m_sys.gpioWriteLevel(layout.igpioPowerADC(),1);
}

void MirrorControlBoard::powerDownADCs() {
    m_sys.gpioWriteLevel(layout.igpioPowerADC(),0);
}

bool MirrorControlBoard::isADCsPoweredUp() {
    return m_sys.gpioReadLevel(layout.igpioPowerADC())?false:true;
}

// ----------------------------------------------------------------------------
// Drives
// ----------------------------------------------------------------------------

void MirrorControlBoard::enableDriveSR(bool enable) {
    m_sys.gpioWriteLevel(layout.igpioSR(), enable?0:1);
}

bool MirrorControlBoard::isDriveSREnabled() {
    return m_sys.gpioReadLevel(layout.igpioSR())?false:true;
}

void MirrorControlBoard::setUStep(UStep ustep) {
    unsigned mslog2 = 0;
    switch(ustep)
    {
        case USTEP_1: mslog2 = 0x0; break;
        case USTEP_2: mslog2 = 0x1; break;
        case USTEP_4: mslog2 = 0x2; break;
        case USTEP_8: mslog2 = 0x3; break;
    }
    m_sys.gpioWriteLevel(layout.igpioMS1(),mslog2&0x1);
    m_sys.gpioWriteLevel(layout.igpioMS2(),mslog2&0x2);
}

MirrorControlBoard::UStep MirrorControlBoard::getUStep() {
    if(m_sys.gpioReadLevel(layout.igpioMS2()))
        return m_sys.gpioReadLevel(layout.igpioMS1())?USTEP_8:USTEP_4;
    else
        return m_sys.gpioReadLevel(layout.igpioMS1())?USTEP_2:USTEP_1;
}

// Steps motor a single step in a direction specified by dir, with some delay controlling the io speed
void MirrorControlBoard:: stepOneDrive(unsigned idrive, Dir dir, unsigned ndelayloop) {
    // Write Direction to the DIR pin
    m_sys.gpioWriteLevel(layout.igpioDir(idrive),(dir==DIR_RETRACT)?1:0);

    // Writes one step to STEP pin
    unsigned igpio = layout.igpioStep(idrive);
    m_sys.gpioWriteLevel(igpio,(dir==DIR_NONE)?0:1);

    // a delay
    loopDelay(ndelayloop);

    // Toggle pin back to low
    m_sys.gpioWriteLevel(igpio,0);
}


// Steps all drives in a configurable direction... should be replaced with a loop over stepOneDrive
void MirrorControlBoard::stepAllDrives(Dir dr1_dir, Dir dr2_dir, Dir dr3_dir, Dir dr4_dir, Dir dr5_dir, Dir dr6_dir, unsigned ndelayloop) {
    m_sys.gpioWriteLevel(layout.igpioDir1(),(dr1_dir==DIR_RETRACT)?1:0);
    m_sys.gpioWriteLevel(layout.igpioDir2(),(dr2_dir==DIR_RETRACT)?1:0);
    m_sys.gpioWriteLevel(layout.igpioDir3(),(dr3_dir==DIR_RETRACT)?1:0);
    m_sys.gpioWriteLevel(layout.igpioDir4(),(dr4_dir==DIR_RETRACT)?1:0);
    m_sys.gpioWriteLevel(layout.igpioDir5(),(dr5_dir==DIR_RETRACT)?1:0);
    m_sys.gpioWriteLevel(layout.igpioDir6(),(dr6_dir==DIR_RETRACT)?1:0);

    m_sys.gpioWriteLevel(layout.igpioStep1(),(dr1_dir==DIR_NONE)?0:1);
    m_sys.gpioWriteLevel(layout.igpioStep2(),(dr2_dir==DIR_NONE)?0:1);
    m_sys.gpioWriteLevel(layout.igpioStep3(),(dr3_dir==DIR_NONE)?0:1);
    m_sys.gpioWriteLevel(layout.igpioStep4(),(dr4_dir==DIR_NONE)?0:1);
    m_sys.gpioWriteLevel(layout.igpioStep5(),(dr5_dir==DIR_NONE)?0:1);
    m_sys.gpioWriteLevel(layout.igpioStep6(),(dr6_dir==DIR_NONE)?0:1);

    loopDelay(ndelayloop);

    m_sys.gpioWriteLevel(layout.igpioStep1(),0);
    m_sys.gpioWriteLevel(layout.igpioStep2(),0);
    m_sys.gpioWriteLevel(layout.igpioStep3(),0);
    m_sys.gpioWriteLevel(layout.igpioStep4(),0);
    m_sys.gpioWriteLevel(layout.igpioStep5(),0);
    m_sys.gpioWriteLevel(layout.igpioStep6(),0);
}

void MirrorControlBoard::setPhaseZeroOnAllDrives() {
    m_sys.gpioWriteLevel(layout.igpioReset(),0);
    loopDelay(1000);
    m_sys.gpioWriteLevel(layout.igpioReset(),1);
}

void MirrorControlBoard::enableAllDrives(bool enable) {
    bool ienable = enable?0:1;
    m_sys.gpioWriteLevel(layout.igpioEnable1(), ienable);
    m_sys.gpioWriteLevel(layout.igpioEnable2(), ienable);
    m_sys.gpioWriteLevel(layout.igpioEnable3(), ienable);
    m_sys.gpioWriteLevel(layout.igpioEnable4(), ienable);
    m_sys.gpioWriteLevel(layout.igpioEnable5(), ienable);
    m_sys.gpioWriteLevel(layout.igpioEnable6(), ienable);
}

// Enable/Disable Stepper Motors
void MirrorControlBoard::enableDrive(unsigned idrive, bool enable) {
    m_sys.gpioWriteLevel(layout.igpioEnable(idrive), enable?0:1);
}

bool MirrorControlBoard::isDriveEnabled(unsigned idrive) {
    return m_sys.gpioReadLevel(layout.igpioEnable(idrive))?false:true;
}

void MirrorControlBoard::enableDriveHiCurrent(bool enable)
{
    m_sys.gpioWriteLevel(layout.igpioPwrIncBar(), enable?0:1);
}

bool MirrorControlBoard::isDriveHiCurrentEnabled() {
    return m_sys.gpioReadLevel(layout.igpioPwrIncBar())?false:true;
}

// ----------------------------------------------------------------------------
// ADCs
// ----------------------------------------------------------------------------

void MirrorControlBoard::initializeADC(unsigned iadc) {
    selectADC(iadc);                                        // Assert Chip Select for ADC in question
    spi.WriteRead(ADC::codeInitialize()); 
    spi.WriteRead(ADC::codeConfig()); 

    //m_sys.spi::WriteRead (ADC_SPI, ADC::codeInitialize());   // Initialize ADC
    //m_sys.spi::WriteRead (ADC_SPI, ADC::codeConfig());       // Configure ADC
    //m_sys.sspTestWritePollRead(ADC_SSP, ADC::codeInitialize());
    //m_sys.sspTestWritePollRead(ADC_SSP, ADC::codeConfig());
}

void MirrorControlBoard::selectADC(unsigned iadc) {
    m_sys.gpioWriteLevel(layout.igpioADCSel1(), iadc==0?1:0);
    m_sys.gpioWriteLevel(layout.igpioADCSel2(), iadc==1?1:0);
}

uint32_t MirrorControlBoard::measureADC(unsigned iadc, unsigned ichan, unsigned ndelayloop) {
    // Assert Chip Select
    selectADC(iadc);

    // ADC Channel Select
    uint32_t code = ADC::codeSelect(ichan);
    spi.WriteRead(code); 

    // some delay
    loopDelay(ndelayloop);

    // Read ADC
    uint32_t datum = spi.WriteRead(ADC::codeReadFIFO()); 

    return ADC::decodeUSB(datum);
}

void MirrorControlBoard::measureManyADC(uint32_t* data, unsigned iadc, unsigned zchan, unsigned nchan, unsigned ndelayloop) {
    // adc chip select
    selectADC(iadc);
    uint32_t datum; 

    // loop over adc channels
    for (unsigned ichan=0; ichan<nchan; ichan++) {
        // read data
        datum = spi.WriteRead(ADC::codeSelect(zchan+ichan));

        if(ichan>0)
            data[ichan-1] = ADC::decodeUSB(datum);

        // some delay
        loopDelay(ndelayloop);
    }

    datum = spi.WriteRead(ADC::codeReadFIFO());
    if(nchan>0)
        data[nchan-1] = ADC::decodeUSB(datum);
}

uint32_t MirrorControlBoard::measureADCWithBurn(unsigned iadc, unsigned ichan, unsigned ndelayloop) {
    selectADC(iadc);
    uint32_t code = ADC::codeSelect(ichan);
    spi.WriteRead(code); 
    loopDelay(ndelayloop);
    spi.WriteRead(code); 
    loopDelay(ndelayloop);
    uint32_t datum = spi.WriteRead(ADC::codeReadFIFO());

    return ADC::decodeUSB(datum);
}

void MirrorControlBoard::measureManyADCWithBurn(uint32_t* data, unsigned iadc, unsigned zchan, unsigned nchan, unsigned ndelayloop) {
    selectADC(iadc);
    for(unsigned ichan=0; ichan<nchan; ichan++) {
        uint32_t code = ADC::codeSelect(zchan+ichan);
        uint32_t datum = spi.WriteRead(code);
        if(ichan>0)
            data[ichan-1] = ADC::decodeUSB(datum);

        loopDelay(ndelayloop);
        spi.WriteRead(code);
        loopDelay(ndelayloop);
    }

    uint32_t datum = spi.WriteRead(ADC::codeReadFIFO());

    if(nchan>0)
        data[nchan-1] = ADC::decodeUSB(datum);
}

void MirrorControlBoard:: measureADCStat(unsigned iadc, unsigned ichan, unsigned nmeas, uint32_t& sum, uint64_t& sumsq, uint32_t& min, uint32_t& max, unsigned nburn, unsigned ndelayloop) {
    selectADC(iadc);
    uint32_t code = ADC::codeSelect(ichan);
    unsigned nloop = nburn + nmeas;
    sum = 0;
    sumsq = 0;
    max = 0;
    min = ~max;
    for(unsigned iloop=0;iloop<nloop;iloop++) {
        uint32_t datum = spi.WriteRead(code);
        if (iloop>nburn) {
            datum = ADC::decodeUSB(datum);
            uint64_t datum64 = datum;
            sum+=datum;
            sumsq+=datum64*datum64;
            if(datum>max)max=datum;
            if(datum<min)min=datum;
        }
        loopDelay(ndelayloop);
    }
    uint32_t datum = spi.WriteRead(ADC::codeReadFIFO());
    datum = ADC::decodeUSB(datum);
    uint64_t datum64 = datum;
    sum+=datum;
    sumsq+=datum64*datum64;

    if(datum>max)
        max=datum;
    if(datum<min)
        min=datum;
}

void MirrorControlBoard::measureADC(unsigned iadc, unsigned ichan, unsigned nmeas, std::vector<uint32_t>& vMeas, unsigned ndelayloop) {
    selectADC(iadc);
    uint32_t code = ADC::codeSelect(ichan);
    unsigned nloop = nmeas;
    vMeas.resize(nmeas);

    for(unsigned iloop=0; iloop <= nloop; iloop++) {
        uint32_t datum;
        if(iloop == nloop)
            datum = spi.WriteRead(ADC::codeReadFIFO());
        else
            datum = spi.WriteRead(code);

        if(iloop == 0)
            continue;

        datum = ADC::decodeUSB(datum);
        vMeas[iloop - 1] = datum;

        loopDelay(ndelayloop);
    }
}

void MirrorControlBoard::loopDelay(unsigned nloop) { 
    for(volatile unsigned iloop=0;iloop<nloop;iloop++);
}
