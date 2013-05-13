#ifndef ROUTETABLE_H
#define ROUTETABLE_H

#include <map>
#include <string>
#include "entry.hpp"

extern "C"
{
	#include "../common/client.h"
}


class RouteTable: public std::map<std::string, Entry>
{
	public:
		RouteTable();
		Client* nextClient(std::string s);

		char* vector(std::string);
};

#endif /* ROUTETABLE_H */
