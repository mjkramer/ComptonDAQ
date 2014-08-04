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
    VME_INTERFACE caen;

    ConfigFileManager *config = new ConfigFileManager();
    HistoManager *histo = new HistoManager();
    UiManager *vis = new UiManager();

    //pass some information to the run here..


    DataAcquisition *daq = new DataAcquisition(*config, *histo, *vis);
      daq->initialize();

      //if(key pressed):
      daq->StartRun();

      //if(key pressed):
      daq->StopRun();



  delete daq;
  delete config;
  delete histo;
  delete vis;



  return 1;
}
