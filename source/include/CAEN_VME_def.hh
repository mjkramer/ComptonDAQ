#include <cstdint>

using namespace std;

#ifndef CAEN_VME_DEF_H_
#define CAEN_VME_DEF_H_




typedef short VARIANT_BOOL;  //Defining VARIANT_BOOL for CAEN libraries
#define VARIANT_TRUE ((VARIANT_BOOL)-1)
#define VARIANT_FALSE ((VARIANT_BOOL)0)

struct global_constant{
	const int success;
	const int failure;
	int32_t handle;
	CVAddressModifier AM = cvA32_S_DATA; // default address modifier
}VME;

struct V2718{
	base = 0xA1000000;
}v2718;

struct V1785{
	base = 0xC1000000;
}v1785;





#endif  //CAEN_VME_DEF_H_