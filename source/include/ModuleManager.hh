//v2718 - VME controller
//v1785 - VME peak sensing ADC
//v1290 - VME TDC
//v1720 - VME digitizer

#include <cstdio>
#include <cstdlib>
#include <cstdint>  //c++11 standard


using namespace std;

#ifndef SOURCE_INCLUDE_MODULEMANAGER_H_
#define SOURCE_INCLUDE_MODULEMANAGER_H_

typedef struct{
	int32_t handle;

} VME_INTERFACE;

class ModuleManager{
  
  public:
    ModuleManager();
    ~ModuleManager();


    //return 1 on success and 0 on failure
    virtual int InitializeVMEModule(VME_INTERFACE *vme);
    virtual int SetOnline();
    virtual int SetOffline();
    virtual int DataReady();
  	virtual int DeleteBuffer();
  	virtual int ResetModule();
    virtual int CloseConnection();
    virtual double GetModuleBuffer();

   	//GetAddress()

  protected:
   	};



#endif //SOURCE_INCLUDE_MODULEMANAGER_H_
