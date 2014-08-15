//Christian Dorfer 
//July 22, 2014

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include <TH1F.h>
#include <TH2F.h>
#include <TFile.h>
#include <TTree.h>
#include <iostream>
using namespace std;

#include "HistoManager.hh"
#include "DataBlock.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HistoManager::HistoManager():rootFile(0), outTree(0){

  for (int k=0; k<maxHisto1D; k++) histo1D[k] = 0;
  for (int i=0; i<maxHisto2D; i++) histo2D[i] = 0;
  
  waveform_adc0 = new int[1100];
  waveform_adc2 = new int[1100];
  ge_adc = 0;
  n_samples = 0;



}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HistoManager::~HistoManager(){ 

   if (rootFile) delete rootFile;
   rootFile = 0;

   delete [] waveform_adc0;
   delete [] waveform_adc2;
   waveform_adc0 = 0;
   waveform_adc2 = 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistoManager::Book(int run_number){ 

  char buf[40];
  sprintf(buf,"/home/dayabay/compton_data/run%i.root",run_number);
  const char *filename = buf;
 
 rootFile = new TFile(filename,"RECREATE");
 if(!rootFile) {
   cout << "Problems creating ROOT file!" << endl;
   return;
 }


histo1D[0] = new TH1F("2", "Germanium Energy Deposit [ADC counts]", 4000, 0., 4000);

outTree = new TTree("datatree","");
outTree->Branch("ge_adc", &ge_adc, "ge_adc/i");
outTree->Branch("n_sample", &n_samples, "n_samples/i");
outTree->Branch("waveform_adc0", waveform_adc0, "waveform_adc0[n_samples]/i");
outTree->Branch("waveform_adc2", waveform_adc2, "waveform_adc2[n_samples]/i");



cout << "Histogram file is opened!" << endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistoManager::ProcessData(std::vector<DataBlock*> *data){

	int ge_peak = 0;
	std::vector<int> waveform1;
	std::vector<int> waveform2;
	int time_difference;
	
	DataBlock_v1785* p1785_cast = dynamic_cast<DataBlock_v1785*>((data->at(0)));
	DataBlock_v1731* p1731_cast = dynamic_cast<DataBlock_v1731*>((data->at(1)));
	//DataBlock_v1290* p1290_cast = dynamic_cast<DataBlock_v1290*>((data->at(2)));

	if(p1785_cast){
		ge_peak = p1785_cast->GetPeak();
		cout << "Peak: " << ge_peak << endl;
	}

	if(p1731_cast){
		waveform1 = p1731_cast->GetWaveform_Channel0();
		waveform2 = p1731_cast->GetWaveform_Channel2();

n_samples = waveform1.size();
for(int i; i<n_samples; i++){
	cout << waveform1[i] << "  -  " << waveform2[i] << endl;
}

	}

	//if(p1290_cast){
		//time_difference = p1290_cast->GetTimeDifference(1,2);
	//	cout << "TDC array: "  << endl;
	//}

	Fill1DHisto(0, ge_peak); //Fill the Ge-energy to a histogram
	FillNTuple(ge_peak, waveform1, waveform2); //Save the waveforms	
	
}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistoManager::Save(){ 
  if (rootFile) {
    rootFile->Write();       // Writing the histograms to the file
    rootFile->Close();        // and closing the tree (and the file)
    cout << "ROOT file is saved" << endl;
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistoManager::Fill1DHisto(int id1D, int bin, double weight){
  if (id1D >= maxHisto1D) {
	cout << "Warning from Histomanager::Fill1DHisto() : histogram " << id1D << " does not exist!" << endl;
    return;
  }
 if  (histo1D[id1D]) { histo1D[id1D]->Fill(bin, weight); }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistoManager::Fill2DHisto(int id2D, int xbin, int ybin){
  if (id2D >= maxHisto2D) {
cout << "Warning from Histomanager::Fill1DHisto() : histogram " << id2D << " does not exist!" << endl;
    return;
  }
 if  (histo2D[id2D]) { histo2D[id2D]->Fill(xbin, ybin); }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

TH1F* HistoManager::Get1DHisto(int id1D) {
  if (id1D >= maxHisto1D) {
    cout << "Warning from HistoManager::Get1DHisto() : histogram " << id1D
	 << " does not exist!" << endl;
  } else if (histo1D[id1D]) { return histo1D[id1D]; }
  return NULL;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

TH2F* HistoManager::Get2DHisto(int id2D) {
  if (id2D >= maxHisto2D) {
    cout << "Warning from HistoManager::Get2DHisto() : histogram " << id2D
	 << " does not exist!" << endl;
  } else if (histo2D[id2D]) { return histo2D[id2D]; }
  return NULL;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistoManager::FillNTuple(int eGe, std::vector<int> wf0, std::vector<int> wf2){

ge_adc = eGe;
n_samples = wf0.size();

for(int i; i<n_samples; i++){
	waveform_adc0[i] = wf0[i];
	waveform_adc2[i] = wf2[i];
	cout << waveform_adc0[i] << "  -  " << waveform_adc2[i] << endl;
}

if (outTree) outTree->Fill();
}
