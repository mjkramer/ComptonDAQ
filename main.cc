#include <iostream>
#include <cstdint>

//msdaq includes
#include "DataAcquisition.hh"
#include "ModuleManager.hh"
#include "HistoManager.hh"
#include "VisManager.hh"
#include "v2718.hh"
#include "v1785.hh"


int main()
{
    VME_INTERFACE caen;

	DataAcquisition *daq = new DataAcquisition();

	ModuleManager *module = new ModuleManager();

//	daq->Add(module);

	HistoManager *histo = new HistoManager();
  histo->Book();
  histo->Save();
//	daq->Add(histo);

	VisManager *vis = new VisManager();
//	daq->Add(vis);

//	daq.initialize();



//	if Keyboard:
//		daq.start()

//	if Keyboard:
//		daq.end()
	Module_v2718 *v2718 = new Module_v2718();
	v2718->InitializeVMEModule(&caen);
	std::cout << "Working" <<endl;

	Module_v1785 *v1785 = new Module_v1785();
	v1785->InitializeVMEModule(&caen);

  std::cout << "Hello World!\n";


  delete daq;
  delete module;
  delete histo;
  delete vis;


  return 1;
}
