#include <TH1F.h>
#include <TH2F.h>
#include <TFile.h>
#include <TTree.h>
#include <iostream>
#include "HistoManager.hh"
#include "DataBlock.hh"
#include <cmath>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HistoManager::HistoManager():waveFile(0), tqFile(0), waveTree(0), tqTree(0){

  nEvents = 0;
  current_run_number = -1;
  current_seq_number = 0;

  for (int k=0; k<maxHisto1D; k++) histo1D[k] = 0;
  for (int i=0; i<maxHisto2D; i++) histo2D[i] = 0;
  
  wf0_adc = new UShort_t[800];
  wf2_adc = new UShort_t[800];
  ge_adc = 0;
  n_samples = 800; // Initialize at maximum waveform length
  wf0_ped = 0;
  wf0_pedRMS = 0;
  wf0_pre = 0;
  wf0_sig = 0;
  wf0_tail = 0;
  wf0_post = 0;
  wf2_ped = 0;
  wf2_pedRMS = 0;
  wf2_pre = 0;
  wf2_sig = 0;
  wf2_tail = 0;
  wf2_post = 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HistoManager::~HistoManager(){ 
   //clean up
   if(waveFile)
     Save();

   delete [] wf0_adc;
   delete [] wf2_adc;
   wf0_adc = 0;
   wf2_adc = 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistoManager::Book(int run_number){ 

  if(run_number >= 0) current_run_number = run_number;

  const char *wavefilename =
    Form("/data/dayabay/compton_data/wave%06d_%04d.root", 
	 current_run_number, current_seq_number);
  const char *tqfilename =
    Form("/data/dayabay/compton_data/run%06d_%04d.root", 
	 current_run_number, current_seq_number);
  
  //create waveform file
  waveFile = new TFile(wavefilename,"NEW");
  if(!waveFile) {
    std::cout << "Problems creating ROOT file!" << std::endl;
    return;
  }

  //initialize root structures

  //waveform tree
  waveTree = new TTree("wavetree","");
  waveTree->Branch("n_samples", &n_samples, "n_samples/I");
  waveTree->Branch("wf0_adc", wf0_adc, "wf0_adc[n_samples]/s");
  waveTree->Branch("wf2_adc", wf2_adc, "wf2_adc[n_samples]/s");

  std::cout << "Root file " << wavefilename << " is opened!" << std::endl;

  //create waveform file
  tqFile = new TFile(tqfilename,"NEW");
  if(!tqFile) {
    std::cout << "Problems creating ROOT file!" << std::endl;
    return;
  }

  //initialize root structures
  //tree
  tqTree = new TTree("datatree","");
  tqTree->Branch("ge_adc", &ge_adc, "ge_adc/I");
  tqTree->Branch("wf0_ped", &wf0_ped, "wf0_ped/F");
  tqTree->Branch("wf0_pedRMS", &wf0_pedRMS, "wf0_pedRMS/F");
  tqTree->Branch("wf0_pre", &wf0_pre, "wf0_pre/F");
  tqTree->Branch("wf0_sig", &wf0_sig, "wf0_sig/F");
  tqTree->Branch("wf0_tail", &wf0_tail, "wf0_tail/F");
  tqTree->Branch("wf0_post", &wf0_post, "wf0_post/F");
  tqTree->Branch("wf2_ped", &wf2_ped, "wf2_ped/F");
  tqTree->Branch("wf2_pedRMS", &wf2_pedRMS, "wf2_pedRMS/F");
  tqTree->Branch("wf2_pre", &wf2_pre, "wf2_pre/F");
  tqTree->Branch("wf2_sig", &wf2_sig, "wf2_sig/F");
  tqTree->Branch("wf2_tail", &wf2_tail, "wf2_tail/F");
  tqTree->Branch("wf2_post", &wf2_post, "wf2_post/F");

  std::cout << "Root file " << tqfilename << " is opened!" << std::endl;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistoManager::ProcessData(std::vector<DataBlock*>& data){

  int ge_peak = 0;
  std::vector<unsigned int> waveform1;
  std::vector<unsigned int> waveform2;
  
  //get individual data blocks via dynamic cast	
  DataBlock_v1785* p1785_cast = dynamic_cast<DataBlock_v1785*>((data.at(0)));
  DataBlock_v1730* p1730_cast = dynamic_cast<DataBlock_v1730*>((data.at(1)));

  //get data
  if(p1785_cast){
    ge_peak = p1785_cast->GetPeak();
  }
  
  if(p1730_cast){
    waveform1 = p1730_cast->GetWaveform(0);
    waveform2 = p1730_cast->GetWaveform(1);
  }

  //pass obtained information to histogram(s) and tree(s)
  FillNTuple(ge_peak, waveform1, waveform2); //Save the waveforms and the Ge-peak
  nEvents++;
  if((nEvents % MAX_ENTRIES_PER_FILE) == 0){
    // Close current sequence and open next sequence
    this->Save();
    this->Book(-1);
  }
}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistoManager::Save(){ 
  if(waveFile){
    waveFile->Write();   // Writing the histograms to the file
    std::cout << "ROOT file " << waveFile->GetName() << " was written" 
	      << std::endl;
    waveFile->Close();	 // and closing the tree (and the file)
    waveFile = NULL;
  }
  if(tqFile){
    tqFile->Write();   // Writing the histograms to the file
    std::cout << "ROOT file " << tqFile->GetName() << " was written" 
	      << std::endl;
    tqFile->Close();	 // and closing the tree (and the file)
    tqFile = NULL;
  }
  waveTree = 0;
  tqTree = 0;
  current_seq_number++;
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
  int ped_end = 400;
  int pre_end = 500;
  int sig_end = 600;
  int tail_end = 700;
  int post_end = 800;
  Float_t wf0_tmp = 0;
  Float_t wf2_tmp = 0;

  if(n_samples < 800) return;

  //calculate pedestal
  wf0_tmp = 0;
  wf2_tmp = 0;
  for(int i=0; i<ped_end; i++){
    wf0_adc[i] = wf0[i];
    wf2_adc[i] = wf2[i];
    wf0_tmp += wf0[i];
    wf2_tmp += wf2[i];
  }
  wf0_ped = wf0_tmp / ped_end;
  wf2_ped = wf2_tmp / ped_end;

  //calculate pedestal RMS
  wf0_tmp = 0;
  wf2_tmp = 0;
  for(int i=0; i<ped_end; i++){
    wf0_tmp += (wf0[i]-wf0_ped)*(wf0[i]-wf0_ped);
    wf2_tmp += (wf2[i]-wf2_ped)*(wf2[i]-wf2_ped);
  }
  wf0_pedRMS = sqrt(wf0_tmp / ped_end);
  wf2_pedRMS = sqrt(wf2_tmp / ped_end);

  //calculate pre-pulse charge
  wf0_tmp = 0;
  wf2_tmp = 0;
  for(int i=ped_end; i<pre_end; i++){
    wf0_adc[i] = wf0[i];
    wf2_adc[i] = wf2[i];
    wf0_tmp += wf0[i]-wf0_ped;
    wf2_tmp += wf2[i]-wf2_ped;
  }
  wf0_pre = -wf0_tmp;
  wf2_pre = -wf2_tmp;

  //calculate pulse charge
  wf0_tmp = 0;
  wf2_tmp = 0;
  for(int i=pre_end; i<sig_end; i++){
    wf0_adc[i] = wf0[i];
    wf2_adc[i] = wf2[i];
    wf0_tmp += wf0[i]-wf0_ped;
    wf2_tmp += wf2[i]-wf2_ped;
  }
  wf0_sig = -wf0_tmp;
  wf2_sig = -wf2_tmp;

  //calculate pulse tail charge
  wf0_tmp = 0;
  wf2_tmp = 0;
  for(int i=sig_end; i<tail_end; i++){
    wf0_adc[i] = wf0[i];
    wf2_adc[i] = wf2[i];
    wf0_tmp += wf0[i]-wf0_ped;
    wf2_tmp += wf2[i]-wf2_ped;
  }
  wf0_tail = -wf0_tmp;
  wf2_tail = -wf2_tmp;

  //calculate post-pulse charge
  wf0_tmp = 0;
  wf2_tmp = 0;
  for(int i=tail_end; i<post_end; i++){
    wf0_adc[i] = wf0[i];
    wf2_adc[i] = wf2[i];
    wf0_tmp += wf0[i]-wf0_ped;
    wf2_tmp += wf2[i]-wf2_ped;
  }
  wf0_post = -wf0_tmp;
  wf2_post = -wf2_tmp;

  waveTree->Fill();
  tqTree->Fill();

  //std::cout << "  wf: " << wf0_adc[0] << "\t" << wf2_adc[0] << std::endl;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistoManager::IntermediateSave(){
  if (waveTree){
    Long64_t filled_bytes = 0;
    filled_bytes = waveTree->AutoSave("SaveSelf");
    if(!filled_bytes){
      printf("AutoSave not successfull\n");}
  }
  if (tqTree){
    Long64_t filled_bytes = 0;
    filled_bytes = tqTree->AutoSave("SaveSelf");
    if(!filled_bytes){
      printf("AutoSave not successfull\n");}
  }
}
