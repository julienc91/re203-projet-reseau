#include <ctime>
#include <map>

class Display
{
	public:
		static void err_unreachable();
		static void err_unknown();

		static void mess_sent(time_t stamp);
		static void mess_deliv(double rtt);
		static void mess_not_deliv(int deltamax);

		static void ping_echo(char * n1, char * n2, time_t stamp);
		static void ping_result(double success, double failure, double min, double avg, double max);

		static void route_init(char * n1, char * n2);
		static void route_hop(int num, char * node);
		static void route_result(int qt, double rtt, bool sym);

		static void routetable(std::map<void,void>);
};
