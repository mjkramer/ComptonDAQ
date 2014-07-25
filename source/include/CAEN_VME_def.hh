#include <cstdint>

using namespace std;

#ifndef CAEN_VME_DEF_H_
#define CAEN_VME_DEF_H_

struct global_constant{
	static const int success = 1;
	static const int failure = 0;
	static int32_t handle;
	static CVAddressModifier AM;
	
	global_constant(){
		handle = 0;
		AM = cvA32_S_DATA;  // default address modifier
	}
	
}VME;

struct V2718{
	static const int32_t base = 0xA1000000;
};

struct V1785{
	static const int32_t base = 0xC1000000;
};





#endif  //CAEN_VME_DEF_H_