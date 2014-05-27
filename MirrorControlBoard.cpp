/*  
 *  MirrorControlBoard.cpp - Class which implements useful mid-level
 *  functionality for the mirror control board.
 */

#include <cassert>
#include <MirrorControlBoard.hpp>
#include <GPIOInterface.hpp>

MirrorControlBoard::MirrorControlBoard(bool no_initialize, unsigned nusb): m_nusb(nusb>7?7:nusb)
{
    if (no_initialize)
        return;
    else
        powerDownAll();
}

MirrorControlBoard::~MirrorControlBoard() { }

void MirrorControlBoard::initialize(const unsigned ssp_clk_div)
{
    setUStep(USTEP_8);
    enableDriveSR();
    disableDriveHiCurrent();
    gpio.WriteLevel(layout.igpioReset,1);
    //initializeSPI();
}

//void MirrorControlBoard::initializeSPI()
//{
//    void spiConfigure (int issp, unsigned clk_phase, bool clk_polarity, bool clk_div, bool epol, int word_length) {
//    for (int issp=0; issp<=8; issp++) { m_sys.spiConfigure (issp, 0, 0, 4, 1, 0xF); }
//    printf("Enabling SPI");
//    m_sys.spiEnable(ADC_SPI);
//    printf("Configuring SPI");
//    m_sys.spiConfigure (ADC_SPI, 0, 0, 4, 1, 0xF);
//    m_sys.spiFlush (ADC_SPI);
//}

void MirrorControlBoard::powerDownAll()
{
    powerDownAllUSB();
    powerDownBase();
}

void MirrorControlBoard::powerUpAll()
{
    powerUpBase();
    powerUpAllUSB();
}

