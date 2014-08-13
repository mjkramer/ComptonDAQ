// XXX make sure to instantiate TApplication in DAQ main() or else we crash 
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <poll.h>
#include <unistd.h>
#include <sstream>

#include <TROOT.h>
#include <TH1F.h>
#include <TH2F.h>

#include "UiManager.hh"
#include "UiProtocol.hh"
#include "HistoManager.hh"

#define MAXTIME_MS 10		// time limit for ProcessIO()

using namespace std;

UiManager::UiManager(HistoManager *histoMgr) :
  m_histoMgr(histoMgr),
  m_buf(TBuffer::kWrite), 
  m_mapClass(gROOT->FindSTLClass("std::map<std::string,std::string>", true)),
  m_vecClass(gROOT->FindSTLClass("std::vector<std::string>", true))
{}

UiManager::~UiManager()
{ 
  for (vector<pollfd>::iterator it = m_pollfds.begin();
       it != m_pollfds.end(); ++it)
    close(it->fd);
}

static void noblock(int fd)
{
  fcntl(fd, F_SETFL, O_NONBLOCK);
}

static int serv_sock(int port)
{
  sockaddr_in sin;
  sin.sin_family = AF_INET;
  sin.sin_addr.s_addr = 0;
  sin.sin_port = htons(port);

  int sock = socket(AF_INET, SOCK_STREAM, 0);
  noblock(sock);
  bind(sock, (sockaddr*)&sin, sizeof(sin));
  listen(sock, 128);
  return sock;
}

static int get_client(int listener)
{
  sockaddr_storage ss;
  socklen_t slen = sizeof(ss);
  int sock = accept(listener, (sockaddr*)&ss, &slen);
  noblock(sock);
  return sock;
}

void UiManager::StartListener(int port)
{
  pollfd pfd;
  pfd.fd = serv_sock(port);
  pfd.events = POLLIN;
  m_pollfds.push_back(pfd);
}

static inline int dt_us(timespec& t1, timespec& t2)
{
  return (t2.tv_sec - t1.tv_sec)*1000000 + (t2.tv_nsec - t1.tv_nsec)/1000;
}

UiManager::DaqCmd UiManager::ProcessIO()
{
  timespec tstart;
  clock_gettime(CLOCK_REALTIME, &tstart);

  poll(&m_pollfds[0], m_pollfds.size(), 0);

  if (m_pollfds[0].revents & POLLIN) { // new connection
    pollfd pfd;
    pfd.fd = get_client(m_pollfds[0].fd);
    pfd.events = POLLIN;
    m_pollfds.push_back(pfd);
  }

  timespec tnow;

  for (vector<pollfd>::iterator it = m_pollfds.begin()+1;
       it != m_pollfds.end(); ++it) {
    clock_gettime(CLOCK_REALTIME, &tnow);
    if (dt_us(tstart, tnow) > 1000*MAXTIME_MS)
      return NoCmd;

    if (it->revents & POLLIN) {
      DaqCmd result = Service(it->fd);
      if (int(result) == -1) {	// client disconnected
	close(it->fd);
	it = m_pollfds.erase(it)-1;
      }
      else if (result != NoCmd)
	return result;
    }
  }

  return NoCmd;
}

// returns -1 if client has disconnected
UiManager::DaqCmd UiManager::Service(int fd)
{
  UiProtocol::Request req;

  ssize_t result = recv(fd, &req, sizeof(UiProtocol::Request), 0);

  if (result == 0)
    return DaqCmd(-1);

  if (result > 0) {
    m_buf.Reset();

    switch (req.cmd) {
    case UiProtocol::GetInfo:
      SendInfo(fd);
      return NoCmd;

    case UiProtocol::GetHist:
      SendHist(fd, req.arg);
      return NoCmd;

    case UiProtocol::GetHistList:
      SendHistList(fd);
      return NoCmd;

    case UiProtocol::SetState:
      if (req.arg)
	return StartDaq;
      else
	return StopDaq;
    }
  }
}

void do_send_response(int fd, void* response, int respSize, TBufferFile& buf)
{
  iovec iov[2];
  iov[0].iov_base = response;
  iov[0].iov_len = respSize;
  iov[1].iov_base = buf.Buffer();
  iov[1].iov_len = buf.Length();

  writev(fd, iov, buf.Length() ? 2 : 1);
}

template <typename R>
void send_response(int fd, R& response, TBufferFile& buf)
{
  response.bufLen = buf.Length();
  do_send_response(fd, &response, sizeof(R), buf);
}

// for responses that don't contain a serialized object
template <typename R>
void send_response(int fd, R& response)
{
  send(fd, &response, sizeof(R), 0);
}

// send number of 1D hists, followed by one vector of titles for 1D,
// then 2D, hists
void UiManager::SendHistList(int fd)
{
  vector<string> hists;

  for (int i = 0; i < maxHisto1D; ++i)
    hists.push_back(m_histoMgr->Get1DHisto(i)->GetTitle());

  for (int i = 0; i < maxHisto2D; ++i)
    hists.push_back(m_histoMgr->Get2DHisto(i)->GetTitle());

  m_buf.WriteObjectAny(&hists, m_vecClass);

  UiProtocol::Responses::HistList response;
  response.n1D = maxHisto1D;
  send_response(fd, response, m_buf);
}

void UiManager::SendHist(int fd, int histId)
{
  TObject* hist = NULL;

  if (0 <= histId && histId < maxHisto1D)
    hist = (TObject*) m_histoMgr->Get1DHisto(histId);
  else if (maxHisto1D <= histId && histId - maxHisto1D < maxHisto2D)
    hist = (TObject*) m_histoMgr->Get2DHisto(histId - maxHisto1D);

  if (hist) m_buf.WriteObject(hist);

  UiProtocol::Responses::Hist response;
  response.dim = histId < maxHisto1D ? 1 : 2;
  send_response(fd, response, m_buf);
}

void UiManager::SendInfo(int fd)
{
  m_buf.WriteObjectAny(&m_data, m_mapClass);

  UiProtocol::Responses::Info response;
  send_response(fd, response, m_buf);
}

// for map-filling magic

template <typename T> string num_to_str(T value)
{
  ostringstream strm;
  strm << value;
  return strm.str();
}

template string num_to_str<int>(int value);
template string num_to_str<double>(double value);
