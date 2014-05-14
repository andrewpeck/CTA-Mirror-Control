//-*-mode:c++; mode:font-lock;-*-

/*! \file Registers.hpp

  Classes which have mapped registers for Gumstix Verdex XM4 architecture

  \author     Stephen Fegan               \n
              UCLA                        \n
              sfegan@astro.ucla.edu       \n

  \version    1.0
  \date       05/20/2008
*/

#ifndef REGISTERS_HPP
#define REGISTERS_HPP

#include <sys/mman.h>
#include <stdint.h>

#include <Bits.hpp>

class MappedRegisters
{
public:
  MappedRegisters();
  ~MappedRegisters();

  // --------------------------------------------------------------------------
  // STATIC CLOCK register (PHYSICAL) address functions
  // --------------------------------------------------------------------------

  static off_t physBaseClock()  { return 0x41300000; }

  static off_t physClockConf()             { return 0x41300000; }
  static off_t physClockEnable()           { return 0x41300004; }
  static off_t physClockOscConf()          { return 0x41300008; }
  static off_t physClockStatus()           { return 0x4130000C; }

  // --------------------------------------------------------------------------
  // STATIC GPIO register (PHYSICAL) address functions
  // --------------------------------------------------------------------------

  static off_t physBaseGPIO()  { return 0x40E00000; }

  static off_t physGPIOReadLevelBaseL()    { return 0x40E00000; }
  static off_t physGPIODirectionBaseL()    { return 0x40E0000C; }
  static off_t physGPIOSetLevelBaseL()     { return 0x40E00018; }
  static off_t physGPIOClrLevelBaseL()     { return 0x40E00024; }
  static off_t physGPIORiseEdgeBaseL()     { return 0x40E00030; }
  static off_t physGPIOFallEdgeBaseL()     { return 0x40E0003C; }
  static off_t physGPIOEdgeStatBaseL()     { return 0x40E00048; }
  static off_t physGPIOReadLevelBaseH()    { return 0x40E00100; }
  static off_t physGPIODirectionBaseH()    { return 0x40E0010C; }
  static off_t physGPIOSetLevelBaseH()     { return 0x40E00118; }
  static off_t physGPIOClrLevelBaseH()     { return 0x40E00124; }
  static off_t physGPIORiseEdgeBaseH()     { return 0x40E00130; }
  static off_t physGPIOFallEdgeBaseH()     { return 0x40E0013C; }
  static off_t physGPIOEdgeStatBaseH()     { return 0x40E00148; }
  static off_t physGPIOAltFuncBase()       { return 0x40E00054; }

#define __32BITREG_LH(IPIN,REGL,REGU)				\
  if(IPIN&0x40)							\
    if(IPIN&0x20)return (REGU);					\
    else return (REGL)+0x00000008;				\
  else								\
    if(IPIN&0x20)return (REGL)+0x00000004;			\
    else return (REGL)

  static off_t physGPIOReadLevel(const unsigned ipin)
  {
    __32BITREG_LH(ipin,physGPIOReadLevelBaseL(),physGPIOReadLevelBaseH());
  }

  static off_t physGPIODirection(const unsigned ipin)
  {
   __32BITREG_LH(ipin,physGPIODirectionBaseL(),physGPIODirectionBaseH());
  }

  static off_t physGPIOSetLevel(const unsigned ipin)
  { 
    __32BITREG_LH(ipin,physGPIOSetLevelBaseL(),physGPIOSetLevelBaseH());
  }

  static off_t physGPIOClrLevel(const unsigned ipin)
  {
    __32BITREG_LH(ipin,physGPIOClrLevelBaseL(),physGPIOClrLevelBaseH());
  }
  
  static off_t physGPIORiseEdge(const unsigned ipin)
  {
    __32BITREG_LH(ipin,physGPIORiseEdgeBaseL(),physGPIORiseEdgeBaseH());
  }

  static off_t physGPIOFallEdge(const unsigned ipin)
  {
    __32BITREG_LH(ipin,physGPIOFallEdgeBaseL(),physGPIOFallEdgeBaseH());
  }

  static off_t physGPIOEdgeStat(const unsigned ipin)
  {
    __32BITREG_LH(ipin,physGPIOEdgeStatBaseL(),physGPIOEdgeStatBaseH());
  }

