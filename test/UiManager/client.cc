#include <netinet/in.h>
#include <sys/socket.h>
#include <fcntl.h>

#include <map>
#include <string>
#include <iostream>

#include <TApplication.h>
#include <TH1F.h>
#include <TCanvas.h>
#include <TBufferFile.h>
#include <TROOT.h>

#include "UiProtocol.hh"

using namespace std;

TBufferFile* get_response(int sock, UiProtocol::Cmd cmd, int arg)
{
  UiProtocol::Request req = {cmd, arg};
  
  send(sock, &req, sizeof(req), 0);

  int len;
  recv(sock, &len, sizeof(len), 0);

  char* buf = new char[len];
  recv(sock, buf, len, 0);

  return new TBufferFile(TBuffer::kRead, len, buf);
}

int main(int argc, char** argv)
{
  TApplication app("client", &argc, argv);

  int port = argc>1 ? atoi(argv[1]) : 8520;

  sockaddr_in sin;
  sin.sin_family = AF_INET;
  sin.sin_addr.s_addr = 0;
  sin.sin_port = htons(port);

  int sock = socket(AF_INET, SOCK_STREAM, 0);
  connect(sock, (sockaddr*)&sin, sizeof(sin));

  TBufferFile* buf = get_response(sock, UiProtocol::GetInfo, 0);
  TClass* mapClass = gROOT->FindSTLClass("std::map<std::string,std::string>", true);
  map<string, string>& data = *(map<string, string>*) buf->ReadObjectAny(mapClass);

  for (map<string, string>::iterator it = data.begin();
       it != data.end(); ++it)
    cout << it->first << " = " << it->second << endl;

  buf = get_response(sock, UiProtocol::GetHist1D, 0);
  TH1F* h = (TH1F*) buf->ReadObjectAny(TH1F::Class());
  h->Draw();

  app.Run();
  return 0;
}
