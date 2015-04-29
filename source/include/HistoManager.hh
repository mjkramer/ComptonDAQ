#ifndef SOURCE_INCLUDE_HISTOMANAGER_H_
#define SOURCE_INCLUDE_HISTOMANAGER_H_

#include "Rtypes.h"

class TFile;
class TH1F;
class TH2F;
class TTree;
class DataBlock;

const int maxHisto1D = 0;
const int maxHisto2D = 0;
const int MAX_ENTRIES_PER_FILE = int(1e6);


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class HistoManager{
  
  public:
    HistoManager();
    ~HistoManager();

    void Book(int run_number);
    void Save();

    void Fill1DHisto(int id1D, int bin, double weight = 1.0);
    void Fill2DHisto(int id2D, int xbin, int ybin);   
    void FillNTuple(int eGe, std::vector<unsigned int>& wf0, 
		    std::vector<unsigned int>& wf2);
    void ProcessData(std::vector<DataBlock*>& data);
    void IntermediateSave();

    TH1F* Get1DHisto(int id1D);
    TH2F* Get2DHisto(int id2D);
  
    
   private:
    TFile*   waveFile;
    TFile*   tqFile;
    TTree*   waveTree;  
    TTree*   tqTree;  
    TH1F*    histo1D[maxHisto1D];
    TH2F*    histo2D[maxHisto2D];
    int      nEvents;
    int      current_run_number;
    int      current_seq_number;
    Int_t ge_adc, n_samples;
    Float_t wf0_ped, wf0_pedRMS, wf0_pre, wf0_sig, wf0_tail, wf0_post;
    Float_t wf2_ped, wf2_pedRMS, wf2_pre, wf2_sig, wf2_tail, wf2_post;
    UShort_t *wf0_adc, *wf2_adc;        
};

#endif

