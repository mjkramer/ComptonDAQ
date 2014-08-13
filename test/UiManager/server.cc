#include <unistd.h>

#include <TApplication.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TRandom.h>

#include "HistoManager.hh"
#include "UiManager.hh"

void fill1D(TH1F* h)
{
  float xmin = h->GetXaxis()->GetXmin();
  float xmax = h->GetXaxis()->GetXmax();
  float mean = (xmin+xmax)/2;
  float sigma = (xmax-xmin)/4;

  for (int j = 0; j < 10000; ++j)
    h->Fill(gRandom->Gaus(mean, sigma));
}

void fill2D(TH2F* h)
{
  float xmin = h->GetXaxis()->GetXmin();
  float xmax = h->GetXaxis()->GetXmax();
  float xmean = (xmin+xmax)/2;
  float xsigma = (xmax-xmin)/4;

  float ymin = h->GetYaxis()->GetXmin();
  float ymax = h->GetYaxis()->GetXmax();
  float ymean = (ymin+ymax)/2;
  float ysigma = (ymax-ymin)/4;

  for (int j = 0; j < 10000; ++j)
    h->Fill(gRandom->Gaus(xmean, xsigma), gRandom->Gaus(ymean, ysigma));
}

int main(int argc, char** argv)
{
  TApplication app("server", &argc, argv);

  HistoManager histoMgr;
  histoMgr.Book();

  for (int i = 0; i < maxHisto1D; ++i)
    fill1D(histoMgr.Get1DHisto(i));

  for (int i = 0; i < maxHisto2D; ++i)
    fill2D(histoMgr.Get2DHisto(i));

  UiManager uiMgr(&histoMgr);

  uiMgr["RunNumber"] = 12345;
  uiMgr["TrigRate_Hz"] = 987.65;
  uiMgr["Running"] = true;
  uiMgr["Tag"] = "GeCalib";

  int port = argc>1 ? atoi(argv[1]) : 8520;
  uiMgr.StartListener(port);

  while (true) {
    uiMgr.ProcessIO();

    usleep(20000);
  }

  return 0;
}
