#include <iostream>


//msdaq includes
#include "DataAcquisition.hh"
#include "ModuleManager.hh"
#include "HistoManager.hh"
#include "UiManager.hh"
#include "v2718.hh"


int main()
{


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
	v2718->InitializeVMEModule();
	std::cout << "Working" <<endl;

  std::cout << "Hello World!\n";


  delete daq;
  delete module;
  delete histo;
  delete vis;


  return 1;
}
