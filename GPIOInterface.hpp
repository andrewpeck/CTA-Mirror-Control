/*
 * Interface to access Overo EarthSTORM COM, Texas Instruments AM3703 CPU
 * Directly accesses CPU Physical Memory at /dev/mem by mapping into Virtual
 * address space.
 */

#ifndef GPIOINTERFACE_H
#define GPIOINTERFACE_H

#include <sys/types.h>
#include <stdint.h>
#include <sys/mman.h>

class GPIOInterface
{
public:
    GPIOInterface();
    ~GPIOInterface();

    // Functions to return pointers to mapped GPIO registers
    volatile uint32_t* ptrGPIOReadLevel(const unsigned ipin);
    volatile uint32_t* ptrGPIODirection(const unsigned ipin);
    volatile uint32_t* ptrGPIOSetLevel(const unsigned ipin);

    void gpioSetLevel(const unsigned ipin);
    void gpioClrLevel(const unsigned ipin);

private:
    // --------------------------------------------------------------------------
    // GPIO register (PHYSICAL) Address Definitions
    // --------------------------------------------------------------------------

    static const off_t physBaseGPIO1             = 0x48310000;
    static const off_t physBaseGPIO2             = 0x49050000;
    static const off_t physBaseGPIO3             = 0x49052000;
    static const off_t physBaseGPIO4             = 0x49054000;
    static const off_t physBaseGPIO5             = 0x49056000;
    static const off_t physBaseGPIO6             = 0x49058000;

    static const off_t gpio_offset_ctrl          = 0x030;
    static const off_t gpio_offset_oe            = 0x034;  //enable the pins output capabilities. Its only function is to carry the pads configuration.
    static const off_t gpio_offset_datain        = 0x038;  //register the data that is read from the GPIO pins
    static const off_t gpio_offset_dataout       = 0x03C;  //setting the value of the GPIO output pins
    static const off_t gpio_offset_cleardataout  = 0x090;
    static const off_t gpio_offset_setdataout    = 0x094;

    // --------------------------------------------------------------------------
    // Functions to Return Addresses for GPIO pins
    // --------------------------------------------------------------------------

    // Adds register offset to correct base address to produce physical address
    off_t offset2adrGPIO(unsigned ipin, off_t offset);

    // Returns Virtual (memory mapped) address for a given GPIO pin
    volatile uint32_t* phys2VirtGPIO32(off_t phys, const unsigned ipin);

    // Takes Physical Address and Returns pointer to memory mapped virtual address
    volatile uint32_t* phys2Virt32(off_t phys, volatile void* map_base_virt, off_t map_base_phys);

    // Returns physical address of GPIO Read Register for a given GPIO pin
    off_t physGPIOReadLevel(const unsigned ipin);

    // Returns physical address of Output Enable Register for a given GPIO pin
    off_t physGPIODirection(const unsigned ipin);

    // Returns physical address of Output Write Register for a given GPIO pin
    off_t physGPIOSetLevel(const unsigned ipin);

    // Base addresses of memory mapped address space
    volatile void*  makeMap(volatile void*& virtual_addr, off_t physical_addr, size_t length=mapSize);
    int             m_mmap_fd;
    volatile void*  m_gpio1_base;
    volatile void*  m_gpio2_base;
    volatile void*  m_gpio3_base;
    volatile void*  m_gpio4_base;
    volatile void*  m_gpio5_base;
    volatile void*  m_gpio6_base;
};
#endif
