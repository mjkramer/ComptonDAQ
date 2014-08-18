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
        (*fUiManager)[UiKeys::knTakingData] = true;
	fConfigFileManager->IncrementRunNumber();

	while(state){
		CheckKeyboardCommands(); //sets "state"
		if(modules[0]->DataReady()){

			//create DataBlock vector
			std::vector<DataBlock*> *data = new std::vector<DataBlock*>;

			//read out all modules
			for(std::vector<ModuleManager*>::iterator i = modules.begin(); i != modules.end(); ++i){
    		        data->push_back((*i)->GetModuleBuffer());}

			fHistoManager->ProcessData(data);

			//delete DataBlock vector
	    		for(std::vector<DataBlock*>::iterator i = data->begin(); i != data->end(); ++i){
	    			delete (*i);
				(*i) = 0;}
			delete data;
			data = 0;
		}

		fUiManager->ProcessIO();


	}//end while

	return 0;
}


			//Module_v1731* p1731_cast = dynamic_cast<Module_v1731*>((modules[1]));
			//if(p1731_cast){
			//		if(p1731_cast->DataReady()){
			//			fHistoManager->ProcessData(p1731_cast->GetModuleBuffer());}










int DataAcquisition::StopRun(){
	fConfigFileManager->CloseConfigFile();
	fHistoManager->Save();
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
