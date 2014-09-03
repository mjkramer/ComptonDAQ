#ifndef SOURCE_INCLUDE_DATAAQUISITION_H_
#define SOURCE_INCLUDE_DATAAQUISITION_H_

#include <vector>
#include <ModuleManager.hh>

class ConfigFileManager;
class HistoManager;
class UiManager;


class DataAcquisition{
  
  public:
    DataAcquisition(ConfigFileManager*, HistoManager*, UiManager*);
    ~DataAcquisition();

    int Initialize();
    int StartRun();
    int StopRun();

    bool GetRunState(){return state;}
    void SetRunState(bool s){state = s;}
    void CheckKeyboardCommands();

  private:
  	ConfigFileManager *fConfigFileManager;
  	HistoManager *fHistoManager;
  	UiManager *fUiManager;

  	int run_number;
	  bool state;
  	std::vector<ModuleManager*>modules;
   	};

#endif 	
