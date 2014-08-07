#include "v1785.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int Module_v1785:: InitializeVMEModule(VME_INTERFACE *vme){
	
    printf("\n\n\n");
	printf("***************************************************\n");
	printf("*    Initializing CAEN V1785 Peak ADC...          *\n");
	printf("***************************************************\n\n\n");

    int32_t Handle;
    Handle = vme->handle;  //Obtain the handle, which was generated from v2718 earlier,
                           //from the struct


    CVErrorCodes error_code;
    printf("Base Address of this CAEN V1785: 0x%x\n", V1785::base);
    CAEN::print_AM_Decode(V1785::am);  //print the address modifier mode

    if(V1785_isPresent(Handle, V1785::base, V1785::am) != 0){
    	printf("\n");
    	printf("V1785 found!\n");

    }else{
    	printf("Error: V1785 not found!\n");
        return 0;
    }

    //clears all the data, event counter, bit set, bit clear..
    printf("Sending reset signal to V1785..\n");
    V1785_SoftReset(Handle, V1785::base, V1785::am);

    //disable 7 out of 8 channels (only ch0-high is used) - each channel has a high&low
    printf("Starting do disable channels 0-7..\n\n");
    uint16_t V1785_threshold[16] = {0x000, //ch0-high
    			    0x100, //ch1-low
    			 0x100,0x100,0x100,
    			 0x100,0x100,0x100,0x100,
    			 0x100,0x100,0x100,0x100,
    			 0x100,0x100,0x100};

    if(V1785_ThresholdWrite(Handle, V1785::base, V1785_threshold, V1785::am) != 8){
    	printf("Error: Disabling channels not successful!\n");
    	return 0;
    }
    printf("Disabling channels successfully.\n");

    printf("---------------CAEN V1785 status-------------------\n\n\n");
    V1785_Status(Handle, V1785::base, V1785::am);

    return 1;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

