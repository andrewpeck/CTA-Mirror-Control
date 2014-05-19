//-*-mode:c++; mode:font-lock;-*-

/*! \file MirrorControlBoard.cpp

  Class which implements useful mid-level functionality for the mirror
  control board.

  \author     Stephen Fegan               \n
  UCLA                        \n
  sfegan@astro.ucla.edu       \n

  \version    1.0
  \date       08/12/2008
  */

#include <cassert>

#include <MirrorControlBoard.hpp>

MirrorControlBoard::MirrorControlBoard(Sys* sys, bool no_initialize, unsigned nusb, unsigned ssp_clk_div): m_sys(sys), m_nusb(nusb>7?7:nusb), m_ssp_clk_div(ssp_clk_div) {
    if(!no_initialize)
    {
        //initialize(m_ssp_clk_div);
        powerDownAll();
    }
    else
    {
        return; 
        //if(m_sys->clockIsEnabledSSP(ADC_SSP))
            //m_sys->sspFlush(ADC_SSP);
    }
}

void MirrorControlBoard::initialize(const unsigned ssp_clk_div) {
    setUStep(USTEP_8);
    enableDriveSR();
    disableDriveHiCurrent();
    m_sys->gpioWriteLevel(LO::igpioReset(),1);
    //initializeSSP(ssp_clk_div);
}

//void MirrorControlBoard::initializeSSP(const unsigned ssp_clk_div) {
//    m_sys->sspConfigure(ADC_SSP,ssp_clk_div,16,m_sys->SSP_FF_SPI,SSP_FLAG_NONE |SSP_FLAG_SPH |SSP_FLAG_MASK_INT_TX_UNDERRUN |SSP_FLAG_MASK_INT_RX_OVERRUN);
//    m_ssp_clk_div = ssp_clk_div;
//    m_sys->sspEnable(ADC_SSP);
//    m_sys->sspFlush(ADC_SSP);
//}

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
    //initializeSSP(m_ssp_clk_div);

    // Initialize on-board ADC
    //initializeADC(0);
    //initializeADC(1);
}

void MirrorControlBoard::powerDownBase() {
    // Set on-board ADC into sleep mode
    //if(m_sys->clockIsEnabledSSP(ADC_SSP))
    //{
    //    selectADC(7);
    //    m_sys->sspTestWritePollRead(ADC_SSP, ADC::codeSWPowerDown());
    //}

    // Set PWM state
    //m_sys->pwmSetDutyCycle(0, 0, false); 
    //m_sys->pwmSetDutyCycle(1, 0, false); 
    //m_sys->pwmSetDutyCycle(2, 0, false); 
    //m_sys->pwmSetDutyCycle(3, 0, false); 
    //m_sys->clockDisablePWM();

    // Power down SSP clocks
    //m_sys->clockDisableSSP1();
    //m_sys->clockDisableSSP2();
    //m_sys->clockDisableSSP3();

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
    for(unsigned iusb=0;iusb<m_nusb;iusb++)powerDownUSB(iusb);
}

void MirrorControlBoard::powerUpAllUSB() {
    for(unsigned iusb=0;iusb<m_nusb;iusb++)powerUpUSB(iusb);
}

void MirrorControlBoard::powerDownUSB(unsigned iusb) {
    // Set the USB power enable_bar bit
    m_sys->gpioWriteLevel(LO::igpioUSBOff(iusb),1);
}

void MirrorControlBoard::powerUpUSB(unsigned iusb) {
    // Clear the USB power enable_bar bit
    m_sys->gpioWriteLevel(LO::igpioUSBOff(iusb),0);
}

bool MirrorControlBoard::isUSBPoweredUp(unsigned iusb) const {
    return m_sys->gpioReadLevel(LO::igpioUSBOff(iusb))?false:true;
}

void MirrorControlBoard::powerDownDriveControllers() {
    m_sys->gpioWriteLevel(LO::igpioSleep(),0);
}

void MirrorControlBoard::powerUpDriveControllers() {
    m_sys->gpioWriteLevel(LO::igpioSleep(),1);
}

bool MirrorControlBoard::isDriveControllersPoweredUp() const {
    return m_sys->gpioReadLevel(LO::igpioSleep())?true:false;
}

