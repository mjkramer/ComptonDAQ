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
  
  //CalibrateChannels(); //channel calibration
  
  //WriteRegister(ZS_NSAMP, 0, cvD32); //set zero supression zero
  //WriteRegister(ZS_NSAMP_CH2, 0, cvD32); //set zero supression zero
  
  WriteRegister(CHANNEL_EN_MASK, 0x5, cvD32);//enable channels 0 and 2
  WriteRegister(CHANNEL_DAC, 0x19B0, cvD32);//dc offset
  WriteRegister(CHANNEL_DAC_CH2, 0x1FB0, cvD32);//dc offset
  usleep(1000); //wait for dc offset to be updated
 
  
  WriteRegister(TRIG_SRCE_EN_MASK, 0x40000000,cvD32); //enable external trigger
  WriteRegister(POST_TRIGGER_SETTING, 0,cvD32); //we only want pre-trigger samples
  
  WriteRegister(BUFFER_ORGANIZATION, 0x0A, cvD32); //seperate buffer into 1024 blocks (each can hold 4k samples @1Gs/s or 2k samples @500Ms/s)
  WriteRegister(CUSTOM_SIZE, 0x64, cvD32);
  
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
  // FIXME: DEBUG //return ((m_bufferEnd - m_currentEvent)/404);	
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
  //printf("events stored: %u\n", eStored);

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
 // printf("events stored: %u %u (%u)\n", eStored,currentReadLength/eSize,eSize);

  // Set endpoint of currently read data
  m_bufferEnd = &m_eventBuffer[totalReadLength];

/*
  std::cout << "Info: Module_v1731: Read " << totalReadLength << " from board."
	    << std::endl;
  std::cout << "Info: Module_v1731: Should contain " << eStored << " events."
	    << std::endl;
  std::cout << "Info: Module_v1731: Contains " << BufferedEvents() << " events."
	    << std::endl;
*/
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
  WriteRegister(SW_CLEAR, 0x2, cvD32); // clears all the memories
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






