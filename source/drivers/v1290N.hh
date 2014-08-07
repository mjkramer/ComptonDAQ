#include  <cstdlib>
#include  <cstdint>
#include  <unistd.h>

#include "CAENVMElib.h"
#include "CAEN_VME_def.hh"
#include "ModuleManager.hh"


#ifndef SOURCE_DRIVERS_V1290_H_
#define SOURCE_DRIVERS_V1290_H_

#define  V1290N_MAX_CHANNELS      (uint32_t) 16      
#define  V1290N_REG_BASE          (uint32_t) (0x1000)
#define  V1290N_CR_RW             (uint32_t) (0x1000)
#define  V1290N_SR_RO             (uint32_t) (0x1002)
#define  V1290N_DATA_READY        (uint32_t) (0x0001)
#define  V1290N_ALMOST_FULL       (uint32_t) (0x0002)
#define  V1290N_FULL              (uint32_t) (0x0004)
#define  V1290N_TRIGGER_MATCH     (uint32_t) (0x0008)
#define  V1290N_HEADER_ENABLE     (uint32_t) (0x0010)
#define  V1290N_GEO_REG_RW        (uint32_t) (0x001E)
#define  V1290N_MODULE_RESET_WO   (uint32_t) (0x1014)
#define  V1290N_SOFT_CLEAR_WO     (uint32_t) (0x1016)
#define  V1290N_SOFT_EVT_RESET_WO (uint32_t) (0x1018)
#define  V1290N_SOFT_TRIGGER_WO   (uint32_t) (0x101A)
#define  V1290N_EVT_CNT_RO        (uint32_t) (0x101C)
#define  V1290N_EVT_STORED_RO     (uint32_t) (0x1020)
#define  V1290N_FIRM_REV_RO       (uint32_t) (0x1026)
#define  V1290N_MICRO_HAND_RO     (uint32_t) (0x1030)
#define  V1290N_MICRO_RW          (uint32_t) (0x102E)
/* Micro code IDs */
#define  V1290N_WINDOW_WIDTH_WO   (uint16_t)  (0x1000)
#define  V1290N_WINDOW_OFFSET_WO  (uint16_t)  (0x1100)
#define  V1290N_WINDOW_EXTRA_WO   (uint16_t)  (0x1200)
#define  V1290N_WINDOW_REJECT_WO  (uint16_t)  (0x1300)//Set reject margin
#define  V1290N_MICRO_WR_OK       (uint16_t)  (0x0001)
#define  V1290N_MICRO_RD_OK       (uint16_t)  (0x0002)
#define  V1290N_MICRO_TDCID       (uint16_t)  (0x6000)
#define  V1290N_EDGE_DETECTION_WO (uint16_t)  (0x2200)
#define  V1290N_LE_RESOLUTION_WO  (uint16_t)  (0x2400)
#define  V1290N_LEW_RESOLUTION_WO (uint16_t)  (0x2500)
#define  V1290N_RESOLUTION_RO     (uint16_t)  (0x2600)
#define  V1290N_TRIGGER_MATCH_WO  (uint16_t)  (0x0000)
#define  V1290N_CONTINUOUS_WO     (uint16_t)  (0x0100)
#define  V1290N_ACQ_MODE_RO       (uint16_t)  (0x0200)
#define  V1290N_DIS_HEAD_TRAILER  (uint16_t)  (0x3100)

#define  LE_RESOLUTION_100        (uint16_t)  (0x10)
#define  LE_RESOLUTION_200        (uint16_t)  (0x01) 
#define  LE_RESOLUTION_800        (uint16_t)  (0x00)  

class Module_v1290N: public ModuleManager{

  public:
	int InitializeVMEModule();  // Return 1 if succeed and 0 if failed
	double GetModuleBuffer();
	
