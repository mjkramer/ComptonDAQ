#include <cstdio>
#include "v1731.hh"
#include <unistd.h>
#include "CAENVMElib.h"
#include "CAEN_VME_def.hh"
#include "DataBlock.hh"
#include <iostream>
#include <string.h>

Module_v1731::Module_v1731():
  m_v1731_handle(0),
  m_bufferSize(0),
  m_eventBuffer(0),
  m_currentEvent(0),
  m_bufferEnd(0)
{
  ;
}

Module_v1731::~Module_v1731()
{
  if(m_eventBuffer){
    delete [] m_eventBuffer;
    m_eventBuffer = 0;
  }
}

int Module_v1731::InitializeVMEModule(){
  printf("****Initializing CAEN V1731 Digitizer****");
  m_v1731_handle = ModuleManager::GetHandle();

  // Enough space for ~1000 events
  m_bufferSize = 1028*1000;
  
  WriteRegister(ACQUISITION_CONTROL, 0x0,cvD32); //reset acquisition
  WriteRegister(SW_RESET, 0x1, cvD32); //software reset
  WriteRegister(SW_CLEAR, 0x2, cvD32); // clears all the memories
  
  CalibrateChannels(); //channel calibration
  
  //WriteRegister(ZS_NSAMP, 0, cvD32); //set zero supression zero
  //WriteRegister(ZS_NSAMP_CH2, 0, cvD32); //set zero supression zero
  
  WriteRegister(CHANNEL_EN_MASK, 0x5, cvD32);//enable channels 0 and 2
  WriteRegister(CHANNEL_DAC, 0x2000, cvD32);//dc offset
  WriteRegister(CHANNEL_DAC_CH2, 0x2000, cvD32);//dc offset
  usleep(1000); //wait for dc offset to be updated
  
  //WriteRegister(ACQUISITION_CONTROL, 0x10,cvD32);//fill up buffers and use last one as circular buffer (p29)
  
  WriteRegister(TRIG_SRCE_EN_MASK, 0x40000000,cvD32); //enable external trigger
  WriteRegister(POST_TRIGGER_SETTING, 0,cvD32); //we only want pre-trigger samples
  //WriteRegister(CHANNEL_CONFIG, 0x0,cvD32); //trigger overlapping disabled (default)
  
  //WriteRegister(CHANNEL_CONFIG, 0x10, cvD32);//seqential memory access
  //WriteRegister(VME_CONTROL, 0xF, cvD32); //BERR bit
  
  WriteRegister(BUFFER_ORGANIZATION, 0x0A, cvD32); //seperate buffer into 1024 blocks (each can hold 4k samples @1Gs/s or 2k samples @500Ms/s)
  //make events custom sized:	
  //m_nsamples = 2048;
  //memory_locations = m_nsamples/(8 bzw 16 for 1Gs/s) = 256@1Gs/s = 0x100
  //WriteRegister(CUSTOM_SIZE, 0x100, cvD32);
  
  //set board online
  SetOnline();
  printf("  --  OK\n");
  
  return 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DataBlock* Module_v1731::GetModuleBuffer(){  
  CVErrorCodes error_code;
  int error_status = 0;	
  
  if( BufferedEvents()<1 ){
    error_status = UpdateEventBuffer();
    if (error_status != 0) return NULL;
  }
  //std::cout << "Info: Module_v1731: Getting next event from buffer containing " 
  //	    << BufferedEvents() << " events." << std::endl;

  int version = 1;
  DataBlock_v1731* dataBlock = new DataBlock_v1731(version, GetNextEvent());
  return dataBlock;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

uint Module_v1731::GetEventSize(uint32_t* event){  
  // Read event size from event data block
  //  See pg. ?? in CAEN v1731 manual.
  return ((event[0] << 4) >> 4);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

uint Module_v1731::BufferedEvents(){  
  // Examine the event buffer and determine the current number of
  // buffered events

  // Check if a pointer to the current event is set
  if(m_currentEvent == 0){
    //std::cout << "Error: Module_v1731 buffer not initialized!" << std::endl;
    return 0;
  }
  int nEvents = 0;
  uint32_t* nextEvent = m_currentEvent;
  while(nextEvent != m_bufferEnd){
    uint eventSize = GetEventSize( nextEvent );  // Get size of this event
    nextEvent += eventSize; // shift to next event
    nEvents++; // increment event count
  }
  //std::cout << "   Buffered Events: " << nEvents << std::endl;
  return nEvents;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

uint32_t* Module_v1731::GetNextEvent(){  
  // Pop the next event from the top of the event buffer

  // Check if there is at least one event in the buffer
  if(BufferedEvents() < 1){
    std::cout << "Error: Module_v1731: Attempting to retrieve event from empty buffer!" << std::endl;
    return NULL;
  }

  // Check if a pointer to the current event is set
  if(m_currentEvent == 0){
    std::cout << "Error: Module_v1731: Buffer not initialized!" << std::endl;
    return NULL;
  }

  // Check for valid event size
  uint eventSize = GetEventSize(m_currentEvent);
  if(eventSize <= 0 
     || eventSize > ((int)(m_currentEvent - m_bufferEnd))){
    std::cout << "Error: Module_v1731: Invalid event size = "
	      << eventSize << std::endl;
    return NULL;
  }

  // Create new block of memory containing only this event.
  //   Note: this memory will be adopted by DataBlock, which will be
  //   responsible for deletion.
  uint32_t* popEvent = new uint32_t[eventSize];
  memcpy(popEvent, m_currentEvent, sizeof(uint32_t)*eventSize);

  // Remove current event from buffer.
  m_currentEvent += eventSize; 
  
  // Return current event
  return popEvent;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int Module_v1731::UpdateEventBuffer(){  
  CVErrorCodes error_code;
  int error_status = 0;	

  // Check if buffer is initialized, and create buffer memory if needed
  if(m_eventBuffer == 0){
    m_eventBuffer = new uint32_t[m_bufferSize];
  }

  // Clear buffer memory
  memset(m_eventBuffer, 0, sizeof(uint32_t)*m_bufferSize);

  // Initialize event pointers
  m_currentEvent = m_eventBuffer;
  m_bufferEnd = m_eventBuffer;
  
  // Request number and size of events stored in board
  uint32_t eStored = ReadRegister(EVENT_STORED, cvD32);
  uint32_t eSize = ReadRegister(EVENT_SIZE, cvD32);

  // If no data, skip reading
  if(eStored < 1){return 0;}	
  
  //Transfer data from board to local event buffer
  uint maxBLTReadLength = 4096/4; // Maximum read in 32-bit units
  uint totalReadLength = eSize*eStored;
  // Don't read more data than event buffer size
  if(totalReadLength > m_bufferSize){
    // Plan to read as many events as can fit into current event buffer
    totalReadLength = (m_bufferSize/eSize)*eSize;
  }

  int count=0;
  uint currentReadLength = 0;  
  while(currentReadLength<totalReadLength){
    uint readLength = maxBLTReadLength;
    // Read only to end of event for last event
    if(readLength > (totalReadLength-currentReadLength))
      readLength = totalReadLength-currentReadLength;
    count = 0;
    error_code = CAENVME_BLTReadCycle(m_v1731_handle, V1731::base, 
				      &m_eventBuffer[currentReadLength], 
				      readLength*4, // Bytes
				      cvA32_S_BLT, cvD32, &count);
    error_status = CAEN::ErrorDecode(error_code);
    if(error_status != 0 || count!=(readLength*4)){
      std::cout << "Error: Module_v1731: BLT read failed after reading "
		<< currentReadLength << " words." << std::endl;
      return error_status;
    }
    if(false){
      std::cout << "Data:" << std::endl;
      for(int i = 0; i<maxBLTReadLength; i+=4){
	printf("%4d %10x %10x %10x %10x\n",
	       i,
	       m_eventBuffer[currentReadLength+i],
	       m_eventBuffer[currentReadLength+i+1],
	       m_eventBuffer[currentReadLength+i+2],
	       m_eventBuffer[currentReadLength+i+3]);
      }
    }
    currentReadLength += readLength;
  }

  // Set endpoint of currently read data
  m_bufferEnd = &m_eventBuffer[totalReadLength];

  std::cout << "Info: Module_v1731: Read " << totalReadLength << " from board."
	    << std::endl;
  std::cout << "Info: Module_v1731: Should contain " << eStored << " events."
	    << std::endl;
  std::cout << "Info: Module_v1731: Contains " << BufferedEvents() << " events."
	    << std::endl;

  // Return
  return error_status;
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
  
  error_code = CAENVME_WriteCycle(m_v1731_handle, V1731::base+offset, &write, V1731::am, width);
  error_status = CAEN::ErrorDecode(error_code);
  if(error_status != 0){printf("Error while writing digitizer registers!\n");}
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


uint32_t Module_v1731::ReadRegister(int offset, CVDataWidth width){
  uint32_t read;
  CVErrorCodes error_code;
  int error_status;
  
  error_code = CAENVME_ReadCycle(m_v1731_handle, V1731::base+offset, &read, V1731::am, width);
  error_status = CAEN::ErrorDecode(error_code);
  
  if(error_status){printf("Error while reading digitizer register.\n");}
  return read;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int Module_v1731::SetOnline(){	
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

void Module_v1731::CalibrateChannels(){
  
  //set 1 to the calibration bit
  WriteRegister(CALIBRATION_CH1,0x2,cvD32);
  WriteRegister(CALIBRATION_CH2,0x2,cvD32);
  WriteRegister(CALIBRATION_CH3,0x2,cvD32);
  WriteRegister(CALIBRATION_CH4,0x2,cvD32);
  usleep(100000);
  
  //reset calibration bit to 0
  WriteRegister(CALIBRATION_CH1,0x0,cvD32);
  WriteRegister(CALIBRATION_CH2,0x0,cvD32);
  WriteRegister(CALIBRATION_CH3,0x0,cvD32);
  WriteRegister(CALIBRATION_CH4,0x0,cvD32);
  usleep(100000);
  
  //readout odd channels:
  //WriteRegister(CHANNEL_EN_MASK, 0xA, cvD32);//enable channels 1 and 3
  //DataBlock *block = GetModuleBuffer();
  //DataBlock_v1731* p1731_cast = dynamic_cast<DataBlock_v1731*>(block);
  //if(p1731_cast){
  //if(DataReady()){
  //std::vector<int> CH1 = p1731_cast->GetWaveform_Channel0(); //actually channel 1
  //std::vector<int> CH3 = p1731_cast->GetWaveform_Channel2(); //actually channel 3
  //printf("Channel 1 ADC value: %d\n", CH1.at(0));
  //printf("Channel 3 ADC value: %d\n", CH3.at(0));
  //}}
  //WriteRegister(CHANNEL_EN_MASK, 0x0, cvD32);//disable all channels
  
  
}

/*
//0x8010
uint32_t Module_v1731:: BufferFreeRead(int32_t  m_v1731_handle, uint32_t base, CVAddressModifier AM){
return  RegisterRead( m_v1731_handle, base,  BUFFER_FREE, AM);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//0x1n94
uint32_t Module_v1731:: BufferOccupancy(int32_t  m_v1731_handle, uint32_t base, uint32_t channel, CVAddressModifier AM){
uint32_t reg;
reg =  BUFFER_OCCUPANCY + (channel<<16);
return  RegisterRead( m_v1731_handle, base, reg, AM);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//0x8010
uint32_t Module_v1731:: BufferFree(int32_t  m_v1731_handle, uint32_t base, int nbuffer, CVAddressModifier AM){
	int mode;
	
	mode =  RegisterRead( m_v1731_handle, base,  BUFFER_FREE, AM);
	if (nbuffer <= (1<<mode) ){
		 WriteRegister( m_v1731_handle, base,  BUFFER_FREE, nbuffer, AM);
		return mode;
	}else
	
	return mode;
	
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


// 0x8100
void     Module_v1731:: AcqCtl(int32_t  m_v1731_handle, uint32_t base, uint32_t operation, CVAddressModifier AM){
	uint32_t reg;
	
	reg =  RegisterRead( m_v1731_handle, base,  ACQUISITION_CONTROL, AM);  
	
	switch (operation) {
	  case  RUN_START:
		   WriteRegister( m_v1731_handle, base,  ACQUISITION_CONTROL, (reg | 0x4), AM);
	      break;
	  case  RUN_STOP:
		   WriteRegister( m_v1731_handle, base,  ACQUISITION_CONTROL, (reg & ~(0x4)), AM);
	      break;
	  case  REGISTER_RUN_MODE:
		   WriteRegister( m_v1731_handle, base,  ACQUISITION_CONTROL, (reg & ~(0x3)), AM);
	      break;
	  case  SIN_RUN_MODE:
		   WriteRegister( m_v1731_handle, base,  ACQUISITION_CONTROL, (reg | 0x01), AM);
	      break;
	  case  SIN_GATE_RUN_MODE:
		   WriteRegister( m_v1731_handle, base,  ACQUISITION_CONTROL, (reg | 0x02), AM);
	      break;
	  case  MULTI_BOARD_SYNC_MODE:
		   WriteRegister( m_v1731_handle, base,  ACQUISITION_CONTROL, (reg | 0x03), AM);
	      break;
	  case  COUNT_ACCEPTED_TRIGGER:
		   WriteRegister( m_v1731_handle, base,  ACQUISITION_CONTROL, (reg | 0x08), AM);
	      break;
	  case  COUNT_ALL_TRIGGER:
		   WriteRegister( m_v1731_handle, base,  ACQUISITION_CONTROL, (reg & ~(0x08)), AM);
	      break;
	  default:
	      break;
	  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void     Module_v1731:: ChannelCtl(int32_t  m_v1731_handle, uint32_t base, uint32_t reg, uint32_t mask, CVAddressModifier AM){
	 WriteRegister( m_v1731_handle, base, reg, mask, AM);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void     Module_v1731:: TrgCtl(int32_t  m_v1731_handle, uint32_t base, uint32_t reg, uint32_t mask, CVAddressModifier AM){
	 WriteRegister( m_v1731_handle, base, reg, mask, AM);
}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//0xEF24
void     Module_v1731:: Reset(int32_t  m_v1731_handle, uint32_t base, CVAddressModifier AM){
	 WriteRegister( m_v1731_handle, base,  SW_RESET, 0, AM);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//0xEF08, 0x8140, 0x8104
void     Module_v1731:: Status(int32_t  m_v1731_handle, uint32_t base, CVAddressModifier AM){
	printf("================================================\n");
	printf("V1731 at A32 0x%x\n", (int)base);
	printf("Board ID             : 0x%x\n",  RegisterRead( m_v1731_handle, base,  BOARD_ID, AM));
	printf("Board Info           : 0x%x\n",  RegisterRead( m_v1731_handle, base,  BOARD_INFO, AM));
	printf("Acquisition status   : 0x%8.8x\n",  RegisterRead( m_v1731_handle, base,  ACQUISITION_STATUS, AM));
	printf("Channel 0 ZS_NSAMP   : 0x%x\n",  RegisterRead( m_v1731_handle, base,  ZS_NSAMP, AM));
	printf("CHannel 2 ZS_NSAMP   : 0x%x\n",  RegisterRead( m_v1731_handle, base,  ZS_NSAMP_CH2, AM));
	
	int buffer_organization_output =  RegisterRead( m_v1731_handle, base,  BUFFER_ORGANIZATION, AM);
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
int      Module_v1731:: Setup(int32_t  m_v1731_handle, uint32_t base, int mode, CVAddressModifier AM){
	
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
	     WriteRegister( m_v1731_handle, base,  BUFFER_ORGANIZATION,  0x0A, AM);    // 1024 buffer
	    
	    // This register bits [0,7] enable the channels to generate a local trigger
	    // when the signal exceeds the threshold which is set by bits [26:24].
	    // Bit 0 set channel 0 and so on.
	    // Bit 30 enables the board to sense TRG-IN signals, while
	    // bit 31 enables the board to sense software trigger
	     WriteRegister( m_v1731_handle, base,  TRIG_SRCE_EN_MASK,    0x4000, AM);  // 0x4000 for External Trigger
	     WriteRegister( m_v1731_handle, base,  CHANNEL_EN_MASK,      0xFF, AM);    // 8ch enable
	     WriteRegister( m_v1731_handle, base,  POST_TRIGGER_SETTING, 800, AM);     // PreTrigger (1K-800)
	     WriteRegister( m_v1731_handle, base,  ACQUISITION_CONTROL,   0x00, AM);   // Reset Acq Control
	    printf("\n");
	    break;
	  case 0x2:
	    printf("--------------------------------------------\n");
	    printf("Trigger from LEMO\n");
	    printf("--------------------------------------------\n");
	     WriteRegister( m_v1731_handle, base,  BUFFER_ORGANIZATION, 1, AM);
	    printf("\n");
	    break;
	  default:
	    printf("Unknown setup mode\n");
	    return 1;
	  }
	   Status( m_v1731_handle, base, AM);
	  return 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

// 0x800C, 0x8120
void     Module_v1731:: info(int32_t  m_v1731_handle, uint32_t base, int *nchannels, uint32_t *n32word, CVAddressModifier AM){
	int i, chanmask;
	// Evaluate the event size
	// Number of samples per channels
	
	uint32_t  M_NSAMPLES_MODE[11] = { (1<<20), (1<<19), (1<<18), (1<<17), (1<<16), (1<<15)
				                        ,(1<<14), (1<<13), (1<<12), (1<<11), (1<<10)};
	
	*n32word =  M_NSAMPLES_MODE[ RegisterRead( m_v1731_handle, base,  BUFFER_ORGANIZATION, AM)];
	
	// times the number of active channels
	chanmask = 0xff &  RegisterRead( m_v1731_handle, base,  CHANNEL_EN_MASK, AM); 
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
uint32_t Module_v1731:: DataRead(int32_t  m_v1731_handle, uint32_t base, uint32_t *pdata, uint32_t n32w, CVAddressModifier AM){
	uint32_t i;
	
	for (i=0;i<n32w;i++) {
	    *pdata =  RegisterRead( m_v1731_handle, base,  EVENT_READOUT_BUFFER, AM);
	    //    printf ("pdata[%i]:%x\n", i, *pdata); 
	    //    if (*pdata != 0xffffffff)
	    pdata++;
	    //    else
	    //      break;
	  }
	  return i;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

uint32_t Module_v1731:: DataBlockRead(int32_t  m_v1731_handle, uint32_t base, uint32_t *pdest, uint32_t *nentry, CVAddressModifier AM){
	CVErrorCodes error_code;
	AM = cvA32_S_DATA;
	int n = 0;
	int error_status;
	
	
	error_code = CAENVME_MBLTReadCycle( m_v1731_handle, base, pdest, *nentry<<2, AM, &n);
	error_status = CAEN::ErrorDecode(error_code);
	
	return n;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//0x1n80
void     Module_v1731:: ChannelThresholdSet(int32_t  m_v1731_handle, uint32_t base, uint32_t channel, uint32_t threshold, CVAddressModifier AM){
	uint32_t reg;
	reg =  CHANNEL_THRESHOLD | (channel << 8);
	printf("base:0x%x reg:0x%x, threshold:%x\n", base, reg, threshold);
	 WriteRegister( m_v1731_handle, base, reg, (threshold & 0xFFF), AM);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//0x1n84
void     Module_v1731:: ChannelOUThresholdSet(int32_t  m_v1731_handle, uint32_t base, uint32_t channel, uint32_t threshold, CVAddressModifier AM){
	uint32_t reg;
	reg =  CHANNEL_OUTHRESHOLD | (channel << 8);
	printf("base:0x%x reg:0x%x, outhreshold:%x\n", base, reg, threshold);
	 WriteRegister( m_v1731_handle, base, reg, (threshold & 0xFFF), AM);
	
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//0x1n98
void     Module_v1731:: ChannelDACSet(int32_t  m_v1731_handle, uint32_t base, uint32_t channel, uint32_t dac, CVAddressModifier AM){
	uint32_t reg;
	reg =  CHANNEL_DAC | (channel << 8);
	printf("base:0x%x reg:0x%x, DAC:%x\n", base, reg, dac);
	 WriteRegister( m_v1731_handle, base, reg, (dac & 0xFFFF), AM);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int      Module_v1731:: ChannelDACGet(int32_t  m_v1731_handle, uint32_t base, uint32_t channel, uint32_t *dac, CVAddressModifier AM){
    uint32_t reg;
	int   status;

	reg =  CHANNEL_DAC | (channel << 8);
	*dac =  RegisterRead( m_v1731_handle, base, reg, AM);
	reg =  CHANNEL_STATUS | (channel << 8);
	status =  RegisterRead( m_v1731_handle, base, reg, AM);
	return status;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//0x1n80, 0x1n84, 0x1n98
void     Module_v1731:: ChannelSet(int32_t  m_v1731_handle, uint32_t base, uint32_t channel, uint32_t what, uint32_t that, CVAddressModifier AM){
	uint32_t reg, mask;

	if (what ==  CHANNEL_THRESHOLD)   mask = 0x0FFF;
	if (what ==  CHANNEL_OUTHRESHOLD) mask = 0x0FFF;
	if (what ==  CHANNEL_DAC)         mask = 0xFFFF;
	reg = what | (channel << 8);
	printf("base:0x%x reg:0x%x, this:%x\n", base, reg, that);
	 WriteRegister( m_v1731_handle, base, reg, (that & 0xFFF), AM);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//0x1n80, 0x1n84, 0x1n98
uint32_t Module_v1731:: ChannelGet(int32_t  m_v1731_handle, uint32_t base, uint32_t channel, uint32_t what, CVAddressModifier AM){
    uint32_t reg, mask;

	if (what ==  CHANNEL_THRESHOLD)   mask = 0x0FFF;
	if (what ==  CHANNEL_OUTHRESHOLD) mask = 0x0FFF;
	if (what ==  CHANNEL_DAC)         mask = 0xFFFF;
	reg = what | (channel << 8);
	return  RegisterRead( m_v1731_handle, base, reg, AM);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//0x8000
void     Module_v1731:: ChannelConfig(int32_t  m_v1731_handle, uint32_t base, uint32_t operation, CVAddressModifier AM){
	uint32_t reg;
	  
	 WriteRegister( m_v1731_handle, base,  CHANNEL_CONFIG, 0x10, AM);
	reg =  RegisterRead( m_v1731_handle, base,  CHANNEL_CONFIG, AM);  
	printf("Channel_config1: 0x%x\n",  RegisterRead( m_v1731_handle, base,  CHANNEL_CONFIG, AM) );  
	switch (operation) {
	  case  TRIGGER_UNDERTH:
		 WriteRegister( m_v1731_handle, base,  CHANNEL_CONFIG, (reg | 0x40), AM);
	    break;
	  case  TRIGGER_OVERTH:
		 WriteRegister( m_v1731_handle, base,  CHANNEL_CONFIG, (reg & ~(0x40)), AM);
	    break;
	  default:
	    break;
	  }
	printf("Channel_config2: 0x%x\n",  RegisterRead( m_v1731_handle, base,  CHANNEL_CONFIG, AM) );
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//0xEF00, 0x20
void     Module_v1731:: Align64Set(int32_t  m_v1731_handle, uint32_t base, CVAddressModifier AM){
	 WriteRegister( m_v1731_handle, base,  VME_CONTROL,  ALIGN64, AM);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int      Module_v1731:: ReadBuffer_and_Output(int32_t  m_v1731_handle, uint32_t base, int m_nsample, CVAddressModifier AM, uint32_t *rawdata){

	CVErrorCodes error_code;
	
	int nr_elem = (m_nsample / 4) * 2 + 4;  // 1/4 samples per DWORD, there are 2 CHs and 4 DWORDs for the header
	int error_status = 0;
	int size = nr_elem * 32 / 8;  // 32 bits per datum => 4 bytes per datum
	int count;
    
    error_code = CAENVME_MBLTReadCycle( m_v1731_handle, base, rawdata, size, cvA32_S_MBLT, &count);
    error_status = CAEN::ErrorDecode(error_code);
    

    
	if (error_status == 0){
	    return 0;
	}else{
		return 1;
	}
}
*/
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......







