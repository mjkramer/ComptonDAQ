//Christian Dorfer 
//July 21, 2014
#include "Rtypes.h"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef SOURCE_INCLUDE_HISTOMANAGER_H_
#define SOURCE_INCLUDE_HISTOMANAGER_H_

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

 class TFile;
 class TH1F;
 class TH2F;
 class TTree;
 class DataBlock;

  const int maxHisto1D = 1;
  const int maxHisto2D = 0;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class HistoManager{
  
  public:
    HistoManager();
    ~HistoManager();

    void Book(int run_number);
    void Save(int run_number);

    void Fill1DHisto(int id1D, int bin, double weight = 1.0);
    void Fill2DHisto(int id2D, int xbin, int ybin);   
    void FillNTuple(int eGe, std::vector<unsigned int>& wf0, 
		    std::vector<unsigned int>& wf2);
    void ProcessData(std::vector<DataBlock*>& data);

    TH1F* Get1DHisto(int id1D);
    TH2F* Get2DHisto(int id2D);
  
    
   private:
    TFile*   rootFile;
    TTree*   outTree;  
    TH1F*    histo1D[maxHisto1D];
    TH2F*    histo2D[maxHisto2D];
    Int_t ge_adc, n_samples;
    Int_t *waveform_adc0, *waveform_adc2;        
   	};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif //SOURCE_INCLUDE_HISTOMANAGER_H_

