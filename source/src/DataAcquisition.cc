#include "DataAcquisition.hh"
#include "ConfigFileManager.hh"
#include "ModuleManager.hh"
#include "HistoManager.hh"
#include "UiManager.hh"
#include "v2718.hh"
#include "v1785.hh"
#include "v1290N.hh"
#include "v1731.hh"

extern "C" {
#include "keyhit.h"
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DataAcquisition::DataAcquisition(ConfigFileManager* fConfig, HistoManager* fHisto, UiManager* fUi):fConfigFileManager(fConfig), fHistoManager(fHisto), fUiManager(fUi){ 

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DataAcquisition::~DataAcquisition(){ 
	delete fConfigFileManager;
	delete fHistoManager;
	delete fUiManager;

	fConfigFileManager = 0;
	fHistoManager = 0;
	fUiManager = 0;

	for(std::vector<ModuleManager*>::iterator i = modules.begin(); i != modules.end(); ++i){
		delete *i;
		*i = 0;
	}	
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int DataAcquisition::Initialize(){
    fConfigFileManager->OpenConfigFile();
    run_number = fConfigFileManager->GetRunNumber();

    (*fUiManager)[UiKeys::knRunNumber] = run_number + 1;
    (*fUiManager)[UiKeys::knTakingData] = false;
    fUiManager->StartListener();

    fHistoManager->Book(run_number + 1);

    Module_v2718 *v2718 = new Module_v2718(); // controller card
    modules.push_back(new Module_v1785()); //Peak sensing ADC
    modules.push_back(new Module_v1731()); //Digitizer
    //modules.push_back(new Module_v1290N()); //TDC

    v2718->InitializeVMEModule();
    for(std::vector<ModuleManager*>::iterator i = modules.begin(); i != modules.end(); ++i){
    	(*i)->InitializeVMEModule();
    }

    return 0;
}


int DataAcquisition::StartRun(){
  fConfigFileManager->IncrementRunNumber();
  (*fUiManager)[UiKeys::knTakingData] = true;

  //Set modules online
  for(std::vector<ModuleManager*>::iterator i = modules.begin(); i != modules.end(); ++i){
    (*i)->SetOnline();
  }

  std::cout << endl << "DAQ is running..." << std::endl;
  long event_count = 0;  
	
  while(state){
    CheckKeyboardCommands(); //sets variable "state"

    if(modules[0]->DataReady()){
      //create DataBlock vector
      std::vector<DataBlock*> data;

      //set modules offline
      for(std::vector<ModuleManager*>::iterator i = modules.begin(); i != modules.end(); ++i){
        (*i)->SetOffline();}

      //Read data blocks from all modules & save if necessary
      for(std::vector<ModuleManager*>::iterator i = modules.begin(); i != modules.end(); ++i){
	data.push_back( (*i)->GetModuleBuffer());
	++event_count;
     	if(event_count%200000 == 0){fHistoManager->IntermediateSave();
	printf("Event count: %l\n", event_count);}
      }

      //set modules online
      for(std::vector<ModuleManager*>::iterator i = modules.begin(); i != modules.end(); ++i){
        (*i)->SetOnline();}

      // Process acquired data
      fHistoManager->ProcessData(data);

      //Delete DataBlocks
      for(std::vector<DataBlock*>::iterator i = data.begin(); i != data.end(); ++i){
	delete (*i);
	(*i) = 0;}
      }

      fUiManager->ProcessIO();

 

  }//end while

  return 0;
}


int DataAcquisition::StopRun(){
	int run_number = fConfigFileManager->GetRunNumber();
	fConfigFileManager->CloseConfigFile();
	fHistoManager->Save(run_number);
	(*fUiManager)[UiKeys::knTakingData] = false;
	return 0;	
}


void DataAcquisition::CheckKeyboardCommands(){
    int c = 0;

    if(!kbhit())
        return;

    c = getch();

    if(c == 'q'){
	SetRunState(false);}

    if(c == 's'){
	SetRunState(true);}

}
