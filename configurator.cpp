#include "minIni.h"
#include "wordexp.h"
#include "configurator.hpp"

const char* configurator::config_file = "/etc/cbc.conf";

configurator::configurator() {};
configurator::~configurator(){}; 

int configurator::frequency()
{
    int frequency = getInt("global", "stepping_frequency"); 
    if (frequency == -1)
        frequency = 4000; 

    return (frequency); 
}

int configurator::microsteps()
{
    int microsteps = getInt("init", "microsteps"); 
    if (microsteps == -1)
        microsteps = 8; 

    return (microsteps); 
}

bool configurator::hicurrent()
{
    bool enabled = getBool("init", "hicurrent"); 
    return (enabled); 
}

bool configurator::drivesr()
{
    wordexp_t p;
    wordexp(config_file, &p, 0);
    minIni ini(p.we_wordv[0]);

    return (ini.getbool("init","drivesr",1)); 
}

bool configurator::usbenabled(int iusb)
{
    bool enabled = 0; 

    if (iusb==1)
        enabled = true; 
    else if (iusb==2) 
        enabled = getBool("init", "enable_usb_2"); 
    else if (iusb==3) 
        enabled = getBool("init", "enable_usb_3"); 
    else if (iusb==4) 
        enabled = getBool("init", "enable_usb_4"); 
    else if (iusb==5) 
        enabled = getBool("init", "enable_usb_5"); 
    else if (iusb==6) 
        enabled = getBool("init", "enable_usb_6"); 
    else if (iusb==7) 
        enabled = getBool("init", "enable_usb_7"); 
    else
        enabled = 0; 

    return (enabled); 
}

bool configurator::driveenabled(int idrive)
{
    bool enabled = 0; 

    if (idrive==1)
        enabled = getBool("init", "enable_drive_1"); 
    else if (idrive==2)
        enabled = getBool("init", "enable_drive_2"); 
    else if (idrive==3)
        enabled = getBool("init", "enable_drive_3"); 
    else if (idrive==4)
        enabled = getBool("init", "enable_drive_4"); 
    else if (idrive==5)
        enabled = getBool("init", "enable_drive_5"); 
    else if (idrive==6)
        enabled = getBool("init", "enable_drive_6"); 
    else 
        enabled = 0; 

    return (enabled); 
}


int configurator::getInt(std::string section, std::string var) 
{
    //bash-like expansion of config file name
    wordexp_t p;
    wordexp(config_file, &p, 0);
    //create minIni config file object
    minIni ini(p.we_wordv[0]);

    return (ini.getl(section,var,-1)); 
}

bool configurator::getBool(std::string section, std::string var) 
{
    wordexp_t p;
    wordexp(config_file, &p, 0);
    minIni ini(p.we_wordv[0]);

    return (ini.getbool(section,var,0)); 
}
