#include "v2718.hh"

int Module_v2718:: InitializeVMEModule(){
	int error_code = 0;
	short board_number = 0; 
	int handle = 0;
	printf("Opening connection to VME controller.\n");
	
	error_code = CAENVME_Init(cvV2718, LINK, board_number, &handle);  //getting the handle
	VME.handle = handle;
	if (error_code == cvSuccess){
		printf("Initialization of V2718 completed successfully\n");
		return VME.success;
	}else if (error_code == cvBusError){
		printf("VME bus error during the cycle.\n");
		printf("Error: Could not open the connection to the VME controller!\n");
		return VME.failure;
	}else if (error_code == cvCommError){
		printf("Communication error\n");
		printf("Error: Could not open the connection to the VME controller!\n");
		return VME.failure;
	}else if (error_code == cvGenericError){
		printf("Encounter unspecified error\n");
		printf("Error: Could not open the connection to the VME controller!\n");
		return VME.failure;
	}else if (error_code == cvInvalidParam){
		printf("Invalid parameter\n");
		printf("Error: Could not open the connection to the VME controller!\n");
		return VME.failure;
	}else if (error_code == cvTimeoutError){
		printf("Timeout error\n");
		printf("Error: Could not open the connection to the VME controller!\n");
		return VME.failure;
	}
	printf("Setting transfer mode to A32 non-privileged block transfer...\n");
	VME.AM = cvA32_U_BLT;
	
	if(VME.AM != cvA32_U_BLT){
		printf("Error: Was not able to set transfer mode to A32!\n");
	 	return VME.failure;}

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

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



// The following functions must be execueted after initialization (i.e. after obtaining
// the handle).

void Module_v2718:: V2718_PulserConfSet(int32_t handle, int pulser, uint32_t period, uint32_t width, int pulseNo){
    CVPulserSelect pulSel;  // 0 for pulser A, 1 for pulser B
	CVOutputSelect outSel_1, outSel_2;  // the number represents the output line no.
	CVTimeUnits unit;
	
	switch (pulser){
	case 0:  //Pulser A
		pulSel = cvPulserA;
		outSel_1 = cvOutput0;
		outSel_2 = cvOutput1;
		break;
	case 1:  //Pulser B
		pulSel = cvPulserB;
		outSel_1 = cvOutput2;
		outSel_2 = cvOutput3;
		break;
	}
	
	if (period < 25*256){
	    period /= 25;
	    width /= 25;
	    unit = cvUnit25ns;
	    
	}else if (period < 1600*256){
	    period /= 1600;
	    width /= 1600;
	    unit = cvUnit1600ns;
	     
	}else if (period < 410000*256){
	    period /= 410000;
	    width /= 410000;
	    unit = cvUnit410us;
	      
	}else {
	    period /= 104000000;
	    width /= 104000000;
	    unit = cvUnit104ms;
	}
	
    if (width == 0)
	    width = 1;
	else if (width > 255)
	    width = 255;
    
    CAENVME_SetOutputConf(handle, outSel_1, cvDirect, cvActiveHigh, cvMiscSignals);
    CAENVME_SetOutputConf(handle, outSel_2, cvDirect, cvActiveHigh, cvMiscSignals);
    CAENVME_SetPulserConf(handle, pulSel, period, width, unit, pulseNo, cvManualSW, cvManualSW);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Module_v2718:: V2718_PulserStart(int32_t handle, int pulser){
	CVPulserSelect pulSel;
	
	switch (pulser){
	case 0:  //Pulser A
		pulSel = cvPulserA;
		break;
	case 1:  //Pulser B
		pulSel = cvPulserB;
		break;
	}
	CAENVME_StartPulser(handle, pulSel);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Module_v2718:: V2718_PulserStop(int32_t handle, int pulser){
	CVPulserSelect pulSel;
	
	switch (pulser){
	case 0:  //Pulser A
		pulSel = cvPulserA;
		break;
	case 1:  //Pulser B
		pulSel = cvPulserB;
		break;
	}
	CAENVME_StopPulser(handle, pulSel);
}














