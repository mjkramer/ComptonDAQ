#include <netinet/in.h>
#include <sys/socket.h>
#include <sstream>
#include <map>
#include <string>
#include <memory>

#include <TROOT.h>
#include <TBufferFile.h>
#include <TH1F.h>

#include "Client.hh"
#include "UiProtocol.hh"

using namespace std;

Client::Client()
{
  m_mapClass = gROOT->FindSTLClass("std::map<std::string,std::string>", true);
}

// stupid TBufferFile doesn't have a copy constructor so we're just
// allocating on heap
auto_ptr<TBufferFile> get_response(int sock, UiProtocol::Cmd cmd, int arg)
{
  UiProtocol::Request req;
  req.cmd = cmd;
  req.arg = arg;

  if (send(sock, &req, sizeof(req), 0) != sizeof(req))
    throw Client::SendError;

  int len;
  if (recv(sock, &len, sizeof(len), 0) != sizeof(len))
    throw Client::ReceiveError;

  char *buf = new char[len];
  if (recv(sock, buf, len, 0) != len) {
    delete [] buf;
    throw Client::ReceiveError;
  }

  return auto_ptr<TBufferFile>(new TBufferFile(TBuffer::kRead, len, buf));
}

void Client::Connect()
{
  sockaddr_in sin;
  sin.sin_family = AF_INET;
  sin.sin_addr.s_addr = 0;
  sin.sin_port = htons(m_port);

  m_sock = socket(AF_INET, SOCK_STREAM, 0);

  if (connect(m_sock, (sockaddr*)&sin, sizeof(sin)) != 0)
    throw ConnectError;
}

string Client::Status()
{
  auto_ptr<TBufferFile> buf = get_response(m_sock, UiProtocol::GetInfo, 0);

  map<string, string>& data = *(map<string, string>*) buf->ReadObjectAny(m_mapClass);

  ostringstream ss;

  for (map<string, string>::iterator it = data.begin();
       it != data.end(); ++it)
    ss << it->first << " = " << it->second << endl;

  return ss.str();
}

string Client::ListHists()
{
  return "Hist list goes here";
}

void Client::ShowHist(int histId)
{
  // XXX need support for 2D hists
  auto_ptr<TBufferFile> buf = get_response(m_sock, UiProtocol::GetHist1D, histId);

  TH1F* h = (TH1F*) buf->ReadObjectAny(TH1F::Class());
  h->Draw();
}
