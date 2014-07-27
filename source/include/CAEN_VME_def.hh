#include <cstdint>
#include "CAENVMElib.h"
#include "ModuleManager.hh"

using namespace std;

#ifndef CAEN_VME_DEF_H_
#define CAEN_VME_DEF_H_


class CAEN{
  public:
	int ErrorDecode(CVErrorCodes error_code){  // Print the error message to screen 
		                                       // ,and return 1 for success and 0 for failure
		if (error_code == cvSuccess){
			//Print nothing
			return 1; 
			
		}else if (error_code == cvBusError){
			printf("VME bus error during the cycle.\n");
			return 0;
			
		}else if (error_code == cvCommError){
			printf("Communication error\n");
			return 0;
			
		}else if (error_code == cvGenericError){
			printf("Encounter unspecified error\n");
			return 0;
			
		}else if (error_code == cvInvalidParam){
			printf("Invalid parameter\n");
			return 0;
			
		}else if (error_code == cvTimeoutError){
			printf("Timeout error\n");
			return 0;
		}
	}
	
  protected:

    int32_t handle;
    CVAddressModifier AM;
    
};

struct V2718{
	static const int32_t base = 0xA1000000;
};

struct V1785{
	static const int32_t base = 0xC1000000;
};





#endif  //CAEN_VME_DEF_H_