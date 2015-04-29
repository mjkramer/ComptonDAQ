#include <cstdio>
#include "v1730.hh"
#include <unistd.h>
#include "CAENVMElib.h"
#include "CAEN_VME_def.hh"
#include "DataBlock.hh"
#include <iostream>
#include <string.h>

Module_v1730::Module_v1730():
  m_v1730_handle(0),
  m_bufferSize(0),
  m_eventBuffer(0),
  m_currentEvent(0),
  m_bufferEnd(0)
{
  ;
}

Module_v1730::~Module_v1730()
{
  if(m_eventBuffer){
    delete [] m_eventBuffer;
    m_eventBuffer = 0;
  }
}

int Module_v1730::InitializeVMEModule(){
  printf("****Initializing CAEN V1730 Digitizer****\n");
  m_v1730_handle = ModuleManager::GetHandle();

  // Enough space for ~1000 events
  m_bufferSize = 1028*1000;
  
  WriteRegister(ACQUISITION_CONTROL, 0x0,cvD32); //reset acquisition
  WriteRegister(SW_RESET, 0x1, cvD32); //software reset
  WriteRegister(SW_CLEAR, 0x2, cvD32); // clears all the memories
  
  //WriteRegister(ZS_NSAMP, 0, cvD32); //set zero supression zero
  //WriteRegister(ZS_NSAMP_CH2, 0, cvD32); //set zero supression zero

  WaitChannelStatus(0xA);
  WriteRegister(CHANNEL_EN_MASK, 0x5, cvD32);//enable channels 0 and 2
  //WriteRegister(CHANNEL_EN_MASK, 0x14, cvD32);//enable channels 2 and 4
  WaitChannelStatus(0xA);
  WriteRegister(CHANNEL_DAC, 0x2000, cvD32);//dc offset channel 0 
  WaitChannelStatus(0xA);
  WriteRegister(CHANNEL_DAC_CH2, 0x2000, cvD32);//dc offset channel 2
  WaitChannelStatus(0xA);
  //WriteRegister(CHANNEL_DAC_CH4, 0x2000, cvD32);//dc offset channel 4
  //WaitChannelStatus(0xA);

  CalibrateChannels(); //channel calibration

  WriteRegister(TRIG_SRCE_EN_MASK, 0x40000000,cvD32); //enable external trigger
  WriteRegister(POST_TRIGGER_SETTING, 0,cvD32); //we only want pre-trigger samples
  
  // Seperate buffer into 512 blocks 
  WriteRegister(BUFFER_ORGANIZATION, 0x09, cvD32); 
  // Waveform length: 800 samples (0x50 * 10), 1.6 us
  WriteRegister(CUSTOM_SIZE, 0x50, cvD32);
  
  printf("  --  OK\n");
  
  return 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DataBlock* Module_v1730::GetModuleBuffer(){  
  CVErrorCodes error_code;
  int error_status = 0;	
  
  if( BufferedEvents()<1 ){
    error_status = UpdateEventBuffer();
    if (error_status != 0) return NULL;
  }
  //std::cout << "Info: Module_v1730: Getting next event from buffer containing " 
  //	    << BufferedEvents() << " events." << std::endl;

  int version = 1;
  DataBlock_v1730* dataBlock = new DataBlock_v1730(version, GetNextEvent());
  return dataBlock;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

uint Module_v1730::GetEventSize(uint32_t* event){  
  // Read event size from event data block
  //  See pg. ?? in CAEN v1730 manual.
  return ((event[0] << 4) >> 4);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

uint Module_v1730::BufferedEvents(){  
  // Examine the event buffer and determine the current number of
  // buffered events

  // Check if a pointer to the current event is set
  if(m_currentEvent == 0){
    //std::cout << "Error: Module_v1730 buffer not initialized!" << std::endl;
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

uint32_t* Module_v1730::GetNextEvent(){  
  // Pop the next event from the top of the event buffer

  // Check if there is at least one event in the buffer
  if(BufferedEvents() < 1){
    std::cout << "Error: Module_v1730: Attempting to retrieve event from empty buffer!" << std::endl;
    return NULL;
  }

  // Check if a pointer to the current event is set
  if(m_currentEvent == 0){
    std::cout << "Error: Module_v1730: Buffer not initialized!" << std::endl;
    return NULL;
  }

  // Check for valid event size
  uint eventSize = GetEventSize(m_currentEvent);
  if(eventSize <= 0 
     || eventSize > ((int)(m_currentEvent - m_bufferEnd))){
    std::cout << "Error: Module_v1730: Invalid event size = "
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

int Module_v1730::UpdateEventBuffer(){  
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
    error_code = CAENVME_BLTReadCycle(m_v1730_handle, V1730::base, 
				      &m_eventBuffer[currentReadLength], 
				      readLength*4, // Bytes
				      cvA32_S_BLT, cvD32, &count);
    error_status = CAEN::ErrorDecode(error_code);
    if(error_status != 0 || count!=(readLength*4)){
      std::cout << "Error: Module_v1730: BLT read failed after reading "
		<< currentReadLength << " words." << std::endl;
      return error_status;
    }
    /*
    if(true){
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
    */
    currentReadLength += readLength;
  }
 // printf("events stored: %u %u (%u)\n", eStored,currentReadLength/eSize,eSize);

  // Set endpoint of currently read data
  m_bufferEnd = &m_eventBuffer[totalReadLength];

/*
  std::cout << "Info: Module_v1730: Read " << totalReadLength << " from board."
	    << std::endl;
  std::cout << "Info: Module_v1730: Should contain " << eStored << " events."
	    << std::endl;
  std::cout << "Info: Module_v1730: Contains " << BufferedEvents() << " events."
	    << std::endl;
*/
  // Return
  return error_status;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Module_v1730::GenerateSoftwareTrigger(){
  WriteRegister(SOFTWARE_TRIGGER,0x2, cvD32);
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Module_v1730::WriteRegister(int offset, uint32_t value, CVDataWidth width){
  uint32_t write = value;
  CVErrorCodes error_code;
  int error_status;
  
  error_code = CAENVME_WriteCycle(m_v1730_handle, V1730::base+offset, &write, V1730::am, width);
  error_status = CAEN::ErrorDecode(error_code);
  if(error_status != 0){printf("Error while writing digitizer registers!\n");}
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


uint32_t Module_v1730::ReadRegister(int offset, CVDataWidth width){
  uint32_t read;
  CVErrorCodes error_code;
  int error_status;
  
  error_code = CAENVME_ReadCycle(m_v1730_handle, V1730::base+offset, &read, V1730::am, width);
  error_status = CAEN::ErrorDecode(error_code);
  
  if(error_status){printf("Error while reading digitizer register.\n");}
  return read;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int Module_v1730::SetOnline(){	
  WriteRegister(SW_CLEAR, 0x2, cvD32); // clears all the memories
  WriteRegister(ACQUISITION_CONTROL, 0x4,cvD32);
  return 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int Module_v1730::SetOffline(){
  WriteRegister(ACQUISITION_CONTROL, 0x0,cvD32);
  return 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int Module_v1730::DataReady(){
  uint32_t status = 0;
  status =  ReadRegister(ACQUISITION_STATUS,cvD32);
  status = ((status << 28) >> 31);
  
  if (status == 1){return 1;}//ready
  else{return 0;}
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Module_v1730::CalibrateChannels(){
  // Run channel calibration process
  std::cout << "Warning: calibration disabled pending CAEN repair." << std::endl;
  return;
  //WaitChannelStatus(0xA);
  //WriteRegister(CHANNEL_CALIBRATION,0x1,cvD32);
  //WaitChannelStatus(0xA);
  return;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

bool Module_v1730::WaitChannelStatus(int expectedStatus){
  // Wait until channel status is correct before proceeding
  //  Return 'true' if status is reached, 'false' if tired of waiting.
  int maxWaits = 100;
  int nWaits = 0;
  int waitTime = 100000;
  while(nWaits < maxWaits){
    //int channelStat0 = (ReadRegister(0x1088,cvD32) & 0xF);
    //int channelStat2 = (ReadRegister(0x1288,cvD32) & 0xF);
    int channelStat0 = (ReadRegister(0x1288,cvD32) & 0xF);
    int channelStat2 = (ReadRegister(0x1488,cvD32) & 0xF);
    //std::cout << "  Chan Stat: " << channelStat0 << "\t" << channelStat2 
    //	      << std::endl;
    if(channelStat0==expectedStatus && channelStat2==expectedStatus){
      //std::cout << "  Channels ready!" << std::endl;
      return true;
    }
    usleep(waitTime); //wait for channel ready
    nWaits++;
  }
  std::cout << "   Warning: Channel status incorrect." << std::endl;
  return false;
}



