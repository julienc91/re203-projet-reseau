#include "entry.hpp"
#include <string>


using namespace std;



Entry::Entry(string name, string nextHop, int dist)
  : _name(name),
    _nextHop(nextHop),
    _dist(dist),
    _neighbor(false),
    _dataComplete(false),
    _secondsInactive(0),
    _secondsMutex(new std::mutex())

{
}

Entry::Entry()
  : _neighbor(false),
  _dataComplete(false),
  _secondsInactive(0),
  _secondsMutex(new std::mutex())
{
}


Entry::Entry(const Entry *data)
{
    _name       = data->_name;
    _nextHop    = data->_nextHop;
    _dist       = data->_dist;
    _neighbor   = data->_neighbor;
}

string &Entry::name(){
  return _name;
}

string Entry::name() const{
  return _name;
}

string &Entry::nextHop(){
  return _nextHop;
}
string Entry::nextHop() const{
  return _nextHop;
}

int &Entry::dist(){
  return _dist;
}

int Entry::dist() const{
  return _dist;
}

bool &Entry::isNeighbor(){
  return _neighbor;
}

bool Entry::isNeighbor() const{
  return _neighbor;
}

Client *Entry::client(){
  return _client;
}

void Entry::setClient(Client* c)
{
	_client = c;
}

bool & Entry::isComplete()
{
	return _dataComplete;
}
bool  Entry::isComplete() const
{
	return _dataComplete;
}

int &Entry::secondsInactive()
{
	_secondsMutex->lock();
	return _secondsInactive;
	_secondsMutex->unlock();
}

int Entry::secondsInactive() const
{
	_secondsMutex->lock();
	return _secondsInactive;
	_secondsMutex->unlock();
}
