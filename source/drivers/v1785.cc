#include  <stdlib.h>
#include  <stdint.h>
#include "CAENVMElib.h"
#include "CAEN_VME_def.hh"
#include "DataBlock.hh"
#include "v1785.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


int Module_v1785::InitializeVMEModule(){
	printf("****Initializing CAEN V1785 Peak ADC****");

   	int32_t Handle;
    	Handle = ModuleManager::GetHandle();
        CVErrorCodes error_code;


    if(IsPresent() == 0){
    	printf("Error: V1785 not found!\n");
        return 1;
    }

    //clears all the data, event counter, bit set, bit clear..
    //if(ResetModule()){
	//return 1;}

    //disable 7 out of 8 channels (only ch0-high is used) - each channel has a high&low
    uint16_t threshold[16] = {0x000, //ch0-high
    			    0x100, //ch1-low
    			 0x100,0x100,0x100,
    			 0x100,0x100,0x100,0x100,
    			 0x100,0x100,0x100,0x100,
    			 0x100,0x100,0x100};

    if(ThresholdWrite(threshold) != 8){
    	printf("Error: Disabling channels not successful!\n");
    	return 1;
    }

    printf(" --  OK\n");
    return 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DataBlock* Module_v1785::GetModuleBuffer(){
	int32_t Handle;
	Handle = ModuleManager::GetHandle();
	
    	int i;
	int nentry = 0;
	int value =0;
	int channel =0;
	int version = 1;
	uint32_t* rawdata = new uint32_t[MAX_CHANNELS+2];
	EventRead(rawdata, &nentry);

	for (i = 0; i < nentry; i++) {
	    uint32_t w = rawdata[i];
	    if (((w >> 24) & 0x7) != 0) continue;
	        channel = (w >> 17) & 0xF;
	        value = (w & 0x3FFF);}

	//DataBlock_v1785* datablock = new Datablock_v1785(version, rawdata);
	
	return NULL;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int Module_v1785::CSR1Read(){
	int32_t Handle;
	Handle = ModuleManager::GetHandle();
	
	CVErrorCodes error_code;  //Error code if any
	int read;
	int error_status;
	
	error_code = CAENVME_ReadCycle(Handle, V1785::base+CSR1_RO, &read, V1785::am, cvD16);
	error_status = CAEN::ErrorDecode(error_code);

	if (error_status == 0){  //success
		return read;

	}else{  //failure
		printf("CSR1Read could not be executed!\n");
		return 0;
	}

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int Module_v1785::CSR2Read(){
	int32_t Handle;
	Handle = ModuleManager::GetHandle();
	
	CVErrorCodes error_code;  //Error code if any
	int read;
	int error_status;
	
	error_code = CAENVME_ReadCycle(Handle, V1785::base+CSR2_RO, &read, V1785::am, cvD16);
	error_status = CAEN::ErrorDecode(error_code);

	if (error_status == 0){  //success
		return read;

	}else{  //failure
		printf("CSR2Read could not be executed!\n");
		return 0;
	}

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int Module_v1785::BitSet2Read(){
	int32_t Handle;
	Handle = ModuleManager::GetHandle();
	
	CVErrorCodes error_code;  //Error code if any
	int read;
	int error_status;
	
	error_code = CAENVME_ReadCycle(Handle, V1785::base+BIT_SET2_RW, &read, V1785::am, cvD16);
	error_status = CAEN::ErrorDecode(error_code);

	if (error_status == 0){  //success
		return read;

	}else{  //failure
		printf("BitSet2Read could not be executed!\n");
		return 0;
	}

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Module_v1785::BitSet2Set(uint16_t pat){
	int32_t Handle;
	Handle = ModuleManager::GetHandle();
	
	CVErrorCodes error_code;  //Error code if any
	int error_status;
	
	error_code = CAENVME_WriteCycle(Handle, V1785::base+BIT_SET2_RW, &pat, V1785::am, cvD16);
	error_status = CAEN::ErrorDecode(error_code);

	if (error_status == 0){  //success
		//do nothing

	}else{  //failure
		printf("BitSet2Set could not be executed!\n");

	}
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Module_v1785::BitSet2Clear(uint16_t pat){
	int32_t Handle;
	Handle = ModuleManager::GetHandle();
	
	CVErrorCodes error_code;  //Error code if any
	int error_status;
	
	error_code = CAENVME_WriteCycle(Handle, V1785::base+BIT_CLEAR2_WO, &pat, V1785::am, cvD16);
	error_status = CAEN::ErrorDecode(error_code);

	if (error_status == 0){  //success
		//do nothing

	}else{  //failure
		printf("BitSet2Clear could not be executed!\n");

	}
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

uint16_t Module_v1785::ControlRegister1Read(){
	int32_t Handle;
	Handle = ModuleManager::GetHandle();
	
	CVErrorCodes error_code;  //Error code if any
	uint16_t pat;
	int error_status;
	
	error_code = CAENVME_ReadCycle(Handle, V1785::base+CR1_RW, &pat, V1785::am, cvD16);
	error_status = CAEN::ErrorDecode(error_code);

	if (error_status == 0){  //success
		return pat;

	}else{
		printf("ControlRegister1Read could not be executed!\n");
		return 0;
	}

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Module_v1785::ControlRegister1Write(uint16_t pat){
	int32_t Handle;
	Handle = ModuleManager::GetHandle();
	
	CVErrorCodes error_code;  //Error code if any
	int error_status;
	
	error_code = CAENVME_WriteCycle(Handle, V1785::base+CR1_RW, &pat, V1785::am, cvD16);
	error_status = CAEN::ErrorDecode(error_code);

	if (error_status == 0){  //success
		//do nothing

	}else{  //failure
		printf("ControlRegister1Write could not be executed!\n");

	}
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


int Module_v1785::SetOnline(){

	int32_t Handle;
    Handle = ModuleManager::GetHandle();

	
	CVErrorCodes error_code;  //Error code if any
	uint16_t write_online = 0x2;
	int error_status;
	

	error_code = CAENVME_WriteCycle(Handle, V1785::base+BIT_CLEAR2_WO, &write_online, V1785::am, cvD16);
	error_status = CAEN::ErrorDecode(error_code);

	if (error_status == 0){  //success
		return 0;
	}else{  //failure

		printf("OnlineSet could not be executed successfully!\n");
		return 1;

	}
	
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


int Module_v1785::SetOffline(){

	int32_t Handle;
    Handle = ModuleManager::GetHandle();

	
	CVErrorCodes error_code;  //Error code if any
	uint16_t write_offline = 0x2;
	int error_status;
	

	error_code = CAENVME_WriteCycle(Handle, V1785::base+BIT_SET2_RW, &write_offline, V1785::am, cvD16);
	error_status = CAEN::ErrorDecode(error_code);

	if (error_status == 0){  //success
		return 0;

	}else{  //failure
		printf("OfflineSet could not be executed successfully.\n");
		return 1;;

	}
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Module_v1785::BlkEndEnable(){
	int32_t Handle;
	Handle = ModuleManager::GetHandle();
	
	CVErrorCodes error_code_1;  //Error code if any
	CVErrorCodes error_code_2;  //Error code if any
	uint16_t pat;
	int error_status_1;
	int error_status_2;

	error_code_1 = CAENVME_ReadCycle(Handle, V1785::base+CR1_RW, &pat, V1785::am, cvD16);
	error_status_1 = CAEN::ErrorDecode(error_code_1);
	
	uint16_t write = (pat|0x04)&0x74;
	error_code_2 = CAENVME_WriteCycle(Handle, V1785::base+CR1_RW, &write, V1785::am, cvD16);
	error_status_2 = CAEN::ErrorDecode(error_code_2);

	if ( (error_status_1 == 1) && (error_status_2 == 1) ){  //success
		//do nothing

	}else{  //failure
		printf("BlkEndEnable could not be executed!\n");

	}

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Module_v1785::OverRangeEnable(){
	int32_t Handle;
	Handle = ModuleManager::GetHandle();
	
	CVErrorCodes error_code;  //Error code if any
	uint16_t write = 0x08;
	int error_status;

	error_code = CAENVME_WriteCycle(Handle, V1785::base+BIT_CLEAR2_WO, &write, V1785::am, cvD16);
	error_status = CAEN::ErrorDecode(error_code);

	if (error_status == 0){  //success
		//do nothing

	}else{  //failure
		printf("OverRangeEnable could not be executed!\n");

	}
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Module_v1785::OverRangeDisable(){
	int32_t Handle;
	Handle = ModuleManager::GetHandle();
	
	CVErrorCodes error_code;  //Error code if any
	uint16_t write = 0x08;
	int error_status;

	error_code = CAENVME_WriteCycle(Handle, V1785::base+BIT_SET2_RW, &write, V1785::am, cvD16);
	error_status = CAEN::ErrorDecode(error_code);

	if (error_status == 0){  //success
		//do nothing

	}else{  //failure
		printf("OverRangeEnable could not be executed!\n");

	}
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Module_v1785::LowThEnable(){
	int32_t Handle;
	Handle = ModuleManager::GetHandle();
	
	CVErrorCodes error_code;
	uint16_t write = 0x10;
	int error_status;

	error_code = CAENVME_WriteCycle(Handle, V1785::base+BIT_CLEAR2_WO, &write, V1785::am, cvD16);
	error_status = CAEN::ErrorDecode(error_code);

	if (error_status == 0){  //success
		//do nothing

	}else{  //failure
		printf("LowThEnable could not be executed!\n");

	}
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Module_v1785::LowThDisable(){
	int32_t Handle;
	Handle = ModuleManager::GetHandle();
	
	CVErrorCodes error_code;
	uint16_t write = 0x10;
	int error_status;

	error_code = CAENVME_WriteCycle(Handle, V1785::base+BIT_SET2_RW, &write, V1785::am, cvD16);
	error_status = CAEN::ErrorDecode(error_code);

	if (error_status == 0){  //success
		//do nothing

	}else{  //failure
		printf("LowThDisable could not be executed!\n");

	}
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Module_v1785::EmptyEnable(){
	int32_t Handle;
	Handle = ModuleManager::GetHandle();
	
	CVErrorCodes error_code;
	uint16_t write = 0x1000;
	int error_status;

	error_code = CAENVME_WriteCycle(Handle, V1785::base+BIT_SET2_RW, &write, V1785::am, cvD16);
	error_status = CAEN::ErrorDecode(error_code);

	if (error_status == 0){  //success
		//do nothing

	}else{  //failure
		printf("EmptyEnable could not be executed!\n");

	}
}





//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int Module_v1785::DataReady(){

	
	int32_t Handle;
    	Handle = ModuleManager::GetHandle();


	CVErrorCodes error_code;  //Error code if any
	int read; //read from CAENVME_Readcycle
	int data_ready;
	int error_status;
	

	error_code = CAENVME_ReadCycle(Handle, V1785::base+CSR1_RO, &read, V1785::am, cvD16);

	error_status = CAEN::ErrorDecode(error_code);
    	data_ready = read & 0x1;

    if (error_status == 0){  //success
    	return data_ready;  //1 for data being ready, 0 for data not being ready
    }

else{  //failure
    	printf("DataReady could not be executed successfully!\n");
    	return 0; 

    }

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int Module_v1785::BufferFull(){
	int32_t Handle;
	Handle = ModuleManager::GetHandle();
	
	CVErrorCodes error_code_buffer_full, error_code_data_ready;
	int read_buffer_full, read_data_ready;
	int buffer_full, data_ready;
	int error_status_buffer_full, error_status_data_ready;
	
	error_code_buffer_full = CAENVME_ReadCycle(Handle, V1785::base+CSR2_RO, &read_buffer_full, V1785::am, cvD16);
	error_status_buffer_full = CAEN::ErrorDecode(error_code_buffer_full);

	error_code_data_ready  = CAENVME_ReadCycle(Handle, V1785::base+CSR1_RO, &read_data_ready, V1785::am, cvD16);
	error_status_data_ready = CAEN::ErrorDecode(error_code_data_ready);
	
	buffer_full = read_buffer_full & 0x4;
	data_ready  = read_data_ready & 0x1;
	
	if ( (error_status_buffer_full == 1) && (error_status_data_ready == 1) ){  //Both read cycles are successfully executed

		if(data_ready && buffer_full){
			return data_ready;
		}else{
			return 0;
		}

	}else{
		printf("BufferFull could not be executed!\n");
		return 0;
	}

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int Module_v1785::isEvtReady(){
	int32_t Handle;
	Handle = ModuleManager::GetHandle();
	int csr;
	csr = CSR1Read();  //Function defined within the sV1785::ame class
	return (csr & 0x100);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int Module_v1785::isBusy(){
	int32_t Handle;
	Handle = ModuleManager::GetHandle();
	
	int busy, timeout, read, error_status;
	CVErrorCodes error_code;
	timeout = 1000;

	 do {
	     error_code = CAENVME_ReadCycle(Handle, V1785::base+CSR1_RO, &read, V1785::am, cvD16);
	     busy = read & 0x4;
	     timeout--;
	   } while (busy || timeout);

	 error_status = CAEN::ErrorDecode(error_code);
	 if (error_status == 0){  //success
	 return (busy != 0 ? 1 : 0);

	 }else{
		 printf("isBusy could not be executed!\n");
		 return -1;
	 }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int Module_v1785::EventRead(uint32_t *pdest, int *nentry){
	int32_t Handle;
	Handle = ModuleManager::GetHandle();
#define USE_BLT_READ_2

#ifdef USE_SINGLE_READ
  
  uint32_t hdata, data;
  CVErrorCodes error_code_1, error_code_2;
  //  int error_status, error_status_2;

  //D32 mode

  *nentry = 0;
  if (DataReady(Handle, V1785::base)) {
    do {
      error_code_1 = CAENVME_ReadCycle(Handle, V1785::base, &hdata, V1785::am, cvD32);
      //  error_status_1 = CAEN::ErrorDecode(error_code_1);
    } while (!(hdata & 0x02000000)); // skip up to the header

    pdest[*nentry] = hdata;
    *nentry += 1;
    do {
      error_code_2 = CAENVME_ReadCycle(Handle, V1785::base, &data, V1785::am, cvD32);
      //  error_status_2 = CAEN::ErrorDecode(error_code_2);
      pdest[*nentry] = data;
      *nentry += 1;
    } while (!(pdest[*nentry-1] & 0x04000000)); // copy until the trailer

    nentry--;
  }

#endif // USE_SINGLE_READ

#ifdef USE_BLT_READ_1
  
  int i;
  uint32_t hdata, data[MAX_CHANNELS+2], data;
  int count;
  CVErrorCodes error_code_1, error_code_2;
  //  int error_status_1, error_status_2;

  *nentry = 0;
  if (DataReady(Handle, V1785::base, V1785::am)) {
    do {
      error_code_1 = CAENVME_ReadCycle(Handle, V1785::base, &hdata, V1785::am, cvD32);
      //  error_status_1 = CAEN::ErrorDecode(error_code_1);
    } while (!(hdata & 0x02000000)); // skip up to the header

    V1785::am = cvA32_S_BLT;
    count = (hdata >> 8) & 0x3F;

    //mvme_read(mvme, data, V1785::base, (cnt+1)*4);
    error_code_2 = CAENVME_BLTReadCycle(Handle, V1785::base, data, (count+1)*4, V1785::am, cvD32, &count);
    //  error_status_2 = CAEN::ErrorDecode(error_code_2);

    pdest[0] = hdata;
    for (i=1 ;i<=count+1 ;i++)
      pdest[i] = data[i-1];

    *nentry = count + 2;
  }

#endif // USE_BLT_READ_1

#ifdef USE_BLT_READ_2
  
  int count;
  CVErrorCodes error_code;
  int error_status;

  *nentry = 0;
//  if (DataReady(Handle, V1785::base)) {
    //V1785::am = cvA32_S_BLT;

    //  mvme_read(mvme, pdest, V1785::base, (MAX_CHANNELS+2)*4); //Handle, data_pointer, V1785::base_address, number of entries
    error_code = CAENVME_BLTReadCycle(Handle, V1785::base, pdest, (MAX_CHANNELS+2)*4, V1785::am, cvD32, &count);
    error_status = CAEN::ErrorDecode(error_code);
    count = (pdest[0] >> 8) & 0x3F;
    *nentry = count + 2;
//  }

#endif //USE_BLT_READ_2

  return *nentry;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int Module_v1785::DataRead(uint32_t *pdest, int nentry){
	int32_t Handle;
	Handle = ModuleManager::GetHandle();
	CVErrorCodes error_code;
	//int error_status;
	int i, n;
	n = 0;

	if (DataReady()){
		for (i=0; i < ((nentry*4)>>2); i++){
		    error_code = CAENVME_ReadCycle(Handle, V1785::base, pdest+(i<<2), V1785::am, cvD32);
		    n = (nentry*4);
		}
	}
	return n;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int Module_v1785::DeleteBuffer(){
	int32_t Handle;
	Handle = ModuleManager::GetHandle();
	CVErrorCodes error_code_1, error_code_2;
	int error_status_1, error_status_2;
	uint16_t write = 0x4;
	

	error_code_1 = CAENVME_WriteCycle(Handle, V1785::base+BIT_SET2_RW, &write, V1785::am, cvD16);
	error_status_1 = CAEN::ErrorDecode(error_code_1);

	error_code_2 = CAENVME_WriteCycle(Handle, V1785::base+BIT_CLEAR2_WO, &write, V1785::am, cvD16);
	error_status_2 = CAEN::ErrorDecode(error_code_2);

	if ( (error_status_1 == 1) && (error_status_2 == 1) ){  //success
		return 0;
	}else{
		printf("DataClear could not be executed!\n");
		return 1;
	}
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Module_v1785::EvtCntRead(uint32_t *evtcnt){
	int32_t Handle;
	Handle = ModuleManager::GetHandle();
	
	CVErrorCodes error_code_1, error_code_2;  //Error code if any
	int read_1, read_2;
	int error_status_1, error_status_2;

	error_code_1 = CAENVME_ReadCycle(Handle, V1785::base+EVT_CNT_L_RO, &read_1, V1785::am, cvD16);
	*evtcnt = read_1;
	error_status_1 = CAEN::ErrorDecode(error_code_1);

	error_code_2 = CAENVME_ReadCycle(Handle, V1785::base+EVT_CNT_H_RO, &read_2, V1785::am, cvD16);
	*evtcnt += (read_2 << 16);
	error_status_2 = CAEN::ErrorDecode(error_code_2);

	if ( (error_status_1 == 1) == (error_status_2 == 1) ){  //success
        //do nothing

	}else{  //failure
		printf("EvtCntRead could not be executed!\n");
	}
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Module_v1785::EvtCntReset(){
	int32_t Handle;
	Handle = ModuleManager::GetHandle();
	CVErrorCodes error_code;
    int error_status;
    uint16_t write = 1;
    

    error_code = CAENVME_WriteCycle(Handle, V1785::base+EVT_CNT_RST_WO, &write, V1785::am, cvD16);
    error_status = CAEN::ErrorDecode(error_code);

    if (error_status == 0){
    	//do nothing
    }else{
    	printf("EvtCntRReset could not be executed!\n");
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Module_v1785::IntSet(int level, int vector){
	int32_t Handle;
	Handle = ModuleManager::GetHandle();
	CVErrorCodes error_code;
    int error_status;
    uint16_t write = vector & 0xFF;
    

    error_code = CAENVME_WriteCycle(Handle, V1785::base+INT_VECTOR_RW, &write, V1785::am, cvD16);
    error_status = CAEN::ErrorDecode(error_code);

    if (error_status == 0){
    	//do nothing
    }else{
    	printf("EIntSet could not be executed!\n");
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Module_v1785::IntEnable(int level){
	int32_t Handle;
	Handle = ModuleManager::GetHandle();
	CVErrorCodes error_code;
    int error_status;
    uint16_t write = level & 0x1F;
    

    error_code = CAENVME_WriteCycle(Handle, V1785::base+EVTRIG_REG_RW, &write, V1785::am, cvD16);
    error_status = CAEN::ErrorDecode(error_code);

    if (error_status == 0){
    	//do nothing
    }else{
    	printf("IntEnable could not be executed!\n");
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Module_v1785::IntDisable(){
	int32_t Handle;
	Handle = ModuleManager::GetHandle();
	CVErrorCodes error_code;
    int error_status;
    uint16_t write = 0;
    

    error_code = CAENVME_WriteCycle(Handle, V1785::base+EVTRIG_REG_RW, &write, V1785::am, cvD16);
    error_status = CAEN::ErrorDecode(error_code);

    if (error_status == 0){
    	//do nothing
    }else{
    	printf("IntDisable could not be executed!\n");
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Module_v1785::EvtTriggerSet(int count){
	int32_t Handle;
	Handle = ModuleManager::GetHandle();
	CVErrorCodes error_code;
    int error_status;
    uint16_t write = count & 0x1F;
    

    error_code = CAENVME_WriteCycle(Handle, V1785::base+EVTRIG_REG_RW, &write, V1785::am, cvD16);
    error_status = CAEN::ErrorDecode(error_code);

    if (error_status == 0){
    	//do nothing
    }else{
    	printf("EvtTriggerSet could not be executed!\n");
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Module_v1785::SingleShotReset(){
	int32_t Handle;
	Handle = ModuleManager::GetHandle();
	CVErrorCodes error_code;
    int error_status;
    uint16_t write = 1;
    

    error_code = CAENVME_WriteCycle(Handle, V1785::base+SINGLE_RST_WO, &write, V1785::am, cvD16);
    error_status = CAEN::ErrorDecode(error_code);

    if (error_status == 0){
    	//do nothing
    }else{
    	printf("SingleShotReset could not be executed!\n");
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int Module_v1785::ResetModule(){
    int32_t Handle;
    Handle = ModuleManager::GetHandle();
    CVErrorCodes error_code_1, error_code_2;
    int error_status_1, error_status_2;
    uint16_t write = SOFT_RESET;
    

    error_code_1 = CAENVME_WriteCycle(Handle, V1785::base+BIT_SET1_RW, &write, V1785::am, cvD16);
    error_status_1 = CAEN::ErrorDecode(error_code_1);

    error_code_2 = CAENVME_WriteCycle(Handle, V1785::base+BIT_CLEAR1_RW, &write, V1785::am, cvD16);
    error_status_2 = CAEN::ErrorDecode(error_code_2);

    if ( (error_status_1 == 1) && (error_status_2 == 1) ){
    	return 0;
    }else{
    	printf("ResetModule could not be executed!\n");
	return 1;
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Module_v1785::Trigger(){
	int32_t Handle;
	Handle = ModuleManager::GetHandle();
	CVErrorCodes error_code;
    int error_status;
    uint16_t write = 0;
    

    error_code = CAENVME_WriteCycle(Handle, V1785::base+SWCOMM_WO, &write, V1785::am, cvD16);
    error_status = CAEN::ErrorDecode(error_code);

    if (error_status == 0){
    	//do nothing
    }else{
    	printf("Trigger could not be executed!\n");
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int Module_v1785::ThresholdRead(uint16_t *threshold){
	int32_t Handle;
	Handle = ModuleManager::GetHandle();
    int k;
    uint16_t read;
    CVErrorCodes error_code;
    //int error_status;
    //

	for (k=0; k < MAX_CHANNELS*2; k++){
        error_code = CAENVME_ReadCycle(Handle, V1785::base+THRES_BASE + 2*k, &read, V1785::am, cvD16);
        threshold[k] = read & 0x1FF;
        //error_status = CAEN::ErrorDecode(error_code);
	}

	return MAX_CHANNELS;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int Module_v1785::ThresholdWrite(uint16_t *threshold){
	int32_t Handle;
	Handle = ModuleManager::GetHandle();
	int k;
	uint16_t write;
	uint16_t read;
	CVErrorCodes error_code_1, error_code_2;
    //int error_status_1, error_status_2;
    //

	for (k=0; k < MAX_CHANNELS*2; k++){
		write = threshold[k] & 0x1FF;
		error_code_1 = CAENVME_WriteCycle(Handle, V1785::base+THRES_BASE + 4*k, &write, V1785::am, cvD16);
		//error_status_1 = CAEN::ErrorDecode(error_code_1);
	}

	for (k=0; k < MAX_CHANNELS*2; k++){
        error_code_2 = CAENVME_ReadCycle(Handle, V1785::base+THRES_BASE + 2*k, &read, V1785::am, cvD16);
        threshold[k] = read & 0x1FF;
	}
	return MAX_CHANNELS;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Module_v1785::Status(){
	int32_t Handle;
	Handle = ModuleManager::GetHandle();
	int i;
	CVErrorCodes err;
	uint16_t threshold[MAX_CHANNELS];
	uint32_t evtcnt;
	uint16_t read;

	printf("V1785 at VME A32 0x%08x:\n", V1785::base);
	err = CAENVME_ReadCycle(Handle, V1785::base+FIRM_REV, &read, V1785::am, cvD16);
	printf("Firmware revision: 0x%x\n",read);

	if (read == 0xFFFF) {
	    printf("Module not present!\n");
	    return;
	  }

	read = CSR1Read();
	printf("CSR1: 0x%x\n", read);
	printf("DataReady    :%s\t", read & 0x1 ? "Y" : "N");
	printf(" - Global Dready:%s\t", read & 0x2 ? "Y" : "N");
	printf(" - Busy         :%s\n", read & 0x4 ? "Y" : "N");
	printf("Global Busy  :%s\t", read & 0x8 ? "Y" : "N");
	printf(" - V1785::amnesia      :%s\t", read & 0x10 ? "Y" : "N");
	printf(" - Purge        :%s\n", read & 0x20 ? "Y" : "N");
	printf("Term ON      :%s\t", read & 0x40 ? "Y" : "N");
	printf(" - TermOFF      :%s\t", read & 0x80 ? "Y" : "N");
	printf(" - Event Ready  :%s\n", read & 0x100 ? "Y" : "N");

	read = CSR2Read();
	printf("CSR2: 0x%x\n", read);
	printf("Buffer Empty :%s\t", read & 0x2 ? "Y" : "N");
	printf(" - Buffer Full  :%s\n", read & 0x4 ? "Y" : "N");

	int dtype = (read & 0xF0) >> 4;
	printf("Daughter card type (CSEL/DSEL)  :%d%d%d%d (0x%x) ",
	         read & 0x80 ? 1 : 0,
	         read & 0x40 ? 1 : 0,
	         read & 0x20 ? 1 : 0,
	         read & 0x10 ? 1 : 0,
	         dtype);
	  switch (dtype) {
	  default:
	    printf("\n");
	    break;
	  case 2:
	    printf("V1785 32ch QDC\n");
	    break;
	  }
	  read = BitSet2Read();
	  printf("BitSet2: 0x%x\n", read);
	  printf("Test Mem     :%s\t", read & 0x1 ? "Y" : "N");
	  printf(" - Offline      :%s\t", read & 0x2 ? "Y" : "N");
	  printf(" - Clear Data   :%s\n", read & 0x4  ? "Y" : "N");
	  printf("Over Range En:%s\t", read & 0x8  ? "Y" : "N");
	  printf(" - Low Thres En :%s\t", read & 0x10 ? "Y" : "N");
	  printf(" - Auto Incr    :%s\n", read & 0x800 ? "Y" : "N");
	  printf("Empty Enable :%s\t", read & 0x1000 ? "Y" : "N");
	  printf(" - Slide sub En :%s\t", read & 0x2000 ? "Y" : "N");
	  printf(" - All Triggers :%s\n", read & 0x4000 ? "Y" : "N");

	  EvtCntRead(&evtcnt);
	  printf("Event counter: %d\n", evtcnt);

	  ThresholdRead(threshold);
	  for (i=0;i<MAX_CHANNELS*2;i+=2) {
	    printf("Threshold[%2i] = 0x%4.4x\t   -  ", i, threshold[i]);
	    printf("Threshold[%2i] = 0x%4.4x\n", i+1, threshold[i+1]);
	  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int Module_v1785::IsPresent(){
	int32_t Handle;
	Handle = ModuleManager::GetHandle();
	int read = 0;
	CVErrorCodes err;

	err =  CAENVME_ReadCycle(Handle, V1785::base+FIRM_REV, &read, V1785::am, cvD16);
	  if (read == 0xFFFF)
	    return 0;
	  else
	    return 1;
}

























