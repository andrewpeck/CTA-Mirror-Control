//-*-mode:c++; mode:font-lock;-*-

/*! \file demo.cpp

  Hexapod motion demo

  \author     Stephen Fegan               \n
              UCLA                        \n
              sfegan@astro.ucla.edu       \n

  \version    1.0
  \date       06/24/2008
*/

#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <string>
#include <cassert>
#include <cmath>

#include "Layout.hpp"
#include "VerdexXM4.hpp"
#include "TLCX5XX_ADC.hpp"

/* by Jim Ulery */
static unsigned julery_isqrt(unsigned long val) {
  unsigned long temp, g=0, b = 0x8000, bshft = 15;
  do {
    if (val >= (temp = (((g << 1) + b)<<bshft--))) {
      g += b;
      val -= temp;
    }
  } while (b >>= 1);
  return g;
}

const char* usage_text = "\
where \"amplitude\" is the amplitude of the motion in steps (half the total\n\
peak-to-peak motion), \"delay\" is the length of the delay loop and\n\
\"period_factor\" is the period of the motion in units of amplitude*2*PI\n\
(this factor must be greater than or equal to 1.0).\n";

void usage(const char* program, std::ostream& stream, 
	   int status = EXIT_FAILURE)
{
  stream << "Usage: " 
	 << program << " amplitude [delay=5000] [period_factor=1.0]\n\n"
	 << usage_text;
  exit(status);
}

#if defined(__arm__)
typedef VerdexXM4<> Sys;
#else
typedef VerdexXM4<SimulatedRegisters> Sys;
#endif
typedef Layout<> LO;

int main(int argc, char** argv)
{
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
  if(pfactor<1.0)pfactor=1.0;
  
  unsigned nperiod = lroundf(pfactor*float(iamp)*M_PI*2.0);

  unsigned nloopA = julery_isqrt(nperiod);
  unsigned phase_offset = nloopA/3;
  nloopA = phase_offset*3;

  unsigned nloopB = nperiod/nloopA;
  nperiod = nloopA*nloopB;

  float amp = float(nperiod)/M_PI*0.5/pfactor;
  float sqrtamp = sqrtf(amp);
  iamp = lroundf(amp);

  int64_t* sinA = new int64_t[nloopA];
  int64_t* cosA = new int64_t[nloopA];
  int64_t* sinB = new int64_t[nloopB];
  int64_t* cosB = new int64_t[nloopB];

  for(unsigned iloopA=0;iloopA<nloopA;iloopA++)
    {
      float phi = float(iloopA)/float(nloopA)*M_PI*2.0;
      sinA[iloopA] = llroundf(amp*sin(phi));
      cosA[iloopA] = llroundf(amp*cos(phi));
    }

  for(unsigned iloopB=0;iloopB<nloopB;iloopB++)
    {
      float phi = float(iloopB)/float(nperiod)*M_PI*2.0;
      sinB[iloopB] = llroundf(amp*sin(phi));
      cosB[iloopB] = llroundf(amp*cos(phi));
    }

  int64_t ix1a = -iamp;
  int64_t ix2a = -iamp;
  int64_t ix3a = -iamp;

  Sys sys;

  while(1)
    {
      for(unsigned iloopA=0;iloopA<nloopA;iloopA++)
	{
	  const unsigned p1 = iloopA;
	  const unsigned p2 = (p1+phase_offset)%nloopA;
	  const unsigned p3 = (p2+phase_offset)%nloopA;
	  for(unsigned iloopB=0;iloopB<nloopB;iloopB++)
	    {
	      const int64_t ix1t = 
		(sinA[p1]*cosB[iloopB]+cosA[p1]*sinB[iloopB])/iamp;
	      const int64_t ix2t =
		(sinA[p2]*cosB[iloopB]+cosA[p2]*sinB[iloopB])/iamp;
	      const int64_t ix3t =
		(sinA[p3]*cosB[iloopB]+cosA[p3]*sinB[iloopB])/iamp;

	      if(ix1t != ix1a)
		{
		  int dir = 0;
		  if(ix1t > ix1a)ix1a++;
		  else ix1a--, dir=1;
		  sys.gpioWriteLevel(LO::igpioDR1Dir(),dir);
		  sys.gpioWriteLevel(LO::igpioDR4Dir(),dir);
		  sys.gpioWriteLevel(LO::igpioDR1Step(),1);
		  sys.gpioWriteLevel(LO::igpioDR4Step(),1);
		}

	      if(ix2t != ix2a)
		{
		  int dir = 0;
		  if(ix2t > ix2a)ix2a++;
		  else ix2a--, dir=1;
		  sys.gpioWriteLevel(LO::igpioDR2Dir(),dir);
		  sys.gpioWriteLevel(LO::igpioDR5Dir(),dir);
		  sys.gpioWriteLevel(LO::igpioDR2Step(),1);
		  sys.gpioWriteLevel(LO::igpioDR5Step(),1);
		}

	      if(ix3t != ix3a)
		{
		  int dir = 0;
		  if(ix3t > ix3a)ix3a++;
		  else ix3a--, dir=1;
		  sys.gpioWriteLevel(LO::igpioDR3Dir(),dir);
		  sys.gpioWriteLevel(LO::igpioDR6Dir(),dir);
		  sys.gpioWriteLevel(LO::igpioDR3Step(),1);
		  sys.gpioWriteLevel(LO::igpioDR6Step(),1);
		}

	      for(volatile unsigned idelay=0;idelay<ndelay;idelay++);
	      sys.gpioWriteLevel(LO::igpioDR1Step(),0);
	      sys.gpioWriteLevel(LO::igpioDR2Step(),0);
	      sys.gpioWriteLevel(LO::igpioDR3Step(),0);
	      sys.gpioWriteLevel(LO::igpioDR4Step(),0);
	      sys.gpioWriteLevel(LO::igpioDR5Step(),0);
	      sys.gpioWriteLevel(LO::igpioDR6Step(),0);
	      for(volatile unsigned idelay=0;idelay<ndelay;idelay++);	  
	      
	      std::cout << ix1a << ' ' << ix2a << ' ' << ix3a << '\n';
	    }
	}
    }
}
