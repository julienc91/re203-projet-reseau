#include <iostream>
#include <ctime>
#include "display.hpp"


void Display::err_unreachable()
{
	std::cout << "\t-> error: host unreachable !" << std::endl;
}

void Display::err_unknown()
{
	std::cout << "\t-> error: unknown destination !" << std::endl;
}

void Display::mess_sent()
{
	std::time_t t =  std::time(NULL);
	std::tm tm = *std::localtime(&t);
	const int bufsize = 100;
	char buf[bufsize];

	if (std::strftime(buf, bufsize, "%c", &tm) != 0)
		std::cout << "\t-> message sent at "<< buf << std::endl;
}

void Display::mess_deliv(double rtt)
{
	std::cout << "\t-> message delivered ! RTT = " << rtt << " ms" << std::endl;
}

void Display::mess_not_deliv(int deltamax)
{
	std::cout << "\t-> error : message not delivered ! (Delta > "<< deltamax <<" seconds)";
}
