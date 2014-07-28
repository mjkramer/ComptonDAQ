#include <iostream>


//msdaq includes
#include "DataAcquisition.hh"
#include "ModuleManager.hh"
#include "HistoManager.hh"
#include "UiManager.hh"
#include "v2718.hh"


int main()
{
    VME_INTERFACE *caen;

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
	Module_v1785 *v1785 = new Module_v1785();
	v2718->InitializeVMEModule(&caen);
	v1785->InitializeVMEModule(&caen);
	std::cout << "Working" <<endl;

  std::cout << "Hello World!\n";


  delete daq;
  delete module;
  delete histo;
  delete vis;


  return 1;
}
