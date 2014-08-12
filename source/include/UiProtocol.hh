#ifndef UIPROTOCOL_HH
#define UIPROTOCOL_HH

namespace UiProtocol {
  enum Cmd {
    GetInfo = 0,
    GetHist,
    GetHistList,
    SetState
  };

  struct Request {
    Cmd cmd;
    int arg;
  };

  // Each response consists of one of the structs below, possibly
  // followed by a buffer containing an object serialized by TBuffer

  namespace Responses {
    struct Info {
      int bufLen;
      // buffer containing map<string, string>
    };

    struct Hist {
      int dim;			// 1D or 2D
      int bufLen;
      // buffer containing TH1F or TH2F
    };

    struct HistList {
      int n1D; // number of the listed hists that are 1D (listed first)
      int bufLen;
      // buffer containing vector<string> of hist titles
    };
  }
}

#endif
