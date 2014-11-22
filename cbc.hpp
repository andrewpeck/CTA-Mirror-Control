#ifndef CBC_H
#define CBC_H

#include <MirrorControlBoard.hpp>
#include <TLC3548_ADC.hpp>


class CBC
{
public:
    CBC(
            int  microsteps        = 8,
            int  steppingFrequency = 4000,
            bool highCurrentMode   = false,
            bool driveSR           = true,
            int  adcReadDelay      = 0,
            int  defaultADCSamples = 100,
            int  usbEnable         = 0,
            int  driveEnable       = 0
       );

    ~CBC();

    /* Power down sensors, encoders, motor drivers, adcs */
    void powerDown();
    /* Power up sensors, encoders, motor drivers, adcs */
    void powerUp();

    //===========================================================================
    //=USB Control===============================================================
    //===========================================================================

    struct USB {
        /* Enable/Disable/Check USB Ports #1-6
         * For Safety, this deliberately does not work with Ethernet USB1 */
        void enable     (int usb);
        void enableAll  ();
        void disable    (int usb);
        void disableAll ();
        bool isEnabled  (int usb);

        /* Alias the first USB port as ethernet */
        void resetEthernet();
        void enableEthernet();
    private:
        void disableEthernet();
        MirrorControlBoard mcb;
    };

    //===========================================================================
    //=Motor Driver Control======================================================
    //===========================================================================

    struct Driver {
    public:
        /*
         * Set number of micro steps to 1, 2, 4 or 8.
         * Attempts to write other values will simply be in vain
         */
        void setMicrosteps (int microsteps);
        int  getMicrosteps ();

        /* Enable/Disable/Check Motor Drive Channel Enables */
        void enable     (int idrive);
        void enableAll  ();
        void disable    (int idrive);
        void disableAll ();
        bool isEnabled  (int drive);

        /* Toggles Motor Driver Sleep Pin */
        void sleep();
        void wakeup();
        bool isAwake();

        /* Motor Driver High Current Mode */
        bool isHighCurrentEnabled();
        void enableHighCurrent();
        void disableHighCurrent();

        /*
         * A3977 Synchronous Rectification Mode.
         * Reduces power dissipation and elimitates
         * the need for external Shottky diodes.
         */
        void enableSR();
        void disableSR();
        bool isSREnabled();

        /*
         * Set default frequency for stepping.
         * This can be overwritten when calling stepping functions,
         * but setting a default will let you use a global frequency
         * and avoid having to specify each time you perform stepping
         */
        int  getSteppingFrequency();
        void setSteppingFrequency(int frequency);

        /* The RESET input (active low) sets the translator to a predefined
         * home state (see figures for home state conditions) and turns off
         * all of the DMOS outputs. The HOME output goes low and all
         * STEP inputs are ignored until the RESET input goes high.
         *
         * resetDriver() toggles this pin LOW then HIGH */
        void reset();

        /* Step drive some number of steps (positive to extend, negative to retract).
         *
         * NOTA BENE:
         *      "Frequency" is a CPU-dependent, load dependent variable. It
         *      can only be roughly calibrated for a particular processor,
         *      under particular circumstances.  A change in hardware necessitates
         *      a change in the calibration constant.
         *
         * nsteps is in MACRO-steps
         * frequency is in MACRO-steps per second
         *
         * OPTIONAL by default, frequency will take the value of steppingFrequency
         * unless otherwise specified.
         */
        void step (int drive, int nsteps);
        void step (int drive, int nsteps, int frequency);
        void stepAll (int nstep1, int nstep2, int nstep3, int nstep4, int nstep5, int nstep6);
        void stepAll (int nstep1, int nstep2, int nstep3, int nstep4, int nstep5, int nstep6, int frequency);

        /* Slew drive (DR) in given direction (move as far as possible)
         * 0 = extend
         * 1 = retract
         * 2 = none
         */
        void slew(int idrive, int dir);
        void slew(int idrive, int dir, int frequency);

        /* Slew all (enabled) drives in given direction */
        void slewAll(MirrorControlBoard::Dir dir);
        void slewAll(MirrorControlBoard::Dir dir, int frequency);

    private:
        int  steppingFrequency;

        /*
         * Some safety check to set a maximum and minimum
         * allowable stepping frequency
         */
        static const int  maximumSteppingFrequency = 0x1 << 16;
        static const int  minimumSteppingFrequency = 0;

        MirrorControlBoard mcb;
    };

    //===========================================================================
    //=Encoder Control===========================================================
    //===========================================================================

    struct Encoder {
        /* Controls Encoder Enable GPIO */
        void enable();
        void disable();
        bool isEnabled();

    private:
        MirrorControlBoard mcb;

    };

    //===========================================================================
    //=ADC Control/Readout=======================================================
    //===========================================================================

    struct ADC {
    public:

        /* Structure to hold ADC data */
        struct adcData {
            float voltage;
            float stddev;
        };

        adcData readEncoder (int encoder);
        adcData readEncoder (int encoder, int nsamples);

        adcData onboardTemp  ();
        adcData onboardTemp  (int nsamples);

        adcData externalTemp ();
        adcData externalTemp (int nsamples);

        /* A delay inserted between successive ADC reads */
        int  getReadDelay();
        void setReadDelay(int delay);

        /* Default number of samples to take in an measurement*/
        void setDefaultSamples(int nsamples);

    private:
        int readDelay;
        int defaultSamples;

        /* ADC voltage reference */
        static const float volt_full = 5.0;

        /* Reads data from any ADC/channel */
        adcData measure(int adc, int channel, int nsamples);

        TLC3548_ADC tlcadc;
        MirrorControlBoard mcb;
    };

    //===========================================================================
    //=External Sensors Control==================================================
    //==========================================================================

    /* Turns power on/off to the External sensor connections (unused?) */
    struct Sensor {
        void enable();
        void disable();
        bool isEnabled();

    private:
        MirrorControlBoard mcb;
    };

    struct USB usb;
    struct Driver driver;
    struct Encoder encoder;
    struct ADC adc;
    struct Sensor sensor;

private:
    GPIOInterface gpio;
    MirrorControlBoard mcb;

};

#endif
