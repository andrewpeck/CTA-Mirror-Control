//-*-mode:c++; mode:font-lock;-*-

/*! \file VerdexXM4.cpp

  Class which maps to Gumstix Verdex XM4 architecture, which mostly
  exposes the functionality of Marvell (formerly Intel) XScale PXA270
  on which it is based.

  \author     Stephen Fegan               \n
              UCLA                        \n
              sfegan@astro.ucla.edu       \n

  \version    1.0
  \date       05/20/2008
*/

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>

#include "Registers.hpp"

#define MMAPFAIL ((void*)-1)

#define MAKEMAP(PHYS,VIRT,TEXT) \
  if(makeMap(PHYS,VIRT) == MMAPFAIL)			\
    {							\
      perror("mmap(" TEXT ")");				\
      exit(EXIT_FAILURE);				\
    }

MappedRegisters::MappedRegisters():
  m_mmap_fd(-1), 
  m_gpio_base(), m_pwm02_base(), m_pwm13_base(), 
  m_ssp1_base(), m_ssp2_base(), m_ssp3_base(),
  m_clock_base()
{
  m_mmap_fd = open("/dev/mem", O_RDWR | O_SYNC);
  if(m_mmap_fd<0) 
    {
      perror("open(\"/dev/mem\")");
      exit(EXIT_FAILURE);
    }

  MAKEMAP(m_gpio_base,  mapBaseGPIO(),  "GPIO");
  MAKEMAP(m_pwm02_base, mapBasePWM02(), "PWM02");
  MAKEMAP(m_pwm13_base, mapBasePWM13(), "PWM13");
  MAKEMAP(m_ssp1_base,  mapBaseSSP1(),  "SSP1");
  MAKEMAP(m_ssp2_base,  mapBaseSSP2(),  "SSP2");
  MAKEMAP(m_ssp3_base,  mapBaseSSP3(),  "SSP3");
  MAKEMAP(m_clock_base, mapBaseClock(), "CLOCK");
}

#define MUNMAP(VIRT) \
  munmap(const_cast<void*>(VIRT), mapSize())

MappedRegisters::~MappedRegisters()
{
  MUNMAP(m_gpio_base);
  MUNMAP(m_pwm02_base);
  MUNMAP(m_pwm13_base);
  MUNMAP(m_ssp1_base);
  MUNMAP(m_ssp2_base);
  MUNMAP(m_ssp3_base);
  MUNMAP(m_clock_base);
  close(m_mmap_fd);
}

volatile void* MappedRegisters::
makeMap(volatile void*& virtual_addr, off_t physical_addr, size_t length)
{
  virtual_addr = mmap(0, length, PROT_READ|PROT_WRITE, MAP_SHARED, m_mmap_fd, 
		      physical_addr);
  return &virtual_addr;
}
