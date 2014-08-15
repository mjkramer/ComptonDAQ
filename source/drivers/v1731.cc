#include <cstdio>
#include "v1731.hh"
#include <unistd.h>
#include "CAENVMElib.h"
#include "CAEN_VME_def.hh"
#include "DataBlock.hh"
#include <iostream>


int Module_v1731::InitializeVMEModule(){
 	 printf("****Initializing CAEN V1731 Digitizer****");
      	 v1731_handle = ModuleManager::GetHandle();

	 WriteRegister(ACQUISITION_CONTROL, 0x0,cvD32); //reset acquisition
	 WriteRegister(SW_RESET, 0x1, cvD32); //software reset
	 WriteRegister(SW_CLEAR, 0x2, cvD32); // clears all the memories



	 /*new*/	WriteRegister(ZS_NSAMP, 0, cvD32);
	 /*new*/	WriteRegister(ZS_NSAMP_CH2, 0, cvD32);

	 WriteRegister(CHANNEL_EN_MASK, 0x5, cvD32);//enable channels 0 and 2
	 WriteRegister(CHANNEL_DAC, 0x2, cvD32);//dc offset
	 WriteRegister(CHANNEL_DAC_CH2, 0x2, cvD32);//dc offset

	 WriteRegister(ACQUISITION_CONTROL, 0x10,cvD32);//fill up buffers and use last one as circular buffer (p29)
	 // WriteRegister(ALMOST_FULL_LEVEL, 0x14,);//triggers a busy signal

	 WriteRegister(TRIG_SRCE_EN_MASK, 0x40000000,cvD32); //enable external trigger
	 WriteRegister(POST_TRIGGER_SETTING, 0,cvD32); //we only want pre-trigger samples
	 WriteRegister(CHANNEL_CONFIG, 0x0,cvD32); //trigger overlapping disabled (default)

	 /*new*/  WriteRegister(CHANNEL_CONFIG, 0x1010, cvD32);//1Gs/s

	 /*modified*/	 WriteRegister(BUFFER_ORGANIZATION, 0xA, cvD32); // (0 instead of A - one buffer block) seperate buffer into 1024 blocks (each can hold 4k samples @1Gs/s or 2k samples @500Ms/s)
	//make events custom sized:	
	//nsamples = 2048;
	//memory_location = nsamples/(8 bzw 16 for 1Gs/s) = 128@1Gs/s = 0x80
	 WriteRegister(CUSTOM_SIZE, 0x20,cvD32);
	 printf("custom size: %d\n",ReadRegister(CUSTOM_SIZE,cvD32));

	//set board online
	SetOnline();
	printf("  --  OK\n");

	return 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DataBlock* Module_v1731::GetModuleBuffer(){  
	usleep(1000000);
	CVErrorCodes error_code;
	int error_status = 0;	

	int nsample = 256;
	int nr_elem = (nsample / 4)*2 + 4;  // 4 samples per DWORD, 2 CHs and 4 DWORDs for the header
	int size = nr_elem*32/8;//size in bytes
	int count = 0; //number of bytes transfered
        uint32_t* rawdata = new uint32_t[nr_elem];
	

	for(int i = 0; i<nr_elem; i++){
	rawdata[i] = 0;}

	error_code = CAENVME_BLTReadCycle(v1731_handle, V1731::base, rawdata, size, cvA32_S_BLT, cvD32, &count);
        error_status = CAEN::ErrorDecode(error_code);


       	if (error_status == 0){
	    ;}
	else{
		printf("Error during readout of digitizer datablocks\n");
	}


	int version = 1; 
	DataBlock_v1731* datablock = new DataBlock_v1731(version, rawdata);
	datablock->Set_nr_sample(nsample);
	//	for(int i = 0; i<nr_elem; i++){
	//	if(rawdata[i] > 0){
	//		printf("%d\n",rawdata[i]);}
	
	//}
	
	return datablock;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Module_v1731::GenerateSoftwareTrigger(){
  WriteRegister(SOFTWARE_TRIGGER,0x2, cvD32);
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Module_v1731::WriteRegister(int offset, uint32_t value, CVDataWidth width){
	uint32_t write = value;
	CVErrorCodes error_code;
	int error_status;

	error_code = CAENVME_WriteCycle(v1731_handle, V1731::base+offset, &write, V1731::am, width);
	error_status = CAEN::ErrorDecode(error_code);
	if(error_status != 0){printf("Error while writing digitizer registers!\n");}
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


uint32_t Module_v1731::ReadRegister(int offset, CVDataWidth width){
	uint32_t read;
	CVErrorCodes error_code;
	int error_status;

	error_code = CAENVME_ReadCycle(v1731_handle, V1731::base+offset, &read, V1731::am, width);
	error_status = CAEN::ErrorDecode(error_code);

	if(error_status){printf("Error while reading digitizer register.\n");}
	return read;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int Module_v1731::SetOnline(){	
	 printf("Setting module online\n");
	   WriteRegister(ACQUISITION_CONTROL, 0x4,cvD32);
	 return 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int Module_v1731::SetOffline(){
  WriteRegister(ACQUISITION_CONTROL, 0x0,cvD32);
    	return 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int Module_v1731::DataReady(){
	uint32_t status = 0;
	  status =  ReadRegister(ACQUISITION_STATUS,cvD32);
	status = ((status << 28) >> 31);
	
	if (status == 1){return 1;}//ready
	else{return 0;}
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


/*

void Module_v1731::CalibrateChannels(){

//set 1 to the calibration bit
 WriteRegister(CALIBRATION_CH1,0x2,V1731::am);
 WriteRegister(CALIBRATION_CH2,0x2,V1731::am);
 WriteRegister(CALIBRATION_CH3,0x2,V1731::am);
 WriteRegister(CALIBRATION_CH4,0x2,V1731::am);

//reset calibration bit to 0
 WriteRegister(CALIBRATION_CH1,0x0,V1731::am);
 WriteRegister(CALIBRATION_CH2,0x0,V1731::am);
 WriteRegister(CALIBRATION_CH3,0x0,V1731::am);
 WriteRegister(CALIBRATION_CH4,0x0,V1731::am);

usleep(100000);
if( RegisterRead(CALIBRATION_STATUS_CH4)){;}

}


*/

/*
//0x8010
uint32_t Module_v1731:: BufferFreeRead(int32_t  v1731_handle, uint32_t base, CVAddressModifier AM){
	  return  RegisterRead( v1731_handle, base,  BUFFER_FREE, AM);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//0x1n94
uint32_t Module_v1731:: BufferOccupancy(int32_t  v1731_handle, uint32_t base, uint32_t channel, CVAddressModifier AM){
	uint32_t reg;
	reg =  BUFFER_OCCUPANCY + (channel<<16);
	return  RegisterRead( v1731_handle, base, reg, AM);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//0x8010
uint32_t Module_v1731:: BufferFree(int32_t  v1731_handle, uint32_t base, int nbuffer, CVAddressModifier AM){
	int mode;
	
	mode =  RegisterRead( v1731_handle, base,  BUFFER_FREE, AM);
	if (nbuffer <= (1<<mode) ){
		 WriteRegister( v1731_handle, base,  BUFFER_FREE, nbuffer, AM);
		return mode;
	}else
	
	return mode;
	
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


// 0x8100
void     Module_v1731:: AcqCtl(int32_t  v1731_handle, uint32_t base, uint32_t operation, CVAddressModifier AM){
	uint32_t reg;
	
	reg =  RegisterRead( v1731_handle, base,  ACQUISITION_CONTROL, AM);  
	
	switch (operation) {
	  case  RUN_START:
		   WriteRegister( v1731_handle, base,  ACQUISITION_CONTROL, (reg | 0x4), AM);
	      break;
	  case  RUN_STOP:
		   WriteRegister( v1731_handle, base,  ACQUISITION_CONTROL, (reg & ~(0x4)), AM);
	      break;
	  case  REGISTER_RUN_MODE:
		   WriteRegister( v1731_handle, base,  ACQUISITION_CONTROL, (reg & ~(0x3)), AM);
	      break;
	  case  SIN_RUN_MODE:
		   WriteRegister( v1731_handle, base,  ACQUISITION_CONTROL, (reg | 0x01), AM);
	      break;
	  case  SIN_GATE_RUN_MODE:
		   WriteRegister( v1731_handle, base,  ACQUISITION_CONTROL, (reg | 0x02), AM);
	      break;
	  case  MULTI_BOARD_SYNC_MODE:
		   WriteRegister( v1731_handle, base,  ACQUISITION_CONTROL, (reg | 0x03), AM);
	      break;
	  case  COUNT_ACCEPTED_TRIGGER:
		   WriteRegister( v1731_handle, base,  ACQUISITION_CONTROL, (reg | 0x08), AM);
	      break;
	  case  COUNT_ALL_TRIGGER:
		   WriteRegister( v1731_handle, base,  ACQUISITION_CONTROL, (reg & ~(0x08)), AM);
	      break;
	  default:
	      break;
	  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void     Module_v1731:: ChannelCtl(int32_t  v1731_handle, uint32_t base, uint32_t reg, uint32_t mask, CVAddressModifier AM){
	 WriteRegister( v1731_handle, base, reg, mask, AM);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void     Module_v1731:: TrgCtl(int32_t  v1731_handle, uint32_t base, uint32_t reg, uint32_t mask, CVAddressModifier AM){
	 WriteRegister( v1731_handle, base, reg, mask, AM);
}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//0xEF24
void     Module_v1731:: Reset(int32_t  v1731_handle, uint32_t base, CVAddressModifier AM){
	 WriteRegister( v1731_handle, base,  SW_RESET, 0, AM);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//0xEF08, 0x8140, 0x8104
void     Module_v1731:: Status(int32_t  v1731_handle, uint32_t base, CVAddressModifier AM){
	printf("================================================\n");
	printf("V1731 at A32 0x%x\n", (int)base);
	printf("Board ID             : 0x%x\n",  RegisterRead( v1731_handle, base,  BOARD_ID, AM));
	printf("Board Info           : 0x%x\n",  RegisterRead( v1731_handle, base,  BOARD_INFO, AM));
	printf("Acquisition status   : 0x%8.8x\n",  RegisterRead( v1731_handle, base,  ACQUISITION_STATUS, AM));
	printf("Channel 0 ZS_NSAMP   : 0x%x\n",  RegisterRead( v1731_handle, base,  ZS_NSAMP, AM));
	printf("CHannel 2 ZS_NSAMP   : 0x%x\n",  RegisterRead( v1731_handle, base,  ZS_NSAMP_CH2, AM));
	
	int buffer_organization_output =  RegisterRead( v1731_handle, base,  BUFFER_ORGANIZATION, AM);
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
int      Module_v1731:: Setup(int32_t  v1731_handle, uint32_t base, int mode, CVAddressModifier AM){
	
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
	     WriteRegister( v1731_handle, base,  BUFFER_ORGANIZATION,  0x0A, AM);    // 1024 buffer
	    
	    // This register bits [0,7] enable the channels to generate a local trigger
	    // when the signal exceeds the threshold which is set by bits [26:24].
	    // Bit 0 set channel 0 and so on.
	    // Bit 30 enables the board to sense TRG-IN signals, while
	    // bit 31 enables the board to sense software trigger
	     WriteRegister( v1731_handle, base,  TRIG_SRCE_EN_MASK,    0x4000, AM);  // 0x4000 for External Trigger
	     WriteRegister( v1731_handle, base,  CHANNEL_EN_MASK,      0xFF, AM);    // 8ch enable
	     WriteRegister( v1731_handle, base,  POST_TRIGGER_SETTING, 800, AM);     // PreTrigger (1K-800)
	     WriteRegister( v1731_handle, base,  ACQUISITION_CONTROL,   0x00, AM);   // Reset Acq Control
	    printf("\n");
	    break;
	  case 0x2:
	    printf("--------------------------------------------\n");
	    printf("Trigger from LEMO\n");
	    printf("--------------------------------------------\n");
	     WriteRegister( v1731_handle, base,  BUFFER_ORGANIZATION, 1, AM);
	    printf("\n");
	    break;
	  default:
	    printf("Unknown setup mode\n");
	    return 1;
	  }
	   Status( v1731_handle, base, AM);
	  return 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

// 0x800C, 0x8120
void     Module_v1731:: info(int32_t  v1731_handle, uint32_t base, int *nchannels, uint32_t *n32word, CVAddressModifier AM){
	int i, chanmask;
	// Evaluate the event size
	// Number of samples per channels
	
	uint32_t  NSAMPLES_MODE[11] = { (1<<20), (1<<19), (1<<18), (1<<17), (1<<16), (1<<15)
				                        ,(1<<14), (1<<13), (1<<12), (1<<11), (1<<10)};
	
	*n32word =  NSAMPLES_MODE[ RegisterRead( v1731_handle, base,  BUFFER_ORGANIZATION, AM)];
	
	// times the number of active channels
	chanmask = 0xff &  RegisterRead( v1731_handle, base,  CHANNEL_EN_MASK, AM); 
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
uint32_t Module_v1731:: DataRead(int32_t  v1731_handle, uint32_t base, uint32_t *pdata, uint32_t n32w, CVAddressModifier AM){
	uint32_t i;
	
	for (i=0;i<n32w;i++) {
	    *pdata =  RegisterRead( v1731_handle, base,  EVENT_READOUT_BUFFER, AM);
	    //    printf ("pdata[%i]:%x\n", i, *pdata); 
	    //    if (*pdata != 0xffffffff)
	    pdata++;
	    //    else
	    //      break;
	  }
	  return i;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

uint32_t Module_v1731:: DataBlockRead(int32_t  v1731_handle, uint32_t base, uint32_t *pdest, uint32_t *nentry, CVAddressModifier AM){
	CVErrorCodes error_code;
	AM = cvA32_S_DATA;
	int n = 0;
	int error_status;
	
	
	error_code = CAENVME_MBLTReadCycle( v1731_handle, base, pdest, *nentry<<2, AM, &n);
	error_status = CAEN::ErrorDecode(error_code);
	
	return n;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//0x1n80
void     Module_v1731:: ChannelThresholdSet(int32_t  v1731_handle, uint32_t base, uint32_t channel, uint32_t threshold, CVAddressModifier AM){
	uint32_t reg;
	reg =  CHANNEL_THRESHOLD | (channel << 8);
	printf("base:0x%x reg:0x%x, threshold:%x\n", base, reg, threshold);
	 WriteRegister( v1731_handle, base, reg, (threshold & 0xFFF), AM);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//0x1n84
void     Module_v1731:: ChannelOUThresholdSet(int32_t  v1731_handle, uint32_t base, uint32_t channel, uint32_t threshold, CVAddressModifier AM){
	uint32_t reg;
	reg =  CHANNEL_OUTHRESHOLD | (channel << 8);
	printf("base:0x%x reg:0x%x, outhreshold:%x\n", base, reg, threshold);
	 WriteRegister( v1731_handle, base, reg, (threshold & 0xFFF), AM);
	
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//0x1n98
void     Module_v1731:: ChannelDACSet(int32_t  v1731_handle, uint32_t base, uint32_t channel, uint32_t dac, CVAddressModifier AM){
	uint32_t reg;
	reg =  CHANNEL_DAC | (channel << 8);
	printf("base:0x%x reg:0x%x, DAC:%x\n", base, reg, dac);
	 WriteRegister( v1731_handle, base, reg, (dac & 0xFFFF), AM);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int      Module_v1731:: ChannelDACGet(int32_t  v1731_handle, uint32_t base, uint32_t channel, uint32_t *dac, CVAddressModifier AM){
    uint32_t reg;
	int   status;

	reg =  CHANNEL_DAC | (channel << 8);
	*dac =  RegisterRead( v1731_handle, base, reg, AM);
	reg =  CHANNEL_STATUS | (channel << 8);
	status =  RegisterRead( v1731_handle, base, reg, AM);
	return status;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//0x1n80, 0x1n84, 0x1n98
void     Module_v1731:: ChannelSet(int32_t  v1731_handle, uint32_t base, uint32_t channel, uint32_t what, uint32_t that, CVAddressModifier AM){
	uint32_t reg, mask;

	if (what ==  CHANNEL_THRESHOLD)   mask = 0x0FFF;
	if (what ==  CHANNEL_OUTHRESHOLD) mask = 0x0FFF;
	if (what ==  CHANNEL_DAC)         mask = 0xFFFF;
	reg = what | (channel << 8);
	printf("base:0x%x reg:0x%x, this:%x\n", base, reg, that);
	 WriteRegister( v1731_handle, base, reg, (that & 0xFFF), AM);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//0x1n80, 0x1n84, 0x1n98
uint32_t Module_v1731:: ChannelGet(int32_t  v1731_handle, uint32_t base, uint32_t channel, uint32_t what, CVAddressModifier AM){
    uint32_t reg, mask;

	if (what ==  CHANNEL_THRESHOLD)   mask = 0x0FFF;
	if (what ==  CHANNEL_OUTHRESHOLD) mask = 0x0FFF;
	if (what ==  CHANNEL_DAC)         mask = 0xFFFF;
	reg = what | (channel << 8);
	return  RegisterRead( v1731_handle, base, reg, AM);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//0x8000
void     Module_v1731:: ChannelConfig(int32_t  v1731_handle, uint32_t base, uint32_t operation, CVAddressModifier AM){
	uint32_t reg;
	  
	 WriteRegister( v1731_handle, base,  CHANNEL_CONFIG, 0x10, AM);
	reg =  RegisterRead( v1731_handle, base,  CHANNEL_CONFIG, AM);  
	printf("Channel_config1: 0x%x\n",  RegisterRead( v1731_handle, base,  CHANNEL_CONFIG, AM) );  
	switch (operation) {
	  case  TRIGGER_UNDERTH:
		 WriteRegister( v1731_handle, base,  CHANNEL_CONFIG, (reg | 0x40), AM);
	    break;
	  case  TRIGGER_OVERTH:
		 WriteRegister( v1731_handle, base,  CHANNEL_CONFIG, (reg & ~(0x40)), AM);
	    break;
	  default:
	    break;
	  }
	printf("Channel_config2: 0x%x\n",  RegisterRead( v1731_handle, base,  CHANNEL_CONFIG, AM) );
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//0xEF00, 0x20
void     Module_v1731:: Align64Set(int32_t  v1731_handle, uint32_t base, CVAddressModifier AM){
	 WriteRegister( v1731_handle, base,  VME_CONTROL,  ALIGN64, AM);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int      Module_v1731:: ReadBuffer_and_Output(int32_t  v1731_handle, uint32_t base, int nsample, CVAddressModifier AM, uint32_t *rawdata){

	CVErrorCodes error_code;
	
	int nr_elem = (nsample / 4) * 2 + 4;  // 1/4 samples per DWORD, there are 2 CHs and 4 DWORDs for the header
	int error_status = 0;
	int size = nr_elem * 32 / 8;  // 32 bits per datum => 4 bytes per datum
	int count;
    
    error_code = CAENVME_MBLTReadCycle( v1731_handle, base, rawdata, size, cvA32_S_MBLT, &count);
    error_status = CAEN::ErrorDecode(error_code);
    

    
	if (error_status == 0){
	    return 0;
	}else{
		return 1;
	}
}
*/
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......







