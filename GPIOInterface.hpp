#ifndef GPIOINTERFACE_H
#define GPIOINTERFACE_H

#include <sys/types.h>
#include <stdint.h>
#include <sys/mman.h>

class GPIOInterface {
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
        static const int channel[]; 

        // Returns Virtual (memory mapped) address for a given GPIO pin 
        volatile uint32_t* phys2VirtGPIO32(off_t phys, const unsigned ipin); 

        // Takes Physical Address and Returns pointer to memory mapped virtual address
        volatile uint32_t* phys2Virt32(off_t phys, volatile void* map_base_virt, off_t map_base_phys); 

        // --------------------------------------------------------------------------
        // GPIO register (PHYSICAL) address functions
        // --------------------------------------------------------------------------

        off_t physBaseGPIO1();                     
        off_t physBaseGPIO2();                     
        off_t physBaseGPIO3();                     
        off_t physBaseGPIO4();                     
        off_t physBaseGPIO5();                     
        off_t physBaseGPIO6();                     

        off_t gpio_offset_ctrl();                    
        off_t gpio_offset_oe();                     //enable the pins output capabilities. Its only function is to carry the pads configuration.
        off_t gpio_offset_datain();                 //register the data that is read from the GPIO pins
        off_t gpio_offset_dataout();                //setting the value of the GPIO output pins
        off_t gpio_offset_cleardataout();          
        off_t gpio_offset_setdataout();            
        

        // Returns physical address of GPIO Read Register for a given GPIO pin
        off_t physGPIOReadLevel(const unsigned ipin); 

        // Returns physical address of Output Enable Register for a given GPIO pin
        off_t physGPIODirection(const unsigned ipin); 

        // Returns physical address of Output Write Register for a given GPIO pin
        off_t physGPIOSetLevel(const unsigned ipin); 

        // --------------------------------------------------------------------------
        // static functions defining (physical) start of memory mapped regions
        // --------------------------------------------------------------------------

        off_t mapSize(); 
        off_t mapMask(); 

        off_t mapBaseGPIO1();
        off_t mapBaseGPIO2();
        off_t mapBaseGPIO3();
        off_t mapBaseGPIO4();
        off_t mapBaseGPIO5();
        off_t mapBaseGPIO6();


        // memory mapped address  stuff
        volatile void* makeMap(volatile void*& virtual_addr, off_t physical_addr, size_t length=4096);

        int             m_mmap_fd;

        volatile void*  m_gpio1_base;
        volatile void*  m_gpio2_base;
        volatile void*  m_gpio3_base;
        volatile void*  m_gpio4_base;
        volatile void*  m_gpio5_base;
        volatile void*  m_gpio6_base;
}; 
#endif
