#include <stdio.h>
#include <CAENDigitizer.h>

#include "v1720.hh"
#include "v1720_constants.hh"


int Module_v1720::InitializeVMEModule(){
	int handle = 1;

	printf("Initializing CAEN v1720 VME digitizer card...\n");
  	error = CAEN_DGTZ_OpenDigitizer(v1720_LinkType, v1720_LinkNum, v1720_ConetNode, v1720_BaseAddress, &handle);
  	if(error){
  		printf("Could not open CAEN v1720 VME digitizer card!\n");
  		return 1;
  	}

  	CAEN_DGTZ_BoardInfo_t	BoardInfo;
  	error = CAEN_DGTZ_GetInfo(handle, &BoardInfo);
  	 if(error){
  		printf("Could not get CAEN v1720 VME digitizer card board info!\n");
  		return 1;
  	}
  	printf("Connected to CAEN Digitizer Model %s\n", BoardInfo.ModelName);
  	return 0;
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
