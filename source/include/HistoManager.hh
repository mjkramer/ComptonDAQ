//Christian Dorfer 
//July 21, 2014

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef SOURCE_INCLUDE_HISTOMANAGER_H_
#define SOURCE_INCLUDE_HISTOMANAGER_H_

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

 class TFile;
 class TH1F;
 class TH2F;
 class TTree;
 class DataBlock;

  const int maxHisto1D = 4;
  const int maxHisto2D = 1;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class HistoManager{
  
  public:
    HistoManager();
    ~HistoManager();

    void Book(int run_number);
    void Save();

    void Fill1DHisto(int id1D, int bin, double weight = 1.0);
    void Fill2DHisto(int id2D, int xbin, int ybin);   
    void FillNtuple(float eLs, float eGe, float aBsT, float rBsT);
    void ProcessData(std::vector<DataBlock*> *data);

    TH1F* Get1DHisto(int id1D);
    TH2F* Get2DHisto(int id2D);
  
    
   private:
    TFile*   rootFile;
    TTree*   outTree;  
    TH1F*    histo1D[maxHisto1D];
    TH2F*    histo2D[maxHisto2D];
    float Els, Ege, absT, relT;         
   	};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif //SOURCE_INCLUDE_HISTOMANAGER_H_

