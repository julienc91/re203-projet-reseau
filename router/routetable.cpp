//
#include "routetable.hpp"
#include "exceptions.hpp"


RouteTable::RouteTable()
	: std::map<std::string, Entry>()
{

}


Client* RouteTable::nextClient(std::string str)
{
	RouteTable::iterator t;
	Client* c = 0;

	if((t = find(str)) != end())
	{
		c = (*this)[(*t).second.nextHop()].client();
	}
	else
	{
		throw unknownDest;
	}

	return c;
}
