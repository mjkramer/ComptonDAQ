#ifndef UIPROTOCOL_HH
#define UIPROTOCOL_HH

namespace UiProtocol {
  enum Cmd {
    GetInfo = 0,
    GetHist1D,
    GetHist2D,
    SetState
  };

  struct Request {
    Cmd cmd;
    int arg;
  };
}

#endif
