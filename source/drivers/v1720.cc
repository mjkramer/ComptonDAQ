#include <cstdio>
#include "v1720.hh"



int Module_v1720::InitializeVMEModule(VME_INTERFACE *vme){
	
	printf("\n\n\n");
	printf("***************************************************\n");
	printf("*    Initializing CAEN V1720 Digitizer            *\n");
	printf("***************************************************\n\n\n");
	
	int32_t Handle = vme->handle;
	
	V1720 v1720;
	CVAddressModifier AM;
	uint32_t base;
	
	AM = v1720.am;
    base = v1720.base;
	
	CAEN caen;
	
	
	
	v1720_Status(Handle, base, AM);
	
	return 1;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

double ModuleManager::GetModuleBuffer(){
	return 1.0;
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


uint32_t Module_v1720::v1720_RegisterRead(int32_t handle, uint32_t base, int offset, CVAddressModifier AM){
	uint32_t read;
	CVErrorCodes error_code;
	int error_status;
	CAEN caen;
	
	
	error_code = CAENVME_ReadCycle(handle, base+offset, &read, AM, cvD32);
	error_status = caen.ErrorDecode(error_code);
	
	return read;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

uint32_t Module_v1720::v1720_BufferFreeRead(int32_t handle, uint32_t base, CVAddressModifier AM){
	  return v1720_RegisterRead(handle, base, V1720_BUFFER_FREE, AM);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

uint32_t Module_v1720::v1720_BufferOccupancy(int32_t handle, uint32_t base, uint32_t channel, CVAddressModifier AM){
	uint32_t reg;
	reg = V1720_BUFFER_OCCUPANCY + (channel<<16);
	return v1720_RegisterRead(handle, base, reg, AM);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

uint32_t Module_v1720::v1720_BufferFree(int32_t handle, uint32_t base, int nbuffer, CVAddressModifier AM){
	int mode;
	
	mode = v1720_RegisterRead(handle, base, V1720_BUFFER_FREE, AM);
	if (nbuffer <= (1<<mode) ){
		v1720_RegisterWrite(handle, base, V1720_BUFFER_FREE, nbuffer, AM);
		return mode;
	}else
	
	return mode;
	
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......



void     Module_v1720::v1720_AcqCtl(int32_t handle, uint32_t base, uint32_t operation, CVAddressModifier AM){
	uint32_t reg;
	
	reg = v1720_RegisterRead(handle, base, V1720_ACQUISITION_CONTROL, AM);  
	
	switch (operation) {
	  case V1720_RUN_START:
		  v1720_RegisterWrite(handle, base, V1720_ACQUISITION_CONTROL, (reg | 0x4), AM);
	      break;
	  case V1720_RUN_STOP:
		  v1720_RegisterWrite(handle, base, V1720_ACQUISITION_CONTROL, (reg & ~(0x4)), AM);
	      break;
	  case V1720_REGISTER_RUN_MODE:
		  v1720_RegisterWrite(handle, base, V1720_ACQUISITION_CONTROL, (reg & ~(0x3)), AM);
	      break;
	  case V1720_SIN_RUN_MODE:
		  v1720_RegisterWrite(handle, base, V1720_ACQUISITION_CONTROL, (reg | 0x01), AM);
	      break;
	  case V1720_SIN_GATE_RUN_MODE:
		  v1720_RegisterWrite(handle, base, V1720_ACQUISITION_CONTROL, (reg | 0x02), AM);
	      break;
	  case V1720_MULTI_BOARD_SYNC_MODE:
		  v1720_RegisterWrite(handle, base, V1720_ACQUISITION_CONTROL, (reg | 0x03), AM);
	      break;
	  case V1720_COUNT_ACCEPTED_TRIGGER:
		  v1720_RegisterWrite(handle, base, V1720_ACQUISITION_CONTROL, (reg | 0x08), AM);
	      break;
	  case V1720_COUNT_ALL_TRIGGER:
		  v1720_RegisterWrite(handle, base, V1720_ACQUISITION_CONTROL, (reg & ~(0x08)), AM);
	      break;
	  default:
	      break;
	  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void     Module_v1720::v1720_ChannelCtl(int32_t handle, uint32_t base, uint32_t reg, uint32_t mask, CVAddressModifier AM){
	v1720_RegisterWrite(handle, base, reg, mask, AM);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void     Module_v1720::v1720_TrgCtl(int32_t handle, uint32_t base, uint32_t reg, uint32_t mask, CVAddressModifier AM){
	v1720_RegisterWrite(handle, base, reg, mask, AM);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void     Module_v1720::v1720_RegisterWrite(int32_t handle, uint32_t base, int offset, uint32_t value, CVAddressModifier AM){
	uint32_t write = value;
	CVErrorCodes error_code;
	int error_status;
	CAEN caen;
	
	error_code = CAENVME_WriteCycle(handle, base+offset, &write, AM, cvD32);
	error_status = caen.ErrorDecode(error_code);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void     Module_v1720::v1720_Reset(int32_t handle, uint32_t base, CVAddressModifier AM){
	v1720_RegisterWrite(handle, base, V1720_SW_RESET, 0, AM);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void     Module_v1720::v1720_Status(int32_t handle, uint32_t base, CVAddressModifier AM){
	printf("================================================\n");
	printf("V1720 at A32 0x%x\n", (int)base);
	printf("Board ID             : 0x%x\n", v1720_RegisterRead(handle, base, V1720_BOARD_ID, AM));
	printf("Board Info           : 0x%x\n", v1720_RegisterRead(handle, base, V1720_BOARD_INFO, AM));
	printf("Acquisition status   : 0x%8.8x\n", v1720_RegisterRead(handle, base, V1720_ACQUISITION_STATUS, AM));
	printf("================================================\n");
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int      Module_v1720::v1720_Setup(int32_t handle, uint32_t base, int mode, CVAddressModifier AM){
	
	switch (mode) {
	  case 0x0:
	    printf("--------------------------------------------\n");
	    printf("Setup Skip\n");
	    printf("--------------------------------------------\n");
	  case 0x1:
	    printf("--------------------------------------------\n");
	    printf("Trigger from FP, 8ch, 1Ks, postTrigger 800\n");
	    printf("--------------------------------------------\n");
	    v1720_RegisterWrite(handle, base, V1720_BUFFER_ORGANIZATION,  0x0A, AM);    // 1K buffer
	    v1720_RegisterWrite(handle, base, V1720_TRIG_SRCE_EN_MASK,    0x4000, AM);  // External Trigger
	    v1720_RegisterWrite(handle, base, V1720_CHANNEL_EN_MASK,      0xFF, AM);    // 8ch enable
	    v1720_RegisterWrite(handle, base, V1720_POST_TRIGGER_SETTING, 800, AM);     // PreTrigger (1K-800)
	    v1720_RegisterWrite(handle, base, V1720_ACQUISITION_CONTROL,   0x00, AM);   // Reset Acq Control
	    printf("\n");
	    break;
	  case 0x2:
	    printf("--------------------------------------------\n");
	    printf("Trigger from LEMO\n");
	    printf("--------------------------------------------\n");
	    v1720_RegisterWrite(handle, base, V1720_BUFFER_ORGANIZATION, 1, AM);
	    printf("\n");
	    break;
	  default:
	    printf("Unknown setup mode\n");
	    return -1;
	  }
	  v1720_Status(handle, base, AM);
	  return 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void     Module_v1720::v1720_info(int32_t handle, uint32_t base, int *nchannels, uint32_t *n32word, CVAddressModifier AM){
	int i, chanmask;
	// Evaluate the event size
	// Number of samples per channels
	
	uint32_t V1720_NSAMPLES_MODE[11] = { (1<<20), (1<<19), (1<<18), (1<<17), (1<<16), (1<<15)
				                        ,(1<<14), (1<<13), (1<<12), (1<<11), (1<<10)};
	
	*n32word = V1720_NSAMPLES_MODE[v1720_RegisterRead(handle, base, V1720_BUFFER_ORGANIZATION, AM)];
	
	// times the number of active channels
	chanmask = 0xff & v1720_RegisterRead(handle, base, V1720_CHANNEL_EN_MASK, AM); 
	*nchannels = 0;
	for (i=0;i<8;i++) {
	    if (chanmask & (1<<i))
	        *nchannels += 1;
	  }

	*n32word *= *nchannels;
	*n32word /= 2;   // 2 samples per 32bit word
	*n32word += 4;   // Headers
	
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

uint32_t Module_v1720::v1720_DataRead(int32_t handle, uint32_t base, uint32_t *pdata, uint32_t n32w, CVAddressModifier AM){
	uint32_t i;
	
	for (i=0;i<n32w;i++) {
	    *pdata = v1720_RegisterRead(handle, base, V1720_EVENT_READOUT_BUFFER, AM);
	    //    printf ("pdata[%i]:%x\n", i, *pdata); 
	    //    if (*pdata != 0xffffffff)
	    pdata++;
	    //    else
	    //      break;
	  }
	  return i;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

uint32_t Module_v1720::v1720_DataBlockRead(int32_t handle, uint32_t base, uint32_t *pdest, uint32_t *nentry, CVAddressModifier AM){
	CVErrorCodes error_code;
	AM = cvA32_S_DATA;
	int n = 0;
	int error_status;
	CAEN caen;
	
	
	error_code = CAENVME_MBLTReadCycle(handle, base, pdest, *nentry<<2, AM, &n);
	error_status = caen.ErrorDecode(error_code);
	
	return n;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void     Module_v1720::v1720_ChannelThresholdSet(int32_t handle, uint32_t base, uint32_t channel, uint32_t threshold, CVAddressModifier AM){
	uint32_t reg;
	reg = V1720_CHANNEL_THRESHOLD | (channel << 8);
	printf("base:0x%x reg:0x%x, threshold:%x\n", base, reg, threshold);
	v1720_RegisterWrite(handle, base, reg, (threshold & 0xFFF), AM);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void     Module_v1720::v1720_ChannelOUThresholdSet(int32_t handle, uint32_t base, uint32_t channel, uint32_t threshold, CVAddressModifier AM){
	uint32_t reg;
	reg = V1720_CHANNEL_OUTHRESHOLD | (channel << 8);
	printf("base:0x%x reg:0x%x, outhreshold:%x\n", base, reg, threshold);
	v1720_RegisterWrite(handle, base, reg, (threshold & 0xFFF), AM);
	
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void     Module_v1720::v1720_ChannelDACSet(int32_t handle, uint32_t base, uint32_t channel, uint32_t dac, CVAddressModifier AM){
	uint32_t reg;
	reg = V1720_CHANNEL_DAC | (channel << 8);
	printf("base:0x%x reg:0x%x, DAC:%x\n", base, reg, dac);
	v1720_RegisterWrite(handle, base, reg, (dac & 0xFFFF), AM);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int      Module_v1720::v1720_ChannelDACGet(int32_t handle, uint32_t base, uint32_t channel, uint32_t *dac, CVAddressModifier AM){
    uint32_t reg;
	int   status;

	reg = V1720_CHANNEL_DAC | (channel << 8);
	*dac = v1720_RegisterRead(handle, base, reg, AM);
	reg = V1720_CHANNEL_STATUS | (channel << 8);
	status = v1720_RegisterRead(handle, base, reg, AM);
	return status;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void     Module_v1720::v1720_ChannelSet(int32_t handle, uint32_t base, uint32_t channel, uint32_t what, uint32_t that, CVAddressModifier AM){
	uint32_t reg, mask;

	if (what == V1720_CHANNEL_THRESHOLD)   mask = 0x0FFF;
	if (what == V1720_CHANNEL_OUTHRESHOLD) mask = 0x0FFF;
	if (what == V1720_CHANNEL_DAC)         mask = 0xFFFF;
	reg = what | (channel << 8);
	printf("base:0x%x reg:0x%x, this:%x\n", base, reg, that);
	v1720_RegisterWrite(handle, base, reg, (that & 0xFFF), AM);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

uint32_t Module_v1720::v1720_ChannelGet(int32_t handle, uint32_t base, uint32_t channel, uint32_t what, CVAddressModifier AM){
    uint32_t reg, mask;

	if (what == V1720_CHANNEL_THRESHOLD)   mask = 0x0FFF;
	if (what == V1720_CHANNEL_OUTHRESHOLD) mask = 0x0FFF;
	if (what == V1720_CHANNEL_DAC)         mask = 0xFFFF;
	reg = what | (channel << 8);
	return v1720_RegisterRead(handle, base, reg, AM);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void     Module_v1720::v1720_ChannelConfig(int32_t handle, uint32_t base, uint32_t operation, CVAddressModifier AM){
	uint32_t reg;
	  
	v1720_RegisterWrite(handle, base, V1720_CHANNEL_CONFIG, 0x10, AM);
	reg = v1720_RegisterRead(handle, base, V1720_CHANNEL_CONFIG, AM);  
	printf("Channel_config1: 0x%x\n", v1720_RegisterRead(handle, base, V1720_CHANNEL_CONFIG, AM) );  
	switch (operation) {
	  case V1720_TRIGGER_UNDERTH:
		v1720_RegisterWrite(handle, base, V1720_CHANNEL_CONFIG, (reg | 0x40), AM);
	    break;
	  case V1720_TRIGGER_OVERTH:
		v1720_RegisterWrite(handle, base, V1720_CHANNEL_CONFIG, (reg & ~(0x40)), AM);
	    break;
	  default:
	    break;
	  }
	printf("Channel_config2: 0x%x\n", v1720_RegisterRead(handle, base, V1720_CHANNEL_CONFIG, AM) );
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void     Module_v1720::v1720_Align64Set(int32_t handle, uint32_t base, CVAddressModifier AM){
	v1720_RegisterWrite(handle, base, V1720_VME_CONTROL, V1720_ALIGN64, AM);
}






