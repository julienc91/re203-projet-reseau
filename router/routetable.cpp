//
#include "routetable.hpp"
#include "exceptions.hpp"

#include <sstream>

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

char* RouteTable::vector()
{
	std::ostringstream s;

	s << "[";
	for(RouteTable::iterator i = begin(); i != end(); i++)
	{
		if (i != begin())
			s << ";";

		s << (*i).first << "," << (*i).second.dist();
	}

	s << "]";
	return (char*) s.str().c_str();
}
