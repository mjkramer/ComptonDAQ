#include <iostream>


//msdaq includes
#include "DataAcquisition.hh"
#include "ModuleManager.hh"
#include "HistoManager.hh"
#include "VisManager.hh"


int main()
{


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


  std::cout << "Hello World!\n";


  delete daq;
  delete module;
  delete histo;
  delete vis;


  return 1;
}