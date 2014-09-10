
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
	return 0;
}

int ModuleManager::SetOnline(){
	return 0;
}

int ModuleManager::SetOffline(){
	return 0;
}

int ModuleManager::DataReady(){
	return 0;
}

int ModuleManager::DeleteBuffer(){
	return 0;
}

int ModuleManager::ResetModule(){
	return 0;
}

DataBlock* ModuleManager::GetModuleBuffer(){
	return NULL;
}


