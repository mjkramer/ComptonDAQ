#include  <stdlib.h>
#include  <stdint.h>
#include "CAENVMElib.h"
#include "ModuleManager.hh"

#ifndef SOURCE_DRIVERS_V1785_H_
#define SOURCE_DRIVERS_V1785_H_

#define  V1785_MAX_CHANNELS       (uint32_t) 8
#define  V1785_REG_BASE           (uint32_t) (0x1000) 
#define  V1785_FIRM_REV           (uint32_t) (0x1000)  
#define  V1785_GEO_ADDR_RW        (uint32_t) (0x1002)  
#define  V1785_MCST_CBLT_RW       (uint32_t) (0x1004)  
#define  V1785_BIT_SET1_RW        (uint32_t) (0x1006)  
#define  V1785_BIT_CLEAR1_RW      (uint32_t) (0x1008) 
#define  V1785_SOFT_RESET         (uint32_t) (0x1<<7) //0x1007 ? 
#define  V1785_INT_LEVEL_RW       (uint32_t) (0x100A)  
#define  V1785_INT_VECTOR_RW      (uint32_t) (0x100C) 
#define  V1785_CSR1_RO            (uint32_t) (0x100E)  
#define  V1785_CR1_RW             (uint32_t) (0x1010)  
#define  V1785_ADER_H_RW          (uint32_t) (0x1012)  
#define  V1785_ADER_L_RW          (uint32_t) (0x1014)  
#define  V1785_SINGLE_RST_WO      (uint32_t) (0x1016)  
#define  V1785_MCST_CBLT_CTRL_RW  (uint32_t) (0x101A)  
#define  V1785_EVTRIG_REG_RW      (uint32_t) (0x1020)  
#define  V1785_CSR2_RO            (uint32_t) (0x1022)  
#define  V1785_EVT_CNT_L_RO       (uint32_t) (0x1024)  
#define  V1785_EVT_CNT_H_RO       (uint32_t) (0x1026)  
#define  V1785_INCR_EVT_WO        (uint32_t) (0x1028)  
#define  V1785_INCR_OFFSET_WO     (uint32_t) (0x102A)  
#define  V1785_LD_TEST_RW         (uint32_t) (0x102C)  
#define  V1785_FCLR_WIN_RW        (uint32_t) (0x102E)  
#define  V1785_BIT_SET2_RW        (uint32_t) (0x1032)  
#define  V1785_BIT_CLEAR2_WO      (uint32_t) (0x1034)  
#define  V1785_W_MEM_TEST_WO      (uint32_t) (0x1036)  
#define  V1785_MEM_TEST_WORD_H_WO (uint32_t) (0x1038)  
#define  V1785_MEM_TEST_WORD_L_WO (uint32_t) (0x103A)  
#define  V1785_CRATE_SEL_RW       (uint32_t) (0x103C)  
#define  V1785_TEST_EVENT_WO      (uint32_t) (0x103E)  
#define  V1785_EVT_CNT_RST_WO     (uint32_t) (0x1040)  
#define  V1785_R_MEM_TEST_WO      (uint32_t) (0x1064)  
#define  V1785_SWCOMM_WO          (uint32_t) (0x1068)  
#define  V1785_SLIDECONST_RW      (uint32_t) (0x106A)  
#define  V1785_AAD_RO             (uint32_t) (0x1070)  
#define  V1785_BAD_RO             (uint32_t) (0x1072)  
#define  V1785_THRES_BASE         (uint32_t) (0x1080)  


class Module_v1785: public ModuleManager{
  public:
	int InitializeVMEModule(); //not implemented yet
	
	int V1785_CSR1Read(int32_t handle, uint32_t base);
	int V1785_CSR2Read(int32_t handle, uint32_t base);
	int V1785_BitSet2Read(int32_t handle, uint32_t base);
	void V1785_BitSet2Set(int32_t handle, uint32_t base, uint16_t pat);  //pat is generated from V1785_ControlRegister1Read
	void V1785_BitSet2Clear(int32_t handle, uint32_t base, uint16_t pat);
	uint16_t V1785_ControlRegister1Read(int32_t handle, uint32_t base);
	void V1785_ControlRegister1Write(int32_t handle, uint32_t base, uint16_t pat);
	void V1785_OnlineSet(int32_t handle, uint32_t base);
	void V1785_OfflineSet(int32_t handle, uint32_t base);
	void V1785_BlkEndEnable(int32_t handle, uint32_t base);
	
	
	
	int V1785_DataReady(int32_t handle, uint32_t base);
	int V1785_BufferFull(int32_t handle, uint32_t base);  //checks if the buffer of the module is full and data is ready
	
};





















#endif //SOURCE_DRIVERS_V1785_H_
