#include <iostream>
#include <cstdint>

//daq includes
#include "DataAcquisition.hh"
#include "ModuleManager.hh"
#include "HistoManager.hh"
#include "UiManager.hh"
#include "ConfigFileManager.hh"
#include "v2718.hh"
#include "v1785.hh"
#include "v1290N.hh"
#include "v1731.hh"


int main()
{
    VME_INTERFACE caen;

    ConfigFileManager *config = new ConfigFileManager();
    config->OpenConfigFile();
    config->IncrementRunNumber();
    config->GetRunNumber();
    config->CloseConfigFile();



	DataAcquisition *daq = new DataAcquisition();

	ModuleManager *module = new ModuleManager();

//	daq->Add(module);

	HistoManager *histo = new HistoManager();
    histo->Book();
    histo->Save();
//	daq->Add(histo);

	UiManager *vis = new UiManager();
//	daq->Add(vis);

//	daq.initialize();



//	if Keyboard:
//		daq.start()

//	if Keyboard:
//		daq.end()
	Module_v2718 *v2718 = new Module_v2718();
	v2718->InitializeVMEModule(&caen);

	Module_v1785 *v1785 = new Module_v1785();
	v1785->InitializeVMEModule(&caen);
	
	Module_v1290N *v1290N = new Module_v1290N();
	v1290N->InitializeVMEModule(&caen);
	
	Module_v1731 *v1731 = new Module_v1731();
	v1731->InitializeVMEModule(&caen);
	v1731->GetModuleBuffer(&caen);
	
	std::cout << "Working" <<endl;

    std::cout << "Hello World!\n";


  delete daq;
  delete module;
  delete histo;
  delete vis;
  delete v1785;
  delete v2718;
  delete v1290N;
  delete v1731;

  return 1;
}
