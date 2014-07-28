#include "v1785.hh"

int Module_v1785:: InitializeVMEModule(){

    return 1;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int Module_v1785:: V1785_CSR1Read(int32_t handle, uint32_t base, CVAddressModifier AM){
	
	CVErrorCodes error_code;  //Error code if any
	int read;
	int error_status;
	
	error_code = CAENVME_ReadCycle(handle, base+V1785_CSR1_RO, &read, AM, cvD16);
	error_status = CAEN::ErrorDecode(error_code);

	if (error_status == 1){  //success
		return read;

	}else{  //failure
		printf("V1785_CSR1Read could not be executed!\n");
		return 0;
	}

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int Module_v1785:: V1785_CSR2Read(int32_t handle, uint32_t base, CVAddressModifier AM){
	
	CVErrorCodes error_code;  //Error code if any
	int read;
	int error_status;
	
	error_code = CAENVME_ReadCycle(handle, base+V1785_CSR2_RO, &read, AM, cvD16);
	error_status = CAEN::ErrorDecode(error_code);

	if (error_status == 1){  //success
		return read;

	}else{  //failure
		printf("V1785_CSR2Read could not be executed!\n");
		return 0;
	}

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int Module_v1785:: V1785_BitSet2Read(int32_t handle, uint32_t base, CVAddressModifier AM){
	
	CVErrorCodes error_code;  //Error code if any
	int read;
	int error_status;
	
	error_code = CAENVME_ReadCycle(handle, base+V1785_BIT_SET2_RW, &read, AM, cvD16);
	error_status = CAEN::ErrorDecode(error_code);

	if (error_status == 1){  //success
		return read;

	}else{  //failure
		printf("V1785_BitSet2Read could not be executed!\n");
		return 0;
	}

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Module_v1785:: V1785_BitSet2Set(int32_t handle, uint32_t base, uint16_t pat, CVAddressModifier AM){
	
	CVErrorCodes error_code;  //Error code if any
	int error_status;
	
	error_code = CAENVME_WriteCycle(handle, base+V1785_BIT_SET2_RW, &pat, AM, cvD16);
	error_status = CAEN::ErrorDecode(error_code);

	if (error_status == 1){  //success
		//do nothing

	}else{  //failure
		printf("V1785_BitSet2Set could not be executed!\n");

	}
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Module_v1785:: V1785_BitSet2Clear(int32_t handle, uint32_t base, uint16_t pat, CVAddressModifier AM){
	
	CVErrorCodes error_code;  //Error code if any
	int error_status;
	
	error_code = CAENVME_WriteCycle(handle, base+V1785_BIT_CLEAR2_WO, &pat, AM, cvD16);
	error_status = CAEN::ErrorDecode(error_code);

	if (error_status == 1){  //success
		//do nothing

	}else{  //failure
		printf("V1785_BitSet2Clear could not be executed!\n");

	}
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

uint16_t Module_v1785:: V1785_ControlRegister1Read(int32_t handle, uint32_t base, CVAddressModifier AM){
	
	CVErrorCodes error_code;  //Error code if any
	uint16_t pat;
	int error_status;
	
	error_code = CAENVME_ReadCycle(handle, base+V1785_CR1_RW, &pat, AM, cvD16);
	error_status = CAEN::ErrorDecode(error_code);

	if (error_status == 1){  //success
		return pat;

	}else{
		printf("V1785_ControlRegister1Read could not be executed!\n");
		return 0;
	}

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Module_v1785:: V1785_ControlRegister1Write(int32_t handle, uint32_t base, uint16_t pat, CVAddressModifier AM){
	
	CVErrorCodes error_code;  //Error code if any
	int error_status;
	
	error_code = CAENVME_WriteCycle(handle, base+V1785_CR1_RW, &pat, AM, cvD16);
	error_status = CAEN::ErrorDecode(error_code);

	if (error_status == 1){  //success
		//do nothing

	}else{  //failure
		printf("V1785_ControlRegister1Write could not be executed!\n");

	}
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Module_v1785:: V1785_OnlineSet(int32_t handle, uint32_t base, CVAddressModifier AM){
	
	CVErrorCodes error_code;  //Error code if any
	uint16_t write_online = 0x2;
	int error_status;
	
	error_code = CAENVME_WriteCycle(handle, base+V1785_BIT_CLEAR2_WO, &write_online, AM, cvD16);
	error_status = CAEN::ErrorDecode(error_code);

	if (error_status == 1){  //success
		//do nothing

	}else{  //failure
		printf("V1785_V1785_OnlineSet could not be executed!\n");

	}
	
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Module_v1785:: V1785_OfflineSet(int32_t handle, uint32_t base, CVAddressModifier AM){
	
	CVErrorCodes error_code;  //Error code if any
	uint16_t write_offline = 0x2;
	int error_status;
	
	error_code = CAENVME_WriteCycle(handle, base+V1785_BIT_SET2_RW, &write_offline, AM, cvD16);
	error_status = CAEN::ErrorDecode(error_code);

	if (error_status == 1){  //success
		//do nothing

	}else{  //failure
		printf("V1785_OfflineSet could not be executed!\n");

	}
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Module_v1785:: V1785_BlkEndEnable(int32_t handle, uint32_t base, CVAddressModifier AM){
	
	CVErrorCodes error_code_1;  //Error code if any
	CVErrorCodes error_code_2;  //Error code if any
	uint16_t pat;
	int error_status_1;
	int error_status_2;

	error_code_1 = CAENVME_ReadCycle(handle, base+V1785_CR1_RW, &pat, AM, cvD16);
	error_status_1 = CAEN::ErrorDecode(error_code_1);
	
	uint16_t write = (pat|0x04)&0x74;
	error_code_2 = CAENVME_WriteCycle(handle, base+V1785_CR1_RW, &write, AM, cvD16);
	error_status_2 = CAEN::ErrorDecode(error_code_2);

	if ( (error_status_1 == 1) && (error_status_2 == 1) ){  //success
		//do nothing

	}else{  //failure
		printf("V1785_BlkEndEnable could not be executed!\n");

	}

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Module_v1785:: V1785_OverRangeEnable(int32_t handle, uint32_t base, CVAddressModifier AM){
	CVErrorCodes error_code;  //Error code if any
	uint16_t write = 0x08;
	int error_status;

	error_code = CAENVME_WriteCycle(handle, base+V1785_BIT_CLEAR2_WO, &write, AM, cvD16);
	error_status = CAEN::ErrorDecode(error_code);

	if (error_status == 1){  //success
		//do nothing

	}else{  //failure
		printf("V1785_OverRangeEnable could not be executed!\n");

	}
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Module_v1785::V1785_OverRangeDisable(int32_t handle, uint32_t base, CVAddressModifier AM){
	CVErrorCodes error_code;  //Error code if any
	uint16_t write = 0x08;
	int error_status;

	error_code = CAENVME_WriteCycle(handle, base+V1785_BIT_SET2_RW, &write, AM, cvD16);
	error_status = CAEN::ErrorDecode(error_code);

	if (error_status == 1){  //success
		//do nothing

	}else{  //failure
		printf("V1785_OverRangeEnable could not be executed!\n");

	}
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......






//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int Module_v1785:: V1785_DataReady(int32_t handle, uint32_t base, CVAddressModifier AM){
	CVErrorCodes error_code;  //Error code if any
	int read; //read from CAENVME_Readcycle
	int data_ready;
	int error_status;
	
	error_code = CAENVME_ReadCycle(handle, base+V1785_CSR1_RO, &read, AM, cvD16);
	error_status = CAEN::ErrorDecode(error_code);
    data_ready = read & 0x1;

    if (error_status == 1){  //success
    	return data_ready;  //1 for data being ready, 0 for data not being ready
    }else{  //failure
    	printf("V1785_DataReady could not be executed!\n");
    	return 0;  //don't know if data is ready, so assume it not be ready
    }

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int Module_v1785:: V1785_BufferFull(int32_t handle, uint32_t base, CVAddressModifier AM){
	
	CVErrorCodes error_code_buffer_full, error_code_data_ready;
	int read_buffer_full, read_data_ready;
	int buffer_full, data_ready;
	int error_status_buffer_full, error_status_data_ready;
	
	error_code_buffer_full = CAENVME_ReadCycle(handle, base+V1785_CSR2_RO, &read_buffer_full, AM, cvD16);
	error_status_buffer_full = CAEN::ErrorDecode(error_code_buffer_full);

	error_code_data_ready  = CAENVME_ReadCycle(handle, base+V1785_CSR1_RO, &read_data_ready, AM, cvD16);
	error_status_data_ready = CAEN::ErrorDecode(error_code_data_ready);
	
	buffer_full = read_buffer_full & 0x4;
	data_ready  = read_data_ready & 0x1;
	
	if ( (error_status_buffer_full == 1) && (error_status_data_ready == 1) ){  //Both read cycles are successfully executed

		if(data_ready && buffer_full){
			return 1;
		}else{
			return 0;
		}

	}else{
		printf("V1785_BufferFull could not be executed!\n");
		return 0;
	}

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......




