	int  v1290N_EventRead(int32_t handle, uint32_t base, uint32_t *pdest, int *nentry, CVAddressModifier AM);  //need to check
	int  v1290N_DataRead(int32_t handle, uint32_t base, uint32_t *pdest, int nentry, CVAddressModifier AM);    //need to check, return -1 when failed
	int  v1290N_GeoWrite(int32_t handle, uint32_t base, int geo, CVAddressModifier AM);  //return -1 when failed
	void v1290N_SoftReset(int32_t handle, uint32_t base, CVAddressModifier AM);
	void v1290N_SoftClear(int32_t handle, uint32_t base, CVAddressModifier AM);
	void v1290N_SoftTrigger(int32_t handle, uint32_t base, CVAddressModifier AM);
	int  v1290N_DataReady(int32_t handle, uint32_t base, CVAddressModifier AM);  // 1 for data ready, 0 for not ready 
	int  v1290N_EvtCounter(int32_t handle, uint32_t base, CVAddressModifier AM);  //return event count
	int  v1290N_EvtStored(int32_t handle, uint32_t base, CVAddressModifier AM);  //return event stored
	int  v1290N_MicroWrite(int32_t handle, uint32_t base, uint16_t data, CVAddressModifier AM);  //return 1 for success, -1 for failure
	int  v1290N_MicroRead(int32_t handle, const uint32_t base, CVAddressModifier AM);  //return -1 for failure
	int  v1290N_MicroFlush(int32_t handle, const uint32_t base, CVAddressModifier AM);  //return count
	void v1290N_TdcIdList(int32_t handle, uint32_t base, CVAddressModifier AM);
	int  v1290N_ResolutionRead(int32_t handle, uint32_t base, CVAddressModifier AM);
	void v1290N_LEResolutionSet(int32_t handle, uint32_t base, uint16_t le, CVAddressModifier AM);
	void v1290N_LEWResolutionSet(int32_t handle, uint32_t base, uint16_t le, uint16_t width, CVAddressModifier AM); //Not yet implemented
	
	void v1290N_TriggerMatchingSet(int32_t handle, uint32_t base, CVAddressModifier AM);
	void v1290N_AcqModeRead(int32_t handle, uint32_t base, CVAddressModifier AM);
	void v1290N_ContinuousSet(int32_t handle, uint32_t base, CVAddressModifier AM);
	
	
	/*********************************************************************************
	 *                             v1290N_WidthSet                                   *
	 * Set the width of the matching Window. The width parameter should be           *
	 * in the range of 1 to 4095 (0xFFF). Example 0x14 == 500ns.                     *
	 * @param *mvme VME structure                                                    *
	 * @param  base Module base address                                              *
	 * @param width window width in ns units                                         *
	 * @return                                                                       *
	 *********************************************************************************/
	void v1290N_WidthSet(int32_t handle, uint32_t base, uint16_t width, CVAddressModifier AM);
	
	
	/*********************************************************************************
	 *                             v1290N_OffsetSet                                  *
	 * Set the offset of the matching window with respect to the trigger.            *
	 * The offset parameter should be in 25ns units. The range is                    *
	 * from -2048(0xF800) to +40(0x28). Example 0xFFD8 == -1 micro sec.              *
	 * @param *mvme VME structure                                                    *
	 * @param  base Module base address                                              *
	 * @param  offset offset in ns units                                             *
	 *********************************************************************************/
	void v1290N_OffsetSet(int32_t handle, uint32_t base, uint16_t offset, CVAddressModifier AM);
	
	void v1290N_ExtraSet(int32_t handle, uint32_t base, uint16_t extra, CVAddressModifier AM);  //The parameter of extra search margin must be in 25ns units 
	                                                                                            // and should be smaller than 50.
	
	void v1290N_RejectSet(int32_t handle, uint32_t base, uint16_t reject, CVAddressModifier AM);//The parameter of reject margin must be in 25 ns units 
	                                                                                            // and should be larger than 1 (25 ns).
	
	void v1290N_Disable_Header_Trailer(int32_t handle, uint32_t base, CVAddressModifier AM);
	int  v1290N_Setup(int32_t handle, uint32_t base, int mode, CVAddressModifier AM);
	int  v1290N_Status(int32_t handle, uint32_t base, CVAddressModifier AM);
	void v1290N_SetEdgeDetection(int32_t handle, uint32_t base, int eLeading, int eTrailing, CVAddressModifier AM);
	int  v1290N_isPresent(int32_t handle, uint32_t base, CVAddressModifier AM);
    


};




#endif //SOURCE_DRIVERS_V1290_H_
