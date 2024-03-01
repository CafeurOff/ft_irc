#include "../inc/irc.hpp"
#include "../inc/Server.hpp"

int	main(int ac, char **av)
{
	std::string args;

	if (ac != 3)
		return (printf("Error\n please follow this instructions\n ./ircserv <port> <password>.\n"), 1);
	args = av[1];
	if (args.length() != 4)
		return (printf("Error\n the port must be valid\n"), 1);
	for (int i = 0; args[i]; i++)
		if (!isdigit(args[i]))
			return (printf("Error\n port must be a integer\n"), 1);
	ft_launch_server(av);
	return (0);
}

void	ft_launch_server(char **av)
{
	Server server(av);
	server.init();
	while (1)
		continue;
}
