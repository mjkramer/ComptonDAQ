#ifndef SOURCE_INCLUDE_DATAAQUISITION_H_
#define SOURCE_INCLUDE_DATAAQUISITION_H_

#include <vector>

class ConfigFileManager;
class HistoManager;
class UiManager;
class ModuleManager;



class DataAcquisition{
  
  public:
    DataAcquisition(ConfigFileManager*, HistoManager*, UiManager*);
    ~DataAcquisition();

    int Initialize();
    int StartRun();
    int StopRun();



  private:
  	ConfigFileManager *fConfigFileManager;
  	HistoManager *fHistoManager;
  	UiManager *fUiManager;

  	int run_number;
  	VME_INTERFACE caen;
  	std::vector<ModuleManager*>modules;
  	std::vector<ModuleManager*>::iterator i; 
  	


   	};



#endif //DATAAQUISITION_H_   	