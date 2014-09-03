#include "ConfigFileManager.hh"
using namespace libconfig;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ConfigFileManager::ConfigFileManager(){
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ConfigFileManager::~ConfigFileManager(){
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ConfigFileManager::OpenConfigFile(){

  try{cfg.readFile("config.cfg");}
    catch(const FileIOException &fioex){
      std::cerr << "I/O error while reading file." << std::endl;
      exit (EXIT_FAILURE);
    }

    catch(const ParseException &pex){
      std::cerr << "Parse error at " << pex.getFile() << ":" << pex.getLine()
              << " - " << pex.getError() << std::endl;
      exit (EXIT_FAILURE);
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int ConfigFileManager::GetRunNumber(){

  int run_number = 0;
  try{run_number = cfg.lookup("run_number");}

    catch(const SettingNotFoundException &nfex){
      std::cerr << "No 'run_number' setting in configuration file." << std::endl;
    }

  return run_number;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ConfigFileManager::IncrementRunNumber(){
   Setting &setting = cfg.lookup("run_number");
   setting = GetRunNumber()+1;	
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ConfigFileManager::CloseConfigFile(){
  try{cfg.writeFile(config_file);
    std::cerr << "New configuration successfully written to: " << config_file
         << std::endl;
  }

    catch(const FileIOException &fioex){
      std::cerr << "I/O error while writing file: " << config_file << std::endl;
      exit (EXIT_FAILURE);
  }

}