  static off_t physGPIOAltFunc(const unsigned ipin)
  { 
    return physGPIOAltFuncBase() + ((ipin>>2)&(~Bits::mask2Bit()));
  }
  
#undef __32BITREG_LH

  // --------------------------------------------------------------------------
  // STATIC PWM register (PHYSICAL) address functions
  // --------------------------------------------------------------------------
  
  static off_t physBasePWM02() { return 0x40B00000; }
  static off_t physBasePWM13() { return 0x40C00000; }

  static off_t physPWM0Control()           { return 0x40B00000; }
  static off_t physPWM0DutyCycle()         { return 0x40B00004; }
  static off_t physPWM0Period()            { return 0x40B00008; }
  static off_t physPWM2Control()           { return 0x40B00010; }
  static off_t physPWM2DutyCycle()         { return 0x40B00014; }
  static off_t physPWM2Period()            { return 0x40B00018; }
  static off_t physPWM1Control()           { return 0x40C00000; }
  static off_t physPWM1DutyCycle()         { return 0x40C00004; }
  static off_t physPWM1Period()            { return 0x40C00008; }
  static off_t physPWM3Control()           { return 0x40C00010; }
  static off_t physPWM3DutyCycle()         { return 0x40C00014; }
  static off_t physPWM3Period()            { return 0x40C00018; }

  static off_t physPWMControl(const unsigned ipwm)
  {
    if(ipwm&0x2)
      if(ipwm&0x1)return physPWM3Control();
      else return physPWM2Control();
    else
      if(ipwm&0x1)return physPWM1Control();
      else return physPWM0Control();
  }

  static off_t physPWMDutyCycle(const unsigned ipwm)
  {
    if(ipwm&0x2)
      if(ipwm&0x1)return physPWM3DutyCycle();
      else return physPWM2DutyCycle();
    else
      if(ipwm&0x1)return physPWM1DutyCycle();
      else return physPWM0DutyCycle();
  }

  static off_t physPWMPeriod(const unsigned ipwm)
  {
    if(ipwm&0x2)
      if(ipwm&0x1)return physPWM3Period();
      else return physPWM2Period();
    else
      if(ipwm&0x1)return physPWM1Period();
      else return physPWM0Period();
  }

  // --------------------------------------------------------------------------
  // STATIC SSP register (PHYSICAL) address functions
  // --------------------------------------------------------------------------

  static off_t physBaseSSP1()  { return 0x41000000; }
  static off_t physBaseSSP2()  { return 0x41700000; }
  static off_t physBaseSSP3()  { return 0x41900000; }

  static off_t physSSP1Control0()          { return 0x41000000; }
  static off_t physSSP1Control1()          { return 0x41000004; }
  static off_t physSSP1Status()            { return 0x41000008; }
  static off_t physSSP1InterruptTest()     { return 0x4100000C; }
  static off_t physSSP1DataRW()            { return 0x41000010; }
  static off_t physSSP1TimeOut()           { return 0x41000028; }
  static off_t physSSP1PSPConfig()         { return 0x4100002C; }
  static off_t physSSP1TXTimeslotActive()  { return 0x41000030; }
  static off_t physSSP1RXTimeslotActive()  { return 0x41000034; }
  static off_t physSSP1TimeslotStatus()    { return 0x41000038; }
  static off_t physSSP1AudioClockDiv()     { return 0x4100003C; }

  static off_t physSSP2Control0()          { return 0x41700000; }
  static off_t physSSP2Control1()          { return 0x41700004; }
  static off_t physSSP2Status()            { return 0x41700008; }
  static off_t physSSP2InterruptTest()     { return 0x4170000C; }
  static off_t physSSP2DataRW()            { return 0x41700010; }
  static off_t physSSP2TimeOut()           { return 0x41700028; }
  static off_t physSSP2PSPConfig()         { return 0x4170002C; }
  static off_t physSSP2TXTimeslotActive()  { return 0x41700030; }
  static off_t physSSP2RXTimeslotActive()  { return 0x41700034; }
  static off_t physSSP2TimeslotStatus()    { return 0x41700038; }
  static off_t physSSP2AudioClockDiv()     { return 0x4170003C; }

