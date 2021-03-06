#ifndef SOURCE_DRIVERS_V1731_H_
#define SOURCE_DRIVERS_V1731_H_

#include "ModuleManager.hh"
#include "CAENVMEtypes.h"

class DataBlock;

#define EVENT_READOUT_BUFFER            0x0000
#define CHANNEL_CONFIG                  0x8000      /* R/W       ; D32 */ 
#define CHANNEL_CFG_BIT_SET             0x8004      /* write only  D32 */ 
#define CHANNEL_CFG_BIT_CLR             0x8008      /* write only; D32 */ 
#define BUFFER_ORGANIZATION             0x800C      /* R/W       ; D32 */ 
#define BUFFER_FREE                     0x8010      /* R/W       ; D32 */ 
#define CUSTOM_SIZE                     0x8020      /* R/W       ; D32 */ 
#define ACQUISITION_CONTROL             0x8100      /* R/W       ; D32 */ 
#define ACQUISITION_STATUS              0x8104      /* read  only; D32 */ 
#define SW_TRIGGER                      0x8108      /* write only; D32 */ 
#define TRIG_SRCE_EN_MASK               0x810C      /* R/W       ; D32 */ 
#define FP_TRIGGER_OUT_EN_MASK          0x8110      /* R/W       ; D32 */ 
#define POST_TRIGGER_SETTING            0x8114      /* R/W       ; D32 */ 
#define FP_IO_DATA                      0x8118      /* R/W       ; D32 */ 
#define FP_IO_CONTROL                   0x811C      /* R/W       ; D32 */  
#define CHANNEL_EN_MASK                 0x8120      /* R/W       ; D32 */ 
#define ROC_FPGA_FW_REV                 0x8124      /* read  only; D32 */ 
#define EVENT_STORED                    0x812C      /* read  only; D32 */ 
#define SET_MONITOR_DAC                 0x8138      /* R/W       ; D32 */ 
#define BOARD_INFO                      0x8140	  /* read  only; D32 */ 
#define MONITOR_MODE                    0x8144      /* R/W       ; D32 */ 
#define EVENT_SIZE                      0x814C	  /* read  only; D32 */
#define FP_LVDS_IO_CRTL                 0x81A0      /* R/W       ; D32 */
#define ALMOST_FULL_LEVEL               0x816C      /* R/W       ; D32 */
#define VME_CONTROL                     0xEF00      /* R/W       ; D32 */ 
#define VME_STATUS                      0xEF04      /* read  only; D32 */ 
#define BOARD_ID                        0xEF08      /* R/W       ; D32 */ 
#define MULTICAST_BASE_ADDCTL           0xEF0C      /* R/W       ; D32 */ 
#define RELOC_ADDRESS                   0xEF10      /* R/W       ; D32 */ 
#define INTERRUPT_STATUS_ID             0xEF14      /* R/W       ; D32 */ 
#define INTERRUPT_EVT_NB                0xEF18      /* R/W       ; D32 */ 
#define BLT_EVENT_NB                    0xEF1C      /* R/W       ; D32 */ 
#define SCRATCH                         0xEF20      /* R/W       ; D32 */ 
#define SW_RESET                        0xEF24      /* write only; D32 */ 
#define SW_CLEAR                        0xEF28      /* write only; D32 */ 
#define FLASH_ENABLE                    0xEF2C      /* R/W       ; D32 */ 
#define FLASH_DATA                      0xEF30      /* R/W       ; D32 */ 
#define CONFIG_RELOAD                   0xEF34      /* write only; D32 */ 
#define CONFIG_ROM                      0xF000      /* read  only; D32 */ 

#define ZS_THRESHOLD                    0x1024      /* For channel 0 */
#define ZS_NSAMP                        0x1028      /* For channel 0 */
#define CHANNEL_THRESHOLD               0x1080      /* For channel 0 */
#define CHANNEL_OUTHRESHOLD             0x1084      /* For channel 0 */
#define CHANNEL_STATUS                  0x1088      /* For channel 0 */
#define FPGA_FWREV                      0x108C      /* For channel 0 */
#define BUFFER_OCCUPANCY                0x1094      /* For channel 0 */
#define CHANNEL_DAC                     0x1098      /* For channel 0 */

#define ZS_THRESHOLD_CH2                0x1224      /* For channel 2 */
#define ZS_NSAMP_CH2                    0x1228      /* For channel 2 */
#define CHANNEL_THRESHOLD_CH2           0x1280      /* For channel 2 */
#define CHANNEL_OUTHRESHOLD_CH2         0x1284      /* For channel 2 */
#define CHANNEL_STATUS_CH2              0x1288      /* For channel 2 */
#define FPGA_FWREV_CH2                  0x128C      /* For channel 2 */
#define BUFFER_OCCUPANCY_CH2            0x1294      /* For channel 2 */
#define CHANNEL_DAC_CH2                 0x1298      /* For channel 2 */

#define SOFTWARE_TRIGGER		      0x8108 

#define CALIBRATION_CH1		      0x119C //for channel 1
#define CALIBRATION_CH2		      0x129C //for channel 2
#define CALIBRATION_CH3		      0x139C //for channel 3
#define CALIBRATION_CH4		      0x149C //for channel 4

#define CALIBRATION_STATUS_CH1	      0x1188 //for channel 1
#define CALIBRATION_STATUS_CH2	      0x1288 //for channel 2
#define CALIBRATION_STATUS_CH3	      0x1388 //for channel 3
#define CALIBRATION_STATUS_CH4	      0x1488 //for channel 4

#define RUN_START                             0x0001
#define RUN_STOP                              0x0002
#define REGISTER_RUN_MODE                     0x0003
#define SIN_RUN_MODE                          0x0004
#define SIN_GATE_RUN_MODE                     0x0005
#define MULTI_BOARD_SYNC_MODE                 0x0006
#define COUNT_ACCEPTED_TRIGGER                0x0007
#define COUNT_ALL_TRIGGER                     0x0008
#define TRIGGER_OVERTH                        0x0001
#define TRIGGER_UNDERTH                       0x0002
#define PACK25_ENABLE                         0x0003
#define PACK25_DISABLE                        0x0004
#define NO_ZERO_SUPPRESSION                   0x0005
#define ZLE                                   0x0006
#define ZS_AMP                                0x0007

#define EVENT_CONFIG_ALL_ADC        0x01000000
#define SOFT_TRIGGER                0x80000000
#define EXTERNAL_TRIGGER            0x40000000
#define ALIGN64                           0x20
#define DONE                                 0


class Module_v1731: public ModuleManager{
public:
  Module_v1731();
  virtual ~Module_v1731();
  int InitializeVMEModule();
  DataBlock* GetModuleBuffer();
  int SetOnline();
  int SetOffline();
  int DataReady();
  void WriteRegister(int offset, uint32_t value, CVDataWidth width);
  uint32_t ReadRegister(int offset, CVDataWidth width);
  void CalibrateChannels();
  void GenerateSoftwareTrigger();

  
private:
  int UpdateEventBuffer();
  uint BufferedEvents();
  uint GetEventSize(uint32_t* event);
  uint32_t* GetNextEvent();

private:
  //int m_nsample;
  int32_t m_v1731_handle;
  uint m_bufferSize;
  uint32_t* m_eventBuffer;
  uint32_t* m_currentEvent;
  uint32_t* m_bufferEnd;
};

#endif
