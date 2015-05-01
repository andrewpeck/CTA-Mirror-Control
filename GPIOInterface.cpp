#include <stdio.h>
#include <math.h> // needed for floor
#include <fcntl.h> // needed to open /dev/mem
#include <stdexcept>

#include "GPIOInterface.hpp"

#define MMAPFAIL ((void*)-1)
#define MAP_SIZE 4096UL
#define MAP_MASK (MAP_SIZE - 1)

//------------------------------------------------------------------------------
// Constructor + Destructor
//------------------------------------------------------------------------------

GPIOInterface::GPIOInterface():
    m_mmap_fd(-1),
    m_gpio_base(),
    gpioSetLevel(),
    gpioGetLevel(),
    gpioDirection()
{
    m_mmap_fd = open("/dev/mem", O_RDWR | O_SYNC);
    if(m_mmap_fd<0)
    {
        perror("open(\"/dev/mem\")");
        exit(EXIT_FAILURE);
    }

    //      virtual adr   physical adr
    for (int i=0; i<6; i++)
        m_gpio_base[i] = makeMap(ADR_GPIO_BASE[i]);

    for (int ibank=0; ibank<=5; ibank++) {
        gpioGetLevel  [ibank] = mappedAddress(OFF_GPIO_DATAIN,  m_gpio_base[ibank]);
        gpioSetLevel  [ibank] = mappedAddress(OFF_GPIO_DATAOUT, m_gpio_base[ibank]);
        gpioDirection [ibank] = mappedAddress(OFF_GPIO_OE,      m_gpio_base[ibank]);
    }
}

GPIOInterface::~GPIOInterface()
{
    // Unmap everything
    for (int i=0; i<6; i++)
        munmap(m_gpio_base[i], 4096);
}

//------------------------------------------------------------------------------
// Public Members
//------------------------------------------------------------------------------

bool GPIOInterface::ReadLevel(int ipin)
{
    bool level = *gpioGetLevel[bank(ipin)] & MaskPin(ipin);
    return level;
}

void GPIOInterface::WriteLevel(int ipin, bool level)
{
    if (level)
        SetLevel(ipin);
    else
        ClrLevel(ipin);
}

bool GPIOInterface::GetDirection(int ipin)
{
    bool dir = static_cast<bool>(*gpioDirection[bank(ipin)] & MaskPin(ipin));
    return dir;
}

void GPIOInterface::SetDirection(int ipin, bool dir)
{
    if (dir)
        *gpioDirection[bank(ipin)] |= (MaskPin(ipin));
    else
        *gpioDirection[bank(ipin)] &= ~(MaskPin(ipin));
}

void GPIOInterface::ConfigureAll()
{
    for (int i=0; i<192; i++) {
        int config = Layout::gpioConfiguration(i);
        if (config < -1 || config > 1)
            throw std::runtime_error("Invalid GPIO Configuration");
        else
            SetDirection(i,config); //0==output, 1==input
    }
}

//------------------------------------------------------------------------------
// Private Members
//------------------------------------------------------------------------------


void GPIOInterface::SetLevel(int ipin)
{
    // Write a One to the bit specified by ipin
    *gpioSetLevel[bank(ipin)] |= MaskPin(ipin);
}

void GPIOInterface::ClrLevel(int ipin)
{
    // Write a zero to the bit specified by ipin
    *gpioSetLevel[bank(ipin)] &= ~(MaskPin(ipin));
}

volatile uint32_t* GPIOInterface::mappedAddress(off_t offset, void* mappedBaseAddress)
{
    void* mappedAddress = static_cast<char *>(mappedBaseAddress) + offset;
    return reinterpret_cast<volatile uint32_t*>(mappedAddress);
}

void* GPIOInterface::makeMap(off_t target)
{
    void *map_base = mmap(0, MAP_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED,
            m_mmap_fd, target & ~MAP_MASK);
    if (map_base == (void*) -1) {
        fprintf(stderr, "ERROR: Failure in mapping virtual memory for CBC GPIO Controller\n");
        exit(EXIT_FAILURE);
    }

    void *virt_address = static_cast<char *>(map_base) + (target & MAP_MASK);

    return virt_address;
}

uint32_t GPIOInterface::MaskPin (int ipin)
{
    return (0x1 << (ipin % 32));
}

int GPIOInterface::bank(int ipin)
{
    return (floor(ipin/32));
}

const off_t GPIOInterface::ADR_GPIO_BASE []= {0x48310000 ,0x49050000 ,0x49052000 ,0x49054000 ,0x49056000 ,0x49058000};