  static off_t physSSP3Control0()          { return 0x41900000; }
  static off_t physSSP3Control1()          { return 0x41900004; }
  static off_t physSSP3Status()            { return 0x41900008; }
  static off_t physSSP3InterruptTest()     { return 0x4190000C; }
  static off_t physSSP3DataRW()            { return 0x41900010; }
  static off_t physSSP3TimeOut()           { return 0x41900028; }
  static off_t physSSP3PSPConfig()         { return 0x4190002C; }
  static off_t physSSP3TXTimeslotActive()  { return 0x41900030; }
  static off_t physSSP3RXTimeslotActive()  { return 0x41900034; }
  static off_t physSSP3TimeslotStatus()    { return 0x41900038; }
  static off_t physSSP3AudioClockDiv()     { return 0x4190003C; }

  static off_t physSSPControl0(const unsigned issp)
  {
    if(issp==2)
      return physSSP3Control0();
    else if(issp==1)
      return physSSP2Control0();
    else 
      return physSSP1Control0();
  }

  static off_t physSSPControl1(const unsigned issp)
  {
    if(issp==2)
      return physSSP3Control1();
    else if(issp==1)
      return physSSP2Control1();
    else 
      return physSSP1Control1();
  }

  static off_t physSSPStatus(const unsigned issp)
  {
    if(issp==2)
      return physSSP3Status();
    else if(issp==1)
      return physSSP2Status();
    else
      return physSSP1Status();
  }

  static off_t physSSPInterruptTest(const unsigned issp)
  {
    if(issp==2)
      return physSSP3InterruptTest();
    else if(issp==1)
      return physSSP2InterruptTest();
    else 
      return physSSP1InterruptTest();
  }

  static off_t physSSPDataRW(const unsigned issp)
  {
    if(issp==2)
      return physSSP3DataRW();
    else if(issp==1)
      return physSSP2DataRW();
    else
      return physSSP1DataRW();
  }

  static off_t physSSPTimeOut(const unsigned issp)
  {
    if(issp==2)
      return physSSP3TimeOut();
    else if(issp==1)
      return physSSP2TimeOut();
    else 
      return physSSP1TimeOut();
  }

  static off_t physSSPPSPConfig(const unsigned issp)
  {
    if(issp==2)
      return physSSP3PSPConfig();
    else if(issp==1)
      return physSSP2PSPConfig();
    else
      return physSSP1PSPConfig();
  }

  static off_t physSSPTXTimeslotActive(const unsigned issp)
  {
    if(issp==2)
      return physSSP3TXTimeslotActive();
    else if(issp==1)
      return physSSP2TXTimeslotActive();
    else 
      return physSSP1TXTimeslotActive();
  }

  static off_t physSSPRXTimeslotActive(const unsigned issp)
  {
    if(issp==2)
      return physSSP3RXTimeslotActive();
    else if(issp==1)
      return physSSP2RXTimeslotActive();
    else 
      return physSSP1RXTimeslotActive();
  }

  static off_t physSSPTimeslotStatus(const unsigned issp)
  {
    if(issp==2)
      return physSSP3TimeslotStatus();
    else if(issp==1)
      return physSSP2TimeslotStatus();
    else 
      return physSSP1TimeslotStatus();
  }

  static off_t physSSPAudioClockDiv(const unsigned issp)
  {
    if(issp==2)
      return physSSP3AudioClockDiv();
    else if(issp==1)
      return physSSP2AudioClockDiv();
    else
      return physSSP1AudioClockDiv();
  }

  // --------------------------------------------------------------------------
  // STATIC functions defining (PHYSICAL) start of memory mapped regions
  // --------------------------------------------------------------------------

  static off_t mapSize() { return 4096; }
  static off_t mapMask() { return ~(mapSize()-1); }
  static off_t mapBaseGPIO()  { return physBaseGPIO()  & mapMask(); }
  static off_t mapBasePWM02() { return physBasePWM02() & mapMask(); }
  static off_t mapBasePWM13() { return physBasePWM13() & mapMask(); }
  static off_t mapBaseSSP1()  { return physBaseSSP1()  & mapMask(); }
  static off_t mapBaseSSP2()  { return physBaseSSP2()  & mapMask(); }
  static off_t mapBaseSSP3()  { return physBaseSSP3()  & mapMask(); }
  static off_t mapBaseClock() { return physBaseClock() & mapMask(); }

