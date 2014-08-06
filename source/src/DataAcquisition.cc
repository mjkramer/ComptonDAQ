#include "DataAcquisition.hh"
#include "ConfigFileManager.hh"
#include "ModuleManager.hh"
#include "HistoManager.hh"
#include "UiManager.hh"
#include "v2718.hh"
#include "v1785.hh"
#include "v1290N.hh"
#include "v1731.hh"

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
    fHistoManager->Book(run_number + 1);

    modules.push_back(new Module_v2718()); // controller card
    modules.push_back(new Module_v1785()); //Peak sensing ADC
    modules.push_back(new Module_v1290N()); //TDC
    modules.push_back(new Module_v1731()); //Digitizer

    for(std::vector<ModuleManager*>::iterator i = modules.begin(); i != modules.end(); ++i){
    	(*i)->InitializeVMEModule(&caen);
    }



    return 0;




}


int DataAcquisition::StartRun(){
	fConfigFileManager->IncrementRunNumber();
	


	return 0;


}



int DataAcquisition::StopRun(){
	fConfigFileManager->CloseConfigFile();
	fHistoManager->Save();


	return 0;	
}
