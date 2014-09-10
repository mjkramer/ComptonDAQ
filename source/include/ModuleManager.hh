//v2718 - VME controller
//v1785 - VME peak sensing ADC
//v1290 - VME TDC
//v1731 - VME digitizer

#include <cstdio>
#include <cstdlib>
#include <cstdint>

class DataBlock;

#ifndef SOURCE_INCLUDE_MODULEMANAGER_H_
#define SOURCE_INCLUDE_MODULEMANAGER_H_

class ModuleManager{
  
  public:
    ModuleManager();
    ~ModuleManager();


    //return 0 on success and 1 on failure
    virtual int InitializeVMEModule();
    virtual int SetOnline();
    virtual int SetOffline();
    virtual int DataReady();
    virtual int DeleteBuffer();
    virtual int ResetModule();
    virtual DataBlock* GetModuleBuffer();

    void SetHandle(int32_t h){handle = h;}
    int32_t GetHandle(){return handle;}


  protected:
    static int32_t handle;

};

#endif //SOURCE_INCLUDE_MODULEMANAGER_H_