  static off_t phys2Offset(off_t phys, off_t map_base_phys)
  {
    return phys - map_base_phys;
  }

  template<typename T> static volatile T* 
  offset2Virt(off_t map_offset, volatile void* map_base_virt)
  {
    return 
      reinterpret_cast<volatile T*>(static_cast<volatile uint8_t*>(map_base_virt) + map_offset);
  }

  static volatile uint32_t* 
  offset2Virt32(off_t map_offset, volatile void* map_base_virt)
  {
    return offset2Virt<uint32_t>(map_offset, map_base_virt);
  }

  template<typename T> static volatile T* 
  phys2Virt(off_t phys, volatile void* map_base_virt, off_t map_base_phys)
  {
    return offset2Virt<T>(phys2Offset(phys, map_base_phys), map_base_virt);
  }

  static volatile uint32_t* 
  phys2Virt32(off_t phys, volatile void* map_base_virt, off_t map_base_phys)
  {
    return phys2Virt<uint32_t>(phys, map_base_virt, map_base_phys);
  }

  // --------------------------------------------------------------------------
  // Functions to return pointers to mapped Clock registers
  // --------------------------------------------------------------------------

  volatile uint32_t* offset2VirtClock32(off_t map_offset)
  {
    return offset2Virt32(map_offset, m_clock_base);
  }

  volatile uint32_t* phys2VirtClock32(off_t phys)
  {
    return phys2Virt32(phys, m_clock_base, mapBaseClock());
  }

  volatile uint32_t* ptrClockConf()
  {
    return phys2VirtClock32(physClockConf());
  }

  volatile uint32_t* ptrClockEnable()
  {
    return phys2VirtClock32(physClockEnable());
  }

  volatile uint32_t* ptrClockOscConf()
  {
    return phys2VirtClock32(physClockOscConf());
  }

  volatile uint32_t* ptrClockStatus()
  {
    return phys2VirtClock32(physClockStatus());
  }

  // --------------------------------------------------------------------------
  // Functions to return pointers to mapped GPIO registers
  // --------------------------------------------------------------------------

  volatile uint32_t* offset2VirtGPIO32(off_t map_offset)
  {
    return offset2Virt32(map_offset, m_gpio_base);
  }

  volatile uint32_t* phys2VirtGPIO32(off_t phys)
  {
    return phys2Virt32(phys, m_gpio_base, mapBaseGPIO());
  }

  volatile uint32_t* ptrGPIOReadLevel(const unsigned ipin)
  {
    return phys2VirtGPIO32(physGPIOReadLevel(ipin));
  }

  volatile uint32_t* ptrGPIODirection(const unsigned ipin)
  {
    return phys2VirtGPIO32(physGPIODirection(ipin));
  }

  volatile uint32_t* ptrGPIOSetLevel(const unsigned ipin)
  { 
    return phys2VirtGPIO32(physGPIOSetLevel(ipin));
  }

  volatile uint32_t* ptrGPIOClrLevel(const unsigned ipin)
  {
    return phys2VirtGPIO32(physGPIOClrLevel(ipin));
  }
  
  volatile uint32_t* ptrGPIORiseEdge(const unsigned ipin)
  {
    return phys2VirtGPIO32(physGPIORiseEdge(ipin));
  }

  volatile uint32_t* ptrGPIOFallEdge(const unsigned ipin)
  {
    return phys2VirtGPIO32(physGPIOFallEdge(ipin));
  }

  volatile uint32_t* ptrGPIOEdgeStat(const unsigned ipin)
  {
    return phys2VirtGPIO32(physGPIOEdgeStat(ipin));
  }

  volatile uint32_t* ptrGPIOAltFunc(const unsigned ipin)
  { 
    return phys2VirtGPIO32(physGPIOAltFunc(ipin));
  }

  void gpioSetLevel(const unsigned ipin)
  {
    *(ptrGPIOSetLevel(ipin)) = Bits::mask1Bit(ipin);
  }

  void gpioClrLevel(const unsigned ipin)
  {
    *(ptrGPIOClrLevel(ipin)) = Bits::mask1Bit(ipin);
  }

