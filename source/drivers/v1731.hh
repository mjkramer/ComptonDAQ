#include "ModuleManager.hh"
#include "CAEN_VME_def.hh"


#ifndef SOURCE_DRIVERS_V1731_H_
#define SOURCE_DRIVERS_V1731_H_

#define V1731_EVENT_READOUT_BUFFER            0x0000

#define V1731_CHANNEL_CONFIG                  0x8000      /* R/W       ; D32 */ 
#define V1731_CHANNEL_CFG_BIT_SET             0x8004      /* write only  D32 */ 
#define V1731_CHANNEL_CFG_BIT_CLR             0x8008      /* write only; D32 */ 
#define V1731_BUFFER_ORGANIZATION             0x800C      /* R/W       ; D32 */ 
#define V1731_BUFFER_FREE                     0x8010      /* R/W       ; D32 */ 
#define V1731_CUSTOM_SIZE                     0x8020      /* R/W       ; D32 */ 
#define V1731_ACQUISITION_CONTROL             0x8100      /* R/W       ; D32 */ 
#define V1731_ACQUISITION_STATUS              0x8104      /* read  only; D32 */ 
#define V1731_SW_TRIGGER                      0x8108      /* write only; D32 */ 
#define V1731_TRIG_SRCE_EN_MASK               0x810C      /* R/W       ; D32 */ 
#define V1731_FP_TRIGGER_OUT_EN_MASK          0x8110      /* R/W       ; D32 */ 
#define V1731_POST_TRIGGER_SETTING            0x8114      /* R/W       ; D32 */ 
#define V1731_FP_IO_DATA                      0x8118      /* R/W       ; D32 */ 
#define V1731_FP_IO_CONTROL                   0x811C      /* R/W       ; D32 */  
#define V1731_CHANNEL_EN_MASK                 0x8120      /* R/W       ; D32 */ 
#define V1731_ROC_FPGA_FW_REV                 0x8124      /* read  only; D32 */ 
#define V1731_EVENT_STORED                    0x812C      /* read  only; D32 */ 
#define V1731_SET_MONITOR_DAC                 0x8138      /* R/W       ; D32 */ 
#define V1731_BOARD_INFO                      0x8140	  /* read  only; D32 */ 
#define V1731_MONITOR_MODE                    0x8144      /* R/W       ; D32 */ 
#define V1731_EVENT_SIZE                      0x814C	  /* read  only; D32 */
#define V1731_FP_LVDS_IO_CRTL                 0x81A0      /* R/W       ; D32 */
#define V1731_ALMOST_FULL_LEVEL               0x816C      /* R/W       ; D32 */
#define V1731_VME_CONTROL                     0xEF00      /* R/W       ; D32 */ 
#define V1731_VME_STATUS                      0xEF04      /* read  only; D32 */ 
#define V1731_BOARD_ID                        0xEF08      /* R/W       ; D32 */ 
#define V1731_MULTICAST_BASE_ADDCTL           0xEF0C      /* R/W       ; D32 */ 
#define V1731_RELOC_ADDRESS                   0xEF10      /* R/W       ; D32 */ 
#define V1731_INTERRUPT_STATUS_ID             0xEF14      /* R/W       ; D32 */ 
#define V1731_INTERRUPT_EVT_NB                0xEF18      /* R/W       ; D32 */ 
#define V1731_BLT_EVENT_NB                    0xEF1C      /* R/W       ; D32 */ 
#define V1731_SCRATCH                         0xEF20      /* R/W       ; D32 */ 
#define V1731_SW_RESET                        0xEF24      /* write only; D32 */ 
#define V1731_SW_CLEAR                        0xEF28      /* write only; D32 */ 
#define V1731_FLASH_ENABLE                    0xEF2C      /* R/W       ; D32 */ 
#define V1731_FLASH_DATA                      0xEF30      /* R/W       ; D32 */ 
#define V1731_CONFIG_RELOAD                   0xEF34      /* write only; D32 */ 
#define V1731_CONFIG_ROM                      0xF000      /* read  only; D32 */ 

#define V1731_ZS_THRESHOLD                    0x1024      /* For channel 0 */
#define V1731_ZS_NSAMP                        0x1028      /* For channel 0 */
#define V1731_CHANNEL_THRESHOLD               0x1080      /* For channel 0 */
#define V1731_CHANNEL_OUTHRESHOLD             0x1084      /* For channel 0 */
#define V1731_CHANNEL_STATUS                  0x1088      /* For channel 0 */
#define V1731_FPGA_FWREV                      0x108C      /* For channel 0 */
#define V1731_BUFFER_OCCUPANCY                0x1094      /* For channel 0 */
#define V1731_CHANNEL_DAC                     0x1098      /* For channel 0 */

#define V1731_ZS_THRESHOLD_CH2                0x1224      /* For channel 2 */
#define V1731_ZS_NSAMP_CH2                    0x1228      /* For channel 2 */
#define V1731_CHANNEL_THRESHOLD_CH2           0x1280      /* For channel 2 */
#define V1731_CHANNEL_OUTHRESHOLD_CH2         0x1284      /* For channel 2 */
#define V1731_CHANNEL_STATUS_CH2              0x1288      /* For channel 2 */
#define V1731_FPGA_FWREV_CH2                  0x128C      /* For channel 2 */
#define V1731_BUFFER_OCCUPANCY_CH2            0x1294      /* For channel 2 */
#define V1731_CHANNEL_DAC_CH2                 0x1298      /* For channel 2 */

