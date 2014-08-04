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

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HistoManager::HistoManager():rootFile(0), outTree(0){

  for (int k=0; k<maxHisto1D; k++) histo1D[k] = 0;
  for (int i=0; i<maxHisto2D; i++) histo2D[i] = 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HistoManager::~HistoManager(){ 
   if (rootFile) delete rootFile;
   rootFile = 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistoManager::Book(){ 

 rootFile = new TFile("tobereplaced.root","RECREATE");
 if(!rootFile) {
   cout << "Problems creating ROOT file!" << endl;
   return;
 }

histo1D[0] = new TH1F("1", "Liquid Scintillator Energy Deposit [ADC counts]", 4000, 0., 4000);
histo1D[1] = new TH1F("2", "Germanium Energy Deposit [ADC counts]", 4000, 0., 4000);
histo1D[2] = new TH1F("3", "Total Energy (Ge+PMT) [ADC counts]", 8000, 0., 8000);
histo1D[3] = new TH1F("4", "Relative time difference between events [ns]", 2000, 0., 2000);
   

outTree = new TTree("datatree","");
outTree->Branch("Els", &Els, "Els/F");
outTree->Branch("Ege", &Ege, "Ege/F");
outTree->Branch("absT", &absT, "absT/F");
outTree->Branch("relT", &relT, "relT/F");

cout << "Histogram file is opened!" << endl;
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
    return;
  }
  if (histo1D[id1D]) { return histo1D[id1D]; }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

TH2F* HistoManager::Get2DHisto(int id2D) {
  if (id2D >= maxHisto2D) {
    cout << "Warning from HistoManager::Get2DHisto() : histogram " << id2D
	 << " does not exist!" << endl;
    return;
  }
  if (histo2D[id2D]) { return histo2D[id2D]; }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistoManager::FillNtuple(float eLs, float eGe, float aBsT, float rElT){

Els = eLs;
Ege = eGe;
absT = aBsT;
relT = rElT;

if (outTree) outTree->Fill();
}
