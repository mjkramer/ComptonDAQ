#include <stdio.h>
#include <CAENDigitizer.h>

#include "v1720.hh"



int Module_v1720::InitializeVMEModule(VME_INTERFACE *vme){
	
  printf("\n\n\n");
  printf("***************************************************\n");
  printf("*    Initializing CAEN V1720 Digitizer...         *\n");
  printf("***************************************************\n\n\n");

  //int32_t Handle;
  //Handle = vme->handle;
  CAEN_DGTZ_ErrorCode error;
  CAEN caen;
  int err_status;

  V1720 v1720;  //struct V1720 stores the base adr, link type, link no. and node
  printf("Base Address of this CAEN V1720: 0x%x\n", v1720.base);
  
  
  int test_handle = 0;
  error = CAEN_DGTZ_OpenDigitizer(v1720.LinkType, v1720.LinkNum, v1720.ConetNode, v1720.base, &test_handle);
  err_status = caen.ErrorDecode_DGTZ(error);
  
  /*
  if(err_status == 0){  //error
		printf("Test handle: %d\n:", test_handle);
  		printf("Could not open CAEN v1720 VME digitizer card!\n");
      //Further error checking has to be implemented..
      return 0;
    }
  	*/

  	CAEN_DGTZ_BoardInfo_t	BoardInfo;
  	error = CAEN_DGTZ_GetInfo(test_handle, &BoardInfo);
  	err_status = caen.ErrorDecode_DGTZ(error);
  	
  	if(err_status == 0){  //error
  		printf("Could not get CAEN v1720 VME digitizer card board info!\n");
  		return 0;
  	}
    
  	printf("Connected to CAEN Digitizer Model %s\n", BoardInfo.ModelName);
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
