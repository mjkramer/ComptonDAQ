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

    daq->SetRunState(false);
    daq->Initialize();

    std::cout << "Press [s] to start the run and [q] to stop it!" << std::endl;
    int c = 0;
    c = getch();
    if(c == 's'){
	daq->SetRunState(true);
        daq->StartRun();}


  delete daq; 
  daq = 0;
  
  return 0;
}
