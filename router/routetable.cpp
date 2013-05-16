//
#include "routetable.hpp"
#include "exceptions.hpp"

#include <sstream>
#include <iostream>
#include <string.h>

extern "C" {
	#include "../common/util.h"
}
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

char* RouteTable::vector(std::string client)
{
	std::ostringstream s;
	int added = 0;

	s << "[";
	for(RouteTable::iterator i = begin(); i != end(); i++)
	{
		if (client.compare((*i).first) != 0 && (*i).second.dist()>=0 )//&& (*i).second.isComplete())
		{
			if (added)
				s << ";";

			s << (*i).first << "," << (*i).second.dist();
			++added;
		}
	}

	s << "]";

	char* tmp = strcopy(s.str().c_str());
	return tmp;
}

void RouteTable::debug()
{
    std::cerr << "[ROUTETABLE] " << size() << " element(s)" << std::endl;
    int n = 0; char s[256];
    for(RouteTable::iterator i = begin(); i != end(); i++)
	{
        Client *c = (*i).second.client();
        if (c != NULL && c->id != NULL)
            strcpy(s, c->id);
        else
            strcpy(s, "NULL");
        std::cerr << "[ROUTETABLE(" << n++ << ")] Key=<" << (*i).first << "> ClientId=<" << s << ">" << std::endl;
	}
}