void MirrorControlBoard::powerDownEncoders() {
    m_sys->gpioWriteLevel(LO::igpioEncoderEnable(),0);
}

void MirrorControlBoard::powerUpEncoders() {
    m_sys->gpioWriteLevel(LO::igpioEncoderEnable(),1);
}

bool MirrorControlBoard::isEncodersPoweredUp() const {
    return m_sys->gpioReadLevel(LO::igpioEncoderEnable())?true:false;
}

void MirrorControlBoard::powerUpADCs() {
    m_sys->gpioWriteLevel(LO::igpioPowerADC(),1);
}

void MirrorControlBoard::powerDownADCs() {
    m_sys->gpioWriteLevel(LO::igpioPowerADC(),0);
}

bool MirrorControlBoard::isADCsPoweredUp() const {
    return m_sys->gpioReadLevel(LO::igpioPowerADC())?false:true;
}

// ----------------------------------------------------------------------------
// Drives
// ----------------------------------------------------------------------------

void MirrorControlBoard::enableDriveSR(bool enable)
{
    m_sys->gpioWriteLevel(LO::igpioSR(), enable?0:1);
}

bool MirrorControlBoard::isDriveSREnabled() const
{
    return m_sys->gpioReadLevel(LO::igpioSR())?false:true;
}

void MirrorControlBoard::setUStep(UStep ustep)
{
    unsigned mslog2 = 0;
    switch(ustep)
    {
        case USTEP_1: mslog2 = 0x0; break;
        case USTEP_2: mslog2 = 0x1; break;
        case USTEP_4: mslog2 = 0x2; break;
        case USTEP_8: mslog2 = 0x3; break;
    }
    m_sys->gpioWriteLevel(LO::igpioMS1(),mslog2&0x1);
    m_sys->gpioWriteLevel(LO::igpioMS2(),mslog2&0x2);
}

MirrorControlBoard::UStep MirrorControlBoard::getUStep() const {
    if(m_sys->gpioReadLevel(LO::igpioMS2()))
        return m_sys->gpioReadLevel(LO::igpioMS1())?USTEP_8:USTEP_4;
    else
        return m_sys->gpioReadLevel(LO::igpioMS1())?USTEP_2:USTEP_1;
}

// Steps motor a single step in a direction specified by dir, with some
// controlling the io speed
void MirrorControlBoard:: stepOneDrive(unsigned idrive, Dir dir, unsigned ndelayloop) {
    // Write Direction to the DIR pin
    m_sys->gpioWriteLevel(LO::igpioDir(idrive),(dir==DIR_RETRACT)?1:0);

    // Writes one step to STEP pin
    unsigned igpio = LO::igpioStep(idrive);
    m_sys->gpioWriteLevel(igpio,(dir==DIR_NONE)?0:1);

    // a delay
    m_sys->loopDelay(ndelayloop);

    // Toggle pin back to low
    m_sys->gpioWriteLevel(igpio,0);
}


// Steps all drives in a configurable direction... should be replaced with a loop over stepOneDrive
void MirrorControlBoard::stepAllDrives(Dir dr1_dir, Dir dr2_dir, Dir dr3_dir, Dir dr4_dir, Dir dr5_dir, Dir dr6_dir, unsigned ndelayloop)
{
    m_sys->gpioWriteLevel(LO::igpioDir1(),(dr1_dir==DIR_RETRACT)?1:0);
    m_sys->gpioWriteLevel(LO::igpioDir2(),(dr2_dir==DIR_RETRACT)?1:0);
    m_sys->gpioWriteLevel(LO::igpioDir3(),(dr3_dir==DIR_RETRACT)?1:0);
    m_sys->gpioWriteLevel(LO::igpioDir4(),(dr4_dir==DIR_RETRACT)?1:0);
    m_sys->gpioWriteLevel(LO::igpioDir5(),(dr5_dir==DIR_RETRACT)?1:0);
    m_sys->gpioWriteLevel(LO::igpioDir6(),(dr6_dir==DIR_RETRACT)?1:0);

    m_sys->gpioWriteLevel(LO::igpioStep1(),(dr1_dir==DIR_NONE)?0:1);
    m_sys->gpioWriteLevel(LO::igpioStep2(),(dr2_dir==DIR_NONE)?0:1);
    m_sys->gpioWriteLevel(LO::igpioStep3(),(dr3_dir==DIR_NONE)?0:1);
    m_sys->gpioWriteLevel(LO::igpioStep4(),(dr4_dir==DIR_NONE)?0:1);
    m_sys->gpioWriteLevel(LO::igpioStep5(),(dr5_dir==DIR_NONE)?0:1);
    m_sys->gpioWriteLevel(LO::igpioStep6(),(dr6_dir==DIR_NONE)?0:1);

    m_sys->loopDelay(ndelayloop);

    m_sys->gpioWriteLevel(LO::igpioStep1(),0);
    m_sys->gpioWriteLevel(LO::igpioStep2(),0);
    m_sys->gpioWriteLevel(LO::igpioStep3(),0);
    m_sys->gpioWriteLevel(LO::igpioStep4(),0);
    m_sys->gpioWriteLevel(LO::igpioStep5(),0);
    m_sys->gpioWriteLevel(LO::igpioStep6(),0);
}

