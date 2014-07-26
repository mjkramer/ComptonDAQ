#include <cstdint>
#include "CAENVMElib.h"

using namespace std;

#ifndef CAEN_VME_DEF_H_
#define CAEN_VME_DEF_H_


class CAEN{
  public:

    int32_t handle;
    CVAddressModifier AM;
    
};

struct V2718{
	static const int32_t base = 0xA1000000;
};

struct V1785{
	static const int32_t base = 0xC1000000;
};





#endif  //CAEN_VME_DEF_H_