//v2718 - VME controller
//v1785 - VME peak sensing ADC
//v1290 - VME TDC
//v1720 - VME digitizer

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


  private:

   	};

#endif //SOURCE_INCLUDE_MODULEMANAGER_H_
