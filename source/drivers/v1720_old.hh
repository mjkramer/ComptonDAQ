#include "ModuleManager.hh"
#include "CAEN_VME_def.hh"

#ifndef SOURCE_DRIVERS_V1720_H_
#define SOURCE_DRIVERS_V1720_H_



class Module_v1720: public ModuleManager{
  public:
	int InitializeVMEModule(VME_INTERFACE *vme);
	


};


#endif //SOURCE_DRIVERS_V1720_H_