/*
 * Interface to access GPIO Moduels of Overo EarthSTORM COM, Texas Instruments
 * AM3703 CPU Directly accesses CPU Physical Memory at /dev/mem by mapping into
 * virtual address space.
 */

#ifndef GPIOINTERFACE_H
#define GPIOINTERFACE_H

#include <sys/types.h>
#include <stdint.h>
#include <sys/mman.h>
#include <Layout.hpp>

class GPIOInterface
{
public:
    GPIOInterface();
    ~GPIOInterface();

    // Read GPIO by ipin (0-191)
    bool ReadLevel(const unsigned ipin);

    // Write GPIO by ipin (0-191)
    void WriteLevel(const unsigned ipin, bool level);

    // Get GPIO Direction In/Out
    bool GetDirection(const unsigned ipin);

    // Set GPIO Direction In/Out (0-191)
    void SetDirection(const unsigned ipin, bool dir);

    // Configures a given GPIO pin for a direction (1=OUT,0=in) and function (0-191)
    void Configure(const unsigned ipin, bool dir);

    // Configure Input/Output directions for ALL GPIOs
    void ConfigureAll();

private:

    const int nGPIO = 192;

    // Functions to return pointers to mapped GPIO registers
    volatile uint32_t* ptrGPIOReadLevel(const unsigned ipin);
    volatile uint32_t* ptrGPIODirection(const unsigned ipin);
    volatile uint32_t* ptrGPIOSetLevel(const unsigned ipin);

    void SetLevel(const unsigned ipin);
    void ClrLevel(const unsigned ipin);

    // --------------------------------------------------------------------------
    // GPIO register (PHYSICAL) Address Definitions
    // --------------------------------------------------------------------------

    const off_t physBaseGPIO1             = 0x48310000;
    const off_t physBaseGPIO2             = 0x49050000;
    const off_t physBaseGPIO3             = 0x49052000;
    const off_t physBaseGPIO4             = 0x49054000;
    const off_t physBaseGPIO5             = 0x49056000;
    const off_t physBaseGPIO6             = 0x49058000;

    const off_t gpio_offset_ctrl          = 0x030;
    const off_t gpio_offset_oe            = 0x034;  //enable the pins output capabilities. Its only function is to carry the pads configuration.
    const off_t gpio_offset_datain        = 0x038;  //register the data that is read from the GPIO pins
    const off_t gpio_offset_dataout       = 0x03C;  //setting the value of the GPIO output pins
    const off_t gpio_offset_cleardataout  = 0x090;
    const off_t gpio_offset_setdataout    = 0x094;

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

    // Create a virtual addressing space for a given physical address
    volatile void*  makeMap(volatile void*& virtual_addr, off_t physical_addr, size_t length=4096);

    // Memory mapped file descriptor
    int             m_mmap_fd;

    // Base addresses of memory mapped address space
    volatile void*  m_gpio1_base;
    volatile void*  m_gpio2_base;
    volatile void*  m_gpio3_base;
    volatile void*  m_gpio4_base;
    volatile void*  m_gpio5_base;
    volatile void*  m_gpio6_base;

    unsigned MaskPin (unsigned ipin);

    Layout layout;

};
#endif