#define V1731_RUN_START                             0x0001
#define V1731_RUN_STOP                              0x0002
#define V1731_REGISTER_RUN_MODE                     0x0003
#define V1731_SIN_RUN_MODE                          0x0004
#define V1731_SIN_GATE_RUN_MODE                     0x0005
#define V1731_MULTI_BOARD_SYNC_MODE                 0x0006
#define V1731_COUNT_ACCEPTED_TRIGGER                0x0007
#define V1731_COUNT_ALL_TRIGGER                     0x0008
#define V1731_TRIGGER_OVERTH                        0x0001
#define V1731_TRIGGER_UNDERTH                       0x0002
#define V1731_PACK25_ENABLE                         0x0003
#define V1731_PACK25_DISABLE                        0x0004
#define V1731_NO_ZERO_SUPPRESSION                   0x0005
#define V1731_ZLE                                   0x0006
#define V1731_ZS_AMP                                0x0007

#define V1731_EVENT_CONFIG_ALL_ADC        0x01000000
#define V1731_SOFT_TRIGGER                0x80000000
#define V1731_EXTERNAL_TRIGGER            0x40000000
#define V1731_ALIGN64                           0x20
#define V1731_DONE                                 0

class Module_v1731: public ModuleManager{
  public:
	int InitializeVMEModule();
	double GetModuleBuffer();
	
	uint32_t v1731_RegisterRead(int32_t handle, uint32_t base, int offset, CVAddressModifier AM);
	
	// The returned integer states which buffers are free
	uint32_t v1731_BufferFreeRead(int32_t handle, uint32_t base, CVAddressModifier AM);
	
	// The returned integer states which buffers are occupied (in binary)
	uint32_t v1731_BufferOccupancy(int32_t handle, uint32_t base, uint32_t channel, CVAddressModifier AM);
	
	// Free the first "nbuffer" output buffer memory blocks // 
	uint32_t v1731_BufferFree(int32_t handle, uint32_t base, int nbuffer, CVAddressModifier AM);

	void     v1731_AcqCtl(int32_t handle, uint32_t base, uint32_t operation, CVAddressModifier AM);
	void     v1731_ChannelCtl(int32_t handle, uint32_t base, uint32_t reg, uint32_t mask, CVAddressModifier AM);
	void     v1731_TrgCtl(int32_t handle, uint32_t base, uint32_t reg, uint32_t mask, CVAddressModifier AM);

	void     v1731_RegisterWrite(int32_t handle, uint32_t base, int offset, uint32_t value, CVAddressModifier AM);
	void     v1731_Reset(int32_t handle, uint32_t base, CVAddressModifier AM);

	void     v1731_Status(int32_t handle, uint32_t base, CVAddressModifier AM);
	int      v1731_Setup(int32_t handle, uint32_t base, int mode, CVAddressModifier AM);
	void     v1731_info(int32_t handle, uint32_t base, int *nchannels, uint32_t *n32word, CVAddressModifier AM);
	uint32_t v1731_DataRead(int32_t handle,uint32_t base, uint32_t *pdata, uint32_t n32w, CVAddressModifier AM);  //Pick data via padata
	uint32_t v1731_DataBlockRead(int32_t handle, uint32_t base, uint32_t *pdest, uint32_t *nentry, CVAddressModifier AM);
	void     v1731_ChannelThresholdSet(int32_t handle, uint32_t base, uint32_t channel, uint32_t threshold, CVAddressModifier AM);
	void     v1731_ChannelOUThresholdSet(int32_t handle, uint32_t base, uint32_t channel, uint32_t threshold, CVAddressModifier AM);
	void     v1731_ChannelDACSet(int32_t handle, uint32_t base, uint32_t channel, uint32_t dac, CVAddressModifier AM);
	int      v1731_ChannelDACGet(int32_t handle, uint32_t base, uint32_t channel, uint32_t *dac, CVAddressModifier AM);
	void     v1731_ChannelSet(int32_t handle, uint32_t base, uint32_t channel, uint32_t what, uint32_t that, CVAddressModifier AM);
	uint32_t v1731_ChannelGet(int32_t handle, uint32_t base, uint32_t channel, uint32_t what, CVAddressModifier AM);
	void     v1731_ChannelConfig(int32_t handle, uint32_t base, uint32_t operation, CVAddressModifier AM);
	void     v1731_Align64Set(int32_t handle, uint32_t base, CVAddressModifier AM);
	
    // CH0[]: Array for ADC counts from CH0; CH2[]: Array for ADC counts from CH2.
	// Return 1 for success and 0 for failure.
	int      v1731_ReadBuffer_and_Output(int32_t handle, uint32_t base, int nsample, CVAddressModifier AM, int *CH0, int *CH2);


};


#endif //SOURCE_DRIVERS_V1731_H_
