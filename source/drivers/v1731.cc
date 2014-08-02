#include <cstdio>
#include "v1731.hh"
#include <unistd.h>



int Module_v1731::InitializeVMEModule(VME_INTERFACE *vme){
	
	printf("\n\n\n");
	printf("***************************************************\n");
	printf("*    Initializing CAEN V1731 Digitizer            *\n");
	printf("***************************************************\n\n\n");
	
	int32_t Handle = vme->handle;
	
	V1731 v1731;
	CVAddressModifier AM;
	uint32_t base;
	
	AM = v1731.am;
    base = v1731.base;
	
	CAEN caen;
	
	
	printf("-------------------Setting-------------------------\n\n");
	
	v1731_RegisterWrite(Handle, base, V1731_ACQUISITION_CONTROL, 0x0, AM);  //Reset acquisition
	v1731_RegisterWrite(Handle, base, V1731_SW_RESET, 0, AM);  //Clean buffer block 1
	
	
	v1731_RegisterWrite(Handle, base, V1731_ZS_NSAMP, 0,AM);
	v1731_RegisterWrite(Handle, base, V1731_ZS_NSAMP_CH2, 0, AM);
	
	printf("Enable ONLY CH0 and 2.\n");
	// 0x8120
	v1731_RegisterWrite(Handle, base, V1731_CHANNEL_EN_MASK, 5, AM); // Bit[2] and Bit[0] = 1
	
	printf("Enable external trigger.\n");
	// 0x810C
	v1731_RegisterWrite(Handle, base, V1731_TRIG_SRCE_EN_MASK, 0x40000000, AM);  //Bit[30] = 1
	
	printf("Setting post trigger sample number to be 0...\n");
	// 0x8114
	v1731_RegisterWrite(Handle, base, V1731_POST_TRIGGER_SETTING, 0, AM);  //All pre trigger samples
	
	// 0x8000
	printf("Setting sampling rate to be 1GS/s...\n");
	v1731_RegisterWrite(Handle, base, V1731_CHANNEL_CONFIG, 0x1010, AM);  // Bit[12] = 1 for 1GS/s sampling rate,
	                                                                      // Bit[4]  = 1 for Memory sequential access
	// 0x800C
	printf("Setting number of buffer block(s) = 1\n");
	v1731_RegisterWrite(Handle, base, V1731_BUFFER_ORGANIZATION,  0, AM);  //Setting 1 buffer block
	
	printf("\n");
	printf("---------------------------------------------------\n\n");
	
	v1731_Status(Handle, base, AM);
	
	
	return 1;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

double Module_v1731::GetModuleBuffer(VME_INTERFACE *vme){  //testing
	
	CVErrorCodes error_code;
	int error_status;
	
	V1731 v1731;
	
	int32_t Handle = vme->handle;
	
	
	CAEN caen;
		
    uint32_t              event[125000];  // 4 MBytes in the buffer, 32 bytes per element. Nr_of_elem == 1000^2*4 /32 == 125000
    CVDataWidth           datawidth[125000];
    CVAddressModifier     AM[125000];
    uint32_t              base[125000];
    CVErrorCodes          err_code[125000];
    
    for(int i=0; i<125000; i++){
    	event[i]         = 0;  //initialize the array
    	datawidth[i]     = cvD32;
    	AM[i]            = v1731.am;
    	base[i]          = v1731.base;
    	err_code[i]      = cvSuccess;
    }
    
    printf("Memory cleared...\n");
    
    v1731_RegisterWrite(Handle, v1731.base, V1731_SW_CLEAR, 0x1, v1731.am);
    
    printf("Data acquisition starts...\n");
    
    
    // 0x8100
	v1731_RegisterWrite(Handle, v1731.base, V1731_ACQUISITION_CONTROL, 0x4, v1731.am);  //Bit[2] = 1 for acquisition run
	
    usleep(75000);  //wait for trigger, delay time must larger than 50000 us
	error_code = CAENVME_MultiRead(Handle, base, event, 125000, AM, datawidth, err_code);
	error_status = caen.ErrorDecode(error_code);

	
    FILE *pfile0;
	pfile0 = fopen("/home/dayabay/daq1/output.dat","w");
	
	for(int i=0; i<125000; i++){
		fprintf(pfile0, "%u\n", (event[i]) );
	}
	
	fclose(pfile0);
	
	printf("Data written\n");
	
	v1731_RegisterWrite(Handle, v1731.base, V1731_ACQUISITION_CONTROL, 0x0, v1731.am);  //stop
	printf("Acquisition stops.\n");
	
	
	FILE *pfile1;
	pfile1 = fopen("/home/dayabay/daq1/data1.dat", "w");
	
	FILE *pfile2;
	pfile2 = fopen("/home/dayabay/daq1/data2.dat", "w");
	
	
	
	for (int i=4 ; i<62502;i++){  //CH0
        fprintf (pfile1, "%d\n", (((event[i]) <<24 )>>24));  
        fprintf (pfile1, "%d\n", (((event[i]) <<16 )>>24)); 
        fprintf (pfile1, "%d\n", (((event[i]) <<8  )>>24));
        fprintf (pfile1, "%d\n" , ((event[i]) >>24));  
	}
	
	for(int i=62502; i<125000; i++){  //CH2
        fprintf (pfile2, "%d\n", (((event[i]) <<24 )>>24));  
        fprintf (pfile2, "%d\n", (((event[i]) <<16 )>>24)); 
        fprintf (pfile2, "%d\n", (((event[i]) <<8  )>>24));
        fprintf (pfile2, "%d\n" , ((event[i]) >>24));
	}
	
	

	fclose(pfile1);
	fclose(pfile2);
	
	
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


uint32_t Module_v1731::v1731_RegisterRead(int32_t handle, uint32_t base, int offset, CVAddressModifier AM){
	uint32_t read;
	CVErrorCodes error_code;
	int error_status;
	CAEN caen;
	
	
	error_code = CAENVME_ReadCycle(handle, base+offset, &read, AM, cvD32);
	error_status = caen.ErrorDecode(error_code);
	
	return read;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//0x8010
uint32_t Module_v1731::v1731_BufferFreeRead(int32_t handle, uint32_t base, CVAddressModifier AM){
	  return v1731_RegisterRead(handle, base, V1731_BUFFER_FREE, AM);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//0x1n94
uint32_t Module_v1731::v1731_BufferOccupancy(int32_t handle, uint32_t base, uint32_t channel, CVAddressModifier AM){
	uint32_t reg;
	reg = V1731_BUFFER_OCCUPANCY + (channel<<16);
	return v1731_RegisterRead(handle, base, reg, AM);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//0x8010
uint32_t Module_v1731::v1731_BufferFree(int32_t handle, uint32_t base, int nbuffer, CVAddressModifier AM){
	int mode;
	
	mode = v1731_RegisterRead(handle, base, V1731_BUFFER_FREE, AM);
	if (nbuffer <= (1<<mode) ){
		v1731_RegisterWrite(handle, base, V1731_BUFFER_FREE, nbuffer, AM);
		return mode;
	}else
	
	return mode;
	
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


// 0x8100
void     Module_v1731::v1731_AcqCtl(int32_t handle, uint32_t base, uint32_t operation, CVAddressModifier AM){
	uint32_t reg;
	
	reg = v1731_RegisterRead(handle, base, V1731_ACQUISITION_CONTROL, AM);  
	
	switch (operation) {
	  case V1731_RUN_START:
		  v1731_RegisterWrite(handle, base, V1731_ACQUISITION_CONTROL, (reg | 0x4), AM);
	      break;
	  case V1731_RUN_STOP:
		  v1731_RegisterWrite(handle, base, V1731_ACQUISITION_CONTROL, (reg & ~(0x4)), AM);
	      break;
	  case V1731_REGISTER_RUN_MODE:
		  v1731_RegisterWrite(handle, base, V1731_ACQUISITION_CONTROL, (reg & ~(0x3)), AM);
	      break;
	  case V1731_SIN_RUN_MODE:
		  v1731_RegisterWrite(handle, base, V1731_ACQUISITION_CONTROL, (reg | 0x01), AM);
	      break;
	  case V1731_SIN_GATE_RUN_MODE:
		  v1731_RegisterWrite(handle, base, V1731_ACQUISITION_CONTROL, (reg | 0x02), AM);
	      break;
	  case V1731_MULTI_BOARD_SYNC_MODE:
		  v1731_RegisterWrite(handle, base, V1731_ACQUISITION_CONTROL, (reg | 0x03), AM);
	      break;
	  case V1731_COUNT_ACCEPTED_TRIGGER:
		  v1731_RegisterWrite(handle, base, V1731_ACQUISITION_CONTROL, (reg | 0x08), AM);
	      break;
	  case V1731_COUNT_ALL_TRIGGER:
		  v1731_RegisterWrite(handle, base, V1731_ACQUISITION_CONTROL, (reg & ~(0x08)), AM);
	      break;
	  default:
	      break;
	  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void     Module_v1731::v1731_ChannelCtl(int32_t handle, uint32_t base, uint32_t reg, uint32_t mask, CVAddressModifier AM){
	v1731_RegisterWrite(handle, base, reg, mask, AM);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void     Module_v1731::v1731_TrgCtl(int32_t handle, uint32_t base, uint32_t reg, uint32_t mask, CVAddressModifier AM){
	v1731_RegisterWrite(handle, base, reg, mask, AM);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void     Module_v1731::v1731_RegisterWrite(int32_t handle, uint32_t base, int offset, uint32_t value, CVAddressModifier AM){
	uint32_t write = value;
	CVErrorCodes error_code;
	int error_status;
	CAEN caen;
	
	error_code = CAENVME_WriteCycle(handle, base+offset, &write, AM, cvD32);
	error_status = caen.ErrorDecode(error_code);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//0xEF24
void     Module_v1731::v1731_Reset(int32_t handle, uint32_t base, CVAddressModifier AM){
	v1731_RegisterWrite(handle, base, V1731_SW_RESET, 0, AM);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//0xEF08, 0x8140, 0x8104
void     Module_v1731::v1731_Status(int32_t handle, uint32_t base, CVAddressModifier AM){
	printf("================================================\n");
	printf("V1731 at A32 0x%x\n", (int)base);
	printf("Board ID             : 0x%x\n", v1731_RegisterRead(handle, base, V1731_BOARD_ID, AM));
	printf("Board Info           : 0x%x\n", v1731_RegisterRead(handle, base, V1731_BOARD_INFO, AM));
	printf("Acquisition status   : 0x%8.8x\n", v1731_RegisterRead(handle, base, V1731_ACQUISITION_STATUS, AM));
	printf("Channel 0 ZS_NSAMP   : 0x%x\n", v1731_RegisterRead(handle, base, V1731_ZS_NSAMP, AM));
	printf("CHannel 2 ZS_NSAMP   : 0x%x\n", v1731_RegisterRead(handle, base, V1731_ZS_NSAMP_CH2, AM));
	
	int buffer_organization_output = v1731_RegisterRead(handle, base, V1731_BUFFER_ORGANIZATION, AM);
	int block_nr = 1;
	if (buffer_organization_output != 0){
		for (int i=1; i<=buffer_organization_output; i++){
			block_nr = block_nr * 2;
		}
	}
	printf("Block number         : %d\n", block_nr);
	printf("================================================\n");
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

// 0x800C, 0x810C, 0x8120, 0x8114, 0x8100
int      Module_v1731::v1731_Setup(int32_t handle, uint32_t base, int mode, CVAddressModifier AM){
	
	switch (mode) {
	  case 0x0:
	    printf("--------------------------------------------\n");
	    printf("Setup Skip\n");
	    printf("--------------------------------------------\n");
	  case 0x1:
	    printf("--------------------------------------------\n");
	    printf("Trigger from FP, 8ch, 1Ks, postTrigger 800\n");
	    printf("--------------------------------------------\n");
	    
	    // This register bits [3:0] set the number of buffer blocks which
	    // equals 2^(code). Eg. when buffer code == 0x0A, number == 2^10 == 1024.
	    v1731_RegisterWrite(handle, base, V1731_BUFFER_ORGANIZATION,  0x0A, AM);    // 1024 buffer
	    
	    // This register bits [0,7] enable the channels to generate a local trigger
	    // when the signal exceeds the threshold which is set by bits [26:24].
	    // Bit 0 set channel 0 and so on.
	    // Bit 30 enables the board to sense TRG-IN signals, while
	    // bit 31 enables the board to sense software trigger
	    v1731_RegisterWrite(handle, base, V1731_TRIG_SRCE_EN_MASK,    0x4000, AM);  // 0x4000 for External Trigger
	    v1731_RegisterWrite(handle, base, V1731_CHANNEL_EN_MASK,      0xFF, AM);    // 8ch enable
	    v1731_RegisterWrite(handle, base, V1731_POST_TRIGGER_SETTING, 800, AM);     // PreTrigger (1K-800)
	    v1731_RegisterWrite(handle, base, V1731_ACQUISITION_CONTROL,   0x00, AM);   // Reset Acq Control
	    printf("\n");
	    break;
	  case 0x2:
	    printf("--------------------------------------------\n");
	    printf("Trigger from LEMO\n");
	    printf("--------------------------------------------\n");
	    v1731_RegisterWrite(handle, base, V1731_BUFFER_ORGANIZATION, 1, AM);
	    printf("\n");
	    break;
	  default:
	    printf("Unknown setup mode\n");
	    return -1;
	  }
	  v1731_Status(handle, base, AM);
	  return 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

// 0x800C, 0x8120
void     Module_v1731::v1731_info(int32_t handle, uint32_t base, int *nchannels, uint32_t *n32word, CVAddressModifier AM){
	int i, chanmask;
	// Evaluate the event size
	// Number of samples per channels
	
	uint32_t V1731_NSAMPLES_MODE[11] = { (1<<20), (1<<19), (1<<18), (1<<17), (1<<16), (1<<15)
				                        ,(1<<14), (1<<13), (1<<12), (1<<11), (1<<10)};
	
	*n32word = V1731_NSAMPLES_MODE[v1731_RegisterRead(handle, base, V1731_BUFFER_ORGANIZATION, AM)];
	
	// times the number of active channels
	chanmask = 0xff & v1731_RegisterRead(handle, base, V1731_CHANNEL_EN_MASK, AM); 
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

// 0x0000
uint32_t Module_v1731::v1731_DataRead(int32_t handle, uint32_t base, uint32_t *pdata, uint32_t n32w, CVAddressModifier AM){
	uint32_t i;
	
	for (i=0;i<n32w;i++) {
	    *pdata = v1731_RegisterRead(handle, base, V1731_EVENT_READOUT_BUFFER, AM);
	    //    printf ("pdata[%i]:%x\n", i, *pdata); 
	    //    if (*pdata != 0xffffffff)
	    pdata++;
	    //    else
	    //      break;
	  }
	  return i;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

uint32_t Module_v1731::v1731_DataBlockRead(int32_t handle, uint32_t base, uint32_t *pdest, uint32_t *nentry, CVAddressModifier AM){
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

//0x1n80
void     Module_v1731::v1731_ChannelThresholdSet(int32_t handle, uint32_t base, uint32_t channel, uint32_t threshold, CVAddressModifier AM){
	uint32_t reg;
	reg = V1731_CHANNEL_THRESHOLD | (channel << 8);
	printf("base:0x%x reg:0x%x, threshold:%x\n", base, reg, threshold);
	v1731_RegisterWrite(handle, base, reg, (threshold & 0xFFF), AM);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//0x1n84
void     Module_v1731::v1731_ChannelOUThresholdSet(int32_t handle, uint32_t base, uint32_t channel, uint32_t threshold, CVAddressModifier AM){
	uint32_t reg;
	reg = V1731_CHANNEL_OUTHRESHOLD | (channel << 8);
	printf("base:0x%x reg:0x%x, outhreshold:%x\n", base, reg, threshold);
	v1731_RegisterWrite(handle, base, reg, (threshold & 0xFFF), AM);
	
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//0x1n98
void     Module_v1731::v1731_ChannelDACSet(int32_t handle, uint32_t base, uint32_t channel, uint32_t dac, CVAddressModifier AM){
	uint32_t reg;
	reg = V1731_CHANNEL_DAC | (channel << 8);
	printf("base:0x%x reg:0x%x, DAC:%x\n", base, reg, dac);
	v1731_RegisterWrite(handle, base, reg, (dac & 0xFFFF), AM);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int      Module_v1731::v1731_ChannelDACGet(int32_t handle, uint32_t base, uint32_t channel, uint32_t *dac, CVAddressModifier AM){
    uint32_t reg;
	int   status;

	reg = V1731_CHANNEL_DAC | (channel << 8);
	*dac = v1731_RegisterRead(handle, base, reg, AM);
	reg = V1731_CHANNEL_STATUS | (channel << 8);
	status = v1731_RegisterRead(handle, base, reg, AM);
	return status;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//0x1n80, 0x1n84, 0x1n98
void     Module_v1731::v1731_ChannelSet(int32_t handle, uint32_t base, uint32_t channel, uint32_t what, uint32_t that, CVAddressModifier AM){
	uint32_t reg, mask;

	if (what == V1731_CHANNEL_THRESHOLD)   mask = 0x0FFF;
	if (what == V1731_CHANNEL_OUTHRESHOLD) mask = 0x0FFF;
	if (what == V1731_CHANNEL_DAC)         mask = 0xFFFF;
	reg = what | (channel << 8);
	printf("base:0x%x reg:0x%x, this:%x\n", base, reg, that);
	v1731_RegisterWrite(handle, base, reg, (that & 0xFFF), AM);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//0x1n80, 0x1n84, 0x1n98
uint32_t Module_v1731::v1731_ChannelGet(int32_t handle, uint32_t base, uint32_t channel, uint32_t what, CVAddressModifier AM){
    uint32_t reg, mask;

	if (what == V1731_CHANNEL_THRESHOLD)   mask = 0x0FFF;
	if (what == V1731_CHANNEL_OUTHRESHOLD) mask = 0x0FFF;
	if (what == V1731_CHANNEL_DAC)         mask = 0xFFFF;
	reg = what | (channel << 8);
	return v1731_RegisterRead(handle, base, reg, AM);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//0x8000
void     Module_v1731::v1731_ChannelConfig(int32_t handle, uint32_t base, uint32_t operation, CVAddressModifier AM){
	uint32_t reg;
	  
	v1731_RegisterWrite(handle, base, V1731_CHANNEL_CONFIG, 0x10, AM);
	reg = v1731_RegisterRead(handle, base, V1731_CHANNEL_CONFIG, AM);  
	printf("Channel_config1: 0x%x\n", v1731_RegisterRead(handle, base, V1731_CHANNEL_CONFIG, AM) );  
	switch (operation) {
	  case V1731_TRIGGER_UNDERTH:
		v1731_RegisterWrite(handle, base, V1731_CHANNEL_CONFIG, (reg | 0x40), AM);
	    break;
	  case V1731_TRIGGER_OVERTH:
		v1731_RegisterWrite(handle, base, V1731_CHANNEL_CONFIG, (reg & ~(0x40)), AM);
	    break;
	  default:
	    break;
	  }
	printf("Channel_config2: 0x%x\n", v1731_RegisterRead(handle, base, V1731_CHANNEL_CONFIG, AM) );
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//0xEF00, 0x20
void     Module_v1731::v1731_Align64Set(int32_t handle, uint32_t base, CVAddressModifier AM){
	v1731_RegisterWrite(handle, base, V1731_VME_CONTROL, V1731_ALIGN64, AM);
}






