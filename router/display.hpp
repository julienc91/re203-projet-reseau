#ifndef _DISPLAY_HPP
#define _DISPLAY_HPP
#include <map>


class Display
{
	public:
		void err_unreachable();
		void err_unknown();

		void mess_sent();
		void mess_deliv(double rtt);
		void mess_not_deliv(int deltamax);
		void mess_received(char* mess);

		void ping_echo(char * n1, char * n2, time_t time);
		void ping_result(double success, double failure, double min, double avg, double max);

		void route_init(char * n1, char * n2);
		void route_hop(int num, char * node);
		void route_result(int qt, double rtt);

		void routetable(std::map<void,void>);
};

#endif
