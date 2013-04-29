#ifndef _NET_ACTIONS_HPP
#define _NET_ACTIONS_HPP
class Router;
class NetActions
{
	public:
		void message(Message* m);
		void ping(Message* m);

	private:
		Router* router;
};
#endif
