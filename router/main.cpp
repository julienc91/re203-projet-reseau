#include "router.hpp"
#include <iostream>
#include <cstdlib>

int main(int argc, char **argv)
{
	if (argc != 4)
	{
		std::cerr << "Usage: " << argv[0] << " <port serveur> <port connexion> <id>" << std::endl;
		return 1;
	}

	Router router(argv[3], atoi(argv[1]),  atoi(argv[2]));


	return 0;
}
