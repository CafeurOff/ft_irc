/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lduthill <lduthill@42perpignan.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 04:34:12 by lduthill          #+#    #+#             */
/*   Updated: 2024/02/20 16:15:04 by lduthill         ###   ########.fr       */
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
	socklen_t addrlen = sizeof(address);
	if ((_new_socket = accept(_socket, (struct sockaddr*)&address, &addrlen)) < 0)
	{
		perror("accept");
		exit(EXIT_FAILURE);
	}

	struct pollfd pstruct[1];

	pstruct[0].fd = _new_socket;
	pstruct[0].events = POLLIN;
	char buffer[1024] = {0};

	while(poll(pstruct, 2, 0) != -1)
	{
		_valread = read(_new_socket, buffer, 1024 - 1);
		std::cout << "BUFFER =" << buffer << std::endl;
	}
}