  // --------------------------------------------------------------------------
  // Functions to return pointers to mapped PWM registers
  // --------------------------------------------------------------------------

  volatile uint32_t* ptrPWMControl(const unsigned ipwm)
  {
    if(ipwm&0x2)
      if(ipwm&0x1)
	return phys2Virt32(physPWM3Control(), m_pwm13_base, mapBasePWM13());
      else 
	return phys2Virt32(physPWM2Control(), m_pwm02_base, mapBasePWM02());
    else
      if(ipwm&0x1)
	return phys2Virt32(physPWM1Control(), m_pwm13_base, mapBasePWM13());
      else 
	return phys2Virt32(physPWM0Control(), m_pwm02_base, mapBasePWM02());
  }

  volatile uint32_t* ptrPWMDutyCycle(const unsigned ipwm)
  {
    if(ipwm&0x2)
      if(ipwm&0x1)
	return phys2Virt32(physPWM3DutyCycle(), m_pwm13_base, mapBasePWM13());
      else 
	return phys2Virt32(physPWM2DutyCycle(), m_pwm02_base, mapBasePWM02());
    else
      if(ipwm&0x1)
	return phys2Virt32(physPWM1DutyCycle(), m_pwm13_base, mapBasePWM13());
      else 
	return phys2Virt32(physPWM0DutyCycle(), m_pwm02_base, mapBasePWM02());
  }

  volatile uint32_t* ptrPWMPeriod(const unsigned ipwm)
  {
    if(ipwm&0x2)
      if(ipwm&0x1)
	return phys2Virt32(physPWM3Period(), m_pwm13_base, mapBasePWM13());
      else 
	return phys2Virt32(physPWM2Period(), m_pwm02_base, mapBasePWM02());
    else
      if(ipwm&0x1)
	return phys2Virt32(physPWM1Period(), m_pwm13_base, mapBasePWM13());
      else 
	return phys2Virt32(physPWM0Period(), m_pwm02_base, mapBasePWM02());
  }

  // --------------------------------------------------------------------------
  // Functions to return pointers to mapped SSP registers
  // --------------------------------------------------------------------------

  volatile uint32_t* ptrSSPControl0(const unsigned issp)
  {
    if(issp==2)
      return phys2Virt32(physSSP3Control0(), m_ssp3_base, mapBaseSSP3());
    else if(issp==1)
      return phys2Virt32(physSSP2Control0(), m_ssp2_base, mapBaseSSP2());
    else 
      return phys2Virt32(physSSP1Control0(), m_ssp1_base, mapBaseSSP1());
  }

  volatile uint32_t* ptrSSPControl1(const unsigned issp)
  {
    if(issp==2)
      return phys2Virt32(physSSP3Control1(), m_ssp3_base, mapBaseSSP3());
    else if(issp==1)
      return phys2Virt32(physSSP2Control1(), m_ssp2_base, mapBaseSSP2());
    else 
      return phys2Virt32(physSSP1Control1(), m_ssp1_base, mapBaseSSP1());
  }

  volatile uint32_t* ptrSSPStatus(const unsigned issp)
  {
    if(issp==2)
      return phys2Virt32(physSSP3Status(), m_ssp3_base, mapBaseSSP3());
    else if(issp==1)
      return phys2Virt32(physSSP2Status(), m_ssp2_base, mapBaseSSP2());
    else 
      return phys2Virt32(physSSP1Status(), m_ssp1_base, mapBaseSSP1());
  }

  volatile uint32_t* ptrSSPInterruptTest(const unsigned issp)
  {
    if(issp==2)
      return phys2Virt32(physSSP3InterruptTest(), m_ssp3_base, mapBaseSSP3());
    else if(issp==1)
      return phys2Virt32(physSSP2InterruptTest(), m_ssp2_base, mapBaseSSP2());
    else 
      return phys2Virt32(physSSP1InterruptTest(), m_ssp1_base, mapBaseSSP1());
  }

