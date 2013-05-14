#include <iostream>
#include <ctime>
#include "display.hpp"

/** \brief display the following messge: "error: host unreachable"
 */
void Display::err_unreachable()
{
	std::cout << "\t-> error: host unreachable !" << std::endl;
}

/** \brief display the following messge: "error: unknown destination"
 */
void Display::err_unknown()
{
	std::cout << "\t-> error: unknown destination !" << std::endl;
}

/** \brief display repport for a message sending, with the time
 */
void Display::mess_sent()
{
	std::time_t t =  std::time(NULL);
	std::tm tm = *std::localtime(&t);
	const int bufsize = 100;
	char buf[bufsize];

	if (std::strftime(buf, bufsize, "%c", &tm) != 0)
		std::cout << "\t-> message sent at "<< buf << std::endl;
}

/** \brief display a repport for a well delivered message
 * 	\param rtt
 */
void Display::mess_deliv(double rtt)
{
	std::cout << "\t-> message delivered ! RTT = " << rtt << " ms" << std::endl;
}

/** \brief display an error message for an message not delivered in time
 * 	\param deltamax maximal time allowed for delivering a message
 */
void Display::mess_not_deliv(int deltamax)
{
	std::cout << "\t-> error : message not delivered ! (Delta > " << deltamax << " seconds)";
}


/** \brief display success message for a well received message
 * 	\param mess message received
 */
void Display::mess_received(char* mess)
{
	std::cout << "\t-> message received !" << std::endl;
	std::cout << mess << std::endl;
}

/** \brief display a ping echo test message
 * 	\param n1 source
 * 	\param n2 destination
 * 	\param time  destination answer time
 */
void Display::ping_echo(char* n1, char* n2, int time)
{
	// -> from N1 to N3 time=hh:mm:ss.ms
	std::cout << "\t-> from " << n1 << " to " << n2 << " time=" << time << std::endl;
}

/** \brief display a full ping test message
 * 	\param success percentage of succes
 * 	\param failure	percentage of failure
 * 	\param min minimal answer time
 * 	\param max	maximal answer time
 * 	\param avg	average answer time
 */
void Display::ping_result(double success, double failure, double min, double avg, double max)
{
	//    -> Result: x% success, y% failure
    //    -> RTT   : min=123.45ms avg=123.45 max=123.45
    std::cout << std::endl;
	std::cout << "\t-> Result: " << success << "% success, " << failure << "% failure" << std::endl;
	std::cout << "\t-> RTT   : min=" << min << "ms avg=" << avg << " max=" << max << std::endl;
}


/** \brief display the first message for the route from n1 to n2
 * 	\param n1 source
 * 	\param n2 destination
 */
void Display::route_init(char * n1, char * n2)
{
	std::cout << "\t-> from " << n1 << " to " << n2 << std::endl;
}

/** \brief display a step of of route
 * 	\param num number of the step
 * 	\param node next hop for this step
 */
void Display::route_hop(int num, char * node)
{
	std::cout << "\t\tHop " << num << " = " << node << std::endl;
}

/** \brief display the summary of the route
 * 	\param qt lengh of the rouyte
 * 	\param rtt time spent to reach destination
 */
void Display::route_result(int qt, double rtt)
{
	//Path = 3 hops
	//RTT = 123.ms
	std::cout << std::endl;
	std::cout << "\t\tPath = " << qt << " hops" << std::endl;
	std::cout << "\t\tRTT = " << rtt << "ms" << std::endl;
}

/** \brief display the routetable
 * 	\param rt \class Routetable to display
 */
void Display::routetable(RouteTable& rt)
{
	std::cout << "\tName\t|\tNext\t|\tDist" << std::endl;
	RouteTable::iterator i;
	for(i = rt.begin(); i != rt.end(); i++)
	{
		std::cout << "\t" << (*i).second.name() << "\t|\t" << (*i).second.nextHop() << "\t|\t" << (*i).second.dist() << std::endl;
	}
	std::cout << std::endl;
}

void Display::loginMessage(char* n)
{
	std::cout << "Welcome to the network. You logged in as " << n << std::endl << std::endl;
}
