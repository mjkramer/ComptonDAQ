
#include "ModuleManager.hh"

int32_t ModuleManager::handle = 0;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ModuleManager::ModuleManager(){ 

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ModuleManager::~ModuleManager(){ 

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


int ModuleManager::InitializeVMEModule(){
	return 1;
}

int ModuleManager::SetOnline(){
	return 1;
}

int ModuleManager::SetOffline(){
	return 1;
}

int ModuleManager::DataReady(){
	return 1;
}

int ModuleManager::DeleteBuffer(){
	return 1;
}

int ModuleManager::ResetModule(){
	return 1;
}

double ModuleManager::GetModuleBuffer(){
	return 1.0;
}

int ModuleManager::CloseConnection(){
	return 1;
}
