#include "v1785.hh"

int Module_v1785:: InitializeVMEModule(){

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int Module_v1785:: V1785_CSR1Read(int32_t handle, uint32_t base){
	
	int status = 0;  //Error code if any
	int read;
	
	status = CAENVME_ReadCycle(handle, base+V1785_CSR1_RO, &read, VME.AM, cvD16);
	return read;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int Module_v1785:: V1785_CSR2Read(int32_t handle, uint32_t base){
	
	int status = 0;  //Error code if any
	int read;
	
	status = CAENVME_ReadCycle(handle, base+V1785_CSR2_RO, &read, VME.AM, cvD16);
	return read;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int Module_v1785:: V1785_BitSet2Read(int32_t handle, uint32_t base){
	
	int status = 0;  //Error code if any
	int read;
	
	status = CAENVME_ReadCycle(handle, base+1785_BIT_SET2_RW, &read, VME.AM, cvD16);
	return read;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Module_v1785:: V1785_BitSet2Set(int32_t handle, uint32_t base, uint16_t pat){
	
	int status = 0;  //Error code if any
	
	status = CAENVME_WriteCycle(handle, base+V1785_BIT_SET2_RW, &pat, VME.AM, cvD16);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Module_v1785:: V1785_BitSet2Clear(int32_t handle, uint32_t base, uint16_t pat){
	
	int status = 0;  //Error code if any
	
	status = CAENVME_WriteCycle(handle, base+V1785_BIT_CLEAR2_WO, &pat, VME.AM, cvD16);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

uint16_t Module_v1785:: V1785_ControlRegister1Read(int32_t handle, uint32_t base){
	
	int status = 0;  //Error code if any
	uint16_t pat;
	
	status = CAENVME_ReadCycle(handle, base+V1785_CR1_RW, &pat, VME.AM, cvD16);
	return pat;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Module_v1785:: V1785_ControlRegister1Write(int32_t handle, uint32_t base, uint16_t pat){
	
	int status = 0;  //Error code if any
	
	status = CAENVME_WriteCycle(handle, base+V1785_CR1_RW, &pat, VME.AM, cvD16);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Module_v1785:: V1785_OnlineSet(int32_t handle, uint32_t base){
	
	int status = 0;  //Error code if any
	uint16_t write_online = 0x2;
	
	status = CAENVME_WriteCycle(handle, base+V1785_BIT_CLEAR2_WO, &write_online, VME.AM, cvD16);
	
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Module_v1785:: V1785_OfflineSet(int32_t handle, uint32_t base){
	
	int status = 0;  //Error code if any
	uint16_t write_offline = 0x2;
	
	status = CAENVME_WriteCycle(handle, base+V1785_BIT_SET2_RW, &write_offline, VME.AM, cvD16);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Module_v1785:: V1785_BlkEndEnable(int32_t handle, uint32_t base){
	
	int status_1 = 0;  //Error code if any
	int status_2 = 0;  //Error code if any
	uint16_t pat;

	status_1 = CAENVME_ReadCycle(handle, base+V1785_CR1_RW, &pat, VME.AM, cvD16);
	
	uint16_t write = (pat|0x04)&0x74;
	status_2 = CAENVME_WriteCycle(handle, base+V1785_CR1_RW, &write, VME.AM, cvD16);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......





//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int Module_v1785:: V1785_DataReady(int32_t handle, uint32_t base){
	int status = 0;  //Error code if any
	int read; //read from CAENVME_Readcycle
	int data_ready;
	
	status = CAENVME_ReadCycle(handle, base+V1785_CSR1_RO, &read, VME.AM, cvD16);
    data_ready = read & 0x1;
    return VME.success;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int Module_v1785:: V1785_BufferFull(int32_t handle, uint32_t base){
	
	int return_buffer_full, return_data_ready; //Error output of CAENVME_ReadCycle
	int read_buffer_full, read_data_ready;
	int buffer_full, data_ready;
	
	return_buffer_full = CAENVME_ReadCycle(handle, base+V1785_CSR2_RO, &read_buffer_full, VME.AM, cvD16);
	return_data_ready  = CAENVME_ReadCycle(handle, base+V1785_CSR1_RO, &read_data_ready, VME.AM, cvD16);
	
	buffer_full = return_buffer_full & 0x4;
	data_ready  = return_data_ready & 0x1;
	
	if(data_ready && buffer_full){
		return VME.success;
	}else{
		return VME.failure;
	}
		
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......




























