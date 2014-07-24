//v2718 - VME controller
//v1785 - VME peak sensing ADC
//v1290 - VME TDC
//v1720 - VME digitizer

#include <stdio.h>
#include <cstdlib>
#include <cstdint>  //c++11 standard

using namespace std;

#ifndef SOURCE_INCLUDE_MODULEMANAGER_H_
#define SOURCE_INCLUDE_MODULEMANAGER_H_


class ModuleManager{
  
  public:
    ModuleManager();
    ~ModuleManager();


    //return 1 on success and 0 on failure
    virtual int InitializeVMEModule();
    virtual int SetOnline();
    virtual int SetOffline();
    virtual int DataReady();
  	virtual int DeleteBuffer();
  	virtual int ResetModule();
    virtual int CloseConnection();
    virtual double GetModuleBuffer();

   	//GetAddress()

  protected:
    const int success = 1;
    const int failure = -1;
    int32_t handle;  //The handle that identifies the device.
    CVAddressModifier AM = cvA32_S_DATA // default address modifier
   	};

#endif //SOURCE_INCLUDE_MODULEMANAGER_H_
