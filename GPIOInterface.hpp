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
#include "Layout.hpp"

class GPIOInterface
{
public:
    GPIOInterface();
    ~GPIOInterface();

    bool ReadLevel(int ipin);              // Read GPIO by ipin (0-191)
    void WriteLevel(int ipin, bool level); // Write GPIO by ipin (0-191)
    bool GetDirection(int ipin);           // Get GPIO Direction In/Out
    void SetDirection(int ipin, bool dir); // Set GPIO Direction In/Out (0-191)
    void ConfigureAll();                   // Configure Input/Output directions for ALL GPIOs
private:
    /* Sets the level of the given GPIO to HIGH */
    void SetLevel(int ipin);
    /* Sets the level of the given GPIO to LOW */
    void ClrLevel(int ipin);
    /* Returns the corresponding GPIO bank of the given pin */
    int bank(int ipin);

    /* Create a virtual addressing space for a given physical address */
    void* makeMap(off_t target);

    /* Given a base address and offset, returns a 32bit unsigned pointer to the
     * data in that address */
    volatile uint32_t* mappedAddress(off_t offset, void* mappedBaseAddress);

    /* File descriptor for /dev/mem */
    int             m_mmap_fd;

    /* Base addresses of memory mapped address space */
    void *m_gpio_base [6];

    /* Array of pointers to GPIO Control Registers */
    volatile uint32_t *gpioSetLevel [6];
    volatile uint32_t *gpioGetLevel [6];
    volatile uint32_t *gpioDirection[6];

    /* Function to mask out a bit for a given pin */
    uint32_t MaskPin (int ipin);

    // --------------------------------------------------------------------------
    // GPIO register (PHYSICAL) Address Definitions
    // --------------------------------------------------------------------------

    static const off_t ADR_GPIO_BASE[6];
    //= {0x48310000 ,0x49050000 ,0x49052000 ,0x49054000 ,0x49056000 ,0x49058000};
    static const off_t OFF_GPIO_OE               = 0x034;  //enable the pins output capabilities. Its only function is to carry the pads configuration.
    static const off_t OFF_GPIO_DATAIN           = 0x038;  //register the data that is read from the GPIO pins
    static const off_t OFF_GPIO_DATAOUT          = 0x03C;  //setting the value of the GPIO output pins
};
#endif
