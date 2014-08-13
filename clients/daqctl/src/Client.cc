#include <netinet/in.h>
#include <sys/socket.h>
#include <sstream>
#include <map>
#include <string>
#include <memory>

#include <TROOT.h>
#include <TBufferFile.h>
#include <TH1F.h>
#include <TH2F.h>

#include "Client.hh"
#include "UiProtocol.hh"

using namespace std;

Client::Client()
{
  m_mapClass = gROOT->FindSTLClass("std::map<std::string,std::string>", true);
  m_vecClass = gROOT->FindSTLClass("std::vector<std::string>", true);
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

template <typename R>		// R = UiProtocol::Responses::*
class ObjectResponse {
public:
  R response;
  auto_ptr<TBufferFile> buf;
  ObjectResponse(ObjectResponse&& r) :
    response(r.response), buf(r.buf) {}
  ObjectResponse() {}
};

template <typename R>
R get_response(int sock, UiProtocol::Cmd cmd, int arg)
{
  UiProtocol::Request req = {cmd, arg};

  if (send(sock, &req, sizeof(req), 0) != sizeof(req))
    throw Client::SendError;

  R response;
  if (recv(sock, &response, sizeof(R), 0) != sizeof(R))
    throw Client::ReceiveError;

  return response;
}

template <typename R>
ObjectResponse<R> get_object_response(int sock, UiProtocol::Cmd cmd, int arg=0)
{
  ObjectResponse<R> ret;

  ret.response = get_response<R>(sock, cmd, arg);

  unsigned len = ret.response.bufLen;
  if (len == 0)
    throw Client::NoData;

  char *buf = new char[len];

  if (recv(sock, buf, len, 0) != len) {
    delete [] buf;
    throw Client::ReceiveError;
  }

  ret.buf = auto_ptr<TBufferFile>(new TBufferFile(TBuffer::kRead, len, buf));
  return ret;
}

string Client::Info()
{
  auto r = get_object_response<UiProtocol::Responses::Info>
    (m_sock, UiProtocol::GetInfo);

  map<string, string> &data = *(map<string, string>*) r.buf->ReadObjectAny(m_mapClass);

  ostringstream ss;

  for (map<string, string>::iterator it = data.begin();
       it != data.end(); ++it)
    ss << it->first << " = " << it->second << endl;

  return ss.str();
}

string Client::ListHists()
{
  auto r = get_object_response<UiProtocol::Responses::HistList>
    (m_sock, UiProtocol::GetHistList);

  vector<string> &titles = *(vector<string>*) r.buf->ReadObjectAny(m_vecClass);

  ostringstream ss;

  for (int i = 0; i < titles.size(); ++i) {
    const char *dim = i < r.response.n1D ? "1D" : "2D";
    ss << "[" << i << "]    (" << dim << ")      " << titles[i] << endl;
  }

  return ss.str();
}

void Client::ShowHist(int histId)
{
  auto r = get_object_response<UiProtocol::Responses::Hist>
    (m_sock, UiProtocol::GetHist, histId);

  TClass *klass = r.response.dim == 1 ? TH1F::Class() : TH2F::Class();
  TH1 *h = (TH1*) r.buf->ReadObjectAny(klass);

  h->Draw();
}
