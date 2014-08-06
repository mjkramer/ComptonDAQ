#include <cstdint>
#include "CAENVMElib.h"
#include "CAENDigitizer.h"


using namespace std;

#ifndef CAEN_VME_DEF_H_
#define CAEN_VME_DEF_H_


namespace CAEN{

	int ErrorDecode(CVErrorCodes error_code){  // Print the error message to screen 
		                                       // ,and return 1 for success and 0 for failure
		if (error_code == cvSuccess){
			//Print nothing
			return 0; 
			
		}else if (error_code == cvBusError){
			printf("VME bus error during the cycle.\n");
			return 1;
			
		}else if (error_code == cvCommError){
			printf("Communication error\n");
			return 1;
			
		}else if (error_code == cvGenericError){
			printf("Encounter unspecified error\n");
			return 1;
			
		}else if (error_code == cvInvalidParam){
			printf("Invalid parameter\n");
			return 1;
			
		}else if (error_code == cvTimeoutError){
			printf("Timeout error\n");
			return 1;
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
	
	int ErrorDecode_DGTZ(CAEN_DGTZ_ErrorCode error_code){
		int err = 0;  // 1 for success, 0 for failure
		
		switch(error_code){
		    case CAEN_DGTZ_Success:  //success
		    	printf("Operation completed successfully\n");
		    	err = 1;
		    	break;
		    	
		    case CAEN_DGTZ_CommError:
		    	printf("Communication error\n");
		    	err = 0;
		    	break;
		    	
		    case CAEN_DGTZ_GenericError:
		        printf("Unspecified error\n");
		        err = 0;
		        break;
		        
		    case CAEN_DGTZ_InvalidParam:
		    	printf("Invalid parameter\n");
		    	err = 0;
		    	break;
		    	
		    case CAEN_DGTZ_InvalidLinkType:
		    	printf("Invalid Link Type\n");
		    	err = 0;
		    	break;
		    	
		    case CAEN_DGTZ_InvalidHandle:
		    	printf("Invalid device handle\n");
		    	err = 0;
		    	break;
		    	
		    case CAEN_DGTZ_MaxDevicesError:
		    	printf("Maximum number of devices exceeded\n");
		    	err = 0;
		    	break;
		    	
		    case CAEN_DGTZ_BadBoardType:
		    	printf("The operation is not allowed on this type of board\n");
		    	err = 0;
		    	break;
		    	
		    case CAEN_DGTZ_BadInterruptLev:
		    	printf("The interrupt level is not allowed\n");
		    	err = 0;
		    	break;
		    	
		    case CAEN_DGTZ_BadEventNumber:
		    	printf("The event number is bad\n");
		    	err = 0;
		    	break;
		    	
		    case CAEN_DGTZ_ReadDeviceRegisterFail:
		        printf("Unable to read the registry\n");
		        err = 0;
		        break;
		        
		    case CAEN_DGTZ_WriteDeviceRegisterFail:
		    	printf("Unable to write into the registry\n");
		    	err = 0;
		        break;
		        
		    case CAEN_DGTZ_InvalidChannelNumber:
		    	printf("The channel number is invalid\n");
		    	err = 0;
		    	break;
		    	
		    case CAEN_DGTZ_ChannelBusy:
		        printf("The Channel is busy\n");
		        err = 0;
		        break;
		        
		    case CAEN_DGTZ_FPIOModeInvalid:
		    	printf("Invalid FPIO Mode\n");
		    	err = 0;
		    	break;
		    	
		    case CAEN_DGTZ_WrongAcqMode:
		        printf("Wrong acquisition mode\n");
		        err = 0;
		        break;
		        
		    case CAEN_DGTZ_FunctionNotAllowed:
		        printf("This function is not allowed for this module\n");
		        err = 0;
		        break;
		        
		    case CAEN_DGTZ_Timeout:
		    	printf("Communication Timeout\n");
		    	err = 0;
		    	break;
		    	
		    case CAEN_DGTZ_InvalidBuffer:
		    	printf("The buffer is invalid\n");
		    	err = 0;
		    	break;
		    	
		    case CAEN_DGTZ_EventNotFound:
		    	printf("The event is not found\n");
		    	err = 0;
		    	break;
		    	
		    case CAEN_DGTZ_InvalidEvent:
		    	printf("The event is invalid\n");
		    	err = 0;
		    	break;
		    	
		    case CAEN_DGTZ_OutOfMemory:
		    	printf("Out of memory\n");
		    	err = 0;
		    	break;
		    	
		    case CAEN_DGTZ_CalibrationError:
		        printf("Unable to calibrate the board\n");
		        err = 0;
		        break;
		        
		    case CAEN_DGTZ_DigitizerNotFound:
		        printf("Unable to open the digitizer\n");
		        err = 0;
		        break;
		        
		    case CAEN_DGTZ_DigitizerAlreadyOpen:
		    	printf("The Digitizer is already open\n");
		    	err = 0;
		    	break;
		    	
		    case CAEN_DGTZ_DigitizerNotReady:
		    	printf("The Digitizer is not ready to operate\n");
		    	err = 0;
		    	break;
		    	
		    case CAEN_DGTZ_InterruptNotConfigured:
		    	printf("The Digitizer has not the IRQ configured\n");
		    	err = 0;
		    	break;
		    	
		    case CAEN_DGTZ_DigitizerMemoryCorrupted:
		    	printf("The digitizer flash memory is corrupted\n");
		    	err = 0;
		    	break;
		    	
		    case CAEN_DGTZ_DPPFirmwareNotSupported:
		    	printf("CAEN_DGTZ_DPPFirmwareNotSupported\n");
		    	err = 0;
		    	break;
		    	
		    case CAEN_DGTZ_InvalidLicense:
		    	printf("Invalid Firmware License\n");
		    	err = 0;
		    	break;
		    	
		    case CAEN_DGTZ_InvalidDigitizerStatus:
		    	printf("The digitizer is found in a corrupted status\n");
		    	err = 0;
		    	break;
		    	
		    case CAEN_DGTZ_NotYetImplemented:
		    	printf("The function is not yet implemented\n");
		    	err = 0;
		    	break;
		    	
		    default:
		    	printf("Unknown DGTZ error code");
		        err = 0;    
		}
		return err;
	}
	

}//end namespace CAEN


namespace V2718{
	static const uint32_t base                       = 0xA1000000;
	static const short board_number					 = 0;
}

struct V1785{  
	static const uint32_t base                       = 0xC1000000;
	static const CVAddressModifier am                = cvA32_S_DATA;  // A32 supervisory data access.
                                                                     // Check the compatibility before changing it!
};

struct V1290N{
	static const uint32_t base                       = 0xD1000000;
	static const CVAddressModifier am                = cvA32_S_DATA;
};

struct V1731{
	static const uint32_t base                      = 0x11110000;
	static const int LinkNum                        = 3;
	static const int ConetNode                      = 0;
	static const CVAddressModifier am               = cvA32_S_DATA;
	
	static const CAEN_DGTZ_ConnectionType LinkType  = CAEN_DGTZ_OpticalLink;

};





#endif  //CAEN_VME_DEF_H_