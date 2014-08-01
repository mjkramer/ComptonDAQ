#include "ModuleManager.hh"
#include "CAEN_VME_def.hh"

#ifndef SOURCE_DRIVERS_V1720_H_
#define SOURCE_DRIVERS_V1720_H_

#define V1720_EVENT_READOUT_BUFFER            0x0000

#define V1720_CHANNEL_CONFIG                  0x8000      /* R/W       ; D32 */ 
#define V1720_CHANNEL_CFG_BIT_SET             0x8004      /* write only  D32 */ 
#define V1720_CHANNEL_CFG_BIT_CLR             0x8008      /* write only; D32 */ 
#define V1720_BUFFER_ORGANIZATION             0x800C      /* R/W       ; D32 */ 
#define V1720_BUFFER_FREE                     0x8010      /* R/W       ; D32 */ 
#define V1720_CUSTOM_SIZE                     0x8020      /* R/W       ; D32 */ 
#define V1720_ACQUISITION_CONTROL             0x8100      /* R/W       ; D32 */ 
#define V1720_ACQUISITION_STATUS              0x8104      /* read  only; D32 */ 
#define V1720_SW_TRIGGER                      0x8108      /* write only; D32 */ 
#define V1720_TRIG_SRCE_EN_MASK               0x810C      /* R/W       ; D32 */ 
#define V1720_FP_TRIGGER_OUT_EN_MASK          0x8110      /* R/W       ; D32 */ 
#define V1720_POST_TRIGGER_SETTING            0x8114      /* R/W       ; D32 */ 
#define V1720_FP_IO_DATA                      0x8118      /* R/W       ; D32 */ 
#define V1720_FP_IO_CONTROL                   0x811C      /* R/W       ; D32 */  
#define V1720_CHANNEL_EN_MASK                 0x8120      /* R/W       ; D32 */ 
#define V1720_ROC_FPGA_FW_REV                 0x8124      /* read  only; D32 */ 
#define V1720_EVENT_STORED                    0x812C      /* read  only; D32 */ 
#define V1720_SET_MONITOR_DAC                 0x8138      /* R/W       ; D32 */ 
#define V1720_BOARD_INFO                      0x8140	  /* read  only; D32 */ 
#define V1720_MONITOR_MODE                    0x8144      /* R/W       ; D32 */ 
#define V1720_EVENT_SIZE                      0x814C	  /* read  only; D32 */
#define V1720_FP_LVDS_IO_CRTL                 0x81A0      /* R/W       ; D32 */
#define V1720_ALMOST_FULL_LEVEL               0x816C      /* R/W       ; D32 */
#define V1720_VME_CONTROL                     0xEF00      /* R/W       ; D32 */ 
#define V1720_VME_STATUS                      0xEF04      /* read  only; D32 */ 
#define V1720_BOARD_ID                        0xEF08      /* R/W       ; D32 */ 
#define V1720_MULTICAST_BASE_ADDCTL           0xEF0C      /* R/W       ; D32 */ 
#define V1720_RELOC_ADDRESS                   0xEF10      /* R/W       ; D32 */ 
#define V1720_INTERRUPT_STATUS_ID             0xEF14      /* R/W       ; D32 */ 
#define V1720_INTERRUPT_EVT_NB                0xEF18      /* R/W       ; D32 */ 
#define V1720_BLT_EVENT_NB                    0xEF1C      /* R/W       ; D32 */ 
#define V1720_SCRATCH                         0xEF20      /* R/W       ; D32 */ 
#define V1720_SW_RESET                        0xEF24      /* write only; D32 */ 
#define V1720_SW_CLEAR                        0xEF28      /* write only; D32 */ 
#define V1720_FLASH_ENABLE                    0xEF2C      /* R/W       ; D32 */ 
#define V1720_FLASH_DATA                      0xEF30      /* R/W       ; D32 */ 
#define V1720_CONFIG_RELOAD                   0xEF34      /* write only; D32 */ 
#define V1720_CONFIG_ROM                      0xF000      /* read  only; D32 */ 

#define V1720_ZS_THRESHOLD                    0x1024      /* For channel 0 */
#define V1720_ZS_NSAMP                        0x1028      /* For channel 0 */
#define V1720_CHANNEL_THRESHOLD               0x1080      /* For channel 0 */
#define V1720_CHANNEL_OUTHRESHOLD             0x1084      /* For channel 0 */
#define V1720_CHANNEL_STATUS                  0x1088      /* For channel 0 */
#define V1720_FPGA_FWREV                      0x108C      /* For channel 0 */
#define V1720_BUFFER_OCCUPANCY                0x1094      /* For channel 0 */
#define V1720_CHANNEL_DAC                     0x1098      /* For channel 0 */

