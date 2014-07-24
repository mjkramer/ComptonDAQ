#include "v1720.hh"

int Module_v1720::InitializeVMEModule(){
  //cout << "Initializing CAEN v2718 VME controller card..."
  
  //if (CAENVME_Init(nVMEBoard, 0, 0, &hVme)) != VME_SUCCESS) cout << "Error" << endl;
  
  //printf ("CAEN_VMEinit: hVme=%x ... done\n",hVme);
  return 1;

}



/* Standard fuctions:

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

*/