  volatile uint32_t* ptrSSPDataRW(const unsigned issp)
  {
    if(issp==2)
      return phys2Virt32(physSSP3DataRW(), m_ssp3_base, mapBaseSSP3());
    else if(issp==1)
      return phys2Virt32(physSSP2DataRW(), m_ssp2_base, mapBaseSSP2());
    else 
      return phys2Virt32(physSSP1DataRW(), m_ssp1_base, mapBaseSSP1());
  }

  volatile uint32_t* ptrSSPTimeOut(const unsigned issp)
  {
    if(issp==2)
      return phys2Virt32(physSSP3TimeOut(), m_ssp3_base, mapBaseSSP3());
    else if(issp==1)
      return phys2Virt32(physSSP2TimeOut(), m_ssp2_base, mapBaseSSP2());
    else 
      return phys2Virt32(physSSP1TimeOut(), m_ssp1_base, mapBaseSSP1());
  }

  volatile uint32_t* ptrSSPPSPConfig(const unsigned issp)
  {
    if(issp==2)
     return phys2Virt32(physSSP3PSPConfig(), m_ssp3_base, mapBaseSSP3());
    else if(issp==1)
     return phys2Virt32(physSSP2PSPConfig(), m_ssp2_base, mapBaseSSP2());
    else 
     return phys2Virt32(physSSP1PSPConfig(), m_ssp1_base, mapBaseSSP1());
  }

  volatile uint32_t* ptrSSPTXTimeslotActive(const unsigned issp)
  {
    if(issp==2)
    return phys2Virt32(physSSP3TXTimeslotActive(), m_ssp3_base, mapBaseSSP3());
    else if(issp==1)
    return phys2Virt32(physSSP2TXTimeslotActive(), m_ssp2_base, mapBaseSSP2());
    else 
    return phys2Virt32(physSSP1TXTimeslotActive(), m_ssp1_base, mapBaseSSP1());
  }

  volatile uint32_t* ptrSSPRXTimeslotActive(const unsigned issp)
  {
    if(issp==2)
    return phys2Virt32(physSSP3RXTimeslotActive(), m_ssp3_base, mapBaseSSP3());
    else if(issp==1)
    return phys2Virt32(physSSP2RXTimeslotActive(), m_ssp2_base, mapBaseSSP2());
    else 
    return phys2Virt32(physSSP1RXTimeslotActive(), m_ssp1_base, mapBaseSSP1());
  }

  volatile uint32_t* ptrSSPTimeslotStatus(const unsigned issp)
  {
    if(issp==2)
      return phys2Virt32(physSSP3TimeslotStatus(), m_ssp3_base, mapBaseSSP3());
    else if(issp==1)
      return phys2Virt32(physSSP2TimeslotStatus(), m_ssp2_base, mapBaseSSP2());
    else 
      return phys2Virt32(physSSP1TimeslotStatus(), m_ssp1_base, mapBaseSSP1());
  }

  volatile uint32_t* ptrSSPAudioClockDiv(const unsigned issp)
  {
    if(issp==2)
      return phys2Virt32(physSSP3AudioClockDiv(), m_ssp3_base, mapBaseSSP3());
    else if(issp==1)
      return phys2Virt32(physSSP2AudioClockDiv(), m_ssp2_base, mapBaseSSP2());
    else
      return phys2Virt32(physSSP1AudioClockDiv(), m_ssp1_base, mapBaseSSP1());
  }

private:
  volatile void* makeMap(volatile void*& virtual_addr, off_t physical_addr, 
			 size_t length = mapSize());
  
  int             m_mmap_fd;
  volatile void*  m_gpio_base;
  volatile void*  m_pwm02_base;
  volatile void*  m_pwm13_base;
  volatile void*  m_ssp1_base;
  volatile void*  m_ssp2_base;
  volatile void*  m_ssp3_base;
  volatile void*  m_clock_base;
};

