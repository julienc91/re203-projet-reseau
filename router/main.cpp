#include "router.hpp"
#include <iostream>
#include <chrono>
#include <thread>

int main(int argc, char **argv)
{
	Router* router;
	if (argc == 2)
	{
		router = new Router(argv[1], 0);
	}
	else if (argc == 3)
	{
		 router = new Router(argv[1], argv[2]);
	}
	else
	{
		std::cerr << "Usage: " << argv[0] << " <id> <fichier de conf>" << std::endl;
		return 1;
	}

	std::chrono::milliseconds half_a_second(500);
	while(router->isRunning())
	{
		std::this_thread::sleep_for(half_a_second);
	}

	delete router;
	return 0;
}
