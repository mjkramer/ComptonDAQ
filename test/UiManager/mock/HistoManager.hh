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

  const int maxHisto1D = 4;
  const int maxHisto2D = 1;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class HistoManager{
  
  public:
    HistoManager();
    ~HistoManager();

    void Book();
    void Save();

    void Fill1DHisto(int id1D, int bin, double weight = 1.0);
    void Fill2DHisto(int id2D, int xbin, int ybin);   
    void FillNtuple(float eLs, float eGe, float aBsT, float rBsT);

    TH1F* Get1DHisto(int id1D);
    TH2F* Get2DHisto(int id2D);
  
    
   private:
    TH1F*    histo1D[maxHisto1D];
    TH2F*    histo2D[maxHisto2D];
    float Els, Ege, absT, relT;         
   	};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif //SOURCE_INCLUDE_HISTOMANAGER_H_
