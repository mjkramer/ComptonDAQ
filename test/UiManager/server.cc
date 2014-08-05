#include <unistd.h>

#include <TApplication.h>
#include <TH1F.h>
#include <TRandom.h>

#include "UiManager.hh"
#include "HistoManager.hh"

int main(int argc, char** argv)
{
  TApplication app("server", &argc, argv);

  HistoManager histoMgr;
  histoMgr.Book(12345);

  for (int i = 0; i < maxHisto1D; ++i) {
    TH1F* h = histoMgr.Get1DHisto(i);
    float xmin = h->GetXaxis()->GetXmin();
    float xmax = h->GetXaxis()->GetXmax();
    float mean = (xmin+xmax)/2;
    float sigma = (xmax-xmin)/4;

    for (int j = 0; j < 10000; ++j)
      h->Fill(gRandom->Gaus(mean, sigma));
  }

  UiManager uiMgr(&histoMgr);

  uiMgr["RunNumber"] = 12345;
  uiMgr["TrigRate_Hz"] = 987.65;
  uiMgr["Running"] = true;
  uiMgr["Tag"] = "GeCalib";

  int port = argc>1 ? atoi(argv[1]) : 8520;
  uiMgr.StartListener(port);

  while (true) {
    uiMgr.ProcessIO();

    usleep(100000);
  }

  return 0;
}
