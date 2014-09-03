#include "v2718.hh"
#include "CAEN_VME_def.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int Module_v2718::InitializeVMEModule(){
	printf("****Initializing CAEN V2718****");

	int32_t Handle = 0;
    	CVErrorCodes error_code = CAENVME_Init(V2718::board_type, V2718::link, V2718::board_number, &Handle);

	if(CAEN::ErrorDecode(error_code)){
		printf("Failed to initialize the V2718 controller!\n");
		return 1;
	}

	ModuleManager::SetHandle(Handle);
	printf("  --  OK\n");
	return 0;	
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

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
