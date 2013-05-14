#ifndef _ENTRY_HPP
#define _ENTRY_HPP
#include <string>

extern "C"
{
	#include "../common/client.h"
}

#include <mutex>

class Entry{
public:
  Entry();
  Entry(std::string name, std::string nextHop, int dist = -1);
  Entry(const Entry *data);

  std::string &name();
  std::string name() const;
  std::string &nextHop();
  std::string nextHop() const;
  int &dist();
  int dist() const;
  Client *client();
  void setClient(Client*);

  bool &isNeighbor();
  bool isNeighbor() const;

  bool &isComplete();
  bool isComplete() const;

  int &secondsInactive();
  int secondsInactive() const;

private:
  std::string _name;
  std::string _nextHop;
  int _dist;
  bool _neighbor;
  Client *_client;
  bool _dataComplete;

  int _secondsInactive;
  std::mutex* _secondsMutex;


};

#endif