#define V1720_RUN_START                             0x0001
#define V1720_RUN_STOP                              0x0002
#define V1720_REGISTER_RUN_MODE                     0x0003
#define V1720_SIN_RUN_MODE                          0x0004
#define V1720_SIN_GATE_RUN_MODE                     0x0005
#define V1720_MULTI_BOARD_SYNC_MODE                 0x0006
#define V1720_COUNT_ACCEPTED_TRIGGER                0x0007
#define V1720_COUNT_ALL_TRIGGER                     0x0008
#define V1720_TRIGGER_OVERTH                        0x0001
#define V1720_TRIGGER_UNDERTH                       0x0002
#define V1720_PACK25_ENABLE                         0x0003
#define V1720_PACK25_DISABLE                        0x0004
#define V1720_NO_ZERO_SUPPRESSION                   0x0005
#define V1720_ZLE                                   0x0006
#define V1720_ZS_AMP                                0x0007

#define V1720_EVENT_CONFIG_ALL_ADC        0x01000000
#define V1720_SOFT_TRIGGER                0x80000000
#define V1720_EXTERNAL_TRIGGER            0x40000000
#define V1720_ALIGN64                           0x20
#define V1720_DONE                                 0

class Module_v1720: public ModuleManager{
  public:
	int InitializeVMEModule(VME_INTERFACE *vme);
	
	uint32_t v1720_RegisterRead(int32_t handle, uint32_t base, int offset, CVAddressModifier AM);
	uint32_t v1720_BufferFreeRead(int32_t handle, uint32_t base, CVAddressModifier AM);
	uint32_t v1720_BufferOccupancy(int32_t handle, uint32_t base, uint32_t channel, CVAddressModifier AM);
	uint32_t v1720_BufferFree(int32_t handle, uint32_t base, int nbuffer, CVAddressModifier AM);

	void     v1720_AcqCtl(int32_t handle, uint32_t base, uint32_t operation, CVAddressModifier AM);
	void     v1720_ChannelCtl(int32_t handle, uint32_t base, uint32_t reg, uint32_t mask, CVAddressModifier AM);
	void     v1720_TrgCtl(int32_t handle, uint32_t base, uint32_t reg, uint32_t mask, CVAddressModifier AM);

	void     v1720_RegisterWrite(int32_t handle, uint32_t base, int offset, uint32_t value, CVAddressModifier AM);
	void     v1720_Reset(int32_t handle, uint32_t base, CVAddressModifier AM);

	void     v1720_Status(int32_t handle, uint32_t base, CVAddressModifier AM);
	int      v1720_Setup(int32_t handle, uint32_t base, int mode, CVAddressModifier AM);
	void     v1720_info(int32_t handle, uint32_t base, int *nchannels, uint32_t *n32word, CVAddressModifier AM);
	uint32_t v1720_DataRead(int32_t handle,uint32_t base, uint32_t *pdata, uint32_t n32w, CVAddressModifier AM);  //Pick data via padata
	uint32_t v1720_DataBlockRead(int32_t handle, uint32_t base, uint32_t *pdest, uint32_t *nentry, CVAddressModifier AM);
	void     v1720_ChannelThresholdSet(int32_t handle, uint32_t base, uint32_t channel, uint32_t threshold, CVAddressModifier AM);
	void     v1720_ChannelOUThresholdSet(int32_t handle, uint32_t base, uint32_t channel, uint32_t threshold, CVAddressModifier AM);
	void     v1720_ChannelDACSet(int32_t handle, uint32_t base, uint32_t channel, uint32_t dac, CVAddressModifier AM);
	int      v1720_ChannelDACGet(int32_t handle, uint32_t base, uint32_t channel, uint32_t *dac, CVAddressModifier AM);
	void     v1720_ChannelSet(int32_t handle, uint32_t base, uint32_t channel, uint32_t what, uint32_t that, CVAddressModifier AM);
	uint32_t v1720_ChannelGet(int32_t handle, uint32_t base, uint32_t channel, uint32_t what, CVAddressModifier AM);
	void     v1720_ChannelConfig(int32_t handle, uint32_t base, uint32_t operation, CVAddressModifier AM);
	void     v1720_Align64Set(int32_t handle, uint32_t base, CVAddressModifier AM);
	

	


};


#endif //SOURCE_DRIVERS_V1720_H_