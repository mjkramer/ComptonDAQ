//v2718 - VME controller
//v1785 - VME peak sensing ADC
//v1290 - VME TDC
//v1720 - VME digitizer

#include "DataBlock.hh"



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DataBlock::DataBlock()
	:version(0), data(0){ 
//type(DataBlock::kUnknown)
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

std::vector<int> DataBlock_v1731::GetWaveform_Channel0(){
	int nr_elem = (nr_sample / 4) * 2 + 4;
	std::vector<int> CH0(nr_sample);
	
    for (int i=4 ; i<((nr_elem-4)/2 + 4) ;i++){ 
    	CH0.at((4*i-16)) = (int) (((DataBlock::data[i]) <<24 )>>24);
    	CH0.at((4*i-15)) = (int) (((DataBlock::data[i]) <<16 )>>24);
    	CH0.at((4*i-14)) = (int) (((DataBlock::data[i]) <<8  )>>24);
    	CH0.at((4*i-13)) = (int) ((DataBlock::data[i]) >>24);
    }
    
    return CH0;
    
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

std::vector<int> DataBlock_v1731::GetWaveform_Channel2(){
	int nr_elem = (nr_sample / 4) * 2 + 4;
	std::vector<int> CH2(nr_sample);
	
    for(int i=((nr_elem-4)/2 + 4); i<nr_elem; i++){ 
    	CH2.at(( 4*i-4*((nr_elem-4)/2 + 4) ))    = (int) (((DataBlock::data[i]) <<24 )>>24);
    	CH2.at(( 4*i-4*((nr_elem-4)/2 + 4) + 1)) = (int) (((DataBlock::data[i]) <<16 )>>24);
    	CH2.at(( 4*i-4*((nr_elem-4)/2 + 4) + 2)) = (int) (((DataBlock::data[i]) <<8  )>>24);
    	CH2.at(( 4*i-4*((nr_elem-4)/2 + 4) + 3)) = (int) ((DataBlock::data[i]) >>24);
    }
    
    return CH2;
    
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
	
	//int time_diff = list[chX][1] - list[chY][1];
	int time_diff = 0;

	//delete list;
	//list = 0;
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
	        value = (w & 0x3FFF);}
	
	
	return value;
	
}











