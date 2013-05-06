#include "router.hpp"
#include <iostream>
#include <cstdlib>

int main(int argc, char **argv)
{
	if (argc == 2)
	{
		Router router(argv[1], 0);
	}
	else if (argc == 3)
	{
		Router router(argv[1], argv[2]);
	}
	else
	{
		std::cerr << "Usage: " << argv[0] << " <id> <fichier de conf>" << std::endl;
		return 1;
	}




	return 0;
}
