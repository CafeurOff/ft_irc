/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lduthill <lduthill@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 04:34:12 by lduthill          #+#    #+#             */
/*   Updated: 2024/02/22 17:28:46 by lduthill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Server.hpp"

Server::Server(char **av)
{
	_socket = socket(AF_INET, SOCK_STREAM, 0);
	_opt = 1;
	_port = atoi(av[1]);
	_password = av[2];
}

Server::~Server()
{
	close(_socket);
}

void	Server::Launch()
{
	struct pollfd pstruct[MAX_CLIENT + 1];

	pstruct[0].fd = _socket;
	pstruct[0].events = POLLIN | POLLPRI;
	int	use_client = 0;

	while (1)
	{
		int rc = poll(pstruct, use_client + 1, 5000);
		if (rc > 0)
		{
			if (pstruct[0].revents & POLLIN)
			{
                struct sockaddr_in cliaddr;
				socklen_t addrlen = sizeof(cliaddr);
				_new_socket = accept(_socket, (struct sockaddr *)&cliaddr, &addrlen);
				for (int i = 1; i < MAX_CLIENT; i++)
				{
					if (pstruct[i].fd == 0)
					{
						pstruct[i].fd = _new_socket;
						pstruct[i].events = POLLIN | POLLPRI;
						use_client++;
						break;
					}
				}
			}
		}
		for (int i = 1; i < MAX_CLIENT; i++)
		{
			if (pstruct[i].fd > 0 && pstruct[i].revents & POLLIN)
			{
				char buffer[1024] = { 0 };
				int	buff_len = read(pstruct[i].fd, buffer, 1024 - 1);
				if (buff_len == -1)
				{
					pstruct[i].fd = 0;
					pstruct[i].events = 0;
					pstruct[i].revents = 0;
					use_client--;
				}
				else if (buff_len == 0)
				{
					pstruct[i].fd = 0;
					pstruct[i].events = 0;
					pstruct[i].revents = 0;
					use_client--;
				}
				else
				{
					std::cout << "FD CLIENT = " << pstruct[i].fd << std::endl;
					std::cout << "Client =" << buffer << std::endl;
				}
			}
		}
	}
}

void	Server::init()
{
	struct sockaddr_in address;

	if (_socket == 0)
	{
		perror("socket failed");
		exit(EXIT_FAILURE);
	}
	if (setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &_opt, sizeof(_opt)))
	{
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(_port);
	if (bind(_socket, (struct sockaddr *)&address, sizeof(address)) < 0)
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
	std::cout << "Server is running on port " << _port << std::endl;
	if (listen(_socket, 3) < 0)
	{
		perror("listen");
		exit(EXIT_FAILURE);
	}
	Launch();
}
