#include <TH1F.h>
#include <TH2F.h>
#include <TFile.h>
#include <TTree.h>
#include <iostream>
#include "HistoManager.hh"
#include "DataBlock.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HistoManager::HistoManager():rootFile(0), outTree(0){

  for (int k=0; k<maxHisto1D; k++) histo1D[k] = 0;
  for (int i=0; i<maxHisto2D; i++) histo2D[i] = 0;
  
  wf0_adc = new Int_t[1000];
  wf2_adc = new Int_t[1000];
  ge_adc = 0;
  n_samples = 0;
  wf0_sig = 0;
  wf2_sig = 0;
  wf0_ped = 0;
  wf2_ped = 0;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HistoManager::~HistoManager(){ 
   //clean up
   if(rootFile)
     Save();

   delete [] wf0_adc;
   delete [] wf2_adc;
   wf0_adc = 0;
   wf2_adc = 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistoManager::Book(int run_number){ 

  const char *filename =
    Form("/home/dayabay/compton_data/run%i.root", run_number);
  
  //create root file
  rootFile = new TFile(filename,"NEW");
  if(!rootFile) {
    std::cout << "Problems creating ROOT file!" << std::endl;
    return;
  }

  //initialize root structures

  //histograms
  histo1D[0] = new TH1F("2", "Germanium Energy Deposit [ADC counts]", 4000, 0., 4000);
  
  //tree
  outTree = new TTree("datatree","");
  outTree->Branch("ge_adc", &ge_adc, "ge_adc/I");
  outTree->Branch("n_sample", &n_samples, "n_samples/I");
  outTree->Branch("wf0_adc", wf0_adc, "wf0_adc[n_samples]/I");
  outTree->Branch("wf2_adc", wf2_adc, "wf2_adc[n_samples]/I");
  outTree->Branch("wf0_sig", &wf0_sig, "wf0_sig/F");
  outTree->Branch("wf2_sig", &wf2_sig, "wf2_sig/F");
  outTree->Branch("wf0_ped", &wf0_ped, "wf0_ped/F");
  outTree->Branch("wf2_ped", &wf2_ped, "wf2_ped/F");

  TTree::SetMaxTreeSize(MAX_TREE_SIZE);
  
  std::cout << "Root file " << "run" << run_number << ".root is opened!" << std::endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistoManager::ProcessData(std::vector<DataBlock*>& data){

	int ge_peak = 0;
	std::vector<unsigned int> waveform1;
	std::vector<unsigned int> waveform2;

  //get individual data blocks via dynamic cast	
	DataBlock_v1785* p1785_cast = dynamic_cast<DataBlock_v1785*>((data.at(0)));
	DataBlock_v1731* p1731_cast = dynamic_cast<DataBlock_v1731*>((data.at(1)));

  //get data
	if(p1785_cast){
		ge_peak = p1785_cast->GetPeak();
	}

	if(p1731_cast){
	  waveform1 = p1731_cast->GetWaveform(0);
	  waveform2 = p1731_cast->GetWaveform(1);
	}

  //pass obtained information to histogram(s) and tree(s)
	Fill1DHisto(0, ge_peak); //Fill the Ge-energy to a histogram
	FillNTuple(ge_peak, waveform1, waveform2); //Save the waveforms and the Ge-peak
	
}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistoManager::Save(){ 
  if(rootFile){
    rootFile = outTree->GetCurrentFile();
    rootFile->Write();   // Writing the histograms to the file
    std::cout << "ROOT file " << rootFile->GetName() << " was written" << std::endl;
    rootFile->Close();	 // and closing the tree (and the file)
    rootFile = NULL;
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistoManager::Fill1DHisto(int id1D, int bin, double weight){
  if (id1D >= maxHisto1D){
	  std::cout << "Warning from Histomanager::Fill1DHisto() : histogram " << id1D << " does not exist!" << std::endl;
    return;
  }
  
  if(histo1D[id1D]){ 
    histo1D[id1D]->Fill(bin, weight); 
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistoManager::Fill2DHisto(int id2D, int xbin, int ybin){
  if (id2D >= maxHisto2D) {
    std::cout << "Warning from Histomanager::Fill1DHisto() : histogram " << id2D << " does not exist!" << std::endl;
    return;
  }

 if(histo2D[id2D]){ 
   histo2D[id2D]->Fill(xbin, ybin); 
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

TH1F* HistoManager::Get1DHisto(int id1D) {
  if (id1D >= maxHisto1D) {
    std::cout << "Warning from HistoManager::Get1DHisto() : histogram " << id1D
	 << " does not exist!" << std::endl;
  } 

  else if(histo1D[id1D]){return histo1D[id1D];}

  return NULL;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

TH2F* HistoManager::Get2DHisto(int id2D) {
  if (id2D >= maxHisto2D) {
    std::cout << "Warning from HistoManager::Get2DHisto() : histogram " << id2D
	 << " does not exist!" << std::endl;
  } 

  else if(histo2D[id2D]){return histo2D[id2D];}

  return NULL;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistoManager::FillNTuple(int eGe, std::vector<unsigned int>& wf0, std::vector<unsigned int>& wf2){
  ge_adc = eGe;
  n_samples = wf0.size(); //800
  int ped_len = 300;
  Float_t wf0_ped_temp = 0;
  Float_t wf2_ped_temp = 0;
  Float_t wf0_sig_temp = 0;
  Float_t wf2_sig_temp = 0;

  if(n_samples < 800) return;

  //calculate pedestal and fill waveforms
  for(int i=0; i<ped_len; i++){
    wf0_adc[i] = wf0[i];
    wf2_adc[i] = wf2[i];
    wf0_ped_temp += wf0[i];
    wf2_ped_temp += wf2[i];
  }

  //calculate peak minus pedestal and fill waveforms
  for(int i=ped_len; i<n_samples; i++){
    wf0_adc[i] = wf0[i];
    wf2_adc[i] = wf2[i];
    wf0_sig_temp += wf0[i];
    wf2_sig_temp += wf2[i];
  }

  wf0_ped = wf0_ped_temp / ped_len;
  wf2_ped = wf2_ped_temp / ped_len;

  wf0_sig = (wf0_ped*(n_samples-ped_len)) - wf0_sig_temp;
  wf2_sig = (wf2_ped*(n_samples-ped_len)) - wf2_sig_temp;

 // std::cout << wf2_sig << " " << wf2_ped << " " << wf0_sig << " " << wf2_ped << std::endl;

  if (outTree) outTree->Fill();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistoManager::IntermediateSave(){
  if (outTree){
    Long64_t filled_bytes = 0;
    filled_bytes = outTree->AutoSave("SaveSelf");
    if(!filled_bytes){
      printf("AutoSave not successfull\n");}
  }
}
