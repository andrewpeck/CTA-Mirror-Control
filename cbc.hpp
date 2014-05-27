#include <iostream>
#include <MirrorControlBoard.hpp>
#include <TLC3548_ADC.hpp>

class cbc
{
public:
    int initialize();

    void set_microstep  (unsigned usint);
    void enable         (int idrive);
    void disable        (int idrive);
    void enableusb      (int iusb);
    void disableusb     (int iusb);

    void testusb        ();
    void frequsb        (unsigned frequency);

    void step(unsigned idrive, int nstep, unsigned ndelay);
    void step_all(int nstep1, int nstep2, int nstep3, int nstep4, int nstep5, int nstep6, unsigned ndelay);

    void slew(unsigned idrive, MirrorControlBoard::Dir dir, unsigned ndelay);
    void slew_all(MirrorControlBoard::Dir dir, unsigned ndelay);

    void status();
    void measure(unsigned iadc, unsigned zchan, unsigned nmeas, unsigned nburn, unsigned ndelay, float volt_full, int hex_out);
    void measure_full(unsigned iadc, unsigned zchan, unsigned nmeas, unsigned ndelay, unsigned volt_full);
    void calibrate(unsigned idrive, unsigned nstep, unsigned ncycle, unsigned ndelay, unsigned iadc, unsigned nmeas, unsigned nburn, unsigned ichan);

    int usage();

private:
    Overo sys;
    Layout layout;
    MirrorControlBoard mcb;
    TLC3548_ADC adc; 

    static std::string usage_text;

    // fast integer square root
    unsigned julery_isqrt(unsigned long val);
};
