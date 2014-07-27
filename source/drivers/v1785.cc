#include "v1785.hh"

int Module_v1785:: InitializeVMEModule(){

    return 1;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int Module_v1785:: V1785_CSR1Read(int32_t handle, uint32_t base, CVAddressModifier AM){
	
	CVErrorCodes error_code;  //Error code if any
	int read;
	
	error_code = CAENVME_ReadCycle(handle, base+V1785_CSR1_RO, &read, AM, cvD16);
	return read;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int Module_v1785:: V1785_CSR2Read(int32_t handle, uint32_t base, CVAddressModifier AM){
	
	CVErrorCodes error_code;  //Error code if any
	int read;
	
	error_code = CAENVME_ReadCycle(handle, base+V1785_CSR2_RO, &read, AM, cvD16);
	return read;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int Module_v1785:: V1785_BitSet2Read(int32_t handle, uint32_t base, CVAddressModifier AM){
	
	CVErrorCodes error_code;  //Error code if any
	int read;
	
	error_code = CAENVME_ReadCycle(handle, base+V1785_BIT_SET2_RW, &read, AM, cvD16);
	return read;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Module_v1785:: V1785_BitSet2Set(int32_t handle, uint32_t base, uint16_t pat, CVAddressModifier AM){
	
	CVErrorCodes error_code;  //Error code if any
	
	error_code = CAENVME_WriteCycle(handle, base+V1785_BIT_SET2_RW, &pat, AM, cvD16);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Module_v1785:: V1785_BitSet2Clear(int32_t handle, uint32_t base, uint16_t pat, CVAddressModifier AM){
	
	CVErrorCodes error_code;  //Error code if any
	
	error_code = CAENVME_WriteCycle(handle, base+V1785_BIT_CLEAR2_WO, &pat, AM, cvD16);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

uint16_t Module_v1785:: V1785_ControlRegister1Read(int32_t handle, uint32_t base, CVAddressModifier AM){
	
	CVErrorCodes error_code;  //Error code if any
	uint16_t pat;
	
	error_code = CAENVME_ReadCycle(handle, base+V1785_CR1_RW, &pat, AM, cvD16);
	return pat;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Module_v1785:: V1785_ControlRegister1Write(int32_t handle, uint32_t base, uint16_t pat, CVAddressModifier AM){
	
	CVErrorCodes error_code;  //Error code if any
	
	error_code = CAENVME_WriteCycle(handle, base+V1785_CR1_RW, &pat, AM, cvD16);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Module_v1785:: V1785_OnlineSet(int32_t handle, uint32_t base, CVAddressModifier AM){
	
	CVErrorCodes error_code;  //Error code if any
	uint16_t write_online = 0x2;
	
	error_code = CAENVME_WriteCycle(handle, base+V1785_BIT_CLEAR2_WO, &write_online, AM, cvD16);
	
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Module_v1785:: V1785_OfflineSet(int32_t handle, uint32_t base, CVAddressModifier AM){
	
	CVErrorCodes error_code;  //Error code if any
	uint16_t write_offline = 0x2;
	
	error_code = CAENVME_WriteCycle(handle, base+V1785_BIT_SET2_RW, &write_offline, AM, cvD16);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Module_v1785:: V1785_BlkEndEnable(int32_t handle, uint32_t base, CVAddressModifier AM){
	
	int error_code_1 = 0;  //Error code if any
	int error_code_2 = 0;  //Error code if any
	uint16_t pat;

	error_code_1 = CAENVME_ReadCycle(handle, base+V1785_CR1_RW, &pat, AM, cvD16);
	
	uint16_t write = (pat|0x04)&0x74;
	error_code_2 = CAENVME_WriteCycle(handle, base+V1785_CR1_RW, &write, AM, cvD16);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Module_v1785:: V1785_OverRangeEnable(int32_t handle, uint32_t base, CVAddressModifier AM){
	CVErrorCodes error_code;  //Error code if any
	uint16_t write = 0x08;

	error_code = CAENVME_WriteCycle(handle, base+V1785_BIT_CLEAR2_WO, &write, AM, cvD16);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void V1785_OverRangeDisable(int32_t handle, uint32_t base, CVAddressModifier AM){
	CVErrorCodes error_code;  //Error code if any
	uint16_t write = 0x08;

	error_code = CAENVME_WriteCycle(handle, base+V1785_BIT_SET2_RW, &write, AM, cvD16);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......






//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int Module_v1785:: V1785_DataReady(int32_t handle, uint32_t base, CVAddressModifier AM){
	CVErrorCodes error_code;  //Error code if any
	int read; //read from CAENVME_Readcycle
	int data_ready;
	
	error_code = CAENVME_ReadCycle(handle, base+V1785_CSR1_RO, &read, AM, cvD16);
    data_ready = read & 0x1;
    return 1;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int Module_v1785:: V1785_BufferFull(int32_t handle, uint32_t base, CVAddressModifier AM){
	
	CVErrorCodes error_code_buffer_full, error_code_data_ready;
	int read_buffer_full, read_data_ready;
	int buffer_full, data_ready;
	
	error_code_buffer_full = CAENVME_ReadCycle(handle, base+V1785_CSR2_RO, &read_buffer_full, AM, cvD16);
	error_code_data_ready  = CAENVME_ReadCycle(handle, base+V1785_CSR1_RO, &read_data_ready, AM, cvD16);
	
	buffer_full = read_buffer_full & 0x4;
	data_ready  = read_data_ready & 0x1;
	
	if(data_ready && buffer_full){
		return 1;
	}else{
		return 0;
	}
		
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......




























