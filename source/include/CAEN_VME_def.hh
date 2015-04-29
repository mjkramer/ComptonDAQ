#ifndef SOURCE_INCLUDE_CAEN_VME_DEF_H_
#define SOURCE_INCLUDE_CAEN_VME_DEF_H_

#include <cstdint>
#include "CAENVMElib.h"
#include "CAENDigitizer.h"


namespace CAEN{
	 extern int ErrorDecode(CVErrorCodes error_code);
	 extern void print_AM_Decode(CVAddressModifier AM);
	 extern int ErrorDecode_DGTZ(CAEN_DGTZ_ErrorCode error_code);
}//end namespace CAEN


namespace V2718{
	 static const uint32_t base                       = 0xA1000000;
	 static const short board_number		  		  = 0;
	 static const CVBoardTypes board_type		  	  = cvV2718;
	 static const short link			              = 3;
}

namespace V1785{  
	 static const uint32_t base                       = 0xC1000000;
	 static const CVAddressModifier am                = cvA32_S_DATA;
}

namespace V1290N{
	 static const uint32_t base                       = 0xD1000000;
	 static const CVAddressModifier am                = cvA32_S_DATA;
}

namespace V1731{
	 static const uint32_t base                      = 0x11110000;
	 static const int LinkNum                        = 3;
	 static const int ConetNode                      = 0;
	 static const CVAddressModifier am               = cvA32_S_DATA;
	 static const CAEN_DGTZ_ConnectionType LinkType  = CAEN_DGTZ_OpticalLink;
}

namespace V1730{
	 static const uint32_t base                      = 0x11110000;
	 static const int LinkNum                        = 3;
	 static const int ConetNode                      = 0;
	 static const CVAddressModifier am               = cvA32_S_DATA;
	 static const CAEN_DGTZ_ConnectionType LinkType  = CAEN_DGTZ_OpticalLink;
}

#endif  //SOURCE_INCLUDE_CAEN_VME_DEF_H_
