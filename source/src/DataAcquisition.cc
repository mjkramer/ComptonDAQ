#include "DataAcquisition.hh"
#include "ConfigFileManager.hh"
#include "ModuleManager.hh"
#include "HistoManager.hh"
#include "UiManager.hh"
#include "v2718.hh"
#include "v1785.hh"
#include "v1290N.hh"
#include "v1730.hh"

extern "C" {
#include "keyhit.h"
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DataAcquisition::DataAcquisition(ConfigFileManager* fConfig, HistoManager* fHisto, UiManager* fUi):fConfigFileManager(fConfig), fHistoManager(fHisto), fUiManager(fUi){ 

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DataAcquisition::~DataAcquisition(){ 

  //clean up
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

void DataAcquisition::Initialize(){
    //update configuration file
    fConfigFileManager->OpenConfigFile();
    run_number = fConfigFileManager->GetRunNumber(); // LAST run number

    //initialize UiManager
    (*fUiManager)[UiKeys::knRunNumber] = run_number + 1;
    (*fUiManager)[UiKeys::knTakingData] = false;
    fUiManager->StartListener();

    //initialize histograms, trees and ouput file
    fHistoManager->Book(run_number + 1);

    //create modules
    Module_v2718 *v2718 = new Module_v2718(); // controller card
    modules.push_back(new Module_v1785()); //Peak sensing ADC
    modules.push_back(new Module_v1730()); //Digitizer
    //modules.push_back(new Module_v1290N()); //TDC

    //initialize VME modules
    v2718->InitializeVMEModule(); //controller card is not included in the module vector for ease of future use of the module vector
    for(std::vector<ModuleManager*>::iterator i = modules.begin(); i != modules.end(); ++i){
    	(*i)->InitializeVMEModule();
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DataAcquisition::StartRun(){
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

      //create DataBlock vector to hold the data
      std::vector<DataBlock*> data;

      //set modules offline
      for(std::vector<ModuleManager*>::iterator i = modules.begin(); i != modules.end(); ++i){
        (*i)->SetOffline();}


      //Read data blocks from all modules & save after a certain number of events was reached
      for(std::vector<ModuleManager*>::iterator i = modules.begin(); i != modules.end(); ++i){
	      data.push_back((*i)->GetModuleBuffer());
	      ++event_count;
     	  if(event_count%10000 == 0){
          fHistoManager->IntermediateSave();}
      }

      //set modules online again
      for(std::vector<ModuleManager*>::iterator i = modules.begin(); i != modules.end(); ++i){
        (*i)->SetOnline();
      }


      // Process acquired data
      fHistoManager->ProcessData(data);

      //delete DataBlock vector again
      for(std::vector<DataBlock*>::iterator i = data.begin(); i != data.end(); ++i){
	      delete (*i);
	      (*i) = 0;
      }
      
      }//end if(DataReady())

      fUiManager->ProcessIO();

  }//end while

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DataAcquisition::StopRun(){
  //get run number for terminal output reasons and close configuration file
	int run_number = fConfigFileManager->GetRunNumber();
	fConfigFileManager->CloseConfigFile();

  //save foot file
	fHistoManager->Save();

  //set modules offline
        for(std::vector<ModuleManager*>::iterator i = modules.begin(); i != modules.end(); ++i){
          (*i)->SetOffline();}

  //inform UiManager of changed run state
	(*fUiManager)[UiKeys::knTakingData] = false;	
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DataAcquisition::CheckKeyboardCommands(){
    int c = 0;

    if(!kbhit())
        return;

    c = getch();

    if(c == 'q'){
	    SetRunState(false);
    }

    if(c == 's'){
	    SetRunState(true);
    }

    else{
      std::cout << "Unknown key input." << std::endl;}
}

