#include <iostream>
#include <cstdint>

extern "C" {
#include "keyhit.h"
}

//daq includes
#include "DataAcquisition.hh"
#include "ModuleManager.hh"
#include "HistoManager.hh"
#include "UiManager.hh"
#include "ConfigFileManager.hh"


int main()
{
    
    ConfigFileManager *config = new ConfigFileManager();
    HistoManager *histo = new HistoManager();
    UiManager *vis = new UiManager(histo);
    DataAcquisition *daq = new DataAcquisition(config, histo, vis);

    daq->Initialize();

    int c = 0;
    std::cout << "Press [s] to start the run!" << std::endl;
    c = getch();

    if(c == 's'){
        daq->StartRun();}

  delete daq; 
  daq = 0;
  

  return 0;
}
