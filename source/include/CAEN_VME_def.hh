#include <cstdint>
#include "CAENVMElib.h"


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
	
	void print_AM_Decode(CVAddressModifier AM){  //print the type addr modifier
		printf("The address modifier is in ");
		if (AM == cvA32_S_BLT){
			printf("A32 supervisory block transfer");
		}else if (AM == cvA32_S_PGM){
			printf("A32 supervisory program access");
		}else if (AM == cvA32_S_DATA){
			printf("A32 supervisory data access");
		}else if (AM == cvA32_S_MBLT){
			printf("A32 supervisory 64-bit block trnsfer");
		}else if (AM == cvA32_U_BLT){
			printf("A32 non-privileged block transfer");
		}else if (AM == cvA32_U_PGM){
			printf("A32 non-privileged program access");
		}else if (AM == cvA32_U_DATA){
			printf("A32 non-privileged data access");
		}else if (AM == cvA32_U_MBLT){
			printf("A32 non-privileged 64-bit block trnsfer");
		}else if (AM == cvA32_LCK){
			printf("A32 lock command");
			
		}else if (AM == cvA24_S_BLT){
			printf("A24 supervisory block transfer");
		}else if (AM == cvA24_S_PGM){
			printf("A24 supervisory program access");
		}else if (AM == cvA24_S_DATA){
			printf("A24 supervisory data access");
		}else if (AM == cvA24_S_MBLT){
			printf("A24 supervisory 64-bit block trnsfer");
		}else if (AM == cvA24_U_BLT){
			printf("A24 non-privileged block transfer");
		}else if (AM == cvA24_U_PGM){
			printf("A24 non-privileged program access");
		}else if (AM == cvA24_U_DATA){
			printf("A24 non-privileged data access");
		}else if (AM == cvA24_U_MBLT){
			printf("A24 non-privileged 64-bit block trnsfer");
		}else if (AM == cvA24_LCK){
			printf("A24 lock command");
			
		}else if (AM == cvA16_S){
			printf("A16 supervisory access");
		}else if (AM == cvA16_U){
			printf("A16 non-privileged");
		}else if (AM == cvA16_LCK){
			printf("A16 lock command");
			
		}else if (AM == cvCR_CSR){
			printf("CR/CSR space");
			
		}else if (AM == cvA40_BLT){  //Starting from here, the AMs are not yet implemented
			printf("A40 block transfer (MD32)");
		}else if (AM == cvA40_LCK){
			printf("A40 lock command");
		}else if (AM == cvA40){
			printf("A40 access");
			
		}else if (AM == cvA64){
			printf("A64 single trnsfer access");
		}else if (AM == cvA64_BLT){
			printf("A64 block transfer");
		}else if (AM == cvA64_MBLT){
			printf("A64 64-bit block transfer");
		}else if (AM == cvA64_LCK){
			printf("A64 lock command");
			
		}else if (AM == cvA3U_2eVME){
			printf("2eVME for 3U modules");
		}else if (AM == cvA6U_2eVME){
			printf("2eVME for 6U modules");
		}else{
			printf("unknown");
		}
		printf(" mode\n");
	}
	

};


struct V2718{
	static const int32_t base = 0xA1000000;
};

struct V1785{  
	static const int32_t base = 0xC1000000;
	static const CVAddressModifier am = cvA32_S_DATA;  // A32 supervisory data access.
                                                      // Check the compatibility before changing it!
};

struct V1290N{
	static const int32_t base = 0xD1000000;
	static const CVAddressModifier am = cvA32_S_DATA;
};





#endif  //CAEN_VME_DEF_H_