void MirrorControlBoard::setPhaseZeroOnAllDrives() {
    m_sys->gpioWriteLevel(LO::igpioReset(),0);
    m_sys->loopDelay(1000);
    m_sys->gpioWriteLevel(LO::igpioReset(),1);
}

void MirrorControlBoard::enableAllDrives(bool enable)
{
    bool ienable = enable?0:1;
    m_sys->gpioWriteLevel(LO::igpioEnable1(), ienable);
    m_sys->gpioWriteLevel(LO::igpioEnable2(), ienable);
    m_sys->gpioWriteLevel(LO::igpioEnable3(), ienable);
    m_sys->gpioWriteLevel(LO::igpioEnable4(), ienable);
    m_sys->gpioWriteLevel(LO::igpioEnable5(), ienable);
    m_sys->gpioWriteLevel(LO::igpioEnable6(), ienable);
}

void MirrorControlBoard::enableDrive(unsigned idrive, bool enable)
{
    m_sys->gpioWriteLevel(LO::igpioEnable(idrive), enable?0:1);
}

bool MirrorControlBoard::isDriveEnabled(unsigned idrive) const
{
    return m_sys->gpioReadLevel(LO::igpioEnable(idrive))?false:true;
}

void MirrorControlBoard::enableDriveHiCurrent(bool enable)
{
    m_sys->gpioWriteLevel(LO::igpioPwrIncBar(), enable?0:1);
}

bool MirrorControlBoard::isDriveHiCurrentEnabled() const
{
    return m_sys->gpioReadLevel(LO::igpioPwrIncBar())?false:true;
}