class SimulatedRegisters: private Bits
{
public:
  SimulatedRegisters()
  {
    m_gpio_level[0] = 0xFFFDEF1B;
    m_gpio_level[1] = 0xFFEFFFFF;
    m_gpio_level[2] = 0xFFFFFDFF;
    m_gpio_level[3] = 0x00000000;

    m_gpio_level[0] &= ~0x08023000;
    m_gpio_level[1] &= ~0x00100200;
    m_gpio_level[2] &= ~0x00010000;
    m_gpio_level[3] &= ~0x00000800;

    m_gpio_direction[0] = 0xC182BA18;
    m_gpio_direction[1] = 0x0012AAA1;
    m_gpio_direction[2] = 0x00010000;
    m_gpio_direction[3] = 0x00014800;

    m_gpio_fn[0] = 0x80040000;
    m_gpio_fn[1] = 0xA5254010;
    m_gpio_fn[2] = 0x69908010;
    m_gpio_fn[3] = 0xAAA5A8AA;
    m_gpio_fn[4] = 0xAAAAAAAA;
    m_gpio_fn[5] = 0x00010000;
    m_gpio_fn[6] = 0x10000000;
    m_gpio_fn[7] = 0x00000001;
  }

  ~SimulatedRegisters()
  {

  }

  // --------------------------------------------------------------------------
  // Functions to return pointers to simulated Clock registers
  // --------------------------------------------------------------------------

  volatile uint32_t* ptrClockEnable()
  {
    return &m_clock_enable;
  }

  // --------------------------------------------------------------------------
  // Functions to return pointers to simulated GPIO registers
  // --------------------------------------------------------------------------

  volatile uint32_t* ptrGPIOReadLevel(const unsigned ipin)
  {
    return &m_gpio_level[(ipin>>5)&0x3];
  }

  volatile uint32_t* ptrGPIODirection(const unsigned ipin)
  {
    return &m_gpio_direction[(ipin>>5)&0x3];
  }

  volatile uint32_t* ptrGPIOAltFunc(const unsigned ipin)
  { 
    return &m_gpio_fn[(ipin>>4)&0x7];
  }

  void gpioSetLevel(const unsigned ipin)
  {
    *(ptrGPIOReadLevel(ipin)) |= Bits::mask1Bit(ipin);
  }

  void gpioClrLevel(const unsigned ipin)
  {
    *(ptrGPIOReadLevel(ipin)) &= ~Bits::mask1Bit(ipin);
  }

  uint32_t simregGPIOLevel(const unsigned ireg) const
  {
    return m_gpio_level[ireg&0x03];
  }
  
  uint32_t simregGPIODirection(const unsigned ireg) const
  {
    return m_gpio_direction[ireg&0x03];
  }

  uint32_t simregGPIOFn(const unsigned ireg) const
  {
    return m_gpio_fn[ireg&0x07];
  }

  // --------------------------------------------------------------------------
  // Functions to return pointers to simulated SSP registers
  // --------------------------------------------------------------------------

  volatile uint32_t* ptrPWMControl(const unsigned ipwm)
  {
    return m_pwm_c + ipwm;
  }

  volatile uint32_t* ptrPWMDutyCycle(const unsigned ipwm)
  {
    return m_pwm_d + ipwm;
  }

  volatile uint32_t* ptrPWMPeriod(const unsigned ipwm)
  {
    return m_pwm_p + ipwm;
  }

  // --------------------------------------------------------------------------
  // Functions to return pointers to simulated SSP registers
  // --------------------------------------------------------------------------

  volatile uint32_t* ptrSSPControl0(const unsigned issp)
  {
    return m_ssp_c0 + issp;
  }

  volatile uint32_t* ptrSSPControl1(const unsigned issp)
  {
    return m_ssp_c1 + issp;
  }

  volatile uint32_t* ptrSSPStatus(const unsigned issp)
  {
    return m_ssp_status + issp;
  }

  volatile uint32_t* ptrSSPDataRW(const unsigned issp)
  {
    return m_ssp_data + issp;
  }

  volatile uint32_t* ptrSSPPSPConfig(const unsigned issp)
  {
    return m_ssp_cpsp + issp;
  }

private:
  uint32_t m_clock_enable;

  uint32_t m_gpio_level[4];
  uint32_t m_gpio_direction[4];
  uint32_t m_gpio_fn[8];

  uint32_t m_pwm_c[4];
  uint32_t m_pwm_d[4];
  uint32_t m_pwm_p[4];

  uint32_t m_ssp_c0[3];
  uint32_t m_ssp_c1[3];
  uint32_t m_ssp_cpsp[3];
  uint32_t m_ssp_status[3];
  uint32_t m_ssp_data[3];
};


#endif // ndef REGISTERS_HPP
