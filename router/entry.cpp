#include "entry.hpp"
#include <string>

#include <iostream>

using namespace std;



Entry::Entry(string name, string nextHop, int dist)
  : _name(name),
    _nextHop(nextHop),
    _dist(dist),
    _neighbor(false),
    _client(NULL),
    _dataComplete(false),
    _secondsInactive(0)
{
}

Entry::Entry()
  : _name(""),
    _nextHop(""),
    _dist(-1),
    _neighbor(false),
    _client(NULL),
    _dataComplete(false),
    _secondsInactive(0)
    
{
}


Entry::Entry(const Entry *data)
{
    _name       = data->_name;
    _nextHop    = data->_nextHop;
    _dist       = data->_dist;
    _neighbor   = data->_neighbor;
    _client = data->_client;
    
    _dataComplete = data->_dataComplete;
    _secondsInactive = data->_dataComplete;
}

Entry &Entry::operator=(const Entry &e)
{
    _name       = e._name;
    _nextHop    = e._nextHop;
    _dist       = e._dist;
    _neighbor   = e._neighbor;
    _client     = e._client;
    
    _dataComplete = e._dataComplete;
    _secondsInactive = e._dataComplete;
    
    return *this;
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
	return _secondsInactive;
}

int Entry::secondsInactive() const
{
	return _secondsInactive;
}