void MirrorControlBoard::powerUpBase()
{
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

void MirrorControlBoard::powerDownBase()
{
    // Set on-board ADC into sleep mode
    selectADC(7);
    spi.WriteRead(ADC.codeSWPowerDown());

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

void MirrorControlBoard::powerDownAllUSB()
{
    for(unsigned iusb=0; iusb<m_nusb; iusb++)
        powerDownUSB(iusb);
}

void MirrorControlBoard::powerUpAllUSB()
{
    for(unsigned iusb=0; iusb<m_nusb; iusb++)
    {
        powerUpUSB(iusb);
    }
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

void MirrorControlBoard::powerUpADCs()
{
    gpio.WriteLevel(layout.igpioPowerADC,1);
}

void MirrorControlBoard::powerDownADCs()
{
    gpio.WriteLevel(layout.igpioPowerADC,0);
}

bool MirrorControlBoard::isADCsPoweredUp()
{
    return gpio.ReadLevel(layout.igpioPowerADC)?false:true;
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
    switch(ustep)
    {
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

void MirrorControlBoard:: stepOneDrive(unsigned idrive, Dir dir, unsigned ndelayloop)
{
    // Write Direction to the DIR pin
    gpio.WriteLevel(layout.igpioDir(idrive),(dir==DIR_RETRACT)?1:0);

    // Writes one step to STEP pin
    unsigned igpio = layout.igpioStep(idrive);
    gpio.WriteLevel(igpio,(dir==DIR_NONE)?0:1);

    // a delay
    loopDelay(ndelayloop);

    // Toggle pin back to low
    gpio.WriteLevel(igpio,0);
}

void MirrorControlBoard::stepAllDrives(Dir dr1_dir, Dir dr2_dir, Dir dr3_dir, Dir dr4_dir, Dir dr5_dir, Dir dr6_dir, unsigned ndelayloop)
{
    gpio.WriteLevel(layout.igpioDir1,(dr1_dir==DIR_RETRACT)?1:0);
    gpio.WriteLevel(layout.igpioDir2,(dr2_dir==DIR_RETRACT)?1:0);
    gpio.WriteLevel(layout.igpioDir3,(dr3_dir==DIR_RETRACT)?1:0);
    gpio.WriteLevel(layout.igpioDir4,(dr4_dir==DIR_RETRACT)?1:0);
    gpio.WriteLevel(layout.igpioDir5,(dr5_dir==DIR_RETRACT)?1:0);
    gpio.WriteLevel(layout.igpioDir6,(dr6_dir==DIR_RETRACT)?1:0);

    gpio.WriteLevel(layout.igpioStep1,(dr1_dir==DIR_NONE)?0:1);
    gpio.WriteLevel(layout.igpioStep2,(dr2_dir==DIR_NONE)?0:1);
    gpio.WriteLevel(layout.igpioStep3,(dr3_dir==DIR_NONE)?0:1);
    gpio.WriteLevel(layout.igpioStep4,(dr4_dir==DIR_NONE)?0:1);
    gpio.WriteLevel(layout.igpioStep5,(dr5_dir==DIR_NONE)?0:1);
    gpio.WriteLevel(layout.igpioStep6,(dr6_dir==DIR_NONE)?0:1);

    loopDelay(ndelayloop);

    gpio.WriteLevel(layout.igpioStep1,0);
    gpio.WriteLevel(layout.igpioStep2,0);
    gpio.WriteLevel(layout.igpioStep3,0);
    gpio.WriteLevel(layout.igpioStep4,0);
    gpio.WriteLevel(layout.igpioStep5,0);
    gpio.WriteLevel(layout.igpioStep6,0);
}

void MirrorControlBoard::setPhaseZeroOnAllDrives()
{
    gpio.WriteLevel(layout.igpioReset,0);
    loopDelay(1000);
    gpio.WriteLevel(layout.igpioReset,1);
}

void MirrorControlBoard::enableDrive(unsigned idrive, bool enable)
{
    gpio.WriteLevel(layout.igpioEnable(idrive), enable?0:1);
}

void MirrorControlBoard::enableAllDrives(bool enable)
{
    bool ienable = enable?0:1;
    gpio.WriteLevel(layout.igpioEnable1, ienable);
    gpio.WriteLevel(layout.igpioEnable2, ienable);
    gpio.WriteLevel(layout.igpioEnable3, ienable);
    gpio.WriteLevel(layout.igpioEnable4, ienable);
    gpio.WriteLevel(layout.igpioEnable5, ienable);
    gpio.WriteLevel(layout.igpioEnable6, ienable);
}

void MirrorControlBoard::disableDrive(unsigned idrive)
{
    enableDrive(idrive, false);
}

void MirrorControlBoard::disableAllDrives()
{
    enableAllDrives(false);
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

uint32_t MirrorControlBoard::measureADC(unsigned iadc, unsigned ichan, unsigned ndelayloop)
{
    // Assert Chip Select
    selectADC(iadc);

    // ADC Channel Select
    uint32_t code = ADC.codeSelect(ichan);
    spi.WriteRead(code);

    // some delay
    loopDelay(ndelayloop);

    // Read ADC
    uint32_t datum = spi.WriteRead(ADC.codeReadFIFO());

    return ADC.decodeUSB(datum);
}

void MirrorControlBoard::measureADC(unsigned iadc, unsigned ichan, unsigned nmeas, std::vector<uint32_t>& vMeas, unsigned ndelayloop)
{
    selectADC(iadc);
    uint32_t code = ADC.codeSelect(ichan);
    unsigned nloop = nmeas;
    vMeas.resize(nmeas);

    // Loop over number of measurements
    for(unsigned iloop=0; iloop <= nloop; iloop++)
    {
        uint32_t datum;
        // last loop
        if (iloop == nloop)
            datum = spi.WriteRead(ADC.codeReadFIFO());
        // not the last loop
        else
            datum = spi.WriteRead(code);

        if (iloop == 0)
            continue;

        // decode data and fill array
        datum = ADC.decodeUSB(datum);
        vMeas[iloop - 1] = datum;

        loopDelay(ndelayloop);
    }
}

void MirrorControlBoard::measureManyADC(uint32_t* data, unsigned iadc, unsigned zchan, unsigned nchan, unsigned ndelayloop)
{
    // adc chip select
    selectADC(iadc);
    uint32_t datum;

    // loop over adc channels
    for (unsigned ichan=0; ichan<nchan; ichan++)
    {
        // read data
        datum = spi.WriteRead(ADC.codeSelect(zchan+ichan));

        if(ichan>0)
            data[ichan-1] = ADC.decodeUSB(datum);

        // some delay
        loopDelay(ndelayloop);
    }

    // Read data from FIFO
    datum = spi.WriteRead(ADC.codeReadFIFO());

    // Decode data and fill array
    if(nchan>0)
        data[nchan-1] = ADC.decodeUSB(datum);
}

uint32_t MirrorControlBoard::measureADCWithBurn(unsigned iadc, unsigned ichan, unsigned ndelayloop)
{
    // ADC Chip Select
    selectADC(iadc);

    // Select ADC Channel
    uint32_t code = ADC.codeSelect(ichan);

    // Read
    spi.WriteRead(code);

    // Some delay
    loopDelay(ndelayloop);

    // Read
    spi.WriteRead(code);

    // Some delay
    loopDelay(ndelayloop);

    // save last data
    uint32_t datum = spi.WriteRead(ADC.codeReadFIFO());

    // return decoded data
    return ADC.decodeUSB(datum);
}

void MirrorControlBoard::measureManyADCWithBurn(uint32_t* data, unsigned iadc, unsigned zchan, unsigned nchan, unsigned ndelayloop)
{
    // ADC Chip Select
    selectADC(iadc);
    // Loop Over the Number of ADC channels
    for(unsigned ichan=0; ichan<nchan; ichan++)
    {
        uint32_t code = ADC.codeSelect(zchan+ichan);
        // Read ADC Channel
        uint32_t datum = spi.WriteRead(code);


        // Decode data and fill array
        if(ichan>0)
            data[ichan-1] = ADC.decodeUSB(datum);

        // some delay
        loopDelay(ndelayloop);

        // Read ADC Channel again (discarding results)
        spi.WriteRead(code);

        // Some Delay
        loopDelay(ndelayloop);
    }

    // Read last data 
    uint32_t datum = spi.WriteRead(ADC.codeReadFIFO());

    // Decode data and fill array
    if(nchan>0)
        data[nchan-1] = ADC.decodeUSB(datum);
}

void MirrorControlBoard::measureADCStat(unsigned iadc, unsigned ichan, unsigned nmeas, uint32_t& sum, uint64_t& sumsq, uint32_t& min, uint32_t& max, unsigned nburn, unsigned ndelayloop)
{
    selectADC(iadc);
    uint32_t code = ADC.codeSelect(ichan);
    unsigned nloop = nburn + nmeas;
    sum = 0;
    sumsq = 0;
    max = 0;
    min = ~max;
    // Loop over number of measurements
    for(unsigned iloop=0; iloop<nloop; iloop++)
    {
        // Read data
        uint32_t datum = spi.WriteRead(code);
        if (iloop>nburn)
        {
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
        // some delay
        loopDelay(ndelayloop);
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


int MirrorControlBoard::measureEncoder(unsigned ichan, unsigned calib_lo, unsigned calib_hi, unsigned ticks_per_rev, const int* correction)
{
    int meas = int(measureADCWithBurn(7, ichan));
    int calib_range = calib_lo - calib_hi;
    meas -= calib_hi;
    meas *= ticks_per_rev;
    meas /= calib_range;
    if (correction && meas>=0 && meas<int(ticks_per_rev))
        meas+=correction[meas];
    return meas;
}

//------------------------------------------------------------------------------
// General Purpose Utilities
//------------------------------------------------------------------------------

void MirrorControlBoard::loopDelay(unsigned nloop)
{
    for(volatile unsigned iloop=0; iloop<nloop; iloop++);
}
