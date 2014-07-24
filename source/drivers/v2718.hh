#include "ModuleManager.hh"

#ifndef SOURCE_DRIVERS_V2718_H_
#define SOURCE_DRIVERS_V2718_H_



#define  V2718_STATUS_RO          (unsigned int) (0x0000)
#define  V2718_VME_CTRL_RW        (unsigned int) (0x0001)
#define  V2718_FW_REV_RO          (unsigned int) (0x0002)
#define  V2718_FW_DWNLD_RW        (unsigned int) (0x0003)
#define  V2718_FL_ENA_RW          (unsigned int) (0x0004)
#define  V2718_IRQ_STAT_RO        (unsigned int) (0x0005)
#define  V2718_IRQ_MASK_RO	  	  (unsigned int) (0x0006)
#define  V2718_IN_REG_RW          (unsigned int) (0x0008)
#define  V2718_OUT_REG_S_RW       (unsigned int) (0x000A)
#define  V2718_IN_MUX_S_RW        (unsigned int) (0x000B)
#define  V2718_OUT_MUX_S_RW       (unsigned int) (0x000C)
#define  V2718_LED_POL_S_RW       (unsigned int) (0x000D)
#define  V2718_OUT_REG_C_WO       (unsigned int) (0x0010)
#define  V2718_IN_MUX_C_WO        (unsigned int) (0x0011)
#define  V2718_OUT_MUX_C_WO       (unsigned int) (0x0012)
#define  V2718_LED_POL_C_WO       (unsigned int) (0x0013)
#define  V2718_PULSEA_0_RW        (unsigned int) (0x0016)
#define  V2718_PULSEA_1_RW        (unsigned int) (0x0017)
#define  V2718_PULSEB_0_RW        (unsigned int) (0x0019)
#define  V2718_PULSEB_1_RW        (unsigned int) (0x001A)
#define  V2718_SCALER0_RW         (unsigned int) (0x001C)
#define  V2718_SCALER1_RO         (unsigned int) (0x001D)
#define  V2718_DISP_ADL_RO        (unsigned int) (0x0020)
#define  V2718_DISP_ADH_RO        (unsigned int) (0x0021)
#define  V2718_DISP_DTL_RO        (unsigned int) (0x0022)
#define  V2718_DISP_DTH_RO        (unsigned int) (0x0023)
#define  V2718_DISP_PC1_RO        (unsigned int) (0x0024)
#define  V2718_DISP_PC2_RO        (unsigned int) (0x0025)
#define  V2718_LM_ADL_RW          (unsigned int) (0x0028)
#define  V2718_LM_ADH_RW          (unsigned int) (0x0029)
#define  V2718_LM_C_RW            (unsigned int) (0x002C)
#define	 LINK					  (unsigned int)	 3

class Module_v2718: public ModuleManager{
  public:
	int InitializeVMEModule();
	


};


#endif //SOURCE_DRIVERS_V2718_H_