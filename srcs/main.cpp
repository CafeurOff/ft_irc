#include "../inc/irc.hpp"


int	main(int ac, char **av)
{
	if (ac != 3)
		return (printf("Error\n please follow this instructions\n ./ircserv <port> <password>.\n"), 1);
	printf("AV : %s\n", av[1]);
}

