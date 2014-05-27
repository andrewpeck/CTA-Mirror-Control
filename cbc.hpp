#include <iostream>
#include <MirrorControlBoard.hpp>
#include <TLC3548_ADC.hpp>

class cbc
{
public:

    // Configure GPIOs, Power up base electronics (not USBs)
    int initialize();

    // Set number of micro steps to 1, 2, 4 or 8.
    void set_microstep  (unsigned usint);

    // enable motor drive
    void enable         (int idrive);
    // disable motor drive
    void disable        (int idrive);

    // enable usb #1-7, or 0 to enable All
    void enableusb      (int iusb);
    // disable usb #1-7, or 0 to disable All
    void disableusb     (int iusb);

    // some temp functions for testing gpios
    void testusb        ();
    void freqloop       (unsigned nloop);
    void frequ          (unsigned frequency);
    void freqn          (unsigned frequency);

    //Step drive some number of steps (positive to extend, negative to retract). 
    void step(unsigned idrive, int nstep, unsigned frequency);
    //Step all drives some number of steps
    void step_all(int nstep1, int nstep2, int nstep3, int nstep4, int nstep5, int nstep6, unsigned frequency);

    // Slew drive (DR) in given direction (move as far as possible)
    void slew(unsigned idrive, MirrorControlBoard::Dir dir, unsigned frequency);
    // Slew all (enabled) drives in given direction (DIR, default 
    void slew_all(MirrorControlBoard::Dir dir, unsigned frequency);

    // Print drive status information. 
    void status();
    // Measure voltage of the specified ADC channel. For multiple passes will print average data. 
    void measure(unsigned iadc, unsigned zchan, unsigned nmeas, unsigned nburn, unsigned ndelay, float volt_full, int hex_out);
    // Measure voltage of the specified ADC channel. For multiple passes will print individual measurement data. 
    void measure_full(unsigned iadc, unsigned zchan, unsigned nmeas, unsigned ndelay, unsigned volt_full);
    // Measure voltage of the specified ADC channel.
    void calibrate(unsigned idrive, unsigned nstep, unsigned ncycle, unsigned ndelay, unsigned iadc, unsigned nmeas, unsigned nburn, unsigned ichan);

    // Print Program Usage 
    int usage();

private:
    GPIOInterface gpio;
    Layout layout;
    MirrorControlBoard mcb;
    TLC3548_ADC adc; 

    static std::string usage_text;

    // fast integer square root
    unsigned julery_isqrt(unsigned long val);
};
