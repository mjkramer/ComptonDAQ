#ifndef CLIENT_HH
#define CLIENT_HH

#include <string>

using namespace std;

class TClass;

class Client {
public:
  Client();

  void SetPort(int port) { m_port = port; }
  void SetHost(const char *host) { m_host = host; }
  void Connect();

  string Info();
  string ListHists();
  void ShowHist(int histId);

  enum CommError {ConnectError, SendError, ReceiveError, NoData };
  
private:
  int m_port, m_sock;
  const char *m_host;
  TClass* m_mapClass, *m_vecClass;
};

#endif
