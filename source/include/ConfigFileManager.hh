//Christian Dorfer 
//July 29, 2014
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <libconfig.h++>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef SOURCE_INCLUDE_CONFIGFILEMANAGER_H_
#define SOURCE_INCLUDE_CONFIGFILEMANAGER_H_

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

static const char *config_file = "config.cfg";

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class ConfigFileManager{
  
  public:
    ConfigFileManager();
   ~ConfigFileManager();

    void OpenConfigFile();
    void CloseConfigFile();
    int GetRunNumber();
    void IncrementRunNumber();

   private: 
	libconfig::Config cfg;      
   	};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif //SOURCE_INCLUDE_CONFIGFILEMANAGER

