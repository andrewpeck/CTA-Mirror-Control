//-*-mode:c++; mode:font-lock;-*-

/*! \file Overo.hpp

  Class which maps to Gumstix Verdex XM4 architecture, which mostly
  exposes the functionality of Marvell (formerly Intel) XScale PXA270
  on which it is based.

  \author     Stephen Fegan               \n
  UCLA                        \n
  sfegan@astro.ucla.edu       \n

  \version    1.0
  \date       05/20/2008
  */

#ifndef BITS_HPP
#define BITS_HPP

class Bits
{
    public:
        // --------------------------------------------------------------------------
        // STATIC shift and mask functions
        // --------------------------------------------------------------------------

        static uint32_t shift1Bit(const unsigned iel) { return iel&0x1F; }
        static uint32_t shift2Bit(const unsigned iel) { return (iel&0x0F)<<1; }
        static uint32_t shift4Bit(const unsigned iel) { return (iel&0x07)<<2; }

        static uint32_t mask1Bit() { return 0x01; }
        static uint32_t mask2Bit() { return 0x03; }
        static uint32_t mask4Bit() { return 0x0F; }

        static uint32_t mask1Bit(const unsigned iel) { return 0x01<<shift1Bit(iel); }
        static uint32_t mask2Bit(const unsigned iel) { return 0x03<<shift2Bit(iel); }
        static uint32_t mask4Bit(const unsigned iel) { return 0x0F<<shift4Bit(iel); }
};

#endif // ndef BITS_HPP
