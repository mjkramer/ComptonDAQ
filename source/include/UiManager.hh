#ifndef UIMANAGER_H
#define UIMANAGER_H

#include <poll.h>
#include <map>
#include <vector>
#include <string>
#include <vector>
#include <TBufferFile.h>

using namespace std;

class HistoManager;
class TClass;

class UiManager {
public:
  UiManager(HistoManager* histoMgr);
  ~UiManager();

  class MapHandle;			// defined below

  MapHandle operator[](const string& key);
  void Remove(const string& key);

  enum DaqCmd { NoCmd=0, StartDaq, StopDaq };

  void StartListener(int port=8520);
  DaqCmd ProcessIO();

private:
  HistoManager *m_histoMgr;
  map<string, string> m_data;
  vector<pollfd> m_pollfds;
  TBufferFile m_buf;
  TClass *m_mapClass;

  friend class MapHandle;

  DaqCmd Service(int fd);
  void SendBuffer(int fd);
  void SendHist(int fd, int dim, int hist);
  void SendInfo(int fd);

public:
  // Utility class for automagic updating
  class MapHandle {
  public:
    MapHandle& operator=(const string& value);
    MapHandle& operator=(const char* value);
    MapHandle& operator=(int value);
    MapHandle& operator=(double value);
    MapHandle& operator=(bool value);

  private:
    string& m_str;		// inside m_data
    MapHandle(string& str) : m_str(str) {}
    friend class UiManager;
  };
};

inline void UiManager::Remove(const string& key)
{ m_data.erase(key); }

typedef UiManager::MapHandle MapHandle;

inline MapHandle UiManager::operator[](const string& key)
{ return MapHandle(m_data[key]); }

inline MapHandle& MapHandle::operator=(const string& value)
{ m_str = value; }

inline MapHandle& MapHandle::operator=(const char* value)
{ m_str = value; }

template <typename T> string num_to_str(T value);

inline MapHandle& MapHandle::operator=(int value)
{ m_str = num_to_str(value); }

inline MapHandle& MapHandle::operator=(double value)
{ m_str = num_to_str(value); }
  
inline MapHandle& MapHandle::operator=(bool value)
{ m_str = value ? "True" : "False"; }

#endif
