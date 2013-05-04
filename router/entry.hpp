#ifndef _ENTRY_HPP
#define _ENTRY_HPP
#include <string>

extern "C"
{
	#include "../common/client.h"
}

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

private:
  std::string _name;
  std::string _nextHop;
  int _dist;
  Client *_client;

};

#endif
