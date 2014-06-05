class configurator
{
public: 
    configurator(); 
    ~configurator(); 

    int frequency(); 
    int microsteps(); 
    bool hicurrent(); 
    bool drivesr(); 
    bool usbenabled(int iusb); 
    bool driveenabled(int idrive); 
private: 
    int getInt(std::string section, std::string var) ; 
    bool getBool(std::string section, std::string var) ; 
    static const char* config_file; 
};
