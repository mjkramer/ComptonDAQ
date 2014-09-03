
#ifndef SOURCE_DRIVERS_V2718_H_
#define SOURCE_DRIVERS_V2718_H_

#include  <stdlib.h>
#include  <stdint.h>
#include "CAENVMElib.h"
#include "ModuleManager.hh"

#define  V2718_STATUS_RO          (uint32_t) (0x0000)
#define  V2718_VME_CTRL_RW        (uint32_t) (0x0001)
#define  V2718_FW_REV_RO          (uint32_t) (0x0002)
#define  V2718_FW_DWNLD_RW        (uint32_t) (0x0003)
#define  V2718_FL_ENA_RW          (uint32_t) (0x0004)
#define  V2718_IRQ_STAT_RO        (uint32_t) (0x0005)
#define  V2718_IRQ_MASK_RO	  	  (uint32_t) (0x0006)
#define  V2718_IN_REG_RW          (uint32_t) (0x0008)
#define  V2718_OUT_REG_S_RW       (uint32_t) (0x000A)
#define  V2718_IN_MUX_S_RW        (uint32_t) (0x000B)
#define  V2718_OUT_MUX_S_RW       (uint32_t) (0x000C)
#define  V2718_LED_POL_S_RW       (uint32_t) (0x000D)
#define  V2718_OUT_REG_C_WO       (uint32_t) (0x0010)
#define  V2718_IN_MUX_C_WO        (uint32_t) (0x0011)
#define  V2718_OUT_MUX_C_WO       (uint32_t) (0x0012)
#define  V2718_LED_POL_C_WO       (uint32_t) (0x0013)
#define  V2718_PULSEA_0_RW        (uint32_t) (0x0016)
#define  V2718_PULSEA_1_RW        (uint32_t) (0x0017)
#define  V2718_PULSEB_0_RW        (uint32_t) (0x0019)
#define  V2718_PULSEB_1_RW        (uint32_t) (0x001A)
#define  V2718_SCALER0_RW         (uint32_t) (0x001C)
#define  V2718_SCALER1_RO         (uint32_t) (0x001D)
#define  V2718_DISP_ADL_RO        (uint32_t) (0x0020)
#define  V2718_DISP_ADH_RO        (uint32_t) (0x0021)
#define  V2718_DISP_DTL_RO        (uint32_t) (0x0022)
#define  V2718_DISP_DTH_RO        (uint32_t) (0x0023)
#define  V2718_DISP_PC1_RO        (uint32_t) (0x0024)
#define  V2718_DISP_PC2_RO        (uint32_t) (0x0025)
#define  V2718_LM_ADL_RW          (uint32_t) (0x0028)
#define  V2718_LM_ADH_RW          (uint32_t) (0x0029)
#define  V2718_LM_C_RW            (uint32_t) (0x002C)


class Module_v2718: public ModuleManager{

  public:
	int InitializeVMEModule(); 

	// Configuration of the pulsers.
	// @param pulser:   0=PulserA, 1=PulserB
	// @param period:   period in ns
	// @param width:    pulse width in ns
	// @param pulseNo:  number of pulses, 0=infinite
	void V2718_PulserConfSet(int32_t handle, int pulser, uint32_t period, uint32_t width, int pulseNo);
	void V2718_PulserStart(int32_t handle, int pulser);
	void V2718_PulserStop(int32_t handle, int pulser);
    


};


#endif //SOURCE_DRIVERS_V2718_H_
