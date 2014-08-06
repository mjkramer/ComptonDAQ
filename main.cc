#include <iostream>
#include <cstdint>

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

    //pass some information to the run here..


    DataAcquisition *daq = new DataAcquisition(config, histo, vis);
      daq->Initialize();

      //if(key pressed):
      daq->StartRun();

      //if(key pressed):
      daq->StopRun();

  delete daq; 
  daq = 0;
  

  return 0;
}