// ----------------------------------------------------------------------------
// ADCs
// ----------------------------------------------------------------------------
/*

//temporarily disabled... until spi controller works
void MirrorControlBoard::initializeADC(unsigned iadc)
{
    selectADC(iadc);
    m_sys->sspTestWritePollRead(ADC_SSP, ADC::codeInitialize());
    m_sys->sspTestWritePollRead(ADC_SSP, ADC::codeConfig());
}

void MirrorControlBoard::selectADC(unsigned iadc)
{
    m_sys->gpioWriteLevel(LO::igpioADCSel1(), iadc==0?1:0);
    m_sys->gpioWriteLevel(LO::igpioADCSel2(), iadc==1?1:0);
}

uint32_t MirrorControlBoard::
measureADC(unsigned iadc, unsigned ichan, unsigned ndelayloop)
{
    selectADC(iadc);
    uint32_t code = ADC::codeSelect(ichan);
    m_sys->sspTestWritePollRead(ADC_SSP, code);
    m_sys->loopDelay(ndelayloop);
    uint32_t datum = m_sys->sspTestWritePollRead(ADC_SSP, ADC::codeReadFIFO());
    return ADC::decodeUSB(datum);
}

void MirrorControlBoard::
measureManyADC(uint32_t* data, unsigned iadc, unsigned zchan, unsigned nchan, 
        unsigned ndelayloop)
{
    selectADC(iadc);
    for(unsigned ichan=0; ichan<nchan; ichan++)
    {
        uint32_t datum = 
            m_sys->sspTestWritePollRead(ADC_SSP, ADC::codeSelect(zchan+ichan));
        if(ichan>0)data[ichan-1] = ADC::decodeUSB(datum);
        m_sys->loopDelay(ndelayloop);
    }
    uint32_t datum = m_sys->sspTestWritePollRead(ADC_SSP, ADC::codeReadFIFO());
    if(nchan>0)data[nchan-1] = ADC::decodeUSB(datum);
}

uint32_t MirrorControlBoard::
measureADCWithBurn(unsigned iadc, unsigned ichan, unsigned ndelayloop)
{
    selectADC(iadc);
    uint32_t code = ADC::codeSelect(ichan);
    m_sys->sspTestWritePollRead(ADC_SSP, code);
    m_sys->loopDelay(ndelayloop);
    m_sys->sspTestWritePollRead(ADC_SSP, code);
    m_sys->loopDelay(ndelayloop);
    uint32_t datum = m_sys->sspTestWritePollRead(ADC_SSP, ADC::codeReadFIFO());
    return ADC::decodeUSB(datum);
}

void MirrorControlBoard::
measureManyADCWithBurn(uint32_t* data, 
        unsigned iadc, unsigned zchan, unsigned nchan, 
        unsigned ndelayloop)
{
    selectADC(iadc);
    for(unsigned ichan=0; ichan<nchan; ichan++)
    {
        uint32_t code = ADC::codeSelect(zchan+ichan);
        uint32_t datum = m_sys->sspTestWritePollRead(ADC_SSP, code);
        if(ichan>0)data[ichan-1] = ADC::decodeUSB(datum);
        m_sys->loopDelay(ndelayloop);
        m_sys->sspTestWritePollRead(ADC_SSP, code);
        m_sys->loopDelay(ndelayloop);
    }
    uint32_t datum = m_sys->sspTestWritePollRead(ADC_SSP, ADC::codeReadFIFO());
    if(nchan>0)data[nchan-1] = ADC::decodeUSB(datum);
}

void MirrorControlBoard::
measureADCStat(unsigned iadc, unsigned ichan, unsigned nmeas, 
        uint32_t& sum, uint64_t& sumsq, uint32_t& min, uint32_t& max,
        unsigned nburn, unsigned ndelayloop)
{
    selectADC(iadc);
    uint32_t code = ADC::codeSelect(ichan);
    unsigned nloop = nburn + nmeas;
    sum = 0;
    sumsq = 0;
    max = 0;
    min = ~max;
    for(unsigned iloop=0;iloop<nloop;iloop++)
    {
        uint32_t datum = m_sys->sspTestWritePollRead(ADC_SSP, code);
        if(iloop>nburn)
        {
            datum = ADC::decodeUSB(datum);
            uint64_t datum64 = datum;
            sum+=datum;
            sumsq+=datum64*datum64;
            if(datum>max)max=datum;
            if(datum<min)min=datum;
        }
        m_sys->loopDelay(ndelayloop);
    }
    uint32_t datum = m_sys->sspTestWritePollRead(ADC_SSP, ADC::codeReadFIFO());
    datum = ADC::decodeUSB(datum);
    uint64_t datum64 = datum;
    sum+=datum;
    sumsq+=datum64*datum64;
    if(datum>max)max=datum;
    if(datum<min)min=datum;
}

void MirrorControlBoard::
measureADC(unsigned iadc, unsigned ichan, unsigned nmeas, 
        std::vector<uint32_t>& vMeas,
        unsigned ndelayloop)
{
    selectADC(iadc);
    uint32_t code = ADC::codeSelect(ichan);
    unsigned nloop = nmeas;
    vMeas.resize(nmeas);

    for(unsigned iloop=0; iloop <= nloop; iloop++)
    {
        uint32_t datum;
        if(iloop == nloop)
            datum = m_sys->sspTestWritePollRead(ADC_SSP, ADC::codeReadFIFO());
        else
            datum = m_sys->sspTestWritePollRead(ADC_SSP, code);

        if(iloop == 0)
            continue;

        datum = ADC::decodeUSB(datum);
        vMeas[iloop - 1] = datum;

        m_sys->loopDelay(ndelayloop);
    }
}
*/
