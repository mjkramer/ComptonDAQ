#include "DataBlock.hh"
#include <iostream>


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DataBlock::DataBlock():version(0), data(0){ 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DataBlock::DataBlock(DataBlock::Type_t type, int version, uint32_t* data)
	:type(type), version(version), data(data){ 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DataBlock::~DataBlock(){   
  delete [] data;
  data = 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int DataBlock:: GetVersion(){
  return 0;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DataBlock_v1731::DataBlock_v1731(int version, uint32_t* data):DataBlock::DataBlock(v1731, version, data){

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

unsigned int DataBlock_v1731::GetEventSize(){
  if(!data){
    std::cout << "Error: DataBlock_v1731 is not initialized!" << std::endl;
    return 0;
  }
  return ((data[0] << 4) >> 4);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

unsigned int DataBlock_v1731::GetNBlocks(){
  if(!data){
    std::cout << "Error: DataBlock_v1731 is not initialized!" << std::endl;
    return 0;
  }
  
  uint32_t channelMask = data[1] & 0xFF;
  unsigned int nBlocks = 0;
  for(int i=0; i<8; i++){
    nBlocks += (channelMask >> i) & 0x1;
  }
  return nBlocks;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

unsigned int DataBlock_v1731::GetBlockSize(){
  if(!data){
    std::cout << "Error: DataBlock_v1731 is not initialized!" << std::endl;
    return 0;
  }
  unsigned int headerSize = 4; // FIXME: move to definitions
  return (GetEventSize() - headerSize)/GetNBlocks();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

unsigned int DataBlock_v1731::GetNumberOfSamples(){
  if(!data){
    std::cout << "Error: DataBlock_v1731 is not initialized!" << std::endl;
    return 0;
  }
  unsigned int waveformBlockSize = GetBlockSize();
  unsigned int samplesPerWord = 4; // FIXME: Move this to module definitions
  return (waveformBlockSize * samplesPerWord);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

std::vector<unsigned int> DataBlock_v1731::GetWaveform(const unsigned int blockIndex){
  // Return a vector containing the waveform samples
  //  blockIndex: count from 0 to n over the number of active read-out channels
  //              (NOT the channel number!)

  std::vector<unsigned int> waveform(0);
  if(!data){
    std::cout << "Error: DataBlock_v1731 is not initialized!" << std::endl;
    return waveform;
  }
  unsigned int headerSize = 4; // FIXME: move to definitions
  unsigned int samplesPerWord = 4; // FIXME: Move this to module definitions
  unsigned int sampleMask = 0xFF; // FIXME: move to definitions
  unsigned int blockSize = GetBlockSize();

  if(blockIndex >= GetNBlocks()){
    std::cout << "Error: DataBlock_v1731: Requesting invalid block " 
	      << blockIndex << " of known " << GetNBlocks() << " blocks."
	      << std::endl;
    return waveform;
  }

  waveform.resize(blockSize * samplesPerWord);
  uint32_t* waveStart = data + headerSize + blockIndex*blockSize;
  for(int i=0; i<blockSize; i++){ 
    waveform[4*i]   = ( (waveStart[i] >> 0) & sampleMask );
    waveform[4*i+1] = ( (waveStart[i] >> 8) & sampleMask );
    waveform[4*i+2] = ( (waveStart[i] >> 16) & sampleMask );
    waveform[4*i+3] = ( (waveStart[i] >> 24) & sampleMask );
  }

  return waveform;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DataBlock_v1730::DataBlock_v1730(int version, uint32_t* data):DataBlock::DataBlock(v1730, version, data){

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

unsigned int DataBlock_v1730::GetEventSize(){
  if(!data){
    std::cout << "Error: DataBlock_v1730 is not initialized!" << std::endl;
    return 0;
  }
  return ((data[0] << 4) >> 4);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

unsigned int DataBlock_v1730::GetNBlocks(){
  if(!data){
    std::cout << "Error: DataBlock_v1730 is not initialized!" << std::endl;
    return 0;
  }
  
  uint32_t channelMask = data[1] & 0xFF;
  unsigned int nBlocks = 0;
  for(int i=0; i<8; i++){
    nBlocks += (channelMask >> i) & 0x1;
  }
  return nBlocks;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

unsigned int DataBlock_v1730::GetBlockSize(){
  if(!data){
    std::cout << "Error: DataBlock_v1730 is not initialized!" << std::endl;
    return 0;
  }
  unsigned int headerSize = 4; // FIXME: move to definitions
  return (GetEventSize() - headerSize)/GetNBlocks();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

unsigned int DataBlock_v1730::GetNumberOfSamples(){
  if(!data){
    std::cout << "Error: DataBlock_v1730 is not initialized!" << std::endl;
    return 0;
  }
  unsigned int waveformBlockSize = GetBlockSize();
  unsigned int samplesPerWord = 2; // FIXME: Move this to module definitions
  return (waveformBlockSize * samplesPerWord);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

std::vector<unsigned int> DataBlock_v1730::GetWaveform(const unsigned int blockIndex){
  // Return a vector containing the waveform samples
  //  blockIndex: count from 0 to n over the number of active read-out channels
  //              (NOT the channel number!)

  std::vector<unsigned int> waveform(0);
  if(!data){
    std::cout << "Error: DataBlock_v1730 is not initialized!" << std::endl;
    return waveform;
  }
  unsigned int headerSize = 4; // FIXME: move to definitions
  unsigned int samplesPerWord = 2; // FIXME: Move this to module definitions
  unsigned int sampleMask = 0xFFFF; // FIXME: move to definitions
  unsigned int blockSize = GetBlockSize();

  if(blockIndex >= GetNBlocks()){
    std::cout << "Error: DataBlock_v1730: Requesting invalid block " 
	      << blockIndex << " of known " << GetNBlocks() << " blocks."
	      << std::endl;
    return waveform;
  }

  waveform.resize(blockSize * samplesPerWord);
  uint32_t* waveStart = data + headerSize + blockIndex*blockSize;
  for(int i=0; i<blockSize; i++){ 
    waveform[2*i]   = ( (waveStart[i] >> 0) & sampleMask );
    waveform[2*i+1] = ( (waveStart[i] >> 16) & sampleMask );
  }

  return waveform;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DataBlock_v1290::DataBlock_v1290(int version, uint32_t* data):DataBlock::DataBlock(v1290N, version, data){

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int DataBlock_v1290::GetTimeDifference(int chX, int chY){
	
	
	int header[nr_entry];
	int time[nr_entry];
	int channel[nr_entry];
	
	int init, final;
	
	for (int i=0; i<nr_entry; i++){
		header[i] = (int) (DataBlock::data[i] >> 27);  //Bits [31,27] identify the type
	}
	
	int i = 0;
	while (header[i] != 0){  //locate the first TDC measurement
		i = i + 1;
	}
	init = i;
	
	while (header[i] == 0){  //record until reaching the trailer
		channel[i] = (int) ((DataBlock::data[i] << 6) >> 27);
		time[i]    = (int) ((DataBlock::data[i] << 11) >> 11);
	    i = i + 1;
	}
	
	final = i;
	
	const int length = (final - init);
	
	int** list = new int *[length];
	for(int k=0; k<length; k++){
		list[k] = new int[length];
	}
	
	for(int j=init; j<=final; j++){
		list[j][0] = channel[j];
		list[j][1] = time[j];
	}
	
	int time_diff = 0;

	return time_diff;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DataBlock_v1785::DataBlock_v1785(int version, uint32_t* data):DataBlock::DataBlock(v1785, version, data){

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int DataBlock_v1785::GetPeak(){
	
	int i;
	int value = 0;
	int channel = 0;
	
	for (i = 0; i < nr_entry; i++) {
	    uint32_t w = DataBlock::data[i];
	    
	    if (((w >> 24) & 0x7) != 0) continue;
	        channel = (w >> 17) & 0xF;
	        value = (w & 0x3FFF);
	}

	return value;
	
}











