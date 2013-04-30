#ifndef _ROUTER_HPP
#define _ROUTER_HPP

#include <cstdio>
#include <cstdlib>
#include <cstring>

extern "C" {
#include "../common/net.h"
#include "../common/messages.h"
#include "../common/prompt.h"
#include "../common/config.h"
}
#include "event.hpp"
#include "exec.hpp"
#include "rout_table.hpp"
#include "prompt.hpp"


class Routeur {
	public:
		Routeur(char* name, int srcport, int destport);
		Routeur(const Routeur * data);
		~Routeur();

		void mainLoop();

		int newSeqnum();
		char* getName();
		Configuration* getConfiguration();


	private:
		std::string *_name;
		RoutTable * _tab;

		network* net;
		Client* c; //euh ? est-ce nécessaire ? c'est pas juste pour des tests ce client ? (jm)

		Configuration* config;
		int seqnum;
		Prompt prompt;
		Exec exec;
};
#endif
