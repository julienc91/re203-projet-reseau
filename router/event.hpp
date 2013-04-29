#ifndef _EVENT_HPP
#define _EVENT_HPP

class Event
{
	public:
		static void input(network *net, char *buffer);
		static void connect(network *net, Client *c, char *buffer);
		static void disconnect(network *net, Client *c);
		static void message(network *net, Client *c, char *buffer);
};
#endif
