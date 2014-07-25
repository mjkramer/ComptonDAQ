#include <cstdint>

using namespace std;

#ifndef CAEN_VME_DEF_H_
#define CAEN_VME_DEF_H_


typedef short VARIANT_BOOL;  //Defining VARIANT_BOOL for CAEN libraries
#define VARIANT_TRUE ((VARIANT_BOOL)-1)
#define VARIANT_FALSE ((VARIANT_BOOL)0)

struct CAEN_VME_card{
	const int success;
	const int failure;
	int32_t handle;
	CVAddressModifier AM = cvA32_S_DATA; // default address modifier
}v2718, v1785, v1720, v1290;


#endif  //CAEN_VME_DEF_H_