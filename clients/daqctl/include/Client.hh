#ifndef CLIENT_HH
#define CLIENT_HH

#include <string>

using namespace std;

class TClass;

class Client {
public:
  Client();

  void SetPort(int port) { m_port = port; }
  void Connect();

  string Status();
  string ListHists();
  void ShowHist(int histId);

  enum CommError {ConnectError, SendError, ReceiveError };
  
private:
  int m_port, m_sock;
  TClass* m_mapClass;
};

#endif
