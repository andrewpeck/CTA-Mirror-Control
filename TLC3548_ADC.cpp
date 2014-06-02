#include <stdint.h>
#include <TLC3548_ADC.hpp>

TLC3548_ADC::TLC3548_ADC() {}; 
TLC3548_ADC::~TLC3548_ADC() {};

uint32_t TLC3548_ADC::codeCommand(uint32_t cmd, uint32_t data)
{
    return ((cmd&0xF)<<12)|(data&0x0FFF);
}

uint32_t TLC3548_ADC::codeSelectChannel(unsigned ichan)
{
    return codeCommand(ichan&0x7);
}

uint32_t TLC3548_ADC::codeSWPowerDown()
{
    return codeCommand(0x8);
}

uint32_t TLC3548_ADC::codeInitialize()
{
    return codeCommand(0xA);
}

uint32_t TLC3548_ADC::codeConfig(SamplePeriod sp, ReferenceSelect rs,
        ConversionClock cc, ConversionMode cm, SweepSequence ss, InputMode im,
        OutputFormat of, PinFunction pf, TriggerLevel tl) 
{
    uint32_t cfr = 0;
    if (rs == RS_EXTERNAL)               
        cfr |= 0x800;
    if (of == OF_BTC)                    
        cfr |= 0x400;
    if (sp == SP_SHORT)                  
        cfr |= 0x200;
    if (cc == CC_SCLK)                   
        cfr |= 0x100;
    if (im == IM_PSEUDO_DIFFERENTIAL)    
        cfr |= 0x080;
    switch(cm)
    {
        case CM_ONE_SHOT:
            break;
        case CM_REPEAT:
            cfr |= 0x020;
            break;
        case CM_SWEEP:
            cfr |= 0x040;
            break;
        case CM_REPEAT_SWEEP:
            cfr |= 0x060;
            break;
    };

    switch(ss)
    {
        case SS_01234567:
            break;
        case SS_02460246:
            cfr |= 0x008;
            break;
        case SS_00224466:
            cfr |= 0x010;
            break;
        case SS_02020202:
            cfr |= 0x018;
            break;
    };

    if(pf == PF_EOC)
        cfr |= 0x004;

    switch(tl)
    {
        case TL_FULL:
            break;
        case TL_75PC:
            cfr |= 0x001;
            break;
        case TL_50PC:
            cfr |= 0x002;
            break;
        case TL_25PC:
            cfr |= 0x003;
            break;
    }
    return codeCommand(0xA,cfr);
}

uint32_t TLC3548_ADC::codeSelectRefMid()
{
    return codeCommand(0xB);
}

uint32_t TLC3548_ADC::codeSelectRefM()
{
    return codeCommand(0xC);
}

uint32_t TLC3548_ADC::codeSelectRefP()
{
    return codeCommand(0xD);
}

uint32_t TLC3548_ADC::codeSelect(unsigned ichan)
{
    if(ichan<8)
        return codeSelectChannel(ichan);
    else if (ichan==8)
        return codeSelectRefP();
    else if (ichan==9)
        return codeSelectRefMid();
    else if (ichan==10)
        return codeSelectRefM();
    else
        return (0); 
}

uint32_t TLC3548_ADC::codeReadFIFO()
{
    return codeCommand(0xE);
}

uint32_t TLC3548_ADC::codeConfigDefault()
{
    return codeCommand(0xF);
}

uint32_t TLC3548_ADC::fullScaleUSB()
{
    return (0x1<<NBIT)-1;
}

uint32_t TLC3548_ADC::decodeUSB(uint32_t data)
{
    data >>= (16-NBIT);
    data &= ((0x1<<NBIT)-1);
    return data;
}

int32_t TLC3548_ADC::decodeBOB(uint32_t data)
{
    data >>= (16-NBIT);
    data &= ((0x1<<NBIT)-1);
    return static_cast<int32_t>(data)-(0x1<<(NBIT-1));
}

int32_t TLC3548_ADC::decodeBTC(uint32_t data)
{
    uint32_t udata = data;
    udata >>= (16-NBIT);
    udata &= ((0x1<<(NBIT-1))-1);
    if (data & 0x00008000)
        udata |= 0xFFFF8000;
    return static_cast<int32_t>(udata);
}

float TLC3548_ADC::fracData(const uint32_t data)
{
    return float(data)/float(fullScaleUSB());
}

float TLC3548_ADC::voltData(const uint32_t data, const float full_volt)
{
    return fracData(data)*full_volt;
}

float TLC3548_ADC::fracUSB(const uint32_t data)
{
    return float(decodeUSB(data))/float(fullScaleUSB());
}

float TLC3548_ADC::voltUSB(const uint32_t data, const float full_volt)
{
    return fracUSB(data)*full_volt;
}
