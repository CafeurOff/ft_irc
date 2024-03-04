#include "../../inc/Server.hpp"

Server::Server(char **av)
{
	_socket = socket(AF_INET, SOCK_STREAM, 0);
	_opt = 1;
	_port = atoi(av[1]);
	_password = av[2];

    commandFunctions["PASS"] = &Server::ft_verif_pass;
    commandFunctions["NICK"] = &Server::ft_nick_receive;
    commandFunctions["USER"] = &Server::ft_user_receive;
    commandFunctions["QUIT"] = &Server::ft_quit_user;
    commandFunctions["JOIN"] = &Server::ft_join_receive;
    commandFunctions["MODE"] = &Server::ft_mode_receive;
    commandFunctions["TOPIC"] = &Server::ft_topic_receive;
    commandFunctions["INVITE"] = &Server::ft_invite_receive;
}

Server::~Server()
{
	close(_socket);
	close(_new_socket);
}

void	Server::Launch()
{
	struct pollfd pstruct[MAX_CLIENT + 1] = {};
	pstruct[0].fd = _socket;
	pstruct[0].events = POLLIN | POLLPRI;
	int	nb_client = 0;

	while (1)
	{
		int rc = poll(pstruct, nb_client + 1, 5000);
		if (rc > 0)
		{
			if (pstruct[0].revents & POLLIN)
			{
                struct sockaddr_in cliaddr;
				socklen_t addrlen = sizeof(cliaddr);
				_new_socket = accept(_socket, (struct sockaddr *)&cliaddr, &addrlen);
				for (int i = 1; i <= MAX_CLIENT; i++)
				{
					if (pstruct[i].fd == 0)
					{
						pstruct[i].fd = _new_socket;
						pstruct[i].events = POLLIN | POLLPRI;
						nb_client++;
						break;
					}
				}
			}
		}
		for (int i = 1; i <= MAX_CLIENT; i++)
		{
			if (pstruct[i].fd > 0 && pstruct[i].revents & POLLIN)
			{
				char buffer[1024] = { 0 };
				_valread = read(pstruct[i].fd, buffer, 1024 - 1);
				if (_valread == -1 || _valread == 0)
				{
					pstruct[i].fd = 0;
					pstruct[i].events = 0;
					pstruct[i].revents = 0;
					nb_client--;
				}
				else
					ft_parse_buffer(buffer, pstruct[i].fd);
			}
		}
	}
}

void	Server::init()
{
	struct sockaddr_in address;

	if (_socket == 0)
	{
		std::cerr << "socket failed" << std::endl;
		exit(EXIT_FAILURE);
	}
	if (setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &_opt, sizeof(_opt)))
	{
		std::cerr << "setsockopt failed" << std::endl;
		exit(EXIT_FAILURE);
	}
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(_port);
	if (bind(_socket, (struct sockaddr *)&address, sizeof(address)) < 0)
	{
		std::cerr << "bind failed" << std::endl;
		exit(EXIT_FAILURE);
	}
	std::cout << "Server is running on port " << _port << std::endl;
	if (listen(_socket, 3) < 0)
	{
		std::cerr << "listen failed" << std::endl;
		exit(EXIT_FAILURE);
	}
	Launch();
}

std::string		Server::ft_getServerName()
{
	char hostname[1024];
	hostname[1023] = '\0';
	gethostbyname(hostname);
	_servername = hostname;
	return (_servername);
}

int Server::findFdByNickname(const std::string& nickname)
{
	std::map<int, Client>::iterator it;
	for (it = _client.begin(); it != _client.end(); ++it)
	{
		if (it->second.getNickname() == nickname)
			return (it->second.getFd());
	}
	return (-1);
}

int Server::findFd(int fd)
{
	std::map<int, Client>::iterator it;
	for (it = _client.begin(); it != _client.end(); ++it)
	{
		if (it->first == fd)
			return (1);
	}
	return (-1);
}

Client *Server::findClient(int fd)
{
	std::map<int, Client>::iterator it;
	for (it = _client.begin(); it != _client.end(); ++it)
	{
		if (it->first == fd)
			return (&it->second);
	}
	return (NULL);
}