double Module_v1785::GetModuleBuffer(VME_INTERFACE *vme){
	
	int32_t Handle = vme->handle;
	
	
    int i;
	int nentry = 0;
	int value =0;
	int channel =0;
	uint32_t data[V1785_MAX_CHANNELS+2];
	V1785_EventRead(Handle, V1785::base, data, &nentry, V1785::am);

	for (i = 0; i < nentry; i++) {
	    uint32_t w = data[i];
	    if (((w >> 24) & 0x7) != 0) continue;
	        channel = (w >> 17) & 0xF;
	        value = (w & 0x3FFF);}
	
	return value;
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

void Module_v1785::V1785_LowThEnable(int32_t handle, uint32_t base, CVAddressModifier AM){
	
	CVErrorCodes error_code;
	uint16_t write = 0x10;
	int error_status;

	error_code = CAENVME_WriteCycle(handle, base+V1785_BIT_CLEAR2_WO, &write, AM, cvD16);
	error_status = CAEN::ErrorDecode(error_code);

	if (error_status == 1){  //success
		//do nothing

	}else{  //failure
		printf("V1785_LowThEnable could not be executed!\n");

	}
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Module_v1785::V1785_LowThDisable(int32_t handle, uint32_t base, CVAddressModifier AM){
	
	CVErrorCodes error_code;
	uint16_t write = 0x10;
	int error_status;

	error_code = CAENVME_WriteCycle(handle, base+V1785_BIT_SET2_RW, &write, AM, cvD16);
	error_status = CAEN::ErrorDecode(error_code);

	if (error_status == 1){  //success
		//do nothing

	}else{  //failure
		printf("V1785_LowThDisable could not be executed!\n");

	}
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Module_v1785::V1785_EmptyEnable(int32_t handle, uint32_t base, CVAddressModifier AM){
	
	CVErrorCodes error_code;
	uint16_t write = 0x1000;
	int error_status;

	error_code = CAENVME_WriteCycle(handle, base+V1785_BIT_SET2_RW, &write, AM, cvD16);
	error_status = CAEN::ErrorDecode(error_code);

	if (error_status == 1){  //success
		//do nothing

	}else{  //failure
		printf("V1785_EmptyEnable could not be executed!\n");

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
			return data_ready;
		}else{
			return 0;
		}

	}else{
		printf("V1785_BufferFull could not be executed!\n");
		return 0;
	}

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int Module_v1785::V1785_isEvtReady(int32_t handle, uint32_t base, CVAddressModifier AM){
	int csr;
	csr = V1785_CSR1Read(handle, base, AM);  //Function defined within the same class
	return (csr & 0x100);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int Module_v1785::V1785_isBusy(int32_t handle, uint32_t base, CVAddressModifier AM){
	
	int busy, timeout, read, error_status;
	CVErrorCodes error_code;
	timeout = 1000;

	 do {
	     error_code = CAENVME_ReadCycle(handle, base+V1785_CSR1_RO, &read, AM, cvD16);
	     busy = read & 0x4;
	     timeout--;
	   } while (busy || timeout);

	 error_status = CAEN::ErrorDecode(error_code);
	 if (error_status == 1){  //success
	 return (busy != 0 ? 1 : 0);

	 }else{
		 printf("V1785_isBusy could not be executed!\n");
		 return -1;
	 }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int Module_v1785::V1785_EventRead(int32_t handle, uint32_t base, uint32_t *pdest, int *nentry, CVAddressModifier AM){
#define USE_BLT_READ_2

#ifdef USE_SINGLE_READ
  
  uint32_t hdata, data;
  CVErrorCodes error_code_1, error_code_2;
  //  int error_status, error_status_2;

  //D32 mode

  *nentry = 0;
  if (V1785_DataReady(handle, base)) {
    do {
      error_code_1 = CAENVME_ReadCycle(handle, base, &hdata, AM, cvD32);
      //  error_status_1 = CAEN::ErrorDecode(error_code_1);
    } while (!(hdata & 0x02000000)); // skip up to the header

    pdest[*nentry] = hdata;
    *nentry += 1;
    do {
      error_code_2 = CAENVME_ReadCycle(handle, base, &data, AM, cvD32);
      //  error_status_2 = CAEN::ErrorDecode(error_code_2);
      pdest[*nentry] = data;
      *nentry += 1;
    } while (!(pdest[*nentry-1] & 0x04000000)); // copy until the trailer

    nentry--;
  }

#endif // USE_SINGLE_READ

#ifdef USE_BLT_READ_1
  
  int i;
  uint32_t hdata, data[V1785_MAX_CHANNELS+2], data;
  int count;
  CVErrorCodes error_code_1, error_code_2;
  //  int error_status_1, error_status_2;

  *nentry = 0;
  if (V1785_DataReady(handle, base, AM)) {
    do {
      error_code_1 = CAENVME_ReadCycle(handle, base, &hdata, AM, cvD32);
      //  error_status_1 = CAEN::ErrorDecode(error_code_1);
    } while (!(hdata & 0x02000000)); // skip up to the header

    AM = cvA32_S_BLT;
    count = (hdata >> 8) & 0x3F;

    //mvme_read(mvme, data, base, (cnt+1)*4);
    error_code_2 = CAENVME_BLTReadCycle(handle, base, data, (count+1)*4, AM, cvD32, &count);
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
//  if (V1785_DataReady(handle, base)) {
    AM = cvA32_S_BLT;

    //  mvme_read(mvme, pdest, base, (V1785_MAX_CHANNELS+2)*4); //handle, data_pointer, base_address, number of entries
    error_code = CAENVME_BLTReadCycle(handle, base, pdest, (V1785_MAX_CHANNELS+2)*4, AM, cvD32, &count);
    error_status = CAEN::ErrorDecode(error_code);
    count = (pdest[0] >> 8) & 0x3F;
    *nentry = count + 2;
//  }

#endif //USE_BLT_READ_2

  return *nentry;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int Module_v1785::V1785_DataRead(int32_t handle, uint32_t base, uint32_t *pdest, int nentry, CVAddressModifier AM){
	CVErrorCodes error_code;
	//int error_status;
	int i, n;
	n = 0;

	if ( V1785_DataReady(handle, base, AM) ){
		for (i=0; i < ((nentry*4)>>2); i++){
		    error_code = CAENVME_ReadCycle(handle, base, pdest+(i<<2), AM, cvD32);
		    n = (nentry*4);
		}
	}
	return n;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Module_v1785::V1785_DataClear(int32_t handle, uint32_t base, CVAddressModifier AM){
	CVErrorCodes error_code_1, error_code_2;
	int error_status_1, error_status_2;
	uint16_t write = 0x4;
	

	error_code_1 = CAENVME_WriteCycle(handle, base+V1785_BIT_SET2_RW, &write, AM, cvD16);
	error_status_1 = CAEN::ErrorDecode(error_code_1);

	error_code_2 = CAENVME_WriteCycle(handle, base+V1785_BIT_CLEAR2_WO, &write, AM, cvD16);
	error_status_2 = CAEN::ErrorDecode(error_code_2);

	if ( (error_status_1 == 1) && (error_status_2 == 1) ){  //success
		//do nothing
	}else{
		printf("V1785_DataClear could not be executed!\n");
	}
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Module_v1785::V1785_EvtCntRead(int32_t handle, uint32_t base, uint32_t *evtcnt, CVAddressModifier AM){
	
	CVErrorCodes error_code_1, error_code_2;  //Error code if any
	int read_1, read_2;
	int error_status_1, error_status_2;

	error_code_1 = CAENVME_ReadCycle(handle, base+V1785_EVT_CNT_L_RO, &read_1, AM, cvD16);
	*evtcnt = read_1;
	error_status_1 = CAEN::ErrorDecode(error_code_1);

	error_code_2 = CAENVME_ReadCycle(handle, base+V1785_EVT_CNT_H_RO, &read_2, AM, cvD16);
	*evtcnt += (read_2 << 16);
	error_status_2 = CAEN::ErrorDecode(error_code_2);

	if ( (error_status_1 == 1) == (error_status_2 == 1) ){  //success
        //do nothing

	}else{  //failure
		printf("V1785_EvtCntRead could not be executed!\n");
	}
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Module_v1785::V1785_EvtCntReset(int32_t handle, uint32_t base, CVAddressModifier AM){
	CVErrorCodes error_code;
    int error_status;
    uint16_t write = 1;
    

    error_code = CAENVME_WriteCycle(handle, base+V1785_EVT_CNT_RST_WO, &write, AM, cvD16);
    error_status = CAEN::ErrorDecode(error_code);

    if (error_status == 1){
    	//do nothing
    }else{
    	printf("V1785_EvtCntRReset could not be executed!\n");
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Module_v1785::V1785_IntSet(int32_t handle, uint32_t base, int level, int vector, CVAddressModifier AM){
	CVErrorCodes error_code;
    int error_status;
    uint16_t write = vector & 0xFF;
    

    error_code = CAENVME_WriteCycle(handle, base+V1785_INT_VECTOR_RW, &write, AM, cvD16);
    error_status = CAEN::ErrorDecode(error_code);

    if (error_status == 1){
    	//do nothing
    }else{
    	printf("V1785_EIntSet could not be executed!\n");
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Module_v1785::V1785_IntEnable(int32_t handle, uint32_t base, int level, CVAddressModifier AM){
	CVErrorCodes error_code;
    int error_status;
    uint16_t write = level & 0x1F;
    

    error_code = CAENVME_WriteCycle(handle, base+V1785_EVTRIG_REG_RW, &write, AM, cvD16);
    error_status = CAEN::ErrorDecode(error_code);

    if (error_status == 1){
    	//do nothing
    }else{
    	printf("V1785_IntEnable could not be executed!\n");
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Module_v1785::V1785_IntDisable(int32_t handle, uint32_t base, CVAddressModifier AM){
	CVErrorCodes error_code;
    int error_status;
    uint16_t write = 0;
    

    error_code = CAENVME_WriteCycle(handle, base+V1785_EVTRIG_REG_RW, &write, AM, cvD16);
    error_status = CAEN::ErrorDecode(error_code);

    if (error_status == 1){
    	//do nothing
    }else{
    	printf("V1785_IntDisable could not be executed!\n");
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Module_v1785::V1785_EvtTriggerSet(int32_t handle, uint32_t base, int count, CVAddressModifier AM){
	CVErrorCodes error_code;
    int error_status;
    uint16_t write = count & 0x1F;
    

    error_code = CAENVME_WriteCycle(handle, base+V1785_EVTRIG_REG_RW, &write, AM, cvD16);
    error_status = CAEN::ErrorDecode(error_code);

    if (error_status == 1){
    	//do nothing
    }else{
    	printf("V1785_EvtTriggerSet could not be executed!\n");
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Module_v1785::V1785_SingleShotReset(int32_t handle, uint32_t base, CVAddressModifier AM){
	CVErrorCodes error_code;
    int error_status;
    uint16_t write = 1;
    

    error_code = CAENVME_WriteCycle(handle, base+V1785_SINGLE_RST_WO, &write, AM, cvD16);
    error_status = CAEN::ErrorDecode(error_code);

    if (error_status == 1){
    	//do nothing
    }else{
    	printf("V1785_SingleShotReset could not be executed!\n");
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Module_v1785::V1785_SoftReset(int32_t handle, uint32_t base, CVAddressModifier AM){
	CVErrorCodes error_code_1, error_code_2;
    int error_status_1, error_status_2;
    uint16_t write = V1785_SOFT_RESET;
    

    error_code_1 = CAENVME_WriteCycle(handle, base+V1785_BIT_SET1_RW, &write, AM, cvD16);
    error_status_1 = CAEN::ErrorDecode(error_code_1);

    error_code_2 = CAENVME_WriteCycle(handle, base+V1785_BIT_CLEAR1_RW, &write, AM, cvD16);
    error_status_2 = CAEN::ErrorDecode(error_code_2);

    if ( (error_status_1 == 1) && (error_status_2 == 1) ){
    	//do nothing
    }else{
    	printf("V1785_SoftReset could not be executed!\n");
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Module_v1785::V1785_Trigger(int32_t handle, uint32_t base, CVAddressModifier AM){
	CVErrorCodes error_code;
    int error_status;
    uint16_t write = 0;
    

    error_code = CAENVME_WriteCycle(handle, base+V1785_SWCOMM_WO, &write, AM, cvD16);
    error_status = CAEN::ErrorDecode(error_code);

    if (error_status == 1){
    	//do nothing
    }else{
    	printf("V1785_Trigger could not be executed!\n");
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int Module_v1785::V1785_ThresholdRead(int32_t handle, uint32_t base, uint16_t *threshold, CVAddressModifier AM){
    int k;
    uint16_t read;
    CVErrorCodes error_code;
    //int error_status;
    //

	for (k=0; k < V1785_MAX_CHANNELS*2; k++){
        error_code = CAENVME_ReadCycle(handle, base+V1785_THRES_BASE + 2*k, &read, AM, cvD16);
        threshold[k] = read & 0x1FF;
        //error_status = CAEN::ErrorDecode(error_code);
	}

	return V1785_MAX_CHANNELS;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int Module_v1785::V1785_ThresholdWrite(int32_t handle, uint32_t base, uint16_t *threshold, CVAddressModifier AM){
	int k;
	uint16_t write;
	uint16_t read;
	CVErrorCodes error_code_1, error_code_2;
    //int error_status_1, error_status_2;
    //

	for (k=0; k < V1785_MAX_CHANNELS*2; k++){
		write = threshold[k] & 0x1FF;
		error_code_1 = CAENVME_WriteCycle(handle, base+V1785_THRES_BASE + 4*k, &write, AM, cvD16);
		//error_status_1 = CAEN::ErrorDecode(error_code_1);
	}

	for (k=0; k < V1785_MAX_CHANNELS*2; k++){
        error_code_2 = CAENVME_ReadCycle(handle, base+V1785_THRES_BASE + 2*k, &read, AM, cvD16);
        threshold[k] = read & 0x1FF;
	}
	return V1785_MAX_CHANNELS;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Module_v1785::V1785_Status(int32_t handle, uint32_t base, CVAddressModifier AM){
	int i;
	CVErrorCodes err;
	uint16_t threshold[V1785_MAX_CHANNELS];
	uint32_t evtcnt;
	uint16_t read;

	printf("V1785 at VME A32 0x%08x:\n", base);
	err = CAENVME_ReadCycle(handle, base+V1785_FIRM_REV, &read, AM, cvD16);
	printf("Firmware revision: 0x%x\n",read);

	if (read == 0xFFFF) {
	    printf("Module not present!\n");
	    return;
	  }

	read = V1785_CSR1Read(handle, base, AM);
	printf("CSR1: 0x%x\n", read);
	printf("DataReady    :%s\t", read & 0x1 ? "Y" : "N");
	printf(" - Global Dready:%s\t", read & 0x2 ? "Y" : "N");
	printf(" - Busy         :%s\n", read & 0x4 ? "Y" : "N");
	printf("Global Busy  :%s\t", read & 0x8 ? "Y" : "N");
	printf(" - Amnesia      :%s\t", read & 0x10 ? "Y" : "N");
	printf(" - Purge        :%s\n", read & 0x20 ? "Y" : "N");
	printf("Term ON      :%s\t", read & 0x40 ? "Y" : "N");
	printf(" - TermOFF      :%s\t", read & 0x80 ? "Y" : "N");
	printf(" - Event Ready  :%s\n", read & 0x100 ? "Y" : "N");

	read = V1785_CSR2Read(handle, base, AM);
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
	  read = V1785_BitSet2Read(handle, base, AM);
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

	  V1785_EvtCntRead(handle, base, &evtcnt, AM);
	  printf("Event counter: %d\n", evtcnt);

	  V1785_ThresholdRead(handle, base, threshold, AM);
	  for (i=0;i<V1785_MAX_CHANNELS*2;i+=2) {
	    printf("Threshold[%2i] = 0x%4.4x\t   -  ", i, threshold[i]);
	    printf("Threshold[%2i] = 0x%4.4x\n", i+1, threshold[i+1]);
	  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int Module_v1785::V1785_isPresent(int32_t handle, uint32_t base, CVAddressModifier AM){
	int read = 0;
	CVErrorCodes err;

	err =  CAENVME_ReadCycle(handle, base+V1785_FIRM_REV, &read, AM, cvD16);
	  if (read == 0xFFFF)
	    return 0;
	  else
	    return 1;
}

























