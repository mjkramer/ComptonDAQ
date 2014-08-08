#include "ModuleManager.hh"

class DataBlock;

#ifndef SOURCE_DRIVERS_V1785_H_
#define SOURCE_DRIVERS_V1785_H_

#define  MAX_CHANNELS       (uint32_t) 8
#define  REG_BASE           (uint32_t) (0x1000) 
#define  FIRM_REV           (uint32_t) (0x1000)  
#define  GEO_ADDR_RW        (uint32_t) (0x1002)  
#define  MCST_CBLT_RW       (uint32_t) (0x1004)  
#define  BIT_SET1_RW        (uint32_t) (0x1006)  
#define  BIT_CLEAR1_RW      (uint32_t) (0x1008) 
#define  SOFT_RESET         (uint16_t) (0x1<<7)
#define  INT_LEVEL_RW       (uint32_t) (0x100A)  
#define  INT_VECTOR_RW      (uint32_t) (0x100C) 
#define  CSR1_RO            (uint32_t) (0x100E)  
#define  CR1_RW             (uint32_t) (0x1010)  
#define  ADER_H_RW          (uint32_t) (0x1012)  
#define  ADER_L_RW          (uint32_t) (0x1014)  
#define  SINGLE_RST_WO      (uint32_t) (0x1016)  
#define  MCST_CBLT_CTRL_RW  (uint32_t) (0x101A)  
#define  EVTRIG_REG_RW      (uint32_t) (0x1020)  
#define  CSR2_RO            (uint32_t) (0x1022)  
#define  EVT_CNT_L_RO       (uint32_t) (0x1024)  
#define  EVT_CNT_H_RO       (uint32_t) (0x1026)  
#define  INCR_EVT_WO        (uint32_t) (0x1028)  
#define  INCR_OFFSET_WO     (uint32_t) (0x102A)  
#define  LD_TEST_RW         (uint32_t) (0x102C)  
#define  FCLR_WIN_RW        (uint32_t) (0x102E)  
#define  BIT_SET2_RW        (uint32_t) (0x1032)  
#define  BIT_CLEAR2_WO      (uint32_t) (0x1034)  
#define  W_MEM_TEST_WO      (uint32_t) (0x1036)  
#define  MEM_TEST_WORD_H_WO (uint32_t) (0x1038)  
#define  MEM_TEST_WORD_L_WO (uint32_t) (0x103A)  
#define  CRATE_SEL_RW       (uint32_t) (0x103C)  
#define  TEST_EVENT_WO      (uint32_t) (0x103E)  
#define  EVT_CNT_RST_WO     (uint32_t) (0x1040)  
#define  R_MEM_TEST_WO      (uint32_t) (0x1064)  
#define  SWCOMM_WO          (uint32_t) (0x1068)  
#define  SLIDECONST_RW      (uint32_t) (0x106A)  
#define  AAD_RO             (uint32_t) (0x1070)  
#define  BAD_RO             (uint32_t) (0x1072)  
#define  THRES_BASE         (uint32_t) (0x1080)  



class Module_v1785: public ModuleManager{

  public:
<<<<<<< HEAD
	int InitializeVMEModule();
	int SetOnline();
	int SetOffline();
	int DataReady();
	int DeleteBuffer(); //initially DataClear
	int ResetModule(); //initially Soft Reset
	DataBlock* GetModuleBuffer();
	

	//other methods
	int CSR1Read();
	int CSR2Read();
	int BitSet2Read();
	void BitSet2Set(uint16_t pat);  //pat is generated from ControlRegister1Read
	void BitSet2Clear(uint16_t pat);
	uint16_t ControlRegister1Read();
	void ControlRegister1Write(uint16_t pat);
	void BlkEndEnable();
	void OverRangeEnable();
	void OverRangeDisable();
	void LowThEnable();
	void LowThDisable();
	void EmptyEnable();
	int BufferFull();  //checks if the buffer of the module is full and data is ready
	int isEvtReady();
	int isBusy();
	int EventRead(uint32_t *pdest, int *nentry);  //Read single event, return event length (number of entries)
	int DataRead(uint32_t *pdest, int nentry);
	void EvtCntRead(uint32_t *evtcnt);
	void EvtCntReset();
	void IntSet(int level, int vector);
	void IntEnable(int level);
	void IntDisable();
	void EvtTriggerSet(int count);
	void SingleShotReset();
	void Trigger();  //cause a software trigger
	int ThresholdRead(uint16_t *threshold);  //Read threshold
	int ThresholdWrite(uint16_t *threshold);  //Write Thresholds and read them back
	void Status();
	int IsPresent();
=======
	int InitializeVMEModule();
	int SetOnline();
	int SetOffline();
	int DataReady();
	double GetModuleBuffer();
	

