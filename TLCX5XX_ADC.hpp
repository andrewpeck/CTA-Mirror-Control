//-*-mode:c++; mode:font-lock;-*-

/*! \file TLC2578.hpp

  Compose and decode messages for TLCx5xx 4/8-channel ADCs

  \author     Stephen Fegan               \n
  UCLA                        \n
  sfegan@astro.ucla.edu       \n

  \version    1.0
  \date       06/02/2008
  */

#ifndef TLCX5XX_ADC_HPP
#define TLCX5XX_ADC_HPP

#include <stdint.h>

#include <Bits.hpp>

template<unsigned NBIT> class TLCX5XX_ADC
{
    public:

        static uint32_t codeCommand(uint32_t cmd, uint32_t data = 0)
        {
            return ((cmd&0xF)<<12)|(data&0x0FFF);
        }

        static uint32_t codeSelectChannel(unsigned ichan)
        {
            return codeCommand(ichan&0x7);
        }

        enum OutputFormat { OF_BOB, OF_USB, OF_BTC };
        enum SamplePeriod { SP_LONG, SP_SHORT };
        enum ReferenceSelect { RS_INTERNAL, RS_EXTERNAL };
        enum ConversionClock { CC_INTERNAL, CC_SCLK };
        enum InputMode { IM_SINGLE_ENDED, IM_PSEUDO_DIFFERENTIAL };
        enum ConversionMode { CM_ONE_SHOT, CM_REPEAT, CM_SWEEP, CM_REPEAT_SWEEP };
        enum SweepSequence { SS_01234567, SS_02460246, SS_00224466, SS_02020202 };
        enum PinFunction { PF_INT_BAR, PF_EOC };
        enum TriggerLevel { TL_FULL, TL_75PC, TL_50PC, TL_25PC };

        static uint32_t codeSWPowerDown() { return codeCommand(0x8); }

        static uint32_t codeInitialize() { return codeCommand(0xA); }
        static uint32_t codeConfig(
                SamplePeriod sp    = SP_SHORT,
                ReferenceSelect rs = RS_EXTERNAL,
                ConversionClock cc = CC_INTERNAL,
                ConversionMode cm  = CM_ONE_SHOT,
                SweepSequence ss   = SS_01234567,
                InputMode im       = IM_SINGLE_ENDED,
                OutputFormat of    = OF_BOB,
                PinFunction pf     = PF_EOC,
                TriggerLevel tl    = TL_50PC)
        {
            uint32_t cfr = 0;
            if(rs == RS_EXTERNAL)               cfr |= 0x800;
            if(of == OF_BTC)                    cfr |= 0x400;
            if(sp == SP_SHORT)                  cfr |= 0x200;
            if(cc == CC_SCLK)                   cfr |= 0x100;
            if(im == IM_PSEUDO_DIFFERENTIAL)    cfr |= 0x080;
            switch(cm) {
                case CM_ONE_SHOT:               break;
                case CM_REPEAT:                 cfr |= 0x020; break;
                case CM_SWEEP:                  cfr |= 0x040; break;
                case CM_REPEAT_SWEEP:           cfr |= 0x060; break;
            };

            switch(ss) {
                case SS_01234567:               break;
                case SS_02460246:               cfr |= 0x008; break;
                case SS_00224466:               cfr |= 0x010; break;
                case SS_02020202:               cfr |= 0x018; break;
            };

            if(pf == PF_EOC)                    cfr |= 0x004;

            switch(tl) {
                case TL_FULL:                   break;
                case TL_75PC:                   cfr |= 0x001; break;
                case TL_50PC:                   cfr |= 0x002; break;
                case TL_25PC:                   cfr |= 0x003; break;
            }
            return codeCommand(0xA,cfr);
        }

        static uint32_t codeSelectRefMid()  { return codeCommand(0xB); }
        static uint32_t codeSelectRefM()    { return codeCommand(0xC); }
        static uint32_t codeSelectRefP()    { return codeCommand(0xD); }

        static uint32_t codeSelect(unsigned ichan) {
            if(ichan<8)
                return codeSelectChannel(ichan);
            else if(ichan==9)
                return codeSelectRefMid();
            else if(ichan==10)
                return codeSelectRefM();
            else 
                return codeSelectRefP();
        }

        static uint32_t codeReadFIFO()      { return codeCommand(0xE); }
        static uint32_t codeConfigDefault() { return codeCommand(0xF); }

        static uint32_t fullScaleUSB()      { return (0x1<<NBIT)-1; }

        static uint32_t decodeUSB(uint32_t data) {
            data >>= (16-NBIT);
            data &= ((0x1<<NBIT)-1);
            return data;
        }

        static int32_t decodeBOB(uint32_t data) {
            data >>= (16-NBIT);
            data &= ((0x1<<NBIT)-1);
            return static_cast<int32_t>(data)-(0x1<<(NBIT-1));
        }

        static int32_t decodeBTC(uint32_t data) {
            uint32_t udata = data;
            udata >>= (16-NBIT);
            udata &= ((0x1<<(NBIT-1))-1);
            if(data&0x00008000)udata |= 0xFFFF8000;
            return static_cast<int32_t>(udata);
        }

        static float fracData(const uint32_t data) {
            return float(data)/float(fullScaleUSB());
        }

        static float voltData(const uint32_t data, const float full_volt = 5.0)
        {
            return fracData(data)*full_volt;
        }

        static float fracUSB(const uint32_t data)
        {
            return float(decodeUSB(data))/float(fullScaleUSB());
        }

        static float voltUSB(const uint32_t data, const float full_volt = 5.0)
        {
            return fracUSB(data)*full_volt;
        }

};

typedef TLCX5XX_ADC<12> TLC2578;
typedef TLCX5XX_ADC<14> TLC3578;
typedef TLCX5XX_ADC<14> TLC3548;

#endif // ndef TLCX5XX_ADC_HPP
