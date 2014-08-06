#include "v1290N.hh"

int  Module_v1290N::InitializeVMEModule(VME_INTERFACE *vme){
	
	printf("\n\n\n");
	
	printf("***************************************************\n");
	printf("*    Initializing CAEN V1290N TDC                 *\n");
	printf("***************************************************\n\n\n");

	int32_t Handle;
	Handle = vme->handle;
	
	V1290N v1290N;  //struct V1290N stores the base addr and AM
	 //for ErrorDecode and AM_Decode
	
	CVErrorCodes error_code;
	printf("Base Address of this CAEN V1290N: 0x%x\n", v1290N.base);
	CAEN::print_AM_Decode(v1290N.am);  //print the address modifier mode
	
    if(v1290N_isPresent(Handle, v1290N.base, v1290N.am) != 0){
    	printf("\n");
    	printf("V1290N found!\n");

    }else{
    	printf("Error: V1290N not found!\n");
        return 0;
    }
    printf("-----------------------------------------\n\n");
    
    printf("Setting to Trigger Matching Mode...\n");
    v1290N_TriggerMatchingSet(Handle, v1290N.base, v1290N.am);
    
    printf("Setting to detect the leading edge of the hot signal...\n");
    int eLeading = 1;
    int eTrailing = 0;
    v1290N_SetEdgeDetection(Handle, v1290N.base, eLeading, eTrailing, v1290N.am);
    
    printf("Setting the the search window width to be 400 ns (0x10)...\n");
    v1290N_WidthSet(Handle, v1290N.base, 0x10, v1290N.am);
    
    printf("Setting the window offset to be -425 ns (0xFFEE)...\n");
    v1290N_OffsetSet(Handle, v1290N.base, 0xFFEE, v1290N.am);
    
    printf("Setting the reject margin to be 25 ns (0x1)...\n");
    v1290N_RejectSet(Handle, v1290N.base, 0x1, v1290N.am);
    
    v1290N_ExtraSet(Handle, v1290N.base, 0, v1290N.am);  //no extra search margin
    
    printf("Disable TDC Headers and Trailers\n");
    v1290N_Disable_Header_Trailer(Handle, v1290N.base, v1290N.am);
    
    printf("Settings of v1290N completed\n");
    printf("-----------------------------------------\n\n");
    
    v1290N_Status(Handle, v1290N.base, v1290N.am);
    
    return 1;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

double Module_v1290N::GetModuleBuffer(VME_INTERFACE *vme){
	
	int nentry = 0;
	uint32_t rawdata[100]; //arbitary dimension
	
	for(int i=0; i<100; i++){
		rawdata[i] = 0;
	}
	
	int32_t header[100];
	int32_t channel[100]; //channel number
	int32_t time[100];
	
	int t1, t2;
	double time_diff;
	int32_t Handle;
	
	Handle = vme->handle;
	
	V1290N v1290N;
	
	
	FILE *pfile;
	pfile = fopen("v1290N_data.dat", "w");
	
	v1290N_SoftClear(Handle, v1290N.base, v1290N.am);
	
	v1290N_EventRead(Handle, v1290N.base, rawdata, &nentry, v1290N.am);
	
	for (int i=0; i<(nentry - 1); i++){
		header[i] = (rawdata[i] >> 27);  //Bits [31,27] identify the type
	}
	
	int i = 0;
	while (header[i] != 0){  //locate the first TDC measurement
		i = i + 1;
	}
	
	while (header[i] == 0){  //record until reaching the trailer
		channel[i] = ((rawdata[i] << 6) >> 27);
		time[i]    = ((rawdata[i] << 11) >> 11);
		if (time[i] != 0){
		    fprintf(pfile, "%d  %d\n", channel[i], time[i]);
		}
	    i = i + 1;
	}
    
	fclose(pfile);
	return 1.0;

}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int  Module_v1290N::v1290N_EventRead(int32_t handle, uint32_t base, uint32_t *pdest, int *nentry, CVAddressModifier AM){
	
	uint32_t hdata;
	CVErrorCodes error_code_1, error_code_2;
	
	*nentry = 0;
	if (v1290N_DataReady(handle, base, AM)){
		do{
			error_code_1 = CAENVME_ReadCycle(handle, base, &hdata, AM, cvD32);
		}while ( !(hdata & 0x40000000) );
		pdest[*nentry] = hdata;
		*nentry += 1;
		
		do{
			error_code_2 = CAENVME_ReadCycle(handle, base, &hdata, AM, cvD32);
			*nentry += 1;
		}while ( !(pdest[*nentry-1] & 0xc0000000) );
	}
	
	return *nentry;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int  Module_v1290N::v1290N_DataRead(int32_t handle, uint32_t base, uint32_t *pdest, int nentry, CVAddressModifier AM){
	CVErrorCodes error_code;
	int error_status;
	int n = 0;
	
	
	error_code = CAENVME_BLTReadCycle(handle, base, pdest, sizeof(uint32_t)*nentry, AM, cvD32, &n);
	error_status = CAEN::ErrorDecode(error_code);
	
	if(error_status == 1){  //success
		return nentry;
		
	}else{
		printf("v1290N_DataRead could not be executed!\n");
		return -1;
	}
	

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int  Module_v1290N::v1290N_GeoWrite(int32_t handle, uint32_t base, int geo, CVAddressModifier AM){
	CVErrorCodes error_code_1, error_code_2;
	int error_status_1, error_status_2;
	int read;
	uint16_t write;
	
	
	write = geo & 0x1F;
	error_code_1 = CAENVME_WriteCycle(handle, base+V1290N_GEO_REG_RW, &write, AM, cvD16);
	error_status_1 = CAEN::ErrorDecode(error_code_1);
	
	error_code_2 = CAENVME_ReadCycle(handle, base+V1290N_GEO_REG_RW, &read, AM, cvD16);
	error_status_2 = CAEN::ErrorDecode(error_code_2);
	
	if ( (error_status_1 == 1)&&(error_status_2 == 1) ){  //success
		return (int) (read & 0x1F);
	}else{
		printf("v1290N_GeoWrite could not be executed!\n");
		return -1;
	}
	
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Module_v1290N::v1290N_SoftReset(int32_t handle, uint32_t base, CVAddressModifier AM){
	CVErrorCodes error_code;
	int error_status;
	uint16_t write = 0;
	
	
	error_code = CAENVME_WriteCycle(handle, base+V1290N_MODULE_RESET_WO, &write, AM, cvD16);
	error_status = CAEN::ErrorDecode(error_code);
	
	if(error_status == 1){  //success
		//do nothing
	}else{
		printf("v1290N_SoftReset could not be executed!\n");
	}
	
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Module_v1290N::v1290N_SoftClear(int32_t handle, uint32_t base, CVAddressModifier AM){
	
	CVErrorCodes error_code;
	int error_status;
	uint16_t write = 0;
	
	
	error_code = CAENVME_WriteCycle(handle, base+V1290N_SOFT_CLEAR_WO, &write, AM, cvD16);
	error_status = CAEN::ErrorDecode(error_code);
	
	if(error_status == 1){  //success
		//do nothing
	}else{
		printf("v1290N_SoftReset could not be executed!\n");
	}
	
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Module_v1290N::v1290N_SoftTrigger(int32_t handle, uint32_t base, CVAddressModifier AM){
	
	CVErrorCodes error_code;
	int error_status;
	uint16_t write = 0;
	
	
	error_code = CAENVME_WriteCycle(handle, base+V1290N_SOFT_TRIGGER_WO, &write, AM, cvD16);
	error_status = CAEN::ErrorDecode(error_code);
	
	if(error_status == 1){  //success
		//do nothing
	}else{
		printf("v1290N_SoftTrigger could not be executed!\n");
	}
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int  Module_v1290N::v1290N_DataReady(int32_t handle, uint32_t base, CVAddressModifier AM){
	
	int read;
	CVErrorCodes error_code;
	int error_status;
	
	
	error_code = CAENVME_ReadCycle(handle, base+V1290N_SR_RO, &read, AM, cvD16);
	error_status = CAEN::ErrorDecode(error_code);
	
	return (read & V1290N_DATA_READY);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int  Module_v1290N::v1290N_EvtCounter(int32_t handle, uint32_t base, CVAddressModifier AM){
	
	int read;
	CVErrorCodes error_code;
	int error_status;
	
	
	error_code = CAENVME_ReadCycle(handle, base+V1290N_EVT_CNT_RO, &read, AM, cvD16);
	error_status = CAEN::ErrorDecode(error_code);
	
	return read;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int  Module_v1290N::v1290N_EvtStored(int32_t handle, uint32_t base, CVAddressModifier AM){
	
	int read;
	CVErrorCodes error_code;
	int error_status;
	
	
	error_code = CAENVME_ReadCycle(handle, base+V1290N_EVT_STORED_RO, &read, AM, cvD16);
	error_status = CAEN::ErrorDecode(error_code);
	
	return read;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int  Module_v1290N::v1290N_MicroWrite(int32_t handle, uint32_t base, uint16_t data, CVAddressModifier AM){
	
	CVErrorCodes error_code;
	int i;
	uint16_t microHS;
	uint16_t write = data;
	
	for (i=0; i<1000000; i++){  //upper bound must be large enough
		error_code = CAENVME_ReadCycle(handle, base+V1290N_MICRO_HAND_RO, &microHS, AM, cvD16);
		
		if (microHS & V1290N_MICRO_WR_OK){
			error_code = CAENVME_WriteCycle(handle, base+V1290N_MICRO_RW, &write, AM, cvD16);
			return 1;
		}
		usleep(500);
	}
	
	printf("v1290N_MicroWrite: Micro not ready for writing!\n");
	return -1;
	
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int  Module_v1290N::v1290N_MicroRead(int32_t handle, const uint32_t base, CVAddressModifier AM){
	
	CVErrorCodes error_code;
	int i;
	int reg = -1;
	uint16_t microHS;
	
	for(i=1000; i>0; i--){
		error_code = CAENVME_ReadCycle(handle, base+V1290N_MICRO_HAND_RO, &microHS, AM, cvD16);
		
		if (microHS & V1290N_MICRO_RD_OK){
			error_code = CAENVME_ReadCycle(handle, base+V1290N_MICRO_RW, &reg, AM, cvD16);
			return reg;
		}
		usleep(500);
	}
	return -1;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int  Module_v1290N::v1290N_MicroFlush(int32_t handle, const uint32_t base, CVAddressModifier AM){
	int count = 0;
	CVErrorCodes error_code;
	int data;
	
	while (1){
		data = v1290N_MicroRead(handle, base, AM);
		printf("microData[%d]: 0x%04x\n", count, data);
		if (data < 0)
	break;
		count++;
	}
	return count;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Module_v1290N::v1290N_TdcIdList(int32_t handle, uint32_t base, CVAddressModifier AM){
	int i, code;
	uint16_t value;
	
	for(i=0; i<2; i++){
		code = V1290N_MICRO_TDCID | (i & 0x0F);
		value = v1290N_MicroWrite(handle, base, code, AM);
		value = v1290N_MicroRead(handle, base, AM);
		value = (v1290N_MicroRead(handle, base, AM) << 16) | value;
	}
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int  Module_v1290N::v1290N_ResolutionRead(int32_t handle, uint32_t base, CVAddressModifier AM){
	int i, value;
	uint16_t code;
	
	for (i=0; i<2; i++){
		code = V1290N_RESOLUTION_RO | (i & 0x0F);
		value = v1290N_MicroWrite(handle, base, code, AM);
	    value = v1290N_MicroRead(handle, base, AM);
	}
	return value; 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Module_v1290N::v1290N_LEResolutionSet(int32_t handle, uint32_t base, uint16_t le, CVAddressModifier AM){
	int value;
	
	  if ((le == LE_RESOLUTION_100) ||
	      (le == LE_RESOLUTION_200) ||
	      (le == LE_RESOLUTION_800)) {
		  
	      printf("le:%x\n", le);
	      value = v1290N_MicroWrite(handle, base, V1290N_LE_RESOLUTION_WO, AM);
	      value = v1290N_MicroWrite(handle, base, le, AM);
	    
	  }else{
	      printf("Wrong Leading Edge Resolution -> Disabled\n");
	      value = v1290N_MicroWrite(handle, base, V1290N_LE_RESOLUTION_WO, AM);
	      value = v1290N_MicroWrite(handle, base, 3, AM);
	  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Module_v1290N::v1290N_LEWResolutionSet(int32_t handle, uint32_t base, uint16_t le, uint16_t width, CVAddressModifier AM){
	printf("Not yet implemented\n");
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Module_v1290N::v1290N_TriggerMatchingSet(int32_t handle, uint32_t base, CVAddressModifier AM){
	int value;
	
	value = v1290N_MicroWrite(handle, base, V1290N_TRIGGER_MATCH_WO, AM);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Module_v1290N::v1290N_AcqModeRead(int32_t handle, uint32_t base, CVAddressModifier AM){
	int value;
	
	value = v1290N_MicroWrite(handle, base, V1290N_ACQ_MODE_RO, AM);
	value = v1290N_MicroRead(handle, base, AM);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Module_v1290N::v1290N_ContinuousSet(int32_t handle, uint32_t base, CVAddressModifier AM){
	int value;
	value = v1290N_MicroWrite(handle, base, V1290N_CONTINUOUS_WO, AM);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Module_v1290N::v1290N_WidthSet(int32_t handle, uint32_t base, uint16_t width, CVAddressModifier AM){
	int value;
	
	value = v1290N_MicroWrite(handle, base, V1290N_WINDOW_WIDTH_WO, AM);
	value = v1290N_MicroWrite(handle, base, width, AM);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Module_v1290N::v1290N_OffsetSet(int32_t handle, uint32_t base, uint16_t offset, CVAddressModifier AM){
	int value;
	
	value = v1290N_MicroWrite(handle, base, V1290N_WINDOW_OFFSET_WO, AM);
	value = v1290N_MicroWrite(handle, base, offset, AM);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Module_v1290N::v1290N_ExtraSet(int32_t handle, uint32_t base, uint16_t extra, CVAddressModifier AM){
	int value;
	
    value = v1290N_MicroWrite(handle, base, V1290N_WINDOW_EXTRA_WO, AM);
    value = v1290N_MicroWrite(handle, base, extra, AM);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Module_v1290N::v1290N_RejectSet(int32_t handle, uint32_t base, uint16_t reject, CVAddressModifier AM){
	int value;
	
    value = v1290N_MicroWrite(handle, base, V1290N_WINDOW_REJECT_WO, AM);
    value = v1290N_MicroWrite(handle, base, reject, AM);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Module_v1290N::v1290N_Disable_Header_Trailer(int32_t handle, uint32_t base, CVAddressModifier AM){
	int value;
	value = v1290N_MicroWrite(handle, base, V1290N_DIS_HEAD_TRAILER, AM);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int  Module_v1290N::v1290N_Setup(int32_t handle, uint32_t base, int mode, CVAddressModifier AM){
	int status = -1;
	uint16_t code;
	int value, i;
	
	switch (mode) {
	case 0x1:
	    printf("Trigger Matching Setup (mode:%d)\n", mode);
	    printf("Default setting + Width : 800ns, Offset : -400ns\n");
	    printf("Time subtract, Leading Edge only\n");
	    code = 0x0000;  // Trigger matching Flag
	    
	if ( (status = v1290N_MicroWrite(handle, base, code, AM) ) < 0 )
	    return status;
	
	    code = 0x1000;  // Width
	    value = v1290N_MicroWrite(handle, base, code, AM);
	    value = v1290N_MicroWrite(handle, base, 0x001C, AM);   // Width : 700ns
	    code = 0x1100;  // Offset
	    value = v1290N_MicroWrite(handle, base, code, AM);
	    value = v1290N_MicroWrite(handle, base, 0x0000, AM);  // offset: 0ns
	    code = 0x1200; // extra search margin
	    value = v1290N_MicroWrite(handle, base, 0x0000, AM); //
	    code = 0x1300; //reject margin
	    value = v1290N_MicroWrite(handle, base, 0x0001, AM); //1ns
	    code = 0x1500;  // Subtraction flag
	    value = v1290N_MicroWrite(handle, base, code, AM);
	    code = 0x2100;  // Leading Edge Detection
	    value = v1290N_MicroWrite(handle, base, code, AM);
	    break;
	  case 0x2:
	    code = 0x0500;  // Default configuration
	    value = v1290N_MicroWrite(handle, base, code, AM);
	    break;
	  default:
	    printf("Unknown setup mode\n");
	    return -1;
	  }
	  v1290N_Status(handle, base, AM);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int  Module_v1290N::v1290N_Status(int32_t handle, uint32_t base, CVAddressModifier AM){
	uint16_t code, pair = 0;
	int value;
	int ns = 25;  // ns per tdc count
	int i;
	
	//-------------------------------------------------
	printf("\n");
	printf("V1290N TDC at VME A32 0x%06x:\n", base);
	printf("\n--- Trigger Section [0x1600]:\n");
	code = 0x1600;
	if ((value = v1290N_MicroWrite(handle, base, code, AM)) < 0)
	    return -value;
	value = v1290N_MicroRead(handle, base, AM);
	printf("  Match Window width       : 0x%04x, %d ns\n", value, value*ns);
	value = v1290N_MicroRead(handle, base, AM);
	printf("  Window offset            : 0x%04x, %d ns\n", value, ((uint16_t)value)*ns);
	value = v1290N_MicroRead(handle, base, AM);
	printf("  Extra Search Window Width: 0x%04x, %d ns\n", value, value*ns);
	value = v1290N_MicroRead(handle, base, AM);
	printf("  Reject Margin            : 0x%04x, %d ns\n", value, value*ns);
	value = v1290N_MicroRead(handle, base, AM);
	printf("  Trigger Time subtraction : %s\n",(value & 0x1) ? "y" : "n");

	//-------------------------------------------------
	printf("\n--- Edge Detection & Resolution Section[0x2300/26/29]:\n");
	code = 0x2300;
	value = v1290N_MicroWrite(handle, base, code, AM);
	pair = value = v1290N_MicroRead(handle, base, AM);
	printf("  Edge Detection (1:T/2:L/3:TL)           : 0x%02x\n", (value&0x3));
	code = 0x2600;
	value = v1290N_MicroWrite(handle, base, code, AM);
	value = v1290N_MicroRead(handle, base, AM);
	if (pair==0x3) {
	    value = v1290N_MicroRead(handle, base, AM);
	    printf("  Leading Edge Resolution (see table)     : 0x%02x\n", (value&0x3));
	    printf("  Pulse Width Resolution (see table)      : 0x%02x\n", ((value>>8)&0xF));
	}else{
	    printf("  Resolution [ps] (0:800/1:200/2:100)     : 0x%02x\n", (value&0x3));
	  }
	code = 0x2900;
	value = v1290N_MicroWrite(handle, base, code, AM);
	value = v1290N_MicroRead(handle, base, AM);
	printf("  Dead Time between hit [~ns](5/10/30/100): 0x%02x\n", (value&0x3));
	
	//-------------------------------------------------
	printf("\n--- Readout Section[0x3200/34/3a/3c]:\n");
	code = 0x3200;
	value = v1290N_MicroWrite(handle, base, code, AM);
	value = v1290N_MicroRead(handle, base, AM);
	printf("  Header/Trailer                            : %s\n",(value & 0x1) ? "y" : "n");
	code = 0x3400;
	value = v1290N_MicroWrite(handle, base, code, AM);
	value = v1290N_MicroRead(handle, base, AM);
	printf("  Max #hits per event 2^n-1 (>128:no limit) : %d\n", value&0xF);
	code = 0x3a00;
	value = v1290N_MicroWrite(handle, base, code, AM);
	value = v1290N_MicroRead(handle, base, AM);
	printf("  Internal TDC error type (see doc)         : 0x%04x\n", (value&0x7FF));
	code = 0x3c00;
	value = v1290N_MicroWrite(handle, base, code, AM);
    value = v1290N_MicroRead(handle, base, AM);
	printf("  Effective size of readout Fifo 2^n-1      : 0x%04x\n", (value&0xF));

	//-------------------------------------------------
	printf("\n--- Channel Enable Section[0x4500/47/49]:\n");
	code = 0x4500;
	value = v1290N_MicroWrite(handle, base, code, AM);
	value = v1290N_MicroRead(handle, base, AM);
	printf("  Read Enable Pattern [  0..15 ] : 0x%04x\n", value);
	value = v1290N_MicroRead(handle, base, AM);
	printf("  Read Enable Pattern [ 16..31 ] : 0x%04x\n", value);
	value = v1290N_MicroRead(handle, base, AM);
	printf("  Read Enable Pattern [ 32..47 ] : 0x%04x\n", value);
	value = v1290N_MicroRead(handle, base, AM);
	printf("  Read Enable Pattern [ 48..63 ] : 0x%04x\n", value);
	code = 0x4700;
	value = v1290N_MicroWrite(handle, base, code, AM);
	value = v1290N_MicroRead(handle, base, AM);
	value = (v1290N_MicroRead(handle, base, AM)<<16) | value;
	printf("  Read Enable Pattern 32 (0) : 0x%08x\n", value);
	code = 0x4701;
	value = v1290N_MicroWrite(handle, base, code, AM);
	value = v1290N_MicroRead(handle, base, AM);
	value = (v1290N_MicroRead(handle, base, AM)<<16) | value;
	printf("  Read Enable Pattern 32 (1) : 0x%08x\n", value);

	//-------------------------------------------------
	printf("\n--- Adjust Section[0x5100/60]:\n");
	code = 0x5100;
	value = v1290N_MicroWrite(handle, base, code, AM);
	value = v1290N_MicroRead(handle, base, AM);
	printf("  Coarse Counter Offset: 0x%04x\n", (value&0x7FF));
	value = v1290N_MicroRead(handle, base, AM);
	printf("  Fine   Counter Offset: 0x%04x\n", (value&0x1F));
	printf("\nMiscellaneous Section:\n");
	for (i=0; i<2 ; i++) {
	    code = 0x6000 | (i & 0x0F);
	    value = v1290N_MicroWrite(handle, base, code, AM);
	    value = v1290N_MicroRead(handle, base, AM);
	    value = (v1290N_MicroRead(handle, base, AM) << 16) | value;
	    printf("  TDC ID(%i)  0x%08x  [code:0x%04x]\n", i, value, code);
	}
	
	return 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Module_v1290N::v1290N_SetEdgeDetection(int32_t handle, uint32_t base, int eLeading, int eTrailing, CVAddressModifier AM){
	int value;
	if (eLeading) //if eLeading == 0, it will be ignored
	    value |= 2; 

	if (eTrailing)
	    value |= 1;

	  v1290N_MicroWrite(handle, base, V1290N_EDGE_DETECTION_WO, AM);
	  v1290N_MicroWrite(handle, base, value, AM);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int  Module_v1290N::v1290N_isPresent(int32_t handle, uint32_t base, CVAddressModifier AM){
	int status;
	CVErrorCodes error_code;
	int error_status;
	
	
	error_code = CAENVME_ReadCycle(handle, base+V1290N_FIRM_REV_RO, &status, AM, cvD16);
	error_status = CAEN::ErrorDecode(error_code);
	
	if (status == 0xFFFF)
	    return 0;
	else
	    return 1;
}