	int V1785_CSR1Read(int32_t handle, uint32_t base, CVAddressModifier AM);
	int V1785_CSR2Read(int32_t handle, uint32_t base, CVAddressModifier AM);
	int V1785_BitSet2Read(int32_t handle, uint32_t base, CVAddressModifier AM);
	void V1785_BitSet2Set(int32_t handle, uint32_t base, uint16_t pat, CVAddressModifier AM);  
	void V1785_BitSet2Clear(int32_t handle, uint32_t base, uint16_t pat, CVAddressModifier AM);
	uint16_t V1785_ControlRegister1Read(int32_t handle, uint32_t base, CVAddressModifier AM);
	void V1785_ControlRegister1Write(int32_t handle, uint32_t base, uint16_t pat, CVAddressModifier AM);
	
	void V1785_BlkEndEnable(int32_t handle, uint32_t base, CVAddressModifier AM);
	void V1785_OverRangeEnable(int32_t handle, uint32_t base, CVAddressModifier AM);
	void V1785_OverRangeDisable(int32_t handle, uint32_t base, CVAddressModifier AM);
	void V1785_LowThEnable(int32_t handle, uint32_t base, CVAddressModifier AM);
	void V1785_LowThDisable(int32_t handle, uint32_t base, CVAddressModifier AM);
	void V1785_EmptyEnable(int32_t handle, uint32_t base, CVAddressModifier AM);
	
	

	int V1785_BufferFull(int32_t handle, uint32_t base, CVAddressModifier AM);  //checks if the buffer of the module is full and data is ready
	int V1785_isEvtReady(int32_t handle, uint32_t base, CVAddressModifier AM);
	int V1785_isBusy(int32_t handle, uint32_t base, CVAddressModifier AM);
	//need to check these two functions
	int V1785_EventRead(int32_t handle, uint32_t base, uint32_t *pdest, int *nentry, CVAddressModifier AM);  //Read single event, return event length (number of entries)
	int V1785_DataRead(int32_t handle, uint32_t base, uint32_t *pdest, int nentry, CVAddressModifier AM);
	//
	void V1785_DataClear(int32_t handle, uint32_t base, CVAddressModifier AM);
	void V1785_EvtCntRead(int32_t handle, uint32_t base, uint32_t *evtcnt, CVAddressModifier AM);
	void V1785_EvtCntReset(int32_t handle, uint32_t base, CVAddressModifier AM);
	void V1785_IntSet(int32_t handle, uint32_t base, int level, int vector, CVAddressModifier AM);
	void V1785_IntEnable(int32_t handle, uint32_t base, int level, CVAddressModifier AM);
	void V1785_IntDisable(int32_t handle, uint32_t base, CVAddressModifier AM);
	void V1785_EvtTriggerSet(int32_t handle, uint32_t base, int count, CVAddressModifier AM);
	void V1785_SingleShotReset(int32_t handle, uint32_t base, CVAddressModifier AM);
	void V1785_SoftReset(int32_t handle, uint32_t base, CVAddressModifier AM);
	void V1785_Trigger(int32_t handle, uint32_t base, CVAddressModifier AM);  //cause a software trigger
	int V1785_ThresholdRead(int32_t handle, uint32_t base, uint16_t *threshold, CVAddressModifier AM);  //Read threshold
	int V1785_ThresholdWrite(int32_t handle, uint32_t base, uint16_t *threshold, CVAddressModifier AM);  //Write Thresholds and read them back

	void V1785_Status(int32_t handle, uint32_t base, CVAddressModifier AM);
	int V1785_isPresent(int32_t handle, uint32_t base, CVAddressModifier AM);
>>>>>>> b26e90f4aa3f3a9eff59f1f6a8eea647155fe5ef

};





















#endif //SOURCE_DRIVERS_H_
